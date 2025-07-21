#ifndef FMT_DOWNLOAD_URL_H
#define FMT_DOWNLOAD_URL_H

#include <string>

#include <fmt/format.h>

#include "installation_method.h"

inline auto gen_url_from_file_name(std::string const& file_name) -> std::string
{
  return fmt::format("https://down.eda.cn/data/kicad-release/{}", file_name);
}

inline auto fmt_download_file_name(INSTALLATION_METHOD method,
                                   std::string const& version) -> std::string
{
  auto base_file_name = fmt::format("kicad-huaqiu-{}-x86_64", version);

  if (method == INSTALLER) {
    base_file_name += ".exe";
  }

  return fmt::format("{}.zip", base_file_name);
}



#endif
