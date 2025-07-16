// SPDX-FileCopyrightText: Olivier Cléro <oclero@hotmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <oclero/qlementine/widgets/AbstractItemListWidget.hpp>

namespace oclero::qlementine {
/// A SegmentedControl like on MacOS.
class SegmentedControl : public AbstractItemListWidget {
public:
  using AbstractItemListWidget::AbstractItemListWidget;

protected: // Inherited via AbstractItemListWidget
  void initStyleOptionFocus(QStyleOptionFocusRoundedRect& opt) const override;
  const QColor& getBgColor(const Theme& theme) const override;
  const QColor& getItemBgColor(MouseState mouse, const Theme& theme) const override;
  const QColor& getItemFgColor(MouseState mouse, bool selected, const Theme& theme) const override;
  const QColor& getItemBadgeBgColor(MouseState mouse, bool selected, const Theme& theme) const override;
  const QColor& getItemBadgeFgColor(MouseState mouse, bool selected, const Theme& theme) const override;
};
} // namespace oclero::qlementine
