/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTAI_H
#define IMPORTAI_H

#include "qglobal.h"
#include "qobject.h"
#include "qstring.h"

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include "vgradient.h"
#include <QList>
#include <QMatrix>
// #include <QPainterPath>

class MultiProgressDialog;
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
	bool import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress = true);

private:
	
	/*!
	\author Franz Schmid
	\date
	\brief Does the conversion.
	\param infile a filename
	\param outfile a filename for output
	\retval bool true if conversion was ok
	 */
	bool extractFromPDF(QString infile, QString outfile);

	bool decompressAIData(QString &fName);
	bool parseHeader(QString fName, double &x, double &y, double &b, double &h);
	QString removeAIPrefix(QString comment);
	QString parseColor(QString data);
	QString parseColorGray(QString data);
	QString parseColorRGB(QString data);
	QString parseCustomColor(QString data, double &shade);
	QString parseCustomColorX(QString data, double &shade, QString type);
	QStringList getStrings(QString data);
	void getCommands(QString data, QStringList &commands);
	void processData(QString data);
	void processGradientData(QString data);
	void processPattern(QDataStream &ts);
	void processRaster(QDataStream &ts);
	void processComment(QDataStream &ts, QString comment);
	bool convert(QString fn);
	
	QList<PageItem*> Elements;
	QList<PageItem*> PatternElements;
	QStack<QList<PageItem*> > groupStack;
	ColorList CustColors;
	double baseX, baseY;
	double docX;
	double docY;
	double docWidth;
	double docHeight;

	double LineW;
	Qt::PenCapStyle CapStyle;
	Qt::PenJoinStyle JoinStyle;
	double DashOffset;
	QList<double> DashPattern;
	double Opacity;
	int blendMode;
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade;
	double CurrFillShade;
	bool fillRule;
	bool itemLocked;

	FPointArray Coords;
	FPointArray clipCoords;
	FPoint currentPoint;
	int currentLayer;
	bool firstLayer;
	bool FirstU, WasU, ClosedPath;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	QStringList commandList;
	bool convertedPDF;
	QMap<QString, VGradient> m_gradients;
	VGradient currentGradient;
	QString currentGradientName;
	QMatrix currentGradientMatrix;
	QPointF currentGradientOrigin;
	double currentGradientAngle;
	double currentGradientLenght;
	bool gradientMode;
	bool wasBC;
	bool itemRendered;
	QMatrix startMatrix;
	QMatrix endMatrix;
	bool patternMode;
	QString currentPatternDefName;
	QString currentPatternName;
	double patternX1;
	double patternY1;
	double patternX2;
	double patternY2;
	double currentPatternX;
	double currentPatternY;
	double currentPatternXScale;
	double currentPatternYScale;
	double currentPatternRotation;
	bool meshMode;
	int meshXSize, meshYSize;
	int currentMeshXPos, currentMeshYPos;
	int meshNodeCounter;
	int meshColorMode;
	double meshNode1PointX, meshNode1PointY;
	double meshNode1Control1X, meshNode1Control1Y;
	double meshNode1Control2X, meshNode1Control2Y;
	double meshNode2PointX, meshNode2PointY;
	double meshNode2Control1X, meshNode2Control1Y;
	double meshNode2Control2X, meshNode2Control2Y;
	double meshNode3PointX, meshNode3PointY;
	double meshNode3Control1X, meshNode3Control1Y;
	double meshNode3Control2X, meshNode3Control2Y;
	double meshNode4PointX, meshNode4PointY;
	double meshNode4Control1X, meshNode4Control1Y;
	double meshNode4Control2X, meshNode4Control2Y;
	double accumColorC, accumColorM, accumColorY, accumColorK;
//	QPainterPath meshBoundingPath;
	QString docCreator;
	QString docDate;
	QString docTime;
	QString docOrganisation;
	QString docTitle;
	int textMode;
	QMatrix textMatrix;
	StoryText textData;
	QString textFont;
	double textSize;
	double maxWidth;
	double tempW;
	double maxHeight;
	double textKern;
	double textScaleH;
	double textScaleV;
	int startCurrentTextRange;
	int endCurrentTextRange;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
