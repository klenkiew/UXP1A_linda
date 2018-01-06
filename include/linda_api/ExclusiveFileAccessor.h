#ifndef UXP1A_LINDA_EXCLUSIVEFILEACCESSOR_H
#define UXP1A_LINDA_EXCLUSIVEFILEACCESSOR_H

#include <string>
#include <fcntl.h>
#include <stdexcept>
#include <sys/file.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include "tuple_space/parsing/TupleParser.h"
#include "tuple_space/parsing/TupleTemplateElementParser.h"


class ExclusiveFileAccessor
{
public:
    /// Creates new file accessor with exclude access to it
    /// as long as the object live, the lock is maintained
    ExclusiveFileAccessor(const std::string &fn)
    {
        fd = open(fn.c_str(), O_RDWR);
        if (fd == -1)
            throw std::runtime_error("File open error");
        flock(fd, LOCK_EX);
    }

    ~ExclusiveFileAccessor()
    {
        flock(fd, LOCK_UN);
    }

    std::string read_whole_file() const
    {
        const size_t BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        std::string file_content;

        while (read(fd, buffer, BUFFER_SIZE) != 0)
        {
            file_content += buffer;
        }

        return file_content;
    }

    void append(const std::string &content) const
    {
        lseek(fd, 0, SEEK_END);
        write(fd, content.c_str(), content.size());
    }

    void rewrite_whole_file(const std::string &new_file_content) const
    {
        ftruncate(fd, 0);
        write(fd, new_file_content.c_str(), new_file_content.size());
    }

private:
    int fd;
};

#endif //UXP1A_LINDA_EXCLUSIVEFILEACCESSOR_H
