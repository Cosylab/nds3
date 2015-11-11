/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSINIFILEPARSER_H
#define NDSINIFILEPARSER_H

/**
 * @file iniFileParser.h
 *
 * @brief Defines the class nds::IniFileParser used to read and parse the INI files.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include <istream>
#include <string>
#include <memory>
#include "definitions.h"

namespace nds
{

class IniFileParserImpl;

/**
 * @brief Parses an INI file and stores all the found sections and keys
 *        for future retrieval via getString().
 *
 */
class NDS3_API IniFileParser
{
public:
    /**
     * @brief Parses the INI file in the input stream and stores all the found
     *        sections and keys with the corresponding values.
     *
     * @param inputStream the stream to parse
     */
    IniFileParser(std::istream& inputStream);

    ~IniFileParser();

    /**
     * @brief Retrieve the value for a specific key in the parsed INI file.
     *
     * Throws INIParserMissingSection if the specified section cannot be found.
     *
     * @param section      the section to which the key belongs
     * @param key          the key to retrieve
     * @param defaultValue the default value to return in the case that the key cannot be found
     * @return             the value related to the key or defaultValue if the key doesn't exist
     */
    const std::string& getString(const std::string& section, const std::string& key, const std::string& defaultValue) const;

    /**
     * @brief Return true if the requested key exists and has a value (even an empty one),
     *               false otherwise.
     *
     * Throws INIParserMissingSection if the specified section cannot be found.
     *
     * @param section the section to which the key belongs
     * @param key     the key
     * @return        true if the key exists in the INI file, false otherwise
     */
    bool keyExists(const std::string& section, const std::string& key) const;

private:
    std::shared_ptr<IniFileParserImpl> m_pImplementation;


};

}

#endif // NDSINIFILEPARSER_H

