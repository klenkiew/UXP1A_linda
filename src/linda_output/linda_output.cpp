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
            ("help,h", "produce this help message")
            ("verbose,v", "set a verbose logging level")
            ("tuple_space,s", po::value<std::string>(), "specify the name of the tuple space which is to be used");

    po::variables_map vm;
    if (!handle_arguments(argc, argv, desc, vm))
        return 0;

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    if (!vm.count("free_params"))
    {
        std::cout << "Please give at least one tuple as a free parameter." << std::endl;
        return 0;
    }

    const std::string& tuple_space_name = get_tuple_space_name(vm);
    const std::vector<std::string> tuples = vm["free_params"].as<std::vector<std::string>>();
    invoke_safe([&tuples, &tuple_space_name]() -> void
    {
        TupleSpace tuple_space(tuple_space_name);
        for (const auto &tuple : tuples)
        {
            tuple_space.linda_output(tuple);
            BOOST_LOG_TRIVIAL(info) << "Tuple " << tuple << " placed successfully.";
        }
    });
    std::cout << "Done." << std::endl;
}