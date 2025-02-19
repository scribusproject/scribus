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
public:
	explicit ScColorSpaceDataTempl_LabA(ScColorProfile& profile);

	uint alphaIndex(void) const override { return m_AIndex; }
	void flattenAlpha(void* dataIn, uint numElems) const override;

protected:
	int m_LIndex { 0 };
	int m_aIndex { 1 };
	int m_bIndex { 2 };
	int m_AIndex { 3 };
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

using ScColorSpaceData_LabA8 = ScColorSpaceDataTempl_LabA<char, Format_LabA_8>;

#endif
