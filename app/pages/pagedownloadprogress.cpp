#include <QFile>
#include <QStandardPaths>
#include <QTimer>

#include "pagedownloadprogress.h"

#include <qapplication.h>
#include <qcontainerfwd.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qtimer.h>

#include "app/utils/downloader.h"
#include "app/utils/fmt_download_url.h"
#include "app/utils/get_latest_version.h"
#include "ui_pagedownloadprogress.h"

inline auto fmt_save_path(QString saveDir, QString const& filename) -> QString
{
  return saveDir + "/" + filename;
}

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

void PageDownloadProgress::startDownload(INSTALLATION_METHOD method)
{
  _downloadThread = std::make_unique<QThread>();

  const auto version = get_latest_version();

  if (!version) {
    QMessageBox::warning(this, "Error", "Failed to get the latest version.");
    return;
  }

  const auto file_name = fmt_download_file_name(method, *version);

  _downloadFilePath = fmt_save_path(
      _saveDir
          ? *_saveDir
          : QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
      +file_name.c_str());

  const auto url = gen_url_from_file_name(file_name);
  _downloader = new DOWNLOADER(url.c_str(), _downloadFilePath);

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
    emit downloadCompleted(_downloadFilePath);
  }
}
