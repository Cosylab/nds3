/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2016 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

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
