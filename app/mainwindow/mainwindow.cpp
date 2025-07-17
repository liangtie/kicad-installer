

#include <QDesktopServices>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "mainwindow.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qdesktopservices.h>
#include <qurl.h>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStyle>

#include "app/titlebar/titlebar.h"

enum MAINWINDOW_SIZE
{
  MAINWINDOW_WIDTH = 800,
  MAINWINDOW_HEIGHT = 600
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
  _windowAgent = new QWK::WidgetWindowAgent(this);
  _windowAgent->setup(this);

  auto container_widget = new QWidget(this);
  setCentralWidget(container_widget);

  auto layout = new QVBoxLayout(container_widget);
  layout->setContentsMargins({});
  container_widget->setLayout(layout);

  auto title_bar = new TitleBar(container_widget);
  auto minButton = title_bar->minimize_button();
  auto closeButton = title_bar->close_button();
  auto helpButton = title_bar->help_button();
  _windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
  _windowAgent->setSystemButton(QWK::WindowAgentBase::Close, closeButton);
  _windowAgent->setHitTestVisible(helpButton);
  _windowAgent->setTitleBar(title_bar);
  layout->addWidget(title_bar);
  connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
  connect(minButton, &QPushButton::clicked, this, &QWidget::showMinimized);
  connect(helpButton,
          &QPushButton::clicked,
          this,
          []
          {
            QDesktopServices::openUrl(QUrl("https://kicad.eda.cn/"));
          });

  auto stack = new QStackedWidget(container_widget);
  stack->setFocus();
  layout->addWidget(stack, 1);

  setAttribute(Qt::WA_DontCreateNativeAncestors);
  setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
}

MainWindow::~MainWindow() {}
