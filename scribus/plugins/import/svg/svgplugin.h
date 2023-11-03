/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SVGPLUG_H
#define SVGPLUG_H

#include <QDomElement>
#include <QFont>
#include <QList>
#include <QRectF>
#include <QSizeF>
#include <QStack>
#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "../../formatidlist.h"
#include "vgradient.h"

class ScrAction;
class ScribusMainWindow;
class TransactionSettings;

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
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		bool fileSupported(QIODevice* file, const QString& fileName = QString()) const override;
		bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0) override;
		QImage readThumbnail(const QString& fileName) override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};

	public slots:
		/*!
		\author Franz Schmid
		\brief Run the SVG import
		\param filename a file name to import
		\retval true for success
		 */
		virtual bool import(QString filename = QString(), int flags = lfUseCurrentPage|lfInteractive);

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
		cspace(false),
		cspaceValid(true),
		gradient(VGradient::linear),
		gradientValid(false),
		matrix(),
		matrixValid(false),
		reference(""),
		type(1),
		typeValid(false),
		x1(0),
		x1Valid(true),
		x2(1),
		x2Valid(true),
		y1(0),
		y1Valid(true),
		y2(0),
		y2Valid(true),
		fx(0),
		fxValid(true),
		fy(0),
		fyValid(true)
		{
		}
	bool cspace;
	bool cspaceValid;
	VGradient gradient;
	bool gradientValid;
	QTransform matrix;
	bool matrixValid;
	QString reference;
	int type;
	bool typeValid;
	double x1;
	bool x1Valid;
	double x2;
	bool x2Valid;
	double y1;
	bool y1Valid;
	double y2;
	bool y2Valid;
	double fx;
	bool fxValid;
	double fy;
	bool fyValid;
	};

class SvgStyle
{
public:
	SvgStyle() :
		Display(true),
		FillCSpace(false),
		StrokeCSpace(false),
		CurCol("Black"),
		dashOffset(0),
		FontFamily(""),
		FontStyle("normal"),
		FontWeight("normal"),
		FontStretch("normal"),
		FontSize(12),
		FillCol("Black"),
		fillRule("nonzero"),
		GFillCol1("Black"),
		GStrokeCol1("Black"),
		FillGradient(VGradient::linear),
		StrokeGradient(VGradient::linear),
		FillGradientType(0),
		StrokeGradientType(0),
		GradFillX1(0),
		GradFillX2(0),
		GradFillY1(0),
		GradFillY2(0),
		GradFillFX(0),
		GradFillFY(0),
		GradStrokeX1(0),
		GradStrokeX2(0),
		GradStrokeY1(0),
		GradStrokeY2(0),
		GradStrokeFX(0),
		GradStrokeFY(0),
		InherCol(false),
		LWidth(1.0),
		matrix(),
		matrixgf(),
		matrixgs(),
		PLineArt(Qt::SolidLine),
		PLineEnd(Qt::FlatCap),
		PLineJoin(Qt::MiterJoin),
		StrokeCol("None"),
		Opacity(1.0),
		FillOpacity(1.0),
		StrokeOpacity(1.0),
		textAnchor("start"),
		clipPath(),
		endMarker(""),
		startMarker("")
		{
		}
	bool Display;
	bool FillCSpace;
	bool StrokeCSpace;
	QString CurCol;
	QVector<double> dashArray;
	double dashOffset;
	QString FontFamily;
	QString FontStyle;
	QString FontWeight;
	QString FontStretch;
	double  FontSize;
	QString FillCol;
	QString fillRule;
	QString GFillCol1;
	QString GStrokeCol1;
	VGradient FillGradient;
	VGradient StrokeGradient;
	int    FillGradientType;
	int    StrokeGradientType;
	double GradFillX1;
	double GradFillX2;
	double GradFillY1;
	double GradFillY2;
	double GradFillFX;
	double GradFillFY;
	double GradStrokeX1;
	double GradStrokeX2;
	double GradStrokeY1;
	double GradStrokeY2;
	double GradStrokeFX;
	double GradStrokeFY;
	bool InherCol;
	double LWidth;
	QTransform matrix;
	QTransform matrixgf;
	QTransform matrixgs;
	Qt::PenStyle PLineArt;
	Qt::PenCapStyle PLineEnd;
	Qt::PenJoinStyle PLineJoin;
	QString StrokeCol;
	double Opacity;
	double FillOpacity;
	double StrokeOpacity;
	QString textAnchor;
	QString textDecoration;
	FPointArray clipPath;
	QString filter;
	QString endMarker;
	QString startMarker;
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
	SVGPlug(ScribusDoc* doc, int flags);
	~SVGPlug();

	bool import(const QString& fname, const TransactionSettings& trSettings, int flags);
	QImage readThumbnail(const QString& fn);
	bool loadData(const QString& fname);
	void convert(const TransactionSettings& trSettings, int flags);
	void addGraphicContext();
	void setupNode( const QDomElement &e );
	void setupTransform( const QDomElement &e );
	PageItem* finishNode( const QDomNode &e, PageItem* item);
	bool isIgnorableNode( const QDomElement &e ) const;
	bool isIgnorableNodeName( const QString &n ) const;
	FPoint parseTextPosition(const QDomElement &e, const FPoint* pos = nullptr);
	QSizeF  parseWidthHeight(const QDomElement &e);
	QRectF  parseViewBox(const QDomElement &e);
	void parseDefs(const QDomElement &e);
	void parseClipPath(const QDomElement &e);
	void parseClipPathAttr(const QDomElement &e, FPointArray& clipPath) const;
	void parseFilterAttr(const QDomElement &e, PageItem* item) const;
	QList<PageItem*> parseA(const QDomElement &e);
	QList<PageItem*> parseGroup(const QDomElement &e);
	QList<PageItem*> parseDoc(const QDomElement &e);
	QList<PageItem*> parseElement(const QDomElement &e);
	QList<PageItem*> parseCircle(const QDomElement &e);
	QList<PageItem*> parseEllipse(const QDomElement &e);
	QList<PageItem*> parseImage(const QDomElement &e);
	QList<PageItem*> parseLine(const QDomElement &e);
	QList<PageItem*> parsePath(const QDomElement &e);
	QList<PageItem*> parsePolyline(const QDomElement &e);
	QList<PageItem*> parseRect(const QDomElement &e);
	QList<PageItem*> parseText(const QDomElement &e);
	QList<PageItem*> parseTextSpan(const QDomElement& e, FPoint& currentPos, double chunkW);
	QList<PageItem*> parseTextNode(const QDomText& e, FPoint& currentPos, double chunkW);
	QList<PageItem*> parseSwitch(const QDomElement &e);
	QList<PageItem*> parseSymbol(const QDomElement &e);
	QList<PageItem*> parseUse(const QDomElement &e);
	const char* getCoord( const char *ptr, double &number) const;
	QFont       getFontFromStyle(const SvgStyle& style) const;
	QDomElement getReferencedNode(const QDomElement &e) const;
	bool        getTextChunkWidth(const QDomElement &e, double& width);
	double  fromPercentage(const QString &s ) const;
	double  parseFontSize(const QString& fsize) const;
	double  parseUnit(const QString &unit) const;
	QTransform parseTransform(const QString &transform) const;
	QString parseColor( const QString &s );
	QString parseIccColor( const QString &s );
	QString parseTagName( const QDomElement &e) const;
	void parsePA( SvgStyle *obj, const QString &command, const QString &params );
	void parseStyle( SvgStyle *obj, const QDomElement &e );
	void parseColorStops(GradientHelper *gradient, const QDomElement &e);
	void parseFilter(const QDomElement &b);
	void parseMarker(const QDomElement &b);
	void parsePattern(const QDomElement &b);
	void parseGradient( const QDomElement &e );

	QMap<QString, QDomElement> buildNodeMap(const QDomElement &e) const;

	QDomDocument inpdoc;
	QString docDesc;
	QString docTitle;
	int groupLevel { 0 };
	QStack<SvgStyle*>	m_gc;
	QMap<QString, GradientHelper>	m_gradients;
	QMap<QString, QDomElement>		m_nodeMap;
	QMap<QString, FPointArray>		m_clipPaths;
	QMap<QString, QString>			m_unsupportedFeatures;

	double viewTransformX { 0.0 };
	double viewTransformY { 0.0 };
	double viewScaleX { 1.0 };
	double viewScaleY { 1.0 };
	bool interactive { false };
	//! \brief Indicator if there is any unsupported feature in imported svg.
	bool unsupported { false };
	bool importFailed { false };
	bool importCanceled { true };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	QStringList importedColors;
	QStringList importedGradients;
	QMap<QString, QString> importedGradTrans;
	QStringList importedPatterns;
	QMap<QString, QString> importedPattTrans;
	double inGroupXOrigin { 0 };
	double inGroupYOrigin { 0 };
	int importerFlags { 0 };
	bool firstLayer { true };
	struct filterSpec
	{
		int blendMode;
	};
	struct markerDesc
	{
		double xref;
		double yref;
		double wpat;
		double hpat;
	};
	QMap<QString, filterSpec> filters;
	QMap<QString, markerDesc> markers;
	QList<PageItem*> Elements;

protected:
	QVector<double> parseNumbersList(const QString& numbersStr) const;
};

#endif
