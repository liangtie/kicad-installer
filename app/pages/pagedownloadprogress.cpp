#include <QTimer>

#include "pagedownloadprogress.h"

#include <qapplication.h>
#include <qmessagebox.h>
#include <qtimer.h>

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

void PageDownloadProgress::startDownload()
{
  QTimer* timer = new QTimer(this);
  connect(timer,
          &QTimer::timeout,
          [this]()
          {
            ui->progressBar->setValue(ui->progressBar->value() + 1);
            if (ui->progressBar->value() == 100) {
              QMessageBox::information(this, "", "下载完成");
              QApplication::quit();
            }
          });
  timer->start(100);
}
