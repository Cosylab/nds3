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
