#ifndef UXP1A_LINDA_SYSTEMTIMER_H
#define UXP1A_LINDA_SYSTEMTIMER_H

#include "Signals.h"
#include <csignal>
#include <stdexcept>
#include <chrono>

class SystemTimer
{
public:
    SystemTimer(const std::string &name, int signal_number);
    virtual ~SystemTimer();

    void start(std::chrono::duration<double> duration);
    void start(int seconds);
    void stop();

private:

    timer_t timer_id;
    std::string timer_name;

    void init_timer(const std::string &name, int signal_number);
};


#endif //UXP1A_LINDA_SYSTEMTIMER_H
