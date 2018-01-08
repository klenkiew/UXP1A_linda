#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include "tuple_space/parsing/TupleParser.h"
#include "tuple_space/parsing/exceptions/ParseException.h"

namespace
{
    std::unique_ptr<Tuple> get_parsed_tuple_element(const std::string &input)
    {
        std::istringstream inputStream((input));
        TupleParser parser(std::make_unique<Scanner>(inputStream));
        auto output = parser.parse();
        return output;
    }
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_integer)
{
    auto tuple = get_parsed_tuple_element("(50)");
    BOOST_TEST(tuple->get(0).get_int() == 50);
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_negative_integer)
{
    auto tuple = get_parsed_tuple_element("(-50)");
    BOOST_TEST(tuple->get(0).get_int() == -50);
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_negative_integer_with_spaces)
{
    auto tuple = get_parsed_tuple_element("(-   50)");
    BOOST_TEST(tuple->get(0).get_int() == -50);
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_doubly_negative_integer)
{
    // because why not?
    auto tuple = get_parsed_tuple_element("(--50)");
    BOOST_TEST(tuple->get(0).get_int() == 50);
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_triply_negative_integer)
{
    auto tuple = get_parsed_tuple_element("(---50)");
    BOOST_TEST(tuple->get(0).get_int() == -50);
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_string)
{
    auto tuple = get_parsed_tuple_element("(\"lol\")");
    BOOST_TEST(tuple->get(0).get_string() == "lol");
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_integer_and_string)
{
    auto tuple = get_parsed_tuple_element("(10, \"abc\")");
    BOOST_TEST(tuple->get(0).get_int() == 10);
    BOOST_TEST(tuple->get(1).get_string() == "abc");
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_negative_integer_and_string)
{
    auto tuple = get_parsed_tuple_element("(- - ---10, \"abc\")");
    BOOST_TEST(tuple->get(0).get_int() == -10);
    BOOST_TEST(tuple->get(1).get_string() == "abc");
}

BOOST_AUTO_TEST_CASE(parser_throws_on_missing_comma_between_elements)
{
    BOOST_CHECK_THROW(get_parsed_tuple_element("(10 \"abc\")"), ParseException);
}

BOOST_AUTO_TEST_CASE(parser_throws_on_empty_file_end_of_file)
{
    BOOST_CHECK_THROW(get_parsed_tuple_element(""), EndOfFile);
}

BOOST_AUTO_TEST_CASE(parser_throws_end_of_file_after_last_read_tuple)
{
    std::istringstream inputStream("(10 ,\"abc\")");
    TupleParser parser(std::make_unique<Scanner>(inputStream));
    parser.parse();
    BOOST_CHECK_THROW(parser.parse(), EndOfFile);
}
