/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus12format.h"
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
#include "util_color.h"
#include "scgzfile.h"
#include <QCursor>
#include <QFileInfo>
#include <QList>
#include <QByteArray>
#include <QTextStream>
#include <QApplication>


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
	about->authors = QString::fromUtf8(
			"Franz Schmid <franz@scribus.info>, "
			"The Scribus Team");
	about->shortDescription = tr("Scribus 1.2.x File Format Support");
	about->description = tr("Allows Scribus to read Scribus 1.2.x formatted files.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
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
	fmt.filter = fmt.trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
	fmt.nameMatch = QRegExp("\\.(sla|scd)(\\.gz)?", Qt::CaseInsensitive);
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-scribus");
	fmt.priority = 64;
	registerFormat(fmt);
}

bool Scribus12Format::fileSupported(QIODevice* /* file */, const QString & fileName) const
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
	if (docBytes.left(16) != "<SCRIBUSUTF8NEW " && (docBytes.left(12) == "<SCRIBUSUTF8" || docBytes.left(9) == "<SCRIBUS>"))
		return true;
	return false;
}

QString Scribus12Format::readSLA(const QString & fileName)
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
	if (docBytes.left(16) != "<SCRIBUSUTF8NEW ") // Not a 1.3.x doc
	{
		if (docBytes.left(12) == "<SCRIBUSUTF8") // 1.2.x UTF8 doc
			docText = QString::fromUtf8(docBytes);
		else if (docBytes.left(9) == "<SCRIBUS>") // Older non utf8 doc
			docText = QString::fromLocal8Bit(docBytes);
		else 
			return QString::null;
	}
	else
	{
		qDebug("scribus12format: SCRIBUSUTF8NEW");
		return QString::null;
	}
	if (docText.endsWith(QChar(10)) || docText.endsWith(QChar(13)))
		docText.truncate(docText.length()-1);
	return docText;	return docText;
}


void Scribus12Format::getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces)
{
	getNewReplacement=false;
	getReplacedFonts.clear();
	getDummyScFaces.clear();
}

static long long scribus12itemID(int itemNr, int pageNr)
{
	return itemNr * 100000 + pageNr; 
}

bool Scribus12Format::loadFile(const QString & fileName, const FileFormat & /* fmt */, int /* flags */, int /* index */)
{
	if (m_Doc==0 || m_View==0 || m_AvailableFonts==0)
	{
		Q_ASSERT(m_Doc==0 || m_View==0 || m_AvailableFonts==0);
		return false;
	}
// 	ReplacedFonts.clear();
// 	newReplacement = false;
// 	dummyScFaces.clear();
	DoVorl.clear();
	DoVorl[0] = "";
	DoVorl[1] = "";
	DoVorl[2] = "";
	DoVorl[3] = "";
	DoVorl[4] = "";
	VorlC = 5;
	
//start old ReadDoc
	//Scribus 1.2 docs, see fileloader.cpp for 1.3 docs
	struct CopyPasteBuffer OB;
	ParagraphStyle vg;
	struct ScribusDoc::BookMa bok;
	int counter;
	bool newVersion = false;
	QString tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QMap<int,int> TableID;
	QList<PageItem*> TableItems;
	int x, a;
//	double xf, xf2;
	PageItem *Neu;
	itemRemap.clear();
	itemNext.clear();
	QDomDocument docu("scridoc");
	QFile fi(fileName);
	// Load the document text
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	// Build the DOM from it
	if (!docu.setContent(f))
		return false;
	// Get file directory
	QString fileDir = QFileInfo(fileName).absolutePath();
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
		m_mwProgressBar->setMaximum(DOC.childNodes().count());
		m_mwProgressBar->setValue(0);
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
		m_Doc->pageMargins.Left=qMax(0.0, dc.attribute("BORDERLEFT").toDouble());
		m_Doc->pageMargins.Right=qMax(0.0, dc.attribute("BORDERRIGHT").toDouble());
		m_Doc->pageMargins.Top=qMax(0.0, dc.attribute("BORDERTOP").toDouble());
		m_Doc->pageMargins.Bottom=qMax(0.0, dc.attribute("BORDERBOTTOM").toDouble());
		m_Doc->marginPreset = dc.attribute("PRESET", "0").toInt();
		m_Doc->PageOri = dc.attribute("ORIENTATION", "0").toInt();
		m_Doc->m_pageSize = dc.attribute("PAGESIZE");
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
// 		DoFonts.clear();
		m_Doc->toolSettings.defSize=qRound(dc.attribute("DSIZE").toDouble() * 10);
		Defont=dc.attribute("DFONT");
		m_Doc->toolSettings.defFont = prefsManager->appPrefs.toolSettings.defFont;
		m_AvailableFonts->findFont(Defont, m_Doc);
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
		//m_Doc->HasCMS = static_cast<bool>(dc.attribute("HCMS", "0").toInt());
		m_Doc->CMSSettings.SoftProofOn = static_cast<bool>(dc.attribute("DPSo", "0").toInt());
		m_Doc->CMSSettings.SoftProofFullOn = static_cast<bool>(dc.attribute("DPSFo", "0").toInt());
		m_Doc->CMSSettings.CMSinUse = static_cast<bool>(dc.attribute("DPuse", "0").toInt());
		m_Doc->CMSSettings.GamutCheck = static_cast<bool>(dc.attribute("DPgam", "0").toInt());
		m_Doc->CMSSettings.BlackPoint = static_cast<bool>(dc.attribute("DPbla", "1").toInt());
		m_Doc->CMSSettings.DefaultMonitorProfile = dc.attribute("DPMo","");
		m_Doc->CMSSettings.DefaultPrinterProfile = dc.attribute("DPPr","");
		m_Doc->CMSSettings.DefaultImageRGBProfile = dc.attribute("DPIn","");
		m_Doc->CMSSettings.DefaultImageCMYKProfile = dc.attribute("DPPr",""); // Use DPPr to match 1.2.x behavior
		m_Doc->CMSSettings.DefaultSolidColorRGBProfile = dc.attribute("DPIn2","");
		m_Doc->CMSSettings.DefaultSolidColorCMYKProfile = dc.attribute("DPPr",""); // Use DPPr to match 1.2.x behavior
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
				m_mwProgressBar->setValue(ObCount);
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
				m_Doc->PageColors.insert(pg.attribute("NAME"), lf);
			}
			if(pg.tagName()=="STYLE")
			{
				vg.erase();
				GetStyle(&pg, &vg, NULL, m_Doc, true);
				StyleSet<ParagraphStyle> temp;
				temp.create(vg);
				m_Doc->redefineStyles(temp, false);
			}
			if(pg.tagName()=="JAVA")
				m_Doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				int lnr   = pg.attribute("NUMMER").toInt();
				int level = pg.attribute("LEVEL").toInt();
				ScLayer la( pg.attribute("NAME"), level, lnr);
				la.isViewable = pg.attribute("SICHTBAR").toInt();
				la.isPrintable = pg.attribute("DRUCKEN").toInt();
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
				m_Doc->Pages->at(a)->marginPreset = pg.attribute("PRESET", "0").toInt();

				// guides reading
				tmp = "";
				GuideManagerIO::readVerticalGuides(pg.attribute("VerticalGuides"),
							m_Doc->Pages->at(a),
							GuideManagerCore::Standard,
							pg.hasAttribute("NumVGuides"));
				GuideManagerIO::readHorizontalGuides(pg.attribute("HorizontalGuides"),
							m_Doc->Pages->at(a),
							GuideManagerCore::Standard,
							pg.hasAttribute("NumHGuides"));

				QDomNode OBJ=PAGE.firstChild();
				int pageNo = a;
				int pageItem = 0;
				while(!OBJ.isNull())
				{
					QDomElement obj=OBJ.toElement();
					/*
					 * Attribute von OBJECT auslesen
					 */
					if (!m_Doc->masterPageMode())
					{
						const long long itemID = scribus12itemID(pageItem++, pageNo);
//						qDebug() << QString("1.2 remap: %1 -> %2 [%3 on page %4]").arg(itemID).arg(m_Doc->Items->count()).arg(pageItem-1).arg(pageNo);
						itemRemap[itemID] = m_Doc->Items->count();
						if (obj.tagName()=="PAGEOBJECT")
						{
							// member of linked chain?
							if (obj.attribute("NEXTITEM").toInt() != -1)
							{
								itemNext[m_Doc->Items->count()] = scribus12itemID(obj.attribute("NEXTITEM").toInt(), obj.attribute("NEXTPAGE").toInt());
							}
						}
					}
					GetItemProps(&obj, &OB, fileDir, newVersion);
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
					m_AvailableFonts->findFont(tmpf, m_Doc);
					OB.IFont = tmpf;
					OB.LayerNr = obj.attribute("LAYER", "0").toInt();
					OB.Language = obj.attribute("LANGUAGE", m_Doc->Language);
					tmp = "";
					if ((obj.hasAttribute("GROUPS")) && (obj.attribute("NUMGROUP", "0").toInt() != 0))
					{
						tmp = obj.attribute("GROUPS");
						QTextStream fg(&tmp, QIODevice::ReadOnly);
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
							handleOldColorShade(m_Doc, name, shade);
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
					LastStyles * lastS = new LastStyles();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="ITEXT")
							GetItemText(&it, m_Doc, true, false, Neu, lastS);
						IT=IT.nextSibling();
					}
					delete lastS;
					Neu->isAutoText=static_cast<bool>(obj.attribute("AUTOTEXT").toInt());
					if (Neu->isAutoText)
						m_Doc->LastAuto = Neu;
//					Neu->NextIt = obj.attribute("NEXTITEM").toInt();
// 					nextPg[Neu->ItemNr] = obj.attribute("NEXTPAGE").toInt();
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
			}
			PAGE=PAGE.nextSibling();
		}
		m_Doc->setMasterPageMode(false);
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
			if(pg.tagName()=="PDF")
			{
				m_Doc->PDF_Options.Articles = static_cast<bool>(pg.attribute("Articles").toInt());
				m_Doc->PDF_Options.Thumbnails = static_cast<bool>(pg.attribute("Thumbnails").toInt());
				m_Doc->PDF_Options.Compress = static_cast<bool>(pg.attribute("Compress").toInt());
				m_Doc->PDF_Options.CompressMethod = (PDFOptions::PDFCompression) pg.attribute("CMethod", "0").toInt();
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
				m_Doc->PDF_Options.fileName = "";
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
				m_Doc->PDF_Options.bleeds.Top = pg.attribute("BTop", "0").toDouble();
				m_Doc->PDF_Options.bleeds.Left = pg.attribute("BLeft", "0").toDouble();
				m_Doc->PDF_Options.bleeds.Right = pg.attribute("BRight", "0").toDouble();
				m_Doc->PDF_Options.bleeds.Bottom = pg.attribute("BBottom", "0").toDouble();
				m_Doc->PDF_Options.EmbeddedI = static_cast<bool>(pg.attribute("ImagePr", "0").toInt());
				m_Doc->PDF_Options.PassOwner = pg.attribute("PassOwner", "");
				m_Doc->PDF_Options.PassUser = pg.attribute("PassUser", "");
				m_Doc->PDF_Options.Permissions = pg.attribute("Permissions", "-4").toInt();
				m_Doc->PDF_Options.Encrypt = static_cast<bool>(pg.attribute("Encrypt", "0").toInt());
				m_Doc->PDF_Options.useLayers = static_cast<bool>(pg.attribute("UseLayers", "0").toInt());
				m_Doc->PDF_Options.UseLPI = static_cast<bool>(pg.attribute("UseLpi", "0").toInt());
				m_Doc->PDF_Options.UseSpotColors = true;
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
		ScLayer* nl = m_Doc->Layers.newLayer( QObject::tr("Background") );
		nl->flowControl = false;
		activeLayer = nl->LNr;
	}
	m_Doc->setActiveLayer(activeLayer);
	
	// reestablish textframe links
	if (itemNext.count() != 0)
	{
		QMap<int,long long>::Iterator lc;
		for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
		{
//			qDebug() << QString("1.2 textframe links: %1->%2[%3]").arg(lc.key()).arg(itemRemap[lc.data()]).arg(lc.data());
			PageItem *Its = m_Doc->Items->at(lc.key());
			PageItem *Itn = m_Doc->Items->at(itemRemap[lc.value()]);
			assert(Its && Its->asTextFrame());
			assert(Itn && Itn->asTextFrame());
			if (Itn->prevInChain() || Its->nextInChain()) 
			{
				qDebug("scribus12format: corruption in linked textframes detected");
				continue;
			}
			Its->link(Itn);
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
	
	setCurrentComboItem(m_View->unitSwitcher, unitGetStrFromIndex(m_Doc->unitIndex()));
	if (m_mwProgressBar!=0)
		m_mwProgressBar->setValue(DOC.childNodes().count());
	return true;
//end old ReadDoc
	
	
	//return false;
}

bool Scribus12Format::saveFile(const QString & /* fileName */, const FileFormat & /* fmt */)
{
	return false;
}

void Scribus12Format::GetItemProps(QDomElement *obj, struct CopyPasteBuffer *OB, const QString& baseDir, bool newVersion)
{
	QString tmp;
	int x, y;
	double xf, yf, xf2;
	OB->PType = static_cast<PageItem::ItemType>(obj->attribute("PTYPE").toInt());
	OB->Width=obj->attribute("WIDTH").toDouble();
	OB->Height=obj->attribute("HEIGHT").toDouble();
	OB->RadRect = obj->attribute("RADRECT", "0").toDouble();
	OB->ClipEdited = obj->attribute("CLIPEDIT", "0").toInt();
	OB->FrameType = obj->attribute("FRTYPE", "0").toInt();
	OB->Pwidth=obj->attribute("PWIDTH").toDouble();
	OB->Pcolor = obj->attribute("PCOLOR");
	if ((!newVersion) && (OB->PType == 4))
	{
		OB->TxtFill = obj->attribute("PCOLOR2");
		OB->Pcolor2 = CommonStrings::None;
	}
	else
	{
		OB->Pcolor2 = obj->attribute("PCOLOR2");
		OB->TxtFill = obj->attribute("TXTFILL", "Black");
	}
	OB->Shade = obj->attribute("SHADE").toInt();
	OB->Shade2 = obj->attribute("SHADE2").toInt();
	OB->FillRule = obj->attribute("fillRule", "1").toInt();
	OB->TxtStroke=obj->attribute("TXTSTROKE", CommonStrings::None);
	OB->ShTxtFill=obj->attribute("TXTFILLSH", "100").toInt();
	OB->ShTxtStroke=obj->attribute("TXTSTRSH", "100").toInt();
	OB->TxtScale=qRound(obj->attribute("TXTSCALE", "100").toDouble() * 10);
	OB->TxtScaleV=qRound(obj->attribute("TXTSCALEV", "100").toDouble() * 10);
	OB->TxTBase=qRound(obj->attribute("TXTBASE", "0").toDouble() * 10);
	OB->TxTStyle=obj->attribute("TXTSTYLE", "0").toInt();
	OB->TxtShadowX=qRound(obj->attribute("TXTSHX", "5").toDouble() * 10);
	OB->TxtShadowY=qRound(obj->attribute("TXTSHY", "-5").toDouble() * 10);
	OB->TxtOutline=qRound(obj->attribute("TXTOUT", "1").toDouble() * 10);
	OB->TxtUnderPos=qRound(obj->attribute("TXTULP", "-0.1").toDouble() * 10);
	OB->TxtUnderWidth=qRound(obj->attribute("TXTULW", "-0.1").toDouble() * 10);
	OB->TxtStrikePos=qRound(obj->attribute("TXTSTP", "-0.1").toDouble() * 10);
	OB->TxtStrikeWidth=qRound(obj->attribute("TXTSTW", "-0.1").toDouble() * 10);
	OB->Cols = obj->attribute("COLUMNS", "1").toInt();
	OB->ColGap = obj->attribute("COLGAP", "0.0").toDouble();
	OB->GrType = obj->attribute("GRTYP", "0").toInt();
	OB->fill_gradient.clearStops();
	if (OB->GrType != 0)
	{
		if (OB->GrType == 8)
		{
			OB->pattern = obj->attribute("pattern", "");
			OB->patternScaleX = obj->attribute("pScaleX", "100.0").toDouble();
			OB->patternScaleY = obj->attribute("pScaleY", "100.0").toDouble();
			OB->patternOffsetX = obj->attribute("pOffsetX", "0.0").toDouble();
			OB->patternOffsetY = obj->attribute("pOffsetY", "0.0").toDouble();
			OB->patternRotation = obj->attribute("pRotation", "0.0").toDouble();
		}
		else
		{
			OB->GrStartX = obj->attribute("GRSTARTX", "0.0").toDouble();
			OB->GrStartY = obj->attribute("GRSTARTY", "0.0").toDouble();
			OB->GrEndX = obj->attribute("GRENDX", "0.0").toDouble();
			OB->GrEndY = obj->attribute("GRENDY", "0.0").toDouble();
			OB->GrColor = obj->attribute("GRCOLOR","");
			if (OB->GrColor.isEmpty())
				OB->GrColor = "Black";
			OB->GrColor2 = obj->attribute("GRCOLOR2","Black");
			if (OB->GrColor2.isEmpty())
				OB->GrColor2 = "Black";
			OB->GrShade = obj->attribute("GRSHADE", "100").toInt();
			OB->GrShade2 = obj->attribute("GRSHADE2", "100").toInt();
		}
	}
	OB->Rot=obj->attribute("ROT").toDouble();
	OB->PLineArt=Qt::PenStyle(obj->attribute("PLINEART").toInt());
	OB->PLineEnd=Qt::PenCapStyle(obj->attribute("PLINEEND", "0").toInt());
	OB->PLineJoin=Qt::PenJoinStyle(obj->attribute("PLINEJOIN", "0").toInt());
	OB->LineSp=obj->attribute("LINESP").toDouble();
	OB->LineSpMode = obj->attribute("LINESPMode", "0").toInt();
	OB->LocalScX=obj->attribute("LOCALSCX").toDouble();
	OB->LocalScY=obj->attribute("LOCALSCY").toDouble();
	OB->LocalX=obj->attribute("LOCALX").toDouble();
	OB->LocalY=obj->attribute("LOCALY").toDouble();
	OB->PicArt=obj->attribute("PICART").toInt();
	OB->flippedH = obj->attribute("FLIPPEDH").toInt() % 2;
	OB->flippedV = obj->attribute("FLIPPEDV").toInt() % 2;
/*	OB->BBoxX=obj->attribute("BBOXX").toDouble();
	OB->BBoxH=obj->attribute("BBOXH").toDouble(); */
	OB->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
	OB->AspectRatio = obj->attribute("RATIO", "0").toInt();
	OB->isPrintable=obj->attribute("PRINTABLE").toInt();
	OB->m_isAnnotation=obj->attribute("ANNOTATION", "0").toInt();
	OB->m_annotation.setType(obj->attribute("ANTYPE", "0").toInt());
	OB->m_annotation.setAction(obj->attribute("ANACTION",""));
	OB->m_annotation.setE_act(obj->attribute("ANEACT",""));
	OB->m_annotation.setX_act(obj->attribute("ANXACT",""));
	OB->m_annotation.setD_act(obj->attribute("ANDACT",""));
	OB->m_annotation.setFo_act(obj->attribute("ANFOACT",""));
	OB->m_annotation.setBl_act(obj->attribute("ANBLACT",""));
	OB->m_annotation.setK_act(obj->attribute("ANKACT",""));
	OB->m_annotation.setF_act(obj->attribute("ANFACT",""));
	OB->m_annotation.setV_act(obj->attribute("ANVACT",""));
	OB->m_annotation.setC_act(obj->attribute("ANCACT",""));
	OB->m_annotation.setActionType(obj->attribute("ANACTYP", "0").toInt());
	OB->m_annotation.setExtern(obj->attribute("ANEXTERN",""));
	if ((!OB->m_annotation.Extern().isEmpty()) && (OB->m_annotation.ActionType() != 8))
	{
		QFileInfo efp(OB->m_annotation.Extern());
		OB->m_annotation.setExtern(efp.absoluteFilePath());
	}
	OB->m_annotation.setZiel(obj->attribute("ANZIEL", "0").toInt());
	OB->AnName=obj->attribute("ANNAME","");
	OB->m_annotation.setToolTip(obj->attribute("ANTOOLTIP",""));
	OB->m_annotation.setRollOver(obj->attribute("ANROLL",""));
	OB->m_annotation.setDown(obj->attribute("ANDOWN",""));
	OB->m_annotation.setBwid(obj->attribute("ANBWID", "1").toInt());
	OB->m_annotation.setBsty(obj->attribute("ANBSTY", "0").toInt());
	OB->m_annotation.setFeed(obj->attribute("ANFEED", "1").toInt());
	OB->m_annotation.setFlag(obj->attribute("ANFLAG", "0").toInt());
	OB->m_annotation.setFont(obj->attribute("ANFONT", "4").toInt());
	OB->m_annotation.setFormat(obj->attribute("ANFORMAT", "0").toInt());
	OB->m_annotation.setVis(obj->attribute("ANVIS", "0").toInt());
	OB->m_annotation.setIsChk(static_cast<bool>(obj->attribute("ANCHK", "0").toInt()));
	OB->m_annotation.setAAact(static_cast<bool>(obj->attribute("ANAA", "0").toInt()));
	OB->m_annotation.setHTML(static_cast<bool>(obj->attribute("ANHTML", "0").toInt()));
	OB->m_annotation.setUseIcons(static_cast<bool>(obj->attribute("ANICON", "0").toInt()));
	OB->m_annotation.setChkStil(obj->attribute("ANCHKS", "0").toInt());
	OB->m_annotation.setMaxChar(obj->attribute("ANMC", "-1").toInt());
	OB->m_annotation.setBorderColor(obj->attribute("ANBCOL",CommonStrings::None));
	OB->m_annotation.setIPlace(obj->attribute("ANPLACE", "1").toInt());
	OB->m_annotation.setScaleW(obj->attribute("ANSCALE", "0").toInt());
	if (obj->attribute("TRANSPARENT", "0").toInt() == 1)
		OB->Pcolor = CommonStrings::None;
	OB->textAlignment=obj->attribute("ALIGN", "0").toInt();
	if ( obj->hasAttribute("TEXTFLOWMODE") )
		OB->TextflowMode = (PageItem::TextFlowMode) obj->attribute("TEXTFLOWMODE", "0").toInt();
	else if ( obj->attribute("TEXTFLOW").toInt() )
	{
		if (obj->attribute("TEXTFLOW2", "0").toInt())
			OB->TextflowMode = PageItem::TextFlowUsesBoundingBox;
		else if (obj->attribute("TEXTFLOW3", "0").toInt())
			OB->TextflowMode = PageItem::TextFlowUsesContourLine;
		else
			OB->TextflowMode = PageItem::TextFlowUsesFrameShape;	
	}
	else
		OB->TextflowMode = PageItem::TextFlowDisabled;
	OB->Extra=obj->attribute("EXTRA").toDouble();
	OB->TExtra=obj->attribute("TEXTRA", "1").toDouble();
	OB->BExtra=obj->attribute("BEXTRA", "1").toDouble();
	OB->RExtra=obj->attribute("REXTRA", "1").toDouble();
	OB->PoShow = obj->attribute("PLTSHOW", "0").toInt();
	OB->BaseOffs = obj->attribute("BASEOF", "0").toDouble();
	OB->textPathType =  obj->attribute("textPathType", "0").toInt();
	OB->textPathFlipped = static_cast<bool>(obj->attribute("textPathFlipped", "0").toInt());
	OB->ISize = qRound(obj->attribute("ISIZE", "12").toDouble() * 10);
	if (obj->hasAttribute("EXTRAV"))
		OB->ExtraV = qRound(obj->attribute("EXTRAV", "0").toDouble() / obj->attribute("ISIZE", "12").toDouble() * 1000.0);
	else
		OB->ExtraV = obj->attribute("TXTKERN").toInt();
	OB->Pfile  = Relative2Path(obj->attribute("PFILE" ,""), baseDir);
	OB->Pfile2 = Relative2Path(obj->attribute("PFILE2",""), baseDir);
	OB->Pfile3 = Relative2Path(obj->attribute("PFILE3",""), baseDir);
	OB->IProfile=obj->attribute("PRFILE","");
	OB->EmProfile=obj->attribute("EPROF","");
	OB->IRender = obj->attribute("IRENDER", "1").toInt();
	OB->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
	OB->Locked = static_cast<bool>(obj->attribute("LOCK", "0").toInt());
	OB->LockRes = static_cast<bool>(obj->attribute("LOCKR", "0").toInt());
	OB->Reverse = static_cast<bool>(obj->attribute("REVERS", "0").toInt());
	OB->isTableItem = static_cast<bool>(obj->attribute("isTableItem", "0").toInt());
	OB->TopLine = static_cast<bool>(obj->attribute("TopLine", "0").toInt());
	OB->LeftLine = static_cast<bool>(obj->attribute("LeftLine", "0").toInt());
	OB->RightLine = static_cast<bool>(obj->attribute("RightLine", "0").toInt());
	OB->BottomLine = static_cast<bool>(obj->attribute("BottomLine", "0").toInt());
	OB->TopLinkID =  obj->attribute("TopLINK", "-1").toInt();
	OB->LeftLinkID =  obj->attribute("LeftLINK", "-1").toInt();
	OB->RightLinkID =  obj->attribute("RightLINK", "-1").toInt();
	OB->BottomLinkID =  obj->attribute("BottomLINK", "-1").toInt();
	OB->Transparency = obj->attribute("TransValue", "0.0").toDouble();
	if (obj->hasAttribute("TransValueS"))
		OB->TranspStroke = obj->attribute("TransValueS", "0.0").toDouble();
	else
		OB->TranspStroke = OB->Transparency;
	OB->TransBlend = obj->attribute("TransBlend", "0").toInt();
	OB->TransBlendS = obj->attribute("TransBlendS", "0").toInt();
	tmp = "";
	if (obj->hasAttribute("NUMCLIP"))
	{
		OB->Clip.resize(obj->attribute("NUMCLIP").toUInt());
		tmp = obj->attribute("CLIPCOOR");
		QTextStream fc(&tmp, QIODevice::ReadOnly);
		for (uint c=0; c<obj->attribute("NUMCLIP").toUInt(); ++c)
		{
			fc >> x;
			fc >> y;
			OB->Clip.setPoint(c, x, y);
		}
	}
	else
		OB->Clip.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMPO"))
	{
		OB->PoLine.resize(obj->attribute("NUMPO").toUInt());
		tmp = obj->attribute("POCOOR");
		QTextStream fp(&tmp, QIODevice::ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMPO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			OB->PoLine.setPoint(cx, xf, yf);
		}
	}
	else
		OB->PoLine.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMCO"))
	{
		OB->ContourLine.resize(obj->attribute("NUMCO").toUInt());
		tmp = obj->attribute("COCOOR");
		QTextStream fp(&tmp, QIODevice::ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMCO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			OB->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		OB->ContourLine.resize(0);
	tmp = "";
	if ((obj->hasAttribute("NUMTAB")) && (obj->attribute("NUMTAB", "0").toInt() != 0))
	{
		ParagraphStyle::TabRecord tb;
		tmp = obj->attribute("TABS");
		QTextStream tgv(&tmp, QIODevice::ReadOnly);
		OB->TabValues.clear();
		for (int cxv = 0; cxv < obj->attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			OB->TabValues.append(tb);
		}
		tmp = "";
	}
	else
		OB->TabValues.clear();
	if ((obj->hasAttribute("NUMDASH")) && (obj->attribute("NUMDASH", "0").toInt() != 0))
	{
		tmp = obj->attribute("DASHS");
		QTextStream dgv(&tmp, QIODevice::ReadOnly);
		OB->DashValues.clear();
		for (int cxv = 0; cxv < obj->attribute("NUMDASH", "0").toInt(); ++cxv)
		{
			dgv >> xf;
			OB->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		OB->DashValues.clear();
	OB->DashOffset = obj->attribute("DASHOFF", "0.0").toDouble();
}

void Scribus12Format::GetItemText(QDomElement *it, ScribusDoc *doc, bool VorLFound, bool impo, PageItem* obj, LastStyles* last)
{
	QString tmp2, tmf, tmpf, tmp3;
	tmp2 = it->attribute("CH");
	tmp2.replace(QRegExp("\r"), QChar(5));
	tmp2.replace(QRegExp("\n"), QChar(5));
	tmp2.replace(QRegExp("\t"), QChar(4));
	tmp2.replace(SpecialChars::OLD_NBHYPHEN, SpecialChars::NBHYPHEN);
	tmp2.replace(SpecialChars::OLD_NBSPACE, SpecialChars::NBSPACE);
	tmpf = it->attribute("CFONT", doc->toolSettings.defFont);
	doc->AllFonts->findFont(tmpf, doc);
	int size = qRound(it->attribute("CSIZE").toDouble() * 10);
	QString fcolor = it->attribute("CCOLOR");
	int shade = it->attribute("CSHADE").toInt();
	handleOldColorShade(doc, fcolor, shade);
	int extra;
	if (it->hasAttribute("CEXTRA"))
		extra = qRound(it->attribute("CEXTRA").toDouble() / it->attribute("CSIZE").toDouble() * 1000.0);
	else
		extra = it->attribute("CKERN").toInt();
	int cstyle = it->attribute("CSTYLE").toInt() & 255;
	int ab = it->attribute("CAB", "0").toInt();
	QString stroke = it->attribute("CSTROKE",CommonStrings::None);
	int shade2 = it->attribute("CSHADE2", "100").toInt();
	handleOldColorShade(doc, stroke, shade2);
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
	for (int cxx=0; cxx<tmp2.length(); ++cxx)
	{
		CharStyle style;
		QChar ch = tmp2.at(cxx);
		if (ch == QChar(5))
			ch = SpecialChars::PARSEP;
		if (ch == QChar(4))
			ch = SpecialChars::TAB;
		style.setFont((*doc->AllFonts)[tmpf]);
		style.setFontSize(size);
		style.setFillColor(fcolor);
		style.setTracking(extra);
		style.setFillShade(shade);
		style.setFeatures(static_cast<StyleFlag>(cstyle).featureList());
		style.setStrokeColor(stroke);
		style.setStrokeShade(shade2);
		style.setScaleH(qMin(qMax(scale, 100), 4000));
		style.setScaleV(qMin(qMax(scalev, 100), 4000));
		style.setBaselineOffset(base);
		style.setShadowXOffset(shX);
		style.setShadowYOffset(shY);
		style.setOutlineWidth(outL);
		style.setUnderlineOffset(ulp);
		style.setUnderlineWidth(ulw);
		style.setStrikethruOffset(stp);
		style.setStrikethruWidth(stw);
		int pos = obj->itemText.length();
		obj->itemText.insertChars(pos, QString(ch));

		if (style != last->Style) {
			//	qDebug() << QString("new style at %1: %2 -> %3").arg(pos).arg(last->Style.asString()).arg(newStyle.asString());
			obj->itemText.applyCharStyle(last->StyleStart, pos-last->StyleStart, last->Style);
			last->Style = style;
			last->StyleStart = pos;
		}

		if (ch == SpecialChars::PARSEP || cxx+1 == tmp2.length()) {
//			qDebug() << QString("scribus12 para: %1 %2 %3 %4").arg(ab)
//				   .arg(ab < signed(DoVorl.size())? DoVorl[ab] : QString("./."))
//				   .arg(VorLFound).arg(DoVorl.size());
			ParagraphStyle pstyle;
			if (ab < 5) {
				pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(ab));
			}
			else if (VorLFound) {
				pstyle.setParent( DoVorl[ab] );
			}
			obj->itemText.applyStyle(pos, pstyle); 
		}
	}
	obj->itemText.applyCharStyle(last->StyleStart, obj->itemText.length()-last->StyleStart, last->Style);
	return;
}

bool Scribus12Format::loadPage(const QString & fileName, int pageNumber, bool Mpage, QString /*renamedPageName*/)
{
//	qDebug() << QString("loading page %2 from file '%1' from 1.2.x plugin").arg(fileName).arg(pageNumber);
	if (m_Doc==0 || m_View==0 || m_AvailableFonts==0)
	{
		Q_ASSERT(m_Doc==0 || m_View==0 || m_AvailableFonts==0);
		return false;
	}

	struct CopyPasteBuffer OB;
	ParagraphStyle vg;
	struct ScribusDoc::BookMa bok;
	PageItem *Neu;
	itemRemap.clear();
	itemNext.clear();
	QString tmV, tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QMap<int,int> TableID;
	QList<PageItem*> TableItems;
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
		maxLayer = qMax(m_Doc->Layers[la2].LNr, maxLayer);
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
		return false;
	if(!docu.setContent(f))
		return false;
	QString fileDir = QFileInfo(fileName).absolutePath();
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
				m_Doc->PageColors.insert(pg.attribute("NAME"), lf);
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
				la.isViewable = pg.attribute("SICHTBAR").toInt();
				la.isPrintable = pg.attribute("DRUCKEN").toInt();
				la.flowControl = true;
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
					sl.Width = MuL.attribute("Width").toDouble();
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
						Nam2 = tr("Copy #%1 of ").arg(copyC)+Nam;
						copyC++;
					}
				}
				m_Doc->MLineStyles.insert(Nam2, ml);
			}
			if ((pg.tagName()=="PAGE") && (pg.attribute("NUM").toInt() == pageNumber))
			{
				a = m_Doc->currentPage()->pageNr();
				if (Mpage)
				{
					if (pg.attribute("NAM", "").isEmpty())
					{
						PAGE=PAGE.nextSibling();
						continue;
					}
				}
				else
				{
					if (!pg.attribute("NAM", "").isEmpty())
					{
						PAGE=PAGE.nextSibling();
						continue;
					}
				}
/*				if ((pg.attribute("NAM", "").isEmpty()) && (Mpage))
				{
					PAGE=PAGE.nextSibling();
					continue;
				} */
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
				GuideManagerIO::readVerticalGuides(pg.attribute("VerticalGuides"),
						m_Doc->Pages->at(a),
						GuideManagerCore::Standard,
						pg.hasAttribute("NumVGuides"));
				GuideManagerIO::readHorizontalGuides(pg.attribute("HorizontalGuides"),
						m_Doc->Pages->at(a),
						GuideManagerCore::Standard,
						pg.hasAttribute("NumHGuides"));
				m_Doc->Pages->at(a)->marginPreset = pg.attribute("PRESET", "0").toInt();

				QDomNode OBJ=PAGE.firstChild();
				counter = m_Doc->Items->count();
				baseobj = counter;
				int pageItem = 0;
				while(!OBJ.isNull())
				{
					QDomElement obj=OBJ.toElement();
				/*
				* Attribute von OBJECT auslesen
				*/
					itemRemap[scribus12itemID(pageItem++, pageNumber)] = m_Doc->Items->count();
					if (obj.tagName()=="PAGEOBJECT")
					{
						// member of linked chain?
						if ((obj.attribute("NEXTITEM").toInt() != -1) && (obj.attribute("NEXTPAGE").toInt() == pageNumber))
						{
							itemNext[m_Doc->Items->count()] = scribus12itemID(obj.attribute("NEXTITEM").toInt(), pageNumber);
						}
					}
					GetItemProps(&obj, &OB, fileDir, newVersion);
					OB.Xpos = obj.attribute("XPOS").toDouble()+m_Doc->Pages->at(a)->xOffset();
					OB.Ypos=obj.attribute("YPOS").toDouble()+m_Doc->Pages->at(a)->yOffset();
					OB.NamedLStyle = obj.attribute("NAMEDLST", "");
					if (!m_Doc->MLineStyles.contains(OB.NamedLStyle))
						OB.NamedLStyle = "";
					OB.startArrowIndex =  0;
					OB.endArrowIndex =  0;
					OB.isBookmark=obj.attribute("BOOKMARK").toInt();
					if ((OB.isBookmark) && (m_Doc->BookMarks.count() == 0))
						m_Doc->OldBM = true;
					OB.textAlignment = obj.attribute("ALIGN", "0").toInt();
					tmpf = obj.attribute("IFONT", m_Doc->toolSettings.defFont);
					if (tmpf.isEmpty())
						tmpf = m_Doc->toolSettings.defFont;
					m_AvailableFonts->findFont(tmpf, m_Doc);
					OB.IFont = tmpf;
					OB.LayerNr = layerTrans[obj.attribute("LAYER", "0").toInt()];
					OB.Language = obj.attribute("LANGUAGE", m_Doc->Language);
					tmp = "";
					if ((obj.hasAttribute("GROUPS")) && (obj.attribute("NUMGROUP", "0").toInt() != 0))
					{
						tmp = obj.attribute("GROUPS");
						QTextStream fg(&tmp, QIODevice::ReadOnly);
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
							handleOldColorShade(m_Doc, name, shade);
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
					LastStyles* last = new LastStyles();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="ITEXT")
							GetItemText(&it, m_Doc, VorLFound, true, Neu, last);
						IT=IT.nextSibling();
					}
					delete last;

// 					if (obj.attribute("NEXTPAGE").toInt() == pageNumber)
// 					{
//						Neu->NextIt = baseobj + obj.attribute("NEXTITEM").toInt();
// 						nextPg[Neu->ItemNr] = a; // obj.attribute("NEXTPAGE").toInt();
// 					}
//					else
//						Neu->NextIt = -1;
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
				// reestablish textframe links
				if (itemNext.count() != 0)
				{
					QMap<int,long long>::Iterator lc;
					for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
					{
						PageItem *Its = m_Doc->Items->at(lc.key());
						PageItem *Itn = m_Doc->Items->at(itemRemap[lc.value()]);
						assert(Its && Its->asTextFrame());
						assert(Itn && Itn->asTextFrame());
						if (Itn->prevInChain() || Its->nextInChain()) 
						{
							qDebug("scribus12format: corruption in linked textframes detected");
							continue;
						}
						Its->link(Itn);
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

void Scribus12Format::GetStyle(QDomElement *pg, ParagraphStyle *vg, StyleSet<ParagraphStyle> * tempParagraphStyles, ScribusDoc* doc, bool fl)
{
	bool fou;
	int fShade, sShade;
	QString fColor, sColor;
	QString tmpf, tmf, tmV;
	double xf, xf2;
	fou = false;
	const StyleSet<ParagraphStyle> & docParagraphStyles(tempParagraphStyles? *tempParagraphStyles : doc->paragraphStyles());
	vg->setName(pg->attribute("NAME"));
	vg->setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(pg->attribute("LINESPMode", "0").toInt()));
	vg->setLineSpacing(pg->attribute("LINESP").toDouble());
	vg->setLeftMargin(pg->attribute("INDENT", "0").toDouble());
	vg->setFirstIndent(pg->attribute("FIRST", "0").toDouble());
	vg->setAlignment(static_cast<ParagraphStyle::AlignmentType>(pg->attribute("ALIGN").toInt()));
	vg->setGapBefore(pg->attribute("VOR", "0").toDouble());
	vg->setGapAfter(pg->attribute("NACH", "0").toDouble());
	tmpf = pg->attribute("FONT", doc->toolSettings.defFont);
	if (tmpf.isEmpty())
		tmpf = doc->toolSettings.defFont;
	PrefsManager *prefsManager=PrefsManager::instance();
	prefsManager->appPrefs.AvailFonts.findFont(tmpf, doc);
	vg->charStyle().setFont(prefsManager->appPrefs.AvailFonts[tmpf]);
	vg->charStyle().setFontSize(qRound(pg->attribute("FONTSIZE", "12").toDouble() * 10.0));
	vg->setHasDropCap(static_cast<bool>(pg->attribute("DROP", "0").toInt()));
	vg->setDropCapLines(pg->attribute("DROPLIN", "2").toInt());
	vg->setDropCapOffset(pg->attribute("DROPDIST", "0").toDouble());
	vg->charStyle().setFeatures(static_cast<StyleFlag>((pg->attribute("EFFECT", "0").toInt())).featureList());
	fColor = pg->attribute("FCOLOR", doc->toolSettings.dBrush);
	fShade = pg->attribute("FSHADE", "100").toInt();
	handleOldColorShade(doc, fColor, fShade);
	sColor = pg->attribute("SCOLOR", doc->toolSettings.dPen);
	sShade = pg->attribute("SSHADE", "100").toInt();
	handleOldColorShade(doc, sColor, sShade);
	vg->charStyle().setFillColor(fColor);
	vg->charStyle().setFillShade(fShade);
	vg->charStyle().setStrokeColor(sColor);
	vg->charStyle().setStrokeShade(sShade);
	if (static_cast<bool>(pg->attribute("BASE", "0").toInt()))
		vg->setLineSpacingMode(ParagraphStyle::BaselineGridLineSpacing);
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
//	vg->tabValues().clear();
	if ((pg->hasAttribute("NUMTAB")) && (pg->attribute("NUMTAB", "0").toInt() != 0))
	{
		QList<ParagraphStyle::TabRecord> tbs;
		ParagraphStyle::TabRecord tb;
		QString tmp = pg->attribute("TABS");
		QTextStream tgv(&tmp, QIODevice::ReadOnly);
		for (int cxv = 0; cxv < pg->attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			tbs.append(tb);
		}
		vg->setTabValues(tbs);
		tmp = "";
	}
	else
	{
		QList<ParagraphStyle::TabRecord> tbs;
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
				tbs.append(tb);
			}
			vg->setTabValues(tbs);
			IT=IT.nextSibling();
		}
	}
	for (int xx=0; xx<docParagraphStyles.count(); ++xx)
	{
		if (vg->name() == docParagraphStyles[xx].name())
		{
			//Compare the attributes of the pasted styles vs existing ones
			if (vg->equiv(docParagraphStyles[xx]))
			{
				if (fl)
				{
					DoVorl[VorlC] = docParagraphStyles[xx].name();
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
		for (int xx=0; xx< docParagraphStyles.count(); ++xx)
		{
			if (vg->equiv(docParagraphStyles[xx]))
			{
				vg->setName(docParagraphStyles[xx].name());
				fou = true;
				if (fl)
				{
					DoVorl[VorlC] = docParagraphStyles[xx].name();
					VorlC++;
				}
				break;
			}
		}
	}
	if (!fou)
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
			VorlC++;
		}
	}
}
/*
QString Scribus12Format::AskForFont(SCFonts &avail, QString fStr, ScribusDoc *doc)
{
	PrefsManager *prefsManager=PrefsManager::instance();
//	QFont fo;
	QString tmpf = fStr;
	if ((!avail.contains(tmpf)) || (!avail[tmpf].usable()))
	{
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!avail[prefsManager->appPrefs.GFontSub[tmpf]].usable()))
		{
			qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
			MissingFont *dia = new MissingFont(0, tmpf, doc);
			dia->exec();
			tmpf = dia->getReplacementFont();
			delete dia;
			qApp->setOverrideCursor(QCursor(Qt::WaitCursor), true);
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
*/
bool Scribus12Format::readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles)
{
	ParagraphStyle vg;
	QDomDocument docu("scridoc");
	QString tmpf, tmf;
// 	DoFonts.clear();
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
				GetStyle(&pg, &vg, &docParagraphStyles, doc, false);
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
				colors.insert(pg.attribute("NAME"), lf);
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
