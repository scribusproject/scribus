/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef GUIDEMANAGERCORE_H
#define GUIDEMANAGERCORE_H

#include <qpair.h>
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

	QPair<double, double> topLeft(double x, double y) const;
	QPair<double, double> topRight(double x, double y) const;
	QPair<double, double> bottomLeft(double x, double y) const;
	QPair<double, double> bottomRight(double x, double y) const;

	/*! \brief Read the guides from XML attribute (file opening).
	It's statis method sou you can call it without instance initialized:
	GuideManagerCore::readHorizontalGuides(foo blah);
	\param guideString a string with all values separated by space (' '). E.g. "1.0 23.17 6"
	\param page a reference to the Page object to append the separated guideString values
	\param type Guide type to load
	\param useOldGuides A little bit hacking here. The guides were stored in a little mess
	in the ancient times. So when is the obsolete XML attribute found in reading document
	the old reading method is used. All guides are saved in new format then. */
	static void readHorizontalGuides(const QString guideString, Page *page, GuideType type, bool useOldGuides=false);

	
	/*! \brief Read the guides from XML attribute (file opening).
	It's statis method sou you can call it without instance initialized:
	GuideManagerCore::readVerticalGuides(foo blah);
	\param guideString a string with all values separated by space (' '). E.g. "1.0 23.17 6"
	\param page a reference to the Page object to append the separated guideString values
	\param type Guide type to load
	\param useOldGuides A little bit hacking here. The guides were stored in a little mess
	in the ancient times. So when is the obsolete XML attribute found in reading document
	the old reading method is used. All guides are saved in new format then. */
	static void readVerticalGuides(const QString guideString, Page *page, GuideType type, bool useOldGuides=false);

	static QString writeHorizontalGuides(Page *page, GuideType type);
	static QString writeVerticalGuides(Page *page, GuideType type);
private:
	UndoManager * const undoManager;
	Page* m_page;
	Guides horizontalStdG;
	Guides verticalStdG;
	Guides horizontalAutoG;
	Guides verticalAutoG;

	double closestHorAbove(double y) const;
	double closestHorBelow(double y) const;
	double closestVertLeft(double x) const;
	double closestVertRight(double x) const;
};

#endif
