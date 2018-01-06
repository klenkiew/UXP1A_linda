#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include <string>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <thread>
#include "linda_api/LindaApi.h"

BOOST_AUTO_TEST_CASE(linda_api_test_read)
{
    for(int i = 0; i < 10; i++) {
        std::thread write_thread([]() -> void {
            TupleSpace api("lol");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            BOOST_LOG_TRIVIAL(debug) << "START: linda_write";
            bool out1 = api.linda_output("(5)");
            bool out2 = api.linda_output("(6)");
            bool out3 = api.linda_output("(6,8)");
            BOOST_LOG_TRIVIAL(debug) << "END: linda_write end1: " << out1;
            BOOST_LOG_TRIVIAL(debug) << "END: linda_write end2: " << out2;
            BOOST_LOG_TRIVIAL(debug) << "END: linda_write end3: " << out3;
            BOOST_CHECK(out1 && out2 && out3);
        });


        TupleSpace api("lol");
        BOOST_LOG_TRIVIAL(debug) << "START: linda_read";
        std::string out = api.linda_read("(integer:==5)", 2);
        BOOST_LOG_TRIVIAL(debug) << "END: linda_read";
        BOOST_REQUIRE(!out.empty());
        BOOST_CHECK_EQUAL(out, "(5)");


        write_thread.join();
    }
}

BOOST_AUTO_TEST_CASE(linda_api_test_input)
{
    for(int i = 0; i < 10; i++) {
        std::thread write_thread([]() -> void {
            TupleSpace api("lol");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            BOOST_LOG_TRIVIAL(debug) << "START: linda_write";
            bool out1 = api.linda_output("(5)");
            bool out2 = api.linda_output("(6)");
            bool out3 = api.linda_output("(6,8)");
            BOOST_LOG_TRIVIAL(debug) << "END: linda_write end1: " << out1;
            BOOST_LOG_TRIVIAL(debug) << "END: linda_write end2: " << out2;
            BOOST_LOG_TRIVIAL(debug) << "END: linda_write end3: " << out3;
            BOOST_CHECK(out1 && out2 && out3);
        });


        TupleSpace api("lol");
        BOOST_LOG_TRIVIAL(debug) << "START: linda_read";
        // PROBLEM WITH LINDA INPUT
        // sometime it just throws "without an active exception", should be checked
        std::string out = api.linda_input("(integer:==5)", 2);
        BOOST_LOG_TRIVIAL(debug) << "END: linda_read";
        BOOST_REQUIRE(!out.empty());
        BOOST_CHECK_EQUAL(out, "(5)");


        write_thread.join();
    }
}

// if any linda api tests throws, this test ensures that the tuples space will be cleaned
BOOST_AUTO_TEST_CASE(cleanup) {
    TupleSpace api_ll("lol");
    api_ll.reset();
}