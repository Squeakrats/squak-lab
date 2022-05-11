#include "../regex.h"
#include <gtest/gtest.h>

using namespace regex;

TEST(regex, PlusOperator) {
  DFA dfa = DFA::FromNFA(regex::Create("a+"));

  EXPECT_EQ(dfa.Match("a"), true);
  EXPECT_EQ(dfa.Match("aa"), true);
  EXPECT_EQ(dfa.Match("aaa"), true);
  EXPECT_EQ(dfa.Match("aaab"), false);
  EXPECT_EQ(dfa.Match(""), false);
}

TEST(regex, StarOperator) {
  DFA dfa = DFA::FromNFA(regex::Create("ba*"));

  EXPECT_EQ(dfa.Match("b"), true);
  EXPECT_EQ(dfa.Match("ba"), true);
  EXPECT_EQ(dfa.Match("baa"), true);
  EXPECT_EQ(dfa.Match("bb"), false);
  EXPECT_EQ(dfa.Match(""), false);
}

TEST(regex, OptionalOperator) {
  DFA dfa = DFA::FromNFA(regex::Create("a?c"));

  EXPECT_EQ(dfa.Match("ac"), true);
  EXPECT_EQ(dfa.Match("c"), true);
  EXPECT_EQ(dfa.Match("bc"), false);
  EXPECT_EQ(dfa.Match("ad"), false);
}

TEST(regex, CharacterClass) {
  DFA dfa = DFA::FromNFA(regex::Create("[abc]"));

  EXPECT_EQ(dfa.Match("a"), true);
  EXPECT_EQ(dfa.Match("b"), true);
  EXPECT_EQ(dfa.Match("c"), true);
  EXPECT_EQ(dfa.Match("d"), false);
}

TEST(regex, Number) {
  DFA dfa = DFA::FromNFA(regex::Create("[0123456789]+.?[0123456789]+"));

  EXPECT_EQ(dfa.Match("0123"), true);
  EXPECT_EQ(dfa.Match("434"), true);
  EXPECT_EQ(dfa.Match("433.232"), true);
  EXPECT_EQ(dfa.Match("32"), true);
  EXPECT_EQ(dfa.Match("32.a"), false);
}

TEST(regex, Negate) {
  DFA dfa = DFA::FromNFA(regex::Create("[^abc]"));

  EXPECT_EQ(dfa.Match("d"), true);
  EXPECT_EQ(dfa.Match("e"), true);
  EXPECT_EQ(dfa.Match("f"), true);
  EXPECT_EQ(dfa.Match("a"), false);
  EXPECT_EQ(dfa.Match("b"), false);
  EXPECT_EQ(dfa.Match("c"), false);
}

TEST(regex, Merge) {
  DFA dfa =
    DFA::FromNFA(regex::Create(std::vector<std::string>{ "true", "false" }));

  std::stringstream source("truefalse");
  EXPECT_EQ(dfa.Longest(source).second, 1);
  EXPECT_EQ(dfa.Longest(source).second, 2);
}

TEST(regex, Longest) {
  DFA dfa = DFA::FromNFA(
    regex::Create(std::vector<std::string>{ "helloWorld", "hello" }));

  std::stringstream source("helloWorldhelloWo");

  auto longest = dfa.Longest(source);
  EXPECT_EQ(longest.second, 1);
  EXPECT_EQ(longest.first, "helloWorld");

  longest = dfa.Longest(source);
  EXPECT_EQ(longest.second, 2);
  EXPECT_EQ(longest.first, "hello");
}