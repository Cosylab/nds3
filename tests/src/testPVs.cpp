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
        std::string readValue;
        timespec readTimestamp;
        pInterface->readCSValue("/rootNode-Channel1.testVariableIn", &readTimestamp, &readValue);
        EXPECT_EQ("Initial value", readValue);

    }

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

TEST(testPVs, testSubscription0)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    nds::parameters_t parameters;
    parameters.push_back("rootNode-Channel1-testVariableIn");
    nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("subscribe", "rootNode-Channel1-testVariableOut", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    for(std::int32_t count(0); count != 10; ++count)
    {
        std::ostringstream value;
        value << "Test string " << count;

        timespec timestamp;
        timestamp.tv_sec = count;
        timestamp.tv_nsec = count + 10;
        pInterface->writeCSValue("/rootNode-Channel1.writeTestVariableIn", timestamp, value.str());

        std::string readValue;
        timespec readTimestamp;
        pInterface->readCSValue("/rootNode-Channel1.readTestVariableOut", &readTimestamp, &readValue);
        EXPECT_EQ(value.str(), readValue);
        EXPECT_EQ(count, readTimestamp.tv_sec);
        EXPECT_EQ(count + 10, readTimestamp.tv_nsec);
    }

    factory.destroyDevice("rootNode");
}

TEST(testPVs, testSubscription1)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    {
        nds::parameters_t parameters;
        parameters.push_back("rootNode-Channel1-testVariableIn");
        nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("subscribe", "rootNode-Channel1-testVariableOut", parameters);
    }

    {
        nds::parameters_t parameters;
        parameters.push_back("0");
        nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("decimation", "rootNode-Channel1-testVariableIn", parameters);
    }

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    for(std::int32_t count(0); count != 10; ++count)
    {
        std::ostringstream value;
        value << "Test string " << count;

        timespec timestamp;
        timestamp.tv_sec = count;
        timestamp.tv_nsec = count + 10;
        pInterface->writeCSValue("/rootNode-Channel1.pushTestVariableIn", timestamp, value.str());

        {
            const std::string* readValue;
            const timespec* readTimestamp;
            EXPECT_THROW(pInterface->getPushedString("/rootNode-Channel1.pushTestVariableIn", readTimestamp, readValue), std::runtime_error);
        }

        std::string readValue;
        timespec readTimestamp;
        pInterface->readCSValue("/rootNode-Channel1.readTestVariableOut", &readTimestamp, &readValue);
        EXPECT_EQ(value.str(), readValue);
        EXPECT_EQ(count, readTimestamp.tv_sec);
        EXPECT_EQ(count + 10, readTimestamp.tv_nsec);
    }

    factory.destroyDevice("rootNode");
}

TEST(testPVs, testReplication)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    nds::parameters_t parameters;
    parameters.push_back("rootNode-Channel1-testVariableIn");
    nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("replicate", "rootNode-Channel1-delegateIn", parameters);

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");

    for(std::int32_t count(0); count != 10; ++count)
    {
        std::ostringstream value;
        value << "Test string " << count;

        timespec timestamp;
        timestamp.tv_sec = count;
        timestamp.tv_nsec = count + 10;
        pInterface->writeCSValue("/rootNode-Channel1.pushTestVariableIn", timestamp, value.str());

        const std::string* pReadValue;
        const timespec* pReadTimestamp;
        pInterface->getPushedString("/rootNode-Channel1.delegateIn", pReadTimestamp, pReadValue);
        EXPECT_EQ(value.str(), *pReadValue);
        EXPECT_EQ(count, pReadTimestamp->tv_sec);
        EXPECT_EQ(count + 10, pReadTimestamp->tv_nsec);
    }

    factory.destroyDevice("rootNode");
}

