#ifndef CFONTSOURCEFILE_H
#define CFONTSOURCEFILE_H

#include <QFile>
#include <QList>
#include <QByteArray>
#include "CSignMap.h"

class CFontSourceFile
{
public:
  CFontSourceFile() = default;
  void setHeaderFilePath(const QString& sPath)
   { m_oHeaderFile.setFileName(sPath); }
  void setSourceFilePath(const QString& sPath)
   { m_oSourceFile.setFileName(sPath); }
  void addSignMap(const CSignMap& oSignMap)
   { m_oSignMaps.append(oSignMap);}

  bool open();
  void writeFiles();
  void close();
  void setCppMode(bool bCppMode)
    { m_bCppMode = bCppMode; }
private:
  QString generateSResource(int iSize);
private:
  QFile m_oSourceFile;
  QFile m_oHeaderFile;
  bool m_bCppMode = false;
  QByteArray m_sSourceGeneratorStart;
  QByteArray m_sSourceGeneratorEnd;
  QByteArray m_sHeaderGeneratorStart;
  QByteArray m_sHeaderGeneratorEnd;
  QList<CSignMap> m_oSignMaps;

  static const QByteArray c_sGeneratorStart;
  static const QByteArray c_sGeneratorEnd;
  static const QByteArray c_sHeaderCommonData;
  static const QString c_sSourceCommonData;
  static const QString c_sSourceCommonData0;
};

#endif // CFONTSOURCEFILE_H
