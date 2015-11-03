#include "../include/nds3/thread.h"
#include "../include/nds3impl/threadBaseImpl.h"

namespace nds
{

Thread::Thread()
{
}

Thread::Thread(const Thread &right): m_pImplementation(right.m_pImplementation)
{
}

Thread& Thread::operator =(const Thread& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

Thread::Thread(std::shared_ptr<ThreadBaseImpl> pImpl): m_pImplementation(pImpl)
{

}

void Thread::join()
{
    m_pImplementation->join();
}

}
