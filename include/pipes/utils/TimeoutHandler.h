#ifndef UXP1A_LINDA_TIMEOUTHANDLER_H
#define UXP1A_LINDA_TIMEOUTHANDLER_H

#include <csignal>
#include "SystemTimer.h"

class TimeoutHandler
{
public:
    explicit TimeoutHandler(int seconds);
    ~TimeoutHandler();

    bool timed_out() const;

private:
    const static int signal_number = SIGUSR1;

    static bool timeout_flag;
    SystemTimer timer;

    static void timer_handler(int sig, siginfo_t *si, void *uc);
};


#endif //UXP1A_LINDA_TIMEOUTHANDLER_H
