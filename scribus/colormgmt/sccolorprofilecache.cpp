/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "sccolorprofilecache.h"

void ScColorProfileCache::addProfile(const ScColorProfile& profile)
{
	QString path = profile.profilePath();
	if (path.isEmpty())
		return;

	QMap<QString, QWeakPointer<ScColorProfileData> >::iterator iter = m_profileMap.find(path);
	if (iter != m_profileMap.end())
	{
		QSharedPointer<ScColorProfileData> strongRef = iter.value().toStrongRef();
		if (strongRef) return;
	}

	m_profileMap.insert(path, profile.weakRef());
}

void ScColorProfileCache::removeProfile(const QString& profilePath)
{
	m_profileMap.remove(profilePath);
}

void ScColorProfileCache::removeProfile(const ScColorProfile& profile)
{
	m_profileMap.remove(profile.profilePath());
}
	
bool ScColorProfileCache::contains(const QString& profilePath)
{
	QMap<QString, QWeakPointer<ScColorProfileData> >::iterator iter = m_profileMap.find(profilePath);
	if (iter != m_profileMap.end())
	{
		QSharedPointer<ScColorProfileData> strongRef = iter.value().toStrongRef();
		return (!strongRef.isNull());
	}
	return false;
}

ScColorProfile ScColorProfileCache::profile(const QString& profilePath)
{
	ScColorProfile profile;
	QMap<QString, QWeakPointer<ScColorProfileData> >::iterator iter = m_profileMap.find(profilePath);
	if (iter != m_profileMap.end())
		profile = ScColorProfile(iter.value());
	return profile;
}
