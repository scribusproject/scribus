#ifndef OODPLUG_H
#define OODPLUG_H

#include <qobject.h>
#include <qdict.h>
#include <qptrlist.h>
#include "pluginapi.h"
#include "scplugin.h"
#include "stylestack.h"

class PLUGIN_API OODrawImportPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		OODrawImportPlugin();
		virtual ~OODrawImportPlugin();
		virtual bool run(QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();

		// Special features (none)
};

extern "C" PLUGIN_API int oodrawimp_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* oodrawimp_getPlugin();
extern "C" PLUGIN_API void oodrawimp_freePlugin(ScPlugin* plugin);

class QWidget;
class ScribusApp;
class ScribusDoc;
class PageItem;
class FPointArray;
class QDomDocument;
class QDomElement;

class OODPlug : public QObject
{
	Q_OBJECT

public:
	OODPlug( QString fName, bool isInteractive );
	~OODPlug();
	void convert();
	QPtrList<PageItem> parseGroup(const QDomElement &e);
	void createStyleMap( QDomDocument &docstyles );
	void insertDraws( const QDomElement& styles );
	void insertStyles( const QDomElement& styles );
	void fillStyleStack( const QDomElement& object );
	void addStyles( const QDomElement* style );
	void storeObjectStyles( const QDomElement& object );
	static double parseUnit(const QString &unit);
	QColor parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	void parseTransform(FPointArray *composite, const QString &transform);
	void parseViewBox( const QDomElement& object, double *x, double *y, double *w, double *h );
	void appendPoints(FPointArray *composite, const QDomElement& object);
	const char * getCoord( const char *ptr, double &number );
	bool parseSVG( const QString &s, FPointArray *ite );
	void calculateArc(FPointArray *ite, bool relative, double &curx, double &cury, double angle, double x, double y, double r1, double r2, bool largeArcFlag, bool sweepFlag);
	void svgClosePath(FPointArray *i);
	void svgMoveTo(double x1, double y1);
	void svgLineTo(FPointArray *i, double x1, double y1);
	void svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3);

	ScribusDoc* Doku;
	QDomDocument inpContents;
	QDomDocument inpStyles;
	QDomDocument inpMeta;
	QDict<QDomElement> m_styles, m_draws;
	StyleStack m_styleStack;
	QString stylePath;
	QString contentPath;
	QString metaPath;
	double CurrX, CurrY, StartX, StartY;
	int PathLen;
	QPtrList<PageItem> Elements;
	bool FirstM, WasM, PathClosed, HaveMeta;
protected:
	bool interactive;
};

#endif
