/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef COLORLISTBOX_H
#define COLORLISTBOX_H

#include <qlistbox.h>
#include <qcolor.h>
#include <qguardedptr.h>

#include "colorsetmanager.h"
#include "scribusapi.h"
#include "scribusstructs.h"
#include "sclistboxpixmap.h"
#include "query.h"
#include "scguardedptr.h"

class DynamicTip;
class ScribusDoc;
class ColorListBox;

class SCRIBUS_API ColorSmallPixmapItem : public ScListBoxPixmap<15,15>
{
	protected:
		ScColor m_color;
		ScGuardedPtr<ScribusDoc> m_doc;
	public:
		ColorSmallPixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName );
		~ColorSmallPixmapItem() {};

		virtual void redraw(void);
		virtual int rtti() const { return 654873547; };
};

class SCRIBUS_API ColorWidePixmapItem : public ScListBoxPixmap<30,15>
{
	protected:
		ScColor m_color;
		ScGuardedPtr<ScribusDoc> m_doc;
	public:
		ColorWidePixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName );
		~ColorWidePixmapItem() {};

		virtual void redraw(void);
		virtual int rtti() const { return 654873548; };
};

class SCRIBUS_API ColorFancyPixmapItem : public ScListBoxPixmap<60,15>
{
	protected:
		ScColor m_color;
		ScGuardedPtr<ScribusDoc> m_doc;
	public:
		ColorFancyPixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName );
		~ColorFancyPixmapItem() {};

		virtual void redraw(void);
		virtual int rtti() const { return 654873549; };
};

/*! \brief Very nice list box with color names and samples.
It's inherited from QListBox with all its methods and properties.
I create it as separate class because it's used now in ColorManager
and ColorWheel too. You can see it in Extras/Color Wheel or in
Edit/Colors dialogs in action.
\author Petr Vanek <petr@yarpen.cz>
*/
class SCRIBUS_API ColorListBox : public QListBox
{
	Q_OBJECT

	public:

		enum PixmapType
		{
			smallPixmap,
			widePixmap,
			fancyPixmap
		};

		/*! \brief Standard QListBox like constructor.
		Just there are initialized pixmaps for icon drawing. */
		ColorListBox(QWidget * parent = 0, const char * name = 0, WFlags f = 0);

		/*! \brief Fill the list box with values taken from list.
		The list is cleared itself. Then is rendered an icon with
		color attributes (RGB/CMYK/Spot etc.).
		\param list a ColorList to present. 
		\param the pixmap type to use */
		void updateBox(ColorList& list, ColorListBox::PixmapType type);

		/*! \brief Fill the list box with values taken from list.
		The list is not cleared before items insertion.
		\param list a ColorList to present. 
		\param the pixmap type to use */
		void insertItems(ColorList& list, ColorListBox::PixmapType type);

		/*! \brief Insert ColorSmallPixmapItems into the list
		\param list a ColorList to present. */
		void insertSmallPixmapItems(ColorList& list);

		/*! \brief Insert ColorWidePixmapItems into the list
		\param list a ColorList to present. */
		void insertWidePixmapItems(ColorList& list);

		/*! \brief Insert ColorFancyPixmapItems into the list
		\param list a ColorList to present. */
		void insertFancyPixmapItems(ColorList& list);
};

#endif
