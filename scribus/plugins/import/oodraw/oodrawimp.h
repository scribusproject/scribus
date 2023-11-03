/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef OODPLUG_H
#define OODPLUG_H

#include "commonstrings.h"
#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "../../formatidlist.h"
#include "stylestack.h"
#include "scribusstructs.h"

class PageItem;
class ScrAction;
class ScribusDoc;
class Selection;
class TransactionSettings;

class PLUGIN_API OODrawImportPlugin : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		OODrawImportPlugin();
		virtual ~OODrawImportPlugin();
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		bool fileSupported(QIODevice* file, const QString& fileName = QString()) const override;
		bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0) override;
		QImage readThumbnail(const QString& fileName) override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};

		// Special features - File->Import slot
	public slots:
		virtual bool import(QString target = QString(), int flags = lfUseCurrentPage|lfInteractive);

	private:
		void registerFormats();
		ScrAction* importAction;
};

extern "C" PLUGIN_API int oodrawimp_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* oodrawimp_getPlugin();
extern "C" PLUGIN_API void oodrawimp_freePlugin(ScPlugin* plugin);

class QWidget;
class ScribusMainWindow;
class ScribusDoc;
class FPointArray;
class QDomDocument;
class QDomElement;

class OODrawStyle
{
public:
	OODrawStyle() = default;

	QString fillColor { CommonStrings::None };
	QString strokeColor { CommonStrings::None };
	double  fillTrans { 0.0 };
	double  strokeTrans { 0.0 };
	double  strokeWidth { 0.0 };
	QVector<double> dashes;
	bool    haveGradient { false };
	int     gradientType { 0 };
	VGradient gradient;
	double  gradientAngle { 0.0 };
	double  gradientPointX { 0.0 };
	double  gradientPointY { 0.0 };
};

class OODPlug : public QObject
{
	Q_OBJECT

public:

	OODPlug(ScribusDoc* doc);
	~OODPlug();

	//! \brief Indicator if there is any unsupported feature in imported svg.
	bool unsupported { false };
	bool importFailed { false };
	bool importCanceled { true };

	bool import(const QString& fName, const TransactionSettings& trSettings, int flags);
	QImage readThumbnail(const QString& fileName );
	static double parseUnit(const QString &unit);

protected:

	bool convert(const TransactionSettings& trSettings, int flags);
	QList<PageItem*> parseGroup(const QDomElement &e);
	QList<PageItem*> parseElement(const QDomElement &e);
	QList<PageItem*> parseRect(const QDomElement &e);
	QList<PageItem*> parseEllipse(const QDomElement &e);
	QList<PageItem*> parseLine(const QDomElement &e);
	QList<PageItem*> parsePolygon(const QDomElement &e);
	QList<PageItem*> parsePolyline(const QDomElement &e);
	QList<PageItem*> parsePath(const QDomElement &e);
	QList<PageItem*> parseTextBox(const QDomElement &e);
	QList<PageItem*> parseFrame(const QDomElement &e);
	QList<PageItem*> parseConnector(const QDomElement &e);
	void parseStyle(OODrawStyle& style, const QDomElement &e);
	void parseCharStyle(CharStyle& style, const QDomElement &e);
	void parseParagraphStyle(ParagraphStyle& style, const QDomElement &e);
	PageItem* parseTextP(const QDomElement& e, PageItem* item);
	PageItem* parseTextSpans(const QDomElement& elm, PageItem* item);
	PageItem* finishNodeParsing(const QDomElement &elm, PageItem* item, OODrawStyle& oostyle);
	void createStyleMap( QDomDocument &docstyles );
	void insertDraws( const QDomElement& styles );
	void insertStyles( const QDomElement& styles );
	void fillStyleStack( const QDomElement& object );
	void addStyles( const QDomElement* style );
	void storeObjectStyles( const QDomElement& object );
	QString parseColor( const QString &s );
	void parseTransform(FPointArray *composite, const QString &transform);
	void parseViewBox( const QDomElement& object, double *x, double *y, double *w, double *h );
	void appendPoints(FPointArray *composite, const QDomElement& object, bool closePath);
	const char * getCoord( const char *ptr, double &number );
	bool parseSVG( const QString &s, FPointArray *ite );
	void calculateArc(FPointArray *ite, bool relative, double &curx, double &cury, double angle, double x, double y, double r1, double r2, bool largeArcFlag, bool sweepFlag);
	void svgClosePath(FPointArray *i);
	void svgMoveTo(double x1, double y1);
	void svgLineTo(FPointArray *i, double x1, double y1);
	void svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3);

	QDomDocument inpContents;
	QDomDocument inpStyles;
	QDomDocument inpMeta;
	QHash<QString, QDomElement*> m_styles;
	QHash<QString, QDomElement*> m_draws;
	StyleStack m_styleStack;
	double CurrX { 0.0 };
	double CurrY { 0.0 };
	double StartX { 0.0 };
	double StartY { 0.0 };
	int PathLen { 0 };
	bool FirstM { true };
	bool WasM { false };
	bool PathClosed { false };
	bool HaveMeta { false };

	bool interactive { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	QStringList importedColors;
};

#endif
