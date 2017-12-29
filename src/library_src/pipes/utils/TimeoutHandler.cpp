#include "pipes/utils/SystemTimer.h"
#include "pipes/utils/TimeoutHandler.h"

bool TimeoutHandler::timeout_flag = false;

TimeoutHandler::TimeoutHandler(int seconds): timer("TimeoutHandler timer", signal_number)
{
    Signals::set_signal_handler(TimeoutHandler::timer_handler, signal_number);
    TimeoutHandler::timeout_flag = false;
    timer.start(seconds);
}

void TimeoutHandler::timer_handler(int sig, siginfo_t *si, void *uc)
{
    TimeoutHandler::timeout_flag = true;
    Signals::set_default_handler(signal_number);
}

bool TimeoutHandler::timed_out() const
{
    return TimeoutHandler::timeout_flag;
}

TimeoutHandler::~TimeoutHandler()
{
    Signals::set_default_handler(signal_number);
}
