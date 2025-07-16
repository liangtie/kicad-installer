#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}

namespace QWK {
    class WidgetWindowAgent;
    class StyleAgent;
}


QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  void install_window_agent();

private:
  Ui::MainWindow* ui;
  QWK::WidgetWindowAgent* windowAgent;
};
#endif  // MAINWINDOW_H
