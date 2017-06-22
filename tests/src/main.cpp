// imebra_tests.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "testDevice.h"
#include "ndsTestFactory.h"

int main(int argc, char **argv)
{
    nds::Factory::registerDriver("testDevice",
                           std::bind(&TestDevice::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&TestDevice::deallocateDevice, std::placeholders::_1));

    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(new nds::tests::TestControlSystemFactoryImpl()));
    nds::Factory::registerControlSystem(testControlSystem);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
