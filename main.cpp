#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QApplication>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/style/ThemeManager.hpp>
#include <oclero/qlementine/icons/QlementineIcons.hpp>
#include <qobject.h>


#include "app/mainwindow/mainwindow.h"

int main(int argc, char* argv[])
{
  QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
  QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
  QApplication qApplication(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString& locale : uiLanguages) {
    const QString baseName = "kicad-installer_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      qApplication.installTranslator(&translator);
      break;
    }
  }

 // Must be set after creating a QApplication.
  QGuiApplication::setApplicationDisplayName(QObject::tr("KiCad Installer"));
  QCoreApplication::setApplicationName(QObject::tr("KiCad Installer"));
  QGuiApplication::setDesktopFileName(QObject::tr("KiCad Installer"));
  QCoreApplication::setOrganizationName("kicad.eda.cn");
  QCoreApplication::setOrganizationDomain("kicad.eda.c");
  QCoreApplication::setApplicationVersion("1.0.0");

  // Custom QStyle.
  auto* style = new oclero::qlementine::QlementineStyle(&qApplication);
  style->setAnimationsEnabled(true);
  style->setAutoIconColor(oclero::qlementine::AutoIconColor::TextColor);
  style->setIconPathGetter(oclero::qlementine::icons::fromFreeDesktop);
  qApplication.setStyle(style);

  // Custom icon theme.
  oclero::qlementine::icons::initializeIconTheme();
  QIcon::setThemeName("qlementine");

  // Theme manager.
  auto* themeManager = new oclero::qlementine::ThemeManager(style);
  themeManager->loadDirectory(":/showcase/themes");

  // Define theme on QStyle.
  themeManager->setCurrentTheme("Light");

  MainWindow w;
  w.show();

  return qApplication.exec();

}
