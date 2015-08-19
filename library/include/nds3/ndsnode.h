#ifndef NDSNODE_H
#define NDSNODE_H

#include "ndsbase.h"

namespace nds
{

class NodeImpl;

class Node: public Base
{
protected:
    Node();

public:
#ifndef SWIG
    Node(std::shared_ptr<NodeImpl> portImpl);
#endif

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

protected:
#ifndef SWIG
    void addChildInternal(Base& child);
#endif

};

}
#endif // NDSNODE_H
