#define BOOST_TEST_NO_LIB

#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/random.hpp>
#include "pipes/NamedPipe.h"
#include <string>
#include <thread>
#include <pipes/exceptions/NamedPipeTimeoutException.h>

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
    std::cout << "pipe path: " + pipe_path << std::endl;
    std::thread write_thread([&]() -> void
                             {
                                 NamedPipe pipe(pipe_path);
                                 pipe.createIfNotExists();
                                 std::cout << "pipe created" << std::endl;
                                 pipe.open(NamedPipe::Mode::Write);
                                 std::cout << "pipe opened for write" << std::endl;
                                 pipe.write("pipe_test");
                                 pipe.close();
                             });
    std::thread read_thread([&]() -> void
                            {
                                NamedPipe pipe(pipe_path);
                                PipeDestroyer pipeDestroyer(pipe);
                                pipe.createIfNotExists();
                                std::cout << "pipe created" << std::endl;
                                pipe.open(NamedPipe::Mode::Read);
                                std::cout << "pipe opened for read" << std::endl;
                                std::string read = pipe.read();
                                pipe.close();
                                std::cout << "read from pipe: " + read << std::endl;
                                BOOST_CHECK(read == "pipe_test");
                            });
    write_thread.join();
    read_thread.join();
}

BOOST_AUTO_TEST_CASE(pipe_read_timeout_works)
{
    const std::string pipe_path = get_random_pipe_path();
    std::cout << "pipe path: " + pipe_path << std::endl;
    std::thread write_thread([&]() -> void
                             {
                                 NamedPipe pipe(pipe_path);
                                 pipe.createIfNotExists();
                                 std::cout << "pipe created" << std::endl;
                                 pipe.open(NamedPipe::Mode::Write);
                                 std::cout << "pipe opened for write" << std::endl;
                                 sleep(3);
                             });
    std::thread read_thread([&]() -> void
                            {
                                NamedPipe pipe(pipe_path);
                                PipeDestroyer pipeDestroyer(pipe);
                                pipe.createIfNotExists();
                                std::cout << "pipe created" << std::endl;
                                pipe.open(NamedPipe::Mode::Read);
                                std::cout << "pipe opened for read" << std::endl;
                                BOOST_CHECK_THROW(pipe.read(2), NamedPipeTimeoutException);
                                std::cout << "read timed out" << std::endl;
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
    std::cout << "pipe created" << std::endl;
    BOOST_CHECK_THROW(pipe.open(NamedPipe::Mode::Read, 2), NamedPipeTimeoutException);
}