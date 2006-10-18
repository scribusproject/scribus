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
#include "scribusdoc.h"
#include "scribusview.h"

#include "units.h"
#include "util.h"
#include "colorutil.h"
#ifdef HAVE_LIBZ
	#include <zlib.h>
#endif
#include <qcursor.h>
#include <qfileinfo.h>
#include <qvaluelist.h>


bool Scribus134Format::saveFile(const QString & fileName, const FileFormat & /* fmt */)
{
	QString text, tf, tf2, tc, tc2;
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
	dc.setAttribute("ScratchBottom", m_Doc->ScratchBottom);
	dc.setAttribute("ScratchLeft", m_Doc->ScratchLeft);
	dc.setAttribute("ScratchRight", m_Doc->ScratchRight);
	dc.setAttribute("ScratchTop", m_Doc->ScratchTop);
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
	CheckerPrefsList::Iterator itcp;
	CheckerPrefsList::Iterator itcpend=m_Doc->checkerProfiles.end();
	for (itcp = m_Doc->checkerProfiles.begin(); itcp != itcpend; ++itcp)
	{
		QDomElement dc79a=docu.createElement("CheckProfile");
		dc79a.setAttribute("Name",itcp.key());
		dc79a.setAttribute("ignoreErrors", static_cast<int>(itcp.data().ignoreErrors));
		dc79a.setAttribute("autoCheck", static_cast<int>(itcp.data().autoCheck));
		dc79a.setAttribute("checkGlyphs", static_cast<int>(itcp.data().checkGlyphs));
		dc79a.setAttribute("checkOrphans", static_cast<int>(itcp.data().checkOrphans));
		dc79a.setAttribute("checkOverflow", static_cast<int>(itcp.data().checkOverflow));
		dc79a.setAttribute("checkPictures", static_cast<int>(itcp.data().checkPictures));
		dc79a.setAttribute("checkResolution", static_cast<int>(itcp.data().checkResolution));
		dc79a.setAttribute("checkTransparency", static_cast<int>(itcp.data().checkTransparency));
		dc79a.setAttribute("minResolution",itcp.data().minResolution);
		dc79a.setAttribute("maxResolution",itcp.data().maxResolution);
		dc79a.setAttribute("checkAnnotations", static_cast<int>(itcp.data().checkAnnotations));
		dc79a.setAttribute("checkRasterPDF", static_cast<int>(itcp.data().checkRasterPDF));
		dc79a.setAttribute("checkForGIF", static_cast<int>(itcp.data().checkForGIF));
		dc79a.setAttribute("ignoreOffLayers", static_cast<int>(itcp.data().ignoreOffLayers));
		dc.appendChild(dc79a);
	}
	QMap<QString,multiLine>::Iterator itMU;
	for (itMU = m_Doc->MLineStyles.begin(); itMU != m_Doc->MLineStyles.end(); ++itMU)
	{
		QDomElement MuL=docu.createElement("MultiLine");
		MuL.setAttribute("Name",itMU.key());
		multiLine ml = itMU.data();
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
			MuL.appendChild(SuL);
		}
		dc.appendChild(MuL);
	}
	QValueList<ArrowDesc>::Iterator itar;
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
	QMap<QString,QString>::Iterator itja;
	for (itja = m_Doc->JavaScripts.begin(); itja != m_Doc->JavaScripts.end(); ++itja)
	{
		QDomElement jav=docu.createElement("JAVA");
		jav.setAttribute("NAME",itja.key());
		jav.setAttribute("SCRIPT",itja.data());
		dc.appendChild(jav);
	}
	QValueList<ScribusDoc::BookMa>::Iterator itbm;
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
	if (m_Doc->docParagraphStyles.count() > 5)
	{
		for (uint ff = 5; ff < m_Doc->docParagraphStyles.count(); ++ff)
		{
			QDomElement fo=docu.createElement("STYLE");
			fo.setAttribute("NAME",m_Doc->docParagraphStyles[ff].name());
			fo.setAttribute("ALIGN",m_Doc->docParagraphStyles[ff].alignment());
			fo.setAttribute("LINESPMode",m_Doc->docParagraphStyles[ff].lineSpacingMode());
			fo.setAttribute("LINESP",m_Doc->docParagraphStyles[ff].lineSpacing());
			fo.setAttribute("INDENT",m_Doc->docParagraphStyles[ff].leftMargin());
			fo.setAttribute("FIRST",m_Doc->docParagraphStyles[ff].firstIndent());
			fo.setAttribute("VOR",m_Doc->docParagraphStyles[ff].gapBefore());
			fo.setAttribute("NACH",m_Doc->docParagraphStyles[ff].gapAfter());
			fo.setAttribute("FONT",m_Doc->docParagraphStyles[ff].charStyle().font().scName());
			fo.setAttribute("FONTSIZE",m_Doc->docParagraphStyles[ff].charStyle().fontSize() / 10.0);
			fo.setAttribute("DROP", static_cast<int>(m_Doc->docParagraphStyles[ff].hasDropCap()));
			fo.setAttribute("DROPLIN", m_Doc->docParagraphStyles[ff].dropCapLines());
			fo.setAttribute("DROPDIST", m_Doc->docParagraphStyles[ff].dropCapOffset());
			fo.setAttribute("EFFECT", m_Doc->docParagraphStyles[ff].charStyle().effects());
			if (m_Doc->docParagraphStyles[ff].tabValues().count() != 0)
			{
				for (uint a = 0; a < m_Doc->docParagraphStyles[ff].tabValues().count(); ++a)
				{
					QDomElement tabs = docu.createElement("Tabs");
					tabs.setAttribute("Type", (*m_Doc->docParagraphStyles[ff].tabValues().at(a)).tabType);
					tabs.setAttribute("Pos", (*m_Doc->docParagraphStyles[ff].tabValues().at(a)).tabPosition);
					QString tabCh = "";
					if (!(*m_Doc->docParagraphStyles[ff].tabValues().at(a)).tabFillChar.isNull())
						tabCh = QString((*m_Doc->docParagraphStyles[ff].tabValues().at(a)).tabFillChar);
					tabs.setAttribute("Fill", tabCh);
					fo.appendChild(tabs);
				}
			}
			fo.setAttribute("FCOLOR",m_Doc->docParagraphStyles[ff].charStyle().fillColor());
			fo.setAttribute("FSHADE",m_Doc->docParagraphStyles[ff].charStyle().fillShade());
			fo.setAttribute("SCOLOR",m_Doc->docParagraphStyles[ff].charStyle().strokeColor());
			fo.setAttribute("SSHADE",m_Doc->docParagraphStyles[ff].charStyle().strokeShade());
			fo.setAttribute("BASE", static_cast<int>(m_Doc->docParagraphStyles[ff].useBaselineGrid()));
			fo.setAttribute("TXTSHX",m_Doc->docParagraphStyles[ff].charStyle().shadowXOffset() / 10.0);
			fo.setAttribute("TXTSHY",m_Doc->docParagraphStyles[ff].charStyle().shadowYOffset() / 10.0);
			fo.setAttribute("TXTOUT",m_Doc->docParagraphStyles[ff].charStyle().outlineWidth() / 10.0);
			fo.setAttribute("TXTULP",m_Doc->docParagraphStyles[ff].charStyle().underlineOffset() / 10.0);
			fo.setAttribute("TXTULW",m_Doc->docParagraphStyles[ff].charStyle().underlineWidth() / 10.0);
			fo.setAttribute("TXTSTP",m_Doc->docParagraphStyles[ff].charStyle().strikethruOffset() / 10.0);
			fo.setAttribute("TXTSTW",m_Doc->docParagraphStyles[ff].charStyle().strikethruWidth() / 10.0);
			fo.setAttribute("SCALEH",m_Doc->docParagraphStyles[ff].charStyle().scaleH() / 10.0);
			fo.setAttribute("SCALEV",m_Doc->docParagraphStyles[ff].charStyle().scaleV() / 10.0);
			fo.setAttribute("BASEO",m_Doc->docParagraphStyles[ff].charStyle().baselineOffset() / 10.0);
			fo.setAttribute("KERN",m_Doc->docParagraphStyles[ff].charStyle().tracking() / 10.0);
			dc.appendChild(fo);
		}
	}
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
	pdf.setAttribute("BTop", m_Doc->PDF_Options.BleedTop);
	pdf.setAttribute("BLeft", m_Doc->PDF_Options.BleedLeft);
	pdf.setAttribute("BRight", m_Doc->PDF_Options.BleedRight);
	pdf.setAttribute("BBottom", m_Doc->PDF_Options.BleedBottom);
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
	for (uint pdoF = 0; pdoF < m_Doc->PDF_Options.EmbedList.count(); ++pdoF)
	{
		QDomElement pdf2 = docu.createElement("Fonts");
		pdf2.setAttribute("Name", m_Doc->PDF_Options.EmbedList[pdoF]);
		pdf.appendChild(pdf2);
	}
	for (uint pdoS = 0; pdoS < m_Doc->PDF_Options.SubsetList.count(); ++pdoS)
	{
		QDomElement pdf4 = docu.createElement("Subset");
		pdf4.setAttribute("Name", m_Doc->PDF_Options.SubsetList[pdoS]);
		pdf.appendChild(pdf4);
	}
	for (uint pdoE = 0; pdoE < m_Doc->PDF_Options.PresentVals.count(); ++pdoE)
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
		pdf4.setAttribute("Frequency", itlp.data().Frequency);
		pdf4.setAttribute("Angle", itlp.data().Angle);
		pdf4.setAttribute("SpotFunction", itlp.data().SpotFunc);
		pdf.appendChild(pdf4);
	}
	dc.appendChild(pdf);
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
		}
		currsection.setAttribute("Start", (*it).sectionstartindex);
		currsection.setAttribute("Reversed", (*it).reversed);
		currsection.setAttribute("Active", (*it).active);
		sectionElem.appendChild(currsection);
	}
	dc.appendChild(sectionElem);

	QDomElement pageSetAttr = docu.createElement("PageSets");
	QValueList<PageSet>::Iterator itpgset;
	for(itpgset = m_Doc->pageSets.begin(); itpgset != m_Doc->pageSets.end(); ++itpgset )
	{
		QDomElement pgst = docu.createElement("Set");
		pgst.setAttribute("Name", (*itpgset).Name);
		pgst.setAttribute("FirstPage", (*itpgset).FirstPage);
		pgst.setAttribute("Rows", (*itpgset).Rows);
		pgst.setAttribute("Columns", (*itpgset).Columns);
		pgst.setAttribute("GapHorizontal", (*itpgset).GapHorizontal);
		pgst.setAttribute("GapVertical", (*itpgset).GapVertical);
		pgst.setAttribute("GapBelow", (*itpgset).GapBelow);
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
	QMap<QString, ScPattern>::Iterator itPat;
	for (itPat = m_Doc->docPatterns.begin(); itPat != m_Doc->docPatterns.end(); ++itPat)
	{
		QDomElement pat = docu.createElement("Pattern");
		pat.setAttribute("Name",itPat.key());
		ScPattern pa = itPat.data();
		pat.setAttribute("width", pa.width);
		pat.setAttribute("height", pa.height);
		pat.setAttribute("scaleX", pa.scaleX);
		pat.setAttribute("scaleY", pa.scaleY);
		WriteObjects(m_Doc, &docu, &pat, 0, 0, 3, &pa.items);
		dc.appendChild(pat);
	}
	if (m_mwProgressBar != 0)
	{
		m_mwProgressBar->setTotalSteps(m_Doc->DocPages.count()+m_Doc->MasterPages.count()+m_Doc->DocItems.count()+m_Doc->MasterItems.count()+m_Doc->FrameItems.count());
		m_mwProgressBar->setProgress(0);
	}
	WritePages(m_Doc, &docu, &dc, m_mwProgressBar, 0, true);
	WritePages(m_Doc, &docu, &dc, m_mwProgressBar, m_Doc->MasterPages.count(), false);
	WriteObjects(m_Doc, &docu, &dc, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count(), 2);
	WriteObjects(m_Doc, &docu, &dc, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count()+m_Doc->FrameItems.count(), 0);
	WriteObjects(m_Doc, &docu, &dc, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count()+m_Doc->MasterItems.count()+m_Doc->FrameItems.count(), 1);
	elem.appendChild(dc);
/**
 * changed to enable saving
 * of *.gz documents
 * 2.7.2002 C.Toepp
 * <c.toepp@gmx.de>
*/
 #ifdef HAVE_LIBZ
	QCString cs = docu.toCString(); // UTF-8 QCString
	if(fileName.right(2) == "gz")
	{
		// zipped saving
		// XXX: latin1() should probably be local8Bit()
		gzFile gzDoc = gzopen(fileName.latin1(),"wb");
		if(gzDoc == NULL)
			return false;
		gzputs(gzDoc, cs.data());
		gzclose(gzDoc);
	}
	else
	{
		QFile f(fileName);
		if(!f.open(IO_WriteOnly))
			return false;
		QTextStream s(&f);
		s.writeRawBytes(cs, cs.length());
		f.close();
	}
#else
	QFile f(fileName);
	if(!f.open(IO_WriteOnly))
		return false;
	QTextStream s(&f);
	QCString cs = docu.toCString();
	s.writeRawBytes(cs, cs.length());
	f.close();
#endif
	return true;
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
			dia2->setProgress(ObCount);
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
		pg.setAttribute("VerticalGuides", GuideManagerCore::writeVerticalGuides(
													page, GuideManagerCore::Standard));
		pg.setAttribute("HorizontalGuides", GuideManagerCore::writeHorizontalGuides(
													page, GuideManagerCore::Standard));
		pg.setAttribute("AGhorizontalAutoGap", page->guides.horizontalAutoGap());
		pg.setAttribute("AGverticalAutoGap", page->guides.verticalAutoGap());
		pg.setAttribute("AGhorizontalAutoCount", page->guides.horizontalAutoCount());
		pg.setAttribute("AGverticalAutoCount", page->guides.verticalAutoCount());
		pg.setAttribute("AGautoRefer", page->guides.autoRefer());
		dc->appendChild(pg);
	}
}

void Scribus134Format::WriteObjects(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, int master, QPtrList<PageItem> *items)
{
	int te, te2, tsh, tsh2, tst, tst2, tsb, tsb2, tshs, tshs2, tobj, tobj2;
	QString text, tf, tf2, tc, tc2, tcs, tcs2, tmp, tmpy, Ndir;
	double ts, ts2, tsc, tsc2, tscv, tscv2, tb, tb2, tsx, tsx2, tsy, tsy2, tout, tout2, tulp, tulp2, tulw, tulw2, tstp, tstp2, tstw, tstw2;
	uint ObCount = maxC;
	PageItem *item;
	QDomElement ob;
	uint objects;
	switch (master)
	{
		case 0:
			objects = doc->MasterItems.count();
			break;
		case 1:
			objects = doc->DocItems.count();
			break;
		case 2:
			objects = doc->FrameItems.count();
			break;
		case 3:
			objects = items->count();
			break;
	}
	for(uint j = 0; j < objects;++j)
	{
		ObCount++;
		if (dia2 != 0)
			dia2->setProgress(ObCount);
		switch (master)
		{
			case 0:
				item = doc->MasterItems.at(j);
				ob = docu->createElement("MASTEROBJECT");
				break;
			case 1:
				item = doc->DocItems.at(j);
				ob = docu->createElement("PAGEOBJECT");
				break;
			case 2:
				item = doc->FrameItems.at(j);
				ob = docu->createElement("FRAMEOBJECT");
				break;
			case 3:
				item = items->at(j);
				ob = docu->createElement("PatternItem");
				break;
		}
		SetItemProps(&ob, item, true);
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
				QPtrVector<VColorStop> cstops = item->fill_gradient.colorStops();
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
			for (uint a = 0; a < item->effectsInUse.count(); ++a)
			{
				QDomElement imeff = docu->createElement("ImageEffect");
				imeff.setAttribute("Code", (*item->effectsInUse.at(a)).effectCode);
				imeff.setAttribute("Param", (*item->effectsInUse.at(a)).effectParameters);
				ob.appendChild(imeff);
			}
		}
		if (item->TabValues.count() != 0)
		{
			for (uint a = 0; a < item->TabValues.count(); ++a)
			{
				QDomElement tabs = docu->createElement("Tabs");
				tabs.setAttribute("Type", (*item->TabValues.at(a)).tabType);
				tabs.setAttribute("Pos", (*item->TabValues.at(a)).tabPosition);
				QString tabCh = "";
				if (!(*item->TabValues.at(a)).tabFillChar.isNull())
					tabCh = QString((*item->TabValues.at(a)).tabFillChar);
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
				psd.setAttribute("Visible", static_cast<int>(it2.data().visible));
				psd.setAttribute("useMask", static_cast<int>(it2.data().useMask));
				psd.setAttribute("Opacity", it2.data().opacity);
				psd.setAttribute("Blend", it2.data().blend);
				ob.appendChild(psd);
			}
		}
		ob.setAttribute("ALIGN",findParagraphStyle(doc, item->itemText.defaultStyle()));
		ob.setAttribute("BOOKMARK", item->isBookmark ? 1 : 0);
		for(int k = 0; k < item->itemText.length(); ++k)
		{
			const CharStyle& style1(item->itemText.charStyle(k));
			QChar ch = item->itemText.text(k);
			QDomElement it=docu->createElement("ITEXT");
			ts = style1.fontSize() / 10.0;
			tf = style1.font().scName();
			tc = style1.fillColor();
			te = style1.tracking();
			tsh = style1.fillShade();
			tst = style1.effects() & 2047;
#ifndef NLS_PROTO
			tsb = findParagraphStyle(doc, item->itemText.paragraphStyle(k));
#else
			tsb = 0;
#endif
			tcs = style1.strokeColor();
			tshs = style1.strokeShade();
			tsc = style1.scaleH() / 10.0;
			tscv = style1.scaleV() / 10.0;
			tb = style1.baselineOffset() / 10.0;
			tsx = style1.shadowXOffset() / 10.0;
			tsy = style1.shadowYOffset() / 10.0;
			tout = style1.outlineWidth() / 10.0;
			tulp = style1.underlineOffset() / 10.0;
			tulw = style1.underlineWidth() / 10.0;
			tstp = style1.strikethruOffset() / 10.0;
			tstw = style1.strikethruWidth() / 10.0;
#ifndef NLS_PROTO
			if ((ch == QChar(25)) && (item->itemText.item(k)->cembedded != 0))
				tobj = item->itemText.item(k)->cembedded->ItemNr;
			else
#endif
				tobj = -1;
			if (ch == QChar(13))
				text = QChar(5);
			else if (ch == QChar(9))
				text = QChar(4);
			else
				text = ch;
			++k;
			if (k == item->itemText.length())
			{
				it.setAttribute("CH",text);
				it.setAttribute("CSIZE",ts);
				it.setAttribute("CFONT",tf);
				it.setAttribute("CCOLOR",tc);
				it.setAttribute("CKERN",te);
				it.setAttribute("CSHADE",tsh);
				it.setAttribute("CSTYLE",tst);
				it.setAttribute("CAB",tsb);
				it.setAttribute("CSTROKE",tcs);
				it.setAttribute("CSHADE2",tshs);
				it.setAttribute("CSCALE",tsc);
				it.setAttribute("CSCALEV",tscv);
				it.setAttribute("CBASE",tb);
				it.setAttribute("CSHX",tsx);
				it.setAttribute("CSHY",tsy);
				it.setAttribute("COUT",tout);
				it.setAttribute("CULP",tulp);
				it.setAttribute("CULW",tulw);
				it.setAttribute("CSTP",tstp);
				it.setAttribute("CSTW",tstw);
				if (tobj != -1)
					it.setAttribute("COBJ", tobj);
				ob.appendChild(it);
				break;
			}
			const CharStyle& style2(item->itemText.charStyle(k));
			ch = item->itemText.text(k);
			ts2 = style2.fontSize() / 10.0;
			tf2 = style2.font().scName();
			tc2 = style2.fillColor();
			te2 = style2.tracking();
			tsh2 = style2.fillShade();
			tst2 = style2.effects() & 2047;
#ifndef NLS_PROTO
			tsb2 = findParagraphStyle(doc, item->itemText.paragraphStyle(k));
#else
			tsb2 = 0;
#endif
			tcs2 = style2.strokeColor();
			tshs2 = style2.strokeShade();
			tsc2 = style2.scaleH() / 10.0;
			tscv2 = style2.scaleV() / 10.0;
			tb2 = style2.baselineOffset() / 10.0;
			tsx2 = style2.shadowXOffset() / 10.0;
			tsy2 = style2.shadowYOffset() / 10.0;
			tout2 = style2.outlineWidth() / 10.0;
			tulp2 = style2.underlineOffset() / 10.0;
			tulw2 = style2.underlineWidth() / 10.0;
			tstp2 = style2.strikethruOffset() / 10.0;
			tstw2 = style2.strikethruWidth() / 10.0;
#ifndef NLS_PROTO
			if ((ch == QChar(25)) && (item->itemText.item(k)->cembedded != 0))
				tobj2 = item->itemText.item(k)->cembedded->ItemNr;
			else
#endif
				tobj2 = -1;
			while ((ts2 == ts)
						&& (tsb2 == tsb)
						&& (tf2 == tf)
						&& (tc2 == tc)
						&& (te2 == te)
						&& (tsh2 == tsh)
						&& (tshs2 == tshs)
						&& (tsc2 == tsc)
						&& (tscv2 == tscv)
						&& (tcs2 == tcs)
						&& (tb2 == tb)
						&& (tsx2 == tsx)
						&& (tsy2 == tsy)
						&& (tout2 == tout)
						&& (tulp2 == tulp)
						&& (tulw2 == tulw)
						&& (tstp2 == tstp)
						&& (tstw2 == tstw)
						&& (tobj2 == tobj)
						&& (tst2 == tst))
			{
				if (ch == QChar(13))
					text += QChar(5);
				else if (ch == QChar(9))
					text += QChar(4);
				else
					text += ch;
				++k;
				if (k == item->itemText.length())
					break;
				const CharStyle& style3(item->itemText.charStyle(k));
				ch = item->itemText.text(k);
				ts2 = style3.fontSize() / 10.0;
				tf2 = style3.font().scName();
				tc2 = style3.fillColor();
				te2 = style3.tracking();
				tsh2 = style3.fillShade();
				tst2 = style3.effects() & 2047;
#ifndef NLS_PROTO
				tsb2 = findParagraphStyle(doc, item->itemText.paragraphStyle(k));
#else
				tsb2 = 0;
#endif
				tcs2 = style3.strokeColor();
				tshs2 = style3.strokeShade();
				tsc2 = style3.scaleH() / 10.0;
				tscv2 = style3.scaleV() / 10.0;
				tb2 = style3.baselineOffset() / 10.0;
				tsx2 = style3.shadowXOffset() / 10.0;
				tsy2 = style3.shadowYOffset() / 10.0;
				tout2 = style3.outlineWidth() / 10.0;
				tulp2 = style3.underlineOffset() / 10.0;
				tulw2 = style3.underlineWidth() / 10.0;
				tstp2 = style3.strikethruOffset() / 10.0;
				tstw2 = style3.strikethruWidth() / 10.0;
#ifndef NLS_PROTO
				if ((ch == QChar(25)) && (item->itemText.item(k)->cembedded != 0))
					tobj2 = item->itemText.item(k)->cembedded->ItemNr;
				else
#endif
					tobj2 = -1;
			}
			it.setAttribute("CH",text);
			it.setAttribute("CSIZE",ts);
			it.setAttribute("CFONT",tf);
			it.setAttribute("CCOLOR",tc);
			it.setAttribute("CKERN",te);
			it.setAttribute("CSHADE",tsh);
			it.setAttribute("CSTYLE",tst);
			it.setAttribute("CAB",tsb);
			it.setAttribute("CSTROKE",tcs);
			it.setAttribute("CSHADE2",tshs);
			it.setAttribute("CSCALE",tsc);
			it.setAttribute("CSCALEV",tscv);
			it.setAttribute("CBASE",tb);
			it.setAttribute("CSHX",tsx);
			it.setAttribute("CSHY",tsy);
			it.setAttribute("COUT",tout);
			it.setAttribute("CULP",tulp);
			it.setAttribute("CULW",tulw);
			it.setAttribute("CSTP",tstp);
			it.setAttribute("CSTW",tstw);
			if (tobj != -1)
				it.setAttribute("COBJ", tobj);
			k--;
			ob.appendChild(it);
		}
		if (item->BackBox != 0)
			ob.setAttribute("BACKITEM", item->BackBox->ItemNr);
		else
			ob.setAttribute("BACKITEM", -1);
		if (item->NextBox != 0)
			ob.setAttribute("NEXTITEM", item->NextBox->ItemNr);
		else
			ob.setAttribute("NEXTITEM", -1);
		ob.setAttribute("LAYER", item->LayerNr);

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

void Scribus134Format::SetItemProps(QDomElement *ob, PageItem* item, bool newFormat)
{
	double xf, yf;
	QString tmp, tmpy;
	if (newFormat)
		ob->setAttribute("OwnPage", item->OwnPage);
	ob->setAttribute("PTYPE",item->itemType());
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
	ob->setAttribute("TXTFILL",item->itemText.defaultStyle().charStyle().fillColor());
	ob->setAttribute("TXTSTROKE",item->itemText.defaultStyle().charStyle().strokeColor());
	ob->setAttribute("TXTSTRSH",item->itemText.defaultStyle().charStyle().strokeShade());
	ob->setAttribute("TXTFILLSH",item->itemText.defaultStyle().charStyle().fillShade());
	ob->setAttribute("TXTSCALE",item->itemText.defaultStyle().charStyle().scaleH() / 10.0);
	ob->setAttribute("TXTSCALEV",item->itemText.defaultStyle().charStyle().scaleV() / 10.0);
	ob->setAttribute("TXTBASE",item->itemText.defaultStyle().charStyle().baselineOffset() / 10.0);
	ob->setAttribute("TXTSHX",item->itemText.defaultStyle().charStyle().shadowXOffset() / 10.0);
	ob->setAttribute("TXTSHY",item->itemText.defaultStyle().charStyle().shadowYOffset() / 10.0);
	ob->setAttribute("TXTOUT",item->itemText.defaultStyle().charStyle().outlineWidth() / 10.0);
	ob->setAttribute("TXTULP",item->itemText.defaultStyle().charStyle().underlineOffset() / 10.0);
	ob->setAttribute("TXTULW",item->itemText.defaultStyle().charStyle().underlineWidth() / 10.0);
	ob->setAttribute("TXTSTP",item->itemText.defaultStyle().charStyle().strikethruOffset() / 10.0);
	ob->setAttribute("TXTSTW",item->itemText.defaultStyle().charStyle().strikethruWidth() / 10.0);
	ob->setAttribute("TXTSTYLE",item->itemText.defaultStyle().charStyle().effects());
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
	ob->setAttribute("LINESP",item->itemText.defaultStyle().lineSpacing());
	ob->setAttribute("LINESPMode", item->itemText.defaultStyle().lineSpacingMode());
	ob->setAttribute("TXTKERN",item->itemText.defaultStyle().charStyle().tracking());
	ob->setAttribute("LOCALSCX",item->imageXScale());
	ob->setAttribute("LOCALSCY",item->imageYScale());
	ob->setAttribute("LOCALX",item->imageXOffset());
	ob->setAttribute("LOCALY",item->imageYOffset());
	ob->setAttribute("PICART", item->imageShown() ? 1 : 0);
	ob->setAttribute("PLTSHOW", item->PoShow ? 1 : 0);
	ob->setAttribute("BASEOF", item->BaseOffs);
	ob->setAttribute("FLIPPEDH", item->imageFlippedH());
	ob->setAttribute("FLIPPEDV", item->imageFlippedV());
/*	ob->setAttribute("BBOXX",item->BBoxX);
	ob->setAttribute("BBOXH",item->BBoxH); */
	ob->setAttribute("IFONT",item->itemText.defaultStyle().charStyle().font().scName());
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
			ob->setAttribute("ANEXTERN", Path2Relative(item->annotation().Extern()));
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
	if (((item->asImageFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
		ob->setAttribute("PFILE",Path2Relative(item->Pfile));
	else
		ob->setAttribute("PFILE","");
	if (!item->Pfile2.isEmpty())
		ob->setAttribute("PFILE2",Path2Relative(item->Pfile2));
	else
		ob->setAttribute("PFILE2","");
	if (!item->Pfile3.isEmpty())
		ob->setAttribute("PFILE3",Path2Relative(item->Pfile3));
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
	QValueList<double>::Iterator dax;
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
	QValueStack<int>::Iterator nx;
	for (nx = item->Groups.begin(); nx != item->Groups.end(); ++nx)
		glp += tmp.setNum((*nx)) + " ";
	ob->setAttribute("GROUPS", glp);
	ob->setAttribute("LANGUAGE", item->itemText.defaultStyle().charStyle().language());
	ob->setAttribute("startArrowIndex", item->startArrowIndex());
	ob->setAttribute("endArrowIndex", item->endArrowIndex());
}
