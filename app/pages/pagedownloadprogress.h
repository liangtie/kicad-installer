#ifndef PAGEDOWNLOADPROGRESS_H
#define PAGEDOWNLOADPROGRESS_H

#include <QWidget>

namespace Ui
{
class PageDownloadProgress;
}

class PageDownloadProgress : public QWidget
{
  Q_OBJECT

public:
  explicit PageDownloadProgress(QWidget* parent = nullptr);
  ~PageDownloadProgress();

  void startDownload();

private:
  Ui::PageDownloadProgress* ui;
};

#endif  // PAGEDOWNLOADPROGRESS_H
