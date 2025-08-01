#ifndef PAGECONFCONTAINER_H
#define PAGECONFCONTAINER_H

#include <QWidget>
#include <functional>

#include "pageconfigportable.h"

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

   std::function<void()> show_home_page;

private:
  Ui::PageConfContainer* ui;

signals:

  void startDownloadInstaller(QString const& saveDir);

  void startDownloadPortable(PortableConfig const& cnf, QString const& saveDir);

  void showBackwardButton(bool v);
};

#endif  // PAGECONFCONTAINER_H
