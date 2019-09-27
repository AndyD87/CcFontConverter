#include "CcFontConverter.h"
#include "ui_CcFontConverter.h"
#include <QFontDialog>
#include <QPushButton>
#include <QPainter>
#include <QFileDialog>
#include <QImage>

#include "CcBase.h"
#include "CSignMap.h"
#include "CFontSourceFile.h"

const char CcFontConverter::c_pUnprintable[] =
{
  1, 2, 3, 4, 5, 6, 7, 8, 11, 14, 15, 16, 17, 18
};
const size_t CcFontConverter::c_uiUnprintableSize = sizeof(CcFontConverter::c_pUnprintable) / sizeof(CcFontConverter::c_pUnprintable[0]);

CcFontConverter::CcFontConverter(QWidget *parent) :
  QMainWindow(parent),
  m_pUi(new Ui::CcFontConverter),
  m_oFont("Arial", 12)
{
  m_pUi->setupUi(this);
  connect(m_pUi->guiButtonFont, &QPushButton::clicked, this, &CcFontConverter::onFontClicked);
  connect(m_pUi->guiButtonGenerate, &QPushButton::clicked, this, &CcFontConverter::onGenerateClicked);
  connect(m_pUi->guiButtonGenerateFiles, &QPushButton::clicked, this, &CcFontConverter::onGenerateFilesClicked);
  checkFonts();
  checkSizes();
}

CcFontConverter::~CcFontConverter()
{
  CCDELETE(m_pUi);
}

void CcFontConverter::onGenerateClicked(bool bChecked)
{
  CCUNUSED(bChecked);
  QList<uint32> oSizes = getSizes();
  QList<QString> oFonts = getFonts();
  m_pUi->guiEditSources->setPlainText("");
  for(QString& sFamily : oFonts)
  {
    for(uint32 uiSize : oSizes)
    {
      generate(sFamily, uiSize, m_pUi->guiEditPrefix->text().trimmed());
    }
  }
}

void CcFontConverter::onGenerateFilesClicked(bool bChecked)
{
  statusReset();
  CCUNUSED(bChecked);
  QStringList oFileFilter;
  oFileFilter.append("C-Files (*.c *.h)");
  oFileFilter.append("CPP-Files (*.cpp *.h)");
  QFileDialog oFileDialog;
  oFileDialog.setFileMode(QFileDialog::AnyFile);
  oFileDialog.setNameFilters(oFileFilter);
  oFileDialog.setViewMode(QFileDialog::Detail);
  if(oFileDialog.exec())
  {
    QString sSourceFilePath;
    QString sHeaderFilePath;
    QStringList oFileNames = oFileDialog.selectedFiles();
    QString sNameFilter = oFileDialog.selectedNameFilter();
    if(sNameFilter == oFileFilter[1])
    {
      if(oFileNames.size() == 1)
      {
        // Cpp Format
        QString sFileName = oFileNames[0];
        if(sFileName.endsWith(".h"))
        {
          sHeaderFilePath = sFileName;
          sSourceFilePath = sFileName.mid(0, sFileName.length() - 2);
          sSourceFilePath += ".cpp";
        }
        else if(sFileName.endsWith(".cpp"))
        {
          sSourceFilePath = sFileName;
          sHeaderFilePath = sFileName.mid(0, sFileName.length() - 4);
          sHeaderFilePath += ".h";
        }
        else
        {
          sSourceFilePath = sFileName + ".cpp";
          sHeaderFilePath = sFileName + ".h";
        }
      }
    }
    else
    {
      // C Format
      if(oFileNames.size() == 1)
      {
        QString sFileName = oFileNames[0];
        if(sFileName.endsWith(".h"))
        {
          sHeaderFilePath = sFileName;
          sSourceFilePath = sFileName.mid(0, sFileName.length() - 2);
          sSourceFilePath += ".c";
        }
        else if(sFileName.endsWith(".c"))
        {
          sSourceFilePath = sFileName;
          sHeaderFilePath = sFileName.mid(0, sFileName.length() - 2);
          sHeaderFilePath += ".h";
        }
        else
        {
          sSourceFilePath = sFileName + ".c";
          sHeaderFilePath = sFileName + ".h";
        }
      }
    }

    CFontSourceFile oFontFile;
    oFontFile.setHeaderFilePath(sHeaderFilePath);
    oFontFile.setSourceFilePath(sSourceFilePath);
    if(oFontFile.open())
    {
      statusWriteLine("Sourcefiles found and opened");
      QList<uint32> oSizes = getSizes();
      QList<QString> oFonts = getFonts();
      for(QString& sFamily : oFonts)
      {
        for(uint32 uiSize : oSizes)
        {
          statusWriteLine(sFamily + " " + QString::number(uiSize) + " created");
          oFontFile.addSignMap(generateSignMap(sFamily, uiSize, m_pUi->guiEditPrefix->text().trimmed()));
        }
      }
      statusWriteLine("Write files");
      oFontFile.writeFiles();
      oFontFile.close();
    }
    else
    {
      statusWriteLine("Sourcefiles could not be opened");
    }
  }
}

CSignMap CcFontConverter::generateSignMap(const QString& sFamily, uint32 uiSize, const QString& sPrefix)
{
  QFont oFont(sFamily, static_cast<int>(uiSize));
  QString sVarName = sPrefix;
  if(sVarName.length()) sVarName += "_";
  sVarName += sFamily + "_";
  sVarName.replace(" ", "");
  sVarName += QString::number(uiSize);
  CSignMap oSignMap(sVarName, static_cast<size_t>(oFont.pointSize()));
  int uiWidth = oFont.pointSize()*2;
  int uiHeight = oFont.pointSize()*2;
  for(char i=0; i >= 0; i++)
  {
    QRect oPictureRect(0,0,uiWidth, uiHeight);
    QImage oPicture(QSize(uiWidth, uiHeight), QImage::Format_ARGB32);
    QPainter* pPainter = new QPainter(&oPicture);
    pPainter->setBrush(QBrush(Qt::white));
    pPainter->setPen(Qt::white);
    pPainter->drawRect(oPictureRect);
    pPainter->setPen(Qt::black);
    pPainter->setFont(oFont);
    try
    {
      QString sString(i);
      // search for unprintable char
      if(!isUnprintable(i))
      {
        pPainter->drawText(oPictureRect, sString);
      }
    }
    catch (...)
    {
      // Ignore unknown chars
    }
    delete pPainter;
    oSignMap[static_cast<size_t>(i)].init(
          static_cast<uint32>(i),
          static_cast<uint32>(uiWidth),
          static_cast<uint32>(uiHeight));
    for(uint32 y=0; y<static_cast<uint32>(uiHeight); y++)
    {
      for(uint32 x=0; x < static_cast<uint32>(uiWidth); x++)
      {
        QRgb oColor = oPicture.pixel(static_cast<int>(x), static_cast<int>(y));
        oSignMap[static_cast<size_t>(i)].setPixel(
              x,
              y,
              static_cast<uint8>(~(oColor&0xff)));
      }
    }
  }
  return oSignMap;
}

void CcFontConverter::generate(const QString& sFamily, uint32 uiSize, const QString& sPrefix)
{
  CSignMap oSignMap = generateSignMap(sFamily, uiSize, sPrefix);
  m_pUi->guiEditSources->appendPlainText(oSignMap.getSFontRectangleMap());
}

void CcFontConverter::onFontClicked(bool bChecked)
{
  bool bOk;
  CCUNUSED(bChecked);
  m_oFont = QFontDialog::getFont(&bOk, m_oFont, this);
  checkFonts();
  checkSizes();
}

bool CcFontConverter::isUnprintable(char iChar)
{
  bool bUnprintable = false;
  for(size_t uiI=0; uiI<c_uiUnprintableSize; uiI++)
  {
    if(c_pUnprintable[uiI] == iChar)
    {
      bUnprintable = true;
      break;
    }
  }
  return  bUnprintable;
}

QList<QString> CcFontConverter::getFonts()
{
  QList<QString> oFonts;
  QString sFonts = m_pUi->guiEditFonts->text();
  QStringList sFontStrings = sFonts.split(",");
  for(QString& sFont : sFontStrings)
  {
    sFont = sFont.trimmed();
    if(sFont.length())
      oFonts.append(sFont);
  }
  return oFonts;
}

void CcFontConverter::checkFonts()
{
  QString sFont = m_oFont.family();
  QList<QString> oFonts = getFonts();
  if(!oFonts.contains(sFont))
  {
    if(oFonts.size() == 0)
    {
      m_pUi->guiEditFonts->setText(sFont);
    }
    else
    {
      m_pUi->guiEditFonts->setText(m_pUi->guiEditFonts->text() + ", " + sFont);
    }
  }
}

QList<uint32> CcFontConverter::getSizes()
{
  QList<uint32> oSizes;
  QString sSizes = m_pUi->guiEditSizes->text();
  QStringList oSizeStrings = sSizes.split(",");
  for(QString& sSize : oSizeStrings)
  {
    bool bOk;
    uint32 uiSize = sSize.trimmed().toUInt(&bOk);
    if(bOk)
    {
      oSizes.append(uiSize);
    }
  }
  return oSizes;
}

void CcFontConverter::checkSizes()
{
  uint32 uiFontSize = static_cast<uint32>(m_oFont.pointSize());
  QList<uint32> oSizes = getSizes();
  if(!oSizes.contains(uiFontSize))
  {
    if(oSizes.size() == 0)
    {
      m_pUi->guiEditSizes->setText(QString::number(uiFontSize));
    }
    else
    {
      m_pUi->guiEditSizes->setText(m_pUi->guiEditSizes->text() + ", " + QString::number(uiFontSize));
    }
  }
}

void CcFontConverter::statusReset()
{
  m_pUi->guiEditSources->setPlainText("");
}

void CcFontConverter::statusWriteLine(const QString& sStatus)
{
  m_pUi->guiEditSources->appendPlainText(sStatus +"\n");
}
