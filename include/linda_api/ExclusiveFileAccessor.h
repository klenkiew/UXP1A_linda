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
#include "FileAccessException.hpp"
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
            throw FileAccessException("File open error");

        if (flock(fd, LOCK_EX) == -1) {
            throw FileAccessException("File lock corruption! Errno: " + std::to_string(errno));
        }
    }

    ~ExclusiveFileAccessor()
    {
        flock(fd, LOCK_UN);
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
        ssize_t char_read;
        while ( (char_read = read(fd, buffer, BUFFER_SIZE)) > 0)
        {
            file_content.append(buffer, char_read);
        }

        if (char_read == -1) {
            throw FileAccessException("File read error!");
        }

        return file_content;
    }

    void append(const std::string &content) const
    {
        lseek(fd, 0, SEEK_END);
        if (write(fd, content.c_str(), content.size()) == -1) {
            throw FileAccessException("File write error!");
        }
    }

    void rewrite_whole_file(const std::string &new_file_content) const
    {
        ftruncate(fd, 0);
        lseek(fd, 0, SEEK_SET);
        if (write(fd, new_file_content.c_str(), new_file_content.size()) == -1) {
            throw FileAccessException("File write error!");
        }

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
