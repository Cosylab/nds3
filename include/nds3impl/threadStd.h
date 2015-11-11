/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSTHREADSTD_H
#define NDSTHREADSTD_H

#include "threadBaseImpl.h"
#include "../nds3/definitions.h"
#include <thread>

namespace nds
{

class ThreadStd: public ThreadBaseImpl
{
public:
    ThreadStd(FactoryBaseImpl* pFactory, const std::string& name, threadFunction_t function);

    virtual void join();

protected:
    std::thread m_thread;
};

}
#endif // NDSTHREADSTD_H
