#ifndef CSIGNMAP_H
#define CSIGNMAP_H

#include <QList>
#include "CSign.h"
#include "CcBase.h"

class CSignMap
{
public:
  CSignMap(const QString& sVarname, size_t uiSize);

  CSign& operator[](size_t uiPos);
  size_t size() const
    { return m_uiSize; }
  QString getSFontRectangleMap(bool bCppMode);
  QString getSFontRectangleHeader();
  void shrinkUpperAndLower();
  void updateRectangleSizes(QList<int> &oSizesList);
private:
  QString       m_sVarName;
  size_t        m_uiSize;
  QList<CSign>  m_oSigns;
};

#endif // CSIGNMAP_H
