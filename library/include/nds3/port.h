#ifndef NDSPORT_H
#define NDSPORT_H

#include "definitions.h"
#include "node.h"

namespace nds
{

class PVBase;
class PortImpl;

/**
 * @brief Represents a node with direct connection with the controlled device.
 */
class NDS3_API Port: public Node
{
protected:
    Port();

public:
#ifndef SWIG
    Port(std::shared_ptr<PortImpl> portImpl);
#endif

    /**
     * @brief Construct an AsynPort.
     *
     * @param port's name. The port name is linked to the parents' names to form
     *        the identifier used to register the AsynPort.
     */
    Port(const std::string& name);

};

}

#endif // NDSPORT_H
