#include "pageconfcontainer.h"

#include "pageconfigportable.h"
#include "pageselectinstallmethod.h"
#include "pageselectsavepath.h"
#include "ui_pageconfcontainer.h"

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

  connect(page_select_install_method,
          &PageSelectInstallMethod::configurePortable,
          [=, this]
          {
            ui->stackedWidget->setCurrentWidget(page_config_portable);
            emit showBackwardButton(true);
          });

  connect(page_select_install_method,
          &PageSelectInstallMethod::startInstaller,
          this,
          [=, this]() {
            emit startDownloadInstaller(widget_select_save_path->getSavePath());
          });

  connect(page_config_portable,
          &PageConfigPortable::startPortable,
          this,
          [=, this](PortableConfig const& config)
          {
            emit startDownloadPortable(config,
                                       widget_select_save_path->getSavePath());
          });

  ui->stackedWidget->setCurrentWidget(page_select_install_method);

  show_home_page = [=, this]()
  {
    ui->stackedWidget->setCurrentWidget(page_select_install_method);
    emit showBackwardButton(false);
  };
}

PageConfContainer::~PageConfContainer()
{
  delete ui;
}
