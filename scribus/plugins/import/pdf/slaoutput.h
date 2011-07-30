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
#include "selection.h"
#include "vgradient.h"

#include <goo/gtypes.h>
#include <Object.h>
#include <OutputDev.h>
#include <Gfx.h>
#include <GfxState.h>
#include <Stream.h>
#include <GfxFont.h>
#include <PDFDoc.h>
#include <Error.h>
#include <Page.h>
#include <Catalog.h>
#include <CharCodeToUnicode.h>
#include <FontEncodingTables.h>
#include <fofi/FoFiTrueType.h>
#include <splash/SplashFontFileID.h>
#include <splash/SplashFontFile.h>
#include <splash/SplashFontEngine.h>
#include <splash/SplashFont.h>
#include <splash/SplashMath.h>
#include <splash/SplashPath.h>
#include <splash/SplashGlyphBitmap.h>

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


class SlaOutputDev : public OutputDev
{
public:
	SlaOutputDev(ScribusDoc* doc, QList<PageItem*> *Elements, QStringList *importedColors, int flags);
	virtual ~SlaOutputDev();
	void startDoc(XRef *xrefA, Catalog *catA);

	GBool isOk() { return gTrue; }
	virtual GBool upsideDown() { return gTrue; }
	virtual GBool useDrawChar() { return gTrue; }
	virtual GBool interpretType3Chars() { return gTrue; }
	virtual GBool useTilingPatternFill() { return gTrue; }
	virtual GBool useShadedFills(int type) { return type <= 7; }
	virtual GBool useFillColorStop() { return gTrue; }
	virtual void startPage(int pageNum, GfxState *state);
	virtual void endPage();
	// grapics state
	virtual void saveState(GfxState *state);
	virtual void restoreState(GfxState *state);

	//----- path painting
	virtual void stroke(GfxState *state);
	virtual void fill(GfxState *state);
	virtual void eoFill(GfxState *state);
	virtual void clipToStrokePath(GfxState *state) { qDebug() << "clip to stroke path"; }
	virtual GBool tilingPatternFill(GfxState *state, Catalog *cat, Object *str, double *pmat, int paintType, Dict *resDict,
				  double *mat, double *bbox, int x0, int y0, int x1, int y1, double xStep, double yStep);
	virtual GBool axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax);
	virtual GBool axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading) { return (shading->getExtend0() == shading->getExtend1()); }
	virtual GBool radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax);
	virtual GBool radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading) { return (shading->getExtend0() == shading->getExtend1()); }
	virtual GBool gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading) { qDebug() << "triangle shaded fill"; return gTrue; }
	virtual GBool patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading);
	//----- path clipping
	virtual void clip(GfxState *state);
	virtual void eoClip(GfxState *state);
	//----- text drawing
	virtual void beginTextObject(GfxState *state);
	virtual GBool deviceHasTextClip(GfxState *state) { return gFalse; }
	virtual void endTextObject(GfxState *state);
	virtual void drawChar(GfxState *state, double /*x*/, double /*y*/, double /*dx*/, double /*dy*/, double /*originX*/, double /*originY*/, CharCode /*code*/, int /*nBytes*/, Unicode * /*u*/, int /*uLen*/);

  // If current colorspace is pattern,
  // does this device support text in pattern colorspace?
	virtual GBool supportTextCSPattern(GfxState *state) {
	  return state->getFillColorSpace()->getMode() == csPattern; }

  // If current colorspace is pattern,
  // need this device special handling for masks in pattern colorspace?
	virtual GBool fillMaskCSPattern(GfxState * state) {
	  return state->getFillColorSpace()->getMode() == csPattern; }

	virtual void endMaskClip(GfxState *state) { qDebug() << "End Mask Clip"; }

  //----- grouping operators
	virtual void beginMarkedContent(char *name, Dict *properties);
	virtual void endMarkedContent(GfxState *state);
	//----- image drawing
	virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
				 int width, int height, GBool invert, GBool interpolate,
				 GBool inlineImg)  { qDebug() << "Draw Image Mask"; }
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

	virtual void updateFillColor(GfxState *state);
	virtual void updateFont(GfxState *state);

private:
	void getPenState(GfxState *state);
	QString getColor(GfxColorSpace *color_space, GfxColor *color);
	QString convertPath(GfxPath *path);
	int getBlendMode(GfxState *state);
	void applyMask(PageItem *ite);
	void pushGroup(QString maskName = "", GBool forSoftMask = gFalse, GBool alpha = gFalse, bool inverted = false);
	bool pathIsClosed;
	bool firstLayer;
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
		FPointArray ClipCoords;
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
	int grStackDepth;
	int layerNum;
	int currentLayer;
	int importerFlags;
	bool firstPage;
	int pagecount;
	XRef *xref;		// xref table for current document
	Catalog *catalog;
	SplashFontEngine *m_fontEngine;
	SplashFont *m_font;
};

#endif
