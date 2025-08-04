#ifndef VERSION_INFO_H
#define VERSION_INFO_H

#include <nlohmann/json.hpp>

struct VERSION_INFO
{
  std::string version;
  std::string release_date;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(VERSION_INFO, version, release_date)
};

#endif
