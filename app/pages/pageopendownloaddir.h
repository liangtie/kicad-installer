#ifndef PAGEOPENDOWNLOADDIR_H
#define PAGEOPENDOWNLOADDIR_H

#include <QWidget>
#include <optional>

#include <qobject.h>

namespace Ui
{
class PageOpenDownloadDir;
}

class PageOpenDownloadDir : public QWidget
{
  Q_OBJECT

public:
  explicit PageOpenDownloadDir(QWidget* parent = nullptr);
  ~PageOpenDownloadDir();

  void setDownloadFilePath(QString const& path) { _downloadFilePath = path; }

  void setExtractDir(std::optional<QString> const& path) { _extractDir = path; }

private:
  Ui::PageOpenDownloadDir* ui;
  QString _downloadFilePath;
  std::optional<QString> _extractDir;
};

#endif  // PAGEOPENDOWNLOADDIR_H
