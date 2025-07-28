#ifndef PAGESELECTSAVEPATH_H
#define PAGESELECTSAVEPATH_H

#include <QWidget>

#include <qobject.h>

namespace Ui
{
class PageSelectSavePath;
}

class PageSelectSavePath : public QWidget
{
  Q_OBJECT

public:
  explicit PageSelectSavePath(QWidget* parent = nullptr);
  ~PageSelectSavePath();

  QString getSavePath() const;
  
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  Ui::PageSelectSavePath* ui;
  void selectDownloadPath();
};

#endif  // PAGESELECTSAVEPATH_H
