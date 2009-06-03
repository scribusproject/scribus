/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus134format.h"
#include "scribus134formatimpl.h"

#include "../../formatidlist.h"
#include "commonstrings.h"
#include "missing.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "scconfig.h"
#include "scpattern.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "sccolorengine.h"
#include "scribuscore.h"
#include "hyphenator.h"
#include "pageitem_latexframe.h"

#include "units.h"
#include "util.h"
#include "util_math.h"
#include "util_color.h"
#include "scgzfile.h"
#include "scpattern.h"
#include <QCursor>
// #include <QDebug>
#include <QFileInfo>
#include <QList>
#include <QByteArray>
#include <QApplication>


// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in scribus134formatimpl.h and scribus134formatimpl.cpp .

Scribus134Format::Scribus134Format() :
	LoadSavePlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file formats.
	languageChange();
}

Scribus134Format::~Scribus134Format()
{
	unregisterAll();
};

void Scribus134Format::languageChange()
{
	//(Re)register file formats.
	unregisterAll();
	registerFormats();
}

const QString Scribus134Format::fullTrName() const
{
	return QObject::tr("Scribus 1.3.4+ Support");
}

const ScActionPlugin::AboutData* Scribus134Format::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8(
			"Franz Schmid <franz@scribus.info>, "
			"The Scribus Team");
	about->shortDescription = tr("Scribus 1.3.4+ File Format Support");
	about->description = tr("Allows Scribus to read Scribus 1.3.4 and higher formatted files.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void Scribus134Format::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void Scribus134Format::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Scribus 1.3.4+ Document");
	fmt.formatId = FORMATID_SLA134IMPORT;
	fmt.load = true;
	fmt.save = true;
	fmt.filter = fmt.trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
	fmt.nameMatch = QRegExp("\\.(sla|scd)(\\.gz)?", Qt::CaseInsensitive);
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-scribus");
	fmt.priority = 64;
	registerFormat(fmt);
}

bool Scribus134Format::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	QByteArray docBytes("");
	if(fileName.right(2) == "gz")
	{
		if (!ScGzFile::readFromFile(fileName, docBytes, 4096))
		{
			// FIXME: Needs better error return
			return false;
		}
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
//	if (docBytes.left(16) == "<SCRIBUSUTF8NEW " && docBytes.left(35).contains("Version=\"1.3.4"))
//		return true;
	int startElemPos = docBytes.left(512).indexOf("<SCRIBUSUTF8NEW ");
	return startElemPos >= 0 && ((docBytes.mid(startElemPos, 64).indexOf("Version=\"1.3.4") >= 0)
				     || (docBytes.mid(startElemPos, 64).indexOf("Version=\"1.3.5") >= 0)
				     || (docBytes.mid(startElemPos, 64).indexOf("Version=\"1.5.0") >= 0));
}

QString Scribus134Format::readSLA(const QString & fileName)
{
	QByteArray docBytes("");
	if(fileName.right(2) == "gz")
	{
		if (!ScGzFile::readFromFile(fileName, docBytes))
		{
			// FIXME: Needs better error return
			return false;
		}
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
	QString docText("");
	int startElemPos = docBytes.left(512).indexOf("<SCRIBUSUTF8NEW ");
	if (startElemPos >= 0 && ((docBytes.mid(startElemPos, 64).indexOf("Version=\"1.3.4") >= 0)
		|| (docBytes.mid(startElemPos, 64).indexOf("Version=\"1.3.5") >= 0)
		|| (docBytes.mid(startElemPos, 64).indexOf("Version=\"1.5.0") >= 0)))
		docText = QString::fromUtf8(docBytes);
	else
		return QString::null;
	if (docText.endsWith(QChar(10)) || docText.endsWith(QChar(13)))
		docText.truncate(docText.length()-1);
	return docText;
}

void Scribus134Format::getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces)
{
	getNewReplacement=false;
	getReplacedFonts.clear();
}

bool Scribus134Format::loadFile(const QString & fileName, const FileFormat & /* fmt */, int /* flags */, int /* index */)
{
	if (m_Doc==0 || m_AvailableFonts==0)
	{
		Q_ASSERT(m_Doc==0 || m_AvailableFonts==0);
		return false;
	}
	ParagraphStyle vg;
	struct ScribusDoc::BookMa bok;
	int counter;//, Pgc;
	//bool AtFl;
	bool newVersion = false;
	QString tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QFont fo;
	QMap<int,int> TableID;
	QList<PageItem*> TableItems;
	QMap<int,int> TableIDM;
	QList<PageItem*> TableItemsM;
	QMap<int,int> TableIDF;
	QList<PageItem*> TableItemsF;
	QMap<PageItem*, int> groupID;
	QMap<PageItem*, int> groupIDM;
	QMap<PageItem*, int> groupIDF;
	int a;
	PageItem *Neu;
	Page* Apage;
	groupRemap.clear();
	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	itemRemapM.clear();
	itemNextM.clear();
	itemCountM = 0;
	itemRemapF.clear();
	itemNextF.clear();
	itemCountF = 0;
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	QString fileDir = QFileInfo(fileName).absolutePath();
	/* 2004/10/02 - petr vanek - bug #1092 - missing <PAGE> crash Scribus. The check constraint moved into IsScribus()
	FIXME: I've add test on containig tag PAGE but returning false freezes S. in scribus.cpp need some hack too...  */
	if (!docu.setContent(f))
		return false;
	m_Doc->PageColors.clear();
	m_Doc->Layers.clear();
	int layerToSetActive=0;
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	if (m_mwProgressBar!=0)
	{
		m_mwProgressBar->setMaximum(DOC.childNodes().count());
		m_mwProgressBar->setValue(0);
	}
	// Stop autosave timer,it will be restarted only if doc has autosave feature is enabled
	if (m_Doc->autoSaveTimer->isActive())
		m_Doc->autoSaveTimer->stop();
	int ObCount = 0;
	TableItems.clear();
	TableID.clear();
	TableItemsM.clear();
	TableIDM.clear();
	TableItemsF.clear();
	TableIDF.clear();
	PrefsManager* prefsManager=PrefsManager::instance();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
	/*
	* Attribute von DOCUMENT auslesen
	*/
		//CB Add this in to set this in the file in memory. Its saved, why not load it.
		//Will of course be replaced by per page settings although we still probably need a document default
		m_Doc->m_pageSize = dc.attribute("PAGESIZE");
		m_Doc->PageOri = dc.attribute("ORIENTATION", "0").toInt();
		m_Doc->FirstPnum = dc.attribute("FIRSTNUM", "1").toInt();
		m_Doc->currentPageLayout=dc.attribute("BOOK", "0").toInt();
		int fp;
		if (m_Doc->currentPageLayout == 0)
			fp = 0;
		else
		{
			if (dc.attribute("FIRSTLEFT", "0").toInt() == 1)
				fp = 0;
			else
				fp = 1;
		}
		if (DOC.namedItem("PageSets").isNull())
		{
			m_Doc->pageSets[m_Doc->currentPageLayout].FirstPage = fp;
//			m_Doc->pageSets[m_Doc->currentPageLayout].GapHorizontal = dc.attribute("GapHorizontal", "0").toDouble();
//			m_Doc->pageSets[m_Doc->currentPageLayout].GapVertical = 0.0;
//			m_Doc->pageSets[m_Doc->currentPageLayout].GapBelow = dc.attribute("GapVertical", "40").toDouble();
		}
		m_Doc->setUsesAutomaticTextFrames(dc.attribute("AUTOTEXT").toInt());
		m_Doc->PageSp=dc.attribute("AUTOSPALTEN").toInt();
		m_Doc->PageSpa=ScCLocale::toDoubleC( dc.attribute("ABSTSPALTEN") );
		m_Doc->setUnitIndex(dc.attribute("UNITS", "0").toInt());
		m_Doc->toolSettings.defSize=qRound(ScCLocale::toDoubleC(dc.attribute("DSIZE")) * 10);
		Defont=dc.attribute("DFONT");
		m_AvailableFonts->findFont(Defont, m_Doc);
		m_Doc->toolSettings.defFont = Defont;
		m_Doc->toolSettings.dCols=dc.attribute("DCOL", "1").toInt();
		m_Doc->toolSettings.dGap=ScCLocale::toDoubleC(dc.attribute("DGAP"), 0.0);
		m_Doc->documentInfo.setAuthor(dc.attribute("AUTHOR"));
		m_Doc->documentInfo.setComments(dc.attribute("COMMENTS"));
		m_Doc->documentInfo.setKeywords(dc.attribute("KEYWORDS",""));
		m_Doc->documentInfo.setTitle(dc.attribute("TITLE"));
		m_Doc->documentInfo.setPublisher(dc.attribute("PUBLISHER", ""));
		m_Doc->documentInfo.setDate(dc.attribute("DOCDATE", ""));
		m_Doc->documentInfo.setType(dc.attribute("DOCTYPE", ""));
		m_Doc->documentInfo.setFormat(dc.attribute("DOCFORMAT", ""));
		m_Doc->documentInfo.setIdent(dc.attribute("DOCIDENT", ""));
		m_Doc->documentInfo.setSource(dc.attribute("DOCSOURCE", ""));
		m_Doc->documentInfo.setLangInfo(dc.attribute("DOCLANGINFO", ""));
		m_Doc->documentInfo.setRelation(dc.attribute("DOCRELATION", ""));
		m_Doc->documentInfo.setCover(dc.attribute("DOCCOVER", ""));
		m_Doc->documentInfo.setRights(dc.attribute("DOCRIGHTS", ""));
		m_Doc->documentInfo.setContrib(dc.attribute("DOCCONTRIB", ""));
		m_Doc->typographicSettings.valueSuperScript = dc.attribute("VHOCH").toInt();
		m_Doc->typographicSettings.scalingSuperScript = dc.attribute("VHOCHSC").toInt();
		m_Doc->typographicSettings.valueSubScript = dc.attribute("VTIEF").toInt();
		m_Doc->typographicSettings.scalingSubScript = dc.attribute("VTIEFSC").toInt();
		m_Doc->typographicSettings.valueSmallCaps = dc.attribute("VKAPIT").toInt();
		m_Doc->typographicSettings.valueBaseGrid = ScCLocale::toDoubleC(dc.attribute("BASEGRID"), 12.0);
		m_Doc->typographicSettings.offsetBaseGrid = ScCLocale::toDoubleC(dc.attribute("BASEO"), 0.0);
		m_Doc->typographicSettings.autoLineSpacing = dc.attribute("AUTOL", "20").toInt();
		m_Doc->typographicSettings.valueUnderlinePos = dc.attribute("UnderlinePos", "-1").toInt();
		m_Doc->typographicSettings.valueUnderlineWidth = dc.attribute("UnderlineWidth", "-1").toInt();
		m_Doc->typographicSettings.valueStrikeThruPos = dc.attribute("StrikeThruPos", "-1").toInt();
		m_Doc->typographicSettings.valueStrikeThruWidth = dc.attribute("StrikeThruWidth", "-1").toInt();
		m_Doc->GroupCounter = 1/*dc.attribute("GROUPC", "1").toInt()*/;
		//m_Doc->HasCMS = static_cast<bool>(dc.attribute("HCMS", "0").toInt());
		m_Doc->CMSSettings.SoftProofOn = static_cast<bool>(dc.attribute("DPSo", "0").toInt());
		m_Doc->CMSSettings.SoftProofFullOn = static_cast<bool>(dc.attribute("DPSFo", "0").toInt());
		m_Doc->CMSSettings.CMSinUse = static_cast<bool>(dc.attribute("DPuse", "0").toInt());
		m_Doc->CMSSettings.GamutCheck = static_cast<bool>(dc.attribute("DPgam", "0").toInt());
		m_Doc->CMSSettings.BlackPoint = static_cast<bool>(dc.attribute("DPbla", "1").toInt());
		m_Doc->CMSSettings.DefaultMonitorProfile = dc.attribute("DPMo","");
		m_Doc->CMSSettings.DefaultPrinterProfile = dc.attribute("DPPr","");
		m_Doc->CMSSettings.DefaultImageRGBProfile = dc.attribute("DPIn","");
		m_Doc->CMSSettings.DefaultImageCMYKProfile = dc.attribute("DPInCMYK","");
		m_Doc->CMSSettings.DefaultSolidColorRGBProfile = dc.attribute("DPIn2","");
		if (dc.hasAttribute("DPIn3"))
			m_Doc->CMSSettings.DefaultSolidColorCMYKProfile = dc.attribute("DPIn3","");
		else
			m_Doc->CMSSettings.DefaultSolidColorCMYKProfile = dc.attribute("DPPr","");
		//m_Doc->CMSSettings.DefaultIntentPrinter = dc.attribute("DIPr", "0").toInt();
		//m_Doc->CMSSettings.DefaultIntentMonitor = dc.attribute("DIMo", "1").toInt();
		m_Doc->CMSSettings.DefaultIntentColors = dc.attribute("DISc", "1").toInt();
		m_Doc->CMSSettings.DefaultIntentImages = dc.attribute("DIIm", "0").toInt();
		layerToSetActive=dc.attribute("ALAYER", "0").toInt();
		m_Doc->Language = dc.attribute("LANGUAGE", "");
		m_Doc->MinWordLen = dc.attribute("MINWORDLEN", "3").toInt();
		m_Doc->HyCount = dc.attribute("HYCOUNT", "2").toInt();
		if (dc.hasAttribute("PAGEWIDTH"))
			m_Doc->pageWidth=ScCLocale::toDoubleC(dc.attribute("PAGEWIDTH"));
		else
			m_Doc->pageWidth=ScCLocale::toDoubleC(dc.attribute("PAGEWITH"));
		m_Doc->pageHeight=ScCLocale::toDoubleC(dc.attribute("PAGEHEIGHT"));
		m_Doc->pageMargins.Left=qMax(0.0, ScCLocale::toDoubleC(dc.attribute("BORDERLEFT")));
		m_Doc->pageMargins.Right=qMax(0.0, ScCLocale::toDoubleC(dc.attribute("BORDERRIGHT")));
		m_Doc->pageMargins.Top=qMax(0.0, ScCLocale::toDoubleC(dc.attribute("BORDERTOP")));
		m_Doc->pageMargins.Bottom=qMax(0.0, ScCLocale::toDoubleC(dc.attribute("BORDERBOTTOM")));
		m_Doc->marginPreset = dc.attribute("PRESET", "0").toInt();
		m_Doc->bleeds.Top    = ScCLocale::toDoubleC(dc.attribute("BleedTop"), 0.0);
		m_Doc->bleeds.Left   = ScCLocale::toDoubleC(dc.attribute("BleedLeft"), 0.0);
		m_Doc->bleeds.Right  = ScCLocale::toDoubleC(dc.attribute("BleedRight"), 0.0);
		m_Doc->bleeds.Bottom = ScCLocale::toDoubleC(dc.attribute("BleedBottom"), 0.0);
		m_Doc->Automatic = static_cast<bool>(dc.attribute("AUTOMATIC", "1").toInt());
		m_Doc->AutoCheck = static_cast<bool>(dc.attribute("AUTOCHECK", "0").toInt());
		m_Doc->GuideLock = static_cast<bool>(dc.attribute("GUIDELOCK", "0").toInt());
		m_Doc->guidesSettings.minorGrid = ScCLocale::toDoubleC(dc.attribute("MINGRID"), prefsManager->appPrefs.guidesSettings.minorGrid);
		m_Doc->guidesSettings.majorGrid = ScCLocale::toDoubleC(dc.attribute("MAJGRID"), prefsManager->appPrefs.guidesSettings.majorGrid);
		m_Doc->guidesSettings.gridShown = static_cast<bool>(dc.attribute("SHOWGRID", "0").toInt());
		m_Doc->guidesSettings.guidesShown = static_cast<bool>(dc.attribute("SHOWGUIDES", "1").toInt());
		m_Doc->guidesSettings.colBordersShown = static_cast<bool>(dc.attribute("showcolborders", "0").toInt());
		m_Doc->guidesSettings.framesShown = static_cast<bool>(dc.attribute("SHOWFRAME", "1").toInt());
		m_Doc->guidesSettings.layerMarkersShown = static_cast<bool>(dc.attribute("SHOWLAYERM", "0").toInt());
		m_Doc->guidesSettings.marginsShown = static_cast<bool>(dc.attribute("SHOWMARGIN", "1").toInt());
		m_Doc->guidesSettings.baseShown = static_cast<bool>(dc.attribute("SHOWBASE", "0").toInt());
		m_Doc->guidesSettings.showPic = static_cast<bool>(dc.attribute("SHOWPICT", "1").toInt());
		m_Doc->guidesSettings.linkShown = static_cast<bool>(dc.attribute("SHOWLINK", "0").toInt());
		m_Doc->guidesSettings.showControls = static_cast<bool>(dc.attribute("SHOWControl", "0").toInt());
		m_Doc->guidesSettings.rulerMode = static_cast<bool>(dc.attribute("rulerMode", "1").toInt());
		m_Doc->guidesSettings.rulersShown = static_cast<bool>(dc.attribute("showrulers", "1").toInt());
		m_Doc->guidesSettings.showBleed = static_cast<bool>(dc.attribute("showBleed", "1").toInt());
		m_Doc->rulerXoffset = ScCLocale::toDoubleC(dc.attribute("rulerXoffset"), 0.0);
		m_Doc->rulerYoffset = ScCLocale::toDoubleC(dc.attribute("rulerYoffset"), 0.0);
		m_Doc->SnapGuides = static_cast<bool>(dc.attribute("SnapToGuides", "0").toInt());
		m_Doc->useRaster = static_cast<bool>(dc.attribute("SnapToGrid", "0").toInt());
		m_Doc->toolSettings.polyC = dc.attribute("POLYC", "4").toInt();
		m_Doc->toolSettings.polyF = ScCLocale::toDoubleC(dc.attribute("POLYF"), 0.5);
		m_Doc->toolSettings.polyR = ScCLocale::toDoubleC(dc.attribute("POLYR"), 0.0);
		m_Doc->toolSettings.polyCurvature = ScCLocale::toDoubleC(dc.attribute("POLYCUR"), 0.0);
		m_Doc->toolSettings.polyFd = dc.attribute("POLYFD", "0").toInt();
		m_Doc->toolSettings.polyS = static_cast<bool>(dc.attribute("POLYS", "0").toInt());
		m_Doc->AutoSave = static_cast<bool>(dc.attribute("AutoSave", "0").toInt());
		m_Doc->AutoSaveTime = dc.attribute("AutoSaveTime", "600000").toInt();
		m_Doc->scratch.Bottom = ScCLocale::toDoubleC(dc.attribute("ScratchBottom"), 20.0);
		// FIXME A typo in early 1.3cvs (MAR 05) means we must support loading of
		// FIXME 'ScatchLeft' for a while too. This can be removed in a few months.
		if (dc.hasAttribute("ScatchLeft"))
			m_Doc->scratch.Left = ScCLocale::toDoubleC( dc.attribute("ScatchLeft"), 100.0);
		else
			m_Doc->scratch.Left = ScCLocale::toDoubleC( dc.attribute("ScratchLeft"), 100.0);
		m_Doc->scratch.Right = ScCLocale::toDoubleC( dc.attribute("ScratchRight"), 100.0);
		m_Doc->scratch.Top   = ScCLocale::toDoubleC( dc.attribute("ScratchTop"), 20.0);
		m_Doc->GapHorizontal = ScCLocale::toDoubleC( dc.attribute("GapHorizontal"), -1.0);
		m_Doc->GapVertical   = ScCLocale::toDoubleC( dc.attribute("GapVertical"), -1.0);
		m_Doc->toolSettings.dStartArrow = dc.attribute("StartArrow", "0").toInt();
		m_Doc->toolSettings.dEndArrow = dc.attribute("EndArrow", "0").toInt();
		m_Doc->toolSettings.scaleX = ScCLocale::toDoubleC( dc.attribute("PICTSCX"), 1.0);
		m_Doc->toolSettings.scaleY = ScCLocale::toDoubleC( dc.attribute("PICTSCY"), 1.0);
		m_Doc->toolSettings.scaleType = static_cast<bool>(dc.attribute("PSCALE", "1").toInt());
		m_Doc->toolSettings.aspectRatio = static_cast<bool>(dc.attribute("PASPECT", "0").toInt());
		m_Doc->toolSettings.lowResType = dc.attribute("HalfRes", "1").toInt();
		m_Doc->toolSettings.useEmbeddedPath = static_cast<bool>(dc.attribute("EmbeddedPath", "0").toInt());
		if (dc.hasAttribute("PEN"))
			m_Doc->toolSettings.dPen = dc.attribute("PEN");
		if (dc.hasAttribute("BRUSH"))
			m_Doc->toolSettings.dBrush = dc.attribute("BRUSH");
		if (dc.hasAttribute("PENLINE"))
			m_Doc->toolSettings.dPenLine = dc.attribute("PENLINE");
		if (dc.hasAttribute("PENTEXT"))
			m_Doc->toolSettings.dPenText = dc.attribute("PENTEXT");
		if (dc.hasAttribute("StrokeText"))
			m_Doc->toolSettings.dStrokeText = dc.attribute("StrokeText");
		m_Doc->toolSettings.dTextBackGround = dc.attribute("TextBackGround", CommonStrings::None);
		m_Doc->toolSettings.dTextLineColor = dc.attribute("TextLineColor", CommonStrings::None);
		m_Doc->toolSettings.dTextBackGroundShade = dc.attribute("TextBackGroundShade", "100").toInt();
		m_Doc->toolSettings.dTextLineShade = dc.attribute("TextLineShade", "100").toInt();
		m_Doc->toolSettings.dTextPenShade = dc.attribute("TextPenShade", "100").toInt();
		m_Doc->toolSettings.dTextStrokeShade = dc.attribute("TextStrokeShade", "100").toInt();
		m_Doc->toolSettings.dLineArt = static_cast<Qt::PenStyle>(dc.attribute("STIL").toInt());
		m_Doc->toolSettings.dLstyleLine = static_cast<Qt::PenStyle>(dc.attribute("STILLINE").toInt());
		m_Doc->toolSettings.dWidth = ScCLocale::toDoubleC( dc.attribute("WIDTH"), 0.0);
		m_Doc->toolSettings.dWidthLine = ScCLocale::toDoubleC( dc.attribute("WIDTHLINE"), 1.0);
		m_Doc->toolSettings.dShade2 = dc.attribute("PENSHADE", "100").toInt();
		m_Doc->toolSettings.dShadeLine = dc.attribute("LINESHADE", "100").toInt();
		m_Doc->toolSettings.dShade = dc.attribute("BRUSHSHADE", "100").toInt();
		m_Doc->toolSettings.magMin = dc.attribute("MAGMIN", "10").toInt();
		m_Doc->toolSettings.magMax = dc.attribute("MAGMAX", "3200").toInt();
		m_Doc->toolSettings.magStep = dc.attribute("MAGSTEP", "200").toInt();
		m_Doc->toolSettings.dispX = ScCLocale::toDoubleC( dc.attribute("dispX"), 10.0);
		m_Doc->toolSettings.dispY = ScCLocale::toDoubleC( dc.attribute("dispY"), 10.0);
		m_Doc->toolSettings.constrain = ScCLocale::toDoubleC( dc.attribute("constrain"), 15.0);
		//CB Reset doc zoom step value to 200% instead of old values.
		if (m_Doc->toolSettings.magStep<100)
			m_Doc->toolSettings.magStep=200;
		m_Doc->toolSettings.tabFillChar = dc.attribute("TabFill","");
		m_Doc->toolSettings.dTabWidth= ScCLocale::toDoubleC(dc.attribute("TabWidth"), 36.0);
		if (dc.hasAttribute("CPICT"))
			m_Doc->toolSettings.dBrushPict = dc.attribute("CPICT");
		m_Doc->toolSettings.shadePict = dc.attribute("PICTSHADE", "100").toInt();
		if (dc.hasAttribute("PAGEC"))
			m_Doc->papColor = QColor(dc.attribute("PAGEC"));
		if (dc.hasAttribute("MARGC"))
			m_Doc->guidesSettings.margColor = QColor(dc.attribute("MARGC"));
		if (dc.hasAttribute("MINORC"))
			m_Doc->guidesSettings.minorColor = QColor(dc.attribute("MINORC"));
		if (dc.hasAttribute("MAJORC"))
			m_Doc->guidesSettings.majorColor = QColor(dc.attribute("MAJORC"));
		if (dc.hasAttribute("GuideC"))
			m_Doc->guidesSettings.guideColor = QColor(dc.attribute("GuideC"));
		if (dc.hasAttribute("BaseC"))
			m_Doc->guidesSettings.baseColor = QColor(dc.attribute("BaseC"));
		m_Doc->marginColored = static_cast<bool>(dc.attribute("RANDF", "0").toInt());
		m_Doc->guidesSettings.before = static_cast<bool>(dc.attribute("BACKG", "1").toInt());
		m_Doc->guidesSettings.guideRad = ScCLocale::toDoubleC( dc.attribute("GuideRad"), 10.0);
		m_Doc->guidesSettings.grabRad = dc.attribute("GRAB", "4").toInt();
		if (dc.hasAttribute("currentProfile"))
		{
			m_Doc->checkerProfiles.clear();
			m_Doc->curCheckProfile = dc.attribute("currentProfile");
		}
		m_Doc->LastAuto = 0;
		QDomNode PAGE=DOC.firstChild();
		counter = 0;
		while(!PAGE.isNull())
		{
			ObCount++;
			if (m_mwProgressBar!=0)
				m_mwProgressBar->setValue(ObCount);
			QDomElement pg=PAGE.toElement();
			if (pg.tagName()=="PageSets")
			{
				QDomNode PGS = PAGE.firstChild();
				m_Doc->pageSets.clear();
				while(!PGS.isNull())
				{
					QDomElement PgsAttr = PGS.toElement();
					if(PgsAttr.tagName() == "Set")
					{
						struct PageSet pageS;
						pageS.Name = CommonStrings::untranslatePageSetString(PgsAttr.attribute("Name"));
						pageS.FirstPage = PgsAttr.attribute("FirstPage", "0").toInt();
						pageS.Rows = PgsAttr.attribute("Rows", "1").toInt();
						pageS.Columns = PgsAttr.attribute("Columns", "1").toInt();
//						pageS.GapHorizontal = ScCLocale::toDoubleC( PgsAttr.attribute("GapHorizontal", "0"));
//						pageS.GapVertical = ScCLocale::toDoubleC( PgsAttr.attribute("GapVertical", "0"));
//						pageS.GapBelow = ScCLocale::toDoubleC( PgsAttr.attribute("GapBelow", "0"));
						pageS.pageNames.clear();
						QDomNode PGSN = PGS.firstChild();
						while(!PGSN.isNull())
						{
							QDomElement PgsAttrN = PGSN.toElement();
							if(PgsAttrN.tagName() == "PageNames")
								pageS.pageNames.append(CommonStrings::untranslatePageSetLocString(PgsAttrN.attribute("Name")));
							PGSN = PGSN.nextSibling();
						}
						m_Doc->pageSets.append(pageS);
						if ((m_Doc->pageSets.count()-1 == m_Doc->currentPageLayout) && ((m_Doc->GapHorizontal < 0) && (m_Doc->GapVertical < 0)))
						{
							m_Doc->GapHorizontal = ScCLocale::toDoubleC( PgsAttr.attribute("GapHorizontal"), 0.0);
							m_Doc->GapVertical   = ScCLocale::toDoubleC( PgsAttr.attribute("GapBelow"), 0.0);
						}
					}
					PGS = PGS.nextSibling();
				}
			}
			if (pg.tagName()=="CheckProfile")
			{
				struct checkerPrefs checkerSettings;
				checkerSettings.ignoreErrors = static_cast<bool>(pg.attribute("ignoreErrors", "0").toInt());
				checkerSettings.autoCheck = static_cast<bool>(pg.attribute("autoCheck", "1").toInt());
				checkerSettings.checkGlyphs = static_cast<bool>(pg.attribute("checkGlyphs", "1").toInt());
				checkerSettings.checkOrphans = static_cast<bool>(pg.attribute("checkOrphans", "1").toInt());
				checkerSettings.checkOverflow = static_cast<bool>(pg.attribute("checkOverflow", "1").toInt());
				checkerSettings.checkPictures = static_cast<bool>(pg.attribute("checkPictures", "1").toInt());
				checkerSettings.checkResolution = static_cast<bool>(pg.attribute("checkResolution", "1").toInt());
				checkerSettings.checkTransparency = static_cast<bool>(pg.attribute("checkTransparency", "1").toInt());
				checkerSettings.minResolution = ScCLocale::toDoubleC( pg.attribute("minResolution"), 72.0);
				checkerSettings.maxResolution = ScCLocale::toDoubleC( pg.attribute("maxResolution"), 4800.0);
				checkerSettings.checkAnnotations = static_cast<bool>(pg.attribute("checkAnnotations", "0").toInt());
				checkerSettings.checkRasterPDF = static_cast<bool>(pg.attribute("checkRasterPDF", "1").toInt());
				checkerSettings.checkForGIF = static_cast<bool>(pg.attribute("checkForGIF", "1").toInt());
				checkerSettings.ignoreOffLayers = static_cast<bool>(pg.attribute("ignoreOffLayers", "0").toInt());
				m_Doc->checkerProfiles[pg.attribute("Name")] = checkerSettings;
			}
			// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
				if (pg.hasAttribute("Spot"))
					lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
				else
					lf.setSpotColor(false);
				if (pg.hasAttribute("Register"))
					lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
				else
					lf.setRegistrationColor(false);
				QString name = pg.attribute("NAME");
				m_Doc->PageColors.insert((name.isEmpty()) ? lf.name() : name, lf);
			}
			if(pg.tagName()=="STYLE")
			{
				readParagraphStyle(vg, pg, *m_AvailableFonts, m_Doc);
				StyleSet<ParagraphStyle>tmp;
				tmp.create(vg);
				m_Doc->redefineStyles(tmp, false);
			}
			if(pg.tagName()=="CHARSTYLE")
			{
				readParagraphStyle(vg, pg, *m_AvailableFonts, m_Doc);
				StyleSet<CharStyle> temp;
				temp.create(vg.charStyle());
				m_Doc->redefineCharStyles(temp, false);
			}
			if(pg.tagName()=="JAVA")
				m_Doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				int lnr   = pg.attribute("NUMMER").toInt();
				int level = pg.attribute("LEVEL").toInt();
				ScLayer la( pg.attribute("NAME"), level, lnr);
				la.isViewable   = pg.attribute("SICHTBAR").toInt();
				la.isPrintable  = pg.attribute("DRUCKEN").toInt();
				la.isEditable   = pg.attribute("EDIT", "1").toInt();
				la.flowControl  = pg.attribute("FLOW", "1").toInt();
				la.transparency = ScCLocale::toDoubleC( pg.attribute("TRANS"), 1.0);
				la.blendMode    = pg.attribute("BLEND", "0").toInt();
				la.outlineMode  = pg.attribute("OUTL", "0").toInt();
				if (pg.hasAttribute("LAYERC"))
					la.markerColor =  QColor(pg.attribute("LAYERC","#000000"));
				m_Doc->Layers.append(la);
			}
/*			if(pg.tagName()=="Bookmark")
			{
				bok.Title = pg.attribute("Title");
				bok.Text = pg.attribute("Text");
				bok.Aktion = pg.attribute("Aktion");
				bok.ItemNr = pg.attribute("ItemNr").toInt();
				bok.Seite = pg.attribute("Seite").toInt();
				bok.Element = pg.attribute("Element").toInt();
				bok.First = pg.attribute("First").toInt();
				bok.Last = pg.attribute("Last").toInt();
				bok.Prev = pg.attribute("Prev").toInt();
				bok.Next = pg.attribute("Next").toInt();
				bok.Parent = pg.attribute("Parent").toInt();
				m_Doc->BookMarks.append(bok);
			} */
			if(pg.tagName()=="MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = PAGE.firstChild();
				while(!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct SingleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = MuL.attribute("Dash").toInt();
					sl.LineEnd = MuL.attribute("LineEnd").toInt();
					sl.LineJoin = MuL.attribute("LineJoin").toInt();
					sl.Shade = MuL.attribute("Shade").toInt();
					sl.Width = ScCLocale::toDoubleC( MuL.attribute("Width"));
					ml.shortcut = MuL.attribute("Shortcut");
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				m_Doc->MLineStyles.insert(pg.attribute("Name"), ml);
			}
			if(pg.tagName()=="Arrows")
			{
				struct ArrowDesc arrow;
				arrow.name = pg.attribute("Name");
				arrow.userArrow = true;
				double xa, ya;
				QString tmp = pg.attribute("Points");
				ScTextStream fp(&tmp, QIODevice::ReadOnly);
				for (uint cx = 0; cx < pg.attribute("NumPoints").toUInt(); ++cx)
				{
					fp >> xa;
					fp >> ya;
					arrow.points.addPoint(xa, ya);
				}
				m_Doc->arrowStyles.append(arrow);
			}
			if(pg.tagName()=="Printer")
			{
				m_Doc->Print_Options.firstUse = static_cast<bool>(pg.attribute("firstUse").toInt());
				m_Doc->Print_Options.toFile = static_cast<bool>(pg.attribute("toFile").toInt());
				m_Doc->Print_Options.useAltPrintCommand = static_cast<bool>(pg.attribute("useAltPrintCommand").toInt());
				m_Doc->Print_Options.outputSeparations = static_cast<bool>(pg.attribute("outputSeparations").toInt());
				m_Doc->Print_Options.useSpotColors = static_cast<bool>(pg.attribute("useSpotColors").toInt());
				m_Doc->Print_Options.useColor = static_cast<bool>(pg.attribute("useColor").toInt());
				m_Doc->Print_Options.mirrorH = static_cast<bool>(pg.attribute("mirrorH").toInt());
				m_Doc->Print_Options.mirrorV = static_cast<bool>(pg.attribute("mirrorV").toInt());
				m_Doc->Print_Options.useICC = static_cast<bool>(pg.attribute("useICC").toInt());
				m_Doc->Print_Options.doGCR = static_cast<bool>(pg.attribute("doGCR").toInt());
				m_Doc->Print_Options.doClip = static_cast<bool>(pg.attribute("doClip").toInt());
				m_Doc->Print_Options.setDevParam = static_cast<bool>(pg.attribute("setDevParam").toInt());
				m_Doc->Print_Options.useDocBleeds = static_cast<bool>(pg.attribute("useDocBleeds").toInt());
				m_Doc->Print_Options.cropMarks = static_cast<bool>(pg.attribute("cropMarks").toInt());
				m_Doc->Print_Options.bleedMarks = static_cast<bool>(pg.attribute("bleedMarks").toInt());
				m_Doc->Print_Options.registrationMarks = static_cast<bool>(pg.attribute("registrationMarks").toInt());
				m_Doc->Print_Options.colorMarks = static_cast<bool>(pg.attribute("colorMarks").toInt());
				m_Doc->Print_Options.includePDFMarks = static_cast<bool>(pg.attribute("includePDFMarks", "1").toInt());
				if (pg.hasAttribute("PrintEngine"))
					m_Doc->Print_Options.prnEngine = (PrintEngine) pg.attribute("PrintEngine", "3").toInt();
				else
					m_Doc->Print_Options.prnEngine = (PrintEngine) pg.attribute("PSLevel", "3").toInt();
				m_Doc->Print_Options.markOffset = ScCLocale::toDoubleC( pg.attribute("markOffset"));
				m_Doc->Print_Options.bleeds.Top = ScCLocale::toDoubleC( pg.attribute("BleedTop"));
				m_Doc->Print_Options.bleeds.Left = ScCLocale::toDoubleC( pg.attribute("BleedLeft"));
				m_Doc->Print_Options.bleeds.Right = ScCLocale::toDoubleC( pg.attribute("BleedRight"));
				m_Doc->Print_Options.bleeds.Bottom = ScCLocale::toDoubleC( pg.attribute("BleedBottom"));
				m_Doc->Print_Options.printer = pg.attribute("printer");
				m_Doc->Print_Options.filename = pg.attribute("filename");
				m_Doc->Print_Options.separationName = pg.attribute("separationName");
				m_Doc->Print_Options.printerCommand = pg.attribute("printerCommand");
				m_Doc->Print_Options.copies = 1;
				QDomNode PFO = PAGE.firstChild();
				while(!PFO.isNull())
				{
					QDomElement pdfF = PFO.toElement();
					if(pdfF.tagName() == "Separation")
						m_Doc->Print_Options.allSeparations.append(pdfF.attribute("Name"));
					PFO = PFO.nextSibling();
				}
			}
			if(pg.tagName()=="PDF")
			{
				m_Doc->PDF_Options.Articles = static_cast<bool>(pg.attribute("Articles").toInt());
				m_Doc->PDF_Options.Thumbnails = static_cast<bool>(pg.attribute("Thumbnails").toInt());
				m_Doc->PDF_Options.Compress = static_cast<bool>(pg.attribute("Compress").toInt());
				m_Doc->PDF_Options.CompressMethod = (PDFOptions::PDFCompression) pg.attribute("CMethod", "0").toInt();
				m_Doc->PDF_Options.Quality = pg.attribute("Quality", "0").toInt();
				m_Doc->PDF_Options.RecalcPic = static_cast<bool>(pg.attribute("RecalcPic").toInt());
				m_Doc->PDF_Options.Bookmarks = static_cast<bool>(pg.attribute("Bookmarks").toInt());
				if (pg.hasAttribute("firstUse"))
					m_Doc->PDF_Options.firstUse = static_cast<bool>(pg.attribute("firstUse").toInt());
				else
					m_Doc->PDF_Options.firstUse = true;
				if (pg.hasAttribute("MirrorH"))
					m_Doc->PDF_Options.MirrorH = static_cast<bool>(pg.attribute("MirrorH").toInt());
				else
					m_Doc->PDF_Options.MirrorH = false;
				if (pg.hasAttribute("MirrorV"))
					m_Doc->PDF_Options.MirrorV = static_cast<bool>(pg.attribute("MirrorV").toInt());
				else
					m_Doc->PDF_Options.MirrorV = false;
				if (pg.hasAttribute("RotateDeg"))
					m_Doc->PDF_Options.RotateDeg = pg.attribute("RotateDeg", "0").toInt();
				else
					m_Doc->PDF_Options.RotateDeg = 0;
				if (pg.hasAttribute("Clip"))
					m_Doc->PDF_Options.doClip = static_cast<bool>(pg.attribute("Clip").toInt());
				else
					m_Doc->PDF_Options.doClip = false;
				m_Doc->PDF_Options.PresentMode = static_cast<bool>(pg.attribute("PresentMode").toInt());
				m_Doc->PDF_Options.PicRes = pg.attribute("PicRes").toInt();
				// Fixme: check input pdf version
				m_Doc->PDF_Options.Version = (PDFOptions::PDFVersion)pg.attribute("Version").toInt();
				m_Doc->PDF_Options.Resolution = pg.attribute("Resolution").toInt();
				m_Doc->PDF_Options.Binding = pg.attribute("Binding").toInt();
				m_Doc->PDF_Options.fileName = "";
				m_Doc->PDF_Options.isGrayscale = static_cast<bool>(pg.attribute("Grayscale", "0").toInt());
				m_Doc->PDF_Options.UseRGB = static_cast<bool>(pg.attribute("RGBMode", "0").toInt());
				m_Doc->PDF_Options.UseProfiles = static_cast<bool>(pg.attribute("UseProfiles", "0").toInt());
				m_Doc->PDF_Options.UseProfiles2 = static_cast<bool>(pg.attribute("UseProfiles2", "0").toInt());
				m_Doc->PDF_Options.Intent = pg.attribute("Intent", "1").toInt();
				m_Doc->PDF_Options.Intent2 = pg.attribute("Intent2", "1").toInt();
				m_Doc->PDF_Options.SolidProf = pg.attribute("SolidP", "");
				m_Doc->PDF_Options.ImageProf = pg.attribute("ImageP", "");
				m_Doc->PDF_Options.PrintProf = pg.attribute("PrintP", "");
				m_Doc->PDF_Options.Info = pg.attribute("InfoString", "");
				m_Doc->PDF_Options.bleeds.Top    = ScCLocale::toDoubleC( pg.attribute("BTop"), 0.0);
				m_Doc->PDF_Options.bleeds.Left   = ScCLocale::toDoubleC( pg.attribute("BLeft"), 0.0);
				m_Doc->PDF_Options.bleeds.Right  = ScCLocale::toDoubleC( pg.attribute("BRight"), 0.0);
				m_Doc->PDF_Options.bleeds.Bottom = ScCLocale::toDoubleC( pg.attribute("BBottom"), 0.0);
				m_Doc->PDF_Options.useDocBleeds = static_cast<bool>(pg.attribute("useDocBleeds", "1").toInt());
				m_Doc->PDF_Options.cropMarks = static_cast<bool>(pg.attribute("cropMarks", "0").toInt());
				m_Doc->PDF_Options.bleedMarks = static_cast<bool>(pg.attribute("bleedMarks", "0").toInt());
				m_Doc->PDF_Options.registrationMarks = static_cast<bool>(pg.attribute("registrationMarks", "0").toInt());
				m_Doc->PDF_Options.colorMarks = static_cast<bool>(pg.attribute("colorMarks", "0").toInt());
				m_Doc->PDF_Options.docInfoMarks = static_cast<bool>(pg.attribute("docInfoMarks", "0").toInt());
				m_Doc->PDF_Options.markOffset = ScCLocale::toDoubleC( pg.attribute("markOffset"), 0.0);
				m_Doc->PDF_Options.EmbeddedI = static_cast<bool>(pg.attribute("ImagePr", "0").toInt());
				m_Doc->PDF_Options.PassOwner = pg.attribute("PassOwner", "");
				m_Doc->PDF_Options.PassUser = pg.attribute("PassUser", "");
				m_Doc->PDF_Options.Permissions = pg.attribute("Permissions", "-4").toInt();
				m_Doc->PDF_Options.Encrypt = static_cast<bool>(pg.attribute("Encrypt", "0").toInt());
				m_Doc->PDF_Options.useLayers = static_cast<bool>(pg.attribute("UseLayers", "0").toInt());
				m_Doc->PDF_Options.UseLPI = static_cast<bool>(pg.attribute("UseLpi", "0").toInt());
				m_Doc->PDF_Options.UseSpotColors = static_cast<bool>(pg.attribute("UseSpotColors", "1").toInt());
				m_Doc->PDF_Options.doMultiFile = static_cast<bool>(pg.attribute("doMultiFile", "0").toInt());
				m_Doc->PDF_Options.displayBookmarks = static_cast<bool>(pg.attribute("displayBookmarks", "0").toInt());
				m_Doc->PDF_Options.displayFullscreen = static_cast<bool>(pg.attribute("displayFullscreen", "0").toInt());
				m_Doc->PDF_Options.displayLayers = static_cast<bool>(pg.attribute("displayLayers", "0").toInt());
				m_Doc->PDF_Options.displayThumbs = static_cast<bool>(pg.attribute("displayThumbs", "0").toInt());
				m_Doc->PDF_Options.hideMenuBar = static_cast<bool>(pg.attribute("hideMenuBar", "0").toInt());
				m_Doc->PDF_Options.hideToolBar = static_cast<bool>(pg.attribute("hideToolBar", "0").toInt());
				m_Doc->PDF_Options.fitWindow = static_cast<bool>(pg.attribute("fitWindow", "0").toInt());
				m_Doc->PDF_Options.PageLayout = pg.attribute("PageLayout", "0").toInt();
				m_Doc->PDF_Options.openAction = pg.attribute("openAction", "");
				QDomNode PFO = PAGE.firstChild();
				while(!PFO.isNull())
				{
					QDomElement pdfF = PFO.toElement();
					if(pdfF.tagName() == "LPI")
					{
						struct LPIData lpo;
						lpo.Angle = pdfF.attribute("Angle").toInt();
						lpo.Frequency = pdfF.attribute("Frequency").toInt();
						lpo.SpotFunc = pdfF.attribute("SpotFunction").toInt();
						m_Doc->PDF_Options.LPISettings[pdfF.attribute("Color")] = lpo;
					}
					if(pdfF.tagName() == "Fonts")
					{
						if (!m_Doc->PDF_Options.EmbedList.contains(pdfF.attribute("Name")))
							m_Doc->PDF_Options.EmbedList.append(pdfF.attribute("Name"));
					}
					if(pdfF.tagName() == "Subset")
					{
						if (!m_Doc->PDF_Options.SubsetList.contains(pdfF.attribute("Name")))
							m_Doc->PDF_Options.SubsetList.append(pdfF.attribute("Name"));
					}
					if(pdfF.tagName() == "Effekte")
					{
						struct PDFPresentationData ef;
						ef.pageEffectDuration = pdfF.attribute("pageEffectDuration").toInt();
						ef.pageViewDuration = pdfF.attribute("pageViewDuration").toInt();
						ef.effectType = pdfF.attribute("effectType").toInt();
						ef.Dm = pdfF.attribute("Dm").toInt();
						ef.M = pdfF.attribute("M").toInt();
						ef.Di = pdfF.attribute("Di").toInt();
						m_Doc->PDF_Options.PresentVals.append(ef);
					}
					PFO = PFO.nextSibling();
				}
			}
			if(pg.tagName()=="DocItemAttributes")
			{
				QDomNode DIA = PAGE.firstChild();
				m_Doc->docItemAttributes.clear();
				while(!DIA.isNull())
				{
					QDomElement itemAttr = DIA.toElement();
					if(itemAttr.tagName() == "ItemAttribute")
					{
						ObjectAttribute objattr;
						objattr.name=itemAttr.attribute("Name");
						objattr.type=itemAttr.attribute("Type");
						objattr.value=itemAttr.attribute("Value");
						objattr.parameter=itemAttr.attribute("Parameter");
						objattr.relationship=itemAttr.attribute("Relationship");
						objattr.relationshipto=itemAttr.attribute("RelationshipTo");
						objattr.autoaddto=itemAttr.attribute("AutoAddTo");
						m_Doc->docItemAttributes.append(objattr);
					}
					DIA = DIA.nextSibling();
				}
			}
			if(pg.tagName()=="TablesOfContents")
			{
				QDomNode TOC = PAGE.firstChild();
				m_Doc->docToCSetups.clear();
				while(!TOC.isNull())
				{
					QDomElement tocElem = TOC.toElement();
					if(tocElem.tagName() == "TableOfContents")
					{
						ToCSetup tocsetup;
						tocsetup.name=tocElem.attribute("Name");
						tocsetup.itemAttrName=tocElem.attribute("ItemAttributeName");
						tocsetup.frameName=tocElem.attribute("FrameName");
						tocsetup.listNonPrintingFrames= QVariant(tocElem.attribute("ListNonPrinting")).toBool();
						tocsetup.textStyle=tocElem.attribute("Style");
						QString numberPlacement=tocElem.attribute("NumberPlacement");
						if (numberPlacement=="Beginning")
							tocsetup.pageLocation=Beginning;
						if (numberPlacement=="End")
							tocsetup.pageLocation=End;
						if (numberPlacement=="NotShown")
							tocsetup.pageLocation=NotShown;
						m_Doc->docToCSetups.append(tocsetup);
					}
					TOC = TOC.nextSibling();
				}
			}
			if(pg.tagName()=="Sections")
			{
				QDomNode Section = PAGE.firstChild();
				while(!Section.isNull())
				{
					QDomElement sectionElem = Section.toElement();
					if(sectionElem.tagName() == "Section")
					{
						struct DocumentSection newSection;
						newSection.number=sectionElem.attribute("Number").toInt();
						newSection.name=sectionElem.attribute("Name");
						newSection.fromindex=sectionElem.attribute("From").toInt();
						newSection.toindex=sectionElem.attribute("To").toInt();
						if (sectionElem.attribute("Type")=="Type_1_2_3")
							newSection.type=Type_1_2_3;
						if (sectionElem.attribute("Type")=="Type_i_ii_iii")
							newSection.type=Type_i_ii_iii;
						if (sectionElem.attribute("Type")=="Type_I_II_III")
							newSection.type=Type_I_II_III;
						if (sectionElem.attribute("Type")=="Type_a_b_c")
							newSection.type=Type_a_b_c;
						if (sectionElem.attribute("Type")=="Type_A_B_C")
							newSection.type=Type_A_B_C;
						if (sectionElem.attribute("Type")=="Type_None")
							newSection.type=Type_None;
						newSection.sectionstartindex=sectionElem.attribute("Start").toInt();
						newSection.reversed=static_cast<bool>(sectionElem.attribute("Reversed").toInt());
						newSection.active=static_cast<bool>(sectionElem.attribute("Active").toInt());
						m_Doc->sections.insert(newSection.number, newSection);
					}
					Section = Section.nextSibling();
				}
			}
			if (pg.tagName()=="HYPHEN")
			{
				QDomNode hyelm = pg.firstChild();
				while(!hyelm.isNull())
				{
					QDomElement hyElem = hyelm.toElement();
					if (hyElem.tagName()=="EXCEPTION")
					{
						QString word = hyElem.attribute("WORD");
						QString hyph = hyElem.attribute("HYPHENATED");
						m_Doc->docHyphenator->specialWords.insert(word, hyph);
					}
					else if (hyElem.tagName()=="IGNORE")
					{
						QString word = hyElem.attribute("WORD");
						m_Doc->docHyphenator->ignoredWords.insert(word);
					}
					hyelm = hyelm.nextSibling();
				}
			}
			if ((pg.tagName()=="PAGE") || (pg.tagName()=="MASTERPAGE"))
			{
				a = pg.attribute("NUM").toInt();
				PgNam = "";
				PgNam = pg.attribute("NAM", "");
				if (pg.tagName()=="MASTERPAGE" && PgNam.isEmpty())
				{
					qDebug() << "scribus134format: corrupted masterpage with empty name detected";
					PAGE=PAGE.nextSibling();
					continue;
				}
				//Pgc = m_Doc->pageCount;
				//AtFl = m_Doc->usesAutomaticTextFrames();
				if (PgNam.isEmpty())
				{
					//m_Doc->pageCount = Pgc;
					//m_Doc->Pages = &m_Doc->DocPages;
					//m_Doc->setUsesAutomaticTextFrames(AtFl);
					m_Doc->setMasterPageMode(false);
				}
				else
				{
					//m_Doc->pageCount = 0;
					//m_Doc->setUsesAutomaticTextFrames(false);
					//m_Doc->Pages = &m_Doc->MasterPages;
					m_Doc->setMasterPageMode(true);
				}
				//CB: Stop calling damn GUI code in loading docs! IT doesnt *look* like
				//this makes a difference apart from being faster, of course.
				//ScMW->slotNewPage(a);
				//Apage = m_Doc->Pages.at(a);
				if (PgNam.isEmpty())
				{
					Apage = m_Doc->addPage(a);
					//m_Doc->DocPages = m_Doc->Pages;
					//++m_Doc->pageCount;
				}
				else
				{
					Apage = m_Doc->addMasterPage(a, PgNam);
					//Apage->setPageName(PgNam);
					//m_Doc->MasterNames[PgNam] = a;
					//m_Doc->MasterPages = m_Doc->Pages;
					//m_Doc->pageCount = Pgc;
				}
				//m_Doc->setUsesAutomaticTextFrames(AtFl);
				Apage->LeftPg=pg.attribute("LEFT", "0").toInt();
				QString Mus = "";
				Mus = pg.attribute("MNAM","Normal");
				if (!m_Doc->masterPageMode())
					Apage->MPageNam = Mus;
				else
					Apage->MPageNam = "";
				if (pg.hasAttribute("Size"))
					Apage->m_pageSize = pg.attribute("Size");
				if (pg.hasAttribute("Orientation"))
					Apage->PageOri = pg.attribute("Orientation").toInt();
				Apage->setXOffset(ScCLocale::toDoubleC(pg.attribute("PAGEXPOS")));
				Apage->setYOffset(ScCLocale::toDoubleC(pg.attribute("PAGEYPOS")));
				if (pg.hasAttribute("PAGEWIDTH"))
					Apage->setWidth(ScCLocale::toDoubleC(pg.attribute("PAGEWIDTH")));
				else
					Apage->setWidth(ScCLocale::toDoubleC(pg.attribute("PAGEWITH")));
				Apage->setHeight(ScCLocale::toDoubleC(pg.attribute("PAGEHEIGHT")));
				Apage->setInitialHeight(Apage->height());
				Apage->setInitialWidth(Apage->width());
				Apage->initialMargins.Top = qMax(0.0, ScCLocale::toDoubleC( pg.attribute("BORDERTOP")));
				Apage->initialMargins.Bottom =  qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERBOTTOM")));
				Apage->initialMargins.Left =  qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERLEFT")));
				Apage->initialMargins.Right =  qMax(0.0,ScCLocale::toDoubleC( pg.attribute("BORDERRIGHT")));
				Apage->marginPreset = pg.attribute("PRESET", "0").toInt();
				Apage->Margins.Top = Apage->initialMargins.Top;
				Apage->Margins.Bottom = Apage->initialMargins.Bottom;
				m_Doc->setMasterPageMode(false);
				//m_Doc->Pages=&m_Doc->DocPages;
				// guides reading
				tmp = "";
				Apage->guides.setHorizontalAutoGap(ScCLocale::toDoubleC(pg.attribute("AGhorizontalAutoGap"), 0.0));
				Apage->guides.setVerticalAutoGap(ScCLocale::toDoubleC(pg.attribute("AGverticalAutoGap"), 0.0));
				Apage->guides.setHorizontalAutoCount(pg.attribute("AGhorizontalAutoCount", "0").toInt());
				Apage->guides.setVerticalAutoCount(pg.attribute("AGverticalAutoCount", "0").toInt());
				Apage->guides.setHorizontalAutoRefer(pg.attribute("AGhorizontalAutoRefer", "0").toInt());
				Apage->guides.setVerticalAutoRefer(pg.attribute("AGverticalAutoRefer", "0").toInt());
				GuideManagerIO::readVerticalGuides(pg.attribute("VerticalGuides"),
						Apage,
						GuideManagerCore::Standard,
						pg.hasAttribute("NumVGuides"));
				GuideManagerIO::readHorizontalGuides(pg.attribute("HorizontalGuides"),
						Apage,
						GuideManagerCore::Standard,
						pg.hasAttribute("NumHGuides"));
				GuideManagerIO::readSelection(pg.attribute("AGSelection"), Apage);
				
			}
			if ((pg.tagName()=="PAGEOBJECT") || (pg.tagName()=="MASTEROBJECT") || (pg.tagName()=="FRAMEOBJECT"))
			{
					if ((pg.tagName()=="PAGEOBJECT") || (pg.tagName()=="FRAMEOBJECT"))
					{
						//m_Doc->Items = m_Doc->DocItems;
						//m_Doc->Pages = &m_Doc->DocPages;
						m_Doc->setMasterPageMode(false);
					}
					else
					{
						//m_Doc->Items = m_Doc->MasterItems;
						//m_Doc->Pages = &m_Doc->MasterPages;
						m_Doc->setMasterPageMode(true);
					}
					int docGc = m_Doc->GroupCounter, pagenr = -1;
					if ((!pg.attribute("OnMasterPage").isEmpty()) && (pg.tagName()=="MASTEROBJECT"))
					{
						m_Doc->setCurrentPage(m_Doc->MasterPages.at(m_Doc->MasterNames[pg.attribute("OnMasterPage")]));
						pagenr = -2;
					}
					/*m_Doc->GroupCounter = 0;*/
					Neu = PasteItem(&pg, m_Doc, fileDir, pagenr);
					Neu->setRedrawBounding();
					if (pg.tagName()=="MASTEROBJECT")
						Neu->OwnPage = m_Doc->OnPage(Neu);
					else
						Neu->OwnPage = pg.attribute("OwnPage").toInt();
					if (pg.tagName()=="PAGEOBJECT")
						Neu->OnMasterPage = "";
					/*m_Doc->GroupCounter = docGc;*/
					tmpf = pg.attribute("IFONT", m_Doc->toolSettings.defFont);
					m_AvailableFonts->findFont(tmpf, m_Doc);
					QDomNode IT=pg.firstChild();
					LastStyles * last = new LastStyles();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = ScCLocale::toDoubleC( it.attribute("RAMP"), 0.0);
							int shade = it.attribute("SHADE", "100").toInt();
							double opa = ScCLocale::toDoubleC( it.attribute("TRANS"), 1.0);
							Neu->fill_gradient.addStop(SetColor(m_Doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							GetItemText(&it, m_Doc, Neu, last);
						else if (it.tagName()=="para")
						{
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PARSEP);
							ParagraphStyle newStyle;
							PrefsManager* prefsManager=PrefsManager::instance();
							readParagraphStyle(newStyle, it, prefsManager->appPrefs.AvailFonts, m_Doc);
							Neu->itemText.setStyle(Neu->itemText.length()-1, newStyle);
							Neu->itemText.setCharStyle(Neu->itemText.length()-1, 1, last->Style);
						}
						else if (it.tagName()=="tab")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::TAB);
						else if (it.tagName()=="breakline")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::LINEBREAK);
						else if (it.tagName()=="breakcol")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::COLBREAK);
						else if (it.tagName()=="breakframe")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::FRAMEBREAK);
						else if (it.tagName()=="nbhyphen")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::NBHYPHEN);
						else if (it.tagName()=="nbspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::NBSPACE);
						else if (it.tagName()=="zwnbspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::ZWNBSPACE);
						else if (it.tagName()=="zwspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::ZWSPACE);
						else if (it.tagName()=="var")
						{
							if (it.attribute("name") == "pgno")
								Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PAGENUMBER);
							else
								Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PAGECOUNT);
							CharStyle newStyle;
							GetCStyle(&it, m_Doc, newStyle);
							Neu->itemText.setCharStyle(Neu->itemText.length()-1, 1, newStyle);
							last->StyleStart = Neu->itemText.length()-1;
							last->Style = newStyle;
						}

						//CB PageItemAttributes
						if(it.tagName()=="PageItemAttributes")
						{
							QDomNode PIA = it.firstChild();
							ObjAttrVector pageItemAttributes;
							while(!PIA.isNull())
							{
								QDomElement itemAttr = PIA.toElement();
								if(itemAttr.tagName() == "ItemAttribute")
								{
									ObjectAttribute objattr;
									objattr.name=itemAttr.attribute("Name");
									objattr.type=itemAttr.attribute("Type");
									objattr.value=itemAttr.attribute("Value");
									objattr.parameter=itemAttr.attribute("Parameter");
									objattr.relationship=itemAttr.attribute("Relationship");
									objattr.relationshipto=itemAttr.attribute("RelationshipTo");
									objattr.autoaddto=itemAttr.attribute("AutoAddTo");
									pageItemAttributes.append(objattr);
								}
								PIA = PIA.nextSibling();
							}
							Neu->setObjectAttributes(&pageItemAttributes);
						}
						IT=IT.nextSibling();
					}
					if (Neu->asTextFrame())
					{
/*
 QString dbg("");
						for (int i=0; i < Neu->itemText.length(); ++i)
						{
							dbg += Neu->itemText.text(i,1);
							if (Neu->itemText.item(i)->effects() & ScStyle_HyphenationPossible)
								dbg += "~";
						}
						qDebug("scribus134format: read itemtext '%s'", dbg.latin1());
*/					}
					if (Neu->asPathText())
					{
						Neu->updatePolyClip();
						Neu->Frame = true;
					}
					delete last;
					if (Neu->fill_gradient.Stops() == 0)
					{
						const ScColor& col1 = m_Doc->PageColors[m_Doc->toolSettings.dBrush];
						const ScColor& col2 = m_Doc->PageColors[m_Doc->toolSettings.dPen];
						Neu->fill_gradient.addStop(ScColorEngine::getRGBColor(col1, m_Doc), 0.0, 0.5, 1.0, m_Doc->toolSettings.dBrush, 100);
						Neu->fill_gradient.addStop(ScColorEngine::getRGBColor(col2, m_Doc), 1.0, 0.5, 1.0, m_Doc->toolSettings.dPen, 100);
					}
//					Neu->Language = ScMW->GetLang(pg.attribute("LANGUAGE", m_Doc->Language));
					Neu->isAutoText = static_cast<bool>(pg.attribute("AUTOTEXT").toInt());
					Neu->isEmbedded = static_cast<bool>(pg.attribute("isInline", "0").toInt());
					Neu->gXpos = ScCLocale::toDoubleC( pg.attribute("gXpos"), 0.0);
					Neu->gYpos = ScCLocale::toDoubleC( pg.attribute("gYpos"), 0.0);
					Neu->gWidth = ScCLocale::toDoubleC( pg.attribute("gWidth"), Neu->width());
					Neu->gHeight = ScCLocale::toDoubleC( pg.attribute("gHeight"), Neu->height());
					if (Neu->isAutoText)
						m_Doc->LastAuto = Neu;
					// first of linked chain?
					if (pg.tagName()=="PAGEOBJECT")
					{
						if (pg.attribute("NEXTITEM").toInt() != -1)
							itemNext[Neu->ItemNr] = pg.attribute("NEXTITEM").toInt();
					}
					else if (pg.tagName()=="MASTEROBJECT")
					{
						if (pg.attribute("NEXTITEM").toInt() != -1)
							itemNextM[Neu->ItemNr] = pg.attribute("NEXTITEM").toInt();
					}
					/* not sure if we want that...
					else if (pg.tagName()=="FRAMEOBJECT")
					{
						if (pg.attribute("NEXTITEM").toInt() != -1)
							itemNextF[Neu->ItemNr] = pg.attribute("NEXTITEM").toInt();
					}*/

					if (pg.tagName()=="FRAMEOBJECT")
					{
						m_Doc->FrameItems.append(m_Doc->Items->takeAt(Neu->ItemNr));
						Neu->ItemNr = m_Doc->FrameItems.count()-1;
					}
					if (Neu->isTableItem)
					{
						if (pg.tagName()=="PAGEOBJECT")
						{
							TableItems.append(Neu);
							TableID.insert(pg.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
						}
						else if (pg.tagName()=="FRAMEOBJECT")
						{
							TableItemsF.append(Neu);
							TableIDF.insert(pg.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
						}
						else
						{
							TableItemsM.append(Neu);
							TableIDM.insert(pg.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
						}
					}
					Neu->isGroupControl = static_cast<bool>(pg.attribute("isGroupControl", "0").toInt());
					if (Neu->isGroupControl)
					{
						if (Neu->Groups.count() == 0) // Sanity check for some broken files created using buggy development versions.
						{
							Neu->isGroupControl = false;
							Neu->setFillColor("None");
						}
						else
						{
							if (pg.tagName()=="PAGEOBJECT")
								groupID.insert(Neu, pg.attribute("groupsLastItem", "0").toInt()+Neu->ItemNr);
							else if (pg.tagName()=="FRAMEOBJECT")
								groupIDF.insert(Neu, pg.attribute("groupsLastItem", "0").toInt()+Neu->ItemNr);
							else
								groupIDM.insert(Neu, pg.attribute("groupsLastItem", "0").toInt()+Neu->ItemNr);
						}
					}
					m_Doc->setMasterPageMode(false);
					counter++;
				}
			PAGE=PAGE.nextSibling();
		}
		PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="Bookmark")
			{
				int elem = pg.attribute("Element").toInt();
				if (elem < m_Doc->Items->count())
				{
					bok.Title = pg.attribute("Title");
					bok.Text = pg.attribute("Text");
					bok.Aktion = pg.attribute("Aktion");
					bok.ItemNr = pg.attribute("ItemNr").toInt();
					bok.PageObject = m_Doc->Items->at(elem);
					bok.First = pg.attribute("First").toInt();
					bok.Last = pg.attribute("Last").toInt();
					bok.Prev = pg.attribute("Prev").toInt();
					bok.Next = pg.attribute("Next").toInt();
					bok.Parent = pg.attribute("Parent").toInt();
					m_Doc->BookMarks.append(bok);
				}
			}
			if(pg.tagName()=="Pattern")
			{
				QMap<PageItem*, int> groupID2;
				QMap<int,int> TableID2;
				QList<PageItem*> TableItems2;
				ScPattern pat;
				QDomNode pa = PAGE.firstChild();
				uint ac = m_Doc->Items->count();
				bool savedAlignGrid = m_Doc->useRaster;
				bool savedAlignGuides = m_Doc->SnapGuides;
				m_Doc->useRaster = false;
				m_Doc->SnapGuides = false;
				while(!pa.isNull())
				{
					QDomElement pite = pa.toElement();
					m_Doc->setMasterPageMode(false);
					int docGc = m_Doc->GroupCounter;
					/*m_Doc->GroupCounter = 0;*/
					Neu = PasteItem(&pite, m_Doc, fileDir);
					Neu->setRedrawBounding();
					Neu->OwnPage = pite.attribute("OwnPage").toInt();
					Neu->OnMasterPage = "";
					/*m_Doc->GroupCounter = docGc;*/
					tmpf = pite.attribute("IFONT", m_Doc->toolSettings.defFont);
					m_AvailableFonts->findFont(tmpf, m_Doc);
					QDomNode IT=pite.firstChild();
					LastStyles * last = new LastStyles();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = ScCLocale::toDoubleC( it.attribute("RAMP"), 0.0);
							int shade = it.attribute("SHADE", "100").toInt();
							double opa = ScCLocale::toDoubleC( it.attribute("TRANS"), 1.0);
							Neu->fill_gradient.addStop(SetColor(m_Doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							GetItemText(&it, m_Doc, Neu, last);
						else if (it.tagName()=="para")
						{
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PARSEP);
							ParagraphStyle newStyle;
							PrefsManager* prefsManager=PrefsManager::instance();
							readParagraphStyle(newStyle, it, prefsManager->appPrefs.AvailFonts, m_Doc);
							Neu->itemText.setStyle(Neu->itemText.length()-1, newStyle);
							Neu->itemText.setCharStyle(Neu->itemText.length()-1, 1, last->Style);
						}
						else if (it.tagName()=="tab")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::TAB);
						else if (it.tagName()=="breakline")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::LINEBREAK);
						else if (it.tagName()=="breakcol")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::COLBREAK);
						else if (it.tagName()=="breakframe")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::FRAMEBREAK);
						else if (it.tagName()=="nbhyphen")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::NBHYPHEN);
						else if (it.tagName()=="nbspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::NBSPACE);
						else if (it.tagName()=="zwnbspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::ZWNBSPACE);
						else if (it.tagName()=="zwspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::ZWSPACE);
						else if (it.tagName()=="var")
						{
							if (it.attribute("name") == "pgno")
								Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PAGENUMBER);
							else
								Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PAGECOUNT);
							CharStyle newStyle;
							GetCStyle(&it, m_Doc, newStyle);
							Neu->itemText.setCharStyle(Neu->itemText.length()-1, 1, newStyle);
							last->StyleStart = Neu->itemText.length()-1;
							last->Style = newStyle;
						}
						
						if(it.tagName()=="PageItemAttributes")
						{
							QDomNode PIA = it.firstChild();
							ObjAttrVector pageItemAttributes;
							while(!PIA.isNull())
							{
								QDomElement itemAttr = PIA.toElement();
								if(itemAttr.tagName() == "ItemAttribute")
								{
									ObjectAttribute objattr;
									objattr.name=itemAttr.attribute("Name");
									objattr.type=itemAttr.attribute("Type");
									objattr.value=itemAttr.attribute("Value");
									objattr.parameter=itemAttr.attribute("Parameter");
									objattr.relationship=itemAttr.attribute("Relationship");
									objattr.relationshipto=itemAttr.attribute("RelationshipTo");
									objattr.autoaddto=itemAttr.attribute("AutoAddTo");
									pageItemAttributes.append(objattr);
								}
								PIA = PIA.nextSibling();
							}
							Neu->setObjectAttributes(&pageItemAttributes);
						}
						IT=IT.nextSibling();
					}
					delete last;
					if (Neu->asPathText())
					{
						Neu->updatePolyClip();
						Neu->Frame = true;
					}
					if (Neu->fill_gradient.Stops() == 0)
					{
						const ScColor& col1 = m_Doc->PageColors[m_Doc->toolSettings.dBrush];
						const ScColor& col2 = m_Doc->PageColors[m_Doc->toolSettings.dPen];
						Neu->fill_gradient.addStop(ScColorEngine::getRGBColor(col1, m_Doc), 0.0, 0.5, 1.0, m_Doc->toolSettings.dBrush, 100);
						Neu->fill_gradient.addStop(ScColorEngine::getRGBColor(col2, m_Doc), 1.0, 0.5, 1.0, m_Doc->toolSettings.dPen, 100);
					}
					Neu->isAutoText = static_cast<bool>(pite.attribute("AUTOTEXT").toInt());
					Neu->isEmbedded = static_cast<bool>(pite.attribute("isInline", "0").toInt());
					Neu->gXpos = ScCLocale::toDoubleC( pite.attribute("gXpos"), 0.0);
					Neu->gYpos = ScCLocale::toDoubleC( pite.attribute("gYpos"), 0.0);
					Neu->gWidth  = ScCLocale::toDoubleC( pite.attribute("gWidth"), Neu->width());
					Neu->gHeight = ScCLocale::toDoubleC( pite.attribute("gHeight"), Neu->height());
					if (Neu->isTableItem)
					{
						TableItems2.append(Neu);
						TableID2.insert(pite.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
					}
					Neu->isGroupControl = static_cast<bool>(pite.attribute("isGroupControl", "0").toInt());
					if (Neu->isGroupControl)
					{
						if (Neu->Groups.count() == 0) // Sanity check for some broken files created using buggy development versions.
						{
							Neu->isGroupControl = false;
							Neu->setFillColor("None");
						}
						else
							groupID2.insert(Neu, pite.attribute("groupsLastItem", "0").toInt()+Neu->ItemNr);
					}
					pa = pa.nextSibling();
				}
				if (groupID2.count() != 0)
				{
					QMap<PageItem*, int>::Iterator it;
					for (it = groupID2.begin(); it != groupID2.end(); ++it)
					{
						it.key()->groupsLastItem = m_Doc->Items->at(it.value());
					}
				}
				if (TableItems2.count() != 0)
				{
					for (int ttc = 0; ttc < TableItems2.count(); ++ttc)
					{
						PageItem* ta = TableItems2.at(ttc);
						if (ta->TopLinkID != -1)
							ta->TopLink = m_Doc->Items->at(TableID2[ta->TopLinkID]);
						else
							ta->TopLink = 0;
						if (ta->LeftLinkID != -1)
							ta->LeftLink = m_Doc->Items->at(TableID2[ta->LeftLinkID]);
						else
							ta->LeftLink = 0;
						if (ta->RightLinkID != -1)
							ta->RightLink = m_Doc->Items->at(TableID2[ta->RightLinkID]);
						else
							ta->RightLink = 0;
						if (ta->BottomLinkID != -1)
							ta->BottomLink = m_Doc->Items->at(TableID2[ta->BottomLinkID]);
						else
							ta->BottomLink = 0;
					}
				}
				m_Doc->useRaster = savedAlignGrid;
				m_Doc->SnapGuides = savedAlignGuides;
				uint ae = m_Doc->Items->count();
				pat.setDoc(m_Doc);
				pat.width = ScCLocale::toDoubleC( pg.attribute("width"), 0.0);
				pat.height = ScCLocale::toDoubleC( pg.attribute("height"), 0.0);
				pat.scaleX = ScCLocale::toDoubleC( pg.attribute("scaleX"), 0.0);
				pat.scaleY = ScCLocale::toDoubleC( pg.attribute("scaleY"), 0.0);
				pat.xoffset = ScCLocale::toDoubleC( pg.attribute("xoffset"), 0.0);
				pat.yoffset = ScCLocale::toDoubleC( pg.attribute("yoffset"), 0.0);
				PageItem* currItem = m_Doc->Items->at(ac);
				pat.pattern = currItem->DrawObj_toImage();
				pat.pattern = pat.pattern.copy(-pat.xoffset, -pat.yoffset, pat.width, pat.height);
				for (uint as = ac; as < ae; ++as)
				{
					Neu = m_Doc->Items->takeAt(ac);
					Neu->moveBy(pat.xoffset, pat.yoffset, true);
					Neu->gXpos += pat.xoffset;
					Neu->gYpos += pat.yoffset;
					Neu->ItemNr = pat.items.count();
					pat.items.append(Neu);
				}
				m_Doc->docPatterns.insert(pg.attribute("Name"), pat);
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	if (TableItemsF.count() != 0)
	{
		for (int ttc = 0; ttc < TableItemsF.count(); ++ttc)
		{
			PageItem* ta = TableItemsF.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->FrameItems.at(TableIDF[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->FrameItems.at(TableIDF[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->FrameItems.at(TableIDF[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->FrameItems.at(TableIDF[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	if (TableItemsM.count() != 0)
	{
		for (int ttc = 0; ttc < TableItemsM.count(); ++ttc)
		{
			PageItem* ta = TableItemsM.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->MasterItems.at(TableIDM[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->MasterItems.at(TableIDM[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->MasterItems.at(TableIDM[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->MasterItems.at(TableIDM[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	if (TableItems.count() != 0)
	{
		for (int ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	if (groupIDF.count() != 0)
	{
		QMap<PageItem*, int>::Iterator it;
		for (it = groupIDF.begin(); it != groupIDF.end(); ++it)
		{
			it.key()->groupsLastItem = m_Doc->FrameItems.at(it.value());
		}
	}
	if (groupID.count() != 0)
	{
		QMap<PageItem*, int>::Iterator it;
		for (it = groupID.begin(); it != groupID.end(); ++it)
		{
			it.key()->groupsLastItem = m_Doc->DocItems.at(it.value());
		}
	}
	if (groupIDM.count() != 0)
	{
		QMap<PageItem*, int>::Iterator it;
		for (it = groupIDM.begin(); it != groupIDM.end(); ++it)
		{
			it.key()->groupsLastItem = m_Doc->MasterItems.at(it.value());
		}
	}
	m_Doc->setActiveLayer(layerToSetActive);
	m_Doc->setMasterPageMode(false);
	m_Doc->reformPages();

	if (m_Doc->Layers.count() == 0)
		m_Doc->Layers.newLayer( QObject::tr("Background") );

	// reestablish textframe links
	if (itemNext.count() != 0)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
		{
			if (lc.value() >= 0)
			{
				PageItem * Its = m_Doc->DocItems.at(lc.key());
				PageItem * Itn = m_Doc->DocItems.at(lc.value());
				if (Itn->prevInChain() || Its->nextInChain()) 
				{
					qDebug() << "scribus134format: corruption in linked textframes detected";
					continue;
				}
				Its->link(Itn);
			}
		}
	}
	
	if (itemNextM.count() != 0)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNextM.begin(); lc != itemNextM.end(); ++lc)
		{
			if (lc.value() >= 0)
			{
				PageItem * Its = m_Doc->MasterItems.at(lc.key());
				PageItem * Itn = m_Doc->MasterItems.at(lc.value());
				if (Itn->prevInChain() || Its->nextInChain()) 
				{
					qDebug() << "scribus134format: corruption in linked textframes detected";
					continue;
				}
				Its->link(Itn);
			}
		}
	}
	
	// reestablish first/lastAuto
	m_Doc->FirstAuto = m_Doc->LastAuto;
	if (m_Doc->LastAuto)
	{
		while (m_Doc->LastAuto->nextInChain())
			m_Doc->LastAuto = m_Doc->LastAuto->nextInChain();
		while (m_Doc->FirstAuto->prevInChain())
			m_Doc->FirstAuto = m_Doc->FirstAuto->prevInChain();
	}

	// start auto save timer if needed
	if (m_Doc->AutoSave  && ScCore->usingGUI())
		m_Doc->autoSaveTimer->start(m_Doc->AutoSaveTime);
	
	if (m_mwProgressBar!=0)
		m_mwProgressBar->setValue(DOC.childNodes().count());
	return true;
// 	return false;
}


// Low level plugin API
int scribus134format_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scribus134format_getPlugin()
{
	Scribus134Format* plug = new Scribus134Format();
	Q_CHECK_PTR(plug);
	return plug;
}

void scribus134format_freePlugin(ScPlugin* plugin)
{
	Scribus134Format* plug = dynamic_cast<Scribus134Format*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}


namespace {
	const int NOVALUE = -16000;
	
	void fixLegacyCharStyle(CharStyle& cstyle) 
	{
		if (! cstyle.font().usable())
			cstyle.resetFont();
		if (cstyle.fontSize() <= -16000 / 10)
			cstyle.resetFontSize();
//		if (cstyle.effects() == 65535)
//			cstyle.resetEffects();
		if (cstyle.fillColor().isEmpty())
			cstyle.resetFillColor();
		if (cstyle.fillShade() <= -16000)
			cstyle.resetFillShade();
		if (cstyle.strokeColor().isEmpty())
			cstyle.resetStrokeColor();
		if (cstyle.strokeShade() <= -16000)
			cstyle.resetStrokeShade();
		if (cstyle.shadowXOffset() <= -16000 / 10)
			cstyle.resetShadowXOffset();
		if (cstyle.shadowYOffset() <= -16000 / 10)
			cstyle.resetShadowYOffset();
		if (cstyle.outlineWidth() <= -16000 / 10)
			cstyle.resetOutlineWidth();
		if (cstyle.underlineOffset() <= -16000 / 10)
			cstyle.resetUnderlineOffset();
		if (cstyle.underlineWidth() <= -16000 / 10)
			cstyle.resetUnderlineWidth();
		if (cstyle.strikethruOffset() <= -16000 / 10)
			cstyle.resetStrikethruOffset();
		if (cstyle.strikethruWidth() <= -16000 / 10)
			cstyle.resetStrikethruWidth();
		if (cstyle.scaleH() <= -16000 / 10)
			cstyle.resetScaleH();
		if (cstyle.scaleV() <= -16000 / 10)
			cstyle.resetScaleV();
		if (cstyle.baselineOffset() <= -16000 / 10)
			cstyle.resetBaselineOffset();
		if (cstyle.tracking() <= -16000 / 10)
			cstyle.resetTracking();
	}
	
	void fixLegacyParStyle(ParagraphStyle& pstyle) 
	{
		if (pstyle.lineSpacing() <= -16000)
			pstyle.resetLineSpacing();
		if (pstyle.leftMargin() <= -16000)
			pstyle.resetLeftMargin();
		if (pstyle.rightMargin() <= -16000)
			pstyle.resetRightMargin();
		if (pstyle.firstIndent() <= -16000)
			pstyle.resetFirstIndent();
		if (pstyle.alignment() < 0)
			pstyle.resetAlignment();
		if (pstyle.gapBefore() <= -16000)
			pstyle.resetGapBefore();
		if (pstyle.gapAfter() <= -16000)
			pstyle.resetGapAfter();
		if (pstyle.dropCapLines() < 0)
			pstyle.resetDropCapLines();
		if (pstyle.dropCapOffset() <= -16000)
			pstyle.resetDropCapOffset();
		fixLegacyCharStyle(pstyle.charStyle());
	}
	
}// namespace


void Scribus134Format::GetCStyle(const QDomElement *it, ScribusDoc *doc, CharStyle & newStyle)
{
	if (it->hasAttribute("CNAME"))
		newStyle.setName(it->attribute("CNAME"));
	// The default style attribute must be correctly set before trying to assign a parent
	if (newStyle.hasName() && it->hasAttribute("DefaultStyle"))
		newStyle.setDefaultStyle(it->attribute("DefaultStyle").toInt());
	else if (newStyle.name() == CommonStrings::DefaultCharacterStyle || newStyle.name() == CommonStrings::trDefaultCharacterStyle)
		newStyle.setDefaultStyle(true);
	else
		newStyle.setDefaultStyle(false);
	if (it->hasAttribute("CPARENT"))
		newStyle.setParent(it->attribute("CPARENT"));
	if (it->hasAttribute("FONT"))
		newStyle.setFont(m_AvailableFonts->findFont(it->attribute("FONT"),doc));
	
	if (it->hasAttribute("FONTSIZE"))
		newStyle.setFontSize(qRound(ScCLocale::toDoubleC(it->attribute("FONTSIZE")) * 10));
	
	if (it->hasAttribute("FCOLOR"))
		newStyle.setFillColor(it->attribute("FCOLOR"));
	
	if (it->hasAttribute("KERN"))
		newStyle.setTracking(qRound(ScCLocale::toDoubleC(it->attribute("KERN")) * 10));
	
	if (it->hasAttribute("FSHADE"))
		newStyle.setFillShade(it->attribute("FSHADE").toInt());
	
	if (it->hasAttribute("EFFECTS"))
		newStyle.setFeatures(static_cast<StyleFlag>(it->attribute("EFFECTS").toInt()).featureList());
	
	if (it->hasAttribute("EFFECT"))
		newStyle.setFeatures(static_cast<StyleFlag>(it->attribute("EFFECT").toInt()).featureList());
	
	if (it->hasAttribute("FEATURES"))
		newStyle.setFeatures(it->attribute("FEATURES").split( " ", QString::SkipEmptyParts));
	
	if (it->hasAttribute("SCOLOR"))
		newStyle.setStrokeColor(it->attribute("SCOLOR", CommonStrings::None));
	
	if (it->hasAttribute("SSHADE"))
		newStyle.setStrokeShade(it->attribute("SSHADE").toInt());
	
	if (it->hasAttribute("SCALEH"))
		newStyle.setScaleH(qRound(ScCLocale::toDoubleC(it->attribute("SCALEH")) * 10));
	
	if (it->hasAttribute("SCALEV"))
		newStyle.setScaleV(qRound(ScCLocale::toDoubleC(it->attribute("SCALEV")) * 10));
	
	if (it->hasAttribute("BASEO"))
		newStyle.setBaselineOffset(qRound(ScCLocale::toDoubleC(it->attribute("BASEO")) * 10));
	
	if (it->hasAttribute("TXTSHX"))
		newStyle.setShadowXOffset(qRound(ScCLocale::toDoubleC(it->attribute("TXTSHX")) * 10));
	
	if (it->hasAttribute("TXTSHY"))
		newStyle.setShadowYOffset(qRound(ScCLocale::toDoubleC(it->attribute("TXTSHY")) * 10));
	
	if (it->hasAttribute("TXTOUT"))
		newStyle.setOutlineWidth(qRound(ScCLocale::toDoubleC(it->attribute("TXTOUT")) * 10));
	
	if (it->hasAttribute("TXTULP"))
		newStyle.setUnderlineOffset(qRound(ScCLocale::toDoubleC(it->attribute("TXTULP")) * 10));
	
	if (it->hasAttribute("TXTULW"))
		newStyle.setUnderlineWidth(qRound(ScCLocale::toDoubleC(it->attribute("TXTULW")) * 10));
	
	if (it->hasAttribute("TXTSTP"))
		newStyle.setStrikethruOffset(qRound(ScCLocale::toDoubleC(it->attribute("TXTSTP")) * 10));
	
	if (it->hasAttribute("TXTSTW"))
		newStyle.setStrikethruWidth(qRound(ScCLocale::toDoubleC(it->attribute("TXTSTW")) * 10));

	if (it->hasAttribute("SHORTCUT"))
		newStyle.setShortcut(it->attribute("SHORTCUT"));

	if (it->hasAttribute("wordTrack"))
		newStyle.setWordTracking(ScCLocale::toDoubleC(it->attribute("wordTrack")));

}	

void Scribus134Format::GetItemText(QDomElement *it, ScribusDoc *doc, PageItem* obj, LastStyles* last, bool impo, bool VorLFound)
{
	QString tmp2, tmpf;
	CharStyle newStyle;
	
	GetCStyle(it, doc, newStyle);

	if (it->hasAttribute("Unicode"))
	{
		tmp2 = QChar(it->attribute("Unicode").toUInt());
	}
	else
	{
		tmp2 = it->attribute("CH");
		
		// legacy stuff:
		tmp2.replace(QRegExp("\r"), QChar(13));
		tmp2.replace(QRegExp("\n"), QChar(13));
		tmp2.replace(QRegExp("\t"), QChar(9));
	}
	
	// more legacy stuff:
	
	if (it->hasAttribute("CFONT"))
		newStyle.setFont(m_AvailableFonts->findFont(it->attribute("CFONT"), doc));
	
	if (it->hasAttribute("CSIZE"))
		newStyle.setFontSize(qRound(ScCLocale::toDoubleC(it->attribute("CSIZE")) * 10));
	
	if (it->hasAttribute("CCOLOR"))
		newStyle.setFillColor(it->attribute("CCOLOR"));
	
	if (it->hasAttribute("CEXTRA"))
		newStyle.setTracking(qRound(ScCLocale::toDoubleC(it->attribute("CEXTRA")) / ScCLocale::toDoubleC(it->attribute("CSIZE")) * 1000.0));
	else if (it->hasAttribute("CKERN"))
		newStyle.setTracking(it->attribute("CKERN").toInt());
	
	if (it->hasAttribute("CSHADE"))
		newStyle.setFillShade(it->attribute("CSHADE").toInt());
	
	if (it->hasAttribute("CSTYLE"))
		newStyle.setFeatures(static_cast<StyleFlag>(it->attribute("CSTYLE").toInt()).featureList());

	QString pstylename = it->attribute("PSTYLE", "");
	int calign = it->attribute("CALIGN", "-1").toInt();		
	
	int ab = it->attribute("CAB", "-1").toInt();
	if (ab >= 5) {
		pstylename = doc->paragraphStyles()[ab-5].name();
		calign = -1;
	}
	else if (ab >= 0) {
		pstylename = "";
		calign = ab;
	}
	
	if (it->hasAttribute("CSTROKE"))
		newStyle.setStrokeColor(it->attribute("CSTROKE", CommonStrings::None));
	
	if (it->hasAttribute("CSHADE2"))
		newStyle.setStrokeShade(it->attribute("CSHADE2", "100").toInt());
	
	if (it->hasAttribute("CSCALE"))
		newStyle.setScaleH(qMin(qMax(qRound(ScCLocale::toDoubleC(it->attribute("CSCALE"), 100.0) * 10), 100), 4000));
	
	if (it->hasAttribute("CSCALEV"))
		newStyle.setScaleV(qMin(qMax(qRound(ScCLocale::toDoubleC(it->attribute("CSCALEV"), 100.0) * 10), 100), 4000));
	
	if (it->hasAttribute("CBASE"))
		newStyle.setBaselineOffset(qRound(ScCLocale::toDoubleC(it->attribute("CBASE")) * 10));
	
	if (it->hasAttribute("CSHX"))
		newStyle.setShadowXOffset(qRound(ScCLocale::toDoubleC(it->attribute("CSHX"), 5.0) * 10));
	
	if (it->hasAttribute("CSHY"))
		newStyle.setShadowYOffset(qRound(ScCLocale::toDoubleC(it->attribute("CSHY"), -5.0) * 10));
	
	if (it->hasAttribute("COUT"))
		newStyle.setOutlineWidth(qRound(ScCLocale::toDoubleC(it->attribute("COUT"), 1.0) * 10));
	
	if (it->hasAttribute("CULP"))
		newStyle.setUnderlineOffset(qRound(ScCLocale::toDoubleC(it->attribute("CULP"), -0.1) * 10));
	
	if (it->hasAttribute("CULW"))
		newStyle.setUnderlineWidth(qRound(ScCLocale::toDoubleC(it->attribute("CULW"), -0.1) * 10));
	
	
	if (it->hasAttribute("CSTP"))
		newStyle.setStrikethruOffset(qRound(ScCLocale::toDoubleC(it->attribute("CSTP"), -0.1) * 10));
	
	if (it->hasAttribute("CSTW"))
		newStyle.setStrikethruWidth(qRound(ScCLocale::toDoubleC(it->attribute("CSTW"), -0.1) * 10));
	
	fixLegacyCharStyle(newStyle);
	
	if (impo && ab >= 0 && VorLFound)
		last->ParaStyle = DoVorl[ab].toInt();
	else
		last->ParaStyle = pstylename;
	// end of legacy stuff
	
	int iobj = it->attribute("COBJ", "-1").toInt();

	for (int cxx=0; cxx<tmp2.length(); ++cxx)
	{
		QChar ch = tmp2.at(cxx);		
		{ // Legacy mode
			if (ch == QChar(5))
				ch = SpecialChars::PARSEP;
			if (ch == QChar(4))
				ch = SpecialChars::TAB;
		}
		
		int pos = obj->itemText.length();
		if (ch == SpecialChars::OBJECT) {
			if (iobj != -1) {
				obj->itemText.insertObject(pos, doc->FrameItems.at(iobj));
			}
		}
		else if (ch == SpecialChars::SHYPHEN && pos > 0)
		{
//			qDebug() << QString("scribus134format: SHYPHEN at %1").arg(pos);
			ScText* lastItem = obj->itemText.item(pos-1);
			// double SHY means user provided SHY, single SHY is automatic one
			if (lastItem->effects() & ScStyle_HyphenationPossible)
			{
				lastItem->setEffects(lastItem->effects() & ~ScStyle_HyphenationPossible);
				obj->itemText.insertChars(pos, QString(ch));
			}
			else
			{
				lastItem->setEffects(lastItem->effects() | ScStyle_HyphenationPossible);
			}
		}
		else {
			obj->itemText.insertChars(pos, QString(ch));
		}
//		qDebug() << QString("style at %1: %2 ^ %3 = %4 (%5)").arg(pos).arg((uint)newStyle.effects()).arg((uint)last->Style.effects()).arg((uint)(newStyle.effects() ^ last->Style.effects())).arg(newStyle != last->Style);
		if (newStyle != last->Style) // || (newStyle.effects() ^ last->Style.effects()) == ScStyle_HyphenationPossible) 
		{  // FIXME StyleFlag operator== ignores hyphen flag
//			qDebug() << QString("new style at %1: %2 -> %3").arg(pos).arg(last->Style.asString()).arg(newStyle.asString());
			obj->itemText.setCharStyle(last->StyleStart, pos-last->StyleStart, last->Style);
			last->Style = newStyle;
			last->StyleStart = pos;
		}
		if (ch == SpecialChars::PARSEP) {
			ParagraphStyle pstyle;
			// Qt4 if (last->ParaStyle >= 0) {
			if (!last->ParaStyle.isEmpty()) {
				pstyle.setParent( last->ParaStyle );
			}
			if (calign >= 0)
				pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(calign));
//			qDebug() << QString("par style at %1: %2/%3 (%4) calign %5").arg(pos).arg(pstyle.name()).arg(pstyle.parent()).arg(last->ParaStyle).arg(calign);
			obj->itemText.applyStyle(pos, pstyle);
		}
	}

	obj->itemText.setCharStyle(last->StyleStart, obj->itemText.length()-last->StyleStart, last->Style);
	last->StyleStart = obj->itemText.length();
/*
	QString dbg("");
	for (int i=0; i < obj->itemText.length(); ++i)
	{
		dbg += obj->itemText.text(i,1);
		if (obj->itemText.item(i)->effects() & ScStyle_HyphenationPossible)
			dbg += "~";
	}
	qDebug("scribus134format: read itemtext %d '%s'", obj->itemText.length(), dbg.latin1());
	*/
	ParagraphStyle pstyle;

	if (!last->ParaStyle.isEmpty()) { // Qt4 >= 0) {
		pstyle.setParent( last->ParaStyle );
		obj->itemText.applyStyle(obj->itemText.length()-1, pstyle);
	}
	if (calign >= 0) {
		pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(calign));
		obj->itemText.applyStyle(obj->itemText.length()-1, pstyle);
	}
}



void Scribus134Format::readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, SCFonts &avail, ScribusDoc *doc)
{
	vg.erase();
	vg.setName(pg.attribute("NAME", ""));
	// The default style attribute must be correctly set before trying to assign a parent
	if (pg.hasAttribute("DefaultStyle"))
		vg.setDefaultStyle(pg.attribute("DefaultStyle").toInt());
	else if (vg.name() == CommonStrings::DefaultParagraphStyle || vg.name() == CommonStrings::trDefaultParagraphStyle)
		vg.setDefaultStyle(true);
	else
		vg.setDefaultStyle(false);
	vg.setParent(pg.attribute("PARENT", ""));
	if (pg.hasAttribute("LINESPMode"))
		vg.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(pg.attribute("LINESPMode").toInt()));
	if (pg.hasAttribute("LINESP"))
		vg.setLineSpacing(ScCLocale::toDoubleC(pg.attribute("LINESP")));
	if (pg.hasAttribute("INDENT"))
		vg.setLeftMargin(ScCLocale::toDoubleC(pg.attribute("INDENT")));
	if (pg.hasAttribute("RMARGIN"))
		vg.setRightMargin(ScCLocale::toDoubleC(pg.attribute("RMARGIN")));
	if (pg.hasAttribute("FIRST"))
		vg.setFirstIndent(ScCLocale::toDoubleC(pg.attribute("FIRST")));
	if (pg.hasAttribute("ALIGN"))
		vg.setAlignment(static_cast<ParagraphStyle::AlignmentType>(pg.attribute("ALIGN").toInt()));
	if (pg.hasAttribute("VOR"))
		vg.setGapBefore(ScCLocale::toDoubleC(pg.attribute("VOR")));
	if (pg.hasAttribute("NACH"))
		vg.setGapAfter(ScCLocale::toDoubleC(pg.attribute("NACH")));
	if (pg.hasAttribute("DROP"))
		vg.setHasDropCap(static_cast<bool>(pg.attribute("DROP").toInt()));
	if (pg.hasAttribute("DROPLIN"))
		vg.setDropCapLines(pg.attribute("DROPLIN").toInt());
	if (pg.hasAttribute("DROPDIST"))
		vg.setDropCapOffset(ScCLocale::toDoubleC(pg.attribute("DROPDIST")));
	if (pg.hasAttribute("PSHORTCUT"))
		vg.setShortcut(pg.attribute("PSHORTCUT"));

	//		vg.tabValues().clear();
	if ((pg.hasAttribute("NUMTAB")) && (pg.attribute("NUMTAB", "0").toInt() != 0))
	{
		QList<ParagraphStyle::TabRecord> tbs;
		ParagraphStyle::TabRecord tb;
		QString tmp = pg.attribute("TABS");
		ScTextStream tgv(&tmp, QIODevice::ReadOnly);
		double xf, xf2;
		for (int cxv = 0; cxv < pg.attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar =  QChar();
			tbs.append(tb);
		}
		vg.setTabValues(tbs);
		tmp = "";
	}
	else
	{
		QList<ParagraphStyle::TabRecord> tbs;
		vg.resetTabValues();
		QDomNode IT = pg.firstChild();
		while(!IT.isNull())
		{
			QDomElement it = IT.toElement();
			if (it.tagName()=="Tabs")
			{
				ParagraphStyle::TabRecord tb;
				tb.tabPosition = ScCLocale::toDoubleC( it.attribute("Pos"));
				tb.tabType = it.attribute("Type").toInt();
				QString tbCh = "";
				tbCh = it.attribute("Fill","");
				if (tbCh.isEmpty())
					tb.tabFillChar = QChar();
				else
					tb.tabFillChar = tbCh[0];
				tbs.append(tb);
			}
			IT=IT.nextSibling();
		}
		if (tbs.count() > 0)
			vg.setTabValues(tbs);
	}
	
	if (pg.hasAttribute("OpticalMargins"))
		vg.setOpticalMargins(pg.attribute("OpticalMargins").toInt());
	if (pg.hasAttribute("HyphenationMode"))
		vg.setHyphenationMode(pg.attribute("HyphenationMode").toInt());
	if (pg.hasAttribute("MinWordTrack"))
		vg.setMinWordTracking(ScCLocale::toDoubleC(pg.attribute("MinWordTrack")));
	if (pg.hasAttribute("NormWordTrack"))
		vg.charStyle().setWordTracking(ScCLocale::toDoubleC(pg.attribute("NormWordTrack")));
	if (pg.hasAttribute("MinGlyphShrink"))
		vg.setMinGlyphExtension(ScCLocale::toDoubleC(pg.attribute("MinGlyphShrink")));
	if (pg.hasAttribute("MaxGlyphExtend"))
		vg.setMaxGlyphExtension(ScCLocale::toDoubleC(pg.attribute("MaxGlyphExtend")));
	
	GetCStyle( &pg, doc, vg.charStyle());
	
	fixLegacyParStyle(vg);
}

PageItem* Scribus134Format::PasteItem(QDomElement *obj, ScribusDoc *doc, const QString& baseDir, int pagenr)
{
	struct ImageLoadRequest loadingInfo;
	int z = 0;
	PageItem::ItemType pt = static_cast<PageItem::ItemType>(obj->attribute("PTYPE").toInt());
	double x = ScCLocale::toDoubleC( obj->attribute("XPOS"));
	double y = ScCLocale::toDoubleC( obj->attribute("YPOS"));
	double w = ScCLocale::toDoubleC( obj->attribute("WIDTH"));
	double h = ScCLocale::toDoubleC( obj->attribute("HEIGHT"));
	double pw = ScCLocale::toDoubleC( obj->attribute("PWIDTH"));
	double scx = ScCLocale::toDoubleC( obj->attribute("LOCALSCX"));
	double scy = ScCLocale::toDoubleC( obj->attribute("LOCALSCY"));
	QString Pcolor = obj->attribute("PCOLOR");
	if (Pcolor.isEmpty())
		Pcolor = "None";
	QString Pcolor2 = obj->attribute("PCOLOR2");
	if (Pcolor2.isEmpty())
		Pcolor2 = "None";
	QColor tmpc;
	PageItem *currItem=NULL;
	QString tmp;
	int xi;
	double xf, yf, xf2;
	QString clPath;
	QDomNode IT;
	switch (pt)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	//
	case PageItem::ImageFrame:
	case PageItem::LatexFrame: /*Everything that is valid for image frames is also valid for latex frames*/
		z = doc->itemAdd(pt, PageItem::Unspecified, x, y, w, h, 1, doc->toolSettings.dBrushPict, CommonStrings::None, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		currItem->setImageXYScale(scx, scy);
		currItem->setImageXYOffset(ScCLocale::toDoubleC(obj->attribute("LOCALX")), ScCLocale::toDoubleC(obj->attribute("LOCALY")));
		if (currItem->asLatexFrame())
		{
			PageItem_LatexFrame *latexitem = currItem->asLatexFrame();
		//	currItem->PictureIsAvailable = true;
			currItem->invalid = true;
			IT = obj->firstChild();
			while(!IT.isNull())
			{
				QDomElement it = IT.toElement();
				if (it.tagName()=="LATEX")
				{
					latexitem->setConfigFile(it.attribute("ConfigFile"), true);
					latexitem->setDpi(it.attribute("DPI").toInt());
					latexitem->setUsePreamble(static_cast<bool>(it.attribute("USE_PREAMBLE").toInt()));
					QDomElement property = it.firstChildElement("PROPERTY");
					while (!property.isNull())
					{
						QString name = property.attribute("name");
						QString value = property.attribute("value");
						property = property.nextSiblingElement("PROPERTY");
						if (name.isEmpty()) continue;
						latexitem->editorProperties[name] = value;
					}
					QString temp = it.text();
					latexitem->setFormula(temp, false);
					
				}
				IT=IT.nextSibling();
			}
		}
		else
		{
			bool inlineF = static_cast<bool>(obj->attribute("isInlineImage", "0").toInt());
			QString dat = obj->attribute("ImageData", "");
			QByteArray inlineImageData;
			inlineImageData.append(dat);
			QString inlineImageExt = obj->attribute("inlineImageExt", "");
			if (inlineF)
			{
				if (inlineImageData.size() > 0)
				{
					currItem->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + inlineImageExt);
					currItem->tempImageFile->open();
					QString fileName = getLongPathName(currItem->tempImageFile->fileName());
					currItem->tempImageFile->close();
					inlineImageData = qUncompress(QByteArray::fromBase64(inlineImageData));
					QFile outFil(fileName);
					if (outFil.open(QIODevice::WriteOnly))
					{
						outFil.write(inlineImageData);
						outFil.close();
						currItem->isInlineImage = true;
						currItem->Pfile = fileName;
					}
				}
			}
			else
				currItem->Pfile = Relative2Path(obj->attribute("PFILE"), baseDir);
		}
		currItem->IProfile  = obj->attribute("PRFILE","");
		currItem->EmProfile = obj->attribute("EPROF","");
		currItem->IRender = obj->attribute("IRENDER", "1").toInt();
		currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
		currItem->pixm.imgInfo.lowResType = obj->attribute("ImageRes", "1").toInt();
		currItem->pixm.imgInfo.actualPageNumber = obj->attribute("Pagenumber", "0").toInt();
		IT = obj->firstChild();
		while(!IT.isNull())
		{
			QDomElement it = IT.toElement();
			if (it.tagName()=="ImageEffect")
			{
				struct ImageEffect ef;
				ef.effectParameters = it.attribute("Param");
				ef.effectCode = it.attribute("Code").toInt();
				currItem->effectsInUse.append(ef);
			}
			IT=IT.nextSibling();
		}
		if (!currItem->Pfile.isEmpty())
			doc->loadPict(currItem->Pfile, currItem, false);
		currItem->IProfile = obj->attribute("PRFILE","");
		currItem->EmProfile = obj->attribute("EPROF","");
		currItem->IRender = obj->attribute("IRENDER", "1").toInt();
		currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
		if (currItem->asLatexFrame())
		{
			currItem->setImageXYOffset(ScCLocale::toDoubleC(obj->attribute("LOCALX")) * scx, ScCLocale::toDoubleC(obj->attribute("LOCALY")) * scy);
	//		currItem->setImageXYScale(1.0, 1.0);
		}
		else
			currItem->setImageXYScale(scx, scy);
		clPath = obj->attribute("ImageClip", "");
		if (currItem->pixm.imgInfo.PDSpathData.contains(clPath))
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[clPath].copy();
			currItem->pixm.imgInfo.usedPath = clPath;
			QMatrix cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		currItem->setImageShown(obj->attribute("PICART").toInt());
/*		currItem->BBoxX = ScCLocale::toDoubleC( obj->attribute("BBOXX"));
		currItem->BBoxH = ScCLocale::toDoubleC( obj->attribute("BBOXH")); */
		currItem->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
		currItem->AspectRatio = obj->attribute("RATIO", "0").toInt();
		currItem->setLineWidth(pw);
		if (currItem->pixm.imgInfo.layerInfo.count() != 0)
		{
			bool found = false;
			IT = obj->firstChild();
			while(!IT.isNull())
			{
				QDomElement it = IT.toElement();
				if (it.tagName() == "PSDLayer")
				{
					found = true;
					loadingInfo.blend = it.attribute("Blend");
					loadingInfo.opacity = it.attribute("Opacity").toInt();
					loadingInfo.visible = static_cast<bool>(it.attribute("Visible").toInt());
					loadingInfo.useMask = static_cast<bool>(it.attribute("useMask", "1").toInt());
					currItem->pixm.imgInfo.RequestProps.insert(it.attribute("Layer").toInt(), loadingInfo);
				}
				IT=IT.nextSibling();
			}
			if (found)
			{
				currItem->pixm.imgInfo.isRequest = true;
				doc->loadPict(currItem->Pfile, currItem, true);
			}
		}
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType3:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	//
	case PageItem::PathText:
		z = doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		if ((obj->attribute("ANNOTATION", "0").toInt()) && (static_cast<bool>(obj->attribute("ANICON", "0").toInt())))
		{
			currItem->setImageXYScale(scx, scy);
			currItem->setImageXYOffset(ScCLocale::toDoubleC(obj->attribute("LOCALX")), ScCLocale::toDoubleC(obj->attribute("LOCALY")));
			currItem->Pfile  = Relative2Path(obj->attribute("PFILE","") , baseDir);
			currItem->Pfile2 = Relative2Path(obj->attribute("PFILE2",""), baseDir);
			currItem->Pfile3 = Relative2Path(obj->attribute("PFILE3",""), baseDir);
			currItem->IProfile = obj->attribute("PRFILE","");
			currItem->EmProfile = obj->attribute("EPROF","");
			currItem->IRender = obj->attribute("IRENDER", "1").toInt();
			currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
			doc->LoadPict(currItem->Pfile, z);
			currItem->setImageXYScale(scx, scy);
			currItem->setImageShown(obj->attribute("PICART").toInt());
/*			currItem->BBoxX = ScCLocale::toDoubleC( obj->attribute("BBOXX"));
			currItem->BBoxH = ScCLocale::toDoubleC( obj->attribute("BBOXH")); */
			currItem->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
			currItem->AspectRatio = obj->attribute("RATIO", "0").toInt();
		}
		//currItem->convertTo(pt);
		break;
	case PageItem::TextFrame:
		z = doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		if ((obj->attribute("ANNOTATION", "0").toInt()) && (static_cast<bool>(obj->attribute("ANICON", "0").toInt())))
		{
			currItem->setImageXYScale(scx, scy);
			currItem->setImageXYOffset(ScCLocale::toDoubleC(obj->attribute("LOCALX")), ScCLocale::toDoubleC(obj->attribute("LOCALY")));
			currItem->Pfile  = Relative2Path(obj->attribute("PFILE","") , baseDir);
			currItem->Pfile2 = Relative2Path(obj->attribute("PFILE2",""), baseDir);
			currItem->Pfile3 = Relative2Path(obj->attribute("PFILE3",""), baseDir);
			currItem->IProfile = obj->attribute("PRFILE","");
			currItem->EmProfile = obj->attribute("EPROF","");
			currItem->IRender = obj->attribute("IRENDER", "1").toInt();
			currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
			doc->LoadPict(currItem->Pfile, z);
			currItem->setImageXYScale(scx, scy);
			currItem->setImageShown(obj->attribute("PICART").toInt());
/*			currItem->BBoxX = ScCLocale::toDoubleC( obj->attribute("BBOXX"));
			currItem->BBoxH = ScCLocale::toDoubleC( obj->attribute("BBOXH")); */
			currItem->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
			currItem->AspectRatio = obj->attribute("RATIO", "0").toInt();
		}
			//currItem->convertTo(pt);
		break;
	case PageItem::Line:
		z = doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	case PageItem::Polygon:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	case PageItem::PolyLine:
		z = doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	case PageItem::Multiple:
		Q_ASSERT(false);
		break;
	}
	currItem->FrameType = obj->attribute("FRTYPE", "0").toInt();
	int startArrowIndex = obj->attribute("startArrowIndex", "0").toInt();
	if ((startArrowIndex < 0) || (startArrowIndex > static_cast<int>(doc->arrowStyles.size())))
	{
		qDebug() << QString("scribus134format: invalid arrow index: %").arg(startArrowIndex);
		startArrowIndex = 0;
	}
	currItem->setStartArrowIndex(startArrowIndex);
	int endArrowIndex = obj->attribute("endArrowIndex", "0").toInt();
	if ((endArrowIndex < 0) || (endArrowIndex > static_cast<int>(doc->arrowStyles.size())))
	{
		qDebug() << QString("scribus134format: invalid arrow index: %").arg(endArrowIndex);
		endArrowIndex = 0;
	}
	currItem->setEndArrowIndex(endArrowIndex);
	currItem->NamedLStyle = obj->attribute("NAMEDLST", "");
	currItem->isBookmark = obj->attribute("BOOKMARK").toInt();
	if ((currItem->isBookmark) && (doc->BookMarks.count() == 0))
		doc->OldBM = true;
	currItem->setImageFlippedH(obj->attribute("FLIPPEDH").toInt());
	currItem->setImageFlippedV(obj->attribute("FLIPPEDV").toInt());
	currItem->setCornerRadius(ScCLocale::toDoubleC(obj->attribute("RADRECT"), 0.0));
	currItem->ClipEdited = obj->attribute("CLIPEDIT", "0").toInt();
	currItem->setFillColor(Pcolor);
	currItem->setLineColor(Pcolor2);
	currItem->setFillShade(obj->attribute("SHADE").toInt());
	currItem->setLineShade(obj->attribute("SHADE2").toInt());
	ParagraphStyle pstyle;
	if (obj->hasAttribute("LINESP"))
		pstyle.setLineSpacing(ScCLocale::toDoubleC(obj->attribute("LINESP")));
	if (obj->hasAttribute("LINESPMode"))
		pstyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(obj->attribute("LINESPMode", "0").toInt()));
	if (obj->hasAttribute("ALIGN"))
		pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(obj->attribute("ALIGN", "0").toInt()));
	if (obj->hasAttribute("IFONT"))
		pstyle.charStyle().setFont(m_AvailableFonts->findFont(obj->attribute("IFONT"), doc));
	if (obj->hasAttribute("ISIZE"))
		pstyle.charStyle().setFontSize(qRound(ScCLocale::toDoubleC(obj->attribute("ISIZE")) * 10));
	if (obj->hasAttribute("TXTSTROKE"))
		pstyle.charStyle().setStrokeColor(obj->attribute("TXTSTROKE"));
	if (obj->hasAttribute("TXTFILL"))
		pstyle.charStyle().setFillColor(obj->attribute("TXTFILL"));
	if (obj->hasAttribute("TXTSTRSH"))
		pstyle.charStyle().setStrokeShade(obj->attribute("TXTSTRSH").toInt());
	if (obj->hasAttribute("TXTFILLSH"))
		pstyle.charStyle().setFillShade(obj->attribute("TXTFILLSH").toInt());
	if (obj->hasAttribute("TXTSCALE"))
		pstyle.charStyle().setScaleH(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSCALE")) * 10));
	if (obj->hasAttribute("TXTSCALEV"))
		pstyle.charStyle().setScaleV(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSCALEV")) * 10));
	if (obj->hasAttribute("TXTBASE"))
		pstyle.charStyle().setBaselineOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTBASE")) * 10));
	if (obj->hasAttribute("TXTSHX"))
		pstyle.charStyle().setShadowXOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSHX")) * 10));
	if (obj->hasAttribute("TXTSHY"))
		pstyle.charStyle().setShadowYOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSHY")) * 10));
	if (obj->hasAttribute("TXTOUT"))
		pstyle.charStyle().setOutlineWidth(qRound(ScCLocale::toDoubleC(obj->attribute("TXTOUT")) * 10));
	if (obj->hasAttribute("TXTULP"))
		pstyle.charStyle().setUnderlineOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTULP")) * 10));
	if (obj->hasAttribute("TXTULW"))
		pstyle.charStyle().setUnderlineWidth(qRound(ScCLocale::toDoubleC(obj->attribute("TXTULW")) * 10));
	if (obj->hasAttribute("TXTSTP"))
		pstyle.charStyle().setStrikethruOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSTP")) * 10));
	if (obj->hasAttribute("TXTSTW"))
		pstyle.charStyle().setStrikethruWidth(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSTW")) * 10));
	if (obj->hasAttribute("TXTSTYLE"))
		pstyle.charStyle().setFeatures(static_cast<StyleFlag>(obj->attribute("TXTSTYLE").toInt()).featureList());
	if (obj->hasAttribute("TXTKERN"))
		pstyle.charStyle().setTracking(qRound(ScCLocale::toDoubleC(obj->attribute("TXTKERN"), 0.0) * 10));
	if (obj->hasAttribute("wordTrack"))
		pstyle.charStyle().setWordTracking(ScCLocale::toDoubleC(obj->attribute("wordTrack")));
	if (obj->hasAttribute("MinWordTrack"))
		pstyle.setMinWordTracking(ScCLocale::toDoubleC(obj->attribute("MinWordTrack")));
	if (obj->hasAttribute("MinGlyphShrink"))
		pstyle.setMinGlyphExtension(ScCLocale::toDoubleC(obj->attribute("MinGlyphShrink")));
	if (obj->hasAttribute("MaxGlyphExtend"))
		pstyle.setMaxGlyphExtension(ScCLocale::toDoubleC(obj->attribute("MaxGlyphExtend")));
	if (obj->hasAttribute("OpticalMargins"))
		pstyle.setOpticalMargins(obj->attribute("OpticalMargins").toInt());
	if (obj->hasAttribute("HyphenationMode"))
		pstyle.setHyphenationMode(obj->attribute("HyphenationMode").toInt());
	if (obj->hasAttribute("leftMargin"))
		pstyle.setLeftMargin(ScCLocale::toDoubleC(obj->attribute("leftMargin")));
	if (obj->hasAttribute("rightMargin"))
		pstyle.setRightMargin(ScCLocale::toDoubleC(obj->attribute("rightMargin")));
	if (obj->hasAttribute("firstIndent"))
		pstyle.setFirstIndent(ScCLocale::toDoubleC(obj->attribute("firstIndent")));
	currItem->itemText.setDefaultStyle(pstyle);

	if (obj->hasAttribute("PSTYLE"))
	{
		QString pstyleName = obj->attribute("PSTYLE");
		if (!pstyleName.isEmpty())
		{
			ParagraphStyle defStyle(currItem->itemText.defaultStyle());
			defStyle.setParent(pstyleName);
			currItem->itemText.setDefaultStyle(defStyle);
		}
	}

	currItem->setRotation(ScCLocale::toDoubleC(obj->attribute("ROT")));
	currItem->setTextToFrameDist(ScCLocale::toDoubleC(obj->attribute("EXTRA")),
								ScCLocale::toDoubleC(obj->attribute("REXTRA"), 1.0),
								ScCLocale::toDoubleC(obj->attribute("TEXTRA"), 1.0),
								ScCLocale::toDoubleC(obj->attribute("BEXTRA"), 1.0));
	currItem->setFirstLineOffset(static_cast<FirstLineOffsetPolicy>(obj->attribute("FLOP").toInt()));

	currItem->PLineArt = Qt::PenStyle(obj->attribute("PLINEART").toInt());
	currItem->PLineEnd = Qt::PenCapStyle(obj->attribute("PLINEEND", "0").toInt());
	currItem->PLineJoin = Qt::PenJoinStyle(obj->attribute("PLINEJOIN", "0").toInt());
	currItem->setPrintEnabled(obj->attribute("PRINTABLE").toInt());
	currItem->setIsAnnotation(obj->attribute("ANNOTATION", "0").toInt());
	currItem->annotation().setType(obj->attribute("ANTYPE", "0").toInt());
	QString AnName = obj->attribute("ANNAME","");
	if (!AnName.isEmpty())
	{
		if (currItem->itemName() == AnName)
			currItem->AutoName = true;
		else
		{
			currItem->setItemName(AnName);
			currItem->AutoName = false;
		}
	}
	currItem->annotation().setAction(obj->attribute("ANACTION",""));
	currItem->annotation().setE_act(obj->attribute("ANEACT",""));
	currItem->annotation().setX_act(obj->attribute("ANXACT",""));
	currItem->annotation().setD_act(obj->attribute("ANDACT",""));
	currItem->annotation().setFo_act(obj->attribute("ANFOACT",""));
	currItem->annotation().setBl_act(obj->attribute("ANBLACT",""));
	currItem->annotation().setK_act(obj->attribute("ANKACT",""));
	currItem->annotation().setF_act(obj->attribute("ANFACT",""));
	currItem->annotation().setV_act(obj->attribute("ANVACT",""));
	currItem->annotation().setC_act(obj->attribute("ANCACT",""));
	currItem->annotation().setActionType(obj->attribute("ANACTYP", "0").toInt());
	currItem->annotation().setExtern(obj->attribute("ANEXTERN",""));
	if ((!currItem->annotation().Extern().isEmpty()) && (currItem->annotation().ActionType() != 8))
		currItem->annotation().setExtern(Relative2Path(obj->attribute("ANEXTERN","") , baseDir));
	currItem->annotation().setZiel(obj->attribute("ANZIEL", "0").toInt());
	currItem->annotation().setToolTip(obj->attribute("ANTOOLTIP",""));
	currItem->annotation().setRollOver(obj->attribute("ANROLL",""));
	currItem->annotation().setDown(obj->attribute("ANDOWN",""));
	currItem->annotation().setBwid(obj->attribute("ANBWID", "1").toInt());
	currItem->annotation().setBsty(obj->attribute("ANBSTY", "0").toInt());
	currItem->annotation().setFeed(obj->attribute("ANFEED", "1").toInt());
	currItem->annotation().setFlag(obj->attribute("ANFLAG", "0").toInt());
	currItem->annotation().setFont(obj->attribute("ANFONT", "4").toInt());
	currItem->annotation().setFormat(obj->attribute("ANFORMAT", "0").toInt());
	currItem->annotation().setVis(obj->attribute("ANVIS", "0").toInt());
	currItem->annotation().setIsChk(static_cast<bool>(obj->attribute("ANCHK", "0").toInt()));
	currItem->annotation().setAAact(static_cast<bool>(obj->attribute("ANAA", "0").toInt()));
	currItem->annotation().setHTML(obj->attribute("ANHTML", "0").toInt());
	currItem->annotation().setUseIcons(static_cast<bool>(obj->attribute("ANICON", "0").toInt()));
	currItem->annotation().setChkStil(obj->attribute("ANCHKS", "0").toInt());
	currItem->annotation().setMaxChar(obj->attribute("ANMC", "-1").toInt());
	currItem->annotation().setBorderColor(obj->attribute("ANBCOL", CommonStrings::None));
	currItem->annotation().setIPlace(obj->attribute("ANPLACE", "1").toInt());
	currItem->annotation().setScaleW(obj->attribute("ANSCALE", "0").toInt());
	currItem->TopLine = static_cast<bool>(obj->attribute("TopLine", "0").toInt());
	currItem->LeftLine = static_cast<bool>(obj->attribute("LeftLine", "0").toInt());
	currItem->RightLine = static_cast<bool>(obj->attribute("RightLine", "0").toInt());
	currItem->BottomLine = static_cast<bool>(obj->attribute("BottomLine", "0").toInt());
	currItem->isTableItem = static_cast<bool>(obj->attribute("isTableItem", "0").toInt());
	currItem->TopLinkID =  obj->attribute("TopLINK", "-1").toInt();
	currItem->LeftLinkID =  obj->attribute("LeftLINK", "-1").toInt();
	currItem->RightLinkID =  obj->attribute("RightLINK", "-1").toInt();
	currItem->BottomLinkID =  obj->attribute("BottomLINK", "-1").toInt();
	currItem->PoShow = obj->attribute("PLTSHOW", "0").toInt();
	currItem->BaseOffs = ScCLocale::toDoubleC( obj->attribute("BASEOF"), 0.0);
	currItem->textPathType =  obj->attribute("textPathType", "0").toInt();
	currItem->textPathFlipped = static_cast<bool>(obj->attribute("textPathFlipped", "0").toInt());
	if ( obj->hasAttribute("TEXTFLOWMODE") )
		currItem->setTextFlowMode((PageItem::TextFlowMode) obj->attribute("TEXTFLOWMODE", "0").toInt());
	else if ( obj->attribute("TEXTFLOW").toInt() )
	{
		if (obj->attribute("TEXTFLOW2", "0").toInt())
			currItem->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
		else if (obj->attribute("TEXTFLOW3", "0").toInt())
			currItem->setTextFlowMode(PageItem::TextFlowUsesContourLine);
		else
			currItem->setTextFlowMode(PageItem::TextFlowUsesFrameShape);	
	}
	else
		currItem->setTextFlowMode(PageItem::TextFlowDisabled);
	currItem->DashOffset = ScCLocale::toDoubleC( obj->attribute("DASHOFF"), 0.0);
	currItem->setReversed(static_cast<bool>(obj->attribute("REVERS", "0").toInt()));
	currItem->setLocked(static_cast<bool>(obj->attribute("LOCK", "0").toInt()));
	currItem->setSizeLocked(static_cast<bool>(obj->attribute("LOCKR", "0").toInt()));
	currItem->setFillTransparency(ScCLocale::toDoubleC(obj->attribute("TransValue"), 0.0));
	currItem->fillRule = static_cast<bool>(obj->attribute("fillRule", "1").toInt());
	currItem->doOverprint = static_cast<bool>(obj->attribute("doOverprint", "0").toInt());
	if (obj->hasAttribute("TransValueS"))
		currItem->setLineTransparency(ScCLocale::toDoubleC(obj->attribute("TransValueS"), 0.0));
	else
		currItem->setLineTransparency(ScCLocale::toDoubleC(obj->attribute("TransValue"), 0.0));
	currItem->setFillBlendmode(obj->attribute("TransBlend", "0").toInt());
	currItem->setLineBlendmode(obj->attribute("TransBlendS", "0").toInt());
	if (obj->attribute("TRANSPARENT", "0").toInt() == 1)
		currItem->setFillColor(CommonStrings::None);
	currItem->Cols = obj->attribute("COLUMNS", "1").toInt();
	currItem->ColGap = ScCLocale::toDoubleC( obj->attribute("COLGAP"), 0.0);
	if (obj->attribute("LAYER", "0").toInt() != -1)
		currItem->LayerNr = obj->attribute("LAYER", "0").toInt();
	tmp = "";
	if ((obj->hasAttribute("GROUPS")) && (obj->attribute("NUMGROUP", "0").toInt() != 0))
	{
		int groupMax = doc->GroupCounter;
		QMap<int, int>::ConstIterator gIt;
		tmp = obj->attribute("GROUPS");
		ScTextStream fg(&tmp, QIODevice::ReadOnly);
		currItem->Groups.clear();
		for (int cx = 0; cx < obj->attribute("NUMGROUP", "0").toInt(); ++cx)
		{
			fg >> xi;
			gIt = groupRemap.find(xi);
			if (gIt != groupRemap.end())
				currItem->Groups.push(gIt.value());
			else
			{
				currItem->Groups.push(groupMax); 
				groupRemap.insert(xi, groupMax);
				++groupMax;
			}
		}
		doc->GroupCounter = groupMax;
		tmp = "";
	}
	else
		currItem->Groups.clear();

	QList<ParagraphStyle::TabRecord> tbs;
	tmp = "";
	if ((obj->hasAttribute("NUMTAB")) && (obj->attribute("NUMTAB", "0").toInt() != 0))
	{
		ParagraphStyle::TabRecord tb;
		tmp = obj->attribute("TABS");
		ScTextStream tgv(&tmp, QIODevice::ReadOnly);
		for (int cxv = 0; cxv < obj->attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			tbs.append(tb);
		}
		tmp = "";
	}
	else
	{
		IT = obj->firstChild();
		while(!IT.isNull())
		{
			QDomElement it = IT.toElement();
			if (it.tagName()=="Tabs")
			{
				ParagraphStyle::TabRecord tb;
				tb.tabPosition = ScCLocale::toDoubleC( it.attribute("Pos"));
				tb.tabType = it.attribute("Type").toInt();
				QString tbCh = "";
				tbCh = it.attribute("Fill","");
				if (tbCh.isEmpty())
					tb.tabFillChar = QChar();
				else
					tb.tabFillChar = tbCh[0];
				tbs.append(tb);
			}
			IT=IT.nextSibling();
		}
	}
	if (tbs.count() > 0) {
		ParagraphStyle newDefault(currItem->itemText.defaultStyle());
		newDefault.setTabValues(tbs);
		currItem->itemText.setDefaultStyle(newDefault);
	}
	
	if ((obj->hasAttribute("NUMDASH")) && (obj->attribute("NUMDASH", "0").toInt() != 0))
	{
		tmp = obj->attribute("DASHS");
		ScTextStream dgv(&tmp, QIODevice::ReadOnly);
		currItem->DashValues.clear();
		for (int cxv = 0; cxv < obj->attribute("NUMDASH", "0").toInt(); ++cxv)
		{
			dgv >> xf;
			currItem->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		currItem->DashValues.clear();
	tmp = "";
	if (obj->hasAttribute("NUMPO"))
	{
		currItem->PoLine.resize(obj->attribute("NUMPO").toUInt());
		tmp = obj->attribute("POCOOR");
		ScTextStream fp(&tmp, QIODevice::ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMPO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			currItem->PoLine.setPoint(cx, xf, yf);
		}
	}
	else
		currItem->PoLine.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMCO"))
	{
		currItem->ContourLine.resize(obj->attribute("NUMCO").toUInt());
		tmp = obj->attribute("COCOOR");
		ScTextStream fp(&tmp, QIODevice::ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMCO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			currItem->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		currItem->ContourLine = currItem->PoLine.copy();
	if (!currItem->asLine())
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	else
	{
		int ph = static_cast<int>(qMax(1.0, currItem->lineWidth() / 2.0));
		currItem->Segments.clear();
		currItem->PoLine.resize(0);
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
		currItem->setHeight(1.0);
	}
	if (currItem->asImageFrame())
		currItem->AdjustPictScale();
	if (currItem->asPathText())
	{
		currItem->updatePolyClip();
		currItem->Frame = true;
	}
	currItem->GrType = obj->attribute("GRTYP", "0").toInt();
	QString GrColor;
	QString GrColor2;
	int GrShade = 0;
	int GrShade2 = 0;
	if (currItem->GrType != 0)
	{
		if (currItem->GrType == 8)
		{
			currItem->setPattern(obj->attribute("pattern", ""));
			double patternScaleX = ScCLocale::toDoubleC( obj->attribute("pScaleX"), 100.0);
			double patternScaleY = ScCLocale::toDoubleC( obj->attribute("pScaleY"), 100.0);
			double patternOffsetX = ScCLocale::toDoubleC( obj->attribute("pOffsetX"), 0.0);
			double patternOffsetY = ScCLocale::toDoubleC( obj->attribute("pOffsetY"), 0.0);
			double patternRotation = ScCLocale::toDoubleC( obj->attribute("pRotation"), 0.0);
			currItem->setPatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
		}
		else
		{
			currItem->GrStartX = ScCLocale::toDoubleC( obj->attribute("GRSTARTX"), 0.0);
			currItem->GrStartY = ScCLocale::toDoubleC( obj->attribute("GRSTARTY"), 0.0);
			currItem->GrEndX = ScCLocale::toDoubleC( obj->attribute("GRENDX"), 0.0);
			currItem->GrEndY = ScCLocale::toDoubleC( obj->attribute("GRENDY"), 0.0);
			GrColor = obj->attribute("GRCOLOR","");
			if (!GrColor.isEmpty())
			{
				GrColor2 = obj->attribute("GRCOLOR2","");
				GrShade = obj->attribute("GRSHADE", "100").toInt();
				GrShade2 = obj->attribute("GRSHADE2", "100").toInt();
			}
		}
	}
	if ((currItem->GrType != 0) && (currItem->GrType != 8))
	{
		currItem->fill_gradient.clearStops();
		if ((!GrColor.isEmpty()) && (!GrColor2.isEmpty()))
		{
			if (currItem->GrType == 5)
			{
				if ((GrColor != CommonStrings::None) && (!GrColor.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor, GrShade);
				currItem->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor, GrShade);
				if ((GrColor2 != CommonStrings::None) && (!GrColor2.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor2, GrShade2);
				currItem->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor2, GrShade2);
			}
			else
			{
				if ((GrColor2 != CommonStrings::None) && (!GrColor2.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor2, GrShade2);
				currItem->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor2, GrShade2);
				if ((GrColor != CommonStrings::None) && (!GrColor.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor, GrShade);
				currItem->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor, GrShade);
			}
		}
//		currItem->updateGradientVectors();
	}
	//currItem->setRedrawBounding();
	//currItem->OwnPage = view->OnPage(currItem);
	return currItem;
}

bool Scribus134Format::loadPage(const QString & fileName, int pageNumber, bool Mpage, QString renamedPageName)
{
// 	qDebug() << QString("loading page %2 from file '%1' from 1.3.x plugin").arg(fileName).arg(pageNumber);
	if (m_Doc==0 || m_AvailableFonts==0)
	{
		Q_ASSERT(m_Doc==0 || m_AvailableFonts==0);
		return false;
	}
	ParagraphStyle vg;
	struct ScribusDoc::BookMa bok;
	PageItem *Neu;
	Page* Apage = NULL;
	groupRemap.clear();
	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	itemRemapM.clear();
	itemNextM.clear();
	itemCountM = 0;
	itemRemapF.clear();
	itemNextF.clear();
	itemCountF = 0;
	QString tmV, tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QFont fo;
	QMap<int,int> TableID;
	QList<PageItem*> TableItems;
	QMap<PageItem*, int> groupID;
	int a, counter, baseobj;
	double pageX = 0, pageY = 0;
	bool newVersion = false;
	bool VorLFound = false;
	QMap<int,int> layerTrans;
	int maxLayer = 0;
	int maxLevel = 0;
	layerTrans.clear();
	uint layerCount=m_Doc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		maxLayer = qMax(m_Doc->Layers[la2].LNr, maxLayer);
		maxLevel = qMax(m_Doc->Layers[la2].Level, maxLevel);
	}
	DoVorl.clear();
	DoVorl[0] = "0";
	DoVorl[1] = "1";
	DoVorl[2] = "2";
	DoVorl[3] = "3";
	DoVorl[4] = "4";
	VorlC = 5;
	QDomDocument docu("scridoc");
 	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QString fileDir = QFileInfo(fileName).absolutePath();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	counter = m_Doc->Items->count();
	baseobj = counter;
//	PrefsManager* prefsManager=PrefsManager::instance();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
			{
				ScColor lf;
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
				if (pg.hasAttribute("Spot"))
					lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
				else
					lf.setSpotColor(false);
				if (pg.hasAttribute("Register"))
					lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
				else
					lf.setRegistrationColor(false);
				QString name = pg.attribute("NAME");
				m_Doc->PageColors.insert((name.isEmpty()) ? lf.name() : name, lf);
			}
			if(pg.tagName()=="STYLE")
			{
				GetStyle(&pg, &vg, NULL, m_Doc, true);
				VorLFound = true;
			}
			if(pg.tagName()=="JAVA")
				m_Doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				int lnr   = pg.attribute("NUMMER").toInt();
				int level = pg.attribute("LEVEL").toInt();
				ScLayer la( pg.attribute("NAME"), level, lnr );
				la.isViewable   = pg.attribute("SICHTBAR").toInt();
				la.isPrintable  = pg.attribute("DRUCKEN").toInt();
				la.isEditable   = pg.attribute("EDIT", "1").toInt();
				la.flowControl  = pg.attribute("FLOW", "1").toInt();
				la.transparency = ScCLocale::toDoubleC( pg.attribute("TRANS"), 1.0);
				la.blendMode    = pg.attribute("BLEND", "0").toInt();
				la.outlineMode  = pg.attribute("OUTL", "0").toInt();
				if (pg.hasAttribute("LAYERC"))
					la.markerColor =  QColor(pg.attribute("LAYERC","#000000"));
				const ScLayer* la2 = m_Doc->Layers.layerByName(la.Name);
				if (la2)
					layerTrans.insert(la.LNr, la2->LNr);
				else
				{
					maxLayer++;
					maxLevel++;
					layerTrans.insert(la.LNr, maxLayer);
					la.LNr = maxLayer;
					la.Level = maxLevel;
					m_Doc->Layers.append(la);
				}
			}
			if(pg.tagName()=="MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = PAGE.firstChild();
				while(!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct SingleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = MuL.attribute("Dash").toInt();
					sl.LineEnd = MuL.attribute("LineEnd").toInt();
					sl.LineJoin = MuL.attribute("LineJoin").toInt();
					sl.Shade = MuL.attribute("Shade").toInt();
					sl.Width = ScCLocale::toDoubleC( MuL.attribute("Width"));
					ml.shortcut = MuL.attribute("Shortcut");
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				QString Nam = pg.attribute("Name");
				QString Nam2 = Nam;
				int copyC = 1;
				QMap<QString,multiLine>::ConstIterator mlit = m_Doc->MLineStyles.find(Nam2);
				if (mlit != m_Doc->MLineStyles.end() && ml != mlit.value())
				{
					while (m_Doc->MLineStyles.contains(Nam2))
					{
						Nam2 = QObject::tr("Copy #%1 of ").arg(copyC)+Nam;
						copyC++;
					}
				}
				m_Doc->MLineStyles.insert(Nam2, ml);
			}
			if(pg.tagName()=="Arrows")
			{
				struct ArrowDesc arrow;
				arrow.name = pg.attribute("Name");
				arrow.userArrow = true;
				double xa, ya;
				QString tmp = pg.attribute("Points");
				ScTextStream fp(&tmp, QIODevice::ReadOnly);
				for (uint cx = 0; cx < pg.attribute("NumPoints").toUInt(); ++cx)
				{
					fp >> xa;
					fp >> ya;
					arrow.points.addPoint(xa, ya);
				}
				m_Doc->arrowStyles.append(arrow);
			}
			if (((pg.tagName()=="PAGE") || (pg.tagName()=="MASTERPAGE")) && (pg.attribute("NUM").toInt() == pageNumber))
			{
				a = m_Doc->currentPage()->pageNr();
				Apage = m_Doc->Pages->at(a);
				if ((pg.tagName()!="MASTERPAGE") && (Mpage))
				{
					PAGE=PAGE.nextSibling();
					continue;
				}
				if (Mpage)
				{
					Apage->LeftPg=pg.attribute("LEFT", "0").toInt();
					
					if (!renamedPageName.isEmpty())
						Apage->setPageName(renamedPageName);
					else
						Apage->setPageName(pg.attribute("NAM",""));
				}
				if (pg.hasAttribute("Size"))
					Apage->m_pageSize = pg.attribute("Size");
				if (pg.hasAttribute("Orientation"))
					Apage->PageOri = pg.attribute("Orientation").toInt();
				if (pg.hasAttribute("PAGEWIDTH"))
					Apage->setWidth(ScCLocale::toDoubleC(pg.attribute("PAGEWIDTH")));
				else
					Apage->setWidth(ScCLocale::toDoubleC(pg.attribute("PAGEWITH")));
				Apage->setHeight(ScCLocale::toDoubleC(pg.attribute("PAGEHEIGHT")));
				Apage->setInitialHeight(Apage->height());
				Apage->setInitialWidth(Apage->width());
				Apage->initialMargins.Top =  qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERTOP")));
				Apage->initialMargins.Bottom = qMax(0.0,  ScCLocale::toDoubleC(pg.attribute("BORDERBOTTOM")));
				Apage->initialMargins.Left =  qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERLEFT")));
				Apage->initialMargins.Right = qMax(0.0, ScCLocale::toDoubleC( pg.attribute("BORDERRIGHT")));
				Apage->marginPreset = pg.attribute("PRESET", "0").toInt();
				Apage->Margins.Top = Apage->initialMargins.Top;
				Apage->Margins.Bottom = Apage->initialMargins.Bottom;
				pageX = ScCLocale::toDoubleC( pg.attribute("PAGEXPOS"));
				pageY = ScCLocale::toDoubleC( pg.attribute("PAGEYPOS"));
				// guides reading
				tmp = "";
				Apage->guides.setHorizontalAutoGap(ScCLocale::toDoubleC(pg.attribute("AGhorizontalAutoGap"), 0.0));
				Apage->guides.setVerticalAutoGap(ScCLocale::toDoubleC(pg.attribute("AGverticalAutoGap"), 0.0));
				Apage->guides.setHorizontalAutoCount(pg.attribute("AGhorizontalAutoCount", "0").toInt());
				Apage->guides.setVerticalAutoCount(pg.attribute("AGverticalAutoCount", "0").toInt());
				Apage->guides.setHorizontalAutoRefer(pg.attribute("AGhorizontalAutoRefer", "0").toInt());
				Apage->guides.setVerticalAutoRefer(pg.attribute("AGverticalAutoRefer", "0").toInt());
				GuideManagerIO::readVerticalGuides(pg.attribute("VerticalGuides"),
												Apage,
												GuideManagerCore::Standard,
												pg.hasAttribute("NumVGuides"));
				GuideManagerIO::readHorizontalGuides(pg.attribute("HorizontalGuides"),
												Apage,
												GuideManagerCore::Standard,
												pg.hasAttribute("NumHGuides"));
				GuideManagerIO::readSelection(pg.attribute("AGSelection"), Apage);
				
			}
			if ((pg.tagName()=="PAGEOBJECT") || (pg.tagName()=="MASTEROBJECT") || (pg.tagName()=="FRAMEOBJECT"))
			{
				if (Mpage)
				{
					if (pg.tagName() != "MASTEROBJECT")
					{
						PAGE=PAGE.nextSibling();
						continue;
					}
				}
				else
				{
					if (pg.tagName() == "MASTEROBJECT")
					{
						PAGE=PAGE.nextSibling();
						continue;
					}
				}
				if (pg.attribute("OwnPage").toInt() != pageNumber)
				{			
					if (pg.tagName()=="PAGEOBJECT")
						itemRemap[itemCount++] = -1;
					else if (pg.tagName()=="MASTEROBJECT")
						itemRemapM[itemCountM++] = -1;
				}
				else
//				if (pg.attribute("OwnPage").toInt() == pageNumber)
				{
					// first of linked chain?
					if (pg.tagName()=="PAGEOBJECT")
					{
						itemRemap[itemCount++] = m_Doc->DocItems.count();
						if (pg.attribute("NEXTITEM").toInt() != -1)
							itemNext[m_Doc->DocItems.count()] = pg.attribute("NEXTITEM").toInt();
					}
					else if (pg.tagName()=="MASTEROBJECT")
					{
						itemRemapM[itemCountM++] = m_Doc->MasterItems.count();
						if (pg.attribute("NEXTITEM").toInt() != -1)
							itemNextM[m_Doc->MasterItems.count()] = pg.attribute("NEXTITEM").toInt();
					}
					/* not sure if we want that...
					else if (pg.tagName()=="FRAMEOBJECT")
					{
						itemRemapF[itemCountF++] = m_Doc->FrameItems->count();
						if (pg.attribute("NEXTITEM").toInt() != -1)
							itemNextF[m_Doc->FrameItems->count()] = pg.attribute("NEXTITEM").toInt();
					}*/
					int docGc = m_Doc->GroupCounter;
					/*m_Doc->GroupCounter = 0;*/
					Neu = PasteItem(&pg, m_Doc, fileDir);
					Neu->moveBy(-pageX + Apage->xOffset(), - pageY + Apage->yOffset());
					Neu->setRedrawBounding();
					//CB Must run onpage as we cant use pagetoload if the page has been renamed. 
					//CB TODO Make this accept a page to place onto.
					//Neu->OwnPage = m_Doc->OnPage(Neu);
					//Neu->OwnPage = PageToLoad;
					Neu->OwnPage = m_Doc->currentPageNumber();
					if (pg.tagName()=="PAGEOBJECT")
						Neu->OnMasterPage = "";
					/*m_Doc->GroupCounter = docGc;*/
					tmpf = pg.attribute("IFONT", m_Doc->toolSettings.defFont);
					m_AvailableFonts->findFont(tmpf, m_Doc);
					QDomNode IT=pg.firstChild();
					LastStyles * last = new LastStyles();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = ScCLocale::toDoubleC( it.attribute("RAMP"), 0.0);
							int shade = it.attribute("SHADE", "100").toInt();
							double opa = ScCLocale::toDoubleC( it.attribute("TRANS"), 1);
							Neu->fill_gradient.addStop(SetColor(m_Doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							GetItemText(&it, m_Doc, Neu, last, true, VorLFound);
						else if (it.tagName()=="para")
						{
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PARSEP);
							ParagraphStyle newStyle;
							PrefsManager* prefsManager=PrefsManager::instance();
							readParagraphStyle(newStyle, it, prefsManager->appPrefs.AvailFonts, m_Doc);
							Neu->itemText.setStyle(Neu->itemText.length()-1, newStyle);
							Neu->itemText.setCharStyle(Neu->itemText.length()-1, 1, last->Style);
						}
						else if (it.tagName()=="tab")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::TAB);
						else if (it.tagName()=="breakline")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::LINEBREAK);
						else if (it.tagName()=="breakcol")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::COLBREAK);
						else if (it.tagName()=="breakframe")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::FRAMEBREAK);
						else if (it.tagName()=="nbhyphen")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::NBHYPHEN);
						else if (it.tagName()=="nbspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::NBSPACE);
						else if (it.tagName()=="zwnbspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::ZWNBSPACE);
						else if (it.tagName()=="zwspace")
							Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::ZWSPACE);
						else if (it.tagName()=="var")
						{
							if (it.attribute("name") == "pgno")
								Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PAGENUMBER);
							else
								Neu->itemText.insertChars(Neu->itemText.length(), SpecialChars::PAGECOUNT);
							CharStyle newStyle;
							GetCStyle(&it, m_Doc, newStyle);
							Neu->itemText.setCharStyle(Neu->itemText.length()-1, 1, newStyle);
							last->StyleStart = Neu->itemText.length()-1;
							last->Style = newStyle;
						}
						
						if(it.tagName()=="PageItemAttributes")
						{
							QDomNode PIA = it.firstChild();
							ObjAttrVector pageItemAttributes;
							while(!PIA.isNull())
							{
								QDomElement itemAttr = PIA.toElement();
								if(itemAttr.tagName() == "ItemAttribute")
								{
									ObjectAttribute objattr;
									objattr.name=itemAttr.attribute("Name");
									objattr.type=itemAttr.attribute("Type");
									objattr.value=itemAttr.attribute("Value");
									objattr.parameter=itemAttr.attribute("Parameter");
									objattr.relationship=itemAttr.attribute("Relationship");
									objattr.relationshipto=itemAttr.attribute("RelationshipTo");
									objattr.autoaddto=itemAttr.attribute("AutoAddTo");
									pageItemAttributes.append(objattr);
								}
								PIA = PIA.nextSibling();
							}
							Neu->setObjectAttributes(&pageItemAttributes);
						}
						IT=IT.nextSibling();
					}
					delete last;
					if (Neu->asPathText())
					{
						Neu->updatePolyClip();
						Neu->Frame = true;
					}
					if (Neu->fill_gradient.Stops() == 0)
					{
						const ScColor& col1 = m_Doc->PageColors[m_Doc->toolSettings.dBrush];
						const ScColor& col2 = m_Doc->PageColors[m_Doc->toolSettings.dPen];
						Neu->fill_gradient.addStop(ScColorEngine::getRGBColor(col1, m_Doc), 0.0, 0.5, 1.0, m_Doc->toolSettings.dBrush, 100);
						Neu->fill_gradient.addStop(ScColorEngine::getRGBColor(col2, m_Doc), 1.0, 0.5, 1.0, m_Doc->toolSettings.dPen, 100);
					}
//					Neu->Language = ScMW->GetLang(pg.attribute("LANGUAGE", m_Doc->Language));
//					Neu->Language = m_Doc->Language;
					Neu->isAutoText = static_cast<bool>(pg.attribute("AUTOTEXT").toInt());
					Neu->isEmbedded = static_cast<bool>(pg.attribute("isInline", "0").toInt());
					Neu->gXpos = ScCLocale::toDoubleC( pg.attribute("gXpos"), 0.0);
					Neu->gYpos = ScCLocale::toDoubleC( pg.attribute("gYpos"), 0.0);
					QString defaultVal;
					defaultVal.setNum(Neu->width());
					Neu->gWidth = ScCLocale::toDoubleC( pg.attribute("gWidth",defaultVal));
					defaultVal.setNum(Neu->height());
					Neu->gHeight = ScCLocale::toDoubleC( pg.attribute("gHeight",defaultVal));
					if (Neu->isAutoText)
						m_Doc->LastAuto = Neu;
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(pg.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
					}
					Neu->isGroupControl = static_cast<bool>(pg.attribute("isGroupControl", "0").toInt());
					if (Neu->isGroupControl)
					{
						if (Neu->Groups.count() == 0) // Sanity check for some broken files created using buggy development versions.
						{
							Neu->isGroupControl = false;
							Neu->setFillColor("None");
						}
						else
							groupID.insert(Neu, pg.attribute("groupsLastItem", "0").toInt()+Neu->ItemNr);
					}
					if (pg.tagName()=="FRAMEOBJECT")
					{
						m_Doc->FrameItems.append(m_Doc->Items->takeAt(Neu->ItemNr));
						Neu->ItemNr = m_Doc->FrameItems.count()-1;
					}
				}
				counter++;
			}
			PAGE=PAGE.nextSibling();
		}
		PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="Bookmark")
			{
				int elem = pg.attribute("Element").toInt();
				if (elem < m_Doc->Items->count())
				{
					bok.Title = pg.attribute("Title");
					bok.Text = pg.attribute("Text");
					bok.Aktion = pg.attribute("Aktion");
					bok.ItemNr = pg.attribute("ItemNr").toInt();
					bok.PageObject = m_Doc->Items->at(elem);
					bok.First = pg.attribute("First").toInt();
					bok.Last = pg.attribute("Last").toInt();
					bok.Prev = pg.attribute("Prev").toInt();
					bok.Next = pg.attribute("Next").toInt();
					bok.Parent = pg.attribute("Parent").toInt();
					m_Doc->BookMarks.append(bok);
				}
			}
			if(pg.tagName()=="Pattern")
			{
				ScPattern pat;
				QDomNode pa = PAGE.firstChild();
				uint ac = m_Doc->Items->count();
				bool savedAlignGrid = m_Doc->useRaster;
				bool savedAlignGuides = m_Doc->SnapGuides;
				m_Doc->useRaster = false;
				m_Doc->SnapGuides = false;
				while(!pa.isNull())
				{
					QDomElement pite = pa.toElement();
					m_Doc->setMasterPageMode(false);
					// first of linked chain?
					if (pg.tagName()=="PAGEOBJECT")
					{
						itemRemap[itemCount++] = m_Doc->DocItems.count();
						if (pg.attribute("NEXTITEM").toInt() != -1)
							itemNext[m_Doc->DocItems.count()] = pg.attribute("NEXTITEM").toInt();
					}
					/* not sure if we want that...
					else if (pg.tagName()=="MASTEROBJECT")
					{
						itemRemapM[itemCountM++] = m_Doc->MasterItems->count();
						if (pg.attribute("NEXTITEM").toInt() != -1)
							itemNextM[m_Doc->MasterItems->count()] = pg.attribute("NEXTITEM").toInt();
					}
					else if (pg.tagName()=="FRAMEOBJECT")
					{
							itemRemapF[itemCountF++] = m_Doc->FrameItems->count();
							if (pg.attribute("NEXTITEM").toInt() != -1)
								itemNextF[m_Doc->FrameItems->count()] = pg.attribute("NEXTITEM").toInt();
					}*/
					int docGc = m_Doc->GroupCounter;
					/*m_Doc->GroupCounter = 0;*/
					Neu = PasteItem(&pite, m_Doc, fileDir);
					Neu->setRedrawBounding();
					Neu->OwnPage = pite.attribute("OwnPage").toInt();
					Neu->OnMasterPage = "";
					/*m_Doc->GroupCounter = docGc;*/
					tmpf = pite.attribute("IFONT", m_Doc->toolSettings.defFont);
					m_AvailableFonts->findFont(tmpf, m_Doc);
					QDomNode IT=pite.firstChild();
					LastStyles * last = new LastStyles();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = ScCLocale::toDoubleC( it.attribute("RAMP"), 0.0);
							int shade = it.attribute("SHADE", "100").toInt();
							double opa = ScCLocale::toDoubleC( it.attribute("TRANS"), 1.0);
							Neu->fill_gradient.addStop(SetColor(m_Doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							GetItemText(&it, m_Doc, Neu, last);
						if(it.tagName()=="PageItemAttributes")
						{
							QDomNode PIA = it.firstChild();
							ObjAttrVector pageItemAttributes;
							while(!PIA.isNull())
							{
								QDomElement itemAttr = PIA.toElement();
								if(itemAttr.tagName() == "ItemAttribute")
								{
									ObjectAttribute objattr;
									objattr.name=itemAttr.attribute("Name");
									objattr.type=itemAttr.attribute("Type");
									objattr.value=itemAttr.attribute("Value");
									objattr.parameter=itemAttr.attribute("Parameter");
									objattr.relationship=itemAttr.attribute("Relationship");
									objattr.relationshipto=itemAttr.attribute("RelationshipTo");
									objattr.autoaddto=itemAttr.attribute("AutoAddTo");
									pageItemAttributes.append(objattr);
								}
								PIA = PIA.nextSibling();
							}
							Neu->setObjectAttributes(&pageItemAttributes);
						}
						IT=IT.nextSibling();
					}
					delete last;
					if (Neu->asPathText())
					{
						Neu->updatePolyClip();
						Neu->Frame = true;
					}
					if (Neu->fill_gradient.Stops() == 0)
					{
						const ScColor& col1 = m_Doc->PageColors[m_Doc->toolSettings.dBrush];
						const ScColor& col2 = m_Doc->PageColors[m_Doc->toolSettings.dPen];
						Neu->fill_gradient.addStop(ScColorEngine::getRGBColor(col1, m_Doc), 0.0, 0.5, 1.0, m_Doc->toolSettings.dBrush, 100);
						Neu->fill_gradient.addStop(ScColorEngine::getRGBColor(col2, m_Doc), 1.0, 0.5, 1.0, m_Doc->toolSettings.dPen, 100);
					}
					Neu->isAutoText = static_cast<bool>(pite.attribute("AUTOTEXT").toInt());
					Neu->isEmbedded = static_cast<bool>(pite.attribute("isInline", "0").toInt());
					Neu->gXpos = ScCLocale::toDoubleC( pite.attribute("gXpos"), 0.0);
					Neu->gYpos = ScCLocale::toDoubleC( pite.attribute("gYpos"), 0.0);
					QString defaultVal;
					defaultVal.setNum(Neu->width());
					Neu->gWidth = ScCLocale::toDoubleC( pite.attribute("gWidth",defaultVal));
					defaultVal.setNum(Neu->height());
					Neu->gHeight = ScCLocale::toDoubleC( pite.attribute("gHeight",defaultVal));
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(pite.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
					}
					Neu->isGroupControl = static_cast<bool>(pite.attribute("isGroupControl", "0").toInt());
					if (Neu->isGroupControl)
					{
						if (Neu->Groups.count() == 0) // Sanity check for some broken files created using buggy development versions.
						{
							Neu->isGroupControl = false;
							Neu->setFillColor("None");
						}
						else
							groupID.insert(Neu, pite.attribute("groupsLastItem", "0").toInt()+Neu->ItemNr);
					}
					pa = pa.nextSibling();
				}
				m_Doc->useRaster = savedAlignGrid;
				m_Doc->SnapGuides = savedAlignGuides;
				uint ae = m_Doc->Items->count();
				pat.setDoc(m_Doc);
				PageItem* currItem = m_Doc->Items->at(ac);
				pat.pattern = currItem->DrawObj_toImage();
				for (uint as = ac; as < ae; ++as)
				{
					Neu = m_Doc->Items->takeAt(ac);
					Neu->ItemNr = pat.items.count();
					pat.items.append(Neu);
				}
				pat.width = ScCLocale::toDoubleC( pg.attribute("width"), 0.0);
				pat.height = ScCLocale::toDoubleC( pg.attribute("height"), 0.0);
				pat.scaleX = ScCLocale::toDoubleC( pg.attribute("scaleX"), 0.0);
				pat.scaleY = ScCLocale::toDoubleC( pg.attribute("scaleY"), 0.0);
				m_Doc->docPatterns.insert(pg.attribute("Name"), pat);
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	if (TableItems.count() != 0)
	{
		for (int ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	if (groupID.count() != 0)
	{
		QMap<PageItem*, int>::Iterator it;
		for (it = groupID.begin(); it != groupID.end(); ++it)
		{
			it.key()->groupsLastItem = m_Doc->Items->at(it.value());
		}
	}
	
	// reestablish textframe links
	if (itemNext.count() != 0 && !Mpage)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
		{
			if (itemRemap[lc.value()] >= 0)
			{
				if ((lc.key() < m_Doc->Items->count()) && (itemRemap[lc.value()] < m_Doc->Items->count()))
				{
					PageItem * Its = m_Doc->DocItems.at(lc.key());
					PageItem * Itn = m_Doc->DocItems.at(itemRemap[lc.value()]);
					if (Itn->prevInChain() || Its->nextInChain()) 
					{
						qDebug() << "scribus134format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	else if (itemNextM.count() != 0 && Mpage)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNextM.begin(); lc != itemNextM.end(); ++lc)
		{
			if (itemRemapM[lc.value()] >= 0)
			{
				if ((lc.key() < m_Doc->MasterItems.count()) && (itemRemapM[lc.value()] < m_Doc->MasterItems.count()))
				{
					PageItem * Its = m_Doc->MasterItems.at(lc.key());
					PageItem * Itn = m_Doc->MasterItems.at(itemRemapM[lc.value()]);
					if (Itn->prevInChain() || Its->nextInChain())
					{
						qDebug() << "scribus134format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	
	// reestablish first/lastAuto
	m_Doc->FirstAuto = m_Doc->LastAuto;
	if (m_Doc->LastAuto)
	{
		while (m_Doc->LastAuto->nextInChain())
			m_Doc->LastAuto = m_Doc->LastAuto->nextInChain();
		while (m_Doc->FirstAuto->prevInChain())
			m_Doc->FirstAuto = m_Doc->FirstAuto->prevInChain();
	}

	return true;
}

void Scribus134Format::GetStyle(QDomElement *pg, ParagraphStyle *vg, StyleSet<ParagraphStyle> * tempStyles, ScribusDoc* doc, bool fl)
{
	bool fou(false);
	QString tmpf, tmf, tmV;
	const StyleSet<ParagraphStyle> * docParagraphStyles = tempStyles? tempStyles : & doc->paragraphStyles();
	PrefsManager* prefsManager=PrefsManager::instance();
	readParagraphStyle(*vg, *pg, prefsManager->appPrefs.AvailFonts, doc);
	for (int xx=0; xx<docParagraphStyles->count(); ++xx)
	{
		if (vg->name() == (*docParagraphStyles)[xx].name())
		{
			if (vg->equiv((*docParagraphStyles)[xx]))
			{
				if (fl)
				{
					DoVorl[VorlC] = tmV.setNum(xx);
					VorlC++;
				}
				fou = true;
			}
			else
			{
				vg->setName("Copy of "+(*docParagraphStyles)[xx].name());
				fou = false;
			}
			break;
		}
	}
	if (!fou)
	{
		for (int xx=0; xx< docParagraphStyles->count(); ++xx)
		{
			if (vg->equiv((*docParagraphStyles)[xx]) && fl)
			{
				vg->setName((*docParagraphStyles)[xx].name());
				fou = true;
// 				if (fl)
				{
					DoVorl[VorlC] = tmV.setNum(xx);
					VorlC++;
				}
				break;
			}
		}
	}
	if (!fou)
	{
		if (tempStyles)
			tempStyles->create(*vg);
		else
		{
			StyleSet<ParagraphStyle> tmp;
			tmp.create(*vg);
			doc->redefineStyles(tmp, false);
		}
		if (fl)
		{
			DoVorl[VorlC] = tmV.setNum(docParagraphStyles->count()-1);
			VorlC++;
		}
	}
}

bool Scribus134Format::readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles)
{
	ParagraphStyle vg;
	QDomDocument docu("scridoc");
	QString tmpf, tmf;
	QString f (readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="STYLE")
				GetStyle(&pg, &vg, &docParagraphStyles, doc, false);
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}

bool Scribus134Format::readCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles)
{
	CharStyle cstyle;
	QDomDocument docu("scridoc");
	QString tmpf, tmf;
	QString f (readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="CHARSTYLE")
			{
				GetCStyle(&pg, doc, cstyle);
				docCharStyles.create(cstyle);
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}

bool Scribus134Format::readLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty)
{
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = PAGE.firstChild();
				while(!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct SingleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = MuL.attribute("Dash").toInt();
					sl.LineEnd = MuL.attribute("LineEnd").toInt();
					sl.LineJoin = MuL.attribute("LineJoin").toInt();
					sl.Shade = MuL.attribute("Shade").toInt();
					sl.Width = ScCLocale::toDoubleC( MuL.attribute("Width"));
					ml.shortcut = MuL.attribute("Shortcut");
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				QString Nam = pg.attribute("Name");
				QString Nam2 = Nam;
				int copyC = 1;
				QMap<QString,multiLine>::ConstIterator mlit = Sty->find(Nam2);
				if (mlit != Sty->end() && ml != mlit.value())
				{
					while (Sty->contains(Nam2))
					{
						Nam2 = tr("Copy #%1 of ").arg(copyC)+Nam;
						copyC++;
					}
				}
				Sty->insert(Nam2, ml);
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}

bool Scribus134Format::readColors(const QString& fileName, ColorList & colors)
{
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	QDomDocument docu("scridoc");
	if(!docu.setContent(f))
		return false;
	colors.clear();
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
				if (pg.hasAttribute("Spot"))
					lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
				else
					lf.setSpotColor(false);
				if (pg.hasAttribute("Register"))
					lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
				else
					lf.setRegistrationColor(false);
				QString name = pg.attribute("NAME");
				colors.insert((name.isEmpty()) ? lf.name() : name, lf);
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}

bool Scribus134Format::readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames)
{
	QString PgNam;
	int counter = 0;
	int counter2 = 0;
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			PgNam = pg.attribute("NAM", "");
			if(pg.tagName()=="PAGE")
				counter++;
			if(pg.tagName()=="MASTERPAGE")
			{
				counter2++;
				masterPageNames.append(PgNam);
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	*num1 = counter;
	*num2 = counter2;
	return true;
}




