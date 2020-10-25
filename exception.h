#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>


class UnknownFileNameException : public std::exception {
public:
    explicit UnknownFileNameException(const std::string &filename)
          : msg(std::string("UnknownFileNameException: ") + filename + std::string(" not found"))
    {}

    virtual const char *what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};


class FileOpenErrorException : public std::exception {
public:
    explicit FileOpenErrorException(const std::string &filename)
          : msg(std::string("FileOpenErrorException: ") + std::string("failed to open ") + filename)
    {}

    virtual const char *what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};


class BadCodeException : public std::exception {
    //BadCodeException() : std::exception() {}

    virtual const char *what() const noexcept override {
        return "BadCodeException: archive is probably damaged";
    }
};

#endif // EXCEPTION_H
