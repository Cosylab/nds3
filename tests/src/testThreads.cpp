#include <gtest/gtest.h>
#include <nds3/nds.h>


void runInThreadFunction(std::int32_t* pCounter)
{
    for(int count(0); count != 5; ++count)
    {
        ::sleep(1);
        *pCounter = count;
    }

}

TEST(testThreads, testThreads)
{
    nds::Port rootNode("rootNode");

    nds::Factory factory("test");
    rootNode.initialize(0, factory);

    std::int32_t counter(10);
    nds::Thread runThread = rootNode.runInThread(std::bind(&runInThreadFunction, &counter));
    EXPECT_EQ(10, counter);
    runThread.join();
    EXPECT_EQ(4, counter);
}



