/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORTRANSFORMPOOL_H
#define SCCOLORTRANSFORMPOOL_H

#include <QList>
#include <QWeakPointer>
#include "sccolormgmtstructs.h"
#include "sccolortransform.h"

class ScColorTransformPool
{
	friend class ScColorMgmtEngineData;

public:
	explicit ScColorTransformPool (int engineID);

	void clear();
	void addTransform(const ScColorTransform& transform, bool force = false);
	void removeTransform(const ScColorTransform& transform);
	void removeTransform(const ScColorTransformInfo& info);

	ScColorTransform findTransform(const ScColorTransformInfo& info) const;

private:
	int m_engineID { 0 };
	QList< QWeakPointer<ScColorTransformData> > m_pool;
};

#endif
