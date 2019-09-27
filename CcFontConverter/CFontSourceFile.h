#ifndef CFONTSOURCEFILE_H
#define CFONTSOURCEFILE_H

#include <QFile>
#include <QList>
#include <QByteArray>
#include "CSignMap.h"

class CFontSourceFile
{
public:
  CFontSourceFile();
  void setHeaderFilePath(const QString& sPath)
   { m_oHeaderFile.setFileName(sPath); }
  void setSourceFilePath(const QString& sPath)
   { m_oSourceFile.setFileName(sPath); }
  void addSignMap(const CSignMap& oSignMap)
   { m_oSignMaps.append(oSignMap);}

  bool open();
  void writeFiles();
  void close();
private:

private:
  QFile m_oSourceFile;
  QFile m_oHeaderFile;
  QByteArray m_sSourceGeneratorStart;
  QByteArray m_sSourceGeneratorEnd;
  QByteArray m_sHeaderGeneratorStart;
  QByteArray m_sHeaderGeneratorEnd;
  QList<CSignMap> m_oSignMaps;

  static const QByteArray c_sGeneratorStart;
  static const QByteArray c_sGeneratorEnd;
  static const QByteArray c_sHeaderCommonData;
};

#endif // CFONTSOURCEFILE_H
