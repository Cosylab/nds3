#include "ndsdataacquisitionimpl.h"

namespace nds
{

timespec DataAcquisitionImpl::getStartTimestamp()
{
    return m_startTime;
}

void DataAcquisitionImpl::onStart()
{
    m_startTime = m_getTimestampFunction();
    m_onStartDelegate();
}


}
