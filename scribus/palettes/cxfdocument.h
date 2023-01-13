/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CXFDOCUMENT_H
#define CXFDOCUMENT_H

#include <QDomDocument>
#include <QList>
#include <QMap>
#include <QString>

#include "cxfcolorspecification.h"
#include "cxfobject.h"

class CxfDocument
{
public:
	CxfDocument();
	~CxfDocument();
	
	const CxfColorSpecification* colorSpecification(const QString& name) const;
	const QMap<QString, CxfColorSpecificationShPtr>& colorSpecifications() const { return m_colorSpecs; }
	bool hasColorSpecification(const QString& name) const { return m_colorSpecs.contains(name); }

	int   objectCount() const { return m_objects.count(); }
	const CxfObject* objectAt(int index) const { return m_objects.at(index); }
	const QList<CxfObject*>& objects() const { return m_objects; }
	
	bool parse(const QString& fileName);
	void reset();
	
private:
	QMap<QString, CxfColorSpecificationShPtr> m_colorSpecs;
	QList<CxfObject*> m_objects;
	
	bool parseColorSpecificationCollection(const QDomElement& elem);
	bool parseObjectCollection(const QDomElement& elem);
};

#endif
