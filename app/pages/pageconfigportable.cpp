#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include "pageconfigportable.h"

#include "ui_pageconfigportable.h"

PageConfigPortable::PageConfigPortable(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageConfigPortable)
{
  ui->setupUi(this);

  // ui->edit_save_path->setText(
  //     QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
  // connect(ui->btn_change_save_path,
  //         &QPushButton::clicked,
  //         this,
  //         &PageConfigPortable::selectDownloadPath);

  // connect(ui->btn_start_download,
  //         &QPushButton::clicked,
  //         this,
  //         [this]()
  //         {
  //           const auto save_path = ui->edit_save_path->text();
  //           if (save_path.isEmpty()) {
  //             QMessageBox::warning(this, "Error", "Please select a save path.");
  //             return;
  //           }

  //           // Check if the dir exists.
  //           if (!QDir(save_path).exists()) {
  //             QMessageBox::warning(
  //                 this, "Error", "The save path does not exist.");
  //             return;
  //           }

  //           emit startDownload(PortableConfig {
  //               save_path,
  //               ui->check_shortcut->isChecked(),
  //               ui->check_auto_run->isChecked()
  //           });
  //         });
}

PageConfigPortable::~PageConfigPortable()
{
  delete ui;
}

bool PageConfigPortable::eventFilter(QObject* obj, QEvent* event)
{
  // if (event->type() == QEvent::MouseButtonPress && obj == ui->edit_save_path) {
  //   selectDownloadPath();
  //   return true;
  // }
  return QWidget::eventFilter(obj, event);
}

void PageConfigPortable::selectDownloadPath()
{
  auto save_path =
      QFileDialog::getExistingDirectory(this, "选择保存路径", QDir::homePath());

  // if (!save_path.isEmpty()) {
  //   ui->edit_save_path->setText(save_path);
  // }
}
