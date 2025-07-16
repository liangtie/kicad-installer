#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <qtoolbutton.h>

namespace Ui
{
class TitleBar;
}

class TitleBar : public QWidget
{
  Q_OBJECT

public:
  explicit TitleBar(QWidget* parent = nullptr);
  ~TitleBar();

  auto close_button() const -> QToolButton*;
  auto minimize_button() const -> QToolButton*;
  auto help_button() const -> QToolButton*;

private:
  Ui::TitleBar* ui;
};

#endif  // TITLEBAR_H
