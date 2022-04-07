#include <gtest/gtest.h>
#include  "../NFA.h"
#include "../DFA.h"

TEST(Compiler, DFAFromNFA) {
	NFA a{};
	a.initialState = a.AddState();
	a.acceptingState = a.AddState();
	a.AddTransition(a.initialState, a.acceptingState, 'a');

	NFA b{};
	b.initialState = b.AddState();
	b.acceptingState = b.AddState();
	b.AddTransition(b.initialState, b.acceptingState, 'b');

	a.Union(std::move(b));

	DFA dfa = DFA::FromNFA(a);

	ASSERT_EQ(dfa.Longest("a"), 1);
	ASSERT_EQ(dfa.Longest("b"), 1);
	ASSERT_EQ(dfa.Longest("c"), 0);
}