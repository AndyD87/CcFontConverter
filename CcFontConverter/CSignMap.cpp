#include "CSignMap.h"

CSignMap::CSignMap(size_t uiSize) :
  m_uiSize(uiSize)
{
  m_oSigns.reserve(static_cast<int>(uiSize));
}

CSign& CSignMap::operator[](size_t uiPos)
{
  while(m_oSigns.size() <= static_cast<int>(uiPos))
    m_oSigns.append(CSign());
  return m_oSigns[static_cast<int>(uiPos)];
}

QString CSignMap::getSRectangleMap()
{
  shrinkUpperAndLower();
  QString sOutput;
  QString sReturn = "";
  QString sStructs = "";
  QString sMap = "SRectangle* Graphics_Sign_" + QString::number(m_uiSize) + "[] = {\r\n";
  // for each letter do create a bitmap
  for(CSign& oSignMap : m_oSigns)
  {
    QString sSignNr = QString::number(oSignMap.getId());
    sStructs += "SRectangle Graphics_Sign_" + QString::number(m_uiSize) + "_" + sSignNr + " = " + oSignMap.getSRectangle() + "\r\n";
    sMap += "  &Graphics_Sign_" + QString::number(m_uiSize) + "_" + sSignNr + ",\r\n";
  }// end for

  sMap    += "};";
  sReturn += sStructs;
  sReturn += "\r\n";
  sReturn += sMap;
  return sReturn;
}

void CSignMap::shrinkUpperAndLower()
{
  uint32 iLastLower = 0;
  uint32 iFirstUpper = 0x7fffffff;
  // Get row fith first and last pixel in all signs
  for (CSign& oSignMap : m_oSigns)
  {
    uint32 iSignLower = oSignMap.getLastLower();
    uint32 iSignUpper = oSignMap.getFirstUpper();
    if (iSignLower != UINT32_MAX &&
        iSignLower > iLastLower)
    {
      iLastLower = iSignLower;
    }
    if(iSignUpper != UINT32_MAX &&
       iSignUpper < iFirstUpper)
    {
      iFirstUpper = iSignUpper;
    }
  }
  // Cut all rows
  for (CSign& oSignMap : m_oSigns)
  {
    oSignMap.cutRows(iFirstUpper, iLastLower);
    if (oSignMap.getId() == ' ')
    {
      oSignMap.cutColumns(oSignMap.getWidth()/2, 0);
    }
    else
    {
      oSignMap.cutColumns();
    }
  }
}
