/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORTRANSFORM_H
#define SCCOLORTRANSFORM_H

#include <QSharedPointer>
#include <QWeakPointer>
#include "scribusapi.h"
#include "sccolortransformdata.h"

class SCRIBUS_API ScColorTransform
{
	friend class ScColorTransformPool;
public:
	ScColorTransform();
	explicit ScColorTransform(ScColorTransformData* data);
	explicit ScColorTransform(const QSharedPointer<ScColorTransformData>& data);

	ScColorMgmtEngine& engine() { return m_data->engine(); }
	const ScColorMgmtEngine& engine() const { return m_data->engine(); }

	void reset() { m_data.reset(); }
	bool isNull()    const { return (m_data.isNull() || m_data->isNull()); }
	operator bool () const { return !isNull(); }

	const ScColorTransformInfo& transformInfo() const { return m_data->transformInfo(); }

	bool apply(void* input, void* output, uint numElem);
	bool apply(QByteArray& input, QByteArray& output, uint numElem);

	bool operator==(const ScColorTransform& other) const;

	ScColorTransform& operator=(const ScColorTransform& other) = default;
	ScColorTransform& operator=(std::nullptr_t) { m_data.reset(); return *this; }

protected:
	QSharedPointer<ScColorTransformData> m_data;

	QWeakPointer<ScColorTransformData>   weakRef()   const { return m_data.toWeakRef(); }
	QSharedPointer<ScColorTransformData> strongRef() const { return m_data; }
};

#endif
