#ifndef IMPORTPS_H
#define IMPORTPS_H

#include "scribus.h"
#include "pluginmanager.h"

/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" void run(QWidget *d, ScribusApp *plug);
/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString name();
/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugins is a import Plugin, which appears in the Import Menue
  * 3 = the Plugins is a export Plugin, which appears in the Export Menue */
extern "C" PluginManager::PluginType type();
extern "C" int ID();

extern "C" QString actionName();
extern "C" QString actionKeySequence();
extern "C" QString actionMenu();
extern "C" QString actionMenuAfterName();
extern "C" bool actionEnabledOnStartup();

class EPSPlug : public QObject
{
	Q_OBJECT

public:
	EPSPlug( ScribusApp *plug, QString fName );
	~EPSPlug() {};
	bool convert(QString fn, double x, double y, double b, double h);
	void parseOutput(QString fn, bool eps);
	void LineTo(FPointArray *i, QString vals);
	void Curve(FPointArray *i, QString vals);
	QString parseColor(QString vals, colorModel model = colorModelCMYK);

	ScribusDoc* Doku;
	ScribusApp* Prog;
	QPtrList<PageItem> Elements;
	ColorList CustColors;
	double LineW, Opacity, DashOffset;
	QValueList<double> DashPattern;
	QString CurrColor;
	FPointArray Coords;
	bool FirstM, WasM, ClosedPath;
	PenCapStyle CapStyle;
	PenJoinStyle JoinStyle;
};

#endif
