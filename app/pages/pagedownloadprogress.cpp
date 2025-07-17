#include "pagedownloadprogress.h"

#include "ui_pagedownloadprogress.h"

PageDownloadProgress::PageDownloadProgress(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageDownloadProgress)
{
  ui->setupUi(this);
}

PageDownloadProgress::~PageDownloadProgress()
{
  delete ui;
}
