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

class QString;
class ScribusDoc;
class ScribusMainWindow;
class PageItem;
class Page;
class ScText;

struct SVGOptions
{
	bool inlineImages;
	bool exportPageBackground;
	bool compressFile;
};

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

	bool doExport( QString fName, SVGOptions &Opts );
	SVGOptions Options;

private:
	ScribusDoc* m_Doc;
	/*!
	\author Franz Schmid
	\brief Process a page to export to SVG format
	\param Seite Page *
	*/
	void ProcessPage(Page *page);
	void ProcessItemOnPage(double xOffset, double yOffset, PageItem *Item, QDomElement *parentElem);
	QDomElement processPolyItem(PageItem *Item, QString trans, QString fill, QString stroke);
	QDomElement processLineItem(PageItem *Item, QString trans, QString stroke);
	QDomElement processImageItem(PageItem *Item, QString trans, QString fill, QString stroke);
	QDomElement processTextItem(PageItem *Item, QString trans, QString fill, QString stroke);
	QDomElement processPathTextItem(PageItem *Item, QString trans, QString stroke);
	QDomElement processInlineItem(double xpos, double ypos, QMatrix &finalMat, ScText *hl, bool pathT, QString trans);
	QString handleGlyph(uint chr, ScText *hl);
	QDomElement processArrows(PageItem *Item, QDomElement line, QString trans);
	QString getFillStyle(PageItem *Item);
	QString getStrokeStyle(PageItem *Item);
	/*!
	\author Franz Schmid
	\param ite PageItem *
	\retval QString Clipping Path
	*/
	QString SetClipPath(FPointArray *ite, bool closed);
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
	\param farbe QString color
	\param shad int
	\param plug ScribusMainWindow *
	\retval QString Colour settings
	*/
	QString MatrixToStr(QMatrix &mat);
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
	int PattCount;
	QString baseDir;
	QDomDocument docu;
	QDomElement docElement;
	QDomElement globalDefs;
	QList<QString> glyphNames;
};

#endif
