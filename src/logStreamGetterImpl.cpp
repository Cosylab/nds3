/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2016 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3impl/logStreamGetterImpl.h"

namespace nds
{

LogStreamGetterImpl::LogStreamGetterImpl()
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys.size(); ++scanLevels)
    {
        pthread_key_create(&(m_loggersKeys[scanLevels]), &LogStreamGetterImpl::deleteLogger);
    }

}

LogStreamGetterImpl::~LogStreamGetterImpl()
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys.size(); ++scanLevels)
    {
        std::ostream* pStream = (std::ostream*)pthread_getspecific(m_loggersKeys[scanLevels]);
        if(pStream != 0)
        {
            pthread_setspecific(m_loggersKeys[scanLevels], 0);
            delete pStream;
        }
        pthread_key_delete(m_loggersKeys[scanLevels]);
    }
}

std::ostream* LogStreamGetterImpl::getLogStream(const logLevel_t logLevel)
{
    std::ostream* pStream = (std::ostream*)pthread_getspecific(m_loggersKeys[(size_t)logLevel]);
    if(pStream == 0)
    {
        pStream = createLogStream(logLevel);
        pthread_setspecific(m_loggersKeys[(size_t)logLevel], pStream);
    }

    return pStream;
}

void LogStreamGetterImpl::deleteLogger(void* pLogger)
{
    delete (std::ostream*)pLogger;
}


}
