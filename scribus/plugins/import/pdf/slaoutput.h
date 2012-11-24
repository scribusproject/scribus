/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SLAOUTPUT_H
#define SLAOUTPUT_H

#include <QString>
#include <QTextStream>
#include <QSizeF>
#include <QBuffer>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QImage>
#include <QList>
#include <QTransform>
#include <QStack>
#include <QDebug>
#include "fpointarray.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "vgradient.h"

#include <poppler/goo/gtypes.h>
#include <poppler/Object.h>
#include <poppler/OutputDev.h>
#include <poppler/Gfx.h>
#include <poppler/GfxState.h>
#include <poppler/Stream.h>
#include <poppler/GfxFont.h>
#include <poppler/Link.h>
#include <poppler/PDFDoc.h>
#include <poppler/Error.h>
#include <poppler/Form.h>
#include <poppler/Page.h>
#include <poppler/Catalog.h>
#include <poppler/CharCodeToUnicode.h>
#include <poppler/FontEncodingTables.h>
#include <poppler/splash/SplashFontFileID.h>
#include <poppler/splash/SplashFontFile.h>
#include <poppler/splash/SplashFontEngine.h>
#include <poppler/splash/SplashFont.h>
#include <poppler/splash/SplashMath.h>
#include <poppler/splash/SplashPath.h>
#include <poppler/splash/SplashGlyphBitmap.h>

//------------------------------------------------------------------------
// LinkSubmitData
//------------------------------------------------------------------------

class LinkSubmitForm: public LinkAction
{
public:
	// Build a LinkImportData from an action dictionary.
	LinkSubmitForm(Object *actionObj);
	// Destructor.
	virtual ~LinkSubmitForm();
	// Was the LinkImportData created successfully?
	virtual GBool isOk() { return fileName != NULL; }
	// Accessors.
	virtual LinkActionKind getKind() { return actionUnknown; }
	GooString *getFileName() { return fileName; }
	int getFlags() { return m_flags; }
private:
	GooString *fileName;		// file name
	int m_flags;
};

//------------------------------------------------------------------------
// LinkImportData
//------------------------------------------------------------------------

class LinkImportData: public LinkAction
{
public:
	// Build a LinkImportData from an action dictionary.
	LinkImportData(Object *actionObj);
	// Destructor.
	virtual ~LinkImportData();
	// Was the LinkImportData created successfully?
	virtual GBool isOk() { return fileName != NULL; }
	// Accessors.
	virtual LinkActionKind getKind() { return actionUnknown; }
	GooString *getFileName() { return fileName; }
private:
	GooString *fileName;		// file name
};
//------------------------------------------------------------------------
// SplashOutFontFileID
//------------------------------------------------------------------------

class SplashOutFontFileID: public SplashFontFileID
{
public:

	SplashOutFontFileID(Ref *rA) { r = *rA; }
	~SplashOutFontFileID() {}
	GBool matches(SplashFontFileID *id)
	{
		return ((SplashOutFontFileID *)id)->r.num == r.num && ((SplashOutFontFileID *)id)->r.gen == r.gen;
	}

private:
	Ref r;
};


class AnoOutputDev : public OutputDev
{
public:
	AnoOutputDev(ScribusDoc* doc, QStringList *importedColors);
	virtual ~AnoOutputDev();
	GBool isOk() { return gTrue; }
	virtual GBool upsideDown() { return gTrue; }
	virtual GBool useDrawChar() { return gFalse; }
	virtual GBool interpretType3Chars() { return gFalse; }
	virtual GBool useTilingPatternFill() { return gFalse; }
	virtual GBool useShadedFills(int type) { return gFalse; }
	virtual GBool useFillColorStop() { return gFalse; }
	virtual GBool useDrawForm() { return gFalse; }
	virtual void stroke(GfxState *state);
	virtual void eoFill(GfxState *state);
	virtual void fill(GfxState *state);
	virtual void drawString(GfxState *state, GooString *s);
	QString CurrColorText;
	QString CurrColorFill;
	QString CurrColorStroke;
	double m_fontSize;
	GooString *m_fontName;
	GooString *m_itemText;
private:
	QString getColor(GfxColorSpace *color_space, GfxColor *color, int *shade);
	ScribusDoc* m_doc;
	QStringList *m_importedColors;
};


class SlaOutputDev : public OutputDev
{
public:
	SlaOutputDev(ScribusDoc* doc, QList<PageItem*> *Elements, QStringList *importedColors, int flags);
	virtual ~SlaOutputDev();
	LinkAction* SC_getAction(AnnotWidget *ano);
	LinkAction* SC_getAdditionalAction(const char *key, AnnotWidget *ano);
	static GBool annotations_callback(Annot *annota, void *user_data);
	bool handleTextAnnot(Annot* annota, double xCoor, double yCoor, double width, double height);
	bool handleLinkAnnot(Annot* annota, double xCoor, double yCoor, double width, double height);
	bool handleWidgetAnnot(Annot* annota, double xCoor, double yCoor, double width, double height);
	void applyTextStyle(PageItem* ite, QString fontName, QString textColor, double fontSize);
	void handleActions(PageItem* ite, AnnotWidget *ano);
	void startDoc(PDFDoc *doc, XRef *xrefA, Catalog *catA);

	GBool isOk() { return gTrue; }
	virtual GBool upsideDown() { return gTrue; }
	virtual GBool useDrawChar() { return gTrue; }
	virtual GBool interpretType3Chars() { return gTrue; }
	virtual GBool useTilingPatternFill() { return gTrue; }
	virtual GBool useShadedFills(int type) { return type <= 7; }
	virtual GBool useFillColorStop() { return gTrue; }
	virtual GBool useDrawForm() { return gFalse; }
	virtual void startPage(int pageNum, GfxState *);
	virtual void endPage();
	// grapics state
	virtual void saveState(GfxState *state);
	virtual void restoreState(GfxState *state);

	//----- path painting
	virtual void stroke(GfxState *state);
	virtual void fill(GfxState *state);
	virtual void eoFill(GfxState *state);
	GBool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, Object *str, double *pmat, int paintType, int tilingType, Dict *resDict, double *mat, double *bbox, int x0, int y0, int x1, int y1, double xStep, double yStep);
	virtual GBool axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax);
	virtual GBool axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading) { return (shading->getExtend0() == shading->getExtend1()); }
	virtual GBool radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax);
	virtual GBool radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading) { return (shading->getExtend0() == shading->getExtend1()); }
	virtual GBool gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading);
	virtual GBool patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading);
	//----- path clipping
	virtual void clip(GfxState *state);
	virtual void eoClip(GfxState *state);
	virtual void clipToStrokePath(GfxState * /*state*/) { qDebug() << "Clip to StrokePath"; }
	virtual GBool deviceHasTextClip(GfxState *state) { return gFalse; }

  // If current colorspace is pattern,
  // does this device support text in pattern colorspace?
	virtual GBool supportTextCSPattern(GfxState *state)
	{
		return state->getFillColorSpace()->getMode() == csPattern;
	}

  // If current colorspace is pattern,
  // need this device special handling for masks in pattern colorspace?
	virtual GBool fillMaskCSPattern(GfxState * state)
	{
		return state->getFillColorSpace()->getMode() == csPattern;
	}

	virtual void endMaskClip(GfxState *state) { qDebug() << "End Mask Clip"; }

  //----- grouping operators
	virtual void beginMarkedContent(char *name, Dict *properties);
	virtual void beginMarkedContent(char *name, Object *dictRef);
	virtual void endMarkedContent(GfxState *state);
	virtual void markPoint(char *name);
	virtual void markPoint(char *name, Dict *properties);
	//----- image drawing
	virtual void drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool interpolate, GBool inlineImg);
	virtual void drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool interpolate, int *maskColors, GBool inlineImg);
	virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   GBool interpolate,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap,
				   GBool maskInterpolate);

	virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   GBool interpolate,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GBool maskInvert, GBool maskInterpolate) { qDebug() << "Draw Masked Image"; }

	//----- transparency groups and soft masks
	virtual void beginTransparencyGroup(GfxState *state, double *bbox, GfxColorSpace * /*blendingColorSpace*/, GBool /*isolated*/, GBool /*knockout*/, GBool /*forSoftMask*/);
	virtual void paintTransparencyGroup(GfxState *state, double *bbox) {}
	virtual void endTransparencyGroup(GfxState *state);
	virtual void setSoftMask(GfxState * /*state*/, double * /*bbox*/, GBool /*alpha*/, Function * /*transferFunc*/, GfxColor * /*backdropColor*/);
	virtual void clearSoftMask(GfxState * /*state*/);

//	virtual void updateFillColor(GfxState *state);
	virtual void updateFont(GfxState *state);
	//----- text drawing
	virtual void beginTextObject(GfxState *state);
	virtual void endTextObject(GfxState *state);
	virtual void drawChar(GfxState *state, double /*x*/, double /*y*/, double /*dx*/, double /*dy*/, double /*originX*/, double /*originY*/, CharCode /*code*/, int /*nBytes*/, Unicode * /*u*/, int /*uLen*/);
	virtual GBool beginType3Char(GfxState * /*state*/, double /*x*/, double /*y*/, double /*dx*/, double /*dy*/, CharCode /*code*/, Unicode * /*u*/, int /*uLen*/);
	virtual void endType3Char(GfxState * /*state*/);
	virtual void type3D0(GfxState * /*state*/, double /*wx*/, double /*wy*/);
	virtual void type3D1(GfxState * /*state*/, double /*wx*/, double /*wy*/, double /*llx*/, double /*lly*/, double /*urx*/, double /*ury*/);
	//----- form XObjects
	virtual void drawForm(Ref /*id*/) { qDebug() << "Draw Form"; }
	//----- links
	virtual void processLink(AnnotLink * /*link*/) { qDebug() << "Draw Link"; }
	bool layersSetByOCG;

private:
	void getPenState(GfxState *state);
	QString getColor(GfxColorSpace *color_space, GfxColor *color, int *shade);
	QString getAnnotationColor(AnnotColor *color);
	QString convertPath(GfxPath *path);
	int getBlendMode(GfxState *state);
	void applyMask(PageItem *ite);
	void pushGroup(QString maskName = "", GBool forSoftMask = gFalse, GBool alpha = gFalse, bool inverted = false);
	QString UnicodeParsedString(GooString *s1);
	bool pathIsClosed;
	QString CurrColorFill;
	QString CurrColorStroke;
	Qt::PenCapStyle PLineEnd;
	Qt::PenJoinStyle PLineJoin;
	QVector<double> DashValues;
	double DashOffset;
	QString Coords;
	struct clipEntry
	{
		PageItem* ClipItem;
		int grStackDepth;
	};
	QStack<clipEntry> m_clipStack;
	struct groupEntry
	{
		QList<PageItem*> Items;
		GBool forSoftMask;
		GBool alpha;
		QString maskName;
		bool inverted;
	};
	QStack<groupEntry> m_groupStack;
	QString m_currentMask;
	ScribusDoc* m_doc;
	Selection* tmpSel;
	QList<PageItem*> *m_Elements;
	QStringList *m_importedColors;
	QTransform m_ctm;
	struct F3Entry
	{
		QTransform ctm;
		QString glyphRef;
		bool colored;
	};
	QStack<F3Entry> m_F3Stack;
	struct F3GlyphEntry
	{
		QString pattern;
		bool colored;
	};
	QMap<QString, F3GlyphEntry> m_Font_Pattern_Map;
	struct mContent
	{
		QString name;
		QString ocgName;
	};
	QStack<mContent> m_mcStack;
	int grStackDepth;
	int layerNum;
	int currentLayer;
	bool firstLayer;
	int importerFlags;
	int updateGUICounter;
	XRef *xref;		// xref table for current document
	PDFDoc *pdfDoc;
	Catalog *catalog;
	SplashFontEngine *m_fontEngine;
	SplashFont *m_font;
	FormPageWidgets *m_formWidgets;
	int m_actPage;
};

#endif
