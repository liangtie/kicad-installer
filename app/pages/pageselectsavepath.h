#ifndef PAGESELECTSAVEPATH_H
#define PAGESELECTSAVEPATH_H

#include <QWidget>

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

private:
  Ui::PageSelectSavePath* ui;
};

#endif  // PAGESELECTSAVEPATH_H
