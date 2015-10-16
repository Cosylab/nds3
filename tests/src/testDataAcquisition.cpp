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

    ::sleep(1);

    factory.destroyDevice("rootNode");

    //EXPECT_EQ((std::int32_t)nds::state_t::off, pInterface->getPushedInt32());
}


