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

void CSign::SetPixel(uint32 uiX, uint32 uiY, uint8 uiAlpha)
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

QString CSign::getSRectangle() const
{
  QString sOutput = "{" + QString::number(m_uiWidth) + "," + QString::number(m_uiHeight) + ",{";
  if(m_uiWidth > 0)
  {
    for(size_t iY=0; iY<m_uiHeight; iY++)
    {
      uint8 uiCurrentByte = 0;
      for(size_t iX=0; iX < m_uiWidth; iX++)
      {
        if(getPixel(iY, iX))
        {
          uiCurrentByte |= 0x1 << (iX%8);
        }
        if(iX % 8 == 8-1)
        {
          sOutput += "0x" + QString::number(uiCurrentByte, 16);
          if (iX+1 < m_uiWidth)
          {
            sOutput += ",";
          }
        }
        else if(iX < m_uiWidth-1)
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
  uint32 iPos = UINT32_MAX;
  for (uint32 iY = 0; iY < m_uiHeight && iPos == UINT32_MAX; iY++)
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
  uint32 iPos = UINT32_MAX;
  for (uint32 iY = m_uiHeight; iY > 0 && iPos == UINT32_MAX; iY--)
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
  uint32 iPos = UINT32_MAX;
  for (uint32 iY = 0; iY < m_uiHeight && iPos == UINT32_MAX; iY++)
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
  uint32 iPos = UINT32_MAX;
  for (uint32 iY = 0; iY < m_uiHeight && iPos == UINT32_MAX; iY++)
  {
    for (uint32 iX = 0; iX < m_uiWidth; iX++)
    {
      if(getPixel(iX, iY) != 0 &&
         iX > iPos)
      {
        iPos = iX;
        break;
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
  cutColumns(getFirstLeft(), getLastLower());
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
