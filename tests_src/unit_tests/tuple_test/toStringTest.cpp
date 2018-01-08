#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include <tuple_space/TupleElement.h>
#include <tuple_space/Tuple.h>
#include <tuple_space/RequiredTypeTemplate.h>
#include <tuple_space/IntegerComparerTemplate.h>
#include <tuple_space/StringComparerTemplate.h>
#include <tuple_space/TupleTemplate.h>
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

BOOST_AUTO_TEST_CASE(tuple_element_to_string)
{
    TupleElement integer = 123;
    TupleElement string("abc789");

    BOOST_TEST(integer.to_string() == "123");
    BOOST_TEST(string.to_string() == "abc789");
}

BOOST_AUTO_TEST_CASE(tuple_with_one_element_to_string)
{
    std::vector<std::unique_ptr<TupleElement>> elements;
    elements.emplace_back(new TupleElement(123));
    Tuple tuple(std::move(elements));

    BOOST_TEST(tuple.to_string() == "(123)");
}

BOOST_AUTO_TEST_CASE(tuple_with_negative_integer_to_string)
{
    std::vector<std::unique_ptr<TupleElement>> elements;
    elements.emplace_back(new TupleElement(-123));
    Tuple tuple(std::move(elements));

    BOOST_TEST(tuple.to_string() == "(-123)");
}

BOOST_AUTO_TEST_CASE(tuple_with_two_elements_to_string)
{
    std::vector<std::unique_ptr<TupleElement>> elements;
    elements.emplace_back(new TupleElement(123));
    elements.emplace_back(new TupleElement("abc789"));
    Tuple tuple(std::move(elements));

    BOOST_TEST(tuple.to_string() == "(123, abc789)");
}

BOOST_AUTO_TEST_CASE(required_type_template_element_to_string)
{
    RequiredTypeTemplate required_integer(TupleElement::Type::Integer);
    RequiredTypeTemplate required_string(TupleElement::Type::String);

    BOOST_TEST(required_integer.to_string() == "integer:*");
    BOOST_TEST(required_string.to_string() == "string:*");
}

BOOST_AUTO_TEST_CASE(integer_comparer_template_element_to_string)
{
    IntegerComparerTemplate greater_than(Operator::Greater, 5);
    IntegerComparerTemplate less_equal(Operator::LessEqual, 8);
    IntegerComparerTemplate equal(Operator::Equal, 111);

    BOOST_TEST(greater_than.to_string() == "integer:>5");
    BOOST_TEST(less_equal.to_string() == "integer:<=8");
    BOOST_TEST(equal.to_string() == "integer:111");
}

BOOST_AUTO_TEST_CASE(string_comparer_template_element_to_string)
{
    StringComparerTemplate greater_than(Operator::Greater, "abc");
    StringComparerTemplate less_equal(Operator::LessEqual, "de*f");
    StringComparerTemplate equal(Operator::Equal, "xy*");

    BOOST_TEST(greater_than.to_string() == "string:>\"abc\"");
    BOOST_TEST(less_equal.to_string() == "string:<=\"de*f\"");
    BOOST_TEST(equal.to_string() == "string:\"xy*\"");
}

BOOST_AUTO_TEST_CASE(tuple_template_one_element_to_string)
{
    std::vector<std::unique_ptr<TupleTemplateElement>> templates_elements;

    templates_elements.emplace_back(new StringComparerTemplate(Operator::Equal, "xy*"));

    TupleTemplate tuple_template(std::move(templates_elements));

    BOOST_TEST(tuple_template.to_string() == "(string:\"xy*\")");
}

BOOST_AUTO_TEST_CASE(tuple_template_three_elements_to_string)
{
    StringComparerTemplate equal(Operator::Equal, "xy*");
    std::vector<std::unique_ptr<TupleTemplateElement>> templates_elements;

    templates_elements.emplace_back(new StringComparerTemplate(Operator::Equal, "xy*"));
    templates_elements.emplace_back(new IntegerComparerTemplate(Operator::GreaterEqual, 5));
    templates_elements.emplace_back(new StringComparerTemplate(Operator::NotEqual, "10"));

    TupleTemplate tuple_template(std::move(templates_elements));

    BOOST_TEST(tuple_template.to_string() == "(string:\"xy*\", integer:>=5, string:!=\"10\")");
}