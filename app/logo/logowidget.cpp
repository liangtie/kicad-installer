#include "logowidget.h"

#include <qnamespace.h>

#include "ui_logowidget.h"

LogoWidget::LogoWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LogoWidget)
{
  ui->setupUi(this);
  for (const auto lay : {ui->lay_logo, ui->lay_main}) {
    lay->setAlignment(Qt::AlignCenter);
  }
}

LogoWidget::~LogoWidget()
{
  delete ui;
}
