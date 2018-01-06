#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include <string>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <thread>
#include "linda_api/LindaApi.h"

BOOST_AUTO_TEST_CASE(linda_api_test)
{
    for (int i = 0; i < 1; ++i) {
        std::thread read_thread([]() -> void {
            TupleSpace api("lol");
            BOOST_LOG_TRIVIAL(debug) << "linda_read";
            std::string out = api.linda_read("(integer:*)", 5);
            BOOST_LOG_TRIVIAL(debug) << "linda_read end: " << out;
            BOOST_CHECK_EQUAL(out, "(5)");
        });

        std::thread write_thread([]() -> void {
            TupleSpace api("lol");
            BOOST_LOG_TRIVIAL(debug) << "linda_write before sleep";
            //std::this_thread::sleep_for(std::chrono::seconds(1));
            BOOST_LOG_TRIVIAL(debug) << "linda_write";
            bool out = api.linda_output("(5)");
            BOOST_LOG_TRIVIAL(debug) << "linda_write end: " << out;
            BOOST_CHECK(out);
        });

        read_thread.join();
        write_thread.join();
    }
}

BOOST_AUTO_TEST_CASE(cleanup) {
    TupleSpace api_ll("lol");
    api_ll.reset();
}