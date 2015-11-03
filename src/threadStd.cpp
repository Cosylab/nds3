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
