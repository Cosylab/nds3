#ifndef NDSPORT_H
#define NDSPORT_H

#include "ndsnode.h"

namespace nds
{

class PVBase;


class Port: public Node
{
protected:
    Port() = delete;

public:
    /**
     * @brief Construct an AsynPort.
     *
     * @param port's name. The port name is linked to the parents' names to form
     *        the identifier used to register the AsynPort.
     */
    Port(const std::string& name);

    /**
     * @brief Register a PV on the AsynPort. This method is called automatically by
     *        the PVs during the initialization.
     *
     * @param pv   reference to the PV to be registered
     */
    void registerPV(PVBase& pv);

};

}

#endif // NDSPORT_H
