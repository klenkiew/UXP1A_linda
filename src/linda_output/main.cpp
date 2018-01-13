#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <linda_api/LindaApi.h>
#include <boost/log/trivial.hpp>
#include <cli_utils/cli_utils.h>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    po::options_description desc("Linda output - a program to place a tuple in a tuple space. Allowed options:");
    desc.add_options()
            ("help", "produce this help message")
            ("timeout", po::value<int>(), "specify a timeout for the operation (in seconds)")
            ("verbose", "set a verbose logging level");

    po::options_description hidden;
    hidden.add_options()
            ("tuples", po::value<std::vector<std::string>>()->composing());

    po::options_description all;
    all.add(desc).add(hidden);

    po::positional_options_description positional;
    positional.add("tuples", -1);
    po::variables_map vm;

    try
    {
        po::parsed_options parsed_options = po::command_line_parser(argc, argv).options(all)
                .positional(positional).allow_unregistered().run();
        po::store(parsed_options, vm);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Unrecognized option(s). Try '--help'." << std::endl;
        return 0;
    }
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    if (!vm.count("tuples"))
    {
        std::cout << "Please give at least one tuple as a free parameter." << std::endl;
        return 0;
    }

    const std::vector<std::string> tuples = vm["tuples"].as<std::vector<std::string>>();
    if (vm.count("timeout") || true)
    {
//        const int timeout = vm["timeout"].as<int>();
        invoke_safe([&tuples]() -> void
        {
            TupleSpace tuple_space("test_space");
            for (const auto &tuple : tuples)
                tuple_space.linda_output(tuple);
        });
    }
}