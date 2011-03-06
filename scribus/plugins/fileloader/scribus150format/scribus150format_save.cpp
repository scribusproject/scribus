/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus150format.h"
#include "scribus150formatimpl.h"

#include <ctime>
#include <memory>

#include "../../formatidlist.h"
#include "commonstrings.h"
#include "ui/missing.h"
#include "prefsmanager.h"
#include "resourcecollection.h"
#include "scconfig.h"
#include "scpattern.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "hyphenator.h"
#include "pageitem_latexframe.h"
#ifdef HAVE_OSG
	#include "pageitem_osgframe.h"
#endif
#include "pageitem_regularpolygon.h"
#include "pageitem_arc.h"
#include "pageitem_spiral.h"

#include "units.h"
#include "util.h"
#include "util_math.h"
#include "util_color.h"
#include "scgzfile.h"
#include <QCursor>
#include <QFileInfo>
#include <QList>
#include <QDataStream>

#include "scxmlstreamwriter.h"

QString Scribus150Format::saveElements(double xp, double yp, double wp, double hp, Selection* selection, QByteArray &prevData)
{
	QString fileDir = QDir::homePath();
	QString documentStr;
	documentStr.reserve(524288);
	ScXmlStreamWriter writer(&documentStr);
	writer.setAutoFormatting(true);
	writer.writeStartElement("SCRIBUSELEMUTF8");
	writer.writeAttribute("XP", xp);
	writer.writeAttribute("YP", yp);
	writer.writeAttribute("W", wp);
	writer.writeAttribute("H", hp);
	writer.writeAttribute("COUNT",   selection->count());
	writer.writeAttribute("Version", QString(VERSION));
	writer.writeAttribute("previewData", QString(prevData));
	writeColors(writer, true);
	writeGradients(writer, true);
	ResourceCollection lists;
	QList<PageItem*> emG;
	for (int cor = 0; cor < selection->count(); ++cor)
	{
		selection->itemAt(cor)->getNamedResources(lists);
		emG.append(selection->itemAt(cor));
	}
	QList<QString>::Iterator it;
	QList<QString> names = lists.styleNames();
	QList<int> styleList = m_Doc->getSortedStyleList();
	for (int a = 0; a < styleList.count(); ++a)
	{
		if (names.contains(m_Doc->paragraphStyles()[styleList[a]].name()))
			putPStyle(writer, m_Doc->paragraphStyles()[styleList[a]], "STYLE");
	}
//	for (it = names.begin(); it != names.end(); ++it)
//	{
//		putPStyle(writer, m_Doc->paragraphStyles().get(*it), "STYLE");
//	}
	names = lists.charStyleNames();
	styleList = m_Doc->getSortedCharStyleList();
	for (int a = 0; a < styleList.count(); ++a)
	{
		if (names.contains(m_Doc->charStyles()[styleList[a]].name()))
		{
			writer.writeStartElement("CHARSTYLE");
			putNamedCStyle(writer, m_Doc->charStyles()[styleList[a]]);
			writer.writeEndElement();
		}
	}
//	for (it = names.begin(); it != names.end(); ++it)
//	{
//		writer.writeStartElement("CHARSTYLE");
//		putNamedCStyle(writer, m_Doc->charStyles().get(*it));
//		writer.writeEndElement();
//	}
/*	names = lists.lineStyleNames();
	for (it = names.begin(); it != names.end(); ++it)
	{
		writeLinestyles(writer, true, *it);
	} */
	writeLinestyles(writer);
	writePatterns(writer, fileDir, true, selection);
	WriteObjects(m_Doc, writer, fileDir, 0, 0, ItemSelectionElements, &emG);
	writer.writeEndElement();
	writer.writeEndDocument();
	documentStr.squeeze();
	return documentStr;
}

bool Scribus150Format::savePalette(const QString & fileName)
{
	QString fileDir = QFileInfo(fileName).absolutePath();
	std::auto_ptr<QIODevice> outputFile;
	outputFile.reset( new QFile(fileName) );
	if (!outputFile->open(QIODevice::WriteOnly))
		return false;
	ScXmlStreamWriter docu;
	docu.setAutoFormatting(true);
	docu.setDevice(outputFile.get());
	docu.writeStartDocument();
	docu.writeStartElement("SCRIBUSCOLORS");
	writeColors(docu);
	writeGradients(docu);
	writePatterns(docu, fileDir);
	docu.writeEndElement();
	docu.writeEndDocument();
	bool  writeSucceed = false;
	const QFile* qFile = dynamic_cast<QFile*>(outputFile.get());
	writeSucceed = (qFile->error() == QFile::NoError);
	outputFile->close();
	return writeSucceed;
}

bool Scribus150Format::saveFile(const QString & fileName, const FileFormat & /* fmt */)
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
	docu.writeAttribute("PAGEWIDTH"   , m_Doc->pageWidth());
	docu.writeAttribute("PAGEHEIGHT"  , m_Doc->pageHeight());
	docu.writeAttribute("BORDERLEFT"  , m_Doc->margins()->Left);
	docu.writeAttribute("BORDERRIGHT" , m_Doc->margins()->Right);
	docu.writeAttribute("BORDERTOP"   , m_Doc->margins()->Top);
	docu.writeAttribute("BORDERBOTTOM", m_Doc->margins()->Bottom);
	docu.writeAttribute("PRESET"      , m_Doc->marginPreset());
	docu.writeAttribute("BleedTop"    , m_Doc->bleeds()->Top);
	docu.writeAttribute("BleedLeft"   , m_Doc->bleeds()->Left);
	docu.writeAttribute("BleedRight"  , m_Doc->bleeds()->Right);
	docu.writeAttribute("BleedBottom" , m_Doc->bleeds()->Bottom);
	docu.writeAttribute("ORIENTATION" , m_Doc->pageOrientation());
	docu.writeAttribute("PAGESIZE"    , m_Doc->pageSize());
	docu.writeAttribute("FIRSTNUM"    , m_Doc->FirstPnum);
	docu.writeAttribute("BOOK"        , m_Doc->pagePositioning());
	if(m_Doc->usesAutomaticTextFrames())
		docu.writeAttribute("AUTOTEXT", 1);
	docu.writeAttribute("AUTOSPALTEN" ,m_Doc->PageSp);
	docu.writeAttribute("ABSTSPALTEN" ,m_Doc->PageSpa);
	docu.writeAttribute("UNITS"       , m_Doc->unitIndex());
	docu.writeAttribute("DFONT"       ,m_Doc->itemToolPrefs().textFont);
	docu.writeAttribute("DSIZE"       ,m_Doc->itemToolPrefs().textSize / 10.0);
	docu.writeAttribute("DCOL"        ,m_Doc->itemToolPrefs().textColumns);
	docu.writeAttribute("DGAP"        ,m_Doc->itemToolPrefs().textColumnGap);
	docu.writeAttribute("TabFill"     ,m_Doc->itemToolPrefs().textTabFillChar);
	docu.writeAttribute("TabWidth"    ,m_Doc->itemToolPrefs().textTabWidth);
	docu.writeAttribute("AUTHOR"      ,m_Doc->documentInfo().author());
	docu.writeAttribute("COMMENTS"    ,m_Doc->documentInfo().comments());
	docu.writeAttribute("KEYWORDS"    ,m_Doc->documentInfo().keywords());
	docu.writeAttribute("PUBLISHER",m_Doc->documentInfo().publisher());
	docu.writeAttribute("DOCDATE",m_Doc->documentInfo().date());
	docu.writeAttribute("DOCTYPE",m_Doc->documentInfo().type());
	docu.writeAttribute("DOCFORMAT",m_Doc->documentInfo().format());
	docu.writeAttribute("DOCIDENT",m_Doc->documentInfo().ident());
	docu.writeAttribute("DOCSOURCE",m_Doc->documentInfo().source());
	docu.writeAttribute("DOCLANGINFO",m_Doc->documentInfo().langInfo());
	docu.writeAttribute("DOCRELATION",m_Doc->documentInfo().relation());
	docu.writeAttribute("DOCCOVER",m_Doc->documentInfo().cover());
	docu.writeAttribute("DOCRIGHTS",m_Doc->documentInfo().rights());
	docu.writeAttribute("DOCCONTRIB",m_Doc->documentInfo().contrib());
	docu.writeAttribute("TITLE",m_Doc->documentInfo().title());
	docu.writeAttribute("SUBJECT",m_Doc->documentInfo().subject());
	docu.writeAttribute("VHOCH"  , m_Doc->typographicPrefs().valueSuperScript);
	docu.writeAttribute("VHOCHSC", m_Doc->typographicPrefs().scalingSuperScript);
	docu.writeAttribute("VTIEF"  , m_Doc->typographicPrefs().valueSubScript);
	docu.writeAttribute("VTIEFSC", m_Doc->typographicPrefs().scalingSubScript);
	docu.writeAttribute("VKAPIT" , m_Doc->typographicPrefs().valueSmallCaps);
	docu.writeAttribute("BASEGRID",m_Doc->guidesPrefs().valueBaselineGrid);
	docu.writeAttribute("BASEO"  , m_Doc->guidesPrefs().offsetBaselineGrid);
	docu.writeAttribute("AUTOL"  , m_Doc->typographicPrefs().autoLineSpacing);
	docu.writeAttribute("UnderlinePos"   , m_Doc->typographicPrefs().valueUnderlinePos);
	docu.writeAttribute("UnderlineWidth" , m_Doc->typographicPrefs().valueUnderlineWidth);
	docu.writeAttribute("StrikeThruPos"  , m_Doc->typographicPrefs().valueStrikeThruPos);
	docu.writeAttribute("StrikeThruWidth", m_Doc->typographicPrefs().valueStrikeThruWidth);
	docu.writeAttribute("GROUPC",m_Doc->GroupCounter);
	docu.writeAttribute("HCMS" , static_cast<int>(m_Doc->HasCMS));
	docu.writeAttribute("DPSo" , static_cast<int>(m_Doc->cmsSettings().SoftProofOn));
	docu.writeAttribute("DPSFo", static_cast<int>(m_Doc->cmsSettings().SoftProofFullOn));
	docu.writeAttribute("DPuse", static_cast<int>(m_Doc->cmsSettings().CMSinUse));
	docu.writeAttribute("DPgam", static_cast<int>(m_Doc->cmsSettings().GamutCheck));
	docu.writeAttribute("DPbla", static_cast<int>(m_Doc->cmsSettings().BlackPoint));
	docu.writeAttribute("DPMo",m_Doc->cmsSettings().DefaultMonitorProfile);
	docu.writeAttribute("DPPr",m_Doc->cmsSettings().DefaultPrinterProfile);
	docu.writeAttribute("DPIn",m_Doc->cmsSettings().DefaultImageRGBProfile);
	docu.writeAttribute("DPInCMYK",m_Doc->cmsSettings().DefaultImageCMYKProfile);
	docu.writeAttribute("DPIn2",m_Doc->cmsSettings().DefaultSolidColorRGBProfile);
	docu.writeAttribute("DPIn3",m_Doc->cmsSettings().DefaultSolidColorCMYKProfile);
	docu.writeAttribute("DISc",m_Doc->cmsSettings().DefaultIntentColors);
	docu.writeAttribute("DIIm",m_Doc->cmsSettings().DefaultIntentImages);
	docu.writeAttribute("ALAYER", m_Doc->activeLayer());
	docu.writeAttribute("LANGUAGE", m_Doc->hyphLanguage());
	docu.writeAttribute("MINWORDLEN", m_Doc->hyphMinimumWordLength());
	docu.writeAttribute("HYCOUNT", m_Doc->hyphConsecutiveLines());
	docu.writeAttribute("AUTOMATIC", static_cast<int>(m_Doc->hyphAutomatic()));
	docu.writeAttribute("AUTOCHECK", static_cast<int>(m_Doc->hyphAutoCheck()));
	docu.writeAttribute("GUIDELOCK", static_cast<int>(m_Doc->GuideLock));
	docu.writeAttribute("SnapToGuides", static_cast<int>(m_Doc->SnapGuides));
	docu.writeAttribute("SnapToGrid", static_cast<int>(m_Doc->useRaster));
	docu.writeAttribute("MINGRID", m_Doc->guidesPrefs().minorGridSpacing);
	docu.writeAttribute("MAJGRID", m_Doc->guidesPrefs().majorGridSpacing);
	docu.writeAttribute("SHOWGRID", static_cast<int>(m_Doc->guidesPrefs().gridShown));
	docu.writeAttribute("SHOWGUIDES", static_cast<int>(m_Doc->guidesPrefs().guidesShown));
	docu.writeAttribute("showcolborders", static_cast<int>(m_Doc->guidesPrefs().colBordersShown));
	docu.writeAttribute("SHOWFRAME", static_cast<int>(m_Doc->guidesPrefs().framesShown));
	docu.writeAttribute("SHOWLAYERM", static_cast<int>(m_Doc->guidesPrefs().layerMarkersShown));
	docu.writeAttribute("SHOWMARGIN", static_cast<int>(m_Doc->guidesPrefs().marginsShown));
	docu.writeAttribute("SHOWBASE", static_cast<int>(m_Doc->guidesPrefs().baselineGridShown));
	docu.writeAttribute("SHOWPICT", static_cast<int>(m_Doc->guidesPrefs().showPic));
	docu.writeAttribute("SHOWControl", static_cast<int>(m_Doc->guidesPrefs().showControls));
	docu.writeAttribute("SHOWLINK", static_cast<int>(m_Doc->guidesPrefs().linkShown));
	docu.writeAttribute("rulerMode", static_cast<int>(m_Doc->guidesPrefs().rulerMode));
	docu.writeAttribute("showrulers", static_cast<int>(m_Doc->guidesPrefs().rulersShown));
	docu.writeAttribute("showBleed", static_cast<int>(m_Doc->guidesPrefs().showBleed));
	docu.writeAttribute("rulerXoffset", m_Doc->rulerXoffset);
	docu.writeAttribute("rulerYoffset", m_Doc->rulerYoffset);
	docu.writeAttribute("GuideRad", m_Doc->guidesPrefs().guideRad);
	docu.writeAttribute("GRAB",m_Doc->guidesPrefs().grabRadius);
	docu.writeAttribute("POLYC", m_Doc->itemToolPrefs().polyCorners);
	docu.writeAttribute("POLYF", m_Doc->itemToolPrefs().polyFactor);
	docu.writeAttribute("POLYR", m_Doc->itemToolPrefs().polyRotation);
	docu.writeAttribute("POLYIR", m_Doc->itemToolPrefs().polyInnerRot);
	docu.writeAttribute("POLYCUR", m_Doc->itemToolPrefs().polyCurvature);
	docu.writeAttribute("POLYOCUR", m_Doc->itemToolPrefs().polyOuterCurvature);
	docu.writeAttribute("POLYS", static_cast<int>(m_Doc->itemToolPrefs().polyUseFactor));
	docu.writeAttribute("arcStartAngle", m_Doc->itemToolPrefs().arcStartAngle);
	docu.writeAttribute("arcSweepAngle", m_Doc->itemToolPrefs().arcSweepAngle);
	docu.writeAttribute("spiralStartAngle", m_Doc->itemToolPrefs().spiralStartAngle);
	docu.writeAttribute("spiralEndAngle", m_Doc->itemToolPrefs().spiralEndAngle);
	docu.writeAttribute("spiralFactor", m_Doc->itemToolPrefs().spiralFactor);
	docu.writeAttribute("AutoSave", static_cast<int>(m_Doc->autoSave()));
	docu.writeAttribute("AutoSaveTime", m_Doc->autoSaveTime());
	docu.writeAttribute("ScratchBottom", m_Doc->scratch()->Bottom);
	docu.writeAttribute("ScratchLeft", m_Doc->scratch()->Left);
	docu.writeAttribute("ScratchRight", m_Doc->scratch()->Right);
	docu.writeAttribute("ScratchTop", m_Doc->scratch()->Top);
	docu.writeAttribute("GapHorizontal", m_Doc->pageGapHorizontal());
	docu.writeAttribute("GapVertical", m_Doc->pageGapVertical());
	docu.writeAttribute("StartArrow", m_Doc->itemToolPrefs().lineStartArrow);
	docu.writeAttribute("EndArrow", m_Doc->itemToolPrefs().lineEndArrow);
	docu.writeAttribute("PEN",m_Doc->itemToolPrefs().shapeLineColor);
	docu.writeAttribute("BRUSH",m_Doc->itemToolPrefs().shapeFillColor);
	docu.writeAttribute("PENLINE",m_Doc->itemToolPrefs().lineColor);
	docu.writeAttribute("PENTEXT",m_Doc->itemToolPrefs().textColor);
	docu.writeAttribute("StrokeText",m_Doc->itemToolPrefs().textStrokeColor);
	docu.writeAttribute("TextBackGround", m_Doc->itemToolPrefs().textFillColor);
	docu.writeAttribute("TextLineColor", m_Doc->itemToolPrefs().textLineColor);
	docu.writeAttribute("TextBackGroundShade", m_Doc->itemToolPrefs().textFillColorShade);
	docu.writeAttribute("TextLineShade", m_Doc->itemToolPrefs().textLineColorShade);
	docu.writeAttribute("TextPenShade", m_Doc->itemToolPrefs().textShade);
	docu.writeAttribute("TextStrokeShade", m_Doc->itemToolPrefs().textStrokeShade);
	docu.writeAttribute("STIL",m_Doc->itemToolPrefs().shapeLineStyle);
	docu.writeAttribute("STILLINE",m_Doc->itemToolPrefs().lineStyle);
	docu.writeAttribute("WIDTH",m_Doc->itemToolPrefs().shapeLineWidth);
	docu.writeAttribute("WIDTHLINE",m_Doc->itemToolPrefs().lineWidth);
	docu.writeAttribute("PENSHADE",m_Doc->itemToolPrefs().shapeLineColorShade);
	docu.writeAttribute("LINESHADE",m_Doc->itemToolPrefs().lineColorShade);
	docu.writeAttribute("BRUSHSHADE",m_Doc->itemToolPrefs().shapeFillColorShade);
	docu.writeAttribute("MAGMIN",m_Doc->opToolPrefs().magMin);
	docu.writeAttribute("MAGMAX",m_Doc->opToolPrefs().magMax);
	docu.writeAttribute("MAGSTEP",m_Doc->opToolPrefs().magStep);
	docu.writeAttribute("CPICT",m_Doc->itemToolPrefs().imageFillColor);
	docu.writeAttribute("PICTSHADE",m_Doc->itemToolPrefs().imageFillColorShade);
	docu.writeAttribute("PICTSCX",m_Doc->itemToolPrefs().imageScaleX);
	docu.writeAttribute("PICTSCY",m_Doc->itemToolPrefs().imageScaleY);
	docu.writeAttribute("PSCALE", static_cast<int>(m_Doc->itemToolPrefs().imageScaleType));
	docu.writeAttribute("PASPECT", static_cast<int>(m_Doc->itemToolPrefs().imageAspectRatio));
	docu.writeAttribute("EmbeddedPath", static_cast<int>(m_Doc->itemToolPrefs().imageUseEmbeddedPath));
	docu.writeAttribute("HalfRes", m_Doc->itemToolPrefs().imageLowResType);
	docu.writeAttribute("dispX", m_Doc->opToolPrefs().dispX);
	docu.writeAttribute("dispY", m_Doc->opToolPrefs().dispY);
	docu.writeAttribute("constrain", m_Doc->opToolPrefs().constrain);
	docu.writeAttribute("MINORC",m_Doc->guidesPrefs().minorGridColor.name());
	docu.writeAttribute("MAJORC",m_Doc->guidesPrefs().majorGridColor.name());
	docu.writeAttribute("GuideC", m_Doc->guidesPrefs().guideColor.name());
	docu.writeAttribute("BaseC", m_Doc->guidesPrefs().baselineGridColor.name());
//	docu.writeAttribute("GuideZ", m_Doc->guidesPrefs().guideRad);
	docu.writeAttribute("BACKG", static_cast<int>(m_Doc->guidesPrefs().guidePlacement));
	docu.writeAttribute("PAGEC",m_Doc->paperColor().name());
	docu.writeAttribute("MARGC",m_Doc->guidesPrefs().marginColor.name());
	docu.writeAttribute("RANDF", static_cast<int>(m_Doc->marginColored()));
	docu.writeAttribute("currentProfile", m_Doc->curCheckProfile());
	docu.writeAttribute("calligrapicPenFillColor", m_Doc->itemToolPrefs().calligrapicPenFillColor);
	docu.writeAttribute("calligrapicPenLineColor", m_Doc->itemToolPrefs().calligrapicPenLineColor);
	docu.writeAttribute("calligrapicPenFillColorShade", m_Doc->itemToolPrefs().calligrapicPenFillColorShade);
	docu.writeAttribute("calligrapicPenLineColorShade", m_Doc->itemToolPrefs().calligrapicPenLineColorShade);
	docu.writeAttribute("calligrapicPenLineWidth", m_Doc->itemToolPrefs().calligrapicPenLineWidth);
	docu.writeAttribute("calligrapicPenAngle", m_Doc->itemToolPrefs().calligrapicPenAngle);
	docu.writeAttribute("calligrapicPenWidth", m_Doc->itemToolPrefs().calligrapicPenWidth);
	docu.writeAttribute("calligrapicPenStyle", m_Doc->itemToolPrefs().calligrapicPenStyle);

	writeCheckerProfiles(docu);
	writeJavascripts(docu);
	writeBookmarks(docu);
	writeColors(docu);
	writeGradients(docu);
	writeHyphenatorLists(docu);
	writePStyles(docu);
	writeCStyles(docu);
	writeLinestyles(docu);
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

void Scribus150Format::writeCheckerProfiles(ScXmlStreamWriter & docu) 
{
	CheckerPrefsList::Iterator itcp;
	CheckerPrefsList::Iterator itcpend=m_Doc->checkerProfiles().end();
	for (itcp = m_Doc->checkerProfiles().begin(); itcp != itcpend; ++itcp)
	{
		docu.writeEmptyElement("CheckProfile");
		docu.writeAttribute("Name",itcp.key());
		docu.writeAttribute("ignoreErrors", static_cast<int>(itcp.value().ignoreErrors));
		docu.writeAttribute("autoCheck", static_cast<int>(itcp.value().autoCheck));
		docu.writeAttribute("checkGlyphs", static_cast<int>(itcp.value().checkGlyphs));
		docu.writeAttribute("checkOrphans", static_cast<int>(itcp.value().checkOrphans));
		docu.writeAttribute("checkOverflow", static_cast<int>(itcp.value().checkOverflow));
		docu.writeAttribute("checkPictures", static_cast<int>(itcp.value().checkPictures));
		docu.writeAttribute("checkPartFilledImageFrames", static_cast<int>(itcp.value().checkPartFilledImageFrames));
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

void Scribus150Format::writeLinestyles(ScXmlStreamWriter& docu) 
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
	for (itar = m_Doc->arrowStyles().begin(); itar != m_Doc->arrowStyles().end(); ++itar)
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


void Scribus150Format::writeJavascripts(ScXmlStreamWriter & docu) 
{
	QMap<QString,QString>::Iterator itja;
	for (itja = m_Doc->JavaScripts.begin(); itja != m_Doc->JavaScripts.end(); ++itja)
	{
		docu.writeEmptyElement("JAVA");
		docu.writeAttribute("NAME",itja.key());
		docu.writeAttribute("SCRIPT",itja.value());
	}
	
}


void Scribus150Format::writeBookmarks(ScXmlStreamWriter & docu) 
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


void Scribus150Format::writeColors(ScXmlStreamWriter & docu, bool part) 
{	
	ColorList::Iterator itc;
	ColorList usedColors;
	if (part)
		m_Doc->getUsedColors(usedColors);
	else
		usedColors = m_Doc->PageColors;
	for (itc = usedColors.begin(); itc != usedColors.end(); ++itc)
	{
		docu.writeEmptyElement("COLOR");
		docu.writeAttribute("NAME",itc.key());
		if (m_Doc->PageColors[itc.key()].getColorModel() == colorModelRGB)
			docu.writeAttribute("RGB",m_Doc->PageColors[itc.key()].nameRGB());
		else
			docu.writeAttribute("CMYK",m_Doc->PageColors[itc.key()].nameCMYK());
		if (m_Doc->PageColors[itc.key()].isSpotColor())
			docu.writeAttribute("Spot",static_cast<int>(m_Doc->PageColors[itc.key()].isSpotColor()));
		if (m_Doc->PageColors[itc.key()].isRegistrationColor())
			docu.writeAttribute("Register",static_cast<int>(m_Doc->PageColors[itc.key()].isRegistrationColor()));
	}
	
}

void Scribus150Format::writeGradients(ScXmlStreamWriter & docu, bool part)
{
	QMap<QString, VGradient>::Iterator itGrad;
	QMap<QString, VGradient> gradMap;
	if (part)
		m_Doc->getUsedGradients(gradMap);
	else
		gradMap = m_Doc->docGradients;
	for (itGrad = gradMap.begin(); itGrad != gradMap.end(); ++itGrad)
	{
		docu.writeStartElement("Gradient");
		docu.writeAttribute("Name",itGrad.key());
		VGradient gra = itGrad.value();
		QList<VColorStop*> cstops = gra.colorStops();
		for (uint cst = 0; cst < gra.Stops(); ++cst)
		{
			docu.writeEmptyElement("CSTOP");
			docu.writeAttribute("RAMP", cstops.at(cst)->rampPoint);
			docu.writeAttribute("NAME", cstops.at(cst)->name);
			docu.writeAttribute("SHADE", cstops.at(cst)->shade);
			docu.writeAttribute("TRANS", cstops.at(cst)->opacity);
		}
		docu.writeEndElement();
	}
}

void Scribus150Format::writeHyphenatorLists(ScXmlStreamWriter &docu)
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

void Scribus150Format::writePStyles(ScXmlStreamWriter & docu) 
{
	QList<int> styleList = m_Doc->getSortedStyleList();
	for (int a = 0; a < styleList.count(); ++a)
	{
		putPStyle(docu, m_Doc->paragraphStyles()[styleList[a]], "STYLE");
	}
	//	if (m_Doc->docParagraphStyles.count() > 5)
//	{
//		for (int ff = 0; ff < m_Doc->paragraphStyles().count(); ++ff)
//			putPStyle(docu, m_Doc->paragraphStyles()[ff], "STYLE");
//	}
}

void Scribus150Format::putPStyle(ScXmlStreamWriter & docu, const ParagraphStyle & style, const QString &nodeName)
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
	if ( style.isDefaultStyle())
		docu.writeAttribute("DefaultStyle", style.isDefaultStyle());
	
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


void Scribus150Format::writeCStyles(ScXmlStreamWriter & docu) 
{
	QList<int> styleList = m_Doc->getSortedCharStyleList();
	for (int a = 0; a < styleList.count(); ++a)
	{
		docu.writeStartElement("CHARSTYLE");
		putNamedCStyle(docu, m_Doc->charStyles()[styleList[a]]);
		docu.writeEndElement();
	}
//	for (int ff = 0; ff < m_Doc->charStyles().count(); ++ff)
//	{
//		docu.writeStartElement("CHARSTYLE");
//		putNamedCStyle(docu, m_Doc->charStyles()[ff]);
//		docu.writeEndElement();
//	}
}

void Scribus150Format::putCStylePT(ScXmlStreamWriter & docu, const CharStyle & style)
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
	docu.writeAttribute("LANGUAGE", style.language());
	docu.writeAttribute("SHORTCUT", style.shortcut()); // shortcuts won't be inherited
}

void Scribus150Format::putCStyle(ScXmlStreamWriter & docu, const CharStyle & style)
{
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
	if ( ! style.isInhLanguage())
		docu.writeAttribute("LANGUAGE", style.language());
	if ( ! style.shortcut().isEmpty() )
		docu.writeAttribute("SHORTCUT", style.shortcut()); // shortcuts won't be inherited
}

void Scribus150Format::putNamedCStyle(ScXmlStreamWriter& docu, const CharStyle & style)
{
	if ( ! style.name().isEmpty() )
		docu.writeAttribute("CNAME", style.name());
	if ( style.hasName() && style.isDefaultStyle())
		docu.writeAttribute("DefaultStyle", style.isDefaultStyle());
	putCStyle(docu, style);
}

void Scribus150Format::writeLayers(ScXmlStreamWriter & docu) 
{	
	uint layerCount=m_Doc->layerCount();
	for (uint lay = 0; lay < layerCount; ++lay)
	{
		docu.writeEmptyElement("LAYERS");
		docu.writeAttribute("NUMMER",m_Doc->Layers[lay].ID);
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

void Scribus150Format::writePrintOptions(ScXmlStreamWriter & docu) 
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
	docu.writeAttribute("useDocBleeds", static_cast<int>(m_Doc->Print_Options.useDocBleeds));
	docu.writeAttribute("cropMarks", static_cast<int>(m_Doc->Print_Options.cropMarks));
	docu.writeAttribute("bleedMarks", static_cast<int>(m_Doc->Print_Options.bleedMarks));
	docu.writeAttribute("registrationMarks", static_cast<int>(m_Doc->Print_Options.registrationMarks));
	docu.writeAttribute("colorMarks", static_cast<int>(m_Doc->Print_Options.colorMarks));
	docu.writeAttribute("includePDFMarks", static_cast<int>(m_Doc->Print_Options.includePDFMarks));
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

void Scribus150Format::writePdfOptions(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("PDF");
	docu.writeAttribute("firstUse", static_cast<int>(m_Doc->pdfOptions().firstUse));
	docu.writeAttribute("Thumbnails", static_cast<int>(m_Doc->pdfOptions().Thumbnails));
	docu.writeAttribute("Articles", static_cast<int>(m_Doc->pdfOptions().Articles));
	docu.writeAttribute("Bookmarks", static_cast<int>(m_Doc->pdfOptions().Bookmarks));
	docu.writeAttribute("Compress", static_cast<int>(m_Doc->pdfOptions().Compress));
	docu.writeAttribute("CMethod", m_Doc->pdfOptions().CompressMethod);
	docu.writeAttribute("Quality", m_Doc->pdfOptions().Quality);
	docu.writeAttribute("MirrorH", static_cast<int>(m_Doc->pdfOptions().MirrorH));
	docu.writeAttribute("MirrorV", static_cast<int>(m_Doc->pdfOptions().MirrorV));
	docu.writeAttribute("Clip", static_cast<int>(m_Doc->pdfOptions().doClip));
	docu.writeAttribute("RotateDeg", static_cast<int>(m_Doc->pdfOptions().RotateDeg));
	docu.writeAttribute("PresentMode", static_cast<int>(m_Doc->pdfOptions().PresentMode));
	docu.writeAttribute("RecalcPic", static_cast<int>(m_Doc->pdfOptions().RecalcPic));
	docu.writeAttribute("Grayscale", static_cast<int>(m_Doc->pdfOptions().isGrayscale));
	docu.writeAttribute("RGBMode", static_cast<int>(m_Doc->pdfOptions().UseRGB));
	docu.writeAttribute("UseProfiles", static_cast<int>(m_Doc->pdfOptions().UseProfiles));
	docu.writeAttribute("UseProfiles2", static_cast<int>(m_Doc->pdfOptions().UseProfiles2));
	docu.writeAttribute("Binding", m_Doc->pdfOptions().Binding);
	docu.writeAttribute("PicRes", m_Doc->pdfOptions().PicRes);
	docu.writeAttribute("Resolution", m_Doc->pdfOptions().Resolution);
	docu.writeAttribute("Version", m_Doc->pdfOptions().Version);
	docu.writeAttribute("Intent", m_Doc->pdfOptions().Intent);
	docu.writeAttribute("Intent2", m_Doc->pdfOptions().Intent2);
	docu.writeAttribute("SolidP", m_Doc->pdfOptions().SolidProf);
	docu.writeAttribute("ImageP", m_Doc->pdfOptions().ImageProf);
	docu.writeAttribute("PrintP", m_Doc->pdfOptions().PrintProf);
	docu.writeAttribute("InfoString", m_Doc->pdfOptions().Info);
	docu.writeAttribute("BTop", m_Doc->pdfOptions().bleeds.Top);
	docu.writeAttribute("BLeft", m_Doc->pdfOptions().bleeds.Left);
	docu.writeAttribute("BRight", m_Doc->pdfOptions().bleeds.Right);
	docu.writeAttribute("BBottom", m_Doc->pdfOptions().bleeds.Bottom);
	docu.writeAttribute("useDocBleeds", static_cast<int>(m_Doc->pdfOptions().useDocBleeds));
	docu.writeAttribute("cropMarks", static_cast<int>(m_Doc->pdfOptions().cropMarks));
	docu.writeAttribute("bleedMarks", static_cast<int>(m_Doc->pdfOptions().bleedMarks));
	docu.writeAttribute("registrationMarks", static_cast<int>(m_Doc->pdfOptions().registrationMarks));
	docu.writeAttribute("colorMarks", static_cast<int>(m_Doc->pdfOptions().colorMarks));
	docu.writeAttribute("docInfoMarks", static_cast<int>(m_Doc->pdfOptions().docInfoMarks));
	docu.writeAttribute("markOffset", m_Doc->pdfOptions().markOffset);
	docu.writeAttribute("ImagePr", static_cast<int>(m_Doc->pdfOptions().EmbeddedI));
	docu.writeAttribute("PassOwner", m_Doc->pdfOptions().PassOwner);
	docu.writeAttribute("PassUser", m_Doc->pdfOptions().PassUser);
	docu.writeAttribute("Permissions", m_Doc->pdfOptions().Permissions);
	docu.writeAttribute("Encrypt", static_cast<int>(m_Doc->pdfOptions().Encrypt));
	docu.writeAttribute("UseLayers", static_cast<int>(m_Doc->pdfOptions().useLayers));
	docu.writeAttribute("UseLpi", static_cast<int>(m_Doc->pdfOptions().UseLPI));
	docu.writeAttribute("UseSpotColors", static_cast<int>(m_Doc->pdfOptions().UseSpotColors));
	docu.writeAttribute("doMultiFile", static_cast<int>(m_Doc->pdfOptions().doMultiFile));
	docu.writeAttribute("displayBookmarks", static_cast<int>(m_Doc->pdfOptions().displayBookmarks));
	docu.writeAttribute("displayFullscreen", static_cast<int>(m_Doc->pdfOptions().displayFullscreen));
	docu.writeAttribute("displayLayers", static_cast<int>(m_Doc->pdfOptions().displayLayers));
	docu.writeAttribute("displayThumbs", static_cast<int>(m_Doc->pdfOptions().displayThumbs));
	docu.writeAttribute("hideMenuBar", static_cast<int>(m_Doc->pdfOptions().hideMenuBar));
	docu.writeAttribute("hideToolBar", static_cast<int>(m_Doc->pdfOptions().hideToolBar));
	docu.writeAttribute("fitWindow", static_cast<int>(m_Doc->pdfOptions().fitWindow));
	docu.writeAttribute("openAfterExport", static_cast<int>(m_Doc->pdfOptions().openAfterExport));
	docu.writeAttribute("PageLayout", m_Doc->pdfOptions().PageLayout);
	docu.writeAttribute("openAction", m_Doc->pdfOptions().openAction);
	for (int pdoF = 0; pdoF < m_Doc->pdfOptions().EmbedList.count(); ++pdoF)
	{
		docu.writeEmptyElement("Fonts");
		docu.writeAttribute("Name", m_Doc->pdfOptions().EmbedList[pdoF]);
	}
	for (int pdoS = 0; pdoS < m_Doc->pdfOptions().SubsetList.count(); ++pdoS)
	{
		docu.writeEmptyElement("Subset");
		docu.writeAttribute("Name", m_Doc->pdfOptions().SubsetList[pdoS]);
	}
	for (int pdoE = 0; pdoE < m_Doc->pdfOptions().PresentVals.count(); ++pdoE)
	{
		docu.writeEmptyElement("Effekte");
		docu.writeAttribute("pageEffectDuration", m_Doc->pdfOptions().PresentVals[pdoE].pageEffectDuration);
		docu.writeAttribute("pageViewDuration", m_Doc->pdfOptions().PresentVals[pdoE].pageViewDuration);
		docu.writeAttribute("effectType", m_Doc->pdfOptions().PresentVals[pdoE].effectType);
		docu.writeAttribute("Dm", m_Doc->pdfOptions().PresentVals[pdoE].Dm);
		docu.writeAttribute("M", m_Doc->pdfOptions().PresentVals[pdoE].M);
		docu.writeAttribute("Di", m_Doc->pdfOptions().PresentVals[pdoE].Di);
	}
	QMap<QString,LPIData>::Iterator itlp;
	for (itlp = m_Doc->pdfOptions().LPISettings.begin(); itlp != m_Doc->pdfOptions().LPISettings.end(); ++itlp)
	{
		docu.writeEmptyElement("LPI");
		docu.writeAttribute("Color", itlp.key());
		docu.writeAttribute("Frequency", itlp.value().Frequency);
		docu.writeAttribute("Angle", itlp.value().Angle);
		docu.writeAttribute("SpotFunction", itlp.value().SpotFunc);
	}
	docu.writeEndElement();
}

void Scribus150Format::writeDocItemAttributes(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("DocItemAttributes");
	for(ObjAttrVector::Iterator objAttrIt = m_Doc->itemAttributes().begin() ; objAttrIt != m_Doc->itemAttributes().end(); ++objAttrIt )
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


void Scribus150Format::writeTOC(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("TablesOfContents");
	for(ToCSetupVector::Iterator tocSetupIt = m_Doc->tocSetups().begin() ; tocSetupIt != m_Doc->tocSetups().end(); ++tocSetupIt )
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


void Scribus150Format::writeSections(ScXmlStreamWriter & docu) 
{
	docu.writeStartElement("Sections");
	for(DocumentSectionMap::Iterator it = m_Doc->sections().begin() ; it != m_Doc->sections().end(); ++it )
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
		docu.writeAttribute("FillChar", (*it).pageNumberFillChar.unicode());
		docu.writeAttribute("FieldWidth", (*it).pageNumberWidth);
	}
	docu.writeEndElement();
}


void Scribus150Format::writePageSets(ScXmlStreamWriter & docu) 
{	
	docu.writeStartElement("PageSets");
	QList<PageSet>::Iterator itpgset;
	QList<PageSet> pageSet(m_Doc->pageSets());
	for(itpgset = pageSet.begin(); itpgset != pageSet.end(); ++itpgset )
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

void Scribus150Format::writePatterns(ScXmlStreamWriter & docu, const QString& baseDir, bool part, Selection* selection)
{
	QStringList patterns;
	if (part)
		patterns = m_Doc->getPatternDependencyList(m_Doc->getUsedPatternsSelection(selection));
	else
		patterns = m_Doc->getPatternDependencyList(m_Doc->docPatterns.keys());
	for (int a = 0; a < patterns.count(); a++)
	{
		docu.writeStartElement("Pattern");
		docu.writeAttribute("Name", patterns[a]);
		ScPattern pa = m_Doc->docPatterns[patterns[a]];
		docu.writeAttribute("width", pa.width);
		docu.writeAttribute("height", pa.height);
		docu.writeAttribute("scaleX", pa.scaleX);
		docu.writeAttribute("scaleY", pa.scaleY);
		docu.writeAttribute("xoffset", pa.xoffset);
		docu.writeAttribute("yoffset", pa.yoffset);
		WriteObjects(m_Doc, docu, baseDir, 0, 0, ItemSelectionPattern, &pa.items);
		docu.writeEndElement();
	}	
}

void Scribus150Format::writeContent(ScXmlStreamWriter & docu, const QString& baseDir) 
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

void Scribus150Format::WritePages(ScribusDoc *doc, ScXmlStreamWriter& docu, QProgressBar *dia2, uint maxC, bool master)
{
	uint ObCount = maxC;
	Page *page;
	uint pages;
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
		docu.writeAttribute("Orientation", page->orientation());
		docu.writeAttribute("LEFT", page->LeftPg);
		docu.writeAttribute("PRESET", page->marginPreset);
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
	QString textWithSoftHyphens(StoryText& itemText, int from, int to)
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


void Scribus150Format::writeITEXTs(ScribusDoc *doc, ScXmlStreamWriter &docu, PageItem* item)
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
			ch == SpecialChars::NBHYPHEN ||
			ch == SpecialChars::NBSPACE ||
			ch == SpecialChars::ZWNBSPACE ||
			ch == SpecialChars::ZWSPACE ||
			ch.unicode() < 32 || 
			(0xd800 <= ch.unicode() && ch.unicode() < 0xe000) ||
			ch.unicode() == 0xfffe || ch.unicode() == 0xffff ||
			style1 != lastStyle)
		{
			// something new, write pending chars
			if  (k - lastPos > 0)
			{
				docu.writeEmptyElement("ITEXT");
				/*if (item->asPathText()) // seems to cause problems when loading pathtext elements
					putCStylePT(docu, lastStyle);
				else*/
					putCStyle(docu, lastStyle);
				docu.writeAttribute("CH", textWithSoftHyphens(item->itemText, lastPos, k));
			}
			lastStyle = style1;
			lastPos = k;
		}

		if (ch == SpecialChars::OBJECT && item->itemText.object(k) != NULL) 
		{
			// each obj in its own ITEXT for now
			docu.writeEmptyElement("ITEXT");
			/*if (item->asPathText()) // seems to cause problems when loading pathtext elements
				putCStylePT(docu, lastStyle);
			else*/
				putCStyle(docu, lastStyle);
			tmpnum.setNum(ch.unicode());
			docu.writeAttribute("Unicode", tmpnum);
			docu.writeAttribute("COBJ", item->itemText.object(k)->ItemNr);		
		}
		else if (ch == SpecialChars::PARSEP)	// stores also the paragraphstyle for preceding chars
			putPStyle(docu, item->itemText.paragraphStyle(k), "para");
		else if (ch == SpecialChars::TAB)
		{
			docu.writeEmptyElement("tab");
			putCStyle(docu, lastStyle);
		}
		else if (ch == SpecialChars::LINEBREAK)
			docu.writeEmptyElement("breakline");
		else if (ch == SpecialChars::COLBREAK)
			docu.writeEmptyElement("breakcol");
		else if (ch == SpecialChars::FRAMEBREAK)
			docu.writeEmptyElement("breakframe");
		else if (ch == SpecialChars::NBHYPHEN)
			docu.writeEmptyElement("nbhyphen");
		else if (ch == SpecialChars::NBSPACE)
			docu.writeEmptyElement("nbspace");
		else if (ch == SpecialChars::ZWNBSPACE)
			docu.writeEmptyElement("zwnbspace");
		else if (ch == SpecialChars::ZWSPACE)
			docu.writeEmptyElement("zwspace");
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
		/*if (item->asPathText())
			putCStylePT(docu, lastStyle);
		else*/
			putCStyle(docu, lastStyle);
		docu.writeAttribute("CH", textWithSoftHyphens(item->itemText, lastPos, item->itemText.length()));
	}
	// paragraphstyle for trailing chars
	if (item->itemText.length() == 0 || item->itemText.text(item->itemText.length()-1) != SpecialChars::PARSEP)
	{
		putPStyle(docu, item->itemText.paragraphStyle(item->itemText.length()), "trail");
	}
}

void Scribus150Format::WriteObjects(ScribusDoc *doc, ScXmlStreamWriter& docu, const QString& baseDir, QProgressBar *dia2, uint maxC, ItemSelection master, QList<PageItem*> *some_items)
{
	uint ObCount = maxC;
	QList<PageItem*> *items = NULL;
	PageItem *item = NULL;
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
		case ItemSelectionGroup:
		case ItemSelectionPattern:
		case ItemSelectionElements:
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
			case ItemSelectionGroup:
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
			case ItemSelectionElements:
				docu.writeStartElement("ITEM");
				break;
		}
		if (master == ItemSelectionElements)
		{
			docu.writeAttribute("XPOS", item->xPos() - doc->currentPage()->xOffset());
			docu.writeAttribute("YPOS", item->yPos() - doc->currentPage()->yOffset());
		}
		else
		{
			docu.writeAttribute("XPOS", item->xPos());
			docu.writeAttribute("YPOS", item->yPos());
		}
		SetItemProps(docu, item, baseDir, true);
		if (!item->OnMasterPage.isEmpty())
			docu.writeAttribute("OnMasterPage", item->OnMasterPage);
		if (!item->pixm.imgInfo.usedPath.isEmpty())
			docu.writeAttribute("ImageClip", item->pixm.imgInfo.usedPath);
		if (item->pixm.imgInfo.lowResType != 1)
			docu.writeAttribute("ImageRes", item->pixm.imgInfo.lowResType);
		if (item->isEmbedded)
			docu.writeAttribute("isInline", 1);
		if (!item->fillRule)
			docu.writeAttribute("fillRule", 0);
		if (item->doOverprint)
			docu.writeAttribute("doOverprint", 1);
		docu.writeAttribute("gXpos", item->gXpos);
		docu.writeAttribute("gYpos", item->gYpos);
		docu.writeAttribute("gWidth", item->gWidth);
		docu.writeAttribute("gHeight", item->gHeight);
		if (item->itemType() == PageItem::Symbol)
			docu.writeAttribute("pattern", item->pattern());
		if (item->GrType != 0)
		{
			if (item->GrType == 8)
			{
				docu.writeAttribute("pattern", item->pattern());
				double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY;
				item->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
				bool mirrorX, mirrorY;
				item->patternFlip(mirrorX, mirrorY);
				docu.writeAttribute("pScaleX", patternScaleX);
				docu.writeAttribute("pScaleY", patternScaleY);
				docu.writeAttribute("pOffsetX", patternOffsetX);
				docu.writeAttribute("pOffsetY", patternOffsetY);
				docu.writeAttribute("pRotation", patternRotation);
				docu.writeAttribute("pSkewX", patternSkewX);
				docu.writeAttribute("pSkewY", patternSkewY);
				docu.writeAttribute("pMirrorX" , mirrorX);
				docu.writeAttribute("pMirrorY" , mirrorY);
			}
			else
			{
				if (item->GrType == 11)
				{
					docu.writeAttribute("GMAY", item->meshGradientArray[0].count());
					docu.writeAttribute("GMAX", item->meshGradientArray.count());
				}
				else
				{
					docu.writeAttribute("GRSTARTX", item->GrStartX);
					docu.writeAttribute("GRSTARTY", item->GrStartY);
					docu.writeAttribute("GRENDX", item->GrEndX);
					docu.writeAttribute("GRENDY", item->GrEndY);
					docu.writeAttribute("GRFOCALX", item->GrFocalX);
					docu.writeAttribute("GRFOCALY", item->GrFocalY);
					docu.writeAttribute("GRSCALE" , item->GrScale);
					docu.writeAttribute("GRSKEW" , item->GrSkew);
					if ((item->GrType == 9) || (item->GrType == 10))
					{
						docu.writeAttribute("GRC1X"   , item->GrControl1.x());
						docu.writeAttribute("GRC1Y"   , item->GrControl1.y());
						docu.writeAttribute("GRCOLP1" , item->GrColorP1);
						docu.writeAttribute("GRC2X"   , item->GrControl2.x());
						docu.writeAttribute("GRC2Y"   , item->GrControl2.y());
						docu.writeAttribute("GRCOLP2" , item->GrColorP2);
						docu.writeAttribute("GRC3X"   , item->GrControl3.x());
						docu.writeAttribute("GRC3Y"   , item->GrControl3.y());
						docu.writeAttribute("GRCOLP3" , item->GrColorP3);
						docu.writeAttribute("GRC4X"   , item->GrControl4.x());
						docu.writeAttribute("GRC4Y"   , item->GrControl4.y());
						docu.writeAttribute("GRC5X"   , item->GrControl5.x());
						docu.writeAttribute("GRC5Y"   , item->GrControl5.y());
						docu.writeAttribute("GRCOLP4" , item->GrColorP4);
						docu.writeAttribute("GRCOLT1" , item->GrCol1transp);
						docu.writeAttribute("GRCOLT2" , item->GrCol2transp);
						docu.writeAttribute("GRCOLT3" , item->GrCol3transp);
						docu.writeAttribute("GRCOLT4" , item->GrCol4transp);
						docu.writeAttribute("GRCOLS1" , item->GrCol1Shade);
						docu.writeAttribute("GRCOLS2" , item->GrCol1Shade);
						docu.writeAttribute("GRCOLS3" , item->GrCol1Shade);
						docu.writeAttribute("GRCOLS4" , item->GrCol1Shade);
					}
				}
			}
		}
		if (!item->gradient().isEmpty())
			docu.writeAttribute("GRNAME", item->gradient());
		if (!item->strokeGradient().isEmpty())
			docu.writeAttribute("GRNAMES", item->strokeGradient());
		if (!item->gradientMask().isEmpty())
			docu.writeAttribute("GRNAMEM", item->gradientMask());
		if (item->GrTypeStroke > 0)
		{
			docu.writeAttribute("GRSTARTXS", item->GrStrokeStartX);
			docu.writeAttribute("GRSTARTYS", item->GrStrokeStartY);
			docu.writeAttribute("GRENDXS", item->GrStrokeEndX);
			docu.writeAttribute("GRENDYS", item->GrStrokeEndY);
			docu.writeAttribute("GRFOCALXS", item->GrStrokeFocalX);
			docu.writeAttribute("GRFOCALYS", item->GrStrokeFocalY);
			docu.writeAttribute("GRSCALES" , item->GrStrokeScale);
			docu.writeAttribute("GRSKEWS" , item->GrStrokeSkew);
		}
		if (!item->strokePattern().isEmpty())
		{
			docu.writeAttribute("patternS", item->strokePattern());
			double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace;
			item->strokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
			bool mirrorX, mirrorY, atPath;
			item->strokePatternFlip(mirrorX, mirrorY);
			atPath = item->isStrokePatternToPath();
			docu.writeAttribute("pScaleXS", patternScaleX);
			docu.writeAttribute("pScaleYS", patternScaleY);
			docu.writeAttribute("pOffsetXS", patternOffsetX);
			docu.writeAttribute("pOffsetYS", patternOffsetY);
			docu.writeAttribute("pRotationS", patternRotation);
			docu.writeAttribute("pSkewXS", patternSkewX);
			docu.writeAttribute("pSkewYS", patternSkewY);
			docu.writeAttribute("pSpaceS", patternSpace);
			docu.writeAttribute("pMirrorXS" , mirrorX);
			docu.writeAttribute("pMirrorYS" , mirrorY);
			docu.writeAttribute("pAtPathS" , atPath);
		}
		if (item->GrMask > 0)
		{
			docu.writeAttribute("GRTYPM", item->GrMask);
			docu.writeAttribute("GRSTARTXM", item->GrMaskStartX);
			docu.writeAttribute("GRSTARTYM", item->GrMaskStartY);
			docu.writeAttribute("GRENDXM", item->GrMaskEndX);
			docu.writeAttribute("GRENDYM", item->GrMaskEndY);
			docu.writeAttribute("GRFOCALXM", item->GrMaskFocalX);
			docu.writeAttribute("GRFOCALYM", item->GrMaskFocalY);
			docu.writeAttribute("GRSCALEM" , item->GrMaskScale);
			docu.writeAttribute("GRSKEWM" , item->GrMaskSkew);
		}
		if (!item->patternMask().isEmpty())
		{
			docu.writeAttribute("patternM", item->patternMask());
			double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY;
			item->maskTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
			bool mirrorX, mirrorY;
			item->maskFlip(mirrorX, mirrorY);
			docu.writeAttribute("pScaleXM", patternScaleX);
			docu.writeAttribute("pScaleYM", patternScaleY);
			docu.writeAttribute("pOffsetXM", patternOffsetX);
			docu.writeAttribute("pOffsetYM", patternOffsetY);
			docu.writeAttribute("pRotationM", patternRotation);
			docu.writeAttribute("pSkewXM", patternSkewX);
			docu.writeAttribute("pSkewYM", patternSkewY);
			docu.writeAttribute("pMirrorXM" , mirrorX);
			docu.writeAttribute("pMirrorYM" , mirrorY);
		}
		if (item->itemText.defaultStyle().hasParent())
			docu.writeAttribute("PSTYLE", item->itemText.defaultStyle().parent());
		if (! item->itemText.defaultStyle().isInhAlignment())
			docu.writeAttribute("ALIGN", item->itemText.defaultStyle().alignment());
		
		docu.writeAttribute("LAYER", item->LayerID);
		if (item->isBookmark)
			docu.writeAttribute("BOOKMARK", 1);

		if (item->asTextFrame() || item->asPathText() || item->asImageFrame())
		{
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
		if (((item->GrType > 0) && (item->GrType != 8) && (item->GrType != 9) && (item->GrType != 11)) && (item->gradient().isEmpty()))
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
		if ((item->GrTypeStroke > 0) && (item->strokeGradient().isEmpty()))
		{
			QList<VColorStop*> cstops = item->stroke_gradient.colorStops();
			for (uint cst = 0; cst < item->stroke_gradient.Stops(); ++cst)
			{
				docu.writeEmptyElement("S_CSTOP");
				docu.writeAttribute("RAMP", cstops.at(cst)->rampPoint);
				docu.writeAttribute("NAME", cstops.at(cst)->name);
				docu.writeAttribute("SHADE", cstops.at(cst)->shade);
				docu.writeAttribute("TRANS", cstops.at(cst)->opacity);
			}
		}
		if ((item->GrMask > 0) && (item->gradientMask().isEmpty()))
		{
			QList<VColorStop*> cstops = item->mask_gradient.colorStops();
			for (uint cst = 0; cst < item->mask_gradient.Stops(); ++cst)
			{
				docu.writeEmptyElement("M_CSTOP");
				docu.writeAttribute("RAMP", cstops.at(cst)->rampPoint);
				docu.writeAttribute("NAME", cstops.at(cst)->name);
				docu.writeAttribute("SHADE", cstops.at(cst)->shade);
				docu.writeAttribute("TRANS", cstops.at(cst)->opacity);
			}
		}
		if (item->GrType == 11)
		{
			for (int grow = 0; grow < item->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < item->meshGradientArray[grow].count(); gcol++)
				{
					meshPoint mp = item->meshGradientArray[grow][gcol];
					docu.writeStartElement("MPoint");
					docu.writeAttribute("GX", mp.gridPoint.x());
					docu.writeAttribute("GY", mp.gridPoint.y());
					docu.writeAttribute("CTX", mp.controlTop.x());
					docu.writeAttribute("CTY", mp.controlTop.y());
					docu.writeAttribute("CBX", mp.controlBottom.x());
					docu.writeAttribute("CBY", mp.controlBottom.y());
					docu.writeAttribute("CLX", mp.controlLeft.x());
					docu.writeAttribute("CLY", mp.controlLeft.y());
					docu.writeAttribute("CRX", mp.controlRight.x());
					docu.writeAttribute("CRY", mp.controlRight.y());
					docu.writeAttribute("NAME", mp.colorName);
					docu.writeAttribute("SHADE", mp.shade);
					docu.writeAttribute("TRANS", mp.transparency);
					docu.writeEndElement();
				}
			}
		}
		
		if (item->asLatexFrame())
		{
			docu.writeStartElement("LATEX");
			PageItem_LatexFrame *latexitem = item->asLatexFrame();
			QFileInfo fi(latexitem->configFile());
			docu.writeAttribute("ConfigFile", fi.fileName());
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
#ifdef HAVE_OSG
		if (item->asOSGFrame())
		{
			PageItem_OSGFrame *osgitem = item->asOSGFrame();
			if (!item->Pfile.isEmpty())
			{
				QHash<QString, PageItem_OSGFrame::viewDefinition>::iterator itv;
				for (itv = osgitem->viewMap.begin(); itv != osgitem->viewMap.end(); ++itv)
				{
					QString tmp;
					docu.writeStartElement("OSGViews");
					docu.writeAttribute("viewName", itv.key());
					docu.writeAttribute("angleFOV", itv.value().angleFOV);
					QString trackM = "";
					for (uint matx = 0; matx < 4; ++matx)
					{
						for (uint maty = 0; maty < 4; ++maty)
						{
							trackM += tmp.setNum(itv.value().trackerMatrix(matx, maty))+" ";
						}
					}
					docu.writeAttribute("trackM", trackM);
					QString trackC = "";
					trackC += tmp.setNum(itv.value().trackerCenter[0])+" ";
					trackC += tmp.setNum(itv.value().trackerCenter[1])+" ";
					trackC += tmp.setNum(itv.value().trackerCenter[2]);
					docu.writeAttribute("trackC", trackC);
					QString cameraP = "";
					cameraP += tmp.setNum(itv.value().cameraPosition[0])+" ";
					cameraP += tmp.setNum(itv.value().cameraPosition[1])+" ";
					cameraP += tmp.setNum(itv.value().cameraPosition[2]);
					docu.writeAttribute("cameraP", cameraP);
					QString cameraU = "";
					cameraU += tmp.setNum(itv.value().cameraUp[0])+" ";
					cameraU += tmp.setNum(itv.value().cameraUp[1])+" ";
					cameraU += tmp.setNum(itv.value().cameraUp[2]);
					docu.writeAttribute("cameraU", cameraU);
					docu.writeAttribute("trackerDist", itv.value().trackerDist);
					docu.writeAttribute("trackerSize", itv.value().trackerSize);
					docu.writeAttribute("illumination", itv.value().illumination);
					docu.writeAttribute("rendermode", itv.value().rendermode);
					docu.writeAttribute("trans", itv.value().addedTransparency);
					docu.writeAttribute("colorAC", itv.value().colorAC.name());
					docu.writeAttribute("colorFC", itv.value().colorFC.name());
					docu.writeEndElement();
				}
			}
		}
#endif
		if (item->asGroupFrame())
		{
			WriteObjects(m_Doc, docu, baseDir, 0, 0, ItemSelectionGroup, &item->groupItemList);
		}

		//CB PageItemAttributes
		ObjAttrVector *attributes=item->getObjectAttributes();
		if (attributes->count() > 0)
		{
			docu.writeStartElement("PageItemAttributes");
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
		}

		docu.writeEndElement();
	}
}

void Scribus150Format::SetItemProps(ScXmlStreamWriter& docu, PageItem* item, const QString& baseDir, bool newFormat)
{
//	double xf, yf;
	QString tmp, tmpy;
	if (newFormat)
		docu.writeAttribute("OwnPage", item->OwnPage);
	docu.writeAttribute("PTYPE",item->realItemType());
	docu.writeAttribute("WIDTH",item->width());
	docu.writeAttribute("HEIGHT",item->height());
	if (item->cornerRadius() != 0)
		docu.writeAttribute("RADRECT",item->cornerRadius());
	docu.writeAttribute("FRTYPE", item->FrameType);
	docu.writeAttribute("CLIPEDIT", item->ClipEdited ? 1 : 0);
	if (item->GrType != 0)
		docu.writeAttribute("GRTYP",item->GrType);
	if (item->GrTypeStroke != 0)
		docu.writeAttribute("GRTYPS",item->GrTypeStroke);
	if (item->rotation() != 0)
		docu.writeAttribute("ROT",item->rotation());
	if (!item->printEnabled())
		docu.writeAttribute("PRINTABLE", 0);
	if (item->imageFlippedH())
		docu.writeAttribute("FLIPPEDH", 1);
	if (item->imageFlippedV())
		docu.writeAttribute("FLIPPEDV", 1);
	if (!(item->isGroup() || item->isSymbol()))
	{
		docu.writeAttribute("PWIDTH",item->lineWidth());
		if (item->fillColor() != CommonStrings::None)
			docu.writeAttribute("PCOLOR",item->fillColor());
		if (item->fillShade() != 100)
			docu.writeAttribute("SHADE",item->fillShade());
		if (item->lineColor() != CommonStrings::None)
			docu.writeAttribute("PCOLOR2",item->lineColor());
		if (item->lineShade() != 100)
			docu.writeAttribute("SHADE2",item->lineShade());
		if (!item->NamedLStyle.isEmpty())
			docu.writeAttribute("NAMEDLST",item->NamedLStyle);
		if (item->PLineArt != 0)
			docu.writeAttribute("PLINEART",item->PLineArt);
		if (item->PLineEnd != 0)
			docu.writeAttribute("PLINEEND", item->PLineEnd);
		if (item->PLineJoin != 0)
			docu.writeAttribute("PLINEJOIN", item->PLineJoin);
	}
	if (item->asRegularPolygon())
	{
		PageItem_RegularPolygon *regitem = item->asRegularPolygon();
		docu.writeAttribute("POLYC", regitem->polyCorners);
		docu.writeAttribute("POLYF", regitem->polyFactor);
		docu.writeAttribute("POLYR", regitem->polyRotation);
		docu.writeAttribute("POLYIR", regitem->polyInnerRot);
		docu.writeAttribute("POLYCUR", regitem->polyCurvature);
		docu.writeAttribute("POLYOCUR", regitem->polyOuterCurvature);
		docu.writeAttribute("POLYS", static_cast<int>(regitem->polyUseFactor));
	}
	if (item->asArc())
	{
		PageItem_Arc *arcitem = item->asArc();
		docu.writeAttribute("arcHeight", arcitem->arcHeight);
		docu.writeAttribute("arcWidth", arcitem->arcWidth);
		docu.writeAttribute("arcStartAngle", arcitem->arcStartAngle);
		docu.writeAttribute("arcSweepAngle", arcitem->arcSweepAngle);
	}
	if (item->asSpiral())
	{
		PageItem_Spiral *arcitem = item->asSpiral();
		docu.writeAttribute("spiralStartAngle", arcitem->spiralStartAngle);
		docu.writeAttribute("spiralEndAngle", arcitem->spiralEndAngle);
		docu.writeAttribute("spiralFactor", arcitem->spiralFactor);
	}
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
	if (!item->AutoName)
		docu.writeAttribute("ANNAME", item->itemName());
	// "TEXTFLOWMODE" succeed to "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes
	if (item->textFlowMode() != 0)
		docu.writeAttribute("TEXTFLOWMODE", (int) item->textFlowMode() );
	// Set "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes for compatibility
	// with versions prior to 1.3.4
//	docu.writeAttribute("TEXTFLOW" , item->textFlowAroundObject() ? 1 : 0);
//	docu.writeAttribute("TEXTFLOW2", item->textFlowUsesBoundingBox() ? 1 : 0);
//	docu.writeAttribute("TEXTFLOW3", item->textFlowUsesContourLine() ? 1 : 0);
	if (item->asTextFrame() || item->asPathText() || item->asImageFrame())
	{
		docu.writeAttribute("LOCALSCX",item->imageXScale());
		docu.writeAttribute("LOCALSCY",item->imageYScale());
		docu.writeAttribute("LOCALX",item->imageXOffset());
		docu.writeAttribute("LOCALY",item->imageYOffset());
		docu.writeAttribute("LOCALROT" ,item->imageRotation());
		docu.writeAttribute("PICART", item->imageShown() ? 1 : 0);
		docu.writeAttribute("SCALETYPE", item->ScaleType ? 1 : 0);
		docu.writeAttribute("RATIO", item->AspectRatio ? 1 : 0);
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
		if ( ! item->itemText.defaultStyle().isInhLeftMargin() )
			docu.writeAttribute("leftMargin", item->itemText.defaultStyle().leftMargin());
		if ( ! item->itemText.defaultStyle().isInhRightMargin())
			docu.writeAttribute("rightMargin", item->itemText.defaultStyle().rightMargin());
		if ( ! item->itemText.defaultStyle().isInhFirstIndent())
			docu.writeAttribute("firstIndent", item->itemText.defaultStyle().firstIndent());
		if ( ! item->itemText.defaultStyle().isInhLineSpacing())
			docu.writeAttribute("LINESP",item->itemText.defaultStyle().lineSpacing());
		if ( ! item->itemText.defaultStyle().isInhLineSpacingMode())
			docu.writeAttribute("LINESPMode", item->itemText.defaultStyle().lineSpacingMode());
		if ( ! item->itemText.defaultStyle().charStyle().isInhFont())
			docu.writeAttribute("IFONT",item->itemText.defaultStyle().charStyle().font().scName());
		if ( ! item->itemText.defaultStyle().charStyle().isInhFontSize())
			docu.writeAttribute("ISIZE",item->itemText.defaultStyle().charStyle().fontSize() / 10.0 );
		if ( ! item->itemText.defaultStyle().charStyle().isInhLanguage())
			docu.writeAttribute("LANGUAGE", item->itemText.defaultStyle().charStyle().language());
	}
	if (item->asTextFrame() || item->asPathText())
	{
		docu.writeAttribute("COLUMNS", item->columns());
		docu.writeAttribute("COLGAP", item->columnGap());
		docu.writeAttribute("AUTOTEXT", item->isAutoText ? 1 : 0);
		docu.writeAttribute("EXTRA",item->textToFrameDistLeft());
		docu.writeAttribute("TEXTRA",item->textToFrameDistTop());
		docu.writeAttribute("BEXTRA",item->textToFrameDistBottom());
		docu.writeAttribute("REXTRA",item->textToFrameDistRight());
		docu.writeAttribute("FLOP",item->firstLineOffset()); // here I think this FLOP "cher à mon cœur" is legitimate!
		docu.writeAttribute("PLTSHOW", item->PoShow ? 1 : 0);
		docu.writeAttribute("BASEOF", item->BaseOffs);
		docu.writeAttribute("textPathType", item->textPathType);
		docu.writeAttribute("textPathFlipped", static_cast<int>(item->textPathFlipped));
	}
#ifdef HAVE_OSG
	if (((item->asImageFrame() && !(item->asLatexFrame() || item->asOSGFrame())) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
#else
	if (((item->asImageFrame() && !(item->asLatexFrame())) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
#endif
	{
		docu.writeAttribute("Pagenumber", item->pixm.imgInfo.actualPageNumber);
		if (item->isInlineImage)
		{
			docu.writeAttribute("PFILE", "");
			docu.writeAttribute("isInlineImage", static_cast<int>(item->isInlineImage));
			QFileInfo inlFi(item->Pfile);
			docu.writeAttribute("inlineImageExt", inlFi.suffix());
			QFile inFil(item->Pfile);
			if (inFil.open(QIODevice::ReadOnly))
			{
				QByteArray ba = qCompress(inFil.readAll()).toBase64();
				docu.writeAttribute("ImageData", QString(ba));
				inFil.close();
			}
		}
		else
			docu.writeAttribute("PFILE",Path2Relative(item->Pfile, baseDir));
	}
#ifdef HAVE_OSG
	else if (item->asOSGFrame())
	{
		if (!item->Pfile.isEmpty())
		{
			docu.writeAttribute("PFILE", "");
			docu.writeAttribute("isInlineImage", static_cast<int>(item->isInlineImage));
			QFileInfo inlFi(item->Pfile);
			docu.writeAttribute("inlineImageExt", inlFi.suffix());
			QFile inFil(item->Pfile);
			if (inFil.open(QIODevice::ReadOnly))
			{
				QByteArray ba = qCompress(inFil.readAll()).toBase64();
				docu.writeAttribute("ImageData", QString(ba));
				inFil.close();
			}
			PageItem_OSGFrame *osgframe = item->asOSGFrame();
			docu.writeAttribute("modelFile", Path2Relative(osgframe->modelFile, baseDir));
			docu.writeAttribute("currentViewName", osgframe->currentView);
		}
	}
#endif
	if (!item->Pfile2.isEmpty())
		docu.writeAttribute("PFILE2",Path2Relative(item->Pfile2, baseDir));
	if (!item->Pfile3.isEmpty())
		docu.writeAttribute("PFILE3",Path2Relative(item->Pfile3, baseDir));
	if (!item->IProfile.isEmpty())
		docu.writeAttribute("PRFILE",item->IProfile);
	if (!item->EmProfile.isEmpty())
		docu.writeAttribute("EPROF", item->EmProfile);
	if (item->IRender != 1)
		docu.writeAttribute("IRENDER",item->IRender);
	if (!item->UseEmbedded)
		docu.writeAttribute("EMBEDDED", 0);
	if (item->asImageFrame())
	{
		if (item->OverrideCompressionMethod)
			docu.writeAttribute("COMPRESSIONMETHOD", item->CompressionMethodIndex);
		if (item->OverrideCompressionQuality)
			docu.writeAttribute("COMPRESSIONQUALITY", item->CompressionQualityIndex);
	}
	if (item->locked())
		docu.writeAttribute("LOCK", 1);
	if (item->sizeLocked())
		docu.writeAttribute("LOCKR", 1);
	if (item->reversed())
		docu.writeAttribute("REVERS", 1);
	if (item->fillTransparency() != 0)
		docu.writeAttribute("TransValue", item->fillTransparency());
	if (item->lineTransparency() != 0)
		docu.writeAttribute("TransValueS", item->lineTransparency());
	if (item->fillBlendmode() != 0)
		docu.writeAttribute("TransBlend", item->fillBlendmode());
	if (item->lineBlendmode() != 0)
		docu.writeAttribute("TransBlendS", item->lineBlendmode());
	if (item->isTableItem)
	{
		docu.writeAttribute("isTableItem", 1);
		docu.writeAttribute("TopLine", static_cast<int>(item->TopLine));
		docu.writeAttribute("LeftLine", static_cast<int>(item->LeftLine));
		docu.writeAttribute("RightLine", static_cast<int>(item->RightLine));
		docu.writeAttribute("BottomLine", static_cast<int>(item->BottomLine));
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
	if (item->isGroup())
	{
		docu.writeAttribute("groupWidth", item->groupWidth);
		docu.writeAttribute("groupHeight", item->groupHeight);
	}
	if (item->DashValues.count() != 0)
	{
		docu.writeAttribute("NUMDASH", static_cast<int>(item->DashValues.count()));
		QString dlp = "";
		QVector<double>::Iterator dax;
		for (dax = item->DashValues.begin(); dax != item->DashValues.end(); ++dax)
			dlp += tmp.setNum((*dax)) + " ";
		docu.writeAttribute("DASHS", dlp);
		docu.writeAttribute("DASHOFF", item->DashOffset);
	}
	if (!(item->isArc() || item->isSpiral() || item->isRegularPolygon()))
		docu.writeAttribute("path", item->PoLine.svgPath(!item->isPolyLine()));
	QString colp = item->ContourLine.svgPath(true);
	if (!colp.isEmpty())
		docu.writeAttribute("copath", colp);
	if (item->asLine() || item->asPolyLine())
	{
		if (item->startArrowIndex() != 0)
			docu.writeAttribute("startArrowIndex", item->startArrowIndex());
		if (item->endArrowIndex() != 0)
			docu.writeAttribute("endArrowIndex", item->endArrowIndex());
		if (item->startArrowScale() != 100)
			docu.writeAttribute("startArrowScale", item->startArrowScale());
		if (item->endArrowScale() != 100)
			docu.writeAttribute("endArrowScale", item->endArrowScale());
	}
}
