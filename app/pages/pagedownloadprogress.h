#ifndef PAGEDOWNLOADPROGRESS_H
#define PAGEDOWNLOADPROGRESS_H

#include <QThread>
#include <QWidget>
#include <memory>

#include <qobject.h>
#include <qtmetamacros.h>

namespace Ui
{
class PageDownloadProgress;
}

enum INSTALLATION_METHOD;
struct DOWNLOAD_PROGRESS;
class DOWNLOADER;

class PageDownloadProgress : public QWidget
{
  Q_OBJECT

public:
  explicit PageDownloadProgress(QWidget* parent = nullptr);
  ~PageDownloadProgress();

  void startDownload(INSTALLATION_METHOD method);

signals:
  void downloadCompleted(QString const& path);

private:
  void updateProgress(DOWNLOAD_PROGRESS const& progress);
  Ui::PageDownloadProgress* ui;
  std::unique_ptr<QThread> _downloadThread;
  DOWNLOADER* _downloader;
  QString _savePath;
};

#endif  // PAGEDOWNLOADPROGRESS_H
