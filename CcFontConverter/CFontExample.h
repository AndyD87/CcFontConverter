// FontConverter start
#pragma pack(push, 1)
/**
 * @brief Structure for describing an filled rectangular.
 *        Containing data structure of row and columns is depending on it's width.
 *        For size of a row, SFontRectangle_GetArrayWidth can be used to get it width of row in bytes.
 */
typedef struct
{
	unsigned char uiWidth;	//!< Width of rectangle
	unsigned char uiHeight;	//!< Height of rectangle
	unsigned char pData[1];	//!< Pix map with size SFontRectangle_GetArrayWidth(this) * uiHeight.
} SFontRectangle;
#pragma pack(pop)

/**
 * @brief Get width of row in bytes
 * @param pRectangle: Get width of row in bytes from this rectangle
 * @return number of real width in bytes.
 */
inline unsigned char SFontRectangle_GetArrayWidth(SFontRectangle* pRectangle)
{
	unsigned char uiValue=0;
	uiValue = pRectangle->uiWidth / 8;
	if((pRectangle->uiWidth % 8) != 0)
	{
		uiValue++;
	}
	return uiValue;
}

extern size_t Arial_12_Size;


// FontConverter end