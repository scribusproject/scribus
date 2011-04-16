/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCLCMS2COLORPROFILEIMPL_H
#define SCLCMS2COLORPROFILEIMPL_H

#include <QString>

#include "lcms2.h"
#include "sccolorprofiledata.h"

class ScLcms2ColorProfileImpl : public ScColorProfileData
{
	friend class ScLcms2ColorMgmtEngineImpl;

public:
	ScLcms2ColorProfileImpl(cmsHPROFILE lcmsProfile);
	virtual ~ScLcms2ColorProfileImpl();

	virtual bool isNull() const;

	virtual eColorSpaceType colorSpace()  const;
	virtual eProfileClass   deviceClass() const;
	virtual QString         productDescription() const;

protected:
	cmsHPROFILE     m_profileHandle;
	mutable QString m_productDescription;

	void closeProfile(void);
};

#endif
