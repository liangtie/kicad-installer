

#include <QDesktopServices>
#include <QLabel>
#include <QMovie>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "mainwindow.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qboxlayout.h>
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

#include "app/pages/page_index.h"
#include "app/pages/pageselectinstallmethod.h"
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
  _windowAgent->setSystemButton(QWK::WindowAgentBase::Help, helpButton);
  _windowAgent->setTitleBar(title_bar);
  layout->addWidget(title_bar);
  connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
  connect(minButton, &QPushButton::clicked, this, &QWidget::showMinimized);
  connect(helpButton,
          &QPushButton::clicked,
          this,
          [] { QDesktopServices::openUrl(QUrl("https://kicad.eda.cn/")); });

  QMovie* movie = new QMovie(":/icons/huaqiu-animate.gif");
  QLabel* label = new QLabel();
  label->setScaledContents(true);
  label->setMovie(movie);
  movie->start();
  auto lay_img = new QVBoxLayout();
  lay_img->setContentsMargins(8, 0, 8, 0);
  lay_img->addWidget(label);
  layout->addLayout(lay_img);

  _stackedWidget = new QStackedWidget(container_widget);
  layout->addWidget(_stackedWidget, 1);
  _stackedWidget->setFocus();

  setAttribute(Qt::WA_DontCreateNativeAncestors);
  setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);

  _stackedWidget->addWidget(new PageSelectInstallMethod);
}

MainWindow::~MainWindow() {}

void MainWindow::show_page(PAGE_INDEX index)
{
  if (index < _stackedWidget->count()) {
    _stackedWidget->setCurrentIndex(index);
  }
}
