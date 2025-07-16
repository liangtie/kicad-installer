#ifndef MAKETHEMEDICON_H
#define MAKETHEMEDICON_H

#include <QIcon>

#include <oclero/qlementine/icons/Icons12.hpp>
#include <oclero/qlementine/icons/Icons16.hpp>
#include <oclero/qlementine/icons/Icons32.hpp>
#include <oclero/qlementine/style/QlementineStyle.hpp>
using Icons16 = oclero::qlementine::icons::Icons16;


inline auto make_themed_icon(Icons16 id, const QSize& size = {16, 16})
{
  const auto svgPath = oclero::qlementine::icons::iconPath(id);
  if (auto* style = oclero::qlementine::appStyle()) {
    return style->makeThemedIcon(svgPath, size);
  } else {
    return QIcon(svgPath);
  }
}




#endif
