#ifndef OODPLUG_H
#define OODPLUG_H

#include <qdom.h>
#include <qdict.h>
#include "scribus.h"
#include "stylestack.h"

/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" void Run(QWidget *d, ScribusApp *plug);
/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString Name();
/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugins is a import Plugin, which appears in the Import Menue
  * 3 = the Plugins is a export Plugin, which appears in the Export Menue */
extern "C" int Type();
extern "C" int ID();

class OODPlug : public QObject
{ 
	Q_OBJECT

public:
	OODPlug( ScribusApp *plug, QString fName );
	~OODPlug();
	void convert();
	void parseGroup(const QDomElement &e);
	void createStyleMap( QDomDocument &docstyles );
	void insertStyles( const QDomElement& styles );
	void fillStyleStack( const QDomElement& object );
	void addStyles( const QDomElement* style );
	void storeObjectStyles( const QDomElement& object );
	static double parseUnit(const QString &unit);
	QColor parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	void parseTransform( const QString &transfor, double *rot, double *dx, double *dy );
	void parseViewBox( const QDomElement& object, double *x, double *y, double *w, double *h );
	void appendPoints(FPointArray *composite, const QDomElement& object);
	const char * getCoord( const char *ptr, double &number );
	bool parseSVG( const QString &s, FPointArray *ite );
	void calculateArc(FPointArray *ite, bool relative, double &curx, double &cury, double angle, double x, double y, double r1, double r2, bool largeArcFlag, bool sweepFlag);
	void svgClosePath(FPointArray *i);
	void svgMoveTo(double x1, double y1);
	void svgLineTo(FPointArray *i, double x1, double y1);
	void svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3);
/*	void addGraphicContext();
	void setupTransform( const QDomElement &e );
	double fromPercentage( const QString &s );	
	double parseUnit(const QString &unit);
	const char * getCoord( const char *ptr, double &number );
	bool parseSVG( const QString &s, FPointArray *ite );
	void calculateArc(FPointArray *ite, bool relative, double &curx, double &cury, double angle, double x, double y, double r1, double r2, bool largeArcFlag, bool sweepFlag);
	void svgClosePath(FPointArray *i);
	void svgMoveTo(double x1, double y1);
	void svgLineTo(FPointArray *i, double x1, double y1);
	void svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3);
	QColor parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	void parsePA( SvgStyle *obj, const QString &command, const QString &params );
	void parseStyle( SvgStyle *obj, const QDomElement &e );
	void parseColorStops(GradientHelper *gradient, const QDomElement &e);
	void parseGradient( const QDomElement &e );
	FPoint GetMaxClipO(FPointArray Clip);
	FPoint GetMinClipO(FPointArray Clip);
	QPtrList<PageItem> parseText(double x, double y, const QDomElement &e); */

	ScribusDoc* Doku;
	ScribusApp* Prog;
	QDomDocument inpContents;
	QDomDocument inpStyles;
	QDict<QDomElement> m_styles;
	StyleStack m_styleStack;
	QString stylePath;
	QString contentPath;
	double CurrX, CurrY, StartX, StartY;
	int PathLen;
	QPtrList<PageItem> Elements;
	bool FirstM, WasM, PathClosed;
};

#endif

