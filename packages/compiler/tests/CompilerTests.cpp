#include <gtest/gtest.h>
#include <NFA.h>

TEST(CompilerTests, NFATests) {
	NFA automata = NFA::FromRegularExpression("abc");

	EXPECT_EQ(automata.initialState, 0);
	EXPECT_EQ(automata.acceptingState, 3);
}
