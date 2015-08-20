#include <iostream>

#include "library/implementation/ndsfactoryimpl.h"
#include "iocsh.h"

#include "library/include/nds3/nds3.h"

//using namespace std;


/**
 * @brief Sadly this class made it into this file.
 *
 * Used as example of a business logic class which implements the reads and writes and one
 * day may react to changes in a state machine class.
 *
 */
class Delegate
{
public:
    void readErrorCode(timespec*, std::int32_t* pCode)
    {
        *pCode = 10;
    }
    void writeErrorCode(const timespec&, const std::int32_t&)
    {
        return;
    }

    void readData(epicsInt32* pData)
    {
        *pData = 2;
    }

    void writeData(epicsInt32 data)
    {
        // nothing for now
    }
};


int main()
{
    nds::FactoryImpl::getInstance().registrationCommand("myIoc");

    Delegate myLogicIsHere;

    // We create a device. We could use directly a port here, but for fun this device will have
    //  more than one Asyn port
    ///////////////////////////////////////////////////////////////////////////////////////////
    nds::Node theDevice("MightyDevice");

    // We add the first port, that will be registered as "MightyDevice-MyPort"
    // addChild takes ownership of the added object and returns a reference
    //  to it.
    //////////////////////////////////////////////////////////////////////////
    nds::Port port = theDevice.addChild(nds::Port("MyPort"));

    // Now we add a PV to the port, which will delegates read & write to 2 methods
    //  in our business logic class.
    // This PV will be registered as "ErrorCode" in the port "MightyDevice-MyPort"
    //////////////////////////////////////////////////////////////////////////////
    port.addChild(nds::PVDelegate<std::int32_t>("ErrorCode",
                                  std::bind(&Delegate::readErrorCode, &myLogicIsHere, std::placeholders::_1, std::placeholders::_2),
                                  std::bind(&Delegate::writeErrorCode, &myLogicIsHere, std::placeholders::_1, std::placeholders::_2)
                                  ));

    port.addChild(nds::PVHoldDelegate("Delegate", new nds::Delegate()));

    // All the structure has been setup. Register the ports and the PVs
    ///////////////////////////////////////////////////////////////////
    theDevice.initialize();

    iocshCmd("dbLoadDatabase /home/codac-dev/Documents/m-nds-test/target/main/epics/dbd/ndsTest.dbd");
    iocshCmd("myIoc pdbbase");
    iocshCmd("dbLoadRecords /home/codac-dev/Documents/m-nds-test/target/main/epics/db/ndsTest.db, \"ASYN_PORT=MightyDevice-MyPort, PREFIX=myport\"");
    iocshCmd("iocInit");
    iocsh(0);
}

