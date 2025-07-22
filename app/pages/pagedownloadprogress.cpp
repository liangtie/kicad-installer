#include <QFile>
#include <QStandardPaths>
#include <QTimer>

#include "pagedownloadprogress.h"

#include <qapplication.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qtimer.h>

#include "app/utils/downloader.h"
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

  if (_downloadThread && _downloadThread->isRunning()) {
    _downloader->cancel();
    _downloadThread->quit();
    _downloadThread->wait();
  }
}

void PageDownloadProgress::startDownload(QString const& url,
                                         QString const& save_path)
{
  _downloadThread = std::make_unique<QThread>();
  _downloader = new DOWNLOADER(url, save_path);

  _downloader->moveToThread(_downloadThread.get());
  connect(_downloadThread.get(),
          &QThread::started,
          _downloader,
          &DOWNLOADER::start);
  connect(_downloader,
          &DOWNLOADER::downloadProgress,
          this,
          &PageDownloadProgress::updateProgress);
  _downloadThread->start();
}

void PageDownloadProgress::updateProgress(DOWNLOAD_PROGRESS const& progress)
{
  ui->progressBar->setValue(progress.downloaded * 100 / progress.total);
  ui->label_down_count->setText(
      QString::number(progress.downloaded / 1024 / 1024) + " MB");
  ui->label_speed->setText(QString::number(progress.speed / 1024) + " KB/s");

  if (progress.finished) {
    emit downloadCompleted();
  }
}
