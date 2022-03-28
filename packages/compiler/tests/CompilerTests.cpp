#include <gtest/gtest.h>
#include <DFA.h>

TEST(CompilerTests, NFATests) {
	NFA nfa = NFA::FromRegularExpression("abc");
	nfa.Append(NFA::FromRegularExpression("def"));

	DFA dfa = DFA::FromNFA(nfa);

	EXPECT_EQ(dfa.Match("abcdef"), true);
}
