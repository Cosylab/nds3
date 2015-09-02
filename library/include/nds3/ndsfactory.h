#ifndef NDSFACTORY_H
#define NDSFACTORY_H

#include <string>
#include <memory>
#include "definitions.h"

namespace nds
{

class FactoryImpl;

class NDS3_API Factory
{
public:
    Factory();

    void registrationCommand(const std::string& registrationCommandName);

    void run();

#ifndef SWIG
private:
    FactoryImpl* m_pFactory;
#endif
};

}
#endif // NDSFACTORY_H
