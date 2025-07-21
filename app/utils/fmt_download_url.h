#ifndef FMT_DOWNLOAD_URL_H
#define FMT_DOWNLOAD_URL_H

#include <string>

#include <fmt/format.h>

#include "Installation_method.h"

inline auto fmt_download_url(INSTALLATION_METHOD method,
                             std::string const& version) -> std::string
{
  return {};
}

#endif
