/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolorprofile.h"

ScColorProfile::ScColorProfile() : m_data(nullptr)
{
}

ScColorProfile::ScColorProfile(ScColorProfileData* data) : m_data(data)
{
}

ScColorProfile::ScColorProfile(const QSharedPointer<ScColorProfileData>& data) : m_data(data)
{
}

eColorSpaceType ScColorProfile::colorSpace() const
{
	if (m_data)
		return m_data->colorSpace();
	return ColorSpace_Unknown;
}

eColorSpaceType ScColorProfile::connectionSpace() const
{
	if (m_data)
		return m_data->connectionSpace();
	return ColorSpace_Unknown;
}

eProfileClass ScColorProfile::deviceClass() const
{
	if (m_data)
		return m_data->deviceClass();
	return Class_Unknown;
}

int ScColorProfile::channelsOfColorSpace() const
{
	if (m_data)
		return m_data->channelsOfColorSpace();
	return 0;
}

int ScColorProfile::channelsOfConnectionSpace() const
{
	if (m_data)
		return m_data->channelsOfConnectionSpace();
	return 0;
}

QString ScColorProfile::dataHash() const
{
	if (m_data)
		return m_data->dataHash();
	return QString();
}

bool ScColorProfile::isSuitableForOutput() const
{
	if (m_data)
		return m_data->isSuitableForOutput();
	return false;
}

QString ScColorProfile::profilePath() const
{
	if (m_data)
		return m_data->path();
	return QString();
}

QString ScColorProfile::productDescription() const
{
	if (m_data)
		return m_data->productDescription();
	return QString();
}

bool ScColorProfile::save(QByteArray& profileData) const
{
	if (m_data)
		return m_data->save(profileData);
	return false;
}

bool ScColorProfile::operator==(const ScColorProfile& other) const
{
	return m_data == other.m_data;
}
