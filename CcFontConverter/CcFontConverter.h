#ifndef CCFONTCONVERTER_H
#define CCFONTCONVERTER_H

#include <QFont>
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

private slots:
  void onFontClicked(bool bChecked);
  void onGenerateClicked(bool bChecked);

private:
  static bool isUnprintable(char iChar);

private:
  Ui::CcFontConverter *m_pUi;
  QFont m_oFont;
  static const size_t c_uiUnprintableSize;
  static const char c_pUnprintable[];
};

#endif // CCFONTCONVERTER_H
