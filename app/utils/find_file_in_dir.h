#ifndef FIND_FILE_IN_DIR_H
#define FIND_FILE_IN_DIR_H

#include <filesystem>
#include <optional>
#include <string>

inline auto find_file_in_dir(std::string const& dir_path,
                             std::string const& file_name)
    -> std::optional<std::string>
{
  namespace fs = std::filesystem;

  try {
    for (auto const& entry : fs::recursive_directory_iterator(dir_path)) {
      if (entry.is_regular_file() && entry.path().filename() == file_name) {
        return entry.path().string();  // Found the file
      }
    }
  } catch (fs::filesystem_error const& e) {
    // Log or handle error if needed
  }

  return {};  // Return empty string if not found
}

// ...existing code...

inline auto find_kicad_huaqiu_installer(std::string const& dir_path)
    -> std::optional<std::string>
{
  namespace fs = std::filesystem;
  try {
    for (auto const& entry : fs::recursive_directory_iterator(dir_path)) {
      if (entry.is_regular_file()) {
        auto filename = entry.path().filename().string();
        if (filename.starts_with("kicad-huaqiu-") && filename.ends_with(".exe"))
        {
          return entry.path().string();
        }
      }
    }
  } catch (fs::filesystem_error const&) {
    // Handle error if needed
  }
  return {};
}

//

#endif
