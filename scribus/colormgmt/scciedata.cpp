/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scciedata.h"

// Init the singleton's "self" address to NULL
ScCIEData* ScCIEData::m_instance = nullptr;

// Singleton's public constructor
const ScCIEData& ScCIEData::instance()
{
	if (!ScCIEData::m_instance)
		ScCIEData::m_instance = new ScCIEData();
	return *ScCIEData::m_instance;
}

// Singleton's public destructor
void ScCIEData::destroy()
{
	if (ScCIEData::m_instance)
	{
		delete ScCIEData::m_instance;
		ScCIEData::m_instance = nullptr;
	}
}

const ScCIEIlluminant& ScCIEData::cieIlluminant(eIlluminant illuminant) const
{
	if (illuminant == Illuminant_D50)
		return m_d50Illuminant;
	if (illuminant == Illuminant_D65)
		return m_d65Illuminant;
	return m_d50Illuminant;
}

const ScCIEObserver& ScCIEData::cieObserver(eObserver observer) const
{
	if (observer == Observer_2deg)
		return m_cie1931Observer;
	if (observer == Observer_10deg)
		return m_cie1964Observer;
	return m_cie1931Observer;
}
