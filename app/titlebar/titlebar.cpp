#include "titlebar.h"

#include <qapplication.h>
#include <qsize.h>
#include <qtoolbutton.h>

#include "ui_titlebar.h"

static constexpr auto kSize16 = QSize {16, 16};

TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TitleBar)
{
  ui->setupUi(this);
  // ui->btn_min->setIcon(
  //     make_themed_icon(Icons16::Action_WindowsMinimize, kSize16));
  // ui->btn_close->setIcon(
  //     make_themed_icon(Icons16::Action_WindowsClose, kSize16));
  // ui->btn_help->setIcon(make_themed_icon(Icons16::Misc_Question, kSize16));

  for (const auto btn : findChildren<QToolButton*>()) {
    btn->setIconSize(kSize16);
    btn->setFixedSize(kSize16);
  }
}

TitleBar::~TitleBar()
{
  delete ui;
}

auto TitleBar::close_button() const -> QToolButton*
{
  return ui->btn_close;
}

auto TitleBar::minimize_button() const -> QToolButton*
{
  return ui->btn_min;
}

auto TitleBar::help_button() const -> QToolButton*
{
  return ui->btn_help;
}
