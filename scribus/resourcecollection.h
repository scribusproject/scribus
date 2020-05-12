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

#include <QList>
#include <QMap>
#include <QString>

#include "scribusapi.h"
#include "vgradient.h"

class ColorList;
class SCFonts;

/** 
*   A simple structure which holds the names of all resources & styles used in a doc or part of a document.
*   Also holds pointers to the doc's fontset and colorset.
*   The names are stored as a QMap<QString, QString>. When a doc is queried for it's used resources, these
*   maps get an entry  (nam -> nam) for each used resource 'nam'. You can replace resources consistently by
*   changing these maps and asking the doc to use the altered maps to change all old names to the new one.
*   Operations:
*   - getNamedResources(ResourceCollection& rsc)              ... fills 'rsc' with identity mappings for used resources
*   - replaceNamedResources(const ResourceCollection& maps)   ... uses 'maps' to change old names to new names
*   - redefineNamedResources(const ResourceCollection& defs)  ... uses the definitions in 'defs' to overwrite/add resources
*   - makeNamedResourcesUnique(ResourceCollection& other)     ... changes mapping in 'other' to unique names
*   Merge options:  keep old def, keep new def, rename new resource to unique name
*/
class SCRIBUS_API ResourceCollection
{
	public:
		ResourceCollection() {}

		void collectFont(const QString& name)         { if (!name.isEmpty()) m_fonts.insert(name, name); }
		void collectFontfeatures(const QString& name) { if (!name.isEmpty()) m_fontfeatures.insert(name, name); }
		void collectPattern(const QString& name)      { if (!name.isEmpty()) m_patterns.insert(name, name); }
		void collectColor(const QString& name)        { if (!name.isEmpty()) m_colors.insert(name, name); }
		void collectGradient(const QString& name)     { if (!name.isEmpty()) m_gradients.insert(name, name); }
		void collectStyle(const QString& name)        { if (!name.isEmpty()) m_pstyles.insert(name, name); }
		void collectCharStyle(const QString& name)    { if (!name.isEmpty()) m_cstyles.insert(name, name); }
		void collectLineStyle(const QString& name)    { if (!name.isEmpty()) m_linestyles.insert(name, name); }
		void collectTableStyle(const QString& name)   { if (!name.isEmpty()) m_tableStyles.insert(name, name); }
		void collectCellStyle(const QString& name)    { if (!name.isEmpty()) m_cellStyles.insert(name, name); }
		void collectNoteStyle(const QString& name)    { if (!name.isEmpty()) m_noteStyles.insert(name, name); }

		void mapFont(const QString& oldname, const QString& newname)         { m_fonts.insert(oldname, newname); }
		void mapPattern(const QString& oldname, const QString& newname)      { m_patterns.insert(oldname, newname); }
		void mapFontfeatures(const QString& oldname, const QString& newname) { m_fontfeatures.insert(oldname, newname); }
		void mapColor(const QString& oldname, const QString& newname)        { m_colors.insert(oldname, newname); }
		void mapGradients(const QString& oldname, const QString& newname)    { m_gradients.insert(oldname, newname); }
		void mapStyle(const QString& oldname, const QString& newname)        { m_pstyles.insert(oldname, newname); }
		void mapCharStyle(const QString& oldname, const QString& newname)    { m_cstyles.insert(oldname, newname); }
		void mapLineStyle(const QString& oldname, const QString& newname)    { m_linestyles.insert(oldname, newname); }
		void mapTableStyle(const QString& oldname, const QString& newname)   { m_tableStyles.insert(oldname, newname); }
		void mapCellStyle(const QString& oldname, const QString& newname)    { m_cellStyles.insert(oldname, newname); }
		void mapNoteStyle(const QString& oldname, const QString& newname)    { m_noteStyles.insert(oldname, newname); }

		void mapFonts(const QMap<QString, QString>& newnames)        { m_fonts = newnames; }
		void mapPatterns(const QMap<QString, QString>& newnames)     { m_patterns = newnames; }
		void mapFontfeatures(const QMap<QString, QString>& newnames) { m_fontfeatures = newnames; }
		void mapColors(const QMap<QString, QString>& newnames)       { m_colors = newnames; }
		void mapGradients(const QMap<QString, QString>& newnames)    { m_gradients = newnames; }
		void mapStyles(const QMap<QString, QString>& newnames)       { m_pstyles = newnames; }
		void mapCharStyles(const QMap<QString, QString>& newnames)   { m_cstyles = newnames; }
		void mapLineStyles(const QMap<QString, QString>& newnames)   { m_linestyles = newnames; }
		void mapTableStyles(const QMap<QString, QString>& newnames)  { m_tableStyles = newnames; }
		void mapCellStyles(const QMap<QString, QString>& newnames)   { m_cellStyles = newnames; }
		void mapNoteStyles(const QMap<QString, QString>& newnames)   { m_noteStyles = newnames; }

		const QMap<QString, QString>& fonts()        { return m_fonts; }
		const QMap<QString, QString>& fontfeatures() { return m_fontfeatures; }
		const QMap<QString, QString>& patterns()     { return m_patterns; }
		const QMap<QString, QString>& colors()       { return m_colors; }
		const QMap<QString, QString>& gradients()    { return m_gradients; }
		const QMap<QString, QString>& styles()       { return m_pstyles; }
		const QMap<QString, QString>& charStyles()   { return m_cstyles; }
		const QMap<QString, QString>& lineStyles()   { return m_linestyles; }
		const QMap<QString, QString>& tableStyles()  { return m_tableStyles; }
		const QMap<QString, QString>& cellStyles()   { return m_cellStyles; }
		const QMap<QString, QString>& noteStyles()   { return m_noteStyles; }

		QList<QString> fontNames() const         { return m_fonts.keys(); }
		QList<QString> patternNames() const      { return m_patterns.keys(); }
		QList<QString> fontfeaturesNames() const { return m_fontfeatures.keys(); }
		QList<QString> colorNames() const        { return m_colors.keys(); }
		QList<QString> gradientNames() const     { return m_gradients.keys(); }
		QList<QString> styleNames() const        { return m_pstyles.keys(); }
		QList<QString> charStyleNames() const    { return m_cstyles.keys(); }
		QList<QString> lineStyleNames() const    { return m_linestyles.keys(); }
		QList<QString> tableStyleNames() const   { return m_tableStyles.keys(); }
		QList<QString> cellStyleNames() const    { return m_cellStyles.keys(); }
		QList<QString> noteStyleNames() const    { return m_noteStyles.keys(); }

		// modifies newNames so that forall x in both newNames.key() and in existingNames, newNames[x] will map to a new unique name
		static void makeUnique(QMap<QString, QString>& newNames, const QList<QString> existingNames);

		void makeNamedResourcesUnique(ResourceCollection& other);

		SCFonts* availableFonts {nullptr};
		ColorList* availableColors {nullptr};
		QMap<QString, VGradient> *availableGradients {nullptr};

	private:
		QMap<QString, QString> m_fonts;
		QMap<QString, QString> m_patterns;
		QMap<QString, QString> m_fontfeatures;
		QMap<QString, QString> m_colors;
		QMap<QString, QString> m_gradients;
		QMap<QString, QString> m_pstyles;
		QMap<QString, QString> m_cstyles;
		QMap<QString, QString> m_linestyles;
		QMap<QString, QString> m_tableStyles;
		QMap<QString, QString> m_cellStyles;
		QMap<QString, QString> m_noteStyles;
};

#endif
