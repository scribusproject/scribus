/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "cxfmeasurementspec.h"
#include "cxfutils.h"

CxfMeasurementSpec::CxfMeasurementSpec()
{

}

bool CxfMeasurementSpec::hasWavelengthRange() const
{
	bool hasRange = (m_waveLengthStart >=360 && m_waveLengthStart <= 400);
	hasRange &= (m_waveLengthIncrement > 0);
	return hasRange;
}

bool CxfMeasurementSpec::isValid() const
{
	return m_measurementType != cxfMeasurementUnknown;
}

bool CxfMeasurementSpec::parse(const QDomElement& elem)
{
	bool convOk = false;
	bool parsingError = false;
	QString txt;

	reset();

	QDomNodeList childNodes = elem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;
		QDomElement childElem = childNode.toElement();
		QString tagName = childElem.tagName();

		if (tagName == QLatin1String("MeasurementType"))
		{
			txt = childElem.text();
			m_measurementType = qstrToCxfMeasurementType(txt);
			if (m_measurementType == cxfMeasurementUnknown)
				parsingError = true;
		}
		if (tagName == QLatin1String("WavelengthRange"))
		{
			txt = childElem.attribute("StartWL");
			if (txt.length() > 0)
			{
				int value = txt.toInt(&convOk);
				if (convOk)
					m_waveLengthStart = value;
				parsingError |= !convOk;
			}
			parsingError |= txt.isEmpty();

			txt = childElem.attribute("Increment");
			if (txt.length() > 0)
			{
				int value = txt.toInt(&convOk);
				if (convOk)
					m_waveLengthIncrement = value;
				parsingError |= !convOk;
			}
			parsingError |= txt.isEmpty();
		}

		if (parsingError)
			break;
	}

	if (parsingError)
		reset();
	return isValid();
}

void CxfMeasurementSpec::reset()
{
	m_measurementType = cxfMeasurementUnknown;
	m_waveLengthStart = 0;
	m_waveLengthIncrement = 0;
}

QVector<int> CxfMeasurementSpec::wavelengths(int count) const
{
	if (!hasWavelengthRange() || (count <= 0))
		return QVector<int>();
	return wavelengths(m_waveLengthStart, m_waveLengthIncrement, count);
}

QVector<int> CxfMeasurementSpec::wavelengths(int start, int increment, int count)
{
	QVector<int> wls;
	wls.reserve(count);
	int wl = start;
	for (int i = 0; i < count; ++i)
	{
		wls.append(wl);
		wl += increment;
	}
	
	return wls;
}
