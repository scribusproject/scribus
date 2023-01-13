/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CXFMEASUREMENTSPEC_H
#define CXFMEASUREMENTSPEC_H

#include <QDomDocument>
#include <QVector>

#include "cxftypes.h"

class CxfMeasurementSpec
{
public:
	CxfMeasurementSpec();
	
	bool isValid() const;
	bool parse(const QDomElement& elem);
	
	CxfMeasurementType measurementType() const { return m_measurementType; }
	
	bool hasWavelengthRange() const;
	int wavelengthStart() const { return m_waveLengthStart; }
	int wavelengthIncrement() const { return m_waveLengthIncrement; }

	void reset();
	
	QVector<int> wavelengths(int count) const;
	static QVector<int> wavelengths(int start, int increment, int count);
	
protected:
	CxfMeasurementType m_measurementType { cxfMeasurementUnknown };
	int m_waveLengthStart { 0 };
	int m_waveLengthIncrement { 0 };
};

#endif
