/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Jan 14 2012
	copyright            : (C) 2012 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef IMPORTIDML_H
#define IMPORTIDML_H

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

#include "third_party/zip/scribus_zip.h"

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief Idml importer plugin
class IdmlPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Idml importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	IdmlPlug( ScribusDoc* doc, int flags );
	~IdmlPlug();

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
	struct ObjectStyle
	{
		QString parentStyle;
		QString fillColor;
		QString fillGradient;
		QString strokeColor;
		QString strokeGradient;
		int fillTint;
		int strokeTint;
		double lineWidth;
		double Opacity;
		int blendMode;
		QPointF gradientFillStart;
		double gradientFillLength;
		double gradientFillAngle;
		QPointF gradientStrokeStart;
		double gradientStrokeLength;
		double gradientStrokeAngle;
		double Extra;
		double TExtra;
		double BExtra;
		double RExtra;
		double TextColumnCount;
		double TextColumnGutter;
		double TextColumnFixedWidth;
		PageItem::TextFlowMode TextFlow;
		QString LeftLineEnd;
		QString RightLineEnd;
	};
	QString getNodeValue(QDomNode &baseNode, QString path);
	bool convert(QString fn);
	bool parseFontsXML(const QDomElement& grElem);
	void parseFontsXMLNode(const QDomElement& grNode);
	bool parseGraphicsXML(const QDomElement& grElem);
	void parseGraphicsXMLNode(const QDomElement& grNode);
	bool parseStylesXML(const QDomElement& sElem);
	void parseStylesXMLNode(const QDomElement& sNode);
	void parseObjectStyle(const QDomElement& styleElem);
	void parseCharacterStyle(const QDomElement& styleElem);
	void parseParagraphStyle(const QDomElement& styleElem);
	bool parsePreferencesXML(const QDomElement& prElem);
	void parsePreferencesXMLNode(const QDomElement& prNode);
	bool parseSpreadXML(const QDomElement& spElem);
	void parseSpreadXMLNode(const QDomElement& spNode);
	QList<PageItem*> parseItemXML(const QDomElement& itElem, QTransform pTrans = QTransform());
	bool parseStoryXML(const QDomElement& stElem);
	void parseStoryXMLNode(const QDomElement& stNode);
	void parseParagraphStyleRange(QDomElement &ste, PageItem* item);
	void parseCharacterStyleRange(QDomElement &stt, PageItem* item, QString fontBase, QString fontStyle, ParagraphStyle &newStyle, int posC);
	void readCharStyleAttributes(CharStyle &newStyle, const QDomElement &styleElem);
	void readParagraphStyleAttributes(ParagraphStyle &newStyle, const QDomElement &styleElem);
	void resolveObjectStyle(ObjectStyle &nstyle, QString baseStyleName);
	int convertBlendMode(QString blendName);
	QString constructFontName(QString fontBaseName, QString fontStyle);
	
	QList<PageItem*> m_Elements;
	double m_baseX, m_baseY;
	double m_docWidth;
	double m_docHeight;

	FPointArray m_Coords;
	bool m_interactive;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	int m_importerFlags;
	QString m_baseFile;
	bool m_firstLayer;
	bool m_firstPage;
	int m_pagecount;
	int m_mpagecount;
	bool m_facingPages;
	QDomDocument m_designMapDom;
	QStringList m_importedColors;
	QMap<QString, QString> m_colorTranslate;
	QStringList m_importedGradients;
	QMap<QString, QString> m_gradientTranslate;
	QMap<QString, int> m_gradientTypeMap;
	QMap<QString, QString> m_layerTranslate;
	QMap<QString, PageItem*> m_storyMap;
	QMap<QString, QString> m_styleTranslate;
	QMap<QString, QStringList> m_styleParents;
	QMap<QString, QString> m_charStyleTranslate;
	QMap<QString, QMap<QString, QString> > m_fontTranslateMap;
	QMap<QString, QStringList> m_masterSpreads;
	QString m_def_fillColor;
	QString m_def_fillGradient;
	QString m_def_strokeColor;
	QString m_def_strokeGradient;
	QString m_def_LeftLineEnd;
	QString m_def_RightLineEnd;
	int m_def_Blendmode;
	int m_def_fillBlendmode;
	int m_def_strokeBlendmode;
	int m_def_fillTint;
	int m_def_strokeTint;
	double m_def_lineWidth;
	double m_def_Opacity;
	double m_def_fillOpacity;
	double m_def_strokeOpacity;
	double m_def_gradientAngle;
	double m_def_gradientLen;
	double m_def_gradientX;
	double m_def_gradientY;
	double m_def_gradientStrokeStartX;
	double m_def_gradientStrokeStartY;
	double m_def_gradientStrokeLength;
	double m_def_gradientStrokeAngle;
	double m_def_Extra;
	double m_def_TExtra;
	double m_def_BExtra;
	double m_def_RExtra;
	double m_def_TextColumnCount;
	double m_def_TextColumnGutter;
	double m_def_TextColumnFixedWidth;
	PageItem::TextFlowMode m_def_TextFlow;
	QMap<PageItem*, QString> m_frameLinks;
	QMap<QString, PageItem*> m_frameTargets;
	QMap<QString, ObjectStyle> m_ObjectStyles;

	ScZipHandler *m_fun;

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
