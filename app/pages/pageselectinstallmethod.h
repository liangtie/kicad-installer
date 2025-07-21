#ifndef PAGESELECTINSTALLMETHOD_H
#define PAGESELECTINSTALLMETHOD_H

#include <QWidget>

#include "app/utils/Installation_method.h"

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

signals:
  void installMethodSelected(INSTALLATION_METHOD method);
};

#endif  // PAGESELECTINSTALLMETHOD_H
