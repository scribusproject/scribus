#ifndef SCCOLORSPACEDATA_GRAY_H
#define SCCOLORSPACEDATA_GRAY_H

#include <cassert>
#include <climits>
#include <limits>
#include "sccolorprofile.h"
#include "sccolorspacedata.h"

template<typename T, eColorFormat COLORFORMAT>
class ScColorSpaceDataTempl_Gray : public ScColorSpaceData
{
public:
	ScColorSpaceDataTempl_Gray(ScColorProfile& profile);

	virtual uint alphaIndex(void) const { return 0; }
	virtual void flattenAlpha(void* dataIn, uint numElems) const {};
};

template<typename T, eColorFormat COLORFORMAT>
ScColorSpaceDataTempl_Gray<T, COLORFORMAT>::ScColorSpaceDataTempl_Gray(ScColorProfile& profile)
{
	m_colorFormat = COLORFORMAT;
	m_profile     = profile;
	assert(m_colorFormat == Format_GRAY_8 || m_colorFormat == Format_GRAY_16);
	if (m_profile)
	{
		assert(m_profile.colorSpace() == ColorSpace_Gray);
	}
};

typedef ScColorSpaceDataTempl_Gray<unsigned char , Format_GRAY_8>  ScColorSpaceData_GRAY8;
typedef ScColorSpaceDataTempl_Gray<unsigned short, Format_GRAY_16> ScColorSpaceData_GRAY16;

#endif
