#ifndef CCFONTCONVERTER_H
#define CCFONTCONVERTER_H

#include <QMainWindow>

namespace Ui {
  class CcFontConverter;
}

class CcFontConverter : public QMainWindow
{
  Q_OBJECT

public:
  explicit CcFontConverter(QWidget *parent = nullptr);
  ~CcFontConverter();

private:
  Ui::CcFontConverter *ui;
};

#endif // CCFONTCONVERTER_H
