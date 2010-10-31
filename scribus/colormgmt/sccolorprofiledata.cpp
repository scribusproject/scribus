/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#include <QCryptographicHash>

#include "sccolorprofiledata.h"

QString ScColorProfileData::dataHash() const
{
	// This only needs to be hashed once, as the profile data doesn't change once it's set.
	// <jghali> mhx: sccolorprofiledata must only be created by a sccolorengine and not changed afterwards
	if (m_profileDataHash.isEmpty())
	{
		if (m_profileData.isEmpty())
			return QString();
		m_profileDataHash = QCryptographicHash::hash(m_profileData, QCryptographicHash::Md5).toHex();
	}
	return m_profileDataHash;
}
