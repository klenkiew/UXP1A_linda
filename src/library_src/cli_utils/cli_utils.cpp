#include <cli_utils/cli_utils.h>
#include <tuple_space/parsing/exceptions/ParseException.h>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <tuple_space/parsing/exceptions/TokenizerException.h>
#include <pipes/exceptions/NamedPipeException.h>
#include <pipes/exceptions/NamedPipeTimeoutException.h>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

void invoke_safe(const std::function<void()>& function)
{
    try
    {
        function();
    }
    catch (const EndOfFile& ex)
    {
        BOOST_LOG_TRIVIAL(error) << ex.what();
        std::cout << "Operation failed - an empty tuple was entered" << std::endl;
    }
    // exposing exception messages directly to the end user - not the best idea, but whatever
    catch (const TokenizerException& ex)
    {
        std::cout << "Operation failed - an error occurred during parsing a tuple: " << ex.what() << std::endl;
    }
    catch (const ParseException& ex)
    {
        std::cout << "Operation failed - an error occurred during parsing a tuple: " << ex.what() << std::endl;
    }
    catch (const NamedPipeTimeoutException& ex)
    {
        std::cout << "Operation failed - timeout" << std::endl;
    }
    catch (const NamedPipeException& ex)
    {
        std::cout << "Operation failed - an error occurred during pipe operations: " << ex.what() << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "Fatal error: " << ex.what() << std::endl;
    }
}

bool handle_arguments(int argc, char** argv, po::options_description options, po::variables_map& vm)
{
    po::options_description hidden;
    // parameters not linked to any options e.g. for
    // 'linda_output --timeout 5 (5) ("abc")' - (5) and ("abc") are free parameters
    hidden.add_options()
            ("free_params", po::value<std::vector<std::string>>()->composing());

    po::options_description all;
    all.add(options).add(hidden);

    po::positional_options_description positional;
    positional.add("free_params", -1);

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
        return false;
    }

    po::notify(vm);
    return true;
}

bool try_get_tuple_template(const po::variables_map& vm, std::string &tuple)
{
    if (!vm.count("free_params"))
    {
        std::cout << "Please give at least one tuple template as a free parameter." << std::endl;
        return false;
    }

    const std::vector<std::string> tuples = vm["free_params"].as<std::vector<std::string>>();
    if (tuples.size() > 1)
    {
        std::cout << "Too many free parameters. Please give exactly one tuple template." << std::endl;
        return false;
    }

    tuple = tuples[0];
    return true;
}

std::string get_tuple_space_name(const po::variables_map& vm, const std::string& default_value)
{
    std::string tuple_space_name;
    if (vm.count("tuple_space"))
        tuple_space_name = vm["tuple_space"].as<std::string>();
    else
    {
        tuple_space_name = default_value;
        BOOST_LOG_TRIVIAL(info) << "No tuple space name given. The default value: "
                                << default_tuple_space << " assumed";
    }
    return tuple_space_name;
}

logging::trivial::severity_level get_log_level_by_string(const std::string &logging_level)
{
    if (logging_level == "trace")
        return logging::trivial::trace;
    if (logging_level == "debug")
        return logging::trivial::debug;
    if (logging_level == "info")
        return logging::trivial::info;
    if (logging_level == "warning")
        return logging::trivial::warning;
    if (logging_level == "error")
        return logging::trivial::error;
    if (logging_level == "fatal")
        return logging::trivial::fatal;

    BOOST_LOG_TRIVIAL(warning) << "Invalid logging level option given. Assumed 'info' logging level.";
    return logging::trivial::info;
}

void set_log_level_internal(const logging::trivial::severity_level& level)
{
    logging::core::get()->set_filter
            (
                    logging::trivial::severity >= level
            );
}

void set_log_level(const boost::program_options::variables_map &vm)
{
    if (vm.count("verbose"))
    {
        set_log_level_internal(logging::trivial::trace);
        return;
    }

    if (!vm.count("log"))
    {
        // default log level - info
        set_log_level_internal(logging::trivial::info);
        return;
    }

    const std::string& logging_level = vm["log"].as<std::string>();
    set_log_level_internal(get_log_level_by_string(logging_level));
}
