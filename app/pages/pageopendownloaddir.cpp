#include <QDesktopServices>
#include <QFileInfo>

#include "pageopendownloaddir.h"

#include <qurl.h>

#include "ui_pageopendownloaddir.h"

PageOpenDownloadDir::PageOpenDownloadDir(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageOpenDownloadDir)
{
  ui->setupUi(this);
  connect(ui->btn_open_download_dir,
          &QPushButton::clicked,
          this,
          [this]()
          {
            // extract the directory from the file path

            QDesktopServices::openUrl(QUrl::fromLocalFile((
                [this]
                {
                  if (_extractDir.has_value()) {
                    return *_extractDir;
                  }

                  return QFileInfo(_downloadFilePath).absolutePath();
                })()));
          });
}

PageOpenDownloadDir::~PageOpenDownloadDir()
{
  delete ui;
}
