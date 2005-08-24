#ifndef PREFSSTRUCTS_H
#define PREFSSTRUCTS_H

#ifdef HAVE_CONFIG_H
#include "scconfig.h"
#endif

#include <qcolor.h>
#include <qstring.h>
#include "scribusstructs.h"
#include "pdfoptions.h"
#include "scfonts.h"

struct checkerPrefs
{
	bool ignoreErrors;
	bool autoCheck;
	bool checkGlyphs;
	bool checkOverflow;
	bool checkOrphans;
	bool checkPictures;
	bool checkResolution;
	double minResolution;
	bool checkTransparency;
	bool checkAnnotations;
	bool checkRasterPDF;
};

//TODO Move to its own file
class CheckerPrefsList : public QMap<QString, checkerPrefs>
{

	public:
	void initDefaults()
	{
		struct checkerPrefs checkerSettings;
		checkerSettings.ignoreErrors = false;
		checkerSettings.autoCheck = true;
		checkerSettings.checkGlyphs = true;
		checkerSettings.checkOrphans = true;
		checkerSettings.checkOverflow = true;
		checkerSettings.checkPictures = true;
		checkerSettings.checkResolution = true;
		checkerSettings.checkTransparency = true;
		checkerSettings.checkAnnotations = false;
		checkerSettings.checkRasterPDF = true;
		checkerSettings.minResolution = 72.0;
		//TODO Stop translating these into settings!!!!!!!!!
		insert( QT_TR_NOOP("Postscript"), checkerSettings);
		insert( QT_TR_NOOP("PDF 1.3"), checkerSettings);
		checkerSettings.checkTransparency = false;
		insert( QT_TR_NOOP("PDF 1.4"), checkerSettings);
		checkerSettings.checkTransparency = true;
		checkerSettings.checkAnnotations = true;
		checkerSettings.minResolution = 144.0;
		insert( QT_TR_NOOP("PDF/X-3"), checkerSettings);
	}
};

struct typoPrefs
{
	int valueSuperScript;
	int scalingSuperScript;
	int valueSubScript;
	int scalingSubScript;
	int valueSmallCaps;
	int autoLineSpacing;
	int valueUnderlinePos;
	int valueUnderlineWidth;
	int valueStrikeThruPos;
	int valueStrikeThruWidth;
	double valueBaseGrid;
	double offsetBaseGrid;
};

struct windowPrefs
{
	int xPosition;
	int yPosition;
	int width;
	int height;
	bool visible;
	bool docked;
};

struct guidesPrefs
{
	bool gridShown;
	QColor majorColor;
	double majorGrid;
	double minorGrid;
	QColor minorColor;
	double guideRad;
	int grabRad;
	bool framesShown;
	bool guidesShown;
	bool marginsShown;
	bool baseShown;
	bool linkShown;
	bool showPic;
	bool showControls;
	bool rulerMode;
	bool before;
	QColor guideColor;
	QColor baseColor;
	QColor margColor;
};

struct toolPrefs
{
	/* Texttool */
	QString defFont;
	int defSize;
	QString dPenText;
	QString dStrokeText;
	int dCols;
	double dGap;
	QString tabFillChar;
	double dTabWidth;
	/* ShapeTool */
	QString dPen;
	QString dBrush;
	int dShade;
	int dShade2;
	int dLineArt;
	double dWidth;
	/* Line Tool */
	QString dPenLine;
	int dShadeLine;
	double dWidthLine;
	int dLstyleLine;
	int dStartArrow;
	int dEndArrow;
	/* Regular Polygon Tool */
	int polyC;
	int polyFd;
	double polyF;
	bool polyS;
	double polyR;
	/* Picture Tool */
	QString dBrushPict;
	int shadePict;
	double scaleX;
	double scaleY;
	bool scaleType;
	bool aspectRatio;
	int lowResType;
	bool useEmbeddedPath;
	/* Magnifier Tool */
	int magMin;
	int magMax;
	int magStep;
};

struct ApplicationPrefs
{
	SCFonts AvailFonts;
	QValueList<ArrowDesc> arrowStyles;
	QValueList<PageSet> pageSets;
	ColorList DColors;
	int Wheelval;
	int AppFontSize;
	QColor DpapColor;
	QString GUI;
	guidesPrefs guidesSettings;
	typoPrefs typographicSettings;
	toolPrefs toolSettings;
	CheckerPrefsList checkerProfiles;
	QString curCheckProfile;
	int docUnitIndex;
	windowPrefs mainToolBarSettings;
	windowPrefs pdfToolBarSettings;
	windowPrefs mainWinSettings;

	PDFOptions PDF_Options;
	ObjAttrVector defaultItemAttributes;
	ToCSetupVector defaultToCSetups;
	bool SepalT;
	bool SepalN;
	int PSize;
	bool SaveAtQ;
	bool marginColored;
	QStringList RecentDocs;
	uint RecentDCount;
	CMSData DCMSset;
	QMap<QString,Keys> KeyActions;
	QString PrinterName;
	QString PrinterFile;
	QString PrinterCommand;
	QString pageSize;
	int pageOrientation;
	double PageWidth;
	double PageHeight;
	double RandOben;
	double RandUnten;
	double RandLinks;
	double RandRechts;
	int FacingPages;
	int MinWordLen;
	int HyCount;
	QString Language;
	QString guiLanguage;
	bool Automatic;
	bool AutoCheck;
	QString DocDir;
	QString ProfileDir;
	QString ScriptDir;
	QString documentTemplatesDir;
	QMap<QString,QString> GFontSub;
	bool AutoSave;
	int AutoSaveTime;
	double DisScale;
	QString DColorSet;
	QStringList CustomColorSets;
	bool PrPr_Mode;
	bool Gcr_Mode;
	bool PrPr_AlphaText;
	bool PrPr_AlphaGraphics;
	bool PrPr_Transparency;
	bool PrPr_C;
	bool PrPr_M;
	bool PrPr_Y;
	bool PrPr_K;
	QString gs_exe;
	bool gs_AntiAliasText;
	bool gs_AntiAliasGraphics;
	int gs_Resolution;
	QString imageEditorExecutable;
	bool ClipMargin;
	bool GCRMode;
	QColor STEcolor;
	QString STEfont;
	double ScratchLeft;
	double ScratchRight;
	double ScratchTop;
	double ScratchBottom;
	bool askBeforeSubstituite;
	bool haveStylePreview;
	// lorem ipsum
	bool useStandardLI;
	int paragraphsLI;
	bool showStartupDialog;
};

#endif
