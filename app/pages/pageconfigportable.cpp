#include "pageconfigportable.h"

#include "ui_pageconfigportable.h"

PageConfigPortable::PageConfigPortable(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageConfigPortable)
{
  ui->setupUi(this);
}

PageConfigPortable::~PageConfigPortable()
{
  delete ui;
}
