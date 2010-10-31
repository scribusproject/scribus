/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORPROFILE_H
#define SCCOLORPROFILE_H

#include <QSharedPointer>
#include <QWeakPointer>
#include "scribusapi.h"
#include "sccolorprofiledata.h"

class SCRIBUS_API ScColorProfile
{
	friend class ScColorProfileCache;

public:
	ScColorProfile();
	ScColorProfile(ScColorProfileData*);
	ScColorProfile(const QSharedPointer<ScColorProfileData>&);
	
	inline bool isNull()    const { return (m_data.isNull() || m_data->isNull()); }
	inline operator bool () const { return !isNull(); }

	QString profilePath() const;
	QString productDescription() const;
	
	eColorSpaceType colorSpace()  const;
	eProfileClass   deviceClass() const;
	
	const ScColorProfileData* data() const { return m_data.data(); }

	bool operator==(const ScColorProfile& prof) const;
	
protected:
	QSharedPointer<ScColorProfileData> m_data;

	QWeakPointer<ScColorProfileData>   weakRef()   const { return m_data.toWeakRef(); }
	QSharedPointer<ScColorProfileData> strongRef() const { return m_data; }
};

#endif
