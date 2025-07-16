#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:

  QWK::WidgetWindowAgent* windowAgent;
};
#endif  // MAINWINDOW_H
