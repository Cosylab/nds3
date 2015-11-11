/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVBASEIMPL_H
#define NDSPVBASEIMPL_H

#include "baseImpl.h"
#include "../nds3/definitions.h"

#include <string>

namespace nds
{

class PVBase;

/**
 * @brief Base class for all the PVs.
 */
class NDS3_API PVBaseImpl: public BaseImpl
{
public:
    PVBaseImpl(const std::string& name);

    /**
     * @brief Register the PV with the control system
     *
     * @param controlSystem the control system on which the PV will be registered
     */
    virtual void initialize(FactoryBaseImpl& controlSystem);

    /**
     * @brief Deregister the PV from the control system.
     */
    virtual void deinitialize();

    /**
     * @brief Called when the control system wants to read the value.
     *
     * The default implementation throws an exception.
     *
     * @param pTimestamp
     * @param pValue
     */
    virtual void read(timespec* pTimestamp, std::int32_t* pValue) const;
    virtual void read(timespec* pTimestamp, double* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::uint8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int32_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<double>* pValue) const;
    virtual void read(timespec* pTimestamp, std::string* pValue) const;

    /**
     * @brief Called when the control system wants to write a value.
     *
     * The default implementation throws an exception.
     *
     * @param timestamp
     * @param value
     */
    virtual void write(const timespec& timestamp, const std::int32_t& value);
    virtual void write(const timespec& timestamp, const double& value);
    virtual void write(const timespec& timestamp, const std::vector<std::int8_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<std::uint8_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<std::int32_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<double>& value);
    virtual void write(const timespec& timestamp, const std::string& value);

    /**
     * @brief Retrieve the data direction.
     *
     * @return the data direction
     */
    virtual dataDirection_t getDataDirection() const = 0;

    /**
     * @brief Return the PV's data type.
     *
     * @return the data type used by the PV
     */
    virtual dataType_t getDataType() const = 0;

    /**
     * @brief Set the PV description.
     *
     * @param description the description associated with the PV.
     */
    void setDescription(const std::string& description);

    /**
     * @brief Set the engineering units associated with the PV.
     *
     * @param units the engineering units (e.g. "Ohm").
     */
    void setUnits(const std::string& units);

    /**
     * @brief Set the scan type.
     *
     * @param scanType      scan type
     * @param periodSeconds seconds between scans if scanType is scanType_t::periodic
     */
    void setScanType(const scanType_t scanType, const double periodSeconds);

    /**
     * @brief Set the maximum number of elements if the data type is an array.
     *
     * @param maxElements maximum number of elements that will be stored in the array
     */
    void setMaxElements(const size_t maxElements);

    /**
     * @brief Assign enumeration strings to the PV.
     *
     * @param enumerations list of strings to use for the enumeration
     */
    void setEnumeration(const enumerationStrings_t& enumerations);

    /**
     * @brief Specifies if the PV has to be processed during the initialization.
     *
     * @param bProcessAtInit true if the PV has to be processed during the initialization,
     *                       false otherwise
     */
    void processAtInit(const bool bProcessAtInit);

    /**
     * @brief Return the PV's description.
     *
     * @return the description associated with the PV
     */
    const std::string& getDescription() const;

    /**
     * @brief Return the PV's engineering units.
     *
     * @return the engineering units associated with the PV
     */
    const std::string& getUnits() const;

    /**
     * @brief Return the scan type.
     *
     * @return the scan type
     */
    scanType_t getScanType() const;

    /**
     * @brief Return the scan period in seconds.
     *
     * @return the scan period in seconds.
     */
    double getScanPeriodSeconds() const;

    /**
     * @brief Return the maximum number of elements that can be stored in the array
     *        (if the PV data type is an array).
     *
     * @return the maximum number of elements that can be stored in the PV
     */
    size_t getMaxElements() const;

    /**
     * @brief Return the list of string used to enumerate the integer values.
     *
     * @return list of string used for the enumeration
     */
    const enumerationStrings_t& getEnumerations() const;

    /**
     * @brief Return true if the PV has to bve processed during the initialization
     *        of the device.
     *
     * @return true if the PV has to be processed during the device initialization,
     *         false otherwise
     */
    bool getProcessAtInit() const;

    /**
     * @brief Return the data type enumerator for the type in the template.
     *
     * @tparam type for which the data type enumerator is requested
     * @return an enumerator for the data type in the template
     */
    template<typename T>
    static dataType_t getDataTypeForCPPType()
    {
        const int type =
                int(std::is_same<T, std::int32_t>::value) * (int)dataType_t::dataInt32 +
                int(std::is_same<T, double>::value) * (int)dataType_t::dataFloat64 +
                int(std::is_same<T, std::vector<std::int8_t> >::value) * (int)dataType_t::dataInt8Array +
                int(std::is_same<T, std::vector<std::uint8_t> >::value) * (int)dataType_t::dataUint8Array +
                int(std::is_same<T, std::vector<std::int32_t> >::value) * (int)dataType_t::dataInt32Array +
                int(std::is_same<T, std::vector<double> >::value) * (int)dataType_t::dataFloat64Array +
                int(std::is_same<T, std::string>::value) * (int)dataType_t::dataString;

        static_assert(type != 0, "Undefined data type");
        return(dataType_t)type;
    }

protected:
    std::string m_description;          ///< The PV's description.
    std::string m_units;                ///< Engineering units
    scanType_t m_scanType;              ///< The PV's scan type.
    double m_periodicScanSeconds;       ///< The interval between data polling (in seconds).
    size_t m_maxElements;               ///< Maximum number of elements that can be stored in the PV.
    enumerationStrings_t m_enumeration; ///< List of strings used for enumeration.
    bool m_bProcessAtInit;              ///< True if the PV has to be processed during the device initialization.
};

}
#endif // NDSPVBASEIMPL_H
