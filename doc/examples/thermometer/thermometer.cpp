#include <nds3/nds.h>

// This class declares our device in the contructor and supplies the functionalities to support it
//////////////////////////////////////////////////////////////////////////////////////////////////
class Thermometer
{
public:

    // Constructor. Here we setup the device structure
    //////////////////////////////////////////////////
    Thermometer(nds::Factory &factory,
                const std::string &deviceName,
                const nds::namedParameters_t &parameters)
    {
        nds::Port rootNode(deviceName);
        rootNode.addChild(nds::PVDelegateIn<double>("Temperature", std::bind(&Thermometer::getTemperature, 
                                                                             this, 
                                                                             std::placeholders::_1, 
                                                                             std::placeholders::_2)));

        rootNode.initialize(this, factory);
    }

    void getTemperature(timespec* pTimestamp, double* pValue)
    {
        *pValue = 10; // It is always cold in here
    }
};

// If we want to load our device support dynamically then we have to compile it
//  as a dynamic module and we have to provide the functions to allocate it,
//  deallocate it and retrieve its name
////////////////////////////////////////////////////////////////////////////////
NDS_DEFINE_DRIVER(Thermometer, Thermometer);

