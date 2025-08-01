#include <QMessageBox>
#include <vector>

#include "get_latest_version.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <qmessagebox.h>

auto get_latest_version() -> std::optional<std::string>
{
  try {
    cpr::Response r =
        cpr::Get(cpr::Url("https://kicad.eda.cn/kicad-versions.json"));
    const auto versions =
        nlohmann::json::parse(r.text).get<std::vector<std::string>>();
    // The latest version is the last one in the list
    if (!versions.empty()) {
      return versions.back();
    }
  }

  catch (...)
  {
  }
  return {};
}
