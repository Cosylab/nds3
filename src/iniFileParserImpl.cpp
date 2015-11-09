#include "../include/nds3impl/iniFileParserImpl.h"
#include "../include/nds3/exceptions.h"
#include <iostream>
#include <sstream>

namespace nds
{

static const std::string m_spaces(" \t\r\n");
static const std::string m_quotes("'\"");

IniFileParserImpl::IniFileParserImpl(std::istream& inputStream)
{
    std::string lastSection;

    std::string line;
    size_t lineCounter(0);
    for(
        std::getline(inputStream, line);
        (inputStream.rdstate() & std::istream::failbit) == 0;
        std::getline(inputStream, line))
    {
        ++lineCounter;

        try
        {
            std::string section = getSection(line);
            if(!section.empty())
            {
                lastSection = section;
                continue;
            }

            keyValue_t valueKey(getKeyValue(line));
            if(!valueKey.first.empty())
            {
                m_sections[lastSection][valueKey.first] = valueKey.second;
            }
        }
        catch(const INIParserError& e)
        {
            std::ostringstream errorMessage;
            errorMessage << "Syntax error on line " << lineCounter << ": " << e.what();
            throw INIParserSyntaxError(errorMessage.str());
        }
    }
}

IniFileParserImpl::~IniFileParserImpl()
{

}

const std::string& IniFileParserImpl::getString(const std::string &section, const std::string &key, const std::string &defaultValue) const
{
    if(key.empty())
    {
        return defaultValue;
    }

    sectionKeyMap_t::const_iterator findSection(m_sections.find(section));
    if(findSection == m_sections.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The section " << section << " is missing from the INI file";
        throw INIParserMissingSection(errorMessage.str());
    }

    keyValueMap_t::const_iterator findKey(findSection->second.find(key));
    if(findKey == findSection->second.end())
    {
        return defaultValue;
    }
    return findKey->second;
}

bool IniFileParserImpl::keyExists(const std::string &section, const std::string &key) const
{
    sectionKeyMap_t::const_iterator findSection(m_sections.find(section));
    if(findSection == m_sections.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The section " << section << " is missing from the INI file";
        throw INIParserMissingSection(errorMessage.str());
    }

    keyValueMap_t::const_iterator findKey(findSection->second.find(key));
    return (findKey != findSection->second.end());
}

std::string IniFileParserImpl::trim(const std::string& string)
{
    size_t lastChar = string.find_last_not_of(m_spaces);
    size_t firstChar = string.find_first_not_of(m_spaces);

    if(lastChar == string.size() - 1 && firstChar == 0)
    {
        return string;
    }

    std::string returnString(string);

    if (lastChar != std::string::npos)
        returnString.erase(++lastChar);
    else
        return "";

    if(firstChar != 0)
    {
        returnString.erase(0, firstChar);
    }

    return returnString;
}

std::string IniFileParserImpl::getSection(const std::string &line)
{
    std::string section(trim(line));
    if(section.empty() || section.at(0) != '[')
    {
        return "";
    }

    size_t endSection(findFirstUnescapedChar(section, ']'));
    if(endSection == std::string::npos)
    {
        return "";
    }

    return trim(section.substr(1, --endSection));

}

size_t IniFileParserImpl::findFirstUnescapedChar(const std::string &string, const char findChar, const size_t startPosition)
{
    for(size_t startPos(startPosition); startPos < string.size();)
    {
        size_t findCharPosition = string.find(findChar, startPos);
        if(findCharPosition == std::string::npos || findCharPosition == 0 || string.at(findCharPosition - 1) != '\\')
        {
            return findCharPosition;
        }

        startPos = ++findCharPosition;
    }
    return std::string::npos;
}

IniFileParserImpl::keyValue_t IniFileParserImpl::getKeyValue(const std::string& line)
{
    static const std::string commentStart("#;");

    size_t equalSign = findFirstUnescapedChar(line, '=');
    if(equalSign == std::string::npos)
    {
        std::string trimmed(trim(line));
        if(trimmed.size() == 0 || commentStart.find(trimmed.at(0)) != std::string::npos)
        {
            return keyValue_t("", "");
        }
        throw INIParserSyntaxError("Syntax error");
    }

    std::string variable(trim(line.substr(0, equalSign)));
    if(variable.empty())
    {
        throw;
    }

    std::string value(trim(line.substr(++equalSign)));
    if(!value.empty())
    {
        if(m_quotes.find(value.at(0)) != std::string::npos)
        {
            size_t findEndQuotes = findFirstUnescapedChar(value, value.at(0), 1);
            if(findEndQuotes == std::string::npos)
            {
                throw;
            }
            return keyValue_t(variable, value.substr(1, --findEndQuotes));
        }
        size_t findEndValue(value.find_first_of(m_spaces));
        if(findEndValue == std::string::npos)
        {
            return keyValue_t(variable, value);
        }
        return keyValue_t(variable, value.substr(0, findEndValue));
    }
    return keyValue_t(variable, "");
}


}
