#ifndef UXP1A_LINDA_SIGNALS_H
#define UXP1A_LINDA_SIGNALS_H

#include <csignal>

class Signals
{
public:
    typedef void signal_handler(int, siginfo_t*, void*);

    static void set_default_handler(int signal_number);
    static void set_signal_handler(signal_handler *handler, int signal_number);
};


#endif //UXP1A_LINDA_SIGNALS_H
