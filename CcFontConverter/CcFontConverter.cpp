#include "CcFontConverter.h"
#include "ui_CcFontConverter.h"
#include <QFontDialog>
#include <QPushButton>
#include <QPainter>
#include <QImage>

#include "CcBase.h"
#include "CSignMap.h"

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
  m_pUi->guiEditFont->setText(m_oFont.toString());
  connect(m_pUi->guiButtonFont, &QPushButton::clicked, this, &CcFontConverter::onFontClicked);
  connect(m_pUi->guiButtonGenerate, &QPushButton::clicked, this, &CcFontConverter::onGenerateClicked);
}

CcFontConverter::~CcFontConverter()
{
  CCDELETE(m_pUi);
}

void CcFontConverter::onGenerateClicked(bool bChecked)
{
  CCUNUSED(bChecked);
  CSignMap oSignMap(static_cast<size_t>(m_oFont.pointSize()));
  int uiWidth = m_oFont.pointSize()*2;
  int uiHeight = m_oFont.pointSize()*2;
  for(char i=0; i >= 0; i++)
  {
    QRect oPictureRect(0,0,uiWidth, uiHeight);
    QImage oPicture(QSize(uiWidth, uiHeight), QImage::Format_ARGB32);
    QPainter* pPainter = new QPainter(&oPicture);
    pPainter->setBrush(QBrush(Qt::white));
    pPainter->setPen(Qt::white);
    pPainter->drawRect(oPictureRect);
    pPainter->setPen(Qt::black);
    pPainter->setFont(m_oFont);
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
        oSignMap[static_cast<size_t>(i)].SetPixel(
              x,
              y,
              static_cast<uint8>(~(oColor&0xff)));
      }
    }
  }
  m_pUi->guiEditSources->setText(oSignMap.getSRectangleMap());
}

void CcFontConverter::onFontClicked(bool bChecked)
{
  bool bOk;
  CCUNUSED(bChecked);
  m_oFont = QFontDialog::getFont(&bOk, m_oFont, this);
  m_pUi->guiEditFont->setText(m_oFont.toString());
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
