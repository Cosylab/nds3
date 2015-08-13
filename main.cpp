#include <iostream>

#include "library/implementation/ndsfactoryimpl.h"
#include "iocsh.h"
using namespace std;

int main()
{
    nds::FactoryImpl::getInstance().registrationCommand("myIoc");

    iocsh(0);
}

