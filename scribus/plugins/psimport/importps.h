#ifndef IMPORTPS_H
#define IMPORTPS_H

#include "qglobal.h"
#include "qobject.h"
#include "qstring.h"
#include "q3valuelist.h"
#include "q3ptrlist.h"

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

	Q3PtrList<PageItem> Elements;
	ColorList CustColors;
	double LineW, Opacity, DashOffset;
	Q3ValueList<double> DashPattern;
	QString CurrColor;
	FPointArray Coords;
	bool FirstM, WasM, ClosedPath;
	Qt::PenCapStyle CapStyle;
	Qt::PenJoinStyle JoinStyle;
	bool interactive;
};

#endif
