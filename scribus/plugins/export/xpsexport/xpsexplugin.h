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
#include <QMap>
#include <QSet>

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

struct XPSResourceInfo
{
	QString id;
	QString uri;
};

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
	bool run(ScribusDoc* doc = nullptr, const QString& filename = QString()) override;
	QString fullTrName() const override;
	const AboutData* getAboutData() const override;
	void deleteAboutData(const AboutData* about) const override;
	void languageChange() override;
	void addToMainWindowMenu(ScribusMainWindow *) override {};
};

extern "C" PLUGIN_API int xpsexplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* xpsexplugin_getPlugin();
extern "C" PLUGIN_API void xpsexplugin_freePlugin(ScPlugin* plugin);

class XPSExPlug : public QObject
{
	Q_OBJECT

	friend class XPSPainter;
public:
	/*!
	\author Franz Schmid
	\brief Create the XPS exporter window
	\param fName QString file name
	 */
	XPSExPlug(ScribusDoc* doc, int output_res);
	~XPSExPlug();

	bool doExport(const QString& fName);

private:
	void writePages(QDomElement &root);
	void writePage(QDomElement &doc_root, QDomElement &rel_root, ScPage *Page);
	void writePageLayer(QDomElement &doc_root, QDomElement &rel_root, ScPage *page, ScLayer& layer);
	void writeItemOnPage(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void handleImageFallBack(PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processPolyItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processLineItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processImageItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processTextItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processSymbolItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processTableItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void paintBorder(const TableBorder &border, const QPointF &start, const QPointF &end, const QPointF &startOffsetFactors, const QPointF &endOffsetFactors, QDomElement &ob);
	void processHatchFill(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processSymbolStroke(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void processArrows(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root);
	void drawArrow(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, FPointArray &arrow);
	void addFontRelationship(QDomElement &rel_root, const XPSResourceInfo& fontInfo);
	XPSResourceInfo embedFont(const ScFace& font, QDomElement &rel_root);
	void getMultiStroke(struct SingleLine *sl, QDomElement &parentElem);
	void getStrokeStyle(PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset, bool forArrow = false);
	void getFillStyle(PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset, bool withTransparency = true);
	void handleMask(int type, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset);
	QString setColor(const QString& farbe, int shad, double transparency);
	void    setClipAttr(QDomElement &elem, FPointArray *ite, bool fillRule);
	QString setClipPath(FPointArray *ite, bool closed);
	void writeDocRels();
	void writeCore();
	void writeContentType();
	void writeBaseRel();
	QString FToStr(double c);
	QString IToStr(int c);
	QString MatrixToStr(QTransform &mat);
	QString MatrixToStr(QTransform &mat, double factor);
	int hex2int(char hex);
	bool checkForFallback(PageItem *Item);

	ScribusDoc* m_Doc {nullptr};
	QString baseDir;
	QDomDocument f_docu;
	QDomDocument p_docu;
	QDomDocument r_docu;
	double conversionFactor { 96.0 / 72.0 };
	int imageCounter { 0 };
	int fontCounter { 0 };
	QMap<QString, XPSResourceInfo> xps_fontMap;
	QSet<QString> xps_fontRel;
	struct txtRunItem
	{
		QChar chr;
		GlyphLayout* glyphs {nullptr};
		CharStyle style;
		int index {0};
		double CurX {0.0};
		PageItem* embItem {nullptr};
	};
	double m_dpi {0.0};
};

#endif
