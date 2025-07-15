/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STYLESEARCH_H
#define STYLESEARCH_H

class QAction;
class QMenu;
class QMenuBar;

#include <QList>
#include <QHash>

#include "scribusapi.h"
#include "scribusdoc.h"

enum class StyleSearchType { paragraph, character };

struct StyleSearchItem
{
	QString name;
	StyleSearchType type {StyleSearchType::paragraph};
};

class SCRIBUS_API StyleSearch
{
public:
	explicit StyleSearch(ScribusDoc *scribusDoc);
	~StyleSearch() = default;

	const QList<StyleSearchItem>& getStyles() const { return styles; }

	void update();
	bool hasStyles() const { return !styles.isEmpty(); }
	void execute(const StyleSearchItem& styleName);

private:
	ScribusDoc* scribusDoc { nullptr };
	QList<PageItem_TextFrame*> textFrames;
	QList<StyleSearchItem> styles;
};

#endif
