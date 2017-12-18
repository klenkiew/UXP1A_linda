#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include "parsing/TupleParser.h"
#include "parsing/exceptions/ParseException.h"

namespace
{
    std::unique_ptr<Tuple> get_parser_output(std::string input)
    {
        std::istringstream inputStream((input));
        TupleParser parser(std::unique_ptr<Scanner>(new Scanner(inputStream)));
        auto output = parser.parse();
        return output;
    }
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_integer)
{
    auto tuple = get_parser_output("(50)");
    BOOST_CHECK(tuple->get(0).get_int() == 50);
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_string)
{
    auto tuple = get_parser_output("(\"lol\")");
    BOOST_CHECK(tuple->get(0).get_string() == "lol");
}

BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_integer_and_string)
{
    auto tuple = get_parser_output("(10, \"abc\")");
    BOOST_CHECK(tuple->get(0).get_int() == 10);
    BOOST_CHECK(tuple->get(1).get_string() == "abc");
}

BOOST_AUTO_TEST_CASE(parser_throws_on_missing_comma_between_elements)
{
    BOOST_CHECK_THROW(get_parser_output("(10 \"abc\")"), ParseException);
}
