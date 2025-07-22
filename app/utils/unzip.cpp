#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "unzip.h"

#include <minizip/unzip.h>

UNZIP_RESULT unzip(const std::string& zipFilePath,
                   const std::string& destinationPath)
{
  UNZIP_RESULT result {true, ""};

  unzFile zipfile = unzOpen(zipFilePath.c_str());
  if (!zipfile) {
    result.success = false;
    result.errorMessage = "Cannot open zip file: " + zipFilePath;
    return result;
  }

  if (unzGoToFirstFile(zipfile) != UNZ_OK) {
    result.success = false;
    result.errorMessage = "Cannot locate first file in archive.";
    unzClose(zipfile);
    return result;
  }

  do {
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
      result.success = false;
      result.errorMessage = "Failed to get file info.";
      unzClose(zipfile);
      return result;
    }

    std::string fullPath = destinationPath + "\\" + filename;

    // Handle directories
    if (filename[strlen(filename) - 1] == '/'
        || filename[strlen(filename) - 1] == '\\')
    {
      std::error_code ec;
      std::filesystem::create_directories(fullPath, ec);
      if (ec) {
        result.success = false;
        result.errorMessage = "Failed to create directory: " + fullPath;
        unzClose(zipfile);
        return result;
      }
      continue;
    }

    // Ensure parent directories exist
    std::error_code ec;
    std::filesystem::create_directories(
        std::filesystem::path(fullPath).parent_path(), ec);
    if (ec) {
      result.success = false;
      result.errorMessage =
          "Failed to create parent directories for: " + fullPath;
      unzClose(zipfile);
      return result;
    }

    if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
      result.success = false;
      result.errorMessage =
          "Cannot open file in archive: " + std::string(filename);
      unzClose(zipfile);
      return result;
    }

    std::ofstream outFile(fullPath, std::ios::binary);
    if (!outFile.is_open()) {
      result.success = false;
      result.errorMessage = "Cannot write to file: " + fullPath;
      unzCloseCurrentFile(zipfile);
      unzClose(zipfile);
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

  } while (unzGoToNextFile(zipfile) == UNZ_OK);

  unzClose(zipfile);
  return result;
}
