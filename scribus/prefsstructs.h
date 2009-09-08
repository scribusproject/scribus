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
	int paletteFontSize; //! Font size to use in the palettes
	QString style; 	//! Currently used QStyle name
	int recentDocCount; //! Number of recent documents to remember
	QStringList RecentDocs; //! List of recent documents
	QString language; //! Language of the user interface
	bool useSmallWidgets; //! Use small widgets in the palettes
	bool showStartupDialog; //! Whether to show the startup dialog or not
	bool stickyTools; //! Whether a user's tool section remains after use or the normal tool is reselected
	bool grayscaleIcons; //! Show icons in toolbars as grayscale
	WindowPrefs mainWinSettings;
	QByteArray mainWinState;
	bool SepalT; //! Page Palette - show page thumbnails
	bool SepalN; //! Page Palette - show names on pages
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
	bool AutoSave;
	int AutoSaveTime;
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
	int magMin; //! Magnification minimum
	int magMax; //! Magnification maximum
	int magStep; //! Default step between magnification levels
	/* Item Duplicate Offset */
	double dispX;
	double dispY;
	/* Rotation Tool constrain value */
	double constrain; //! Angle to constrain rotation to when constraining is active
};

struct HyphenatorPrefs
{
	int MinWordLen; //! Minimum word length to hyphenate
	int HyCount; //! Maximum number of consecutive rows to hyphenate
	QString Language; //! Language to use for hyphenation
	QHash<QString, QString> specialWords; //! List of special words to hyphenate
	QSet<QString> ignoredWords; //! List of words the hyphenator ignores
	bool Automatic;
	bool AutoCheck;
};

struct FontPrefs
{
	SCFonts AvailFonts; //! Fonts that Scribus has available to it, or the current document has available to use
	bool askBeforeSubstitute; //! Request that the user confirms a font substituion or not
	QMap<QString,QString> GFontSub;
};

struct PrinterPrefs
{
	QString PrinterName; //! Default Printer name (source from CUPS)
	QString PrinterFile; //! File to print to
	QString PrinterCommand; //! Special printer command to use
	bool ClipMargin; //! When printing, clip the print job to the margins
	bool GCRMode;
};

struct ColorPrefs
{
	ColorList DColors;
	QString DColorSet;
	QStringList CustomColorSets;
	CMSData DCMSset;
};

struct ItemAttrPrefs
{
	ObjAttrVector defaultItemAttributes; //! The default item attributes
};

struct TOCPrefs
{
	ToCSetupVector defaultToCSetups; //! Default Tables of Contents
};

struct KeyboardShortcutsPrefs
{
	QMap<QString,Keys> KeyActions; //! Keyboard Shortcuts lists for the ScrActions
};

struct ScrapbookPrefs
{
	QStringList RecentScrapbooks; //! List of recently used scrapbooks
	int numScrapbookCopies;
	bool doCopyToScrapbook; //! When copying (copy/paste), also copy to a scrapbook
	bool persistentScrapbook; //! Keep the scrapbook contents between sessions
};

struct DisplayPrefs
{
	bool marginColored; //! Indicates if the margin to edge of page area will be colored in some other color or not
	bool showPageShadow; //! Show a shadow around the pages
	QColor paperColor; //! Color of paper (onscreen only)
	QColor frameColor; //! Color of frame border (onscreen only)
	QColor frameNormColor; //! Color of normal frame border (onscreen only)
	QColor frameGroupColor; //! Color of border of grouped frames (onscreen only)
	QColor frameLinkColor; //! Color of frame link indicators (onscreen only)
	QColor frameLockColor; //! Color of locked frame border (onscreen only)
	QColor frameAnnotationColor; //! Color of annotation frames border (onscreen only)
	QColor pageBorderColor; //! Color of page border (onscreen only)
	QColor controlCharColor; //! Color of control characters in text frames if they are shown (onscreen only)
	bool showToolTips; //! Show tool tips in the GUI or not.
	bool showMouseCoordinates; //! Show mouse coordinates when interaction with frames
	MarginStruct scratch; //! Scratch space distances
	double pageGapHorizontal; //! Horizontal gap between pages
	double pageGapVertical; //! Vertical gap between pages
	double displayScale; //! Display scale, typically used to set the scale of the display to 100% of real values.
};

struct ExternalToolsPrefs
{
	QString gs_exe; //! Location on the system of the Ghostscript interpreter (gs, gswin32c.exe, etc) incl path
	bool gs_AntiAliasText; //! Tell Ghostscript to antialias text or not
	bool gs_AntiAliasGraphics; //! Tell Ghostscript to antialias graphics or not
	int gs_Resolution; //! Resolution of Ghostscript rendered graphics
	QString imageEditorExecutable;
	QString extBrowserExecutable;
	QString uniconvExecutable;
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

struct StoryEditorPrefs
{
	QColor STEcolor;
	QString STEfont;
};

struct PrintPreviewPrefs
{
	bool PrPr_Mode;
	bool PrPr_AntiAliasing;
	bool PrPr_Transparency;
	bool PrPr_C;
	bool PrPr_M;
	bool PrPr_Y;
	bool PrPr_K;
	bool PrPr_InkCoverage;
	int PrPr_InkThreshold;
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
	ColorPrefs colorPrefs;
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
	StoryEditorPrefs storyEditorPrefs;
	PrintPreviewPrefs printPreviewPrefs;

	QList<ArrowDesc> arrowStyles;
	QList<PageSet> pageSets;
	QString curCheckProfile;


	//TODO : Remove these no longer used items
	//! System default QStyle name for current instance, seemingly unused
	QString ui_SystemTheme;
	//	bool Gcr_Mode;
};

#endif
