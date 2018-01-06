#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include <string>
#include <iostream>
#include "linda_api/LindaApi.h"

BOOST_AUTO_TEST_CASE(linda_api_test)
{
    TupleSpace api("lol");
    api.reset();
    api.linda_output("(5)");

}
