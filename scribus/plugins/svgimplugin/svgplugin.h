#ifndef SVGPLUG_H
#define SVGPLUG_H

#include <qdom.h>
#include <qptrstack.h>
#include "scribus.h"
#include "vgradient.h"

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

class GradientHelper
{
public:
	GradientHelper()
		{
		Type = 1;
		Color1 = "Black";
		Color2 = "Black";
		CSpace = false;
		matrix = QWMatrix();
		}
	int Type;
	QString Color1;
	QString Color2;
	VGradient	gradient;
	double X1;
	double Y1;
	double X2;
	double Y2;
	bool CSpace;
	QWMatrix	matrix;
	};

class SvgStyle
{
public:
	SvgStyle()
		{
		LWidth = 1.0;
		Transparency = 0.0;
		TranspStroke = 0.0;
		StrokeCol = "None";
		FillCol = "Black";
		CurCol = "None";
		Gradient = 0;
		GCol1 = "Black";
		GCol2 = "Black";
		GX1 = 0;
		GY1 = 0;
		GX2 = 0;
		GY2 = 0;
		GradCo = VGradient(VGradient::linear);
		CSpace = false;
		Family = "";
		FontSize = 12;
		matrix = QWMatrix();
		matrixg = QWMatrix();
		PLineArt = Qt::SolidLine;
		PLineJoin = Qt::MiterJoin;
		PLineEnd = Qt::FlatCap;
		InherCol = false;
		dashOffset = 0;
		dashArray.clear();
		}
	QWMatrix	matrix;
	QWMatrix	matrixg;
	double LWidth;
	Qt::PenStyle PLineArt;
	Qt::PenJoinStyle PLineJoin;
	Qt::PenCapStyle PLineEnd;
	QString StrokeCol;
	QString FillCol;
	QString CurCol;
	QString GCol1;
	QString GCol2;
	double GX1;
	double GY1;
	double GX2;
	double GY2;
	int Gradient;
	VGradient	GradCo;
	bool CSpace;
	int FontSize;
	QString Family;
	double Transparency;
	double TranspStroke;
	bool InherCol;
	double dashOffset;
	QValueList<double> dashArray;
};

class SVGPlug : public QObject
{ 
	Q_OBJECT

public:
	SVGPlug( QWidget* parent, ScribusApp *plug, QString fName );
	~SVGPlug();
	void convert();
	void addGraphicContext();
	void setupTransform( const QDomElement &e );
	void parseGroup(const QDomElement &e);
	double fromPercentage( const QString &s );
	void getDefaultUnit(const QString &unit);
	double parseUnit(const QString &unit);
	QWMatrix parseTransform(const QString &transform);
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
	QPtrList<PageItem> parseText(double x, double y, const QDomElement &e);

	ScribusDoc* Doku;
	ScribusApp* Prog;
	QDomDocument inpdoc;
	double CurrX, CurrY, StartX, StartY, Conversion;
	int PathLen;
	QPtrList<PageItem> Elements;
	QPtrStack<SvgStyle>	m_gc;
	QMap<QString, GradientHelper>	m_gradients;
	bool FirstM, WasM, PathClosed;
};

#endif

