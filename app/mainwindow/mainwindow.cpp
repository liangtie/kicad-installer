#include <QLabel>

#include "mainwindow.h"

#include <QWKWidgets/widgetwindowagent.h>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStyle>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#  include <QtGui/QActionGroup>
#else
#  include <QtWidgets/QActionGroup>
#endif

enum MAINWINDOW_SIZE
{
  MAINWINDOW_WIDTH = 800,
  MAINWINDOW_HEIGHT = 600
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
  setAttribute(Qt::WA_DontCreateNativeAncestors);
  install_window_agent();
  resize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
}

MainWindow::~MainWindow() {}

void MainWindow::install_window_agent()
{
  // 1. Setup window agent
  windowAgent = new QWK::WidgetWindowAgent(this);
  windowAgent->setup(this);
}
