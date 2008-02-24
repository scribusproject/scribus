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


bool Scribus134Format::saveFile(const QString & fileName, const FileFormat & /* fmt */)
{
	QString text, tf, tf2, tc, tc2;
	QString fileDir = QFileInfo(fileName).absolutePath();
	m_lastSavedFile = "";
	QDomDocument docu("scribus");
	QString st="<SCRIBUSUTF8NEW></SCRIBUSUTF8NEW>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	elem.setAttribute("Version", QString(VERSION));
	QDomElement dc=docu.createElement("DOCUMENT");
	dc.setAttribute("ANZPAGES",m_Doc->DocPages.count());
	dc.setAttribute("PAGEWIDTH",m_Doc->pageWidth);
	dc.setAttribute("PAGEHEIGHT",m_Doc->pageHeight);
	dc.setAttribute("BORDERLEFT",m_Doc->pageMargins.Left);
	dc.setAttribute("BORDERRIGHT",m_Doc->pageMargins.Right);
	dc.setAttribute("BORDERTOP",m_Doc->pageMargins.Top);
	dc.setAttribute("BORDERBOTTOM",m_Doc->pageMargins.Bottom);
	dc.setAttribute("BleedTop", m_Doc->bleeds.Top);
	dc.setAttribute("BleedLeft", m_Doc->bleeds.Left);
	dc.setAttribute("BleedRight", m_Doc->bleeds.Right);
	dc.setAttribute("BleedBottom", m_Doc->bleeds.Bottom);
	dc.setAttribute("ORIENTATION",m_Doc->PageOri);
	dc.setAttribute("PAGESIZE",m_Doc->m_pageSize);
	dc.setAttribute("FIRSTNUM",m_Doc->FirstPnum);
	dc.setAttribute("BOOK", m_Doc->currentPageLayout);
	if(m_Doc->usesAutomaticTextFrames())
		dc.setAttribute("AUTOTEXT",1);
	dc.setAttribute("AUTOSPALTEN",m_Doc->PageSp);
	dc.setAttribute("ABSTSPALTEN",m_Doc->PageSpa);
	dc.setAttribute("UNITS",m_Doc->unitIndex());
	dc.setAttribute("DFONT",m_Doc->toolSettings.defFont);
	dc.setAttribute("DSIZE",m_Doc->toolSettings.defSize / 10.0);
	dc.setAttribute("DCOL",m_Doc->toolSettings.dCols);
	dc.setAttribute("DGAP",m_Doc->toolSettings.dGap);
	dc.setAttribute("TabFill",m_Doc->toolSettings.tabFillChar);
	dc.setAttribute("TabWidth",m_Doc->toolSettings.dTabWidth);
	dc.setAttribute("AUTHOR",m_Doc->documentInfo.getAuthor());
	dc.setAttribute("COMMENTS",m_Doc->documentInfo.getComments());
	dc.setAttribute("KEYWORDS",m_Doc->documentInfo.getKeywords());
	dc.setAttribute("PUBLISHER",m_Doc->documentInfo.getPublisher());
	dc.setAttribute("DOCDATE",m_Doc->documentInfo.getDate());
	dc.setAttribute("DOCTYPE",m_Doc->documentInfo.getType());
	dc.setAttribute("DOCFORMAT",m_Doc->documentInfo.getFormat());
	dc.setAttribute("DOCIDENT",m_Doc->documentInfo.getIdent());
	dc.setAttribute("DOCSOURCE",m_Doc->documentInfo.getSource());
	dc.setAttribute("DOCLANGINFO",m_Doc->documentInfo.getLangInfo());
	dc.setAttribute("DOCRELATION",m_Doc->documentInfo.getRelation());
	dc.setAttribute("DOCCOVER",m_Doc->documentInfo.getCover());
	dc.setAttribute("DOCRIGHTS",m_Doc->documentInfo.getRights());
	dc.setAttribute("DOCCONTRIB",m_Doc->documentInfo.getContrib());
	dc.setAttribute("TITLE",m_Doc->documentInfo.getTitle());
	dc.setAttribute("VHOCH",m_Doc->typographicSettings.valueSuperScript);
	dc.setAttribute("VHOCHSC",m_Doc->typographicSettings.scalingSuperScript);
	dc.setAttribute("VTIEF",m_Doc->typographicSettings.valueSubScript);
	dc.setAttribute("VTIEFSC",m_Doc->typographicSettings.scalingSubScript);
	dc.setAttribute("VKAPIT",m_Doc->typographicSettings.valueSmallCaps);
	dc.setAttribute("BASEGRID",m_Doc->typographicSettings.valueBaseGrid);
	dc.setAttribute("BASEO", m_Doc->typographicSettings.offsetBaseGrid);
	dc.setAttribute("AUTOL", m_Doc->typographicSettings.autoLineSpacing);
	dc.setAttribute("UnderlinePos", m_Doc->typographicSettings.valueUnderlinePos);
	dc.setAttribute("UnderlineWidth", m_Doc->typographicSettings.valueUnderlineWidth);
	dc.setAttribute("StrikeThruPos", m_Doc->typographicSettings.valueStrikeThruPos);
	dc.setAttribute("StrikeThruWidth", m_Doc->typographicSettings.valueStrikeThruWidth);
	dc.setAttribute("GROUPC",m_Doc->GroupCounter);
	dc.setAttribute("HCMS", static_cast<int>(m_Doc->HasCMS));
	dc.setAttribute("DPSo", static_cast<int>(m_Doc->CMSSettings.SoftProofOn));
	dc.setAttribute("DPSFo", static_cast<int>(m_Doc->CMSSettings.SoftProofFullOn));
	dc.setAttribute("DPuse", static_cast<int>(m_Doc->CMSSettings.CMSinUse));
	dc.setAttribute("DPgam", static_cast<int>(m_Doc->CMSSettings.GamutCheck));
	dc.setAttribute("DPbla", static_cast<int>(m_Doc->CMSSettings.BlackPoint));
	dc.setAttribute("DPMo",m_Doc->CMSSettings.DefaultMonitorProfile);
	dc.setAttribute("DPPr",m_Doc->CMSSettings.DefaultPrinterProfile);
	dc.setAttribute("DPIn",m_Doc->CMSSettings.DefaultImageRGBProfile);
	dc.setAttribute("DPInCMYK",m_Doc->CMSSettings.DefaultImageCMYKProfile);
	dc.setAttribute("DPIn2",m_Doc->CMSSettings.DefaultSolidColorRGBProfile);
	dc.setAttribute("DPIn3",m_Doc->CMSSettings.DefaultSolidColorCMYKProfile);
	dc.setAttribute("DISc",m_Doc->CMSSettings.DefaultIntentColors);
	dc.setAttribute("DIIm",m_Doc->CMSSettings.DefaultIntentImages);
	dc.setAttribute("ALAYER", m_Doc->activeLayer());
	dc.setAttribute("LANGUAGE", m_Doc->Language);
	dc.setAttribute("MINWORDLEN", m_Doc->MinWordLen);
	dc.setAttribute("HYCOUNT", m_Doc->HyCount);
	dc.setAttribute("AUTOMATIC", static_cast<int>(m_Doc->Automatic));
	dc.setAttribute("AUTOCHECK", static_cast<int>(m_Doc->AutoCheck));
	dc.setAttribute("GUIDELOCK", static_cast<int>(m_Doc->GuideLock));
	dc.setAttribute("SnapToGuides", static_cast<int>(m_Doc->SnapGuides));
	dc.setAttribute("SnapToGrid", static_cast<int>(m_Doc->useRaster));
	dc.setAttribute("MINGRID", m_Doc->guidesSettings.minorGrid);
	dc.setAttribute("MAJGRID", m_Doc->guidesSettings.majorGrid);
	dc.setAttribute("SHOWGRID", static_cast<int>(m_Doc->guidesSettings.gridShown));
	dc.setAttribute("SHOWGUIDES", static_cast<int>(m_Doc->guidesSettings.guidesShown));
	dc.setAttribute("showcolborders", static_cast<int>(m_Doc->guidesSettings.colBordersShown));
	dc.setAttribute("SHOWFRAME", static_cast<int>(m_Doc->guidesSettings.framesShown));
	dc.setAttribute("SHOWLAYERM", static_cast<int>(m_Doc->guidesSettings.layerMarkersShown));
	dc.setAttribute("SHOWMARGIN", static_cast<int>(m_Doc->guidesSettings.marginsShown));
	dc.setAttribute("SHOWBASE", static_cast<int>(m_Doc->guidesSettings.baseShown));
	dc.setAttribute("SHOWPICT", static_cast<int>(m_Doc->guidesSettings.showPic));
	dc.setAttribute("SHOWControl", static_cast<int>(m_Doc->guidesSettings.showControls));
	dc.setAttribute("SHOWLINK", static_cast<int>(m_Doc->guidesSettings.linkShown));
	dc.setAttribute("rulerMode", static_cast<int>(m_Doc->guidesSettings.rulerMode));
	dc.setAttribute("showrulers", static_cast<int>(m_Doc->guidesSettings.rulersShown));
	dc.setAttribute("showBleed", static_cast<int>(m_Doc->guidesSettings.showBleed));
	dc.setAttribute("rulerXoffset", m_Doc->rulerXoffset);
	dc.setAttribute("rulerYoffset", m_Doc->rulerYoffset);
	dc.setAttribute("GuideRad", m_Doc->guidesSettings.guideRad);
	dc.setAttribute("GRAB",m_Doc->guidesSettings.grabRad);
	dc.setAttribute("POLYC", m_Doc->toolSettings.polyC);
	dc.setAttribute("POLYF", m_Doc->toolSettings.polyF);
	dc.setAttribute("POLYR", m_Doc->toolSettings.polyR);
	dc.setAttribute("POLYFD", m_Doc->toolSettings.polyFd);
	dc.setAttribute("POLYS", static_cast<int>(m_Doc->toolSettings.polyS));
	dc.setAttribute("AutoSave", static_cast<int>(m_Doc->AutoSave));
	dc.setAttribute("AutoSaveTime", m_Doc->AutoSaveTime);
	dc.setAttribute("ScratchBottom", m_Doc->scratch.Bottom);
	dc.setAttribute("ScratchLeft", m_Doc->scratch.Left);
	dc.setAttribute("ScratchRight", m_Doc->scratch.Right);
	dc.setAttribute("ScratchTop", m_Doc->scratch.Top);
	dc.setAttribute("GapHorizontal", m_Doc->GapHorizontal);
	dc.setAttribute("GapVertical", m_Doc->GapVertical);
	dc.setAttribute("StartArrow", m_Doc->toolSettings.dStartArrow);
	dc.setAttribute("EndArrow", m_Doc->toolSettings.dEndArrow);
	dc.setAttribute("PEN",m_Doc->toolSettings.dPen);
	dc.setAttribute("BRUSH",m_Doc->toolSettings.dBrush);
	dc.setAttribute("PENLINE",m_Doc->toolSettings.dPenLine);
	dc.setAttribute("PENTEXT",m_Doc->toolSettings.dPenText);
	dc.setAttribute("StrokeText",m_Doc->toolSettings.dStrokeText);
	dc.setAttribute("TextBackGround", m_Doc->toolSettings.dTextBackGround);
	dc.setAttribute("TextLineColor", m_Doc->toolSettings.dTextLineColor);
	dc.setAttribute("TextBackGroundShade", m_Doc->toolSettings.dTextBackGroundShade);
	dc.setAttribute("TextLineShade", m_Doc->toolSettings.dTextLineShade);
	dc.setAttribute("TextPenShade", m_Doc->toolSettings.dTextPenShade);
	dc.setAttribute("TextStrokeShade", m_Doc->toolSettings.dTextStrokeShade);
	dc.setAttribute("STIL",m_Doc->toolSettings.dLineArt);
	dc.setAttribute("STILLINE",m_Doc->toolSettings.dLstyleLine);
	dc.setAttribute("WIDTH",m_Doc->toolSettings.dWidth);
	dc.setAttribute("WIDTHLINE",m_Doc->toolSettings.dWidthLine);
	dc.setAttribute("PENSHADE",m_Doc->toolSettings.dShade2);
	dc.setAttribute("LINESHADE",m_Doc->toolSettings.dShadeLine);
	dc.setAttribute("BRUSHSHADE",m_Doc->toolSettings.dShade);
	dc.setAttribute("MAGMIN",m_Doc->toolSettings.magMin);
	dc.setAttribute("MAGMAX",m_Doc->toolSettings.magMax);
	dc.setAttribute("MAGSTEP",m_Doc->toolSettings.magStep);
	dc.setAttribute("CPICT",m_Doc->toolSettings.dBrushPict);
	dc.setAttribute("PICTSHADE",m_Doc->toolSettings.shadePict);
	dc.setAttribute("PICTSCX",m_Doc->toolSettings.scaleX);
	dc.setAttribute("PICTSCY",m_Doc->toolSettings.scaleY);
	dc.setAttribute("PSCALE", static_cast<int>(m_Doc->toolSettings.scaleType));
	dc.setAttribute("PASPECT", static_cast<int>(m_Doc->toolSettings.aspectRatio));
	dc.setAttribute("EmbeddedPath", static_cast<int>(m_Doc->toolSettings.useEmbeddedPath));
	dc.setAttribute("HalfRes", m_Doc->toolSettings.lowResType);
	dc.setAttribute("dispX", m_Doc->toolSettings.dispX);
	dc.setAttribute("dispY", m_Doc->toolSettings.dispY);
	dc.setAttribute("constrain", m_Doc->toolSettings.constrain);
	dc.setAttribute("MINORC",m_Doc->guidesSettings.minorColor.name());
	dc.setAttribute("MAJORC",m_Doc->guidesSettings.majorColor.name());
	dc.setAttribute("GuideC", m_Doc->guidesSettings.guideColor.name());
	dc.setAttribute("BaseC", m_Doc->guidesSettings.baseColor.name());
	dc.setAttribute("GuideZ", m_Doc->guidesSettings.guideRad);
	dc.setAttribute("BACKG", static_cast<int>(m_Doc->guidesSettings.before));
	dc.setAttribute("PAGEC",m_Doc->papColor.name());
	dc.setAttribute("MARGC",m_Doc->guidesSettings.margColor.name());
	dc.setAttribute("RANDF", static_cast<int>(m_Doc->marginColored));
	dc.setAttribute("currentProfile", m_Doc->curCheckProfile);
	elem.appendChild(dc);
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
	
	/**
		* changed to enable saving
	 * of *.gz documents
	 * 2.7.2002 C.Toepp
	 * <c.toepp@gmx.de>
	 */

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

	bool   writeSucceed = false;
	static const char xmlpi[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	QByteArray cs = docu.toByteArray(); // UTF-8 QCString
	if(fileName.right(2) == "gz")
	{
		ScGzFile gzf(tmpFileName, cs);
		writeSucceed = gzf.write(xmlpi);
	}
	else
	{
		QFile f(tmpFileName);
		if(f.open(QIODevice::WriteOnly))
		{
			QDataStream s(&f);
			int  xmlpilen = strlen(xmlpi);
			int  bytesWritten1 = s.writeRawData(xmlpi, xmlpilen);
			int  bytesWritten2 = s.writeRawData(cs, cs.length());
			bool writeSuccess1 = (bytesWritten1 > 0 && (bytesWritten1 == xmlpilen));
			bool writeSuccess2 = (bytesWritten2 > 0 && (bytesWritten2 == cs.length()));
			writeSucceed = (writeSuccess1 && writeSuccess2 && f.error() == QFile::NoError);
			f.close();
		}
	}
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

void Scribus134Format::writeCheckerProfiles(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	CheckerPrefsList::Iterator itcp;
	CheckerPrefsList::Iterator itcpend=m_Doc->checkerProfiles.end();
	for (itcp = m_Doc->checkerProfiles.begin(); itcp != itcpend; ++itcp)
	{
		QDomElement dc79a=docu.createElement("CheckProfile");
		dc79a.setAttribute("Name",itcp.key());
		dc79a.setAttribute("ignoreErrors", static_cast<int>(itcp.value().ignoreErrors));
		dc79a.setAttribute("autoCheck", static_cast<int>(itcp.value().autoCheck));
		dc79a.setAttribute("checkGlyphs", static_cast<int>(itcp.value().checkGlyphs));
		dc79a.setAttribute("checkOrphans", static_cast<int>(itcp.value().checkOrphans));
		dc79a.setAttribute("checkOverflow", static_cast<int>(itcp.value().checkOverflow));
		dc79a.setAttribute("checkPictures", static_cast<int>(itcp.value().checkPictures));
		dc79a.setAttribute("checkResolution", static_cast<int>(itcp.value().checkResolution));
		dc79a.setAttribute("checkTransparency", static_cast<int>(itcp.value().checkTransparency));
		dc79a.setAttribute("minResolution",itcp.value().minResolution);
		dc79a.setAttribute("maxResolution",itcp.value().maxResolution);
		dc79a.setAttribute("checkAnnotations", static_cast<int>(itcp.value().checkAnnotations));
		dc79a.setAttribute("checkRasterPDF", static_cast<int>(itcp.value().checkRasterPDF));
		dc79a.setAttribute("checkForGIF", static_cast<int>(itcp.value().checkForGIF));
		dc79a.setAttribute("ignoreOffLayers", static_cast<int>(itcp.value().ignoreOffLayers));
		dc.appendChild(dc79a);
	}
}

void Scribus134Format::writeLinestyles(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QMap<QString,multiLine>::Iterator itMU;
	for (itMU = m_Doc->MLineStyles.begin(); itMU != m_Doc->MLineStyles.end(); ++itMU)
	{
		QDomElement MuL=docu.createElement("MultiLine");
		MuL.setAttribute("Name",itMU.key());
		multiLine ml = itMU.value();
		multiLine::iterator itMU2;
		for (itMU2 = ml.begin(); itMU2 != ml.end(); ++itMU2)
		{
			QDomElement SuL=docu.createElement("SubLine");
			SuL.setAttribute("Color", (*itMU2).Color);
			SuL.setAttribute("Shade", (*itMU2).Shade);
			SuL.setAttribute("Dash", (*itMU2).Dash);
			SuL.setAttribute("LineEnd", (*itMU2).LineEnd);
			SuL.setAttribute("LineJoin", (*itMU2).LineJoin);
			SuL.setAttribute("Width", (*itMU2).Width);
			SuL.setAttribute("Shortcut", ml.shortcut);
			MuL.appendChild(SuL);
		}
		dc.appendChild(MuL);
	}
	QList<ArrowDesc>::Iterator itar;
	for (itar = m_Doc->arrowStyles.begin(); itar != m_Doc->arrowStyles.end(); ++itar)
	{
		if ((*itar).userArrow)
		{
			QDomElement ar=docu.createElement("Arrows");
			ar.setAttribute("NumPoints", (*itar).points.size());
			QString arp = "";
			QString tmp, tmpy;
			double xa, ya;
			for (uint nxx = 0; nxx < (*itar).points.size(); ++nxx)
			{
				(*itar).points.point(nxx, &xa, &ya);
				arp += tmp.setNum(xa) + " " + tmpy.setNum(ya) + " ";
			}
			ar.setAttribute("Points", arp);
			ar.setAttribute("Name", (*itar).name);
			dc.appendChild(ar);
		}
	}
}


void Scribus134Format::writeJavascripts(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QMap<QString,QString>::Iterator itja;
	for (itja = m_Doc->JavaScripts.begin(); itja != m_Doc->JavaScripts.end(); ++itja)
	{
		QDomElement jav=docu.createElement("JAVA");
		jav.setAttribute("NAME",itja.key());
		jav.setAttribute("SCRIPT",itja.value());
		dc.appendChild(jav);
	}
	
}


void Scribus134Format::writeBookmarks(QDomDocument & docu) 
{	
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QList<ScribusDoc::BookMa>::Iterator itbm;
	for (itbm = m_Doc->BookMarks.begin(); itbm != m_Doc->BookMarks.end(); ++itbm)
	{
		QDomElement fn=docu.createElement("Bookmark");
		fn.setAttribute("Title",(*itbm).Title);
		fn.setAttribute("Text",(*itbm).Text);
		fn.setAttribute("Aktion",(*itbm).Aktion);
		fn.setAttribute("ItemNr", (*itbm).ItemNr);
		fn.setAttribute("Element", (*itbm).PageObject->ItemNr);
		fn.setAttribute("First", (*itbm).First);
		fn.setAttribute("Last", (*itbm).Last);
		fn.setAttribute("Prev", (*itbm).Prev);
		fn.setAttribute("Next", (*itbm).Next);
		fn.setAttribute("Parent", (*itbm).Parent);
		dc.appendChild(fn);
	}
}


void Scribus134Format::writeColors(QDomDocument & docu) 
{	
	QDomElement dc=docu.documentElement().firstChild().toElement();

	ColorList::Iterator itc;
	for (itc = m_Doc->PageColors.begin(); itc != m_Doc->PageColors.end(); ++itc)
	{
		QDomElement co=docu.createElement("COLOR");
		co.setAttribute("NAME",itc.key());
		if (m_Doc->PageColors[itc.key()].getColorModel() == colorModelRGB)
			co.setAttribute("RGB",m_Doc->PageColors[itc.key()].nameRGB());
		else
			co.setAttribute("CMYK",m_Doc->PageColors[itc.key()].nameCMYK());
		co.setAttribute("Spot",static_cast<int>(m_Doc->PageColors[itc.key()].isSpotColor()));
		co.setAttribute("Register",static_cast<int>(m_Doc->PageColors[itc.key()].isRegistrationColor()));
		dc.appendChild(co);
	}
	
}

void Scribus134Format::writeHyphenatorLists(QDomDocument &docu)
{
	QDomElement dc=docu.documentElement().firstChild().toElement();
	QDomElement rde = docu.createElement("HYPHEN");
	for (QHash<QString, QString>::Iterator hyit = m_Doc->docHyphenator->specialWords.begin(); hyit != m_Doc->docHyphenator->specialWords.end(); ++hyit)
	{
		QDomElement hyelm = docu.createElement("EXCEPTION");
		hyelm.setAttribute("WORD", hyit.key());
		hyelm.setAttribute("HYPHENATED", hyit.value());
		rde.appendChild(hyelm);
	}
	for (QSet<QString>::Iterator hyit2 = m_Doc->docHyphenator->ignoredWords.begin(); hyit2 != m_Doc->docHyphenator->ignoredWords.end(); ++hyit2)
	{
		QDomElement hyelm2 = docu.createElement("IGNORE");
		hyelm2.setAttribute("WORD", (*hyit2));
		rde.appendChild(hyelm2);
	}
	dc.appendChild(rde);
}

void Scribus134Format::writePStyles(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	//	if (m_Doc->docParagraphStyles.count() > 5)
	{
		for (int ff = 0; ff < m_Doc->paragraphStyles().count(); ++ff)
		{
			QDomElement fo=docu.createElement("STYLE");
			putPStyle(docu, fo, m_Doc->paragraphStyles()[ff]);
			dc.appendChild(fo);
		}
	}
}

void Scribus134Format::putPStyle(QDomDocument & docu, QDomElement & fo, const ParagraphStyle & style)
{
	if ( ! style.name().isEmpty() )
		fo.setAttribute("NAME", style.name());
	if ( ! style.parent().isEmpty())
		fo.setAttribute("PARENT", style.parent());
	
	if ( ! style.isInhAlignment())
		fo.setAttribute("ALIGN", style.alignment());
	if ( ! style.isInhLineSpacingMode())
		fo.setAttribute("LINESPMode", style.lineSpacingMode());
	if ( ! style.isInhLineSpacing())
		fo.setAttribute("LINESP", style.lineSpacing());
	if ( ! style.isInhLeftMargin())
		fo.setAttribute("INDENT", style.leftMargin());
	if ( ! style.isInhRightMargin())
		fo.setAttribute("RMARGIN", style.rightMargin());
	if ( ! style.isInhFirstIndent())
		fo.setAttribute("FIRST", style.firstIndent());
	if ( ! style.isInhGapBefore())
		fo.setAttribute("VOR", style.gapBefore());
	if ( ! style.isInhGapAfter())
		fo.setAttribute("NACH", style.gapAfter());
	if ( ! style.isInhHasDropCap())
		fo.setAttribute("DROP", static_cast<int>(style.hasDropCap()));
	if ( ! style.isInhDropCapLines())
		fo.setAttribute("DROPLIN", style.dropCapLines());
	if ( ! style.isInhDropCapOffset())
		fo.setAttribute("DROPDIST", style.dropCapOffset());
	if ( ! style.isInhOpticalMargins())
		fo.setAttribute("OpticalMargins", style.opticalMargins());
	if ( ! style.isInhHyphenationMode())
		fo.setAttribute("HyphenationMode", style.hyphenationMode());
	if ( ! style.isInhMinWordTracking())
		fo.setAttribute("MinWordTrack", style.minWordTracking());
	if ( ! style.isInhMinGlyphExtension())
		fo.setAttribute("MinGlyphShrink", style.minGlyphExtension());
	if ( ! style.isInhMaxGlyphExtension())
		fo.setAttribute("MaxGlyphExtend", style.maxGlyphExtension());

	if ( ! style.shortcut().isEmpty() )
		fo.setAttribute("PSHORTCUT", style.shortcut()); // shortcuts won't be inherited
	
	putCStyle(docu, fo, style.charStyle());

	if ( ! style.isInhTabValues())
	{
		for (int a = 0; a < style.tabValues().count(); ++a)
		{
			QDomElement tabs = docu.createElement("Tabs");
			tabs.setAttribute("Type", (style.tabValues().at(a)).tabType);
			tabs.setAttribute("Pos", (style.tabValues().at(a)).tabPosition);
			QString tabCh = "";
			if (!(style.tabValues().at(a)).tabFillChar.isNull())
				tabCh = QString((style.tabValues().at(a)).tabFillChar);
			tabs.setAttribute("Fill", tabCh);
			fo.appendChild(tabs);
		}
	}
}


void Scribus134Format::writeCStyles(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();
	
	for (int ff = 0; ff < m_Doc->charStyles().count(); ++ff)
	{
		QDomElement fo=docu.createElement("CHARSTYLE");
		putCStyle(docu, fo, m_Doc->charStyles()[ff]);
		dc.appendChild(fo);
	}
}

void Scribus134Format::putCStylePT(QDomDocument & docu, QDomElement & fo, const CharStyle & style)
{
	fo.setAttribute("CNAME", style.name());
	fo.setAttribute("CPARENT", style.parent());
	fo.setAttribute("FONT", style.font().scName());
	fo.setAttribute("FONTSIZE", style.fontSize() / 10.0);
	fo.setAttribute("FEATURES", style.features().join(" "));
	fo.setAttribute("FCOLOR", style.fillColor());
	fo.setAttribute("FSHADE", style.fillShade());
	fo.setAttribute("SCOLOR", style.strokeColor());
	fo.setAttribute("SSHADE", style.strokeShade());
	fo.setAttribute("TXTSHX", style.shadowXOffset() / 10.0);
	fo.setAttribute("TXTSHY", style.shadowYOffset() / 10.0);
	fo.setAttribute("TXTOUT", style.outlineWidth() / 10.0);
	fo.setAttribute("TXTULP", style.underlineOffset() / 10.0);
	fo.setAttribute("TXTULW", style.underlineWidth() / 10.0);
	fo.setAttribute("TXTSTP", style.strikethruOffset() / 10.0);
	fo.setAttribute("TXTSTW", style.strikethruWidth() / 10.0);
	fo.setAttribute("SCALEH", style.scaleH() / 10.0);
	fo.setAttribute("SCALEV", style.scaleV() / 10.0);
	fo.setAttribute("BASEO", style.baselineOffset() / 10.0);
	fo.setAttribute("KERN", style.tracking() / 10.0);
	fo.setAttribute("wordTrack", style.wordTracking());
	fo.setAttribute("SHORTCUT", style.shortcut()); // shortcuts won't be inherited
}

void Scribus134Format::putCStyle(QDomDocument & docu, QDomElement & fo, const CharStyle & style)
{
	if ( ! style.name().isEmpty() )
		fo.setAttribute("CNAME", style.name());
	if ( ! style.parent().isEmpty() )
		fo.setAttribute("CPARENT", style.parent());
	if ( ! style.isInhFont())	
		fo.setAttribute("FONT", style.font().scName());
	if ( ! style.isInhFontSize())
		fo.setAttribute("FONTSIZE", style.fontSize() / 10.0);
	if ( ! style.isInhFeatures())
		fo.setAttribute("FEATURES", style.features().join(" "));
	if ( ! style.isInhFillColor())
		fo.setAttribute("FCOLOR", style.fillColor());
	if ( ! style.isInhFillShade())
		fo.setAttribute("FSHADE", style.fillShade());
	if ( ! style.isInhStrokeColor())
		fo.setAttribute("SCOLOR", style.strokeColor());
	if ( ! style.isInhStrokeShade())
		fo.setAttribute("SSHADE", style.strokeShade());
	if ( ! style.isInhShadowXOffset())
		fo.setAttribute("TXTSHX", style.shadowXOffset() / 10.0);
	if ( ! style.isInhShadowYOffset())
		fo.setAttribute("TXTSHY", style.shadowYOffset() / 10.0);
	if ( ! style.isInhOutlineWidth())
		fo.setAttribute("TXTOUT", style.outlineWidth() / 10.0);
	if ( ! style.isInhUnderlineOffset())
		fo.setAttribute("TXTULP", style.underlineOffset() / 10.0);
	if ( ! style.isInhUnderlineWidth())
		fo.setAttribute("TXTULW", style.underlineWidth() / 10.0);
	if ( ! style.isInhStrikethruOffset())
		fo.setAttribute("TXTSTP", style.strikethruOffset() / 10.0);
	if ( ! style.isInhStrikethruWidth())
		fo.setAttribute("TXTSTW", style.strikethruWidth() / 10.0);
	if ( ! style.isInhScaleH())
		fo.setAttribute("SCALEH", style.scaleH() / 10.0);
	if ( ! style.isInhScaleV())
		fo.setAttribute("SCALEV", style.scaleV() / 10.0);
	if ( ! style.isInhBaselineOffset())
		fo.setAttribute("BASEO", style.baselineOffset() / 10.0);
	if ( ! style.isInhTracking())
		fo.setAttribute("KERN", style.tracking() / 10.0); 
	if ( ! style.isInhWordTracking())
		fo.setAttribute("wordTrack", style.wordTracking());
	if ( ! style.shortcut().isEmpty() )
		fo.setAttribute("SHORTCUT", style.shortcut()); // shortcuts won't be inherited
}

void Scribus134Format::writeLayers(QDomDocument & docu) 
{	
	QDomElement dc=docu.documentElement().firstChild().toElement();

	uint layerCount=m_Doc->layerCount();
	for (uint lay = 0; lay < layerCount; ++lay)
	{
		QDomElement la = docu.createElement("LAYERS");
		la.setAttribute("NUMMER",m_Doc->Layers[lay].LNr);
		la.setAttribute("LEVEL",m_Doc->Layers[lay].Level);
		la.setAttribute("NAME",m_Doc->Layers[lay].Name);
		la.setAttribute("SICHTBAR", static_cast<int>(m_Doc->Layers[lay].isViewable));
		la.setAttribute("DRUCKEN", static_cast<int>(m_Doc->Layers[lay].isPrintable));
		la.setAttribute("EDIT", static_cast<int>(m_Doc->Layers[lay].isEditable));
		la.setAttribute("FLOW", static_cast<int>(m_Doc->Layers[lay].flowControl));
		la.setAttribute("TRANS", m_Doc->Layers[lay].transparency);
		la.setAttribute("BLEND", m_Doc->Layers[lay].blendMode);
		la.setAttribute("OUTL", static_cast<int>(m_Doc->Layers[lay].outlineMode));
		la.setAttribute("LAYERC",m_Doc->Layers[lay].markerColor.name());
		dc.appendChild(la);
	}
}

void Scribus134Format::writePrintOptions(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QDomElement pri = docu.createElement("Printer");
	pri.setAttribute("firstUse", static_cast<int>(m_Doc->Print_Options.firstUse));
	pri.setAttribute("toFile", static_cast<int>(m_Doc->Print_Options.toFile));
	pri.setAttribute("useAltPrintCommand", static_cast<int>(m_Doc->Print_Options.useAltPrintCommand));
	pri.setAttribute("outputSeparations", static_cast<int>(m_Doc->Print_Options.outputSeparations));
	pri.setAttribute("useSpotColors", static_cast<int>(m_Doc->Print_Options.useSpotColors));
	pri.setAttribute("useColor", static_cast<int>(m_Doc->Print_Options.useColor));
	pri.setAttribute("mirrorH", static_cast<int>(m_Doc->Print_Options.mirrorH));
	pri.setAttribute("mirrorV", static_cast<int>(m_Doc->Print_Options.mirrorV));
	pri.setAttribute("useICC", static_cast<int>(m_Doc->Print_Options.useICC));
	pri.setAttribute("doGCR", static_cast<int>(m_Doc->Print_Options.doGCR));
	pri.setAttribute("doClip", static_cast<int>(m_Doc->Print_Options.doClip));
	pri.setAttribute("setDevParam", static_cast<int>(m_Doc->Print_Options.setDevParam));
	pri.setAttribute("doOverprint", static_cast<int>(m_Doc->Print_Options.doOverprint));
	pri.setAttribute("cropMarks", static_cast<int>(m_Doc->Print_Options.cropMarks));
	pri.setAttribute("bleedMarks", static_cast<int>(m_Doc->Print_Options.bleedMarks));
	pri.setAttribute("registrationMarks", static_cast<int>(m_Doc->Print_Options.registrationMarks));
	pri.setAttribute("colorMarks", static_cast<int>(m_Doc->Print_Options.colorMarks));
	pri.setAttribute("PSLevel", (m_Doc->Print_Options.prnEngine < WindowsGDI) ? m_Doc->Print_Options.prnEngine : PostScript3);
	pri.setAttribute("PDLanguage" , m_Doc->Print_Options.prnEngine);
	pri.setAttribute("markOffset" , m_Doc->Print_Options.markOffset);
	pri.setAttribute("BleedTop"   , m_Doc->Print_Options.bleeds.Top);
	pri.setAttribute("BleedLeft"  , m_Doc->Print_Options.bleeds.Left);
	pri.setAttribute("BleedRight" , m_Doc->Print_Options.bleeds.Right);
	pri.setAttribute("BleedBottom", m_Doc->Print_Options.bleeds.Bottom);
	pri.setAttribute("printer" , m_Doc->Print_Options.printer);
	pri.setAttribute("filename", m_Doc->Print_Options.filename);
	pri.setAttribute("separationName", m_Doc->Print_Options.separationName);
	pri.setAttribute("printerCommand", m_Doc->Print_Options.printerCommand);
	for (int p = 0; p < m_Doc->Print_Options.allSeparations.count(); ++p)
	{
		QDomElement pri2 = docu.createElement("Separation");
		pri2.setAttribute("Name", m_Doc->Print_Options.allSeparations[p]);
		pri.appendChild(pri2);
	}
	dc.appendChild(pri);
}

void Scribus134Format::writePdfOptions(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QDomElement pdf = docu.createElement("PDF");
	pdf.setAttribute("firstUse", static_cast<int>(m_Doc->PDF_Options.firstUse));
	pdf.setAttribute("Thumbnails", static_cast<int>(m_Doc->PDF_Options.Thumbnails));
	pdf.setAttribute("Articles", static_cast<int>(m_Doc->PDF_Options.Articles));
	pdf.setAttribute("Bookmarks", static_cast<int>(m_Doc->PDF_Options.Bookmarks));
	pdf.setAttribute("Compress", static_cast<int>(m_Doc->PDF_Options.Compress));
	pdf.setAttribute("CMethod", m_Doc->PDF_Options.CompressMethod);
	pdf.setAttribute("Quality", m_Doc->PDF_Options.Quality);
	pdf.setAttribute("MirrorH", static_cast<int>(m_Doc->PDF_Options.MirrorH));
	pdf.setAttribute("MirrorV", static_cast<int>(m_Doc->PDF_Options.MirrorV));
	pdf.setAttribute("Clip", static_cast<int>(m_Doc->PDF_Options.doClip));
	pdf.setAttribute("RotateDeg", static_cast<int>(m_Doc->PDF_Options.RotateDeg));
	pdf.setAttribute("PresentMode", static_cast<int>(m_Doc->PDF_Options.PresentMode));
	pdf.setAttribute("RecalcPic", static_cast<int>(m_Doc->PDF_Options.RecalcPic));
	pdf.setAttribute("Grayscale", static_cast<int>(m_Doc->PDF_Options.isGrayscale));
	pdf.setAttribute("RGBMode", static_cast<int>(m_Doc->PDF_Options.UseRGB));
	pdf.setAttribute("UseProfiles", static_cast<int>(m_Doc->PDF_Options.UseProfiles));
	pdf.setAttribute("UseProfiles2", static_cast<int>(m_Doc->PDF_Options.UseProfiles2));
	pdf.setAttribute("Binding", m_Doc->PDF_Options.Binding);
	pdf.setAttribute("PicRes", m_Doc->PDF_Options.PicRes);
	pdf.setAttribute("Resolution", m_Doc->PDF_Options.Resolution);
	pdf.setAttribute("Version", m_Doc->PDF_Options.Version);
	pdf.setAttribute("Intent", m_Doc->PDF_Options.Intent);
	pdf.setAttribute("Intent2", m_Doc->PDF_Options.Intent2);
	pdf.setAttribute("SolidP", m_Doc->PDF_Options.SolidProf);
	pdf.setAttribute("ImageP", m_Doc->PDF_Options.ImageProf);
	pdf.setAttribute("PrintP", m_Doc->PDF_Options.PrintProf);
	pdf.setAttribute("InfoString", m_Doc->PDF_Options.Info);
	pdf.setAttribute("BTop", m_Doc->PDF_Options.bleeds.Top);
	pdf.setAttribute("BLeft", m_Doc->PDF_Options.bleeds.Left);
	pdf.setAttribute("BRight", m_Doc->PDF_Options.bleeds.Right);
	pdf.setAttribute("BBottom", m_Doc->PDF_Options.bleeds.Bottom);
	pdf.setAttribute("useDocBleeds", static_cast<int>(m_Doc->PDF_Options.useDocBleeds));
	pdf.setAttribute("cropMarks", static_cast<int>(m_Doc->PDF_Options.cropMarks));
	pdf.setAttribute("bleedMarks", static_cast<int>(m_Doc->PDF_Options.bleedMarks));
	pdf.setAttribute("registrationMarks", static_cast<int>(m_Doc->PDF_Options.registrationMarks));
	pdf.setAttribute("colorMarks", static_cast<int>(m_Doc->PDF_Options.colorMarks));
	pdf.setAttribute("docInfoMarks", static_cast<int>(m_Doc->PDF_Options.docInfoMarks));
	pdf.setAttribute("markOffset", m_Doc->PDF_Options.markOffset);
	pdf.setAttribute("ImagePr", static_cast<int>(m_Doc->PDF_Options.EmbeddedI));
	pdf.setAttribute("PassOwner", m_Doc->PDF_Options.PassOwner);
	pdf.setAttribute("PassUser", m_Doc->PDF_Options.PassUser);
	pdf.setAttribute("Permissions", m_Doc->PDF_Options.Permissions);
	pdf.setAttribute("Encrypt", static_cast<int>(m_Doc->PDF_Options.Encrypt));
	pdf.setAttribute("UseLayers", static_cast<int>(m_Doc->PDF_Options.useLayers));
	pdf.setAttribute("UseLpi", static_cast<int>(m_Doc->PDF_Options.UseLPI));
	pdf.setAttribute("UseSpotColors", static_cast<int>(m_Doc->PDF_Options.UseSpotColors));
	pdf.setAttribute("doOverprint", static_cast<int>(m_Doc->PDF_Options.doOverprint));
	pdf.setAttribute("doMultiFile", static_cast<int>(m_Doc->PDF_Options.doMultiFile));
	pdf.setAttribute("displayBookmarks", static_cast<int>(m_Doc->PDF_Options.displayBookmarks));
	pdf.setAttribute("displayFullscreen", static_cast<int>(m_Doc->PDF_Options.displayFullscreen));
	pdf.setAttribute("displayLayers", static_cast<int>(m_Doc->PDF_Options.displayLayers));
	pdf.setAttribute("displayThumbs", static_cast<int>(m_Doc->PDF_Options.displayThumbs));
	pdf.setAttribute("hideMenuBar", static_cast<int>(m_Doc->PDF_Options.hideMenuBar));
	pdf.setAttribute("hideToolBar", static_cast<int>(m_Doc->PDF_Options.hideToolBar));
	pdf.setAttribute("fitWindow", static_cast<int>(m_Doc->PDF_Options.fitWindow));
	pdf.setAttribute("PageLayout", m_Doc->PDF_Options.PageLayout);
	pdf.setAttribute("openAction", m_Doc->PDF_Options.openAction);
	for (int pdoF = 0; pdoF < m_Doc->PDF_Options.EmbedList.count(); ++pdoF)
	{
		QDomElement pdf2 = docu.createElement("Fonts");
		pdf2.setAttribute("Name", m_Doc->PDF_Options.EmbedList[pdoF]);
		pdf.appendChild(pdf2);
	}
	for (int pdoS = 0; pdoS < m_Doc->PDF_Options.SubsetList.count(); ++pdoS)
	{
		QDomElement pdf4 = docu.createElement("Subset");
		pdf4.setAttribute("Name", m_Doc->PDF_Options.SubsetList[pdoS]);
		pdf.appendChild(pdf4);
	}
	for (int pdoE = 0; pdoE < m_Doc->PDF_Options.PresentVals.count(); ++pdoE)
	{
		QDomElement pdf3 = docu.createElement("Effekte");
		pdf3.setAttribute("pageEffectDuration", m_Doc->PDF_Options.PresentVals[pdoE].pageEffectDuration);
		pdf3.setAttribute("pageViewDuration", m_Doc->PDF_Options.PresentVals[pdoE].pageViewDuration);
		pdf3.setAttribute("effectType", m_Doc->PDF_Options.PresentVals[pdoE].effectType);
		pdf3.setAttribute("Dm", m_Doc->PDF_Options.PresentVals[pdoE].Dm);
		pdf3.setAttribute("M", m_Doc->PDF_Options.PresentVals[pdoE].M);
		pdf3.setAttribute("Di", m_Doc->PDF_Options.PresentVals[pdoE].Di);
		pdf.appendChild(pdf3);
	}
	QMap<QString,LPIData>::Iterator itlp;
	for (itlp = m_Doc->PDF_Options.LPISettings.begin(); itlp != m_Doc->PDF_Options.LPISettings.end(); ++itlp)
	{
		QDomElement pdf4 = docu.createElement("LPI");
		pdf4.setAttribute("Color", itlp.key());
		pdf4.setAttribute("Frequency", itlp.value().Frequency);
		pdf4.setAttribute("Angle", itlp.value().Angle);
		pdf4.setAttribute("SpotFunction", itlp.value().SpotFunc);
		pdf.appendChild(pdf4);
	}
	dc.appendChild(pdf);
}

void Scribus134Format::writeDocItemAttributes(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QDomElement docItemAttrs = docu.createElement("DocItemAttributes");
	for(ObjAttrVector::Iterator objAttrIt = m_Doc->docItemAttributes.begin() ; objAttrIt != m_Doc->docItemAttributes.end(); ++objAttrIt )
	{
		QDomElement itemAttr = docu.createElement("ItemAttribute");
		itemAttr.setAttribute("Name", (*objAttrIt).name);
		itemAttr.setAttribute("Type", (*objAttrIt).type);
		itemAttr.setAttribute("Value", (*objAttrIt).value);
		itemAttr.setAttribute("Parameter", (*objAttrIt).parameter);
		itemAttr.setAttribute("Relationship", (*objAttrIt).relationship);
		itemAttr.setAttribute("RelationshipTo", (*objAttrIt).relationshipto);
		itemAttr.setAttribute("AutoAddTo", (*objAttrIt).autoaddto);
		docItemAttrs.appendChild(itemAttr);
	}
	dc.appendChild(docItemAttrs);
}


void Scribus134Format::writeTOC(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QDomElement tocElem = docu.createElement("TablesOfContents");
	for(ToCSetupVector::Iterator tocSetupIt = m_Doc->docToCSetups.begin() ; tocSetupIt != m_Doc->docToCSetups.end(); ++tocSetupIt )
	{
		QDomElement tocsetup = docu.createElement("TableOfContents");
		tocsetup.setAttribute("Name", (*tocSetupIt).name);
		tocsetup.setAttribute("ItemAttributeName", (*tocSetupIt).itemAttrName);
		tocsetup.setAttribute("FrameName", (*tocSetupIt).frameName);
		tocsetup.setAttribute("ListNonPrinting", (*tocSetupIt).listNonPrintingFrames);
		tocsetup.setAttribute("Style", (*tocSetupIt).textStyle);
		switch ((*tocSetupIt).pageLocation)
		{
			case Beginning:
				tocsetup.setAttribute("NumberPlacement", "Beginning");
				break;
			case End:
				tocsetup.setAttribute("NumberPlacement", "End");
				break;
			case NotShown:
				tocsetup.setAttribute("NumberPlacement", "NotShown");
				break;
		}
		tocElem.appendChild(tocsetup);
	}
	dc.appendChild(tocElem);
}


void Scribus134Format::writeSections(QDomDocument & docu) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QDomElement sectionElem = docu.createElement("Sections");
	for(DocumentSectionMap::Iterator it = m_Doc->sections.begin() ; it != m_Doc->sections.end(); ++it )
	{
		QDomElement currsection = docu.createElement("Section");
		currsection.setAttribute("Number", (*it).number);
		currsection.setAttribute("Name", (*it).name);
		currsection.setAttribute("From", (*it).fromindex);
		currsection.setAttribute("To", (*it).toindex);
		switch ((*it).type)
		{
			case Type_1_2_3:
				currsection.setAttribute("Type", "Type_1_2_3");
				break;
			case Type_i_ii_iii:
				currsection.setAttribute("Type", "Type_i_ii_iii");
				break;
			case Type_I_II_III:
				currsection.setAttribute("Type", "Type_I_II_III");
				break;
			case Type_a_b_c:
				currsection.setAttribute("Type", "Type_a_b_c");
				break;
			case Type_A_B_C:
				currsection.setAttribute("Type", "Type_A_B_C");
				break;
			case Type_None:
				currsection.setAttribute("Type", "Type_None");
				break;
		}
		currsection.setAttribute("Start", (*it).sectionstartindex);
		currsection.setAttribute("Reversed", (*it).reversed);
		currsection.setAttribute("Active", (*it).active);
		sectionElem.appendChild(currsection);
	}
	dc.appendChild(sectionElem);
}


void Scribus134Format::writePageSets(QDomDocument & docu) 
{	
	QDomElement dc=docu.documentElement().firstChild().toElement();

	QDomElement pageSetAttr = docu.createElement("PageSets");
	QList<PageSet>::Iterator itpgset;
	for(itpgset = m_Doc->pageSets.begin(); itpgset != m_Doc->pageSets.end(); ++itpgset )
	{
		QDomElement pgst = docu.createElement("Set");
		pgst.setAttribute("Name", (*itpgset).Name);
		pgst.setAttribute("FirstPage", (*itpgset).FirstPage);
		pgst.setAttribute("Rows", (*itpgset).Rows);
		pgst.setAttribute("Columns", (*itpgset).Columns);
//		pgst.setAttribute("GapHorizontal", (*itpgset).GapHorizontal);
//		pgst.setAttribute("GapVertical", (*itpgset).GapVertical);
//		pgst.setAttribute("GapBelow", (*itpgset).GapBelow);
		QStringList pNames = (*itpgset).pageNames;
		QStringList::Iterator itpgsetN;
		for(itpgsetN = pNames.begin(); itpgsetN != pNames.end(); ++itpgsetN )
		{
			QDomElement pgstN = docu.createElement("PageNames");
			pgstN.setAttribute("Name", (*itpgsetN));
			pgst.appendChild(pgstN);
		}
		pageSetAttr.appendChild(pgst);
	}
	dc.appendChild(pageSetAttr);
}

void Scribus134Format::writePatterns(QDomDocument & docu, const QString& baseDir) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();
	
	QMap<QString, ScPattern>::Iterator itPat;
	for (itPat = m_Doc->docPatterns.begin(); itPat != m_Doc->docPatterns.end(); ++itPat)
	{
		QDomElement pat = docu.createElement("Pattern");
		pat.setAttribute("Name",itPat.key());
		ScPattern pa = itPat.value();
		pat.setAttribute("width", pa.width);
		pat.setAttribute("height", pa.height);
		pat.setAttribute("scaleX", pa.scaleX);
		pat.setAttribute("scaleY", pa.scaleY);
		WriteObjects(m_Doc, &docu, &pat, baseDir, 0, 0, ItemSelectionPattern, &pa.items);
		dc.appendChild(pat);
	}	
}

void Scribus134Format::writeContent(QDomDocument & docu, const QString& baseDir) 
{
	QDomElement dc=docu.documentElement().firstChild().toElement();

	if (m_mwProgressBar != 0)
	{
		m_mwProgressBar->setMaximum(m_Doc->DocPages.count()+m_Doc->MasterPages.count()+m_Doc->DocItems.count()+m_Doc->MasterItems.count()+m_Doc->FrameItems.count());
		m_mwProgressBar->setValue(0);
	}
	WritePages(m_Doc, &docu, &dc, m_mwProgressBar, 0, true);
	WritePages(m_Doc, &docu, &dc, m_mwProgressBar, m_Doc->MasterPages.count(), false);
	WriteObjects(m_Doc, &docu, &dc, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count(), ItemSelectionFrame);
	WriteObjects(m_Doc, &docu, &dc, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count()+m_Doc->FrameItems.count(), ItemSelectionMaster);
	WriteObjects(m_Doc, &docu, &dc, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count()+m_Doc->MasterItems.count()+m_Doc->FrameItems.count(), ItemSelectionPage);
}

void Scribus134Format::WritePages(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, bool master)
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
			pg = docu->createElement("MASTERPAGE");
			page = doc->MasterPages.at(i);
		}
		else
		{
			pg = docu->createElement("PAGE");
			page = doc->DocPages.at(i);
		}
		pg.setAttribute("PAGEXPOS",page->xOffset());
		pg.setAttribute("PAGEYPOS",page->yOffset());
		pg.setAttribute("PAGEWIDTH",page->width());
		pg.setAttribute("PAGEHEIGHT",page->height());
		pg.setAttribute("BORDERLEFT",page->initialMargins.Left);
		pg.setAttribute("BORDERRIGHT",page->initialMargins.Right);
		pg.setAttribute("BORDERTOP",page->initialMargins.Top);
		pg.setAttribute("BORDERBOTTOM",page->initialMargins.Bottom);
		pg.setAttribute("NUM",page->pageNr());
		pg.setAttribute("NAM",page->pageName());
		pg.setAttribute("MNAM",page->MPageNam);
		pg.setAttribute("Size", page->m_pageSize);
		pg.setAttribute("Orientation", page->PageOri);
		pg.setAttribute("LEFT", page->LeftPg);
		pg.setAttribute("VerticalGuides", GuideManagerIO::writeVerticalGuides(
													page, GuideManagerCore::Standard));
		pg.setAttribute("HorizontalGuides", GuideManagerIO::writeHorizontalGuides(
													page, GuideManagerCore::Standard));
		pg.setAttribute("AGhorizontalAutoGap", page->guides.horizontalAutoGap());
		pg.setAttribute("AGverticalAutoGap", page->guides.verticalAutoGap());
		pg.setAttribute("AGhorizontalAutoCount", page->guides.horizontalAutoCount());
		pg.setAttribute("AGverticalAutoCount", page->guides.verticalAutoCount());
		pg.setAttribute("AGhorizontalAutoRefer", page->guides.horizontalAutoRefer());
		pg.setAttribute("AGverticalAutoRefer", page->guides.verticalAutoRefer());
		pg.setAttribute("AGSelection", GuideManagerIO::writeSelection(page));
		dc->appendChild(pg);
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


void Scribus134Format::writeITEXTs(ScribusDoc *doc, QDomDocument *docu, QDomElement ob, PageItem* item)
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
				QDomElement it=docu->createElement("ITEXT");
				if (item->asPathText())
					putCStylePT(*docu, it, lastStyle);
				else
					putCStyle(*docu, it, lastStyle);
				it.setAttribute("CH", textWithSmartHyphens(item->itemText, lastPos, k));
				ob.appendChild(it);
			}
			lastStyle = style1;
			lastPos = k;
		}

		if (ch == SpecialChars::OBJECT && item->itemText.object(k) != NULL) 
		{
			// each obj in its own ITEXT for now
			QDomElement emb = docu->createElement("ITEXT");
			if (item->asPathText())
				putCStylePT(*docu, emb, lastStyle);
			else
				putCStyle(*docu, emb, lastStyle);
			emb.setAttribute("CH", QString(ch));
			emb.setAttribute("COBJ", item->itemText.object(k)->ItemNr);
			ob.appendChild(emb);			
		}
		else if (ch == SpecialChars::PARSEP)
		{
			// stores also the paragraphstyle for preceding chars
			QDomElement par = docu->createElement("para");
			putPStyle(*docu, par, item->itemText.paragraphStyle(k));
			ob.appendChild(par);
		}
		else if (ch == SpecialChars::TAB) 
		{
			ob.appendChild(docu->createElement("tab"));
		}
		else if (ch == SpecialChars::LINEBREAK)
		{
			ob.appendChild(docu->createElement("breakline"));
		}
		else if (ch == SpecialChars::COLBREAK)
		{
			ob.appendChild(docu->createElement("breakcol"));
		}
		else if (ch == SpecialChars::FRAMEBREAK)
		{
			ob.appendChild(docu->createElement("breakframe"));
		}
		else if (ch == SpecialChars::PAGENUMBER) 
		{
			QDomElement it = docu->createElement("var");
			it.setAttribute("name", "pgno");
			putCStyle(*docu, it, lastStyle);
			ob.appendChild(it);
		}
		else if (ch == SpecialChars::PAGECOUNT) 
		{
			QDomElement it = docu->createElement("var");
			it.setAttribute("name", "pgco");
			putCStyle(*docu, it, lastStyle);
			ob.appendChild(it);
		}
		else if (ch.unicode() < 32 || 
				 (0xd800 <= ch.unicode() && ch.unicode() < 0xe000) ||
				 ch.unicode() == 0xfffe || ch.unicode() == 0xffff)
		{
			QDomElement cod = docu->createElement("ITEXT");
			putCStyle(*docu, cod, lastStyle);
			tmpnum.setNum(ch.unicode());
			cod.setAttribute("Unicode", tmpnum);
			ob.appendChild(cod);			
			
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
		QDomElement it=docu->createElement("ITEXT");
		if (item->asPathText())
			putCStylePT(*docu, it, lastStyle);
		else
			putCStyle(*docu, it, lastStyle);
		it.setAttribute("CH", textWithSmartHyphens(item->itemText, lastPos, item->itemText.length()));
		ob.appendChild(it);
	}
	// paragraphstyle for trailing chars
	if (item->itemText.length() == 0 || item->itemText.text(item->itemText.length()-1) != SpecialChars::PARSEP)
	{
		QDomElement par = docu->createElement("para");
		putPStyle(*docu, par, item->itemText.paragraphStyle(item->itemText.length()));
		ob.appendChild(par);
	}
}

void Scribus134Format::WriteObjects(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, const QString& baseDir, QProgressBar *dia2, uint maxC, ItemSelection master, QList<PageItem*> *some_items)
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
				ob = docu->createElement("MASTEROBJECT");
				break;
			case ItemSelectionPage:
//				item = doc->DocItems.at(j);
				ob = docu->createElement("PAGEOBJECT");
				break;
			case ItemSelectionFrame:
//				item = doc->FrameItems.at(j);
				ob = docu->createElement("FRAMEOBJECT");
				break;
			case ItemSelectionPattern:
				ob = docu->createElement("PatternItem");
				break;
		}
		SetItemProps(&ob, item, baseDir, true);
		ob.setAttribute("OnMasterPage", item->OnMasterPage);
		ob.setAttribute("ImageClip", item->pixm.imgInfo.usedPath);
		ob.setAttribute("ImageRes", item->pixm.imgInfo.lowResType);
		ob.setAttribute("isInline", static_cast<int>(item->isEmbedded));
		ob.setAttribute("fillRule", static_cast<int>(item->fillRule));
		ob.setAttribute("doOverprint", static_cast<int>(item->doOverprint));
		ob.setAttribute("gXpos", item->gXpos);
		ob.setAttribute("gYpos", item->gYpos);
		ob.setAttribute("gWidth", item->gWidth);
		ob.setAttribute("gHeight", item->gHeight);
		if (item->GrType != 0)
		{
			if (item->GrType == 8)
			{
				ob.setAttribute("pattern", item->pattern());
				double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
				item->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
				ob.setAttribute("pScaleX", patternScaleX);
				ob.setAttribute("pScaleY", patternScaleY);
				ob.setAttribute("pOffsetX", patternOffsetX);
				ob.setAttribute("pOffsetY", patternOffsetY);
				ob.setAttribute("pRotation", patternRotation);
			}
			else
			{
				QList<VColorStop*> cstops = item->fill_gradient.colorStops();
				for (uint cst = 0; cst < item->fill_gradient.Stops(); ++cst)
				{
					QDomElement itcl = docu->createElement("CSTOP");
					itcl.setAttribute("RAMP", cstops.at(cst)->rampPoint);
					itcl.setAttribute("NAME", cstops.at(cst)->name);
					itcl.setAttribute("SHADE", cstops.at(cst)->shade);
					itcl.setAttribute("TRANS", cstops.at(cst)->opacity);
					ob.appendChild(itcl);
				}
				ob.setAttribute("GRSTARTX", item->GrStartX);
				ob.setAttribute("GRSTARTY", item->GrStartY);
				ob.setAttribute("GRENDX", item->GrEndX);
				ob.setAttribute("GRENDY", item->GrEndY);
			}
		}
		if (item->effectsInUse.count() != 0)
		{
			for (int a = 0; a < item->effectsInUse.count(); ++a)
			{
				QDomElement imeff = docu->createElement("ImageEffect");
				imeff.setAttribute("Code", item->effectsInUse.at(a).effectCode);
				imeff.setAttribute("Param", item->effectsInUse.at(a).effectParameters);
				ob.appendChild(imeff);
			}
		}

		if (item->itemText.defaultStyle().tabValues().count() != 0)
		{
			for (int a = 0; a < item->itemText.defaultStyle().tabValues().count(); ++a)
			{
				QDomElement tabs = docu->createElement("Tabs");
				tabs.setAttribute("Type", (item->itemText.defaultStyle().tabValues().at(a)).tabType);
				tabs.setAttribute("Pos", (item->itemText.defaultStyle().tabValues().at(a)).tabPosition);
				QString tabCh = "";
				if (!(item->itemText.defaultStyle().tabValues().at(a)).tabFillChar.isNull())
					tabCh = QString((item->itemText.defaultStyle().tabValues().at(a)).tabFillChar);
				tabs.setAttribute("Fill", tabCh);
				ob.appendChild(tabs);
			}
		}
		if (((item->asImageFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()) && (item->pixm.imgInfo.layerInfo.count() != 0) && (item->pixm.imgInfo.isRequest))
		{
			QMap<int, ImageLoadRequest>::iterator it2;
			for (it2 = item->pixm.imgInfo.RequestProps.begin(); it2 != item->pixm.imgInfo.RequestProps.end(); ++it2)
			{
				QDomElement psd = docu->createElement("PSDLayer");
				psd.setAttribute("Layer",it2.key());
				psd.setAttribute("Visible", static_cast<int>(it2.value().visible));
				psd.setAttribute("useMask", static_cast<int>(it2.value().useMask));
				psd.setAttribute("Opacity", it2.value().opacity);
				psd.setAttribute("Blend", it2.value().blend);
				ob.appendChild(psd);
			}
		}
		if (item->itemText.defaultStyle().hasParent())
			ob.setAttribute("PSTYLE", item->itemText.defaultStyle().parent());
		if (! item->itemText.defaultStyle().isInhAlignment())
			ob.setAttribute("ALIGN", item->itemText.defaultStyle().alignment());
		
		ob.setAttribute("BOOKMARK", item->isBookmark ? 1 : 0);

		if (item->nextInChain() != 0)
			ob.setAttribute("NEXTITEM", item->nextInChain()->ItemNr);
		else
			ob.setAttribute("NEXTITEM", -1);
		ob.setAttribute("LAYER", item->LayerNr);

		if (item->prevInChain() != 0 && items->contains(item->prevInChain()))
			ob.setAttribute("BACKITEM", item->prevInChain()->ItemNr);
		else
		{
			ob.setAttribute("BACKITEM", -1);
			writeITEXTs(doc, docu, ob, item); 
		}
		
		if (item->asLatexFrame()) {
			QDomElement latexinfo = docu->createElement("LATEX");
			PageItem_LatexFrame *latexitem = item->asLatexFrame();
			latexinfo.setAttribute("ConfigFile", latexitem->configFile());
			latexinfo.setAttribute("DPI", latexitem->dpi());
			latexinfo.setAttribute("USE_PREAMBLE", latexitem->usePreamble());
			QMapIterator<QString, QString> i(latexitem->editorProperties);
			while (i.hasNext()) {
				i.next();
				QDomElement property = docu->createElement("PROPERTY");
				property.setAttribute("name", i.key());
				property.setAttribute("value", i.value());
				latexinfo.appendChild(property);
			}
			QDomText latextext = docu->createTextNode(latexitem->formula());
			latexinfo.appendChild(latextext);
			ob.appendChild(latexinfo);
		}

		//CB PageItemAttributes
		QDomElement docItemAttrs = docu->createElement("PageItemAttributes");
		ObjAttrVector *attributes=item->getObjectAttributes();
		for(ObjAttrVector::Iterator objAttrIt = attributes->begin() ; objAttrIt != attributes->end(); ++objAttrIt )
		{
			QDomElement itemAttr = docu->createElement("ItemAttribute");
			itemAttr.setAttribute("Name", (*objAttrIt).name);
			itemAttr.setAttribute("Type", (*objAttrIt).type);
			itemAttr.setAttribute("Value", (*objAttrIt).value);
			itemAttr.setAttribute("Parameter", (*objAttrIt).parameter);
			itemAttr.setAttribute("Relationship", (*objAttrIt).relationship);
			itemAttr.setAttribute("RelationshipTo", (*objAttrIt).relationshipto);
			itemAttr.setAttribute("AutoAddTo", (*objAttrIt).autoaddto);
			docItemAttrs.appendChild(itemAttr);
		}
		ob.appendChild(docItemAttrs);

		dc->appendChild(ob);
	}
}

void Scribus134Format::SetItemProps(QDomElement *ob, PageItem* item, const QString& baseDir, bool newFormat)
{
	double xf, yf;
	QString tmp, tmpy;
	if (newFormat)
		ob->setAttribute("OwnPage", item->OwnPage);
	ob->setAttribute("PTYPE",item->realItemType());
	ob->setAttribute("XPOS",item->xPos());
	ob->setAttribute("YPOS",item->yPos());
	ob->setAttribute("WIDTH",item->width());
	ob->setAttribute("HEIGHT",item->height());
	ob->setAttribute("RADRECT",item->cornerRadius());
	ob->setAttribute("FRTYPE", item->FrameType);
	ob->setAttribute("CLIPEDIT", item->ClipEdited ? 1 : 0);
	ob->setAttribute("PWIDTH",item->lineWidth());
	ob->setAttribute("PCOLOR",item->fillColor());
	ob->setAttribute("PCOLOR2",item->lineColor());
	if ( ! item->itemText.defaultStyle().charStyle().isInhFillColor())
		ob->setAttribute("TXTFILL",item->itemText.defaultStyle().charStyle().fillColor());
	if ( ! item->itemText.defaultStyle().charStyle().isInhStrokeColor())
		ob->setAttribute("TXTSTROKE",item->itemText.defaultStyle().charStyle().strokeColor());
	if ( ! item->itemText.defaultStyle().charStyle().isInhStrokeShade())
		ob->setAttribute("TXTSTRSH",item->itemText.defaultStyle().charStyle().strokeShade());
	if ( ! item->itemText.defaultStyle().charStyle().isInhFillShade())
		ob->setAttribute("TXTFILLSH",item->itemText.defaultStyle().charStyle().fillShade());
	if ( ! item->itemText.defaultStyle().charStyle().isInhScaleH())
		ob->setAttribute("TXTSCALE",item->itemText.defaultStyle().charStyle().scaleH() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhScaleV())
		ob->setAttribute("TXTSCALEV",item->itemText.defaultStyle().charStyle().scaleV() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhBaselineOffset())
		ob->setAttribute("TXTBASE",item->itemText.defaultStyle().charStyle().baselineOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhShadowXOffset())
		ob->setAttribute("TXTSHX",item->itemText.defaultStyle().charStyle().shadowXOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhShadowYOffset())
		ob->setAttribute("TXTSHY",item->itemText.defaultStyle().charStyle().shadowYOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhOutlineWidth())
		ob->setAttribute("TXTOUT",item->itemText.defaultStyle().charStyle().outlineWidth() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhUnderlineOffset())
		ob->setAttribute("TXTULP",item->itemText.defaultStyle().charStyle().underlineOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhUnderlineWidth())
		ob->setAttribute("TXTULW",item->itemText.defaultStyle().charStyle().underlineWidth() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhStrikethruOffset())
		ob->setAttribute("TXTSTP",item->itemText.defaultStyle().charStyle().strikethruOffset() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhStrikethruWidth())
		ob->setAttribute("TXTSTW",item->itemText.defaultStyle().charStyle().strikethruWidth() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhTracking())
		ob->setAttribute("TXTKERN",item->itemText.defaultStyle().charStyle().tracking() / 10.0);
	if ( ! item->itemText.defaultStyle().charStyle().isInhWordTracking())
		ob->setAttribute("wordTrack",item->itemText.defaultStyle().charStyle().wordTracking());
	if ( ! item->itemText.defaultStyle().isInhMinWordTracking())
		ob->setAttribute("MinWordTrack", item->itemText.defaultStyle().minWordTracking());
	if ( ! item->itemText.defaultStyle().isInhMinGlyphExtension())
		ob->setAttribute("MinGlyphShrink", item->itemText.defaultStyle().minGlyphExtension());
	if ( ! item->itemText.defaultStyle().isInhMaxGlyphExtension())
		ob->setAttribute("MaxGlyphExtend", item->itemText.defaultStyle().maxGlyphExtension());
	if ( ! item->itemText.defaultStyle().isInhOpticalMargins())
		ob->setAttribute("OpticalMargins", item->itemText.defaultStyle().opticalMargins());
	if ( ! item->itemText.defaultStyle().isInhHyphenationMode())
		ob->setAttribute("HyphenationMode", item->itemText.defaultStyle().hyphenationMode());
	ob->setAttribute("COLUMNS", item->columns());
	ob->setAttribute("COLGAP", item->columnGap());
	ob->setAttribute("NAMEDLST",item->NamedLStyle);
	ob->setAttribute("SHADE",item->fillShade());
	ob->setAttribute("SHADE2",item->lineShade());
	ob->setAttribute("GRTYP",item->GrType);
	ob->setAttribute("ROT",item->rotation());
	ob->setAttribute("PLINEART",item->PLineArt);
	ob->setAttribute("PLINEEND", item->PLineEnd);
	ob->setAttribute("PLINEJOIN", item->PLineJoin);
	if ( ! item->itemText.defaultStyle().isInhLineSpacing())
		ob->setAttribute("LINESP",item->itemText.defaultStyle().lineSpacing());
	if ( ! item->itemText.defaultStyle().isInhLineSpacingMode())
		ob->setAttribute("LINESPMode", item->itemText.defaultStyle().lineSpacingMode());
	ob->setAttribute("LOCALSCX",item->imageXScale());
	ob->setAttribute("LOCALSCY",item->imageYScale());
	ob->setAttribute("LOCALX",item->imageXOffset());
	ob->setAttribute("LOCALY",item->imageYOffset());
	ob->setAttribute("PICART", item->imageShown() ? 1 : 0);
	ob->setAttribute("PLTSHOW", item->PoShow ? 1 : 0);
	ob->setAttribute("BASEOF", item->BaseOffs);
	ob->setAttribute("textPathType", item->textPathType);
	ob->setAttribute("textPathFlipped", static_cast<int>(item->textPathFlipped));
	ob->setAttribute("FLIPPEDH", item->imageFlippedH());
	ob->setAttribute("FLIPPEDV", item->imageFlippedV());
/*	ob->setAttribute("BBOXX",item->BBoxX);
	ob->setAttribute("BBOXH",item->BBoxH); */
	if ( ! item->itemText.defaultStyle().charStyle().isInhFont())
		ob->setAttribute("IFONT",item->itemText.defaultStyle().charStyle().font().scName());
	if ( ! item->itemText.defaultStyle().charStyle().isInhFontSize())
		ob->setAttribute("ISIZE",item->itemText.defaultStyle().charStyle().fontSize() / 10.0 );
	ob->setAttribute("SCALETYPE", item->ScaleType ? 1 : 0);
	ob->setAttribute("RATIO", item->AspectRatio ? 1 : 0);
	ob->setAttribute("PRINTABLE", item->printEnabled() ? 1 : 0);
	if(item->isAnnotation())
	{
		ob->setAttribute("ANNOTATION",1);
		ob->setAttribute("ANTYPE", item->annotation().Type());
		ob->setAttribute("ANACTION", item->annotation().Action());
		ob->setAttribute("ANEACT", item->annotation().E_act());
		ob->setAttribute("ANXACT", item->annotation().X_act());
		ob->setAttribute("ANDACT", item->annotation().D_act());
		ob->setAttribute("ANFOACT", item->annotation().Fo_act());
		ob->setAttribute("ANBLACT", item->annotation().Bl_act());
		ob->setAttribute("ANKACT", item->annotation().K_act());
		ob->setAttribute("ANFACT", item->annotation().F_act());
		ob->setAttribute("ANVACT", item->annotation().V_act());
		ob->setAttribute("ANCACT", item->annotation().C_act());
		if (item->annotation().ActionType() == 8)
			ob->setAttribute("ANEXTERN", item->annotation().Extern());
		else
			ob->setAttribute("ANEXTERN", Path2Relative(item->annotation().Extern(), baseDir));
		ob->setAttribute("ANZIEL", item->annotation().Ziel());
		ob->setAttribute("ANACTYP", item->annotation().ActionType());
		ob->setAttribute("ANTOOLTIP", item->annotation().ToolTip());
		ob->setAttribute("ANBWID", item->annotation().Bwid());
		ob->setAttribute("ANBSTY", item->annotation().Bsty());
		ob->setAttribute("ANFEED", item->annotation().Feed());
		ob->setAttribute("ANFLAG", item->annotation().Flag());
		ob->setAttribute("ANFONT", item->annotation().Font());
		ob->setAttribute("ANFORMAT", item->annotation().Format());
		ob->setAttribute("ANROLL", item->annotation().RollOver());
		ob->setAttribute("ANDOWN", item->annotation().Down());
		ob->setAttribute("ANVIS", item->annotation().Vis());
		ob->setAttribute("ANMC", item->annotation().MaxChar());
		ob->setAttribute("ANCHK", item->annotation().IsChk());
		ob->setAttribute("ANAA", item->annotation().AAact());
		ob->setAttribute("ANCHKS", item->annotation().ChkStil());
		ob->setAttribute("ANBCOL", item->annotation().borderColor());
		ob->setAttribute("ANHTML", item->annotation().HTML());
		ob->setAttribute("ANICON", item->annotation().UseIcons());
		ob->setAttribute("ANPLACE", item->annotation().IPlace());
		ob->setAttribute("ANSCALE", item->annotation().ScaleW());
	}
	else
		ob->setAttribute("ANNOTATION",0);
	ob->setAttribute("ANNAME", !item->AutoName ? item->itemName() : QString(""));
	// "TEXTFLOWMODE" succeed to "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes
	ob->setAttribute("TEXTFLOWMODE", (int) item->textFlowMode() );
	// Set "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes for compatibility
	// with versions prior to 1.3.4
	ob->setAttribute("TEXTFLOW" , item->textFlowAroundObject() ? 1 : 0);
	ob->setAttribute("TEXTFLOW2", item->textFlowUsesBoundingBox() ? 1 : 0);
	ob->setAttribute("TEXTFLOW3", item->textFlowUsesContourLine() ? 1 : 0);
	ob->setAttribute("AUTOTEXT", item->isAutoText ? 1 : 0);
	ob->setAttribute("EXTRA",item->textToFrameDistLeft());
	ob->setAttribute("TEXTRA",item->textToFrameDistTop());
	ob->setAttribute("BEXTRA",item->textToFrameDistBottom());
	ob->setAttribute("REXTRA",item->textToFrameDistRight());
	if (((item->asImageFrame() && !item->asLatexFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
		ob->setAttribute("PFILE",Path2Relative(item->Pfile, baseDir));
	else
		ob->setAttribute("PFILE","");
	if (!item->Pfile2.isEmpty())
		ob->setAttribute("PFILE2",Path2Relative(item->Pfile2, baseDir));
	else
		ob->setAttribute("PFILE2","");
	if (!item->Pfile3.isEmpty())
		ob->setAttribute("PFILE3",Path2Relative(item->Pfile3, baseDir));
	else
		ob->setAttribute("PFILE3","");
	ob->setAttribute("PRFILE",item->IProfile);
	ob->setAttribute("EPROF", item->EmProfile);
	ob->setAttribute("IRENDER",item->IRender);
	ob->setAttribute("EMBEDDED", item->UseEmbedded ? 1 : 0);
	ob->setAttribute("LOCK", item->locked() ? 1 : 0);
	ob->setAttribute("LOCKR", item->sizeLocked() ? 1 : 0);
	ob->setAttribute("REVERS", item->reversed() ? 1 : 0);
	ob->setAttribute("TransValue", item->fillTransparency());
	ob->setAttribute("TransValueS", item->lineTransparency());
	ob->setAttribute("TransBlend", item->fillBlendmode());
	ob->setAttribute("TransBlendS", item->lineBlendmode());
	ob->setAttribute("isTableItem", static_cast<int>(item->isTableItem));
	ob->setAttribute("TopLine", static_cast<int>(item->TopLine));
	ob->setAttribute("LeftLine", static_cast<int>(item->LeftLine));
	ob->setAttribute("RightLine", static_cast<int>(item->RightLine));
	ob->setAttribute("BottomLine", static_cast<int>(item->BottomLine));
	if (item->isTableItem)
	{
		if (item->TopLink != 0)
			ob->setAttribute("TopLINK", item->TopLink->ItemNr);
		else
			ob->setAttribute("TopLINK", -1);
		if (item->LeftLink != 0)
			ob->setAttribute("LeftLINK", item->LeftLink->ItemNr);
		else
			ob->setAttribute("LeftLINK", -1);
		if (item->RightLink != 0)
			ob->setAttribute("RightLINK", item->RightLink->ItemNr);
		else
			ob->setAttribute("RightLINK", -1);
		if (item->BottomLink != 0)
			ob->setAttribute("BottomLINK", item->BottomLink->ItemNr);
		else
			ob->setAttribute("BottomLINK", -1);
		ob->setAttribute("OwnLINK", item->ItemNr);
	}
	ob->setAttribute("isGroupControl", static_cast<int>(item->isGroupControl));
	if (item->isGroupControl)
	{
		if (item->groupsLastItem != 0)
			ob->setAttribute("groupsLastItem", item->groupsLastItem->ItemNr - item->ItemNr);
	}
	ob->setAttribute("NUMDASH", static_cast<int>(item->DashValues.count()));
	QString dlp = "";
	QList<double>::Iterator dax;
	for (dax = item->DashValues.begin(); dax != item->DashValues.end(); ++dax)
		dlp += tmp.setNum((*dax)) + " ";
	ob->setAttribute("DASHS", dlp);
	ob->setAttribute("DASHOFF", item->DashOffset);
	ob->setAttribute("NUMPO",item->PoLine.size());
	QString polp = "";
	for (uint nxx=0; nxx<item->PoLine.size(); ++nxx)
	{
		item->PoLine.point(nxx, &xf, &yf);
		polp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	ob->setAttribute("POCOOR", polp);
	ob->setAttribute("NUMCO",item->ContourLine.size());
	QString colp = "";
	for (uint nxx=0; nxx<item->ContourLine.size(); ++nxx)
	{
		item->ContourLine.point(nxx, &xf, &yf);
		colp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	ob->setAttribute("COCOOR", colp);
	ob->setAttribute("NUMGROUP", static_cast<int>(item->Groups.count()));
	QString glp = "";
	QStack<int>::Iterator nx;
	for (nx = item->Groups.begin(); nx != item->Groups.end(); ++nx)
		glp += tmp.setNum((*nx)) + " ";
	ob->setAttribute("GROUPS", glp);
	if ( ! item->itemText.defaultStyle().charStyle().isInhLanguage())
		ob->setAttribute("LANGUAGE", item->itemText.defaultStyle().charStyle().language());
	ob->setAttribute("startArrowIndex", item->startArrowIndex());
	ob->setAttribute("endArrowIndex", item->endArrowIndex());
}
