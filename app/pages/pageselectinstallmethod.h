#ifndef PAGESELECTINSTALLMETHOD_H
#define PAGESELECTINSTALLMETHOD_H

#include <QWidget>

namespace Ui
{
class PageSelectInstallMethod;
}

class PageSelectInstallMethod : public QWidget
{
  Q_OBJECT

public:
  explicit PageSelectInstallMethod(QWidget* parent = nullptr);
  ~PageSelectInstallMethod();

private:
  Ui::PageSelectInstallMethod* ui;
};

#endif  // PAGESELECTINSTALLMETHOD_H
