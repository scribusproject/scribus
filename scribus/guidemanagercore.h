/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef GUIDEMANAGERCORE_H
#define GUIDEMANAGERCORE_H

#include <QPair>

#include "undoobject.h"
#include "scribusapi.h"

class QColor;
class Page;
class ScPainter;
class UndoManager;
class ScribusDoc;
typedef QList<double> Guides;

/*! \brief Core manipulation with the guides.
Basic idea:
- guides are handled "on the fly", no by modal dialog.
- 2 types of guides: Standard = created one by one.
Auto = created by automatic division algorithms.
Automatic guides are kept by 2 ways: 1st its parameters (m_horizontalAutoCount etc.)
to setup the GUI and compute guides itself. 2nd the horizontalAutoG etc. set
with precomputed values from GUI actions for drawing.
- user can move only Stnadard ones
- Auto guides can be deleted only in manipulation dialog
- Auto guides are painted in different color / (probably) with diff. line.
\author Petr vanek <petr@scribus.info>
*/
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

	QPair<double, double> topLeft(double x, double y);// const;
	QPair<double, double> topRight(double x, double y);// const;
	QPair<double, double> bottomLeft(double x, double y);// const;
	QPair<double, double> bottomRight(double x, double y);// const;

	//! \brief Properties for Auto guides remembrance. See GuideManager.
	int horizontalAutoCount() { return m_horizontalAutoCount; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	void setHorizontalAutoCount(int val) { m_horizontalAutoCount = val; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	int verticalAutoCount() { return m_verticalAutoCount; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	void setVerticalAutoCount(int val) { m_verticalAutoCount = val; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	double horizontalAutoGap(){ return m_horizontalAutoGap; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	double verticalAutoGap(){return m_verticalAutoGap; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	void setHorizontalAutoGap(double gap){ m_horizontalAutoGap = gap; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	void setVerticalAutoGap(double gap){ m_verticalAutoGap = gap; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	int horizontalAutoRefer() { return m_horizontalAutoRefer; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	void setHorizontalAutoRefer(int val) { m_horizontalAutoRefer = val; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	int verticalAutoRefer() { return m_verticalAutoRefer; };
	//! \brief Properties for Auto guides remembrance. See GuideManager.
	void setVerticalAutoRefer(int val) { m_verticalAutoRefer = val; };

	/*! \brief Selection/group coordinates
	It's used to simulate the original selection "freezed in time"
	for parent page */
	double gx, gy, gw, gh;


private:
	UndoManager * const undoManager;
	Page* m_page;
	Guides horizontalStdG;
	Guides verticalStdG;
	Guides horizontalAutoG;
	Guides verticalAutoG;

	double m_horizontalAutoGap;
	double m_verticalAutoGap;
	int m_horizontalAutoCount;
	int m_verticalAutoCount;
	int m_horizontalAutoRefer;
	int m_verticalAutoRefer;

	double closestHorAbove(double y);// const;
	double closestHorBelow(double y);// const;
	double closestVertLeft(double x);// const;
	double closestVertRight(double x);// const;
};


/*! \brief A separate clas for Guides IO operations in reading or closing
the documents.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API GuideManagerIO
{
	public:
		GuideManagerIO(){};
		~GuideManagerIO(){};

		/*! \brief Read the guides from XML attribute (file opening).
		It's statis method sou you can call it without instance initialized:
		GuideManagerIO::readHorizontalGuides(foo blah);
		\param guideString a string with all values separated by space (' '). E.g. "1.0 23.17 6"
		\param page a reference to the Page object to append the separated guideString values
		\param type Guide type to load
		\param useOldGuides A little bit hacking here. The guides were stored in a little mess
		in the ancient times. So when is the obsolete XML attribute found in reading document
		the old reading method is used. All guides are saved in new format then. */
		static void readHorizontalGuides(const QString guideString,
										 Page *page,
										 GuideManagerCore::GuideType type,
										 bool useOldGuides=false);

		/*! \brief Read the guides from XML attribute (file opening).
		It's statis method sou you can call it without instance initialized:
		GuideManagerIO::readVerticalGuides(foo blah);
		\param guideString a string with all values separated by space (' '). E.g. "1.0 23.17 6"
		\param page a reference to the Page object to append the separated guideString values
		\param type Guide type to load
		\param useOldGuides A little bit hacking here. The guides were stored in a little mess
		in the ancient times. So when is the obsolete XML attribute found in reading document
		the old reading method is used. All guides are saved in new format then. */
		static void readVerticalGuides(const QString guideString,
									   Page *page,
									   GuideManagerCore::GuideType type,
									   bool useOldGuides=false);

		static QString writeHorizontalGuides(Page *page, GuideManagerCore::GuideType type);
		static QString writeVerticalGuides(Page *page, GuideManagerCore::GuideType type);

		static void readSelection(const QString guideString, Page *page);
		static QString writeSelection(Page *page);
};

#endif
