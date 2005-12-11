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
#include "selection.h"

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
 

/*!
	\fn ScriXmlDoc::IsScribus(QString fileName)
	\author Frederic Dubuy <effediwhy@gmail.com>, Petr Vanek
	\date august 17th 2004, 10/03/2004
	\brief Preliminary Scribus file validator. totally rewritten when fixing crash bug #1092. It's much simpler now.
	\param Qtring filename of file to test
	\retval true = Scribus format file, false : not Scribus
*/
bool ScriXmlDoc::IsScribus(QString fileName)
{
	QString fText(FileLoader::readSLA(fileName));
	if ((fText.isEmpty()) || (!fText.startsWith("<SCRIBUS")) || (fText.contains("<PAGE ", true) == 0))
		return false;
	return true;
}

void ScriXmlDoc::GetItemText(QDomElement *it, ScribusDoc *doc, bool VorLFound, bool impo, PageItem* obj)
{
	QString tmp2, tmf, tmpf, tmp3;
	tmp2 = it->attribute("CH");
	tmp2.replace(QRegExp("\r"), QChar(5));
	tmp2.replace(QRegExp("\n"), QChar(5));
	tmp2.replace(QRegExp("\t"), QChar(4));
	tmpf = it->attribute("CFONT", doc->toolSettings.defFont);
	bool unknown = false;
	struct ScText *hg;
	Foi* dummy;
	if ((!prefsManager->appPrefs.AvailFonts.find(tmpf)) || (!prefsManager->appPrefs.AvailFonts[tmpf]->UseFont))
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
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.GFontSub[tmpf]]->UseFont))
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
	int style = it->attribute("CSTYLE").toInt() & 255;
	int ab = it->attribute("CAB", "0").toInt();
	QString stroke = it->attribute("CSTROKE","None");
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
	for (uint cxx=0; cxx<tmp2.length(); ++cxx)
	{
		hg = new ScText;
		hg->ch = tmp2.at(cxx);
		if (hg->ch == QChar(5))
			hg->ch = QChar(13);
		if (hg->ch == QChar(4))
			hg->ch = QChar(9);
		if (unknown)
			hg->cfont = dummy;
		else
			hg->cfont = (*doc->AllFonts)[tmpf];
		hg->csize = size;
		hg->ccolor = fcolor;
		hg->cextra = extra;
		hg->cshade = shade;
		hg->cselect = false;
		hg->cstyle = style;
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
		hg->cstroke = stroke;
		hg->cshade2 = shade2;
		hg->cscale = QMIN(QMAX(scale, 100), 4000);
		hg->cscalev = QMIN(QMAX(scalev, 100), 4000);
		hg->cbase = base;
		hg->cshadowx = shX;
		hg->cshadowy = shY;
		hg->coutline = outL;
		hg->cunderpos = ulp;
		hg->cunderwidth = ulw;
		hg->cstrikepos = stp;
		hg->cstrikewidth = stw;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		hg->cembedded = 0;
		obj->itemText.append(hg);
	}
	return;
}

QString ScriXmlDoc::AskForFont(SCFonts &avail, QString fStr, ScribusDoc *doc)
{
//	QFont fo;
	QString tmpf = fStr;
	if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
	{
		if ((!prefsManager->appPrefs.GFontSub.contains(tmpf)) || (!avail[prefsManager->appPrefs.GFontSub[tmpf]]->UseFont))
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
	ob->setAttribute("PWIDTH",item->Pwidth);
	ob->setAttribute("PCOLOR",item->fillColor());
	ob->setAttribute("PCOLOR2",item->lineColor());
	ob->setAttribute("TXTFILL",item->TxtFill);
	ob->setAttribute("TXTSTROKE",item->TxtStroke);
	ob->setAttribute("TXTSTRSH",item->ShTxtStroke);
	ob->setAttribute("TXTFILLSH",item->ShTxtFill);
	ob->setAttribute("TXTSCALE",item->TxtScale / 10.0);
	ob->setAttribute("TXTSCALEV",item->TxtScaleV / 10.0);
	ob->setAttribute("TXTBASE",item->TxtBase / 10.0);
	ob->setAttribute("TXTSHX",item->TxtShadowX / 10.0);
	ob->setAttribute("TXTSHY",item->TxtShadowY / 10.0);
	ob->setAttribute("TXTOUT",item->TxtOutline / 10.0);
	ob->setAttribute("TXTULP",item->TxtUnderPos / 10.0);
	ob->setAttribute("TXTULW",item->TxtUnderWidth / 10.0);
	ob->setAttribute("TXTSTP",item->TxtStrikePos / 10.0);
	ob->setAttribute("TXTSTW",item->TxtStrikeWidth / 10.0);
	ob->setAttribute("TXTSTYLE",item->TxTStyle);
	ob->setAttribute("COLUMNS", item->Cols);
	ob->setAttribute("COLGAP", item->ColGap);
	ob->setAttribute("NAMEDLST",item->NamedLStyle);
	ob->setAttribute("SHADE",item->fillShade());
	ob->setAttribute("SHADE2",item->lineShade());
	ob->setAttribute("GRTYP",item->GrType);
	ob->setAttribute("ROT",item->rotation());
	ob->setAttribute("PLINEART",item->PLineArt);
	ob->setAttribute("PLINEEND", item->PLineEnd);
	ob->setAttribute("PLINEJOIN", item->PLineJoin);
	ob->setAttribute("LINESP",item->LineSp);
	ob->setAttribute("LINESPMode", item->LineSpMode);
	ob->setAttribute("TXTKERN",item->ExtraV);
	ob->setAttribute("LOCALSCX",item->imageXScale());
	ob->setAttribute("LOCALSCY",item->imageYScale());
	ob->setAttribute("LOCALX",item->imageXOffset());
	ob->setAttribute("LOCALY",item->imageYOffset());
	ob->setAttribute("PICART", item->PicArt ? 1 : 0);
	ob->setAttribute("PLTSHOW", item->PoShow ? 1 : 0);
	ob->setAttribute("BASEOF", item->BaseOffs);
	ob->setAttribute("FLIPPEDH", item->imageFlippedH());
	ob->setAttribute("FLIPPEDV", item->imageFlippedV());
	ob->setAttribute("BBOXX",item->BBoxX);
	ob->setAttribute("BBOXH",item->BBoxH);
	ob->setAttribute("IFONT",item->IFont);
	ob->setAttribute("ISIZE",item->ISize / 10.0 );
	ob->setAttribute("SCALETYPE", item->ScaleType ? 1 : 0);
	ob->setAttribute("RATIO", item->AspectRatio ? 1 : 0);
	ob->setAttribute("PRINTABLE", item->printable() ? 1 : 0);
	if(item->isAnnotation)
	{
		ob->setAttribute("ANNOTATION",1);
		ob->setAttribute("ANTYPE", item->AnType);
		ob->setAttribute("ANACTION", item->AnAction);
		ob->setAttribute("ANEACT", item->An_E_act);
		ob->setAttribute("ANXACT", item->An_X_act);
		ob->setAttribute("ANDACT", item->An_D_act);
		ob->setAttribute("ANFOACT", item->An_Fo_act);
		ob->setAttribute("ANBLACT", item->An_Bl_act);
		ob->setAttribute("ANKACT", item->An_K_act);
		ob->setAttribute("ANFACT", item->An_F_act);
		ob->setAttribute("ANVACT", item->An_V_act);
		ob->setAttribute("ANCACT", item->An_C_act);
		if (item->AnActType == 8)
			ob->setAttribute("ANEXTERN", item->An_Extern);
		else
			ob->setAttribute("ANEXTERN", Path2Relative(item->An_Extern));
		ob->setAttribute("ANZIEL", item->AnZiel);
		ob->setAttribute("ANACTYP", item->AnActType);
		ob->setAttribute("ANTOOLTIP", item->AnToolTip);
		ob->setAttribute("ANBWID", item->AnBwid);
		ob->setAttribute("ANBSTY", item->AnBsty);
		ob->setAttribute("ANFEED", item->AnFeed);
		ob->setAttribute("ANFLAG", item->AnFlag);
		ob->setAttribute("ANFONT", item->AnFont);
		ob->setAttribute("ANFORMAT", item->AnFormat);
		ob->setAttribute("ANROLL", item->AnRollOver);
		ob->setAttribute("ANDOWN", item->AnDown);
		ob->setAttribute("ANVIS", item->AnVis);
		ob->setAttribute("ANMC", item->AnMaxChar);
		ob->setAttribute("ANCHK", item->AnIsChk);
		ob->setAttribute("ANAA", item->AnAAact);
		ob->setAttribute("ANCHKS", item->AnChkStil);
		ob->setAttribute("ANBCOL", item->AnBColor);
		ob->setAttribute("ANHTML", item->AnHTML);
		ob->setAttribute("ANICON", item->AnUseIcons);
		ob->setAttribute("ANPLACE", item->AnIPlace);
		ob->setAttribute("ANSCALE", item->AnScaleW);
	}
	else
		ob->setAttribute("ANNOTATION",0);
	ob->setAttribute("ANNAME", !item->AutoName ? item->itemName() : QString(""));
	ob->setAttribute("TEXTFLOW", item->textFlowsAroundFrame() ? 1 : 0);
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
	ob->setAttribute("LANGUAGE", item->Language);
	ob->setAttribute("startArrowIndex", item->startArrowIndex());
	ob->setAttribute("endArrowIndex", item->endArrowIndex());
}

bool ScriXmlDoc::ReadLStyles(QString fileName, QMap<QString,multiLine> *Sty)
{
	QDomDocument docu("scridoc");
	QString f(FileLoader::readSLA(fileName));
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

void ScriXmlDoc::GetStyle(QDomElement *pg, struct ParagraphStyle *vg, QValueList<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl)
{
	bool fou;
	QString tmpf, tmf, tmV;
	double xf, xf2;
	fou = false;
	bool tabEQ = false;
	vg->Vname = pg->attribute("NAME");
	vg->LineSpaMode = pg->attribute("LINESPMode", "0").toInt();
	vg->LineSpa = pg->attribute("LINESP").toDouble();
	vg->Indent = pg->attribute("INDENT", "0").toDouble();
	vg->First = pg->attribute("FIRST", "0").toDouble();
	vg->textAlignment = pg->attribute("ALIGN").toInt();
	vg->gapBefore = pg->attribute("VOR", "0").toDouble();
	vg->gapAfter = pg->attribute("NACH", "0").toDouble();
	tmpf = pg->attribute("FONT", doc->toolSettings.defFont);
	if (tmpf.isEmpty())
		tmpf = doc->toolSettings.defFont;
	tmf = tmpf;
	if (!DoFonts.contains(tmpf))
		tmpf = AskForFont(prefsManager->appPrefs.AvailFonts, tmpf, doc);
	else
		tmpf = DoFonts[tmf];
	vg->Font = tmpf;
	vg->FontSize = qRound(pg->attribute("FONTSIZE", "12").toDouble() * 10.0);
	vg->Drop = static_cast<bool>(pg->attribute("DROP", "0").toInt());
	vg->DropLin = pg->attribute("DROPLIN", "2").toInt();
	vg->DropDist = pg->attribute("DROPDIST", "0").toDouble();
	vg->FontEffect = pg->attribute("EFFECT", "0").toInt();
	vg->FColor = pg->attribute("FCOLOR", doc->toolSettings.dBrush);
	vg->FShade = pg->attribute("FSHADE", "100").toInt();
	vg->SColor = pg->attribute("SCOLOR", doc->toolSettings.dPen);
	vg->SShade = pg->attribute("SSHADE", "100").toInt();
	vg->BaseAdj = static_cast<bool>(pg->attribute("BASE", "0").toInt());
	vg->txtShadowX = qRound(pg->attribute("TXTSHX", "5").toDouble() * 10);
	vg->txtShadowY = qRound(pg->attribute("TXTSHY", "-5").toDouble() * 10);
	vg->txtOutline = qRound(pg->attribute("TXTOUT", "1").toDouble() * 10);
	vg->txtUnderPos = qRound(pg->attribute("TXTULP", "-0.1").toDouble() * 10);
	vg->txtUnderWidth = qRound(pg->attribute("TXTULW", "-0.1").toDouble() * 10);
	vg->txtStrikePos = qRound(pg->attribute("TXTSTP", "-0.1").toDouble() * 10);
	vg->txtStrikeWidth = qRound(pg->attribute("TXTSTW", "-0.1").toDouble() * 10);
	vg->scaleH = qRound(pg->attribute("SCALEH", "100").toDouble() * 10);
	vg->scaleV = qRound(pg->attribute("SCALEV", "100").toDouble() * 10);
	vg->baseOff = qRound(pg->attribute("BASEO", "0").toDouble() * 10);
	vg->kernVal = qRound(pg->attribute("KERN", "0").toDouble() * 10);
	vg->TabValues.clear();
	if ((pg->hasAttribute("NUMTAB")) && (pg->attribute("NUMTAB", "0").toInt() != 0))
	{
		struct PageItem::TabRecord tb;
		QString tmp = pg->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		vg->TabValues.clear();
		for (int cxv = 0; cxv < pg->attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			vg->TabValues.append(tb);
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
				struct PageItem::TabRecord tb;
				tb.tabPosition = it.attribute("Pos").toDouble();
				tb.tabType = it.attribute("Type").toInt();
				QString tbCh = "";
				tbCh = it.attribute("Fill","");
				if (tbCh.isEmpty())
					tb.tabFillChar = QChar();
				else
					tb.tabFillChar = tbCh[0];
				vg->TabValues.append(tb);
			}
			IT=IT.nextSibling();
		}
	}
	for (uint xx=0; xx<docParagraphStyles.count(); ++xx)
	{
		if (vg->Vname == docParagraphStyles[xx].Vname)
		{
			struct PageItem::TabRecord tb;
			tabEQ = false;
			if ((docParagraphStyles[xx].TabValues.count() == 0) && (vg->TabValues.count() == 0))
				tabEQ = true;
			else
			{
				for (uint t1 = 0; t1 < docParagraphStyles[xx].TabValues.count(); t1++)
				{
					tb.tabPosition = docParagraphStyles[xx].TabValues[t1].tabPosition;
					tb.tabType = docParagraphStyles[xx].TabValues[t1].tabType;
					tb.tabFillChar = docParagraphStyles[xx].TabValues[t1].tabFillChar;
					for (uint t2 = 0; t2 < vg->TabValues.count(); t2++)
					{
						struct PageItem::TabRecord tb2;
						tb2.tabPosition = vg->TabValues[t2].tabPosition;
						tb2.tabType = vg->TabValues[t2].tabType;
						tb2.tabFillChar = vg->TabValues[t2].tabFillChar;
						if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
						{
							tabEQ = true;
							break;
						}
					}
					if (tabEQ)
						break;
				}
			}
			if ((vg->LineSpa == docParagraphStyles[xx].LineSpa) &&
					(vg->LineSpaMode == docParagraphStyles[xx].LineSpaMode) &&
					(vg->Indent == docParagraphStyles[xx].Indent) &&
					(vg->First == docParagraphStyles[xx].First) &&
					(vg->textAlignment == docParagraphStyles[xx].textAlignment) &&
					(vg->gapBefore == docParagraphStyles[xx].gapBefore) &&
					(vg->gapAfter == docParagraphStyles[xx].gapAfter) &&
					(vg->Font == docParagraphStyles[xx].Font) && (tabEQ) &&
					(vg->Drop == docParagraphStyles[xx].Drop) &&
					(vg->DropLin == docParagraphStyles[xx].DropLin) &&
					(vg->DropDist == docParagraphStyles[xx].DropDist) &&
					(vg->FontEffect == docParagraphStyles[xx].FontEffect) &&
					(vg->FColor == docParagraphStyles[xx].FColor) &&
					(vg->FShade == docParagraphStyles[xx].FShade) &&
					(vg->SColor == docParagraphStyles[xx].SColor) &&
					(vg->SShade == docParagraphStyles[xx].SShade) &&
					(vg->BaseAdj == docParagraphStyles[xx].BaseAdj) &&
					(vg->txtShadowX == docParagraphStyles[xx].txtShadowX) &&
					(vg->txtShadowY == docParagraphStyles[xx].txtShadowY) &&
					(vg->txtOutline == docParagraphStyles[xx].txtOutline) &&
					(vg->txtUnderPos == docParagraphStyles[xx].txtUnderPos) &&
					(vg->txtUnderWidth == docParagraphStyles[xx].txtUnderWidth) &&
					(vg->txtStrikePos == docParagraphStyles[xx].txtStrikePos) &&
					(vg->txtStrikeWidth == docParagraphStyles[xx].txtStrikeWidth) &&
					(vg->scaleH == docParagraphStyles[xx].scaleH) &&
					(vg->scaleV == docParagraphStyles[xx].scaleV) &&
					(vg->baseOff == docParagraphStyles[xx].baseOff) &&
					(vg->kernVal == docParagraphStyles[xx].kernVal) &&
					(vg->FontSize == docParagraphStyles[xx].FontSize))
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
				vg->Vname = "Copy of "+docParagraphStyles[xx].Vname;
				fou = false;
			}
			break;
		}
	}
	if (!fou)
	{
		for (uint xx=0; xx< docParagraphStyles.count(); ++xx)
		{
			struct PageItem::TabRecord tb;
			tabEQ = false;
			for (uint t1 = 0; t1 < docParagraphStyles[xx].TabValues.count(); t1++)
			{
				tb.tabPosition = docParagraphStyles[xx].TabValues[t1].tabPosition;
				tb.tabType = docParagraphStyles[xx].TabValues[t1].tabType;
				tb.tabFillChar = docParagraphStyles[xx].TabValues[t1].tabFillChar;
				for (uint t2 = 0; t2 < vg->TabValues.count(); t2++)
				{
					struct PageItem::TabRecord tb2;
					tb2.tabPosition = vg->TabValues[t2].tabPosition;
					tb2.tabType = vg->TabValues[t2].tabType;
					tb2.tabFillChar = vg->TabValues[t2].tabFillChar;
					if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
					{
						tabEQ = true;
						break;
					}
				}
				if (tabEQ)
					break;
			}
			if ((vg->LineSpa == docParagraphStyles[xx].LineSpa) &&
				(vg->LineSpaMode == docParagraphStyles[xx].LineSpaMode) &&
				(vg->Indent == docParagraphStyles[xx].Indent) &&
				(vg->First == docParagraphStyles[xx].First) &&
				(vg->textAlignment == docParagraphStyles[xx].textAlignment) &&
				(vg->gapBefore == docParagraphStyles[xx].gapBefore) &&
				(vg->gapAfter == docParagraphStyles[xx].gapAfter) &&
				(vg->Font == docParagraphStyles[xx].Font) && (tabEQ) &&
				(vg->Drop == docParagraphStyles[xx].Drop) &&
				(vg->DropLin == docParagraphStyles[xx].DropLin) &&
				(vg->DropDist == docParagraphStyles[xx].DropDist) &&
				(vg->FontEffect == docParagraphStyles[xx].FontEffect) &&
				(vg->FColor == docParagraphStyles[xx].FColor) &&
				(vg->FShade == docParagraphStyles[xx].FShade) &&
				(vg->SColor == docParagraphStyles[xx].SColor) &&
				(vg->SShade == docParagraphStyles[xx].SShade) &&
				(vg->BaseAdj == docParagraphStyles[xx].BaseAdj) &&
				(vg->txtShadowX == docParagraphStyles[xx].txtShadowX) &&
				(vg->txtShadowY == docParagraphStyles[xx].txtShadowY) &&
				(vg->txtOutline == docParagraphStyles[xx].txtOutline) &&
				(vg->txtUnderPos == docParagraphStyles[xx].txtUnderPos) &&
				(vg->txtUnderWidth == docParagraphStyles[xx].txtUnderWidth) &&
				(vg->txtStrikePos == docParagraphStyles[xx].txtStrikePos) &&
				(vg->txtStrikeWidth == docParagraphStyles[xx].txtStrikeWidth) &&
				(vg->scaleH == docParagraphStyles[xx].scaleH) &&
				(vg->scaleV == docParagraphStyles[xx].scaleV) &&
				(vg->baseOff == docParagraphStyles[xx].baseOff) &&
				(vg->kernVal == docParagraphStyles[xx].kernVal) &&
				(vg->FontSize == docParagraphStyles[xx].FontSize))
			{
				vg->Vname = docParagraphStyles[xx].Vname;
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

bool ScriXmlDoc::ReadStyles(QString fileName, ScribusDoc* doc)
{
	struct ParagraphStyle vg;
	QDomDocument docu("scridoc");
	QString tmpf, tmf;
	DoFonts.clear();
	QString f (FileLoader::readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8") && (elem.tagName() != "SCRIBUSUTF8NEW"))
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

bool ScriXmlDoc::ReadColors(QString fileName)
{
	QDomDocument docu("scridoc");
	QString f(FileLoader::readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	Farben.clear();
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8") && (elem.tagName() != "SCRIBUSUTF8NEW"))
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
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!="None")
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
				Farben[pg.attribute("NAME")] = lf;
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}

bool ScriXmlDoc::ReadPageCount(QString fileName, int *num1, int *num2)
{
	QString PgNam;
	int counter = 0;
	int counter2 = 0;
	MNames.clear();
	QDomDocument docu("scridoc");
	QString f(FileLoader::readSLA(fileName));
	if (f.isEmpty())
		return false;
	if(!docu.setContent(f))
		return false;
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8") && (elem.tagName() != "SCRIBUSUTF8NEW"))
		return false;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomNode PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if (elem.tagName() == "SCRIBUSUTF8NEW")
			{
				PgNam = pg.attribute("NAM", "");
				if(pg.tagName()=="PAGE")
					counter++;
				if(pg.tagName()=="MASTERPAGE")
				{
					counter2++;
					MNames.append(PgNam);
				}
			}
			else
			{
				if(pg.tagName()=="PAGE")
				{
					PgNam = pg.attribute("NAM", "");
					if (PgNam.isEmpty())
						counter++;
					else
					{
						counter2++;
						MNames.append(PgNam);
					}
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

bool ScriXmlDoc::ReadPage(QString fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, int PageToLoad, bool Mpage)
{
	struct CopyPasteBuffer OB;
	struct ParagraphStyle vg;
	struct Layer la;
	struct ScribusDoc::BookMa bok;
	struct Linked Link;
	PageItem *Neu;
	LFrames.clear();
	QString tmV, tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	int x, a, counter, baseobj;
	double xf;
	bool newVersion = false;
	bool VorLFound = false;
	QMap<int,int> layerTrans;
	int maxLayer = 0;
	int maxLevel = 0;
	layerTrans.clear();
	uint layerCount=doc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		maxLayer = QMAX(doc->Layers[la2].LNr, maxLayer);
		maxLevel = QMAX(doc->Layers[la2].Level, maxLevel);
	}
	DoVorl.clear();
	DoFonts.clear();
	DoFonts[doc->toolSettings.defFont] = doc->toolSettings.defFont;
	DoVorl[0] = "0";
	DoVorl[1] = "1";
	DoVorl[2] = "2";
	DoVorl[3] = "3";
	DoVorl[4] = "4";
	VorlC = 5;
	QDomDocument docu("scridoc");
	QString f(FileLoader::readSLA(fileName));
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
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!="None")
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
		  		doc->PageColors[pg.attribute("NAME")] = lf;
			}
			if(pg.tagName()=="STYLE")
			{
				GetStyle(&pg, &vg, doc->docParagraphStyles, doc, true);
				VorLFound = true;
			}
			if(pg.tagName()=="JAVA")
				doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = pg.attribute("NUMMER").toInt();
				la.Level = pg.attribute("LEVEL").toInt();
				la.Name = pg.attribute("NAME");
				la.isViewable = pg.attribute("SICHTBAR").toInt();
				la.isPrintable = pg.attribute("DRUCKEN").toInt();
				bool laex = false;
				uint layerCount=doc->layerCount();
				for (uint la2 = 0; la2 < layerCount; ++la2)
				{
					if (doc->Layers[la2].Name == la.Name)
					{
						laex = true;
						layerTrans.insert(la.LNr, doc->Layers[la2].LNr);
					}
				}
				if (!laex)
				{
					maxLayer++;
					maxLevel++;
					layerTrans.insert(la.LNr, maxLayer);
					la.LNr = maxLayer;
					la.Level = maxLevel;
					doc->Layers.append(la);
				}
			}
			if(pg.tagName()=="Bookmark")
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
				doc->BookMarks.append(bok);
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
				while (doc->MLineStyles.contains(Nam2))
				{
					Nam2 = tr("Copy #%1 of ").arg(copyC)+Nam;
					copyC++;
				}
				doc->MLineStyles.insert(Nam2, ml);
			}
			if ((pg.tagName()=="PAGE") && (pg.attribute("NUM").toInt() == PageToLoad))
			{
				a = doc->currentPage->pageNr();
				if ((pg.attribute("NAM", "").isEmpty()) && (Mpage))
				{
					PAGE=PAGE.nextSibling();
					continue;
				}
				if (Mpage)
				{
					doc->Pages->at(a)->LeftPg=pg.attribute("LEFT", "0").toInt();
					doc->Pages->at(a)->setPageName(pg.attribute("NAM",""));
				}
				TableItems.clear();
				TableID.clear();
			/*
			* Attribute von PAGE auslesen
			*/
				if ((pg.hasAttribute("NumVGuides")) && (pg.attribute("NumVGuides", "0").toInt() != 0))
				{
					tmp = pg.attribute("VerticalGuides");
					QTextStream fgv(&tmp, IO_ReadOnly);
					doc->Pages->at(a)->YGuides.clear();
					for (int cxv = 0; cxv < pg.attribute("NumVGuides", "0").toInt(); ++cxv)
					{
						fgv >> xf;
						doc->Pages->at(a)->YGuides.append(xf);
					}
					qHeapSort(doc->Pages->at(a)->YGuides);
					tmp = "";
				}
				else
					doc->Pages->at(a)->YGuides.clear();
				if ((pg.hasAttribute("NumHGuides")) && (pg.attribute("NumHGuides", "0").toInt() != 0))
				{
					tmp = pg.attribute("HorizontalGuides");
					QTextStream fgh(&tmp, IO_ReadOnly);
					doc->Pages->at(a)->XGuides.clear();
					for (int cxh = 0; cxh < pg.attribute("NumHGuides", "0").toInt(); ++cxh)
					{
						fgh >> xf;
						doc->Pages->at(a)->XGuides.append(xf);
					}
					qHeapSort(doc->Pages->at(a)->XGuides);
					tmp = "";
				}
				else
					doc->Pages->at(a)->XGuides.clear();
				QDomNode OBJ=PAGE.firstChild();
				counter = doc->Items->count();
				baseobj = counter;
				while(!OBJ.isNull())
				{
					QDomElement obj=OBJ.toElement();
				/*
				* Attribute von OBJECT auslesen
				*/
					if ((obj.attribute("NEXTITEM").toInt() != -1) && (obj.attribute("NEXTPAGE").toInt() == PageToLoad))
					{
						if (obj.attribute("BACKITEM").toInt() == -1)
						{
							Link.Start = counter;
							Link.StPag = a;
							LFrames.append(Link);
						}
					}
					GetItemProps(newVersion, &obj, &OB);
					OB.Xpos = obj.attribute("XPOS").toDouble()+doc->Pages->at(a)->xOffset();
					OB.Ypos=obj.attribute("YPOS").toDouble()+doc->Pages->at(a)->yOffset();
					OB.NamedLStyle = obj.attribute("NAMEDLST", "");
					if (!doc->MLineStyles.contains(OB.NamedLStyle))
						OB.NamedLStyle = "";
					OB.isBookmark=obj.attribute("BOOKMARK").toInt();
					if ((OB.isBookmark) && (doc->BookMarks.count() == 0))
						doc->OldBM = true;
					OB.BMnr = obj.attribute("BookNr", "0").toInt();
					OB.textAlignment = DoVorl[obj.attribute("ALIGN", "0").toInt()].toUInt();
					tmpf = obj.attribute("IFONT", doc->toolSettings.defFont);
					if (tmpf.isEmpty())
						tmpf = doc->toolSettings.defFont;
					tmf = tmpf;
					if (!DoFonts.contains(tmpf))
						tmpf = AskForFont(avail, tmpf, doc);
					else
						tmpf = DoFonts[tmf];
					OB.IFont = tmpf;
					OB.LayerNr = layerTrans[obj.attribute("LAYER", "0").toInt()];
					OB.Language = obj.attribute("LANGUAGE", doc->Language);
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
						OB.fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
						OB.GrColor = "";
						OB.GrColor2 = "";
					}
					IT=IT.nextSibling();
					}
					OB.itemText = "";
					view->PasteItem(&OB, true);
					Neu = doc->Items->at(counter);
					IT=OBJ.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="ITEXT")
							GetItemText(&it, doc, VorLFound, true, Neu);
						IT=IT.nextSibling();
					}
					if (obj.attribute("NEXTPAGE").toInt() == PageToLoad)
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
				if (LFrames.count() != 0)
				{
					PageItem *Its;
					PageItem *Itn;
					PageItem *Itr;
					QValueList<Linked>::Iterator lc;
					for (lc = LFrames.begin(); lc != LFrames.end(); ++lc)
					{
						Its = doc->Items->at((*lc).Start);
						Itr = Its;
						Its->BackBox = 0;
						while (Its->NextIt != -1)
						{
							if (Its->NextPg == a)
							{
								Itn = doc->Items->at(Its->NextIt);
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
					view->reformPages();
				return true;
			}
		PAGE=PAGE.nextSibling();
		}
	DOC=DOC.nextSibling();
	}
	return false;
}

bool ScriXmlDoc::ReadDoc(QString fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, QProgressBar *dia2)
{
	//Scribus 1.2 docs, see fileloader.cpp for 1.3 docs
	struct CopyPasteBuffer OB;
	struct ParagraphStyle vg;
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
	QString f(FileLoader::readSLA(fileName));
	// Build the DOM from it
	if (!docu.setContent(f))
		return false;
	// and begin loading the doc
	doc->PageColors.clear();
	doc->Layers.clear();
	ScColor lf = ScColor();
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8"))
		return false;
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	dia2->setTotalSteps(DOC.childNodes().count());
	dia2->setProgress(0);
	int ObCount = 0;
	int activeLayer = 0;
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
	/*
	* Attribute von DOCUMENT auslesen
	*/
		if (dc.hasAttribute("PAGEWIDTH"))
			doc->pageWidth=dc.attribute("PAGEWIDTH").toDouble();
		else
			doc->pageWidth=dc.attribute("PAGEWITH").toDouble();
		doc->pageHeight=dc.attribute("PAGEHEIGHT").toDouble();
		doc->pageMargins.Left=dc.attribute("BORDERLEFT").toDouble();
		doc->pageMargins.Right=dc.attribute("BORDERRIGHT").toDouble();
		doc->pageMargins.Top=dc.attribute("BORDERTOP").toDouble();
		doc->pageMargins.Bottom=dc.attribute("BORDERBOTTOM").toDouble();
		doc->PageOri = dc.attribute("ORIENTATION", "0").toInt();
		doc->PageSize = dc.attribute("PAGESIZE");
		doc->FirstPnum = dc.attribute("FIRSTNUM", "1").toInt();
		doc->currentPageLayout=dc.attribute("BOOK", "0").toInt();
		int fp;
		if (dc.attribute("FIRSTLEFT", "0").toInt() == 1)
			fp = 0;
		else
			fp = 1;
		if (doc->currentPageLayout == 0)
			fp = 0;
		doc->pageSets[doc->currentPageLayout].FirstPage = fp;
		doc->setUsesAutomaticTextFrames(dc.attribute("AUTOTEXT").toInt());
		doc->PageSp=dc.attribute("AUTOSPALTEN").toInt();
		doc->PageSpa=dc.attribute("ABSTSPALTEN").toDouble();
		doc->setUnitIndex(dc.attribute("UNITS", "0").toInt());
		doc->guidesSettings.gridShown = view->Prefs->guidesSettings.gridShown;
		doc->guidesSettings.guidesShown = view->Prefs->guidesSettings.guidesShown;
		doc->guidesSettings.framesShown = view->Prefs->guidesSettings.framesShown;
		doc->guidesSettings.marginsShown = view->Prefs->guidesSettings.marginsShown;
		doc->guidesSettings.baseShown = view->Prefs->guidesSettings.baseShown;
		doc->guidesSettings.linkShown = view->Prefs->guidesSettings.linkShown;
		doc->guidesSettings.showPic = true;
		doc->guidesSettings.showControls = false;
		DoFonts.clear();
		doc->toolSettings.defSize=qRound(dc.attribute("DSIZE").toDouble() * 10);
		Defont=dc.attribute("DFONT");
		if ((!avail.find(Defont)) || (!avail[Defont]->UseFont))
		{
			ReplacedFonts.insert(Defont, view->Prefs->toolSettings.defFont);
			Defont = view->Prefs->toolSettings.defFont;
		}
		else
		{
			if (!doc->UsedFonts.contains(tmpf))
			{
//				QFont fo = avail[Defont]->Font;
//				fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
				doc->AddFont(Defont);
			}
		}
		doc->toolSettings.defFont = Defont;
		doc->toolSettings.dCols=dc.attribute("DCOL", "1").toInt();
		doc->toolSettings.dGap=dc.attribute("DGAP", "0.0").toDouble();
		doc->documentInfo.setAuthor(dc.attribute("AUTHOR"));
		doc->documentInfo.setComments(dc.attribute("COMMENTS"));
		doc->documentInfo.setKeywords(dc.attribute("KEYWORDS",""));
		doc->documentInfo.setTitle(dc.attribute("TITLE"));
		doc->documentInfo.setPublisher(dc.attribute("PUBLISHER", ""));
		doc->documentInfo.setDate(dc.attribute("DOCDATE", ""));
		doc->documentInfo.setType(dc.attribute("DOCTYPE", ""));
		doc->documentInfo.setFormat(dc.attribute("DOCFORMAT", ""));
		doc->documentInfo.setIdent(dc.attribute("DOCIDENT", ""));
		doc->documentInfo.setSource(dc.attribute("DOCSOURCE", ""));
		doc->documentInfo.setLangInfo(dc.attribute("DOCLANGINFO", ""));
		doc->documentInfo.setRelation(dc.attribute("DOCRELATION", ""));
		doc->documentInfo.setCover(dc.attribute("DOCCOVER", ""));
		doc->documentInfo.setRights(dc.attribute("DOCRIGHTS", ""));
		doc->documentInfo.setContrib(dc.attribute("DOCCONTRIB", ""));
		doc->typographicSettings.valueSuperScript = dc.attribute("VHOCH").toInt();
		doc->typographicSettings.scalingSuperScript = dc.attribute("VHOCHSC").toInt();
		doc->typographicSettings.valueSubScript = dc.attribute("VTIEF").toInt();
		doc->typographicSettings.scalingSubScript = dc.attribute("VTIEFSC").toInt();
		doc->typographicSettings.valueSmallCaps = dc.attribute("VKAPIT").toInt();
		doc->typographicSettings.valueBaseGrid = dc.attribute("BASEGRID", "12").toDouble();
		doc->typographicSettings.offsetBaseGrid = dc.attribute("BASEO", "0").toDouble();
		doc->typographicSettings.autoLineSpacing = dc.attribute("AUTOL", "20").toInt();
		doc->GroupCounter=dc.attribute("GROUPC", "1").toInt();
		doc->HasCMS = static_cast<bool>(dc.attribute("HCMS", "0").toInt());
		doc->CMSSettings.SoftProofOn = static_cast<bool>(dc.attribute("DPSo", "0").toInt());
		doc->CMSSettings.CMSinUse = static_cast<bool>(dc.attribute("DPuse", "0").toInt());
		doc->CMSSettings.GamutCheck = static_cast<bool>(dc.attribute("DPgam", "0").toInt());
		doc->CMSSettings.BlackPoint = static_cast<bool>(dc.attribute("DPbla", "1").toInt());
		doc->CMSSettings.DefaultMonitorProfile = dc.attribute("DPMo","");
		doc->CMSSettings.DefaultPrinterProfile = dc.attribute("DPPr","");
		doc->CMSSettings.DefaultImageRGBProfile = dc.attribute("DPIn","");
		doc->CMSSettings.DefaultSolidColorProfile = dc.attribute("DPIn2","");
		doc->CMSSettings.DefaultIntentPrinter = dc.attribute("DIPr", "0").toInt();
		doc->CMSSettings.DefaultIntentMonitor = dc.attribute("DIMo", "1").toInt();
		doc->CMSSettings.DefaultIntentImages = dc.attribute("DIMo2", "1").toInt();
		activeLayer = dc.attribute("ALAYER", "0").toInt();
		doc->Language = dc.attribute("LANGUAGE", "");
		doc->MinWordLen = dc.attribute("MINWORDLEN", "3").toInt();
		doc->HyCount = dc.attribute("HYCOUNT", "2").toInt();
		doc->Automatic = static_cast<bool>(dc.attribute("AUTOMATIC", "1").toInt());
		doc->AutoCheck = static_cast<bool>(dc.attribute("AUTOCHECK", "0").toInt());
		doc->GuideLock = static_cast<bool>(dc.attribute("GUIDELOCK", "0").toInt());
		doc->SnapGuides = static_cast<bool>(dc.attribute("SnapToGuides", "0").toInt());
		doc->useRaster = static_cast<bool>(dc.attribute("SnapToGrid", "0").toInt());
		doc->guidesSettings.minorGrid = dc.attribute("MINGRID", tmp.setNum(view->Prefs->guidesSettings.minorGrid)).toDouble();
		doc->guidesSettings.majorGrid = dc.attribute("MAJGRID", tmp.setNum(view->Prefs->guidesSettings.majorGrid)).toDouble();
		doc->toolSettings.dStartArrow = 0;
		doc->toolSettings.dEndArrow = 0;
		doc->LastAuto = 0;
		QDomNode PAGE=DOC.firstChild();
		counter = 0;
		while(!PAGE.isNull())
		{
			ObCount++;
			dia2->setProgress(ObCount);
			QDomElement pg=PAGE.toElement();
			// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!="None")
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
			  doc->PageColors[pg.attribute("NAME")] = lf;
			}
			if(pg.tagName()=="STYLE")
			{
				vg.Vname = pg.attribute("NAME");
				vg.LineSpaMode = pg.attribute("LINESPMode", "0").toInt();
				vg.LineSpa = pg.attribute("LINESP").toDouble();
				vg.Indent = pg.attribute("INDENT", "0").toDouble();
				vg.First = pg.attribute("FIRST", "0").toDouble();
				vg.textAlignment = pg.attribute("ALIGN").toInt();
				vg.gapBefore = pg.attribute("VOR", "0").toDouble();
				vg.gapAfter = pg.attribute("NACH", "0").toDouble();
				tmpf = pg.attribute("FONT", doc->toolSettings.defFont);
				if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
				{
					if ((!view->Prefs->GFontSub.contains(tmpf)) || (!avail[view->Prefs->GFontSub[tmpf]]->UseFont))
					{
						newReplacement = true;
						ReplacedFonts.insert(tmpf, view->Prefs->toolSettings.defFont);
					}
					else
						ReplacedFonts.insert(tmpf, view->Prefs->GFontSub[tmpf]);
				}
				else
				{
					if (!doc->UsedFonts.contains(tmpf))
					{
//						QFont fo = avail[tmpf]->Font;
//						fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
						doc->AddFont(tmpf);
					}
				}
				vg.Font = tmpf;
				vg.FontSize = qRound(pg.attribute("FONTSIZE", "12").toDouble() * 10.0);
				vg.Drop = static_cast<bool>(pg.attribute("DROP", "0").toInt());
				vg.DropLin = pg.attribute("DROPLIN", "2").toInt();
				vg.DropDist = pg.attribute("DROPDIST", "0").toDouble();
				vg.FontEffect = pg.attribute("EFFECT", "0").toInt();
				vg.FColor = pg.attribute("FCOLOR", doc->toolSettings.dBrush);
				vg.FShade = pg.attribute("FSHADE", "100").toInt();
				vg.SColor = pg.attribute("SCOLOR", doc->toolSettings.dPen);
				vg.SShade = pg.attribute("SSHADE", "100").toInt();
				vg.BaseAdj = static_cast<bool>(pg.attribute("BASE", "0").toInt());
				vg.txtShadowX = 50;
				vg.txtShadowY = -50;
				vg.txtOutline = 10;
				vg.txtUnderPos = doc->typographicSettings.valueUnderlinePos;
				vg.txtUnderWidth = doc->typographicSettings.valueUnderlineWidth;
				vg.txtStrikePos = doc->typographicSettings.valueStrikeThruPos;
				vg.txtStrikeWidth = doc->typographicSettings.valueStrikeThruPos;
				vg.scaleH = 1000;
				vg.scaleV = 1000;
				vg.baseOff = 0;
				vg.kernVal = 0;
				if ((pg.hasAttribute("NUMTAB")) && (pg.attribute("NUMTAB", "0").toInt() != 0))
				{
					tmp = pg.attribute("TABS");
					QTextStream tgv(&tmp, IO_ReadOnly);
					vg.TabValues.clear();
					struct PageItem::TabRecord tb;
					for (int cxv = 0; cxv < pg.attribute("NUMTAB", "0").toInt(); cxv += 2)
					{
						tgv >> xf;
						tgv >> xf2;
						tb.tabPosition = xf2;
						tb.tabType = static_cast<int>(xf);
						tb.tabFillChar = QChar();
						vg.TabValues.append(tb);
					}
					tmp = "";
				}
				else
					vg.TabValues.clear();
				doc->docParagraphStyles.append(vg);
			}
			if(pg.tagName()=="JAVA")
				doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = pg.attribute("NUMMER").toInt();
				la.Level = pg.attribute("LEVEL").toInt();
				la.Name = pg.attribute("NAME");
				la.isViewable = pg.attribute("SICHTBAR").toInt();
				la.isPrintable = pg.attribute("DRUCKEN").toInt();
				doc->Layers.append(la);
			}
			if(pg.tagName()=="Bookmark")
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
				doc->BookMarks.append(bok);
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
				doc->MLineStyles.insert(pg.attribute("Name"), ml);
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
					doc->setMasterPageMode(false);
					doc->currentPage=doc->addPage(a);
					doc->currentPage->MPageNam=Mus;
				}
				else
				{
					doc->setMasterPageMode(true);
					doc->currentPage=doc->addMasterPage(a, PgNam);
				}
				//CB: Remove this unnecessarily "slow" slot call when we have no gui for the doc yet!
				//Items dont appear in the right place if we just doc->addPage(a); for <=1.2.x docs
				//so we have to call the view, but we certainly dont need to emit to the mainwindow!
				//This call now picks up the added page and does some view black magic. A must for 
				//1.2.x docs!
				view->addPage(a);
				//emit NewPage(a);
				doc->Pages->at(a)->LeftPg=pg.attribute("LEFT", "0").toInt();

				if ((pg.hasAttribute("NumVGuides")) && (pg.attribute("NumVGuides", "0").toInt() != 0))
				{
					tmp = pg.attribute("VerticalGuides");
					QTextStream fgv(&tmp, IO_ReadOnly);
					doc->Pages->at(a)->YGuides.clear();
					for (int cxv = 0; cxv < pg.attribute("NumVGuides", "0").toInt(); ++cxv)
					{
						fgv >> xf;
						doc->Pages->at(a)->YGuides.append(xf);
					}
					qHeapSort(doc->Pages->at(a)->YGuides);
					tmp = "";
				}
				else
					doc->Pages->at(a)->YGuides.clear();
				if ((pg.hasAttribute("NumHGuides")) && (pg.attribute("NumHGuides", "0").toInt() != 0))
				{
					tmp = pg.attribute("HorizontalGuides");
					QTextStream fgh(&tmp, IO_ReadOnly);
					doc->Pages->at(a)->XGuides.clear();
					for (int cxh = 0; cxh < pg.attribute("NumHGuides", "0").toInt(); ++cxh)
					{
						fgh >> xf;
						doc->Pages->at(a)->XGuides.append(xf);
					}
					qHeapSort(doc->Pages->at(a)->XGuides);
					tmp = "";
				}
				else
					doc->Pages->at(a)->XGuides.clear();
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
					OB.Xpos = obj.attribute("XPOS").toDouble()+doc->Pages->at(a)->xOffset();
					OB.Ypos=obj.attribute("YPOS").toDouble()+doc->Pages->at(a)->yOffset();
					OB.NamedLStyle = obj.attribute("NAMEDLST", "");
					OB.isBookmark=obj.attribute("BOOKMARK").toInt();
					if ((OB.isBookmark) && (doc->BookMarks.count() == 0))
						doc->OldBM = true;
					OB.BMnr = obj.attribute("BookNr", "0").toInt();
					OB.textAlignment = obj.attribute("ALIGN", "0").toInt();
					OB.startArrowIndex =  0;
					OB.endArrowIndex =  0;
					tmpf = obj.attribute("IFONT", doc->toolSettings.defFont);
					if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
					{
						if ((!view->Prefs->GFontSub.contains(tmpf)) || (!avail[view->Prefs->GFontSub[tmpf]]->UseFont))
						{
							newReplacement = true;
							ReplacedFonts.insert(tmpf, view->Prefs->toolSettings.defFont);
						}
						else
							ReplacedFonts.insert(tmpf, view->Prefs->GFontSub[tmpf]);
					}
					else
					{
						if (!doc->UsedFonts.contains(tmpf))
						{
//							QFont fo = avail[tmpf]->Font;
//							fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
							doc->AddFont(tmpf);
						}
					}
					OB.IFont = tmpf;
					OB.LayerNr = obj.attribute("LAYER", "0").toInt();
					OB.Language = obj.attribute("LANGUAGE", doc->Language);
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
							OB.fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
							OB.GrColor = "";
							OB.GrColor2 = "";
						}
						IT=IT.nextSibling();
					}
					OB.itemText = "";
					int docGc = doc->GroupCounter;
					doc->GroupCounter = 0;
					uint last = doc->Items->count();
					view->PasteItem(&OB, true);
					doc->GroupCounter = docGc;
					Neu = doc->Items->at(last);
					Neu->OnMasterPage = PgNam;
					Neu->OwnPage = a; //No need to scan for OnPage as we know page by page in 1.2.x
					Neu->oldOwnPage = 0;
					Neu->setRedrawBounding();
					IT=OBJ.firstChild();
					while(!IT.isNull())
					{
						QDomElement it=IT.toElement();
						if (it.tagName()=="ITEXT")
							GetItemText(&it, doc, false, false, Neu);
						IT=IT.nextSibling();
					}
					Neu->isAutoText=static_cast<bool>(obj.attribute("AUTOTEXT").toInt());
					if (Neu->isAutoText)
						doc->LastAuto = Neu;
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
			}
			PAGE=PAGE.nextSibling();
		}
		PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="PDF")
			{
				doc->PDF_Options.Articles = static_cast<bool>(pg.attribute("Articles").toInt());
				doc->PDF_Options.Thumbnails = static_cast<bool>(pg.attribute("Thumbnails").toInt());
				doc->PDF_Options.Compress = static_cast<bool>(pg.attribute("Compress").toInt());
				doc->PDF_Options.CompressMethod = pg.attribute("CMethod", "0").toInt();
				doc->PDF_Options.Quality = pg.attribute("Quality", "0").toInt();
				doc->PDF_Options.RecalcPic = static_cast<bool>(pg.attribute("RecalcPic").toInt());
				doc->PDF_Options.Bookmarks = static_cast<bool>(pg.attribute("Bookmarks").toInt());
				if (pg.hasAttribute("MirrorH"))
					doc->PDF_Options.MirrorH = static_cast<bool>(pg.attribute("MirrorH").toInt());
				else
					doc->PDF_Options.MirrorH = false;
				if (pg.hasAttribute("MirrorV"))
					doc->PDF_Options.MirrorV = static_cast<bool>(pg.attribute("MirrorV").toInt());
				else
					doc->PDF_Options.MirrorV = false;
				if (pg.hasAttribute("RotateDeg"))
					doc->PDF_Options.RotateDeg = pg.attribute("RotateDeg", "0").toInt();
				else
					doc->PDF_Options.RotateDeg = 0;
				doc->PDF_Options.PresentMode = static_cast<bool>(pg.attribute("PresentMode").toInt());
				doc->PDF_Options.PicRes = pg.attribute("PicRes").toInt();
				doc->PDF_Options.Version = (PDFOptions::PDFVersion)pg.attribute("Version").toInt();
				doc->PDF_Options.Resolution = pg.attribute("Resolution").toInt();
				doc->PDF_Options.Binding = pg.attribute("Binding").toInt();
				doc->PDF_Options.Datei = "";
				doc->PDF_Options.isGrayscale = false;
				doc->PDF_Options.UseRGB = static_cast<bool>(pg.attribute("RGBMode", "0").toInt());
				doc->PDF_Options.UseProfiles = static_cast<bool>(pg.attribute("UseProfiles", "0").toInt());
				doc->PDF_Options.UseProfiles2 = static_cast<bool>(pg.attribute("UseProfiles2", "0").toInt());
				doc->PDF_Options.Intent = pg.attribute("Intent", "1").toInt();
				doc->PDF_Options.Intent2 = pg.attribute("Intent2", "1").toInt();
				doc->PDF_Options.SolidProf = pg.attribute("SolidP", "");
				doc->PDF_Options.ImageProf = pg.attribute("ImageP", "");
				doc->PDF_Options.PrintProf = pg.attribute("PrintP", "");
				doc->PDF_Options.Info = pg.attribute("InfoString", "");
				doc->PDF_Options.BleedTop = pg.attribute("BTop", "0").toDouble();
				doc->PDF_Options.BleedLeft = pg.attribute("BLeft", "0").toDouble();
				doc->PDF_Options.BleedRight = pg.attribute("BRight", "0").toDouble();
				doc->PDF_Options.BleedBottom = pg.attribute("BBottom", "0").toDouble();
				doc->PDF_Options.EmbeddedI = static_cast<bool>(pg.attribute("ImagePr", "0").toInt());
				doc->PDF_Options.PassOwner = pg.attribute("PassOwner", "");
				doc->PDF_Options.PassUser = pg.attribute("PassUser", "");
				doc->PDF_Options.Permissions = pg.attribute("Permissions", "-4").toInt();
				doc->PDF_Options.Encrypt = static_cast<bool>(pg.attribute("Encrypt", "0").toInt());
				doc->PDF_Options.useLayers = static_cast<bool>(pg.attribute("UseLayers", "0").toInt());
				doc->PDF_Options.UseLPI = static_cast<bool>(pg.attribute("UseLpi", "0").toInt());
				doc->PDF_Options.UseSpotColors = true;
				doc->PDF_Options.doMultiFile = false;
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
						doc->PDF_Options.LPISettings[pdfF.attribute("Color")] = lpo;
					}
					if(pdfF.tagName() == "Fonts")
					{
						if (!doc->PDF_Options.EmbedList.contains(pdfF.attribute("Name")))
							doc->PDF_Options.EmbedList.append(pdfF.attribute("Name"));
					}
					if(pdfF.tagName() == "Subset")
					{
						if (!doc->PDF_Options.SubsetList.contains(pdfF.attribute("Name")))
							doc->PDF_Options.SubsetList.append(pdfF.attribute("Name"));
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
						doc->PDF_Options.PresentVals.append(ef);
					}
					PFO = PFO.nextSibling();
				}
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	//doc->Pages = &doc->DocPages;
	//doc->pageCount = doc->Pages->count();
	//doc->Items = doc->DocItems;
	doc->setMasterPageMode(false);
	view->reformPages();
	if (doc->layerCount() == 0)
	{
		la.LNr = 0;
		la.Level = 0;
		la.Name = tr("Background");
		la.isViewable = true;
		la.isPrintable = true;
		doc->Layers.append(la);
	}
	doc->setActiveLayer(activeLayer);
	if (LFrames.count() != 0)
	{
		PageItem *Its;
		PageItem *Itn;
		PageItem *Itr;
		QValueList<Linked>::Iterator lc;
		for (lc = LFrames.begin(); lc != LFrames.end(); ++lc)
		{
			Its = doc->Items->at((*lc).Start);
			Itr = Its;
			Its->BackBox = 0;
			if (Its->isAutoText)
				doc->FirstAuto = Its;
			while (Its->NextIt != -1)
			{
				int itnr = 0;
				for (uint nn = 0; nn < doc->Items->count(); ++nn)
				{
					if (doc->Items->at(nn)->OwnPage == Its->NextPg)
					{
						if (itnr == Its->NextIt)
						{
							itnr = nn;
							break;
						}
						itnr++;
					}
				}
				Itn = doc->Items->at(itnr);
				Its->NextBox = Itn;
				Itn->BackBox = Its;
				Its = Itn;
			}
			Its->NextBox = 0;
		}
	}
	view->unitSwitcher->setCurrentText(unitGetStrFromIndex(doc->unitIndex()));
	dia2->setProgress(DOC.childNodes().count());
	return true;
}

bool ScriXmlDoc::ReadElemHeader(QString file, bool isFile, double *x, double *y, double *w, double *h)
{
	QString f;
	QDomDocument docu("scridoc");
	if (isFile)
	{
		if (!loadText(file, &f))
			return false;
		QString ff = "";
		if (f.startsWith("<SCRIBUSELEMUTF8"))
			ff = QString::fromUtf8(f);
		else
			ff = f;
		if(!docu.setContent(ff))
			return false;
	}
	else
	{
		QString ff = "";
		if (file.startsWith("<SCRIBUSELEMUTF8"))
			ff = QString::fromUtf8(file);
		else
			ff = file;
		if(!docu.setContent(ff))
			return false;
	}
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
	struct ParagraphStyle vg;
	QString tmp, tmpf, tmp2, tmp3, tmp4, f, tmV, tmf;
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
		if (!loadText(fileName, &f))
			return false;
		QString ff = "";
		if (f.startsWith("<SCRIBUSELEMUTF8"))
			ff = QString::fromUtf8(f);
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
			if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
			{
				if (!FontSub.contains(tmpf) || (!avail[FontSub[tmpf]]->UseFont))
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
		if(pg.tagName()=="COLOR" && pg.attribute("Name")!="None")
		{
			if (pg.hasAttribute("CMYK"))
				lf.setNamedColor(pg.attribute("CMYK"));
			else
				lf.fromQColor(QColor(pg.attribute("RGB")));
			if (pg.hasAttribute("Spot"))
				lf.setSpotColor(static_cast<bool>(pg.attribute("Spot")));
			else
				lf.setSpotColor(false);
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
			OB.BMnr = 0;
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
					struct PageItem::TabRecord tb;
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
				IT=IT.nextSibling();
			}
			if (Neu->effectsInUse.count() != 0)
				doc->LoadPict(Neu->Pfile, Neu->ItemNr, true);
			if (Neu->isTableItem)
			{
				TableItems.append(Neu);
				TableID.insert(pg.attribute("OwnLINK", "0").toInt(), Neu->ItemNr);
			}
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

//QString ScriXmlDoc::WriteElem(QPtrList<PageItem> *Selitems, ScribusDoc *doc, ScribusView *view)
QString ScriXmlDoc::WriteElem(ScribusDoc *doc, ScribusView *view, int selectionListNumber)
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
	item = doc->selection->itemAt(0, selectionListNumber);
	QValueList<uint> ELL;
	//for (uint cor=0; cor<Selitems->count(); ++cor)
	for (uint cor=0; cor<doc->selection->count(selectionListNumber); ++cor)
		//ELL.append(Selitems->at(cor)->ItemNr);
		ELL.append(doc->selection->itemAt(cor, selectionListNumber)->ItemNr);
	qHeapSort(ELL);
	if (doc->selection->isMultipleSelection())
	{
		xp = view->GroupX - doc->currentPage->xOffset();
		yp = view->GroupY - doc->currentPage->yOffset();
		elem.setAttribute("W", view->GroupW);
		elem.setAttribute("H", view->GroupH);
	}
	else
	{
		xp = item->xPos() - doc->currentPage->xOffset();
		yp = item->yPos() - doc->currentPage->yOffset();
		elem.setAttribute("W", item->width());
		elem.setAttribute("H", item->height());
	}
	elem.setAttribute("XP", xp);
	elem.setAttribute("YP", yp);
	//elem.setAttribute("COUNT", Selitems->count());
	elem.setAttribute("COUNT", doc->selection->count(selectionListNumber));
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
		co.setAttribute("RGB",doc->PageColors[itc.key()].getRGBColor().name());
		co.setAttribute("CMYK",doc->PageColors[itc.key()].name());
		co.setAttribute("Spot",static_cast<int>(doc->PageColors[itc.key()].isSpotColor()));
		co.setAttribute("Register",static_cast<int>(doc->PageColors[itc.key()].isRegistrationColor()));
		elem.appendChild(co);
	}
	QMap<int, ParagraphStyle> UsedStyles;
	QMap<int, int> UsedMapped2Saved;
	int NewStylesNum = 5;
	UsedStyles.clear();
	UsedMapped2Saved.clear();
	struct ParagraphStyle vg;
	if (doc->docParagraphStyles.count() > 5)
	{
		//for (uint co=0; co<Selitems->count(); ++co)
		for (uint co=0; co<doc->selection->count(selectionListNumber); ++co)
		{
			item = doc->Items->at(ELL[co]);
			if (item->textAlignment > 4)
			{
				vg.Vname = doc->docParagraphStyles[item->textAlignment].Vname;
				vg.LineSpaMode = doc->docParagraphStyles[item->textAlignment].LineSpaMode;
				vg.LineSpa = doc->docParagraphStyles[item->textAlignment].LineSpa;
				vg.textAlignment = doc->docParagraphStyles[item->textAlignment].textAlignment;
				vg.Indent = doc->docParagraphStyles[item->textAlignment].Indent;
				vg.First = doc->docParagraphStyles[item->textAlignment].First;
				vg.gapBefore = doc->docParagraphStyles[item->textAlignment].gapBefore;
				vg.gapAfter = doc->docParagraphStyles[item->textAlignment].gapAfter;
				vg.Font = doc->docParagraphStyles[item->textAlignment].Font;
				vg.FontSize = doc->docParagraphStyles[item->textAlignment].FontSize;
				vg.TabValues = doc->docParagraphStyles[item->textAlignment].TabValues;
				vg.Drop = doc->docParagraphStyles[item->textAlignment].Drop;
				vg.DropLin = doc->docParagraphStyles[item->textAlignment].DropLin;
				vg.DropDist = doc->docParagraphStyles[item->textAlignment].DropDist;
				vg.FontEffect = doc->docParagraphStyles[item->textAlignment].FontEffect;
				vg.FColor = doc->docParagraphStyles[item->textAlignment].FColor;
				vg.FShade = doc->docParagraphStyles[item->textAlignment].FShade;
				vg.SColor = doc->docParagraphStyles[item->textAlignment].SColor;
				vg.SShade = doc->docParagraphStyles[item->textAlignment].SShade;
				vg.BaseAdj = doc->docParagraphStyles[item->textAlignment].BaseAdj;
				vg.txtShadowX = doc->docParagraphStyles[item->textAlignment].txtShadowX;
				vg.txtShadowY = doc->docParagraphStyles[item->textAlignment].txtShadowY;
				vg.txtOutline = doc->docParagraphStyles[item->textAlignment].txtOutline;
				vg.txtUnderPos = doc->docParagraphStyles[item->textAlignment].txtUnderPos;
				vg.txtUnderWidth = doc->docParagraphStyles[item->textAlignment].txtUnderWidth;
				vg.txtStrikePos = doc->docParagraphStyles[item->textAlignment].txtStrikePos;
				vg.txtStrikeWidth = doc->docParagraphStyles[item->textAlignment].txtStrikeWidth;
				vg.scaleH = doc->docParagraphStyles[item->textAlignment].scaleH;
				vg.scaleV = doc->docParagraphStyles[item->textAlignment].scaleV;
				vg.baseOff = doc->docParagraphStyles[item->textAlignment].baseOff;
				vg.kernVal = doc->docParagraphStyles[item->textAlignment].kernVal;
				UsedStyles[item->textAlignment] = vg;
			}
			if (((item->asTextFrame()) || (item->asPathText())) && (item->itemText.count() != 0))
			{
				for (uint tx = 0; tx < item->itemText.count(); ++tx)
				{
					if (item->itemText.at(tx)->cab > 4)
					{
						vg.Vname = doc->docParagraphStyles[item->itemText.at(tx)->cab].Vname;
						vg.LineSpaMode = doc->docParagraphStyles[item->itemText.at(tx)->cab].LineSpaMode;
						vg.LineSpa = doc->docParagraphStyles[item->itemText.at(tx)->cab].LineSpa;
						vg.textAlignment = doc->docParagraphStyles[item->itemText.at(tx)->cab].textAlignment;
						vg.Indent = doc->docParagraphStyles[item->itemText.at(tx)->cab].Indent;
						vg.First = doc->docParagraphStyles[item->itemText.at(tx)->cab].First;
						vg.gapBefore = doc->docParagraphStyles[item->itemText.at(tx)->cab].gapBefore;
						vg.gapAfter = doc->docParagraphStyles[item->itemText.at(tx)->cab].gapAfter;
						vg.Font = doc->docParagraphStyles[item->itemText.at(tx)->cab].Font;
						vg.FontSize = doc->docParagraphStyles[item->itemText.at(tx)->cab].FontSize;
						vg.TabValues = doc->docParagraphStyles[item->itemText.at(tx)->cab].TabValues;
						vg.Drop = doc->docParagraphStyles[item->itemText.at(tx)->cab].Drop;
						vg.DropLin = doc->docParagraphStyles[item->itemText.at(tx)->cab].DropLin;
						vg.DropDist = doc->docParagraphStyles[item->itemText.at(tx)->cab].DropDist;
						vg.FontEffect = doc->docParagraphStyles[item->itemText.at(tx)->cab].FontEffect;
						vg.FColor = doc->docParagraphStyles[item->itemText.at(tx)->cab].FColor;
						vg.FShade = doc->docParagraphStyles[item->itemText.at(tx)->cab].FShade;
						vg.SColor = doc->docParagraphStyles[item->itemText.at(tx)->cab].SColor;
						vg.SShade = doc->docParagraphStyles[item->itemText.at(tx)->cab].SShade;
						vg.BaseAdj = doc->docParagraphStyles[item->itemText.at(tx)->cab].BaseAdj;
						vg.txtShadowX = doc->docParagraphStyles[item->itemText.at(tx)->cab].txtShadowX;
						vg.txtShadowY = doc->docParagraphStyles[item->itemText.at(tx)->cab].txtShadowY;
						vg.txtOutline = doc->docParagraphStyles[item->itemText.at(tx)->cab].txtOutline;
						vg.txtUnderPos = doc->docParagraphStyles[item->itemText.at(tx)->cab].txtUnderPos;
						vg.txtUnderWidth = doc->docParagraphStyles[item->itemText.at(tx)->cab].txtUnderWidth;
						vg.txtStrikePos = doc->docParagraphStyles[item->itemText.at(tx)->cab].txtStrikePos;
						vg.txtStrikeWidth = doc->docParagraphStyles[item->itemText.at(tx)->cab].txtStrikeWidth;
						vg.scaleH = doc->docParagraphStyles[item->itemText.at(tx)->cab].scaleH;
						vg.scaleV = doc->docParagraphStyles[item->itemText.at(tx)->cab].scaleV;
						vg.baseOff = doc->docParagraphStyles[item->itemText.at(tx)->cab].baseOff;
						vg.kernVal = doc->docParagraphStyles[item->itemText.at(tx)->cab].kernVal;
						UsedStyles[item->itemText.at(tx)->cab] = vg;
					}
				}
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
			fo.setAttribute("NAME",UsedStyles[actSt].Vname);
			fo.setAttribute("ALIGN",UsedStyles[actSt].textAlignment);
			fo.setAttribute("LINESPMode",UsedStyles[actSt].LineSpaMode);
			fo.setAttribute("LINESP",UsedStyles[actSt].LineSpa);
			fo.setAttribute("INDENT",UsedStyles[actSt].Indent);
			fo.setAttribute("FIRST",UsedStyles[actSt].First);
			fo.setAttribute("VOR",UsedStyles[actSt].gapBefore);
			fo.setAttribute("NACH",UsedStyles[actSt].gapAfter);
			fo.setAttribute("FONT",UsedStyles[actSt].Font);
			fo.setAttribute("FONTSIZE",UsedStyles[actSt].FontSize / 10.0);
			fo.setAttribute("DROP", static_cast<int>(UsedStyles[actSt].Drop));
			fo.setAttribute("DROPLIN", UsedStyles[actSt].DropLin);
			fo.setAttribute("DROPDIST", UsedStyles[actSt].DropDist);
			fo.setAttribute("EFFECT", UsedStyles[actSt].FontEffect);
			if (UsedStyles[actSt].TabValues.count() != 0)
			{
				for (uint a = 0; a < UsedStyles[actSt].TabValues.count(); ++a)
				{
					QDomElement tabs = docu.createElement("Tabs");
					tabs.setAttribute("Type", (*UsedStyles[actSt].TabValues.at(a)).tabType);
					tabs.setAttribute("Pos", (*UsedStyles[actSt].TabValues.at(a)).tabPosition);
					QString tabCh = "";
					if (!(*UsedStyles[actSt].TabValues.at(a)).tabFillChar.isNull())
						tabCh = QString((*UsedStyles[actSt].TabValues.at(a)).tabFillChar);
					tabs.setAttribute("Fill", tabCh);
					fo.appendChild(tabs);
				}
			}
			fo.setAttribute("FCOLOR",UsedStyles[actSt].FColor);
			fo.setAttribute("FSHADE",UsedStyles[actSt].FShade);
			fo.setAttribute("SCOLOR",UsedStyles[actSt].SColor);
			fo.setAttribute("SSHADE",UsedStyles[actSt].SShade);
			fo.setAttribute("BASE", static_cast<int>(UsedStyles[actSt].BaseAdj));
			fo.setAttribute("TXTSHX",UsedStyles[actSt].txtShadowX / 10.0);
			fo.setAttribute("TXTSHY",UsedStyles[actSt].txtShadowY / 10.0);
			fo.setAttribute("TXTOUT",UsedStyles[actSt].txtOutline / 10.0);
			fo.setAttribute("TXTULP",UsedStyles[actSt].txtUnderPos / 10.0);
			fo.setAttribute("TXTULW",UsedStyles[actSt].txtUnderWidth / 10.0);
			fo.setAttribute("TXTSTP",UsedStyles[actSt].txtStrikePos / 10.0);
			fo.setAttribute("TXTSTW",UsedStyles[actSt].txtStrikeWidth / 10.0);
			fo.setAttribute("SCALEH",UsedStyles[actSt].scaleH / 10.0);
			fo.setAttribute("SCALEV",UsedStyles[actSt].scaleV / 10.0);
			fo.setAttribute("BASEO",UsedStyles[actSt].baseOff / 10.0);
			fo.setAttribute("KERN",UsedStyles[actSt].kernVal / 10.0);
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
	for (uint co=0; co<doc->selection->count(selectionListNumber); ++co)
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
	for (uint co=0; co<doc->selection->count(selectionListNumber); ++co)
	{
		QString CurDirP = QDir::currentDirPath();
		QDir::setCurrent(QDir::homeDirPath());
		item = doc->Items->at(ELL[co]);
		QDomElement ob=docu.createElement("ITEM");
		if (item->textAlignment > 4)
			ob.setAttribute("ALIGN",UsedMapped2Saved[item->textAlignment]);
		else
			ob.setAttribute("ALIGN",item->textAlignment);
 		SetItemProps(&ob, item, false);
		ob.setAttribute("XPOS",item->xPos() - doc->currentPage->xOffset());
		ob.setAttribute("YPOS",item->yPos() - doc->currentPage->yOffset());
		ob.setAttribute("BOOKMARK", item->isBookmark ? 1 : 0);
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
		for(uint k=0;k<item->itemText.count();++k)
		{
			ScText * itemTextAtK = item->itemText.at(k);
			QDomElement it=docu.createElement("ITEXT");
			ts = itemTextAtK->csize / 10.0;
			tf = itemTextAtK->cfont->scName();
			tc = itemTextAtK->ccolor;
			te = itemTextAtK->cextra;
			tsh = itemTextAtK->cshade;
			tst = itemTextAtK->cstyle & 2047;
			if (itemTextAtK->cab > 4)
				tsb = UsedMapped2Saved[itemTextAtK->cab];
			else
				tsb = itemTextAtK->cab;
			tcs = itemTextAtK->cstroke;
			tshs = itemTextAtK->cshade2;
			tsc = itemTextAtK->cscale / 10.0;
			tscv = itemTextAtK->cscalev / 10.0;
			tb = itemTextAtK->cbase / 10.0;
			tsx = itemTextAtK->cshadowx / 10.0;
			tsy = itemTextAtK->cshadowy / 10.0;
			tout = itemTextAtK->coutline / 10.0;
			tulp = itemTextAtK->cunderpos / 10.0;
			tulw = itemTextAtK->cunderwidth / 10.0;
			tstp = itemTextAtK->cstrikepos / 10.0;
			tstw = itemTextAtK->cstrikewidth / 10.0;
			if (itemTextAtK->ch == QChar(13))
				text = QChar(5);
			else if (itemTextAtK->ch == QChar(9))
				text = QChar(4);
			else
				text = itemTextAtK->ch;
			++k;
			if (k == item->itemText.count())
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
			itemTextAtK = item->itemText.at(k);
			ts2 = itemTextAtK->csize / 10.0;
			tf2 = itemTextAtK->cfont->scName();
			tc2 = itemTextAtK->ccolor;
			te2 = itemTextAtK->cextra;
			tsh2 = itemTextAtK->cshade;
			tst2 = itemTextAtK->cstyle & 2047;
			if (itemTextAtK->cab > 4)
				tsb2 = UsedMapped2Saved[itemTextAtK->cab];
			else
				tsb2 = itemTextAtK->cab;
			tcs2 = itemTextAtK->cstroke;
			tshs2 = itemTextAtK->cshade2;
			tsc2 = itemTextAtK->cscale / 10.0;
			tscv2 = itemTextAtK->cscalev / 10.0;
			tb2 = itemTextAtK->cbase / 10.0;
			tsx2 = itemTextAtK->cshadowx / 10.0;
			tsy2 = itemTextAtK->cshadowy / 10.0;
			tout2 = itemTextAtK->coutline / 10.0;
			tulp2 = itemTextAtK->cunderpos / 10.0;
			tulw2 = itemTextAtK->cunderwidth / 10.0;
			tstp2 = itemTextAtK->cstrikepos / 10.0;
			tstw2 = itemTextAtK->cstrikewidth / 10.0;
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
				if (itemTextAtK->ch == QChar(13))
					text += QChar(5);
				else if (itemTextAtK->ch == QChar(9))
					text += QChar(4);
				else
					text += itemTextAtK->ch;
				++k;
				if (k == item->itemText.count())
					break;
				itemTextAtK = item->itemText.at(k);
				ts2 = itemTextAtK->csize / 10.0;
				tf2 = itemTextAtK->cfont->scName();
				tc2 = itemTextAtK->ccolor;
				te2 = itemTextAtK->cextra;
				tsh2 = itemTextAtK->cshade;
				tst2 = itemTextAtK->cstyle & 2047;
				if (itemTextAtK->cab > 4)
					tsb2 = UsedMapped2Saved[itemTextAtK->cab];
				else
					tsb2 = itemTextAtK->cab;
				tcs2 = itemTextAtK->cstroke;
				tshs2 = itemTextAtK->cshade2;
				tsc2 = itemTextAtK->cscale / 10.0;
				tscv2 = itemTextAtK->cscalev / 10.0;
				tb2 = itemTextAtK->cbase / 10.0;
				tsx2 = itemTextAtK->cshadowx / 10.0;
				tsy2 = itemTextAtK->cshadowy / 10.0;
				tout2 = itemTextAtK->coutline / 10.0;
				tulp2 = itemTextAtK->cunderpos / 10.0;
				tulw2 = itemTextAtK->cunderwidth / 10.0;
				tstp2 = itemTextAtK->cstrikepos / 10.0;
				tstw2 = itemTextAtK->cstrikewidth / 10.0;
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
		ob.setAttribute("NUMTEXT",item->itemText.count());
		QString txnu = "";
		for(uint kt=0;kt<item->itemText.count();++kt)
			txnu += tmp.setNum(item->itemText.at(kt)->xp) + " " + tmpy.setNum(item->itemText.at(kt)->yp) + " ";
		ob.setAttribute("TEXTCOOR", txnu);
		ob.setAttribute("BACKITEM", -1);
		ob.setAttribute("BACKPAGE", -1);
		ob.setAttribute("NEXTITEM", -1);
		ob.setAttribute("NEXTPAGE", -1);
		elem.appendChild(ob);
	}
	return docu.toString().utf8();
}

void ScriXmlDoc::WritePages(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, bool master)
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
		pg.setAttribute("NAM",page->PageNam);
		pg.setAttribute("MNAM",page->MPageNam);
		pg.setAttribute("Size", page->PageSize);
		pg.setAttribute("Orientation", page->PageOri);
		pg.setAttribute("LEFT", page->LeftPg);
		pg.setAttribute("NumVGuides", static_cast<int>(page->YGuides.count()));
		QString Vgui = "";
		for (uint vgu = 0; vgu < page->YGuides.count(); ++vgu)
			Vgui += tmp.setNum(page->YGuides[vgu]) + " ";
		pg.setAttribute("VerticalGuides", Vgui);
		pg.setAttribute("NumHGuides", static_cast<int>(page->XGuides.count()));
		QString Hgui = "";
		for (uint hgu = 0; hgu < page->XGuides.count(); ++hgu)
			Hgui += tmp.setNum(page->XGuides[hgu]) + " ";
		pg.setAttribute("HorizontalGuides", Hgui);
		dc->appendChild(pg);
	}
}

void ScriXmlDoc::WriteObjects(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, int master)
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
		}
		SetItemProps(&ob, item, true);
		ob.setAttribute("OnMasterPage", item->OnMasterPage);
		ob.setAttribute("ImageClip", item->pixm.imgInfo.usedPath);
		ob.setAttribute("ImageRes", item->pixm.imgInfo.lowResType);
		ob.setAttribute("isInline", static_cast<int>(item->isEmbedded));
		ob.setAttribute("gXpos", item->gXpos);
		ob.setAttribute("gYpos", item->gYpos);
		ob.setAttribute("gWidth", item->gWidth);
		ob.setAttribute("gHeight", item->gHeight);
		if (item->GrType != 0)
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
		ob.setAttribute("ALIGN",item->textAlignment);
		ob.setAttribute("BOOKMARK", item->isBookmark ? 1 : 0);
		ob.setAttribute("BookNr", item->BMnr);
		for(uint k=0;k<item->itemText.count();++k)
		{
			ScText *itemTextAtK = item->itemText.at(k);
			QDomElement it=docu->createElement("ITEXT");
			ts = itemTextAtK->csize / 10.0;
			tf = itemTextAtK->cfont->scName();
			tc = itemTextAtK->ccolor;
			te = itemTextAtK->cextra;
			tsh = itemTextAtK->cshade;
			tst = itemTextAtK->cstyle & 2047;
			tsb = itemTextAtK->cab;
			tcs = itemTextAtK->cstroke;
			tshs = itemTextAtK->cshade2;
			tsc = itemTextAtK->cscale / 10.0;
			tscv = itemTextAtK->cscalev / 10.0;
			tb = itemTextAtK->cbase / 10.0;
			tsx = itemTextAtK->cshadowx / 10.0;
			tsy = itemTextAtK->cshadowy / 10.0;
			tout = itemTextAtK->coutline / 10.0;
			tulp = itemTextAtK->cunderpos / 10.0;
			tulw = itemTextAtK->cunderwidth / 10.0;
			tstp = itemTextAtK->cstrikepos / 10.0;
			tstw = itemTextAtK->cstrikewidth / 10.0;
			if ((itemTextAtK->ch == QChar(25)) && (itemTextAtK->cembedded != 0))
				tobj = itemTextAtK->cembedded->ItemNr;
			else
				tobj = -1;
			if (itemTextAtK->ch == QChar(13))
				text = QChar(5);
			else if (itemTextAtK->ch == QChar(9))
				text = QChar(4);
			else
				text = itemTextAtK->ch;
			++k;
			if (k == item->itemText.count())
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
			itemTextAtK = item->itemText.at(k);
			ts2 = itemTextAtK->csize / 10.0;
			tf2 = itemTextAtK->cfont->scName();
			tc2 = itemTextAtK->ccolor;
			te2 = itemTextAtK->cextra;
			tsh2 = itemTextAtK->cshade;
			tst2 = itemTextAtK->cstyle & 2047;
			tsb2 = itemTextAtK->cab;
			tcs2 = itemTextAtK->cstroke;
			tshs2 = itemTextAtK->cshade2;
			tsc2 = itemTextAtK->cscale / 10.0;
			tscv2 = itemTextAtK->cscalev / 10.0;
			tb2 = itemTextAtK->cbase / 10.0;
			tsx2 = itemTextAtK->cshadowx / 10.0;
			tsy2 = itemTextAtK->cshadowy / 10.0;
			tout2 = itemTextAtK->coutline / 10.0;
			tulp2 = itemTextAtK->cunderpos / 10.0;
			tulw2 = itemTextAtK->cunderwidth / 10.0;
			tstp2 = itemTextAtK->cstrikepos / 10.0;
			tstw2 = itemTextAtK->cstrikewidth / 10.0;
			if ((itemTextAtK->ch == QChar(25)) && (itemTextAtK->cembedded != 0))
				tobj2 = itemTextAtK->cembedded->ItemNr;
			else
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
				if (itemTextAtK->ch == QChar(13))
					text += QChar(5);
				else if (itemTextAtK->ch == QChar(9))
					text += QChar(4);
				else
					text += itemTextAtK->ch;
				++k;
				if (k == item->itemText.count())
					break;
				itemTextAtK = item->itemText.at(k);
				ts2 = itemTextAtK->csize / 10.0;
				tf2 = itemTextAtK->cfont->scName();
				tc2 = itemTextAtK->ccolor;
				te2 = itemTextAtK->cextra;
				tsh2 = itemTextAtK->cshade;
				tst2 = itemTextAtK->cstyle & 2047;
				tsb2 = itemTextAtK->cab;
				tcs2 = itemTextAtK->cstroke;
				tshs2 = itemTextAtK->cshade2;
				tsc2 = itemTextAtK->cscale / 10.0;
				tscv2 = itemTextAtK->cscalev / 10.0;
				tb2 = itemTextAtK->cbase / 10.0;
				tsx2 = itemTextAtK->cshadowx / 10.0;
				tsy2 = itemTextAtK->cshadowy / 10.0;
				tout2 = itemTextAtK->coutline / 10.0;
				tulp2 = itemTextAtK->cunderpos / 10.0;
				tulw2 = itemTextAtK->cunderwidth / 10.0;
				tstp2 = itemTextAtK->cstrikepos / 10.0;
				tstw2 = itemTextAtK->cstrikewidth / 10.0;
				if ((itemTextAtK->ch == QChar(25)) && (itemTextAtK->cembedded != 0))
					tobj2 = itemTextAtK->cembedded->ItemNr;
				else
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

bool ScriXmlDoc::WriteDoc(QString fileName, ScribusDoc *doc, QProgressBar *dia2)
{
	QString text, tf, tf2, tc, tc2;
	QDomDocument docu("scribus");
	QString st="<SCRIBUSUTF8NEW></SCRIBUSUTF8NEW>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	elem.setAttribute("Version", QString(VERSION));
	QDomElement dc=docu.createElement("DOCUMENT");
	dc.setAttribute("ANZPAGES",doc->DocPages.count());
	dc.setAttribute("PAGEWIDTH",doc->pageWidth);
	dc.setAttribute("PAGEHEIGHT",doc->pageHeight);
	dc.setAttribute("BORDERLEFT",doc->pageMargins.Left);
	dc.setAttribute("BORDERRIGHT",doc->pageMargins.Right);
	dc.setAttribute("BORDERTOP",doc->pageMargins.Top);
	dc.setAttribute("BORDERBOTTOM",doc->pageMargins.Bottom);
	dc.setAttribute("ORIENTATION",doc->PageOri);
	dc.setAttribute("PAGESIZE",doc->PageSize);
	dc.setAttribute("FIRSTNUM",doc->FirstPnum);
	dc.setAttribute("BOOK", doc->currentPageLayout);
	if(doc->usesAutomaticTextFrames())
		dc.setAttribute("AUTOTEXT",1);
	dc.setAttribute("AUTOSPALTEN",doc->PageSp);
	dc.setAttribute("ABSTSPALTEN",doc->PageSpa);
	dc.setAttribute("UNITS",doc->unitIndex());
	dc.setAttribute("DFONT",doc->toolSettings.defFont);
	dc.setAttribute("DSIZE",doc->toolSettings.defSize / 10.0);
	dc.setAttribute("DCOL",doc->toolSettings.dCols);
	dc.setAttribute("DGAP",doc->toolSettings.dGap);
	dc.setAttribute("TabFill",doc->toolSettings.tabFillChar);
	dc.setAttribute("TabWidth",doc->toolSettings.dTabWidth);
	dc.setAttribute("AUTHOR",doc->documentInfo.getAuthor());
	dc.setAttribute("COMMENTS",doc->documentInfo.getComments());
	dc.setAttribute("KEYWORDS",doc->documentInfo.getKeywords());
	dc.setAttribute("PUBLISHER",doc->documentInfo.getPublisher());
	dc.setAttribute("DOCDATE",doc->documentInfo.getDate());
	dc.setAttribute("DOCTYPE",doc->documentInfo.getType());
	dc.setAttribute("DOCFORMAT",doc->documentInfo.getFormat());
	dc.setAttribute("DOCIDENT",doc->documentInfo.getIdent());
	dc.setAttribute("DOCSOURCE",doc->documentInfo.getSource());
	dc.setAttribute("DOCLANGINFO",doc->documentInfo.getLangInfo());
	dc.setAttribute("DOCRELATION",doc->documentInfo.getRelation());
	dc.setAttribute("DOCCOVER",doc->documentInfo.getCover());
	dc.setAttribute("DOCRIGHTS",doc->documentInfo.getRights());
	dc.setAttribute("DOCCONTRIB",doc->documentInfo.getContrib());
	dc.setAttribute("TITLE",doc->documentInfo.getTitle());
	dc.setAttribute("VHOCH",doc->typographicSettings.valueSuperScript);
	dc.setAttribute("VHOCHSC",doc->typographicSettings.scalingSuperScript);
	dc.setAttribute("VTIEF",doc->typographicSettings.valueSubScript);
	dc.setAttribute("VTIEFSC",doc->typographicSettings.scalingSubScript);
	dc.setAttribute("VKAPIT",doc->typographicSettings.valueSmallCaps);
	dc.setAttribute("BASEGRID",doc->typographicSettings.valueBaseGrid);
	dc.setAttribute("BASEO", doc->typographicSettings.offsetBaseGrid);
	dc.setAttribute("AUTOL", doc->typographicSettings.autoLineSpacing);
	dc.setAttribute("UnderlinePos", doc->typographicSettings.valueUnderlinePos);
	dc.setAttribute("UnderlineWidth", doc->typographicSettings.valueUnderlineWidth);
	dc.setAttribute("StrikeThruPos", doc->typographicSettings.valueStrikeThruPos);
	dc.setAttribute("StrikeThruWidth", doc->typographicSettings.valueStrikeThruWidth);
	dc.setAttribute("GROUPC",doc->GroupCounter);
	dc.setAttribute("HCMS", static_cast<int>(doc->HasCMS));
	dc.setAttribute("DPSo", static_cast<int>(doc->CMSSettings.SoftProofOn));
	dc.setAttribute("DPuse", static_cast<int>(doc->CMSSettings.CMSinUse));
	dc.setAttribute("DPgam", static_cast<int>(doc->CMSSettings.GamutCheck));
	dc.setAttribute("DPbla", static_cast<int>(doc->CMSSettings.BlackPoint));
	dc.setAttribute("DPMo",doc->CMSSettings.DefaultMonitorProfile);
	dc.setAttribute("DPPr",doc->CMSSettings.DefaultPrinterProfile);
	dc.setAttribute("DPIn",doc->CMSSettings.DefaultImageRGBProfile);
	dc.setAttribute("DPInCMYK",doc->CMSSettings.DefaultImageCMYKProfile);
	dc.setAttribute("DPIn2",doc->CMSSettings.DefaultSolidColorProfile);
	dc.setAttribute("DIPr",doc->CMSSettings.DefaultIntentPrinter);
	dc.setAttribute("DIMo",doc->CMSSettings.DefaultIntentMonitor);
	dc.setAttribute("DIMo2",doc->CMSSettings.DefaultIntentImages);
	dc.setAttribute("ALAYER", doc->activeLayer());
	dc.setAttribute("LANGUAGE", doc->Language);
	dc.setAttribute("MINWORDLEN", doc->MinWordLen);
	dc.setAttribute("HYCOUNT", doc->HyCount);
	dc.setAttribute("AUTOMATIC", static_cast<int>(doc->Automatic));
	dc.setAttribute("AUTOCHECK", static_cast<int>(doc->AutoCheck));
	dc.setAttribute("GUIDELOCK", static_cast<int>(doc->GuideLock));
	dc.setAttribute("SnapToGuides", static_cast<int>(doc->SnapGuides));
	dc.setAttribute("SnapToGrid", static_cast<int>(doc->useRaster));
	dc.setAttribute("MINGRID", doc->guidesSettings.minorGrid);
	dc.setAttribute("MAJGRID", doc->guidesSettings.majorGrid);
	dc.setAttribute("SHOWGRID", static_cast<int>(doc->guidesSettings.gridShown));
	dc.setAttribute("SHOWGUIDES", static_cast<int>(doc->guidesSettings.guidesShown));
	dc.setAttribute("SHOWFRAME", static_cast<int>(doc->guidesSettings.framesShown));
	dc.setAttribute("SHOWMARGIN", static_cast<int>(doc->guidesSettings.marginsShown));
	dc.setAttribute("SHOWBASE", static_cast<int>(doc->guidesSettings.baseShown));
	dc.setAttribute("SHOWPICT", static_cast<int>(doc->guidesSettings.showPic));
	dc.setAttribute("SHOWControl", static_cast<int>(doc->guidesSettings.showControls));
	dc.setAttribute("SHOWLINK", static_cast<int>(doc->guidesSettings.linkShown));
	dc.setAttribute("rulerMode", static_cast<int>(doc->guidesSettings.rulerMode));
	dc.setAttribute("rulerXoffset", doc->rulerXoffset);
	dc.setAttribute("rulerYoffset", doc->rulerYoffset);
	dc.setAttribute("GuideRad", doc->guidesSettings.guideRad);
	dc.setAttribute("GRAB",doc->guidesSettings.grabRad);
	dc.setAttribute("POLYC", doc->toolSettings.polyC);
	dc.setAttribute("POLYF", doc->toolSettings.polyF);
	dc.setAttribute("POLYR", doc->toolSettings.polyR);
	dc.setAttribute("POLYFD", doc->toolSettings.polyFd);
	dc.setAttribute("POLYS", static_cast<int>(doc->toolSettings.polyS));
	dc.setAttribute("AutoSave", static_cast<int>(doc->AutoSave));
	dc.setAttribute("AutoSaveTime", doc->AutoSaveTime);
	dc.setAttribute("ScratchBottom", doc->ScratchBottom);
	dc.setAttribute("ScratchLeft", doc->ScratchLeft);
	dc.setAttribute("ScratchRight", doc->ScratchRight);
	dc.setAttribute("ScratchTop", doc->ScratchTop);
	dc.setAttribute("StartArrow", doc->toolSettings.dStartArrow);
	dc.setAttribute("EndArrow", doc->toolSettings.dEndArrow);
	dc.setAttribute("PEN",doc->toolSettings.dPen);
	dc.setAttribute("BRUSH",doc->toolSettings.dBrush);
	dc.setAttribute("PENLINE",doc->toolSettings.dPenLine);
	dc.setAttribute("PENTEXT",doc->toolSettings.dPenText);
	dc.setAttribute("StrokeText",doc->toolSettings.dStrokeText);
	dc.setAttribute("TextBackGround", doc->toolSettings.dTextBackGround);
	dc.setAttribute("TextLineColor", doc->toolSettings.dTextLineColor);
	dc.setAttribute("TextBackGroundShade", doc->toolSettings.dTextBackGroundShade);
	dc.setAttribute("TextLineShade", doc->toolSettings.dTextLineShade);
	dc.setAttribute("TextPenShade", doc->toolSettings.dTextPenShade);
	dc.setAttribute("TextStrokeShade", doc->toolSettings.dTextStrokeShade);
	dc.setAttribute("STIL",doc->toolSettings.dLineArt);
	dc.setAttribute("STILLINE",doc->toolSettings.dLstyleLine);
	dc.setAttribute("WIDTH",doc->toolSettings.dWidth);
	dc.setAttribute("WIDTHLINE",doc->toolSettings.dWidthLine);
	dc.setAttribute("PENSHADE",doc->toolSettings.dShade2);
	dc.setAttribute("LINESHADE",doc->toolSettings.dShadeLine);
	dc.setAttribute("BRUSHSHADE",doc->toolSettings.dShade);
	dc.setAttribute("MAGMIN",doc->toolSettings.magMin);
	dc.setAttribute("MAGMAX",doc->toolSettings.magMax);
	dc.setAttribute("MAGSTEP",doc->toolSettings.magStep);
	dc.setAttribute("CPICT",doc->toolSettings.dBrushPict);
	dc.setAttribute("PICTSHADE",doc->toolSettings.shadePict);
	dc.setAttribute("PICTSCX",doc->toolSettings.scaleX);
	dc.setAttribute("PICTSCY",doc->toolSettings.scaleY);
	dc.setAttribute("PSCALE", static_cast<int>(doc->toolSettings.scaleType));
	dc.setAttribute("PASPECT", static_cast<int>(doc->toolSettings.aspectRatio));
	dc.setAttribute("EmbeddedPath", static_cast<int>(doc->toolSettings.useEmbeddedPath));
	dc.setAttribute("HalfRes", doc->toolSettings.lowResType);
	dc.setAttribute("MINORC",doc->guidesSettings.minorColor.name());
	dc.setAttribute("MAJORC",doc->guidesSettings.majorColor.name());
	dc.setAttribute("GuideC", doc->guidesSettings.guideColor.name());
	dc.setAttribute("BaseC", doc->guidesSettings.baseColor.name());
	dc.setAttribute("GuideZ", doc->guidesSettings.guideRad);
	dc.setAttribute("BACKG", static_cast<int>(doc->guidesSettings.before));
	dc.setAttribute("PAGEC",doc->papColor.name());
	dc.setAttribute("MARGC",doc->guidesSettings.margColor.name());
	dc.setAttribute("RANDF", static_cast<int>(doc->marginColored));
	dc.setAttribute("currentProfile", doc->curCheckProfile);
	CheckerPrefsList::Iterator itcp;
	CheckerPrefsList::Iterator itcpend=doc->checkerProfiles.end();
	for (itcp = doc->checkerProfiles.begin(); itcp != itcpend; ++itcp)
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
		dc79a.setAttribute("checkAnnotations", static_cast<int>(itcp.data().checkAnnotations));
		dc79a.setAttribute("checkRasterPDF", static_cast<int>(itcp.data().checkRasterPDF));
		dc.appendChild(dc79a);
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
		dc.appendChild(MuL);
	}
	QValueList<ArrowDesc>::Iterator itar;
	for (itar = doc->arrowStyles.begin(); itar != doc->arrowStyles.end(); ++itar)
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
	for (itja = doc->JavaScripts.begin(); itja != doc->JavaScripts.end(); ++itja)
	{
		QDomElement jav=docu.createElement("JAVA");
		jav.setAttribute("NAME",itja.key());
		jav.setAttribute("SCRIPT",itja.data());
		dc.appendChild(jav);
	}
	QValueList<ScribusDoc::BookMa>::Iterator itbm;
	for (itbm = doc->BookMarks.begin(); itbm != doc->BookMarks.end(); ++itbm)
	{
		QDomElement fn=docu.createElement("Bookmark");
		fn.setAttribute("Title",(*itbm).Title);
		fn.setAttribute("Text",(*itbm).Text);
		fn.setAttribute("Aktion",(*itbm).Aktion);
		fn.setAttribute("ItemNr", (*itbm).ItemNr);
		fn.setAttribute("Seite", (*itbm).Seite);
		fn.setAttribute("Element", (*itbm).Element);
		fn.setAttribute("First", (*itbm).First);
		fn.setAttribute("Last", (*itbm).Last);
		fn.setAttribute("Prev", (*itbm).Prev);
		fn.setAttribute("Next", (*itbm).Next);
		fn.setAttribute("Parent", (*itbm).Parent);
		dc.appendChild(fn);
	}
	ColorList::Iterator itc;
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		QDomElement co=docu.createElement("COLOR");
		co.setAttribute("NAME",itc.key());
		co.setAttribute("RGB",doc->PageColors[itc.key()].getRGBColor().name());
		co.setAttribute("CMYK",doc->PageColors[itc.key()].name());
		co.setAttribute("Spot",static_cast<int>(doc->PageColors[itc.key()].isSpotColor()));
		co.setAttribute("Register",static_cast<int>(doc->PageColors[itc.key()].isRegistrationColor()));
		dc.appendChild(co);
	}
	if (doc->docParagraphStyles.count() > 5)
	{
		for (uint ff = 5; ff < doc->docParagraphStyles.count(); ++ff)
		{
			QDomElement fo=docu.createElement("STYLE");
			fo.setAttribute("NAME",doc->docParagraphStyles[ff].Vname);
			fo.setAttribute("ALIGN",doc->docParagraphStyles[ff].textAlignment);
			fo.setAttribute("LINESPMode",doc->docParagraphStyles[ff].LineSpaMode);
			fo.setAttribute("LINESP",doc->docParagraphStyles[ff].LineSpa);
			fo.setAttribute("INDENT",doc->docParagraphStyles[ff].Indent);
			fo.setAttribute("FIRST",doc->docParagraphStyles[ff].First);
			fo.setAttribute("VOR",doc->docParagraphStyles[ff].gapBefore);
			fo.setAttribute("NACH",doc->docParagraphStyles[ff].gapAfter);
			fo.setAttribute("FONT",doc->docParagraphStyles[ff].Font);
			fo.setAttribute("FONTSIZE",doc->docParagraphStyles[ff].FontSize / 10.0);
			fo.setAttribute("DROP", static_cast<int>(doc->docParagraphStyles[ff].Drop));
			fo.setAttribute("DROPLIN", doc->docParagraphStyles[ff].DropLin);
			fo.setAttribute("DROPDIST", doc->docParagraphStyles[ff].DropDist);
			fo.setAttribute("EFFECT", doc->docParagraphStyles[ff].FontEffect);
			if (doc->docParagraphStyles[ff].TabValues.count() != 0)
			{
				for (uint a = 0; a < doc->docParagraphStyles[ff].TabValues.count(); ++a)
				{
					QDomElement tabs = docu.createElement("Tabs");
					tabs.setAttribute("Type", (*doc->docParagraphStyles[ff].TabValues.at(a)).tabType);
					tabs.setAttribute("Pos", (*doc->docParagraphStyles[ff].TabValues.at(a)).tabPosition);
					QString tabCh = "";
					if (!(*doc->docParagraphStyles[ff].TabValues.at(a)).tabFillChar.isNull())
						tabCh = QString((*doc->docParagraphStyles[ff].TabValues.at(a)).tabFillChar);
					tabs.setAttribute("Fill", tabCh);
					fo.appendChild(tabs);
				}
			}
			fo.setAttribute("FCOLOR",doc->docParagraphStyles[ff].FColor);
			fo.setAttribute("FSHADE",doc->docParagraphStyles[ff].FShade);
			fo.setAttribute("SCOLOR",doc->docParagraphStyles[ff].SColor);
			fo.setAttribute("SSHADE",doc->docParagraphStyles[ff].SShade);
			fo.setAttribute("BASE", static_cast<int>(doc->docParagraphStyles[ff].BaseAdj));
			fo.setAttribute("TXTSHX",doc->docParagraphStyles[ff].txtShadowX / 10.0);
			fo.setAttribute("TXTSHY",doc->docParagraphStyles[ff].txtShadowY / 10.0);
			fo.setAttribute("TXTOUT",doc->docParagraphStyles[ff].txtOutline / 10.0);
			fo.setAttribute("TXTULP",doc->docParagraphStyles[ff].txtUnderPos / 10.0);
			fo.setAttribute("TXTULW",doc->docParagraphStyles[ff].txtUnderWidth / 10.0);
			fo.setAttribute("TXTSTP",doc->docParagraphStyles[ff].txtStrikePos / 10.0);
			fo.setAttribute("TXTSTW",doc->docParagraphStyles[ff].txtStrikeWidth / 10.0);
			fo.setAttribute("SCALEH",doc->docParagraphStyles[ff].scaleH / 10.0);
			fo.setAttribute("SCALEV",doc->docParagraphStyles[ff].scaleV / 10.0);
			fo.setAttribute("BASEO",doc->docParagraphStyles[ff].baseOff / 10.0);
			fo.setAttribute("KERN",doc->docParagraphStyles[ff].kernVal / 10.0);
			dc.appendChild(fo);
		}
	}
	uint layerCount=doc->layerCount();
	for (uint lay = 0; lay < layerCount; ++lay)
	{
		QDomElement la = docu.createElement("LAYERS");
		la.setAttribute("NUMMER",doc->Layers[lay].LNr);
		la.setAttribute("LEVEL",doc->Layers[lay].Level);
		la.setAttribute("NAME",doc->Layers[lay].Name);
		la.setAttribute("SICHTBAR", static_cast<int>(doc->Layers[lay].isViewable));
		la.setAttribute("DRUCKEN", static_cast<int>(doc->Layers[lay].isPrintable));
		dc.appendChild(la);
	}
	QDomElement pdf = docu.createElement("PDF");
	pdf.setAttribute("Thumbnails", static_cast<int>(doc->PDF_Options.Thumbnails));
	pdf.setAttribute("Articles", static_cast<int>(doc->PDF_Options.Articles));
	pdf.setAttribute("Bookmarks", static_cast<int>(doc->PDF_Options.Bookmarks));
	pdf.setAttribute("Compress", static_cast<int>(doc->PDF_Options.Compress));
	pdf.setAttribute("CMethod", doc->PDF_Options.CompressMethod);
	pdf.setAttribute("Quality", doc->PDF_Options.Quality);
	pdf.setAttribute("MirrorH", static_cast<int>(doc->PDF_Options.MirrorH));
	pdf.setAttribute("MirrorV", static_cast<int>(doc->PDF_Options.MirrorV));
	pdf.setAttribute("RotateDeg", static_cast<int>(doc->PDF_Options.RotateDeg));
	pdf.setAttribute("PresentMode", static_cast<int>(doc->PDF_Options.PresentMode));
	pdf.setAttribute("RecalcPic", static_cast<int>(doc->PDF_Options.RecalcPic));
	pdf.setAttribute("Grayscale", static_cast<int>(doc->PDF_Options.isGrayscale));
	pdf.setAttribute("RGBMode", static_cast<int>(doc->PDF_Options.UseRGB));
	pdf.setAttribute("UseProfiles", static_cast<int>(doc->PDF_Options.UseProfiles));
	pdf.setAttribute("UseProfiles2", static_cast<int>(doc->PDF_Options.UseProfiles2));
	pdf.setAttribute("Binding", doc->PDF_Options.Binding);
	pdf.setAttribute("PicRes", doc->PDF_Options.PicRes);
	pdf.setAttribute("Resolution", doc->PDF_Options.Resolution);
	pdf.setAttribute("Version", doc->PDF_Options.Version);
	pdf.setAttribute("Intent", doc->PDF_Options.Intent);
	pdf.setAttribute("Intent2", doc->PDF_Options.Intent2);
	pdf.setAttribute("SolidP", doc->PDF_Options.SolidProf);
	pdf.setAttribute("ImageP", doc->PDF_Options.ImageProf);
	pdf.setAttribute("PrintP", doc->PDF_Options.PrintProf);
	pdf.setAttribute("InfoString", doc->PDF_Options.Info);
	pdf.setAttribute("BTop", doc->PDF_Options.BleedTop);
	pdf.setAttribute("BLeft", doc->PDF_Options.BleedLeft);
	pdf.setAttribute("BRight", doc->PDF_Options.BleedRight);
	pdf.setAttribute("BBottom", doc->PDF_Options.BleedBottom);
	pdf.setAttribute("ImagePr", static_cast<int>(doc->PDF_Options.EmbeddedI));
	pdf.setAttribute("PassOwner", doc->PDF_Options.PassOwner);
	pdf.setAttribute("PassUser", doc->PDF_Options.PassUser);
	pdf.setAttribute("Permissions", doc->PDF_Options.Permissions);
	pdf.setAttribute("Encrypt", static_cast<int>(doc->PDF_Options.Encrypt));
	pdf.setAttribute("UseLayers", static_cast<int>(doc->PDF_Options.useLayers));
	pdf.setAttribute("UseLpi", static_cast<int>(doc->PDF_Options.UseLPI));
	pdf.setAttribute("UseSpotColors", static_cast<int>(doc->PDF_Options.UseSpotColors));
	pdf.setAttribute("doMultiFile", static_cast<int>(doc->PDF_Options.doMultiFile));
	for (uint pdoF = 0; pdoF < doc->PDF_Options.EmbedList.count(); ++pdoF)
	{
		QDomElement pdf2 = docu.createElement("Fonts");
		pdf2.setAttribute("Name", doc->PDF_Options.EmbedList[pdoF]);
		pdf.appendChild(pdf2);
	}
	for (uint pdoS = 0; pdoS < doc->PDF_Options.SubsetList.count(); ++pdoS)
	{
		QDomElement pdf4 = docu.createElement("Subset");
		pdf4.setAttribute("Name", doc->PDF_Options.SubsetList[pdoS]);
		pdf.appendChild(pdf4);
	}
	for (uint pdoE = 0; pdoE < doc->PDF_Options.PresentVals.count(); ++pdoE)
	{
		QDomElement pdf3 = docu.createElement("Effekte");
		pdf3.setAttribute("pageEffectDuration", doc->PDF_Options.PresentVals[pdoE].pageEffectDuration);
		pdf3.setAttribute("pageViewDuration", doc->PDF_Options.PresentVals[pdoE].pageViewDuration);
		pdf3.setAttribute("effectType", doc->PDF_Options.PresentVals[pdoE].effectType);
		pdf3.setAttribute("Dm", doc->PDF_Options.PresentVals[pdoE].Dm);
		pdf3.setAttribute("M", doc->PDF_Options.PresentVals[pdoE].M);
		pdf3.setAttribute("Di", doc->PDF_Options.PresentVals[pdoE].Di);
		pdf.appendChild(pdf3);
	}
	QMap<QString,LPIData>::Iterator itlp;
	for (itlp = doc->PDF_Options.LPISettings.begin(); itlp != doc->PDF_Options.LPISettings.end(); ++itlp)
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
	for(ObjAttrVector::Iterator objAttrIt = doc->docItemAttributes.begin() ; objAttrIt != doc->docItemAttributes.end(); ++objAttrIt )
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
	for(ToCSetupVector::Iterator tocSetupIt = doc->docToCSetups.begin() ; tocSetupIt != doc->docToCSetups.end(); ++tocSetupIt )
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
	for(DocumentSectionMap::Iterator it = doc->sections.begin() ; it != doc->sections.end(); ++it )
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
	for(itpgset = doc->pageSets.begin(); itpgset != doc->pageSets.end(); ++itpgset )
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
	if (dia2 != 0)
	{
		dia2->setTotalSteps(doc->DocPages.count()+doc->MasterPages.count()+doc->DocItems.count()+doc->MasterItems.count()+doc->FrameItems.count());
		dia2->setProgress(0);
	}
	WritePages(doc, &docu, &dc, dia2, 0, true);
	WritePages(doc, &docu, &dc, dia2, doc->MasterPages.count(), false);
	WriteObjects(doc, &docu, &dc, dia2, doc->MasterPages.count()+doc->DocPages.count(), 2);
	WriteObjects(doc, &docu, &dc, dia2, doc->MasterPages.count()+doc->DocPages.count()+doc->FrameItems.count(), 0);
	WriteObjects(doc, &docu, &dc, dia2, doc->MasterPages.count()+doc->DocPages.count()+doc->MasterItems.count()+doc->FrameItems.count(), 1);
	elem.appendChild(dc);
/**
 * changed to enable saving
 * of *.gz documents
 * 2.7.2002 C.Toepp
 * <c.toepp@gmx.de>
*/
 #ifdef HAVE_LIBZ
	if(fileName.right(2) == "gz")
	{
  // zipped saving
		gzFile gzDoc = gzopen(fileName.latin1(),"wb");
		if(gzDoc == NULL)
			return false;
		gzputs(gzDoc, docu.toString().utf8());
		gzclose(gzDoc);
	}
	else
	{
		QFile f(fileName);
		if(!f.open(IO_WriteOnly))
			return false;
		QTextStream s(&f);
		QString wr = docu.toString().utf8();
		s.writeRawBytes(wr, wr.length());
		f.close();
	}
#else
	QFile f(fileName);
	if(!f.open(IO_WriteOnly))
		return false;
	QTextStream s(&f);
	QString wr = docu.toString().utf8();
	s.writeRawBytes(wr, wr.length());
	f.close();
#endif
	return true;
}
