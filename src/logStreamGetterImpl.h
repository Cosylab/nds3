#ifndef NDSLOGSTREAMGETTERIMPL_H
#define NDSLOGSTREAMGETTERIMPL_H

#include <ostream>
#include "../include/nds3/definitions.h"

namespace nds
{

class BaseImpl;

class LogStreamGetterImpl
{
public:
    virtual ~LogStreamGetterImpl();

    virtual std::ostream* getLogStream(const BaseImpl& node, const logLevel_t logLevel) = 0;
};
}
#endif // NDSLOGSTREAMGETTERIMPL_H
