#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include <string>
#include <pipes/utils/SystemTimer.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace
{
    bool flag = false;

    void timer_handler(int sig, siginfo_t *si, void *uc)
    {
        flag = true;
        BOOST_LOG_TRIVIAL(debug) << "I am timer " << (char *) si->si_value.sival_ptr;
    }
}

BOOST_AUTO_TEST_CASE(timer_works)
{
    Signals::set_signal_handler(timer_handler, SIGUSR1);
    auto timer = SystemTimer("test_timer", SIGUSR1);
    timer.start(3);
    sleep(5);
    BOOST_CHECK_EQUAL(flag, true);
}