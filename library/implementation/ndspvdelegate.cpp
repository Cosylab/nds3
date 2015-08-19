#include "../include/nds3/ndspvdelegate.h"
#include "ndspvdelegateimpl.h"

namespace nds
{

template <typename T>
PVDelegate<T>::PVDelegate(const std::string& name, tRead readFunction, tWrite writeFunction):
    PVBase(std::shared_ptr<PVBaseImpl>(new PVDelegateImpl<T>(name, readFunction, writeFunction)))
{}

template <typename T>
void PVDelegate<T>::read(timespec* pTimestamp, T* pValue)
    {
        m_reader(pTimestamp, pValue);
    }

template <typename T>
void PVDelegate<T>::write(const timespec& timestamp, const T& value)
    {
        m_writer(timestamp, value);
    }

template class PVDelegate<std::int32_t>;


}
