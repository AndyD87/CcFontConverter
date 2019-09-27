#include "CSignMap.h"

CSignMap::CSignMap(const QString& sVarName, size_t uiSize):
  m_sVarName(sVarName),
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

QString CSignMap::getSFontRectangleMap()
{
  shrinkUpperAndLower();
  QString sOutput;
  QString sReturn = "";
  QString sStructs = "";
  QString sMap = "SFontRectangle* " + m_sVarName + "[] = {\n";
  // for each letter do create a bitmap
  for(CSign& oSignMap : m_oSigns)
  {
    QString sSignNr = QString::number(oSignMap.getId());
    sStructs += "SFontRectangle " + m_sVarName + "_" + sSignNr + " = " + oSignMap.getSFontRectangle() + "\n";
    sMap += "  &" + m_sVarName + "_" + sSignNr + ",\n";
  }// end for

  sMap    += "};\n\n";
  sMap = "size_t " + m_sVarName + "_Size = sizeof("+m_sVarName+")/sizeof("+m_sVarName+"[0]);\n";
  sReturn += sStructs;
  sReturn += "\n";
  sReturn += sMap;
  return sReturn;
}

QString CSignMap::getSFontRectangleHeader()
{
  QString sOutput = "extern SFontRectangle* " + m_sVarName + "[];\n";
  sOutput = "extern size_t " + m_sVarName + "_Size;\n\n";
  return sOutput;
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
