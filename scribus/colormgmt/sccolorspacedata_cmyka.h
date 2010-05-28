#ifndef SCCOLORSPACEDATA_CMYKA_H
#define SCCOLORSPACEDATA_CMYKA_H

#include <cassert>
#include <climits>
#include <limits>
#include "sccolorprofile.h"
#include "sccolorspacedata.h"

template<typename T, eColorFormat COLORFORMAT>
class ScColorSpaceDataTempl_CMYKA : public ScColorSpaceData
{
protected:
	int m_cIndex;
	int m_mIndex;
	int m_yIndex;
	int m_kIndex;
	int m_aIndex;

public:
	ScColorSpaceDataTempl_CMYKA(ScColorProfile& profile);

	virtual uint alphaIndex(void) const { return m_aIndex; }
	virtual void flattenAlpha(void* dataIn, uint numElems) const;
};

template<typename T, eColorFormat COLORFORMAT>
ScColorSpaceDataTempl_CMYKA<T, COLORFORMAT>::ScColorSpaceDataTempl_CMYKA(ScColorProfile& profile)
{
	m_colorFormat = COLORFORMAT;
	m_profile     = profile;
	if (m_colorFormat == Format_CMYKA_8 || m_colorFormat == Format_CMYKA_16)
	{
		m_cIndex = 0;
		m_mIndex = 1;
		m_yIndex = 2;
		m_kIndex = 3;
		m_aIndex = 4;
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

template<typename T, eColorFormat COLORFORMAT>
void ScColorSpaceDataTempl_CMYKA<T, COLORFORMAT>::flattenAlpha(void* dataIn, uint numElems) const
{
	T* data   = ((T*) dataIn) + m_aIndex;
	T  nLimit = std::numeric_limits<T>::max();
	while (numElems > 0)
	{
		*data = nLimit;
		data += 5;
		--numElems;
	};
};

typedef ScColorSpaceDataTempl_CMYKA<unsigned char , Format_CMYKA_8>  ScColorSpaceData_CMYKA8;
typedef ScColorSpaceDataTempl_CMYKA<unsigned short, Format_CMYKA_16> ScColorSpaceData_CMYKA16;

#endif
