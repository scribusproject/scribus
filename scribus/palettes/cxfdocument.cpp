/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFile>

#include "cxfdocument.h"

CxfDocument::CxfDocument()
{
	
}

CxfDocument::~CxfDocument()
{
	while (m_objects.count() > 0)
	{
		CxfObject* object = m_objects.takeAt(0);
		delete object;
	}
}

const CxfColorSpecification* CxfDocument::colorSpecification(const QString& name) const
{
	CxfColorSpecificationShPtr specPtr = m_colorSpecs.value(name, CxfColorSpecificationShPtr());
	return specPtr.data();
}

bool CxfDocument::parse(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return false;

	QDomDocument domDoc;
	if (!domDoc.setContent(&file, true))
	{
		file.close();
		return false;
	}
	file.close();

	QDomElement docElem = domDoc.documentElement();
	if (docElem.tagName() != "CxF")
		return false;

	QDomElement rsrcElem = docElem.firstChildElement("Resources");
	if (rsrcElem.isNull())
		return false;

	// Parse color specification collections first, they are required to parse colors properly
	QDomNodeList collectionNodes = rsrcElem.elementsByTagName("ColorSpecificationCollection");
	for (int i = 0; i < collectionNodes.count(); ++i)
	{
		QDomElement collectionNode = collectionNodes.at(i).toElement();
		parseColorSpecificationCollection(collectionNode);
	}

	if (m_colorSpecs.count() <= 0)
		return false;

	// Parse objects
	QDomNodeList objectNodes = rsrcElem.elementsByTagName("ObjectCollection");
	for (int i = 0; i < objectNodes.count(); ++i)
	{
		QDomElement collectionNode = objectNodes.at(i).toElement();
		parseObjectCollection(collectionNode);
	}

	return (m_objects.count() > 0);
}

bool CxfDocument::parseColorSpecificationCollection(const QDomElement& elem)
{
	QDomNodeList childNodes = elem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;

		QDomElement childElem = childNode.toElement();
		if (childElem.tagName() != "ColorSpecification")
			continue;

		CxfColorSpecificationShPtr colorSpec(new CxfColorSpecification());
		if (!colorSpec->parse(childElem))
			continue;
		m_colorSpecs.insert(colorSpec->id(), colorSpec);
	}
	return (m_colorSpecs.count() > 0);
}

bool CxfDocument::parseObjectCollection(const QDomElement& elem)
{
	QDomNodeList childNodes = elem.childNodes();
	for (int i = 0; i < childNodes.count(); ++i)
	{
		QDomNode childNode = childNodes.at(i);
		if (!childNode.isElement())
			continue;

		QDomElement childElem = childNode.toElement();
		if (childElem.tagName() != "Object")
			continue;

		auto* object = new CxfObject(this);
		if (!object->parse(childElem))
		{
			delete object;
			continue;
		}
		m_objects.append(object);
	}
	return (m_colorSpecs.count() > 0);
}

void CxfDocument::reset()
{
	m_colorSpecs.clear();
}