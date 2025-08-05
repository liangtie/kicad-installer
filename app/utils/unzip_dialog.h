#ifndef UNZIP_DIALOG_H
#define UNZIP_DIALOG_H

#include <QDialog>
#include <QString>
#include <atomic>

#include <qthread.h>

class UNZIP_WORKER : public QObject
{
  Q_OBJECT

public:
  explicit UNZIP_WORKER(QString zipFile, QString destDir);

  void cancel();

public slots:
  void run();

signals:
  void unzipFinished();
  void unzipProgress(int unzipped_count, QString const& currentFileName);
  void unzipError(QString const& errorMessage);

private:
  QString _zipFile;
  QString _destDir;
  std::atomic_bool _cancel = false;
};

class UNZIP_DIALOG : public QDialog
{
  Q_OBJECT

public:
  static bool execUnzip(QString const& zipFile,
                        QString const& destDir,
                        QWidget* parent = nullptr);

private:
  explicit UNZIP_DIALOG(QString const& zipDir,
                        QString const& destDir,
                        QWidget* parent = nullptr);
  ~UNZIP_DIALOG();

  void startWorker();

private slots:
  void onProgress(int count, QString const& filename);
  void onError(QString const& msg);
  void onFinished();
  void reject() override;


private:
  QString _zipDir;
  QString _destDir;
  bool _result = false;
  QThread* _thread;
  UNZIP_WORKER* _worker = nullptr;
};

#endif
