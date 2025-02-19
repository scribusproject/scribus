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
public:
	explicit ScColorSpaceDataTempl_RGB(ScColorProfile& profile);

	uint alphaIndex(void) const override { return 0; }
	void flattenAlpha(void* dataIn, uint numElems) const override {};

protected:
	int m_rIndex { 0 };
	int m_gIndex { 1 };
	int m_bIndex { 2 };
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

using ScColorSpaceData_RGB8		= ScColorSpaceDataTempl_RGB<unsigned char, Format_RGB_8>;
using ScColorSpaceData_RGB16	= ScColorSpaceDataTempl_RGB<unsigned short, Format_RGB_16>;

#endif
