#include "pageselectsavepath.h"

#include "ui_pageselectsavepath.h"

PageSelectSavePath::PageSelectSavePath(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageSelectSavePath)
{
  ui->setupUi(this);
}

PageSelectSavePath::~PageSelectSavePath()
{
  delete ui;
}
