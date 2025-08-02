/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORTRANSFORMDATA_H
#define SCCOLORTRANSFORMDATA_H

#include <QByteArray>
#include <QString>

#include "sccolormgmtelem.h"
#include "sccolormgmtstructs.h"

class ScColorTransformData  : public ScColorMgmtElem
{
public:
	const ScColorTransformInfo& transformInfo() const { return m_transformInfo; }
	void  setTransformInfo(const ScColorTransformInfo& info) { m_transformInfo = info; }

	virtual bool isNull() const = 0;

	virtual bool apply(void* input, void* output, uint numElem) = 0;
	virtual bool apply(QByteArray& input, QByteArray& output, uint numElem) = 0;

protected:
	ScColorTransformInfo m_transformInfo;
};

#endif
