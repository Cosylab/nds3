#include <gtest/gtest.h>
#include <nds3/nds.h>

// Callback for the state machine
void noFunction()
{
}

bool allowStateChangeFunction(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}

TEST(testNamingRules, testDefaultRules)
{
    nds::Factory factory("test");

    std::string rules;
    rules += "[TEST]\n";
    rules += "separator0 = ;;\n";
    rules += "separator1 = +\n";
    rules += "separator2 = /\n";

    rules += "rootNode = ROOT%s\n";
    rules += "genericNode = GENERIC%s\n";
    rules += "inputNode = INPUT%s\n";
    rules += "outputNode = OUTPUT%s\n";
    rules += "sourceNode = SOURCE%s\n";
    rules += "sinkNode = SINK%s\n";
    rules += "inputPV = GET_%s\n";
    rules += "outputPV = SET_%s\n";
    rules += "stateMachineNode = STATE\n";
    rules += "setStatePV = SET_LOCALSTATE\n";
    rules += "getStatePV = GET_LOCALSTATE\n";
    rules += "getGlobalStatePV = GET_GLOBALSTATE\n";
    rules += "setDecimationPV = DECIMATION\n";

    std::istringstream rulesStream(rules);
    factory.loadNamingRules(rulesStream);

    nds::Port rootNode("rootNode");
    std::function<void(void)> function(std::bind(&::noFunction));
    std::function<bool(const nds::state_t, const nds::state_t, const nds::state_t)> allowStateChange(std::bind(&::allowStateChangeFunction, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    nds::Base stateMachine = rootNode.addChild(nds::StateMachine(true, function, function, function, function, function, allowStateChange));
    nds::Base acquisition = rootNode.addChild(nds::DataAcquisition<std::int32_t>("Acquisition", 100, function, function, function, function, function, allowStateChange));

    nds::Node child0 = rootNode.addChild(nds::Node("child0"));
    nds::Node child1 = child0.addChild(nds::Node("child1", nds::nodeType_t::inputChannel));
    nds::Node child2 = child1.addChild(nds::Node("child2", nds::nodeType_t::outputChannel));
    nds::Node child3 = child2.addChild(nds::Node("child3", nds::nodeType_t::dataSourceChannel));
    nds::Node child4 = child3.addChild(nds::Node("child4", nds::nodeType_t::dataSinkChannel));

    nds::Base pv0 = child4.addChild(nds::PVVariableOut<std::int32_t>("value0"));
    pv0.setExternalName("value");

    nds::Base pv1 = child4.addChild(nds::PVVariableIn<std::int32_t>("value1"));
    pv1.setExternalName("value");

    rootNode.initialize(0, factory);

    EXPECT_EQ(";;ROOTrootNode", rootNode.getFullExternalName());
    EXPECT_EQ(";;ROOTrootNode+STATE", stateMachine.getFullExternalName());
    EXPECT_EQ(";;ROOTrootNode+SOURCEAcquisition", acquisition.getFullExternalName());
    EXPECT_EQ(";;ROOTrootNode+GENERICchild0", child0.getFullExternalName());
    EXPECT_EQ(";;ROOTrootNode+GENERICchild0/INPUTchild1", child1.getFullExternalName());
    EXPECT_EQ(";;ROOTrootNode+GENERICchild0/INPUTchild1/OUTPUTchild2", child2.getFullExternalName());
    EXPECT_EQ(";;ROOTrootNode+GENERICchild0/INPUTchild1/OUTPUTchild2/SOURCEchild3", child3.getFullExternalName());
    EXPECT_EQ(";;ROOTrootNode+GENERICchild0/INPUTchild1/OUTPUTchild2/SOURCEchild3/SINKchild4/SET_value", pv0.getFullExternalName());
    EXPECT_EQ(";;ROOTrootNode+GENERICchild0/INPUTchild1/OUTPUTchild2/SOURCEchild3/SINKchild4/GET_value", pv1.getFullExternalName());

    factory.setNamingRules("");
    factory.destroyDevice("");
}


TEST(testNamingRules, testFallbackRules)
{
    nds::Factory factory("test");

    std::string rules;
    rules += "[TEST]\n";
    rules += "separator1 = +\n";
    rules += "separator2 = /\n";

    rules += "genericNode = GENERIC%s\n";
    rules += "inputNode = INPUT%s\n";
    rules += "inputPV = GET_%s\n";
    rules += "stateMachineNode = STATE\n";
    rules += "setStatePV = SET_LOCALSTATE\n";
    rules += "getStatePV = GET_LOCALSTATE\n";
    rules += "getGlobalStatePV = GET_GLOBALSTATE\n";
    rules += "setDecimationPV = DECIMATION\n";

    std::istringstream rulesStream(rules);
    factory.loadNamingRules(rulesStream);

    nds::Port rootNode("rootNode");
    std::function<void(void)> function(std::bind(&::noFunction));
    std::function<bool(const nds::state_t, const nds::state_t, const nds::state_t)> allowStateChange(std::bind(&::allowStateChangeFunction, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    nds::Base stateMachine = rootNode.addChild(nds::StateMachine(true, function, function, function, function, function, allowStateChange));
    nds::Base acquisition = rootNode.addChild(nds::DataAcquisition<std::int32_t>("Acquisition", 100, function, function, function, function, function, allowStateChange));

    nds::Node child0 = rootNode.addChild(nds::Node("child0"));
    nds::Node child1 = child0.addChild(nds::Node("child1", nds::nodeType_t::inputChannel));
    nds::Node child2 = child1.addChild(nds::Node("child2", nds::nodeType_t::outputChannel));
    nds::Node child3 = child2.addChild(nds::Node("child3", nds::nodeType_t::dataSourceChannel));
    nds::Node child4 = child3.addChild(nds::Node("child4", nds::nodeType_t::dataSinkChannel));

    nds::Base pv0 = child4.addChild(nds::PVVariableOut<std::int32_t>("value0"));
    pv0.setExternalName("value");

    nds::Base pv1 = child4.addChild(nds::PVVariableIn<std::int32_t>("value1"));
    pv1.setExternalName("value");

    rootNode.initialize(0, factory);

    EXPECT_EQ("/GENERICrootNode", rootNode.getFullExternalName());
    EXPECT_EQ("/GENERICrootNode+STATE", stateMachine.getFullExternalName());
    EXPECT_EQ("/GENERICrootNode+INPUTAcquisition", acquisition.getFullExternalName());
    EXPECT_EQ("/GENERICrootNode+GENERICchild0", child0.getFullExternalName());
    EXPECT_EQ("/GENERICrootNode+GENERICchild0/INPUTchild1", child1.getFullExternalName());
    EXPECT_EQ("/GENERICrootNode+GENERICchild0/INPUTchild1/GENERICchild2", child2.getFullExternalName());
    EXPECT_EQ("/GENERICrootNode+GENERICchild0/INPUTchild1/GENERICchild2/INPUTchild3", child3.getFullExternalName());
    EXPECT_EQ("/GENERICrootNode+GENERICchild0/INPUTchild1/GENERICchild2/INPUTchild3/GENERICchild4/value", pv0.getFullExternalName());
    EXPECT_EQ("/GENERICrootNode+GENERICchild0/INPUTchild1/GENERICchild2/INPUTchild3/GENERICchild4/GET_value", pv1.getFullExternalName());

    factory.setNamingRules("");
    factory.destroyDevice("");
}



