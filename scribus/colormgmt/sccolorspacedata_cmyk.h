#ifndef SCCOLORSPACEDATA_CMYK_H
#define SCCOLORSPACEDATA_CMYK_H

#include <cassert>
#include <climits>
#include <limits>
#include "sccolorprofile.h"
#include "sccolorspacedata.h"

template<typename T, eColorFormat COLORFORMAT>
class ScColorSpaceDataTempl_CMYK : public ScColorSpaceData
{
protected:
	int m_cIndex;
	int m_mIndex;
	int m_yIndex;
	int m_kIndex;

public:
	ScColorSpaceDataTempl_CMYK(ScColorProfile& profile);

	virtual uint alphaIndex(void) const { return 0; }
	virtual void flattenAlpha(void* dataIn, uint numElems) const {};
};

template<typename T, eColorFormat COLORFORMAT>
ScColorSpaceDataTempl_CMYK<T, COLORFORMAT>::ScColorSpaceDataTempl_CMYK(ScColorProfile& profile)
{
	m_colorFormat = COLORFORMAT;
	m_profile     = profile;
	if (m_colorFormat == Format_CMYK_8 || m_colorFormat == Format_CMYK_16)
	{
		m_cIndex = 0;
		m_mIndex = 1;
		m_yIndex = 2;
		m_kIndex = 3;
	}
	else if (m_colorFormat == Format_YMCK_8 || m_colorFormat == Format_YMCK_16)
	{
		m_cIndex = 2;
		m_mIndex = 1;
		m_yIndex = 0;
		m_kIndex = 3;
	}
	else
	{
		assert(false);
	}
	if (m_profile)
	{
		assert(m_profile.colorSpace() == ColorSpace_Cmyk || m_profile.colorSpace() == ColorSpace_Cmy);
	}
};

typedef ScColorSpaceDataTempl_CMYK<unsigned char , Format_CMYK_8>  ScColorSpaceData_CMYK8;
typedef ScColorSpaceDataTempl_CMYK<unsigned short, Format_CMYK_16> ScColorSpaceData_CMYK16;
typedef ScColorSpaceDataTempl_CMYK<unsigned char , Format_YMCK_8>  ScColorSpaceData_YMCK8;
typedef ScColorSpaceDataTempl_CMYK<unsigned short, Format_YMCK_16> ScColorSpaceData_YMCK16;

#endif
