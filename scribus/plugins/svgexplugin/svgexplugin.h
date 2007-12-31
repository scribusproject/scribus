/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SVGPLUG_H
#define SVGPLUG_H

#include <QObject>
#include <QDomElement>
#include "pluginapi.h"
#include "loadsaveplugin.h"

#ifdef HAVE_CAIRO
	#include <cairo.h>
//	#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 1, 6)
		#define USECAIRO
		#include "scpainter.h"
//	#else
//		#undef USECAIRO
//	#endif
#else
	#undef USECAIRO
#endif

class QString;
class ScribusDoc;
class ScribusMainWindow;
class ScribusView;
class PageItem;
class Page;
class ScText;

class PLUGIN_API SVGExportPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		SVGExportPlugin();
		virtual ~SVGExportPlugin();
		/*!
		\author Franz Schmid
		\brief Run the SVG export
		\param filename a file to export to
		\retval bool true
		*/
		virtual bool run(ScribusDoc* doc=0, QString filename = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features (none)
};

extern "C" PLUGIN_API int svgexplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* svgexplugin_getPlugin();
extern "C" PLUGIN_API void svgexplugin_freePlugin(ScPlugin* plugin);

class SVGExPlug : public QObject
{
    Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\brief Create the SVG exporter window
	\param fName QString file name
	 */
	SVGExPlug( ScribusDoc* doc );
	~SVGExPlug();

	bool doExport( QString fName ); 

private:
	ScribusDoc* m_Doc;
	ScribusMainWindow* m_ScMW;
	ScribusView* m_View;
#ifndef USECAIRO

		/*!
		\author Franz Schmid
		\brief Process a page to export to SVG format
		\param Seite Page *
		\param docu QDomDocument *
		\param elem QDomElement *
		*/
		void ProcessPage(Page *Seite, QDomDocument *docu, QDomElement *elem);
		QString SetClipPathImage(PageItem *ite);
		/*!
		\author Franz Schmid
		\param ite PageItem *
		\retval QString Clipping Path
		*/
		QString SetClipPath(PageItem *ite);
		/*!
		\author Franz Schmid
		\brief Converts double to string
		\param c double
		\retval QString
		 */
		QString FToStr(double c);
		/*!
		\author Franz Schmid
		\brief Converts integer to QString
		\param c int
		\retval QString representation of value
 		*/
		QString IToStr(int c);
		/*!
		\author Franz Schmid
		\brief Set text properties
		\param tp QDomElement *
		\param hl ScText *
		*/
		void SetTextProps(QDomElement *tp, ScText *hl);
		/*!
		\author Franz Schmid
		\param farbe QString color
		\param shad int
		\param plug ScribusMainWindow *
		\retval QString Colour settings
		*/
		QString SetColor(QString farbe, int shad);
		/*!
		\author Franz Schmid
		\param sl struct SingleLine *
		\param Item PageItem *
		\retval QString Stroke settings
		*/
		QString GetMultiStroke(struct SingleLine *sl, PageItem *Item);
		int GradCount;
		int ClipCount;
#endif
};

#endif // CMSPLUG_H
