#include "pageselectinstallmethod.h"

#include "ui_pageselectinstallmethod.h"

PageSelectInstallMethod::PageSelectInstallMethod(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageSelectInstallMethod)
{
  ui->setupUi(this);
}

PageSelectInstallMethod::~PageSelectInstallMethod()
{
  delete ui;
}
