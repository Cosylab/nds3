#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "testDevice.h"
#include "ndsTestInterface.h"
#include <sstream>

TEST(testIniParser, parseFile)
{
    std::string iniContent;

    iniContent += " [section1  ] \n";
    iniContent += " key0 = value0 \n";
    iniContent += " key1 = \"value0 plus 1 \"\n";
    iniContent += "key2=\" value0 plus 2\"\n";
    iniContent += "key3 = 'value0 plus 3 '\n";
    iniContent += "key4 =";

    std::istringstream iniFile(iniContent);
    nds::IniFileParser parser(iniFile);

    EXPECT_EQ("value0", parser.getString("section1", "key0", "default0"));
    EXPECT_TRUE(parser.keyExists("section1", "key0"));
    EXPECT_FALSE(parser.keyExists("section1", "key5"));
    EXPECT_THROW(parser.keyExists("section", "key0"), nds::INIParserMissingSection);
    EXPECT_EQ("value0 plus 1 ", parser.getString("section1", "key1", "default1"));
    EXPECT_TRUE(parser.keyExists("section1", "key1"));
    EXPECT_EQ(" value0 plus 2", parser.getString("section1", "key2", "default2"));
    EXPECT_TRUE(parser.keyExists("section1", "key2"));
    EXPECT_EQ("value0 plus 3 ", parser.getString("section1", "key3", "default3"));
    EXPECT_TRUE(parser.keyExists("section1", "key3"));
    EXPECT_EQ("", parser.getString("section1", "key4", "default4"));
    EXPECT_TRUE(parser.keyExists("section1", "key4"));
    EXPECT_EQ("default5", parser.getString("section1", "key5", "default5"));
    EXPECT_FALSE(parser.keyExists("section1", "key5"));
}

TEST(testIniParser, testEmptySectionAndComments)
{
    std::string iniContent;

    iniContent += " key0 = value0 #This is a comment\n";
    iniContent += " key1 = \"value0 plus 1 \"\n";
    iniContent += "\r\n\n\r";
    iniContent += "[section1]\r\n";
    iniContent += "key2=\" value0 plus #2\"\n";
    iniContent += "   #comment\n";
    iniContent += "key3 = 'value0 plus #3 ' #Another commenr\n";
    iniContent += "key4 =";

    std::istringstream iniFile(iniContent);
    nds::IniFileParser parser(iniFile);

    EXPECT_EQ("value0", parser.getString("", "key0", "default0"));
    EXPECT_TRUE(parser.keyExists("", "key0"));
    EXPECT_FALSE(parser.keyExists("", "key5"));
    EXPECT_FALSE(parser.keyExists("section1", "key0"));
    EXPECT_EQ("value0 plus 1 ", parser.getString("", "key1", "default1"));
    EXPECT_TRUE(parser.keyExists("", "key1"));
    EXPECT_EQ(" value0 plus #2", parser.getString("section1", "key2", "default2"));
    EXPECT_TRUE(parser.keyExists("section1", "key2"));
    EXPECT_EQ("value0 plus #3 ", parser.getString("section1", "key3", "default3"));
    EXPECT_TRUE(parser.keyExists("section1", "key3"));
    EXPECT_EQ("", parser.getString("section1", "key4", "default4"));
    EXPECT_TRUE(parser.keyExists("section1", "key4"));
    EXPECT_EQ("default5", parser.getString("section1", "key5", "default5"));
    EXPECT_FALSE(parser.keyExists("section1", "key5"));
}



