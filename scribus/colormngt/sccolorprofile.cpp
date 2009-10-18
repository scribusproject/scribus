/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolorprofile.h"

ScColorProfile::ScColorProfile() : m_data(NULL)
{
}

ScColorProfile::ScColorProfile(ScColorProfileData* data) : m_data(data)
{
}

icColorSpaceSignature ScColorProfile::colorSpace() const
{
	return m_data->colorSpace();
}

icProfileClassSignature ScColorProfile::deviceClass() const
{
	return m_data->deviceClass();
}

QString ScColorProfile::profilePath() const
{
	return m_data->path();
}

QString ScColorProfile::productDescription() const
{
	return m_data->productDescription();
}

bool ScColorProfile::operator==(const ScColorProfile& other) const
{
	return m_data == other.m_data;
}