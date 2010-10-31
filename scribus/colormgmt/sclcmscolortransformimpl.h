/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCLCMSCOLORTRANSFORMIMPL_H
#define SCLCMSCOLORTRANSFORMIMPL_H

#include "lcms.h"
#include "sccolortransformdata.h"

class ScLcmsColorTransformImpl : public ScColorTransformData
{
	friend class ScLcmsColorMgmtEngineImpl;

public:
	ScLcmsColorTransformImpl(cmsHTRANSFORM lcmsTransform);
	virtual ~ScLcmsColorTransformImpl();

	virtual bool isNull() const;

	virtual bool apply(void* input, void* output, uint numElem);
	virtual bool apply(QByteArray& input, QByteArray& output, uint numElem);

	virtual void changeBufferFormat(eColorFormat inputFormat, eColorFormat outputFormat);

protected:
	cmsHTRANSFORM m_transformHandle;

	void deleteTransform(void);
};

#endif
