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

#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>

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

static inline void emulateLeaveEvent(QWidget* widget)
{
  Q_ASSERT(widget);
  if (!widget) {
    return;
  }
  QTimer::singleShot(
      0,
      widget,
      [widget]()
      {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        const QScreen* screen = widget->screen();
#else
        const QScreen* screen = widget->windowHandle()->screen();
#endif
        const QPoint globalPos = QCursor::pos(screen);
        if (!QRect(widget->mapToGlobal(QPoint {0, 0}), widget->size())
                 .contains(globalPos))
        {
          QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
          if (widget->testAttribute(Qt::WA_Hover)) {
            const QPoint localPos = widget->mapFromGlobal(globalPos);
            const QPoint scenePos = widget->window()->mapFromGlobal(globalPos);
            static constexpr const auto oldPos = QPoint {};
            const Qt::KeyboardModifiers modifiers =
                QGuiApplication::keyboardModifiers();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
            const auto event = new QHoverEvent(
                QEvent::HoverLeave, scenePos, globalPos, oldPos, modifiers);
            Q_UNUSED(localPos);
#elif (QT_VERSION >= QT_VERSION_CHECK(6, 3, 0))
            const auto event = new QHoverEvent(
                QEvent::HoverLeave, localPos, globalPos, oldPos, modifiers);
            Q_UNUSED(scenePos);
#else
            const auto event = new QHoverEvent(
                QEvent::HoverLeave, localPos, oldPos, modifiers);
            Q_UNUSED(scenePos);
#endif
            QCoreApplication::postEvent(widget, event);
          }
        }
      });
}

void MainWindow::install_window_agent()
{
  // 1. Setup window agent
  windowAgent = new QWK::WidgetWindowAgent(this);
  windowAgent->setup(this);

  auto titleLabel = new QLabel("KiCad HQ Installer");
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setObjectName(QStringLiteral("win-title-label"));

  auto iconButton = new QWK::WindowButton();
  iconButton->setObjectName(QStringLiteral("icon-button"));
  iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  auto pinButton = new QWK::WindowButton();
  pinButton->setCheckable(true);
  pinButton->setObjectName(QStringLiteral("pin-button"));
  pinButton->setProperty("system-button", true);
  pinButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  auto minButton = new QWK::WindowButton();
  minButton->setObjectName(QStringLiteral("min-button"));
  minButton->setProperty("system-button", true);
  minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  auto maxButton = new QWK::WindowButton();
  maxButton->setCheckable(true);
  maxButton->setObjectName(QStringLiteral("max-button"));
  maxButton->setProperty("system-button", true);
  maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  auto closeButton = new QWK::WindowButton();
  closeButton->setObjectName(QStringLiteral("close-button"));
  closeButton->setProperty("system-button", true);
  closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  auto windowBar = new QWK::WindowBar();
  windowBar->setIconButton(iconButton);
  windowBar->setPinButton(pinButton);
  windowBar->setMinButton(minButton);
  windowBar->setMaxButton(maxButton);
  windowBar->setCloseButton(closeButton);
  windowBar->setTitleLabel(titleLabel);
  windowBar->setHostWidget(this);

  windowAgent->setTitleBar(windowBar);
  windowAgent->setHitTestVisible(pinButton, true);
  windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
  windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
  windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, maxButton);
  windowAgent->setSystemButton(QWK::WindowAgentBase::Close, closeButton);

  setMenuWidget(windowBar);

  connect(windowBar,
          &QWK::WindowBar::pinRequested,
          this,
          [this, pinButton](bool pin)
          {
            if (isHidden() || isMinimized() || isMaximized() || isFullScreen())
            {
              return;
            }
            setWindowFlag(Qt::WindowStaysOnTopHint, pin);
            show();
            pinButton->setChecked(pin);
          });
  connect(windowBar,
          &QWK::WindowBar::minimizeRequested,
          this,
          &QWidget::showMinimized);
  connect(windowBar,
          &QWK::WindowBar::maximizeRequested,
          this,
          [this, maxButton](bool max)
          {
            if (max) {
              showMaximized();
            } else {
              showNormal();
            }

            // It's a Qt issue that if a QAbstractButton::clicked triggers a
            // window's maximization, the button remains to be hovered until the
            // mouse move. As a result, we need to manually send leave events to
            // the button.
            emulateLeaveEvent(maxButton);
          });
  connect(windowBar, &QWK::WindowBar::closeRequested, this, &QWidget::close);
}
