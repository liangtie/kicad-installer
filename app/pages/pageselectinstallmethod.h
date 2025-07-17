#ifndef PAGESELECTINSTALLMETHOD_H
#define PAGESELECTINSTALLMETHOD_H

#include <QWidget>

#include <qtmetamacros.h>

namespace Ui
{
class PageSelectInstallMethod;
}

enum INSTALL_METHOD
{
  PORTABLE,
  INSTALLER
};

class PageSelectInstallMethod : public QWidget
{
  Q_OBJECT

public:
  explicit PageSelectInstallMethod(QWidget* parent = nullptr);
  ~PageSelectInstallMethod();

private:
  Ui::PageSelectInstallMethod* ui;

signals:
  void installMethodSelected(INSTALL_METHOD method);
};

#endif  // PAGESELECTINSTALLMETHOD_H
