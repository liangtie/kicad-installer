#ifndef PAGECONFIGPORTABLE_H
#define PAGECONFIGPORTABLE_H

#include <QWidget>

#include <qobject.h>

struct PortableConfig
{
  bool create_shortcut;
  bool auto_start;
};

Q_DECLARE_METATYPE(PortableConfig)

namespace Ui
{
class PageConfigPortable;
}

class PageConfigPortable : public QWidget
{
  Q_OBJECT

public:
  explicit PageConfigPortable(QWidget* parent = nullptr);
  ~PageConfigPortable();

  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  Ui::PageConfigPortable* ui;

  void selectDownloadPath();

signals:

  void startDownload(PortableConfig const& config);
};

#endif  // PAGECONFIGPORTABLE_H
