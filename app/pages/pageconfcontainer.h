#ifndef PAGECONFCONTAINER_H
#define PAGECONFCONTAINER_H

#include <QWidget>

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
};

#endif  // PAGECONFCONTAINER_H
