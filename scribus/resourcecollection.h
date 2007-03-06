/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#ifndef RESOURCE_COLLECTION_H
#define RESOURCE_COLLECTION_H

#include <qstring.h>
#include <qmap.h>
#include <qvaluelist.h>


class ResourceCollection
{
public:
	void collectFont(const QString& name)      { if (!name.isEmpty()) fonts.insert(name,name); }
	void collectPattern(const QString& name)   { if (!name.isEmpty()) patterns.insert(name,name); }
	void collectColor(const QString& name)     { if (!name.isEmpty()) colors.insert(name,name); }
	void collectStyle(const QString& name)     { if (!name.isEmpty()) pstyles.insert(name,name); }
	void collectCharStyle(const QString& name) { if (!name.isEmpty()) cstyles.insert(name,name); }
	void collectLineStyle(const QString& name) { if (!name.isEmpty()) linestyles.insert(name,name); }
	
	QValueList<QString> fontNames() const      { return fonts.keys(); }
	QValueList<QString> patternNames() const   { return patterns.keys(); }
	QValueList<QString> colorNames() const     { return colors.keys(); }
	QValueList<QString> styleNames() const     { return pstyles.keys(); }
	QValueList<QString> charStyleNames() const { return cstyles.keys(); }
	QValueList<QString> lineStyleNames() const { return linestyles.keys(); }

	// modifies newNames so that forall x in both newNames.key() and in existingNames, newNames[x] will map to a new unique name
	static void makeUnique(QMap<QString,QString>& newNames, const QValueList<QString> existingNames);

	QMap<QString,QString> fonts;
	QMap<QString,QString> patterns;
	QMap<QString,QString> colors;
	QMap<QString,QString> pstyles;
	QMap<QString,QString> cstyles;
	QMap<QString,QString> linestyles;
};


#endif
