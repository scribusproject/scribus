/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Sep 29 2013
	copyright            : (C) 2013 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef IMPORTVIVA_H
#define IMPORTVIVA_H

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "sctextstruct.h"
#include "text/storytext.h"
#include "fpointarray.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

class AttributeValue
{
	public:
		AttributeValue() : valid(false),  value("")
		{
		}
		AttributeValue(QString val) : valid(true),  value(val)
		{
		}
		bool valid;
		QString value;
};

//! \brief Viva importer plugin
class VivaPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Viva importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	VivaPlug( ScribusDoc* doc, int flags );
	~VivaPlug();

	/*!
	\author Franz Schmid
	\date
	\brief Perform import.
	\param fn QString
	\param trSettings undo transaction settings
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval bool true if import was ok
	 */
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);
	bool readColors(const QString& fileName, ColorList & colors);

private:
	struct triplePoint
	{
		FPoint beforePolyPoint;
		FPoint PolyPoint;
		FPoint afterPolyPoint;
	};

	struct AttributeSet
	{
		AttributeValue applyedParStyle;
		AttributeValue parentStyle;
		// Character Attributes
		AttributeValue fontFullName;
		AttributeValue fontFamily;
		AttributeValue fontStyle;
		AttributeValue fontSize;
		AttributeValue fontColor;
		AttributeValue fontColorDensity;
		AttributeValue fontEffect;
		AttributeValue placement;
		AttributeValue underline;
		AttributeValue underlineWidth;
		AttributeValue underlineOffset;
		AttributeValue strikethrough;
		AttributeValue strikethroughWidth;
		AttributeValue strikethroughOffset;
		AttributeValue outline;
		AttributeValue outlineWidth;
		AttributeValue outlineColor;
		AttributeValue widthScale;
		AttributeValue heightScale;
		AttributeValue spacing;
		AttributeValue baselineOffset;
		// Paragraph Attributes
		AttributeValue justification;
		AttributeValue gapbefore;
		AttributeValue gapafter;
		AttributeValue lineSpacing;
		AttributeValue indent;
		AttributeValue firstLineIndent;
		AttributeValue rightIndent;
		AttributeValue columnCount;
		AttributeValue columnGutter;
		AttributeValue dropCaps;
		AttributeValue dropCapsLines;
		AttributeValue dropCapsDist;
		AttributeValue tabulators;
	};
	double parseUnit(const QString &unit);
	bool convert(QString fn);
	void parseSettingsXML(const QDomElement& grNode);
	void parseColorsXML(const QDomElement& grNode);
	void parsePreferencesXML(const QDomElement& spNode);
	void parseLayerXML(const QDomElement& spNode);
	void parseMasterSpreadXML(const QDomElement& spNode);
	void parseSpreadXML(const QDomElement& spElem);
	void parseTextChainsXML(const QDomElement& obNode);
	PageItem* parseObjectXML(const QDomElement& obNode);
	PageItem *parseObjectDetailsXML(const QDomElement& obNode, int baseType);
	void parseTextXML(const QDomElement& obNode, StoryText &itemText, int &textColumnCount, double &textColumnGap);
	void parseAttributeSetXML(const QDomElement& obNode, AttributeSet &attrs);
	void parseAttributeSetsXML(const QDomElement& obNode);
	void parseStylesheetsXML(const QDomElement& obNode);
	void applyParagraphAttrs(ParagraphStyle &newStyle, AttributeSet &pAttrs);
	void applyCharacterAttrs(CharStyle &tmpCStyle, ParagraphStyle &newStyle, AttributeSet &pAttrs);
	QString constructFontName(QString fontBaseName, QString fontStyle);
	QPointF intersectBoundingRect(PageItem *item, QLineF gradientVector);

	QList<PageItem*> Elements;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	bool facingPages;
	bool hasLayers;
	bool firstLayer;
	bool firstPage;
	int pagecount;
	int mpagecount;
	QMap<QString, int> mspreadTypes;
	FPointArray Coords;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	QString baseFile;
	QDomDocument designMapDom;
	QStringList importedColors;
	double topMargin;
	double leftMargin;
	double rightMargin;
	double bottomMargin;
	QString papersize;
	QHash<QString, AttributeSet> AttributeSets;
	QHash<QString, QString> colorTranslate;
	QStringList importedGradients;
	QMap<QString, QString> gradientTranslate;
	QMap<QString, int> gradientTypeMap;
	QMap<QString, PageItem*> storyMap;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
