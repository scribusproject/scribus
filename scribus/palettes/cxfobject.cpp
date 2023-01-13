/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "cxfcolor.h"
#include "cxfdocument.h"
#include "cxfobject.h"

CxfObject::CxfObject(CxfDocument* cxfDoc)
         : m_cxfDoc(cxfDoc)
{

}

const CxfColor* CxfObject::color(CxfColorType type) const
{
	CxfColorShPtr colorPtr = m_colorMap.value(type, CxfColorShPtr());
	return colorPtr.data();
}

bool CxfObject::parse(const QDomElement& elem)
{
	m_id = elem.attribute("Id");
	if (m_id.isEmpty())
		return false;

	m_name = elem.attribute("Name");
	if (m_name.isEmpty())
		return false;

	QDomElement colorValElem = elem.firstChildElement("ColorValues");
	if (!colorValElem.isNull())
		parseColorValues(colorValElem);

	QDomElement deviceValElem = elem.firstChildElement("DeviceColorValues");
	if (!deviceValElem.isNull())
		parseDeviceColorValues(deviceValElem);

	return (m_colorMap.count() > 0);
}

void CxfObject::parseColorValues(const QDomElement& elem)
{
	QDomNodeList childNodes = elem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;

		QDomElement childElem = childNode.toElement();
		QString tagName = childElem.tagName();
		if (tagName == "ReflectanceSpectrum")
		{
			CxfColorShPtr colorPtr(new CxfReflectanceSpectrum(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfReflectanceSpectrum, colorPtr);
			continue;
		}
		if (tagName == "ColorAdobeRGB")
		{
			CxfColorShPtr colorPtr(new CxfColorAdobeRGB(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfColorAdobeRGB, colorPtr);
			continue;
		}
		if (tagName == "ColorSRGB")
		{
			CxfColorShPtr colorPtr(new CxfColorSRGB(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfColorSRGB, colorPtr);
			continue;
		}
		if (tagName == "ColorHTML")
		{
			CxfColorShPtr colorPtr(new CxfColorHTML(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfColorHTML, colorPtr);
			continue;
		}
		if (tagName == "ColorCIELab")
		{
			CxfColorShPtr colorPtr(new CxfColorCIELab(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfColorCIELab, colorPtr);
			continue;
		}
		if (tagName == "ColorCIELCh")
		{
			CxfColorShPtr colorPtr(new CxfColorCIELCh(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfColorCIELCh, colorPtr);
			continue;
		}
		if (tagName == "ColorCIEXYZ")
		{
			CxfColorShPtr colorPtr(new CxfColorCIEXYZ(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfColorCIEXYZ, colorPtr);
			continue;
		}
	}
}

void CxfObject::parseDeviceColorValues(const QDomElement& elem)
{
	QDomNodeList childNodes = elem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;

		QDomElement childElem = childNode.toElement();
		QString tagName = childElem.tagName();
		if (tagName == "ColorRGB")
		{
			CxfColorShPtr colorPtr(new CxfColorRGB(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfColorRGB, colorPtr);
			continue;
		}
		if (tagName == "ColorCMYK")
		{
			CxfColorShPtr colorPtr(new CxfColorCMYK(m_cxfDoc));
			if (colorPtr->parse(childElem))
				m_colorMap.insert(cxfColorCMYK, colorPtr);
			continue;
		}
	}
}