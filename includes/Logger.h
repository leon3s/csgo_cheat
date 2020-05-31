#ifndef LOGGER_H
#define LOGGER_H

#define BUFFER_SIZE 4086

#include <Windows.h>

using namespace std;

class Logger {
  public:
    HANDLE fd;
    char *file_path;
    Logger(char *file_path);
    ~Logger();
    void  writeLog(const char *format, ...);
};


#endif
