#ifndef NDSPORT_H
#define NDSPORT_H

#include "ndsnode.h"

namespace nds
{

class PVBase;
class PortImpl;


class Port: public Node
{
protected:
    Port() = delete;

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
