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

#include <memory>

#include <QDomDocument>
#include <QDomElement>
#include <QList>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <QTransform>

#include "fpointarray.h"
#include "pageitem.h"
#include "pluginapi.h"
#include "sccolor.h"

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
	~IdmlPlug() override;

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
	bool importFile(const QString& fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);
	bool readColors(const QString& fileName, ColorList & colors);

private:
	struct ObjectStyle
	{
		QString parentStyle;
		QString fillColor;
		QString fillGradient;
		QString strokeColor;
		QString strokeGradient;
		int fillTint { 100 };
		int strokeTint { 100 };
		double lineWidth { 0.0 };
		double Opacity { 0.0 };
		int blendMode { 0 };
		QPointF gradientFillStart;
		double gradientFillLength { 0.0 };
		double gradientFillAngle { 0.0 };
		QPointF gradientStrokeStart;
		double gradientStrokeLength { 0.0 };
		double gradientStrokeAngle { 0.0 };
		double Extra { 0.0 };
		double TExtra { 0.0 };
		double BExtra { 0.0 };
		double RExtra { 0.0 };
		double TextColumnCount { 1.0 };
		double TextColumnGutter { 0.0 };
		double TextColumnFixedWidth { 0.0 };
		PageItem::TextFlowMode TextFlow { PageItem::TextFlowMode::TextFlowDisabled };
		QString LeftLineEnd;
		QString RightLineEnd;
	};
	QString getNodeValue(const QDomNode &baseNode, const QString& path) const;
	bool convert(const QString& fn);
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
	QList<PageItem*> parseItemXML(const QDomElement& itElem, const QTransform& pTrans = QTransform());
	bool parseStoryXML(const QDomElement& stElem);
	void parseStoryXMLNode(const QDomElement& stNode);
	void parseParagraphStyleRange(QDomElement &ste, PageItem* item);
	void parseCharacterStyleRange(QDomElement &stt, PageItem* item, QString fontBase, QString fontStyle, ParagraphStyle &newStyle, int posC);
	void readCharStyleAttributes(CharStyle &newStyle, const QDomElement &styleElem);
	void readParagraphStyleAttributes(ParagraphStyle &newStyle, const QDomElement &styleElem);
	void resolveObjectStyle(ObjectStyle &nstyle, const QString& baseStyleName);
	int convertBlendMode(const QString& blendName) const;
	QString constructFontName(const QString& fontBaseName, const QString& fontStyle);
	
	QList<PageItem*> Elements;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };

	FPointArray Coords;
	bool interactive { false };
	MultiProgressDialog * progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	QString baseFile;
	bool firstLayer { true };
	bool firstPage { true };
	int pagecount { 1 };
	int mpagecount { 0 };
	bool facingPages { false };
	QDomDocument designMapDom;
	QStringList importedColors;
	QMap<QString, QString> colorTranslate;
	QStringList importedGradients;
	QMap<QString, QString> gradientTranslate;
	QMap<QString, int> gradientTypeMap;
	QMap<QString, QString> layerTranslate;
	QMap<QString, PageItem*> storyMap;
	QMap<QString, QString> styleTranslate;
	QMap<QString, QStringList> styleParents;
	QMap<QString, QString> charStyleTranslate;
	QMap<QString, QMap<QString, QString> > fontTranslateMap;
	QMap<QString, QStringList> masterSpreads;
	QString def_fillColor;
	QString def_fillGradient;
	QString def_strokeColor;
	QString def_strokeGradient;
	QString def_LeftLineEnd;
	QString def_RightLineEnd;
	int def_Blendmode { 0 };
	int def_fillBlendmode { 0 };
	int def_strokeBlendmode { 0 };
	int def_fillTint { 100 };
	int def_strokeTint { 100 };
	double def_lineWidth { 0.0 };
	double def_Opacity { 0.0 };
	double def_fillOpacity { 0.0 };
	double def_strokeOpacity { 0.0 };
	double def_gradientAngle { 0.0 };
	double def_gradientLen { 0.0 };
	double def_gradientX { 0.0 };
	double def_gradientY { 0.0 };
	double def_gradientStrokeStartX { 0.0 };
	double def_gradientStrokeStartY { 0.0 };
	double def_gradientStrokeLength { 0.0 };
	double def_gradientStrokeAngle { 0.0 };
	double def_Extra { 0.0 };
	double def_TExtra { 0.0 };
	double def_BExtra { 0.0 };
	double def_RExtra { 0.0 };
	double def_TextColumnCount { 1.0 };
	double def_TextColumnGutter { 0.0 };
	double def_TextColumnFixedWidth { 0.0 };
	PageItem::TextFlowMode def_TextFlow { PageItem::TextFlowMode::TextFlowDisabled };
	QMap<PageItem*, QString> frameLinks;
	QMap<QString, PageItem*> frameTargets;
	QMap<QString, ObjectStyle> ObjectStyles;

	std::unique_ptr<ScZipHandler> m_zip;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
