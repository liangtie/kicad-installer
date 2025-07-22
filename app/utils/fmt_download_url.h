#ifndef FMT_DOWNLOAD_URL_H
#define FMT_DOWNLOAD_URL_H

#include <string>

#include <fmt/format.h>

#include "installation_method.h"

inline auto gen_url_from_file_name(std::string const& file_name) -> std::string
{
  return fmt::format("http://localhost:8000/{}", file_name);
}

inline auto fmt_base_kicad_name(std::string const& version) -> std::string
{
  return fmt::format("kicad-huaqiu-{}-x86_64", version);
}

inline auto fmt_download_file_name(INSTALLATION_METHOD method,
                                   std::string const& version) -> std::string
{
  auto base_file_name = fmt_base_kicad_name(version);

  if (method == INSTALLER) {
    base_file_name += ".exe";
  }

  return fmt::format("{}.zip", base_file_name);
}

#endif
