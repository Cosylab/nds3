/**

@page naming_rules Naming rules

Each node or PV can have an external name set via nds::Base::setExternalName().
If the external name is not set then it has the same value as the node or PV's name.

The following code creates a Thermometer device with a single input PV named "Temperature"; not however
that we add a line of code that changes the external name of the PV to "temp".\n

@code
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
   
    nds::PVBaseIn ourOnlyPV = rootNode.addChild(nds::PVDelegateIn<double>("Temperature", std::bind(&Thermometer::getTemperature, 
                                                                         this, 
                                                                         std::placeholders::_1, 
                                                                         std::placeholders::_2));

    // The following line causes our temperature PV to be exposed as "temp"
    ///////////////////////////////////////////////////////////////////////
    ourOnlyPV.setExternalName("temp");

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
NDS_DEFINE_DRIVER("Thermometer", Thermometer);
@endcode

For instance, if we create the device on an EPICS IOC we will see the following:
@code
epics> createNdsDevice Thermometer myThermometer
epics> iocInit
epics> dbl
myThermometer-temp
@endcode

The external names can be modified even further by loading and applying a @ref naming_rules_file.

The naming rules definition file contains the transformations to apply to each node according to its type
and also defines the separator to insert between the component names.


@section naming_rules_file Naming rules definition file

The naming rules definition file is an INI file that can contain one or more sections, which in turn can have
several lines defining the naming rules (one rule per line).

Each section name is enclosed by square brackets while each rule is a variable assignment: this section also
lists which variables can be used in the definition file.

Comments begin with the char #.

Example:
@code
[COSYLAB]   # Naming rules for COSYLAB

toUpper = 1 # All the names will be converted to upper-case

# The separator for the level 0 is empty (nothing is prepended to the root node name)
separator0 =

separator1 = /   # Example root/node0
separator2 = -   # Example root/node0-node1

# We don't have a separator for the level 3 and subsequent, so the separator2 will be used
# for deeper levels

# How do we form the root node name
rootNode = "DEVICE_%s_COSYLAB"

# Input PV
inputPV = "GET_%s"
@endcode

If we apply those rules to the sample code from @ref naming_rules then the temperature PV will be available
with the following name:
@code
epics> dbl
DEVICE_MYTHERMOMETER_COSYLAB/GET_TEMP
@endcode

@subsection naming_rules_variables Recognized variables in the Rules Definition File

@subsubsection toUpper

If the value is 1 then all the external names are converted to upper-case before being fed into the naming rules.
The default value is 0.

@subsubsection toLower

If the value is 1 then all the external names are converted to lower-case before being fed into the naming rules.
The default value is 0.




*/

