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
#include "pagestructs.h"
#include "pdfoptions.h"
#include "scfonts.h"
#include "scpattern.h"
#include "vgradient.h"
#include "documentinformation.h"

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
	bool checkOffConflictLayers; //Check whether layers are marked as visible but not to be printed or vice versa
	bool checkNotCMYKOrSpot; // colors must be either CMYK or spot (PDF/X-1a)
	bool checkDeviceColorsAndOutputIntend; // unmanaged colors (device colors) must agree with output intend
	bool checkFontNotEmbedded; // embedded PDF might use fonts without embedding
	bool checkFontIsOpenType; // embedded PDF might use OpenType font program (only allowed in PDF/X-4 and PDF 1.6)
	bool checkPartFilledImageFrames;
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
	bool useTabs;		 //! Use a tabbed MainWidget a la FireFox 
	bool showStartupDialog; //! Whether to show the startup dialog or not
	bool showSplashOnStartup; //! Whether to show the splashscreen or not
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
	bool saveCompressed;
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
	double imageScaleX; //! X scale of an image within an image frame
	double imageScaleY; //! Y scale of an image within an image frame
	bool imageScaleType; //! Scale type of image
	bool imageAspectRatio; //! Use stored aspect ratio for the image
	int imageLowResType; //! Preview type for an image frame
	bool imageUseEmbeddedPath; //! Use embedded path, eg from an EPS etc.
	/* Calligraphic Pen Tool */
	QString calligrapicPenFillColor; //! Default calligrapicPen fill color
	QString calligrapicPenLineColor; //! Default calligrapicPen line color
	int calligrapicPenFillColorShade; //! Default calligrapicPen fill shade
	int calligrapicPenLineColorShade; //! Default calligrapicPen line shade
	double calligrapicPenLineWidth; //! Width of line
	double calligrapicPenAngle;		//! Angle of the calligraphics Brush
	double calligrapicPenWidth;		//! Width of the calligraphics Brush
	int calligrapicPenStyle; //! Line style of the calligraphics Brush
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
	PrintPreviewPrefs printPreviewPrefs;
	PrinterPrefs printerPrefs;
	ScrapbookPrefs scrapbookPrefs;
	ScripterPrefs scripterPrefs;
	ShortWordPrefs shortwordPrefs;
	StoryEditorPrefs storyEditorPrefs;
	TOCPrefs tocPrefs;
	TypoPrefs typoPrefs;
	UIPrefs uiPrefs;
	VerifierPrefs verifierPrefs;

	QList<ArrowDesc> arrowStyles;
	QMap<QString, VGradient> defaultGradients;
	QMap<QString, ScPattern> defaultPatterns;
	QList<PageSet> pageSets;
	QStringList activePageSizes;



	//TODO : Remove these no longer used items
	//! System default QStyle name for current instance, seemingly unused
	QString ui_SystemTheme;
	//	bool Gcr_Mode;

	//Added for Doc Only
	DocumentInformation docInfo;
	DocumentSectionMap docSectionMap;
};

#endif
