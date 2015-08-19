#include "ndsnodeimpl.h"
#include <memory>

namespace nds
{

NodeImpl::NodeImpl(const std::string &name): BaseImpl(name)
{}


void NodeImpl::addChild(std::shared_ptr<BaseImpl> pChild)
{
    std::string name(pChild->getComponentName());

    if(m_children.find(name) != m_children.end())
    {
        throw;
    }
    pChild->setParent(std::static_pointer_cast<NodeImpl>(shared_from_this()) );
    m_children[name] = pChild;
}

void NodeImpl::initialize()
{
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->initialize();
    }
}


}
