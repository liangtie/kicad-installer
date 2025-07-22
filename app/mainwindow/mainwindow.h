#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include <qstackedwidget.h>

struct INSTALLATION_CONFIG;

namespace QWK
{
class WidgetWindowAgent;
class StyleAgent;
}  // namespace QWK

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void startDownload();

private:
  QWK::WidgetWindowAgent* _windowAgent;
  QStackedWidget* _stackedWidget;
  std::unique_ptr<INSTALLATION_CONFIG> _installationConfig;
};
#endif  // MAINWINDOW_H
