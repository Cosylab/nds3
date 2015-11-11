/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSLOGSTREAMGETTERIMPL_H
#define NDSLOGSTREAMGETTERIMPL_H

#include <ostream>
#include <array>
#include "../nds3/definitions.h"

namespace nds
{

class BaseImpl;


class NDS3_API LogStreamGetterImpl
{
public:
    LogStreamGetterImpl();

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

    std::ostream* getLogStream(const logLevel_t logLevel);

protected:
    virtual std::ostream* createLogStream(const logLevel_t logLevel) = 0;

    /**
     * @brief This method is registered with pthread_create_key(&m_removeLoggersKey, this)
     *        to remove the loggers that are specific to the running thread
     */

private:
    static void deleteLogger(void* logger);

    /**
     * @brief Used to gain access to the node's loggers (they are different for each thread)
     */
    std::array<pthread_key_t, (size_t)logLevel_t::none> m_loggersKeys;


};
}
#endif // NDSLOGSTREAMGETTERIMPL_H
