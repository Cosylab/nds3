#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "testDevice.h"
#include "ndsTestInterface.h"
#include "ndsTestFactory.h"

TEST(testTime, testDelegate)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    // Set the channel1 time
    ////////////////////////
    std::int32_t startTimestamp = 400;
    timespec timestamp = {0, 0};
    pInterface->writeCSValue("/rootNode-Channel1.setCurrentTime", timestamp, startTimestamp);

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    pInterface->writeCSValue("/rootNode-Channel1.data.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);

    // Wait for the switch on state (it should take one second)
    ///////////////////////////////////////////////////////////
    ::sleep(2);
    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    EXPECT_EQ(400, pStateMachineSwitchTime->tv_sec);
    EXPECT_EQ(410, pStateMachineSwitchTime->tv_nsec);

    factory.destroyDevice("rootNode");

}
