#ifndef NDSBASEIMPL_H
#define NDSBASEIMPL_H

#include <string>
#include <map>
#include <memory>
#include <vector>


namespace nds
{

class Port;
class Base;
class Node;
class PortImpl;

/**
 * @internal
 * @brief This is the base class for nodes, devices, PVs.
 *
 */
class BaseImpl
{
    friend class NodeImpl;

protected:
    BaseImpl(const std::string& name, Base* pInterface):
        m_pInterfaceObject(pInterface), m_name(name), m_pParent(0)
    {}

public:
    BaseImpl(const BaseImpl& right) = delete;

    virtual ~BaseImpl(){}


    /**
     * @brief Get the Node that holds an ASYN port. Query the parent nodes if necessary.
     *
     * @return a reference to the parent AsynNode, which can be used to communicate directly with
     *         the AsynDriver
     */
    virtual Port& getPort();


    /**
     * @brief Return the node's name.
     *
     * @return the node's name.
     */
    std::string getComponentName() const;


    /**
     * @brief Return the full node's name, prepending the parents' names if necessary
     *        (e.g. "ROOT-CHANNEL1-THISNODE")
     *
     * @return the fully qualified node's name
     */
    std::string getFullName() const;

    void setParent(Node* pParent);


    /**
     * @brief Return the node's name as seen by the parent AsynPort.
     *
     * For instance, for a record "TIME" contained in the channel "CH0" that is part of
     * an AsynPort named "DEVICE" the return value will be "CH0-TIME": node that the "DEVICE"
     * part is missing.
     *
     * @return the node's name relative to the AsynPort that contains it.
     */
    virtual std::string getFullNameFromPort() const;

    /**
     * @brief Registers all the records with the AsynDriver. Call this from the root node
     *        which will take care of traversing its children and initialize them.
     */
    virtual void initialize() = 0;

protected:
    Base* m_pInterfaceObject;

    std::string m_name;
    Node* m_pParent;

};


}










#endif // NDSBASEIMPL_H
