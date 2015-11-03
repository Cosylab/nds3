#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "include/testDevice.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testPVs, testDelegate)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    TestDevice* pDevice = TestDevice::getInstance("rootNode");

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    timespec timestamp;
    timestamp.tv_sec = 2;
    timestamp.tv_nsec = 12;
    pInterface->writeCSValue("rootNode-Channel1-delegateOut", timestamp, std::string("this is a test"));

    std::string readValue;
    timespec readTimestamp;
    pInterface->readCSValue("rootNode-Channel1-delegateIn", &readTimestamp, &readValue);
    EXPECT_EQ("this is a test", readValue);
    EXPECT_EQ(2, readTimestamp.tv_sec);
    EXPECT_EQ(12, readTimestamp.tv_nsec);

    factory.destroyDevice("rootNode");

}


