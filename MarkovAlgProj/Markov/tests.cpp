#include <UnitTest++/UnitTest++.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "../Markov/MarkovAlg.h"
#include "../Markov/Rule.h"
#include "../Markov/RuleParser.h"
#include "../Markov/Tape.h"

TEST(Rule_Constructor_WithParameters) {
  Rule rule("pattern", "replacement", true);

  CHECK_EQUAL("pattern", rule.getPattern());
  CHECK_EQUAL("replacement", rule.getReplacement());
  CHECK_EQUAL(true, rule.isFinal());
}

TEST(Rule_Constructor_Default) {
  Rule rule;

  CHECK_EQUAL("", rule.getPattern());
  CHECK_EQUAL("", rule.getReplacement());
  CHECK_EQUAL(false, rule.isFinal());
}

TEST(Rule_CopyConstructor) {
  Rule original("abc", "xyz", true);
  Rule copy(original);

  CHECK_EQUAL("abc", copy.getPattern());
  CHECK_EQUAL("xyz", copy.getReplacement());
  CHECK_EQUAL(true, copy.isFinal());
}

TEST(Rule_AssignmentOperator) {
  Rule rule1("a", "b", false);
  Rule rule2("x", "y", true);

  rule1 = rule2;

  CHECK_EQUAL("x", rule1.getPattern());
  CHECK_EQUAL("y", rule1.getReplacement());
  CHECK_EQUAL(true, rule1.isFinal());
}

TEST(Rule_Equality_IdenticalRules) {
  Rule rule1("pattern", "replacement", true);
  Rule rule2("pattern", "replacement", true);

  CHECK(rule1 == rule2);
}

TEST(Rule_Equality_DifferentPattern) {
  Rule rule1("pattern1", "replacement", true);
  Rule rule2("pattern2", "replacement", true);

  CHECK(rule1 != rule2);
}

TEST(Rule_Equality_DifferentReplacement) {
  Rule rule1("pattern", "replacement1", true);
  Rule rule2("pattern", "replacement2", true);

  CHECK(rule1 != rule2);
}

TEST(Rule_Equality_DifferentFinalFlag) {
  Rule rule1("pattern", "replacement", true);
  Rule rule2("pattern", "replacement", false);

  CHECK(rule1 != rule2);
}

TEST(MarkovTape_Constructor_WithInitialString) {
  MarkovTape tape("hello world");

  CHECK_EQUAL("hello world", tape.getContent());
}

TEST(MarkovTape_Constructor_Default) {
  MarkovTape tape;

  CHECK_EQUAL("", tape.getContent());
}

TEST(MarkovTape_SetContent) {
  MarkovTape tape;
  tape.setContent("new content");

  CHECK_EQUAL("new content", tape.getContent());
}

TEST(MarkovTape_Equality_IdenticalContent) {
  MarkovTape tape1("content");
  MarkovTape tape2("content");

  CHECK(tape1 == tape2);
}

TEST(MarkovTape_Equality_DifferentContent) {
  MarkovTape tape1("content1");
  MarkovTape tape2("content2");

  CHECK(tape1 != tape2);
}

TEST(RuleParser_Parse_SimpleRule) {
  Rule rule = RuleParser::parse("a -> b");

  CHECK_EQUAL("a", rule.getPattern());
  CHECK_EQUAL("b", rule.getReplacement());
  CHECK_EQUAL(false, rule.isFinal());
}

TEST(RuleParser_Parse_FinalRule) {
  Rule rule = RuleParser::parse("ab -> x final");

  CHECK_EQUAL("ab", rule.getPattern());
  CHECK_EQUAL("x", rule.getReplacement());
  CHECK_EQUAL(true, rule.isFinal());
}

TEST(RuleParser_Parse_EmptyPattern) {
  Rule rule = RuleParser::parse("-> start");

  CHECK_EQUAL("", rule.getPattern());
  CHECK_EQUAL("start", rule.getReplacement());
  CHECK_EQUAL(false, rule.isFinal());
}

TEST(RuleParser_Parse_EmptyReplacement) {
  Rule rule = RuleParser::parse("remove ->");

  CHECK_EQUAL("remove", rule.getPattern());
  CHECK_EQUAL("", rule.getReplacement());
  CHECK_EQUAL(false, rule.isFinal());
}

TEST(RuleParser_Parse_WithEMPTYKeyword) {
  Rule rule = RuleParser::parse("EMPTY -> begin");

  CHECK_EQUAL("", rule.getPattern());
  CHECK_EQUAL("begin", rule.getReplacement());
}

TEST(RuleParser_Parse_WithEMPTYReplacement) {
  Rule rule = RuleParser::parse("delete -> EMPTY");

  CHECK_EQUAL("delete", rule.getPattern());
  CHECK_EQUAL("", rule.getReplacement());
}

TEST(RuleParser_Parse_WithSpaces) {
  Rule rule = RuleParser::parse("  hello   ->   world   final  ");

  CHECK_EQUAL("hello", rule.getPattern());
  CHECK_EQUAL("world", rule.getReplacement());
  CHECK_EQUAL(true, rule.isFinal());
}

TEST(RuleParser_Parse_InvalidFormat_MissingArrow) {
  CHECK_THROW(RuleParser::parse("invalid rule"), std::runtime_error);
}

TEST(RuleParser_Parse_EmptyString) {
  CHECK_THROW(RuleParser::parse(""), std::invalid_argument);
}

TEST(RuleParser_TryParse_Success) {
  Rule rule;
  std::string error;

  bool result = RuleParser::tryParse("a -> b final", rule, error);

  CHECK(result);
  CHECK_EQUAL("a", rule.getPattern());
  CHECK_EQUAL("b", rule.getReplacement());
  CHECK_EQUAL(true, rule.isFinal());
  CHECK_EQUAL("", error);
}

TEST(RuleParser_TryParse_Failure) {
  Rule rule;
  std::string error;

  bool result = RuleParser::tryParse("invalid", rule, error);

  CHECK(!result);
  CHECK(!error.empty());
}

TEST(RuleParser_IsValidFormat_Valid) {
  CHECK(RuleParser::isValidFormat("a -> b"));
  CHECK(RuleParser::isValidFormat("-> start"));
  CHECK(RuleParser::isValidFormat("remove ->"));
}

TEST(RuleParser_IsValidFormat_Invalid) {
  CHECK(!RuleParser::isValidFormat(""));
  CHECK(!RuleParser::isValidFormat("no arrow here"));
}

TEST(MarkovAlgorithm_Constructor_Default) {
  MarkovAlgorithm algo;

  CHECK_EQUAL("", algo.getCurrentTape());
  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_AssignmentOperator) {
  MarkovAlgorithm algo1;
  algo1.addRule(Rule("a", "b", false));
  MarkovTape tape("test");
  algo1.setTape(tape);

  MarkovAlgorithm algo2;
  algo2 = algo1;

  CHECK_EQUAL("test", algo2.getCurrentTape());
  CHECK_EQUAL(false, algo2.isHalted());
}

TEST(MarkovAlgorithm_AddRule) {
  MarkovAlgorithm algo;
  Rule rule("a", "b", true);

  algo.addRule(rule);

  MarkovTape tape("a");
  algo.setTape(tape);
  algo.step();

  CHECK_EQUAL("b", algo.getCurrentTape());
  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_SetTape) {
  MarkovAlgorithm algo;
  MarkovTape tape("input string");

  algo.setTape(tape);

  CHECK_EQUAL("input string", algo.getCurrentTape());
  CHECK_EQUAL(false, algo.isHalted());
}

TEST(MarkovAlgorithm_LoadRulesFromStream_ValidRules) {
  MarkovAlgorithm algo;
  std::istringstream stream("a -> b\nb -> c final\n-> start");

  algo.loadRulesFromStream(stream);

  MarkovTape tape("a");
  algo.setTape(tape);
  algo.run();

  CHECK_EQUAL("c", algo.getCurrentTape());
}

TEST(MarkovAlgorithm_LoadRulesFromStream_InvalidRules) {
  MarkovAlgorithm algo;
  std::istringstream stream("a -> b\ninvalid rule\nc -> d");

  CHECK_THROW(algo.loadRulesFromStream(stream), std::runtime_error);
  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_LoadRulesFromStream_EmptyStream) {
  MarkovAlgorithm algo;
  std::istringstream stream("");

  algo.loadRulesFromStream(stream);

  MarkovTape tape("test");
  algo.setTape(tape);
  algo.step();

  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_Step_BasicReplacement) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("a", "b", false));
  MarkovTape tape("abc");
  algo.setTape(tape);

  algo.step();

  CHECK_EQUAL("bbc", algo.getCurrentTape());
  CHECK_EQUAL(false, algo.isHalted());
}

TEST(MarkovAlgorithm_Step_FinalRule) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("a", "b", true));
  MarkovTape tape("abc");
  algo.setTape(tape);

  algo.step();

  CHECK_EQUAL("bbc", algo.getCurrentTape());
  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_Step_NoApplicableRule) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("x", "y", false));
  MarkovTape tape("abc");
  algo.setTape(tape);

  algo.step();

  CHECK_EQUAL("abc", algo.getCurrentTape());
  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_Step_EmptyPattern) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("", "start", false));
  MarkovTape tape("content");
  algo.setTape(tape);

  algo.step();

  CHECK_EQUAL("startcontent", algo.getCurrentTape());
}

TEST(MarkovAlgorithm_Step_MultipleRules_FirstApplicable) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("aa", "x", false));
  algo.addRule(Rule("a", "y", false));
  MarkovTape tape("aaa");
  algo.setTape(tape);

  algo.step();
  CHECK_EQUAL("xa", algo.getCurrentTape());
  algo.step();
  CHECK_EQUAL("xy", algo.getCurrentTape());
}

TEST(MarkovAlgorithm_Step_MaxIterations) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("a", "aa", false));
  MarkovTape tape("a");
  algo.setTape(tape);

  while (!algo.isHalted()) {
    algo.step();
  }

  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_Run_SimpleCase) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("a", "b", false));
  algo.addRule(Rule("b", "c", false));
  MarkovTape tape("a");
  algo.setTape(tape);

  algo.run();

  CHECK_EQUAL("c", algo.getCurrentTape());
  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_Run_WithFinalRule) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("a", "b", true));
  algo.addRule(Rule("b", "c", false));
  MarkovTape tape("a");
  algo.setTape(tape);

  algo.run();

  CHECK_EQUAL("b", algo.getCurrentTape());
}

TEST(MarkovAlgorithm_Run_EmptyTape) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("a", "b", false));
  MarkovTape tape("");
  algo.setTape(tape);

  algo.run();

  CHECK_EQUAL("", algo.getCurrentTape());
  CHECK_EQUAL(true, algo.isHalted());
}

TEST(MarkovAlgorithm_Reset) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("a", "b", false));
  MarkovTape tape("a");
  algo.setTape(tape);
  algo.step();  // Выполняем один шаг

  algo.reset();

  CHECK_EQUAL(false, algo.isHalted());
  CHECK_EQUAL("b", algo.getCurrentTape());
}

TEST(MarkovAlgorithm_ComplexScenario) {
  MarkovAlgorithm algo;
  algo.addRule(Rule("a", "", false));
  algo.addRule(Rule("", "[", false));
  algo.addRule(Rule("", "]", true));

  MarkovTape tape("abcada");
  algo.setTape(tape);
  algo.run();

  CHECK_EQUAL(true, algo.isHalted());
}

TEST(Integration_FileLoadingAndExecution) {
  MarkovAlgorithm algo;

  std::ofstream testFile("test_rules.txt");
  testFile << "a -> b\n";
  testFile << "b -> c final\n";
  testFile.close();

  std::ifstream file("test_rules.txt");
  algo.loadRulesFromStream(file);
  file.close();

  MarkovTape tape("aaa");
  algo.setTape(tape);
  algo.run();

  CHECK_EQUAL("cbb", algo.getCurrentTape());

  remove("test_rules.txt");
}

TEST(Integration_ManualRulesAndStepExecution) {
  MarkovAlgorithm algo;

  algo.addRule(Rule("1", "one", false));
  algo.addRule(Rule("2", "two", true));
  algo.addRule(Rule("3", "three", false));

  MarkovTape tape("123");
  algo.setTape(tape);

  algo.step();
  CHECK_EQUAL("one23", algo.getCurrentTape());

  algo.step();
  CHECK_EQUAL("onetwo3", algo.getCurrentTape());
  CHECK_EQUAL(true, algo.isHalted());

  algo.step();
  CHECK_EQUAL("onetwo3", algo.getCurrentTape());
}

TEST(Integration_ComplexTransformation) {
  MarkovAlgorithm algo;

  algo.addRule(Rule("0", "1", false));
  algo.addRule(Rule("1", "0", false));
  algo.addRule(Rule("", "result: ", true));

  MarkovTape tape("10110");
  algo.setTape(tape);
  algo.run();
  CHECK_EQUAL(true, algo.isHalted());
}

int main() { return UnitTest::RunAllTests(); }
