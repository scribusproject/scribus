/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTPS_H
#define IMPORTPS_H

#include "qglobal.h"
#include "qobject.h"
#include "qstring.h"
#include "qvaluelist.h"
#include "qptrlist.h"

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"

class EPSPlug : public QObject
{
	Q_OBJECT

public:
	EPSPlug( QString fName, bool isInteractive );
	~EPSPlug() {};
	bool convert(QString fn, double x, double y, double b, double h);
	void parseOutput(QString fn, bool eps);
	void LineTo(FPointArray *i, QString vals);
	void Curve(FPointArray *i, QString vals);
	QString parseColor(QString vals, colorModel model = colorModelCMYK);

	QPtrList<PageItem> Elements;
	ColorList CustColors;
	double LineW, Opacity, DashOffset;
	QValueList<double> DashPattern;
	QString CurrColor;
	FPointArray Coords;
	bool FirstM, WasM, ClosedPath;
	PenCapStyle CapStyle;
	PenJoinStyle JoinStyle;
	bool interactive;
};

#endif
