/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCLCMS2COLORTRANSFORMIMPL_H
#define SCLCMS2COLORTRANSFORMIMPL_H

#include "lcms2.h"
#include "sccolormgmtimplelem.h"
#include "sccolortransformdata.h"

class ScLcms2ColorTransformImpl : public ScColorTransformImplBase
{
	friend class ScLcms2ColorMgmtEngineImpl;

public:
	ScLcms2ColorTransformImpl(ScColorMgmtEngine& engine, cmsHTRANSFORM lcmsTransform);
	~ScLcms2ColorTransformImpl() override;

	bool isNull() const override;

	bool apply(void* input, void* output, uint numElem) override;
	bool apply(QByteArray& input, QByteArray& output, uint numElem) override;

protected:
	cmsHTRANSFORM m_transformHandle { nullptr };

	void deleteTransform();
};

#endif
