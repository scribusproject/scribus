/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTAI_H
#define IMPORTAI_H

#include <QList>
#include <QTransform>
#include <QObject>
#include <QString>

#include "pluginapi.h"

#include "fpointarray.h"
#include "mesh.h"
#include "sccolor.h"
#include "text/storytext.h"
#include "vgradient.h"

class MultiProgressDialog;
class PageItem;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief Adobe Illustrator importer plugin
class AIPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the AI importer window.
	\param doc a Scribus document reference
	\param flags combination of loadFlags - see loadFlags in LoadSavePlugin
	*/
	AIPlug( ScribusDoc* doc, int flags );
	~AIPlug();

	/*!
	\author Franz Schmid
	\date
	\brief Perform import.
	\param fNameIn QString a filename to import
	\param trSettings undo transaction settings
	\param flags combination of loadFlags in LoadSavePlugin
	\param showProgress if progress must be displayed
	\retval bool true if import was ok
	 */
	bool import(const QString& fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);
	bool readColors(const QString& fileName, ColorList & colors);

private:
	
	/*!
	\author Franz Schmid
	\date
	\brief Does the conversion.
	\param infile a filename
	\param outfile a filename for output
	\retval bool true if conversion was ok
	 */
	bool extractFromPDF(const QString& infile, const QString& outfile);

	bool decompressAIData(QString &fName);
	bool parseHeader(const QString& fName, double &x, double &y, double &b, double &h);
	QString removeAIPrefix(QString comment);
	QString parseColor(QString data);
	QString parseColorGray(QString data);
	QString parseColorRGB(QString data);
	QString parseCustomColor(QString data, double &shade);
	QString parseCustomColorX(QString data, double &shade, const QString& type);
	QStringList getStrings(const QString& data);
	void getCommands(const QString& data, QStringList &commands);
	void decodeA85(QByteArray &psdata, const QString& tmp);
	void processData(const QString& data);
	void processGradientData(const QString& data);
	void processSymbol(QDataStream &ts, bool sym);
	void processPattern(QDataStream &ts);
	void processRaster(QDataStream &ts);
	void processComment(QDataStream &ts, const QString& comment);
	bool convert(const QString& fn);
	
	QList<PageItem*> Elements;
	QList<PageItem*> PatternElements;
	QStack<QList<PageItem*> > groupStack;
	QStack<FPointArray> clipStack;
	ColorList CustColors;
	QStringList importedColors;
	QStringList importedGradients;
	QStringList importedPatterns;
	double baseX = 0.0;
	double baseY = 0.0;
	double docX = 0.0;
	double docY = 0.0;
	double docWidth = 0.0;
	double docHeight = 0.0;

	double LineW = 1.0;
	Qt::PenCapStyle CapStyle = Qt::FlatCap;
	Qt::PenJoinStyle JoinStyle = Qt::MiterJoin;
	double DashOffset = 0.0;
	QList<double> DashPattern;
	double Opacity = 1.0;
	int blendMode = 0;
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade = 100.0;
	double CurrFillShade = 100.0;
	bool fillRule = false;
	bool itemLocked = false;

	FPointArray Coords;
	FPointArray clipCoords;
	FPointArray currentSpecialPath;
	FPoint currentPoint;
	int currentLayer = 0;
	bool firstLayer = true;
	bool FirstU = false;
	bool WasU = false;
	bool interactive;
	MultiProgressDialog * progressDialog = nullptr;
	bool cancel = false;
	ScribusDoc* m_Doc = nullptr;
	Selection* tmpSel = nullptr;
	int importerFlags { 0 };
	QStringList commandList;
	bool convertedPDF = false;
	QMap<QString, VGradient> m_gradients;
	VGradient currentGradient;
	QString currentGradientName;
	QTransform currentGradientMatrix;
	QPointF currentGradientOrigin;
	double currentGradientAngle = 0.0;
	double currentGradientLength = 1.0;
	bool gradientMode = false;
	bool wasBC = false;
	bool itemRendered = false;
	QTransform startMatrix;
	QTransform endMatrix;
	bool patternMode = false;
	QString currentPatternDefName;
	QString currentPatternName;
	double patternX1 = 0.0;
	double patternY1 = 0.0;
	double patternX2 = 0.0;
	double patternY2 = 0.0;
	double currentPatternX = 0.0;
	double currentPatternY = 0.0;
	double currentPatternXScale = 1.0;
	double currentPatternYScale = 1.0;
	double currentPatternRotation = 0.0;
	QString currentStrokePatternName;
	double currentStrokePatternX = 0.0;
	double currentStrokePatternY = 0.0;
	double currentStrokePatternXScale = 1.0;
	double currentStrokePatternYScale = 1.0;
	double currentStrokePatternRotation = 0.0;
	bool meshMode = false;
	int meshXSize = 0;
	int meshYSize = 0;
	int currentMeshXPos = 0;
	int currentMeshYPos = 0;
	int meshNodeCounter = 0;
	int meshColorMode = 0;
	double meshNode1PointX = 0.0;
	double meshNode1PointY = 0.0;
	double meshNode1Control1X = 0.0, meshNode1Control1Y = 0.0;
	double meshNode1Control2X = 0.0, meshNode1Control2Y = 0.0;
	double meshNode2PointX = 0.0, meshNode2PointY = 0.0;
	double meshNode2Control1X = 0.0, meshNode2Control1Y = 0.0;
	double meshNode2Control2X = 0.0, meshNode2Control2Y = 0.0;
	double meshNode3PointX = 0.0, meshNode3PointY = 0.0;
	double meshNode3Control1X = 0.0, meshNode3Control1Y = 0.0;
	double meshNode3Control2X = 0.0, meshNode3Control2Y = 0.0;
	double meshNode4PointX = 0.0, meshNode4PointY = 0.0;
	double meshNode4Control1X = 0.0, meshNode4Control1Y = 0.0;
	double meshNode4Control2X = 0.0, meshNode4Control2Y = 0.0;
	QString meshColor1, meshColor2, meshColor3, meshColor4;
	QList<QList<MeshPoint> > meshGradientArray;
	QString docCreator;
	QString docDate;
	QString docTime;
	QString docOrganisation;
	QString docTitle;
	int textMode = 0;
	QTransform textMatrix;
	StoryText textData;
	QString textFont;
	double textSize = 0.0;
	double maxWidth = 0.0;
	double tempW = 0.0;
	double maxHeight = 0.0;
	double textKern= 0.0;
	double textScaleH = 0.0;
	double textScaleV = 0.0;
	int startCurrentTextRange = 0;
	int endCurrentTextRange = 0;
	QString currentSymbolName;
	QMap<QString, QPointF> importedSymbols;
	bool symbolMode = false;
	bool dataMode = false;
    bool fObjectMode = false;
	QString dataString;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
