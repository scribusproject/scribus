/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          importdrw.h  -  description
                             -------------------
    begin                : Mon Jan 11 2010
    copyright            : (C) 2010 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef IMPORTDRW_H
#define IMPORTDRW_H

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

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief Drw importer plugin
class DrwPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Cvg importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	DrwPlug( ScribusDoc* doc, int flags );
	~DrwPlug();

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

private:
	bool convert(QString fn);
	void decodeCmdData(QDataStream &ts, uint dataLen, quint8 cmd);
	void decodeCmd(quint8 cmd, int pos);
	void decodeSymbol(QDataStream &ds);
	void handlePreviewBitmap(QDataStream &ds);
	QString handleColor(ScColor &color, QString proposedName);
	QString getColor(QDataStream &ds);
	void finishItem(PageItem* ite, bool scale = true);
	double getValue(QDataStream &ds);
	double getRawValue(QDataStream &ds);
	QPointF getCoordinate(QDataStream &ds);
	
	QList<PageItem*> Elements;
	struct DRWGroup
	{
		double xoffset;
		double yoffset;
		double width;
		double height;
		int nrOfItems;
		int counter;
		quint8 patternIndex;
		QString fillColor;
		QList<PageItem*> GElements;
	};
	QStack<DRWGroup> groupStack;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	QStringList importedColors;
	QString lineColor;
	QString fillColor;
	double lineWidth;
	int createObjCode;
	int nrOfPoints;
	PageItem *currentItem;

	int symbolCount;
	int recordCount;

	FPointArray Coords;
	QByteArray cmdData;
	double scaleFactor;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	QString baseFile;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
