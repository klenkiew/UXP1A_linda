#ifndef UXP1A_LINDA_CLI_UTILS_H
#define UXP1A_LINDA_CLI_UTILS_H

#include <functional>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

constexpr auto default_tuple_space = "test_space";

void invoke_safe(const std::function<void()>& function);
bool try_get_tuple_template(const boost::program_options::variables_map& vm, std::string &tuple);

std::string get_tuple_space_name(const boost::program_options::variables_map& vm,
                                 const std::string& default_value = default_tuple_space);

bool handle_arguments(int argc,
                      char** argv,
                      boost::program_options::options_description options,
                      boost::program_options::variables_map& vm);

#endif //UXP1A_LINDA_CLI_UTILS_H
