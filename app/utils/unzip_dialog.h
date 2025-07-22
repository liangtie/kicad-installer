#ifndef UNZIP_DIALOG_H
#define UNZIP_DIALOG_H

#include <QDialog>
#include <QString>

class UNZIP_WORKER : public QObject
{
  Q_OBJECT

public:
  explicit UNZIP_WORKER(QString zipFile, QString destDir);

public slots:
  void run();

signals:
  void unzipFinished();
  void unzipProgress(int unzipped_count, QString const& currentFileName);
  void unzipError(QString const& errorMessage);

private:
  QString _zipFile;
  QString _destDir;
};

class UNZIP_DIALOG : public QDialog
{
  Q_OBJECT

public:
  static bool execUnzip(QString const& zipFile, QString const& destDir, QWidget* parent = nullptr);

private:
  explicit UNZIP_DIALOG(QString const& zipDir,
                        QString const& destDir,
                        QWidget* parent = nullptr);

  void startWorker();

private slots:
  void onProgress(int count, QString const& filename);
  void onError(QString const& msg);
  void onFinished();

private:
  QString _zipDir;
  QString _destDir;
  bool _result = false;
};

#endif
