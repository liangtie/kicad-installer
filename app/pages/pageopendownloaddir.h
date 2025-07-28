#ifndef PAGEOPENDOWNLOADDIR_H
#define PAGEOPENDOWNLOADDIR_H

#include <QWidget>

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

private:
  Ui::PageOpenDownloadDir* ui;
};

#endif  // PAGEOPENDOWNLOADDIR_H
