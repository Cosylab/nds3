#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "include/testDevice.h"

TEST(testDeviceAllocation, testAllocationMissingDevice)
{
    nds::Factory factory("test");
    EXPECT_THROW(factory.createDevice("testDevi", "rootNode", nds::namedParameters_t()), nds::DriverNotFound);
    EXPECT_EQ((void*)0, TestDevice::getInstance("rootNode"));
}

TEST(testDeviceAllocation, testDoubleAllocation)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    EXPECT_NE((void*)0, TestDevice::getInstance("rootNode"));

    EXPECT_THROW(factory.createDevice("testDevice", "rootNode", nds::namedParameters_t()), nds::DeviceAlreadyCreated);

    factory.destroyDevice("rootNode");

    EXPECT_EQ((void*)0, TestDevice::getInstance("rootNode"));

    EXPECT_THROW(factory.destroyDevice("rootNode"), nds::DeviceNotAllocated);
}

TEST(testDeviceAllocation, testInitDeinit)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    factory.subscribe("rootNode-Channel1-variableIn0", "rootNode-Channel1-numAcquisitions");

    factory.destroyDevice("rootNode");

    // If the PVs have been removed the they have been de-registered correctly
    EXPECT_THROW(factory.subscribe("rootNode-Channel1-variableIn0", "rootNode-Channel1-numAcquisitions"), nds::MissingOutputPV);
}
