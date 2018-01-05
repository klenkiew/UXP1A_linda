#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include <string>
#include "tuple_space/parsing/ast/Operator.h"
#include "tuple_space/parsing/ast/PunctuationMark.h"

BOOST_AUTO_TEST_CASE(punctuation_mark_to_string)
{
    auto leftParenthesis = PunctuationMark::LeftParenthesis;
    auto rightParenthesis = PunctuationMark::RightParenthesis;
    auto comma = PunctuationMark::Comma;
    auto star = PunctuationMark::Star;
    auto colon = PunctuationMark::Colon;

    BOOST_TEST(to_string(leftParenthesis) == "(");
    BOOST_TEST(to_string(rightParenthesis) == ")");
    BOOST_TEST(to_string(comma) == ",");
    BOOST_TEST(to_string(star) == "*");
    BOOST_TEST(to_string(colon) == ":");
}

BOOST_AUTO_TEST_CASE(operator_to_string)
{
    auto equal = Operator::Equal;
    auto notEqual = Operator::NotEqual;
    auto less = Operator::Less;
    auto lessEqual = Operator::LessEqual;
    auto greater = Operator::Greater;
    auto greaterEqual = Operator::GreaterEqual;

    BOOST_TEST(to_string(equal) == "==");
    BOOST_TEST(to_string(notEqual) == "!=");
    BOOST_TEST(to_string(less) == "<");
    BOOST_TEST(to_string(lessEqual) == "<=");
    BOOST_TEST(to_string(greater) == ">");
    BOOST_TEST(to_string(greaterEqual) == ">=");
}
