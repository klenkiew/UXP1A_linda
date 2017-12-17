#define BOOST_TEST_NO_LIB

#include <boost/test/unit_test.hpp>
#include "lol.hpp"

BOOST_AUTO_TEST_CASE(first_test)
{
fun();
  int i = 1;
  BOOST_CHECK(i);
  BOOST_CHECK(i == 2);
}
