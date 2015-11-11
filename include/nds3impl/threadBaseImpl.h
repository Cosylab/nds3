/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSTHREADBASEIMPL_H
#define NDSTHREADBASEIMPL_H

#include "../nds3/definitions.h"
#include <string>
#include <memory>

namespace nds
{

class FactoryBaseImpl;

class NDS3_API ThreadBaseImpl: public std::enable_shared_from_this<ThreadBaseImpl>
{
public:
    ThreadBaseImpl(FactoryBaseImpl* pFactory, const std::string& name);
    virtual ~ThreadBaseImpl();

    std::string getName() const;

    virtual void join() = 0;

private:
    FactoryBaseImpl* m_pFactory;
    std::string m_name;

};

}
#endif // NDSTHREADBASEIMPL_H
