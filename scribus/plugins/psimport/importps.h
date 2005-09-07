#ifndef IMPORTPS_H
#define IMPORTPS_H

#include "pluginapi.h"
#include "scribus.h"
#include "scplugin.h"

class PLUGIN_API ImportPSPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ImportPSPlugin();
		virtual ~ImportPSPlugin();
		virtual bool run(QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();

		// Special features (none)
};

extern "C" PLUGIN_API int importps_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* importps_getPlugin();
extern "C" PLUGIN_API void importps_freePlugin(ScPlugin* plugin);

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
	bool interactive;
};

#endif
