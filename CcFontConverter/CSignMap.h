#ifndef CSIGNMAP_H
#define CSIGNMAP_H

#include <QList>
#include "CSign.h"
#include "CcBase.h"

class CSignMap
{
public:
  CSignMap(size_t uiSize);

  CSign& operator[](size_t uiPos);
  size_t getSize() const
    { return m_uiSize; }
  QString getSRectangleMap(const QString& sVarName);
  void shrinkUpperAndLower();
private:
  size_t        m_uiSize;
  QList<CSign>  m_oSigns;
};

#endif // CSIGNMAP_H
