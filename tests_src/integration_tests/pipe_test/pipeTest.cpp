#define BOOST_TEST_NO_LIB

#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/random.hpp>
#include "pipes/NamedPipe.h"
#include <string>
#include <thread>
#include <pipes/exceptions/NamedPipeTimeoutException.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace
{
    class PipeDestroyer
    {
    public:
        PipeDestroyer(NamedPipe &pipe) : pipe(pipe)
        {}

        virtual ~PipeDestroyer()
        {
            pipe.destroy();
        }

    private:
        NamedPipe& pipe;
    };

    std::string get_random_pipe_path()
    {
        namespace fs = boost::filesystem;
        boost::random::mt19937 rng;
        boost::random::uniform_int_distribution<> int_distribution(1, static_cast<int>(1e10));
        return (fs::temp_directory_path()
                / fs::path("test_pipe_" + std::to_string(int_distribution(rng)))).string();
    }
}

BOOST_AUTO_TEST_CASE(pipe_works)
{
    const std::string pipe_path = get_random_pipe_path();
    BOOST_LOG_TRIVIAL(debug) << "Pipe path: " + pipe_path;
    std::thread write_thread([&]() -> void
                             {
                                 NamedPipe pipe(pipe_path);
                                 pipe.createIfNotExists();
                                 BOOST_LOG_TRIVIAL(debug) << "Pipe created";
                                 pipe.open(NamedPipe::Mode::Write);
                                 BOOST_LOG_TRIVIAL(debug) << "Pipe opened for write";
                                 pipe.write("pipe_test");
                                 pipe.close();
                             });
    std::thread read_thread([&]() -> void
                            {
                                NamedPipe pipe(pipe_path);
                                PipeDestroyer pipeDestroyer(pipe);
                                pipe.createIfNotExists();
                                BOOST_LOG_TRIVIAL(debug) << "Pipe created";
                                pipe.open(NamedPipe::Mode::Read);
                                BOOST_LOG_TRIVIAL(debug) << "Pipe opened for read";
                                std::string read = pipe.read();
                                pipe.close();
                                BOOST_LOG_TRIVIAL(debug) << "String read from pipe: " + read;
                                BOOST_TEST(read == "pipe_test");
                            });
    write_thread.join();
    read_thread.join();
}

BOOST_AUTO_TEST_CASE(pipe_read_timeout_works)
{
    const std::string pipe_path = get_random_pipe_path();
    BOOST_LOG_TRIVIAL(debug) << "pipe path: " + pipe_path;
    std::thread write_thread([&]() -> void
                             {
                                 NamedPipe pipe(pipe_path);
                                 pipe.createIfNotExists();
                                 BOOST_LOG_TRIVIAL(debug) << "pipe created";
                                 pipe.open(NamedPipe::Mode::Write);
                                 BOOST_LOG_TRIVIAL(debug) << "pipe opened for write";
                                 sleep(3);
                             });
    std::thread read_thread([&]() -> void
                            {
                                NamedPipe pipe(pipe_path);
                                PipeDestroyer pipeDestroyer(pipe);
                                pipe.createIfNotExists();
                                BOOST_LOG_TRIVIAL(debug) << "pipe created";
                                pipe.open(NamedPipe::Mode::Read);
                                BOOST_LOG_TRIVIAL(debug) << "pipe opened for read";
                                BOOST_CHECK_THROW(pipe.read(2), NamedPipeTimeoutException);
                                BOOST_LOG_TRIVIAL(debug) << "read timed out";
                            });
    write_thread.join();
    read_thread.join();
}

BOOST_AUTO_TEST_CASE(pipe_works_with_timeout)
{
    const std::string pipe_path = get_random_pipe_path();
    NamedPipe pipe(pipe_path);
    PipeDestroyer pipeDestroyer(pipe);
    pipe.createIfNotExists();
    BOOST_LOG_TRIVIAL(debug) << "pipe created";
    BOOST_CHECK_THROW(pipe.open(NamedPipe::Mode::Read, 2), NamedPipeTimeoutException);
}