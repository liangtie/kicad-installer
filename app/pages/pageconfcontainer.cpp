#include "pageconfcontainer.h"

#include "ui_pageconfcontainer.h"
#include "pageselectsavepath.h"
#include "pageselectinstallmethod.h"
#include "pageconfigportable.h"

PageConfContainer::PageConfContainer(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageConfContainer)
{
  ui->setupUi(this);

  auto widget_select_save_path = new PageSelectSavePath(this);
  ui->lay_path->addWidget(widget_select_save_path);

  auto page_select_install_method = new PageSelectInstallMethod(this);
  auto page_config_portable = new PageConfigPortable(this);

  ui->stackedWidget->addWidget(page_select_install_method);
  ui->stackedWidget->addWidget(page_config_portable);


}

PageConfContainer::~PageConfContainer()
{
  delete ui;
}
