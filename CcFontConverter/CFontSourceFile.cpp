#include "CFontSourceFile.h"
#include <QString>

const QByteArray CFontSourceFile::c_sGeneratorStart("// FontConverter start");
const QByteArray CFontSourceFile::c_sGeneratorEnd("// FontConverter end");
const QByteArray CFontSourceFile::c_sHeaderCommonData(
  "#include <stdlib.h>\n"
  "#ifndef SFONTRECTANGLE_DEFINED\n"
  "#define SFONTRECTANGLE_DEFINED //!< Avoid douple defintions\n"
  "#pragma pack(push, 1)\n"
  "/**\n"
  " * @brief Structure for describing an filled rectangular.\n"
  " *        Containing data structure of row and columns is depending on it's width.\n"
  " *        For size of a row, SFontRectangle_GetArrayWidth can be used to get it width of row in bytes.\n"
  " */\n"
  "typedef struct\n"
  "{\n"
  "	unsigned char uiWidth;	//!< Width of rectangle\n"
  "	unsigned char uiHeight;	//!< Height of rectangle\n"
  "	unsigned char pData[1];	//!< Pix map with size SFontRectangle_GetArrayWidth(this) * uiHeight.\n"
  "} SFontRectangle;\n"
  "#pragma pack(pop)\n"
  "\n"
  "/**\n"
  " * @brief Get width of row in bytes\n"
  " * @param pRectangle: Get width of row in bytes from this rectangle\n"
  " * @return number of real width in bytes.\n"
  " */\n"
  "inline unsigned char SFontRectangle_GetArrayWidth(SFontRectangle* pRectangle)\n"
  "{\n"
  "	unsigned char uiValue=0;\n"
  "	uiValue = pRectangle->uiWidth / 8;\n"
  "	if((pRectangle->uiWidth % 8) != 0)\n"
  "	{\n"
  "		uiValue++;\n"
  "	}\n"
  "	return uiValue;\n"
  "}\n"
  "#endif // SFONTRECTANGLE_DEFINED\n\n"
);

const QString CFontSourceFile::c_sSourceCommonData(
  "#pragma pack(push, 1)\n"
  "typedef struct\n"
  "{\n"
  "	unsigned char uiWidth;	//!< Width of rectangle\n"
  "	unsigned char uiHeight;	//!< Height of rectangle\n"
  "	unsigned char pData[%1];	//!< Pix map with size SFontRectangle_GetArrayWidth(this) * uiHeight.\n"
  "} SFontRectangle_%1;\n"
  "#pragma pack(pop)\n"
  "\n"
);

bool CFontSourceFile::open()
{
  bool bSuccess = false;
  if(m_oSourceFile.fileName().endsWith("cpp"))
    m_bCppMode = true;
  else
    m_bCppMode = false;
  if(m_oHeaderFile.open(QIODevice::ReadOnly))
  {
    qint64 uiHeaderGeneratorStart = 0;
    qint64 uiHeaderGeneratorEnd = 0;
    while (!m_oHeaderFile.atEnd())
    {
      QByteArray oLine = m_oHeaderFile.readLine();
      if(oLine.startsWith(c_sGeneratorStart))
      {
        uiHeaderGeneratorStart = m_oHeaderFile.pos();
      }
      else if(oLine.startsWith(c_sGeneratorEnd))
      {
        uiHeaderGeneratorEnd = m_oHeaderFile.pos();
      }
    }
    if(uiHeaderGeneratorStart && uiHeaderGeneratorEnd)
    {
      m_oHeaderFile.seek(0);
      m_sHeaderGeneratorStart = m_oHeaderFile.read(uiHeaderGeneratorStart);
      m_oHeaderFile.seek(uiHeaderGeneratorEnd);
      m_sHeaderGeneratorEnd = m_oHeaderFile.readAll();
      m_sSourceGeneratorEnd.prepend(c_sGeneratorEnd + "\n");
    }
    m_oHeaderFile.close();
  }
  if(m_oSourceFile.open(QIODevice::ReadOnly))
  {
    qint64 uiSourceGeneratorStart = 0;
    qint64 uiSourceGeneratorEnd = 0;
    while (!m_oSourceFile.atEnd())
    {
      QByteArray oLine = m_oSourceFile.readLine();
      if(oLine.startsWith(c_sGeneratorStart))
      {
        uiSourceGeneratorStart = m_oSourceFile.pos();
      }
      else if(oLine.startsWith(c_sGeneratorEnd))
      {
        uiSourceGeneratorEnd = m_oSourceFile.pos();
      }
    }
    if(uiSourceGeneratorStart && uiSourceGeneratorEnd)
    {
      m_oSourceFile.seek(0);
      m_sSourceGeneratorStart = m_oSourceFile.read(uiSourceGeneratorStart);
      m_oSourceFile.seek(uiSourceGeneratorEnd);
      m_sSourceGeneratorEnd = m_oSourceFile.readAll();
      m_sSourceGeneratorEnd.prepend(c_sGeneratorEnd + "\n");
    }
    m_oSourceFile.close();
  }
  if(m_sSourceGeneratorStart.length() == 0)
  {
    m_sSourceGeneratorStart = c_sGeneratorStart + "\n";
  }
  if(m_sHeaderGeneratorStart.length() == 0)
  {
    m_sHeaderGeneratorStart = c_sGeneratorStart + "\n";
  }
  if(m_sSourceGeneratorEnd.length() == 0)
  {
    m_sSourceGeneratorEnd = "\n" + c_sGeneratorEnd;
  }
  if(m_sHeaderGeneratorEnd.length() == 0)
  {
    m_sHeaderGeneratorEnd = "\n" + c_sGeneratorEnd;
  }
  if(m_oHeaderFile.open(QIODevice::WriteOnly))
  {
    if(m_oSourceFile.open(QIODevice::WriteOnly))
    {
      bSuccess = true;
    }
    else
    {
      m_oHeaderFile.close()  ;
    }
  }
  return bSuccess;
}

void CFontSourceFile::writeFiles()
{
  QStringList oFileNameSplitted = m_oHeaderFile.fileName().split("/");
  QString sFileName = oFileNameSplitted.at(oFileNameSplitted.size()-1);
  QByteArray oInclude = QString("#include \"" + sFileName +"\"\n\n").toLatin1();
  QList<int> oSizesList;
  for(CSignMap& rSignMap : m_oSignMaps)
  {
    rSignMap.shrinkUpperAndLower();
    rSignMap.updateRectangleSizes(oSizesList);
  }
  m_oSourceFile.write(m_sSourceGeneratorStart);
  for(int iSize : oSizesList)
  {
    QString sRectangle = generateSResource(iSize);
    m_oSourceFile.write(sRectangle.toUtf8());
  }
  m_oSourceFile.write(oInclude);
  for(CSignMap& rSignMap : m_oSignMaps)
  {
    m_oSourceFile.write(rSignMap.getSFontRectangleMap(m_bCppMode).toUtf8());
  }
  m_oSourceFile.write(m_sSourceGeneratorEnd);

  m_oHeaderFile.write(m_sHeaderGeneratorStart);
  m_oHeaderFile.write(c_sHeaderCommonData);
  for(CSignMap& rSignMap : m_oSignMaps)
  {
    m_oHeaderFile.write(rSignMap.getSFontRectangleHeader().toUtf8());
  }
  m_oHeaderFile.write(m_sHeaderGeneratorEnd);
}

void CFontSourceFile::close()
{
  m_oSourceFile.close();
  m_oHeaderFile.close();
}

QString CFontSourceFile::generateSResource(int iSize)
{
  return c_sSourceCommonData.arg(QString::number(iSize));
}
