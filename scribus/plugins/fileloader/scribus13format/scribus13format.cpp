/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus13format.h"
#include "scribus13formatimpl.h"

#include "../../formatidlist.h"
#include "commonstrings.h"
#include "langmgr.h"
#include "pageitem_group.h"
#include "pageitem_line.h"
#include "prefsmanager.h"
#include "qtiocompressor.h"

#include "scconfig.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_color.h"
#include "util_layer.h"
#include "util_math.h"
#include "util_text.h"

#include "ui/missing.h"
#include "ui/scmessagebox.h"

#include <QCursor>
// #include <QDebug>
#include <QFileInfo>
#include <QByteArray>
#include <QDataStream>
#include <QApplication>
#include <QMessageBox>

// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in scribus13formatimpl.h and scribus13formatimpl.cpp .

Scribus13Format::Scribus13Format()
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file formats.
	registerFormats();
	languageChange();
}

Scribus13Format::~Scribus13Format()
{
	unregisterAll();
}

void Scribus13Format::languageChange()
{
	FileFormat* fmt = getFormatByID(FORMATID_SLA13XIMPORT);
	fmt->trName = tr("Scribus 1.3.0->1.3.3.7 Document");
	fmt->filter = fmt->trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
}

QString Scribus13Format::fullTrName() const
{
	return QObject::tr("Scribus 1.3.0->1.3.3.x Support");
}

const ScActionPlugin::AboutData* Scribus13Format::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8(
			"Franz Schmid <franz@scribus.info>, "
			"The Scribus Team");
	about->shortDescription = tr("Scribus 1.3.x File Format Support");
	about->description = tr("Allows Scribus to read Scribus 1.3.0->1.3.3.x formatted files.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void Scribus13Format::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void Scribus13Format::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Scribus 1.3.0->1.3.3.7 Document");
	fmt.formatId = FORMATID_SLA13XIMPORT;
	fmt.load = true;
	fmt.save = false; //Only support 134format saving in 134cvs
	fmt.colorReading = true;
	fmt.filter = fmt.trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-scribus");
	fmt.fileExtensions = QStringList() << "sla" << "sla.gz" << "scd" << "scd.gz";
	fmt.priority = 64;
	fmt.nativeScribus = true;
	registerFormat(fmt);
}

bool Scribus13Format::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	QByteArray docBytes("");
	if (fileName.right(2) == "gz")
	{
		QFile file(fileName);
		QtIOCompressor compressor(&file);
		compressor.setStreamFormat(QtIOCompressor::GzipFormat);
		compressor.open(QIODevice::ReadOnly);
		docBytes = compressor.read(1024);
		compressor.close();
		if (docBytes.isEmpty())
			return false;
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
	return docBytes.left(16) == "<SCRIBUSUTF8NEW " && !docBytes.left(35).contains("Version=\"1.3.4");
}

QString Scribus13Format::readSLA(const QString & fileName)
{
	QByteArray docBytes("");
	if (fileName.right(2) == "gz")
	{
		QFile file(fileName);
		QtIOCompressor compressor(&file);
		compressor.setStreamFormat(QtIOCompressor::GzipFormat);
		compressor.open(QIODevice::ReadOnly);
		docBytes = compressor.readAll();
		compressor.close();
		if (docBytes.isEmpty())
			return QString();
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
	QString docText("");
	if (docBytes.left(16) == "<SCRIBUSUTF8NEW " && !docBytes.left(35).contains("Version=\"1.3.4"))
		docText = QString::fromUtf8(docBytes);
	else
		return QString();
	if (docText.endsWith(QChar(10)) || docText.endsWith(QChar(13)))
		docText.truncate(docText.length() - 1);
	return docText;
}

void Scribus13Format::getReplacedFontData(bool & getNewReplacement, QMap<QString, QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces)
{
	getNewReplacement = newReplacement;
	getReplacedFonts = ReplacedFonts;
}

bool Scribus13Format::loadFile(const QString & fileName, const FileFormat & /* fmt */, int /* flags */, int /* index */)
{
	if (m_Doc==nullptr || m_AvailableFonts==nullptr)
	{
		Q_ASSERT(m_Doc==nullptr || m_AvailableFonts==nullptr);
		return false;
	}
	ReplacedFonts.clear();
	newReplacement = false;
	ParagraphStyle vg;
	struct ScribusDoc::BookMa bok;
	int counter;//, Pgc;
	QString tmp, pageName, Defont;
	QMap<int,int> TableID;
	QList<PageItem*> TableItems;
	QMap<int,int> TableIDM;
	QList<PageItem*> TableItemsM;
	QMap<int,int> TableIDF;
	QList<PageItem*> TableItemsF;
	int a;
	PageItem *newItem;
	ScPage* Apage;
	FrameItems.clear();
	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	DoVorl.clear();
	DoVorl[0] = "";
	DoVorl[1] = "";
	DoVorl[2] = "";
	DoVorl[3] = "";
	DoVorl[4] = "";
	VorlC = 5;
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	if (f.isEmpty())
	{
		setFileReadError();
		return false;
	}
	QString fileDir = QFileInfo(fileName).absolutePath();
	/* 2004/10/02 - petr vanek - bug #1092 - missing <PAGE> crash Scribus. The check constraint moved into IsScribus()
	FIXME: I've add test on containig tag PAGE but returning false freezes S. in scribus.cpp need some hack too...  */
	QString errorMsg;
	int errorLine, errorColumn;
	if (!docu.setContent(f, &errorMsg, &errorLine, &errorColumn))
	{
		setDomParsingError(errorMsg, errorLine, errorColumn);
		return false;
	}
	m_Doc->PageColors.clear();
	m_Doc->Layers.clear();
	int layerToSetActive=0;
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode docNode = elem.firstChild();
	if (m_mwProgressBar!=nullptr)
	{
		m_mwProgressBar->setMaximum(docNode.childNodes().count());
		m_mwProgressBar->setValue(0);
	}
	// Stop  autosave timer,it will be started only if doc has autosave feature is enabled
	if (m_Doc->autoSaveTimer->isActive())
		m_Doc->autoSaveTimer->stop();
	int ObCount = 0;
	TableItems.clear();
	TableID.clear();
	TableItemsM.clear();
	TableIDM.clear();
	TableItemsF.clear();
	TableIDF.clear();
	PrefsManager& prefsManager=PrefsManager::instance();
	while (!docNode.isNull())
	{
		QDomElement dc = docNode.toElement();
	/*
	* Attribute von DOCUMENT auslesen
	*/
		//CB Add this in to set this in the file in memory. Its saved, why not load it.
		//Will of course be replaced by per page settings although we still probably need a document default
		m_Doc->setPageSize(dc.attribute("PAGESIZE"));
		m_Doc->setPageOrientation(dc.attribute("ORIENTATION", "0").toInt());
		m_Doc->FirstPnum = dc.attribute("FIRSTNUM", "1").toInt();
		m_Doc->setPagePositioning(dc.attribute("BOOK", "0").toInt());
		int fp;
		if (m_Doc->pagePositioning() == 0)
			fp = 0;
		else
		{
			if (dc.attribute("FIRSTLEFT", "0").toInt() == 1)
				fp = 0;
			else
				fp = 1;
		}
		if (docNode.namedItem("PageSets").isNull())
		{
			m_Doc->setPageSetFirstPage(m_Doc->pagePositioning(), fp);
		}
		m_Doc->setUsesAutomaticTextFrames(dc.attribute("AUTOTEXT").toInt());
		m_Doc->PageSp = dc.attribute("AUTOSPALTEN").toInt();
		m_Doc->PageSpa = ScCLocale::toDoubleC(dc.attribute("ABSTSPALTEN"));
		m_Doc->setUnitIndex(dc.attribute("UNITS", "0").toInt());
		m_Doc->itemToolPrefs().textSize = qRound(ScCLocale::toDoubleC(dc.attribute("DSIZE")) * 10);
		Defont = dc.attribute("DFONT");
		//findFont will use that if it doesnt find the font:
		m_Doc->itemToolPrefs().textFont = prefsManager.appPrefs.itemToolPrefs.textFont;
		m_AvailableFonts->findFont(Defont, m_Doc);
		m_Doc->itemToolPrefs().textFont = Defont;
		m_Doc->itemToolPrefs().textColumns = dc.attribute("DCOL", "1").toInt();
		m_Doc->itemToolPrefs().textColumnGap = ScCLocale::toDoubleC(dc.attribute("DGAP"), 0.0);
		DocumentInformation di;
		di.setAuthor(dc.attribute("AUTHOR"));
		di.setComments(dc.attribute("COMMENTS"));
		di.setKeywords(dc.attribute("KEYWORDS",""));
		di.setTitle(dc.attribute("TITLE"));
		di.setPublisher(dc.attribute("PUBLISHER", ""));
		di.setDate(dc.attribute("DOCDATE", ""));
		di.setType(dc.attribute("DOCTYPE", ""));
		di.setFormat(dc.attribute("DOCFORMAT", ""));
		di.setIdent(dc.attribute("DOCIDENT", ""));
		di.setSource(dc.attribute("DOCSOURCE", ""));
		di.setLangInfo(dc.attribute("DOCLANGINFO", ""));
		di.setRelation(dc.attribute("DOCRELATION", ""));
		di.setCover(dc.attribute("DOCCOVER", ""));
		di.setRights(dc.attribute("DOCRIGHTS", ""));
		di.setContrib(dc.attribute("DOCCONTRIB", ""));
		m_Doc->setDocumentInfo(di);
		m_Doc->typographicPrefs().valueSuperScript = dc.attribute("VHOCH").toInt();
		m_Doc->typographicPrefs().scalingSuperScript = dc.attribute("VHOCHSC").toInt();
		m_Doc->typographicPrefs().valueSubScript = dc.attribute("VTIEF").toInt();
		m_Doc->typographicPrefs().scalingSubScript = dc.attribute("VTIEFSC").toInt();
		m_Doc->typographicPrefs().valueSmallCaps = dc.attribute("VKAPIT").toInt();
		m_Doc->guidesPrefs().valueBaselineGrid  = ScCLocale::toDoubleC(dc.attribute("BASEGRID"), 12.0);
		m_Doc->guidesPrefs().offsetBaselineGrid = ScCLocale::toDoubleC(dc.attribute("BASEO"), 0.0);
		// #9621 : autolinespacing is now express as a percentage of the font height
		// It was not working in regualer text frame in 1.3.x, so set it to the default value
		m_Doc->typographicPrefs().autoLineSpacing = 100 /*dc.attribute("AUTOL", "20").toInt()*/;
		m_Doc->typographicPrefs().valueUnderlinePos = dc.attribute("UnderlinePos", "-1").toInt();
		m_Doc->typographicPrefs().valueUnderlineWidth = dc.attribute("UnderlineWidth", "-1").toInt();
		m_Doc->typographicPrefs().valueStrikeThruPos = dc.attribute("StrikeThruPos", "-1").toInt();
		m_Doc->typographicPrefs().valueStrikeThruWidth = dc.attribute("StrikeThruWidth", "-1").toInt();
		m_Doc->GroupCounter = 1 /*dc.attribute("GROUPC", "1").toInt()*/;
		m_Doc->cmsSettings().SoftProofOn = static_cast<bool>(dc.attribute("DPSo", "0").toInt());
		m_Doc->cmsSettings().SoftProofFullOn = static_cast<bool>(dc.attribute("DPSFo", "0").toInt());
		m_Doc->cmsSettings().CMSinUse = static_cast<bool>(dc.attribute("DPuse", "0").toInt());
		m_Doc->cmsSettings().GamutCheck = static_cast<bool>(dc.attribute("DPgam", "0").toInt());
		m_Doc->cmsSettings().BlackPoint = static_cast<bool>(dc.attribute("DPbla", "1").toInt());
		m_Doc->cmsSettings().DefaultMonitorProfile = prefsManager.appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile;
		m_Doc->cmsSettings().DefaultPrinterProfile = dc.attribute("DPPr","");
		m_Doc->cmsSettings().DefaultImageRGBProfile = dc.attribute("DPIn","");
		m_Doc->cmsSettings().DefaultImageCMYKProfile = dc.attribute("DPInCMYK","");
		m_Doc->cmsSettings().DefaultSolidColorRGBProfile = dc.attribute("DPIn2","");
		if (dc.hasAttribute("DPIn3"))
			m_Doc->cmsSettings().DefaultSolidColorCMYKProfile = dc.attribute("DPIn3","");
		else
			m_Doc->cmsSettings().DefaultSolidColorCMYKProfile = dc.attribute("DPPr","");
		m_Doc->cmsSettings().DefaultIntentColors = (eRenderIntent) dc.attribute("DISc", "1").toInt();
		m_Doc->cmsSettings().DefaultIntentImages = (eRenderIntent) dc.attribute("DIIm", "0").toInt();
		layerToSetActive=dc.attribute("ALAYER", "0").toInt();
		//m_Doc->setHyphLanguage(dc.attribute("LANGUAGE", ""));
		static const QString LANGUAGE("LANGUAGE");
		QString l(dc.attribute(LANGUAGE, "en"));
		if (LanguageManager::instance()->langTableIndex(l) != -1)
			m_Doc->setLanguage(l); //new style storage
		else
		{ //old style storage
			QString lnew=LanguageManager::instance()->getAbbrevFromLang(l, false);
			if (lnew.isEmpty())
				lnew=LanguageManager::instance()->getAbbrevFromLang(l, false);
			m_Doc->setLanguage(lnew);
		}

//		m_Doc->setHyphMinimumWordLength(dc.attribute("MINWORDLEN", "3").toInt());
//		m_Doc->setHyphConsecutiveLines(dc.attribute("HYCOUNT", "2").toInt());
		if (dc.hasAttribute("PAGEWIDTH"))
			m_Doc->setPageWidth(ScCLocale::toDoubleC(dc.attribute("PAGEWIDTH")));
		else
			m_Doc->setPageWidth(ScCLocale::toDoubleC(dc.attribute("PAGEWITH")));
		m_Doc->setPageHeight(ScCLocale::toDoubleC(dc.attribute("PAGEHEIGHT")));
		m_Doc->margins()->setLeft(qMax(0.0, ScCLocale::toDoubleC("BORDERLEFT")));
		m_Doc->margins()->setRight(qMax(0.0, ScCLocale::toDoubleC("BORDERRIGHT")));
		m_Doc->margins()->setTop(qMax(0.0, ScCLocale::toDoubleC("BORDERTOP")));
		m_Doc->margins()->setBottom(qMax(0.0, ScCLocale::toDoubleC("BORDERBOTTOM")));
		m_Doc->setMarginPreset(dc.attribute("PRESET", "0").toInt());
		m_Doc->setHyphAutomatic(static_cast<bool>(dc.attribute("AUTOMATIC", "1").toInt()));
		m_Doc->setHyphAutoCheck(static_cast<bool>(dc.attribute("AUTOCHECK", "0").toInt()));
		m_Doc->GuideLock = static_cast<bool>(dc.attribute("GUIDELOCK", "0").toInt());
		m_Doc->guidesPrefs().minorGridSpacing = ScCLocale::toDoubleC(dc.attribute("MINGRID"), prefsManager.appPrefs.guidesPrefs.minorGridSpacing);
		m_Doc->guidesPrefs().majorGridSpacing = ScCLocale::toDoubleC(dc.attribute("MAJGRID"), prefsManager.appPrefs.guidesPrefs.majorGridSpacing);
		m_Doc->guidesPrefs().gridShown = static_cast<bool>(dc.attribute("SHOWGRID", "0").toInt());
		m_Doc->guidesPrefs().guidesShown = static_cast<bool>(dc.attribute("SHOWGUIDES", "1").toInt());
		m_Doc->guidesPrefs().colBordersShown = static_cast<bool>(dc.attribute("showcolborders", "0").toInt());
		m_Doc->guidesPrefs().framesShown = static_cast<bool>(dc.attribute("SHOWFRAME", "1").toInt());
		m_Doc->guidesPrefs().layerMarkersShown = static_cast<bool>(dc.attribute("SHOWLAYERM", "0").toInt());
		m_Doc->guidesPrefs().marginsShown = static_cast<bool>(dc.attribute("SHOWMARGIN", "1").toInt());
		m_Doc->guidesPrefs().baselineGridShown = static_cast<bool>(dc.attribute("SHOWBASE", "0").toInt());
		m_Doc->guidesPrefs().showPic = static_cast<bool>(dc.attribute("SHOWPICT", "1").toInt());
		m_Doc->guidesPrefs().linkShown = static_cast<bool>(dc.attribute("SHOWLINK", "0").toInt());
		m_Doc->guidesPrefs().showControls = static_cast<bool>(dc.attribute("SHOWControl", "0").toInt());
		m_Doc->guidesPrefs().rulerMode = static_cast<bool>(dc.attribute("rulerMode", "1").toInt());
		m_Doc->guidesPrefs().rulersShown = static_cast<bool>(dc.attribute("showrulers", "1").toInt());
		m_Doc->rulerXoffset = ScCLocale::toDoubleC(dc.attribute("rulerXoffset"), 0.0);
		m_Doc->rulerYoffset = ScCLocale::toDoubleC(dc.attribute("rulerYoffset"), 0.0);
		m_Doc->SnapGuides = static_cast<bool>(dc.attribute("SnapToGuides", "0").toInt());
		m_Doc->SnapGrid  = static_cast<bool>(dc.attribute("SnapToGrid", "0").toInt());
		m_Doc->itemToolPrefs().polyCorners = dc.attribute("POLYC", "4").toInt();
		m_Doc->itemToolPrefs().polyFactor = ScCLocale::toDoubleC(dc.attribute("POLYF"), 0.5);
		m_Doc->itemToolPrefs().polyRotation = ScCLocale::toDoubleC(dc.attribute("POLYR"), 0.0);
		m_Doc->itemToolPrefs().polyCurvature = 0.0;
		m_Doc->itemToolPrefs().polyUseFactor = static_cast<bool>(dc.attribute("POLYS", "0").toInt());
		m_Doc->setAutoSave(static_cast<bool>(dc.attribute("AutoSave", "0").toInt()));
		m_Doc->setAutoSaveTime(dc.attribute("AutoSaveTime", "600000").toInt());
		double leftScratch;
		// FIXME A typo in early 1.3cvs (MAR 05) means we must support loading of
		// FIXME 'ScatchLeft' for a while too. This can be removed in a few months.
		if (dc.hasAttribute("ScatchLeft"))
			leftScratch = ScCLocale::toDoubleC(dc.attribute("ScatchLeft"), 100.0);
		else
			leftScratch = ScCLocale::toDoubleC(dc.attribute("ScratchLeft"), 100.0);
		m_Doc->scratch()->set(ScCLocale::toDoubleC(dc.attribute("ScratchTop"), 20.0), leftScratch,
							  ScCLocale::toDoubleC(dc.attribute("ScratchBottom"), 20.0), ScCLocale::toDoubleC(dc.attribute("ScratchRight"), 100.0));
		m_Doc->itemToolPrefs().lineStartArrow = dc.attribute("StartArrow", "0").toInt();
		m_Doc->itemToolPrefs().lineEndArrow = dc.attribute("EndArrow", "0").toInt();
		m_Doc->itemToolPrefs().imageScaleX = ScCLocale::toDoubleC(dc.attribute("PICTSCX"), 1.0);
		m_Doc->itemToolPrefs().imageScaleY = ScCLocale::toDoubleC(dc.attribute("PICTSCY"), 1.0);
		m_Doc->itemToolPrefs().imageScaleType = static_cast<bool>(dc.attribute("PSCALE", "1").toInt());
		m_Doc->itemToolPrefs().imageAspectRatio = static_cast<bool>(dc.attribute("PASPECT", "0").toInt());
		m_Doc->itemToolPrefs().imageLowResType = dc.attribute("HalfRes", "1").toInt();
		m_Doc->itemToolPrefs().imageUseEmbeddedPath = static_cast<bool>(dc.attribute("EmbeddedPath", "0").toInt());
		if (dc.hasAttribute("PEN"))
			m_Doc->itemToolPrefs().shapeLineColor = dc.attribute("PEN");
		if (dc.hasAttribute("BRUSH"))
			m_Doc->itemToolPrefs().shapeFillColor = dc.attribute("BRUSH");
		if (dc.hasAttribute("PENLINE"))
			m_Doc->itemToolPrefs().lineColor = dc.attribute("PENLINE");
		if (dc.hasAttribute("PENTEXT"))
			m_Doc->itemToolPrefs().textColor = dc.attribute("PENTEXT");
		if (dc.hasAttribute("StrokeText"))
			m_Doc->itemToolPrefs().textStrokeColor = dc.attribute("StrokeText");
		m_Doc->itemToolPrefs().textFillColor = dc.attribute("TextBackGround", CommonStrings::None);
		m_Doc->itemToolPrefs().textLineColor = dc.attribute("TextLineColor", CommonStrings::None);
		m_Doc->itemToolPrefs().textFillColorShade = dc.attribute("TextBackGroundShade", "100").toInt();
		m_Doc->itemToolPrefs().textLineColorShade = dc.attribute("TextLineShade", "100").toInt();
		m_Doc->itemToolPrefs().textShade = dc.attribute("TextPenShade", "100").toInt();
		m_Doc->itemToolPrefs().textStrokeShade = dc.attribute("TextStrokeShade", "100").toInt();
		m_Doc->itemToolPrefs().shapeLineStyle = static_cast<Qt::PenStyle>(dc.attribute("STIL").toInt());
		m_Doc->itemToolPrefs().lineStyle = static_cast<Qt::PenStyle>(dc.attribute("STILLINE").toInt());
		m_Doc->itemToolPrefs().shapeLineWidth     = ScCLocale::toDoubleC(dc.attribute("WIDTH"), 1.0);
		m_Doc->itemToolPrefs().lineWidth = ScCLocale::toDoubleC(dc.attribute("WIDTHLINE"), 1.0);
		m_Doc->itemToolPrefs().shapeLineColorShade    = dc.attribute("PENSHADE", "100").toInt();
		m_Doc->itemToolPrefs().lineColorShade = dc.attribute("LINESHADE", "100").toInt();
		m_Doc->itemToolPrefs().shapeFillColorShade     = dc.attribute("BRUSHSHADE", "100").toInt();
		m_Doc->itemToolPrefs().textTabFillChar = dc.attribute("TabFill","");
		m_Doc->itemToolPrefs().textTabWidth   = ScCLocale::toDoubleC(dc.attribute("TabWidth"), 36.0);
		if (dc.hasAttribute("CPICT"))
			m_Doc->itemToolPrefs().imageFillColor = dc.attribute("CPICT");
		m_Doc->itemToolPrefs().imageFillColorShade = ScCLocale::toDoubleC(dc.attribute("PICTSHADE"), 100.0);
		if (dc.hasAttribute("PAGEC"))
			m_Doc->setPaperColor(QColor(dc.attribute("PAGEC")));
		if (dc.hasAttribute("MARGC"))
			m_Doc->guidesPrefs().marginColor = QColor(dc.attribute("MARGC"));
		if (dc.hasAttribute("MINORC"))
			m_Doc->guidesPrefs().minorGridColor = QColor(dc.attribute("MINORC"));
		if (dc.hasAttribute("MAJORC"))
			m_Doc->guidesPrefs().majorGridColor = QColor(dc.attribute("MAJORC"));
		if (dc.hasAttribute("GuideC"))
			m_Doc->guidesPrefs().guideColor = QColor(dc.attribute("GuideC"));
		if (dc.hasAttribute("BaseC"))
			m_Doc->guidesPrefs().baselineGridColor = QColor(dc.attribute("BaseC"));
		m_Doc->setMarginColored(static_cast<bool>(dc.attribute("RANDF", "0").toInt()));
		m_Doc->guidesPrefs().renderStackOrder.clear();
		if (static_cast<bool>(dc.attribute("BACKG", "1").toInt()))
			m_Doc->guidesPrefs().renderStackOrder << 0 << 1 << 2 << 3 << 4;
		else
			m_Doc->guidesPrefs().renderStackOrder << 4 << 0 << 1 << 2 << 3;
		m_Doc->guidesPrefs().gridType = 0;
		m_Doc->guidesPrefs().guideRad = ScCLocale::toDoubleC(dc.attribute("GuideRad"), 10.0);
		m_Doc->guidesPrefs().grabRadius  = dc.attribute("GRAB", "4").toInt();
		if (dc.hasAttribute("currentProfile"))
		{
			m_Doc->clearCheckerProfiles();
			m_Doc->setCurCheckProfile(dc.attribute("currentProfile"));
		}
		m_Doc->LastAuto = nullptr;
		QDomNode pageNode = docNode.firstChild();
		counter = 0;
		while (!pageNode.isNull())
		{
			ObCount++;
			if (m_mwProgressBar!=nullptr)
				m_mwProgressBar->setValue(ObCount);
			QDomElement pg = pageNode.toElement();
			if (pg.tagName() == "PageSets")
			{
				QDomNode PGS = pageNode.firstChild();
				m_Doc->clearPageSets();
				while (!PGS.isNull())
				{
					QDomElement PgsAttr = PGS.toElement();
					if (PgsAttr.tagName() == "Set")
					{
						struct PageSet pageS;
						pageS.Name = CommonStrings::untranslatePageSetString(PgsAttr.attribute("Name"));
						pageS.FirstPage = PgsAttr.attribute("FirstPage", "0").toInt();
						pageS.Rows = PgsAttr.attribute("Rows", "1").toInt();
						pageS.Columns = PgsAttr.attribute("Columns", "1").toInt();
						pageS.pageNames.clear();
						QDomNode PGSN = PGS.firstChild();
						while (!PGSN.isNull())
						{
							QDomElement PgsAttrN = PGSN.toElement();
							if (PgsAttrN.tagName() == "PageNames")
								pageS.pageNames.append(CommonStrings::untranslatePageSetLocString(PgsAttrN.attribute("Name")));
							PGSN = PGSN.nextSibling();
						}
						m_Doc->appendToPageSets(pageS);
						if (m_Doc->pageSets().count() - 1 == m_Doc->pagePositioning())
						{
							m_Doc->setPageGapHorizontal(ScCLocale::toDoubleC(PgsAttr.attribute("GapHorizontal"), 0.0));
							m_Doc->setPageGapVertical(ScCLocale::toDoubleC(PgsAttr.attribute("GapBelow"), 0.0));
						}
					}
					PGS = PGS.nextSibling();
				}
			}
			if (pg.tagName() == "CheckProfile")
			{
				struct CheckerPrefs checkerSettings;
				checkerSettings.ignoreErrors = static_cast<bool>(pg.attribute("ignoreErrors", "0").toInt());
				checkerSettings.autoCheck = static_cast<bool>(pg.attribute("autoCheck", "1").toInt());
				checkerSettings.checkGlyphs = static_cast<bool>(pg.attribute("checkGlyphs", "1").toInt());
				checkerSettings.checkOrphans = static_cast<bool>(pg.attribute("checkOrphans", "1").toInt());
				checkerSettings.checkOverflow = static_cast<bool>(pg.attribute("checkOverflow", "1").toInt());
				checkerSettings.checkPictures = static_cast<bool>(pg.attribute("checkPictures", "1").toInt());
				checkerSettings.checkPartFilledImageFrames = static_cast<bool>(pg.attribute("checkPartFilledImageFrames", "0").toInt());
				checkerSettings.checkResolution = static_cast<bool>(pg.attribute("checkResolution", "1").toInt());
				checkerSettings.checkTransparency = static_cast<bool>(pg.attribute("checkTransparency", "1").toInt());
				checkerSettings.minResolution = ScCLocale::toDoubleC(pg.attribute("minResolution"), 72.0);
				checkerSettings.maxResolution = ScCLocale::toDoubleC(pg.attribute("maxResolution"), 4800.0);
				checkerSettings.checkAnnotations = static_cast<bool>(pg.attribute("checkAnnotations", "0").toInt());
				checkerSettings.checkRasterPDF = static_cast<bool>(pg.attribute("checkRasterPDF", "1").toInt());
				checkerSettings.checkForGIF = static_cast<bool>(pg.attribute("checkForGIF", "1").toInt());
				checkerSettings.ignoreOffLayers = false;
				m_Doc->set1CheckerProfile(pg.attribute("Name"), checkerSettings);
			}
			// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
			if (pg.tagName() == "COLOR" && pg.attribute("NAME") != CommonStrings::None)
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
			if (pg.tagName() == "STYLE")
			{
				readParagraphStyle(vg, pg, m_Doc);
				StyleSet<ParagraphStyle> temp;
				temp.create(vg);
				m_Doc->redefineStyles(temp, false);
				DoVorl[VorlC] = vg.name();
				++VorlC;
			}
			if (pg.tagName() == "JAVA")
				m_Doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if (pg.tagName() == "LAYERS")
			{
				int lId   = pg.attribute("NUMMER").toInt();
				int level = pg.attribute("LEVEL").toInt();
				ScLayer la( pg.attribute("NAME"), level, lId);
				la.isViewable   = pg.attribute("SICHTBAR").toInt();
				la.isPrintable  = pg.attribute("DRUCKEN").toInt();
				la.isEditable   = pg.attribute("EDIT", "1").toInt();
				la.flowControl  = pg.attribute("FLOW", "1").toInt();
				la.transparency = ScCLocale::toDoubleC(pg.attribute("TRANS"), 1.0);
				la.blendMode    = pg.attribute("BLEND", "0").toInt();
				la.outlineMode  = pg.attribute("OUTL", "0").toInt();
				if (pg.hasAttribute("LAYERC"))
					la.markerColor =  QColor(pg.attribute("LAYERC","#000000"));
				m_Doc->Layers.append(la);
			}
/*			if (pg.tagName() == "Bookmark")
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
			if (pg.tagName() == "MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = pageNode.firstChild();
				while (!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct SingleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = MuL.attribute("Dash").toInt();
					sl.LineEnd = MuL.attribute("LineEnd").toInt();
					sl.LineJoin = MuL.attribute("LineJoin").toInt();
					sl.Shade = MuL.attribute("Shade").toInt();
					sl.Width = ScCLocale::toDoubleC(MuL.attribute("Width"));
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				m_Doc->docLineStyles.insert(pg.attribute("Name"), ml);
			}
			if (pg.tagName() == "Arrows")
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
				m_Doc->appendToArrowStyles(arrow);
			}
			if (pg.tagName() == "PDF")
			{
				m_Doc->pdfOptions().Articles = static_cast<bool>(pg.attribute("Articles").toInt());
				m_Doc->pdfOptions().Thumbnails = static_cast<bool>(pg.attribute("Thumbnails").toInt());
				m_Doc->pdfOptions().Compress = static_cast<bool>(pg.attribute("Compress").toInt());
				m_Doc->pdfOptions().CompressMethod = (PDFOptions::PDFCompression) pg.attribute("CMethod", "0").toInt();
				m_Doc->pdfOptions().Quality = pg.attribute("Quality", "0").toInt();
				m_Doc->pdfOptions().RecalcPic = static_cast<bool>(pg.attribute("RecalcPic").toInt());
				m_Doc->pdfOptions().Bookmarks = static_cast<bool>(pg.attribute("Bookmarks").toInt());
				if (pg.hasAttribute("MirrorH"))
					m_Doc->pdfOptions().MirrorH = static_cast<bool>(pg.attribute("MirrorH").toInt());
				else
					m_Doc->pdfOptions().MirrorH = false;
				if (pg.hasAttribute("MirrorV"))
					m_Doc->pdfOptions().MirrorV = static_cast<bool>(pg.attribute("MirrorV").toInt());
				else
					m_Doc->pdfOptions().MirrorV = false;
				if (pg.hasAttribute("RotateDeg"))
					m_Doc->pdfOptions().RotateDeg = pg.attribute("RotateDeg", "0").toInt();
				else
					m_Doc->pdfOptions().RotateDeg = 0;
				if (pg.hasAttribute("Clip"))
					m_Doc->pdfOptions().doClip = static_cast<bool>(pg.attribute("Clip").toInt());
				else
					m_Doc->pdfOptions().doClip = false;
				m_Doc->pdfOptions().PresentMode = static_cast<bool>(pg.attribute("PresentMode").toInt());
				m_Doc->pdfOptions().PicRes = pg.attribute("PicRes").toInt();
				// Fixme: check input pdf version
				m_Doc->pdfOptions().Version = (PDFVersion::Version) pg.attribute("Version").toInt();
				m_Doc->pdfOptions().Resolution = pg.attribute("Resolution").toInt();
				m_Doc->pdfOptions().Binding = pg.attribute("Binding").toInt();
				m_Doc->pdfOptions().fileName = "";
				m_Doc->pdfOptions().FontEmbedding = PDFOptions::EmbedFonts;
				m_Doc->pdfOptions().isGrayscale = static_cast<bool>(pg.attribute("Grayscale", "0").toInt());
				m_Doc->pdfOptions().UseRGB = static_cast<bool>(pg.attribute("RGBMode", "0").toInt());
				m_Doc->pdfOptions().UseProfiles = static_cast<bool>(pg.attribute("UseProfiles", "0").toInt());
				m_Doc->pdfOptions().UseProfiles2 = static_cast<bool>(pg.attribute("UseProfiles2", "0").toInt());
				m_Doc->pdfOptions().Intent = pg.attribute("Intent", "1").toInt();
				m_Doc->pdfOptions().Intent2 = pg.attribute("Intent2", "1").toInt();
				m_Doc->pdfOptions().SolidProf = pg.attribute("SolidP", "");
				m_Doc->pdfOptions().ImageProf = pg.attribute("ImageP", "");
				m_Doc->pdfOptions().PrintProf = pg.attribute("PrintP", "");
				m_Doc->pdfOptions().Info = pg.attribute("InfoString", "");
				m_Doc->pdfOptions().bleeds.setTop(ScCLocale::toDoubleC(pg.attribute("BTop"), 0.0));
				m_Doc->pdfOptions().bleeds.setLeft(ScCLocale::toDoubleC(pg.attribute("BLeft"), 0.0));
				m_Doc->pdfOptions().bleeds.setRight(ScCLocale::toDoubleC(pg.attribute("BRight"), 0.0));
				m_Doc->pdfOptions().bleeds.setBottom(ScCLocale::toDoubleC(pg.attribute("BBottom"), 0.0));
				m_Doc->pdfOptions().EmbeddedI = static_cast<bool>(pg.attribute("ImagePr", "0").toInt());
				m_Doc->pdfOptions().PassOwner = pg.attribute("PassOwner", "");
				m_Doc->pdfOptions().PassUser  = pg.attribute("PassUser", "");
				m_Doc->pdfOptions().Permissions = pg.attribute("Permissions", "-4").toInt();
				m_Doc->pdfOptions().Encrypt = static_cast<bool>(pg.attribute("Encrypt", "0").toInt());
				m_Doc->pdfOptions().useLayers = static_cast<bool>(pg.attribute("UseLayers", "0").toInt());
				m_Doc->pdfOptions().UseLPI = static_cast<bool>(pg.attribute("UseLpi", "0").toInt());
				m_Doc->pdfOptions().UseSpotColors = static_cast<bool>(pg.attribute("UseSpotColors", "1").toInt());
				m_Doc->pdfOptions().doMultiFile = static_cast<bool>(pg.attribute("doMultiFile", "0").toInt());
				m_Doc->pdfOptions().displayBookmarks = static_cast<bool>(pg.attribute("displayBookmarks", "0").toInt());
				m_Doc->pdfOptions().displayFullscreen = static_cast<bool>(pg.attribute("displayFullscreen", "0").toInt());
				m_Doc->pdfOptions().displayLayers = static_cast<bool>(pg.attribute("displayLayers", "0").toInt());
				m_Doc->pdfOptions().displayThumbs = static_cast<bool>(pg.attribute("displayThumbs", "0").toInt());
				m_Doc->pdfOptions().hideMenuBar = static_cast<bool>(pg.attribute("hideMenuBar", "0").toInt());
				m_Doc->pdfOptions().hideToolBar = static_cast<bool>(pg.attribute("hideToolBar", "0").toInt());
				m_Doc->pdfOptions().fitWindow = static_cast<bool>(pg.attribute("fitWindow", "0").toInt());
				m_Doc->pdfOptions().PageLayout = pg.attribute("PageLayout", "0").toInt();
				m_Doc->pdfOptions().openAction = pg.attribute("openAction", "");
				QDomNode PFO = pageNode.firstChild();
				while (!PFO.isNull())
				{
					QDomElement pdfF = PFO.toElement();
					if (pdfF.tagName() == "LPI")
					{
						struct LPIData lpo;
						lpo.Angle = pdfF.attribute("Angle").toInt();
						lpo.Frequency = pdfF.attribute("Frequency").toInt();
						lpo.SpotFunc = pdfF.attribute("SpotFunction").toInt();
						m_Doc->pdfOptions().LPISettings[pdfF.attribute("Color")] = lpo;
					}
					if (pdfF.tagName() == "Fonts")
					{
						if (!m_Doc->pdfOptions().EmbedList.contains(pdfF.attribute("Name")))
							m_Doc->pdfOptions().EmbedList.append(pdfF.attribute("Name"));
					}
					if (pdfF.tagName() == "Subset")
					{
						if (!m_Doc->pdfOptions().SubsetList.contains(pdfF.attribute("Name")))
							m_Doc->pdfOptions().SubsetList.append(pdfF.attribute("Name"));
					}
					if (pdfF.tagName() == "Effekte")
					{
						struct PDFPresentationData ef;
						ef.pageEffectDuration = pdfF.attribute("pageEffectDuration").toInt();
						ef.pageViewDuration = pdfF.attribute("pageViewDuration").toInt();
						ef.effectType = pdfF.attribute("effectType").toInt();
						ef.Dm = pdfF.attribute("Dm").toInt();
						ef.M = pdfF.attribute("M").toInt();
						ef.Di = pdfF.attribute("Di").toInt();
						EffVal.append(ef);
					}
					PFO = PFO.nextSibling();
				}
			}
			if (pg.tagName() == "DocItemAttributes")
			{
				QDomNode DIA = pageNode.firstChild();
				m_Doc->clearItemAttributes();
				while (!DIA.isNull())
				{
					QDomElement itemAttr = DIA.toElement();
					if (itemAttr.tagName() == "ItemAttribute")
					{
						ObjectAttribute objattr;
						objattr.name=itemAttr.attribute("Name");
						objattr.type=itemAttr.attribute("Type");
						objattr.value=itemAttr.attribute("Value");
						objattr.parameter=itemAttr.attribute("Parameter");
						objattr.relationship=itemAttr.attribute("Relationship");
						objattr.relationshipto=itemAttr.attribute("RelationshipTo");
						objattr.autoaddto=itemAttr.attribute("AutoAddTo");
						m_Doc->appendToItemAttributes(objattr);
					}
					DIA = DIA.nextSibling();
				}
			}
			if (pg.tagName() == "TablesOfContents")
			{
				QDomNode TOC = pageNode.firstChild();
				m_Doc->clearTocSetups();
				while (!TOC.isNull())
				{
					QDomElement tocElem = TOC.toElement();
					if (tocElem.tagName() == "TableOfContents")
					{
						ToCSetup tocsetup;
						tocsetup.name=tocElem.attribute("Name");
						tocsetup.itemAttrName=tocElem.attribute("ItemAttributeName");
						tocsetup.frameName=tocElem.attribute("FrameName");
						tocsetup.listNonPrintingFrames=QVariant(tocElem.attribute("ListNonPrinting")).toBool();
						tocsetup.textStyle=tocElem.attribute("Style");
						QString numberPlacement=tocElem.attribute("NumberPlacement");
						if (numberPlacement=="Beginning")
							tocsetup.pageLocation=Beginning;
						if (numberPlacement=="End")
							tocsetup.pageLocation=End;
						if (numberPlacement=="NotShown")
							tocsetup.pageLocation=NotShown;
						m_Doc->appendToTocSetups(tocsetup);
					}
					TOC = TOC.nextSibling();
				}
			}
			if (pg.tagName() == "Sections")
			{
				QDomNode Section = pageNode.firstChild();
				while (!Section.isNull())
				{
					QDomElement sectionElem = Section.toElement();
					if (sectionElem.tagName() == "Section")
					{
						struct DocumentSection newSection;
						newSection.number=sectionElem.attribute("Number").toInt();
						newSection.name=sectionElem.attribute("Name");
						newSection.fromindex=sectionElem.attribute("From").toInt();
						newSection.toindex=sectionElem.attribute("To").toInt();
						if (sectionElem.attribute("Type") == "Type_1_2_3")
							newSection.type=Type_1_2_3;
						if (sectionElem.attribute("Type") == "Type_i_ii_iii")
							newSection.type=Type_i_ii_iii;
						if (sectionElem.attribute("Type") == "Type_I_II_III")
							newSection.type=Type_I_II_III;
						if (sectionElem.attribute("Type") == "Type_a_b_c")
							newSection.type=Type_a_b_c;
						if (sectionElem.attribute("Type") == "Type_A_B_C")
							newSection.type=Type_A_B_C;
						newSection.sectionstartindex=sectionElem.attribute("Start").toInt();
						newSection.reversed=static_cast<bool>(sectionElem.attribute("Reversed").toInt());
						newSection.active=static_cast<bool>(sectionElem.attribute("Active").toInt());
						newSection.pageNumberWidth = 0;
						m_Doc->sections().insert(newSection.number, newSection);
					}
					Section = Section.nextSibling();
				}
			}
			if ((pg.tagName() == "PAGE") || (pg.tagName() == "MASTERPAGE"))
			{
				a = pg.attribute("NUM").toInt();
				pageName = "";
				pageName = pg.attribute("NAM", "");
				if (pg.tagName() == "MASTERPAGE" && pageName.isEmpty())
				{
					qDebug() << "scribus13format: corrupted masterpage with empty name detected";
					pageNode = pageNode.nextSibling();
					continue;
				}
				if (pageName.isEmpty())
					m_Doc->setMasterPageMode(false);
				else
					m_Doc->setMasterPageMode(true);
				if (pageName.isEmpty())
					Apage = m_Doc->addPage(a);
				else
					Apage = m_Doc->addMasterPage(a, pageName);
				Apage->LeftPg=pg.attribute("LEFT", "0").toInt();
				QString Mus = "";
				Mus = pg.attribute("MNAM","Normal");
				if (!m_Doc->masterPageMode())
					Apage->setMasterPageName(Mus);
				else
					Apage->clearMasterPageName();
				if (pg.hasAttribute("Size"))
					Apage->setSize(pg.attribute("Size"));
				if (pg.hasAttribute("Orientation"))
					Apage->setOrientation(pg.attribute("Orientation").toInt());
				Apage->setXOffset( ScCLocale::toDoubleC(pg.attribute("PAGEXPOS")) );
				Apage->setYOffset( ScCLocale::toDoubleC(pg.attribute("PAGEYPOS")) );
				if (pg.hasAttribute("PAGEWIDTH"))
					Apage->setWidth( ScCLocale::toDoubleC(pg.attribute("PAGEWIDTH")) );
				else
					Apage->setWidth( ScCLocale::toDoubleC(pg.attribute("PAGEWITH")) );
				Apage->setHeight( ScCLocale::toDoubleC(pg.attribute("PAGEHEIGHT")) );
				Apage->setInitialHeight(Apage->height());
				Apage->setInitialWidth(Apage->width());
				Apage->initialMargins.setTop(qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERTOP"))));
				Apage->initialMargins.setBottom(qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERBOTTOM"))));
				Apage->initialMargins.setLeft(qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERLEFT"))));
				Apage->initialMargins.setRight(qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERRIGHT"))));
				Apage->marginPreset = pg.attribute("PRESET", "0").toInt();
				Apage->Margins.setTop(Apage->initialMargins.top());
				Apage->Margins.setBottom(Apage->initialMargins.bottom());
				m_Doc->setMasterPageMode(false);
				// guides reading
				tmp = "";
				Apage->guides.setHorizontalAutoGap(0.0);
				Apage->guides.setVerticalAutoGap(0.0);
				Apage->guides.setHorizontalAutoCount(0);
				Apage->guides.setVerticalAutoCount(0);
				Apage->guides.setHorizontalAutoRefer(0);
				Apage->guides.setVerticalAutoRefer(0);
				GuideManagerIO::readVerticalGuides(pg.attribute("VerticalGuides"),
						Apage,
						GuideManagerCore::Standard,
						pg.hasAttribute("NumVGuides"));
				GuideManagerIO::readHorizontalGuides(pg.attribute("HorizontalGuides"),
						Apage,
						GuideManagerCore::Standard,
						pg.hasAttribute("NumHGuides"));
			}
			if ((pg.tagName() == "PAGEOBJECT") || (pg.tagName() == "MASTEROBJECT") || (pg.tagName() == "FRAMEOBJECT"))
			{
				if ((pg.tagName() == "PAGEOBJECT") || (pg.tagName() == "FRAMEOBJECT"))
					m_Doc->setMasterPageMode(false);
				else
					m_Doc->setMasterPageMode(true);
				int pagenr = -1;
				if ((!pg.attribute("OnMasterPage").isEmpty()) && (pg.tagName() == "MASTEROBJECT"))
				{
					m_Doc->setCurrentPage(m_Doc->MasterPages.at(m_Doc->MasterNames[pg.attribute("OnMasterPage")]));
					pagenr = -2;
				}

				if (pg.tagName() == "PAGEOBJECT")
				{
					itemRemap[itemCount++] = m_Doc->Items->count();
					// member of linked chain?
					if ((pg.attribute("NEXTITEM").toInt() != -1) )
					{
						itemNext[m_Doc->Items->count()] = pg.attribute("NEXTITEM").toInt();
					}
				}
				PageItem::ItemKind itemKind = PageItem::StandardItem;
				if (pg.tagName() == "FRAMEOBJECT")
					itemKind = PageItem::InlineItem;
				newItem = PasteItem(&pg, m_Doc, fileDir, itemKind, pagenr);
				newItem->setRedrawBounding();
				if (pg.tagName() == "MASTEROBJECT")
					newItem->setOwnerPage(m_Doc->OnPage(newItem));
				else
					newItem->setOwnerPage(pg.attribute("OwnPage").toInt());
				if (pg.tagName() == "PAGEOBJECT")
					newItem->setMasterPageName(QString());
				QDomNode node = pg.firstChild();
				LastStyles * last = new LastStyles();
				while (!node.isNull())
				{
					QDomElement it = node.toElement();
					if (it.tagName() == "CSTOP")
					{
						QString name = it.attribute("NAME");
						double ramp = ScCLocale::toDoubleC(it.attribute("RAMP"), 0.0);
						int shade   = it.attribute("SHADE", "100").toInt();
						double opa  = ScCLocale::toDoubleC(it.attribute("TRANS"), 1.0);
						handleOldColorShade(m_Doc, name, shade);
						newItem->fill_gradient.addStop(SetColor(m_Doc, name, shade), ramp, 0.5, opa, name, shade);
					}
					if (it.tagName() == "ITEXT")
						GetItemText(&it, m_Doc, newItem, last);

					//CB PageItemAttributes
					if (it.tagName() == "PageItemAttributes")
					{
						QDomNode pia = it.firstChild();
						ObjAttrVector pageItemAttributes;
						while (!pia.isNull())
						{
							QDomElement itemAttr = pia.toElement();
							if (itemAttr.tagName() == "ItemAttribute")
							{
								ObjectAttribute objattr;
								objattr.name = itemAttr.attribute("Name");
								objattr.type = itemAttr.attribute("Type");
								objattr.value = itemAttr.attribute("Value");
								objattr.parameter = itemAttr.attribute("Parameter");
								objattr.relationship = itemAttr.attribute("Relationship");
								objattr.relationshipto = itemAttr.attribute("RelationshipTo");
								objattr.autoaddto = itemAttr.attribute("AutoAddTo");
								pageItemAttributes.append(objattr);
							}
							pia = pia.nextSibling();
						}
						newItem->setObjectAttributes(&pageItemAttributes);
					}
					node = node.nextSibling();
				}
				delete last;
				if (newItem->fill_gradient.stops() == 0)
				{
					const ScColor& col1 = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeFillColor];
					const ScColor& col2 = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeLineColor];
					newItem->fill_gradient.addStop(ScColorEngine::getRGBColor(col1, m_Doc), 0.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeFillColor, 100);
					newItem->fill_gradient.addStop(ScColorEngine::getRGBColor(col2, m_Doc), 1.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeLineColor, 100);
				}
				newItem->isAutoText = static_cast<bool>(pg.attribute("AUTOTEXT").toInt());
				newItem->isEmbedded = static_cast<bool>(pg.attribute("isInline", "0").toInt());
				newItem->gXpos = ScCLocale::toDoubleC(pg.attribute("gXpos"), 0.0);
				newItem->gYpos = ScCLocale::toDoubleC(pg.attribute("gYpos"), 0.0);
				newItem->gWidth  = ScCLocale::toDoubleC(pg.attribute("gWidth"), newItem->width());
				newItem->gHeight = ScCLocale::toDoubleC(pg.attribute("gHeight"), newItem->height());
				if (newItem->isAutoText)
					m_Doc->LastAuto = newItem;
				if (pg.tagName() == "FRAMEOBJECT")
				{
					FrameItems.append(m_Doc->Items->takeAt(m_Doc->Items->indexOf(newItem)));
					newItem->setLayer(m_Doc->firstLayerID());
				}
				if (newItem->isTableItem)
				{
					if (pg.tagName() == "PAGEOBJECT")
					{
						TableItems.append(newItem);
						TableID.insert(pg.attribute("OwnLINK", "0").toInt(), m_Doc->DocItems.indexOf(newItem));
					}
					else if (pg.tagName() == "FRAMEOBJECT")
					{
						TableItemsF.append(newItem);
						TableIDF.insert(pg.attribute("OwnLINK", "0").toInt(), FrameItems.indexOf(newItem));
					}
					else
					{
						TableItemsM.append(newItem);
						TableIDM.insert(pg.attribute("OwnLINK", "0").toInt(), m_Doc->MasterItems.indexOf(newItem));
					}
				}
				m_Doc->setMasterPageMode(false);
				counter++;
			}
			pageNode = pageNode.nextSibling();
		}
		pageNode = docNode.firstChild();
		while (!pageNode.isNull())
		{
			QDomElement pg = pageNode.toElement();
			if (pg.tagName() == "Bookmark")
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
			pageNode = pageNode.nextSibling();
		}
		docNode = docNode.nextSibling();
	}
	if (TableItemsF.count() != 0)
	{
		for (int ttc = 0; ttc < TableItemsF.count(); ++ttc)
		{
			PageItem* ta = TableItemsF.at(ttc);
			if (ta->TopLinkID != -1)
				ta->m_topLink = FrameItems.at(TableIDF[ta->TopLinkID]);
			else
				ta->m_topLink = nullptr;
			if (ta->LeftLinkID != -1)
				ta->m_leftLink = FrameItems.at(TableIDF[ta->LeftLinkID]);
			else
				ta->m_leftLink = nullptr;
			if (ta->RightLinkID != -1)
				ta->m_rightLink = FrameItems.at(TableIDF[ta->RightLinkID]);
			else
				ta->m_rightLink = nullptr;
			if (ta->BottomLinkID != -1)
				ta->m_bottomLink = FrameItems.at(TableIDF[ta->BottomLinkID]);
			else
				ta->m_bottomLink = nullptr;
		}
	}
	if (TableItemsM.count() != 0)
	{
		for (int ttc = 0; ttc < TableItemsM.count(); ++ttc)
		{
			PageItem* ta = TableItemsM.at(ttc);
			if (ta->TopLinkID != -1)
				ta->m_topLink = m_Doc->MasterItems.at(TableIDM[ta->TopLinkID]);
			else
				ta->m_topLink = nullptr;
			if (ta->LeftLinkID != -1)
				ta->m_leftLink = m_Doc->MasterItems.at(TableIDM[ta->LeftLinkID]);
			else
				ta->m_leftLink = nullptr;
			if (ta->RightLinkID != -1)
				ta->m_rightLink = m_Doc->MasterItems.at(TableIDM[ta->RightLinkID]);
			else
				ta->m_rightLink = nullptr;
			if (ta->BottomLinkID != -1)
				ta->m_bottomLink = m_Doc->MasterItems.at(TableIDM[ta->BottomLinkID]);
			else
				ta->m_bottomLink = nullptr;
		}
	}
	if (TableItems.count() != 0)
	{
		for (int ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->m_topLink = m_Doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->m_topLink = nullptr;
			if (ta->LeftLinkID != -1)
				ta->m_leftLink = m_Doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->m_leftLink = nullptr;
			if (ta->RightLinkID != -1)
				ta->m_rightLink = m_Doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->m_rightLink = nullptr;
			if (ta->BottomLinkID != -1)
				ta->m_bottomLink = m_Doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->m_bottomLink = nullptr;
		}
	}
	m_Doc->setMasterPageMode(false);
	m_Doc->setLoading(false);
	m_Doc->reformPages(true);
	m_Doc->setLoading(true);

	handleOldLayerBehavior(m_Doc);
	if (m_Doc->Layers.count() == 0)
	{
		ScLayer* nl = m_Doc->Layers.newLayer( QObject::tr("Background") );
		nl->flowControl  = false;
		layerToSetActive = nl->ID;
	}
	m_Doc->setActiveLayer(layerToSetActive);
	if (!EffVal.isEmpty())
	{
		for (int pdoE = 0; pdoE < EffVal.count(); ++pdoE)
		{
			if (pdoE < m_Doc->Pages->count())
				m_Doc->Pages->at(pdoE)->PresentVals = EffVal[pdoE];
		}
	}

	// reestablish textframe links
	if (itemNext.count() != 0)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
		{
			if (itemRemap[lc.value()] >= 0)
			{
				PageItem *Its(nullptr), *Itn(nullptr);
				if (lc.key() < m_Doc->Items->count())
					Its = m_Doc->Items->at(lc.key());
				if (itemRemap[lc.value()] < m_Doc->Items->count())
					Itn = m_Doc->Items->at(itemRemap[lc.value()]);
				if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
				{
					qDebug("scribus13format: corruption in linked textframes detected");
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

	// fix legacy char formatting
	for (int i = 0; i < m_Doc->DocItems.count(); ++i)
	{
		PageItem* item = m_Doc->DocItems.at(i);
		if (item->prevInChain() == nullptr && item->itemText.length() > 0)
			item->itemText.fixLegacyFormatting();
	}
	for (int i = 0; i < m_Doc->MasterItems.count(); ++i)
	{
		PageItem* item = m_Doc->MasterItems.at(i);
		if (item->prevInChain() == nullptr && item->itemText.length() > 0)
			item->itemText.fixLegacyFormatting();
	}
	for (auto itf = m_Doc->FrameItems.begin(); itf != m_Doc->FrameItems.end(); ++itf)
	{
		PageItem *item = itf.value();
		if (item->prevInChain() == nullptr && item->itemText.length() > 0)
			item->itemText.fixLegacyFormatting();
	}
	for (int i = 0; i < m_Doc->DocItems.count(); ++i)
	{
		QList<PageItem*> allItems;
		PageItem* item = m_Doc->DocItems.at(i);
		if (item->isGroup())
			allItems = item->getAllChildren();
		else
			allItems.append(item);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			PageItem* gItem = allItems[ii];
			if (gItem->isGroup() && gItem->groupItemList[0]->isTableItem)
			{
				if (gItem->isGroupChild())
					convertOldTable(m_Doc, gItem, gItem->groupItemList, nullptr, &(gItem->asGroupFrame()->groupItemList));
				else
					convertOldTable(m_Doc, gItem, gItem->groupItemList, nullptr, &m_Doc->DocItems);
			}
		}
		allItems.clear();
	}
	for (int i = 0; i < m_Doc->MasterItems.count(); ++i)
	{
		QList<PageItem*> allItems;
		PageItem* item = m_Doc->MasterItems.at(i);
		if (item->isGroup())
			allItems = item->getAllChildren();
		else
			allItems.append(item);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			PageItem* gItem = allItems[ii];
			if (gItem->isGroup() && gItem->groupItemList[0]->isTableItem)
			{
				if (gItem->isGroupChild())
					convertOldTable(m_Doc, gItem, gItem->groupItemList, nullptr, &(gItem->asGroupFrame()->groupItemList));
				else
					convertOldTable(m_Doc, gItem, gItem->groupItemList, nullptr, &m_Doc->MasterItems);
			}
		}
		allItems.clear();
	}

	// start auto save timer if needed
	if (m_Doc->autoSave() && ScCore->usingGUI())
		m_Doc->restartAutoSaveTimer();
//		m_Doc->autoSaveTimer->start(m_Doc->autoSaveTime());

	if (m_mwProgressBar!=nullptr)
		m_mwProgressBar->setValue(docNode.childNodes().count());

	ScMessageBox::warning(ScCore->primaryMainWindow(),
						  CommonStrings::trWarning,
						  tr("You have opened a file produced by Scribus 1.3.3.x.\n"
						     "If you save it in this version, it will no longer be readable by older Scribus versions."));

	return true;
// 	return false;
}

// Low level plugin API
int scribus13format_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scribus13format_getPlugin()
{
	Scribus13Format* plug = new Scribus13Format();
	Q_CHECK_PTR(plug);
	return plug;
}

void scribus13format_freePlugin(ScPlugin* plugin)
{
	Scribus13Format* plug = dynamic_cast<Scribus13Format*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

void Scribus13Format::GetItemText(QDomElement *it, ScribusDoc *doc, PageItem* obj, LastStyles* last, bool impo, bool VorLFound)
{
	ScFace dummy = ScFace::none();
	QString tmp2, tmpf;
	CharStyle newStyle;

	tmp2 = it->attribute("CH");
	tmp2.replace(QRegExp("\r"), QChar(13));
	tmp2.replace(QRegExp("\n"), QChar(13));
	tmp2.replace(QRegExp("\t"), QChar(9));
	tmp2.replace(SpecialChars::OLD_LINEBREAK, SpecialChars::LINEBREAK);
	tmp2.replace(SpecialChars::OLD_NBHYPHEN, SpecialChars::NBHYPHEN);
	tmp2.replace(SpecialChars::OLD_NBSPACE, SpecialChars::NBSPACE);
	tmpf = it->attribute("CFONT", "");

	if (! tmpf.isEmpty() )
		newStyle.setFont(m_AvailableFonts->findFont(tmpf, doc));

	if (it->hasAttribute("CSIZE"))
		newStyle.setFontSize(qRound(ScCLocale::toDoubleC(it->attribute("CSIZE")) * 10));

	if (it->hasAttribute("CCOLOR"))
		newStyle.setFillColor(it->attribute("CCOLOR"));

	if (it->hasAttribute("CSHADE"))
		newStyle.setFillShade(it->attribute("CSHADE").toInt());

	QString fColor = newStyle.fillColor();
	int fShade = qRound(newStyle.fillShade());
	handleOldColorShade(doc, fColor, fShade);
	newStyle.setFillColor(fColor);
	newStyle.setFillShade(fShade);

	if (it->hasAttribute("CEXTRA"))
	{
		double cv = ScCLocale::toDoubleC(it->attribute("CEXTRA"));
		double cs = ScCLocale::toDoubleC(it->attribute("CSIZE"));
		newStyle.setTracking(qRound(cv / cs * 1000.0));
	}
	else if (it->hasAttribute("CKERN"))
		newStyle.setTracking(it->attribute("CKERN").toInt());

	if (it->hasAttribute("CSTYLE"))
		newStyle.setFeatures(static_cast<StyleFlag>(it->attribute("CSTYLE").toInt()).featureList());

	QString pstylename = it->attribute("PSTYLE", "");
	int calign = it->attribute("CALIGN", "-1").toInt();

	int ab = it->attribute("CAB", "-1").toInt();
	if (ab >= 5) {
		pstylename = DoVorl[ab];
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

	QString sColor = newStyle.strokeColor();
	int sShade = qRound(newStyle.strokeShade());
	handleOldColorShade(doc, sColor, sShade);
	newStyle.setStrokeColor(sColor);
	newStyle.setStrokeShade(sShade);

	if (it->hasAttribute("CSCALE"))
		newStyle.setScaleH(qMin(qMax(qRound(ScCLocale::toDoubleC(it->attribute("CSCALE"), 100.0) * 10), 100), 4000));

	if (it->hasAttribute("CSCALEV"))
		newStyle.setScaleV(qMin(qMax(qRound(ScCLocale::toDoubleC(it->attribute("CSCALEV"), 100.0) * 10), 100), 4000));

	if (it->hasAttribute("CBASE"))
		newStyle.setBaselineOffset(qRound(ScCLocale::toDoubleC(it->attribute("CBASE"), 0.0) * 10));

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
		
		if (impo && ab >= 0 && VorLFound)
			last->ParaStyle = DoVorl[ab];
		else
			last->ParaStyle = pstylename;

		int pos = obj->itemText.length();
		if (ch == SpecialChars::OBJECT)
		{
			if (iobj >= 0)
			{
				if (iobj < FrameItems.count())
				{
					int fIndex = doc->addToInlineFrames(FrameItems.at(iobj));
					obj->itemText.insertObject(pos, fIndex);
				}
				else
					qDebug() << QString("scribus134format: invalid inline frame used in text object : %1").arg(iobj);
			}
		}
		else {
			obj->itemText.insertChars(pos, QString(ch));
		}
		if (newStyle != last->Style) {
//			qDebug() << QString("new style at %1: %2 -> %3").arg(pos).arg(last->Style.asString()).arg(newStyle.asString();
			obj->itemText.applyCharStyle(last->StyleStart, pos-last->StyleStart, last->Style);
			last->Style = newStyle;
			last->StyleStart = pos;
		}
		if (ch == SpecialChars::PARSEP) {
			ParagraphStyle pstyle;
			pstyle.setParent( last->ParaStyle );
			if (calign >= 0)
				pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(calign));
//			qDebug() << QString("par style at %1: %2/%3 (%4) calign %5").arg(pos).arg(pstyle.name()).arg(pstyle.parent()).arg(last->ParaStyle).arg(calign);
			obj->itemText.applyStyle(pos, pstyle);
		}
	}
	obj->itemText.applyCharStyle(last->StyleStart, obj->itemText.length()-last->StyleStart, last->Style);
	ParagraphStyle pstyle;
	pstyle.setParent( last->ParaStyle );
	if (calign >= 0)
		pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(calign));
//	qDebug() << QString("par style at end: %1/%2 (%3) calign %4").arg(pstyle.name()).arg(pstyle.parent()).arg(last->ParaStyle).arg(calign);
	obj->itemText.applyStyle(obj->itemText.length() - 1, pstyle);
	//last->StyleStart = obj->itemText.length();
}


void Scribus13Format::readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, ScribusDoc *doc)
{
	vg.setName(pg.attribute("NAME"));
	vg.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(pg.attribute("LINESPMode", "0").toInt()));
	vg.setLineSpacing(ScCLocale::toDoubleC(pg.attribute("LINESP")));
	vg.setLeftMargin(ScCLocale::toDoubleC(pg.attribute("INDENT"), 0.0));
	if (pg.hasAttribute("RMARGIN"))
		vg.setRightMargin(ScCLocale::toDoubleC(pg.attribute("RMARGIN"), 0.0));
	else
		vg.setRightMargin(0);
	vg.setFirstIndent(ScCLocale::toDoubleC(pg.attribute("FIRST"), 0.0));
	vg.setAlignment(static_cast<ParagraphStyle::AlignmentType>(pg.attribute("ALIGN").toInt()));
	vg.setGapBefore(ScCLocale::toDoubleC(pg.attribute("VOR"), 0.0));
	vg.setGapAfter(ScCLocale::toDoubleC(pg.attribute("NACH"), 0.0));
	
	vg.charStyle().setFont(m_AvailableFonts->findFont(pg.attribute("FONT", doc->itemToolPrefs().textFont), doc));
	vg.charStyle().setFontSize(qRound(ScCLocale::toDoubleC(pg.attribute("FONTSIZE"), 12.0) * 10.0));
	vg.setHasDropCap(static_cast<bool>(pg.attribute("DROP", "0").toInt()));
	vg.setDropCapLines(pg.attribute("DROPLIN", "2").toInt());
	vg.setParEffectOffset(ScCLocale::toDoubleC(pg.attribute("DROPDIST"), 0.0));
	vg.charStyle().setFeatures(static_cast<StyleFlag>(pg.attribute("EFFECT", "0").toInt()).featureList());
	QString fColor = pg.attribute("FCOLOR", doc->itemToolPrefs().shapeFillColor);
	int fShade = pg.attribute("FSHADE", "100").toInt();
	handleOldColorShade(doc, fColor, fShade);
	QString sColor = pg.attribute("SCOLOR", doc->itemToolPrefs().shapeLineColor);
	int sShade = pg.attribute("SSHADE", "100").toInt();
	handleOldColorShade(doc, sColor, sShade);
	vg.charStyle().setFillColor(fColor);
	vg.charStyle().setFillShade(fShade);
	vg.charStyle().setStrokeColor(sColor);
	vg.charStyle().setStrokeShade(sShade);
	if (static_cast<bool>(pg.attribute("BASE", "0").toInt()))
		vg.setLineSpacingMode(ParagraphStyle::BaselineGridLineSpacing);
	vg.charStyle().setShadowXOffset(qRound(ScCLocale::toDoubleC(pg.attribute("TXTSHX"), 5.0) * 10));
	vg.charStyle().setShadowYOffset(qRound(ScCLocale::toDoubleC(pg.attribute("TXTSHY"), -5.0) * 10));
	vg.charStyle().setOutlineWidth(qRound(ScCLocale::toDoubleC(pg.attribute("TXTOUT"), 1.0) * 10));
	vg.charStyle().setUnderlineOffset(qRound(ScCLocale::toDoubleC(pg.attribute("TXTULP"), -0.1) * 10));
	vg.charStyle().setUnderlineWidth(qRound(ScCLocale::toDoubleC(pg.attribute("TXTULW"), -0.1) * 10));
	vg.charStyle().setStrikethruOffset(qRound(ScCLocale::toDoubleC(pg.attribute("TXTSTP"), -0.1) * 10));
	vg.charStyle().setStrikethruWidth(qRound(ScCLocale::toDoubleC(pg.attribute("TXTSTW"), -0.1) * 10));
	vg.charStyle().setScaleH(qRound(ScCLocale::toDoubleC(pg.attribute("SCALEH"), 100.0) * 10));
	vg.charStyle().setScaleV(qRound(ScCLocale::toDoubleC(pg.attribute("SCALEV"), 100.0) * 10));
	vg.charStyle().setBaselineOffset(qRound(ScCLocale::toDoubleC(pg.attribute("BASEO"), 0.0) * 10));
	vg.charStyle().setTracking(qRound(ScCLocale::toDoubleC(pg.attribute("KERN"), 0.0) * 10));
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
		vg.setTabValues(tbs);
		QDomNode node = pg.firstChild();
		while (!node.isNull())
		{
			QDomElement it = node.toElement();
			if (it.tagName() == "Tabs")
			{
				ParagraphStyle::TabRecord tb;
				tb.tabPosition = ScCLocale::toDoubleC(it.attribute("Pos"));
				tb.tabType = it.attribute("Type").toInt();
				QString tbCh = "";
				tbCh = it.attribute("Fill","");
				if (tbCh.isEmpty())
					tb.tabFillChar = QChar();
				else
					tb.tabFillChar = tbCh[0];
				tbs.append(tb);
			}
// 			vg.setTabValues(tbs);
			node = node.nextSibling();
		}
		if (tbs.count() > 0)
			vg.setTabValues(tbs);
	}
}

PageItem* Scribus13Format::PasteItem(QDomElement *obj, ScribusDoc *doc, const QString& baseDir, PageItem::ItemKind itemKind, int pagenr)
{
	struct ImageLoadRequest loadingInfo;
	int z = 0;
	PageItem::ItemType pt = static_cast<PageItem::ItemType>(obj->attribute("PTYPE").toInt());
	double x  = ScCLocale::toDoubleC(obj->attribute("XPOS"));
	double y  = ScCLocale::toDoubleC(obj->attribute("YPOS"));
	double w  = ScCLocale::toDoubleC(obj->attribute("WIDTH"));
	double h  = ScCLocale::toDoubleC(obj->attribute("HEIGHT"));
	double pw = ScCLocale::toDoubleC(obj->attribute("PWIDTH"));
	double offsX = ScCLocale::toDoubleC(obj->attribute("LOCALX"));
	double offsY = ScCLocale::toDoubleC(obj->attribute("LOCALY"));
	double scx = ScCLocale::toDoubleC(obj->attribute("LOCALSCX"));
	double scy = ScCLocale::toDoubleC(obj->attribute("LOCALSCY"));
	double rot = ScCLocale::toDoubleC(obj->attribute("LOCALROT"));
	QString Pcolor = obj->attribute("PCOLOR");
	if (Pcolor.isEmpty())
		Pcolor = "None";
	QString Pcolor2 = obj->attribute("PCOLOR2");
	if (Pcolor2.isEmpty())
		Pcolor2 = "None";
	int Pshade = obj->attribute("SHADE").toInt();
	int Pshade2 = obj->attribute("SHADE2").toInt();
	handleOldColorShade(doc, Pcolor, Pshade);
	handleOldColorShade(doc, Pcolor2, Pshade2);
	QColor tmpc;
	PageItem *currItem=nullptr;
	QString tmp;
	double xf, yf, xf2;
	QString clPath;
	QDomNode node;
	switch (pt)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->setOwnerPage(pagenr);
		break;
	//
	case PageItem::ImageFrame:
		z = doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x, y, w, h, 1, doc->itemToolPrefs().imageFillColor, CommonStrings::None, itemKind);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->setOwnerPage(pagenr);
		UndoManager::instance()->setUndoEnabled(false);
		currItem->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
		currItem->AspectRatio = obj->attribute("RATIO", "0").toInt();
		currItem->setImageXYScale(scx, scy);
		currItem->setImageXYOffset(offsX, offsY);
		currItem->setImageRotation(rot);
		currItem->Pfile     = Relative2Path(obj->attribute("PFILE"), baseDir);
		currItem->ImageProfile = obj->attribute("PRFILE","");
		currItem->ImageIntent  = (eRenderIntent) obj->attribute("IRENDER", "1").toInt();
		currItem->EmbeddedProfile = obj->attribute("EPROF","");
		currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
		currItem->pixm.imgInfo.lowResType = obj->attribute("ImageRes", "1").toInt();
		node = obj->firstChild();
		while (!node.isNull())
		{
			QDomElement it = node.toElement();
			if (it.tagName() == "ImageEffect")
			{
				struct ImageEffect ef;
				ef.effectParameters = it.attribute("Param");
				ef.effectCode = it.attribute("Code").toInt();
				currItem->effectsInUse.append(ef);
			}
			node = node.nextSibling();
		}
		if (!currItem->Pfile.isEmpty())
			doc->loadPict(currItem->Pfile, currItem, false);
		currItem->ImageProfile    = obj->attribute("PRFILE","");
		currItem->ImageIntent     = (eRenderIntent) obj->attribute("IRENDER", "1").toInt();
		currItem->EmbeddedProfile = obj->attribute("EPROF","");
		currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
		currItem->setImageXYScale(scx, scy);
		clPath = obj->attribute("ImageClip", "");
		if (currItem->pixm.imgInfo.PDSpathData.contains(clPath))
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[clPath].copy();
			currItem->pixm.imgInfo.usedPath = clPath;
			QTransform cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		currItem->setImageVisible(obj->attribute("PICART").toInt());
/*		currItem->BBoxX = obj->attribute("BBOXX").toDouble();
		currItem->BBoxH = obj->attribute("BBOXH").toDouble(); */
		currItem->setLineWidth(pw);
		if (currItem->pixm.imgInfo.layerInfo.count() != 0)
		{
			bool found = false;
			node = obj->firstChild();
			while (!node.isNull())
			{
				QDomElement it = node.toElement();
				if (it.tagName() == "PSDLayer")
				{
					found = true;
					loadingInfo.blend = it.attribute("Blend");
					loadingInfo.opacity = it.attribute("Opacity").toInt();
					loadingInfo.visible = static_cast<bool>(it.attribute("Visible").toInt());
					loadingInfo.useMask = static_cast<bool>(it.attribute("useMask", "1").toInt());
					currItem->pixm.imgInfo.RequestProps.insert(it.attribute("Layer").toInt(), loadingInfo);
				}
				node = node.nextSibling();
			}
			if (found)
			{
				currItem->pixm.imgInfo.isRequest = true;
				doc->loadPict(currItem->Pfile, currItem, true);
			}
		}
		UndoManager::instance()->setUndoEnabled(true);
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType3:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->setOwnerPage(pagenr);
		break;
	//
	case PageItem::PathText:
		z = doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor, itemKind);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->setOwnerPage(pagenr);
		break;
	case PageItem::TextFrame:
		z = doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor, itemKind);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->setOwnerPage(pagenr);
		break;
	case PageItem::Line:
		z = doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->setOwnerPage(pagenr);
		break;
	case PageItem::Polygon:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->setOwnerPage(pagenr);
		break;
	case PageItem::PolyLine:
		z = doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->setOwnerPage(pagenr);
		break;
	case PageItem::Multiple:
	default:
		assert(false);
		break;
	}

	UndoManager::instance()->setUndoEnabled(false);
	currItem->FrameType = obj->attribute("FRTYPE", "0").toInt();
	int startArrowIndex = obj->attribute("startArrowIndex", "0").toInt();
	if ((startArrowIndex < 0) || (startArrowIndex > static_cast<int>(doc->arrowStyles().size())))
	{
		qDebug() << QString("scribus13format: invalid arrow index: %").arg(startArrowIndex);
		startArrowIndex = 0;
	}
	currItem->setStartArrowIndex(startArrowIndex);
	int endArrowIndex = obj->attribute("endArrowIndex", "0").toInt();
	if ((endArrowIndex < 0) || (endArrowIndex > static_cast<int>(doc->arrowStyles().size())))
	{
		qDebug() << QString("scribus13format: invalid arrow index: %").arg(endArrowIndex);
		endArrowIndex = 0;
	}
	currItem->setEndArrowIndex(endArrowIndex);
	currItem->setStartArrowScale(obj->attribute("startArrowScale", "100").toInt());
	currItem->setEndArrowScale(obj->attribute("endArrowScale", "100").toInt());
	currItem->NamedLStyle = obj->attribute("NAMEDLST", "");
	currItem->isBookmark = obj->attribute("BOOKMARK").toInt();
	if ((currItem->isBookmark) && (doc->BookMarks.count() == 0))
		doc->OldBM = true;
	currItem->setImageFlippedH(obj->attribute("FLIPPEDH").toInt());
	currItem->setImageFlippedV(obj->attribute("FLIPPEDV").toInt());
	currItem->setCornerRadius(ScCLocale::toDoubleC(obj->attribute("RADRECT"), 0.0));
	currItem->ClipEdited = obj->attribute("CLIPEDIT", "0").toInt();
	currItem->setFillColor(Pcolor);
	if (currItem->lineWidth() == 0.0)
		currItem->setLineColor(CommonStrings::None);
	else
		currItem->setLineColor(Pcolor2);
	currItem->setFillShade(Pshade);
	currItem->setLineShade(Pshade2);
	ParagraphStyle pstyle;
	pstyle.setLineSpacing(ScCLocale::toDoubleC(obj->attribute("LINESP")));
	pstyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(obj->attribute("LINESPMode", "0").toInt()));
	int align = obj->attribute("ALIGN", "-1").toInt();
	if (align >= 5)
		pstyle.setParent(DoVorl[align-5]);
	else if (align >= 0)
		pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(align));
	if (static_cast<bool>(obj->attribute("REVERS", "0").toInt()))
		pstyle.setDirection(ParagraphStyle::RTL);
	pstyle.charStyle().setFont(m_AvailableFonts->findFont(obj->attribute("IFONT", m_Doc->itemToolPrefs().textFont), m_Doc));
	pstyle.charStyle().setFontSize(qRound(ScCLocale::toDoubleC(obj->attribute("ISIZE"), 12.0) * 10));
	pstyle.charStyle().setStrokeColor(obj->attribute("TXTSTROKE", CommonStrings::None));
	pstyle.charStyle().setFillColor(obj->attribute("TXTFILL", "Black"));
	pstyle.charStyle().setStrokeShade(obj->attribute("TXTSTRSH", "100").toInt());
	pstyle.charStyle().setFillShade(obj->attribute("TXTFILLSH", "100").toInt());
	pstyle.charStyle().setScaleH(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSCALE"), 100.0) * 10));
	pstyle.charStyle().setScaleV(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSCALEV"), 100.0) * 10));
	pstyle.charStyle().setBaselineOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTBASE"), 0.0) * 10));
	pstyle.charStyle().setShadowXOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSHX"), 5.0) * 10));
	pstyle.charStyle().setShadowYOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSHY"), -5.0) * 10));
	pstyle.charStyle().setOutlineWidth(qRound(ScCLocale::toDoubleC(obj->attribute("TXTOUT"), 1.0) * 10));
	pstyle.charStyle().setUnderlineOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTULP"), -0.1) * 10));
	pstyle.charStyle().setUnderlineWidth(qRound(ScCLocale::toDoubleC(obj->attribute("TXTULW"), -0.1) * 10));
	pstyle.charStyle().setStrikethruOffset(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSTP"), -0.1) * 10));
	pstyle.charStyle().setStrikethruWidth(qRound(ScCLocale::toDoubleC(obj->attribute("TXTSTW"), -0.1) * 10));
	if (obj->hasAttribute("EXTRAV"))
	{
		double ev = ScCLocale::toDoubleC(obj->attribute("EXTRAV"), 0.0);
		double iv = ScCLocale::toDoubleC(obj->attribute("ISIZE"), 12.0);
		pstyle.charStyle().setTracking(qRound(ev / iv * 10000.0));
	}
	else 
		pstyle.charStyle().setTracking(qRound(ScCLocale::toDoubleC(obj->attribute("TXTKERN"), 0.0) * 10));
	pstyle.charStyle().setFeatures(static_cast<StyleFlag>(obj->attribute("TXTSTYLE", "0").toInt()).featureList());
	tmp = "";
	QList<ParagraphStyle::TabRecord> tbValues;
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
			tbValues.append(tb);
		}
		tmp = "";
	}
	else
	{
		node = obj->firstChild();
		while (!node.isNull())
		{
			QDomElement it = node.toElement();
			if (it.tagName() == "Tabs")
			{
				ParagraphStyle::TabRecord tb;
				tb.tabPosition = ScCLocale::toDoubleC(it.attribute("Pos"));
				tb.tabType = it.attribute("Type").toInt();
				QString tbCh = "";
				tbCh = it.attribute("Fill","");
				if (tbCh.isEmpty())
					tb.tabFillChar = QChar();
				else
					tb.tabFillChar = tbCh[0];
				tbValues.append(tb);
			}
			node = node.nextSibling();
		}
	}
	if (!tbValues.empty())
		pstyle.setTabValues(tbValues);
	//FIXME: what if linked frames have different styles?
	currItem->itemText.setDefaultStyle(pstyle);

	currItem->setRotation(ScCLocale::toDoubleC(obj->attribute("ROT")));
	currItem->oldRot = currItem->rotation();
	currItem->setTextToFrameDist(ScCLocale::toDoubleC(obj->attribute("EXTRA")),
								ScCLocale::toDoubleC(obj->attribute("REXTRA"), 1.0),
								ScCLocale::toDoubleC(obj->attribute("TEXTRA"), 1.0),
								ScCLocale::toDoubleC(obj->attribute("BEXTRA"), 1.0));

	currItem->PLineArt = Qt::PenStyle(obj->attribute("PLINEART").toInt());
	currItem->PLineEnd = Qt::PenCapStyle(obj->attribute("PLINEEND", "0").toInt());
	currItem->PLineJoin = Qt::PenJoinStyle(obj->attribute("PLINEJOIN", "0").toInt());
	currItem->setPrintEnabled(obj->attribute("PRINTABLE").toInt());
	currItem->setIsAnnotation(obj->attribute("ANNOTATION", "0").toInt());
	currItem->annotation().setType(obj->attribute("ANTYPE", "0").toInt());
	QString itemName = obj->attribute("ANNAME","");
	if (!itemName.isEmpty())
	{
		if (currItem->itemName() == itemName)
			currItem->AutoName = true;
		else
			currItem->setItemName(itemName);
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
	{
		QFileInfo efp(currItem->annotation().Extern());
		currItem->annotation().setExtern(efp.absoluteFilePath());
	}
	currItem->annotation().setZiel(obj->attribute("ANZIEL", "0").toInt());
	currItem->annotation().setToolTip(obj->attribute("ANTOOLTIP",""));
	currItem->annotation().setRollOver(obj->attribute("ANROLL",""));
	currItem->annotation().setDown(obj->attribute("ANDOWN",""));
	currItem->annotation().setBorderWidth(obj->attribute("ANBWID", "1").toInt());
	currItem->annotation().setBorderStyle(obj->attribute("ANBSTY", "0").toInt());
	currItem->annotation().setFeed(obj->attribute("ANFEED", "1").toInt());
	currItem->annotation().setFlag(obj->attribute("ANFLAG", "0").toInt());
	currItem->annotation().setFont(obj->attribute("ANFONT", "4").toInt());
	currItem->annotation().setFormat(obj->attribute("ANFORMAT", "0").toInt());
	currItem->annotation().setVis(obj->attribute("ANVIS", "0").toInt());
	currItem->annotation().setIsChk(static_cast<bool>(obj->attribute("ANCHK", "0").toInt()));
	currItem->annotation().setCheckState(currItem->annotation().IsChk());
	currItem->annotation().setAAact(static_cast<bool>(obj->attribute("ANAA", "0").toInt()));
	currItem->annotation().setHTML(obj->attribute("ANHTML", "0").toInt());
	currItem->annotation().setUseIcons(static_cast<bool>(obj->attribute("ANICON", "0").toInt()));
	currItem->annotation().setChkStil(obj->attribute("ANCHKS", "0").toInt());
	currItem->annotation().setMaxChar(obj->attribute("ANMC", "-1").toInt());
	currItem->annotation().setBorderColor(obj->attribute("ANBCOL", CommonStrings::None));
	currItem->annotation().setIPlace(obj->attribute("ANPLACE", "1").toInt());
	currItem->annotation().setScaleW(obj->attribute("ANSCALE", "0").toInt());

	if (currItem->asTextFrame() || currItem->asPathText())
	{
		UndoManager::instance()->setUndoEnabled(false);
		if (currItem->isAnnotation() && currItem->annotation().UseIcons())
		{
			currItem->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
			currItem->AspectRatio = obj->attribute("RATIO", "0").toInt();
			currItem->setImageXYScale(scx, scy);
			currItem->setImageXYOffset(offsX, offsY);
			currItem->setImageRotation(rot);
			currItem->Pfile  = Relative2Path(obj->attribute("PFILE" ,""), baseDir);
			currItem->Pfile2 = Relative2Path(obj->attribute("PFILE2",""), baseDir);
			currItem->Pfile3 = Relative2Path(obj->attribute("PFILE3",""), baseDir);
			currItem->ImageProfile    = obj->attribute("PRFILE","");
			currItem->ImageIntent     = (eRenderIntent) obj->attribute("IRENDER", "1").toInt();
			currItem->EmbeddedProfile = obj->attribute("EPROF","");
			currItem->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
			doc->loadPict(currItem->Pfile, currItem);
			currItem->setImageXYScale(scx, scy);
			currItem->setImageVisible(obj->attribute("PICART").toInt());
/*			currItem->BBoxX = obj->attribute("BBOXX").toDouble();
			currItem->BBoxH = obj->attribute("BBOXH").toDouble(); */
		}
		//currItem->convertTo(pt);
		UndoManager::instance()->setUndoEnabled(true);
	}

	currItem->TopLine = static_cast<bool>(obj->attribute("TopLine", "0").toInt());
	currItem->LeftLine = static_cast<bool>(obj->attribute("LeftLine", "0").toInt());
	currItem->RightLine = static_cast<bool>(obj->attribute("RightLine", "0").toInt());
	currItem->BottomLine = static_cast<bool>(obj->attribute("BottomLine", "0").toInt());
	currItem->isTableItem = static_cast<bool>(obj->attribute("isTableItem", "0").toInt());
	currItem->TopLinkID =  obj->attribute("TopLINK", "-1").toInt();
	currItem->LeftLinkID =  obj->attribute("LeftLINK", "-1").toInt();
	currItem->RightLinkID =  obj->attribute("RightLINK", "-1").toInt();
	currItem->BottomLinkID =  obj->attribute("BottomLINK", "-1").toInt();
	currItem->PoShow   = obj->attribute("PLTSHOW", "0").toInt();
	currItem->BaseOffs = ScCLocale::toDoubleC(obj->attribute("BASEOF"), 0.0);
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
	currItem->DashOffset = ScCLocale::toDoubleC(obj->attribute("DASHOFF"), 0.0);
	currItem->setLocked(static_cast<bool>(obj->attribute("LOCK", "0").toInt()));
	currItem->setSizeLocked(static_cast<bool>(obj->attribute("LOCKR", "0").toInt()));
	currItem->setFillTransparency(ScCLocale::toDoubleC(obj->attribute("TransValue"), 0.0));
	currItem->fillRule = static_cast<bool>(obj->attribute("fillRule", "1").toInt());
	currItem->doOverprint = static_cast<bool>(obj->attribute("doOverprint", "0").toInt());
	if (obj->hasAttribute("TransValueS"))
		currItem->setLineTransparency(ScCLocale::toDoubleC(obj->attribute("TransValueS"), 0.0));
	else
		currItem->setLineTransparency(ScCLocale::toDoubleC(obj->attribute("TransValue"), 0.0));
	currItem->setFillBlendmode(0);
	currItem->setLineBlendmode(0);
	if (obj->attribute("TRANSPARENT", "0").toInt() == 1)
		currItem->setFillColor(CommonStrings::None);
	currItem->m_columns   = obj->attribute("COLUMNS", "1").toInt();
	currItem->m_columnGap = ScCLocale::toDoubleC(obj->attribute("COLGAP"), 0.0);
	if (obj->attribute("LAYER", "0").toInt() != -1)
		currItem->setLayer(obj->attribute("LAYER", "0").toInt());
	tmp = "";
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
		double maxVal = std::numeric_limits<double>::max() / 2.0;
		ScTextStream fp(&tmp, QIODevice::ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMPO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			if (xf >= 999999)
				xf = maxVal;
			if (yf >= 999999)
				yf = maxVal;
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
		double maxVal = std::numeric_limits<double>::max() / 2.0;
		ScTextStream fp(&tmp, QIODevice::ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMCO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			if (xf >= 999999)
				xf = maxVal;
			if (yf >= 999999)
				yf = maxVal;
			currItem->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		currItem->ContourLine = currItem->PoLine.copy();
	if (!currItem->asLine())
		currItem->Clip = flattenPath(currItem->PoLine, currItem->Segments);
	else
	{
		currItem->Segments.clear();
		currItem->PoLine.resize(0);
		currItem->setHeight(1.0);
		currItem->asLine()->setLineClip();
	}
	if (currItem->asImageFrame())
		currItem->adjustPictScale();
	if (currItem->asPathText())
		currItem->updatePolyClip();
	currItem->GrType = obj->attribute("GRTYP", "0").toInt();
	QString GrColor;
	QString GrColor2;
	int GrShade = 0;
	int GrShade2 = 0;
	if (currItem->GrType != 0)
	{
		currItem->GrStartX = ScCLocale::toDoubleC(obj->attribute("GRSTARTX"), 0.0);
		currItem->GrStartY = ScCLocale::toDoubleC(obj->attribute("GRSTARTY"), 0.0);
		currItem->GrEndX = ScCLocale::toDoubleC(obj->attribute("GRENDX"), currItem->width());
		currItem->GrEndY = ScCLocale::toDoubleC(obj->attribute("GRENDY"), 0.0);
		currItem->GrFocalX = currItem->GrStartX;
		currItem->GrFocalY = currItem->GrStartY;
		currItem->GrScale  = 1.0;
		currItem->GrSkew  = 0.0;
		GrColor = obj->attribute("GRCOLOR","");
		if (!GrColor.isEmpty())
		{
			GrColor2 = obj->attribute("GRCOLOR2","");
			GrShade = obj->attribute("GRSHADE", "100").toInt();
			GrShade2 = obj->attribute("GRSHADE2", "100").toInt();
		}
	}
	if (currItem->GrType != 0)
	{
		currItem->fill_gradient.clearStops();
		if ((!GrColor.isEmpty()) && (!GrColor2.isEmpty()))
		{
			if (currItem->GrType == 5)
			{
				handleOldColorShade(doc, GrColor, GrShade);
				if ((GrColor != CommonStrings::None) && (!GrColor.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor, GrShade);
				currItem->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor, GrShade);
				handleOldColorShade(doc, GrColor2, GrShade2);
				if ((GrColor2 != CommonStrings::None) && (!GrColor2.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor2, GrShade2);
				currItem->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor2, GrShade2);
			}
			else
			{
				handleOldColorShade(doc, GrColor2, GrShade2);
				if ((GrColor2 != CommonStrings::None) && (!GrColor2.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor2, GrShade2);
				currItem->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor2, GrShade2);
				handleOldColorShade(doc, GrColor, GrShade);
				if ((GrColor != CommonStrings::None) && (!GrColor.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor, GrShade);
				currItem->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor, GrShade);
			}
		}
//		currItem->updateGradientVectors();
	}
	switch (currItem->GrType)
	{
		case 1:
		case 2:
		case 3:
		case 4:
			currItem->GrType = 6;
			break;
		case 5:
			currItem->GrType = 7;
			break;
		default:
			break;
	}
	//currItem->setRedrawBounding();
	//currItem->OwnPage = view->OnPage(currItem);
	UndoManager::instance()->setUndoEnabled(true);
	return currItem;
}

bool Scribus13Format::loadPage(const QString & fileName, int pageNumber, bool Mpage, const QString& renamedPageName)
{
// 	qDebug() << QString("loading page %2 from file '%1' from 1.3.3.x plugin").arg(fileName).arg(pageNumber);
	if (m_Doc==nullptr || m_AvailableFonts==nullptr)
	{
		Q_ASSERT(m_Doc==nullptr || m_AvailableFonts==nullptr);
		return false;
	}
	ParagraphStyle vg;
	struct ScribusDoc::BookMa bok;
	PageItem *newItem;
	ScPage* Apage = nullptr;
	FrameItems.clear();
	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	QString tmp;
	ReplacedFonts.clear();
	newReplacement = false;
	QMap<int,int> TableID;
	QList<PageItem*> TableItems;
	int a, counter;
	double pageX = 0, pageY = 0;
	bool VorLFound = false;
	QMap<int,int> layerTrans;
	int maxLayer = 0;
	int maxLevel = 0;
	layerTrans.clear();
	uint layerCount=m_Doc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		maxLayer = qMax(m_Doc->Layers[la2].ID, maxLayer);
		maxLevel = qMax(m_Doc->Layers[la2].Level, maxLevel);
	}
	DoVorl.clear();
	DoVorl[0] = "";
	DoVorl[1] = "";
	DoVorl[2] = "";
	DoVorl[3] = "";
	DoVorl[4] = "";
	VorlC = 5;
	QDomDocument docu("scridoc");
 	QString f(readSLA(fileName));
	if (f.isEmpty())
	{
		setFileReadError();
		return false;
	}
	QString errorMsg;
	int errorLine, errorColumn;
	if (!docu.setContent(f, &errorMsg, &errorLine, &errorColumn))
	{
		setDomParsingError(errorMsg, errorLine, errorColumn);
		return false;
	}
	QString fileDir = QFileInfo(fileName).absolutePath();
	ScColor lf = ScColor();
	QDomElement elem = docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode docNode = elem.firstChild();
	counter = m_Doc->Items->count();
	while (!docNode.isNull())
	{
		QDomElement dc = docNode.toElement();
		QDomNode pageNode = docNode.firstChild();
		while (!pageNode.isNull())
		{
			QDomElement pg = pageNode.toElement();
			if (pg.tagName() == "COLOR" && pg.attribute("NAME") != CommonStrings::None)
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
			if (pg.tagName() == "STYLE")
			{
				GetStyle(&pg, &vg, nullptr, m_Doc, true);
				VorLFound = true;
			}
			if (pg.tagName() == "JAVA")
				m_Doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if (pg.tagName() == "LAYERS")
			{
				int lId   = pg.attribute("NUMMER").toInt();
				int level = pg.attribute("LEVEL").toInt();
				ScLayer la( pg.attribute("NAME"), level, lId );
				la.isViewable   = pg.attribute("SICHTBAR").toInt();
				la.isPrintable  = pg.attribute("DRUCKEN").toInt();
				la.isEditable   = pg.attribute("EDIT", "1").toInt();
				la.flowControl  = pg.attribute("FLOW", "1").toInt();
				la.transparency = ScCLocale::toDoubleC(pg.attribute("TRANS"), 1.0);
				la.blendMode    = pg.attribute("BLEND", "0").toInt();
				la.outlineMode  = pg.attribute("OUTL", "0").toInt();
				if (pg.hasAttribute("LAYERC"))
					la.markerColor =  QColor(pg.attribute("LAYERC","#000000"));
				const ScLayer* la2 = m_Doc->Layers.layerByName(la.Name);
				if (la2)
					layerTrans.insert(la.ID, la2->ID);
				else
				{
					maxLayer++;
					maxLevel++;
					layerTrans.insert(la.ID, maxLayer);
					la.ID = maxLayer;
					la.Level = maxLevel;
					m_Doc->Layers.append(la);
				}
			}
			if (pg.tagName() == "MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = pageNode.firstChild();
				while (!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct SingleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = MuL.attribute("Dash").toInt();
					sl.LineEnd = MuL.attribute("LineEnd").toInt();
					sl.LineJoin = MuL.attribute("LineJoin").toInt();
					sl.Shade = MuL.attribute("Shade").toInt();
					sl.Width = ScCLocale::toDoubleC(MuL.attribute("Width"));
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				QString mlName = pg.attribute("Name");
				QString mlName2 = mlName;
				QHash<QString,multiLine>::ConstIterator mlit = m_Doc->docLineStyles.find(mlName2);
				if (mlit != m_Doc->docLineStyles.constEnd() && ml != mlit.value())
					mlName2 = getUniqueName(mlName2, m_Doc->docLineStyles);
				m_Doc->docLineStyles.insert(mlName2, ml);
			}
			if (pg.tagName() == "Arrows")
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
				m_Doc->appendToArrowStyles(arrow);
			}
			if (((pg.tagName() == "PAGE") || (pg.tagName() == "MASTERPAGE")) && (pg.attribute("NUM").toInt() == pageNumber))
			{
				a = m_Doc->currentPage()->pageNr();
				Apage = m_Doc->Pages->at(a);
				if ((pg.tagName() != "MASTERPAGE") && (Mpage))
				{
					pageNode = pageNode.nextSibling();
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
					Apage->setSize(pg.attribute("Size"));
				if (pg.hasAttribute("Orientation"))
					Apage->setOrientation(pg.attribute("Orientation").toInt());
				if (pg.hasAttribute("PAGEWIDTH"))
					Apage->setWidth(ScCLocale::toDoubleC(pg.attribute("PAGEWIDTH")));
				else
					Apage->setWidth(ScCLocale::toDoubleC(pg.attribute("PAGEWITH")));
				Apage->setHeight(ScCLocale::toDoubleC(pg.attribute("PAGEHEIGHT")));
				Apage->setInitialHeight(Apage->height());
				Apage->setInitialWidth(Apage->width());
				Apage->initialMargins.setTop(qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERTOP"))));
				Apage->initialMargins.setBottom(qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERBOTTOM"))));
				Apage->initialMargins.setLeft(qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERLEFT"))));
				Apage->initialMargins.setRight(qMax(0.0, ScCLocale::toDoubleC(pg.attribute("BORDERRIGHT"))));
				Apage->marginPreset = pg.attribute("PRESET", "0").toInt();
				Apage->Margins.setTop(Apage->initialMargins.top());
				Apage->Margins.setBottom(Apage->initialMargins.bottom());
				pageX = ScCLocale::toDoubleC(pg.attribute("PAGEXPOS"));
				pageY = ScCLocale::toDoubleC(pg.attribute("PAGEYPOS"));
				// guides reading
				tmp = "";
				GuideManagerIO::readVerticalGuides(pg.attribute("VerticalGuides"),
												Apage,
												GuideManagerCore::Standard,
												pg.hasAttribute("NumVGuides"));
				GuideManagerIO::readHorizontalGuides(pg.attribute("HorizontalGuides"),
												Apage,
												GuideManagerCore::Standard,
												pg.hasAttribute("NumHGuides"));
			}
			if ((pg.tagName() == "PAGEOBJECT") || (pg.tagName() == "MASTEROBJECT") || (pg.tagName() == "FRAMEOBJECT"))
			{
				if (Mpage)
				{
					if (pg.tagName() != "MASTEROBJECT")
					{
						pageNode = pageNode.nextSibling();
						continue;
					}
				}
				else
				{
					if (pg.tagName() == "MASTEROBJECT")
					{
						pageNode = pageNode.nextSibling();
						continue;
					}
				}
				
				if (pg.attribute("OwnPage").toInt() != pageNumber)
				{			
					if (pg.tagName() == "PAGEOBJECT")
						itemRemap[itemCount++] = -1;
				}
				else
				{
					// first of linked chain?
					if (pg.tagName() == "PAGEOBJECT")
					{
						itemRemap[itemCount++] = m_Doc->Items->count();
						if (pg.attribute("NEXTITEM").toInt() != -1)
						{
							itemNext[m_Doc->Items->count()] = pg.attribute("NEXTITEM").toInt();
						}
					}
					PageItem::ItemKind itemKind = PageItem::StandardItem;
					if (pg.tagName() == "FRAMEOBJECT")
						itemKind = PageItem::InlineItem;
					/*int docGc = m_Doc->GroupCounter;
					m_Doc->GroupCounter = 0;*/
					newItem = PasteItem(&pg, m_Doc, fileDir, itemKind);
					newItem->moveBy(-pageX + Apage->xOffset(), - pageY + Apage->yOffset());
					newItem->setRedrawBounding();
					//CB Must run onpage as we cant use pagetoload if the page has been renamed. 
					//CB TODO Make this accept a page to place onto.
					//newItem->OwnPage = m_Doc->OnPage(newItem);
					//newItem->OwnPage = PageToLoad;
					newItem->setOwnerPage(m_Doc->currentPageNumber());
					if (pg.tagName() == "PAGEOBJECT")
						newItem->setMasterPageName(QString());
					else if (Mpage && !renamedPageName.isEmpty())
						newItem->setMasterPageName(renamedPageName);
					newItem->setLayer(layerTrans.value(newItem->m_layerID, newItem->m_layerID));
					/*m_Doc->GroupCounter = docGc;*/
					QDomNode node = pg.firstChild();
					LastStyles * last = new LastStyles();
					while (!node.isNull())
					{
						QDomElement it = node.toElement();
						if (it.tagName() == "CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp  = ScCLocale::toDoubleC(it.attribute("RAMP"), 0.0);
							int shade  = it.attribute("SHADE", "100").toInt();
							double opa = ScCLocale::toDoubleC(it.attribute("TRANS"), 1.0);
							handleOldColorShade(m_Doc, name, shade);
							newItem->fill_gradient.addStop(SetColor(m_Doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName() == "ITEXT")
							GetItemText(&it, m_Doc, newItem, last, true, VorLFound);
						if (it.tagName() == "PageItemAttributes")
						{
							QDomNode pia = it.firstChild();
							ObjAttrVector pageItemAttributes;
							while (!pia.isNull())
							{
								QDomElement itemAttr = pia.toElement();
								if (itemAttr.tagName() == "ItemAttribute")
								{
									ObjectAttribute objattr;
									objattr.name = itemAttr.attribute("Name");
									objattr.type = itemAttr.attribute("Type");
									objattr.value = itemAttr.attribute("Value");
									objattr.parameter = itemAttr.attribute("Parameter");
									objattr.relationship = itemAttr.attribute("Relationship");
									objattr.relationshipto = itemAttr.attribute("RelationshipTo");
									objattr.autoaddto = itemAttr.attribute("AutoAddTo");
									pageItemAttributes.append(objattr);
								}
								pia = pia.nextSibling();
							}
							newItem->setObjectAttributes(&pageItemAttributes);
						}
						node = node.nextSibling();
					}
					delete last;
					if (newItem->fill_gradient.stops() == 0)
					{
						const ScColor& col1 = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeFillColor];
						const ScColor& col2 = m_Doc->PageColors[m_Doc->itemToolPrefs().shapeLineColor];
						newItem->fill_gradient.addStop(ScColorEngine::getRGBColor(col1, m_Doc), 0.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeFillColor, 100);
						newItem->fill_gradient.addStop(ScColorEngine::getRGBColor(col2, m_Doc), 1.0, 0.5, 1.0, m_Doc->itemToolPrefs().shapeLineColor, 100);
					}
//					newItem->Language = ScMW->GetLang(pg.attribute("LANGUAGE", m_Doc->Language));
//					newItem->Language = m_Doc->Language;
					newItem->isAutoText = static_cast<bool>(pg.attribute("AUTOTEXT").toInt());
					newItem->isEmbedded = static_cast<bool>(pg.attribute("isInline", "0").toInt());
					newItem->gXpos = ScCLocale::toDoubleC(pg.attribute("gXpos"), 0.0);
					newItem->gYpos = ScCLocale::toDoubleC(pg.attribute("gYpos"), 0.0);
					newItem->gWidth  = ScCLocale::toDoubleC(pg.attribute("gWidth"), newItem->width());
					newItem->gHeight = ScCLocale::toDoubleC(pg.attribute("gHeight"),newItem->height());
					if (newItem->isAutoText)
						m_Doc->LastAuto = newItem;
					if (newItem->isTableItem)
					{
						TableItems.append(newItem);
						TableID.insert(pg.attribute("OwnLINK", "0").toInt(), m_Doc->Items->indexOf(newItem));
					}
					if (pg.tagName() == "FRAMEOBJECT")
					{
						FrameItems.append(m_Doc->Items->takeAt(m_Doc->Items->indexOf(newItem)));
						newItem->setLayer(m_Doc->firstLayerID());
					}
				}
				counter++;
			}
			pageNode = pageNode.nextSibling();
		}
		pageNode = docNode.firstChild();
		while (!pageNode.isNull())
		{
			QDomElement pg = pageNode.toElement();
			if (pg.tagName() == "Bookmark")
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
			pageNode = pageNode.nextSibling();
		}
		docNode = docNode.nextSibling();
	}
	if (TableItems.count() != 0)
	{
		for (int ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->m_topLink = m_Doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->m_topLink = nullptr;
			if (ta->LeftLinkID != -1)
				ta->m_leftLink = m_Doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->m_leftLink = nullptr;
			if (ta->RightLinkID != -1)
				ta->m_rightLink = m_Doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->m_rightLink = nullptr;
			if (ta->BottomLinkID != -1)
				ta->m_bottomLink = m_Doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->m_bottomLink = nullptr;
		}
	}
	// reestablish textframe links
	if (itemNext.count() != 0)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
		{
			if (itemRemap[lc.value()] >= 0)
			{
				PageItem *Its(nullptr), *Itn(nullptr);
				if (lc.key() < m_Doc->Items->count())
					Its = m_Doc->Items->at(lc.key());
				if (itemRemap[lc.value()] < m_Doc->Items->count())
					Itn = m_Doc->Items->at(itemRemap[lc.value()]);
				if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
				{
					qDebug("scribus13format: corruption in linked textframes detected");
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

	// fix legacy char formatting
	for (int i = 0; i < m_Doc->DocItems.count(); ++i)
	{
		PageItem* item = m_Doc->DocItems.at(i);
		if (item->prevInChain() == nullptr && item->itemText.length() > 0)
			item->itemText.fixLegacyFormatting();
	}
	for (int i = 0; i < m_Doc->MasterItems.count(); ++i)
	{
		PageItem* item = m_Doc->MasterItems.at(i);
		if (item->prevInChain() == nullptr && item->itemText.length() > 0)
			item->itemText.fixLegacyFormatting();
	}
	for (auto itf = m_Doc->FrameItems.begin(); itf != m_Doc->FrameItems.end(); ++itf)
	{
		PageItem *item = itf.value();
		if (item->prevInChain() == nullptr && item->itemText.length() > 0)
			item->itemText.fixLegacyFormatting();
	}
	for (int i = 0; i < m_Doc->DocItems.count(); ++i)
	{
		QList<PageItem*> allItems;
		PageItem* item = m_Doc->DocItems.at(i);
		if (item->isGroup())
			allItems = item->getAllChildren();
		else
			allItems.append(item);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			PageItem* gItem = allItems[ii];
			if (gItem->isGroup() && gItem->groupItemList[0]->isTableItem)
			{
				if (gItem->isGroupChild())
					convertOldTable(m_Doc, gItem, gItem->groupItemList, nullptr, &(gItem->asGroupFrame()->groupItemList));
				else
					convertOldTable(m_Doc, gItem, gItem->groupItemList, nullptr, &m_Doc->DocItems);
			}
		}
		allItems.clear();
	}
	for (int i = 0; i < m_Doc->MasterItems.count(); ++i)
	{
		QList<PageItem*> allItems;
		PageItem* item = m_Doc->MasterItems.at(i);
		if (item->isGroup())
			allItems = item->getAllChildren();
		else
			allItems.append(item);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			PageItem* gItem = allItems[ii];
			if (gItem->isGroup() && gItem->groupItemList[0]->isTableItem)
			{
				if (gItem->isGroupChild())
					convertOldTable(m_Doc, gItem, gItem->groupItemList, nullptr, &(gItem->asGroupFrame()->groupItemList));
				else
					convertOldTable(m_Doc, gItem, gItem->groupItemList, nullptr, &m_Doc->MasterItems);
			}
		}
		allItems.clear();
	}
	
	return true;
}

void Scribus13Format::GetStyle(QDomElement *pg, ParagraphStyle *vg, StyleSet<ParagraphStyle> *tempParagraphStyles, ScribusDoc* doc, bool fl)
{
	bool found = false;
	readParagraphStyle(*vg, *pg, doc);
	const StyleSet<ParagraphStyle> & docParagraphStyles(tempParagraphStyles ? *tempParagraphStyles : doc->paragraphStyles());
	for (int i = 0; i < docParagraphStyles.count(); ++i)
	{
		const ParagraphStyle& paraStyle = docParagraphStyles[i];
		if (vg->name() == paraStyle.name())
		{
			if (vg->equiv(paraStyle))
			{
				if (fl)
				{
					DoVorl[VorlC] = vg->name();
					++VorlC;
				}
				found = true;
			}
			else
			{
				vg->setName(docParagraphStyles.getUniqueCopyName(paraStyle.name()));
				found = false;
			}
			break;
		}
	}
	if (!found)
	{
		for (int i = 0; i < docParagraphStyles.count(); ++i)
		{
			const ParagraphStyle& paraStyle = docParagraphStyles[i];
			if (vg->equiv(paraStyle))
			{
				vg->setName(paraStyle.name());
				found = true;
				if (fl)
				{
					DoVorl[VorlC] = vg->name();
					++VorlC;
				}
				break;
			}
		}
	}
	if (!found)
	{
		if (tempParagraphStyles)
			tempParagraphStyles->create(*vg);
		else {
			StyleSet<ParagraphStyle> temp;
			temp.create(*vg);
			doc->redefineStyles(temp, false);
		}
		if (fl)
		{
			DoVorl[VorlC] = vg->name();
			++VorlC;
		}
	}
}

bool Scribus13Format::readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles)
{
	ParagraphStyle pstyle;
	QDomDocument docu("scridoc");
	QString f (readSLA(fileName));
	if (f.isEmpty())
		return false;
	if (!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode docNode = elem.firstChild();
	while (!docNode.isNull())
	{
		QDomElement dc = docNode.toElement();
		QDomNode pageNode = docNode.firstChild();
		while (!pageNode.isNull())
		{
			QDomElement pg = pageNode.toElement();
			if (pg.tagName() == "STYLE")
			{
				pstyle.erase();
				GetStyle(&pg, &pstyle, &docParagraphStyles, doc, false);
			}
			pageNode = pageNode.nextSibling();
		}
		docNode = docNode.nextSibling();
	}
	return true;
}

bool Scribus13Format::readLineStyles(const QString& fileName, QHash<QString,multiLine> *Sty)
{
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	if (!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode docNode = elem.firstChild();
	while (!docNode.isNull())
	{
		QDomElement dc = docNode.toElement();
		QDomNode pageNode = docNode.firstChild();
		while (!pageNode.isNull())
		{
			QDomElement pg = pageNode.toElement();
			if (pg.tagName() == "MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = pageNode.firstChild();
				while (!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct SingleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = MuL.attribute("Dash").toInt();
					sl.LineEnd = MuL.attribute("LineEnd").toInt();
					sl.LineJoin = MuL.attribute("LineJoin").toInt();
					sl.Shade = MuL.attribute("Shade").toInt();
					sl.Width = ScCLocale::toDoubleC(MuL.attribute("Width"));
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				QString Nam = pg.attribute("Name");
				QString Nam2 = Nam;
				int copyC = 1;
				QHash<QString,multiLine>::ConstIterator mlit = Sty->find(Nam2);
				if (mlit != Sty->constEnd() && ml != mlit.value())
				{
					while (Sty->contains(Nam2))
					{
						Nam2 = tr("Copy #%1 of ").arg(copyC)+Nam;
						copyC++;
					}
				}
				Sty->insert(Nam2, ml);
			}
			pageNode = pageNode.nextSibling();
		}
		docNode = docNode.nextSibling();
	}
	return true;
}

bool Scribus13Format::readColors(const QString& fileName, ColorList & colors)
{
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	QDomDocument docu("scridoc");
	if (!docu.setContent(f))
		return false;
	ScColor lf = ScColor();
	colors.clear();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode docNode = elem.firstChild();
	while (!docNode.isNull())
	{
		QDomElement dc = docNode.toElement();
		QDomNode pageNode = docNode.firstChild();
		while (!pageNode.isNull())
		{
			QDomElement pg = pageNode.toElement();
			// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
			if (pg.tagName() == "COLOR" && pg.attribute("NAME") != CommonStrings::None)
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
			pageNode = pageNode.nextSibling();
		}
		docNode = docNode.nextSibling();
	}
	return true;
}

bool Scribus13Format::readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames)
{
	QString pageName;
	int counter = 0;
	int counter2 = 0;
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	if (!docu.setContent(f))
		return false;
	QDomElement elem = docu.documentElement();
	if (elem.tagName() != "SCRIBUSUTF8NEW")
		return false;
	QDomNode docNode = elem.firstChild();
	while (!docNode.isNull())
	{
		QDomNode pageNode = docNode.firstChild();
		while (!pageNode.isNull())
		{
			QDomElement pg = pageNode.toElement();
			pageName = pg.attribute("NAM", "");
			if (pg.tagName() == "PAGE")
				counter++;
			if (pg.tagName() == "MASTERPAGE")
			{
				counter2++;
				masterPageNames.append(pageName);
			}
			pageNode = pageNode.nextSibling();
		}
		docNode = docNode.nextSibling();
	}
	*num1 = counter;
	*num2 = counter2;
	return true;
}
