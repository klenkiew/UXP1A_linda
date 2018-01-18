#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <linda_api/LindaApi.h>
#include <boost/log/trivial.hpp>
#include <cli_utils/cli_utils.h>
#include <climits>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    po::options_description desc("Linda read - a program to read a tuple from a tuple space. Allowed options:");
    desc.add_options()
            ("help,h", "produce this help message")
            ("timeout,t", po::value<int>(), "specify a timeout for the operation (in seconds)")
            ("log,l", po::value<std::string>(),
                 "set the logging level, possible values: ['trace', 'debug', 'info', 'warning', 'error', 'fatal'")
            ("verbose,v", "use a verbose logging level (equivalent to '--log trace'")
            ("tuple_space,s", po::value<std::string>(), "specify the name of the tuple space which is to be used");

    po::variables_map vm;
    if (!handle_arguments(argc, argv, desc, vm))
        return 0;

    set_log_level(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    std::string tuple;
    if (!try_get_tuple_template(vm, tuple))
        return 0;

    const std::string& tuple_space_name = get_tuple_space_name(vm);

    if (vm.count("timeout"))
    {
        const int timeout = vm["timeout"].as<int>();
        invoke_safe([&tuple, timeout, &tuple_space_name]() -> void
        {
            TupleSpace tuple_space(tuple_space_name);
            std::cout << "Received tuple: " << tuple_space.linda_read(tuple, timeout) << std::endl;
        });
    }
    else
    {
        invoke_safe([&tuple, &tuple_space_name]() -> void
        {
            TupleSpace tuple_space(tuple_space_name);
            // TODO should be a version of linda_read without the 'timeout' argument
            std::cout << "Received tuple: " << tuple_space.linda_read(tuple, INT_MAX) << std::endl;
        });
    }
}