#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>
#include <parsing/TupleTemplateElementParser.h>
#include <iostream>

namespace
{
    std::unique_ptr<TupleTemplateElement> get_parser_output(std::string input)
    {
        std::istringstream inputStream((input));
        TupleTemplateElementParser parser(std::unique_ptr<Scanner>(new Scanner(inputStream)));
        auto output = parser.parse();
        return output;
    }
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_template_with_integer)
{
    auto tuple_template = get_parser_output("integer:*");
    BOOST_CHECK(tuple_template->matches(50));
    BOOST_CHECK(!tuple_template->matches(std::string("50")));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_template_with_string)
{
    auto tuple_template = get_parser_output("string:*");
    BOOST_CHECK(tuple_template->matches(std::string("lololo")));
    BOOST_CHECK(!tuple_template->matches(50));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_template_with_operator)
{
    auto tuple_template = get_parser_output("integer:>6");
    BOOST_CHECK(tuple_template->matches(7));
    BOOST_CHECK(!tuple_template->matches(6));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_string_template_with_operator)
{
    auto tuple_template = get_parser_output("string:<=\"abc\"");
    BOOST_CHECK(!tuple_template->matches(std::string("abd")));
    BOOST_CHECK(tuple_template->matches(std::string("abb")));
    BOOST_CHECK(tuple_template->matches(std::string("abc")));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_string_template_with_wildcard)
{
    auto tuple_template = get_parser_output("string:>\"abc*\"");
    BOOST_CHECK(tuple_template->matches(std::string("abcasdasdasdasd")));
    BOOST_CHECK(!tuple_template->matches(std::string("abbdfgdfgdfg")));
}

