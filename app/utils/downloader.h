#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <atomic>

#include <qobject.h>

struct DOWNLOAD_PROGRESS
{
  unsigned long long total {};
  unsigned long long downloaded {};
  unsigned long long speed {};  // bytes per second
  unsigned long long eta {};  // seconds
  bool finished {};
  bool error {};
  QString errorMessage;
};

Q_DECLARE_METATYPE(DOWNLOAD_PROGRESS)

class DOWNLOADER : public QObject
{
  Q_OBJECT
public:
  DOWNLOADER(QString url, QString save_path);
  ~DOWNLOADER();

  void start();

  void cancel();

signals:
  void downloadProgress(DOWNLOAD_PROGRESS progress);

private:
  std::atomic_bool m_cancelled {false};
  QString _url;
  QString _savePath;
  DOWNLOAD_PROGRESS m_progress;

  void downloadFile();
};

#endif
