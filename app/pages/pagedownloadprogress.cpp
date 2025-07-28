#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTimer>

#include "pagedownloadprogress.h"

#include <fmt/format.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qtimer.h>

#include "app/utils/downloader.h"
#include "ui_pagedownloadprogress.h"

enum
{
  GB = 1024 * 1024 * 1024,
  MB = 1024 * 1024,
  KB = 1024
};

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
  _downloadFilePath = save_path;
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

void PageDownloadProgress::setExtractDir(QString const& path)
{
  _extractDir = path;
}

void PageDownloadProgress::updateProgress(DOWNLOAD_PROGRESS const& progress)
{
  if (progress.total == 0) {
    ui->progressBar->setValue(0);
    ui->label_down_count->setText("正在获取文件大小...");
    return;
  }

  ui->progressBar->setValue(progress.downloaded * 100 / progress.total);
  ui->lb_progress->setText(
      fmt::format("{}%", progress.downloaded * 100 / progress.total).c_str());
  ui->label_down_count->setText(
      fmt::format("已下载{}MB，待下载{}MB",
                  progress.downloaded / MB,
                  (progress.total - progress.downloaded) / MB)
          .c_str());

  QString speedText;
  if (progress.speed >= MB) {
    speedText = QString("下载速度:")
        + QString::number(progress.speed / double(MB), 'f', 2) + " MB/s";
  } else {
    speedText = QString("下载速度:")
        + QString::number(progress.speed / double(KB), 'f', 2) + " KB/s";
  }
  ui->label_speed->setText(speedText);

  if (progress.finished) {
    emit downloadCompleted();
  }

  if (progress.error) {
    QMessageBox::critical(this, "下载错误", progress.errorMessage);
  }
}
