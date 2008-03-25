/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus134format.h"
#include "scribus134formatimpl.h"

#include <memory>

#include "../../formatidlist.h"
#include "commonstrings.h"
#include "missing.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "scpattern.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "hyphenator.h"
#include "pageitem_latexframe.h"

#include "units.h"
#include "util.h"
#include "util_math.h"
#include "util_color.h"
#include "scgzfile.h"
#include "scpattern.h"
#include <QCursor>
#include <QFileInfo>
#include <QList>
#include <QDataStream>
#include <QXmlStreamWriter>

class ScXmlStreamWriter : public QXmlStreamWriter
{
public:
	ScXmlStreamWriter(void) : QXmlStreamWriter() {}
	ScXmlStreamWriter(QIODevice* device) : QXmlStreamWriter(device) {}
	void writeAttribute(const QString & name, const QString & value) { QXmlStreamWriter::writeAttribute(name, value); }
	void writeAttribute(const QString & name, int value)    { QXmlStreamWriter::writeAttribute(name, QString::number(value)); }
	void writeAttribute(const QString & name, uint value)   { QXmlStreamWriter::writeAttribute(name, QString::number(value)); }
	void writeAttribute(const QString & name, double value) { QXmlStreamWriter::writeAttribute(name, QString::number(value)); }
};

bool Scribus134Format::saveFile(const QString & fileName, const FileFormat & /* fmt */)
{
	QString text, tf, tf2, tc, tc2;
	QString fileDir = QFileInfo(fileName).absolutePath();
	m_lastSavedFile = "";

	// Create a random temporary file name
	srand(time(NULL)); // initialize random sequence each time
	long     randt = 0, randn = 1 + (int) (((double) rand() / ((double) RAND_MAX + 1)) * 10000);
	QString  tmpFileName  = QString("%1.%2").arg(fileName).arg(randn);
	while (QFile::exists(tmpFileName) && (randt < 100))
	{
		randn = 1 + (int) (((double) rand() / ((double) RAND_MAX + 1)) * 10000);
		tmpFileName = QString("%1.%2").arg(fileName).arg(randn);
		++randt;
	}
	if (QFile::exists(tmpFileName))
		return false;

	/*QDomDocument docu("scribus");
	QString st="<SCRIBUSUTF8NEW></SCRIBUSUTF8NEW>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	elem.setAttribute("Version", QString(VERSION));
	QDomElement dc=docu.createElement("DOCUMENT");*/

	std::auto_ptr<QIODevice> outputFile;
	if (fileName.toLower().right(2) == "gz")
		outputFile.reset( new ScGzFile(tmpFileName) );
	else
		outputFile.reset( new QFile(tmpFileName) );

	if (!outputFile->open(QIODevice::WriteOnly))
		return false;

	ScXmlStreamWriter docu;
	docu.setAutoFormatting(true);
	docu.setDevice(outputFile.get());
	docu.writeStartDocument();
	docu.writeStartElement("SCRIBUSUTF8NEW");
	docu.writeAttribute("Version", QString(VERSION));

	docu.writeStartElement("DOCUMENT");
	docu.writeAttribute("ANZPAGES"    , m_Doc->DocPages.count());
	docu.writeAttribute("PAGEWIDTH"   , m_Doc->pageWidth);
	docu.writeAttribute("PAGEHEIGHT"  , m_Doc->pageHeight);
	docu.writeAttribute("BORDERLEFT"  , m_Doc->pageMargins.Left);
	docu.writeAttribute("BORDERRIGHT" , m_Doc->pageMargins.Right);
	docu.writeAttribute("BORDERTOP"   , m_Doc->pageMargins.Top);
	docu.writeAttribute("BORDERBOTTOM", m_Doc->pageMargins.Bottom);
	docu.writeAttribute("BleedTop"    , m_Doc->bleeds.Top);
	docu.writeAttribute("BleedLeft"   , m_Doc->bleeds.Left);
	docu.writeAttribute("BleedRight"  , m_Doc->bleeds.Right);
	docu.writeAttribute("BleedBottom" , m_Doc->bleeds.Bottom);
	docu.writeAttribute("ORIENTATION" , m_Doc->PageOri);
	docu.writeAttribute("PAGESIZE"    , m_Doc->m_pageSize);
	docu.writeAttribute("FIRSTNUM"    , m_Doc->FirstPnum);
	docu.writeAttribute("BOOK"        , m_Doc->currentPageLayout);
	if(m_Doc->usesAutomaticTextFrames())
		docu.writeAttribute("AUTOTEXT", 1);
	docu.writeAttribute("AUTOSPALTEN" ,m_Doc->PageSp);
	docu.writeAttribute("ABSTSPALTEN" ,m_Doc->PageSpa);
	docu.writeAttribute("UNITS"       , m_Doc->unitIndex());
	docu.writeAttribute("DFONT"       ,m_Doc->toolSettings.defFont);
	docu.writeAttribute("DSIZE"       ,m_Doc->toolSettings.defSize / 10.0);
	docu.writeAttribute("DCOL"        ,m_Doc->toolSettings.dCols);
	docu.writeAttribute("DGAP"        ,m_Doc->toolSettings.dGap);
	docu.writeAttribute("TabFill"     ,m_Doc->toolSettings.tabFillChar);
	docu.writeAttribute("TabWidth"    ,m_Doc->toolSettings.dTabWidth);
	docu.writeAttribute("AUTHOR"      ,m_Doc->documentInfo.getAuthor());
	docu.writeAttribute("COMMENTS"    ,m_Doc->documentInfo.getComments());
	docu.writeAttribute("KEYWORDS"    ,m_Doc->documentInfo.getKeywords());
	docu.writeAttribute("PUBLISHER",m_Doc->documentInfo.getPublisher());
	docu.writeAttribute("DOCDATE",m_Doc->documentInfo.getDate());
	docu.writeAttribute("DOCTYPE",m_Doc->documentInfo.getType());
	docu.writeAttribute("DOCFORMAT",m_Doc->documentInfo.getFormat());
	docu.writeAttribute("DOCIDENT",m_Doc->documentInfo.getIdent());
	docu.writeAttribute("DOCSOURCE",m_Doc->documentInfo.getSource());
	docu.writeAttribute("DOCLANGINFO",m_Doc->documentInfo.getLangInfo());
	docu.writeAttribute("DOCRELATION",m_Doc->documentInfo.getRelation());
	docu.writeAttribute("DOCCOVER",m_Doc->documentInfo.getCover());
	docu.writeAttribute("DOCRIGHTS",m_Doc->documentInfo.getRights());
	docu.writeAttribute("DOCCONTRIB",m_Doc->documentInfo.getContrib());
	docu.writeAttribute("TITLE",m_Doc->documentInfo.getTitle());
	docu.writeAttribute("VHOCH"  , m_Doc->typographicSettings.valueSuperScript);
	docu.writeAttribute("VHOCHSC", m_Doc->typographicSettings.scalingSuperScript);
	docu.writeAttribute("VTIEF"  , m_Doc->typographicSettings.valueSubScript);
	docu.writeAttribute("VTIEFSC", m_Doc->typographicSettings.scalingSubScript);
	docu.writeAttribute("VKAPIT" , m_Doc->typographicSettings.valueSmallCaps);
	docu.writeAttribute("BASEGRID",m_Doc->typographicSettings.valueBaseGrid);
	docu.writeAttribute("BASEO"  , m_Doc->typographicSettings.offsetBaseGrid);
	docu.writeAttribute("AUTOL"  , m_Doc->typographicSettings.autoLineSpacing);
	docu.writeAttribute("UnderlinePos"   , m_Doc->typographicSettings.valueUnderlinePos);
	docu.writeAttribute("UnderlineWidth" , m_Doc->typographicSettings.valueUnderlineWidth);
	docu.writeAttribute("StrikeThruPos"  , m_Doc->typographicSettings.valueStrikeThruPos);
	docu.writeAttribute("StrikeThruWidth", m_Doc->typographicSettings.valueStrikeThruWidth);
	docu.writeAttribute("GROUPC",m_Doc->GroupCounter);
	docu.writeAttribute("HCMS" , static_cast<int>(m_Doc->HasCMS));
	docu.writeAttribute("DPSo" , static_cast<int>(m_Doc->CMSSettings.SoftProofOn));
	docu.writeAttribute("DPSFo", static_cast<int>(m_Doc->CMSSettings.SoftProofFullOn));
	docu.writeAttribute("DPuse", static_cast<int>(m_Doc->CMSSettings.CMSinUse));
	docu.writeAttribute("DPgam", static_cast<int>(m_Doc->CMSSettings.GamutCheck));
	docu.writeAttribute("DPbla", static_cast<int>(m_Doc->CMSSettings.BlackPoint));
	docu.writeAttribute("DPMo",m_Doc->CMSSettings.DefaultMonitorProfile);
	docu.writeAttribute("DPPr",m_Doc->CMSSettings.DefaultPrinterProfile);
	docu.writeAttribute("DPIn",m_Doc->CMSSettings.DefaultImageRGBProfile);
	docu.writeAttribute("DPInCMYK",m_Doc->CMSSettings.DefaultImageCMYKProfile);
	docu.writeAttribute("DPIn2",m_Doc->CMSSettings.DefaultSolidColorRGBProfile);
	docu.writeAttribute("DPIn3",m_Doc->CMSSettings.DefaultSolidColorCMYKProfile);
	docu.writeAttribute("DISc",m_Doc->CMSSettings.DefaultIntentColors);
	docu.writeAttribute("DIIm",m_Doc->CMSSettings.DefaultIntentImages);
	docu.writeAttribute("ALAYER", m_Doc->activeLayer());
	docu.writeAttribute("LANGUAGE", m_Doc->Language);
	docu.writeAttribute("MINWORDLEN", m_Doc->MinWordLen);
	docu.writeAttribute("HYCOUNT", m_Doc->HyCount);
	docu.writeAttribute("AUTOMATIC", static_cast<int>(m_Doc->Automatic));
	docu.writeAttribute("AUTOCHECK", static_cast<int>(m_Doc->AutoCheck));
	docu.writeAttribute("GUIDELOCK", static_cast<int>(m_Doc->GuideLock));
	docu.writeAttribute("SnapToGuides", static_cast<int>(m_Doc->SnapGuides));
	docu.writeAttribute("SnapToGrid", static_cast<int>(m_Doc->useRaster));
	docu.writeAttribute("MINGRID", m_Doc->guidesSettings.minorGrid);
	docu.writeAttribute("MAJGRID", m_Doc->guidesSettings.majorGrid);
	docu.writeAttribute("SHOWGRID", static_cast<int>(m_Doc->guidesSettings.gridShown));
	docu.writeAttribute("SHOWGUIDES", static_cast<int>(m_Doc->guidesSettings.guidesShown));
	docu.writeAttribute("showcolborders", static_cast<int>(m_Doc->guidesSettings.colBordersShown));
	docu.writeAttribute("SHOWFRAME", static_cast<int>(m_Doc->guidesSettings.framesShown));
	docu.writeAttribute("SHOWLAYERM", static_cast<int>(m_Doc->guidesSettings.layerMarkersShown));
	docu.writeAttribute("SHOWMARGIN", static_cast<int>(m_Doc->guidesSettings.marginsShown));
	docu.writeAttribute("SHOWBASE", static_cast<int>(m_Doc->guidesSettings.baseShown));
	docu.writeAttribute("SHOWPICT", static_cast<int>(m_Doc->guidesSettings.showPic));
	docu.writeAttribute("SHOWControl", static_cast<int>(m_Doc->guidesSettings.showControls));
	docu.writeAttribute("SHOWLINK", static_cast<int>(m_Doc->guidesSettings.linkShown));
	docu.writeAttribute("rulerMode", static_cast<int>(m_Doc->guidesSettings.rulerMode));
	docu.writeAttribute("showrulers", static_cast<int>(m_Doc->guidesSettings.rulersShown));
	docu.writeAttribute("showBleed", static_cast<int>(m_Doc->guidesSettings.showBleed));
	docu.writeAttribute("rulerXoffset", m_Doc->rulerXoffset);
	docu.writeAttribute("rulerYoffset", m_Doc->rulerYoffset);
	docu.writeAttribute("GuideRad", m_Doc->guidesSettings.guideRad);
	docu.writeAttribute("GRAB",m_Doc->guidesSettings.grabRad);
	docu.writeAttribute("POLYC", m_Doc->toolSettings.polyC);
	docu.writeAttribute("POLYF", m_Doc->toolSettings.polyF);
	docu.writeAttribute("POLYR", m_Doc->toolSettings.polyR);
	docu.writeAttribute("POLYFD", m_Doc->toolSettings.polyFd);
	docu.writeAttribute("POLYS", static_cast<int>(m_Doc->toolSettings.polyS));
	docu.writeAttribute("AutoSave", static_cast<int>(m_Doc->AutoSave));
	docu.writeAttribute("AutoSaveTime", m_Doc->AutoSaveTime);
	docu.writeAttribute("ScratchBottom", m_Doc->scratch.Bottom);
	docu.writeAttribute("ScratchLeft", m_Doc->scratch.Left);
	docu.writeAttribute("ScratchRight", m_Doc->scratch.Right);
	docu.writeAttribute("ScratchTop", m_Doc->scratch.Top);
	docu.writeAttribute("GapHorizontal", m_Doc->GapHorizontal);
	docu.writeAttribute("GapVertical", m_Doc->GapVertical);
	docu.writeAttribute("StartArrow", m_Doc->toolSettings.dStartArrow);
	docu.writeAttribute("EndArrow", m_Doc->toolSettings.dEndArrow);
	docu.writeAttribute("PEN",m_Doc->toolSettings.dPen);
	docu.writeAttribute("BRUSH",m_Doc->toolSettings.dBrush);
	docu.writeAttribute("PENLINE",m_Doc->toolSettings.dPenLine);
	docu.writeAttribute("PENTEXT",m_Doc->toolSettings.dPenText);
	docu.writeAttribute("StrokeText",m_Doc->toolSettings.dStrokeText);
	docu.writeAttribute("TextBackGround", m_Doc->toolSettings.dTextBackGround);
	docu.writeAttribute("TextLineColor", m_Doc->toolSettings.dTextLineColor);
	docu.writeAttribute("TextBackGroundShade", m_Doc->toolSettings.dTextBackGroundShade);
	docu.writeAttribute("TextLineShade", m_Doc->toolSettings.dTextLineShade);
	docu.writeAttribute("TextPenShade", m_Doc->toolSettings.dTextPenShade);
	docu.writeAttribute("TextStrokeShade", m_Doc->toolSettings.dTextStrokeShade);
	docu.writeAttribute("STIL",m_Doc->toolSettings.dLineArt);
	docu.writeAttribute("STILLINE",m_Doc->toolSettings.dLstyleLine);
	docu.writeAttribute("WIDTH",m_Doc->toolSettings.dWidth);
	docu.writeAttribute("WIDTHLINE",m_Doc->toolSettings.dWidthLine);
	docu.writeAttribute("PENSHADE",m_Doc->toolSettings.dShade2);
	docu.writeAttribute("LINESHADE",m_Doc->toolSettings.dShadeLine);
	docu.writeAttribute("BRUSHSHADE",m_Doc->toolSettings.dShade);
	docu.writeAttribute("MAGMIN",m_Doc->toolSettings.magMin);
	docu.writeAttribute("MAGMAX",m_Doc->toolSettings.magMax);
	docu.writeAttribute("MAGSTEP",m_Doc->toolSettings.magStep);
	docu.writeAttribute("CPICT",m_Doc->toolSettings.dBrushPict);
	docu.writeAttribute("PICTSHADE",m_Doc->toolSettings.shadePict);
	docu.writeAttribute("PICTSCX",m_Doc->toolSettings.scaleX);
	docu.writeAttribute("PICTSCY",m_Doc->toolSettings.scaleY);
	docu.writeAttribute("PSCALE", static_cast<int>(m_Doc->toolSettings.scaleType));
	docu.writeAttribute("PASPECT", static_cast<int>(m_Doc->toolSettings.aspectRatio));
	docu.writeAttribute("EmbeddedPath", static_cast<int>(m_Doc->toolSettings.useEmbeddedPath));
	docu.writeAttribute("HalfRes", m_Doc->toolSettings.lowResType);
	docu.writeAttribute("dispX", m_Doc->toolSettings.dispX);
	docu.writeAttribute("dispY", m_Doc->toolSettings.dispY);
	docu.writeAttribute("constrain", m_Doc->toolSettings.constrain);
	docu.writeAttribute("MINORC",m_Doc->guidesSettings.minorColor.name());
	docu.writeAttribute("MAJORC",m_Doc->guidesSettings.majorColor.name());
	docu.writeAttribute("GuideC", m_Doc->guidesSettings.guideColor.name());
	docu.writeAttribute("BaseC", m_Doc->guidesSettings.baseColor.name());
	docu.writeAttribute("GuideZ", m_Doc->guidesSettings.guideRad);
	docu.writeAttribute("BACKG", static_cast<int>(m_Doc->guidesSettings.before));
	docu.writeAttribute("PAGEC",m_Doc->papColor.name());
	docu.writeAttribute("MARGC",m_Doc->guidesSettings.margColor.name());
	docu.writeAttribute("RANDF", static_cast<int>(m_Doc->marginColored));
	docu.writeAttribute("currentProfile", m_Doc->curCheckProfile);

	writeCheckerProfiles(docu);
	writeLinestyles(docu);
	writeJavascripts(docu);
	writeBookmarks(docu);
	writeColors(docu);
	writeHyphenatorLists(docu);
	writePStyles(docu);
	writeCStyles(docu);
	writeLayers(docu);
	writePrintOptions(docu);
	writePdfOptions(docu);
	writeDocItemAttributes(docu);
	writeTOC(docu);
	writePageSets(docu);
	writeSections(docu);
	writePatterns(docu, fileDir);
	writeContent (docu, fileDir);
	
	docu.writeEndElement();
	docu.writeEndDocument();
	
	bool  writeSucceed = false;
	const QFile* qFile = dynamic_cast<QFile*>(outputFile.get());
	const ScGzFile* gzFile = dynamic_cast<ScGzFile*>(outputFile.get());
	if (qFile)
		writeSucceed = (qFile->error() == QFile::NoError);
	else if (gzFile)
		writeSucceed = !gzFile->errorOccurred();
	outputFile->close();

	if (writeSucceed)
	{
		if (QFile::exists(fileName))
			writeSucceed = QFile::remove(fileName) ? QFile::rename(tmpFileName, fileName) : false;
		else
			writeSucceed = QFile::rename(tmpFileName, fileName);
		m_lastSavedFile = writeSucceed ? fileName : tmpFileName;
	}
	else if (QFile::exists(tmpFileName))
		QFile::remove(tmpFileName);
	if (writeSucceed) 
		QFile::remove(tmpFileName);
	return writeSucceed;
}

void Scribus134Format::writeCheckerProfiles(ScXmlStreamWriter & docu) 
{
	CheckerPrefsList::Iterator itcp;
	CheckerPrefsList::Iterator itcpend=m_Doc->checkerProfiles.end();
	for (itcp = m_Doc->checkerProfiles.begin(); itcp != itcpend; ++itcp)
	{
		docu.writeEmptyElement("CheckProfile");
		docu.writeAttribute("Name",itcp.key());
		docu.writeAttribute("ignoreErrors", static_cast<int>(itcp.value().ignoreErrors));
		docu.writeAttribute("autoCheck", static_cast<int>(itcp.value().autoCheck));
		docu.writeAttribute("checkGlyphs", static_cast<int>(itcp.value().checkGlyphs));
		docu.writeAttribute("checkOrphans", static_cast<int>(itcp.value().checkOrphans));
		docu.writeAttribute("checkOverflow", static_cast<int>(itcp.value().checkOverflow));
		docu.writeAttribute("checkPictures", static_cast<int>(itcp.value().checkPictures));
		docu.writeAttribute("checkResolution", static_cast<int>(itcp.value().checkResolution));
		docu.writeAttribute("checkTransparency", static_cast<int>(itcp.value().checkTransparency));
		docu.writeAttribute("minResolution",itcp.value().minResolution);
		docu.writeAttribute("maxResolution",itcp.value().maxResolution);
		docu.writeAttribute("checkAnnotations", static_cast<int>(itcp.value().checkAnnotations));
		docu.writeAttribute("checkRasterPDF", static_cast<int>(itcp.value().checkRasterPDF));
		docu.writeAttribute("checkForGIF", static_cast<int>(itcp.value().checkForGIF));
		docu.writeAttribute("ignoreOffLayers", static_cast<int>(itcp.value().ignoreOffLayers));
		//docu.writeEndElement();
	}
}

void Scribus134Format::writeLinestyles(ScXmlStreamWriter& docu) 
{
	QMap<QString,multiLine>::Iterator itMU;
	for (itMU = m_Doc->MLineStyles.begin(); itMU != m_Doc->MLineStyles.end(); ++itMU)
	{
		docu.writeStartElement("MultiLine");
		docu.writeAttribute("Name",itMU.key());
		multiLine ml = itMU.value();
		multiLine::iterator itMU2;
		for (itMU2 = ml.begin(); itMU2 != ml.end(); ++itMU2)
		{
			docu.writeEmptyElement("SubLine");
			docu.writeAttribute("Color", (*itMU2).Color);
			docu.writeAttribute("Shade", (*itMU2).Shade);
			docu.writeAttribute("Dash", (*itMU2).Dash);
			docu.writeAttribute("LineEnd", (*itMU2).LineEnd);
			docu.writeAttribute("LineJoin", (*itMU2).LineJoin);
			docu.writeAttribute("Width", (*itMU2).Width);
			docu.writeAttribute("Shortcut", ml.shortcut);
		}
		docu.writeEndElement();
	}
	QList<ArrowDesc>::Iterator itar;
	for (itar = m_Doc->arrowStyles.begin(); itar != m_Doc->arrowStyles.end(); ++itar)
	{
		if ((*itar).userArrow)
		{
			docu.writeEmptyElement("Arrows");
			docu.writeAttribute("NumPoints", (*itar).points.size());
			QString arp = "";
			QString tmp, tmpy;
			double xa, ya;
			for (uint nxx = 0; nxx < (*itar).points.size(); ++nxx)
			{
				(*itar).points.point(nxx, &xa, &ya);
				arp += tmp.setNum(xa) + " " + tmpy.setNum(ya) + " ";
			}
			docu.writeAttribute("Points", arp);
			docu.writeAttribute("Name", (*itar).name);
		}
	}
}


void Scribus134Format::writeJavascripts(ScXmlStreamWriter & docu) 
{
	QMap<QString,QString>::Iterator itja;
	for (itja = m_Doc->JavaScripts.begin(); itja != m_Doc->JavaScripts.end(); ++itja)
	{
		docu.writeEmptyElement("JAVA");
		docu.writeAttribute("NAME",itja.key());
		docu.writeAttribute("SCRIPT",itja.value());
	}
	
}


void Scribus134Format::writeBookmarks(ScXmlStreamWriter & docu) 
{	
	QList<ScribusDoc::BookMa>::Iterator itbm;
	for (itbm = m_Doc->BookMarks.begin(); itbm != m_Doc->BookMarks.end(); ++itbm)
	{
		docu.writeEmptyElement("Bookmark");
		docu.writeAttribute("Title",(*itbm).Title);
		docu.writeAttribute("Text",(*itbm).Text);
		docu.writeAttribute("Aktion",(*itbm).Aktion);
		docu.writeAttribute("ItemNr", (*itbm).ItemNr);
		docu.writeAttribute("Element", (*itbm).PageObject->ItemNr);
		docu.writeAttribute("First", (*itbm).First);
		docu.writeAttribute("Last", (*itbm).Last);
		docu.writeAttribute("Prev", (*itbm).Prev);
		docu.writeAttribute("Next", (*itbm).Next);
		docu.writeAttribute("Parent", (*itbm).Parent);
	}
}


void Scribus134Format::writeColors(ScXmlStreamWriter & docu) 
{	
	ColorList::Iterator itc;
	for (itc = m_Doc->PageColors.begin(); itc != m_Doc->PageColors.end(); ++itc)
	{
		docu.writeEmptyElement("COLOR");
		docu.writeAttribute("NAME",itc.key());
		if (m_Doc->PageColors[itc.key()].getColorModel() == colorModelRGB)
			docu.writeAttribute("RGB",m_Doc->PageColors[itc.key()].nameRGB());
		else
			docu.writeAttribute("CMYK",m_Doc->PageColors[itc.key()].nameCMYK());
		docu.writeAttribute("Spot",static_cast<int>(m_Doc->PageColors[itc.key()].isSpotColor()));
		docu.writeAttribute("Register",static_cast<int>(m_Doc->PageColors[itc.key()].isRegistrationColor()));
	}
	
}

void Scribus134Format::writeHyphenatorLists(ScXmlStreamWriter &docu)
{
	docu.writeStartElement("HYPHEN");
	for (QHash<QString, QString>::Iterator hyit = m_Doc->docHyphenator->specialWords.begin(); hyit != m_Doc->docHyphenator->specialWords.end(); ++hyit)
	{
		docu.writeEmptyElement("EXCEPTION");
		docu.writeAttribute("WORD", hyit.key());
		docu.writeAttribute("HYPHENATED", hyit.value());
	}
	for (QSet<QString>::Iterator hyit2 = m_Doc->docHyphenator->ignoredWords.begin(); hyit2 != m_Doc->docHyphenator->ignoredWords.end(); ++hyit2)
	{
		docu.writeEmptyElement("IGNORE");
		docu.writeAttribute("WORD", (*hyit2));
	}
	docu.writeEndElement();
}

void Scribus134Format::writePStyles(ScXmlStreamWriter & docu) 
{
	//	if (m_Doc->docParagraphStyles.count() > 5)
	{
		for (int ff = 0; ff < m_Doc->paragraphStyles().count(); ++ff)
			putPStyle(docu, m_Doc->paragraphStyles()[ff], "STYLE");
	}
}

void Scribus134Format::putPStyle(ScXmlStreamWriter & docu, const ParagraphStyle & style, const QString &nodeName)
{
	bool styleHasTabs = (!style.isInhTabValues() && (style.tabValues().count() > 0));
	if (styleHasTabs)
		docu.writeStartElement(nodeName);
	else
		docu.writeEmptyElement(nodeName);
	if ( ! style.name().isEmpty() )
		docu.writeAttribute("NAME", style.name());
	if ( ! style.parent().isEmpty())
		docu.writeAttribute("PARENT", style.parent());
	
	if ( ! style.isInhAlignment())
		docu.writeAttribute("ALIGN", style.alignment());
	if ( ! style.isInhLineSpacingMode())
		docu.writeAttribute("LINESPMode", style.lineSpacingMode());
	if ( ! style.isInhLineSpacing())
		docu.writeAttribute("LINESP", style.lineSpacing());
	if ( ! style.isInhLeftMargin())
		docu.writeAttribute("INDENT", style.leftMargin());
	if ( ! style.isInhRightMargin())
		docu.writeAttribute("RMARGIN", style.rightMargin());
	if ( ! style.isInhFirstIndent())
		docu.writeAttribute("FIRST", style.firstIndent());
	if ( ! style.isInhGapBefore())
		docu.writeAttribute("VOR", style.gapBefore());
	if ( ! style.isInhGapAfter())
		docu.writeAttribute("NACH", style.gapAfter());
	if ( ! style.isInhHasDropCap())
		docu.writeAttribute("DROP", static_cast<int>(style.hasDropCap()));
	if ( ! style.isInhDropCapLines())
		docu.writeAttribute("DROPLIN", style.dropCapLines());
	if ( ! style.isInhDropCapOffset())
		docu.writeAttribute("DROPDIST", style.dropCapOffset());
	if ( ! style.isInhOpticalMargins())
		docu.writeAttribute("OpticalMargins", style.opticalMargins());
	if ( ! style.isInhHyphenationMode())
		docu.writeAttribute("HyphenationMode", style.hyphenationMode());
	if ( ! style.isInhMinWordTracking())
		docu.writeAttribute("MinWordTrack", style.minWordTracking());
	if ( ! style.isInhMinGlyphExtension())
		docu.writeAttribute("MinGlyphShrink", style.minGlyphExtension());
	if ( ! style.isInhMaxGlyphExtension())
		docu.writeAttribute("MaxGlyphExtend", style.maxGlyphExtension());

	if ( ! style.shortcut().isEmpty() )
		docu.writeAttribute("PSHORTCUT", style.shortcut()); // shortcuts won't be inherited

	putCStyle(docu, style.charStyle());

	if ( ! style.isInhTabValues())
	{
		for (int a = 0; a < style.tabValues().count(); ++a)
		{
			docu.writeEmptyElement("Tabs");
			docu.writeAttribute("Type", (style.tabValues().at(a)).tabType);
			docu.writeAttribute("Pos", (style.tabValues().at(a)).tabPosition);
			QString tabCh = "";
			if (!(style.tabValues().at(a)).tabFillChar.isNull())
				tabCh = QString((style.tabValues().at(a)).tabFillChar);
			docu.writeAttribute("Fill", tabCh);
		}
	}

	if (styleHasTabs)
		docu.writeEndElement();
}


void Scribus134Format::writeCStyles(ScXmlStreamWriter & docu) 
{
	for (int ff = 0; ff < m_Doc->charStyles().count(); ++ff)
	{
		docu.writeStartElement("CHARSTYLE");
		putCStyle(docu, m_Doc->charStyles()[ff]);
		docu.writeEndElement();
	}
}

void Scribus134Format::putCStylePT(ScXmlStreamWriter & docu, const CharStyle & style)
{
	docu.writeAttribute("CNAME", style.name());
	docu.writeAttribute("CPARENT", style.parent());
	docu.writeAttribute("FONT", style.font().scName());
	docu.writeAttribute("FONTSIZE", style.fontSize() / 10.0);
	docu.writeAttribute("FEATURES", style.features().join(" "));
	docu.writeAttribute("FCOLOR", style.fillColor());
	docu.writeAttribute("FSHADE", style.fillShade());
	docu.writeAttribute("SCOLOR", style.strokeColor());
	docu.writeAttribute("SSHADE", style.strokeShade());
	docu.writeAttribute("TXTSHX", style.shadowXOffset() / 10.0);
	docu.writeAttribute("TXTSHY", style.shadowYOffset() / 10.0);
	docu.writeAttribute("TXTOUT", style.outlineWidth() / 10.0);
	docu.writeAttribute("TXTULP", style.underlineOffset() / 10.0);
	docu.writeAttribute("TXTULW", style.underlineWidth() / 10.0);
	docu.writeAttribute("TXTSTP", style.strikethruOffset() / 10.0);
	docu.writeAttribute("TXTSTW", style.strikethruWidth() / 10.0);
	docu.writeAttribute("SCALEH", style.scaleH() / 10.0);
	docu.writeAttribute("SCALEV", style.scaleV() / 10.0);
	docu.writeAttribute("BASEO", style.baselineOffset() / 10.0);
	docu.writeAttribute("KERN", style.tracking() / 10.0);
	docu.writeAttribute("wordTrack", style.wordTracking());
	docu.writeAttribute("SHORTCUT", style.shortcut()); // shortcuts won't be inherited
}

void Scribus134Format::putCStyle(ScXmlStreamWriter & docu, const CharStyle & style)
{
	if ( ! style.name().isEmpty() )
		docu.writeAttribute("CNAME", style.name());
	if ( ! style.parent().isEmpty() )
		docu.writeAttribute("CPARENT", style.parent());
	if ( ! style.isInhFont())	
		docu.writeAttribute("FONT", style.font().scName());
	if ( ! style.isInhFontSize())
		docu.writeAttribute("FONTSIZE", style.fontSize() / 10.0);
	if ( ! style.isInhFeatures())
		docu.writeAttribute("FEATURES", style.features().join(" "));
	if ( ! style.isInhFillColor())
		docu.writeAttribute("FCOLOR", style.fillColor());
	if ( ! style.isInhFillShade())
		docu.writeAttribute("FSHADE", style.fillShade());
	if ( ! style.isInhStrokeColor())
		docu.writeAttribute("SCOLOR", style.strokeColor());
	if ( ! style.isInhStrokeShade())
		docu.writeAttribute("SSHADE", style.strokeShade());
	if ( ! style.isInhShadowXOffset())
		docu.writeAttribute("TXTSHX", style.shadowXOffset() / 10.0);
	if ( ! style.isInhShadowYOffset())
		docu.writeAttribute("TXTSHY", style.shadowYOffset() / 10.0);
	if ( ! style.isInhOutlineWidth())
		docu.writeAttribute("TXTOUT", style.outlineWidth() / 10.0);
	if ( ! style.isInhUnderlineOffset())
		docu.writeAttribute("TXTULP", style.underlineOffset() / 10.0);
	if ( ! style.isInhUnderlineWidth())
		docu.writeAttribute("TXTULW", style.underlineWidth() / 10.0);
	if ( ! style.isInhStrikethruOffset())
		docu.writeAttribute("TXTSTP", style.strikethruOffset() / 10.0);
	if ( ! style.isInhStrikethruWidth())
		docu.writeAttribute("TXTSTW", style.strikethruWidth() / 10.0);
	if ( ! style.isInhScaleH())
		docu.writeAttribute("SCALEH", style.scaleH() / 10.0);
	if ( ! style.isInhScaleV())
		docu.writeAttribute("SCALEV", style.scaleV() / 10.0);
	if ( ! style.isInhBaselineOffset())
		docu.writeAttribute("BASEO", style.baselineOffset() / 10.0);
	if ( ! style.isInhTracking())
		docu.writeAttribute("KERN", style.tracking() / 10.0); 
	if ( ! style.isInhWordTracking())
		docu.writeAttribute("wordTrack", style.wordTracking());
	if ( ! style.shortcut().isEmpty() )
		docu.writeAttribute("SHORTCUT", style.shortcut()); // shortcuts won't be inherited
}

void Scribus134Format::writeLayers(ScXmlStreamWriter & docu) 
{	
	uint layerCount=m_Doc->layerCount();
	for (uint lay = 0; lay < layerCount; ++lay)
	{
		docu.writeEmptyElement("LAYERS");
		docu.writeAttribute("NUMMER",m_Doc->Layers[lay].LNr);
		docu.writeAttribute("LEVEL",m_Doc->Layers[lay].Level);
		docu.writeAttribute("NAME",m_Doc->Layers[lay].Name);
		docu.writeAttribute("SICHTBAR", static_cast<int>(m_Doc->Layers[lay].isViewable));
		docu.writeAttribute("DRUCKEN", static_cast<int>(m_Doc->Layers[lay].isPrintable));
		docu.writeAttribute("EDIT", static_cast<int>(m_Doc->Layers[lay].isEditable));
		docu.writeAttribute("FLOW", static_cast<int>(m_Doc->Layers[lay].flowControl));
		docu.writeAttribute("TRANS", m_Doc->Layers[lay].transparency);
		docu.writeAttribute("BLEND", m_Doc->Layers[lay].blendMode);
		docu.writeAttribute("OUTL", static_cast<int>(m_Doc->Layers[lay].outlineMode));
		docu.writeAttribute("LAYERC",m_Doc->Layers[lay].markerColor.name());
	}
}

void Scribus134Format::writePrintOptions(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("Printer");
	docu.writeAttribute("firstUse", static_cast<int>(m_Doc->Print_Options.firstUse));
	docu.writeAttribute("toFile", static_cast<int>(m_Doc->Print_Options.toFile));
	docu.writeAttribute("useAltPrintCommand", static_cast<int>(m_Doc->Print_Options.useAltPrintCommand));
	docu.writeAttribute("outputSeparations", static_cast<int>(m_Doc->Print_Options.outputSeparations));
	docu.writeAttribute("useSpotColors", static_cast<int>(m_Doc->Print_Options.useSpotColors));
	docu.writeAttribute("useColor", static_cast<int>(m_Doc->Print_Options.useColor));
	docu.writeAttribute("mirrorH", static_cast<int>(m_Doc->Print_Options.mirrorH));
	docu.writeAttribute("mirrorV", static_cast<int>(m_Doc->Print_Options.mirrorV));
	docu.writeAttribute("useICC", static_cast<int>(m_Doc->Print_Options.useICC));
	docu.writeAttribute("doGCR", static_cast<int>(m_Doc->Print_Options.doGCR));
	docu.writeAttribute("doClip", static_cast<int>(m_Doc->Print_Options.doClip));
	docu.writeAttribute("setDevParam", static_cast<int>(m_Doc->Print_Options.setDevParam));
	docu.writeAttribute("doOverprint", static_cast<int>(m_Doc->Print_Options.doOverprint));
	docu.writeAttribute("cropMarks", static_cast<int>(m_Doc->Print_Options.cropMarks));
	docu.writeAttribute("bleedMarks", static_cast<int>(m_Doc->Print_Options.bleedMarks));
	docu.writeAttribute("registrationMarks", static_cast<int>(m_Doc->Print_Options.registrationMarks));
	docu.writeAttribute("colorMarks", static_cast<int>(m_Doc->Print_Options.colorMarks));
	docu.writeAttribute("PSLevel", (m_Doc->Print_Options.prnEngine < WindowsGDI) ? m_Doc->Print_Options.prnEngine : PostScript3);
	docu.writeAttribute("PDLanguage" , m_Doc->Print_Options.prnEngine);
	docu.writeAttribute("markOffset" , m_Doc->Print_Options.markOffset);
	docu.writeAttribute("BleedTop"   , m_Doc->Print_Options.bleeds.Top);
	docu.writeAttribute("BleedLeft"  , m_Doc->Print_Options.bleeds.Left);
	docu.writeAttribute("BleedRight" , m_Doc->Print_Options.bleeds.Right);
	docu.writeAttribute("BleedBottom", m_Doc->Print_Options.bleeds.Bottom);
	docu.writeAttribute("printer" , m_Doc->Print_Options.printer);
	docu.writeAttribute("filename", m_Doc->Print_Options.filename);
	docu.writeAttribute("separationName", m_Doc->Print_Options.separationName);
	docu.writeAttribute("printerCommand", m_Doc->Print_Options.printerCommand);
	for (int p = 0; p < m_Doc->Print_Options.allSeparations.count(); ++p)
	{
		docu.writeEmptyElement("Separation");
		docu.writeAttribute("Name", m_Doc->Print_Options.allSeparations[p]);
	}
	docu.writeEndElement();
}

void Scribus134Format::writePdfOptions(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("PDF");
	docu.writeAttribute("firstUse", static_cast<int>(m_Doc->PDF_Options.firstUse));
	docu.writeAttribute("Thumbnails", static_cast<int>(m_Doc->PDF_Options.Thumbnails));
	docu.writeAttribute("Articles", static_cast<int>(m_Doc->PDF_Options.Articles));
	docu.writeAttribute("Bookmarks", static_cast<int>(m_Doc->PDF_Options.Bookmarks));
	docu.writeAttribute("Compress", static_cast<int>(m_Doc->PDF_Options.Compress));
	docu.writeAttribute("CMethod", m_Doc->PDF_Options.CompressMethod);
	docu.writeAttribute("Quality", m_Doc->PDF_Options.Quality);
	docu.writeAttribute("MirrorH", static_cast<int>(m_Doc->PDF_Options.MirrorH));
	docu.writeAttribute("MirrorV", static_cast<int>(m_Doc->PDF_Options.MirrorV));
	docu.writeAttribute("Clip", static_cast<int>(m_Doc->PDF_Options.doClip));
	docu.writeAttribute("RotateDeg", static_cast<int>(m_Doc->PDF_Options.RotateDeg));
	docu.writeAttribute("PresentMode", static_cast<int>(m_Doc->PDF_Options.PresentMode));
	docu.writeAttribute("RecalcPic", static_cast<int>(m_Doc->PDF_Options.RecalcPic));
	docu.writeAttribute("Grayscale", static_cast<int>(m_Doc->PDF_Options.isGrayscale));
	docu.writeAttribute("RGBMode", static_cast<int>(m_Doc->PDF_Options.UseRGB));
	docu.writeAttribute("UseProfiles", static_cast<int>(m_Doc->PDF_Options.UseProfiles));
	docu.writeAttribute("UseProfiles2", static_cast<int>(m_Doc->PDF_Options.UseProfiles2));
	docu.writeAttribute("Binding", m_Doc->PDF_Options.Binding);
	docu.writeAttribute("PicRes", m_Doc->PDF_Options.PicRes);
	docu.writeAttribute("Resolution", m_Doc->PDF_Options.Resolution);
	docu.writeAttribute("Version", m_Doc->PDF_Options.Version);
	docu.writeAttribute("Intent", m_Doc->PDF_Options.Intent);
	docu.writeAttribute("Intent2", m_Doc->PDF_Options.Intent2);
	docu.writeAttribute("SolidP", m_Doc->PDF_Options.SolidProf);
	docu.writeAttribute("ImageP", m_Doc->PDF_Options.ImageProf);
	docu.writeAttribute("PrintP", m_Doc->PDF_Options.PrintProf);
	docu.writeAttribute("InfoString", m_Doc->PDF_Options.Info);
	docu.writeAttribute("BTop", m_Doc->PDF_Options.bleeds.Top);
	docu.writeAttribute("BLeft", m_Doc->PDF_Options.bleeds.Left);
	docu.writeAttribute("BRight", m_Doc->PDF_Options.bleeds.Right);
	docu.writeAttribute("BBottom", m_Doc->PDF_Options.bleeds.Bottom);
	docu.writeAttribute("useDocBleeds", static_cast<int>(m_Doc->PDF_Options.useDocBleeds));
	docu.writeAttribute("cropMarks", static_cast<int>(m_Doc->PDF_Options.cropMarks));
	docu.writeAttribute("bleedMarks", static_cast<int>(m_Doc->PDF_Options.bleedMarks));
	docu.writeAttribute("registrationMarks", static_cast<int>(m_Doc->PDF_Options.registrationMarks));
	docu.writeAttribute("colorMarks", static_cast<int>(m_Doc->PDF_Options.colorMarks));
	docu.writeAttribute("docInfoMarks", static_cast<int>(m_Doc->PDF_Options.docInfoMarks));
	docu.writeAttribute("markOffset", m_Doc->PDF_Options.markOffset);
	docu.writeAttribute("ImagePr", static_cast<int>(m_Doc->PDF_Options.EmbeddedI));
	docu.writeAttribute("PassOwner", m_Doc->PDF_Options.PassOwner);
	docu.writeAttribute("PassUser", m_Doc->PDF_Options.PassUser);
	docu.writeAttribute("Permissions", m_Doc->PDF_Options.Permissions);
	docu.writeAttribute("Encrypt", static_cast<int>(m_Doc->PDF_Options.Encrypt));
	docu.writeAttribute("UseLayers", static_cast<int>(m_Doc->PDF_Options.useLayers));
	docu.writeAttribute("UseLpi", static_cast<int>(m_Doc->PDF_Options.UseLPI));
	docu.writeAttribute("UseSpotColors", static_cast<int>(m_Doc->PDF_Options.UseSpotColors));
	docu.writeAttribute("doOverprint", static_cast<int>(m_Doc->PDF_Options.doOverprint));
	docu.writeAttribute("doMultiFile", static_cast<int>(m_Doc->PDF_Options.doMultiFile));
	docu.writeAttribute("displayBookmarks", static_cast<int>(m_Doc->PDF_Options.displayBookmarks));
	docu.writeAttribute("displayFullscreen", static_cast<int>(m_Doc->PDF_Options.displayFullscreen));
	docu.writeAttribute("displayLayers", static_cast<int>(m_Doc->PDF_Options.displayLayers));
	docu.writeAttribute("displayThumbs", static_cast<int>(m_Doc->PDF_Options.displayThumbs));
	docu.writeAttribute("hideMenuBar", static_cast<int>(m_Doc->PDF_Options.hideMenuBar));
	docu.writeAttribute("hideToolBar", static_cast<int>(m_Doc->PDF_Options.hideToolBar));
	docu.writeAttribute("fitWindow", static_cast<int>(m_Doc->PDF_Options.fitWindow));
	docu.writeAttribute("PageLayout", m_Doc->PDF_Options.PageLayout);
	docu.writeAttribute("openAction", m_Doc->PDF_Options.openAction);
	for (int pdoF = 0; pdoF < m_Doc->PDF_Options.EmbedList.count(); ++pdoF)
	{
		docu.writeEmptyElement("Fonts");
		docu.writeAttribute("Name", m_Doc->PDF_Options.EmbedList[pdoF]);
	}
	for (int pdoS = 0; pdoS < m_Doc->PDF_Options.SubsetList.count(); ++pdoS)
	{
		docu.writeEmptyElement("Subset");
		docu.writeAttribute("Name", m_Doc->PDF_Options.SubsetList[pdoS]);
	}
	for (int pdoE = 0; pdoE < m_Doc->PDF_Options.PresentVals.count(); ++pdoE)
	{
		docu.writeEmptyElement("Effekte");
		docu.writeAttribute("pageEffectDuration", m_Doc->PDF_Options.PresentVals[pdoE].pageEffectDuration);
		docu.writeAttribute("pageViewDuration", m_Doc->PDF_Options.PresentVals[pdoE].pageViewDuration);
		docu.writeAttribute("effectType", m_Doc->PDF_Options.PresentVals[pdoE].effectType);
		docu.writeAttribute("Dm", m_Doc->PDF_Options.PresentVals[pdoE].Dm);
		docu.writeAttribute("M", m_Doc->PDF_Options.PresentVals[pdoE].M);
		docu.writeAttribute("Di", m_Doc->PDF_Options.PresentVals[pdoE].Di);
	}
	QMap<QString,LPIData>::Iterator itlp;
	for (itlp = m_Doc->PDF_Options.LPISettings.begin(); itlp != m_Doc->PDF_Options.LPISettings.end(); ++itlp)
	{
		docu.writeEmptyElement("LPI");
		docu.writeAttribute("Color", itlp.key());
		docu.writeAttribute("Frequency", itlp.value().Frequency);
		docu.writeAttribute("Angle", itlp.value().Angle);
		docu.writeAttribute("SpotFunction", itlp.value().SpotFunc);
	}
	docu.writeEndElement();
}

void Scribus134Format::writeDocItemAttributes(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("DocItemAttributes");
	for(ObjAttrVector::Iterator objAttrIt = m_Doc->docItemAttributes.begin() ; objAttrIt != m_Doc->docItemAttributes.end(); ++objAttrIt )
	{
		docu.writeEmptyElement("ItemAttribute");
		docu.writeAttribute("Name", (*objAttrIt).name);
		docu.writeAttribute("Type", (*objAttrIt).type);
		docu.writeAttribute("Value", (*objAttrIt).value);
		docu.writeAttribute("Parameter", (*objAttrIt).parameter);
		docu.writeAttribute("Relationship", (*objAttrIt).relationship);
		docu.writeAttribute("RelationshipTo", (*objAttrIt).relationshipto);
		docu.writeAttribute("AutoAddTo", (*objAttrIt).autoaddto);
	}
	docu.writeEndElement();
}


void Scribus134Format::writeTOC(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("TablesOfContents");
	for(ToCSetupVector::Iterator tocSetupIt = m_Doc->docToCSetups.begin() ; tocSetupIt != m_Doc->docToCSetups.end(); ++tocSetupIt )
	{
		docu.writeEmptyElement("TableOfContents");
		docu.writeAttribute("Name", (*tocSetupIt).name);
		docu.writeAttribute("ItemAttributeName", (*tocSetupIt).itemAttrName);
		docu.writeAttribute("FrameName", (*tocSetupIt).frameName);
		docu.writeAttribute("ListNonPrinting", (*tocSetupIt).listNonPrintingFrames);
		docu.writeAttribute("Style", (*tocSetupIt).textStyle);
		switch ((*tocSetupIt).pageLocation)
		{
			case Beginning:
				docu.writeAttribute("NumberPlacement", "Beginning");
				break;
			case End:
				docu.writeAttribute("NumberPlacement", "End");
				break;
			case NotShown:
				docu.writeAttribute("NumberPlacement", "NotShown");
				break;
		}
	}
	docu.writeEndElement();
}


void Scribus134Format::writeSections(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("Sections");
	for(DocumentSectionMap::Iterator it = m_Doc->sections.begin() ; it != m_Doc->sections.end(); ++it )
	{
		docu.writeEmptyElement("Section");
		docu.writeAttribute("Number", (*it).number);
		docu.writeAttribute("Name", (*it).name);
		docu.writeAttribute("From", (*it).fromindex);
		docu.writeAttribute("To", (*it).toindex);
		switch ((*it).type)
		{
			case Type_1_2_3:
				docu.writeAttribute("Type", "Type_1_2_3");
				break;
			case Type_i_ii_iii:
				docu.writeAttribute("Type", "Type_i_ii_iii");
				break;
			case Type_I_II_III:
				docu.writeAttribute("Type", "Type_I_II_III");
				break;
			case Type_a_b_c:
				docu.writeAttribute("Type", "Type_a_b_c");
				break;
			case Type_A_B_C:
				docu.writeAttribute("Type", "Type_A_B_C");
				break;
			case Type_None:
				docu.writeAttribute("Type", "Type_None");
				break;
		}
		docu.writeAttribute("Start", (*it).sectionstartindex);
		docu.writeAttribute("Reversed", (*it).reversed);
		docu.writeAttribute("Active", (*it).active);
	}
	docu.writeEndElement();
}


void Scribus134Format::writePageSets(ScXmlStreamWriter & docu) 
{	
	docu.writeStartElement("PageSets");
	QList<PageSet>::Iterator itpgset;
	for(itpgset = m_Doc->pageSets.begin(); itpgset != m_Doc->pageSets.end(); ++itpgset )
	{
		docu.writeStartElement("Set");
		docu.writeAttribute("Name", (*itpgset).Name);
		docu.writeAttribute("FirstPage", (*itpgset).FirstPage);
		docu.writeAttribute("Rows", (*itpgset).Rows);
		docu.writeAttribute("Columns", (*itpgset).Columns);
//		docu.writeAttribute("GapHorizontal", (*itpgset).GapHorizontal);
//		docu.writeAttribute("GapVertical", (*itpgset).GapVertical);
//		docu.writeAttribute("GapBelow", (*itpgset).GapBelow);
		QStringList pNames = (*itpgset).pageNames;
		QStringList::Iterator itpgsetN;
		for(itpgsetN = pNames.begin(); itpgsetN != pNames.end(); ++itpgsetN )
		{
			docu.writeEmptyElement("PageNames");
			docu.writeAttribute("Name", (*itpgsetN));
		}
		docu.writeEndElement();
	}
	docu.writeEndElement();
}

void Scribus134Format::writePatterns(ScXmlStreamWriter & docu, const QString& baseDir) 
{
	QMap<QString, ScPattern>::Iterator itPat;
	for (itPat = m_Doc->docPatterns.begin(); itPat != m_Doc->docPatterns.end(); ++itPat)
	{
		docu.writeStartElement("Pattern");
		docu.writeAttribute("Name",itPat.key());
		ScPattern pa = itPat.value();
		docu.writeAttribute("width", pa.width);
		docu.writeAttribute("height", pa.height);
		docu.writeAttribute("scaleX", pa.scaleX);
		docu.writeAttribute("scaleY", pa.scaleY);
		WriteObjects(m_Doc, docu, baseDir, 0, 0, ItemSelectionPattern, &pa.items);
		docu.writeEndElement();
	}	
}

void Scribus134Format::writeContent(ScXmlStreamWriter & docu, const QString& baseDir) 
{
	if (m_mwProgressBar != 0)
	{
		m_mwProgressBar->setMaximum(m_Doc->DocPages.count()+m_Doc->MasterPages.count()+m_Doc->DocItems.count()+m_Doc->MasterItems.count()+m_Doc->FrameItems.count());
		m_mwProgressBar->setValue(0);
	}
	WritePages(m_Doc, docu, m_mwProgressBar, 0, true);
	WritePages(m_Doc, docu, m_mwProgressBar, m_Doc->MasterPages.count(), false);
	WriteObjects(m_Doc, docu, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count(), ItemSelectionFrame);
	WriteObjects(m_Doc, docu, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count()+m_Doc->FrameItems.count(), ItemSelectionMaster);
	WriteObjects(m_Doc, docu, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count()+m_Doc->MasterItems.count()+m_Doc->FrameItems.count(), ItemSelectionPage);
}

void Scribus134Format::WritePages(ScribusDoc *doc, ScXmlStreamWriter& docu, QProgressBar *dia2, uint maxC, bool master)
{
	uint ObCount = maxC;
	Page *page;
	uint pages;
	QDomElement pg;
	QString tmp;
	if (master)
		pages = doc->MasterPages.count();
	else
		pages = doc->DocPages.count();
	for(uint i = 0; i < pages; ++i)
	{
		ObCount++;
		if (dia2 != 0)
			dia2->setValue(ObCount);
		if (master)
		{
			docu.writeStartElement("MASTERPAGE");
			page = doc->MasterPages.at(i);
		}
		else
		{
			docu.writeStartElement("PAGE");
			page = doc->DocPages.at(i);
		}
		docu.writeAttribute("PAGEXPOS",page->xOffset());
		docu.writeAttribute("PAGEYPOS",page->yOffset());
		docu.writeAttribute("PAGEWIDTH",page->width());
		docu.writeAttribute("PAGEHEIGHT",page->height());
		docu.writeAttribute("BORDERLEFT",page->initialMargins.Left);
		docu.writeAttribute("BORDERRIGHT",page->initialMargins.Right);
		docu.writeAttribute("BORDERTOP",page->initialMargins.Top);
		docu.writeAttribute("BORDERBOTTOM",page->initialMargins.Bottom);
		docu.writeAttribute("NUM",page->pageNr());
		docu.writeAttribute("NAM",page->pageName());
		docu.writeAttribute("MNAM",page->MPageNam);
		docu.writeAttribute("Size", page->m_pageSize);
		docu.writeAttribute("Orientation", page->PageOri);
		docu.writeAttribute("LEFT", page->LeftPg);
		docu.writeAttribute("VerticalGuides", GuideManagerIO::writeVerticalGuides(
													page, GuideManagerCore::Standard));
		docu.writeAttribute("HorizontalGuides", GuideManagerIO::writeHorizontalGuides(
													page, GuideManagerCore::Standard));
		docu.writeAttribute("AGhorizontalAutoGap", page->guides.horizontalAutoGap());
		docu.writeAttribute("AGverticalAutoGap", page->guides.verticalAutoGap());
		docu.writeAttribute("AGhorizontalAutoCount", page->guides.horizontalAutoCount());
		docu.writeAttribute("AGverticalAutoCount", page->guides.verticalAutoCount());
		docu.writeAttribute("AGhorizontalAutoRefer", page->guides.horizontalAutoRefer());
		docu.writeAttribute("AGverticalAutoRefer", page->guides.verticalAutoRefer());
		docu.writeAttribute("AGSelection", GuideManagerIO::writeSelection(page));
		docu.writeEndElement();
	}
}


namespace { // anon
	QString textWithSmartHyphens(StoryText& itemText, int from, int to)
	{
		QString result("");
		int lastPos = from;
		for (int i = from; i < to; ++i)
		{
			if (itemText.charStyle(i).effects() & ScStyle_HyphenationPossible 
				// duplicate SHYPHEN if already present to indicate a user provided SHYPHEN:
				|| itemText.text(i) == SpecialChars::SHYPHEN)
			{
				result += itemText.text(lastPos, i + 1 - lastPos);
				result += SpecialChars::SHYPHEN;
				lastPos = i+1;
			}
		}
		if (lastPos < to)
			result += itemText.text(lastPos, to - lastPos);
		return result;
	}
} // namespace anon


void Scribus134Format::writeITEXTs(ScribusDoc *doc, ScXmlStreamWriter &docu, PageItem* item)
{
	CharStyle lastStyle;
	int lastPos = 0;
	QString tmpnum;
	for(int k = 0; k < item->itemText.length(); ++k)
	{
		const CharStyle& style1(item->itemText.charStyle(k));
		const QChar ch = item->itemText.text(k);
		
		if (ch == SpecialChars::OBJECT ||
			ch == SpecialChars::TAB ||
			ch == SpecialChars::PARSEP ||
			ch == SpecialChars::LINEBREAK ||
			ch == SpecialChars::COLBREAK ||
			ch == SpecialChars::FRAMEBREAK ||
			ch == SpecialChars::PAGENUMBER ||
			ch == SpecialChars::PAGECOUNT ||
			ch.unicode() < 32 || 
			(0xd800 <= ch.unicode() && ch.unicode() < 0xe000) ||
			ch.unicode() == 0xfffe || ch.unicode() == 0xffff ||
			style1 != lastStyle)
		{
			// something new, write pending chars
			if  (k - lastPos > 0)
			{
				docu.writeEmptyElement("ITEXT");
				if (item->asPathText())
					putCStylePT(docu, lastStyle);
				else
					putCStyle(docu, lastStyle);
				docu.writeAttribute("CH", textWithSmartHyphens(item->itemText, lastPos, k));
			}
			lastStyle = style1;
			lastPos = k;
		}

		if (ch == SpecialChars::OBJECT && item->itemText.object(k) != NULL) 
		{
			// each obj in its own ITEXT for now
			docu.writeEmptyElement("ITEXT");
			if (item->asPathText())
				putCStylePT(docu, lastStyle);
			else
				putCStyle(docu, lastStyle);
			docu.writeAttribute("CH", QString(ch));
			docu.writeAttribute("COBJ", item->itemText.object(k)->ItemNr);		
		}
		else if (ch == SpecialChars::PARSEP)
		{
			// stores also the paragraphstyle for preceding chars
			putPStyle(docu, item->itemText.paragraphStyle(k), "para");
		}
		else if (ch == SpecialChars::TAB) 
		{
			//ob.appendChild(docu->createElement("tab"));
			docu.writeEmptyElement("tab");
		}
		else if (ch == SpecialChars::LINEBREAK)
		{
			//ob.appendChild(docu->createElement("breakline"));
			docu.writeEmptyElement("breakline");
		}
		else if (ch == SpecialChars::COLBREAK)
		{
			//ob.appendChild(docu->createElement("breakcol"));
			docu.writeEmptyElement("breakcol");
		}
		else if (ch == SpecialChars::FRAMEBREAK)
		{
			//ob.appendChild(docu->createElement("breakframe"));
			docu.writeEmptyElement("breakframe");
		}
		else if (ch == SpecialChars::PAGENUMBER) 
		{
			docu.writeEmptyElement("var");
			docu.writeAttribute("name", "pgno");
			putCStyle(docu, lastStyle);
		}
		else if (ch == SpecialChars::PAGECOUNT) 
		{
			docu.writeEmptyElement("var");
			docu.writeAttribute("name", "pgco");
			putCStyle(docu, lastStyle);
		}
		else if (ch.unicode() < 32 || 
				 (0xd800 <= ch.unicode() && ch.unicode() < 0xe000) ||
				 ch.unicode() == 0xfffe || ch.unicode() == 0xffff)
		{
			docu.writeEmptyElement("ITEXT");
			putCStyle(docu, lastStyle);
			tmpnum.setNum(ch.unicode());
			docu.writeAttribute("Unicode", tmpnum);		
		}
		else
		{
			continue;
		}
		// otherwise we just wrote something special and are done with position 'k'
		lastPos = k + 1;
	}
	// write pending chars
	if ( item->itemText.length() - lastPos > 0)
	{
		docu.writeEmptyElement("ITEXT");
		if (item->asPathText())
			putCStylePT(docu, lastStyle);
		else
			putCStyle(docu, lastStyle);
		docu.writeAttribute("CH", textWithSmartHyphens(item->itemText, lastPos, item->itemText.length()));
	}
	// paragraphstyle for trailing chars
	if (item->itemText.length() == 0 || item->itemText.text(item->itemText.length()-1) != SpecialChars::PARSEP)
	{
		putPStyle(docu, item->itemText.paragraphStyle(item->itemText.length()), "para");
	}
}

void Scribus134Format::WriteObjects(ScribusDoc *doc, ScXmlStreamWriter& docu, const QString& baseDir, QProgressBar *dia2, uint maxC, ItemSelection master, QList<PageItem*> *some_items)
{
	uint ObCount = maxC;
	QList<PageItem*> *items = NULL;
	PageItem *item = NULL;
	QDomElement ob;
	uint objects = 0;
	switch (master)
	{
		case ItemSelectionMaster:
			items = &doc->MasterItems;
			break;
		case ItemSelectionPage:
			items = &doc->DocItems;
			break;
		case ItemSelectionFrame:
			items = &doc->FrameItems;
			break;
		case ItemSelectionPattern:
			items = some_items;
			break;
		default:
			assert(false);
	}
	objects = items->count();
	for(uint j = 0; j < objects;++j)
	{
		ObCount++;
		if (dia2 != 0)
			dia2->setValue(ObCount);
		item = items->at(j);
		switch (master)
		{
			case ItemSelectionMaster:
//				item = doc->MasterItems.at(j);
				docu.writeStartElement("MASTEROBJECT");
				break;
			case ItemSelectionPage:
//				item = doc->DocItems.at(j);
				docu.writeStartElement("PAGEOBJECT");
				break;
			case ItemSelectionFrame:
//				item = doc->FrameItems.at(j);
				docu.writeStartElement("FRAMEOBJECT");
				break;
			case ItemSelectionPattern:
				docu.writeStartElement("PatternItem");
				break;
		}
		SetItemProps(docu, item, baseDir, true);
		docu.writeAttribute("OnMasterPage", item->OnMasterPage);
		docu.writeAttribute("ImageClip", item->pixm.imgInfo.usedPath);
		docu.writeAttribute("ImageRes", item->pixm.imgInfo.lowResType);
		docu.writeAttribute("isInline", static_cast<int>(item->isEmbedded));
		docu.writeAttribute("fillRule", static_cast<int>(item->fillRule));
		docu.writeAttribute("doOverprint", static_cast<int>(item->doOverprint));
		docu.writeAttribute("gXpos", item->gXpos);
		docu.writeAttribute("gYpos", item->gYpos);
		docu.writeAttribute("gWidth", item->gWidth);
		docu.writeAttribute("gHeight", item->gHeight);
		if (item->GrType != 0)
		{
			if (item->GrType == 8)
			{
				docu.writeAttribute("pattern", item->pattern());
				double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
				item->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
				docu.writeAttribute("pScaleX", patternScaleX);
				docu.writeAttribute("pScaleY", patternScaleY);
				docu.writeAttribute("pOffsetX", patternOffsetX);
				docu.writeAttribute("pOffsetY", patternOffsetY);
				docu.writeAttribute("pRotation", patternRotation);
			}
			else
			{
				docu.writeAttribute("GRSTARTX", item->GrStartX);
				docu.writeAttribute("GRSTARTY", item->GrStartY);
				docu.writeAttribute("GRENDX", item->GrEndX);
				docu.writeAttribute("GRENDY", item->GrEndY);
			}
		}
		if (item->itemText.defaultStyle().hasParent())
			docu.writeAttribute("PSTYLE", item->itemText.defaultStyle().parent());
		if (! item->itemText.defaultStyle().isInhAlignment())
			docu.writeAttribute("ALIGN", item->itemText.defaultStyle().alignment());
		
		docu.writeAttribute("LAYER", item->LayerNr);
		docu.writeAttribute("BOOKMARK", item->isBookmark ? 1 : 0);

		if (item->nextInChain() != 0)
			docu.writeAttribute("NEXTITEM", item->nextInChain()->ItemNr);
		else
			docu.writeAttribute("NEXTITEM", -1);
		
		if (item->prevInChain() != 0 && items->contains(item->prevInChain()))
			docu.writeAttribute("BACKITEM", item->prevInChain()->ItemNr);
		else
		{
			docu.writeAttribute("BACKITEM", -1);
			writeITEXTs(doc, docu, item); 	
		}

		if (item->effectsInUse.count() != 0)
		{
			for (int a = 0; a < item->effectsInUse.count(); ++a)
			{
				docu.writeEmptyElement("ImageEffect");
				docu.writeAttribute("Code", item->effectsInUse.at(a).effectCode);
				docu.writeAttribute("Param", item->effectsInUse.at(a).effectParameters);
			}
		}
		if (((item->asImageFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()) && (item->pixm.imgInfo.layerInfo.count() != 0) && (item->pixm.imgInfo.isRequest))
		{
			QMap<int, ImageLoadRequest>::iterator it2;
			for (it2 = item->pixm.imgInfo.RequestProps.begin(); it2 != item->pixm.imgInfo.RequestProps.end(); ++it2)
			{
				docu.writeEmptyElement("PSDLayer");
				docu.writeAttribute("Layer",it2.key());
				docu.writeAttribute("Visible", static_cast<int>(it2.value().visible));
				docu.writeAttribute("useMask", static_cast<int>(it2.value().useMask));
				docu.writeAttribute("Opacity", it2.value().opacity);
				docu.writeAttribute("Blend", it2.value().blend);
			}
		}
		if (item->itemText.defaultStyle().tabValues().count() != 0)
		{
			for (int a = 0; a < item->itemText.defaultStyle().tabValues().count(); ++a)
			{
				docu.writeEmptyElement("Tabs");
				docu.writeAttribute("Type", (item->itemText.defaultStyle().tabValues().at(a)).tabType);
				docu.writeAttribute("Pos", (item->itemText.defaultStyle().tabValues().at(a)).tabPosition);
				QString tabCh = "";
				if (!(item->itemText.defaultStyle().tabValues().at(a)).tabFillChar.isNull())
					tabCh = QString((item->itemText.defaultStyle().tabValues().at(a)).tabFillChar);
				docu.writeAttribute("Fill", tabCh);
			}
		}
		if ((item->GrType > 0) && (item->GrType != 8))
		{
			QList<VColorStop*> cstops = item->fill_gradient.colorStops();
			for (uint cst = 0; cst < item->fill_gradient.Stops(); ++cst)
			{
				docu.writeEmptyElement("CSTOP");
				docu.writeAttribute("RAMP", cstops.at(cst)->rampPoint);
				docu.writeAttribute("NAME", cstops.at(cst)->name);
				docu.writeAttribute("SHADE", cstops.at(cst)->shade);
				docu.writeAttribute("TRANS", cstops.at(cst)->opacity);
			}
		}
		
		if (item->asLatexFrame())
		{
			docu.writeStartElement("LATEX");
			PageItem_LatexFrame *latexitem = item->asLatexFrame();
			docu.writeAttribute("ConfigFile", latexitem->configFile());
			docu.writeAttribute("DPI", latexitem->dpi());
			docu.writeAttribute("USE_PREAMBLE", latexitem->usePreamble());
			QMapIterator<QString, QString> i(latexitem->editorProperties);
			while (i.hasNext())
			{
				i.next();
				docu.writeStartElement("PROPERTY");
				docu.writeAttribute("name", i.key());
				docu.writeAttribute("value", i.value());
				docu.writeEndElement();
			}
			docu.writeCharacters(latexitem->formula());
			docu.writeEndElement();
			/*QDomText latextext = docu->createTextNode(latexitem->formula());
			latexinfo.appendChild(latextext);
			ob.appendChild(latexinfo);*/
		}

		//CB PageItemAttributes
		docu.writeStartElement("PageItemAttributes");
		ObjAttrVector *attributes=item->getObjectAttributes();
		for(ObjAttrVector::Iterator objAttrIt = attributes->begin() ; objAttrIt != attributes->end(); ++objAttrIt )
		{
			docu.writeEmptyElement("ItemAttribute");
			docu.writeAttribute("Name", (*objAttrIt).name);
			docu.writeAttribute("Type", (*objAttrIt).type);
			docu.writeAttribute("Value", (*objAttrIt).value);
			docu.writeAttribute("Parameter", (*objAttrIt).parameter);
			docu.writeAttribute("Relationship", (*objAttrIt).relationship);
			docu.writeAttribute("RelationshipTo", (*objAttrIt).relationshipto);
			docu.writeAttribute("AutoAddTo", (*objAttrIt).autoaddto);
		}
		docu.writeEndElement();

		docu.writeEndElement();
	}
}

void Scribus134Format::SetItemProps(ScXmlStreamWriter& docu, PageItem* item, const QString& baseDir, bool newFormat)
{
	double xf, yf;
	QString tmp, tmpy;
	if (newFormat)
		docu.writeAttribute("OwnPage", item->OwnPage);
	docu.writeAttribute("PTYPE",item->realItemType());
	docu.writeAttribute("XPOS",item->xPos());
	docu.writeAttribute("YPOS",item->yPos());
	docu.writeAttribute("WIDTH",item->width());
	docu.writeAttribute("HEIGHT",item->height());
	docu.writeAttribute("RADRECT",item->cornerRadius());
	docu.writeAttribute("FRTYPE", item->FrameType);
	docu.writeAttribute("CLIPEDIT", item->ClipEdited ? 1 : 0);
	docu.writeAttribute("PWIDTH",item->lineWidth());
	docu.writeAttribute("PCOLOR",item->fillColor());
	docu.writeAttribute("PCOLOR2",item->lineColor());
	if ( ! item->itemText.defaultStyle().charStyle().isInhFillColor())
		docu.writeAttribute("TXTFILL",item->itemText.defaultStyle().charStyle().fillColor());
	if ( ! item->itemText.defaultStyle().charStyle().isInhStrokeColor())
		docu.writeAttribute("TXTSTROKE",item->itemText.defaultStyle().charStyle().strokeColor());
	if ( ! item->itemText.defaultStyle().charStyle().isInhStrokeShade())
		docu.writeAttribute("TXTSTRSH",item->itemText.defaultStyle().charStyle().strokeShade());
	if ( ! item->itemText.defaultStyle().charStyle().isInhFillShade())
		docu.writeAttribute("TXTFILLSH",item->itemText.defaultStyle().charStyle().fillShade());
	if ( ! item->itemText.defaultStyle().charStyle().isInhScaleH())
		docu.writeAttribute("TXTSCALE",item->itemText.defaultStyle().charStyle().scaleH() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhScaleV())
		docu.writeAttribute("TXTSCALEV",item->itemText.defaultStyle().charStyle().scaleV() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhBaselineOffset())
		docu.writeAttribute("TXTBASE",item->itemText.defaultStyle().charStyle().baselineOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhShadowXOffset())
		docu.writeAttribute("TXTSHX",item->itemText.defaultStyle().charStyle().shadowXOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhShadowYOffset())
		docu.writeAttribute("TXTSHY",item->itemText.defaultStyle().charStyle().shadowYOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhOutlineWidth())
		docu.writeAttribute("TXTOUT",item->itemText.defaultStyle().charStyle().outlineWidth() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhUnderlineOffset())
		docu.writeAttribute("TXTULP",item->itemText.defaultStyle().charStyle().underlineOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhUnderlineWidth())
		docu.writeAttribute("TXTULW",item->itemText.defaultStyle().charStyle().underlineWidth() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhStrikethruOffset())
		docu.writeAttribute("TXTSTP",item->itemText.defaultStyle().charStyle().strikethruOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhStrikethruWidth())
		docu.writeAttribute("TXTSTW",item->itemText.defaultStyle().charStyle().strikethruWidth() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhTracking())
		docu.writeAttribute("TXTKERN",item->itemText.defaultStyle().charStyle().tracking() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhWordTracking())
		docu.writeAttribute("wordTrack",item->itemText.defaultStyle().charStyle().wordTracking());
	if ( ! item->itemText.defaultStyle().isInhMinWordTracking())
		docu.writeAttribute("MinWordTrack", item->itemText.defaultStyle().minWordTracking());
	if ( ! item->itemText.defaultStyle().isInhMinGlyphExtension())
		docu.writeAttribute("MinGlyphShrink", item->itemText.defaultStyle().minGlyphExtension());
	if ( ! item->itemText.defaultStyle().isInhMaxGlyphExtension())
		docu.writeAttribute("MaxGlyphExtend", item->itemText.defaultStyle().maxGlyphExtension());
	if ( ! item->itemText.defaultStyle().isInhOpticalMargins())
		docu.writeAttribute("OpticalMargins", item->itemText.defaultStyle().opticalMargins());
	if ( ! item->itemText.defaultStyle().isInhHyphenationMode())
		docu.writeAttribute("HyphenationMode", item->itemText.defaultStyle().hyphenationMode());
	docu.writeAttribute("COLUMNS", item->columns());
	docu.writeAttribute("COLGAP", item->columnGap());
	docu.writeAttribute("NAMEDLST",item->NamedLStyle);
	docu.writeAttribute("SHADE",item->fillShade());
	docu.writeAttribute("SHADE2",item->lineShade());
	docu.writeAttribute("GRTYP",item->GrType);
	docu.writeAttribute("ROT",item->rotation());
	docu.writeAttribute("PLINEART",item->PLineArt);
	docu.writeAttribute("PLINEEND", item->PLineEnd);
	docu.writeAttribute("PLINEJOIN", item->PLineJoin);
	if ( ! item->itemText.defaultStyle().isInhLineSpacing())
		docu.writeAttribute("LINESP",item->itemText.defaultStyle().lineSpacing());
	if ( ! item->itemText.defaultStyle().isInhLineSpacingMode())
		docu.writeAttribute("LINESPMode", item->itemText.defaultStyle().lineSpacingMode());
	docu.writeAttribute("LOCALSCX",item->imageXScale());
	docu.writeAttribute("LOCALSCY",item->imageYScale());
	docu.writeAttribute("LOCALX",item->imageXOffset());
	docu.writeAttribute("LOCALY",item->imageYOffset());
	docu.writeAttribute("PICART", item->imageShown() ? 1 : 0);
	docu.writeAttribute("PLTSHOW", item->PoShow ? 1 : 0);
	docu.writeAttribute("BASEOF", item->BaseOffs);
	docu.writeAttribute("textPathType", item->textPathType);
	docu.writeAttribute("textPathFlipped", static_cast<int>(item->textPathFlipped));
	docu.writeAttribute("FLIPPEDH", item->imageFlippedH());
	docu.writeAttribute("FLIPPEDV", item->imageFlippedV());
/*	docu.writeAttribute("BBOXX",item->BBoxX);
	docu.writeAttribute("BBOXH",item->BBoxH); */
	if ( ! item->itemText.defaultStyle().charStyle().isInhFont())
		docu.writeAttribute("IFONT",item->itemText.defaultStyle().charStyle().font().scName());
	if ( ! item->itemText.defaultStyle().charStyle().isInhFontSize())
		docu.writeAttribute("ISIZE",item->itemText.defaultStyle().charStyle().fontSize() / 10.0 );
	docu.writeAttribute("SCALETYPE", item->ScaleType ? 1 : 0);
	docu.writeAttribute("RATIO", item->AspectRatio ? 1 : 0);
	docu.writeAttribute("PRINTABLE", item->printEnabled() ? 1 : 0);
	if(item->isAnnotation())
	{
		docu.writeAttribute("ANNOTATION",1);
		docu.writeAttribute("ANTYPE", item->annotation().Type());
		docu.writeAttribute("ANACTION", item->annotation().Action());
		docu.writeAttribute("ANEACT", item->annotation().E_act());
		docu.writeAttribute("ANXACT", item->annotation().X_act());
		docu.writeAttribute("ANDACT", item->annotation().D_act());
		docu.writeAttribute("ANFOACT", item->annotation().Fo_act());
		docu.writeAttribute("ANBLACT", item->annotation().Bl_act());
		docu.writeAttribute("ANKACT", item->annotation().K_act());
		docu.writeAttribute("ANFACT", item->annotation().F_act());
		docu.writeAttribute("ANVACT", item->annotation().V_act());
		docu.writeAttribute("ANCACT", item->annotation().C_act());
		if (item->annotation().ActionType() == 8)
			docu.writeAttribute("ANEXTERN", item->annotation().Extern());
		else
			docu.writeAttribute("ANEXTERN", Path2Relative(item->annotation().Extern(), baseDir));
		docu.writeAttribute("ANZIEL", item->annotation().Ziel());
		docu.writeAttribute("ANACTYP", item->annotation().ActionType());
		docu.writeAttribute("ANTOOLTIP", item->annotation().ToolTip());
		docu.writeAttribute("ANBWID", item->annotation().Bwid());
		docu.writeAttribute("ANBSTY", item->annotation().Bsty());
		docu.writeAttribute("ANFEED", item->annotation().Feed());
		docu.writeAttribute("ANFLAG", item->annotation().Flag());
		docu.writeAttribute("ANFONT", item->annotation().Font());
		docu.writeAttribute("ANFORMAT", item->annotation().Format());
		docu.writeAttribute("ANROLL", item->annotation().RollOver());
		docu.writeAttribute("ANDOWN", item->annotation().Down());
		docu.writeAttribute("ANVIS", item->annotation().Vis());
		docu.writeAttribute("ANMC", item->annotation().MaxChar());
		docu.writeAttribute("ANCHK", item->annotation().IsChk());
		docu.writeAttribute("ANAA", item->annotation().AAact());
		docu.writeAttribute("ANCHKS", item->annotation().ChkStil());
		docu.writeAttribute("ANBCOL", item->annotation().borderColor());
		docu.writeAttribute("ANHTML", item->annotation().HTML());
		docu.writeAttribute("ANICON", item->annotation().UseIcons());
		docu.writeAttribute("ANPLACE", item->annotation().IPlace());
		docu.writeAttribute("ANSCALE", item->annotation().ScaleW());
	}
	else
		docu.writeAttribute("ANNOTATION",0);
	docu.writeAttribute("ANNAME", !item->AutoName ? item->itemName() : QString(""));
	// "TEXTFLOWMODE" succeed to "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes
	docu.writeAttribute("TEXTFLOWMODE", (int) item->textFlowMode() );
	// Set "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes for compatibility
	// with versions prior to 1.3.4
	docu.writeAttribute("TEXTFLOW" , item->textFlowAroundObject() ? 1 : 0);
	docu.writeAttribute("TEXTFLOW2", item->textFlowUsesBoundingBox() ? 1 : 0);
	docu.writeAttribute("TEXTFLOW3", item->textFlowUsesContourLine() ? 1 : 0);
	docu.writeAttribute("AUTOTEXT", item->isAutoText ? 1 : 0);
	docu.writeAttribute("EXTRA",item->textToFrameDistLeft());
	docu.writeAttribute("TEXTRA",item->textToFrameDistTop());
	docu.writeAttribute("BEXTRA",item->textToFrameDistBottom());
	docu.writeAttribute("REXTRA",item->textToFrameDistRight());
	if (((item->asImageFrame() && !item->asLatexFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
		docu.writeAttribute("PFILE",Path2Relative(item->Pfile, baseDir));
	else
		docu.writeAttribute("PFILE","");
	if (!item->Pfile2.isEmpty())
		docu.writeAttribute("PFILE2",Path2Relative(item->Pfile2, baseDir));
	else
		docu.writeAttribute("PFILE2","");
	if (!item->Pfile3.isEmpty())
		docu.writeAttribute("PFILE3",Path2Relative(item->Pfile3, baseDir));
	else
		docu.writeAttribute("PFILE3","");
	docu.writeAttribute("PRFILE",item->IProfile);
	docu.writeAttribute("EPROF", item->EmProfile);
	docu.writeAttribute("IRENDER",item->IRender);
	docu.writeAttribute("EMBEDDED", item->UseEmbedded ? 1 : 0);
	docu.writeAttribute("LOCK", item->locked() ? 1 : 0);
	docu.writeAttribute("LOCKR", item->sizeLocked() ? 1 : 0);
	docu.writeAttribute("REVERS", item->reversed() ? 1 : 0);
	docu.writeAttribute("TransValue", item->fillTransparency());
	docu.writeAttribute("TransValueS", item->lineTransparency());
	docu.writeAttribute("TransBlend", item->fillBlendmode());
	docu.writeAttribute("TransBlendS", item->lineBlendmode());
	docu.writeAttribute("isTableItem", static_cast<int>(item->isTableItem));
	docu.writeAttribute("TopLine", static_cast<int>(item->TopLine));
	docu.writeAttribute("LeftLine", static_cast<int>(item->LeftLine));
	docu.writeAttribute("RightLine", static_cast<int>(item->RightLine));
	docu.writeAttribute("BottomLine", static_cast<int>(item->BottomLine));
	if (item->isTableItem)
	{
		if (item->TopLink != 0)
			docu.writeAttribute("TopLINK", item->TopLink->ItemNr);
		else
			docu.writeAttribute("TopLINK", -1);
		if (item->LeftLink != 0)
			docu.writeAttribute("LeftLINK", item->LeftLink->ItemNr);
		else
			docu.writeAttribute("LeftLINK", -1);
		if (item->RightLink != 0)
			docu.writeAttribute("RightLINK", item->RightLink->ItemNr);
		else
			docu.writeAttribute("RightLINK", -1);
		if (item->BottomLink != 0)
			docu.writeAttribute("BottomLINK", item->BottomLink->ItemNr);
		else
			docu.writeAttribute("BottomLINK", -1);
		docu.writeAttribute("OwnLINK", item->ItemNr);
	}
	docu.writeAttribute("isGroupControl", static_cast<int>(item->isGroupControl));
	if (item->isGroupControl)
	{
		if (item->groupsLastItem != 0)
			docu.writeAttribute("groupsLastItem", item->groupsLastItem->ItemNr - item->ItemNr);
	}
	docu.writeAttribute("NUMDASH", static_cast<int>(item->DashValues.count()));
	QString dlp = "";
	QList<double>::Iterator dax;
	for (dax = item->DashValues.begin(); dax != item->DashValues.end(); ++dax)
		dlp += tmp.setNum((*dax)) + " ";
	docu.writeAttribute("DASHS", dlp);
	docu.writeAttribute("DASHOFF", item->DashOffset);
	docu.writeAttribute("NUMPO",item->PoLine.size());
	QString polp = "";
	for (uint nxx=0; nxx<item->PoLine.size(); ++nxx)
	{
		item->PoLine.point(nxx, &xf, &yf);
		polp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	docu.writeAttribute("POCOOR", polp);
	docu.writeAttribute("NUMCO",item->ContourLine.size());
	QString colp = "";
	for (uint nxx=0; nxx<item->ContourLine.size(); ++nxx)
	{
		item->ContourLine.point(nxx, &xf, &yf);
		colp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	docu.writeAttribute("COCOOR", colp);
	docu.writeAttribute("NUMGROUP", static_cast<int>(item->Groups.count()));
	QString glp = "";
	QStack<int>::Iterator nx;
	for (nx = item->Groups.begin(); nx != item->Groups.end(); ++nx)
		glp += tmp.setNum((*nx)) + " ";
	docu.writeAttribute("GROUPS", glp);
	if ( ! item->itemText.defaultStyle().charStyle().isInhLanguage())
		docu.writeAttribute("LANGUAGE", item->itemText.defaultStyle().charStyle().language());
	docu.writeAttribute("startArrowIndex", item->startArrowIndex());
	docu.writeAttribute("endArrowIndex", item->endArrowIndex());
}
