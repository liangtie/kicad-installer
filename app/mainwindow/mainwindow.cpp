

#include <QDesktopServices>
#include <QFileInfo>
#include <QLabel>
#include <QProcess>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QVBoxLayout>

#include "mainwindow.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qboxlayout.h>
#include <qdesktopservices.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qprocess.h>
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

#include "app/logo/logowidget.h"
#include "app/pages/pageconfcontainer.h"
#include "app/pages/pagedownloadprogress.h"
#include "app/pages/pageopendownloaddir.h"
#include "app/titlebar/titlebar.h"
#include "app/utils/create_shortcut.h"
#include "app/utils/find_file_in_dir.h"
#include "app/utils/fmt_download_url.h"
#include "app/utils/get_latest_version.h"
#include "app/utils/installation_method.h"
#include "app/utils/unzip_dialog.h"

struct INSTALLATION_CONFIG
{
  PortableConfig portableConfig;
  INSTALLATION_METHOD method;
  QString downloadDir;
};

enum MAINWINDOW_SIZE
{
  MAINWINDOW_WIDTH = 555,
  MAINWINDOW_HEIGHT = 420,
  SIDE_MARGIN = 50
};

inline auto fmt_save_path(QString saveDir, QString const& filename) -> QString
{
  return saveDir + "/" + filename;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , _installationConfig(std::make_unique<INSTALLATION_CONFIG>())
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
  _windowAgent->setHitTestVisible(title_bar->back_button());
  _windowAgent->setTitleBar(title_bar);
  layout->addWidget(title_bar);
  connect(
      closeButton, &QPushButton::clicked, this, [] { QApplication::quit(); });
  connect(minButton, &QPushButton::clicked, this, &QWidget::showMinimized);
  connect(helpButton,
          &QPushButton::clicked,
          this,
          [] { QDesktopServices::openUrl(QUrl("https://kicad.eda.cn/")); });

  layout->addWidget(new LogoWidget);

  _stackedWidget = new QStackedWidget(container_widget);
  auto lay_stackedWidget = new QVBoxLayout;
  lay_stackedWidget->setContentsMargins({SIDE_MARGIN, 0, SIDE_MARGIN, 0});
  lay_stackedWidget->addWidget(_stackedWidget);
  layout->addLayout(lay_stackedWidget, 1);
  _stackedWidget->setFocus();

  auto page_conf_container = new PageConfContainer;
  _stackedWidget->addWidget(page_conf_container);

  auto download_page = new PageDownloadProgress;
  _stackedWidget->addWidget(download_page);

  auto open_download_dir = new PageOpenDownloadDir;
  _stackedWidget->addWidget(open_download_dir);

  auto start_download = [=](INSTALLATION_METHOD method)
  {
    title_bar->set_backward_button_visible(false);
    _installationConfig->method = method;
    const auto version = get_latest_version();

    if (!version) {
      QMessageBox::warning(this, "错误", "获取最新版本失败");
      return;
    }

    _latestVersion = *version;

    const auto file_name = fmt_download_file_name(method, _latestVersion);
    const auto url = gen_url_from_file_name(file_name);

    _downloadFilePath =
        fmt_save_path(_installationConfig->downloadDir, +file_name.c_str());

    _stackedWidget->setCurrentWidget(download_page);
    download_page->startDownload(url.c_str(), *_downloadFilePath);
  };

  connect(page_conf_container,
          &PageConfContainer::startDownloadInstaller,
          this,
          [=](QString const& saveDir)
          {
            _installationConfig->downloadDir = saveDir;
            start_download(INSTALLATION_METHOD::INSTALLER);
          });

  connect(page_conf_container,
          &PageConfContainer::startDownloadPortable,
          this,
          [=](PortableConfig const& cnf, QString const& saveDir)
          {
            _installationConfig->portableConfig = cnf;
            _installationConfig->downloadDir = saveDir;
            start_download(INSTALLATION_METHOD::PORTABLE);
          });

  connect(download_page,
          &PageDownloadProgress::downloadCompleted,
          this,
          [=]
          {
            open_download_dir->setDownloadFilePath(*_downloadFilePath);
            _stackedWidget->setCurrentWidget(open_download_dir);
            const auto extract_dir =
                QFileInfo(*_downloadFilePath).absoluteDir().absolutePath() + "/"
                + fmt_base_kicad_name(_latestVersion).c_str();

            if (UNZIP_DIALOG::execUnzip(*_downloadFilePath, extract_dir)) {
              open_download_dir->setExtractDir(extract_dir);

              switch (_installationConfig->method) {
                case PORTABLE: {
                  auto kicad_exe =
                      find_file_in_dir(extract_dir.toStdString(), "kicad.exe");

                  if (!kicad_exe) {
                    return;
                  }

                  if (_installationConfig->portableConfig.create_shortcut) {
                    create_shortcut_on_desktop(*kicad_exe);
                  }

                  if (_installationConfig->portableConfig.auto_start) {
                    QProcess::startDetached(kicad_exe->c_str());
                  }

                  break;
                }
                case INSTALLER: {
                  if (auto kicad_installer_exe = find_kicad_huaqiu_installer(
                          extract_dir.toStdString()))
                  {
                    QProcess::startDetached(kicad_installer_exe->c_str());
                  }
                  break;
                }
              }
            }
          });

  connect(title_bar,
          &TitleBar::backward_clicked,
          [=] { page_conf_container->show_home_page(); });

  connect(page_conf_container,
          &PageConfContainer::showBackwardButton,
          title_bar,
          &TitleBar::set_backward_button_visible);

  setAttribute(Qt::WA_DontCreateNativeAncestors);
  setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
}

MainWindow::~MainWindow() {}
