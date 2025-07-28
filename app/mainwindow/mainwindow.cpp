

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

#include "app/pages/page_index.h"
#include "app/pages/pageconfigportable.h"
#include "app/pages/pagedownloadprogress.h"
#include "app/pages/pageselectinstallmethod.h"
#include "app/titlebar/titlebar.h"
#include "app/utils/create_shortcut.h"
#include "app/utils/find_file_in_dir.h"
#include "app/utils/fmt_download_url.h"
#include "app/utils/get_latest_version.h"
#include "app/utils/installation_method.h"
#include "app/utils/unzip_dialog.h"
#include "app/logo/logowidget.h"

struct INSTALLATION_CONFIG
{
  PortableConfig portableConfig;
  INSTALLATION_METHOD method;
};

enum MAINWINDOW_SIZE
{
  MAINWINDOW_WIDTH = 800,
  MAINWINDOW_HEIGHT = 600
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
  _windowAgent->setTitleBar(title_bar);
  layout->addWidget(title_bar);
  connect(closeButton,
          &QPushButton::clicked,
          this,
          []
          {
            if (QMessageBox::question(
                    nullptr, tr("提示"), tr("确定推出吗？"))
                == QMessageBox::Yes)
            {
              QApplication::quit();
            }
          });
  connect(minButton, &QPushButton::clicked, this, &QWidget::showMinimized);
  connect(helpButton,
          &QPushButton::clicked,
          this,
          [] { QDesktopServices::openUrl(QUrl("https://kicad.eda.cn/")); });



  layout->addWidget(new LogoWidget);

  _stackedWidget = new QStackedWidget(container_widget);
  layout->addWidget(_stackedWidget, 1);
  _stackedWidget->setFocus();

  auto select_page = new PageSelectInstallMethod;
  _stackedWidget->addWidget(select_page);

  auto portable_page = new PageConfigPortable;
  _stackedWidget->addWidget(portable_page);

  auto download_page = new PageDownloadProgress;
  _stackedWidget->addWidget(download_page);

  auto start_download = [this, download_page](INSTALLATION_METHOD method)
  {
    const auto version = get_latest_version();

    if (!version) {
      QMessageBox::warning(this, "Error", "Failed to get the latest version.");
      return;
    }

    _latestVersion = *version;

    const auto file_name = fmt_download_file_name(method, _latestVersion);
    const auto url = gen_url_from_file_name(file_name);

    _downloadFilePath =
        fmt_save_path(method == INSTALLATION_METHOD::PORTABLE
                          ? _installationConfig->portableConfig.save_path
                          : QStandardPaths::writableLocation(
                                QStandardPaths::DownloadLocation),
                      +file_name.c_str());

    _stackedWidget->setCurrentIndex(PAGE_DOWNLOAD_PROGRESS);
    download_page->startDownload(url.c_str(), *_downloadFilePath);
  };

  connect(select_page,
          &PageSelectInstallMethod::installMethodSelected,
          this,
          [=, this](INSTALLATION_METHOD method)
          {
            switch (method) {
              case PORTABLE: {
                _stackedWidget->setCurrentWidget(portable_page);
                break;
              }
              case INSTALLER: {
                _installationConfig->method = INSTALLATION_METHOD::INSTALLER;
                start_download(INSTALLATION_METHOD::INSTALLER);
                break;
              }
            }
          });
  connect(portable_page,
          &PageConfigPortable::startDownload,
          this,
          [=, this](PortableConfig const& config)
          {
            _installationConfig->portableConfig = config;
            _installationConfig->method = INSTALLATION_METHOD::PORTABLE;
            start_download(INSTALLATION_METHOD::PORTABLE);
          });

  connect(download_page,
          &PageDownloadProgress::downloadCompleted,
          this,
          [=, this]
          {
            const auto extract_dir =
                QFileInfo(*_downloadFilePath).absoluteDir().absolutePath() + "/"
                + fmt_base_kicad_name(_latestVersion).c_str();

            if (UNZIP_DIALOG::execUnzip(*_downloadFilePath, extract_dir)) {

              download_page->setExtractDir(extract_dir);

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

  setAttribute(Qt::WA_DontCreateNativeAncestors);
  setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
}

MainWindow::~MainWindow() {}
