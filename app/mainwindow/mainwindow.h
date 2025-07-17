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

enum PAGE_INDEX;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  void show_page(PAGE_INDEX index);

private:
  QWK::WidgetWindowAgent* _windowAgent;
  std::map<PAGE_INDEX, QWidget*> _pages;
  QStackedWidget* _stackedWidget;
};
#endif  // MAINWINDOW_H
