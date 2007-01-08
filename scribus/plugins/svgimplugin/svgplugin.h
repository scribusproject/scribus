/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SVGPLUG_H
#define SVGPLUG_H

#include <qdom.h>
#include <qptrstack.h>
#include <qsize.h>
#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "../formatidlist.h"
#include "vgradient.h"

class ScrAction;
class ScribusMainWindow;

/**
 * \brief A class providing the plugin interface implementation for this plugin
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
		virtual bool fileSupported(QIODevice* file, const QString & fileName=QString::null) const;
		virtual bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0);
		virtual void addToMainWindowMenu(ScribusMainWindow *);

	public slots:
		/*!
		\author Franz Schmid
		\brief Run the SVG import
		\param filename a file name to import
		\retval true for success
		 */
		virtual bool import(QString filename = QString::null, int flags = lfUseCurrentPage|lfInteractive);

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
		Display(true),
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
		Opacity(1.0),
		FillOpacity(1.0),
		StrokeOpacity(1.0),
		textAnchor("start")
		{
		}
	bool Display;
	bool CSpace;
	QString CurCol;
	QValueList<double> dashArray;
	double dashOffset;
	QString Family;
	QString FillCol;
	QString fillRule;
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
	double Opacity;
	double FillOpacity;
	double StrokeOpacity;
	QString textAnchor;
};

class SVGPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\brief Create the SVG importer window
	\param fName QString
	\param isInteractive flag to use GUI
	 */
	SVGPlug(ScribusMainWindow *mw, QString fname, int flags);
	~SVGPlug();
	void convert(int flags);
	void addGraphicContext();
	void setupNode( const QDomElement &e );
	void setupTransform( const QDomElement &e );
	void finishNode( const QDomElement &e, PageItem* item);
	bool isIgnorableNode( const QDomElement &e );
	QSize parseWidthHeight(const QDomElement &e, double conv);
	QRect parseViewBox(const QDomElement &e);
	void parseDefs(const QDomElement &e);
	void parseClipPath(const QDomElement &e);
	void parseClipPathAttr(const QDomElement &e, FPointArray& clipPath);
	QPtrList<PageItem> parseGroup(const QDomElement &e);
	QPtrList<PageItem> parseElement(const QDomElement &e);
	QPtrList<PageItem> parseCircle(const QDomElement &e);
	QPtrList<PageItem> parseEllipse(const QDomElement &e);
	QPtrList<PageItem> parseImage(const QDomElement &e);
	QPtrList<PageItem> parseLine(const QDomElement &e);
	QPtrList<PageItem> parsePath(const QDomElement &e);
	QPtrList<PageItem> parsePolyline(const QDomElement &e);
	QPtrList<PageItem> parseRect(const QDomElement &e);
	QPtrList<PageItem> parseText(const QDomElement &e);
	QPtrList<PageItem> parseTextElement(double x, double y, const QDomElement &e);
	QPtrList<PageItem> parseSwitch(const QDomElement &e);
	QPtrList<PageItem> parseUse(const QDomElement &e);
	QDomElement getNodeFromUseElement(const QDomElement &e);
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
	QDomDocument inpdoc;
	double CurrX, CurrY, StartX, StartY, Conversion;
	int PathLen;
	QPtrStack<SvgStyle>	m_gc;
	QMap<QString, GradientHelper>	m_gradients;
	QMap<QString, QDomElement>		m_nodeMap;
	QMap<QString, FPointArray>		m_clipPaths;
	bool FirstM, WasM, PathClosed;
	double viewTransformX;
	double viewTransformY;
	double viewScaleX;
	double viewScaleY;
	bool interactive;
	//! \brief Indicator if there is any unsupported feature in imported svg.
	bool unsupported;
	bool importFailed;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
};

#endif
