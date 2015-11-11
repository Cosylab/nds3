/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSFACTORYBASEIMPL_H
#define NDSFACTORYBASEIMPL_H

#include "../nds3/definitions.h"
#include <map>
#include <mutex>
#include <memory>
#include <thread>

namespace nds
{

class InterfaceBaseImpl;
class BaseImpl;
class NodeImpl;
class PVBaseImpl;
class LogStreamGetterImpl;
class ThreadBaseImpl;
class IniFileParserImpl;

/**
 * @brief This is the base class for objects that interact with specific control systems
 *        and has to be allocated only once (singletons).
 *
 * In order to create support for a control system you need to overwrite this class and
 *  InterfaceBaseImpl.
 *
 * The FactoryBaseImpl derived object will be created when NDS starts and will stay alive until NDS
 *  is shutdown.
 *
 * The overwritten classes must implement the following methods:
 * - getName()
 * - getNewInterface()
 * - run()
 * - getLogStreamGetter()
 * - registerCommand()
 * - deregisterCommand()
 *
 * Optionally, the method createThread() can also be overwritten.
 */

class NDS3_API FactoryBaseImpl: public std::enable_shared_from_this<FactoryBaseImpl>
{
    friend class InterfaceBaseImpl;
public:
    FactoryBaseImpl();

    virtual ~FactoryBaseImpl();

    /**
     * @brief This function is called before the destructor is invoked.
     *
     * It takes care of de-initializing all the PVs while the control system is
     *  still up and running.
     */
    virtual void preDelete();

    virtual ThreadBaseImpl* runInThread(const std::string& name, threadFunction_t function);

    static void loadDriver(const std::string& libraryName);

    /**
     * @brief Allocate a device. The device driver must have been registered via
     *        registerDriver() or must have been loaded from the folder defined
     *        in the enviroment variables LD_LIBRARY_PATH or NDS_DEVICES
     *
     * @param driverName
     * @param deviceName
     * @param parameters
     * @return an opaque pointer that represents the allocated device. Pass it to
     *         destroyDevice() to remove the device
     */
    void* createDevice(const std::string& driverName, const std::string& deviceName, const namedParameters_t& parameters);

    /**
     * @brief Deallocate an allocated device
     * @param pDevice
     */
    void destroyDevice(void* pDevice);

    void destroyDevice(const std::string& deviceName);

    void holdNode(void* pDeviceObject, std::shared_ptr<NodeImpl> pHoldNode);

    /**
     * @brief Returns the separator to use between the node names.
     *
     * Considers the naming rules set via setNamingRules(). If the naming rules don't
     *  define a separator then passes the request to getDefaultSeparator().
     *
     * @param nodeLevel the node level for which the separator is required. The level 0
     *                   is the separator inserted righ before the root node name.
     * @return the separator for the specified level. Can be empty if no separator is
     *         required.
     *
     */
    const std::string& getSeparator(const std::uint32_t nodeLevel) const;

    void loadNamingRules(std::istream& rules);
    void setNamingRules(const std::string& rulesName);

    std::string getRootNodeName(const std::string& name) const;
    std::string getGenericChannelName(const std::string& name) const;
    std::string getInputChannelName(const std::string& name) const;
    std::string getOutputChannelName(const std::string& name) const;
    std::string getSourceChannelName(const std::string& name) const;
    std::string getSinkChannelName(const std::string& name) const;
    std::string getInputPVName(const std::string& name) const;
    std::string getOutputPVName(const std::string& name) const;
    std::string getStateMachineNodeName(const std::string& name) const;
    std::string getStateMachineSetStateName(const std::string& name) const;
    std::string getStateMachineGetStateName(const std::string& name) const;
    std::string getStateMachineGetGlobalStateName(const std::string& name) const;
    std::string getDecimationPVName(const std::string& name) const;

    std::string buildNameFromRule(const std::string& name,
                                  const std::string& rule0,
                                  const std::string& rule1 = "",
                                  const std::string& rule2 = "",
                                  const std::string& rule3 = "",
                                  const std::string& rule4 = ""
                                  ) const;

    virtual const std::string& getDefaultSeparator(const std::uint32_t nodeLevel) const = 0;

    virtual const std::string getName() const = 0;


    /**
     * @brief This method is called by a PortNodeImpl object in order to create a
     *         named connection with the control system.
     *
     * @param fullName the full name of the PortNodeImpl
     * @return a pointer to an InterfaceBaseImpl object. The lifetime of the InterfaceBaseImpl
     *         object is controlled by the PortNodeImpl object.
     */
    virtual InterfaceBaseImpl* getNewInterface(const std::string& fullName) = 0;

    virtual void run(int argc,char *argv[]) = 0;

    virtual LogStreamGetterImpl* getLogStreamGetter() = 0;

    /**
     * @brief Called to register a command tied to a specific node.
     *
     * The NDS framework guarantees that there are no multi-threading concurrency
     *  issues when calling this function.
     *
     * @param node            the node to which the command has to be tied
     * @param command         the command
     * @param usage           command's description
     * @param numParameters   the number of accepted parameters
     * @param commandFunction the delegate function to call to execute the command
     */
    virtual void registerCommand(const BaseImpl& node,
                                 const std::string& command,
                                 const std::string& usage,
                                 const size_t numParameters, command_t commandFunction) = 0;

    /**
     * @brief Called to deregister a node from all the NDS commands
     *
     * The NDS framework guarantees that there are no multi-threading concurrency
     *  issues when calling this function.
     *
     * @param node          to node from which the command has to be removed
     */
    virtual void deregisterCommand(const BaseImpl& node) = 0;

private:
    struct allocatedDevice_t
    {
        void* m_pDevice;
        deallocateDriver_t m_deallocationFunction;
    };

    typedef std::map<std::string, allocatedDevice_t> allocatedDevices_t;
    allocatedDevices_t m_allocatedDevices;

    typedef std::list<std::shared_ptr<NodeImpl> > nodesList_t;
    typedef std::map<void*, nodesList_t> heldNodes_t;
    heldNodes_t m_heldNodes;

    std::mutex m_mutex;

    std::unique_ptr<IniFileParserImpl> m_namingRules;
    std::string m_namingRulesName;

};

}


#endif // NDSFACTORYBASEIMPL_H
