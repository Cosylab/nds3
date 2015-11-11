/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSNODE_H
#define NDSNODE_H

/**
 * @file node.h
 *
 * @brief Defines the nds::Node class, a class that can contain other device's nodes, ports
 *         or PVs.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */


#include "definitions.h"
#include "base.h"

namespace nds
{

class NodeImpl;
class PVBase;
class StateMachine;

/**
 * @brief A node object that can contain other nodes or PVs as children.
 *
 */
class NDS3_API Node: public Base
{

public:
#ifndef SWIG
    Node(std::shared_ptr<NodeImpl> portImpl);
#endif

    Node();

    /**
     * @brief Construct the node.
     *
     * @param name     node's name (e.g. "CHANNEL0" or "CHANNELGROUP1")
     * @param nodeType specifies the node's role. This information is used to
     *                 calculate the external node name when the @ref naming_rules
     *                 naming rules are used.
     */
    Node(const std::string& name, const nodeType_t nodeType = nodeType_t::generic);

    /**
     * @brief Add a child node or record to this node. This node will
     *        take ownership of the added one and will take care of deleting it.
     *
     * @param child the child node to add
     * @return a reference to the added node
     */
    template<class T>
    T addChild(T child)
    {
        addChildInternal(child);
        return child;
    }

    /**
     * @brief Registers all the records with the control system. Call this from the root node
     *        which will take care of traversing its children and initialize them.
     *
     * @param pDeviceObject   pointer to the device object
     * @param factory         control system with which the node must be registered
     */
    void initialize(void* pDeviceObject, Factory& factory);

    Node addNode(Node& node);     // Specialized for SWIG

    PVBase addPV(PVBase& pvBase); // Specialized for SWIG

    StateMachine addStateMachine(StateMachine& stateMachine); // Specialized for SWIG

protected:
#ifndef SWIG
    void addChildInternal(Base& child);
#endif

};

}
#endif // NDSNODE_H
