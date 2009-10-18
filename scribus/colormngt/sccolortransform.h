/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORTRANSFORM_H
#define SCCOLORTRANSFORM_H

#include <QSharedPointer>
#include "scribusapi.h"
#include "sccolortransformdata.h"

class SCRIBUS_API ScColorTransform
{
public:
	ScColorTransform();
	ScColorTransform(ScColorTransformData* data);
	
	ScColorMngtEngine& engine() { return m_data->engine(); }
	const ScColorMngtEngine& engine() const { return m_data->engine(); }

	inline bool isNull()    const { return (m_data.isNull() || m_data->isNull()); }
	inline operator bool () const { return !isNull(); }

	const ScColorTransformInfo& transformInfo() const { return m_data->transformInfo(); }
	
	bool apply(void* input, void* output, uint numElem);
	bool apply(QByteArray& input, QByteArray& output, uint numElem);

	// FIXME get rif of this API as soon as possible (cf scimage)
	void changeBufferFormat(eColorFormat input, eColorFormat output);

	bool operator==(const ScColorTransform& other) const;

protected:
	QSharedPointer<ScColorTransformData> m_data;
};

#endif
