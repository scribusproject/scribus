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

struct ToolPrefs
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

//User Interface
struct UIPrefs
{
	int mouseMoveTimeout; //! Mouse move timeout for move/resize operations
	int wheelJump; //! Distance to jump with mouse wheel scrolling
	int applicationFontSize; //! Font size to use in the application, apart from pßalettes
	int paletteFontSize; //! Fotn size to use in the palettes
	QString style; 	//! Currently used QStyle name
	int recentDocCount; //! Number of recent documents to remember
	QString language; //! Language of the user interface
	bool useSmallWidgets; //! Use small widgets in the palettes
	bool showStartupDialog; //! Whether to show the startup dialog or not
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
	bool guidePlacement;

	int grabRadius;
	double guideRad;

	bool guidesShown;
	bool marginsShown;
	bool gridShown;
	bool baselineGridShown;
	bool framesShown;
	bool colBordersShown;
	bool layerMarkersShown;
	bool linkShown;
	bool rulersShown;

	double majorGridSpacing;
	double minorGridSpacing;
	double valueBaselineGrid;
	double offsetBaselineGrid;

	bool showPic;
	bool showControls;
	bool showBleed;
	bool rulerMode;



	QColor guideColor;
	QColor marginColor;
	QColor majorGridColor;
	QColor minorGridColor;
	QColor baselineGridColor;
};


struct ApplicationPrefs
{
	UIPrefs uiPrefs;
	PathPrefs pathPrefs;
	DocumentSetupPrefs docSetupPrefs;
	GuidesPrefs guidesPrefs;
	//Typography
	//Item Tools
	//Operators Tools
	//Hyphenator
	//Fonts
	//Printer
	//ColorManagement
	//PDF Export
	//Document Item Attributes
	//Tables of Contents
	//Keyboard Shortcuts
	//Scrapbook
	//Display
	//External Tools
	//Miscellaneous
	//Plugins
	//Short Words
	//Scripter


	SCFonts AvailFonts;
	QList<ArrowDesc> arrowStyles;
	QList<PageSet> pageSets;
	ColorList DColors;

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

	TypoPrefs typographicSettings;
	ToolPrefs toolSettings;
	CheckerPrefsList checkerProfiles;
	QString curCheckProfile;

	WindowPrefs mainWinSettings;
	QByteArray mainWinState;
	


	PDFOptions PDF_Options;
	ObjAttrVector defaultItemAttributes;
	ToCSetupVector defaultToCSetups;
	bool SepalT;
	bool SepalN;
//	int PSize;
	bool marginColored;
	QStringList RecentDocs;
	QStringList RecentScrapbooks;
	int numScrapbookCopies;
	bool doCopyToScrapbook;
	bool persistentScrapbook;

	CMSData DCMSset;
	QMap<QString,Keys> KeyActions;
	QString PrinterName;
	QString PrinterFile;
	QString PrinterCommand;

	int MinWordLen;
	int HyCount;
	QString Language;
	QHash<QString, QString> specialWords;
	QSet<QString> ignoredWords;

	bool Automatic;
	bool AutoCheck;

	QMap<QString,QString> GFontSub;
	bool AutoSave;
	int AutoSaveTime;
	double DisScale;
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
	bool ClipMargin;
	bool GCRMode;
	QColor STEcolor;
	QString STEfont;
	MarginStruct scratch;
	double GapHorizontal;
	double GapVertical;
	bool askBeforeSubstituite;
	bool haveStylePreview;
	bool showToolTips;
	bool showMouseCoordinates;
	// lorem ipsum
	bool useStandardLI;
	int paragraphsLI;


	bool stickyTools;
	bool grayscaleIcons;
	//TODO : Remove these no longer used items
	//! System default QStyle name for current instance, seemingly unused
	QString ui_SystemTheme;
};

#endif
