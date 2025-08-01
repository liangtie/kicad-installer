#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

#include <qtmetamacros.h>
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
  auto back_button() const -> QToolButton*;

  auto set_backward_button_visible(bool v) -> void;

private:
  Ui::TitleBar* ui;

signals:

  void backward_clicked();
};

#endif  // TITLEBAR_H
