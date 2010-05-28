#ifndef SCCOLORSPACEDATA_LABA_H
#define SCCOLORSPACEDATA_LABA_H

#include <cassert>
#include <climits>
#include <limits>
#include "sccolorprofile.h"
#include "sccolorspacedata.h"

template<typename T, eColorFormat COLORFORMAT>
class ScColorSpaceDataTempl_LabA : public ScColorSpaceData
{
protected:
	int m_LIndex;
	int m_aIndex;
	int m_bIndex;
	int m_AIndex;

public:
	ScColorSpaceDataTempl_LabA(ScColorProfile& profile);

	virtual uint alphaIndex(void) const { return m_AIndex; }
	virtual void flattenAlpha(void* dataIn, uint numElems) const;
};

template<typename T, eColorFormat COLORFORMAT>
ScColorSpaceDataTempl_LabA<T, COLORFORMAT>::ScColorSpaceDataTempl_LabA(ScColorProfile& profile)
{
	m_colorFormat = COLORFORMAT;
	m_profile     = profile;
	if (m_colorFormat == Format_LabA_8)
	{
		m_LIndex = 0;
		m_aIndex = 1;
		m_bIndex = 2;
		m_AIndex = 3;
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

template<typename T, eColorFormat COLORFORMAT>
void ScColorSpaceDataTempl_LabA<T, COLORFORMAT>::flattenAlpha(void* dataIn, uint numElems) const
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

typedef ScColorSpaceDataTempl_LabA<char, Format_LabA_8>  ScColorSpaceData_LabA8;

#endif
