/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3impl/threadStd.h"
#include <thread>

namespace nds
{

ThreadStd::ThreadStd(FactoryBaseImpl* pFactory, const std::string &name, threadFunction_t function):
    ThreadBaseImpl(pFactory, name), m_thread(function)
{
    pthread_setname_np(m_thread.native_handle(), name.c_str());
}

void ThreadStd::join()
{
    m_thread.join();
}

}
