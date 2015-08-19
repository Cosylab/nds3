#ifndef NDSPORTIMPL_H
#define NDSPORTIMPL_H

#include "ndsnodeimpl.h"

namespace nds
{

class InterfaceImpl;
class PVBaseImpl;

/**
 * @brief Node that holds an ASYN port
 *
 * When initialize() is called then registers the port and all the children AsynPVBase
 * (also the ones not directly owned but owned by its children).
 */
class PortImpl: public NodeImpl
{
public:
    /**
     * @brief Construct an AsynPort.
     *
     * @param port's name. Used also as port name in the AsynDriver
     */
    PortImpl(const std::string& name);

    virtual ~PortImpl();


    /**
     * @brief Return a pointer to this object.
     * @return a pointer to this object
     */
    virtual std::shared_ptr<PortImpl> getPort();

    virtual std::string getFullNameFromPort() const;

    virtual void initialize();

    void registerPV(std::shared_ptr<PVBaseImpl> pv);

private:
    std::unique_ptr<InterfaceImpl> m_pAsynInterface;

    typedef std::map<int, std::shared_ptr<PVBaseImpl> > tRecords;
    tRecords m_records;
};

}
#endif // NDSPORTIMPL_H
