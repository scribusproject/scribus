/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "cxfcolorspecification.h"

bool CxfColorSpecification::isValid() const
{
	bool valid = true;
	valid &= (m_id.length() > 0);
	valid &= m_hasMeasurementSpec;
	valid &= m_measurementSpec.isValid();
	if (m_hasTristimulusSpec)
		valid &= m_tristimulusSpec.isValid();
	return valid;
}

bool CxfColorSpecification::parse(const QDomElement& elem)
{
	reset();
	
	m_id = elem.attribute("Id");
	if (m_id.isEmpty())
		return false;
	
	// MeasurementSpec element is mandatory
	QDomElement measSpecElem = elem.firstChildElement("MeasurementSpec");
	if (!measSpecElem.isNull())
	{
		m_hasMeasurementSpec = true;
		bool parsed = m_measurementSpec.parse(measSpecElem);
		if (!parsed)
			return false;
	}
	
	// TristimulusSpec element is optional
	QDomElement trisSpecElem = elem.firstChildElement("TristimulusSpec");
	if (!trisSpecElem.isNull())
	{
		m_hasTristimulusSpec = true;
		bool parsed = m_tristimulusSpec.parse(trisSpecElem);
		if (!parsed)
			return false;
	}
	
	return isValid();
}

void CxfColorSpecification::reset()
{
	m_id.clear();
	
	m_hasMeasurementSpec = false;
	m_hasTristimulusSpec = false;
	
	m_measurementSpec.reset();
	m_tristimulusSpec.reset();
}