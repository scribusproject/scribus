/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus12format.h"
#include "scribus12format.moc"
#include "scribus12formatimpl.h"

#include "../../formatidlist.h"
#include "commonstrings.h"
#include "missing.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "scribusdoc.h"
#include "scribusview.h"

#include "units.h"
#include "util.h"
#ifdef HAVE_LIBZ
	#include <zlib.h>
#endif
#include <qcursor.h>
#include <qfileinfo.h>
#include <qvaluelist.h>


// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in scribus12formatimpl.h and scribus12formatimpl.cpp .

Scribus12Format::Scribus12Format() :
	LoadSavePlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file formats.
	languageChange();
}

Scribus12Format::~Scribus12Format()
{
	unregisterAll();
};

void Scribus12Format::languageChange()
{
	//(Re)register file formats.
	unregisterAll();
	registerFormats();
}

const QString Scribus12Format::fullTrName() const
{
	return QObject::tr("Scribus 1.2.x Support");
}

const ScActionPlugin::AboutData* Scribus12Format::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	return about;
}

void Scribus12Format::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

// Low level plugin API
int scribus12format_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scribus12format_getPlugin()
{
	Scribus12Format* plug = new Scribus12Format();
	Q_CHECK_PTR(plug);
	return plug;
}

void scribus12format_freePlugin(ScPlugin* plugin)
{
	Scribus12Format* plug = dynamic_cast<Scribus12Format*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}



void Scribus12Format::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Scribus 1.2.x Document");
	fmt.formatId = FORMATID_SLA12XIMPORT;
	fmt.load = true;
	fmt.save = false;
#ifdef HAVE_LIBZ
	fmt.filter = fmt.trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
	fmt.nameMatch = QRegExp("\\.(sla|scd)(\\.gz)?", false);
#else
	fmt.filter = fmt.trName + " (*.sla *.SLA *.scd *.SCD)";
	fmt.nameMatch = QRegExp("\\.(sla|scd)", false);
#endif
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-scribus");
	fmt.priority = 64;
	registerFormat(fmt);
}

bool Scribus12Format::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	QCString docBytes("");
	if(fileName.right(2) == "gz")
	{
#ifdef HAVE_LIBZ
		static const int gzipExpansionFactor=8;
		// The file is gzip encoded and we can load gzip files.
		// Set up to read the gzip file
		gzFile gzDoc;
		int i;
		gzDoc = gzopen(fileName.latin1(),"rb");
		if(gzDoc == NULL)
		{
			// FIXME: Needs better error return
			return "";
		}
		// Allocate a buffer of a multiple of the compressed size of the file
		// as a starting point for loading. We'll expand this buffer by powers
		// of two if we run out of space.
		const QFileInfo fi(fileName);
		uint bufSize = QMIN(4096, fi.size()*gzipExpansionFactor);
		docBytes = QCString(bufSize);
		char* buf = docBytes.data();
		uint bytesRead = 0;
		// While there's free space, read into the buffer....
		while (bytesRead<4096 && (i = gzread(gzDoc,buf,bufSize-bytesRead-1)) > 0)
		{
			// Ensure the string is null-terminated and move the
			// write pointer to the current position.
			buf[i]=0;
			buf+=i;
			bytesRead += i;
			// And check that there's free space to work with, expanding the
			// buffer if there's not.
			if (bufSize - bytesRead < 4096)
			{
				bufSize *= 2;
				docBytes.resize(bufSize);
				buf = docBytes.data() + bytesRead;
			}
		}
		gzclose(gzDoc);
#else
		// The file is gzip encoded but we can't load gzip files.
		// Leave `f' empty, since we have no way to
		// report a failure condition from here.
		return false;
#endif
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
	if (docBytes.left(16) != "<SCRIBUSUTF8NEW " && (docBytes.left(12) == "<SCRIBUSUTF8" || docBytes.left(9) == "<SCRIBUS>"))
		return true;
	return false;
}

QString Scribus12Format::readSLA(const QString & fileName)
{
	QCString docBytes("");
	if(fileName.right(2) == "gz")
	{
#ifdef HAVE_LIBZ
		static const int gzipExpansionFactor=8;
		// The file is gzip encoded and we can load gzip files.
		// Set up to read the gzip file
		gzFile gzDoc;
		int i;
		gzDoc = gzopen(fileName.latin1(),"rb");
		if(gzDoc == NULL)
		{
			// FIXME: Needs better error return
			return "";
		}
		// Allocate a buffer of a multiple of the compressed size of the file
		// as a starting point for loading. We'll expand this buffer by powers
		// of two if we run out of space.
		const QFileInfo fi(fileName);
		uint bufSize = fi.size()*gzipExpansionFactor;
		docBytes = QCString(bufSize);
		char* buf = docBytes.data();
		uint bytesRead = 0;
		// While there's free space, read into the buffer....
		while ((i = gzread(gzDoc,buf,bufSize-bytesRead-1)) > 0)
		{
			// Ensure the string is null-terminated and move the
			// write pointer to the current position.
			buf[i]=0;
			buf+=i;
			bytesRead += i;
			// And check that there's free space to work with, expanding the
			// buffer if there's not.
			if (bufSize - bytesRead < 4096)
			{
				bufSize *= 2;
				docBytes.resize(bufSize);
				buf = docBytes.data() + bytesRead;
			}
		}
		gzclose(gzDoc);
#else
		// The file is gzip encoded but we can't load gzip files.
		// Leave `f' empty, since we have no way to
		// report a failure condition from here.
		return false;
#endif
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
	QString docText("");
	if (docBytes.left(16) != "<SCRIBUSUTF8NEW " && (docBytes.left(12) == "<SCRIBUSUTF8" || docBytes.left(9) == "<SCRIBUS>"))
		docText = QString::fromLocal8Bit(docBytes);
	else
		return QString::null;
	if (docText.endsWith(QChar(10)) || docText.endsWith(QChar(13)))
		docText.truncate(docText.length()-1);
	return docText;
}


void Scribus12Format::getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QPtrList<Foi> &getDummyFois)
{
	getNewReplacement=newReplacement;
	getReplacedFonts=ReplacedFonts;
	getDummyFois=dummyFois;
}

bool Scribus12Format::loadFile(const QString & fileName, const FileFormat & /* fmt */, int /* flags */, int /* index */)
{
	if (m_Doc==0 || m_View==0 || m_AvailableFonts==0)
	{
		Q_ASSERT(m_Doc==0 || m_View==0 || m_AvailableFonts==0);
		return false;
	}
	ReplacedFonts.clear();
	newReplacement = false;
	dummyFois.clear();
	
//start old ReadDoc
	//Scribus 1.2 docs, see fileloader.cpp for 1.3 docs
	struct CopyPasteBuffer OB;
	ParagraphStyle vg;
	struct Layer la;
	struct ScribusDoc::BookMa bok;
	int counter;
	bool newVersion = false;
	struct Linked Link;
	QString tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	int x, a;
	double xf, xf2;
	PageItem *Neu;
	LFrames.clear();
	QDomDocument docu("scridoc");
	QFile fi(fileName);
	// Load the document text
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	// Build the DOM from it
	if (!docu.setContent(f))
		return false;
	// and begin loading the doc
	m_Doc->PageColors.clear();
	m_Doc->Layers.clear();
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8"))
		return false;
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	if (m_mwProgressBar!=0)
	{
		m_mwProgressBar->setTotalSteps(DOC.childNodes().count());
		m_mwProgressBar->setProgress(0);
	}
	int ObCount = 0;
	int activeLayer = 0;
	PrefsManager* prefsManager=PrefsManager::instance();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
	/*
	* Attribute von DOCUMENT auslesen
	*/
		if (dc.hasAttribute("PAGEWIDTH"))
			m_Doc->pageWidth=dc.attribute("PAGEWIDTH").toDouble();
		else
			m_Doc->pageWidth=dc.attribute("PAGEWITH").toDouble();
		m_Doc->pageHeight=dc.attribute("PAGEHEIGHT").toDouble();
		m_Doc->pageMargins.Left=QMAX(0.0, dc.attribute("BORDERLEFT").toDouble());
		m_Doc->pageMargins.Right=QMAX(0.0, dc.attribute("BORDERRIGHT").toDouble());
		m_Doc->pageMargins.Top=QMAX(0.0, dc.attribute("BORDERTOP").toDouble());
		m_Doc->pageMargins.Bottom=QMAX(0.0, dc.attribute("BORDERBOTTOM").toDouble());
		m_Doc->PageOri = dc.attribute("ORIENTATION", "0").toInt();
		m_Doc->PageSize = dc.attribute("PAGESIZE");
		m_Doc->FirstPnum = dc.attribute("FIRSTNUM", "1").toInt();
		m_Doc->currentPageLayout=dc.attribute("BOOK", "0").toInt();
		int fp;
		if (dc.attribute("FIRSTLEFT", "0").toInt() == 1)
			fp = 0;
		else
			fp = 1;
		if (m_Doc->currentPageLayout == 0)
			fp = 0;
		m_Doc->pageSets[m_Doc->currentPageLayout].FirstPage = fp;
		m_Doc->setUsesAutomaticTextFrames(dc.attribute("AUTOTEXT").toInt());
		m_Doc->PageSp=dc.attribute("AUTOSPALTEN").toInt();
		m_Doc->PageSpa=dc.attribute("ABSTSPALTEN").toDouble();
		m_Doc->setUnitIndex(dc.attribute("UNITS", "0").toInt());
		m_Doc->guidesSettings.gridShown = prefsManager->appPrefs.guidesSettings.gridShown;
		m_Doc->guidesSettings.guidesShown = prefsManager->appPrefs.guidesSettings.guidesShown;
		m_Doc->guidesSettings.colBordersShown = prefsManager->appPrefs.guidesSettings.colBordersShown;
		m_Doc->guidesSettings.framesShown = prefsManager->appPrefs.guidesSettings.framesShown;
		m_Doc->guidesSettings.layerMarkersShown = prefsManager->appPrefs.guidesSettings.layerMarkersShown;
		m_Doc->guidesSettings.marginsShown = prefsManager->appPrefs.guidesSettings.marginsShown;
		m_Doc->guidesSettings.baseShown = prefsManager->appPrefs.guidesSettings.baseShown;
		m_Doc->guidesSettings.linkShown = prefsManager->appPrefs.guidesSettings.linkShown;
		m_Doc->guidesSettings.showPic = true;
		m_Doc->guidesSettings.showControls = false;
		DoFonts.clear();
		m_Doc->toolSettings.defSize=qRound(dc.attribute("DSIZE").toDouble() * 10);
		Defont=dc.attribute("DFONT");
		if ((!m_AvailableFonts->find(Defont)) || (!(*m_AvailableFonts)[Defont]->usable()))
		{
			ReplacedFonts.insert(Defont, prefsManager->appPrefs.toolSettings.defFont);
			Defont = prefsManager->appPrefs.toolSettings.defFont;
		}
		else
		{
			if (!m_Doc->UsedFonts.contains(tmpf))
			{
//				QFont fo = avail[Defont]->Font;
//				fo.setPointSize(qRound(m_Doc->toolSettings.defSize / 10.0));
				m_Doc->AddFont(Defont);
			}
		}
		m_Doc->toolSettings.defFont = Defont;
		m_Doc->toolSettings.dCols=dc.attribute("DCOL", "1").toInt();
		m_Doc->toolSettings.dGap=dc.attribute("DGAP", "0.0").toDouble();
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
		m_Doc->typographicSettings.valueBaseGrid = dc.attribute("BASEGRID", "12").toDouble();
		m_Doc->typographicSettings.offsetBaseGrid = dc.attribute("BASEO", "0").toDouble();
		m_Doc->typographicSettings.autoLineSpacing = dc.attribute("AUTOL", "20").toInt();
		m_Doc->GroupCounter=dc.attribute("GROUPC", "1").toInt();
		m_Doc->HasCMS = static_cast<bool>(dc.attribute("HCMS", "0").toInt());
		m_Doc->CMSSettings.SoftProofOn = static_cast<bool>(dc.attribute("DPSo", "0").toInt());
		m_Doc->CMSSettings.SoftProofFullOn = static_cast<bool>(dc.attribute("DPSFo", "0").toInt());
		m_Doc->CMSSettings.CMSinUse = static_cast<bool>(dc.attribute("DPuse", "0").toInt());
		m_Doc->CMSSettings.GamutCheck = static_cast<bool>(dc.attribute("DPgam", "0").toInt());
		m_Doc->CMSSettings.BlackPoint = static_cast<bool>(dc.attribute("DPbla", "1").toInt());
		m_Doc->CMSSettings.DefaultMonitorProfile = dc.attribute("DPMo","");
		m_Doc->CMSSettings.DefaultPrinterProfile = dc.attribute("DPPr","");
		m_Doc->CMSSettings.DefaultImageRGBProfile = dc.attribute("DPIn","");
		m_Doc->CMSSettings.DefaultSolidColorRGBProfile = dc.attribute("DPIn2","");
		m_Doc->CMSSettings.DefaultSolidColorCMYKProfile = dc.attribute("DPIn3","");
		//m_Doc->CMSSettings.DefaultIntentPrinter = dc.attribute("DIPr", "0").toInt();
		//m_Doc->CMSSettings.DefaultIntentMonitor = dc.attribute("DIMo", "1").toInt();
		m_Doc->CMSSettings.DefaultIntentColors = dc.attribute("DISc", "1").toInt();
		m_Doc->CMSSettings.DefaultIntentImages = dc.attribute("DIIm", "0").toInt();
		activeLayer = dc.attribute("ALAYER", "0").toInt();
		m_Doc->Language = dc.attribute("LANGUAGE", "");
		m_Doc->MinWordLen = dc.attribute("MINWORDLEN", "3").toInt();
		m_Doc->HyCount = dc.attribute("HYCOUNT", "2").toInt();
		m_Doc->Automatic = static_cast<bool>(dc.attribute("AUTOMATIC", "1").toInt());
		m_Doc->AutoCheck = static_cast<bool>(dc.attribute("AUTOCHECK", "0").toInt());
		m_Doc->GuideLock = static_cast<bool>(dc.attribute("GUIDELOCK", "0").toInt());
		m_Doc->SnapGuides = static_cast<bool>(dc.attribute("SnapToGuides", "0").toInt());
		m_Doc->useRaster = static_cast<bool>(dc.attribute("SnapToGrid", "0").toInt());
		m_Doc->guidesSettings.minorGrid = dc.attribute("MINGRID", tmp.setNum(prefsManager->appPrefs.guidesSettings.minorGrid)).toDouble();
		m_Doc->guidesSettings.majorGrid = dc.attribute("MAJGRID", tmp.setNum(prefsManager->appPrefs.guidesSettings.majorGrid)).toDouble();
		m_Doc->toolSettings.dStartArrow = 0;
		m_Doc->toolSettings.dEndArrow = 0;
		m_Doc->LastAuto = 0;
		QDomNode PAGE=DOC.firstChild();
		counter = 0;
		while(!PAGE.isNull())
		{
			ObCount++;
			if (m_mwProgressBar!=0)
				m_mwProgressBar->setProgress(ObCount);
			QDomElement pg=PAGE.toElement();
			// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
				lf.setSpotColor(false);
				lf.setRegistrationColor(false);
				m_Doc->PageColors[pg.attribute("NAME")] = lf;
			}
			if(pg.tagName()=="STYLE")
			{
				vg.setName(pg.attribute("NAME"));
				vg.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(pg.attribute("LINESPMode", "0").toInt()));
				vg.setLineSpacing(pg.attribute("LINESP").toDouble());
				vg.setLeftMargin(pg.attribute("INDENT", "0").toDouble());
				vg.setFirstIndent(pg.attribute("FIRST", "0").toDouble());
				vg.setAlignment(pg.attribute("ALIGN").toInt());
				vg.setGapBefore(pg.attribute("VOR", "0").toDouble());
				vg.setGapAfter(pg.attribute("NACH", "0").toDouble());
				tmpf = pg.attribute("FONT", m_Doc->toolSettings.defFont);
				if ((!m_AvailableFonts->find(tmpf)) || (!(*m_AvailableFonts)[tmpf]->usable()))
				{
					if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!(*m_AvailableFonts)[prefsManager->appPrefs.GFontSub[tmpf]]->usable()))
					{
						newReplacement = true;
						ReplacedFonts.insert(tmpf, prefsManager->appPrefs.toolSettings.defFont);
					}
					else
						ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
				}
				else
				{
					if (!m_Doc->UsedFonts.contains(tmpf))
					{
//						QFont fo = avail[tmpf]->Font;
//						fo.setPointSize(qRound(m_Doc->toolSettings.defSize / 10.0));
						m_Doc->AddFont(tmpf);
					}
				}
				vg.charStyle().setFont((*m_AvailableFonts)[tmpf]);
				vg.charStyle().setFontSize(qRound(pg.attribute("FONTSIZE", "12").toDouble() * 10.0));
				vg.setHasDropCap(static_cast<bool>(pg.attribute("DROP", "0").toInt()));
				vg.setDropCapLines(pg.attribute("DROPLIN", "2").toInt());
				vg.setDropCapOffset(pg.attribute("DROPDIST", "0").toDouble());
				vg.charStyle().setEffects(static_cast<StyleFlag>(pg.attribute("EFFECT", "0").toInt()));
				vg.charStyle().setFillColor(pg.attribute("FCOLOR", m_Doc->toolSettings.dBrush));
				vg.charStyle().setFillShade(pg.attribute("FSHADE", "100").toInt());
				vg.charStyle().setStrokeColor(pg.attribute("SCOLOR", m_Doc->toolSettings.dPen));
				vg.charStyle().setStrokeShade(pg.attribute("SSHADE", "100").toInt());
				vg.setUseBaselineGrid(static_cast<bool>(pg.attribute("BASE", "0").toInt()));
				vg.charStyle().setShadowXOffset(50);
				vg.charStyle().setShadowYOffset(-50);
				vg.charStyle().setOutlineWidth(10);
				vg.charStyle().setUnderlineOffset(m_Doc->typographicSettings.valueUnderlinePos);
				vg.charStyle().setUnderlineWidth(m_Doc->typographicSettings.valueUnderlineWidth);
				vg.charStyle().setStrikethruOffset(m_Doc->typographicSettings.valueStrikeThruPos);
				vg.charStyle().setStrikethruWidth(m_Doc->typographicSettings.valueStrikeThruPos);
				vg.charStyle().setScaleH(1000);
				vg.charStyle().setScaleV(1000);
				vg.charStyle().setBaselineOffset(0);
				vg.charStyle().setTracking(0);
				if ((pg.hasAttribute("NUMTAB")) && (pg.attribute("NUMTAB", "0").toInt() != 0))
				{
					tmp = pg.attribute("TABS");
					QTextStream tgv(&tmp, IO_ReadOnly);
					vg.tabValues().clear();
					ParagraphStyle::TabRecord tb;
					for (int cxv = 0; cxv < pg.attribute("NUMTAB", "0").toInt(); cxv += 2)
					{
						tgv >> xf;
						tgv >> xf2;
						tb.tabPosition = xf2;
						tb.tabType = static_cast<int>(xf);
						tb.tabFillChar = QChar();
						vg.tabValues().append(tb);
					}
					tmp = "";
				}
				else
					vg.tabValues().clear();
				m_Doc->docParagraphStyles.append(vg);
			}
			if(pg.tagName()=="JAVA")
				m_Doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = pg.attribute("NUMMER").toInt();
				la.Level = pg.attribute("LEVEL").toInt();
				la.Name = pg.attribute("NAME");
				la.isViewable = pg.attribute("SICHTBAR").toInt();
				la.isPrintable = pg.attribute("DRUCKEN").toInt();
				la.isEditable = true;
				la.flowControl = true;
				la.blendMode = 0;
				la.transparency = 1.0;
				QColor marker;
				switch (la.LNr % 7)
				{
					case 0:
						marker = Qt::black;
						break;
					case 1:
						marker = Qt::red;
						break;
					case 2:
						marker = Qt::green;
						break;
					case 3:
						marker = Qt::blue;
						break;
					case 4:
						marker = Qt::cyan;
						break;
					case 5:
						marker = Qt::magenta;
						break;
					case 6:
						marker = Qt::yellow;;
						break;
				}
				la.markerColor = marker;
				la.outlineMode = false;
				m_Doc->Layers.append(la);
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
					sl.Width = MuL.attribute("Width").toDouble();
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				m_Doc->MLineStyles.insert(pg.attribute("Name"), ml);
			}
			if(pg.tagName()=="PAGE")
			{
				TableItems.clear();
				TableID.clear();
			/*
			* Attribute von PAGE auslesen
			*/
				a = pg.attribute("NUM").toInt();
				PgNam = "";
				PgNam = pg.attribute("NAM", "");
				QString Mus = "";
				Mus = pg.attribute("MNAM","Normal");
				if (PgNam.isEmpty())
				{
					//We store the pages master page but do not apply it now
					//as it may not exist yet. They are applied in scribus.cpp for now.
					m_Doc->setMasterPageMode(false);
					m_Doc->setCurrentPage(m_Doc->addPage(a));
					m_Doc->currentPage()->MPageNam=Mus;
				}
				else
				{
					m_Doc->setMasterPageMode(true);
					m_Doc->setCurrentPage(m_Doc->addMasterPage(a, PgNam));
				}
				//CB: Remove this unnecessarily "slow" slot call when we have no gui for the doc yet!
				//Items dont appear in the right place if we just m_Doc->addPage(a); for <=1.2.x docs
				//so we have to call the view, but we certainly dont need to emit to the mainwindow!
				//This call now picks up the added page and does some view black magic. A must for
				//1.2.x docs!
				m_View->addPage(a);
				//emit NewPage(a);
				m_Doc->Pages->at(a)->LeftPg=pg.attribute("LEFT", "0").toInt();

				// guides reading
				tmp = "";
				GuideManagerCore::readVerticalGuides(pg.attribute("VerticalGuides"),
							m_Doc->Pages->at(a),
							GuideManagerCore::Standard,
							pg.hasAttribute("NumVGuides"));
				GuideManagerCore::readHorizontalGuides(pg.attribute("HorizontalGuides"),
							m_Doc->Pages->at(a),
							GuideManagerCore::Standard,
							pg.hasAttribute("NumHGuides"));

				QDomNode OBJ=PAGE.firstChild();
				while(!OBJ.isNull())
				{
					QDomElement obj=OBJ.toElement();
				/*
				* Attribute von OBJECT auslesen
				*/
					if ((obj.attribute("NEXTITEM").toInt() != -1) || (static_cast<bool>(obj.attribute("AUTOTEXT").toInt())))
					{
						if (obj.attribute("BACKITEM").toInt() == -1)
						{
							Link.Start = counter;
							Link.StPag = a;
							LFrames.append(Link);
						}
					}
					GetItemProps(newVersion, &obj, &OB);
					OB.Xpos = obj.attribute("XPOS").toDouble()+m_Doc->Pages->at(a)->xOffset();
					OB.Ypos=obj.attribute("YPOS").toDouble()+m_Doc->Pages->at(a)->yOffset();
					OB.NamedLStyle = obj.attribute("NAMEDLST", "");
					OB.isBookmark=obj.attribute("BOOKMARK").toInt();
					if ((OB.isBookmark) && (m_Doc->BookMarks.count() == 0))
						m_Doc->OldBM = true;
					OB.textAlignment = obj.attribute("ALIGN", "0").toInt();
					OB.startArrowIndex =  0;
					OB.endArrowIndex =  0;
					tmpf = obj.attribute("IFONT", m_Doc->toolSettings.defFont);
					if ((!m_AvailableFonts->find(tmpf)) || (!(*m_AvailableFonts)[tmpf]->usable()))
					{
						if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!(*m_AvailableFonts)[prefsManager->appPrefs.GFontSub[tmpf]]->usable()))
						{
							newReplacement = true;
							ReplacedFonts.insert(tmpf, prefsManager->appPrefs.toolSettings.defFont);
						}
						else
							ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
					}
					else
					{
						if (!m_Doc->UsedFonts.contains(tmpf))
						{
//							QFont fo = avail[tmpf]->Font;
//							fo.setPointSize(qRound(m_Doc->toolSettings.defSize / 10.0));
							m_Doc->AddFont(tmpf);
						}
					}
					OB.IFont = tmpf;
					OB.LayerNr = obj.attribute("LAYER", "0").toInt();
					OB.Language = obj.attribute("LANGUAGE", m_Doc->Language);
					tmp = "";
					if ((obj.hasAttribute("GROUPS")) && (obj.attribute("NUMGROUP", "0").toInt() != 0))
					{
						tmp = obj.attribute("GROUPS");
						QTextStream fg(&tmp, IO_ReadOnly);
						OB.Groups.clear();
						for (int cx = 0; cx < obj.attribute("NUMGROUP", "0").toInt(); ++cx)
						{
							fg >> x;
							OB.Groups.push(x);
						}
						tmp = "";
					}
					else
						OB.Groups.clear();
					QDomNode IT=OBJ.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = it.attribute("RAMP", "0.0").toDouble();
							int shade = it.attribute("SHADE", "100").toInt();
							double opa = it.attribute("TRANS", "1").toDouble();
							OB.fill_gradient.addStop(SetColor(m_Doc, name, shade), ramp, 0.5, opa, name, shade);
							OB.GrColor = "";
							OB.GrColor2 = "";
						}
						IT=IT.nextSibling();
					}
					OB.itemText = "";
					int docGc = m_Doc->GroupCounter;
					m_Doc->GroupCounter = 0;
					uint last = m_Doc->Items->count();
					m_View->PasteItem(&OB, true);
					m_Doc->GroupCounter = docGc;
					Neu = m_Doc->Items->at(last);
					Neu->OnMasterPage = PgNam;
					Neu->OwnPage = a; //No need to scan for OnPage as we know page by page in 1.2.x
					Neu->oldOwnPage = 0;
					Neu->setRedrawBounding();
					IT=OBJ.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="ITEXT")
							GetItemText(&it, m_Doc, false, false, Neu);
						IT=IT.nextSibling();
					}
					Neu->isAutoText=static_cast<bool>(obj.attribute("AUTOTEXT").toInt());
					if (Neu->isAutoText)
						m_Doc->LastAuto = Neu;
					Neu->NextIt = obj.attribute("NEXTITEM").toInt();
					Neu->NextPg = obj.attribute("NEXTPAGE").toInt();
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(obj.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
					}
					counter++;
					OBJ=OBJ.nextSibling();
				}
				if (TableItems.count() != 0)
				{
					for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
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
			}
			PAGE=PAGE.nextSibling();
		}
		PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="Bookmark")
			{
				bok.Title = pg.attribute("Title");
				bok.Text = pg.attribute("Text");
				bok.Aktion = pg.attribute("Aktion");
				bok.ItemNr = pg.attribute("ItemNr").toInt();
				bok.PageObject = m_Doc->Items->at(pg.attribute("Element").toInt());
				bok.First = pg.attribute("First").toInt();
				bok.Last = pg.attribute("Last").toInt();
				bok.Prev = pg.attribute("Prev").toInt();
				bok.Next = pg.attribute("Next").toInt();
				bok.Parent = pg.attribute("Parent").toInt();
				m_Doc->BookMarks.append(bok);
			}
			if(pg.tagName()=="PDF")
			{
				m_Doc->PDF_Options.Articles = static_cast<bool>(pg.attribute("Articles").toInt());
				m_Doc->PDF_Options.Thumbnails = static_cast<bool>(pg.attribute("Thumbnails").toInt());
				m_Doc->PDF_Options.Compress = static_cast<bool>(pg.attribute("Compress").toInt());
				m_Doc->PDF_Options.CompressMethod = pg.attribute("CMethod", "0").toInt();
				m_Doc->PDF_Options.Quality = pg.attribute("Quality", "0").toInt();
				m_Doc->PDF_Options.RecalcPic = static_cast<bool>(pg.attribute("RecalcPic").toInt());
				m_Doc->PDF_Options.Bookmarks = static_cast<bool>(pg.attribute("Bookmarks").toInt());
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
				m_Doc->PDF_Options.PresentMode = static_cast<bool>(pg.attribute("PresentMode").toInt());
				m_Doc->PDF_Options.PicRes = pg.attribute("PicRes").toInt();
				m_Doc->PDF_Options.Version = (PDFOptions::PDFVersion)pg.attribute("Version").toInt();
				m_Doc->PDF_Options.Resolution = pg.attribute("Resolution").toInt();
				m_Doc->PDF_Options.Binding = pg.attribute("Binding").toInt();
				m_Doc->PDF_Options.Datei = "";
				m_Doc->PDF_Options.isGrayscale = false;
				m_Doc->PDF_Options.UseRGB = static_cast<bool>(pg.attribute("RGBMode", "0").toInt());
				m_Doc->PDF_Options.UseProfiles = static_cast<bool>(pg.attribute("UseProfiles", "0").toInt());
				m_Doc->PDF_Options.UseProfiles2 = static_cast<bool>(pg.attribute("UseProfiles2", "0").toInt());
				m_Doc->PDF_Options.Intent = pg.attribute("Intent", "1").toInt();
				m_Doc->PDF_Options.Intent2 = pg.attribute("Intent2", "1").toInt();
				m_Doc->PDF_Options.SolidProf = pg.attribute("SolidP", "");
				m_Doc->PDF_Options.ImageProf = pg.attribute("ImageP", "");
				m_Doc->PDF_Options.PrintProf = pg.attribute("PrintP", "");
				m_Doc->PDF_Options.Info = pg.attribute("InfoString", "");
				m_Doc->PDF_Options.BleedTop = pg.attribute("BTop", "0").toDouble();
				m_Doc->PDF_Options.BleedLeft = pg.attribute("BLeft", "0").toDouble();
				m_Doc->PDF_Options.BleedRight = pg.attribute("BRight", "0").toDouble();
				m_Doc->PDF_Options.BleedBottom = pg.attribute("BBottom", "0").toDouble();
				m_Doc->PDF_Options.EmbeddedI = static_cast<bool>(pg.attribute("ImagePr", "0").toInt());
				m_Doc->PDF_Options.PassOwner = pg.attribute("PassOwner", "");
				m_Doc->PDF_Options.PassUser = pg.attribute("PassUser", "");
				m_Doc->PDF_Options.Permissions = pg.attribute("Permissions", "-4").toInt();
				m_Doc->PDF_Options.Encrypt = static_cast<bool>(pg.attribute("Encrypt", "0").toInt());
				m_Doc->PDF_Options.useLayers = static_cast<bool>(pg.attribute("UseLayers", "0").toInt());
				m_Doc->PDF_Options.UseLPI = static_cast<bool>(pg.attribute("UseLpi", "0").toInt());
				m_Doc->PDF_Options.UseSpotColors = true;
				m_Doc->PDF_Options.doOverprint = false;
				m_Doc->PDF_Options.doMultiFile = false;
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
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	//m_Doc->Pages = &m_Doc->DocPages;
	//m_Doc->pageCount = m_Doc->Pages->count();
	//m_Doc->Items = m_Doc->DocItems;
	m_Doc->setMasterPageMode(false);
	m_View->reformPages();
	if (m_Doc->layerCount() == 0)
	{
		la.LNr = 0;
		la.Level = 0;
		la.Name = tr("Background");
		la.isViewable = true;
		la.isPrintable = true;
		m_Doc->Layers.append(la);
	}
	m_Doc->setActiveLayer(activeLayer);
	if (LFrames.count() != 0)
	{
		PageItem *Its;
		PageItem *Itn;
		PageItem *Itr;
		QValueList<Linked>::Iterator lc;
		for (lc = LFrames.begin(); lc != LFrames.end(); ++lc)
		{
			Its = m_Doc->Items->at((*lc).Start);
			Itr = Its;
			Its->BackBox = 0;
			if (Its->isAutoText)
				m_Doc->FirstAuto = Its;
			while (Its->NextIt != -1)
			{
				int itnr = 0;
				for (uint nn = 0; nn < m_Doc->Items->count(); ++nn)
				{
					if (m_Doc->Items->at(nn)->OwnPage == Its->NextPg)
					{
						if (itnr == Its->NextIt)
						{
							itnr = nn;
							break;
						}
						itnr++;
					}
				}
				Itn = m_Doc->Items->at(itnr);
				Its->NextBox = Itn;
				Itn->BackBox = Its;
				Its = Itn;
			}
			Its->NextBox = 0;
		}
	}
	m_View->unitSwitcher->setCurrentText(unitGetStrFromIndex(m_Doc->unitIndex()));
	if (m_mwProgressBar!=0)
		m_mwProgressBar->setProgress(DOC.childNodes().count());
	return true;
//end old ReadDoc
	
	
	//return false;
}

bool Scribus12Format::saveFile(const QString & /* fileName */, const FileFormat & /* fmt */)
{
	return false;
}

void Scribus12Format::GetItemText(QDomElement *it, ScribusDoc *doc, bool VorLFound, bool impo, PageItem* obj)
{
	QString tmp2, tmf, tmpf, tmp3;
	tmp2 = it->attribute("CH");
	tmp2.replace(QRegExp("\r"), QChar(5));
	tmp2.replace(QRegExp("\n"), QChar(5));
	tmp2.replace(QRegExp("\t"), QChar(4));
	tmpf = it->attribute("CFONT", doc->toolSettings.defFont);
	bool unknown = false;
	ScText *hg;
	Foi* dummy = NULL;
	PrefsManager* prefsManager=PrefsManager::instance();
	if ((!prefsManager->appPrefs.AvailFonts.find(tmpf)) || (!prefsManager->appPrefs.AvailFonts[tmpf]->usable()))
	{
		bool isThere = false;
		for (uint dl = 0; dl < dummyFois.count(); ++dl)
		{
			if (dummyFois.at(dl)->scName() == tmpf)
			{
				isThere = true;
				dummy = dummyFois.at(dl);
				break;
			}
		}
		if (!isThere)
		{
			dummy = new Foi(tmpf, "", tmpf, "", "", 1, false);
			dummyFois.append(dummy);
		}
		unknown = true;
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.GFontSub[tmpf]]->usable()))
		{
			newReplacement = true;
			ReplacedFonts.insert(tmpf, prefsManager->appPrefs.toolSettings.defFont);
		}
		else
			ReplacedFonts.insert(tmpf, prefsManager->appPrefs.GFontSub[tmpf]);
	}
	else
	{
		if (!doc->UsedFonts.contains(tmpf))
		{
//			QFont fo = prefsManager->appPrefs.AvailFonts[tmpf]->Font;
//			fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
			doc->AddFont(tmpf);
		}
	}
	int size = qRound(it->attribute("CSIZE").toDouble() * 10);
	QString fcolor = it->attribute("CCOLOR");
	int extra;
	if (it->hasAttribute("CEXTRA"))
		extra = qRound(it->attribute("CEXTRA").toDouble() / it->attribute("CSIZE").toDouble() * 1000.0);
	else
		extra = it->attribute("CKERN").toInt();
	int shade = it->attribute("CSHADE").toInt();
	int cstyle = it->attribute("CSTYLE").toInt() & 255;
	int ab = it->attribute("CAB", "0").toInt();
	QString stroke = it->attribute("CSTROKE",CommonStrings::None);
	int shade2 = it->attribute("CSHADE2", "100").toInt();
	int scale = qRound(it->attribute("CSCALE", "100").toDouble() * 10);
	int scalev = qRound(it->attribute("CSCALEV", "100").toDouble() * 10);
	int base = qRound(it->attribute("CBASE", "0").toDouble() * 10);
	int shX = qRound(it->attribute("CSHX", "5").toDouble() * 10);
	int shY = qRound(it->attribute("CSHY", "-5").toDouble() * 10);
	int outL = qRound(it->attribute("COUT", "1").toDouble() * 10);
	int ulp = qRound(it->attribute("CULP", "-0.1").toDouble() * 10);
	int ulw = qRound(it->attribute("CULW", "-0.1").toDouble() * 10);
	int stp = qRound(it->attribute("CSTP", "-0.1").toDouble() * 10);
	int stw = qRound(it->attribute("CSTW", "-0.1").toDouble() * 10);
#ifndef NLS_PROTO
	for (uint cxx=0; cxx<tmp2.length(); ++cxx)
	{
		hg = new ScText;
		hg->ch = tmp2.at(cxx);
		if (hg->ch == QChar(5))
			hg->ch = QChar(13);
		if (hg->ch == QChar(4))
			hg->ch = QChar(9);
		if (unknown)
			hg->setFont(dummy);
		else
			hg->setFont((*doc->AllFonts)[tmpf]);
		hg->setFontSize(size);
		hg->setFillColor(fcolor);
		hg->setTracking(extra);
		hg->setFillShade(shade);
		hg->setEffects(static_cast<StyleFlag>(cstyle));
		if (impo)
		{
			if (VorLFound)
				hg->cab = DoVorl[ab].toUInt();
			else
			{
				if (ab < 5)
					hg->cab = ab;
				else
					hg->cab = 0;
			}
		}
		else
			hg->cab = ab;
		hg->setStrokeColor(stroke);
		hg->setStrokeShade(shade2);
		hg->setScaleH(QMIN(QMAX(scale, 100), 4000));
		hg->setScaleV(QMIN(QMAX(scalev, 100), 4000));
		hg->setBaselineOffset(base);
		hg->setShadowXOffset(shX);
		hg->setShadowYOffset(shY);
		hg->setOutlineWidth(outL);
		hg->setUnderlineOffset(ulp);
		hg->setUnderlineWidth(ulw);
		hg->setStrikethruOffset(stp);
		hg->setStrikethruWidth(stw);
		obj->itemText.append(hg);
	}
#else
	for (uint cxx=0; cxx<tmp2.length(); ++cxx)
	{
		CharStyle style;
		QChar ch = tmp2.at(cxx);
		if (ch == QChar(5))
			ch = QChar(13);
		if (ch == QChar(4))
			ch = QChar(9);
		if (unknown)
			style.cfont = dummy;
		else
			style.cfont = (*doc->AllFonts)[tmpf];
		style.csize = size;
		style.ccolor = fcolor;
		style.cextra = extra;
		style.cshade = shade;
		style.cstyle = cstyle;
		/*FIXME:NLS
			if (impo)
		{
				if (VorLFound)
					hg->cab = DoVorl[ab].toUInt();
				else
				{
					if (ab < 5)
						hg->cab = ab;
					else
						hg->cab = 0;
				}
		}
		else
			hg->cab = ab;
		*/
		style.cstroke = stroke;
		style.cshade2 = shade2;
		style.cscale = QMIN(QMAX(scale, 100), 4000);
		style.cscalev = QMIN(QMAX(scalev, 100), 4000);
		style.cbase = base;
		style.cshadowx = shX;
		style.cshadowy = shY;
		style.coutline = outL;
		style.cunderpos = ulp;
		style.cunderwidth = ulw;
		style.cstrikepos = stp;
		style.cstrikewidth = stw;
		int pos = obj->itemText.length();
		obj->itemText.insertChars(pos, QString(ch));
		obj->itemText.applyStyle(pos, 1, style);
	}
#endif	
	return;
}

bool Scribus12Format::loadPage(const QString & fileName, int pageNumber, bool Mpage, QString /*renamedPageName*/)
{
	qDebug(QString("loading page %2 from file '%1' from 1.2.x plugin").arg(fileName).arg(pageNumber));
	if (m_Doc==0 || m_View==0 || m_AvailableFonts==0)
	{
		Q_ASSERT(m_Doc==0 || m_View==0 || m_AvailableFonts==0);
		return false;
	}

	struct CopyPasteBuffer OB;
	ParagraphStyle vg;
	struct Layer la;
	struct ScribusDoc::BookMa bok;
	struct Linked Link;
	PageItem *Neu;
	LFrames.clear();
	QString tmV, tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	int x, a, counter, baseobj;
	bool newVersion = false;
	bool VorLFound = false;
	QMap<int,int> layerTrans;
	int maxLayer = 0;
	int maxLevel = 0;
	layerTrans.clear();
	uint layerCount=m_Doc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		maxLayer = QMAX(m_Doc->Layers[la2].LNr, maxLayer);
		maxLevel = QMAX(m_Doc->Layers[la2].Level, maxLevel);
	}
	DoVorl.clear();
	DoFonts.clear();
	DoFonts[m_Doc->toolSettings.defFont] = m_Doc->toolSettings.defFont;
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
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8"))
		return false;
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
	/*
	* Attribute von DOCUMENT auslesen
	*/
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
				lf.setSpotColor(false);
				lf.setRegistrationColor(false);
				m_Doc->PageColors[pg.attribute("NAME")] = lf;
			}
			if(pg.tagName()=="STYLE")
			{
				GetStyle(&pg, &vg, m_Doc->docParagraphStyles, m_Doc, true);
				VorLFound = true;
			}
			if(pg.tagName()=="JAVA")
				m_Doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = pg.attribute("NUMMER").toInt();
				la.Level = pg.attribute("LEVEL").toInt();
				la.Name = pg.attribute("NAME");
				la.isViewable = pg.attribute("SICHTBAR").toInt();
				la.isPrintable = pg.attribute("DRUCKEN").toInt();
				la.isEditable = true;
				la.flowControl = true;
				la.transparency = 1.0;
				la.blendMode = 0;
				QColor marker;
				switch (la.LNr % 7)
				{
					case 0:
						marker = Qt::black;
						break;
					case 1:
						marker = Qt::red;
						break;
					case 2:
						marker = Qt::green;
						break;
					case 3:
						marker = Qt::blue;
						break;
					case 4:
						marker = Qt::cyan;
						break;
					case 5:
						marker = Qt::magenta;
						break;
					case 6:
						marker = Qt::yellow;;
						break;
				}
				la.markerColor = marker;
				la.outlineMode = false;
				bool laex = false;
				uint layerCount=m_Doc->layerCount();
				for (uint la2 = 0; la2 < layerCount; ++la2)
				{
					if (m_Doc->Layers[la2].Name == la.Name)
					{
						laex = true;
						layerTrans.insert(la.LNr, m_Doc->Layers[la2].LNr);
					}
				}
				if (!laex)
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
					sl.Width = MuL.attribute("Width").toDouble();
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				QString Nam = pg.attribute("Name");
				QString Nam2 = Nam;
				int copyC = 1;
				while (m_Doc->MLineStyles.contains(Nam2))
				{
					Nam2 = tr("Copy #%1 of ").arg(copyC)+Nam;
					copyC++;
				}
				m_Doc->MLineStyles.insert(Nam2, ml);
			}
			if ((pg.tagName()=="PAGE") && (pg.attribute("NUM").toInt() == pageNumber))
			{
				a = m_Doc->currentPage()->pageNr();
				if ((pg.attribute("NAM", "").isEmpty()) && (Mpage))
				{
					PAGE=PAGE.nextSibling();
					continue;
				}
				if (Mpage)
				{
					m_Doc->Pages->at(a)->LeftPg=pg.attribute("LEFT", "0").toInt();
					m_Doc->Pages->at(a)->setPageName(pg.attribute("NAM",""));
				}
				TableItems.clear();
				TableID.clear();
				/*
				* Attribute von PAGE auslesen
				*/
				tmp = "";
				GuideManagerCore::readVerticalGuides(pg.attribute("VerticalGuides"),
						m_Doc->Pages->at(a),
						GuideManagerCore::Standard,
						pg.hasAttribute("NumVGuides"));
				GuideManagerCore::readHorizontalGuides(pg.attribute("HorizontalGuides"),
						m_Doc->Pages->at(a),
						GuideManagerCore::Standard,
						pg.hasAttribute("NumHGuides"));

				QDomNode OBJ=PAGE.firstChild();
				counter = m_Doc->Items->count();
				baseobj = counter;
				while(!OBJ.isNull())
				{
					QDomElement obj=OBJ.toElement();
				/*
				* Attribute von OBJECT auslesen
				*/
					if ((obj.attribute("NEXTITEM").toInt() != -1) && (obj.attribute("NEXTPAGE").toInt() == pageNumber))
					{
						if (obj.attribute("BACKITEM").toInt() == -1)
						{
							Link.Start = counter;
							Link.StPag = a;
							LFrames.append(Link);
						}
					}
					GetItemProps(newVersion, &obj, &OB);
					OB.Xpos = obj.attribute("XPOS").toDouble()+m_Doc->Pages->at(a)->xOffset();
					OB.Ypos=obj.attribute("YPOS").toDouble()+m_Doc->Pages->at(a)->yOffset();
					OB.NamedLStyle = obj.attribute("NAMEDLST", "");
					if (!m_Doc->MLineStyles.contains(OB.NamedLStyle))
						OB.NamedLStyle = "";
					OB.isBookmark=obj.attribute("BOOKMARK").toInt();
					if ((OB.isBookmark) && (m_Doc->BookMarks.count() == 0))
						m_Doc->OldBM = true;
					OB.textAlignment = DoVorl[obj.attribute("ALIGN", "0").toInt()].toUInt();
					tmpf = obj.attribute("IFONT", m_Doc->toolSettings.defFont);
					if (tmpf.isEmpty())
						tmpf = m_Doc->toolSettings.defFont;
					tmf = tmpf;
					if (!DoFonts.contains(tmpf))
						tmpf = AskForFont(*m_AvailableFonts, tmpf, m_Doc);
					else
						tmpf = DoFonts[tmf];
					OB.IFont = tmpf;
					OB.LayerNr = layerTrans[obj.attribute("LAYER", "0").toInt()];
					OB.Language = obj.attribute("LANGUAGE", m_Doc->Language);
					tmp = "";
					if ((obj.hasAttribute("GROUPS")) && (obj.attribute("NUMGROUP", "0").toInt() != 0))
					{
						tmp = obj.attribute("GROUPS");
						QTextStream fg(&tmp, IO_ReadOnly);
						OB.Groups.clear();
						for (int cx = 0; cx < obj.attribute("NUMGROUP", "0").toInt(); ++cx)
						{
							fg >> x;
							OB.Groups.push(x);
						}
					tmp = "";
					}
					else
						OB.Groups.clear();
					QDomNode IT=OBJ.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="CSTOP")
						{
							QString name = it.attribute("NAME");
							double ramp = it.attribute("RAMP", "0.0").toDouble();
							int shade = it.attribute("SHADE", "100").toInt();
							double opa = it.attribute("TRANS", "1").toDouble();
							OB.fill_gradient.addStop(SetColor(m_Doc, name, shade), ramp, 0.5, opa, name, shade);
							OB.GrColor = "";
							OB.GrColor2 = "";
						}
						IT=IT.nextSibling();
					}
					OB.itemText = "";
					m_View->PasteItem(&OB, true);
					Neu = m_Doc->Items->at(counter);
					IT=OBJ.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="ITEXT")
							GetItemText(&it, m_Doc, VorLFound, true, Neu);
						IT=IT.nextSibling();
					}
					if (obj.attribute("NEXTPAGE").toInt() == pageNumber)
					{
						Neu->NextIt = baseobj + obj.attribute("NEXTITEM").toInt();
						Neu->NextPg = a; // obj.attribute("NEXTPAGE").toInt();
					}
					else
						Neu->NextIt = -1;
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(obj.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
					}
					counter++;
					OBJ=OBJ.nextSibling();
				}
				if (TableItems.count() != 0)
				{
					for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
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
				if (LFrames.count() != 0)
				{
					PageItem *Its;
					PageItem *Itn;
					PageItem *Itr;
					QValueList<Linked>::Iterator lc;
					for (lc = LFrames.begin(); lc != LFrames.end(); ++lc)
					{
						Its = m_Doc->Items->at((*lc).Start);
						Itr = Its;
						Its->BackBox = 0;
						while (Its->NextIt != -1)
						{
							if (Its->NextPg == a)
							{
								Itn = m_Doc->Items->at(Its->NextIt);
								Its->NextBox = Itn;
								Itn->BackBox = Its;
								Its = Itn;
							}
							else
								break;
						}
						Its->NextBox = 0;
					}
				}
				if (!Mpage)
					m_View->reformPages();
				PAGE=DOC.firstChild();
				while(!PAGE.isNull())
				{
					QDomElement pg=PAGE.toElement();
					if(pg.tagName()=="Bookmark")
					{
						bok.Title = pg.attribute("Title");
						bok.Text = pg.attribute("Text");
						bok.Aktion = pg.attribute("Aktion");
						bok.ItemNr = pg.attribute("ItemNr").toInt();
						bok.PageObject = m_Doc->Items->at(pg.attribute("Element").toInt());
						bok.First = pg.attribute("First").toInt();
						bok.Last = pg.attribute("Last").toInt();
						bok.Prev = pg.attribute("Prev").toInt();
						bok.Next = pg.attribute("Next").toInt();
						bok.Parent = pg.attribute("Parent").toInt();
						m_Doc->BookMarks.append(bok);
					}
				PAGE=PAGE.nextSibling();
				}
				return true;
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return false;
}

void Scribus12Format::GetStyle(QDomElement *pg, ParagraphStyle *vg, QValueList<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl)
{
	bool fou;
	QString tmpf, tmf, tmV;
	double xf, xf2;
	fou = false;
	vg->setName(pg->attribute("NAME"));
	vg->setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(pg->attribute("LINESPMode", "0").toInt()));
	vg->setLineSpacing(pg->attribute("LINESP").toDouble());
	vg->setLeftMargin(pg->attribute("INDENT", "0").toDouble());
	vg->setFirstIndent(pg->attribute("FIRST", "0").toDouble());
	vg->setAlignment(pg->attribute("ALIGN").toInt());
	vg->setGapBefore(pg->attribute("VOR", "0").toDouble());
	vg->setGapAfter(pg->attribute("NACH", "0").toDouble());
	tmpf = pg->attribute("FONT", doc->toolSettings.defFont);
	if (tmpf.isEmpty())
		tmpf = doc->toolSettings.defFont;
	tmf = tmpf;
	PrefsManager* prefsManager=PrefsManager::instance();
	if (!DoFonts.contains(tmpf))
		tmpf = AskForFont(prefsManager->appPrefs.AvailFonts, tmpf, doc);
	else
		tmpf = DoFonts[tmf];
	vg->charStyle().setFont(prefsManager->appPrefs.AvailFonts[tmpf]);
	vg->charStyle().setFontSize(qRound(pg->attribute("FONTSIZE", "12").toDouble() * 10.0));
	vg->setHasDropCap(static_cast<bool>(pg->attribute("DROP", "0").toInt()));
	vg->setDropCapLines(pg->attribute("DROPLIN", "2").toInt());
	vg->setDropCapOffset(pg->attribute("DROPDIST", "0").toDouble());
	vg->charStyle().setEffects(static_cast<StyleFlag>((pg->attribute("EFFECT", "0").toInt())));
	vg->charStyle().setFillColor(pg->attribute("FCOLOR", doc->toolSettings.dBrush));
	vg->charStyle().setFillShade(pg->attribute("FSHADE", "100").toInt());
	vg->charStyle().setStrokeColor(pg->attribute("SCOLOR", doc->toolSettings.dPen));
	vg->charStyle().setStrokeShade(pg->attribute("SSHADE", "100").toInt());
	vg->setUseBaselineGrid(static_cast<bool>(pg->attribute("BASE", "0").toInt()));
	vg->charStyle().setShadowXOffset(qRound(pg->attribute("TXTSHX", "5").toDouble() * 10));
	vg->charStyle().setShadowYOffset(qRound(pg->attribute("TXTSHY", "-5").toDouble() * 10));
	vg->charStyle().setOutlineWidth(qRound(pg->attribute("TXTOUT", "1").toDouble() * 10));
	vg->charStyle().setUnderlineOffset(qRound(pg->attribute("TXTULP", "-0.1").toDouble() * 10));
	vg->charStyle().setUnderlineWidth(qRound(pg->attribute("TXTULW", "-0.1").toDouble() * 10));
	vg->charStyle().setStrikethruOffset(qRound(pg->attribute("TXTSTP", "-0.1").toDouble() * 10));
	vg->charStyle().setStrikethruWidth(qRound(pg->attribute("TXTSTW", "-0.1").toDouble() * 10));
	vg->charStyle().setScaleH(qRound(pg->attribute("SCALEH", "100").toDouble() * 10));
	vg->charStyle().setScaleV(qRound(pg->attribute("SCALEV", "100").toDouble() * 10));
	vg->charStyle().setBaselineOffset(qRound(pg->attribute("BASEO", "0").toDouble() * 10));
	vg->charStyle().setTracking(qRound(pg->attribute("KERN", "0").toDouble() * 10));
	vg->tabValues().clear();
	if ((pg->hasAttribute("NUMTAB")) && (pg->attribute("NUMTAB", "0").toInt() != 0))
	{
		ParagraphStyle::TabRecord tb;
		QString tmp = pg->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		vg->tabValues().clear();
		for (int cxv = 0; cxv < pg->attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			vg->tabValues().append(tb);
		}
		tmp = "";
	}
	else
	{
		QDomNode IT = pg->firstChild();
		while(!IT.isNull())
		{
			QDomElement it = IT.toElement();
			if (it.tagName()=="Tabs")
			{
				ParagraphStyle::TabRecord tb;
				tb.tabPosition = it.attribute("Pos").toDouble();
				tb.tabType = it.attribute("Type").toInt();
				QString tbCh = "";
				tbCh = it.attribute("Fill","");
				if (tbCh.isEmpty())
					tb.tabFillChar = QChar();
				else
					tb.tabFillChar = tbCh[0];
				vg->tabValues().append(tb);
			}
			IT=IT.nextSibling();
		}
	}
	for (uint xx=0; xx<docParagraphStyles.count(); ++xx)
	{
		if (vg->name() == docParagraphStyles[xx].name())
		{
			//Compare the attributes of the pasted styles vs existing ones
			if (vg->equiv(docParagraphStyles[xx]))
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
				vg->setName("Copy of "+docParagraphStyles[xx].name());
				fou = false;
			}
			break;
		}
	}
	if (!fou)
	{
		for (uint xx=0; xx< docParagraphStyles.count(); ++xx)
		{
			if (vg->equiv(docParagraphStyles[xx]))
			{
				vg->setName(docParagraphStyles[xx].name());
				fou = true;
				if (fl)
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
		docParagraphStyles.append(*vg);
		if (fl)
		{
			DoVorl[VorlC] = tmV.setNum(docParagraphStyles.count()-1);
			VorlC++;
		}
	}
}

QString Scribus12Format::AskForFont(SCFonts &avail, QString fStr, ScribusDoc *doc)
{
	PrefsManager *prefsManager=PrefsManager::instance();
//	QFont fo;
	QString tmpf = fStr;
	if ((!avail.find(tmpf)) || (!avail[tmpf]->usable()))
	{
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!avail[prefsManager->appPrefs.GFontSub[tmpf]]->usable()))
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			MissingFont *dia = new MissingFont(0, tmpf, doc);
			dia->exec();
			tmpf = dia->getReplacementFont();
			delete dia;
			qApp->setOverrideCursor(QCursor(waitCursor), true);
			prefsManager->appPrefs.GFontSub[fStr] = tmpf;
		}
		else
			tmpf = prefsManager->appPrefs.GFontSub[tmpf];
		ReplacedFonts[fStr] = tmpf;
	}
	if (!doc->UsedFonts.contains(tmpf))
	{
//		fo = avail[tmpf]->Font;
//		fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
		doc->AddFont(tmpf);
	}
	DoFonts[fStr] = tmpf;
	return tmpf;
}

bool Scribus12Format::readStyles(const QString& fileName, ScribusDoc* doc, QValueList<ParagraphStyle> &docParagraphStyles)
{
	ParagraphStyle vg;
	QDomDocument docu("scridoc");
	QString tmpf, tmf;
	DoFonts.clear();
	QString f (readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8"))
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
				GetStyle(&pg, &vg, docParagraphStyles, doc, false);
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}

bool Scribus12Format::readLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty)
{
	QDomDocument docu("scridoc");
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8"))
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
					sl.Width = MuL.attribute("Width").toDouble();
					ml.push_back(sl);
					MuLn = MuLn.nextSibling();
				}
				QString Nam = pg.attribute("Name");
				QString Nam2 = Nam;
				int copyC = 1;
				while (Sty->contains(Nam2))
				{
					Nam2 = tr("Copy #%1 of ").arg(copyC)+Nam;
					copyC++;
				}
				Sty->insert(Nam2, ml);
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}


bool Scribus12Format::readColors(const QString& fileName, ColorList & colors)
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
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8"))
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
				colors[pg.attribute("NAME")] = lf;
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}

bool Scribus12Format::readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames)
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
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8"))
		return false;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="PAGE")
			{
				PgNam = pg.attribute("NAM", "");
				if (PgNam.isEmpty())
					counter++;
				else
				{
					counter2++;
					masterPageNames.append(PgNam);
				}
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	*num1 = counter;
	*num2 = counter2;
	return true;
}
