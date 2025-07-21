#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "unzip.h"

#include <windows.h>  // for FILE_ATTRIBUTE_DIRECTORY

#include "minizip/unzip.h"

static auto isDirectoryEntry(const char* filename,
                             const unz_file_info& fileInfo) -> bool
{
  size_t len = strlen(filename);
  if (len > 0 && (filename[len - 1] == '/' || filename[len - 1] == '\\')) {
    return true;
  }

  return (fileInfo.external_fa & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

auto unzip(const std::string& zipFilePath,
           const std::string& destinationDir) -> UNZIP_RESULT
{
  UNZIP_RESULT result;

  unzFile zipfile = unzOpen(zipFilePath.c_str());
  if (!zipfile) {
    std::cerr << "Cannot open zip file: " << zipFilePath << std::endl;
    result.success = false;
    return result;
  }

  if (unzGoToFirstFile(zipfile) != UNZ_OK) {
    std::cerr << "Cannot go to first file in zip." << std::endl;
    unzClose(zipfile);
    result.success = false;
    return result;
  }

  char filename[512];
  unz_file_info fileInfo;

  if (unzGetCurrentFileInfo(zipfile,
                            &fileInfo,
                            filename,
                            sizeof(filename),
                            nullptr,
                            0,
                            nullptr,
                            0)
      != UNZ_OK)
  {
    std::cerr << "Failed to get file info." << std::endl;
    unzClose(zipfile);
    result.success = false;
    return result;
  }

  std::string fullPath = destinationDir + "\\" + filename;
  result.full_path = fullPath;
  result.is_directory = isDirectoryEntry(filename, fileInfo);

  if (result.is_directory) {
    std::filesystem::create_directories(fullPath);
    result.success = true;
  } else {
    std::filesystem::create_directories(
        std::filesystem::path(fullPath).parent_path());

    if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
      std::cerr << "Cannot open entry in zip." << std::endl;
      unzClose(zipfile);
      result.success = false;
      return result;
    }

    std::ofstream outFile(fullPath, std::ios::binary);
    if (!outFile.is_open()) {
      std::cerr << "Cannot write to file: " << fullPath << std::endl;
      unzCloseCurrentFile(zipfile);
      unzClose(zipfile);
      result.success = false;
      return result;
    }

    const size_t bufferSize = 8192;
    std::vector<char> buffer(bufferSize);
    int bytesRead;
    while ((bytesRead = unzReadCurrentFile(zipfile, buffer.data(), bufferSize))
           > 0)
    {
      outFile.write(buffer.data(), bytesRead);
    }

    outFile.close();
    unzCloseCurrentFile(zipfile);
    result.success = true;
  }

  unzClose(zipfile);
  return result;
}
