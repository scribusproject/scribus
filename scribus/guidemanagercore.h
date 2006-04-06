/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef GUIDEMANAGERCORE_H
#define GUIDEMANAGERCORE_H

#include <qvaluelist.h>

#include "scribusstructs.h"
#include "undoobject.h"
#include "scribusapi.h"

class QColor;
class Page;
class ScPainter;
class UndoManager;


/*! \brief Core manipulation with the guides.
Basic idea:
- keep all guides operation here.
- guides will be handled "on the fly", no by modal dialog.
- 2 types of guides: Standard = created one by one. Auto = created by automatic division algorithms.
- user can move only Stnadard ones
- Auto guides can be deleted only in manipulation dialog
- Auto guides are painted in different color / (propably) with diff. line.
- it will need document format change (for auto guides)

Implementing of class Guide() as standalone entity looks great in object
design bit it's too slow */
class SCRIBUS_API GuideManagerCore
{
	public:
		GuideManagerCore();
		GuideManagerCore(Page* parentPage);
		~GuideManagerCore();

		typedef enum {Standard, Auto} GuideType;

		void addHorizontal(double value, GuideType type);
		void addHorizontals(Guides values, GuideType type);
		void addVertical(double value, GuideType type);
		void addVerticals(Guides values, GuideType type);
		void deleteHorizontal(double value, GuideType type);
		void deleteVertical(double value, GuideType type);
		void moveHorizontal(double from, double to, GuideType type);
		void moveVertical(double from, double to, GuideType type);

		Guides horizontals(GuideType type);
		Guides verticals(GuideType type);
		double horizontal(uint ix, GuideType type);
		double vertical(uint ix, GuideType type);

		void clearHorizontals(GuideType type);
		void clearVerticals(GuideType type);

		void copy(GuideManagerCore *target);
		void copy(GuideManagerCore *target, GuideType type);

		void drawPage(ScPainter *p, ScribusDoc *doc, double lineWidth);

		bool isMouseOnHorizontal(double low, double high, GuideType type);
		bool isMouseOnVertical(double low, double high, GuideType type);

		void setPage(Page *p);

	private:
		UndoManager * const undoManager;
		Page* page;
		Guides horizontalStdG;
		Guides verticalStdG;
		Guides horizontalAutoG;
		Guides verticalAutoG;
};

#endif
