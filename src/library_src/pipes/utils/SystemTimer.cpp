#include "pipes/utils/SystemTimer.h"
#include <ctime>

SystemTimer::SystemTimer(const std::string &name, int signal_number)
{
    init_timer(name, signal_number);
}

void SystemTimer::start(std::chrono::duration<double> duration)
{
    struct itimerspec its;
    const auto interval_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    its.it_value.tv_sec = interval_ms.count()/1000;
    its.it_value.tv_nsec = (interval_ms.count()%1000)*1000;;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    if (timer_settime(this->timer_id, 0, &its, nullptr) == -1)
        throw std::runtime_error("timer_settime");
}

void SystemTimer::start(int seconds)
{
    start(std::chrono::duration<int>(std::chrono::seconds(seconds)));
}

void SystemTimer::stop()
{
    start(0);
}

void SystemTimer::init_timer(const std::string &name, int signal_number)
{
    struct sigevent te;
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = signal_number;
    te.sigev_value.sival_ptr = (void *) name.c_str();

    timer_t timer_id;

    if (timer_create(CLOCK_REALTIME, &te, &timer_id) == -1)
        throw std::runtime_error("timer_create");

    this->timer_id = timer_id;
}

SystemTimer::~SystemTimer()
{
    timer_delete(timer_id);
}
