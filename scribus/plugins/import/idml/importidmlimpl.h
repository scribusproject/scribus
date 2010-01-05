/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTIDMLIMPL_H
#define IMPORTIDMLIMPL_H

#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include "docpreference.h"

#include <QObject>
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QRect>
#include <QStack>
#include <QDomNode>
#include <QDomElement>
#include <QPointF>

class QString;
class ScribusDoc;
class MultiProgressDialog;
class Selection;
class TransactionSettings;
class ScLayer;

enum Capitalization
{
	AllCaps,
	SmallCaps,
	Normal
};
enum Justification
{
	LeftAlign,
	CenterAlign,
	RightAlign,
	FullyJustified
};
enum Position
{
	NormalP,
	SuperScript,
	SubScript
};
struct CommonTextProps
{
//  CommonTextProperties represented as Attributes
	QString AppliedCStyle;
	QString AppliedLanguage;
	QString AppliedPStyle;
	double AutoLeading;
	double BaselineShift;
	Capitalization Capital;
	double DesiredGlyphScale;
	double DesiredWordSpace;
	double DropCapChars;				//Not supported in Scribus
	double DropCapLines;
	QString FillColor;
	QString FillTint;
	double FirstLineIndent;
	QString FontStyle;
	double GradFillAngle;
	double GradFillLength;
	double GradStrokeAngle;
	double GradStrokeLength;
	double HorizontalScale;
	Justification justify;
	double KerningValue;
	double LeftIndent;
	double MaxGlyphScaling;
	double MaxWordSpacing;
	double MinGlyphScaling;
	double MinWordSpacing;
	double PointSize;
	Position position;
	double RightIndent;
	double SpaceAfter;
	double SpaceBefore;
	double StrikeThruWeight;
	double StrikeThruOffset;
	bool StrikeThru;
	QString StrokeColor;
	double StrokeTint;
	double Tracking;
	bool Underline;
	double UnderlineOffset;
	double UnderlineWeight;
//CommonTextProperties represented as elements
	QString AppliedFont;
	/* Handle Tablists later */
};

struct CStyleRange
{
	CommonTextProps properties;
	QString Content;
};

/*!
\brief Stuct PStyleRange will hold the values of all those attributes/elements which can occur in the ParagraphStyleRange in a Story. Since it can contain any number of CharacterStyleRange elements as its children, we use a list to store all of them.
\param properties contain all the common text properties, of which if anyone is skipped, we use the default value for it from the docpreference object.
\param cstyleRange contain all the CharacterStyleRange elements in the ParagraphStyleRange
*/
struct PStyleRange
{
	CommonTextProps properties;
	QList<CStyleRange> cstyleRange;
};

class Story
{
  public:
	QList<PStyleRange> pstyleRange;
	CommonTextProps properties;
};

struct PathPoint
{
	QPointF Anchor;
	QPointF Left;
	QPoint Right;
};

class ImportIdmlImpl : public QObject
{
	Q_OBJECT
	public:
		ImportIdmlImpl();
		~ImportIdmlImpl() {};
		bool run(const QString & target, ScribusDoc* doc=0);
		
		QString desmapPath, fontPath, stylePath, graphicPath, preferPath, mspreadPath;
	private:
		bool processDesignMap();
		bool processFont();
		bool processStyles();
		bool importPStyles(QDomNode&);
		bool importCStyles(QDomNode&) { return false; };
		bool processSpreads();
		bool processStory();
		bool readStoryObjects(QDomNode);
		void readComTextProps(QDomNode, CommonTextProps&);
		void readSpread(QDomNode);
		void readMSpread(QDomNode);
		void docAttributes(QDomElement&);
		void handleLanguage(QDomElement&);
		void handleLayer(ScLayer&,QDomElement&);
		void handleSection(ScribusDoc*, QDomElement&);
		void handleCRFormat(QDomElement&);
		void handleBookmarks(QDomElement&);
		
		/** PageItem Functions */
		void handleGroup(QDomNode);
		void readGradiant(QDomNode);
		void addTextFrame(ScribusDoc*,QDomNode);
		void addPage(ScribusDoc*, QDomNode);
		void addMasterPage(ScribusDoc*,QDomNode);
		void addRectangle(ScribusDoc*,QDomNode) {};
		void addOval(ScribusDoc*,QDomNode);
		void addPolygon(ScribusDoc*,QDomNode) {};
		void addGraphicLine(ScribusDoc*,QDomNode) {};
		double findWidth(double,double,double,double);
		double findHeight(double,double,double,double);
		QTransform applyTransform(QString) { return QTransform(); }
		PathPoint readPathPoint(QDomElement);
		
		void getMSpreadSrc(QDomElement&);
		void getSpreadSrc(QDomElement&);
		void getStorySrc(QDomElement&);
		void getBStorySrc(QDomElement&);
		void readParagraphStyle(ScribusDoc *doc, QDomNode node, ParagraphStyle& newStyle, SCFonts &fonts);
		void readCharacterStyle(ScribusDoc *doc, QDomNode node, CharStyle& newStyle);
		void fixLegacyParStyle(ParagraphStyle& pstyle);
		void fixLegacyCStyle(CharStyle& cstyle) {};
		CStyleRange readCStyleRange(QDomNode node);
	
		QDomElement loadXmlFile(QString &filename);
		QDomDocument doc;
		QDomNode node;
		QDomElement root;
		QDomElement element;
		QMap<QString,int> layerMap;
		QMap<QString,Story> storyMap;
		QList<QString> storyList;
		QList<QString> storyPath;
		QList<QString> Spreads;
		QList<QString> spreadPath;
		QList<QString> MasterSpreads;
		QList<QString> MSpreadPath;
		QList<PageItem*> Elements;
		int layerID;
		int pageNumber;
		int currentLayer;
		int importerFlags;
		int currentPageNr;
		bool interactive;
		bool cancel;
		bool firstLayer;
		double baseX, baseY;
		double docWidth;
		double docHeight;
		QString baseFile;
		QString activeLayer;
		DocPreferences preference;
		FPointArray coords;
		MultiProgressDialog *progressDialog;
		ScribusDoc* m_Doc;
		SCFonts* m_AvailableFonts;
		Selection *tmpSel;
		ParagraphStyle vg;
		CharStyle cs;
		PageItem *currItem;
};

#endif
