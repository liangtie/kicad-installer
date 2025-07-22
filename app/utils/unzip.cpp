#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "unzip.h"

#include <minizip/unzip.h>

bool unzip(const std::string& zipFilePath, const std::string& destinationPath)
{
  unzFile zipfile = unzOpen(zipFilePath.c_str());
  if (!zipfile) {
    std::cerr << "Cannot open zip file: " << zipFilePath << std::endl;
    return false;
  }

  if (unzGoToFirstFile(zipfile) != UNZ_OK) {
    std::cerr << "Cannot locate first file in archive." << std::endl;
    unzClose(zipfile);
    return false;
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
      std::cerr << "Failed to get file info." << std::endl;
      unzClose(zipfile);
      return false;
    }

    std::string fullPath = destinationPath + "\\" + filename;

    // If it's a directory, create it
    if (filename[strlen(filename) - 1] == '/'
        || filename[strlen(filename) - 1] == '\\')
    {
      std::filesystem::create_directories(fullPath);
      continue;
    }

    // Ensure parent directories exist
    std::filesystem::create_directories(
        std::filesystem::path(fullPath).parent_path());

    if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
      std::cerr << "Cannot open file in archive: " << filename << std::endl;
      unzClose(zipfile);
      return false;
    }

    std::ofstream outFile(fullPath, std::ios::binary);
    if (!outFile.is_open()) {
      std::cerr << "Cannot write to file: " << fullPath << std::endl;
      unzCloseCurrentFile(zipfile);
      unzClose(zipfile);
      return false;
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
  return true;
}
