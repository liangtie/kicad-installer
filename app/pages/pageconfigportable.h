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

private:
  Ui::PageConfigPortable* ui;

signals:

  void startPortable(PortableConfig const& config);
};

#endif  // PAGECONFIGPORTABLE_H
