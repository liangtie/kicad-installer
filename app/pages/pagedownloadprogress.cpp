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
  ui->btn_open_dir->hide();
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

void PageDownloadProgress::updateProgress(DOWNLOAD_PROGRESS const& progress)
{
  ui->progressBar->setValue(progress.downloaded * 100 / progress.total);
  ui->label_down_count->setText(
      fmt::format("已下载{}MB，待下载{}MB",
                  progress.downloaded / MB,
                  (progress.total - progress.downloaded) / MB)
          .c_str());

  QString speedText;
  if (progress.speed >= MB) {
    speedText = QString("下载速度:") + QString::number(progress.speed / double(MB), 'f', 2) + " MB/s";
  } else {
    speedText = QString("下载速度:") + QString::number(progress.speed / double(KB), 'f', 2) + " KB/s";
  }
  ui->label_speed->setText(speedText);

  if (progress.finished) {
    emit downloadCompleted();
    ui->widget_progress->hide();
    ui->btn_open_dir->show();
    connect(ui->btn_open_dir,
            &QPushButton::clicked,
            this,
            [this]()
            {
              // extract the directory from the file path
              QString dir = QFileInfo(_downloadFilePath).absolutePath();
              QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
            });
  }

  if (progress.error) {
    QMessageBox::critical(this, "下载错误", progress.errorMessage);
  }
}