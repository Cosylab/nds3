/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3/iniFileParser.h"
#include "../include/nds3impl/iniFileParserImpl.h"

namespace nds
{

IniFileParser::IniFileParser(std::istream &inputStream):
    m_pImplementation(std::make_shared<IniFileParserImpl>(inputStream))
{
}

IniFileParser::~IniFileParser()
{
}

const std::string& IniFileParser::getString(const std::string &section, const std::string &key, const std::string &defaultValue) const
{
    return m_pImplementation->getString(section, key, defaultValue);
}

bool IniFileParser::keyExists(const std::string &section, const std::string &key) const
{
    return m_pImplementation->keyExists(section, key);
}

}
