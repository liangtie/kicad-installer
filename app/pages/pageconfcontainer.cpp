#include "pageconfcontainer.h"

#include "ui_pageconfcontainer.h"

PageConfContainer::PageConfContainer(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageConfContainer)
{
  ui->setupUi(this);
}

PageConfContainer::~PageConfContainer()
{
  delete ui;
}
