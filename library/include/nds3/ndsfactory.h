#ifndef NDSFACTORY_H
#define NDSFACTORY_H

#include <string>
#include <memory>

namespace nds
{

class FactoryImpl;

class Factory
{
public:
    Factory();

    void registrationCommand(const std::string& registrationCommandName);

    void run();

private:
    FactoryImpl* m_pFactory;
};

}
#endif // NDSFACTORY_H
