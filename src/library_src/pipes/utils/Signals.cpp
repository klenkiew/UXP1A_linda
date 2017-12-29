#include "pipes/utils/Signals.h"
#include <stdexcept>

void Signals::set_signal_handler(signal_handler *handler, int signal_number)
{
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(signal_number, &sa, nullptr) == -1)
        throw std::runtime_error("sigaction");
}

void Signals::set_default_handler(int signal_number)
{
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);

    if (sigaction(signal_number, &sa, nullptr) == -1)
        throw std::runtime_error("sigaction");
}