/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "cxftristimulusspec.h"
#include "cxfutils.h"

CxfTristimulusSpec::CxfTristimulusSpec()
{
	m_illuminant = cxfIlluminantUnknown;
	m_observer   = cxfObserverUnknown;
	m_convMethod = cxfMethodUnknown;
}

bool CxfTristimulusSpec::isValid() const
{
	bool valid = true;
	valid &= (m_illuminant != cxfIlluminantUnknown);
	valid &= (m_observer != cxfObserverUnknown);
	valid &= (m_convMethod != cxfMethodUnknown);
	return valid;
}

bool CxfTristimulusSpec::isSupported() const
{
	bool supported = true;
	supported &= (m_illuminant == cxfIlluminantD50 || m_illuminant == cxfIlluminantD65);
	supported &= (m_observer == cxfObserver2_Degree || m_observer == cxfObserver10_Degree);
	return supported;
}

bool CxfTristimulusSpec::parse(const QDomElement& elem)
{
	bool parsingError = false;
	reset();

	QDomNodeList childNodes = elem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;
		QDomElement childElem = childNode.toElement();
		QString tagName = childElem.tagName();

		if (tagName == QLatin1String("CustomIlluminant"))
		{
			m_illuminant = cxfIlluminantCustom;
		}
		if (tagName == QLatin1String("Illuminant"))
		{
			m_illuminant = qstrToCxfIlluminant(childElem.text());
			if (m_illuminant == cxfIlluminantUnknown)
				parsingError = true;
		}
		if (tagName == QLatin1String("Observer"))
		{
			m_observer = qstrToCxfObserver(childElem.text());
			if (m_observer == cxfObserverUnknown)
				parsingError = true;
		}
		if (tagName == QLatin1String("Method"))
		{
			m_convMethod = qstrToCxfCIEConversionMethod(childElem.text());
			if (m_convMethod == cxfMethodUnknown)
				parsingError = true;
		}

		if (parsingError)
			break;
	}

	if (parsingError)
		reset();
	return isValid();
}

void CxfTristimulusSpec::reset()
{
	m_illuminant = cxfIlluminantUnknown;
	m_observer   = cxfObserverUnknown;
	m_convMethod = cxfMethodUnknown;
}
