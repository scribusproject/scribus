/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                 	scribusXml.cpp the document xml library for scribus
                             -------------------
    begin                : Sam Jul 14 10:00:00 CEST 2001
    copyright            : (C) 2001 by Christian T�p
    email                : christian.toepp@mr-ct@gmx.de
 ***************************************************************************/

#include "scribusXml.h"
#include "scribusXml.moc"
#include <qfile.h>
#include <qtextstream.h>
#include <qapplication.h>
#include <qtl.h>
#include <qcursor.h>
#include <qregexp.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <cstdlib>
#include <cmath>
#include "missing.h"
#include "page.h"
#include "pageitem.h"
#include "units.h"
#include "prefsmanager.h"
#include "scribusview.h"
#include "commonstrings.h"
#include "text/nlsconfig.h"

// We use some common routines defined in fileloader.h
#include "fileloader.h"

#include <iostream>

#include "scconfig.h"

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif
#include "util.h"

using namespace std;

ScriXmlDoc::ScriXmlDoc()
{
	prefsManager=PrefsManager::instance();
}

// bool ScriXmlDoc::IsScribus(QString fileName)
// {
// 	QString fText(FileLoader::readSLA(fileName));
// 	if ((fText.isEmpty()) || (!fText.startsWith("<SCRIBUS")) || (fText.contains("<PAGE ", true) == 0))
// 		return false;
// 	return true;
// }

void ScriXmlDoc::GetItemText(QDomElement *it, ScribusDoc *doc, bool VorLFound, bool impo, PageItem* obj)
{
	QString tmp2, tmf, tmpf, tmp3;
	tmp2 = it->attribute("CH");
	tmp2.replace(QRegExp("\r"), QChar(5));
	tmp2.replace(QRegExp("\n"), QChar(5));
	tmp2.replace(QRegExp("\t"), QChar(4));
	tmpf = it->attribute("CFONT", doc->toolSettings.defFont);
	bool unknown = false;
	ScFace dummy = ScFace::none();
	if ((!prefsManager->appPrefs.AvailFonts.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[tmpf].usable()))
	{
		bool isThere = false;
		for (uint dl = 0; dl < dummyScFaces.count(); ++dl)
		{
			if ((*dummyScFaces.at(dl)).scName() == tmpf)
			{
				isThere = true;
				dummy = *dummyScFaces.at(dl);
				break;
			}
		}
		if (!isThere)
		{
//FIXME			dummy = new ScFace(tmpf, "", tmpf, "", "", 1, false);
			dummyScFaces.append(dummy);
		}
		unknown = true;
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.GFontSub[tmpf]].usable()))
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
#if 0
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
			ch = SpecialChars::PARSEP;
		if (ch == QChar(4))
			ch = SpecialChars::TAB;
		if (unknown)
			style.setFont(dummy);
		else
			style.setFont((*doc->AllFonts)[tmpf]);
		style.setFontSize(size);
		style.setFillColor(fcolor);
		style.setTracking(extra);
		style.setFillShade(shade);
		style.setEffects(static_cast<StyleFlag>(cstyle));
		int pstyleNr;
		if (impo)
		{
				if (VorLFound)
					pstyleNr = DoVorl[ab].toUInt();
				else
				{
					if (ab < 5)
						pstyleNr = ab;
					else
						pstyleNr = 0;
				}
		}
		else
			pstyleNr = ab;
		
		style.setStrokeColor(stroke);
		style.setStrokeShade(shade2);
		style.setScaleH(QMIN(QMAX(scale, 100), 4000));
		style.setScaleV(QMIN(QMAX(scalev, 100), 4000));
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
		if (ch == SpecialChars::PARSEP)			
			obj->itemText.applyStyle(pos, doc->docParagraphStyles[pstyleNr]);
		else
			obj->itemText.applyCharStyle(pos, 1, style);
	}
#endif	
	return;
}

QString ScriXmlDoc::AskForFont(SCFonts &avail, QString fStr, ScribusDoc *doc)
{
	PrefsManager *prefsManager=PrefsManager::instance();
//	QFont fo;
	QString tmpf = fStr;
	if ((!avail.contains(tmpf)) || (!avail[tmpf].usable()))
	{
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!avail[prefsManager->appPrefs.GFontSub[tmpf]].usable()))
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

void ScriXmlDoc::SetItemProps(QDomElement *ob, PageItem* item, bool newFormat)
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

//CB: Private only now
void ScriXmlDoc::GetStyle(QDomElement *pg, ParagraphStyle *vg, StyleSet<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl)
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
	if (!DoFonts.contains(tmpf))
		tmpf = AskForFont(prefsManager->appPrefs.AvailFonts, tmpf, doc);
	else
		tmpf = DoFonts[tmf];
	vg->charStyle().setFont(prefsManager->appPrefs.AvailFonts[tmpf]);
	vg->charStyle().setFontSize(qRound(pg->attribute("FONTSIZE", "12").toDouble() * 10.0));
	vg->setHasDropCap(static_cast<bool>(pg->attribute("DROP", "0").toInt()));
	vg->setDropCapLines(pg->attribute("DROPLIN", "2").toInt());
	vg->setDropCapOffset(pg->attribute("DROPDIST", "0").toDouble());
	vg->charStyle().setEffects(static_cast<StyleFlag>(pg->attribute("EFFECT", "0").toInt()));
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
		docParagraphStyles.append(vg);
		if (fl)
		{
			DoVorl[VorlC] = tmV.setNum(docParagraphStyles.count()-1);
			VorlC++;
		}
	}
}

bool ScriXmlDoc::ReadElemHeader(QString file, bool isFile, double *x, double *y, double *w, double *h)
{
	QString ff = "";
	QDomDocument docu("scridoc");
	if (isFile)
	{
		QCString f;
		if (!loadRawText(file, f))
			return false;
		if (qstrncmp(f.data(), "<SCRIBUSELEMUTF8", 16) == 0)
			ff = QString::fromUtf8(f.data());
		else
			ff = f;
	}
	else
	{
		// JG Ugly and wrong conversion from an utf8 encoded array
		// to a QString using local enconding
		/*if (file.startsWith("<SCRIBUSELEMUTF8"))
			ff = QString::fromUtf8(file);
		else
			ff = file;*/
		ff  = file;
	}
	if(!docu.setContent(ff))
		return false;
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSELEM") && (elem.tagName() != "SCRIBUSELEMUTF8"))
		return false;
	*x = elem.attribute("XP").toDouble();
	*y = elem.attribute("YP").toDouble();
	*w = elem.attribute("W", "0").toDouble();
	*h = elem.attribute("H", "0").toDouble();
	return true;
}

bool ScriXmlDoc::ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, double Xp, double Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub, ScribusView *view)
{
	struct CopyPasteBuffer OB;
	ParagraphStyle vg;
	QString tmp, tmpf, tmp2, tmp3, tmp4, tmV, tmf;
	QMap<QString,QString> DoMul;
	QMap<int,int> TableID;
	QMap<int,int> arrowID;
	QPtrList<PageItem> TableItems;
	bool VorLFound = false;
	bool newVersion = false;
	int x;
	double GrX, GrY;
	int GrMax = doc->GroupCounter;
	ScColor lf = ScColor();
	QDomDocument docu("scridoc");
	if (Fi)
	{
		QCString f;
		if (!loadRawText(fileName, f))
			return false;
		QString ff = "";
		if (qstrncmp(f.data(), "<SCRIBUSELEMUTF8", 16) == 0)
			ff = QString::fromUtf8(f.data());
		else
			ff = f;
		if(!docu.setContent(ff))
			return false;
	}
	else
	{
		QString ff = "";
		if (fileName.startsWith("<SCRIBUSELEMUTF8"))
			ff = QString::fromUtf8(fileName);
		else
			ff = fileName;
		if(!docu.setContent(ff))
			return false;
	}
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSELEM") && (elem.tagName() != "SCRIBUSELEMUTF8"))
		return false;
	if (loc)
	{
		GrX = 0.0;
		GrY = 0.0;
	}
	else
	{
		GrX = elem.attribute("XP").toDouble();
		GrY = elem.attribute("YP").toDouble();
	}
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	DoFonts.clear();
	DoFonts[doc->toolSettings.defFont] = doc->toolSettings.defFont;
	DoVorl.clear();
	DoVorl[0] = "0";
	DoVorl[1] = "1";
	DoVorl[2] = "2";
	DoVorl[3] = "3";
	DoVorl[4] = "4";
	VorlC = 5;
	TableItems.clear();
	TableID.clear();
	arrowID.clear();
	QString CurDirP = QDir::currentDirPath();
	QDir::setCurrent(QDir::homeDirPath());
	int startNumArrows = doc->arrowStyles.count();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="Arrows")
		{
			if (pg.attribute("Index").toInt() > startNumArrows)
			{
				struct ArrowDesc arrow;
				double xa, ya;
				arrow.name = pg.attribute("Name");
				arrow.userArrow = true;
				QString tmp = pg.attribute("Points");
				QTextStream fp(&tmp, IO_ReadOnly);
				for (uint cx = 0; cx < pg.attribute("NumPoints").toUInt(); ++cx)
				{
					fp >> xa;
					fp >> ya;
					arrow.points.addPoint(xa, ya);
				}
				doc->arrowStyles.append(arrow);
				arrowID.insert(pg.attribute("Index").toInt(), doc->arrowStyles.count());
			}
			else
				arrowID.insert(pg.attribute("Index").toInt(), pg.attribute("Index").toInt());
		}
		if(pg.tagName()=="FONT")
		{
			tmpf = pg.attribute("NAME");
			if ((!avail.contains(tmpf)) || (!avail[tmpf].usable()))
			{
				if (!FontSub.contains(tmpf) || (!avail[FontSub[tmpf]].usable()))
				{
					MissingFont *dia = new MissingFont(0, tmpf, doc);
					dia->exec();
					tmpf = dia->getReplacementFont();
					FontSub[pg.attribute("NAME")] = tmpf;
					delete dia;
				}
				else
					tmpf = FontSub[tmpf];
			}
//			fo = avail[tmpf]->Font;
//			fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
			if(!doc->UsedFonts.contains(tmpf))
				doc->AddFont(tmpf);
			DoFonts[pg.attribute("NAME")] = tmpf;
		}
		// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
		if(pg.tagName()=="COLOR" && pg.attribute("Name")!=CommonStrings::None)
		{
			lf.setDocument(doc);
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
			if (!doc->PageColors.contains(pg.attribute("NAME")))
			  	doc->PageColors[pg.attribute("NAME")] = lf;
		}
		if(pg.tagName()=="MultiLine")
		{
			multiLine ml;
			QDomNode MuLn = DOC.firstChild();
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
			if (!doc->MLineStyles.contains(pg.attribute("Name")))
				doc->MLineStyles.insert(pg.attribute("Name"), ml);
		}
		if(pg.tagName()=="STYLE")
		{
			GetStyle(&pg, &vg, doc->docParagraphStyles, doc, true);
			VorLFound = true;
		}
		DOC=DOC.nextSibling();
	}
	DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="ITEM")
		{
			GetItemProps(newVersion, &pg, &OB);
			OB.Xpos = Xp + pg.attribute("XPOS").toDouble() - GrX;
			OB.Ypos = Yp + pg.attribute("YPOS").toDouble() - GrY;
			OB.startArrowIndex =  arrowID[pg.attribute("startArrowIndex", "0").toInt()];
			OB.endArrowIndex =  arrowID[pg.attribute("endArrowIndex", "0").toInt()];
			OB.isBookmark=pg.attribute("BOOKMARK").toInt();
			OB.NamedLStyle = pg.attribute("NAMEDLST", "");
			if (!doc->MLineStyles.contains(OB.NamedLStyle))
				OB.NamedLStyle = "";
			OB.textAlignment = DoVorl[pg.attribute("ALIGN", "0").toInt()].toUInt();
			tmf = pg.attribute("IFONT", doc->toolSettings.defFont);
			if (tmf.isEmpty())
				tmf = doc->toolSettings.defFont;
			if (DoFonts[tmf].isEmpty())
				OB.IFont = doc->toolSettings.defFont;
			else
				OB.IFont = DoFonts[tmf];
			OB.LayerNr = 0;
			OB.Language = pg.attribute("LANGUAGE", doc->Language);
			tmp = "";
			if ((pg.hasAttribute("GROUPS")) && (pg.attribute("NUMGROUP", "0").toInt() != 0))
			{
				tmp = pg.attribute("GROUPS");
				QTextStream fg(&tmp, IO_ReadOnly);
				OB.Groups.clear();
				for (int cx = 0; cx < pg.attribute("NUMGROUP", "0").toInt(); ++cx)
				{
					fg >> x;
					OB.Groups.push(x+doc->GroupCounter);
					GrMax = QMAX(GrMax, x+doc->GroupCounter);
				}
				tmp = "";
			}
			else
				OB.Groups.clear();
			tmp = "";
			QDomNode IT=DOC.firstChild();
			while(!IT.isNull())
			{
				QDomElement it=IT.toElement();
				if (it.tagName()=="CSTOP")
				{
					QString name = it.attribute("NAME");
					double ramp = it.attribute("RAMP", "0.0").toDouble();
					int shade = it.attribute("SHADE", "100").toInt();
					double opa = it.attribute("TRANS", "1").toDouble();
					OB.fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
					OB.GrColor = "";
					OB.GrColor2 = "";
				}
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
					OB.TabValues.append(tb);
				}
				IT=IT.nextSibling();
			}
			OB.itemText = "";
			OB.LayerNr = -1;
			view->PasteItem(&OB, true, true);
			PageItem* Neu = doc->Items->at(doc->Items->count()-1);
			IT=DOC.firstChild();
			while(!IT.isNull())
			{
				QDomElement it=IT.toElement();
				if (it.tagName()=="ITEXT")
					GetItemText(&it, doc, VorLFound, true, Neu);
				if (it.tagName()=="ImageEffect")
				{
					struct ScImage::imageEffect ef;
					ef.effectParameters = it.attribute("Param");
					ef.effectCode = it.attribute("Code").toInt();
					Neu->effectsInUse.append(ef);
				}
				if (it.tagName() == "PSDLayer")
				{
					struct ImageLoadRequest loadingInfo;
					loadingInfo.blend = it.attribute("Blend");
					loadingInfo.opacity = it.attribute("Opacity").toInt();
					loadingInfo.visible = static_cast<bool>(it.attribute("Visible").toInt());
					Neu->pixm.imgInfo.RequestProps.insert(it.attribute("Layer").toInt(), loadingInfo);
					Neu->pixm.imgInfo.isRequest = true;
				}
				IT=IT.nextSibling();
			}
			if ((Neu->effectsInUse.count() != 0) || (Neu->pixm.imgInfo.RequestProps.count() != 0))
				doc->LoadPict(Neu->Pfile, Neu->ItemNr, true);
			if (Neu->isTableItem)
			{
				TableItems.append(Neu);
				TableID.insert(pg.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
			}
			if (Neu->asPathText())
				Neu->updatePolyClip();
		}
		DOC=DOC.nextSibling();
	}
	if (TableItems.count() != 0)
	{
		for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	doc->GroupCounter = GrMax + 1;
	QDir::setCurrent(CurDirP);
	return true;
}

QString ScriXmlDoc::WriteElem(ScribusDoc *doc, ScribusView *view, Selection* selection)
{
	int te, te2, tsh, tsh2, tst, tst2, tsb, tsb2, tshs, tshs2;
	QString text, tf, tf2, tc, tc2, tcs, tcs2, tmp, tmpy;
	double ts, ts2, tsc, tsc2, tscv, tscv2, tb, tb2, tsx, tsx2, tsy, tsy2, tout, tout2, tulp, tulp2, tulw, tulw2, tstp, tstp2, tstw, tstw2, xp, yp;
	PageItem *item;
	QDomDocument docu("scribus");
	QString st="<SCRIBUSELEMUTF8></SCRIBUSELEMUTF8>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	//item = Selitems->at(0);
	item = selection->itemAt(0);
	QValueList<uint> ELL;
	//for (uint cor=0; cor<Selitems->count(); ++cor)
	for (uint cor=0; cor<selection->count(); ++cor)
		//ELL.append(Selitems->at(cor)->ItemNr);
		ELL.append(selection->itemAt(cor)->ItemNr);
	qHeapSort(ELL);
	if (selection->isMultipleSelection())
	{
		xp = view->GroupX - doc->currentPage()->xOffset();
		yp = view->GroupY - doc->currentPage()->yOffset();
		elem.setAttribute("W", view->GroupW);
		elem.setAttribute("H", view->GroupH);
	}
	else
	{
		xp = item->xPos() - doc->currentPage()->xOffset();
		yp = item->yPos() - doc->currentPage()->yOffset();
		elem.setAttribute("W", item->width());
		elem.setAttribute("H", item->height());
	}
	elem.setAttribute("XP", xp);
	elem.setAttribute("YP", yp);
	//elem.setAttribute("COUNT", Selitems->count());
	elem.setAttribute("COUNT", selection->count());
	elem.setAttribute("Version", QString(VERSION));
	QMap<QString,int>::Iterator itf;
	for (itf = doc->UsedFonts.begin(); itf != doc->UsedFonts.end(); ++itf)
	{
		QDomElement fn=docu.createElement("FONT");
		fn.setAttribute("NAME",itf.key());
		elem.appendChild(fn);
	}
	ColorList::Iterator itc;
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		QDomElement co=docu.createElement("COLOR");
		co.setAttribute("NAME",itc.key());
		if (doc->PageColors[itc.key()].getColorModel() == colorModelRGB)
			co.setAttribute("RGB",doc->PageColors[itc.key()].nameRGB());
		else
			co.setAttribute("CMYK",doc->PageColors[itc.key()].nameCMYK());
		co.setAttribute("Spot",static_cast<int>(doc->PageColors[itc.key()].isSpotColor()));
		co.setAttribute("Register",static_cast<int>(doc->PageColors[itc.key()].isRegistrationColor()));
		elem.appendChild(co);
	}
	QMap<int, ParagraphStyle> UsedStyles;
	QMap<int, int> UsedMapped2Saved;
	int NewStylesNum = 5;
	UsedStyles.clear();
	UsedMapped2Saved.clear();
	ParagraphStyle vg;
	if (doc->docParagraphStyles.count() > 5)
	{
		//for (uint co=0; co<Selitems->count(); ++co)
		for (uint co=0; co<selection->count(); ++co)
		{
			item = doc->Items->at(ELL[co]);
			int parstyle = findParagraphStyle(doc, item->itemText.defaultStyle());
			if (parstyle > 4)
			{
				vg = item->itemText.defaultStyle();
				UsedStyles[parstyle] = vg;
			}
			if (((item->asTextFrame()) || (item->asPathText())) && (item->itemText.length() != 0))
			{
#ifndef NLS_PROTO
				for (int tx = 0; tx < item->itemText.length(); ++tx)
				{
					if (item->itemText.text(tx) == SpecialChars::PARSEP)
					{
						vg = item->itemText.paragraphStyle(tx);
						UsedStyles[findParagraphStyle(doc, vg)] = vg;
					}
				}
#endif
			}
		}
		QValueList<int> StyleNumb = UsedStyles.keys();
		qHeapSort(StyleNumb);
		for (uint ff = 0; ff < StyleNumb.count(); ++ff)
		{
			int actSt = (*StyleNumb.at(ff));
			UsedMapped2Saved.insert(actSt, NewStylesNum);
			NewStylesNum++;
			QDomElement fo=docu.createElement("STYLE");
			fo.setAttribute("NAME",UsedStyles[actSt].name());
			fo.setAttribute("ALIGN",UsedStyles[actSt].alignment());
			fo.setAttribute("LINESPMode",UsedStyles[actSt].lineSpacingMode());
			//CB #2738:
			//UsedStyles[actSt].LineSpa is something like this is using automatic from the font:
			//10.34912109375000000000. Default attribute value is then 10.3491 which then becomes //10.34909999999999996589 and then does not compare. This fixes, should we change our
			//default precision?
			fo.setAttribute("LINESP",QString::number(UsedStyles[actSt].lineSpacing(),'f',13));
			fo.setAttribute("INDENT",UsedStyles[actSt].leftMargin());
			fo.setAttribute("FIRST",UsedStyles[actSt].firstIndent());
			fo.setAttribute("VOR",UsedStyles[actSt].gapBefore());
			fo.setAttribute("NACH",UsedStyles[actSt].gapAfter());
			fo.setAttribute("FONT",UsedStyles[actSt].charStyle().font().scName());
			fo.setAttribute("FONTSIZE",UsedStyles[actSt].charStyle().fontSize() / 10.0);
			fo.setAttribute("DROP", static_cast<int>(UsedStyles[actSt].hasDropCap()));
			fo.setAttribute("DROPLIN", UsedStyles[actSt].dropCapLines());
			fo.setAttribute("DROPDIST", UsedStyles[actSt].dropCapOffset());
			fo.setAttribute("EFFECT", UsedStyles[actSt].charStyle().effects());
			if (UsedStyles[actSt].tabValues().count() != 0)
			{
				for (uint a = 0; a < UsedStyles[actSt].tabValues().count(); ++a)
				{
					QDomElement tabs = docu.createElement("Tabs");
					tabs.setAttribute("Type", (*UsedStyles[actSt].tabValues().at(a)).tabType);
					tabs.setAttribute("Pos", (*UsedStyles[actSt].tabValues().at(a)).tabPosition);
					QString tabCh = "";
					if (!(*UsedStyles[actSt].tabValues().at(a)).tabFillChar.isNull())
						tabCh = QString((*UsedStyles[actSt].tabValues().at(a)).tabFillChar);
					tabs.setAttribute("Fill", tabCh);
					fo.appendChild(tabs);
				}
			}
			fo.setAttribute("FCOLOR",UsedStyles[actSt].charStyle().fillColor());
			fo.setAttribute("FSHADE",UsedStyles[actSt].charStyle().fillShade());
			fo.setAttribute("SCOLOR",UsedStyles[actSt].charStyle().strokeColor());
			fo.setAttribute("SSHADE",UsedStyles[actSt].charStyle().strokeShade());
			fo.setAttribute("BASE", static_cast<int>(UsedStyles[actSt].useBaselineGrid()));
			fo.setAttribute("TXTSHX",UsedStyles[actSt].charStyle().shadowXOffset() / 10.0);
			fo.setAttribute("TXTSHY",UsedStyles[actSt].charStyle().shadowYOffset() / 10.0);
			fo.setAttribute("TXTOUT",UsedStyles[actSt].charStyle().outlineWidth() / 10.0);
			fo.setAttribute("TXTULP",UsedStyles[actSt].charStyle().underlineOffset() / 10.0);
			fo.setAttribute("TXTULW",UsedStyles[actSt].charStyle().underlineWidth() / 10.0);
			fo.setAttribute("TXTSTP",UsedStyles[actSt].charStyle().strikethruOffset() / 10.0);
			fo.setAttribute("TXTSTW",UsedStyles[actSt].charStyle().strikethruWidth() / 10.0);
			fo.setAttribute("SCALEH",UsedStyles[actSt].charStyle().scaleH() / 10.0);
			fo.setAttribute("SCALEV",UsedStyles[actSt].charStyle().scaleV() / 10.0);
			fo.setAttribute("BASEO",UsedStyles[actSt].charStyle().baselineOffset() / 10.0);
			fo.setAttribute("KERN",UsedStyles[actSt].charStyle().tracking() / 10.0);
			elem.appendChild(fo);
		}
	}
	QMap<QString,multiLine>::Iterator itMU;
	for (itMU = doc->MLineStyles.begin(); itMU != doc->MLineStyles.end(); ++itMU)
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
		elem.appendChild(MuL);
	}
	QMap<int, ArrowDesc> usedArrows;
	QMap<int, ArrowDesc>::Iterator itar;
	struct ArrowDesc arrow;
	//for (uint co=0; co<Selitems->count(); ++co)
	for (uint co=0; co<selection->count(); ++co)
	{
		item = doc->Items->at(ELL[co]);
		int startIndex = item->startArrowIndex();
		int endIndex = item->endArrowIndex();
		if (startIndex != 0)
		{
			arrow.points = (*doc->arrowStyles.at(startIndex-1)).points.copy();
			arrow.name = (*doc->arrowStyles.at(startIndex-1)).name;
			usedArrows.insert(startIndex, arrow);
		}
		if (endIndex != 0)
		{
			arrow.points = (*doc->arrowStyles.at(endIndex-1)).points.copy();
			arrow.name = (*doc->arrowStyles.at(endIndex-1)).name;
			usedArrows.insert(endIndex, arrow);
		}
	}
	if (usedArrows.count() != 0)
	{
		for (itar = usedArrows.begin(); itar != usedArrows.end(); ++itar)
		{
			if (itar.data().userArrow)
			{
				QDomElement ar=docu.createElement("Arrows");
				ar.setAttribute("NumPoints", itar.data().points.size());
				QString arp = "";
				double xa, ya;
				for (uint nxx = 0; nxx < itar.data().points.size(); ++nxx)
				{
					itar.data().points.point(nxx, &xa, &ya);
					arp += tmp.setNum(xa) + " " + tmpy.setNum(ya) + " ";
				}
				ar.setAttribute("Points", arp);
				ar.setAttribute("Name", itar.data().name);
				ar.setAttribute("Index", itar.key());
				elem.appendChild(ar);
			}
		}
	}
	//for (uint co=0; co<Selitems->count(); ++co)
	for (uint co=0; co<selection->count(); ++co)
	{
		QString CurDirP = QDir::currentDirPath();
		QDir::setCurrent(QDir::homeDirPath());
		item = doc->Items->at(ELL[co]);
		QDomElement ob=docu.createElement("ITEM");
		int textAlignment = findParagraphStyle(doc, item->itemText.defaultStyle());
		if (textAlignment > 4)
			ob.setAttribute("ALIGN",UsedMapped2Saved[textAlignment]);
		else
			ob.setAttribute("ALIGN",textAlignment);
 		SetItemProps(&ob, item, false);
		ob.setAttribute("XPOS",item->xPos() - doc->currentPage()->xOffset());
		ob.setAttribute("YPOS",item->yPos() - doc->currentPage()->yOffset());
		ob.setAttribute("BOOKMARK", item->isBookmark ? 1 : 0);
		ob.setAttribute("fillRule", static_cast<int>(item->fillRule));
		ob.setAttribute("doOverprint", static_cast<int>(item->doOverprint));
		if (item->effectsInUse.count() != 0)
		{
			for (uint a = 0; a < item->effectsInUse.count(); ++a)
			{
				QDomElement imeff = docu.createElement("ImageEffect");
				imeff.setAttribute("Code", (*item->effectsInUse.at(a)).effectCode);
				imeff.setAttribute("Param", (*item->effectsInUse.at(a)).effectParameters);
				ob.appendChild(imeff);
			}
		}
		if (item->TabValues.count() != 0)
		{
			for (uint a = 0; a < item->TabValues.count(); ++a)
			{
				QDomElement tabs = docu.createElement("Tabs");
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
				QDomElement psd = docu.createElement("PSDLayer");
				psd.setAttribute("Layer",it2.key());
				psd.setAttribute("Visible", static_cast<int>(it2.data().visible));
				psd.setAttribute("Opacity", it2.data().opacity);
				psd.setAttribute("Blend", it2.data().blend);
				ob.appendChild(psd);
			}
		}
		if (item->GrType != 0)
		{
			QPtrVector<VColorStop> cstops = item->fill_gradient.colorStops();
			for (uint cst = 0; cst < item->fill_gradient.Stops(); ++cst)
			{
				QDomElement itcl = docu.createElement("CSTOP");
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
		QDir::setCurrent(CurDirP);
		for(int k=0;k<item->itemText.length();++k)
		{
			const CharStyle& style4(item->itemText.charStyle(k));
			QChar ch = item->itemText.text(k);
			QDomElement it=docu.createElement("ITEXT");
			ts = style4.fontSize() / 10.0;
			tf = style4.font().scName();
			tc = style4.fillColor();
			te = style4.tracking();
			tsh = style4.fillShade();
			tst = style4.effects() & 2047;
#if 0 // FIXME NLS ndef NLS_PROTO
			if (item->itemText.item(k)->cab > 4)
				tsb = UsedMapped2Saved[item->itemText.item(k)->cab];
			else
				tsb = item->itemText.item(k)->cab;
#else
			tsb = 0;
#endif
			tcs = style4.strokeColor();
			tshs = style4.strokeShade();
			tsc = style4.scaleH() / 10.0;
			tscv = style4.scaleV() / 10.0;
			tb = style4.baselineOffset() / 10.0;
			tsx = style4.shadowXOffset() / 10.0;
			tsy = style4.shadowYOffset() / 10.0;
			tout = style4.outlineWidth() / 10.0;
			tulp = style4.underlineOffset() / 10.0;
			tulw = style4.underlineWidth() / 10.0;
			tstp = style4.strikethruOffset()/ 10.0;
			tstw = style4.strikethruWidth() / 10.0;
			if (ch == SpecialChars::PARSEP)
				text = QChar(5);
			else if (ch == SpecialChars::TAB)
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
				ob.appendChild(it);
				break;
			}
			const CharStyle& style5(item->itemText.charStyle(k));
			ch = item->itemText.text(k);
			ts2 = style5.fontSize() / 10.0;
			tf2 = style5.font().scName();
			tc2 = style5.fillColor();
			te2 = style5.tracking();
			tsh2 = style5.fillShade();
			tst2 = style5.effects() & 2047;
#if 0 //FIXME NLS ndef NLS_PROTO
			if (item->itemText.item(k)->cab > 4)
				tsb2 = UsedMapped2Saved[item->itemText.item(k)->cab];
			else
				tsb2 = item->itemText.item(k)->cab;
#else
			tsb2 = 0;
#endif
			tcs2 = style5.strokeColor();
			tshs2 = style5.strokeShade();
			tsc2 = style5.scaleH() / 10.0;
			tscv2 = style5.scaleV() / 10.0;
			tb2 = style5.baselineOffset() / 10.0;
			tsx2 = style5.shadowXOffset() / 10.0;
			tsy2 = style5.shadowYOffset() / 10.0;
			tout2 = style5.outlineWidth() / 10.0;
			tulp2 = style5.underlineOffset() / 10.0;
			tulw2 = style5.underlineWidth() / 10.0;
			tstp2 = style5.strikethruOffset() / 10.0;
			tstw2 = style5.strikethruWidth() / 10.0;
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
				const CharStyle& style6(item->itemText.charStyle(k));
				ch = item->itemText.text(k);
				ts2 = style6.fontSize() / 10.0;
				tf2 = style6.font().scName();
				tc2 = style6.fillColor();
				te2 = style6.tracking();
				tsh2 = style6.fillShade();
				tst2 = style6.effects() & 2047;
#if 0 //FIXME NLS ndef NLS_PROTO
				if (item->itemText.item(k)->cab > 4)
					tsb2 = UsedMapped2Saved[item->itemText.item(k)->cab];
				else
					tsb2 = item->itemText.item(k)->cab;
#else
				tsb2 = 0;
#endif
				tcs2 = style6.strokeColor();
				tshs2 = style6.strokeShade();
				tsc2 = style6.scaleH() / 10.0;
				tscv2 = style6.scaleV() / 10.0;
				tb2 = style6.baselineOffset() / 10.0;
				tsx2 = style6.shadowXOffset() / 10.0;
				tsy2 = style6.shadowYOffset() / 10.0;
				tout2 = style6.outlineWidth() / 10.0;
				tulp2 = style6.underlineOffset() / 10.0;
				tulw2 = style6.underlineWidth() / 10.0;
				tstp2 = style6.strikethruOffset() / 10.0;
				tstw2 = style6.strikethruWidth() / 10.0;
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
			k--;
			ob.appendChild(it);
		}
		ob.setAttribute("NUMTEXT",item->itemText.length());
		QString txnu = "";
		for(int kt = 0; kt < item->itemText.length(); ++kt)
#ifndef NLS_PROTO
			txnu += tmp.setNum(item->itemText.item(kt)->glyph.xoffset) + " " + tmpy.setNum(item->itemText.item(kt)->glyph.yoffset) + " ";
#else
			txnu += "0 0 ";
#endif
		ob.setAttribute("TEXTCOOR", txnu);
		ob.setAttribute("BACKITEM", -1);
		ob.setAttribute("BACKPAGE", -1);
		ob.setAttribute("NEXTITEM", -1);
		ob.setAttribute("NEXTPAGE", -1);
		elem.appendChild(ob);
	}
	return docu.toString();
}
