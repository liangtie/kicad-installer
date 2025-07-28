#include "pageopendownloaddir.h"

#include "ui_pageopendownloaddir.h"

PageOpenDownloadDir::PageOpenDownloadDir(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageOpenDownloadDir)
{
  ui->setupUi(this);
}

PageOpenDownloadDir::~PageOpenDownloadDir()
{
  delete ui;
}
