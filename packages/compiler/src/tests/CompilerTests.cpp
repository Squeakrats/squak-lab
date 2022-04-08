#include <gtest/gtest.h>
#include  "../NFA.h"
#include "../DFA.h"

TEST(Compiler, DFAFromNFA) {
	NFA a{};
	a.initialState = a.AddState();
	a.acceptingState = a.AddState();
	a.AddTransition(a.initialState, a.acceptingState, 'a');
	a.states.at(a.acceptingState).tag = 1;

	NFA b{};
	b.initialState = b.AddState();
	b.acceptingState = b.AddState();
	b.AddTransition(b.initialState, b.acceptingState, 'b');
	b.states.at(b.acceptingState).tag = 2;

	a.Union(std::move(b));

	DFA dfa = DFA::FromNFA(a);

	std::stringstream source("ab");

	auto longest = dfa.Longest(source);
	ASSERT_EQ(longest.first, "a");
	ASSERT_EQ(longest.second, 1);

	longest = dfa.Longest(source);
	ASSERT_EQ(longest.first, "b");
	ASSERT_EQ(longest.second, 2);
}