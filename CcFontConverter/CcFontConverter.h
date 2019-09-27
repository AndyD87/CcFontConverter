#ifndef CCFONTCONVERTER_H
#define CCFONTCONVERTER_H

#include <QFont>
#include <QMainWindow>
#include "CcBase.h"

class QString;
class QStringList;

namespace Ui {
  class CcFontConverter;
}

class CcFontConverter : public QMainWindow
{
  Q_OBJECT

public:
  explicit CcFontConverter(QWidget *parent = nullptr);
  ~CcFontConverter();
  void generate(const QString& sFamily, uint32 uiSize, const QString& sPrefix);

private slots:
  void onFontClicked(bool bChecked);
  void onGenerateClicked(bool bChecked);
  void onGenerateFilesClicked(bool bChecked);

private:
  static bool isUnprintable(char iChar);
  QList<QString> getFonts();
  void checkFonts();
  QList<uint32> getSizes();
  void checkSizes();
private:
  Ui::CcFontConverter *m_pUi;
  QFont m_oFont;
  static const size_t c_uiUnprintableSize;
  static const char c_pUnprintable[];
};

#endif // CCFONTCONVERTER_H
