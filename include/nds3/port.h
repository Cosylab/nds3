/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPORT_H
#define NDSPORT_H

/**
 * @file port.h
 *
 * @brief Defines the nds::Port, an nds::Node derived class that can communicate with
 *        the control system.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

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
     * @brief Construct a Port.
     *
     * @param name port's name. The port name is linked to the parents' names to form
     *             the identifier used to register the AsynPort.
     * @param nodeType specifies the node's role. This information is used to
     *                 calculate the external node name when the @ref naming_rules
     *                 naming rules are used.
     */
    Port(const std::string& name, const nodeType_t nodeType = nodeType_t::generic);

};

}

#endif // NDSPORT_H
