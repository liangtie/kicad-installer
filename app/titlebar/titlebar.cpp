#include "titlebar.h"

#include <qapplication.h>
#include <qicon.h>
#include <qsize.h>
#include <qtoolbutton.h>

#include "ui_titlebar.h"

static constexpr auto kSize16 = QSize {16, 16};

TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TitleBar)
{
  ui->setupUi(this);
  ui->btn_min->setIcon(QIcon(":/icons/minimize.svg"));
  ui->btn_close->setIcon(QIcon(":/icons/close.svg"));
  ui->btn_help->setIcon(QIcon(":/icons/help.svg"));
  ui->btn_back->setIcon(QIcon(":/icons/arrow-left.svg"));

  for (const auto btn : findChildren<QToolButton*>()) {
    btn->setIconSize(kSize16);
    btn->setFixedSize(kSize16);
    btn->setProperty("system-button", true);
  }

  set_backward_button_visible(false);

  connect(
      ui->btn_back, &QToolButton::clicked, this, &TitleBar::backward_clicked);
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

auto TitleBar::back_button() const -> QToolButton*
{
   return ui->btn_back;
}

auto TitleBar::set_backward_button_visible(bool v) -> void
{
  ui->btn_back->setVisible(v);
}
