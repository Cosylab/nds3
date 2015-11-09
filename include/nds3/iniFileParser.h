#ifndef NDSINIFILEPARSER_H
#define NDSINIFILEPARSER_H

#include <istream>
#include <string>
#include <memory>
#include "definitions.h"

namespace nds
{

class IniFileParserImpl;

class NDS3_API IniFileParser
{
public:
    IniFileParser(std::istream& inputStream);

    ~IniFileParser();

    /**
     * @brief Retrieve the value for a specific key in the parsed INI file.
     *
     * Throws
     * @param section
     * @param key
     * @param defaultValue
     * @return
     */
    const std::string& getString(const std::string& section, const std::string& key, const std::string& defaultValue) const;

    bool keyExists(const std::string& section, const std::string& key) const;

private:
    std::shared_ptr<IniFileParserImpl> m_pImplementation;


};

}

#endif // NDSINIFILEPARSER_H

