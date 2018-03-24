/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CXFCOLORSPECIFICATION_H
#define CXFCOLORSPECIFICATION_H

#include <QDomDocument>
#include <QSharedPointer>
#include <QString>

#include "cxfmeasurementspec.h"
#include "cxftristimulusspec.h"

class CxfColorSpecification
{
public:
	CxfColorSpecification();

	const QString& id() const { return m_id; }
	
	bool hasMeasurementSpec() const { m_hasMeasurementSpec; }
	bool hasTristimulusSpec() const { m_hasTristimulusSpec; }
	bool hasWavelengthRange() const { return m_measurementSpec.hasWavelengthRange(); }
	
	const CxfMeasurementSpec& measurementSpec() const { return m_measurementSpec; }
	const CxfTristimulusSpec& tristimulusSpec() const { return m_tristimulusSpec; }
	
	bool isValid() const;
	bool parse(QDomElement& elem);
	
	void reset();
	
protected:
	QString m_id;

	bool m_hasMeasurementSpec;
	bool m_hasTristimulusSpec;
	
	CxfMeasurementSpec m_measurementSpec;
	CxfTristimulusSpec m_tristimulusSpec;
};

typedef QSharedPointer<CxfColorSpecification> CxfColorSpecificationShPtr;

#endif