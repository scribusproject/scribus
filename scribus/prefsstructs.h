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

#include <cstring>

#include <QColor>
#include <QString>
#include <QList>
#include <QSet>
#include <QHash>

#include "documentinformation.h"
#include "pagestructs.h"
#include "pdfoptions.h"
#include "scfonts.h"
#include "scpattern.h"
#include "vgradient.h"

struct CheckerPrefs
{
	CheckerPrefs() { 
		memset(this, 0, sizeof(CheckerPrefs));
	}

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
	bool checkOffConflictLayers; //Check whether layers are marked as visible but not to be printed or vice versa
	bool checkNotCMYKOrSpot; // colors must be either CMYK or spot (PDF/X-1a)
	bool checkDeviceColorsAndOutputIntent; // unmanaged colors (device colors) must agree with output intend
	bool checkFontNotEmbedded; // embedded PDF might use fonts without embedding
	bool checkFontIsOpenType; // embedded PDF might use OpenType font program (only allowed in PDF/X-4 and PDF 1.6)
	bool checkPartFilledImageFrames;
	bool checkOppositePageMaster;
	bool checkAppliedMasterDifferentSide;
	bool checkEmptyTextFrames;
};

typedef QMap<QString, CheckerPrefs> CheckerPrefsList;

struct VerifierPrefs
{
	CheckerPrefsList checkerPrefsList;
	bool showPagesWithoutErrors;
	bool showNonPrintingLayerErrors;
	QString curCheckProfile;
};

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

	inline bool operator==(const TypoPrefs &other) const
	{
		return (memcmp(this, &other, sizeof(TypoPrefs)) == 0);
	}
	inline bool operator!=(const TypoPrefs &other) const
	{
		return (memcmp(this, &other, sizeof(TypoPrefs)) != 0);
	}
};

struct WindowPrefs
{
	int xPosition {0};
	int yPosition {0};
	int width {640};
	int height {480};
	bool visible {true};
	bool docked {false};
	bool maximized {false};
	int screenNumber {0};
};

struct tabPrefs
{
	int activeTab;
	QStringList palettes;
};

//User Interface
struct UIPrefs
{
	int mouseMoveTimeout {150}; //! Mouse move timeout for move/resize operations
	int wheelJump {40}; //! Distance to jump with mouse wheel scrolling
	int applicationFontSize {12}; //! Font size to use in the application, apart from pßalettes
	int paletteFontSize {10}; //! Font size to use in the palettes
	QString style; 	//! Currently used QStyle name
	int recentDocCount {5}; //! Number of recent documents to remember
	QStringList RecentDocs; //! List of recent documents
	QString language; //! Language of the user interface
	QString userPreferredLocale; //! System or interface language number formats
	bool useSmallWidgets {false}; //! Use small widgets in the palettes
	bool useTabs {false}; //! Use a tabbed MainWidget a la FireFox
	bool showStartupDialog {true}; //! Whether to show the startup dialog or not
	bool showSplashOnStartup {true}; //! Whether to show the splashscreen or not
	bool stickyTools {false}; //! Whether a user's tool section remains after use or the normal tool is reselected
	bool grayscaleIcons {false}; //! Show icons in toolbars as grayscale
	QString iconSet; //! Icon set name
	WindowPrefs mainWinSettings;
	QByteArray mainWinState;
	QList<tabPrefs> tabbedPalettes;
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
	QString language; //! Default language of a document
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
	int AutoSaveCount;
	bool AutoSaveKeep;
	bool AutoSaveLocation;
	QString AutoSaveDir;
	bool saveCompressed;
};

//Guides
struct GuidesPrefs
{
	QList<int> renderStackOrder;
	// render stack id 0 = PageMargins
	// render stack id 1 = BaselineGrid
	// render stack id 2 = Grid
	// render stack id 3 = Guides
	// render stack id 4 = Items
	int gridType;
	// type 0 lines
	// type 1 crosses and dots

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
	QString textFont; //! Default font for text frames, should be default font for default style
	int textSize; //! Default font size
	QString textColor; //! Default text color
	int textShade; //! Default text color shade
	QString textStrokeColor; //! Default text stroke color
	int textStrokeShade; //! Default text stroke color shade
	QString textFillColor; //! Default text frame fill color
	QString textLineColor; //! Default text frame line color
	int textFillColorShade; //! Default text frame fill shade
	int textLineColorShade; //! Default text frame line shade
	int textColumns; //! Default number of columns in a text frame
	double textColumnGap; //! Default gap between columns of a text frame
	QString textTabFillChar; //! Default tab fill character
	MarginStruct textDistances; //! Default text to frame distances
	double textTabWidth; //! Default tab width
	/* ShapeTool */
	QString shapeLineColor; //! Default shape line color
	QString shapeFillColor; //! Default shape fill color
	int shapeFillColorShade; //! Default shape fill color shade
	int shapeLineColorShade; //! Default shape line color shade
	int shapeLineStyle; //! Line style of shapes
	double shapeLineWidth; //! Line width of shape
	/* Line Tool */
	QString lineColor; //! Color of a line
	int lineColorShade; //! Shade of line color
	double lineWidth; //! Width of line
	int lineStyle; //! Style of line
	int lineStartArrow; //! Starting arrow, 0 = none
	int lineEndArrow; //! Ending arrow, 0 = none
	/* Regular Polygon Tool */
	int polyCorners;  //! Number of corners for a polygon
	double polyFactor; //! Convex/Concave factor as a double
	bool polyUseFactor; //! Whether to use a factor for shaping
	double polyRotation; //! Rotation of a polygon
	double polyInnerRot; //! Rotation of the inner points of a star
	double polyCurvature; //! Curvature of polygon
	double polyOuterCurvature; //! outer Curvature of polygon
	/* Image Tool */
	QString imageFillColor; //! Default fill color of an image frame
	int imageFillColorShade; //! Default shade of fill color of an image grame
	QString imageStrokeColor; //! Default stroke color of an image frame
	int imageStrokeColorShade; //! Default shade of stroke color of an image grame
	double imageScaleX; //! X scale of an image within an image frame
	double imageScaleY; //! Y scale of an image within an image frame
	bool imageScaleType; //! Scale type of image
	bool imageAspectRatio; //! Use stored aspect ratio for the image
	int imageLowResType; //! Preview type for an image frame
	bool imageUseEmbeddedPath; //! Use embedded path, eg from an EPS etc.
	/* Calligraphic Pen Tool */
	QString calligraphicPenFillColor; //! Default calligrapicPen fill color
	QString calligraphicPenLineColor; //! Default calligrapicPen line color
	int calligraphicPenFillColorShade; //! Default calligrapicPen fill shade
	int calligraphicPenLineColorShade; //! Default calligrapicPen line shade
	double calligraphicPenLineWidth; //! Width of line
	double calligraphicPenAngle;		//! Angle of the calligraphics Brush
	double calligraphicPenWidth;		//! Width of the calligraphics Brush
	int calligraphicPenStyle; //! Line style of the calligraphics Brush
	/* Arc Tool */
	double arcStartAngle;	//! angle where the arc starts
	double arcSweepAngle;	//! angle the arc spans
	/* Spiral Tool */
	double spiralStartAngle;	//! angle where the spiral starts
	double spiralEndAngle;	//! angle where the spiral ends
	double spiralFactor;	//! factor the spiral gets smaller
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
	bool writePreviews;		//! Write previews to the scrapbook dir;
};

struct DisplayPrefs
{
	bool marginColored; //! Indicates if the margin to edge of page area will be colored in some other color or not
	bool showPageShadow; //! Show a shadow around the pages
	QColor paperColor; //! Color of paper (onscreen only)
	QColor scratchColor; //! Color of scratch space (onscreen only)
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
	bool showVerifierWarningsOnCanvas; //! Show preflight verifier warnings on canvas
	bool showAutosaveClockOnCanvas; //! Show autosave countdown on canvas
};

struct ExternalToolsPrefs
{
	QString gs_exe; //! Location on the system of the Ghostscript interpreter (gs, gswin32c.exe, etc) incl path
	bool gs_AntiAliasText; //! Tell Ghostscript to antialias text or not
	bool gs_AntiAliasGraphics; //! Tell Ghostscript to antialias graphics or not
	int gs_Resolution; //! Resolution of Ghostscript rendered graphics
	QString imageEditorExecutable; //! Editor for graphics, defaults to Gimp
	QString extBrowserExecutable; //! External browser for launching URLs in
	QString uniconvExecutable; //! Uniconverter executable location
	QStringList latexConfigs;
	QMap<QString, QString> latexCommands;
	QString latexEditorExecutable; //! LaTeX executable location
	int latexResolution; //! LaTeX export resolution
	bool latexForceDpi;
	bool latexStartWithEmptyFrames;
	QString pdfViewerExecutable; //! Viewer for PDF files
};

struct MiscellaneousPrefs
{
	bool haveStylePreview; //! Show previews in the Style setup areas like Style Manager
	bool saveEmergencyFile; //! true = try to save emergency files when crashing

	// lorem ipsum
	bool useStandardLI; //! Use the standard Lorem Ipsum text
	int paragraphsLI; //! Number of paragraphs to insert with Lorem Ipsum text
};

struct StoryEditorPrefs
{
	QColor guiFontColorBackground; //! Color of the background for text used in the Story Editor window
	QString guiFont; //! Font of the text used in the Story Editor window
	bool smartTextSelection; //! Use smart text selection (relates to spacing mostly)
};

struct PrintPreviewPrefs
{
	bool PrPr_Mode { false };
	bool PrPr_AntiAliasing { true };
	bool PrPr_Transparency { false };
	bool PrPr_C { true };
	bool PrPr_M { true };
	bool PrPr_Y { true };
	bool PrPr_K { true };
	bool PrPr_InkCoverage { false };
	int PrPr_InkThreshold { 250 };
};

struct PDFOutputPreviewPrefs
{
	bool enableAntiAliasing { true };
	bool showTransparency { false };
	bool cmykPreviewMode { false };
	bool isCyanVisible { true };
	bool isMagentaVisible { true };
	bool isYellowVisible { true };
	bool isBlackVisible { true };
	bool displayInkCoverage { false };
	int  inkCoverageThreshold { 250 };
};

struct PSOutputPreviewPrefs
{
	int  psLevel { 3 };
	bool enableAntiAliasing { true };
	bool showTransparency { false };
	bool cmykPreviewMode { false };
	bool isCyanVisible { true };
	bool isMagentaVisible { true };
	bool isYellowVisible { true };
	bool isBlackVisible { true };
	bool displayInkCoverage { false };
	int  inkCoverageThreshold { 250 };
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

// Image Cache
struct ImageCachePrefs
{
	bool cacheEnabled;	//!< Enable the image cache
	int maxCacheSizeMiB;  //!< Maximum total size of image cache in MiB
	int maxCacheEntries;  //!< Maximum number of cache entries
	int compressionLevel; //!< Cache image compression level (see QImage)
};

struct ApplicationPrefs
{
	ColorPrefs colorPrefs;
	DisplayPrefs displayPrefs;
	DocumentSetupPrefs docSetupPrefs;
	ExternalToolsPrefs extToolPrefs;
	FontPrefs fontPrefs;
	GuidesPrefs guidesPrefs;
	HyphenatorPrefs hyphPrefs;
	ImageCachePrefs imageCachePrefs;
	ItemAttrPrefs itemAttrPrefs;
	ItemToolPrefs itemToolPrefs;
	KeyboardShortcutsPrefs keyShortcutPrefs;
	MiscellaneousPrefs miscPrefs;
	OperatorToolPrefs opToolPrefs;
	PDFOptions pdfPrefs;
	PathPrefs pathPrefs;
	PluginPrefs pluginPrefs;
	PrinterPrefs printerPrefs;
	PrintPreviewPrefs printPreviewPrefs;
	PDFOutputPreviewPrefs pdfOutputPreviewPrefs;
	PSOutputPreviewPrefs  psOutputPreviewPrefs;
	ScrapbookPrefs scrapbookPrefs;
	ScripterPrefs scripterPrefs;
	ShortWordPrefs shortwordPrefs;
	StoryEditorPrefs storyEditorPrefs;
	TOCPrefs tocPrefs;
	TypoPrefs typoPrefs;
	UIPrefs uiPrefs;
	VerifierPrefs verifierPrefs;

	QList<ArrowDesc> arrowStyles;
	QHash<QString, VGradient> defaultGradients;
	QHash<QString, ScPattern> defaultPatterns;
	QList<PageSet> pageSets;
	QStringList activePageSizes;

	//TODO : Remove these no longer used items
	//! System default QStyle name for current instance, seemingly unused
	QString ui_SystemTheme;

	//Added for Doc Only
	DocumentInformation docInfo;
	DocumentSectionMap docSectionMap;
};

struct ScIconSetData
{
	QString path;
	QString baseName;
	QString license;
	QString author;
	QString activeversion;
	QString variant;
	QMap<QString, QString> nameTranslations;
};


#endif
