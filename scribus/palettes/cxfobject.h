/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CXFOBJECT_H
#define CXFOBJECT_H

#include <QDomDocument>
#include <QList>
#include <QMap>

#include "cxfcolor.h"
#include "cxftypes.h"

class CxfDocument;

class CxfObject
{
public:
	CxfObject(CxfDocument* cxfDoc);
	
	const QString& id() const { return m_id; }
	const QString& name() const { return m_name; }
	
	QList<CxfColorType> availableColorTypes() const { return m_colorMap.keys(); }
	int   colorCount() const { return m_colorMap.count(); }
	
	bool  hasColor(CxfColorType type) const { return m_colorMap.contains(type); }
	const CxfColor* color(CxfColorType type) const;
	
	bool parse(const QDomElement& elem);

private:
	CxfDocument* m_cxfDoc { nullptr };
	
	QString m_id;
	QString m_name;
	QMap<CxfColorType, CxfColorShPtr> m_colorMap;

	void parseColorValues(const QDomElement& elem);
	void parseDeviceColorValues(const QDomElement& elem);
};

#endif