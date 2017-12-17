#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include "TupleParser.h"

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


BOOST_AUTO_TEST_CASE(parser_parses_tuple_with_integer_and_string)
{
    auto tuple = get_parser_output("(10, \"abc\")");
    BOOST_CHECK(tuple->get(0).get_int() == 10);
    BOOST_CHECK(tuple->get(1).get_string() == "abc");
}
