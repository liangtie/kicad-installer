#ifndef LOGOWIDGET_H
#define LOGOWIDGET_H

#include <QWidget>

namespace Ui
{
class LogoWidget;
}

class LogoWidget : public QWidget
{
  Q_OBJECT

public:
  explicit LogoWidget(QWidget* parent = nullptr);
  ~LogoWidget();

private:
  Ui::LogoWidget* ui;
};

#endif  // LOGOWIDGET_H
