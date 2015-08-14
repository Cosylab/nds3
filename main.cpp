#include <iostream>

#include "library/implementation/ndsfactoryimpl.h"
#include "iocsh.h"
using namespace std;

int main()
{
    nds::FactoryImpl::getInstance().registrationCommand("myIoc");

    iocshCmd("dbLoadDatabase /home/codac-dev/Documents/m-nds-test/target/main/epics/dbd/ndsTest.dbd");
    iocshCmd("myIoc pdbbase");
    iocsh(0);
}

