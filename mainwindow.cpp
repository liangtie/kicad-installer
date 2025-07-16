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

#include "./ui_mainwindow.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#  include <QtGui/QActionGroup>
#else
#  include <QtWidgets/QActionGroup>
#endif

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setAttribute(Qt::WA_DontCreateNativeAncestors);
  install_window_agent();
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::install_window_agent()
{
  // 1. Setup window agent
  windowAgent = new QWK::WidgetWindowAgent(this);
  windowAgent->setup(this);

}
