/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cmath>

#include "cxfcolor.h"
#include "cxfcolorspecification.h"
#include "cxfdocument.h"

#include "colormgmt/sccolormgmtstructs.h"

CxfColor::CxfColor(CxfDocument* cxfDoc)
{
	m_cxfDoc = cxfDoc;
	m_colorSpec = 0;
}

bool CxfColor::isValid() const
{
	bool valid = true;
	valid &= (m_cxfDoc != 0);
	valid &= (m_colorSpec != 0);
	return valid;
}

void CxfColor::reset()
{
	m_colorSpec = 0;
}

CxfColorRGB::CxfColorRGB(CxfDocument* cxfDoc)
           : CxfColor(cxfDoc)
{
	m_maxRange = 255.0;
	m_values[0] = 0.0;
	m_values[1] = 0.0;
	m_values[2] = 0.0;
}

bool CxfColorRGB::parse(QDomElement& colorElem)
{
	bool convOk = false;
	bool gotRed(false);
	bool gotGreen(false);
	bool gotBlue(false);

	reset();

	QString colorSpec = colorElem.attribute("ColorSpecification");
	if (colorSpec.isEmpty())
		return false;

	m_colorSpec = m_cxfDoc->colorSpecification(colorSpec);
	if (!m_colorSpec)
		return false;

	QDomElement maxRangeElem = colorElem.firstChildElement("MaxRange");
	if (!maxRangeElem.isNull())
	{
		QString str = maxRangeElem.text();
		double maxRange = str.toDouble(&convOk);
		if (!convOk)
			return false;
		m_maxRange = maxRange;
	}

	QDomNodeList childNodes = colorElem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;
		QDomElement childElem = childNode.toElement();

		QString tagName = childElem.tagName();
		if (tagName == "R")
		{
			QString str = childElem.text();
			double red = str.toDouble(&gotRed);
			if (!gotRed)
				return false;
			m_values[0] = qMax(0.0, qMin(red, m_maxRange));
			continue;
		}
		if (tagName == "G")
		{
			QString str = childElem.text();
			double green = str.toDouble(&gotGreen);
			if (!gotGreen)
				return false;
			m_values[1] = qMax(0.0, qMin(green, m_maxRange));
			continue;
		}
		if (tagName == "B")
		{
			QString str = childElem.text();
			double blue = str.toDouble(&gotBlue);
			if (!gotBlue)
				return false;
			m_values[2] = qMax(0.0, qMin(blue, m_maxRange));
			continue;
		}
	}

	return (gotRed && gotGreen && gotBlue);
}

void CxfColorRGB::reset()
{
	CxfColor::reset();

	m_maxRange = 255.0;
	m_values[0] = 0.0;
	m_values[1] = 0.0;
	m_values[2] = 0.0;
}

CxfColorSRGB::CxfColorSRGB(CxfDocument* cxfDoc)
	        : CxfColorRGB(cxfDoc)
{

}

CxfColorAdobeRGB::CxfColorAdobeRGB(CxfDocument* cxfDoc)
	            : CxfColorRGB(cxfDoc)
{

}

CxfColorCMYK::CxfColorCMYK(CxfDocument* cxfDoc)
                : CxfColor(cxfDoc)
{
	m_values[0] = 0.0;
	m_values[1] = 0.0;
	m_values[2] = 0.0;
	m_values[3] = 0.0;
}

bool CxfColorCMYK::parse(QDomElement& colorElem)
{
	bool convOk = false;
	bool gotCyan(false);
	bool gotMagenta(false);
	bool gotYellow(false);
	bool gotBlack(false);

	reset();

	QString colorSpec = colorElem.attribute("ColorSpecification");
	if (colorSpec.isEmpty())
		return false;

	m_colorSpec = m_cxfDoc->colorSpecification(colorSpec);
	if (!m_colorSpec)
		return false;

	QDomNodeList childNodes = colorElem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;
		QDomElement childElem = childNode.toElement();

		QString tagName = childElem.tagName();
		if (tagName == "C")
		{
			QString str = childElem.text();
			double cyan = str.toDouble(&gotCyan);
			if (!gotCyan)
				return false;
			m_values[0] = qMax(0.0, qMin(cyan, 100.0));
			continue;
		}
		if (tagName == "M")
		{
			QString str = childElem.text();
			double magenta = str.toDouble(&gotMagenta);
			if (!gotMagenta)
				return false;
			m_values[1] = qMax(0.0, qMin(magenta, 100.0));
			continue;
		}
		if (tagName == "Y")
		{
			QString str = childElem.text();
			double yellow = str.toDouble(&gotYellow);
			if (!gotYellow)
				return false;
			m_values[2] = qMax(0.0, qMin(yellow, 100.0));
			continue;
		}
		if (tagName == "K")
		{
			QString str = childElem.text();
			double black = str.toDouble(&gotBlack);
			if (!gotBlack)
				return false;
			m_values[3] = qMax(0.0, qMin(black, 100.0));
			continue;
		}
	}

	return (gotCyan && gotMagenta && gotYellow && gotBlack);
}

void CxfColorCMYK::reset()
{
	CxfColor::reset();

	m_values[0] = 0.0;
	m_values[1] = 0.0;
	m_values[2] = 0.0;
	m_values[3] = 0.0;
}

CxfColorCIELab::CxfColorCIELab(CxfDocument* cxfDoc)
	          : CxfColor(cxfDoc)
{
	m_values[0] = 0.0;
	m_values[1] = 0.0;
	m_values[2] = 0.0;
}

bool CxfColorCIELab::parse(QDomElement& colorElem)
{
	bool gotLab_L(false);
	bool gotLab_a(false);
	bool gotLab_b(false);

	reset();

	QString colorSpec = colorElem.attribute("ColorSpecification");
	if (colorSpec.isEmpty())
		return false;

	m_colorSpec = m_cxfDoc->colorSpecification(colorSpec);
	if (!m_colorSpec)
		return false;

	QDomNodeList childNodes = colorElem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;
		QDomElement childElem = childNode.toElement();

		QString tagName = childElem.tagName();
		if (tagName == "L")
		{
			QString str = childElem.text();
			double lab_L = str.toDouble(&gotLab_L);
			if (!gotLab_L)
				return false;
			m_values[0] = qMax(0.0, qMin(lab_L, 100.0));
			continue;
		}
		if (tagName == "A")
		{
			QString str = childElem.text();
			double lab_a = str.toDouble(&gotLab_a);
			if (!gotLab_a)
				return false;
			m_values[1] = lab_a;
			continue;
		}
		if (tagName == "B")
		{
			QString str = childElem.text();
			double lab_b = str.toDouble(&gotLab_b);
			if (!gotLab_b)
				return false;
			m_values[2] = lab_b;
			continue;
		}
	}

	return (gotLab_L && gotLab_a && gotLab_b);
}

void CxfColorCIELab::reset()
{
	CxfColor::reset();

	m_values[0] = 0.0;
	m_values[1] = 0.0;
	m_values[2] = 0.0;
}

CxfColorCIELCh::CxfColorCIELCh(CxfDocument* cxfDoc)
	          : CxfColor(cxfDoc)
{
	m_values[0] = 0.0;
	m_values[1] = 0.0;
	m_values[2] = 0.0;
}

ScLab CxfColorCIELCh::lab() const
{
	ScLab lab;
	lab.L = m_values[0];
	lab.a = m_values[1] * cos(m_values[2] * M_PI / 180.0);
	lab.b = m_values[1] * cos(m_values[2] * M_PI / 180.0);
	return lab;
}

bool CxfColorCIELCh::parse(QDomElement& colorElem)
{
	bool gotLCh_L(false);
	bool gotLCh_C(false);
	bool gotLCh_h(false);

	reset();

	QString colorSpec = colorElem.attribute("ColorSpecification");
	if (colorSpec.isEmpty())
		return false;

	m_colorSpec = m_cxfDoc->colorSpecification(colorSpec);
	if (!m_colorSpec)
		return false;

	QDomNodeList childNodes = colorElem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;
		QDomElement childElem = childNode.toElement();

		QString tagName = childElem.tagName();
		if (tagName == "L")
		{
			QString str = childElem.text();
			double lch_L = str.toDouble(&gotLCh_L);
			if (!gotLCh_L)
				return false;
			m_values[0] = qMax(0.0, qMin(lch_L, 100.0));
			continue;
		}
		if (tagName == "C")
		{
			QString str = childElem.text();
			double lch_C = str.toDouble(&gotLCh_C);
			if (!gotLCh_C)
				return false;
			m_values[1] = qMax(0.0, lch_C);
			continue;
		}
		if (tagName == "H")
		{
			QString str = childElem.text();
			double lch_h = str.toDouble(&gotLCh_h);
			if (!gotLCh_h)
				return false;
			while (lch_h < 0)
				lch_h += 360.0;
			while (lch_h > 360)
				lch_h -= 360.0;
			m_values[2] = lch_h;
			continue;
		}
	}

	return (gotLCh_L && gotLCh_C && gotLCh_h);
}

void CxfColorCIELCh::reset()
{
	CxfColor::reset();

	m_values[0] = 0.0;
	m_values[1] = 0.0;
	m_values[2] = 0.0;
}