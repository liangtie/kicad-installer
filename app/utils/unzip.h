#ifndef UNZIP_H
#define UNZIP_H

#include <string>

struct UNZIP_RESULT
{
  bool success {};
  bool is_directory {};
  std::string full_path {};
};

UNZIP_RESULT unzip(const std::string& zipFilePath,
                   const std::string& destinationDir);

#endif
