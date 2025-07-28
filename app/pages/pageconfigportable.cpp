

#include <QMessageBox>

#include "pageconfigportable.h"

#include "ui_pageconfigportable.h"

PageConfigPortable::PageConfigPortable(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageConfigPortable)
{
  ui->setupUi(this);

  connect(ui->btn_start_portable,
          &QPushButton::clicked,
          this,
          [this]()
          {
            emit startPortable({ui->check_shortcut->isChecked(),
                                ui->check_auto_run->isChecked()});
          });
}

PageConfigPortable::~PageConfigPortable()
{
  delete ui;
}
