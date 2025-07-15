/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "stylesearch.h"

#include <QAction>
#include <QDebug>
#include <QList>
#include <QStringList>

#include "scribus.h"
#include "selection.h"

StyleSearch::StyleSearch(ScribusDoc *scribusDoc)
	: scribusDoc{scribusDoc}
{
}

void StyleSearch::update()
{
	if (scribusDoc->m_Selection->isEmpty())
		return;
	// TODO: we might simply accept that it's ok if there is a selection
	// or if there is at least one text frame in the selection
	// for (int i = 0; i < scribusDoc->m_Selection->count(); i++)
	// {
	// 	PageItem *currItem = scribusDoc->m_Selection->itemAt(i);
	// 	if (currItem->isTextFrame())
	// 	{
	// 		textFrames.append(currItem->asTextFrame());
	// 	}
	// }

	// if (textFrames.empty())
	// 	return;

	styles.clear();

	int n = scribusDoc->paragraphStyles().count();
	for (int i = 0; i < n; ++i )
	{
		auto style = scribusDoc->paragraphStyles()[i];
		styles.append({style.name(), StyleSearchType::paragraph});
	}
	n = scribusDoc->charStyles().count();
	for (int i = 0; i < n; ++i )
	{
		auto style = scribusDoc->charStyles()[i];
		styles.append({style.name(), StyleSearchType::character});
	}
}

/**
 * The implementation execute is based on the scripter's scribus_setparagraphstyle
 * and scribus_setcharstyle.
 */
void StyleSearch::execute(const StyleSearchItem& style)
{
	if (scribusDoc->m_Selection->isEmpty())
		return;

	if (style.type == StyleSearchType::paragraph)
	{
		if (!scribusDoc->paragraphStyles().contains(style.name))
			return;

		ParagraphStyle paragraphStyle;
		paragraphStyle.setParent(style.name);
		scribusDoc->itemSelection_ApplyParagraphStyle(paragraphStyle);
	}
	else if (style.type == StyleSearchType::character)
	{
		if (!scribusDoc->charStyles().contains(style.name))
			return;

		scribusDoc->itemSelection_SetNamedCharStyle(style.name);
	}
}
