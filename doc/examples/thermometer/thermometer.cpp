#include <nds3/nds.h>
#include <iostream>

// This class declares our device in the contructor and supplies the functionalities to support it
//////////////////////////////////////////////////////////////////////////////////////////////////
class Thermometer
{
private:
	nds::PVDelegateIn<double> m_thermometerPiniPV;

public:

    // Constructor. Here we setup the device structure
    //////////////////////////////////////////////////
    Thermometer(nds::Factory &factory,
                const std::string &deviceName,
                const nds::namedParameters_t &parameters) :
	m_thermometerPiniPV(nds::PVDelegateIn<double>("TemperaturePINI", std::bind(&Thermometer::getTempRandom, 
                                                                             this, 
                                                                             std::placeholders::_1, 
                                                                             std::placeholders::_2)))
    {
        nds::Port rootNode(deviceName);
        rootNode.addChild(nds::PVDelegateIn<double>("Temperature", std::bind(&Thermometer::getTemperature, 
                                                                             this, 
                                                                             std::placeholders::_1, 
                                                                             std::placeholders::_2)));

	m_thermometerPiniPV.processAtInit(1);
	rootNode.addChild(m_thermometerPiniPV);

        rootNode.initialize(this, factory);
    }

    void getTemperature(timespec* pTimestamp, double* pValue)
    {
        *pValue = 10; // It is always cold in here
	std::cout << "Temperature #1: " << *pValue << std::endl;
    }

    void getTempRandom(timespec* pTimestamp, double* pValue)
    {
        *pValue = 35; // It is always hot right there
	std::cout << "Temperature #2 (pini): " << *pValue << std::endl;
    }
};

// If we want to load our device support dynamically then we have to compile it
//  as a dynamic module and we have to provide the functions to allocate it,
//  deallocate it and retrieve its name
////////////////////////////////////////////////////////////////////////////////
NDS_DEFINE_DRIVER(Thermometer, Thermometer)

