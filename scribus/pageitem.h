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

#include <qobject.h>
#include <qwidget.h>
#include <qpointarray.h>
#include <qptrlist.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qvaluestack.h>
#include <qvaluelist.h>
#include "scpainter.h"
#include "undoobject.h"
class ScribusDoc;
class UndoManager;
class UndoState;
/**
  *@author Franz Schmid
  */

struct PSDLayer
{
	QValueList<uint> channelLen;
	QValueList<int> channelType;
	int xpos;
	int ypos;
	int width;
	int height;
	ushort opacity;
	uchar clipping;
	uchar flags;
	QString layerName;
	QString blend;
	QImage thumb;
};

struct ImageInfoRecord
{
	int xres;
	int yres;
	bool colorspace;
	bool valid;
	bool isRequest;
	QMap<QString, FPointArray> PDSpathData;
	QString clipPath;
	QString usedPath;
	QValueList<PSDLayer> layerInfo;
};

class PageItem : public QObject, public UndoObject
{
	Q_OBJECT

	// Properties - see http://doc.trolltech.com/3.3/properties.html
	// See the getters and setters of these properties for details on their use.
	Q_PROPERTY(QString itemName READ itemName WRITE setItemName DESIGNABLE false)
	Q_PROPERTY(QString fillColor READ fillColor WRITE setFillColor DESIGNABLE false)
	Q_PROPERTY(QString lineColor READ lineColor WRITE setLineColor DESIGNABLE false)
	Q_PROPERTY(int fillShade READ fillShade WRITE setFillShade DESIGNABLE false)
	Q_PROPERTY(int lineShade READ lineShade WRITE setLineShade DESIGNABLE false)
	Q_PROPERTY(double fillTransparency READ fillTransparency WRITE setFillTransparency DESIGNABLE false)
	Q_PROPERTY(double lineTransparency READ lineTransparency WRITE setLineTransparency DESIGNABLE false)
	Q_PROPERTY(bool locked READ locked WRITE setLocked DESIGNABLE false)
	Q_PROPERTY(bool sizeLocked READ sizeLocked WRITE setSizeLocked DESIGNABLE false)
	Q_PROPERTY(bool imageFlippedV READ imageFlippedV WRITE setImageFlippedV DESIGNABLE false)
	Q_PROPERTY(bool imageFlippedH READ imageFlippedH WRITE setImageFlippedH DESIGNABLE false)
	Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth DESIGNABLE false)
	Q_PROPERTY(QString customLineStyle READ customLineStyle WRITE setCustomLineStyle DESIGNABLE false)
	Q_PROPERTY(int startArrowIndex READ getStartArrowIndex WRITE setStartArrowIndex DESIGNABLE false)
	Q_PROPERTY(int endArrowIndex READ getEndArrowIndex WRITE setEndArrowIndex DESIGNABLE false)
	Q_PROPERTY(QString font READ font WRITE setFont DESIGNABLE false)
	Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize DESIGNABLE false)
	Q_PROPERTY(int fontWidth READ fontWidth WRITE setFontWidth DESIGNABLE false)
	Q_PROPERTY(QString fontFillColor READ fontFillColor WRITE setFontFillColor DESIGNABLE false)
	Q_PROPERTY(int fontFillShade READ fontFillShade WRITE setFontFillShade DESIGNABLE false)
	Q_PROPERTY(QString fontStrokeColor READ fontStrokeColor WRITE setFontStrokeColor DESIGNABLE false)
	Q_PROPERTY(int fontStrokeShade READ fontStrokeShade WRITE setFontStrokeShade DESIGNABLE false)
	Q_PROPERTY(int fontEffects READ fontEffects WRITE setFontEffects DESIGNABLE false)
	Q_PROPERTY(double kerning READ kerning WRITE setKerning  DESIGNABLE false)
	Q_PROPERTY(double lineSpacing READ lineSpacing WRITE setLineSpacing DESIGNABLE false)
	Q_PROPERTY(QString language READ language WRITE setLanguage DESIGNABLE false)

	// FIXME: QMetaProperty can't translate these to/from enumerator names, probably because the
	// properties aren't moc'd in the Qt sources. They work fine in their
	// current state as plain integer properties.
	Q_ENUMS(PenStyle)
	Q_PROPERTY(PenStyle lineStyle READ lineStyle WRITE setLineStyle DESIGNABLE false)
	Q_ENUMS(PenCapStyle)
	Q_PROPERTY(PenCapStyle lineEnd READ lineEnd WRITE setLineEnd DESIGNABLE false)
	Q_ENUMS(PenJoinStyle)
	Q_PROPERTY(PenJoinStyle lineJoin READ lineJoin WRITE setLineJoin DESIGNABLE false)

	// This property may not hang around for too long, but should be useful
	// when testing out the pageitem refactoring work.  Setting it is unlikely
	// to currently have the desired effect.
	/**
	 * @brief Frame type.
	 * @warning Do not set this property except for testing and debug purposes.
	 */
	Q_ENUMS(ItemType)
	Q_PROPERTY(ItemType itemType READ itemType WRITE convertTo DESIGNABLE false)

public:
	// Enumerator definitions

	/** @brief Frame Type
	 *
	 * Later, frame type will probably go away in favour of using
	 * subclasses and checking types using more conventional methods
	 * and using Qt's MetaObject introspection.
	 */
	enum ItemType {
		FrameType1  = 1,
		ImageFrame	= 2,
		FrameType3  = 3,
		TextFrame	= 4,
		Line		= 5,
		Polygon		= 6,
		PolyLine	= 7,
		PathText	= 8
	};


	PageItem(ScribusDoc *pa, ItemType newType, double x, double y, double w, double h, double w2, QString fill, QString outline);
	~PageItem() {};
	struct ZZ {
				double xco;
				double yco;
				double wide;
				double kern;
				int Siz;
				int Style;
				int scale;
				int shade;
				int shade2;
				bool Sele;
				QString Zeich;
				QString Farb;
				QString Farb2;
				QString ZFo;
			  };

  /** Zeichnet das Item */
	void paintObj(QRect e=QRect(), QPixmap *ppX = 0);
	void DrawObj(ScPainter *p, QRect e);
	double SetZeichAttr(struct ScText *hl, int *chs, QString *chx);
	void SetFarbe(QColor *tmp, QString farbe, int shad);
	void DrawZeichenS(ScPainter *p, struct ZZ *hl);
	void DrawPolyL(QPainter *p, QPointArray pts);
	QString ExpandToken(uint base);
	bool Reverse;
  /** X-Position auf der Seite */
	double Xpos;
	/** @brief Stores the old X-position for undo action. Is used to detect move actions.*/
	double oldXpos;
  /** Y-Position auf der Seite */
	double Ypos;
	/** @brief Stores the old Y-position for undo action. Is used to detect move actions. */
	double oldYpos;
  /** Breite des Elements */
	double Width;
	/** @brief Stores the old width for undo action. Is used to detect resize actions. */
	double oldWidth;
  /** Hoehe des Elements */
	double Height;
	/** @brief Stores the old height for undo action. Is used to detect resize actions. */
	double oldHeight;
  /** Eckrundung von Rechtecken */
	double RadRect;
  /** Winkel um den das Item gedreht wird */
	double Rot;
	/** @brief Stores the old rotation value for undo action. Is used to detect rotation actions. */
	double oldRot;
  /** Enthaelt das Dokument */
	ScribusDoc *Doc;
	int GrType;
	double GrStartX;
	double GrStartY;
	double GrEndX;
	double GrEndY;
	QString TxtStroke;
	QString TxtFill;
	int ShTxtStroke;
	int ShTxtFill;
	int TxtScale;
	int TxTStyle;
	int Cols;
	double ColGap;
  /** Strichstaerke */
	double Pwidth;
	double OldPwidth;
  /** Linienart */
	PenStyle PLineArt;
	PenCapStyle PLineEnd;
	PenJoinStyle PLineJoin;
	QString NamedLStyle;
  /** Element selektiert Ja/Nein */
	bool Select;
  /** Definiert die Clipping-Region des Elements; */
	QPointArray Clip;
	FPointArray PoLine;
	bool UseContour;
	FPointArray ContourLine;
	QValueList<uint> Segments;
	bool PoShow;
	double BaseOffs;
	bool ClipEdited;
	// Don't know exactly what this is, but it's not the same as itemType
	int FrameType;
  /** Interne Item-Nummer */
	uint ItemNr;
  /** Hat Element Rahmen? */
	bool Frame;
  /** Seite zu der das Element gehoert */
	int OwnPage;
	/** @brief Old page number tracked for the move undo action */
	int oldOwnPage;
  /** Darzustellendes Bild */
	QImage pixm;
	QImage pixmOrg;
	ImageInfoRecord imgInfo;
  /** Dateiname des Bildes */
	QString Pfile;
	QString Pfile2;
	QString Pfile3;
	QString IProfile;
	bool UseEmbedded;
	QString EmProfile;
	int IRender;
  /** Darstellungsart Bild/Titel */
	bool PicArt;
  /** Bild verfuegbar */
	bool PicAvail;
  /** Lokales Scaling x*/
	double LocalScX;
	double LocalViewX;
  /** Lokales Scaling y*/
	double LocalScY;
	double LocalViewY;
  /** Lokales X */
	double LocalX;
  /** Lokales Y */
	double LocalY;
	int OrigW;
	int OrigH;
	double dpiX;
	double dpiY;
  /** BoundigBox-X */
	double BBoxX;
  /** BoundingBox-H */
	double BBoxH;
  /** Extra Abstand vom Rand */
	double Extra;
	double TExtra;
	double BExtra;
	double RExtra;
  /** Linespacing */
	double LineSp;
  /** Zeichen X-Position */
	double CurX;
  /** Zeichen Y-Position */
	double CurY;
  /** Cursorposition */
	int CPos;
  /** Text des Elements */
	QPtrList<ScText> itemText;
  /** Flag ob Element gedruckt wird */
	bool isPrintable;
  /** Flag fuer PDF-Bookmark */
	bool isBookmark;
	int BMnr;
  /** Flag fuer PDF-Annotation */
	bool isAnnotation;
	int AnType;
	int AnActType;
	QString AnAction;
	QString An_E_act;
	QString An_X_act;
	QString An_D_act;
	QString An_Fo_act;
	QString An_Bl_act;
	QString An_K_act;
	QString An_F_act;
	QString An_V_act;
	QString An_C_act;
	bool AutoName;
	QString AnToolTip;
	QString AnRollOver;
	QString AnDown;
	QString AnBColor;
	QString An_Extern;
	int AnBsty;
	int AnBwid;
	int AnFeed;
	int AnZiel;
	int AnFlag;
	int AnMaxChar;
	int AnVis;
	int AnChkStil;
	int AnFont;
	bool AnIsChk;
	bool AnAAact;
	bool AnHTML;
	bool AnUseIcons;
	int AnIPlace;
	int AnScaleW;
	int AnFormat;
  /** Flag fuer neuzeichnen im EditMode */
	bool Dirty;
  /** Item Font */
	QString IFont;
  /** Item Fontgroesse */
	int ISize;
  /** Flag fuer Auswahl */
	bool HasSel;
  /** Flag fuer Textfluss */
	bool Textflow;
	bool Textflow2;
	bool FrameOnly;
	PageItem *BackBox;
	PageItem *NextBox;
	int NextIt;
	int NextPg;
	bool Tinput;
	bool isAutoText;
	int textAlignment;
	uint MaxChars;
	bool Redrawn;
	double ExtraV;
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
	QValueStack<int> Groups;
	bool InvPict;
	QValueList<double> DashValues;
	QValueList<double> TabValues;
	double DashOffset;
	VGradient fill_gradient;
	QString Language;
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
	bool isSingleSel;
	double BoundingX;
	double BoundingY;
	double BoundingW;
	double BoundingH;
	bool ChangedMasterItem;
	QString OnMasterPage;
	int startArrowIndex;
	int endArrowIndex;

	/** @brief Manages undostack and is where all undo actions/states are sent. */
	UndoManager *undoManager;
	/**  @brief Get name of the item
	 *
	 * This is unrelated to QObject::name(); the pageItem's name is independent
	 * of its Qt name.
	 * See also PageItem::setItemName()
	 */
	QString itemName() const;
	/**
	 * @brief Set name of the item
	 * @param newName name for the item
	 * @author Riku Leino
	 *
	 * Note that this is unrelated to QObject::setName()
	 * See also PageItem::itemName()
	 */
	void setItemName(const QString& newName);

	/** @brief Get the (name of the) fill color of the object */
	QString fillColor() const;
	/**
	 * @brief Set the fill color of the object.
	 * @param newColor fill color for the object
	 */
	void setFillColor(const QString &newColor);

	/** @brief Get the shade of the fill color */
	int fillShade() const;
	/**
	 * @brief Set the fill color shade.
	 * @param newShade shade for the fill color
	 */
	void setFillShade(int newShade);

	/** @brief Get the transparency of the fill color */
	double fillTransparency() const;
	/**
	 * @brief Set the transparency of the fill color.
	 * @param newTransparency transparency of the fill color
	 */
	void setFillTransparency(double newTransparency);

	/** @brief Get the line color of the object */
	QString lineColor() const;
	/**
	 * @brief Set the line color of the object.
	 * @param newFill line color for the object
	 */
	void setLineColor(const QString &newColor);

	/** @brief Get the line color shade */
	int lineShade() const;
	/**
	 * @brief Set the line color shade.
	 * @param newColor shade for the line color
	 */
	void setLineShade(int newShade);

	/** @brief Get the line transparency */
	double lineTransparency() const;
	/**
	 * @brief Set the transparency of the line color.
	 * @param newTransparency transparency of the line color
	 */
	void setLineTransparency(double newTransparency);

	/** @brief Get the style of line */
	PenStyle lineStyle() const;
	/**
	 * @brief Set the style of line.
	 * @param newStyle style of line
	 * @sa Qt::PenStyle
	 */
	void setLineStyle(PenStyle newStyle);

	/** @brief Get the width of the line */
	double lineWidth() const;
	/**
	 * @brief Set the width of line
	 * @param newWidth width of line
	 */
	void setLineWidth(double newWidth);

	/** @brief Get the end cap style of the line */
	PenCapStyle lineEnd() const;
	/**
	 * @brief Set the end style of line
	 * @param newStyle end style of line
	 * @sa Qt::PenCapStyle
	 */
	void setLineEnd(PenCapStyle newStyle);

	/** @brief Get the join style of multi-segment lines */
	PenJoinStyle lineJoin() const;
	/**
	 * @brief Set the join style of line
	 * @param newStyle join style of line
	 * @sa Qt::PenJoinStyle
	 */
	void setLineJoin(PenJoinStyle newStyle);

	/** @brief Get name of active custom line style */
	QString customLineStyle() const;
	/**
	 * @brief Set custom line style
	 * @param newStyle name of the custom style
	 */
	void setCustomLineStyle(const QString& newStyle);

	/** @brief Get start arrow index
	 * @sa PageItem::getEndArrowIndex(), PageItem::setStartArrowIndex()
	 */
	int getStartArrowIndex() const;
	/**
	 * @brief Set start arrow index
	 * @param newIndex index for start arrow
	 */
	void setStartArrowIndex(int newIndex);

	/** @brief Get end arrow index
	 * @sa PageItem::getStartArrowIndex(), PageItem::setEndArrowIndex()
	 */
	int getEndArrowIndex() const;
	/**
	 * @brief Set end arrow index
	 * @param newIndex index for end arrow
	 */
	void setEndArrowIndex(int newIndex);

	/** @brief Is the image flipped horizontally? */
	bool imageFlippedH() const;
	/** @brief Horizontally flip / unflip the image */
	void setImageFlippedH(bool flipped);
	/** @brief Flip an image horizontally. */
	void flipImageH();

	/** @brief Is the image flipped vertically? */
	bool imageFlippedV() const;
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
	bool locked() const;
	/** @brief Lock or unlock this pageitem */
	void setLocked(bool isLocked);

	/** @brief Toggle lock for resizing */
	void toggleSizeLock();
	/** @brief Is the item's size locked? */
	bool sizeLocked() const;
	/** @brief set lock for resizing */
	void setSizeLocked(bool isLocked);

	/** @brief Get the PageItem-wide font name */
	QString font() const;
	/**
	 * @brief Set font for the PageItem.
	 * @param newFont name of the font
	 */
	void setFont(const QString& newFont);

	/** @brief Get the PageItem-wide font size */
	int fontSize() const;
	/**
	 * @brief Set the font size of the frame
	 * @param newSize font size
	 */
	void setFontSize(int newSize);

	/** @brief Get the PageItem-wide character width scaling percentage */
	int fontWidth() const;
	/**
	 * @brief Set scaling width of character
	 * @param newWidth width of character
	 */
	void setFontWidth(int newWidth);

	/** @brief Get the name of the PageItem-wide font fill color */
	QString fontFillColor() const;
	/**
	 * @brief Set font fill color
	 * @param newColor font fill color
	 */
	void setFontFillColor(const QString& newColor);

	/** @brief Get the PageItem-wide font fill shade */
	int fontFillShade() const;
	/**
	 * @brief Set the shade of font fill color
	 * @param newShade shade of font fill color
	 */
	void setFontFillShade(int newShade);

	/** @brief Get the PageItem-wide font stroke color */
	QString fontStrokeColor() const;
	/**
	 * @brief Set the color of font stroke
	 * @param newColor color of font stroke
	 */
	void setFontStrokeColor(const QString& newColor);

	/** @brief Get the PageItem-wide font stroke shade */
	int fontStrokeShade() const;
	/**
	 * @brief Set the shade of font stroke color
	 * @param newShade shade of font stroke color
	 */
	void setFontStrokeShade(int newShade);

	/** @brief Get the PageItem-wide font effects flags
	 *
	 * TODO This should probably be an enum set
	 */
	int fontEffects() const;
	/**
	 * @brief Set font effects
	 * @param newEffects font effects
	 */
	void setFontEffects(int newEffects);

	/** @brief Get PageItem-wide text kerning */
	double kerning() const;
	/**
	 * @brief Set kerning for the text
	 * @param newKerning kerning for the text
	 */
	void setKerning(double newKerning);

	/** @brief Get the PageItem-wide line spacing */
	double lineSpacing() const;
	/**
	 * @brief Set a line spacing for the frame
	 * @param newSpacing line spacing for the frame
	 */
	void setLineSpacing(double newSpacing);

	/** @brief Get the hyphenation language for the frame */
	QString language() const;
	/**
	 * @brief Set the hyphenation language for the frame
	 * @param newLanguage hyphenation language for the frame
	 */
	void setLanguage(const QString& newLanguage);

	/**
	 * @brief Does the text flow around this object.
	 * @param isFlowing true if text is wanted to flow around this object or false if not
	 */
	void setTextFlow(bool isFlowing);

	/**
	 * @brief Tells if the text flow should follow the square frame border if <code>useBounding</code>
	 * @brief is true, if it is set false text fill flow around the object border rather than frame.
	 *
	 * Setting this to true will unset contour line to false. Bounding box and contour line cannot
	 * be used at the same time.
	 * @param useBounding true if text should flow around the frame border false if it should follow
	 * the actual shape of the object.
	 * @sa setTextFlow()
	 * @sa useContourLine()
	 */
	void useBoundingBox(bool useBounding);

	/**
	 * @brief Tells if the text flow should follow the contour line of the frame.
	 *
	 * Setting this to true will unset bounding box to false. Contour line and bounding box cannot
	 * be used at the same time.
	 * @param useContour true if text should flow around the contour line of the frame false if
	 * it should flow around the actual shap of the object.
	 * @sa setTextFlow()
	 * @sa useBoundingBox()
	 */
	void useContourLine(bool useContour);

	/** @brief Get the frame type
	 *
	 * @attention The whole concept of frame types is due for some radical
	 *            re-working, so don't rely on this interface staying stable.
	 *            It's here as an interim step to eliminate direct member access
	 *            on PageItems.
	 */
	ItemType itemType() const;
	/**
	 * @brief Convert this PageItem to PageItem type <code>newType</code>
	 * @param newType PageItem type for conversion
	 */
	void convertTo(ItemType newType);

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
	/*@}*/
	/** @brief Required by the UndoObject */
	void restore(UndoState *state, bool isUndo);

protected:
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
	void restoreFont(SimpleState *state, bool isUndo);
	void restoreFontSize(SimpleState *state, bool isUndo);
	void restoreFontWidth(SimpleState *state, bool isUndo);
	void restoreFontFill(SimpleState *state, bool isUndo);
	void restoreFontStroke(SimpleState *state, bool isUndo);
	void restoreFontFillShade(SimpleState *state, bool isUndo);
	void restoreFontStrokeShade(SimpleState *state, bool isUndo);
	void restoreKerning(SimpleState *state, bool isUndo);
	void restoreLineSpacing(SimpleState *state, bool isUndo);
	void restoreLanguage(SimpleState *state, bool isUndo);
	void restorePStyle(SimpleState *state, bool isUndo);
	void restoreFontEffect(SimpleState *state, bool isUndo);
	void restoreType(SimpleState *state, bool isUndo);
	void restoreTextFlowing(SimpleState *state, bool isUndo);
	void restoreImageScaling(SimpleState *state, bool isUndo);
	void restorePoly(SimpleState *state, bool isUndo, bool isContour);
	void restoreContourLine(SimpleState *state, bool isUndo);
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
	ItemType itemTypeVal;

	/**
	 * @brief Item name. Unicode. User visible (outline, property palette, etc).
	 * @sa PageItem::itemName(), PageItem::setItemName()
	 */
	QString AnName;

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
	int lineShadeVal;

	/**
	 * @brief Fill shade
	 * @sa PageItem::fillShade, PageItem::setFillShade()
	 */
	int fillShadeVal;

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
	 * @brief Is the image in this image item flipped horizontally?
	 * @sa PageItem::isImageFlippedH(), PageItem::setImageFlippedH(),
	 *     PageItem::flipImageH(), PageItem::flippedV
	 */
	bool imageIsFlippedH;

	/**
	 * @brief Is the image in this image item flipped vertically?
	 * @sa PageItem::isImageFlippedV(), PageItem::setImageFlippedV(),
	 *     PageItem::flipImageV(), PageItem::flippedH
	 */
	bool imageIsFlippedV;

	/**
	 * @brief Is the item locked (cannot be moved, resized, etc)?
	 * @sa PageItem::locked(), PageItem::setLocked(), PageItem::toggleLock()
	 */
	bool Locked;

	/**
	 * @brief Is the item's size locked?
	 * @sa PageItem::sizeLocked(), PageItem::setSizeLocked(), PageItem::toggleSizeLock()
	 */
	bool LockRes;
};

#endif
