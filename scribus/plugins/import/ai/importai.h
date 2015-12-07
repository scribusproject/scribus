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
	bool import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);
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
	void decodeA85(QByteArray &psdata, QString tmp);
	void processData(QString data);
	void processGradientData(QString data);
	void processSymbol(QDataStream &ts, bool sym);
	void processPattern(QDataStream &ts);
	void processRaster(QDataStream &ts);
	void processComment(QDataStream &ts, QString comment);
	bool convert(QString fn);
	
	QList<PageItem*> m_Elements;
	QList<PageItem*> m_PatternElements;
	QStack<QList<PageItem*> > m_groupStack;
	QStack<FPointArray> m_clipStack;
	ColorList m_CustColors;
	QStringList m_importedColors;
	QStringList m_importedGradients;
	QStringList m_importedPatterns;
	double m_baseX, m_baseY;
	double m_docX;
	double m_docY;
	double m_docWidth;
	double m_docHeight;

	double m_LineW;
	Qt::PenCapStyle m_CapStyle;
	Qt::PenJoinStyle m_JoinStyle;
	double m_DashOffset;
	QList<double> m_DashPattern;
	double m_Opacity;
	int m_blendMode;
	QString m_CurrColorFill;
	QString m_CurrColorStroke;
	double m_CurrStrokeShade;
	double m_CurrFillShade;
	bool m_fillRule;
	bool m_itemLocked;

	FPointArray m_Coords;
	FPointArray m_clipCoords;
	FPointArray m_currentSpecialPath;
	FPoint m_currentPoint;
	int m_currentLayer;
	bool m_firstLayer;
	bool m_FirstU, m_WasU, m_ClosedPath;
	bool m_interactive;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	int m_importerFlags;
	QStringList m_commandList;
	bool m_convertedPDF;
	QMap<QString, VGradient> m_gradients;
	VGradient m_currentGradient;
	QString m_currentGradientName;
	QTransform m_currentGradientMatrix;
	QPointF m_currentGradientOrigin;
	double m_currentGradientAngle;
	double m_currentGradientLenght;
	bool m_gradientMode;
	bool m_wasBC;
	bool m_itemRendered;
	QTransform m_startMatrix;
	QTransform m_endMatrix;
	bool m_patternMode;
	QString m_currentPatternDefName;
	QString m_currentPatternName;
	double m_patternX1;
	double m_patternY1;
	double m_patternX2;
	double m_patternY2;
	double m_currentPatternX;
	double m_currentPatternY;
	double m_currentPatternXScale;
	double m_currentPatternYScale;
	double m_currentPatternRotation;
	QString m_currentStrokePatternName;
	double m_currentStrokePatternX;
	double m_currentStrokePatternY;
	double m_currentStrokePatternXScale;
	double m_currentStrokePatternYScale;
	double m_currentStrokePatternRotation;
	bool m_meshMode;
	int m_meshXSize, m_meshYSize;
	int m_currentMeshXPos, m_currentMeshYPos;
	int m_meshNodeCounter;
	int m_meshColorMode;
	double m_meshNode1PointX, m_meshNode1PointY;
	double m_meshNode1Control1X, m_meshNode1Control1Y;
	double m_meshNode1Control2X, m_meshNode1Control2Y;
	double m_meshNode2PointX, m_meshNode2PointY;
	double m_meshNode2Control1X, m_meshNode2Control1Y;
	double m_meshNode2Control2X, m_meshNode2Control2Y;
	double m_meshNode3PointX, m_meshNode3PointY;
	double m_meshNode3Control1X, m_meshNode3Control1Y;
	double m_meshNode3Control2X, m_meshNode3Control2Y;
	double m_meshNode4PointX, m_meshNode4PointY;
	double m_meshNode4Control1X, m_meshNode4Control1Y;
	double m_meshNode4Control2X, m_meshNode4Control2Y;
	QString m_meshColor1, m_meshColor2, m_meshColor3, m_meshColor4;
	QList<QList<meshPoint> > m_meshGradientArray;
	QString m_docCreator;
	QString m_docDate;
	QString m_docTime;
	QString m_docOrganisation;
	QString m_docTitle;
	int m_textMode;
	QTransform m_textMatrix;
	StoryText m_textData;
	QString m_textFont;
	double m_textSize;
	double m_maxWidth;
	double m_tempW;
	double m_maxHeight;
	double m_textKern;
	double m_textScaleH;
	double m_textScaleV;
	int m_startCurrentTextRange;
	int m_endCurrentTextRange;
	QString m_currentSymbolName;
	QMap<QString, QPointF> m_importedSymbols;
	bool m_symbolMode;
	bool m_dataMode;
	bool m_fObjectMode;
	QString m_dataString;

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
