#ifndef NDSNODE_H
#define NDSNODE_H

#include "definitions.h"
#include "ndsbase.h"

namespace nds
{

class NodeImpl;
class PVBase;
class StateMachine;

/**
 * @brief A node object that can contain other nodes as children.
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
     * @param name  node's name (e.g. "CHANNEL0" or "CHANNELGROUP1")
     */
    Node(const std::string& name);

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
