#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/random.hpp>
#include "pipes/NamedPipe.h"
#include <string>
#include <thread>

namespace 
{
    std::string get_random_pipe_path()
    {
        namespace fs = boost::filesystem;
        boost::random::mt19937 rng;
        boost::random::uniform_int_distribution<> int_distribution(1, static_cast<int>(1e10));
        return (fs::temp_directory_path()
               / fs::path("test_pipe" + std::to_string(int_distribution(rng)))).string();
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
                                pipe.createIfNotExists();
                                std::cout << "pipe created" << std::endl;
                                pipe.open(NamedPipe::Mode::Read);
                                std::cout << "pipe opened for read" << std::endl;
                                std::string read = pipe.read();
                                pipe.close();
                                pipe.destroy();
                                std::cout << "read from pipe: " + read << std::endl;
                                BOOST_CHECK(read == "pipe_test");
                            });
    write_thread.join();
    read_thread.join();
}