#ifndef SCCOLORSPACEDATA_RGBA_H
#define SCCOLORSPACEDATA_RGBA_H

#include <cassert>
#include <climits>
#include <limits>
#include "sccolorprofile.h"
#include "sccolorspacedata.h"

template<typename T, eColorFormat COLORFORMAT>
class ScColorSpaceDataTempl_RGBA : public ScColorSpaceData
{
protected:
	int m_rIndex;
	int m_gIndex;
	int m_bIndex;
	int m_aIndex;

public:
	ScColorSpaceDataTempl_RGBA(ScColorProfile& profile);

	virtual uint alphaIndex(void) const { return m_aIndex; }
	virtual void flattenAlpha(void* dataIn, uint numElems) const;
};

template<typename T, eColorFormat COLORFORMAT>
ScColorSpaceDataTempl_RGBA<T, COLORFORMAT>::ScColorSpaceDataTempl_RGBA(ScColorProfile& profile)
{
	m_colorFormat = COLORFORMAT;
	m_profile     = profile;
	if (m_colorFormat == Format_RGBA_8 || m_colorFormat == Format_RGBA_16)
	{
		m_rIndex = 0;
		m_gIndex = 1;
		m_bIndex = 2;
		m_aIndex = 3;
	}
	else if (m_colorFormat == Format_ARGB_8 || m_colorFormat == Format_ARGB_16)
	{
		m_rIndex = 1;
		m_gIndex = 2;
		m_bIndex = 3;
		m_aIndex = 0;
	}
	else if (m_colorFormat == Format_BGRA_8 || m_colorFormat == Format_BGRA_16)
	{
		m_rIndex = 2;
		m_gIndex = 1;
		m_bIndex = 0;
		m_aIndex = 3;
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

template<typename T, eColorFormat COLORFORMAT>
void ScColorSpaceDataTempl_RGBA<T, COLORFORMAT>::flattenAlpha(void* dataIn, uint numElems) const
{
	T* data   = ((T*) dataIn) + m_aIndex;
	T  nLimit = std::numeric_limits<T>::max();
	while (numElems > 0)
	{
		*data = nLimit;
		data += 4;
		--numElems;
	};
};

typedef ScColorSpaceDataTempl_RGBA<unsigned char , Format_RGBA_8>  ScColorSpaceData_RGBA8;
typedef ScColorSpaceDataTempl_RGBA<unsigned short, Format_RGBA_16> ScColorSpaceData_RGBA16;
typedef ScColorSpaceDataTempl_RGBA<unsigned char , Format_ARGB_8>  ScColorSpaceData_ARGB8;
typedef ScColorSpaceDataTempl_RGBA<unsigned short, Format_ARGB_16> ScColorSpaceData_ARGB16;
typedef ScColorSpaceDataTempl_RGBA<unsigned char , Format_BGRA_8>  ScColorSpaceData_BGRA8;
typedef ScColorSpaceDataTempl_RGBA<unsigned short, Format_BGRA_16> ScColorSpaceData_BGRA16;

#endif
