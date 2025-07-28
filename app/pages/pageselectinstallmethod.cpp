#include "pageselectinstallmethod.h"

#include <qpushbutton.h>

#include "ui_pageselectinstallmethod.h"

PageSelectInstallMethod::PageSelectInstallMethod(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageSelectInstallMethod)
{
  ui->setupUi(this);
  ui->lay_main->setAlignment(Qt::AlignCenter);

  connect(ui->btn_start_installer,
          &QPushButton::clicked,
          this,
          &PageSelectInstallMethod::startInstaller);
}

PageSelectInstallMethod::~PageSelectInstallMethod()
{
  delete ui;
}

bool PageSelectInstallMethod::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonPress && obj == ui->lb_portable) {
    emit configurePortable();
    return true;
  }
  return QWidget::eventFilter(obj, event);
}
