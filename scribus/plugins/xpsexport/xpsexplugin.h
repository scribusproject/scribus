/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          xpsexport.cpp  -  description
                             -------------------
    begin                : Sun Nov 24 08:00:00 CEST 2013
    copyright            : (C) 2013 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef XPSPLUG_H
#define XPSPLUG_H

#include <QObject>
#include <QDomElement>
#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "tableborder.h"

class QString;
class QTemporaryDir;
class ScLayer;
class ScribusDoc;
class ScribusMainWindow;
class PageItem;
class ScPage;
class ScText;
class ScZipHandler;

class PLUGIN_API XPSExportPlugin : public ScActionPlugin
{
	Q_OBJECT

public:
	// Standard plugin implementation
	XPSExportPlugin();
	virtual ~XPSExportPlugin();
	/*!
	\author Franz Schmid
	\brief Run the XPS export
	\param filename a file to export to
	\retval bool true
	*/
	virtual bool run(ScribusDoc* doc=0, QString filename = QString::null);
	virtual const QString fullTrName() const;
	virtual const AboutData* getAboutData() const;
	virtual void deleteAboutData(const AboutData* about) const;
	virtual void languageChange();
	virtual void addToMainWindowMenu(ScribusMainWindow *) {};
};

extern "C" PLUGIN_API int xpsexplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* xpsexplugin_getPlugin();
extern "C" PLUGIN_API void xpsexplugin_freePlugin(ScPlugin* plugin);

class XPSExPlug : public QObject
{
    Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\brief Create the XPS exporter window
	\param fName QString file name
	 */
	XPSExPlug(ScribusDoc* doc, int output_res);
	~XPSExPlug();

	bool doExport(QString fName);

private:
	void writePages(QDomElement &root);
	void writePage(QDomElement &doc_root, QDomElement &rel_root, ScPage *Page);
	void writePageLayer(QDomElement &doc_root, QDomElement &rel_root, ScPage *page, ScLayer& layer);
	void writeItemOnPage(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void handleImageFallBack(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processPolyItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processLineItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processImageItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processTextItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processPathTextItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processSymbolItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processTableItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void paintBorder(const TableBorder &border, const QPointF &start, const QPointF &end, const QPointF &startOffsetFactors, const QPointF &endOffsetFactors, QDomElement &ob);
	void processSymbolStroke(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processArrows(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void drawArrow(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, FPointArray &arrow);
	QString embedFont(const ScFace *font, QDomElement &rel_root);
	void GetMultiStroke(struct SingleLine *sl, QDomElement &parentElem);
	void getStrokeStyle(PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset, bool forArrow = false);
	void getFillStyle(PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset, bool withTransparency = true);
	void handleMask(int type, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset);
	QString SetColor(QString farbe, int shad, double transparency);
	QString SetClipPath(FPointArray *ite, bool closed);
	void writeDocRels();
	void writeCore();
	void writeContentType();
	void writeBaseRel();
	QString FToStr(double c);
	QString IToStr(int c);
	QString MatrixToStr(QTransform &mat);
	int hex2int(char hex);
	bool checkForFallback(PageItem *Item);
	ScribusDoc* m_Doc;
	ScZipHandler *zip;
	QTemporaryDir *dir;
	QString baseDir;
	QDomDocument f_docu;
	QDomDocument p_docu;
	QDomDocument r_docu;
	double conversionFactor;
	int imageCounter;
	int fontCounter;
	QMap<QString, QString> xps_fontMap;
	struct txtRunItem
	{
		ScText* hl;
		CharStyle style;
		int index;
		double CurX;
	};
	double m_dpi;
};

#endif
