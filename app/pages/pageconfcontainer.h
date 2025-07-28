#ifndef PAGECONFCONTAINER_H
#define PAGECONFCONTAINER_H

#include <QWidget>
#include <qcontainerfwd.h>

namespace Ui
{
class PageConfContainer;
}

class PageConfContainer : public QWidget
{
  Q_OBJECT

public:
  explicit PageConfContainer(QWidget* parent = nullptr);
  ~PageConfContainer();

private:
  Ui::PageConfContainer* ui;

signals:

  void startDownloadInstaller(QString const& saveDir);
};

#endif  // PAGECONFCONTAINER_H
