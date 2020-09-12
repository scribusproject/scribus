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
#include "sccolormgmtimplelem.h"
#include "sccolorprofiledata.h"

class ScLcms2ColorProfileImpl : public ScColorProfileImplBase
{
	friend class ScLcms2ColorMgmtEngineImpl;

public:
	ScLcms2ColorProfileImpl(ScColorMgmtEngine& engine, cmsHPROFILE lcmsProfile);
	virtual ~ScLcms2ColorProfileImpl();

	bool isNull() const override;

	eColorSpaceType colorSpace() const override;
	eColorSpaceType connectionSpace() const override;
	eProfileClass   deviceClass() const override;

	int channelsOfColorSpace() const override;
	int channelsOfConnectionSpace() const override;

	bool            isSuitableForOutput() const override;
	QString         productDescription() const override;
	bool            save(QByteArray& profileData) const override;

protected:
	cmsHPROFILE     m_profileHandle;
	mutable QString m_productDescription;

	void closeProfile(void);
};

#endif
