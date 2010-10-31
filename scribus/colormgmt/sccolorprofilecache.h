/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCOLORPROFILECACHE_H
#define SCCOLORPROFILECACHE_H

#include <QMap>
#include <QString>
#include <QWeakPointer>
#include "sccolorprofile.h"

class ScColorProfileCache 
{
public:
	void addProfile(const ScColorProfile& profile);
	void removeProfile(const QString& profilePath);
	void removeProfile(const ScColorProfile& profile);
	
	bool contains(const QString& profilePath);
	ScColorProfile profile(const QString& profilePath);

protected:
	QMap<QString, QWeakPointer<ScColorProfileData> > m_profileMap;
};

#endif
