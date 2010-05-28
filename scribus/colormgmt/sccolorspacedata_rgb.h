#ifndef SCCOLORSPACEDATA_RGB_H
#define SCCOLORSPACEDATA_RGB_H

#include <cassert>
#include <climits>
#include <limits>
#include "sccolorprofile.h"
#include "sccolorspacedata.h"

template<typename T, eColorFormat COLORFORMAT>
class ScColorSpaceDataTempl_RGB : public ScColorSpaceData
{
protected:
	int m_rIndex;
	int m_gIndex;
	int m_bIndex;

public:
	ScColorSpaceDataTempl_RGB(ScColorProfile& profile);

	virtual uint alphaIndex(void) const { return 0; }
	virtual void flattenAlpha(void* dataIn, uint numElems) const {};
};

template<typename T, eColorFormat COLORFORMAT>
ScColorSpaceDataTempl_RGB<T, COLORFORMAT>::ScColorSpaceDataTempl_RGB(ScColorProfile& profile)
{
	m_colorFormat = COLORFORMAT;
	m_profile     = profile;
	if (m_colorFormat == Format_RGB_8 || m_colorFormat == Format_RGB_16)
	{
		m_rIndex = 0;
		m_gIndex = 1;
		m_bIndex = 2;
	}
	else
	{
		assert(false);
	}
	if (m_profile)
	{
		assert(m_profile.colorSpace() == ColorSpace_Rgb);
	}
};

typedef ScColorSpaceDataTempl_RGB<unsigned char , Format_RGB_8>  ScColorSpaceData_RGB8;
typedef ScColorSpaceDataTempl_RGB<unsigned short, Format_RGB_16> ScColorSpaceData_RGB16;

#endif
