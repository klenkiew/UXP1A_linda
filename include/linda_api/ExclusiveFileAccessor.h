#ifndef UXP1A_LINDA_EXCLUSIVEFILEACCESSOR_H
#define UXP1A_LINDA_EXCLUSIVEFILEACCESSOR_H

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <sys/file.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "tuple_space/parsing/TupleParser.h"
#include "tuple_space/parsing/TupleTemplateElementParser.h"


class ExclusiveFileAccessor
{
public:
    /// Creates new file accessor with exclude access to it
    /// as long as the object live, the lock is maintained
    ExclusiveFileAccessor(const std::string &fn)
        : filename(fn)
    {
        fd = open(filename.c_str(), O_RDWR | O_CREAT, 0777);
        if (fd == -1)
            throw std::runtime_error("File open error");

        BOOST_LOG_TRIVIAL(debug) << std::this_thread::get_id() << " pid try to lock " << filename;
        if (flock(fd, LOCK_EX) == -1) {
            throw std::runtime_error("File lock corruption! Errno: " + std::to_string(errno));
        }
        BOOST_LOG_TRIVIAL(debug) << std::this_thread::get_id() << " locked successfully " << filename;
    }

    ~ExclusiveFileAccessor()
    {
        flock(fd, LOCK_UN);
        BOOST_LOG_TRIVIAL(debug) << std::to_string(getpid()) << " unlocked successfully " << filename;
        close(fd);
    }

    void unlink() {
        flock(fd, LOCK_UN);
        close(fd);
        ::unlink(filename.c_str());
    }

    std::string read_whole_file() const
    {
        const size_t BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        std::string file_content;

        lseek(fd, 0, SEEK_SET);
        int char_read;
        while ( (char_read = read(fd, buffer, BUFFER_SIZE)) > 0)
        {
            file_content.append(buffer, char_read);
            BOOST_LOG_TRIVIAL(debug) << "READ: " << buffer;

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

    void erase(const std::string &content) const
    {
        std::string current = read_whole_file();
        auto pos = current.find(content);
        if (pos != std::string::npos) {
            current.erase(pos, content.length());
            rewrite_whole_file(current);
        }
    }

private:
    int fd;
    std::string filename;
};

#endif //UXP1A_LINDA_EXCLUSIVEFILEACCESSOR_H
