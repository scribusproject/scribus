#ifndef SVGPLUG_H
#define SVGPLUG_H

#include <qdom.h>
#include <qptrstack.h>
#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "vgradient.h"

class ScrAction;

/**
 * The ID for the SVG Import format. This must be a macro not a static const member
 * because it must be available even when the SVGImportPlugin is not linked in
 * or even compiled.
 */
#define FORMATID_SVGIMPORT 3

/**
 * A class providing the plugin interface implementation for this plugin
 */
class PLUGIN_API SVGImportPlugin : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		SVGImportPlugin();
		virtual ~SVGImportPlugin();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool fileSupported(QIODevice* file) const;
		virtual bool loadFile(const QString & fileName, const FileFormat & fmt);

	public slots:
		virtual bool import(QString target = QString::null);

	private:
		void registerFormats();
		ScrAction* importAction;

};

extern "C" PLUGIN_API int svgimplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* svgimplugin_getPlugin();
extern "C" PLUGIN_API void svgimplugin_freePlugin(ScPlugin* plugin);

class PageItem;
class ScribusDoc;
class PrefsManager;
class FPointArray;

class GradientHelper
{
public:
	GradientHelper() :
		CSpace(false),
		cspaceValid(true),
		gradient(VGradient::linear),
		gradientValid(false),
		matrix(),
		matrixValid(false),
		reference(""),
		Type(1),
		typeValid(false),
		X1(0),
		x1Valid(true),
		X2(1),
		x2Valid(true),
		Y1(0),
		y1Valid(true),
		Y2(0),
		y2Valid(true)
		{
		}
	bool CSpace;
	bool cspaceValid;
	VGradient gradient;
	bool gradientValid;
	QWMatrix matrix;
	bool matrixValid;
	QString reference;
	int Type;
	bool typeValid;
	double X1;
	bool x1Valid;
	double X2;
	bool x2Valid;
	double Y1;
	bool y1Valid;
	double Y2;
	bool y2Valid;
	};

class SvgStyle
{
public:
	SvgStyle() :
		CSpace(false),
		CurCol("None"),
		dashOffset(0),
		Family(""),
		FillCol("Black"),
		FontSize(12),
		GCol1("Black"),
		GCol2("Black"),
		GradCo(VGradient::linear),
		Gradient(0),
		GX1(0),
		GX2(0),
		GY1(0),
		GY2(0),
		InherCol(false),
		LWidth(1.0),
		matrix(),
		matrixg(),
		PLineArt(Qt::SolidLine),
		PLineEnd(Qt::FlatCap),
		PLineJoin(Qt::MiterJoin),
		StrokeCol("None"),
		Transparency(0.0),
		TranspStroke(0.0)
		{
		}
	bool CSpace;
	QString CurCol;
	QValueList<double> dashArray;
	double dashOffset;
	QString Family;
	QString FillCol;
	int FontSize;
	QString GCol1;
	QString GCol2;
	VGradient	GradCo;
	int Gradient;
	double GX1;
	double GX2;
	double GY1;
	double GY2;
	bool InherCol;
	double LWidth;
	QWMatrix matrix;
	QWMatrix matrixg;
	Qt::PenStyle PLineArt;
	Qt::PenCapStyle PLineEnd;
	Qt::PenJoinStyle PLineJoin;
	QString StrokeCol;
	double Transparency;
	double TranspStroke;
};

class SVGPlug : public QObject
{
	Q_OBJECT

public:
	SVGPlug(QString fname, bool isInteractive);
	~SVGPlug();
	void convert();
	void addGraphicContext();
	void setupTransform( const QDomElement &e );
	QPtrList<PageItem> parseGroup(const QDomElement &e);
	double fromPercentage( const QString &s );
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

	ScribusDoc* currDoc;
	QDomDocument inpdoc;
	double CurrX, CurrY, StartX, StartY, Conversion;
	int PathLen;
	QPtrList<PageItem> Elements;
	QPtrStack<SvgStyle>	m_gc;
	QMap<QString, GradientHelper>	m_gradients;
	bool FirstM, WasM, PathClosed;
	double viewTransformX;
	double viewTransformY;
	double viewScaleX;
	double viewScaleY;
	bool haveViewBox;
	bool interactive;
	//! \brief Indicator if there is any unsupported feature in imported svg.
	bool unsupported;
};

#endif
