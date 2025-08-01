#include <QApplication>
#include <QFile>
#include <QFontDatabase>  // Add this include
#include <QLocale>
#include <QTextStream>
#include <QTranslator>

#include <qdebug.h>
#include <qobject.h>
#include <qtenvironmentvariables.h>

#include "app/mainwindow/mainwindow.h"

int main(int argc, char* argv[])
{
  qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
  QApplication::setHighDpiScaleFactorRoundingPolicy(
      Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
  QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
  QApplication qApplication(argc, argv);

  // Load custom fonts
  // Load only used custom fonts
  QFontDatabase::addApplicationFont(":/fonts/PingFang Regular.ttf");
  QFontDatabase::addApplicationFont(":/fonts/PingFang Bold.ttf");

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
  QCoreApplication::setOrganizationDomain("kicad.eda.cn");
  QCoreApplication::setApplicationVersion("1.0.0");

  const auto styleSheet = ([]() {
    QFile styleSheetFile(":/themes/light-style.css");
    QTextStream styleSheetStream(&styleSheetFile);
    return styleSheetFile.open(QIODevice::ReadOnly) ? styleSheetStream.readAll() : QString();
  })();
  qApplication.setStyleSheet(styleSheet);

  MainWindow w;
  w.show();

  return qApplication.exec();
}
