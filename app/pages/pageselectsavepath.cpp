#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include "pageselectsavepath.h"

#include "ui_pageselectsavepath.h"

PageSelectSavePath::PageSelectSavePath(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PageSelectSavePath)
{
  ui->setupUi(this);
  ui->le_download_dir->setText(
      QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
  ui->le_download_dir->installEventFilter(this);

  connect(ui->btn_change_dir,
          &QPushButton::clicked,
          this,
          &PageSelectSavePath::selectDownloadPath);
}

PageSelectSavePath::~PageSelectSavePath()
{
  delete ui;
}

QString PageSelectSavePath::getSavePath() const
{
  return ui->le_download_dir->text();
}

bool PageSelectSavePath::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonPress && obj == ui->le_download_dir) {
    selectDownloadPath();
    return true;
  }
  return QWidget::eventFilter(obj, event);
}

void PageSelectSavePath::selectDownloadPath()
{
  auto save_path =
      QFileDialog::getExistingDirectory(this, "选择保存路径", QDir::homePath());
  if (!save_path.isEmpty()) {
    ui->le_download_dir->setText(save_path);
  }
}
