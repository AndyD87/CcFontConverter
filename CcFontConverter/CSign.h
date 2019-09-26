#ifndef CSIGN_H
#define CSIGN_H

#include <QString>
#include "CcBase.h"

class CSign
{
public:
  CSign() = default;
  CSign(uint32 uiId, uint32 uiWidth, uint32 uiHeight);
  ~CSign();
  void SetPixel(uint32 uiX, uint32 uiY, uint8 uiAlpha);

  void init(uint32 uiId, uint32 uiWidth, uint32 uiHeight);
  QString getSRectangle() const;
  uint8& getPixel(size_t uiX, size_t uiY) const
    { return m_pData[uiX + (uiY * m_uiWidth)]; }
  uint32 getId() const
    {return m_uiId; }
  uint32 getFirstUpper();
  uint32 getLastLower();
  uint32 getFirstLeft();
  uint32 getLastRight();
  uint32 getHeight() const
    { return m_uiHeight; }
  uint32 getWidth() const
    { return m_uiWidth; }

  void cutRows(uint32 uiBegin, uint32 uiEnd);
  void cutColumns(uint32 uiBegin, uint32 uiEnd);
  void cutColumns();

private:
  static uint32 getBytesFromWidth(uint32 iWidth);
private:
  uint32 m_uiId     = 0;
  uint32 m_uiWidth  = 0;
  uint32 m_uiHeight = 0;
  uint8* m_pData    = nullptr;
};

#endif // CSIGN_H
