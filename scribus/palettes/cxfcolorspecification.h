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
	CxfColorSpecification() = default;

	const QString& id() const { return m_id; }
	
	bool hasMeasurementSpec() const { return m_hasMeasurementSpec; }
	bool hasTristimulusSpec() const { return m_hasTristimulusSpec; }
	bool hasWavelengthRange() const { return m_measurementSpec.hasWavelengthRange(); }
	
	const CxfMeasurementSpec& measurementSpec() const { return m_measurementSpec; }
	const CxfTristimulusSpec& tristimulusSpec() const { return m_tristimulusSpec; }
	
	bool isValid() const;
	bool parse(const QDomElement& elem);
	
	void reset();
	
private:
	QString m_id;

	bool m_hasMeasurementSpec { false };
	bool m_hasTristimulusSpec { false };
	
	CxfMeasurementSpec m_measurementSpec;
	CxfTristimulusSpec m_tristimulusSpec;
};

using CxfColorSpecificationShPtr = QSharedPointer<CxfColorSpecification>;

#endif
