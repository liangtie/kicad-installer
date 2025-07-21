#include "pageselectinstallmethod.h"

#include "ui_pageselectinstallmethod.h"
#include "app/utils/installation_method.h"

PageSelectInstallMethod::PageSelectInstallMethod(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageSelectInstallMethod)
{
  ui->setupUi(this);
  for (const auto btn : {ui->btn_installer, ui->btn_portable}) {
    connect(btn,
            &QPushButton::clicked,
            [this, btn]()
            {
              emit installMethodSelected(btn == ui->btn_installer ? INSTALLER
                                                                  : PORTABLE);
            });
  }
}

PageSelectInstallMethod::~PageSelectInstallMethod()
{
  delete ui;
}
