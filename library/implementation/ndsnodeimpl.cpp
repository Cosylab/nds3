#include "ndsnodeimpl.h"
#include "../include/nds3/ndsbase.h"
#include "../include/nds3/ndsnode.h"

namespace nds
{

NodeImpl::NodeImpl(const std::string &name, Node *pInterface): BaseImpl(name, (Base*)pInterface)
{}

NodeImpl::~NodeImpl()
{
    for(tChildren::iterator scanChildren(m_children.begin()), end(m_children.end()); scanChildren != end; ++scanChildren)
    {
        delete scanChildren->second;
    }
}

void NodeImpl::addChild(Base* pChild)
{
    std::unique_ptr<Base> pChildPtr(pChild);

    std::string name(pChildPtr->getComponentName());

    if(m_children.find(name) != m_children.end())
    {
        throw;
    }
    pChildPtr->setParent(static_cast<Node*>(m_pInterfaceObject));
    m_children.insert(std::make_pair(name, pChildPtr.release()));
}

void NodeImpl::initialize()
{
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->initialize();
    }
}


}
