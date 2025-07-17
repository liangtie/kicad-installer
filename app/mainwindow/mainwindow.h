#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <qstackedwidget.h>

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
};
#endif  // MAINWINDOW_H
