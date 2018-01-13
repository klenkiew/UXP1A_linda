#include <cli_utils/cli_utils.h>
#include <tuple_space/parsing/exceptions/ParseException.h>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <tuple_space/parsing/exceptions/TokenizerException.h>
#include <pipes/exceptions/NamedPipeException.h>

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
    catch (const NamedPipeException& ex)
    {
        std::cout << "Operation failed - an error occurred during pipe operations: " << ex.what() << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "Fatal error: " << ex.what() << std::endl;
    }
}