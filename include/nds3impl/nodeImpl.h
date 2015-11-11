/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSNODEIMPL_H
#define NDSNODEIMPL_H

#include "baseImpl.h"
#include "../nds3/definitions.h"
#include <list>

namespace nds
{

class Node;
class StateMachineImpl;

/**
 * @brief Represents a node (channel or channelGroup in the old NDS) which can contain
 *        other nodes.
 *
 * Can contain other nodes (including AsynPort) or PVs.
 */
class NodeImpl: public BaseImpl
{
public:
    /**
     * @brief Construct the node
     *
     * @param name  node's name (e.g. "CHANNEL0" or "CHANNELGROUP1")
     */
    NodeImpl(const std::string& name, const nodeType_t nodeType);

    void addChild(std::shared_ptr<BaseImpl> pChild);

    void initializeRootNode(void* pDeviceObject, FactoryBaseImpl& controlSystem);

    void deinitializeRootNode();

    virtual void initialize(FactoryBaseImpl& controlSystem);

    virtual void deinitialize();

    virtual state_t getLocalState() const;

    virtual void getGlobalState(timespec* pTimestamp, state_t* pState) const;
    void getChildrenState(timespec* pTimestamp, state_t* pState) const;

    virtual void setLogLevel(const logLevel_t logLevel);

    virtual std::string buildFullExternalName(const FactoryBaseImpl& controlSystem) const;
    virtual std::string buildFullExternalNameFromPort(const FactoryBaseImpl& controlSystem) const;

protected:
    std::string buildFullExternalName(const FactoryBaseImpl& controlSystem, const bool bStopAtPort) const;

    nodeType_t m_nodeType;

private:
    typedef std::map<std::string, std::shared_ptr<BaseImpl> > tChildren;
    tChildren m_children;

    std::shared_ptr<StateMachineImpl> m_pStateMachine;

};

}

#endif // NDSNODEIMPL_H
