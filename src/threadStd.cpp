/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <thread>

#ifdef _WIN32
#include <pthread.h>
#endif

#include "nds3/impl/threadStd.h"

namespace nds
{

ThreadStd::ThreadStd(FactoryBaseImpl* pFactory, const std::string &name, threadFunction_t function):
    ThreadBaseImpl(pFactory, name), m_thread(function)
{
#ifdef _WIN32
	pthread_t thread;
	thread.p = m_thread.native_handle();
	thread.x = 0;
	pthread_setname_np(thread, name.c_str());
#else
	pthread_setname_np(m_thread.native_handle(), name.c_str());
#endif

}

void ThreadStd::join()
{
    m_thread.join();
}

}
