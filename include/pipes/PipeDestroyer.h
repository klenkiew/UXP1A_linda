#ifndef UXP1A_LINDA_PIPEDESTROYER_H
#define UXP1A_LINDA_PIPEDESTROYER_H

#include <boost/log/trivial.hpp>
#include "NamedPipe.h"

class PipeDestroyer
{
public:
    explicit PipeDestroyer(NamedPipe &pipe) : pipe(pipe)
    {}

    virtual ~PipeDestroyer()
    {
        try
        {
            pipe.destroy();
        }
        // prevent throwing exceptions from destructor
        catch (const std::exception& e)
        {
            BOOST_LOG_TRIVIAL(error) << "Cannot destroy a pipe: " << e.what();
        }
        catch (...)
        {
            // non-exception thing thrown - ignore it
        }
    }

private:
    NamedPipe& pipe;
};

#endif //UXP1A_LINDA_PIPEDESTROYER_H
