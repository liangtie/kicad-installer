#include <QMessageBox>
#include <vector>

#include "httplib_wrapper.h"
#include "get_latest_version.h"

#include <nlohmann/json.hpp>
#include <qmessagebox.h>

#include "version_info.h"

auto get_latest_version() -> std::optional<std::string>
{
  try {
    auto client = httplib::Client("https://kicad.eda.cn" );
    auto res = client.Get("/kicad-versions.json");
    const auto versions =
        nlohmann::json::parse(res->body).get<std::vector<VERSION_INFO>>();
    if (!versions.empty()) {
      return versions.back().version;
    }
  }

  catch (...)
  {
  }
  return {};
}
