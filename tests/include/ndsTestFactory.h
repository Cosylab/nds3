#include <nds3impl/factoryBaseImpl.h>
#include <nds3impl/logStreamGetterImpl.h>

namespace nds
{

namespace tests
{

class TestControlSystemFactoryImpl: public FactoryBaseImpl, public LogStreamGetterImpl
{
public:
    virtual const std::string getName() const;

    virtual InterfaceBaseImpl* getNewInterface(const std::string& fullName);

    virtual void run(int argc,char *argv[]);

    virtual LogStreamGetterImpl* getLogStreamGetter();

    virtual void registerCommand(const BaseImpl& node,
                                 const std::string& command,
                                 const std::string& usage,
                                 const size_t numParameters, command_t commandFunction);

    virtual void deregisterCommand(const BaseImpl& node);

    virtual std::ostream* getLogStream(const BaseImpl& node, const logLevel_t logLevel);
};

}

}
