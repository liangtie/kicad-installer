#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>

namespace QWK
{
class WidgetWindowAgent;
class StyleAgent;
}  // namespace QWK

enum PAGE_INDEX;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  QWK::WidgetWindowAgent* _windowAgent;
  std::map<PAGE_INDEX, QWidget*> _pages;
};
#endif  // MAINWINDOW_H
