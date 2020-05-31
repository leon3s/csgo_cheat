#include "Logger.h"

#include <iostream>

Logger::Logger(char *file_path) {
  TCHAR **lppPart = { NULL };
  TCHAR file_full_path[MAX_PATH] = TEXT("");
  unsigned int file_full_path_len;

  file_full_path_len = 0;
  if (!GetFullPathName(file_path, MAX_PATH, file_full_path, lppPart)) {
    throw "[!]Fail to get Full path";
  }
  this->fd = CreateFileA(
    file_full_path,    // name of the file
    FILE_APPEND_DATA, // open for writing
    FILE_SHARE_READ,             // sharing mode
    NULL,             // use default security descriptor
    CREATE_ALWAYS, // overwrite if exists
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );
  file_full_path_len = strlen(file_full_path);
  this->file_path = (char *)malloc(sizeof(&this->file_path) * file_full_path_len);
  memcpy(this->file_path, file_full_path, file_full_path_len);
}

Logger::~Logger() {
  CloseHandle(this->fd);
  free(this->file_path);
  this->file_path = nullptr;
}

void Logger::writeLog(const char *format, ...) {
  DWORD   dwBytesWritten;
  char    msg[BUFFER_SIZE];
  va_list args;

  va_start(args, format);
  vsnprintf(msg, sizeof(msg), format, args);
  va_end(args);
  SetFilePointer(this->fd, 0, NULL, FILE_END);
  WriteFile(this->fd, msg, strlen(msg), &dwBytesWritten, NULL);
}
