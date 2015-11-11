#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/testDevice.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testPVs, testDelegate)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    timespec timestamp;
    timestamp.tv_sec = 2;
    timestamp.tv_nsec = 12;
    pInterface->writeCSValue("/rootNode-Channel1.delegateOut", timestamp, std::string("this is a test"));

    std::string readValue;
    timespec readTimestamp;
    pInterface->readCSValue("/rootNode-Channel1.delegateIn", &readTimestamp, &readValue);
    EXPECT_EQ("this is a test", readValue);
    EXPECT_EQ(2, readTimestamp.tv_sec);
    EXPECT_EQ(12, readTimestamp.tv_nsec);

    factory.destroyDevice("rootNode");

}

TEST(testPVs, testVariable)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    {
        timespec timestamp;
        timestamp.tv_sec = 2;
        timestamp.tv_nsec = 12;
        pInterface->writeCSValue("/rootNode-Channel1.writeTestVariableIn", timestamp, std::string("testing input variable"));

        std::string readValue;
        timespec readTimestamp;
        pInterface->readCSValue("/rootNode-Channel1.testVariableIn", &readTimestamp, &readValue);
        EXPECT_EQ("testing input variable", readValue);
        EXPECT_EQ(2, readTimestamp.tv_sec);
        EXPECT_EQ(12, readTimestamp.tv_nsec);
    }

    {
        timespec timestamp;
        timestamp.tv_sec = 4;
        timestamp.tv_nsec = 14;
        pInterface->writeCSValue("/rootNode-Channel1.testVariableOut", timestamp, std::string("testing output variable"));

        std::string readValue;
        timespec readTimestamp;
        pInterface->readCSValue("/rootNode-Channel1.readTestVariableOut", &readTimestamp, &readValue);
        EXPECT_EQ("testing output variable", readValue);
        EXPECT_EQ(4, readTimestamp.tv_sec);
        EXPECT_EQ(14, readTimestamp.tv_nsec);
    }

    factory.destroyDevice("rootNode");

}

