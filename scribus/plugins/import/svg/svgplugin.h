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
		bool cspace {false};
		bool cspaceValid {true};
		VGradient gradient {VGradient::linear};
		bool gradientValid {false};
		QTransform matrix;
		bool matrixValid {false};
		QString reference;
		int type {1};
		bool typeValid {false};
		double x1 {0.0};
		double x2 {0.0};
		double y1 {0.0};
		double y2 {0.0};
		double fx {0.0};
		double fy {0.0};
		bool x1Valid {true};
		bool x2Valid {true};
		bool y1Valid {true};
		bool y2Valid {true};
		bool fxValid {true};
		bool fyValid {true};
};

class SvgStyle
{
	public:
		QString stylename;
		bool Display {true};
		bool FillCSpace {false};
		bool StrokeCSpace {false};
		QString CurCol {"Black"};
		QVector<double> dashArray;
		double dashOffset {0.0};
		QString FontFamily;
		QString FontStyle {"normal"};
		QString FontWeight {"normal"};
		QString FontStretch {"normal"};
		double FontSize {12.0};
		QString FillCol {"Black"};
		QString fillRule {"nonzero"};
		QString GFillCol1 {"Black"};
		QString GStrokeCol1 {"Black"};
		VGradient FillGradient {VGradient::linear};
		VGradient StrokeGradient {VGradient::linear};
		int FillGradientType {0};
		int StrokeGradientType {0};
		double GradFillX1 {0.0};
		double GradFillX2 {0.0};
		double GradFillY1 {0.0};
		double GradFillY2 {0.0};
		double GradFillFX {0.0};
		double GradFillFY {0.0};
		double GradStrokeX1 {0.0};
		double GradStrokeX2 {0.0};
		double GradStrokeY1 {0.0};
		double GradStrokeY2 {0.0};
		double GradStrokeFX {0.0};
		double GradStrokeFY {0.0};
		bool InherCol {false};
		double LWidth {1.0};
		QTransform matrix;
		QTransform matrixgf;
		QTransform matrixgs;
		Qt::PenStyle PLineArt {Qt::SolidLine};
		Qt::PenCapStyle PLineEnd {Qt::FlatCap};
		Qt::PenJoinStyle PLineJoin {Qt::MiterJoin};
		QString StrokeCol {"None"};
		double Opacity {1.0};
		double FillOpacity {1.0};
		double StrokeOpacity {1.0};
		QString textAnchor {"start"};
		QString textDecoration;
		FPointArray clipPath;
		QString filter;
		QString endMarker;
		QString startMarker;
};

struct CSSStyle
{
	QString fillColor;
	QString strokeColor;
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
		void setupNode(const QDomElement &e);
		void setupTransform(const QDomElement &e);
		PageItem* finishNode( const QDomNode &e, PageItem* item);
		bool isIgnorableNode( const QDomElement &e ) const;
		bool isIgnorableNodeName( const QString &n ) const;
		FPoint parseTextPosition(const QDomElement &e, const FPoint* pos = nullptr);
		QSizeF parseWidthHeight(const QDomElement &e);
		QRectF parseViewBox(const QDomElement &e);
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
		QFont getFontFromStyle(const SvgStyle& style) const;
		QDomElement getReferencedNode(const QDomElement &e) const;
		bool getTextChunkWidth(const QDomElement &e, double& width);
		double fromPercentage(const QString &s ) const;
		double parseFontSize(const QString& fsize) const;
		double parseUnit(const QString &unit) const;
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
		void parseGradient(const QDomElement &e);

		QMap<QString, QDomElement> buildNodeMap(const QDomElement &e) const;

		QDomDocument inpdoc;
		QString docDesc;
		QString docTitle;
		int groupLevel { 0 };
		QStack<SvgStyle*> m_gc;
		QMap<QString, GradientHelper> m_gradients;
		QMap<QString, QDomElement> m_nodeMap;
		QMap<QString, FPointArray> m_clipPaths;
		QMap<QString, QString> m_unsupportedFeatures;

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
		double inGroupXOrigin { 0.0 };
		double inGroupYOrigin { 0.0 };
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
		QMap<QString, CSSStyle> cssStyleList;

	protected:
		QVector<double> parseNumbersList(const QString& numbersStr) const;
};

#endif
