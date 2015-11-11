/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSTHREAD_H
#define NDSTHREAD_H

/**
 * @file thread.h
 *
 * @brief Defines the nds::Thread class used to execute tasks on parallel threads.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "definitions.h"
#include <string>
#include <memory>
namespace nds
{

class ThreadBaseImpl;

/**
 * @brief Represents a thread.
 *
 * In NDS3 you can use the standard library threads (std::thread), but some
 *  control systems allow you to exercise a greater control on the individual threads
 *  if you create them throught the specific control system facilities (e.g. EPICS).
 *
 * To take full advantage of the thread features provided by some control system it is
 *  suggested that you launch the thread by using this class.
 *
 * Thread can be created by using the control system factory method Factory::runInThread()
 *  or by calling Base::runInThread() on any node or PV.
 */
class NDS3_API Thread
{
public:

    /**
     * @brief Constructor. Create an empty thread placeholder.
     */
    Thread();

    Thread(const Thread& right);

    Thread& operator=(const Thread& right);

#ifndef SWIG
    Thread(std::shared_ptr<ThreadBaseImpl> pImpl);
#endif

    void join();

protected:
    std::shared_ptr<ThreadBaseImpl> m_pImplementation;

};

}
#endif // THREAD_H
