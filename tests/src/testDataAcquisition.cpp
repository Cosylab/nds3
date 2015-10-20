#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/testDevice.h"
#include "../include/ndsTestInterface.h"

TEST(testDataAcquisition, testPushData)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    TestDevice* pDevice = TestDevice::getInstance("rootNode");

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    pInterface->getPushedInt32("rootNode-Channel1-data-StateMachine-getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    timespec timestamp = {0, 0};
    pInterface->writeCSValue("rootNode-Channel1-data-StateMachine-setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("rootNode-Channel1-data-StateMachine-getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);

    // Wait for the switch on state (it should take one second)
    ///////////////////////////////////////////////////////////
    ::sleep(2);
    pInterface->getPushedInt32("rootNode-Channel1-data-StateMachine-getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    // Start the data acquisition
    /////////////////////////////
    pInterface->writeCSValue("rootNode-Channel1-numAcquisitions", timestamp, (std::int32_t)100);
    pInterface->writeCSValue("rootNode-Channel1-data-StateMachine-setState", timestamp, (std::int32_t)nds::state_t::running);
    ::sleep(1);
    pInterface->getPushedInt32("rootNode-Channel1-data-StateMachine-getState", pStateMachineSwitchTime, pStateMachineState); // Retrieve the starting state
    pInterface->getPushedInt32("rootNode-Channel1-data-StateMachine-getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(1);

    // Initialize comparison vectors
    std::vector<std::int32_t> pushData0(10000);
    std::vector<std::int32_t> pushData1(10000);
    for(size_t initializeVectors(0); initializeVectors != 10000; ++initializeVectors)
    {
        pushData0[initializeVectors] = initializeVectors;
        pushData1[initializeVectors] = 10000 - initializeVectors;
    }

    const std::vector<std::int32_t>* pRetrievedPushedValues;
    timespec firstTime;
    timespec lastTime;
    const timespec* pTime;
    for(size_t numAcquisitions(0); numAcquisitions != 100; ++numAcquisitions)
    {
        pInterface->getPushedVectorInt32("rootNode-Channel1-data-Data", pTime, pRetrievedPushedValues);
        lastTime = *pTime;

        if(numAcquisitions == 0)
        {
            firstTime = *pTime;
        }
        if((numAcquisitions & 1) == 0)
        {
            EXPECT_EQ(pushData0, *pRetrievedPushedValues);
        }
        else
        {
            EXPECT_EQ(pushData1, *pRetrievedPushedValues);
        }
    }

    std::int64_t nanosecondsStart = firstTime.tv_sec * 1000000000 + firstTime.tv_nsec;
    std::int64_t nanosecondsEnd = lastTime.tv_sec * 1000000000 + lastTime.tv_nsec;
    std::cout << "Nanoseconds spent pushing 100 values (10000 samples each) " << (nanosecondsEnd - nanosecondsStart) << std::endl;

    pInterface->writeCSValue("rootNode-Channel1-data-StateMachine-setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("rootNode-Channel1-data-StateMachine-getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);

    factory.destroyDevice("rootNode");

}


