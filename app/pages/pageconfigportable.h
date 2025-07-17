#ifndef PAGECONFIGPORTABLE_H
#define PAGECONFIGPORTABLE_H

#include <QWidget>

#include <qtmetamacros.h>

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


  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  Ui::PageConfigPortable* ui;

  void selectDownloadPath();

signals:

  void startDownload();
};

#endif  // PAGECONFIGPORTABLE_H
