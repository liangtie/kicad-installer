#ifndef PAGECONFIGPORTABLE_H
#define PAGECONFIGPORTABLE_H

#include <QWidget>

namespace Ui
{
class PageConfigPortable;
}

class PageConfigPortable : public QWidget
{
  Q_OBJECT

public:
  explicit PageConfigPortable(QWidget* parent = nullptr);
  ~PageConfigPortable();

private:
  Ui::PageConfigPortable* ui;
};

#endif  // PAGECONFIGPORTABLE_H
