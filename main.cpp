#include <iostream>
#include <memory>
#include <vector>

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

    void readData(timespec*, std::vector<std::int32_t> * pWave)
    {
        pWave->resize(5);
        (*pWave)[0] = 1;
        (*pWave)[1] = 2;
        (*pWave)[2] = 3;
        (*pWave)[3] = 4;
        (*pWave)[4] = 5;
    }

    void writeData(const timespec&, const std::vector<std::int32_t>& wave)
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
    nds::PVBase errorCode = port.addChild(nds::PVDelegate<std::int32_t>("ErrorCode",
                                  std::bind(&Delegate::readErrorCode, &myLogicIsHere, std::placeholders::_1, std::placeholders::_2),
                                  std::bind(&Delegate::writeErrorCode, &myLogicIsHere, std::placeholders::_1, std::placeholders::_2)
                                  ));
    errorCode.setType(nds::longin);
    errorCode.setDescription("Represents the error code");
    errorCode.setInterfaceName("STS");
    errorCode.setScanType(nds::interrupt, 0);

    nds::PVBase data = port.addChild(nds::PVDelegate<std::vector<std::int32_t> >("Data",
                                  std::bind(&Delegate::readData, &myLogicIsHere, std::placeholders::_1, std::placeholders::_2),
                                  std::bind(&Delegate::writeData, &myLogicIsHere, std::placeholders::_1, std::placeholders::_2)
                                  ));
    data.setType(nds::waveformIn);
    data.setDescription("Acquired data");
    data.setInterfaceName("DATA");
    data.setScanType(nds::passive, 0);
    data.setMaxElements(20);

    port.addChild(nds::PVHoldDelegate("Delegate", nds::dataInt32, std::shared_ptr<nds::Delegate>(new nds::Delegate())));

    // All the structure has been setup. Register the ports and the PVs
    ///////////////////////////////////////////////////////////////////
    theDevice.initialize();

    iocshCmd("dbLoadDatabase ndsTest.dbd /home/codac-dev/Documents/m-nds-test/target/main/epics/dbd");
    iocshCmd("myIoc pdbbase");
    iocshCmd("dbLoadRecords auto_generated_MightyDevice-MyPort.db");
    iocshCmd("iocInit");

    ::sleep(2);
    timespec time;
    errorCode.push(time, 4);
    iocsh(0);
}

