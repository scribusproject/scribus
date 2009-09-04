/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PREFSSTRUCTS_H
#define PREFSSTRUCTS_H

#ifdef HAVE_CONFIG_H
#include "scconfig.h"
#endif

#include <QColor>
#include <QString>
#include <QList>
#include <QSet>
#include <QHash>
#include "pdfoptions.h"
#include "scfonts.h"

struct CheckerPrefs
{
	bool ignoreErrors;
	bool autoCheck;
	bool checkGlyphs;
	bool checkOverflow;
	bool checkOrphans;
	bool checkPictures;
	bool checkResolution;
	double minResolution;
	double maxResolution;
	bool checkTransparency;
	bool checkAnnotations;
	bool checkRasterPDF;
	bool checkForGIF;
	bool ignoreOffLayers;
};

typedef QMap<QString, CheckerPrefs> CheckerPrefsList;

struct TypoPrefs
{
	int valueSuperScript; //! Displacement of superscript
	int scalingSuperScript; //! Scaling of superscript
	int valueSubScript; //! Displacement of subscript
	int scalingSubScript; //! Scaling of subscript
	int valueSmallCaps; //! Scaling of small caps
	int autoLineSpacing; //! Automatic line spacing percentage
	int valueUnderlinePos; //! Underline displacement
	int valueUnderlineWidth; //! Underline width
	int valueStrikeThruPos; //! Strike-through displacement
	int valueStrikeThruWidth; //! Strike-through line width
};

struct WindowPrefs
{
	int xPosition;
	int yPosition;
	int width;
	int height;
	bool visible;
	bool docked;
	bool maximized;
};

//User Interface
struct UIPrefs
{
	int mouseMoveTimeout; //! Mouse move timeout for move/resize operations
	int wheelJump; //! Distance to jump with mouse wheel scrolling
	int applicationFontSize; //! Font size to use in the application, apart from pßalettes
	int paletteFontSize; //! Fotn size to use in the palettes
	QString style; 	//! Currently used QStyle name
	int recentDocCount; //! Number of recent documents to remember
	QStringList RecentDocs;
	QString language; //! Language of the user interface
	bool useSmallWidgets; //! Use small widgets in the palettes
	bool showStartupDialog; //! Whether to show the startup dialog or not
	bool stickyTools;
	bool grayscaleIcons;
	WindowPrefs mainWinSettings;
	QByteArray mainWinState;
};

//Paths
struct PathPrefs
{
	QString documents; //! Default document location
	QString colorProfiles; //! Default color profile location
	QString scripts; //! Default script location
	QString documentTemplates; //! Default document template location
};

//Document Setup
struct DocumentSetupPrefs
{
	QString pageSize; //! Default page size of a document
	int pageOrientation; //! Default orientation of the page
	double pageWidth; //! Width of a page
	double pageHeight; //! Height of a page
	MarginStruct margins; //! Margins for a page
	MarginStruct bleeds; //! Bleeds for a page
	int marginPreset; //! Use a preset ratio margin setup
	int pagePositioning; //! Show pages in 1,2,3,4 pages side by side on screen
	int docUnitIndex; //! The index of the default unit
};

//Guides
struct GuidesPrefs
{
	bool guidePlacement; //! Placement of guides, in front or behind

	int grabRadius;
	double guideRad;

	bool guidesShown; //! Show the guides or not
	bool marginsShown; //! Show the margins or not
	bool gridShown; //! Show the grid or not
	bool baselineGridShown; //! Show the baseline grid or not
	bool framesShown; //! Show frame borders or not
	bool colBordersShown; //! Show text frame column borders or not
	bool layerMarkersShown; //! Show a layer indicator when layers are active or not
	bool linkShown; //! Show text frame links or not
	bool rulersShown; //! Show the page rulers or not

	double majorGridSpacing; //! Major grid spacing
	double minorGridSpacing; //! Minor grid spacing
	double valueBaselineGrid; //! Baseline grid spacing
	double offsetBaselineGrid; //! Offset of first line of baseline grid from top of page

	bool showPic;
	bool showControls;
	bool showBleed;
	bool rulerMode;



	QColor guideColor; //! Color of guides
	QColor marginColor; //! Color of margins
	QColor majorGridColor; //! Color of major grid
	QColor minorGridColor; //! Color of minor grid
	QColor baselineGridColor; //! Color of baseline grid
};

struct ItemToolPrefs
{
	/* Texttool */
	QString defFont;
	int defSize;
	QString dPenText;
	int dTextPenShade;
	QString dStrokeText;
	int dTextStrokeShade;
	QString dTextBackGround;
	QString dTextLineColor;
	int dTextBackGroundShade;
	int dTextLineShade;
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
	double polyCurvature;
	/* Picture Tool */
	QString dBrushPict;
	int shadePict;
	double scaleX;
	double scaleY;
	bool scaleType;
	bool aspectRatio;
	int lowResType;
	bool useEmbeddedPath;
};

struct OperatorToolPrefs
{
	/* Magnifier Tool */
	int magMin;
	int magMax;
	int magStep;
	/* Item Duplicate Offset */
	double dispX;
	double dispY;
	/* Rotation Tool constrain value */
	double constrain;
};

struct HyphenatorPrefs
{
	int MinWordLen;
	int HyCount;
	QString Language;
	QHash<QString, QString> specialWords;
	QSet<QString> ignoredWords;
	bool Automatic;
	bool AutoCheck;
};

struct FontPrefs
{
	SCFonts AvailFonts;
	bool askBeforeSubstitute;
};

struct PrinterPrefs
{
	QString PrinterName;
	QString PrinterFile;
	QString PrinterCommand;
	bool ClipMargin;
};

struct ColorManagementPrefs
{
};

struct ItemAttrPrefs
{
	ObjAttrVector defaultItemAttributes;
};

struct TOCPrefs
{
	ToCSetupVector defaultToCSetups;
};

struct KeyboardShortcutsPrefs
{
	QMap<QString,Keys> KeyActions;
};

struct ScrapbookPrefs
{
	QStringList RecentScrapbooks;
	int numScrapbookCopies;
	bool doCopyToScrapbook;
	bool persistentScrapbook;
};

struct DisplayPrefs
{
	bool marginColored;
	bool showPageShadow;
	QColor DpapColor;
	QColor DFrameColor;
	QColor DFrameNormColor;
	QColor DFrameGroupColor;
	QColor DFrameLinkColor;
	QColor DFrameLockColor;
	QColor DFrameAnnotationColor;
	QColor DPageBorderColor;
	QColor DControlCharColor;
	bool showToolTips;
	bool showMouseCoordinates;
	MarginStruct scratch;
	double GapHorizontal;
	double GapVertical;
	double DisScale;
};

struct ExternalToolsPrefs
{
	QString gs_exe;
	bool gs_AntiAliasText;
	bool gs_AntiAliasGraphics;
	int gs_Resolution;
	QString imageEditorExecutable;
	QString extBrowserExecutable;
	QStringList latexConfigs;
	QMap<QString, QString> latexCommands;
	QString latexEditorExecutable;
	int latexResolution;
	bool latexForceDpi;
	bool latexStartWithEmptyFrames;
};

struct MiscellaneousPrefs
{
	bool haveStylePreview;

	// lorem ipsum
	bool useStandardLI;
	int paragraphsLI;
};

struct PluginPrefs
{
};

struct ShortWordPrefs
{
};

struct ScripterPrefs
{
};

struct ApplicationPrefs
{
	UIPrefs uiPrefs;
	PathPrefs pathPrefs;
	DocumentSetupPrefs docSetupPrefs;
	GuidesPrefs guidesPrefs;
	TypoPrefs typoPrefs;
	ItemToolPrefs itemToolPrefs;
	OperatorToolPrefs opToolPrefs;
	HyphenatorPrefs hyphPrefs;
	FontPrefs fontPrefs;
	PrinterPrefs printerPrefs;
	ColorManagementPrefs colorMgmtPrefs;
	PDFOptions pdfPrefs;
	ItemAttrPrefs itemAttrPrefs;
	TOCPrefs tocPrefs;
	KeyboardShortcutsPrefs keyShortcutPrefs;
	ScrapbookPrefs scrapbookPrefs;
	DisplayPrefs displayPrefs;
	ExternalToolsPrefs extToolPrefs;
	MiscellaneousPrefs miscPrefs;
	PluginPrefs pluginPrefs;
	ShortWordPrefs shortwordPrefs;
	ScripterPrefs scripterPrefs;
	CheckerPrefsList checkerPrefsList;


	// Remaining to sort out
	QList<ArrowDesc> arrowStyles;
	QList<PageSet> pageSets;
	ColorList DColors;
	QString curCheckProfile;
	bool SepalT;
	bool SepalN;
	CMSData DCMSset;
	QMap<QString,QString> GFontSub;
	bool AutoSave;
	int AutoSaveTime;
	QString DColorSet;
	QStringList CustomColorSets;
	bool PrPr_Mode;
	bool Gcr_Mode;
	bool PrPr_AntiAliasing;
	bool PrPr_Transparency;
	bool PrPr_C;
	bool PrPr_M;
	bool PrPr_Y;
	bool PrPr_K;
	bool PrPr_InkCoverage;
	int PrPr_InkThreshold;
	bool GCRMode;
	QColor STEcolor;
	QString STEfont;




	//TODO : Remove these no longer used items
	//! System default QStyle name for current instance, seemingly unused
	QString ui_SystemTheme;
};

#endif
