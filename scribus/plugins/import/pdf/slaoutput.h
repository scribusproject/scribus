/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SLAOUTPUT_H
#define SLAOUTPUT_H

#include <QBuffer>
#include <QColor>
#include <QBrush>
#include <QDebug>
#include <QImage>
#include <QPen>
#include <QList>
#include <QSizeF>
#include <QStack>
#include <QString>
#include <QTextStream>
#include <QTransform>

#include <memory>

#include "fpointarray.h"
#include "importpdfconfig.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "vgradient.h"

#if POPPLER_ENCODED_VERSION < POPPLER_VERSION_ENCODE(0, 73, 0)
#include <poppler/goo/gtypes.h>
#endif
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
	GBool isOk() POPPLER_CONST override { return fileName != nullptr; }
	// Accessors.
	LinkActionKind getKind() POPPLER_CONST override { return actionUnknown; }
	GooString *getFileName() { return fileName; }
	int getFlags() { return m_flags; }

private:
	GooString *fileName {nullptr};		// file name
	int m_flags {0};
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
	GBool isOk() POPPLER_CONST override { return fileName != nullptr; }
	// Accessors.
	LinkActionKind getKind() POPPLER_CONST override { return actionUnknown; }
	GooString *getFileName() { return fileName; }

private:
	GooString *fileName {nullptr};		// file name
};

//------------------------------------------------------------------------
// SlaOutFontFileID
//------------------------------------------------------------------------

class SlaOutFontFileID: public SplashFontFileID
{
public:
	SlaOutFontFileID(const Ref *rA) { r = *rA; }
	~SlaOutFontFileID() {}

	GBool matches(SplashFontFileID *id) override
	{
		return ((SlaOutFontFileID*) id)->r.num == r.num && ((SlaOutFontFileID *) id)->r.gen == r.gen;
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
	GBool upsideDown() override { return gTrue; }
	GBool useDrawChar() override { return gFalse; }
	GBool interpretType3Chars() override { return gFalse; }
	GBool useTilingPatternFill() override  { return gFalse; }
	GBool useShadedFills(int type) override { return gFalse; }
	GBool useFillColorStop() override { return gFalse; }
	GBool useDrawForm() override { return gFalse; }

	void stroke(GfxState *state) override;
	void eoFill(GfxState *state) override;
	void fill(GfxState *state) override;
	void drawString(GfxState *state, POPPLER_CONST GooString *s) override;

	QString currColorText;
	QString currColorFill;
	QString currColorStroke;
	double  fontSize {12};
	GooString *fontName {nullptr};
	GooString *itemText {nullptr};

private:
	QString getColor(GfxColorSpace *color_space, POPPLER_CONST_070 GfxColor *color, int *shade);
	ScribusDoc* m_doc;
	QStringList *m_importedColors;
};



class SlaOutputDev : public OutputDev
{
public:
	SlaOutputDev(ScribusDoc* doc, QList<PageItem*> *Elements, QStringList *importedColors, int flags);
	virtual ~SlaOutputDev();

	LinkAction* SC_getAction(AnnotWidget *ano);
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(0, 86, 0)
	std::unique_ptr<LinkAction> SC_getAdditionalAction(const char *key, AnnotWidget *ano);
#else
	LinkAction* SC_getAdditionalAction(const char *key, AnnotWidget *ano);
#endif
	static GBool annotations_callback(Annot *annota, void *user_data);
	bool handleTextAnnot(Annot* annota, double xCoor, double yCoor, double width, double height);
	bool handleLinkAnnot(Annot* annota, double xCoor, double yCoor, double width, double height);
	bool handleWidgetAnnot(Annot* annota, double xCoor, double yCoor, double width, double height);
	void applyTextStyle(PageItem* ite, const QString& fontName, const QString& textColor, double fontSize);
	void handleActions(PageItem* ite, AnnotWidget *ano);
	void startDoc(PDFDoc *doc, XRef *xrefA, Catalog *catA);

	GBool isOk() { return gTrue; }
	GBool upsideDown() override { return gTrue; }
	GBool useDrawChar() override { return gTrue; }
	GBool interpretType3Chars() override { return gTrue; }
	GBool useTilingPatternFill() override { return gTrue; }
	GBool useShadedFills(int type) override { return type <= 7; }
	GBool useFillColorStop() override { return gTrue; }
	GBool useDrawForm() override { return gFalse; }

//	virtual GBool needClipToCropBox() { return gTrue; }
	void startPage(int pageNum, GfxState *, XRef *) override;
	void endPage() override;

	// graphics state
	void saveState(GfxState *state) override;
	void restoreState(GfxState *state) override;

	//----- path painting
	void stroke(GfxState *state) override;
	void fill(GfxState *state) override;
	void eoFill(GfxState *state) override;
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(21, 3, 0)
	bool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, GfxTilingPattern *tPat, const double *mat, int x0, int y0, int x1, int y1, double xStep, double yStep) override;
#else
	GBool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, Object *str, POPPLER_CONST_070 double *pmat, int paintType, int tilingType, Dict *resDict, POPPLER_CONST_070 double *mat, POPPLER_CONST_070 double *bbox, int x0, int y0, int x1, int y1, double xStep, double yStep) override;
#endif
	GBool functionShadedFill(GfxState * /*state*/, GfxFunctionShading * /*shading*/) override { qDebug() << "Function Shaded Fill";  return gFalse; }
	GBool axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax) override;
	GBool axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading)  override { return (shading->getExtend0() == shading->getExtend1()); }
	GBool radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax) override;
	GBool radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading) override { return (shading->getExtend0() == shading->getExtend1()); }
	GBool gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading) override;
	GBool patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading) override;

	//----- path clipping
	void clip(GfxState *state) override;
	void eoClip(GfxState *state) override;
	void clipToStrokePath(GfxState * /*state*/) override { qDebug() << "Clip to StrokePath"; }
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
	void beginMarkedContent(POPPLER_CONST char *name, Dict *properties) override;
	virtual void beginMarkedContent(POPPLER_CONST char *name, Object *dictRef);
	void endMarkedContent(GfxState *state) override;
	void markPoint(POPPLER_CONST char *name) override;
	void markPoint(POPPLER_CONST char *name, Dict *properties) override;

	//----- image drawing
	void drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool interpolate, GBool inlineImg) override;
	void drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool interpolate, POPPLER_CONST_082 int *maskColors, GBool inlineImg) override;
	void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   GBool interpolate,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap,
				   GBool maskInterpolate) override;

	void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   GBool interpolate,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GBool maskInvert, GBool maskInterpolate) override; // { qDebug() << "Draw Masked Image"; }

	//----- transparency groups and soft masks
	void beginTransparencyGroup(GfxState *state, POPPLER_CONST_070 double *bbox, GfxColorSpace * /*blendingColorSpace*/, GBool /*isolated*/, GBool /*knockout*/, GBool /*forSoftMask*/) override;
	void paintTransparencyGroup(GfxState *state, POPPLER_CONST_070 double *bbox) override;
	void endTransparencyGroup(GfxState *state) override;
	void setSoftMask(GfxState * /*state*/, POPPLER_CONST_070 double * /*bbox*/, GBool /*alpha*/, Function * /*transferFunc*/, GfxColor * /*backdropColor*/) override;
	void clearSoftMask(GfxState * /*state*/) override;

	void updateFillColor(GfxState *state) override;
	void updateStrokeColor(GfxState *state) override;
	void updateFont(GfxState* state) override;

	//----- text drawing
	void  beginTextObject(GfxState *state) override;
	void  endTextObject(GfxState *state) override;
	void  drawChar(GfxState *state, double /*x*/, double /*y*/, double /*dx*/, double /*dy*/, double /*originX*/, double /*originY*/, CharCode /*code*/, int /*nBytes*/, POPPLER_CONST_082 Unicode * /*u*/, int /*uLen*/) override;
	GBool beginType3Char(GfxState * /*state*/, double /*x*/, double /*y*/, double /*dx*/, double /*dy*/, CharCode /*code*/, POPPLER_CONST_082 Unicode * /*u*/, int /*uLen*/) override;
	void  endType3Char(GfxState * /*state*/) override;
	void  type3D0(GfxState * /*state*/, double /*wx*/, double /*wy*/) override;
	void  type3D1(GfxState * /*state*/, double /*wx*/, double /*wy*/, double /*llx*/, double /*lly*/, double /*urx*/, double /*ury*/) override;

	//----- form XObjects
	void drawForm(Ref /*id*/) override { qDebug() << "Draw Form"; }

	//----- links
	void processLink(AnnotLink * /*link*/) override { qDebug() << "Draw Link"; }

	bool layersSetByOCG { false };
	double cropOffsetX { 0.0 };
	double cropOffsetY { 0.0 };
	int rotate { 0 };

protected:
	void setItemFillAndStroke(GfxState* state, PageItem* textNode);
	void applyMask(PageItem* ite);
	void pushGroup(const QString& maskName = "", GBool forSoftMask = gFalse, GBool alpha = gFalse, bool inverted = false);

	ScribusDoc* m_doc;
	Qt::PenCapStyle m_lineEnd { Qt::FlatCap };
	Qt::PenJoinStyle m_lineJoin { Qt::MiterJoin };
	QList<PageItem*>* m_Elements;

	class GraphicState
	{
	public:
		QString fillColor { "Black" };
		int     fillShade { 100 };
		QString strokeColor { "Black "};
		int     strokeShade { 100 };

		// The currently visible area. If it is empty, everything is visible.
		// QPainterPath has the drawback that it sometimes approximates Bezier curves
		// by line segments for numerical stability. If available, a better class
		// should be used. However, it is important that the used class knows which
		// areas are covered and does not rely on external information about the fill
		// rule to use.
		QPainterPath clipPath;
	};

	class GraphicStack
	{
	public:
		GraphicStack()
		{
			m_stack.push(GraphicState());
		}

		void clear()
		{
			m_stack.clear();
			m_stack.push(GraphicState());
		}

		void save()
		{
			m_stack.push(m_stack.top());
		}

		void restore()
		{
			if (m_stack.count() > 0)
				m_stack.pop();
			if (m_stack.count() == 0)
				m_stack.push(GraphicState());
		}

		GraphicState& top() { return m_stack.top(); }
		const GraphicState& top() const  { return m_stack.top(); }

	private:
		QStack<GraphicState> m_stack;
	};

	struct groupEntry
	{
		QList<PageItem*> Items;
		GBool forSoftMask { gFalse };
		GBool isolated { gFalse };
		GBool alpha { gFalse };
		QString maskName;
		QPointF maskPos;
		bool inverted { false };
	};

	QStack<groupEntry> m_groupStack;
	GraphicStack m_graphicStack;

private:
	void getPenState(GfxState *state);
	QString getColor(GfxColorSpace *color_space, POPPLER_CONST_070 GfxColor *color, int *shade);
	QString getAnnotationColor(const AnnotColor *color);
	QString convertPath(POPPLER_CONST_083 GfxPath *path);
	int getBlendMode(GfxState *state);
	QString UnicodeParsedString(POPPLER_CONST GooString *s1);
	QString UnicodeParsedString(const std::string& s1);
	bool checkClip();

	// Intersect the current clip path with the new path in state where filled areas
	// are interpreted according to fillRule.
	void adjustClip(GfxState *state, Qt::FillRule fillRule);

	// Take the current path of state and interpret it according to fillRule,
	// intersect it with the clipping path and create a new pageitem for it.
	void createFillItem(GfxState *state, Qt::FillRule fillRule);

	void createImageFrame(QImage& image, GfxState *state, int numColorComponents);

	bool m_pathIsClosed { false };
	QVector<double> m_dashValues;
	double m_dashOffset { 0.0 };
	QString m_coords;

	// Collect the paths of character glyphs for clipping of a whole text group.
	QPainterPath  m_clipTextPath;

	QString m_currentMask;
	QPointF m_currentMaskPosition;
	Selection* m_tmpSel { nullptr };
	QStringList *m_importedColors;
	QTransform m_ctm;
	struct F3Entry
	{
		bool colored;
	};
	QStack<F3Entry> m_F3Stack;
	struct mContent
	{
		QString name;
		QString ocgName;
	};
	QStack<mContent> m_mcStack;
	int m_inPattern {0};
	int m_layerNum {1};
	int m_currentLayer { 0 };
	bool m_firstLayer {true};
	int m_importerFlags;
	int m_updateGUICounter {0};
	XRef *m_xref {nullptr};		// xref table for current document
	PDFDoc *m_pdfDoc {nullptr};
	Catalog *m_catalog {nullptr};
	SplashFontEngine *m_fontEngine {nullptr};
	SplashFont *m_font {nullptr};
#if POPPLER_ENCODED_VERSION >= POPPLER_VERSION_ENCODE(21, 4, 0)
	std::unique_ptr<FormPageWidgets> m_formWidgets;
#else
	FormPageWidgets *m_formWidgets {nullptr};
#endif
	QHash<QString, QList<int> > m_radioMap;
	QHash<int, PageItem*> m_radioButtons;
	int m_actPage { 1 };
};
#endif
