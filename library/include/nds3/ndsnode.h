#ifndef NDSNODE_H
#define NDSNODE_H

#include "ndsbase.h"

namespace nds
{

class Node: public Base
{
protected:
    Node();

public:
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
    T& addChild(T* pChild)
    {
        addChildInternal(pChild);
        return *pChild;
    }

protected:
    void addChildInternal(Base* pChild);

};

}
#endif // NDSNODE_H
