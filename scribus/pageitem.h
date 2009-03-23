/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem.h  -  description
                             -------------------
    begin                : Sat Apr 7 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <QObject>
#include <QWidget>
#include <QPolygon>
#include <QStack>
#include <QList>
#include <QKeyEvent>
#include <QMenu>
#include <QRect>
#include <QRectF>
#include <QVector>
#include <QTemporaryFile>

#include "scribusapi.h"
#include "annotation.h"
#include "observable.h"
#include "pagestructs.h"
#include "scimage.h"
#include "sctextstruct.h"
#include "undoobject.h"
#include "vgradient.h"
#include "text/nlsconfig.h"
#include "text/storytext.h"
#include "desaxe/saxio.h"

class ScPainter;
class ScribusDoc;
class SimpleState;
class UndoManager;
class UndoState;
class ResourceCollection;
class QFrame;
class QGridLayout;

class PageItem_ImageFrame;
class PageItem_Line;
class PageItem_Polygon;
class PageItem_PolyLine;
class PageItem_TextFrame;
class PageItem_PathText;
class PageItem_LatexFrame;

struct CopyPasteBuffer;
/**
  *@author Franz Schmid
  */

class SCRIBUS_API PageItem : public QObject, public UndoObject, public SaxIO, public SingleObservable<PageItem>
{
	Q_OBJECT

	// Properties - see http://doc.trolltech.com/3.3/properties.html
	// See the accessors of these properties for details on their use.
	Q_PROPERTY(QString itemName READ itemName WRITE setItemName DESIGNABLE false)
	Q_PROPERTY(QString fillColor READ fillColor WRITE setFillColor DESIGNABLE false)
	Q_PROPERTY(QString lineColor READ lineColor WRITE setLineColor DESIGNABLE false)
	Q_PROPERTY(double fillShade READ fillShade WRITE setFillShade DESIGNABLE false)
	Q_PROPERTY(double lineShade READ lineShade WRITE setLineShade DESIGNABLE false)
	Q_PROPERTY(double fillTransparency READ fillTransparency WRITE setFillTransparency DESIGNABLE false)
	Q_PROPERTY(double lineTransparency READ lineTransparency WRITE setLineTransparency DESIGNABLE false)
	Q_PROPERTY(bool m_Locked READ locked WRITE setLocked DESIGNABLE false)
	Q_PROPERTY(bool m_SizeLocked READ sizeLocked WRITE setSizeLocked DESIGNABLE false)
	Q_PROPERTY(bool m_ImageIsFlippedV READ imageFlippedV WRITE setImageFlippedV DESIGNABLE false)
	Q_PROPERTY(bool m_ImageIsFlippedH READ imageFlippedH WRITE setImageFlippedH DESIGNABLE false)
	Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth DESIGNABLE false)
	Q_PROPERTY(QString customLineStyle READ customLineStyle WRITE setCustomLineStyle DESIGNABLE false)
	Q_PROPERTY(int startArrowIndex READ startArrowIndex WRITE setStartArrowIndex DESIGNABLE false)
	Q_PROPERTY(int endArrowIndex READ endArrowIndex WRITE setEndArrowIndex DESIGNABLE false)

	Q_PROPERTY(bool m_PrintEnabled READ printEnabled WRITE setPrintEnabled DESIGNABLE false)
	Q_PROPERTY(double xPos READ xPos WRITE setXPos DESIGNABLE false)
	Q_PROPERTY(double yPos READ yPos WRITE setYPos DESIGNABLE false)
	Q_PROPERTY(double width READ width WRITE setWidth DESIGNABLE false)
	Q_PROPERTY(double height READ height WRITE setHeight DESIGNABLE false)
	Q_PROPERTY(double rotation READ rotation WRITE setRotation DESIGNABLE false)
	Q_PROPERTY(double imageXScale READ imageXScale WRITE setImageXScale DESIGNABLE false)
	Q_PROPERTY(double imageYScale READ imageYScale WRITE setImageYScale DESIGNABLE false)
	Q_PROPERTY(double imageXOffset READ imageXOffset WRITE setImageXOffset DESIGNABLE false)
	Q_PROPERTY(double imageYOffset READ imageYOffset WRITE setImageYOffset DESIGNABLE false)
	Q_PROPERTY(bool reversed READ reversed WRITE setReversed DESIGNABLE false)
	Q_PROPERTY(double cornerRadius READ cornerRadius WRITE setCornerRadius DESIGNABLE false)
	Q_PROPERTY(double textToFrameDistLeft READ textToFrameDistLeft WRITE setTextToFrameDistLeft DESIGNABLE false)
	Q_PROPERTY(double textToFrameDistRight READ textToFrameDistRight WRITE setTextToFrameDistRight DESIGNABLE false)
	Q_PROPERTY(double textToFrameDistTop READ textToFrameDistTop WRITE setTextToFrameDistTop DESIGNABLE false)
	Q_PROPERTY(double textToFrameDistBottom READ textToFrameDistBottom WRITE setTextToFrameDistBottom DESIGNABLE false)
	Q_PROPERTY(double ColGap READ columnGap WRITE setColumnGap DESIGNABLE false)
	Q_PROPERTY(int Cols READ columns WRITE setColumns DESIGNABLE false)
	Q_ENUMS(FirstLineOffsetPolicy)
	Q_PROPERTY(FirstLineOffsetPolicy firstLineOffset READ firstLineOffset WRITE setFirstLineOffset DESIGNABLE false)
	// FIXME: QMetaProperty can't translate these to/from enumerator names, probably because the
	// properties aren't moc'd in the Qt sources. They work fine in their
	// current state as plain integer properties.
//  	Q_ENUMS(PenStyle)
// 	Q_PROPERTY(PenStyle lineStyle READ lineStyle WRITE setLineStyle DESIGNABLE false)
// 	Q_ENUMS(PenCapStyle)
// 	Q_PROPERTY(PenCapStyle lineEnd READ lineEnd WRITE setLineEnd DESIGNABLE false)
// 	Q_ENUMS(PenJoinStyle)
// 	Q_PROPERTY(PenJoinStyle lineJoin READ lineJoin WRITE setLineJoin DESIGNABLE false)

	// This property may not hang around for too long, but should be useful
	// when testing out the pageitem refactoring work.  Setting it is unlikely
	// to currently have the desired effect.
	/**
	 * @brief Item type.
	 * @warning Do not set this property except for testing and debug purposes.
	 */
	Q_ENUMS(ItemType)
	Q_PROPERTY(ItemType itemType READ itemType WRITE convertTo DESIGNABLE false)

public:
	// Enumerator definitions

	/** @brief Item Type
	 *
	 * Soon, item type will probably go away in favour of using
	 * subclasses and checking types using more conventional methods
	 * and using Qt's MetaObject introspection.
	 * Multiple is used for checking in @sa Selection for all same type of items
	 */
	enum ItemType {
		ItemType1	= 1,
		ImageFrame	= 2,
		ItemType3	= 3,
		TextFrame	= 4,
		Line		= 5,
		Polygon		= 6,
		PolyLine	= 7,
		PathText	= 8,
		LatexFrame  = 9,
		Multiple	= 99
	};

	/** @brief Text flow mode
	 *
	 * Describe if and how text flow around object
	 */
	enum TextFlowMode {
		TextFlowDisabled = 0,
		TextFlowUsesFrameShape  = 1,
		TextFlowUsesBoundingBox = 2,
		TextFlowUsesContourLine = 3,
		TextFlowUsesImageClipping = 4
	};

	/* these do essentially the same as a dynamic cast but might be more readable */
	virtual PageItem_ImageFrame * asImageFrame() { return NULL; }
	virtual PageItem_Line * asLine() { return NULL; }
	virtual PageItem_PathText * asPathText() { return NULL; }
	virtual PageItem_Polygon * asPolygon() { return NULL; }
	virtual PageItem_PolyLine * asPolyLine() { return NULL; }
	virtual PageItem_TextFrame * asTextFrame() { return NULL; }
	virtual PageItem_LatexFrame * asLatexFrame() { return NULL; }


	/** @brief Frame Type
	 *
	 * 
	 */
	enum ItemFrameType {
		Unspecified =-1,
		Rectangle	= 0,
		Ellipse		= 1,
		Round		= 2,
		Other		= 3
	};

protected:
	PageItem(const PageItem & other);
	
public:
	PageItem(ScribusDoc *pa, ItemType newType, double x, double y, double w, double h, double w2, QString fill, QString outline);
	~PageItem()
	{
		if (tempImageFile != NULL)
			delete tempImageFile;
	}

	static const Xml_string saxxDefaultElem;
	static void  desaxeRules(const Xml_string& prefixPattern, desaxe::Digester& ruleset, Xml_string elemtag = saxxDefaultElem);
	
	virtual void saxx(SaxHandler& handler, const Xml_string& elemtag) const;
	virtual void saxx(SaxHandler& handler)                     const { saxx(handler, saxxDefaultElem); }
	
	/**
	 * @brief Clear the contents of a frame.
	 * WARNING: Currently *they* do not check if the user wants this.
	 * The view does when these are called.
	 */	
	virtual void clearContents() {};
	
	/**
	 * @brief Adjust the picture scale, moved from the view, no view code here
	 * FIXME: Move to PageItem_TextFrame
	 */
	void AdjustPictScale();
	
	/**
	 * @brief Set or get the redraw bounding box of the item, moved from the View
	 */
	QRect getRedrawBounding(const double);
	void setRedrawBounding();
			
	/**
	 * @brief Update the gradient vectors, moved from the View
	 */		
	void updateGradientVectors();
	/**
	 * @brief Move the image within the frame
	 * Old ScribusView::MoveItemI
	 * @todo Move to PageItem_ImageFrame
	 */
	void moveImageInFrame(double newX, double newY);

	ObjAttrVector* getObjectAttributes();
	/*!
	 * brief Returns a complete ObjectAttribute struct if 1 is found, or ObjectAttribute.name will be QString::null if 0 or >1 are found
	 */
	ObjectAttribute getObjectAttribute(QString) const;
	void setObjectAttributes(ObjAttrVector*);
	
	virtual bool createInfoGroup(QFrame *, QGridLayout *) {return false;}
//	virtual bool createContextMenu(QMenu *, int step) {return false;}
	
  /** Zeichnet das Item */
	void paintObj(QPainter *p);
	void DrawObj(ScPainter *p, QRectF e);
	void DrawObj_Pre(ScPainter *p, double &sc);
	virtual void DrawObj_Post(ScPainter *p);
	virtual void DrawObj_Item(ScPainter *p, QRectF e, double sc) = 0;
	QImage DrawObj_toImage();
	QImage DrawObj_toImage(QList<PageItem*> &emG);
	
	virtual void applicableActions(QStringList& actionList) = 0;
	virtual QString infoDescription();
			
protected:
	void DrawObj_ImageFrame(ScPainter *p, double sc);
	//void DrawObj_TextFrame(ScPainter *p, QRectF e, double sc);
	//void DrawObj_Line(ScPainter *p);
	void DrawObj_Polygon(ScPainter *p);
	void DrawObj_PolyLine(ScPainter *p);
	void DrawObj_PathText(ScPainter *p, double sc);
public:
	void DrawObj_Embedded(ScPainter *p, QRectF e, const CharStyle& style, PageItem* cembedded);
	void SetFrameShape(int count, double *vals);
	void SetRectFrame();
	void SetOvalFrame();
	void SetFrameRound();
	void setPolyClip(int up, int down = 0);
	void updatePolyClip();
	void updateClip();
	void convertClip();
	//QRect getRedrawBounding(const double);
	//void setRedrawBounding();
	void getBoundingRect(double *x1, double *y1, double *x2, double *y2) const;
	void getVisualBoundingRect(double *x1, double *y1, double *x2, double *y2) const;
	QRectF getBoundingRect() const;
	QRectF getVisualBoundingRect() const;
	void getTransform(QMatrix& mat) const;
	QMatrix getTransform() const;
	/**
	 * @brief Check if a QPoint is within the items boundaries
	 * No coordinates transformation is performed
	 * @param x X position
		@param y Y position
	 * @return bool true if x, i in the item
	 */
	bool pointWithinItem(const int x, const int y) const;
	/**
	 * @brief Check if the mouse is within the items boundaries
	 * This method performs necessary page to device transformations
		@param x X position
		@param y Y position
		@param scale scale of the vport
	 * @return bool true if the x, y is in the bounds 
	 */
	bool mouseWithinItem(const int x, const int y, double scale) const;
	void copyToCopyPasteBuffer(struct CopyPasteBuffer *Buffer);
	
	virtual void handleModeEditKey(QKeyEvent *k, bool &keyRepeat);
	
	/// invalidates current layout information
	virtual void invalidateLayout() { invalid = true; }
	/// creates valid layout information
	virtual void layout() {}
	/// returns true if text overflows
	bool frameOverflows() const;
	/// returns index of first char displayed in this frame, used to be 0
	int firstInFrame() const;
	/// returns index of last char displayed in this frame, used to be MaxChars-1
	int lastInFrame() const;
	/// tests if a character is displayed by this frame
	bool frameDisplays(int textpos) const;
	/// returns the style at the current charpos
	const ParagraphStyle& currentStyle() const;
	/// returns the style at the current charpos
	ParagraphStyle& changeCurrentStyle();
	/// returns the style at the current charpos
	const CharStyle& currentCharStyle() const;
	// deprecated:
	double layoutGlyphs(const CharStyle& style, const QString& chars, GlyphLayout& layout);
	void SetQColor(QColor *tmp, QString farbe, double shad);
	void drawGlyphs(ScPainter *p, const CharStyle& style, GlyphLayout& glyphs );
	void DrawPolyL(QPainter *p, QPolygon pts);
	QString ExpandToken(uint base);
	
	bool AutoName;	
	double gXpos;
	double gYpos;
	double gWidth;
	double gHeight;
	int GrType;
	double GrStartX;
	double GrStartY;
	double GrEndX;
	double GrEndY;
	int Cols;
	double ColGap;
	double gridOffset_;
	double gridValue_;

	/** Linestyle */
	Qt::PenStyle PLineArt;
	Qt::PenCapStyle PLineEnd;
	Qt::PenJoinStyle PLineJoin;
	QString NamedLStyle;
  /** Defines clipping region of the elements; */
	QPolygon Clip;
	
	FPointArray PoLine;
	const FPointArray shape() const { return PoLine; }
	void setShape(FPointArray val) { PoLine = val; }
	
	FPointArray ContourLine;
	const FPointArray contour() const { return ContourLine; }
	void setContour(FPointArray val) { ContourLine = val; }
	
	FPointArray imageClip;
	QList<uint> Segments;
	ScImageEffectList effectsInUse;
	bool PoShow;
	double BaseOffs;
	int textPathType;
	bool textPathFlipped;
	bool flipPathText() const { return textPathFlipped; }
	void setFlipPathText(bool val) { textPathFlipped = val; }
	int pathTextType() const { return textPathType; }
	void setPathTextType(int val) { textPathType = val; }
	double pathTextBaseOffset() const { return BaseOffs; }
	void setPathTextBaseOffset(double val) { BaseOffs = val; }
	bool pathTextShowFrame() const { return PoShow; }
	void setPathTextShowFrame(bool val) { PoShow = val; }
	
	bool useEmbeddedImageProfile() const { return UseEmbedded; }
	void setUseEmbeddedImageProfile(bool val) { UseEmbedded = val; }
	QString embeddedImageProfile() const { return EmProfile; }
	void setEmbeddedImageProfile(QString val) { EmProfile = val; }

	
	bool ClipEdited;
	// Don't know exactly what this is, but it's not the same as itemType
	int FrameType;
  /** Interne Item-Nummer */
	uint ItemNr;
  /** Internal unique Item-Number, used for the undo system */
	uint uniqueNr;
  /** Hat Element Rahmen? FIXME: still used? - in DrawObject_Post */
	bool Frame;
  /** page this element belongs to */
	int OwnPage;
	/** @brief Old page number tracked for the move undo action */
	int oldOwnPage;
	int savedOwnPage;
  /** Darzustellendes Bild */
	ScImage pixm;
  /** Dateiname des Bildes */
	QString Pfile;
	QString Pfile2;
	QString Pfile3;
	QString externalFile() const { return Pfile; }
	void setExternalFile(QString val);
	
	//FIXME: maybe these should go into annotation?
	QString fileIconPressed() const { return Pfile2; }
	void setFileIconPressed(QString val);
	QString fileIconRollover() const { return Pfile3; }
	void setFileIconRollover(QString val);
	
	QString IProfile;
	bool UseEmbedded;
	QString EmProfile;
	int IRender;
	// some accessor methods:
	int cmsRenderingIntent() const { return IRender; }
	void setCmsRenderingIntent(int val) { IRender = val; }
	QString cmsProfile() const { return IProfile; }
	void setCmsProfile(QString val) { IProfile = val; }
	
	/*! Flag to hiold image file availability */
	bool PictureIsAvailable;
	int OrigW;
	int OrigH;
  /** BoundigBox-X */
	double BBoxX;
  /** BoundingBox-H */
	double BBoxH;
  /** Zeichen X-Position */
	double CurX;
  /** Zeichen Y-Position */
	double CurY;
  /** Cursorposition */
	int CPos;
  /** Text des Elements */
	StoryText itemText;
  /** Flag fuer PDF-Bookmark */
	bool isBookmark;
  /** Flag for redraw in EditMode */
	bool Dirty;
	/** Flag indicates that layout has changed (eg. for textlayout) */
	bool invalid;
  /** Flag fuer Auswahl */
	bool HasSel;
	/** avoid artefacts while moving */
	bool FrameOnly;
	bool isAutoText;
	PageItem* prevInChain() { return BackBox; }
	PageItem* nextInChain() { return NextBox; }
	const PageItem* prevInChain() const { return BackBox; }
	const PageItem* nextInChain() const { return NextBox; }
	void unlink();
	void link(PageItem* nextFrame);
	void dropLinks();

protected:
	PageItem *BackBox;
	PageItem *NextBox;
	uint firstChar;
	uint MaxChars;
public:
	bool inPdfArticle;
	bool isRaster;
	double OldB;
	double OldH;
	double OldB2;
	double OldH2;
	bool Sizing;
	bool toPixmap;
	int LayerNr;
	bool ScaleType;
	bool AspectRatio;
	QStack<int> Groups;
	const QStack<int>& groups() const { return Groups; }
	QStack<int>& groups() { return Groups; }
	void setGroups( QStack<int> val) { Groups = val; }
	
	bool controlsGroup() const { return isGroupControl; }
	void setControlsGroup(bool val) { isGroupControl = val; }
	
	QVector<double> DashValues;
	double DashOffset;
	const QVector<double>& dashes() const { return DashValues; }
	QVector<double>& dashes() { return DashValues; }
	void setDashes(QVector<double> val) { DashValues = val; }
	double dashOffset() const { return DashOffset; }
	void setDashOffset(double val) { DashOffset = val; }
	VGradient fill_gradient;
	bool fillRule;
	bool doOverprint;
/* Additions for Table Support */
	PageItem* LeftLink;
	PageItem* RightLink;
	PageItem* TopLink;
	PageItem* BottomLink;
	int LeftLinkID;
	int RightLinkID;
	int TopLinkID;
	int BottomLinkID;
	bool LeftLine;
	bool RightLine;
	bool TopLine;
	bool BottomLine;
	bool isTableItem;
	void setIsTableItem(bool val) { isTableItem = val; }
	void setHasLeftLine(bool val) { LeftLine = val; }
	void setHasRightLine(bool val) { RightLine = val; }
	void setHasTopLine(bool val) { TopLine = val; }
	void setHasBottomLine(bool val) { BottomLine = val; }
	void setLeftLink(PageItem* lnk) { LeftLink = lnk; }
	void setRightLink(PageItem* lnk) { RightLink = lnk; }
	void setTopLink(PageItem* lnk) { TopLink = lnk; }
	void setBottomLink(PageItem* lnk) { BottomLink = lnk; }
	bool isSingleSel;
	bool isGroupControl;
	PageItem *groupsLastItem;
	void setGroupsLastItem(PageItem* lnk) { groupsLastItem = lnk; }
	double BoundingX;
	double BoundingY;
	double BoundingW;
	double BoundingH;
	bool ChangedMasterItem;
	QString OnMasterPage;
	bool isEmbedded;
	
	//Position
	double xPos() const { return Xpos; }
	double yPos() const { return Ypos; }
	double visualXPos() const;
	double visualYPos() const;
	FPoint xyPos() const { return FPoint(Xpos, Ypos); }
	void setXPos(const double, bool drawingOnly=false);
	void setYPos(const double, bool drawingOnly=false);
	void setXYPos(const double, const double, bool drawingOnly=false);
	void moveBy(const double, const double, bool drawingOnly=false);
	//Size
	double width() const { return Width; }
	double height() const { return Height; }
	double visualWidth() const;
	double visualHeight() const;
	void setWidth(const double);
	void setHeight(const double);
	void setWidthHeight(const double, const double, bool drawingOnly);
	void setWidthHeight(const double, const double);
	void resizeBy(const double, const double);
	//Rotation
	double rotation() const { return Rot; }
	void setRotation(const double, bool drawingOnly);
	void setRotation(const double r) { setRotation(r, false); }  // needed for deSaXe
	void rotateBy(const double);
	//Selection
	bool isSelected() const { return Select; }
	void setSelected(const bool);
	//Image Data
	double imageXScale() const { return LocalScX; }
	double imageYScale() const { return LocalScY; }
	void setImageXScale(const double);
	void setImageYScale(const double);
	void setImageXYScale(const double, const double);
	double imageXOffset() const { return LocalX; }
	double imageYOffset() const { return LocalY; }
	void setImageXOffset(const double);
	void setImageYOffset(const double);
	void moveImageXYOffsetBy(const double, const double);
	void setImageXYOffset(const double, const double);
	//Reverse
	bool reversed() const { return Reverse; }
	void setReversed(bool);
	//Rounded Corners
	double cornerRadius() const { return RadRect; }
	void setCornerRadius(double);
	// PDF bookmark
	bool isPDFBookmark() const { return isBookmark; }
	void setIsPDFBookmark(bool val) { isBookmark = val; }
	// 0=none, 1,2,3,4=linear, 5=radial, 6=free linear, 7=free radial, 8=pattern 
	int gradientType() const { return GrType; }
	void setGradientType(int val) { GrType = val; }
	void gradientVector(double& startX, double& startY, double& endX, double& endY) const;
	void setGradientVector(double startX, double startY, double endX, double endY);
	// 
	bool fillEvenOdd() const { return fillRule; }
	void setFillEvenOdd(bool val) { fillRule = val; }
	//
	bool overprint() const { return doOverprint; }
	void setOverprint(bool val) { doOverprint = val; }
	// rect / oval / round / other
	int frameType() const { return FrameType; }
	void setFrameType(int val) { FrameType = val; }
	//
	bool hasDefaultShape() const { return !ClipEdited; }
	void setHasDefaultShape(bool val) { ClipEdited = !val; }
	//
	bool isAutoFrame() const { return isAutoText; }
	void setIsAutoFrame(bool val) { isAutoText = val; }
	//
	bool keepAspectRatio() const { return AspectRatio; }
	void setKeepAspectRatio(bool val) { AspectRatio = val; }
	//
	bool fitImageToFrame() const { return !ScaleType; }
	void setFitImageToFrame(bool val) { ScaleType = !val; }
	
	//Text Data - Move to PageItem_TextFrame at some point? --- no, to FrameStyle, av
	double textToFrameDistLeft() const { return Extra; }
	double textToFrameDistRight() const { return RExtra; }
	double textToFrameDistTop() const { return TExtra; }
	double textToFrameDistBottom() const { return BExtra; }
	int columns() const { return Cols; }
	double columnGap() const { return ColGap; }
	double gridOffset() const;
	double gridDistance() const;
	void setTextToFrameDistLeft(double);
	void setTextToFrameDistRight(double);
	void setTextToFrameDistTop(double);
	void setTextToFrameDistBottom(double);
	void setColumns(int);
	void setColumnGap(double);
	void setGridOffset(double);
	void setGridDistance(double);
	FirstLineOffsetPolicy firstLineOffset()const;
	void setFirstLineOffset(FirstLineOffsetPolicy);
	/**
	 * \brief Set the text to frame distances all at once
	 * @param newLeft left distance
	 * @param newRight right distance
	 * @param newTop top distance
	 * @param newBottom bottom distance
	 */
	void setTextToFrameDist(double newLeft, double newRight, double newTop, double newBottom);

	/**  @brief Get name of the item
	 *
	 * This is unrelated to QObject::name(); the pageItem's name is independent
	 * of its Qt name.
	 * See also PageItem::setItemName()
	 */
	QString itemName() const { return AnName; }
	/**
	 * @brief Set name of the item
	 * @param newName name for the item
	 * @author Riku Leino
	 *
	 * Note that this is unrelated to QObject::setName()
	 * See also PageItem::itemName()
	 */
	void setItemName(const QString& newName);

	/** @brief Get the name of the pattern of the object */
	QString pattern() const { return patternVal; }

	/** @brief Get the pattern transformation matrix of the object */
	void patternTransform(double &scaleX, double &scaleY, double &offsetX, double &offsetY, double &rotation) const;

	/**
	 * @brief Set the fill pattern of the object.
	 * @param newPattern fill pattern for the object
	 */
	void setPattern(const QString &newPattern);
	
	/**
	 * @brief Set the fill pattern transformation of the object.
	 */
	void setPatternTransform(double scaleX, double scaleY, double offsetX, double offsetY, double rotation);

	/** @brief Get the (name of the) fill color of the object */
	QString fillColor() const { return fillColorVal; }
	/**
	 * @brief Set the fill color of the object.
	 * @param newColor fill color for the object
	 */
	void setFillColor(const QString &newColor);

	/** @brief Get the shade of the fill color */
	double fillShade() const { return fillShadeVal; }
	/**
	 * @brief Set the fill color shade.
	 * @param newShade shade for the fill color
	 */
	void setFillShade(double newShade);

	/** @brief Get the transparency of the fill color */
	double fillTransparency() const { return fillTransparencyVal; }
	/**
	 * @brief Set the transparency of the fill color.
	 * @param newTransparency transparency of the fill color
	 */
	void setFillTransparency(double newTransparency);

	/** @brief Get the blendmode of the fill color */
	int fillBlendmode() const { return fillBlendmodeVal; }
	/**
	 * @brief Set the blendmode of the fill color.
	 * @param newBlendmode blendmode of the fill color
	 */
	void setFillBlendmode(int newBlendmode);

	/** @brief Get the blendmode of the stroke color */
	int lineBlendmode() const { return lineBlendmodeVal; }
	/**
	 * @brief Set the blendmode of the stroke color.
	 * @param newBlendmode blendmode of the stroke color
	 */
	void setLineBlendmode(int newBlendmode);

	/** @brief Get the line color of the object */
	QString lineColor() const { return lineColorVal; }
	/**
	 * @brief Set the line color of the object.
	 * @param newColor line color for the object
	 */
	void setLineColor(const QString &newColor);

	/** @brief Get the line color shade */
	double lineShade() const { return lineShadeVal; }
	/**
	 * @brief Set the line color shade.
	 * @param newShade shade for the line color
	 */
	void setLineShade(double newShade);

	/** @brief Get the line transparency */
	double lineTransparency() const { return lineTransparencyVal; }
	/**
	 * @brief Set the transparency of the line color.
	 * @param newTransparency transparency of the line color
	 */
	void setLineTransparency(double newTransparency);

	/** @brief Set the QColor for the line */
	void setLineQColor();
	/** @brief Set the QColor for the fill */
	void setFillQColor();

	/** @brief Get the style of line */
	Qt::PenStyle lineStyle() const { return PLineArt; }
	/**
	 * @brief Set the style of line.
	 * @param newStyle style of line
	 * @sa Qt::PenStyle
	 */
	void setLineStyle(Qt::PenStyle newStyle);

	/** @brief Get the width of the line */
	double lineWidth() const { return m_lineWidth; }
	/**
	 * @brief Set the width of line
	 * @param newWidth width of line
	 */
	void setLineWidth(double newWidth);

	/** @brief Get the end cap style of the line */
	Qt::PenCapStyle lineEnd() const { return PLineEnd; }
	/**
	 * @brief Set the end style of line
	 * @param newStyle end style of line
	 * @sa Qt::PenCapStyle
	 */
	void setLineEnd(Qt::PenCapStyle newStyle);

	/** @brief Get the join style of multi-segment lines */
	Qt::PenJoinStyle lineJoin() const { return PLineJoin; }
	/**
	 * @brief Set the join style of line
	 * @param newStyle join style of line
	 * @sa Qt::PenJoinStyle
	 */
	void setLineJoin(Qt::PenJoinStyle newStyle);

	/** @brief Get name of active custom line style */
	QString customLineStyle() const { return NamedLStyle; }
	/**
	 * @brief Set custom line style
	 * @param newStyle name of the custom style
	 */
	void setCustomLineStyle(const QString& newStyle);

	/** @brief Get start arrow index
	 * @sa PageItem::endArrowIndex(), PageItem::setStartArrowIndex()
	 */
	int startArrowIndex() const { return m_startArrowIndex; }
	/**
	 * @brief Set start arrow index
	 * @param newIndex index for start arrow
	 */
	void setStartArrowIndex(int newIndex);

	/** @brief Get end arrow index
	 * @sa PageItem::startArrowIndex(), PageItem::setEndArrowIndex()
	 */
	int endArrowIndex() const { return m_endArrowIndex; }
	/**
	 * @brief Set end arrow index
	 * @param newIndex index for end arrow
	 */
	void setEndArrowIndex(int newIndex);

	/** @brief Is the image flipped horizontally? */
	bool imageFlippedH() const { return m_ImageIsFlippedH; }
	/** @brief Horizontally flip / unflip the image */
	void setImageFlippedH(bool flipped);
	/** @brief Flip an image horizontally. */
	void flipImageH();

	/** @brief Is the image flipped vertically? */
	bool imageFlippedV() const { return m_ImageIsFlippedV; }
	/** @brief Vertically flip / unflip the image */
	void setImageFlippedV(bool flipped);
	/** @brief Flip an image vertically */
	void flipImageV();

	/**
	 * @brief Set the image scaling mode.
	 * @param freeScale is the scaling free (not forced to frame size)
	 * @param keepRatio should the image's aspect ratio be respected
	 */
	void setImageScalingMode(bool freeScale, bool keepRatio);

	/** @brief Lock or unlock this pageitem. */
	void toggleLock();
	/** @brief is the item locked ? */
	bool locked() const { return m_Locked; }
	/** @brief Lock or unlock this pageitem */
	void setLocked(bool isLocked);

	/** @brief Toggle lock for resizing */
	void toggleSizeLock();
	/** @brief Is the item's size locked? */
	bool sizeLocked() const { return m_SizeLocked; }
	/** @brief set lock for resizing */
	void setSizeLocked(bool isLocked);

	/**
	 * @brief Does text flow around this object and how
	 * @sa setTextFlowMode()
	 */
	TextFlowMode textFlowMode() const { return textFlowModeVal; }

	/**
	 * @brief Changes the way text flows around this item
	 * @param mode true if text is wanted to flow around this object or false if not
	 * @sa textFlowMode()
	 */
	void setTextFlowMode(TextFlowMode mode);

	/**
	 * @brief If text should flow around object frame
	 * @sa PageItem::setTextFlowMode()
	 */
	bool textFlowAroundObject() const { return (textFlowModeVal != TextFlowDisabled); }

	/**
	 * @brief If text should flow around object frame
	 * @sa PageItem::setTextFlowMode()
	 */
	bool textFlowUsesFrameShape() const { return (textFlowModeVal == TextFlowUsesFrameShape); }

	/**
	 * @brief If text should flow around bounding box
	 * @sa PageItem::setTextFlowMode()
	 */
	bool textFlowUsesBoundingBox() const { return (textFlowModeVal == TextFlowUsesBoundingBox); }

	/**
	 * @brief If text should flow around contour line
	 * @sa PageItem::setTextFlowMode()
	 */
	bool textFlowUsesContourLine() const { return (textFlowModeVal == TextFlowUsesContourLine); }

	/**
	 * @brief If text should flow around image clipping path
	 * @sa PageItem::setTextFlowMode()
	 */
	bool textFlowUsesImageClipping() const { return (textFlowModeVal == TextFlowUsesImageClipping); }
	
	/**
	 * @brief To be called carefully because it eventually triggers a relayout of long text frames strings, but necesarily when you change the document.
	 * @param allItems While you generally want to check for items below, it can happen that you want to update full range of text frames (e.g. when shuffle items order). Default to false.
	 */
	void checkTextFlowInteractions(bool allItems = false);
	
	/** @brief Get the frame type
	 *
	 * @attention The whole concept of frame types is due for some radical
	 *            re-working, so don't rely on this interface staying stable.
	 *            It's here as an interim step to eliminate direct member access
	 *            on PageItems.
	 */
	ItemType itemType() const { return m_ItemType; }
	/** @brief Get the subclass item type
	 *
	 * This function should be used everywhere, where a itemType is required, but
	 * no C++ type informations is available. e.g. when saving files, etc.
	 * It returns the same type as itemType() for the standard classes, but 
	 * subclasses override it.
	 */
	virtual ItemType realItemType() const { return m_ItemType; }
	/**
	 * @brief Convert this PageItem to PageItem type <code>newType</code>
	 * @param newType PageItem type for conversion
	 */
	void convertTo(ItemType newType);

	/**
	* Set the layer for the item
	* @param layerId layer where this item is moved
	*/
	void setLayer(int layerId);

	/**
	 * @brief Check the changes to the item and add undo actions for them.
	 * @param force Force the check. Do not care if mouse button or arrow key is down
	 * check anyway.
	 * @author Riku Leino
	 */
	void checkChanges(bool force = false);
	/**
	 * @name Store undo actions
	 * @brief Add an undo action to the undo guis
	 * @author Riku Leino
	 */
	/*@{*/
	void moveUndoAction();
	void resizeUndoAction();
	void rotateUndoAction();
	void changeImageOffsetUndoAction();
	void changeImageScaleUndoAction();
	/*@}*/
	/** @brief Required by the UndoObject */
	void restore(UndoState *state, bool isUndo);

	void getNamedResources(ResourceCollection& lists) const;
	void replaceNamedResources(ResourceCollection& newNames);

	/**
	 * @brief Return a variant of `originalName' that is guaranteed to be unique
	 *        in the same document as this PageItem.  If the passed name is not
	 *        in use it is returned unchanged.
	 * @author Craig Ringer
	 *
	 * Usually of the form 'Copy of [name]' or 'Copy of [name] (n)'
	 */
	QString generateUniqueCopyName(const QString originalName) const;
	/**
	 * @brief Is this item printed?
	 * @sa setPrintEnabled()
	 */
	bool printEnabled() const { return m_PrintEnabled; }
	/**
	 * @brief Tells if the frame is set to be printed or not
	 * @sa printable()
	 */
	void setPrintEnabled(bool toPrint);
	
	/** @brief Toggle printable
	 * @sa setPrintable()
	 */
	void togglePrintEnabled();
	
	/**
	 * @brief Tells if the frame is tagged or not
	 * @sa isTagged()
	 */
	bool isTagged() const { return tagged; }
	/**
	 * @brief Set the tagged member for use when deleting items, instead of reselecting them.
	 * @sa setTagged()
	 */
	void setTagged(bool);

	/**
	 * @brief Load an image into an image frame, moved from ScribusView
	 * @return True if load succeeded
	 */
	bool loadImage(const QString& filename, const bool reload, const int gsResolution=-1, bool showMsg = false);
	
	
	/**
	 * @brief Connect the item's signals to the GUI, primarily the Properties palette, also some to ScMW
	 * @return 
	 */
	bool connectToGUI();
	bool disconnectFromGUI();
	/**
	 * @brief Emit the items properties to the GUI in one go
	 */
	void emitAllToGUI();
	
	/**
	 * @brief Get the document that this item belongs to
	 */
	ScribusDoc* doc() const { return m_Doc; }
	
	bool isAnnotation() const { return m_isAnnotation; }
	void setIsAnnotation(bool);
	void setAnnotation(const Annotation& ad);
	Annotation& annotation() { return m_annotation; }
	const Annotation& annotation() const { return m_annotation; }
	
	bool imageShown() const { return PicArt; }
	void setImageShown(bool);
	
	void updateConstants();
	
protected:

	void drawLockedMarker(ScPainter *p);
	void drawArrow(ScPainter *p, QMatrix &arrowTrans, int arrowIndex);
	
	/** @brief Manages undostack and is where all undo actions/states are sent. */
	UndoManager * const undoManager;
	/**
	 * @name Restore helper methods
	 * Split the restore method for easier handling.
	 * @author Riku Leino
	 */
	/*@{*/
	void restoreMove(SimpleState *state, bool isUndo);
	void restoreResize(SimpleState *state, bool isUndo);
	void restoreRotate(SimpleState *state, bool isUndo);
	void restoreFill(SimpleState *state, bool isUndo);
	void restoreShade(SimpleState *state, bool isUndo);
	void restoreLineColor(SimpleState *state, bool isUndo);
	void restoreLineShade(SimpleState *state, bool isUndo);
	void restoreName(SimpleState *state, bool isUndo);
	void restoreFillTP(SimpleState *state, bool isUndo);
	void restoreLineTP(SimpleState *state, bool isUndo);
	void restoreLineStyle(SimpleState *state, bool isUndo);
	void restoreLineEnd(SimpleState *state, bool isUndo);
	void restoreLineJoin(SimpleState *state, bool isUndo);
	void restoreLineWidth(SimpleState *state, bool isUndo);
	void restoreCustomLineStyle(SimpleState *state, bool isUndo);
	void restoreArrow(SimpleState *state, bool isUndo, bool isStart);

	void restorePStyle(SimpleState *state, bool isUndo);

	void restoreType(SimpleState *state, bool isUndo);
	void restoreTextFlowing(SimpleState *state, bool isUndo);
	void restoreImageScaleType(SimpleState *state, bool isUndo);
	void restoreImageScaleChange(SimpleState *state, bool isUndo);
	void restoreImageOffsetChange(SimpleState *state, bool isUndo);
	void restorePoly(SimpleState *state, bool isUndo, bool isContour);
	void restoreContourLine(SimpleState *state, bool isUndo);
	void restoreShapeType(SimpleState *state, bool isUndo);
	void restoreLayer(SimpleState *state, bool isUndo);
	void restoreGetImage(SimpleState *state, bool isUndo);

	void restoreShapeContour(UndoState *state, bool isUndo);
	void restoreImageEffects(UndoState *state, bool isUndo);
	/*@}*/

	/**
	 * @brief Returns true if the actions should be sent to the UndoManager.
	 *
	 * Checks the state of the arrow keys and mouse buttons. If a key or a
	 * mouse button is down PageItem is under some action which should only be
	 * stored after the action has been finished (to get a single undo action).
	 * @return true if the actions should be sent to the UndoManager based on the
	 * state of arrow keys and mouse buttons else returns false.
	 */
	bool shouldCheck();
	/** @brief Clears the current selection and selects this PageItem. */
	void select();

	// Protected members

	/**
	 * @brief Frame Type, eg line, text frame, etc.
	 *
	 * This will probably go away when pageitem is split into
	 * subclasses.
	 */
	ItemType m_ItemType;

	/**
	 * @brief Item name. Unicode. User visible (outline, property palette, etc).
	 * @todo This is Annotation Name.. not item name, needs splitting up.
	 * @sa PageItem::itemName(), PageItem::setItemName()
	 */
	QString AnName; 

	/**
	 * @brief Fill pattern name
	 * @sa PageItem::pattern(), PageItem::setPattern()
	 */
	QString patternVal;
	/**
	 * @brief Fill pattern transformation matrix
	 */
	double patternScaleX;
	double patternScaleY;
	double patternOffsetX;
	double patternOffsetY;
	double patternRotation;

	/**
	 * @brief Fill color name
	 * @sa PageItem::fillColor(), PageItem::setFillColor()
	 */
	QString fillColorVal;

	/**
	 * @brief Line color name
	 * @sa PageItem::lineColor(), PageItem::setLineColor()
	 */
	QString lineColorVal;

	/**
	 * @brief Line shade
	 * @sa PageItem::lineShade, PageItem::setLineShade()
	 */
	double lineShadeVal;

	/**
	 * @brief Fill shade
	 * @sa PageItem::fillShade, PageItem::setFillShade()
	 */
	double fillShadeVal;

	/**
	 * @brief Fill transparency
	 * @sa PageItem::fillTransparency(), PageItem::setFillTransparency()
	 */
	double fillTransparencyVal;

	/**
	 * @brief Line stroke transparency.
	 * @sa PageItem::lineTransparency(), PageItem::setLineTransparency()
	 */
	double lineTransparencyVal;

	/**
	 * @brief Fill transparency blendmode
	 * @sa PageItem::fillBlendmode(), PageItem::setFillBlendmode()
	 */
	int fillBlendmodeVal;

	/**
	 * @brief Line stroke transparency blendmode.
	 * @sa PageItem::lineBlendmode(), PageItem::setLineBlendmode()
	 */
	int lineBlendmodeVal;

	/**
	 * @brief Is the image in this image item flipped horizontally?
	 * @sa PageItem::isImageFlippedH(), PageItem::setImageFlippedH(),
	 *     PageItem::flipImageH(), PageItem::flippedV
	 */
	bool m_ImageIsFlippedH;

	/**
	 * @brief Is the image in this image item flipped vertically?
	 * @sa PageItem::isImageFlippedV(), PageItem::setImageFlippedV(),
	 *     PageItem::flipImageV(), PageItem::flippedH
	 */
	bool m_ImageIsFlippedV;

	/**
	 * @brief Is the item locked (cannot be moved, resized, etc)?
	 * @sa PageItem::locked(), PageItem::setLocked(), PageItem::toggleLock()
	 */
	bool m_Locked;

	/**
	 * @brief Is the item's size locked?
	 * @sa PageItem::sizeLocked(), PageItem::setSizeLocked(), PageItem::toggleSizeLock()
	 */
	bool m_SizeLocked;

	/**
	 * @brief Should text flow around the item
	 * @sa PageItem::textFlowMode(), PateItem::setTextFlowMode()
	 */
	TextFlowMode textFlowModeVal;

	/**
	 * @brief Stores the attributes of the pageitem (NOT properties, the user defined ATTRIBUTES)
	 * @sa
	 */
	ObjAttrVector pageItemAttributes;

	/**
	 * @brief Is this item set to be printed/exported
	 * @sa PageItem::printable(), PageItem::setPrintable()
	 */
	bool m_PrintEnabled;
	
	/**
	 * @brief Is this item set to have an action done to it, eg deleted
	 * @sa PageItem::isTagged(), PageItem::setTagged()
	 */
	bool tagged;
	
	QColor fillQColor;
	QColor strokeQColor;
	
	/** X position on the page */
	double Xpos;
	/** Y position on the page */
	double Ypos;
	/** Width of the item */
	double Width;
	/** Height of the item */
	double Height;
	/** Rotation of the item */
	double Rot;
	/** Element selected? */
	bool Select;
	/** Scaling X Factor for images */
	double LocalScX;
	/** Scaling Y Factor for images*/
	double LocalScY;
	/** Image X Offset to frame */
	double LocalX;
	/** Image Y Offset to frame */
	double LocalY;
	/** If the frame is reversed */
	bool Reverse;

	int m_startArrowIndex;
	int m_endArrowIndex;

protected:
  	/** Left, Top, Bottom, Right distances of text from the frame */
	double Extra;
	double TExtra;
	double BExtra;
	double RExtra;
	
	FirstLineOffsetPolicy firstLineOffsetP;
	
public:
	/** Radius of rounded corners */
	double RadRect;
	
	//Undo Data
	/** @brief Stores the old X-position for undo action. Is used to detect move actions.*/
	double oldXpos;
	/** @brief Stores the old Y-position for undo action. Is used to detect move actions. */
	double oldYpos;
	/** @brief Stores the old width for undo action. Is used to detect resize actions. */
	double oldWidth;
	/** @brief Stores the old height for undo action. Is used to detect resize actions. */
	double oldHeight;
	/** @brief Stores the old rotation value for undo action. Is used to detect rotation actions. */
	double oldRot;
	/** @brief Stores the old LocalScX value for undo action. Is used to detect image scale actions. */
	double oldLocalScX;
	/** @brief Stores the old LocalScY value for undo action. Is used to detect image scale actions. */
	double oldLocalScY;
	/** @brief Stores the old LocalX value for undo action. Is used to detect image offset actions. */
	double oldLocalX;
	/** @brief Stores the old LocalY value for undo action. Is used to detect image offset actions. */
	double oldLocalY;
	
	/** Document this item belongs to */
	ScribusDoc *m_Doc;
	
	/** Flags and data for PDF Annotations */
	bool m_isAnnotation;
	Annotation m_annotation;
	
	/** Darstellungsart Bild/Titel */
	bool PicArt;
	
	 /** Line width */
	double m_lineWidth;
	double Oldm_lineWidth;

	/** Inline Image */
	bool isInlineImage;
	QTemporaryFile *tempImageFile;
	
signals:
	//Frame signals
	void myself(PageItem *);
	void frameType(int);   // not related to Frametype but to m_itemIype :-/
	void position(double, double); //X,Y
	void widthAndHeight(double, double); //W,H
	void rotation(double); //Degrees rotation	
	void colors(QString, QString, double, double); //lineColor, fillColor, lineShade, fillShade
	void gradientType(int); //Normal, horizontal, vertical, etc.
	void patternFill(QString, double, double, double, double, double);
	void gradientColorUpdate(double, double, double, double, double, double); //Cpal updatespecialgradient
	void transparency(double, double); //fillTransparency, lineTransparency
	void blendmode(int, int); //fillBlendmode, lineBlendmode
	void frameLocked(bool); //Frame lock
	void frameSizeLocked(bool); //Frame size lock
	void frameFlippedH(bool); //Frame flipped horizontally
	void frameFlippedV(bool); //Frame flipped vertically
	void printEnabled(bool); //Frame is set to print or not
	//Shape signals
	void columns(int, double); //Number, gap
	void cornerRadius(double); //Corner radius of the shape
	//Line signals
	void lineWidth(double);
	void lineStyleCapJoin(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle);
	//Frame text signals
	void lineSpacing(double);
	void textKerning(double);
	void textStyle(int);
	void textFont(const QString&);
	void textSize(double);
	void textWidthScale(double);
	void textHeightScale(double);
	void textBaseLineOffset(double);
	void textOutline(double);
	void textShadow(double, double);
	void textUnderline(double, double);
	void textStrike(double, double);
	void textColor(QString, QString, double, double);
	void textFormatting(int);
	void textToFrameDistances(double, double, double, double); //left, top, bottom, right: Extra, TExtra, BExtra, RExtra
	//FIXME: columns, grid ?
	//Frame image signals
	void imageOffsetScale(double, double, double, double);
};

Q_DECLARE_METATYPE(PageItem*);

#endif
