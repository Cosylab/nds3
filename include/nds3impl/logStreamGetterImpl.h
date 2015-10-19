#ifndef NDSLOGSTREAMGETTERIMPL_H
#define NDSLOGSTREAMGETTERIMPL_H

#include <ostream>
#include "../nds3/definitions.h"

namespace nds
{

class BaseImpl;


class NDS3_API LogStreamGetterImpl
{
public:
    virtual ~LogStreamGetterImpl();

    /**
     * @brief Allocates a new stream logger and returns it.
     *
     * It is the responsability of the caller to delete the logger when it is no longer
     *  needed.
     *
     * @param node     the node that will stream data to the new log stream
     * @param logLevel the log level
     * @return         the newly allocate log stream
     */
    virtual std::ostream* getLogStream(const BaseImpl& node, const logLevel_t logLevel) = 0;
};
}
#endif // NDSLOGSTREAMGETTERIMPL_H
