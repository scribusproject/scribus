#ifndef SCCOLORSPACEDATA_LABD_H
#define SCCOLORSPACEDATA_LABD_H

#include <cassert>
#include <climits>
#include <limits>
#include "sccolorprofile.h"
#include "sccolorspacedata.h"

template<typename T, eColorFormat COLORFORMAT>
class ScColorSpaceDataTempl_LabDbl : public ScColorSpaceData
{
protected:
	int m_LIndex;
	int m_aIndex;
	int m_bIndex;

public:
	ScColorSpaceDataTempl_LabDbl(ScColorProfile& profile);

	virtual uint alphaIndex(void) const { return 0; }
	virtual void flattenAlpha(void* dataIn, uint numElems) const {};
};

template<typename T, eColorFormat COLORFORMAT>
ScColorSpaceDataTempl_LabDbl<T, COLORFORMAT>::ScColorSpaceDataTempl_LabDbl(ScColorProfile& profile)
{
	m_colorFormat = COLORFORMAT;
	m_profile     = profile;
	if (m_colorFormat == Format_Lab_Dbl)
	{
		m_LIndex = 0;
		m_aIndex = 1;
		m_bIndex = 2;
	}
	else
	{
		assert(false);
	}
	if (m_profile)
	{
		assert(m_profile.colorSpace() == ColorSpace_Lab);
	}
};

typedef ScColorSpaceDataTempl_LabDbl<char, Format_Lab_Dbl>  ScColorSpaceData_LabD;

#endif
