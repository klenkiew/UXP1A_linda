#ifndef LINDAAPI_H
#define LINDAAPI_H

#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/file.h>
#include <random>
#include <string>
#include "linda_api/ExclusiveFileAccessor.h"
#include "pipes/NamedPipe.h"


class TupleSpace
{
public:
    TupleSpace(const std::string &name);

    void reset();

    void linda_output(std::string tuple);
    std::string linda_input(std::string pattern, int timeout);
    std::string linda_read(std::string pattern, int timeout);

private:
    std::string tuple_space_name;
    std::string tuples_path;
    std::string templates_path;

    std::string get_home_dir_path();

    std::vector<std::pair<std::unique_ptr<TupleTemplate>, std::string>>
    get_template_with_fifo_pairs(const ExclusiveFileAccessor &file);

    std::vector<std::pair<std::unique_ptr<Tuple>, std::string>> get_tuples(const ExclusiveFileAccessor &tuples_file);
    void send_tuple(const std::string &tuple, const std::string &fifo);
    std::unique_ptr<TupleTemplate> get_parsed_tuple_template(const std::string &input);
    std::unique_ptr<Tuple> get_parsed_tuple(const std::string &input);
    std::string get_random_string(std::string::size_type length);
    std::string tuple_read_util(const std::string &pattern, int timeout, bool is_input);
};

#endif // LINDAAPI_H
