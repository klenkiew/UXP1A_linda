#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "tuple_space/parsing/TupleParser.h"
#include "tuple_space/parsing/TupleTemplateElementParser.h"
#include "tuple_space/parsing/exceptions/ParseException.h"

namespace
{
    std::unique_ptr<TupleTemplateElement> get_parsed_tuple_template_element(const std::string &input)
    {
        std::istringstream inputStream(input);
        TupleTemplateElementParser parser(std::make_unique<Scanner>(inputStream));
        auto output = parser.parse_tuple_template_element();
        return output;
    }

    std::unique_ptr<TupleTemplate> get_parsed_tuple_template(const std::string &input)
    {
        std::istringstream inputStream((input));
        TupleTemplateElementParser parser(std::make_unique<Scanner>(inputStream));
        auto output = parser.parse();
        return output;
    }

    std::unique_ptr<Tuple> get_parsed_tuple(const std::string &input)
    {
        std::istringstream inputStream((input));
        TupleParser parser(std::make_unique<Scanner>(inputStream));
        auto output = parser.parse();
        return output;
    }
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_template_with_integer)
{
    auto tuple_template = get_parsed_tuple_template_element("integer:*");
    BOOST_TEST(tuple_template->matches(50));
    BOOST_TEST(!tuple_template->matches(std::string("50")));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_template_with_string)
{
    auto tuple_template = get_parsed_tuple_template_element("string:*");
    BOOST_TEST(tuple_template->matches(std::string("lololo")));
    BOOST_TEST(!tuple_template->matches(50));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_template_with_operator)
{
    auto tuple_template = get_parsed_tuple_template_element("integer:>6");
    BOOST_TEST(tuple_template->matches(7));
    BOOST_TEST(!tuple_template->matches(6));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_string_template_with_operator)
{
    auto tuple_template = get_parsed_tuple_template_element("string:<=\"abc\"");
    BOOST_TEST(!tuple_template->matches(std::string("abd")));
    BOOST_TEST(tuple_template->matches(std::string("abb")));
    BOOST_TEST(tuple_template->matches(std::string("abc")));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_string_template_with_wildcard)
{
    auto tuple_template = get_parsed_tuple_template_element("string:>\"abc*\"");
    BOOST_TEST(tuple_template->matches(std::string("abcasdasdasdasd")));
    BOOST_TEST(!tuple_template->matches(std::string("abbdfgdfgdfg")));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_template)
{
    auto tuple_template = get_parsed_tuple_template("(integer:>0)");
    // too lazy to create the tuple manually...
    auto tuple = get_parsed_tuple("(1, 2)");
    auto tuple_matches = get_parsed_tuple("(1)");
    BOOST_TEST(!tuple_template->matches(*tuple));
    BOOST_TEST(tuple_template->matches(*tuple_matches));
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_template_with_multiple_elements)
{
    auto tuple_template = get_parsed_tuple_template("(string:\"aa*b\", integer:>0)");

    auto tuple_matches = get_parsed_tuple("(\"aaab\", 2)");
    auto tuple_wrong_string = get_parsed_tuple("(\"aaaa\", 1)");
    auto tuple_one_element = get_parsed_tuple("(\"aaab\")");
    auto tuple_three_elements = get_parsed_tuple("(\"aaab\", 1, 2)");

    BOOST_TEST(tuple_template->matches(*tuple_matches));
    BOOST_TEST(!tuple_template->matches(*tuple_wrong_string));
    BOOST_TEST(!tuple_template->matches(*tuple_one_element));
    BOOST_TEST(!tuple_template->matches(*tuple_three_elements));
}

BOOST_AUTO_TEST_CASE(end_of_file_if_nothing_to_parse)
{
    std::string input = "(integer:*, string:\"lol\")\n"
                        "\n"
                        "\n"
                        "(integer:*, string:\"lol\")\n"
                        "\n";
    std::istringstream inputStream(input);
    TupleTemplateElementParser parser(std::make_unique<Scanner>(inputStream));

    // get 2 templates
    for (int i = 0; i < 2; ++i)
    {
        parser.parse();
    }

    // expect end of file
    BOOST_CHECK_THROW(parser.parse(), EndOfFile);
}
