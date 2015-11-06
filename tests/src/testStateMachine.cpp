#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <functional>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"
#include <unistd.h>

void wait1sec()
{
    ::sleep(1);
}

void rollback()
{
    ::usleep(100000);
    throw nds::StateMachineRollBack("rolling back");
}


bool returnTrue(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}

TEST(testStateMachine, testLocalGlobalState)
{
    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachine0 = rootNode.addChild(nds::StateMachine(true,
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Node ch0 = rootNode.addChild(nds::Node("ch0"));
    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));

    nds::StateMachine stateMachine1 = ch0.addChild(nds::StateMachine(true,
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));


    nds::StateMachine stateMachine2 = ch1.addChild(nds::StateMachine(true,
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&rollback),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Check the local and global state: should be 0
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    // Switch on state machine 1
    stateMachine1.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::initializing, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    // Switch state machine 2 to on and then to running.
    // Should go back to on because of the rollback
    stateMachine2.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::initializing, (int)stateMachine2.getLocalState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());

    stateMachine2.setState(nds::state_t::running);
    EXPECT_EQ((int)nds::state_t::starting, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());
    EXPECT_EQ((int)nds::state_t::starting, (int)stateMachine2.getLocalState());
    ::sleep(1);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());

    stateMachine2.setState(nds::state_t::off);
    EXPECT_EQ((int)nds::state_t::switchingOff, (int)stateMachine2.getLocalState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine2.getLocalState());

    factory.destroyDevice("");
}

