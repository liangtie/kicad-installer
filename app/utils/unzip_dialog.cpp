#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "unzip_dialog.h"

#include <minizip/unzip.h>

UNZIP_WORKER::UNZIP_WORKER(QString zipFile, QString destDir)
    : _zipFile(std::move(zipFile))
    , _destDir(std::move(destDir))
{
}

void UNZIP_WORKER::run()
{
  unzFile zipfile = unzOpen(_zipFile.toStdString().c_str());
  if (!zipfile) {
    emit unzipError("Cannot open zip file: " + _zipFile);
    return;
  }

  if (unzGoToFirstFile(zipfile) != UNZ_OK) {
    emit unzipError("Cannot locate first file in archive.");
    unzClose(zipfile);
    return;
  }

  int count = 0;
  do {
    char filename[512];
    unz_file_info fileInfo;
    if (unzGetCurrentFileInfo(zipfile,
                              &fileInfo,
                              filename,
                              sizeof(filename),
                              nullptr,
                              0,
                              nullptr,
                              0)
        != UNZ_OK)
    {
      emit unzipError("Failed to get file info.");
      unzClose(zipfile);
      return;
    }

    QString qFilename = QString::fromUtf8(filename);
    emit unzipProgress(++count, qFilename);

    std::string fullPath = _destDir.toStdString() + "\\" + filename;

    if (filename[strlen(filename) - 1] == '/'
        || filename[strlen(filename) - 1] == '\\')
    {
      std::filesystem::create_directories(fullPath);
      continue;
    }

    std::filesystem::create_directories(
        std::filesystem::path(fullPath).parent_path());

    if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
      emit unzipError("Cannot open file in archive: " + qFilename);
      unzClose(zipfile);
      return;
    }

    std::ofstream outFile(fullPath, std::ios::binary);
    if (!outFile.is_open()) {
      emit unzipError("Cannot write to file: "
                      + QString::fromStdString(fullPath));
      unzCloseCurrentFile(zipfile);
      unzClose(zipfile);
      return;
    }

    std::vector<char> buffer(8192);
    int bytesRead;
    while (
        (bytesRead = unzReadCurrentFile(zipfile, buffer.data(), buffer.size()))
        > 0)
    {
      outFile.write(buffer.data(), bytesRead);
    }

    outFile.close();
    unzCloseCurrentFile(zipfile);

  } while (unzGoToNextFile(zipfile) == UNZ_OK);

  unzClose(zipfile);
  emit unzipFinished();
}

// --- UNZIP_DIALOG ---

UNZIP_DIALOG::UNZIP_DIALOG(QString const& zipDir,
                           QString const& destDir,
                           QWidget* parent)
    : QDialog(parent)
    , _zipDir(zipDir)
    , _destDir(destDir)
{
  setWindowTitle("Unzipping...");
  resize(400, 100);

  auto layout = new QVBoxLayout(this);
  auto label = new QLabel("Unzipping files...", this);
  layout->addWidget(label);

  auto cancel = new QPushButton("Cancel", this);
  layout->addWidget(cancel);
  connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
}

void UNZIP_DIALOG::startWorker()
{
  QThread* thread = new QThread(this);
  UNZIP_WORKER* worker = new UNZIP_WORKER(_zipDir, _destDir);
  worker->moveToThread(thread);

  connect(thread, &QThread::started, worker, &UNZIP_WORKER::run);
  connect(
      worker, &UNZIP_WORKER::unzipProgress, this, &UNZIP_DIALOG::onProgress);
  connect(worker, &UNZIP_WORKER::unzipError, this, &UNZIP_DIALOG::onError);
  connect(
      worker, &UNZIP_WORKER::unzipFinished, this, &UNZIP_DIALOG::onFinished);

  connect(worker, &UNZIP_WORKER::unzipFinished, thread, &QThread::quit);
  connect(worker, &UNZIP_WORKER::unzipError, thread, &QThread::quit);
  connect(thread, &QThread::finished, worker, &QObject::deleteLater);
  connect(thread, &QThread::finished, thread, &QObject::deleteLater);

  thread->start();
}

bool UNZIP_DIALOG::execUnzip(QString const& zipFile,
                             QString const& destDir,
                             QWidget* parent)
{
  UNZIP_DIALOG dialog(zipFile, destDir, parent);
  dialog.startWorker();
  int result = dialog.exec();
  return dialog._result && (result == QDialog::Accepted);
}

void UNZIP_DIALOG::onProgress(int count, QString const& filename)
{
  setWindowTitle(QString("Unzipping %1: %2").arg(count).arg(filename));
}

void UNZIP_DIALOG::onError(QString const& msg)
{
  QMessageBox::critical(this, "Unzip Error", msg);
  _result = false;
  reject();
}

void UNZIP_DIALOG::onFinished()
{
  _result = true;
  accept();
}
