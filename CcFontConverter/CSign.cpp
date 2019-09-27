#include "CSign.h"
#include <string.h>
#include <cstring>

CSign::CSign(uint32 uiId, uint32 uiWidth, uint32 uiHeight) :
  m_uiId(uiId),
  m_uiWidth(uiWidth),
  m_uiHeight(uiHeight)
{
  CCNEWARRAY(m_pData, uint8, static_cast<uint64>(m_uiWidth) * m_uiHeight);
  memset(m_pData, 0, sizeof(static_cast<uint64>(m_uiWidth) * m_uiHeight));
}

CSign::~CSign()
{
  CCDELETEARR(m_pData);
}

CSign& CSign::operator=(const CSign& oToCopy)
{
  if(&oToCopy != this)
  {
    clear();
    init(oToCopy.getId(), oToCopy.getWidth(), oToCopy.getHeight());
    memcpy(m_pData, oToCopy.m_pData, static_cast<uint64>(m_uiWidth) * m_uiHeight);
  }
  return *this;
}

void CSign::setPixel(uint32 uiX, uint32 uiY, uint8 uiAlpha)
{
  if(uiX < m_uiWidth && uiY < m_uiHeight)
  {
    m_pData[(uiY * m_uiWidth) + uiX ] = uiAlpha;
  }
}

void CSign::init(uint32 uiId, uint32 uiWidth, uint32 uiHeight)
{
  CCDELETEARR(m_pData);
  m_uiId = uiId;
  m_uiWidth = uiWidth;
  m_uiHeight = uiHeight;
  CCNEWARRAY(m_pData, uint8, static_cast<uint64>(m_uiWidth) * m_uiHeight);
  memset(m_pData, 0, sizeof(static_cast<uint64>(m_uiWidth) * m_uiHeight));
}

QString CSign::getSFontRectangle() const
{
  QString sOutput = "{" + QString::number(m_uiWidth) + "," + QString::number(m_uiHeight) + ",{";
  if(m_uiWidth > 0)
  {
    for(size_t iY=0; iY<m_uiHeight; iY++)
    {
      uint8 uiCurrentByte = 0;
      for(size_t iX=0; iX < m_uiWidth; iX++)
      {
        if(getPixel(iX, iY))
        {
          uiCurrentByte |= 0x1 << (iX%8);
        }
        if(iX % 8 == 8-1)
        {
          sOutput += "0x" + QString::number(uiCurrentByte, 16);
          if (iX != m_uiWidth-1)
          {
            sOutput += ",";
          }
          uiCurrentByte = 0;
        }
        else if(iX == m_uiWidth-1)
        {
          sOutput += "0x" + QString::number(uiCurrentByte, 16);
        }
      }
      if (iY + 1 < m_uiHeight)
      {
        sOutput += ",";
      }
    }
  }
  sOutput += "}};";
  return sOutput;
}

uint32 CSign::getFirstUpper()
{
  uint32 iPos = m_uiHeight-1;
  for (uint32 iY = 0; iY < m_uiHeight && iPos == m_uiHeight-1; iY++)
  {
    for (uint32 iX = 0; iX < m_uiWidth; iX++)
    {
      if(getPixel(iX, iY) != 0)
      {
        iPos = iY;
        break;
      }
    }
  }
  return iPos;
}

uint32 CSign::getLastLower()
{
  uint32 iPos = 0;
  for (uint32 iY = m_uiHeight; iY > 0 && iPos == 0; iY--)
  {
    for (uint32 iX = 0; iX < m_uiWidth; iX++)
    {
      if (getPixel(iY-1, iX) != 0)
      {
        iPos = iY;
        break;
      }
    }
  }
  return iPos;
}

uint32 CSign::getFirstLeft()
{
  uint32 iPos = m_uiWidth-1;
  for (uint32 iY = 0; iY < m_uiHeight; iY++)
  {
    for (uint32 iX = 0; iX < m_uiWidth; iX++)
    {
      if(getPixel(iX, iY) != 0 &&
         iX < iPos)
      {
        iPos = iX;
        break;
      }
    }
  }
  return iPos;
}

uint32 CSign::getLastRight()
{
  uint32 iPos = 0;
  for (uint32 iY = 0; iY < m_uiHeight; iY++)
  {
    for (uint32 iX = 0; iX < m_uiWidth; iX++)
    {
      if(getPixel(iX, iY) != 0 &&
         iX > iPos)
      {
        iPos = iX;
      }
    }
  }
  return iPos;
}

void CSign::cutRows(uint32 uiBegin, uint32 uiEnd)
{
  if(uiEnd > uiBegin)
  {
    uint32 uiNewHeight  = uiEnd - uiBegin ;
    uiNewHeight++;
    CCNEWARRAYTYPE(pNewData, uint8, static_cast<uint64>(m_uiWidth)*uiNewHeight);
    memcpy(pNewData,
           m_pData + (static_cast<uint64>(m_uiWidth)*uiBegin),
           static_cast<uint64>(m_uiWidth)*uiNewHeight);
    CCDELETEARR(m_pData);
    m_pData = pNewData;
    m_uiHeight = uiNewHeight;
  }
  else
  {
    CCDELETEARR(m_pData);
    m_uiWidth = 0;
  }
}

void CSign::cutColumns(uint32 uiBegin, uint32 uiEnd)
{
  if(uiEnd > uiBegin)
  {
    uint32 uiNewWidth  = uiEnd - uiBegin;
    uiNewWidth ++;
    CCNEWARRAYTYPE(pNewData, uint8, static_cast<uint64>(uiNewWidth)*m_uiHeight);

    for (uint32 iY = 0; iY < m_uiHeight; iY++)
    {
      for (uint32 iX = uiBegin; iX < uiEnd + 1; iX++)
      {
        pNewData[(iY*uiNewWidth) + (iX-uiBegin)] = getPixel(iX, iY);
      }
    }

    CCDELETEARR(m_pData);
    m_pData = pNewData;
    m_uiWidth = uiNewWidth;
  }
  else
  {
    CCDELETEARR(m_pData);
    m_uiWidth = 0;
  }
}

void CSign::cutColumns()
{
  cutColumns(getFirstLeft(), getLastRight());
}

uint32 CSign::getBytesFromWidth(uint32 iWidth)
{
  uint32 iRet = iWidth / 8;
  if((iWidth % 8) > 0)
  {
    iRet++;
  }
  return iRet;
}

void CSign::clear()
{
  CCDELETEARR(m_pData);
  m_uiId = 0;
  m_uiWidth = 0;
  m_uiHeight = 0;
}
