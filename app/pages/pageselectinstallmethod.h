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

  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  Ui::PageSelectInstallMethod* ui;

signals:
  void startInstaller();
  void configurePortable();
};

#endif  // PAGESELECTINSTALLMETHOD_H
