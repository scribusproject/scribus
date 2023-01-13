/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CXFTRISTIMULUSSPEC_H
#define CXFTRISTIMULUSSPEC_H

#include <QDomDocument>
#include "cxftypes.h"

class CxfTristimulusSpec
{
public:
	CxfTristimulusSpec();
	
	CxfIlluminant illuminant() const { return m_illuminant; }
	CxfObserver   observer() const { return m_observer; }
	CxfCIEConversionMethod conversionMethod() const { return m_convMethod; }

	bool isD50_2degree() const { return (m_illuminant == cxfIlluminantD50 && m_observer == cxfObserver2_Degree); }
	
	bool isValid() const;
	bool isSupported() const;
	bool parse(const QDomElement& elem);
	void reset();
	
protected:
	CxfIlluminant m_illuminant;
	CxfObserver   m_observer;
	CxfCIEConversionMethod m_convMethod;
};

#endif