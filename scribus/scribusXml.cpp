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
#include <qstylefactory.h>
#include <qtl.h>
#include <qcursor.h>
#include <qregexp.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <cstdlib>
#include <cmath>
#include "missing.h"

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

using namespace std;

extern double QStodouble(QString in);
extern int QStoInt(QString in);
extern bool loadText(QString nam, QString *Buffer);
extern QString Path2Relative(QString Path);

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
	QString fText = ReadDatei(fileName);
	if ((fText == "") || (!fText.startsWith("<SCRIBUS")) || (fText.contains("<PAGE ", TRUE) == 0))
		return false;
	return true;
}

/**
 * added to support gz docs
 * 2.7.2002 C.Toepp
 * <c.toepp@gmx.de>
 */
QString ScriXmlDoc::ReadDatei(QString fileName)
{
	QString f = "";
#ifdef HAVE_LIBZ
	if(fileName.right(2) == "gz")
	{
		gzFile gzDoc;
		char buff[4097];
		int i;
		gzDoc = gzopen(fileName.latin1(),"rb");
		if(gzDoc == NULL)
			return "";
		while((i = gzread(gzDoc,&buff,4096)) > 0)
		{
			buff[i] = '\0';
			f.append(buff);
		}
		gzclose(gzDoc);
	}
	else
// a normal document
		loadText(fileName, &f);
#else
	loadText(fileName, &f);
#endif
	QString ff = "";
	if (f.startsWith("<SCRIBUSUTF8"))
		ff = QString::fromUtf8(f);
	else if (f.startsWith("<SCRIBUS"))
		ff = f;
	return ff;
/** end changes */
}

void ScriXmlDoc::GetItemProps(bool newVersion, QDomElement *obj, struct CLBuf *OB)
{
	QString tmp;
	int x, y;
	double xf, yf;
	OB->PType = QStoInt(obj->attribute("PTYPE"));
	OB->Width=QStodouble(obj->attribute("WIDTH"));
	OB->Height=QStodouble(obj->attribute("HEIGHT"));
	OB->RadRect = QStodouble(obj->attribute("RADRECT","0"));
	OB->ClipEdited = QStoInt(obj->attribute("CLIPEDIT", "0"));
	OB->FrameType = QStoInt(obj->attribute("FRTYPE", "0"));
	OB->Pwidth=QStodouble(obj->attribute("PWIDTH"));
	OB->Pcolor=obj->attribute("PCOLOR");
	if ((!newVersion) && (OB->PType == 4))
	{
		OB->TxtFill = obj->attribute("PCOLOR2");
		OB->Pcolor2 = "None";
	}
	else
	{
		OB->Pcolor2 = obj->attribute("PCOLOR2");
		OB->TxtFill = obj->attribute("TXTFILL", "Black");
	}
	OB->Shade=QStoInt(obj->attribute("SHADE"));
	OB->Shade2=QStoInt(obj->attribute("SHADE2"));
	OB->TxtStroke=obj->attribute("TXTSTROKE", "None");
	OB->ShTxtFill=QStoInt(obj->attribute("TXTFILLSH", "100"));
	OB->ShTxtStroke=QStoInt(obj->attribute("TXTSTRSH", "100"));
	OB->TxtScale=QStoInt(obj->attribute("TXTSCALE", "100"));
	OB->TxTStyle=QStoInt(obj->attribute("TXTSTYLE", "0"));
	OB->Cols = QStoInt(obj->attribute("COLUMNS","1"));
	OB->ColGap = QStodouble(obj->attribute("COLGAP","0.0"));
	OB->GrType = QStoInt(obj->attribute("GRTYP","0"));
	OB->fill_gradient.clearStops();
	if (OB->GrType != 0)
	{
		OB->GrStartX = QStodouble(obj->attribute("GRSTARTX","0.0"));
		OB->GrStartY = QStodouble(obj->attribute("GRSTARTY","0.0"));
		OB->GrEndX = QStodouble(obj->attribute("GRENDX","0.0"));
		OB->GrEndY = QStodouble(obj->attribute("GRENDY","0.0"));
		OB->GrColor = obj->attribute("GRCOLOR","");
		if (OB->GrColor != "")
		{
			OB->GrColor2 = obj->attribute("GRCOLOR2","");
			OB->GrShade = QStoInt(obj->attribute("GRSHADE","100"));
			OB->GrShade2 = QStoInt(obj->attribute("GRSHADE2","100"));
		}
	}
	OB->Rot=QStodouble(obj->attribute("ROT"));
	OB->PLineArt=Qt::PenStyle(QStoInt(obj->attribute("PLINEART")));
	OB->PLineEnd=Qt::PenCapStyle(QStoInt(obj->attribute("PLINEEND","0")));
	OB->PLineJoin=Qt::PenJoinStyle(QStoInt(obj->attribute("PLINEJOIN","0")));
	OB->LineSp=QStodouble(obj->attribute("LINESP"));
	OB->ExtraV=QStodouble(obj->attribute("EXTRAV","0"));
	OB->LocalScX=QStodouble(obj->attribute("LOCALSCX"));
	OB->LocalScY=QStodouble(obj->attribute("LOCALSCY"));
	OB->LocalX=QStodouble(obj->attribute("LOCALX"));
	OB->LocalY=QStodouble(obj->attribute("LOCALY"));
	OB->PicArt=QStoInt(obj->attribute("PICART"));
	OB->flippedH=QStoInt(obj->attribute("FLIPPEDH"));
	OB->flippedV=QStoInt(obj->attribute("FLIPPEDV"));
	OB->BBoxX=QStodouble(obj->attribute("BBOXX"));
	OB->BBoxH=QStodouble(obj->attribute("BBOXH"));
	OB->ScaleType = QStoInt(obj->attribute("SCALETYPE","1"));
	OB->AspectRatio = QStoInt(obj->attribute("RATIO","0"));
	OB->isPrintable=QStoInt(obj->attribute("PRINTABLE"));
	OB->isAnnotation=QStoInt(obj->attribute("ANNOTATION","0"));
	OB->AnType = QStoInt(obj->attribute("ANTYPE","0"));
	OB->AnAction = obj->attribute("ANACTION","");
	OB->An_E_act = obj->attribute("ANEACT","");
	OB->An_X_act = obj->attribute("ANXACT","");
	OB->An_D_act = obj->attribute("ANDACT","");
	OB->An_Fo_act = obj->attribute("ANFOACT","");
	OB->An_Bl_act = obj->attribute("ANBLACT","");
	OB->An_K_act = obj->attribute("ANKACT","");
	OB->An_F_act = obj->attribute("ANFACT","");
	OB->An_V_act = obj->attribute("ANVACT","");
	OB->An_C_act = obj->attribute("ANCACT","");
	OB->AnActType = QStoInt(obj->attribute("ANACTYP","0"));
	OB->An_Extern = obj->attribute("ANEXTERN","");
	if ((OB->An_Extern != "") && (OB->AnActType != 8))
	{
		QFileInfo efp(OB->An_Extern);
		OB->An_Extern = efp.absFilePath();
	}
	OB->AnZiel = QStoInt(obj->attribute("ANZIEL","0"));
	OB->AnName = obj->attribute("ANNAME","");
	OB->AnToolTip = obj->attribute("ANTOOLTIP","");
	OB->AnRollOver = obj->attribute("ANROLL","");
	OB->AnDown = obj->attribute("ANDOWN","");
	OB->AnBwid = QStoInt(obj->attribute("ANBWID","1"));
	OB->AnBsty = QStoInt(obj->attribute("ANBSTY","0"));
	OB->AnFeed = QStoInt(obj->attribute("ANFEED","1"));
	OB->AnFlag = QStoInt(obj->attribute("ANFLAG","0"));
	OB->AnFont = QStoInt(obj->attribute("ANFONT","4"));
	OB->AnFormat = QStoInt(obj->attribute("ANFORMAT","0"));
	OB->AnVis = QStoInt(obj->attribute("ANVIS","0"));
	OB->AnIsChk = static_cast<bool>(QStoInt(obj->attribute("ANCHK","0")));
	OB->AnAAact = static_cast<bool>(QStoInt(obj->attribute("ANAA","0")));
	OB->AnHTML = static_cast<bool>(QStoInt(obj->attribute("ANHTML","0")));
	OB->AnUseIcons = static_cast<bool>(QStoInt(obj->attribute("ANICON","0")));
	OB->AnChkStil = QStoInt(obj->attribute("ANCHKS","0"));
	OB->AnMaxChar = QStoInt(obj->attribute("ANMC","-1"));
	OB->AnBColor = obj->attribute("ANBCOL","None");
	OB->AnIPlace = QStoInt(obj->attribute("ANPLACE","1"));
	OB->AnScaleW = QStoInt(obj->attribute("ANSCALE","0"));
	if (QStoInt(obj->attribute("TRANSPARENT","0")) == 1)
		OB->Pcolor = "None";
	OB->Textflow=QStoInt(obj->attribute("TEXTFLOW"));
	OB->Textflow2 =QStoInt(obj->attribute("TEXTFLOW2","0"));
	OB->UseContour = QStoInt(obj->attribute("TEXTFLOW3","0"));
	OB->Extra=QStodouble(obj->attribute("EXTRA"));
	OB->TExtra=QStodouble(obj->attribute("TEXTRA", "1"));
	OB->BExtra=QStodouble(obj->attribute("BEXTRA", "1"));
	OB->RExtra=QStodouble(obj->attribute("REXTRA", "1"));
	OB->PoShow = QStoInt(obj->attribute("PTLSHOW","0"));
	OB->BaseOffs = QStodouble(obj->attribute("BASEOF","0"));
	OB->ISize = qRound(QStodouble(obj->attribute("ISIZE","12")) * 10);
	OB->Pfile=obj->attribute("PFILE");
	OB->Pfile2=obj->attribute("PFILE2","");
	OB->Pfile3=obj->attribute("PFILE3","");
	OB->IProfile=obj->attribute("PRFILE","");
	OB->EmProfile=obj->attribute("EPROF","");
	OB->IRender = QStoInt(obj->attribute("IRENDER","1"));
	OB->UseEmbedded = QStoInt(obj->attribute("EMBEDDED","1"));
	OB->Locked = static_cast<bool>(QStoInt(obj->attribute("LOCK","0")));
	OB->LockRes = static_cast<bool>(QStoInt(obj->attribute("LOCKR","0")));
	OB->Reverse = static_cast<bool>(QStoInt(obj->attribute("REVERS","0")));
	OB->InvPict = static_cast<bool>(QStoInt(obj->attribute("INVERS","0")));
	OB->isTableItem = static_cast<bool>(QStoInt(obj->attribute("isTableItem","0")));
	OB->TopLine = static_cast<bool>(QStoInt(obj->attribute("TopLine","0")));
	OB->LeftLine = static_cast<bool>(QStoInt(obj->attribute("LeftLine","0")));
	OB->RightLine = static_cast<bool>(QStoInt(obj->attribute("RightLine","0")));
	OB->BottomLine = static_cast<bool>(QStoInt(obj->attribute("BottomLine","0")));
	OB->TopLinkID =  QStoInt(obj->attribute("TopLINK","-1"));
	OB->LeftLinkID =  QStoInt(obj->attribute("LeftLINK","-1"));
	OB->RightLinkID =  QStoInt(obj->attribute("RightLINK","-1"));
	OB->BottomLinkID =  QStoInt(obj->attribute("BottomLINK","-1"));
	OB->Transparency = QStodouble(obj->attribute("TransValue","0.0"));
	if (obj->hasAttribute("TransValueS"))
		OB->TranspStroke = QStodouble(obj->attribute("TransValueS","0.0"));
	else
		OB->TranspStroke = OB->Transparency;
	tmp = "";
	if (obj->hasAttribute("NUMCLIP"))
	{
		OB->Clip.resize(obj->attribute("NUMCLIP").toUInt());
		tmp = obj->attribute("CLIPCOOR");
		QTextStream fc(&tmp, IO_ReadOnly);
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
		QTextStream fp(&tmp, IO_ReadOnly);
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
		QTextStream fp(&tmp, IO_ReadOnly);
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
	if ((obj->hasAttribute("NUMTAB")) && (QStoInt(obj->attribute("NUMTAB","0")) != 0))
	{
		tmp = obj->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		OB->TabValues.clear();
		for (int cxv = 0; cxv < QStoInt(obj->attribute("NUMTAB","0")); ++cxv)
		{
			tgv >> xf;
			OB->TabValues.append(xf);
		}
		tmp = "";
	}
	else
		OB->TabValues.clear();
	if ((obj->hasAttribute("NUMDASH")) && (QStoInt(obj->attribute("NUMDASH","0")) != 0))
	{
		tmp = obj->attribute("DASHS");
		QTextStream dgv(&tmp, IO_ReadOnly);
		OB->DashValues.clear();
		for (int cxv = 0; cxv < QStoInt(obj->attribute("NUMDASH","0")); ++cxv)
		{
			dgv >> xf;
			OB->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		OB->DashValues.clear();
	OB->DashOffset = QStodouble(obj->attribute("DASHOFF","0.0"));
}

void ScriXmlDoc::SetItemProps(QDomElement *ob, PageItem* item)
{
	double xf, yf;
	QString tmp, tmpy;
	ob->setAttribute("PTYPE",item->PType);
	ob->setAttribute("XPOS",item->Xpos);
	ob->setAttribute("YPOS",item->Ypos);
	ob->setAttribute("WIDTH",item->Width);
	ob->setAttribute("HEIGHT",item->Height);
	ob->setAttribute("RADRECT",item->RadRect);
	ob->setAttribute("FRTYPE", item->FrameType);
	ob->setAttribute("CLIPEDIT", item->ClipEdited ? 1 : 0);
	ob->setAttribute("PWIDTH",item->Pwidth);
	ob->setAttribute("PCOLOR",item->Pcolor);
	ob->setAttribute("PCOLOR2",item->Pcolor2);
	ob->setAttribute("TXTFILL",item->TxtFill);
	ob->setAttribute("TXTSTROKE",item->TxtStroke);
	ob->setAttribute("TXTSTRSH",item->ShTxtStroke);
	ob->setAttribute("TXTFILLSH",item->ShTxtFill);
	ob->setAttribute("TXTSCALE",item->TxtScale);
	ob->setAttribute("TXTSTYLE",item->TxTStyle);
	ob->setAttribute("COLUMNS", item->Cols);
	ob->setAttribute("COLGAP", item->ColGap);
	ob->setAttribute("NAMEDLST",item->NamedLStyle);
	ob->setAttribute("SHADE",item->Shade);
	ob->setAttribute("SHADE2",item->Shade2);
	ob->setAttribute("GRTYP",item->GrType);
	ob->setAttribute("ROT",item->Rot);
	ob->setAttribute("PLINEART",item->PLineArt);
	ob->setAttribute("PLINEEND", item->PLineEnd);
	ob->setAttribute("PLINEJOIN", item->PLineJoin);
	ob->setAttribute("LINESP",item->LineSp);
	ob->setAttribute("EXTRAV",item->ExtraV);
	ob->setAttribute("LOCALSCX",item->LocalScX);
	ob->setAttribute("LOCALSCY",item->LocalScY);
	ob->setAttribute("LOCALX",item->LocalX);
	ob->setAttribute("LOCALY",item->LocalY);
	ob->setAttribute("PICART", item->PicArt ? 1 : 0);
	ob->setAttribute("PLTSHOW", item->PoShow ? 1 : 0);
	ob->setAttribute("BASEOF", item->BaseOffs);
	ob->setAttribute("FLIPPEDH",item->flippedH);
	ob->setAttribute("FLIPPEDV",item->flippedV);
	ob->setAttribute("BBOXX",item->BBoxX);
	ob->setAttribute("BBOXH",item->BBoxH);
	ob->setAttribute("IFONT",item->IFont);
	ob->setAttribute("ISIZE",item->ISize / 10.0 );
	ob->setAttribute("SCALETYPE", item->ScaleType ? 1 : 0);
	ob->setAttribute("RATIO", item->AspectRatio ? 1 : 0);
	ob->setAttribute("PRINTABLE", item->isPrintable ? 1 : 0);
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
	ob->setAttribute("ANNAME", !item->AutoName ? item->AnName : QString(""));
	ob->setAttribute("TEXTFLOW", item->Textflow ? 1 : 0);
	ob->setAttribute("TEXTFLOW2", item->Textflow2 ? 1 : 0);
	ob->setAttribute("TEXTFLOW3", item->UseContour ? 1 : 0);
	ob->setAttribute("AUTOTEXT", item->isAutoText ? 1 : 0);
	ob->setAttribute("EXTRA",item->Extra);
	ob->setAttribute("TEXTRA",item->TExtra);
	ob->setAttribute("BEXTRA",item->BExtra);
	ob->setAttribute("REXTRA",item->RExtra);
	if (((item->PType == 2) || (item->PType == 4)) && (item->Pfile != ""))
		ob->setAttribute("PFILE",Path2Relative(item->Pfile));
	else
		ob->setAttribute("PFILE","");
	if (item->Pfile2 != "")
		ob->setAttribute("PFILE2",Path2Relative(item->Pfile2));
	else
		ob->setAttribute("PFILE2","");
	if (item->Pfile3 != "")
		ob->setAttribute("PFILE3",Path2Relative(item->Pfile3));
	else
		ob->setAttribute("PFILE3","");
	ob->setAttribute("PRFILE",item->IProfile);
	ob->setAttribute("EPROF", item->EmProfile);
	ob->setAttribute("IRENDER",item->IRender);
	ob->setAttribute("EMBEDDED", item->UseEmbedded ? 1 : 0);
	ob->setAttribute("LOCK", item->Locked ? 1 : 0);
	ob->setAttribute("LOCKR", item->LockRes ? 1 : 0);
	ob->setAttribute("REVERS", item->Reverse ? 1 : 0);
	ob->setAttribute("INVERS", item->InvPict ? 1 : 0);
	ob->setAttribute("TransValue", item->Transparency);
	ob->setAttribute("TransValueS", item->TranspStroke);
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
	ob->setAttribute("NUMTAB", static_cast<int>(item->TabValues.count()));
	QString tlp = "";
	QValueList<double>::Iterator tax;
	for (tax = item->TabValues.begin(); tax != item->TabValues.end(); ++tax)
		tlp += tmp.setNum((*tax)) + " ";
	ob->setAttribute("TABS", tlp);
	ob->setAttribute("NUMGROUP", static_cast<int>(item->Groups.count()));
	QString glp = "";
	QValueStack<int>::Iterator nx;
	for (nx = item->Groups.begin(); nx != item->Groups.end(); ++nx)
		glp += tmp.setNum((*nx)) + " ";
	ob->setAttribute("GROUPS", glp);
	ob->setAttribute("LANGUAGE", item->Language);
}

QString ScriXmlDoc::GetItemText(QDomElement *it, ScribusDoc *doc, preV *Prefs, bool VorLFound, bool impo)
{
	QString tmp2, tmf, tmpf, tmp3, tmp;
	tmp = "";
	tmp2 = it->attribute("CH");
	tmp2.replace(QRegExp("\r"), QChar(5));
	tmp2.replace(QRegExp("\n"), QChar(5));
	tmp2.replace(QRegExp("\t"), QChar(4));
	tmpf = it->attribute("CFONT", doc->Dfont);
	if (tmpf == "")
		tmpf = doc->Dfont;
	tmf = tmpf;
	if (!DoFonts.contains(tmpf))
		tmpf = AskForFont(Prefs->AvailFonts, tmpf, Prefs, doc);
	else
		tmpf = DoFonts[tmf];
	tmp3 = "\t" + tmpf + "\t";
	tmp3 += it->attribute("CSIZE") + "\t";
	tmp3 += it->attribute("CCOLOR") + "\t";
	tmp3 += it->attribute("CEXTRA") + "\t";
	tmp3 += it->attribute("CSHADE") + "\t";
	tmp3 += it->attribute("CSTYLE") + "\t";
	if (impo)
	{
		if (VorLFound)
			tmp3 += DoVorl[it->attribute("CAB","0").toUInt()] + "\t";
		else
		{
			if (it->attribute("CAB","0").toUInt() < 5)
				tmp3 += it->attribute("CAB","0")+"\t";
			else
				tmp3 += "0\t";
		}
	}
	else
		tmp3 += it->attribute("CAB","0") + "\t";
	tmp3 += it->attribute("CSTROKE","None") + "\t";
	tmp3 += it->attribute("CSHADE2","100") + "\t";
	tmp3 += it->attribute("CSCALE","100") + "\n";
	for (uint cxx=0; cxx<tmp2.length(); ++cxx)
		tmp += tmp2.at(cxx)+tmp3;
	return tmp;
}

QString ScriXmlDoc::AskForFont(SCFonts &avail, QString fStr, preV *Prefs, ScribusDoc *doc)
{
	QFont fo;
	QString tmpf = fStr;
	if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
	{
		if ((!Prefs->GFontSub.contains(tmpf)) || (!avail[Prefs->GFontSub[tmpf]]->UseFont))
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			DmF *dia = new DmF(0, tmpf, Prefs);
			dia->exec();
			tmpf = dia->Ersatz;
			delete dia;
			qApp->setOverrideCursor(QCursor(waitCursor), true);
			Prefs->GFontSub[fStr] = tmpf;
		}
		else
			tmpf = Prefs->GFontSub[tmpf];
	}
	fo = avail[tmpf]->Font;
	fo.setPointSize(qRound(doc->Dsize / 10.0));
	doc->AddFont(tmpf, fo);
	DoFonts[fStr] = tmpf;
	return tmpf;
}

bool ScriXmlDoc::ReadLStyles(QString fileName, QMap<QString,multiLine> *Sty)
{
	QDomDocument docu("scridoc");
	QString f = "";
	f = ReadDatei(fileName);
	if (f == "")
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
					struct singleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = QStoInt(MuL.attribute("Dash"));
					sl.LineEnd = QStoInt(MuL.attribute("LineEnd"));
					sl.LineJoin = QStoInt(MuL.attribute("LineJoin"));
					sl.Shade = QStoInt(MuL.attribute("Shade"));
					sl.Width = QStodouble(MuL.attribute("Width"));
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

void ScriXmlDoc::GetStyle(QDomElement *pg, struct StVorL *vg, QValueList<StVorL> &Vorlagen, ScribusDoc* doc, preV *Prefs, bool fl)
{
	bool fou;
	QString tmpf, tmf, tmV;
	double xf;
	fou = false;
	vg->Vname = pg->attribute("NAME");
	vg->LineSpa = QStodouble(pg->attribute("LINESP"));
	vg->Indent = QStodouble(pg->attribute("INDENT","0"));
	vg->First = QStodouble(pg->attribute("FIRST","0"));
	vg->Ausri = QStoInt(pg->attribute("ALIGN"));
	vg->Avor = QStodouble(pg->attribute("VOR","0"));
	vg->Anach = QStodouble(pg->attribute("NACH","0"));
	tmpf = pg->attribute("FONT", doc->Dfont);
	if (tmpf == "")
		tmpf = doc->Dfont;
	tmf = tmpf;
	if (!DoFonts.contains(tmpf))
		tmpf = AskForFont(Prefs->AvailFonts, tmpf, Prefs, doc);
	else
		tmpf = DoFonts[tmf];
	vg->Font = tmpf;
	vg->FontSize = qRound(QStodouble(pg->attribute("FONTSIZE","12")) * 10.0);
	vg->Drop = static_cast<bool>(QStoInt(pg->attribute("DROP","0")));
	vg->DropLin = QStoInt(pg->attribute("DROPLIN","2"));
	vg->FontEffect = QStoInt(pg->attribute("EFFECT","0"));
	vg->FColor = pg->attribute("FCOLOR", doc->Dbrush);
	vg->FShade = QStoInt(pg->attribute("FSHADE", "100"));
	vg->SColor = pg->attribute("SCOLOR", doc->Dpen);
	vg->SShade = QStoInt(pg->attribute("SSHADE", "100"));
	vg->BaseAdj = static_cast<bool>(QStoInt(pg->attribute("BASE","0")));
	if ((pg->hasAttribute("NUMTAB")) && (QStoInt(pg->attribute("NUMTAB","0")) != 0))
	{
		QString tmp = pg->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		vg->TabValues.clear();
		for (int cxv = 0; cxv < QStoInt(pg->attribute("NUMTAB","0")); ++cxv)
		{
			tgv >> xf;
			vg->TabValues.append(xf);
		}
		tmp = "";
	}
	else
		vg->TabValues.clear();
	for (uint xx=0; xx<Vorlagen.count(); ++xx)
	{
		if (vg->Vname == Vorlagen[xx].Vname)
		{
			if ((vg->LineSpa == Vorlagen[xx].LineSpa) &&
					(vg->Indent == Vorlagen[xx].Indent) &&
					(vg->First == Vorlagen[xx].First) &&
					(vg->Ausri == Vorlagen[xx].Ausri) &&
					(vg->Avor == Vorlagen[xx].Avor) &&
					(vg->Anach == Vorlagen[xx].Anach) &&
					(vg->Font == Vorlagen[xx].Font) &&
					(vg->TabValues == Vorlagen[xx].TabValues) &&
					(vg->Drop == Vorlagen[xx].Drop) &&
					(vg->DropLin == Vorlagen[xx].DropLin) &&
					(vg->FontEffect == Vorlagen[xx].FontEffect) &&
					(vg->FColor == Vorlagen[xx].FColor) &&
					(vg->FShade == Vorlagen[xx].FShade) &&
					(vg->SColor == Vorlagen[xx].SColor) &&
					(vg->SShade == Vorlagen[xx].SShade) &&
					(vg->BaseAdj == Vorlagen[xx].BaseAdj) &&
					(vg->FontSize == Vorlagen[xx].FontSize))
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
				vg->Vname = "Copy of "+Vorlagen[xx].Vname;
				fou = false;
			}
			break;
		}
	}
	if (!fou)
	{
		for (uint xx=0; xx< Vorlagen.count(); ++xx)
		{
			if ((vg->LineSpa == Vorlagen[xx].LineSpa) &&
				(vg->Indent == Vorlagen[xx].Indent) &&
				(vg->First == Vorlagen[xx].First) &&
				(vg->Ausri == Vorlagen[xx].Ausri) &&
				(vg->Avor == Vorlagen[xx].Avor) &&
				(vg->Anach == Vorlagen[xx].Anach) &&
				(vg->Font == Vorlagen[xx].Font) &&
				(vg->TabValues == Vorlagen[xx].TabValues) &&
				(vg->Drop == Vorlagen[xx].Drop) &&
				(vg->DropLin == Vorlagen[xx].DropLin) &&
				(vg->FontEffect == Vorlagen[xx].FontEffect) &&
				(vg->FColor == Vorlagen[xx].FColor) &&
				(vg->FShade == Vorlagen[xx].FShade) &&
				(vg->SColor == Vorlagen[xx].SColor) &&
				(vg->SShade == Vorlagen[xx].SShade) &&
				(vg->BaseAdj == Vorlagen[xx].BaseAdj) &&
				(vg->FontSize == Vorlagen[xx].FontSize))
			{
				vg->Vname = Vorlagen[xx].Vname;
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
		Vorlagen.append(*vg);
		if (fl)
		{
			DoVorl[VorlC] = tmV.setNum(Vorlagen.count()-1);
			VorlC++;
		}
	}

}

bool ScriXmlDoc::ReadStyles(QString fileName, ScribusDoc* doc, preV *Prefs)
{
	struct StVorL vg;
	QDomDocument docu("scridoc");
	QString f = "";
	QString tmpf, tmf;
	DoFonts.clear();
	f = ReadDatei(fileName);
	if (f == "")
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
				GetStyle(&pg, &vg, Vorlagen, doc, Prefs, false);
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	return true;
}

bool ScriXmlDoc::ReadColors(QString fileName)
{
	QDomDocument docu("scridoc");
	QString f = "";
	f = ReadDatei(fileName);
	if (f == "")
		return false;
	if(!docu.setContent(f))
		return false;
	Farben.clear();
	CMYKColor lf = CMYKColor();
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
			if(pg.tagName()=="COLOR" && pg.attribute("NAME")!="None")
			{
				if (pg.hasAttribute("CMYK"))
					lf.setNamedColor(pg.attribute("CMYK"));
				else
					lf.fromQColor(QColor(pg.attribute("RGB")));
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
	QString f = "";
	f = ReadDatei(fileName);
	if (f == "")
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
				if (PgNam == "")
					counter++;
				else
				{
					counter2++;
					MNames.append(PgNam);
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
	struct CLBuf OB;
	struct StVorL vg;
	struct Layer la;
	struct ScribusDoc::BookMa bok;
	struct Linked Link;
	PageItem *Neu;
	LFrames.clear();
	QString tmV, tmp, tmpf, tmp2, tmp3, tmp4, PgNam, f, Defont, tmf;
	QFont fo;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	int x, a, counter, baseobj;
	double xf;
	bool newVersion = false;
	bool VorLFound = false;
	DoVorl.clear();
	DoFonts.clear();
	DoFonts[doc->Dfont] = doc->Dfont;
	DoVorl[0] = "0";
	DoVorl[1] = "1";
	DoVorl[2] = "2";
	DoVorl[3] = "3";
	DoVorl[4] = "4";
	VorlC = 5;
	QDomDocument docu("scridoc");
	f = "";
	f = ReadDatei(fileName);
	if (f == "")
		return false;
	if(!docu.setContent(f))
		return false;
	CMYKColor lf = CMYKColor();
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
				GetStyle(&pg, &vg, doc->Vorlagen, doc, view->Prefs, true);
				VorLFound = true;
			}
			if(pg.tagName()=="JAVA")
				doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = QStoInt(pg.attribute("NUMMER"));
				la.Level = QStoInt(pg.attribute("LEVEL"));
				la.Name = pg.attribute("NAME");
				la.Sichtbar = QStoInt(pg.attribute("SICHTBAR"));
				la.Drucken = QStoInt(pg.attribute("DRUCKEN"));
				bool laex = false;
				for (uint la2 = 0; la2 < doc->Layers.count(); ++la2)
				{
					if (doc->Layers[la2].Name == la.Name)
						laex = true;
				}
				if (!laex)
					doc->Layers.append(la);
			}
			if(pg.tagName()=="Bookmark")
			{
				bok.Title = pg.attribute("Title");
				bok.Text = pg.attribute("Text");
				bok.Aktion = pg.attribute("Aktion");
				bok.ItemNr = QStoInt(pg.attribute("ItemNr"));
				bok.Seite = QStoInt(pg.attribute("Seite"));
				bok.Element = QStoInt(pg.attribute("Element"));
				bok.First = QStoInt(pg.attribute("First"));
				bok.Last = QStoInt(pg.attribute("Last"));
				bok.Prev = QStoInt(pg.attribute("Prev"));
				bok.Next = QStoInt(pg.attribute("Next"));
				bok.Parent = QStoInt(pg.attribute("Parent"));
				doc->BookMarks.append(bok);
			}
			if(pg.tagName()=="MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = PAGE.firstChild();
				while(!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct singleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = QStoInt(MuL.attribute("Dash"));
					sl.LineEnd = QStoInt(MuL.attribute("LineEnd"));
					sl.LineJoin = QStoInt(MuL.attribute("LineJoin"));
					sl.Shade = QStoInt(MuL.attribute("Shade"));
					sl.Width = QStodouble(MuL.attribute("Width"));
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
			if ((pg.tagName()=="PAGE") && (QStoInt(pg.attribute("NUM")) == PageToLoad))
			{
				a = doc->ActPage->PageNr;
				if ((pg.attribute("NAM", "") == "") && (Mpage))
				{
					PAGE=PAGE.nextSibling();
					continue;
				}
				if (Mpage)
				{
					view->Pages.at(a)->LeftPg=QStoInt(pg.attribute("LEFT","0"));
					view->Pages.at(a)->PageNam = pg.attribute("NAM","");
				}
				TableItems.clear();
				TableID.clear();
			/*
			* Attribute von PAGE auslesen
			*/
				if ((pg.hasAttribute("NumVGuides")) && (QStoInt(pg.attribute("NumVGuides","0")) != 0))
				{
					tmp = pg.attribute("VerticalGuides");
					QTextStream fgv(&tmp, IO_ReadOnly);
					view->Pages.at(a)->YGuides.clear();
					for (int cxv = 0; cxv < QStoInt(pg.attribute("NumVGuides","0")); ++cxv)
					{
						fgv >> xf;
						view->Pages.at(a)->YGuides.append(xf);
					}
					qHeapSort(view->Pages.at(a)->YGuides);
					tmp = "";
				}
				else
					view->Pages.at(a)->YGuides.clear();
				if ((pg.hasAttribute("NumHGuides")) && (QStoInt(pg.attribute("NumHGuides","0")) != 0))
				{
					tmp = pg.attribute("HorizontalGuides");
					QTextStream fgh(&tmp, IO_ReadOnly);
					view->Pages.at(a)->XGuides.clear();
					for (int cxh = 0; cxh < QStoInt(pg.attribute("NumHGuides","0")); ++cxh)
					{
						fgh >> xf;
						view->Pages.at(a)->XGuides.append(xf);
					}
					qHeapSort(view->Pages.at(a)->XGuides);
					tmp = "";
				}
				else
					view->Pages.at(a)->XGuides.clear();
				QDomNode OBJ=PAGE.firstChild();
				counter = doc->ActPage->Items.count();
				baseobj = counter;
				while(!OBJ.isNull())
				{
					QDomElement obj=OBJ.toElement();
				/*
				* Attribute von OBJECT auslesen
				*/
					if ((QStoInt(obj.attribute("NEXTITEM")) != -1) &&
							(QStoInt(obj.attribute("NEXTPAGE")) == PageToLoad))
					{
						if (QStoInt(obj.attribute("BACKITEM")) == -1)
						{
							Link.Start = counter;
							Link.StPag = a;
							LFrames.append(Link);
						}
					}
					GetItemProps(newVersion, &obj, &OB);
					OB.Xpos = QStodouble(obj.attribute("XPOS"));
					OB.Ypos=QStodouble(obj.attribute("YPOS"));
					OB.NamedLStyle = obj.attribute("NAMEDLST", "");
					if (!doc->MLineStyles.contains(OB.NamedLStyle))
						OB.NamedLStyle = "";
					OB.isBookmark=QStoInt(obj.attribute("BOOKMARK"));
					if ((OB.isBookmark) && (doc->BookMarks.count() == 0))
						doc->OldBM = true;
					OB.BMnr = QStoInt(obj.attribute("BookNr","0"));
					OB.Ausrich = DoVorl[QStoInt(obj.attribute("ALIGN","0"))].toUInt();
					tmpf = obj.attribute("IFONT", doc->Dfont);
					if (tmpf == "")
						tmpf = doc->Dfont;
					tmf = tmpf;
					if (!DoFonts.contains(tmpf))
						tmpf = AskForFont(avail, tmpf, view->Prefs, doc);
					else
						tmpf = DoFonts[tmf];
					OB.IFont = tmpf;
					OB.LayerNr = QStoInt(obj.attribute("LAYER","0"));
					OB.Language = obj.attribute("LANGUAGE", doc->Language);
					tmp = "";
					if ((obj.hasAttribute("GROUPS")) && (QStoInt(obj.attribute("NUMGROUP","0")) != 0))
					{
						tmp = obj.attribute("GROUPS");
						QTextStream fg(&tmp, IO_ReadOnly);
						OB.Groups.clear();
						for (int cx = 0; cx < QStoInt(obj.attribute("NUMGROUP","0")); ++cx)
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
						double ramp = QStodouble(it.attribute("RAMP","0.0"));
						int shade = QStoInt(it.attribute("SHADE","100"));
						double opa = QStodouble(it.attribute("TRANS","1"));
						OB.fill_gradient.addStop(SetFarbe(doc, name, shade), ramp, 0.5, opa, name, shade);
					}
					if (it.tagName()=="ITEXT")
						tmp += GetItemText(&it, doc, view->Prefs, VorLFound, true);
					IT=IT.nextSibling();
					}
					OB.Ptext = tmp;
					if ((OB.PType == 5) && (OB.Height != 0))
					{
						OB.Rot += atan2(OB.Height,OB.Width)*(180.0/3.1415927);
						OB.Width = sqrt(pow(OB.Width,2)+pow(OB.Height,2));
						OB.Height = 0;
						OB.Clip.setPoints(4, -1,-1, static_cast<int>(OB.Width+1),-1, static_cast<int>(OB.Width+1),
											static_cast<int>(OB.Height+1), -1, static_cast<int>(OB.Height+1));
					}
					view->Pages.at(a)->PasteItem(&OB, true);
					Neu = view->Pages.at(a)->Items.at(counter);
					if (QStoInt(obj.attribute("NEXTPAGE")) == PageToLoad)
					{
						Neu->NextIt = baseobj + QStoInt(obj.attribute("NEXTITEM"));
						Neu->NextPg = a; // QStoInt(obj.attribute("NEXTPAGE"));
					}
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(QStoInt(obj.attribute("OwnLINK","0")), Neu->ItemNr);
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
							ta->TopLink = view->Pages.at(a)->Items.at(TableID[ta->TopLinkID]);
						else
							ta->TopLink = 0;
						if (ta->LeftLinkID != -1)
							ta->LeftLink = view->Pages.at(a)->Items.at(TableID[ta->LeftLinkID]);
						else
							ta->LeftLink = 0;
						if (ta->RightLinkID != -1)
							ta->RightLink = view->Pages.at(a)->Items.at(TableID[ta->RightLinkID]);
						else
							ta->RightLink = 0;
						if (ta->BottomLinkID != -1)
							ta->BottomLink = view->Pages.at(a)->Items.at(TableID[ta->BottomLinkID]);
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
						Its = view->Pages.at((*lc).StPag)->Items.at((*lc).Start);
						Itr = Its;
						Its->BackBox = 0;
						while (Its->NextIt != -1)
						{
							if (Its->NextPg == a)
							{
								Itn = view->Pages.at(Its->NextPg)->Items.at(Its->NextIt);
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
	struct CLBuf OB;
	struct StVorL vg;
	struct Layer la;
	struct ScribusDoc::BookMa bok;
	int counter, Pgc;
	bool AtFl;
	bool newVersion = false;
	struct Linked Link;
	QString tmp, tmpf, tmp2, tmp3, tmp4, PgNam, Defont, tmf;
	QFont fo;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	int x, a;
	double xf;
	PageItem *Neu;
	LFrames.clear();
	QDomDocument docu("scridoc");
	QString f = "";
	f = ReadDatei(fileName);
	/* 2004/10/02 - petr vanek - bug #1092 - missing <PAGE> crash Scribus. The check constraint moved into IsScribus()
	FIXME: I've add test on containig tag PAGE but returning FALSE freezes S. in scribus.cpp need some hack too...  */
	if (!docu.setContent(f))
		return false;
	doc->PageColors.clear();
	doc->Layers.clear();
	CMYKColor lf = CMYKColor();
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8"))
		return false;
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	dia2->setTotalSteps(DOC.childNodes().count());
	dia2->setProgress(0);
	int ObCount = 0;
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
	/*
	* Attribute von DOCUMENT auslesen
	*/
		doc->PageB=QStodouble(dc.attribute("PAGEWITH"));
		doc->PageH=QStodouble(dc.attribute("PAGEHEIGHT"));
		doc->PageM.Left=QStodouble(dc.attribute("BORDERLEFT"));
		doc->PageM.Right=QStodouble(dc.attribute("BORDERRIGHT"));
		doc->PageM.Top=QStodouble(dc.attribute("BORDERTOP"));
		doc->PageM.Bottom=QStodouble(dc.attribute("BORDERBOTTOM"));
		doc->PageOri = QStoInt(dc.attribute("ORIENTATION","0"));
		doc->PageSize = dc.attribute("PAGESIZE");
		doc->FirstPnum = QStoInt(dc.attribute("FIRSTNUM","1"));
		doc->PageFP=QStoInt(dc.attribute("BOOK", "0"));
		doc->FirstPageLeft=QStoInt(dc.attribute("FIRSTLEFT","0"));
		doc->PageAT=QStoInt(dc.attribute("AUTOTEXT"));
		doc->PageSp=QStoInt(dc.attribute("AUTOSPALTEN"));
		doc->PageSpa=QStodouble(dc.attribute("ABSTSPALTEN"));
		doc->Einheit = QStoInt(dc.attribute("UNITS","0"));
		DoFonts.clear();
		doc->Dsize=qRound(QStodouble(dc.attribute("DSIZE")) * 10);
		Defont=dc.attribute("DFONT");
		if (!avail.find(Defont))
		{
			QString dd = Defont;
			if (view->Prefs->GFontSub.contains(Defont))
				Defont = view->Prefs->GFontSub[dd];
			else
				Defont = view->Prefs->DefFont;
			DoFonts[dd] = Defont;
		}
		else
			DoFonts[Defont] = Defont;
		fo = avail[Defont]->Font;
		fo.setPointSize(qRound(doc->Dsize / 10.0));
		doc->AddFont(Defont, fo);
		doc->Dfont = Defont;
		doc->DCols=QStoInt(dc.attribute("DCOL", "1"));
		doc->DGap=QStodouble(dc.attribute("DGAP", "0.0"));
		doc->DocAutor=dc.attribute("AUTHOR");
		doc->DocComments=dc.attribute("COMMENTS");
		doc->DocKeyWords=dc.attribute("KEYWORDS","");
		doc->DocTitel=dc.attribute("TITLE");
		doc->DocPublisher = dc.attribute("PUBLISHER", "");
		doc->DocDate = dc.attribute("DOCDATE", "");
		doc->DocType = dc.attribute("DOCTYPE", "");
		doc->DocFormat = dc.attribute("DOCFORMAT", "");
		doc->DocIdent = dc.attribute("DOCIDENT", "");
		doc->DocSource = dc.attribute("DOCSOURCE", "");
		doc->DocLangInfo = dc.attribute("DOCLANGINFO", "");
		doc->DocRelation = dc.attribute("DOCRELATION", "");
		doc->DocCover = dc.attribute("DOCCOVER", "");
		doc->DocRights = dc.attribute("DOCRIGHTS", "");
		doc->DocContrib = dc.attribute("DOCCONTRIB", "");
		doc->VHoch=QStoInt(dc.attribute("VHOCH"));
		doc->VHochSc=QStoInt(dc.attribute("VHOCHSC"));
		doc->VTief=QStoInt(dc.attribute("VTIEF"));
		doc->VTiefSc=QStoInt(dc.attribute("VTIEFSC"));
		doc->VKapit=QStoInt(dc.attribute("VKAPIT"));
		doc->GroupCounter=QStoInt(dc.attribute("GROUPC","1"));
		doc->HasCMS = static_cast<bool>(QStoInt(dc.attribute("HCMS","0")));
		doc->CMSSettings.SoftProofOn = static_cast<bool>(QStoInt(dc.attribute("DPSo","0")));
		doc->CMSSettings.CMSinUse = static_cast<bool>(QStoInt(dc.attribute("DPuse","0")));
		doc->CMSSettings.GamutCheck = static_cast<bool>(QStoInt(dc.attribute("DPgam","0")));
		doc->CMSSettings.BlackPoint = static_cast<bool>(QStoInt(dc.attribute("DPbla","1")));
		doc->CMSSettings.DefaultMonitorProfile = dc.attribute("DPMo","");
		doc->CMSSettings.DefaultPrinterProfile = dc.attribute("DPPr","");
		doc->CMSSettings.DefaultInputProfile = dc.attribute("DPIn","");
		doc->CMSSettings.DefaultInputProfile2 = dc.attribute("DPIn2","");
		doc->CMSSettings.DefaultIntentPrinter = QStoInt(dc.attribute("DIPr","0"));
		doc->CMSSettings.DefaultIntentMonitor = QStoInt(dc.attribute("DIMo","1"));
		doc->CMSSettings.DefaultIntentMonitor2 = QStoInt(dc.attribute("DIMo2","1"));
		doc->ActiveLayer = QStoInt(dc.attribute("ALAYER","0"));
		doc->Language = dc.attribute("LANGUAGE", "");
		doc->MinWordLen = QStoInt(dc.attribute("MINWORDLEN", "3"));
		doc->HyCount = QStoInt(dc.attribute("HYCOUNT", "2"));
		doc->Automatic = static_cast<bool>(QStoInt(dc.attribute("AUTOMATIC", "1")));
		doc->AutoCheck = static_cast<bool>(QStoInt(dc.attribute("AUTOCHECK", "0")));
		doc->GuideLock = static_cast<bool>(QStoInt(dc.attribute("GUIDELOCK", "0")));
		doc->BaseGrid = QStodouble(dc.attribute("BASEGRID", "12"));
		doc->BaseOffs = QStodouble(dc.attribute("BASEO", "0"));
		doc->minorGrid = QStodouble(dc.attribute("MINGRID", tmp.setNum(view->Prefs->DminGrid)));
		doc->majorGrid = QStodouble(dc.attribute("MAJGRID", tmp.setNum(view->Prefs->DmajGrid)));
		QDomNode PAGE=DOC.firstChild();
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
				vg.LineSpa = QStodouble(pg.attribute("LINESP"));
				vg.Indent = QStodouble(pg.attribute("INDENT","0"));
				vg.First = QStodouble(pg.attribute("FIRST","0"));
				vg.Ausri = QStoInt(pg.attribute("ALIGN"));
				vg.Avor = QStodouble(pg.attribute("VOR","0"));
				vg.Anach = QStodouble(pg.attribute("NACH","0"));
				tmpf = pg.attribute("FONT", doc->Dfont);
				if (tmpf == "")
					tmpf = doc->Dfont;
				tmf = tmpf;
				if (!DoFonts.contains(tmpf))
					tmpf = AskForFont(avail, tmpf, view->Prefs, doc);
				else
					tmpf = DoFonts[tmf];
				vg.Font = tmpf;
				vg.FontSize = qRound(QStodouble(pg.attribute("FONTSIZE","12")) * 10.0);
				vg.Drop = static_cast<bool>(QStoInt(pg.attribute("DROP","0")));
				vg.DropLin = QStoInt(pg.attribute("DROPLIN","2"));
				vg.FontEffect = QStoInt(pg.attribute("EFFECT","0"));
				vg.FColor = pg.attribute("FCOLOR", doc->Dbrush);
				vg.FShade = QStoInt(pg.attribute("FSHADE","100"));
				vg.SColor = pg.attribute("SCOLOR", doc->Dpen);
				vg.SShade = QStoInt(pg.attribute("SSHADE","100"));
				vg.BaseAdj = static_cast<bool>(QStoInt(pg.attribute("BASE","0")));
				if ((pg.hasAttribute("NUMTAB")) && (QStoInt(pg.attribute("NUMTAB","0")) != 0))
				{
					tmp = pg.attribute("TABS");
					QTextStream tgv(&tmp, IO_ReadOnly);
					vg.TabValues.clear();
					for (int cxv = 0; cxv < QStoInt(pg.attribute("NUMTAB","0")); ++cxv)
					{
						tgv >> xf;
						vg.TabValues.append(xf);
					}
					tmp = "";
				}
				else
					vg.TabValues.clear();
				doc->Vorlagen.append(vg);
			}
			if(pg.tagName()=="JAVA")
				doc->JavaScripts[pg.attribute("NAME")] = pg.attribute("SCRIPT");
			if(pg.tagName()=="LAYERS")
			{
				la.LNr = QStoInt(pg.attribute("NUMMER"));
				la.Level = QStoInt(pg.attribute("LEVEL"));
				la.Name = pg.attribute("NAME");
				la.Sichtbar = QStoInt(pg.attribute("SICHTBAR"));
				la.Drucken = QStoInt(pg.attribute("DRUCKEN"));
				doc->Layers.append(la);
			}
			if(pg.tagName()=="Bookmark")
			{
				bok.Title = pg.attribute("Title");
				bok.Text = pg.attribute("Text");
				bok.Aktion = pg.attribute("Aktion");
				bok.ItemNr = QStoInt(pg.attribute("ItemNr"));
				bok.Seite = QStoInt(pg.attribute("Seite"));
				bok.Element = QStoInt(pg.attribute("Element"));
				bok.First = QStoInt(pg.attribute("First"));
				bok.Last = QStoInt(pg.attribute("Last"));
				bok.Prev = QStoInt(pg.attribute("Prev"));
				bok.Next = QStoInt(pg.attribute("Next"));
				bok.Parent = QStoInt(pg.attribute("Parent"));
				doc->BookMarks.append(bok);
			}
			if(pg.tagName()=="MultiLine")
			{
				multiLine ml;
				QDomNode MuLn = PAGE.firstChild();
				while(!MuLn.isNull())
				{
					QDomElement MuL = MuLn.toElement();
					struct singleLine sl;
					sl.Color = MuL.attribute("Color");
					sl.Dash = QStoInt(MuL.attribute("Dash"));
					sl.LineEnd = QStoInt(MuL.attribute("LineEnd"));
					sl.LineJoin = QStoInt(MuL.attribute("LineJoin"));
					sl.Shade = QStoInt(MuL.attribute("Shade"));
					sl.Width = QStodouble(MuL.attribute("Width"));
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
				a = QStoInt(pg.attribute("NUM"));
				PgNam = "";
				PgNam = pg.attribute("NAM", "");
				Pgc = doc->PageC;
				AtFl = doc->PageAT;
				if (PgNam == "")
				{
					doc->PageC = Pgc;
					view->Pages = view->DocPages;
					doc->PageAT = AtFl;
					doc->MasterP = false;
				}
				else
				{
					doc->PageC = 0;
					doc->PageAT = false;
					view->Pages = view->MasterPages;
					doc->MasterP = true;
				}
				emit NewPage(a);
				view->Pages.at(a)->LeftPg=QStoInt(pg.attribute("LEFT","0"));
				QString Mus = "";
				Mus = pg.attribute("MNAM","Normal");
				if (!doc->MasterP)
					view->Pages.at(a)->MPageNam = Mus;
				else
					view->Pages.at(a)->MPageNam = "";
				if ((pg.hasAttribute("NumVGuides")) && (QStoInt(pg.attribute("NumVGuides","0")) != 0))
				{
					tmp = pg.attribute("VerticalGuides");
					QTextStream fgv(&tmp, IO_ReadOnly);
					view->Pages.at(a)->YGuides.clear();
					for (int cxv = 0; cxv < QStoInt(pg.attribute("NumVGuides","0")); ++cxv)
					{
						fgv >> xf;
						view->Pages.at(a)->YGuides.append(xf);
					}
					qHeapSort(view->Pages.at(a)->YGuides);
					tmp = "";
				}
				else
					view->Pages.at(a)->YGuides.clear();
				if ((pg.hasAttribute("NumHGuides")) && (QStoInt(pg.attribute("NumHGuides","0")) != 0))
				{
					tmp = pg.attribute("HorizontalGuides");
					QTextStream fgh(&tmp, IO_ReadOnly);
					view->Pages.at(a)->XGuides.clear();
					for (int cxh = 0; cxh < QStoInt(pg.attribute("NumHGuides","0")); ++cxh)
					{
						fgh >> xf;
						view->Pages.at(a)->XGuides.append(xf);
					}
					qHeapSort(view->Pages.at(a)->XGuides);
					tmp = "";
				}
				else
					view->Pages.at(a)->XGuides.clear();
				counter = 0;
				QDomNode OBJ=PAGE.firstChild();
				while(!OBJ.isNull())
				{
					QDomElement obj=OBJ.toElement();
				/*
				* Attribute von OBJECT auslesen
				*/
					if ((QStoInt(obj.attribute("NEXTITEM")) != -1) || (static_cast<bool>(QStoInt(obj.attribute("AUTOTEXT")))))
					{
						if (QStoInt(obj.attribute("BACKITEM")) == -1)
						{
							Link.Start = counter;
							Link.StPag = a;
							LFrames.append(Link);
						}
					}
					GetItemProps(newVersion, &obj, &OB);
					OB.Xpos = QStodouble(obj.attribute("XPOS"));
					OB.Ypos=QStodouble(obj.attribute("YPOS"));
					OB.NamedLStyle = obj.attribute("NAMEDLST", "");
					OB.isBookmark=QStoInt(obj.attribute("BOOKMARK"));
					if ((OB.isBookmark) && (doc->BookMarks.count() == 0))
						doc->OldBM = true;
					OB.BMnr = QStoInt(obj.attribute("BookNr","0"));
					OB.Ausrich = QStoInt(obj.attribute("ALIGN","0"));
					tmpf = obj.attribute("IFONT", doc->Dfont);
					if (tmpf == "")
						tmpf = doc->Dfont;
					tmf = tmpf;
					if (!DoFonts.contains(tmpf))
						tmpf = AskForFont(avail, tmpf, view->Prefs, doc);
					else
						tmpf = DoFonts[tmf];
					OB.IFont = tmpf;
					OB.LayerNr = QStoInt(obj.attribute("LAYER","0"));
					OB.Language = obj.attribute("LANGUAGE", doc->Language);
					tmp = "";
					if ((obj.hasAttribute("GROUPS")) && (QStoInt(obj.attribute("NUMGROUP","0")) != 0))
					{
						tmp = obj.attribute("GROUPS");
						QTextStream fg(&tmp, IO_ReadOnly);
						OB.Groups.clear();
						for (int cx = 0; cx < QStoInt(obj.attribute("NUMGROUP","0")); ++cx)
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
							double ramp = QStodouble(it.attribute("RAMP","0.0"));
							int shade = QStoInt(it.attribute("SHADE","100"));
							double opa = QStodouble(it.attribute("TRANS","1"));
							OB.fill_gradient.addStop(SetFarbe(doc, name, shade), ramp, 0.5, opa, name, shade);
						}
						if (it.tagName()=="ITEXT")
							tmp += GetItemText(&it, doc, view->Prefs, false, false);
						IT=IT.nextSibling();
					}
					OB.Ptext = tmp;
					int docGc = doc->GroupCounter;
					doc->GroupCounter = 0;
					if ((OB.PType == 5) && (OB.Height != 0))
					{
						OB.Rot += atan2(OB.Height,OB.Width)*(180.0/3.1415927);
						OB.Width = sqrt(pow(OB.Width,2)+pow(OB.Height,2));
						OB.Height = 0;
						OB.Clip.setPoints(4, -1,-1, static_cast<int>(OB.Width+1),-1, static_cast<int>(OB.Width+1),
											 static_cast<int>(OB.Height+1), -1, static_cast<int>(OB.Height+1));
					}
					view->Pages.at(a)->PasteItem(&OB, true);
					doc->GroupCounter = docGc;
					Neu = view->Pages.at(a)->Items.at(counter);
					Neu->isAutoText=static_cast<bool>(QStoInt(obj.attribute("AUTOTEXT")));
					if (Neu->isAutoText)
						doc->LastAuto = Neu;
					Neu->NextIt = QStoInt(obj.attribute("NEXTITEM"));
					Neu->NextPg = QStoInt(obj.attribute("NEXTPAGE"));
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(QStoInt(obj.attribute("OwnLINK","0")), Neu->ItemNr);
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
							ta->TopLink = view->Pages.at(a)->Items.at(TableID[ta->TopLinkID]);
						else
							ta->TopLink = 0;
						if (ta->LeftLinkID != -1)
							ta->LeftLink = view->Pages.at(a)->Items.at(TableID[ta->LeftLinkID]);
						else
							ta->LeftLink = 0;
						if (ta->RightLinkID != -1)
							ta->RightLink = view->Pages.at(a)->Items.at(TableID[ta->RightLinkID]);
						else
							ta->RightLink = 0;
						if (ta->BottomLinkID != -1)
							ta->BottomLink = view->Pages.at(a)->Items.at(TableID[ta->BottomLinkID]);
						else
							ta->BottomLink = 0;
					}
				}
				if (PgNam == "")
					view->DocPages = view->Pages;
				else
				{
					view->Pages.at(a)->PageNam = PgNam;
					view->MasterNames[PgNam] = a;
					view->MasterPages = view->Pages;
				}
				doc->MasterP = false;
				doc->PageC = Pgc+1;
				doc->PageAT = AtFl;
			}
			PAGE=PAGE.nextSibling();
		}
		PAGE=DOC.firstChild();
		while(!PAGE.isNull())
		{
			QDomElement pg=PAGE.toElement();
			if(pg.tagName()=="PDF")
			{
				doc->PDF_Optionen.Articles = static_cast<bool>(QStoInt(pg.attribute("Articles")));
				doc->PDF_Optionen.Thumbnails = static_cast<bool>(QStoInt(pg.attribute("Thumbnails")));
				doc->PDF_Optionen.Compress = static_cast<bool>(QStoInt(pg.attribute("Compress")));
				doc->PDF_Optionen.CompressMethod = QStoInt(pg.attribute("CMethod","0"));
				doc->PDF_Optionen.Quality = QStoInt(pg.attribute("Quality","0"));
				doc->PDF_Optionen.RecalcPic = static_cast<bool>(QStoInt(pg.attribute("RecalcPic")));
				doc->PDF_Optionen.Bookmarks = static_cast<bool>(QStoInt(pg.attribute("Bookmarks")));
				if (pg.hasAttribute("MirrorH"))
					doc->PDF_Optionen.MirrorH = static_cast<bool>(QStoInt(pg.attribute("MirrorH")));
				else
					doc->PDF_Optionen.MirrorH = false;
				if (pg.hasAttribute("MirrorV"))
					doc->PDF_Optionen.MirrorV = static_cast<bool>(QStoInt(pg.attribute("MirrorV")));
				else
					doc->PDF_Optionen.MirrorV = false;
				if (pg.hasAttribute("RotateDeg"))
					doc->PDF_Optionen.RotateDeg = QStoInt(pg.attribute("RotateDeg","0"));
				else
					doc->PDF_Optionen.RotateDeg = 0;
				doc->PDF_Optionen.PresentMode = static_cast<bool>(QStoInt(pg.attribute("PresentMode")));
				doc->PDF_Optionen.PicRes = QStoInt(pg.attribute("PicRes"));
				doc->PDF_Optionen.Version = QStoInt(pg.attribute("Version"));
				doc->PDF_Optionen.Resolution = QStoInt(pg.attribute("Resolution"));
				doc->PDF_Optionen.Binding = QStoInt(pg.attribute("Binding"));
				doc->PDF_Optionen.Datei = "";
				doc->PDF_Optionen.UseRGB = static_cast<bool>(QStoInt(pg.attribute("RGBMode","0")));
				doc->PDF_Optionen.UseProfiles = static_cast<bool>(QStoInt(pg.attribute("UseProfiles","0")));
				doc->PDF_Optionen.UseProfiles2 = static_cast<bool>(QStoInt(pg.attribute("UseProfiles2","0")));
				doc->PDF_Optionen.Intent = QStoInt(pg.attribute("Intent","1"));
				doc->PDF_Optionen.Intent2 = QStoInt(pg.attribute("Intent2","1"));
				doc->PDF_Optionen.SolidProf = pg.attribute("SolidP", "");
				doc->PDF_Optionen.ImageProf = pg.attribute("ImageP", "");
				doc->PDF_Optionen.PrintProf = pg.attribute("PrintP", "");
				doc->PDF_Optionen.Info = pg.attribute("InfoString", "");
				doc->PDF_Optionen.BleedTop = QStodouble(pg.attribute("BTop","0"));
				doc->PDF_Optionen.BleedLeft = QStodouble(pg.attribute("BLeft","0"));
				doc->PDF_Optionen.BleedRight = QStodouble(pg.attribute("BRight","0"));
				doc->PDF_Optionen.BleedBottom = QStodouble(pg.attribute("BBottom","0"));
				doc->PDF_Optionen.EmbeddedI = static_cast<bool>(QStoInt(pg.attribute("ImagePr","0")));
				doc->PDF_Optionen.PassOwner = pg.attribute("PassOwner", "");
				doc->PDF_Optionen.PassUser = pg.attribute("PassUser", "");
				doc->PDF_Optionen.Permissions = QStoInt(pg.attribute("Permissions","-4"));
				doc->PDF_Optionen.Encrypt = static_cast<bool>(QStoInt(pg.attribute("Encrypt","0")));
				doc->PDF_Optionen.UseLPI = static_cast<bool>(QStoInt(pg.attribute("UseLpi","0")));
				QDomNode PFO = PAGE.firstChild();
				while(!PFO.isNull())
				{
					QDomElement pdfF = PFO.toElement();
					if(pdfF.tagName() == "LPI")
					{
						struct LPIset lpo;
						lpo.Angle = QStoInt(pdfF.attribute("Angle"));
						lpo.Frequency = QStoInt(pdfF.attribute("Frequency"));
						lpo.SpotFunc = QStoInt(pdfF.attribute("SpotFunction"));
						doc->PDF_Optionen.LPISettings[pdfF.attribute("Color")] = lpo;
					}
					if(pdfF.tagName() == "Fonts")
					{
						if (!doc->PDF_Optionen.EmbedList.contains(DoFonts[pdfF.attribute("Name")]))
							doc->PDF_Optionen.EmbedList.append(DoFonts[pdfF.attribute("Name")]);
					}
					if(pdfF.tagName() == "Subset")
					{
						if (!doc->PDF_Optionen.SubsetList.contains(DoFonts[pdfF.attribute("Name")]))
							doc->PDF_Optionen.SubsetList.append(DoFonts[pdfF.attribute("Name")]);
					}
					if(pdfF.tagName() == "Effekte")
					{
    					struct PreSet ef;
    					ef.EffektLen = QStoInt(pdfF.attribute("EffektLen"));
    					ef.AnzeigeLen = QStoInt(pdfF.attribute("AnzeigeLen"));
    					ef.Effekt = QStoInt(pdfF.attribute("Effekt"));
    					ef.Dm = QStoInt(pdfF.attribute("Dm"));
    					ef.M = QStoInt(pdfF.attribute("M"));
		    			ef.Di = QStoInt(pdfF.attribute("Di"));
						doc->PDF_Optionen.PresentVals.append(ef);
					}
					PFO = PFO.nextSibling();
				}
			}
			PAGE=PAGE.nextSibling();
		}
		DOC=DOC.nextSibling();
	}
	for (uint ap=0; ap<view->MasterPages.count(); ++ap)
		view->MasterPages.at(ap)->parentWidget()->hide();
	view->Pages = view->DocPages;
	doc->PageC = view->Pages.count();
	view->reformPages();
	if (doc->Layers.count() == 0)
	{
		la.LNr = 0;
		la.Level = 0;
		la.Name = tr("Background");
		la.Sichtbar = true;
		la.Drucken = true;
		doc->Layers.append(la);
	}
	if (LFrames.count() != 0)
	{
		PageItem *Its;
		PageItem *Itn;
		PageItem *Itr;
		QValueList<Linked>::Iterator lc;
		for (lc = LFrames.begin(); lc != LFrames.end(); ++lc)
		{
			Its = view->Pages.at((*lc).StPag)->Items.at((*lc).Start);
			Itr = Its;
			Its->BackBox = 0;
			if (Its->isAutoText)
				doc->FirstAuto = Its;
			while (Its->NextIt != -1)
			{
				Itn = view->Pages.at(Its->NextPg)->Items.at(Its->NextIt);
				Its->NextBox = Itn;
				Itn->BackBox = Its;
				Its = Itn;
			}
			Its->NextBox = 0;
		}
	}
	view->UN->setText(doc->Einheit == 0 ? "pt" : "mm");
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
	*x = QStodouble(elem.attribute("XP"));
	*y = QStodouble(elem.attribute("YP"));
	*w = QStodouble(elem.attribute("W","0"));
	*h = QStodouble(elem.attribute("H","0"));
	return true;
}

bool ScriXmlDoc::ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, int Xp, int Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub, preV *Prefs)
{
	struct CLBuf OB;
	struct StVorL vg;
	QString tmp, tmpf, tmp2, tmp3, tmp4, f, tmV, tmf;
	QFont fo;
	QMap<QString,QString> DoMul;
	QMap<int,int> TableID;
	QPtrList<PageItem> TableItems;
	bool VorLFound = false;
	bool newVersion = false;
	int x;
	double GrX, GrY;
	int GrMax = doc->GroupCounter;
	CMYKColor lf = CMYKColor();
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
		GrX = 0;
		GrY = 0;
	}
	else
	{
		GrX = QStodouble(elem.attribute("XP"));
		GrY = QStodouble(elem.attribute("YP"));
	}
	if (elem.hasAttribute("Version"))
		newVersion = true;
	QDomNode DOC=elem.firstChild();
	DoFonts.clear();
	DoFonts[doc->Dfont] = doc->Dfont;
	DoVorl.clear();
	DoVorl[0] = "0";
	DoVorl[1] = "1";
	DoVorl[2] = "2";
	DoVorl[3] = "3";
	DoVorl[4] = "4";
	VorlC = 5;
	TableItems.clear();
	TableID.clear();
	QString CurDirP = QDir::currentDirPath();
	QDir::setCurrent(QDir::homeDirPath());
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="FONT")
		{
			tmpf = pg.attribute("NAME");
			if ((!avail.find(tmpf)) || (!avail[tmpf]->UseFont))
			{
				if (!FontSub.contains(tmpf) || (!avail[FontSub[tmpf]]->UseFont))
				{
					DmF *dia = new DmF(0, tmpf, Prefs);
					dia->exec();
					tmpf = dia->Ersatz;
					FontSub[pg.attribute("NAME")] = tmpf;
					delete dia;
				}
				else
					tmpf = FontSub[tmpf];
			}
			fo = avail[tmpf]->Font;
			fo.setPointSize(qRound(doc->Dsize / 10.0));
			if(!doc->UsedFonts.contains(tmpf))
				doc->AddFont(tmpf, fo);
			DoFonts[pg.attribute("NAME")] = tmpf;
		}
		// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
		if(pg.tagName()=="COLOR" && pg.attribute("Name")!="None")
		{
			if (pg.hasAttribute("CMYK"))
				lf.setNamedColor(pg.attribute("CMYK"));
			else
				lf.fromQColor(QColor(pg.attribute("RGB")));
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
				struct singleLine sl;
				sl.Color = MuL.attribute("Color");
				sl.Dash = QStoInt(MuL.attribute("Dash"));
				sl.LineEnd = QStoInt(MuL.attribute("LineEnd"));
				sl.LineJoin = QStoInt(MuL.attribute("LineJoin"));
				sl.Shade = QStoInt(MuL.attribute("Shade"));
				sl.Width = QStodouble(MuL.attribute("Width"));
				ml.push_back(sl);
				MuLn = MuLn.nextSibling();
			}
			if (!doc->MLineStyles.contains(pg.attribute("Name")))
				doc->MLineStyles.insert(pg.attribute("Name"), ml);
		}
		if(pg.tagName()=="STYLE")
		{
			GetStyle(&pg, &vg, doc->Vorlagen, doc, Prefs, true);
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
			OB.Xpos = static_cast<double>(Xp) + QStodouble(pg.attribute("XPOS")) - GrX;
			OB.Ypos = static_cast<double>(Yp) + QStodouble(pg.attribute("YPOS")) - GrY;
			GetItemProps(newVersion, &pg, &OB);
			OB.NamedLStyle = pg.attribute("NAMEDLST", "");
			if (!doc->MLineStyles.contains(OB.NamedLStyle))
				OB.NamedLStyle = "";
			OB.isBookmark = false;
			OB.BMnr = 0;
			OB.Ausrich = DoVorl[QStoInt(pg.attribute("ALIGN","0"))].toUInt();
			tmf = pg.attribute("IFONT", doc->Dfont);
			if (tmf == "")
				tmf = doc->Dfont;
			if (DoFonts[tmf] == "")
				OB.IFont = doc->Dfont;
			else
				OB.IFont = DoFonts[tmf];
			OB.LayerNr = 0;
			OB.Language = pg.attribute("LANGUAGE", doc->Language);
			tmp = "";
			if ((pg.hasAttribute("GROUPS")) && (QStoInt(pg.attribute("NUMGROUP","0")) != 0))
			{
				tmp = pg.attribute("GROUPS");
				QTextStream fg(&tmp, IO_ReadOnly);
				OB.Groups.clear();
				for (int cx = 0; cx < QStoInt(pg.attribute("NUMGROUP","0")); ++cx)
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
					double ramp = QStodouble(it.attribute("RAMP","0.0"));
					int shade = QStoInt(it.attribute("SHADE","100"));
					double opa = QStodouble(it.attribute("TRANS","1"));
					OB.fill_gradient.addStop(SetFarbe(doc, name, shade), ramp, 0.5, opa, name, shade);
				}
				if (it.tagName()=="ITEXT")
					tmp += GetItemText(&it, doc, Prefs, VorLFound, true);
				IT=IT.nextSibling();
			}
			OB.Ptext = tmp;
			if ((OB.PType == 5) && (OB.Height != 0))
			{
				OB.Rot += atan2(OB.Height,OB.Width)*(180.0/3.1415927);
				OB.Width = sqrt(pow(OB.Width,2)+pow(OB.Height,2));
				OB.Height = 0;
				OB.Clip.setPoints(4, -1,-1, static_cast<int>(OB.Width+1),-1, static_cast<int>(OB.Width+1),
									static_cast<int>(OB.Height+1), -1, static_cast<int>(OB.Height+1));
			}
			OB.LayerNr = -1;
			doc->ActPage->PasteItem(&OB, true, true);
			PageItem* Neu = doc->ActPage->Items.at(doc->ActPage->Items.count()-1);
			if (Neu->isTableItem)
			{
				TableItems.append(Neu);
				TableID.insert(QStoInt(pg.attribute("OwnLINK","0")), Neu->ItemNr);
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
				ta->TopLink = doc->ActPage->Items.at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = doc->ActPage->Items.at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = doc->ActPage->Items.at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = doc->ActPage->Items.at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	doc->GroupCounter = GrMax + 1;
	QDir::setCurrent(CurDirP);
	return true;
}

QString ScriXmlDoc::WriteElem(QPtrList<PageItem> *Selitems, ScribusDoc *doc)
{
	int tsh, tsh2, tst, tst2, tsb, tsb2, tshs, tshs2, tsc, tsc2;
	QString text, tf, tf2, tc, tc2, tcs, tcs2, tmp, tmpy;
	double te, te2, ts, ts2;
	PageItem *item;
	QDomDocument docu("scribus");
	QString st="<SCRIBUSELEMUTF8></SCRIBUSELEMUTF8>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	item = Selitems->at(0);
	QValueList<uint> ELL;
	for (uint cor=0; cor<Selitems->count(); ++cor)
		ELL.append(Selitems->at(cor)->ItemNr);
	qHeapSort(ELL);
	if (doc->ActPage->GroupSel)
	{
		elem.setAttribute("XP", doc->ActPage->GroupX);
		elem.setAttribute("YP", doc->ActPage->GroupY);
		elem.setAttribute("W", doc->ActPage->GroupW);
		elem.setAttribute("H", doc->ActPage->GroupH);
	}
	else
	{
		if (item->Rot == 0)
		{
			elem.setAttribute("XP", item->Xpos);
			elem.setAttribute("YP", item->Ypos);
			elem.setAttribute("W", item->Width);
			elem.setAttribute("H", item->Height);
		}
		else
		{
			QPainter p;
			p.begin(doc->ActPage);
			p.translate(static_cast<int>(item->Xpos), static_cast<int>(item->Ypos));
			p.rotate(item->Rot);
			QRect apr = QRegion(p.xForm(QRect(0, 0, static_cast<int>(item->Width),
											 static_cast<int>(QMAX(item->Height, 1))))).boundingRect();
			p.end();
			elem.setAttribute("XP", apr.x());
			elem.setAttribute("YP", apr.y());
			elem.setAttribute("W", apr.width());
			elem.setAttribute("H", apr.height());
		}
	}
	elem.setAttribute("COUNT", Selitems->count());
	elem.setAttribute("Version", "1.2cvs");
	QMap<QString,QFont>::Iterator itf;
	for (itf = doc->UsedFonts.begin(); itf != doc->UsedFonts.end(); ++itf)
	{
		QDomElement fn=docu.createElement("FONT");
		fn.setAttribute("NAME",itf.key());
		elem.appendChild(fn);
	}
	CListe::Iterator itc;
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		QDomElement co=docu.createElement("COLOR");
		co.setAttribute("NAME",itc.key());
		co.setAttribute("RGB",doc->PageColors[itc.key()].getRGBColor().name());
		co.setAttribute("CMYK",doc->PageColors[itc.key()].name());
		elem.appendChild(co);
	}
	QMap<int, StVorL> UsedStyles;
	QMap<int, int> UsedMapped2Saved;
	int NewStylesNum = 5;
	UsedStyles.clear();
	UsedMapped2Saved.clear();
	struct StVorL vg;
	if (doc->Vorlagen.count() > 5)
	{
		for (uint co=0; co<Selitems->count(); ++co)
		{
			item = doc->ActPage->Items.at(ELL[co]);
			if (item->Ausrich > 4)
			{
				vg.Vname = doc->Vorlagen[item->Ausrich].Vname;
				vg.LineSpa = doc->Vorlagen[item->Ausrich].LineSpa;
				vg.Ausri = doc->Vorlagen[item->Ausrich].Ausri;
				vg.Indent = doc->Vorlagen[item->Ausrich].Indent;
				vg.First = doc->Vorlagen[item->Ausrich].First;
				vg.Avor = doc->Vorlagen[item->Ausrich].Avor;
				vg.Anach = doc->Vorlagen[item->Ausrich].Anach;
				vg.Font = doc->Vorlagen[item->Ausrich].Font;
				vg.FontSize = doc->Vorlagen[item->Ausrich].FontSize;
				vg.TabValues = doc->Vorlagen[item->Ausrich].TabValues;
				vg.Drop = doc->Vorlagen[item->Ausrich].Drop;
				vg.DropLin = doc->Vorlagen[item->Ausrich].DropLin;
				vg.FontEffect = doc->Vorlagen[item->Ausrich].FontEffect;
				vg.FColor = doc->Vorlagen[item->Ausrich].FColor;
				vg.FShade = doc->Vorlagen[item->Ausrich].FShade;
				vg.SColor = doc->Vorlagen[item->Ausrich].SColor;
				vg.SShade = doc->Vorlagen[item->Ausrich].SShade;
				vg.BaseAdj = doc->Vorlagen[item->Ausrich].BaseAdj;
				UsedStyles[item->Ausrich] = vg;
			}
			if (((item->PType == 4) || (item->PType == 8)) && (item->Ptext.count() != 0))
			{
				for (uint tx = 0; tx < item->Ptext.count(); ++tx)
				{
					if (item->Ptext.at(tx)->cab > 4)
					{
						vg.Vname = doc->Vorlagen[item->Ptext.at(tx)->cab].Vname;
						vg.LineSpa = doc->Vorlagen[item->Ptext.at(tx)->cab].LineSpa;
						vg.Ausri = doc->Vorlagen[item->Ptext.at(tx)->cab].Ausri;
						vg.Indent = doc->Vorlagen[item->Ptext.at(tx)->cab].Indent;
						vg.First = doc->Vorlagen[item->Ptext.at(tx)->cab].First;
						vg.Avor = doc->Vorlagen[item->Ptext.at(tx)->cab].Avor;
						vg.Anach = doc->Vorlagen[item->Ptext.at(tx)->cab].Anach;
						vg.Font = doc->Vorlagen[item->Ptext.at(tx)->cab].Font;
						vg.FontSize = doc->Vorlagen[item->Ptext.at(tx)->cab].FontSize;
						vg.TabValues = doc->Vorlagen[item->Ptext.at(tx)->cab].TabValues;
						vg.Drop = doc->Vorlagen[item->Ptext.at(tx)->cab].Drop;
						vg.DropLin = doc->Vorlagen[item->Ptext.at(tx)->cab].DropLin;
						vg.FontEffect = doc->Vorlagen[item->Ptext.at(tx)->cab].FontEffect;
						vg.FColor = doc->Vorlagen[item->Ptext.at(tx)->cab].FColor;
						vg.FShade = doc->Vorlagen[item->Ptext.at(tx)->cab].FShade;
						vg.SColor = doc->Vorlagen[item->Ptext.at(tx)->cab].SColor;
						vg.SShade = doc->Vorlagen[item->Ptext.at(tx)->cab].SShade;
						vg.BaseAdj = doc->Vorlagen[item->Ptext.at(tx)->cab].BaseAdj;
						UsedStyles[item->Ptext.at(tx)->cab] = vg;
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
			fo.setAttribute("ALIGN",UsedStyles[actSt].Ausri);
			fo.setAttribute("LINESP",UsedStyles[actSt].LineSpa);
			fo.setAttribute("INDENT",UsedStyles[actSt].Indent);
			fo.setAttribute("FIRST",UsedStyles[actSt].First);
			fo.setAttribute("VOR",UsedStyles[actSt].Avor);
			fo.setAttribute("NACH",UsedStyles[actSt].Anach);
			fo.setAttribute("FONT",UsedStyles[actSt].Font);
			fo.setAttribute("FONTSIZE",UsedStyles[actSt].FontSize / 10.0);
			fo.setAttribute("DROP", static_cast<int>(UsedStyles[actSt].Drop));
			fo.setAttribute("DROPLIN", UsedStyles[actSt].DropLin);
			fo.setAttribute("EFFECT", UsedStyles[actSt].FontEffect);
			fo.setAttribute("NUMTAB", static_cast<int>(UsedStyles[actSt].TabValues.count()));
			QString tlp = "";
			QValueList<double>::Iterator tax;
			for (tax = UsedStyles[actSt].TabValues.begin(); tax != UsedStyles[actSt].TabValues.end(); ++tax)
				tlp += tmp.setNum((*tax)) + " ";
			fo.setAttribute("TABS", tlp);
			fo.setAttribute("FCOLOR",UsedStyles[actSt].FColor);
			fo.setAttribute("FSHADE",UsedStyles[actSt].FShade);
			fo.setAttribute("SCOLOR",UsedStyles[actSt].SColor);
			fo.setAttribute("SSHADE",UsedStyles[actSt].SShade);
			fo.setAttribute("BASE", static_cast<int>(UsedStyles[actSt].BaseAdj));
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
	for (uint co=0; co<Selitems->count(); ++co)
	{
		QString CurDirP = QDir::currentDirPath();
		QDir::setCurrent(QDir::homeDirPath());
		item = doc->ActPage->Items.at(ELL[co]);
		QDomElement ob=docu.createElement("ITEM");
		if (item->Ausrich > 4)
			ob.setAttribute("ALIGN",UsedMapped2Saved[item->Ausrich]);
		else
			ob.setAttribute("ALIGN",item->Ausrich);
		SetItemProps(&ob, item);
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
		for(uint k=0;k<item->Ptext.count();++k)
		{
			QDomElement it=docu.createElement("ITEXT");
			ts = item->Ptext.at(k)->csize / 10.0;
			tf = item->Ptext.at(k)->cfont;
			tc = item->Ptext.at(k)->ccolor;
			te = item->Ptext.at(k)->cextra;
			tsh = item->Ptext.at(k)->cshade;
			tst = item->Ptext.at(k)->cstyle;
			if (item->Ptext.at(k)->cab > 4)
				tsb = UsedMapped2Saved[item->Ptext.at(k)->cab];
			else
				tsb = item->Ptext.at(k)->cab;
			tcs = item->Ptext.at(k)->cstroke;
			tshs = item->Ptext.at(k)->cshade2;
			tsc = item->Ptext.at(k)->cscale;
			if (item->Ptext.at(k)->ch == QChar(13))
				text = QChar(5);
			else if (item->Ptext.at(k)->ch == QChar(9))
				text = QChar(4);
			else
				text = item->Ptext.at(k)->ch;
			k++;
			if (k == item->Ptext.count())
			{
				it.setAttribute("CH",text);
				it.setAttribute("CSIZE",ts);
				it.setAttribute("CFONT",tf);
				it.setAttribute("CCOLOR",tc);
				it.setAttribute("CEXTRA",te);
				it.setAttribute("CSHADE",tsh);
				it.setAttribute("CSTYLE",tst);
				it.setAttribute("CAB",tsb);
				it.setAttribute("CSTROKE",tcs);
				it.setAttribute("CSHADE2",tshs);
				it.setAttribute("CSCALE",tsc);
				ob.appendChild(it);
				break;
			}
			ts2 = item->Ptext.at(k)->csize / 10.0;
			tf2 = item->Ptext.at(k)->cfont;
			tc2 = item->Ptext.at(k)->ccolor;
			te2 = item->Ptext.at(k)->cextra;
			tsh2 = item->Ptext.at(k)->cshade;
			tst2 = item->Ptext.at(k)->cstyle;
			if (item->Ptext.at(k)->cab > 4)
				tsb2 = UsedMapped2Saved[item->Ptext.at(k)->cab];
			else
				tsb2 = item->Ptext.at(k)->cab;
			tcs2 = item->Ptext.at(k)->cstroke;
			tshs2 = item->Ptext.at(k)->cshade2;
			tsc2 = item->Ptext.at(k)->cscale;
			while ((ts2 == ts)
							&& (tsb2 == tsb)
							&& (tf2 == tf)
							&& (tc2 == tc)
							&& (te2 == te)
							&& (tsh2 == tsh)
							&& (tshs2 == tshs)
							&& (tsc2 == tsc)
							&& (tcs2 == tcs)
							&& (tst2 == tst))
			{
				if (item->Ptext.at(k)->ch == QChar(13))
					text += QChar(5);
				else if (item->Ptext.at(k)->ch == QChar(9))
					text += QChar(4);
				else
					text += item->Ptext.at(k)->ch;
				k++;
				if (k == item->Ptext.count())
					break;
				ts2 = item->Ptext.at(k)->csize / 10.0;
				tf2 = item->Ptext.at(k)->cfont;
				tc2 = item->Ptext.at(k)->ccolor;
				te2 = item->Ptext.at(k)->cextra;
				tsh2 = item->Ptext.at(k)->cshade;
				tst2 = item->Ptext.at(k)->cstyle;
				if (item->Ptext.at(k)->cab > 4)
					tsb2 = UsedMapped2Saved[item->Ptext.at(k)->cab];
				else
					tsb2 = item->Ptext.at(k)->cab;
				tcs2 = item->Ptext.at(k)->cstroke;
				tshs2 = item->Ptext.at(k)->cshade2;
				tsc2 = item->Ptext.at(k)->cscale;
			}
			it.setAttribute("CH",text);
			it.setAttribute("CSIZE",ts);
			it.setAttribute("CFONT",tf);
			it.setAttribute("CCOLOR",tc);
			it.setAttribute("CEXTRA",te);
			it.setAttribute("CSHADE",tsh);
			it.setAttribute("CSTYLE",tst);
			it.setAttribute("CAB",tsb);
			it.setAttribute("CSTROKE",tcs);
			it.setAttribute("CSHADE2",tshs);
			it.setAttribute("CSCALE",tsc);
			k--;
			ob.appendChild(it);
		}
		ob.setAttribute("NUMTEXT",item->Ptext.count());
		QString txnu = "";
		for(uint kt=0;kt<item->Ptext.count();++kt)
			txnu += tmp.setNum(item->Ptext.at(kt)->xp) + " " + tmpy.setNum(item->Ptext.at(kt)->yp) + " ";
		ob.setAttribute("TEXTCOOR", txnu);
		ob.setAttribute("BACKITEM", -1);
		ob.setAttribute("BACKPAGE", -1);
		ob.setAttribute("NEXTITEM", -1);
		ob.setAttribute("NEXTPAGE", -1);
		elem.appendChild(ob);
	}
	return docu.toString().utf8();
}

void ScriXmlDoc::WritePages(ScribusView *view, QDomDocument docu, QDomElement dc, QProgressBar *dia2, uint maxC)
{
	int tsh, tsh2, tst, tst2, tsb, tsb2, tshs, tshs2, tsc, tsc2;
	QString text, tf, tf2, tc, tc2, tcs, tcs2, tmp, tmpy, Ndir;
	double ts, ts2, te, te2;
	uint ObCount = maxC;
	Page *page;
	PageItem *item;
	for(uint i=0;i<view->Pages.count();++i)
	{
		ObCount++;
		if (dia2 != 0)
			dia2->setProgress(ObCount);
		page = view->Pages.at(i);
		QDomElement pg=docu.createElement("PAGE");
		pg.setAttribute("NUM",page->PageNr);
		pg.setAttribute("NAM",page->PageNam);
		pg.setAttribute("MNAM",page->MPageNam);
		pg.setAttribute("LEFT", page->LeftPg ? 1 : 0);
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
		for(uint j=0;j<page->Items.count();++j)
		{
			item = page->Items.at(j);
			QDomElement ob=docu.createElement("PAGEOBJECT");
			SetItemProps(&ob, item);
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
			ob.setAttribute("ALIGN",item->Ausrich);
			ob.setAttribute("BOOKMARK", item->isBookmark ? 1 : 0);
			ob.setAttribute("BookNr", item->BMnr);
			for(uint k=0;k<item->Ptext.count();++k)
			{
				QDomElement it=docu.createElement("ITEXT");
				ts = item->Ptext.at(k)->csize / 10.0;
				tf = item->Ptext.at(k)->cfont;
				tc = item->Ptext.at(k)->ccolor;
				te = item->Ptext.at(k)->cextra;
				tsh = item->Ptext.at(k)->cshade;
				tst = item->Ptext.at(k)->cstyle;
				tsb = item->Ptext.at(k)->cab;
				tcs = item->Ptext.at(k)->cstroke;
				tshs = item->Ptext.at(k)->cshade2;
				tsc = item->Ptext.at(k)->cscale;
				if (item->Ptext.at(k)->ch == QChar(13))
					text = QChar(5);
				else if (item->Ptext.at(k)->ch == QChar(9))
					text = QChar(4);
				else
					text = item->Ptext.at(k)->ch;
				k++;
				if (k == item->Ptext.count())
				{
					it.setAttribute("CH",text);
					it.setAttribute("CSIZE",ts);
					it.setAttribute("CFONT",tf);
					it.setAttribute("CCOLOR",tc);
					it.setAttribute("CEXTRA",te);
					it.setAttribute("CSHADE",tsh);
					it.setAttribute("CSTYLE",tst);
					it.setAttribute("CAB",tsb);
					it.setAttribute("CSTROKE",tcs);
					it.setAttribute("CSHADE2",tshs);
					it.setAttribute("CSCALE",tsc);
					ob.appendChild(it);
					break;
				}
				ts2 = item->Ptext.at(k)->csize / 10.0;
				tf2 = item->Ptext.at(k)->cfont;
				tc2 = item->Ptext.at(k)->ccolor;
				te2 = item->Ptext.at(k)->cextra;
				tsh2 = item->Ptext.at(k)->cshade;
				tst2 = item->Ptext.at(k)->cstyle;
				tsb2 = item->Ptext.at(k)->cab;
				tcs2 = item->Ptext.at(k)->cstroke;
				tshs2 = item->Ptext.at(k)->cshade2;
				tsc2 = item->Ptext.at(k)->cscale;
				while ((ts2 == ts)
							&& (tsb2 == tsb)
							&& (tf2 == tf)
							&& (tc2 == tc)
							&& (te2 == te)
							&& (tsh2 == tsh)
							&& (tshs2 == tshs)
							&& (tsc2 == tsc)
							&& (tcs2 == tcs)
							&& (tst2 == tst))
				{
					if (item->Ptext.at(k)->ch == QChar(13))
						text += QChar(5);
					else if (item->Ptext.at(k)->ch == QChar(9))
						text += QChar(4);
					else
						text += item->Ptext.at(k)->ch;
					k++;
					if (k == item->Ptext.count())
						break;
					ts2 = item->Ptext.at(k)->csize / 10.0;
					tf2 = item->Ptext.at(k)->cfont;
					tc2 = item->Ptext.at(k)->ccolor;
					te2 = item->Ptext.at(k)->cextra;
					tsh2 = item->Ptext.at(k)->cshade;
					tst2 = item->Ptext.at(k)->cstyle;
					tsb2 = item->Ptext.at(k)->cab;
					tcs2 = item->Ptext.at(k)->cstroke;
					tshs2 = item->Ptext.at(k)->cshade2;
					tsc2 = item->Ptext.at(k)->cscale;
				}
				it.setAttribute("CH",text);
				it.setAttribute("CSIZE",ts);
				it.setAttribute("CFONT",tf);
				it.setAttribute("CCOLOR",tc);
				it.setAttribute("CEXTRA",te);
				it.setAttribute("CSHADE",tsh);
				it.setAttribute("CSTYLE",tst);
				it.setAttribute("CAB",tsb);
				it.setAttribute("CSTROKE",tcs);
				it.setAttribute("CSHADE2",tshs);
				it.setAttribute("CSCALE",tsc);
				k--;
				ob.appendChild(it);
			}
			if (item->BackBox != 0)
			{
				ob.setAttribute("BACKITEM", item->BackBox->ItemNr);
				ob.setAttribute("BACKPAGE", item->BackBox->OwnPage->PageNr);
			}
			else
			{
				ob.setAttribute("BACKITEM", -1);
				ob.setAttribute("BACKPAGE", -1);
			}
			if (item->NextBox != 0)
			{
				ob.setAttribute("NEXTITEM", item->NextBox->ItemNr);
				ob.setAttribute("NEXTPAGE", item->NextBox->OwnPage->PageNr);
			}
			else
			{
				ob.setAttribute("NEXTITEM", -1);
				ob.setAttribute("NEXTPAGE", -1);
			}
			ob.setAttribute("LAYER", item->LayerNr);
			pg.appendChild(ob);
		}
		dc.appendChild(pg);
	}
}

bool ScriXmlDoc::WriteDoc(QString fileName, ScribusDoc *doc, ScribusView *view, QProgressBar *dia2)
{
	QString text, tf, tf2, tc, tc2;
	QDomDocument docu("scribus");
	QString st="<SCRIBUSUTF8></SCRIBUSUTF8>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	elem.setAttribute("Version", "1.2.1cvs");
	QDomElement dc=docu.createElement("DOCUMENT");
	dc.setAttribute("ANZPAGES",view->DocPages.count());
	dc.setAttribute("PAGEWITH",doc->PageB);
	dc.setAttribute("PAGEHEIGHT",doc->PageH);
	dc.setAttribute("BORDERLEFT",doc->PageM.Left);
	dc.setAttribute("BORDERRIGHT",doc->PageM.Right);
	dc.setAttribute("BORDERTOP",doc->PageM.Top);
	dc.setAttribute("BORDERBOTTOM",doc->PageM.Bottom);
	dc.setAttribute("ORIENTATION",doc->PageOri);
	dc.setAttribute("PAGESIZE",doc->PageSize);
	dc.setAttribute("FIRSTNUM",doc->FirstPnum);
	if(doc->PageFP)
		dc.setAttribute("BOOK",1);
	if(doc->FirstPageLeft)
		dc.setAttribute("FIRSTLEFT",1);
	if(doc->PageAT)
		dc.setAttribute("AUTOTEXT",1);
	dc.setAttribute("AUTOSPALTEN",doc->PageSp);
	dc.setAttribute("ABSTSPALTEN",doc->PageSpa);
	dc.setAttribute("UNITS",doc->Einheit);
	dc.setAttribute("DFONT",doc->Dfont);
	dc.setAttribute("DSIZE",doc->Dsize / 10.0);
	dc.setAttribute("DCOL",doc->DCols);
	dc.setAttribute("DGAP",doc->DGap);
	dc.setAttribute("AUTHOR",doc->DocAutor);
	dc.setAttribute("COMMENTS",doc->DocComments);
	dc.setAttribute("KEYWORDS",doc->DocKeyWords);
	dc.setAttribute("PUBLISHER",doc->DocPublisher);
	dc.setAttribute("DOCDATE",doc->DocDate);
	dc.setAttribute("DOCTYPE",doc->DocType);
	dc.setAttribute("DOCFORMAT",doc->DocFormat);
	dc.setAttribute("DOCIDENT",doc->DocIdent);
	dc.setAttribute("DOCSOURCE",doc->DocSource);
	dc.setAttribute("DOCLANGINFO",doc->DocLangInfo);
	dc.setAttribute("DOCRELATION",doc->DocRelation);
	dc.setAttribute("DOCCOVER",doc->DocCover);
	dc.setAttribute("DOCRIGHTS",doc->DocRights);
	dc.setAttribute("DOCCONTRIB",doc->DocContrib);
	dc.setAttribute("TITLE",doc->DocTitel);
	dc.setAttribute("VHOCH",doc->VHoch);
	dc.setAttribute("VHOCHSC",doc->VHochSc);
	dc.setAttribute("VTIEF",doc->VTief);
	dc.setAttribute("VTIEFSC",doc->VTiefSc);
	dc.setAttribute("VKAPIT",doc->VKapit);
	dc.setAttribute("GROUPC",doc->GroupCounter);
	dc.setAttribute("HCMS", static_cast<int>(doc->HasCMS));
	dc.setAttribute("DPSo", static_cast<int>(doc->CMSSettings.SoftProofOn));
	dc.setAttribute("DPuse", static_cast<int>(doc->CMSSettings.CMSinUse));
	dc.setAttribute("DPgam", static_cast<int>(doc->CMSSettings.GamutCheck));
	dc.setAttribute("DPbla", static_cast<int>(doc->CMSSettings.BlackPoint));
	dc.setAttribute("DPMo",doc->CMSSettings.DefaultMonitorProfile);
	dc.setAttribute("DPPr",doc->CMSSettings.DefaultPrinterProfile);
	dc.setAttribute("DPIn",doc->CMSSettings.DefaultInputProfile);
	dc.setAttribute("DPIn2",doc->CMSSettings.DefaultInputProfile2);
	dc.setAttribute("DIPr",doc->CMSSettings.DefaultIntentPrinter);
	dc.setAttribute("DIMo",doc->CMSSettings.DefaultIntentMonitor);
	dc.setAttribute("DIMo2",doc->CMSSettings.DefaultIntentMonitor2);
	dc.setAttribute("ALAYER", doc->ActiveLayer);
	dc.setAttribute("LANGUAGE", doc->Language);
	dc.setAttribute("MINWORDLEN", doc->MinWordLen);
	dc.setAttribute("HYCOUNT", doc->HyCount);
	dc.setAttribute("AUTOMATIC", static_cast<int>(doc->Automatic));
	dc.setAttribute("AUTOCHECK", static_cast<int>(doc->AutoCheck));
	dc.setAttribute("GUIDELOCK", static_cast<int>(doc->GuideLock));
	dc.setAttribute("BASEGRID",doc->BaseGrid);
	dc.setAttribute("BASEO", doc->BaseOffs);
	dc.setAttribute("MINGRID", doc->minorGrid);
	dc.setAttribute("MAJGRID", doc->majorGrid);
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
	CListe::Iterator itc;
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		QDomElement co=docu.createElement("COLOR");
		co.setAttribute("NAME",itc.key());
		co.setAttribute("RGB",doc->PageColors[itc.key()].getRGBColor().name());
		co.setAttribute("CMYK",doc->PageColors[itc.key()].name());
		dc.appendChild(co);
	}
	if (doc->Vorlagen.count() > 5)
	{
		for (uint ff = 5; ff < doc->Vorlagen.count(); ++ff)
		{
			QDomElement fo=docu.createElement("STYLE");
			fo.setAttribute("NAME",doc->Vorlagen[ff].Vname);
			fo.setAttribute("ALIGN",doc->Vorlagen[ff].Ausri);
			fo.setAttribute("LINESP",doc->Vorlagen[ff].LineSpa);
			fo.setAttribute("INDENT",doc->Vorlagen[ff].Indent);
			fo.setAttribute("FIRST",doc->Vorlagen[ff].First);
			fo.setAttribute("VOR",doc->Vorlagen[ff].Avor);
			fo.setAttribute("NACH",doc->Vorlagen[ff].Anach);
			fo.setAttribute("FONT",doc->Vorlagen[ff].Font);
			fo.setAttribute("FONTSIZE",doc->Vorlagen[ff].FontSize / 10.0);
			fo.setAttribute("DROP", static_cast<int>(doc->Vorlagen[ff].Drop));
			fo.setAttribute("DROPLIN", doc->Vorlagen[ff].DropLin);
			fo.setAttribute("EFFECT", doc->Vorlagen[ff].FontEffect);
			fo.setAttribute("NUMTAB", static_cast<int>(doc->Vorlagen[ff].TabValues.count()));
			QString tlp = "";
			QString tmp = "";
			QValueList<double>::Iterator tax;
			for (tax = doc->Vorlagen[ff].TabValues.begin(); tax != doc->Vorlagen[ff].TabValues.end(); ++tax)
				tlp += tmp.setNum((*tax)) + " ";
			fo.setAttribute("TABS", tlp);
			fo.setAttribute("FCOLOR",doc->Vorlagen[ff].FColor);
			fo.setAttribute("FSHADE",doc->Vorlagen[ff].FShade);
			fo.setAttribute("SCOLOR",doc->Vorlagen[ff].SColor);
			fo.setAttribute("SSHADE",doc->Vorlagen[ff].SShade);
			fo.setAttribute("BASE", static_cast<int>(doc->Vorlagen[ff].BaseAdj));
			dc.appendChild(fo);
		}
	}
	for (uint lay = 0; lay < doc->Layers.count(); ++lay)
	{
		QDomElement la = docu.createElement("LAYERS");
		la.setAttribute("NUMMER",doc->Layers[lay].LNr);
		la.setAttribute("LEVEL",doc->Layers[lay].Level);
		la.setAttribute("NAME",doc->Layers[lay].Name);
		la.setAttribute("SICHTBAR", static_cast<int>(doc->Layers[lay].Sichtbar));
		la.setAttribute("DRUCKEN", static_cast<int>(doc->Layers[lay].Drucken));
		dc.appendChild(la);
	}
	QDomElement pdf = docu.createElement("PDF");
	pdf.setAttribute("Thumbnails", static_cast<int>(doc->PDF_Optionen.Thumbnails));
	pdf.setAttribute("Articles", static_cast<int>(doc->PDF_Optionen.Articles));
	pdf.setAttribute("Bookmarks", static_cast<int>(doc->PDF_Optionen.Bookmarks));
	pdf.setAttribute("Compress", static_cast<int>(doc->PDF_Optionen.Compress));
	pdf.setAttribute("CMethod", doc->PDF_Optionen.CompressMethod);
	pdf.setAttribute("Quality", doc->PDF_Optionen.Quality);
	pdf.setAttribute("MirrorH", static_cast<int>(doc->PDF_Optionen.MirrorH));
	pdf.setAttribute("MirrorV", static_cast<int>(doc->PDF_Optionen.MirrorV));
	pdf.setAttribute("RotateDeg", static_cast<int>(doc->PDF_Optionen.RotateDeg));
	pdf.setAttribute("PresentMode", static_cast<int>(doc->PDF_Optionen.PresentMode));
	pdf.setAttribute("RecalcPic", static_cast<int>(doc->PDF_Optionen.RecalcPic));
	pdf.setAttribute("RGBMode", static_cast<int>(doc->PDF_Optionen.UseRGB));
	pdf.setAttribute("UseProfiles", static_cast<int>(doc->PDF_Optionen.UseProfiles));
	pdf.setAttribute("UseProfiles2", static_cast<int>(doc->PDF_Optionen.UseProfiles2));
	pdf.setAttribute("Binding", doc->PDF_Optionen.Binding);
	pdf.setAttribute("PicRes", doc->PDF_Optionen.PicRes);
	pdf.setAttribute("Resolution", doc->PDF_Optionen.Resolution);
	pdf.setAttribute("Version", doc->PDF_Optionen.Version);
	pdf.setAttribute("Intent", doc->PDF_Optionen.Intent);
	pdf.setAttribute("Intent2", doc->PDF_Optionen.Intent2);
	pdf.setAttribute("SolidP", doc->PDF_Optionen.SolidProf);
	pdf.setAttribute("ImageP", doc->PDF_Optionen.ImageProf);
	pdf.setAttribute("PrintP", doc->PDF_Optionen.PrintProf);
	pdf.setAttribute("InfoString", doc->PDF_Optionen.Info);
	pdf.setAttribute("BTop", doc->PDF_Optionen.BleedTop);
	pdf.setAttribute("BLeft", doc->PDF_Optionen.BleedLeft);
	pdf.setAttribute("BRight", doc->PDF_Optionen.BleedRight);
	pdf.setAttribute("BBottom", doc->PDF_Optionen.BleedBottom);
	pdf.setAttribute("ImagePr", static_cast<int>(doc->PDF_Optionen.EmbeddedI));
	pdf.setAttribute("PassOwner", doc->PDF_Optionen.PassOwner);
	pdf.setAttribute("PassUser", doc->PDF_Optionen.PassUser);
	pdf.setAttribute("Permissions", doc->PDF_Optionen.Permissions);
	pdf.setAttribute("Encrypt", static_cast<int>(doc->PDF_Optionen.Encrypt));
	pdf.setAttribute("UseLpi", static_cast<int>(doc->PDF_Optionen.UseLPI));
	for (uint pdoF = 0; pdoF < doc->PDF_Optionen.EmbedList.count(); ++pdoF)
	{
		QDomElement pdf2 = docu.createElement("Fonts");
		pdf2.setAttribute("Name", doc->PDF_Optionen.EmbedList[pdoF]);
		pdf.appendChild(pdf2);
	}
	for (uint pdoS = 0; pdoS < doc->PDF_Optionen.SubsetList.count(); ++pdoS)
	{
		QDomElement pdf4 = docu.createElement("Subset");
		pdf4.setAttribute("Name", doc->PDF_Optionen.SubsetList[pdoS]);
		pdf.appendChild(pdf4);
	}
	for (uint pdoE = 0; pdoE < doc->PDF_Optionen.PresentVals.count(); ++pdoE)
	{
		QDomElement pdf3 = docu.createElement("Effekte");
		pdf3.setAttribute("EffektLen", doc->PDF_Optionen.PresentVals[pdoE].EffektLen);
		pdf3.setAttribute("AnzeigeLen", doc->PDF_Optionen.PresentVals[pdoE].AnzeigeLen);
		pdf3.setAttribute("Effekt", doc->PDF_Optionen.PresentVals[pdoE].Effekt);
		pdf3.setAttribute("Dm", doc->PDF_Optionen.PresentVals[pdoE].Dm);
		pdf3.setAttribute("M", doc->PDF_Optionen.PresentVals[pdoE].M);
		pdf3.setAttribute("Di", doc->PDF_Optionen.PresentVals[pdoE].Di);
		pdf.appendChild(pdf3);
	}
	QMap<QString,LPIset>::Iterator itlp;
	for (itlp = doc->PDF_Optionen.LPISettings.begin(); itlp != doc->PDF_Optionen.LPISettings.end(); ++itlp)
	{
		QDomElement pdf4 = docu.createElement("LPI");
		pdf4.setAttribute("Color", itlp.key());
		pdf4.setAttribute("Frequency", itlp.data().Frequency);
		pdf4.setAttribute("Angle", itlp.data().Angle);
		pdf4.setAttribute("SpotFunction", itlp.data().SpotFunc);
		pdf.appendChild(pdf4);
	}
	if (dia2 != 0)
	{
		dia2->setTotalSteps(view->DocPages.count()+view->MasterPages.count());
		dia2->setProgress(0);
	}
	dc.appendChild(pdf);
	if (doc->TemplateMode)
	{
		view->MasterPages = view->Pages;
		view->Pages = view->DocPages;
		WritePages(view, docu, dc, dia2, 0);
		view->Pages = view->MasterPages;
		WritePages(view, docu, dc, dia2, view->DocPages.count());
	}
	else
	{
		WritePages(view, docu, dc, dia2, 0);
		view->DocPages = view->Pages;
		view->Pages = view->MasterPages;
		WritePages(view, docu, dc, dia2, view->DocPages.count());
		view->Pages = view->DocPages;
	}
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

void ScriXmlDoc::WritePref(preV *Vor, QString ho)
{
	QDomDocument docu("scribusrc");
	QString st="<SCRIBUSRC></SCRIBUSRC>";
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	QDomElement dc=docu.createElement("GUI");
	dc.setAttribute("STILT",Vor->GUI);
	dc.setAttribute("RAD",Vor->Wheelval);
	dc.setAttribute("APF",Vor->AppFontSize);
	dc.setAttribute("GRAB",Vor->GrabRad);
	dc.setAttribute("UNIT",Vor->Einheit);
	dc.setAttribute("SBS", Vor->PagesSbS);
	dc.setAttribute("RCD", Vor->RecentDCount);
	dc.setAttribute("DOC", Vor->DocDir);
	dc.setAttribute("PROFILES", Vor->ProfileDir);
	dc.setAttribute("SCRIPTS", Vor->ScriptDir);
	dc.setAttribute("TEMPLATES", Vor->TemplateDir);
	dc.setAttribute("SHOWGUIDES", static_cast<int>(Vor->GuidesShown));
	dc.setAttribute("FRV", static_cast<int>(Vor->FramesShown));
	dc.setAttribute("SHOWMARGIN", static_cast<int>(Vor->MarginsShown));
	dc.setAttribute("SHOWBASE", static_cast<int>(Vor->BaseShown));
	dc.setAttribute("STECOLOR", Vor->STEcolor.name());
	dc.setAttribute("STEFONT", Vor->STEfont);
	elem.appendChild(dc);
	QDomElement dc1=docu.createElement("GRID");
	dc1.setAttribute("MINOR",Vor->DminGrid);
	dc1.setAttribute("MAJOR",Vor->DmajGrid);
	dc1.setAttribute("MINORC",Vor->DminColor.name());
	dc1.setAttribute("MAJORC",Vor->DmajColor.name());
	dc1.setAttribute("GuideC", Vor->guideColor.name());
	dc1.setAttribute("BaseC", Vor->baseColor.name());
	dc1.setAttribute("GuideZ", Vor->GuideRad);
	dc1.setAttribute("BACKG", static_cast<int>(Vor->Before));
	dc1.setAttribute("SHOW", static_cast<int>(Vor->GridShown));
	elem.appendChild(dc1);
	QDomElement dc1a=docu.createElement("PAGE");
	dc1a.setAttribute("PAGEC",Vor->DpapColor.name());
	dc1a.setAttribute("MARGC",Vor->DmargColor.name());
	dc1a.setAttribute("RANDF", static_cast<int>(Vor->RandFarbig));
	dc1a.setAttribute("TRANS", static_cast<int>(Vor->PDFTransparency));
	dc1a.setAttribute("DScale",Vor->DisScale);
	elem.appendChild(dc1a);
	QDomElement dc2=docu.createElement("FONTS");
	dc2.setAttribute("FACE",Vor->DefFont);
	dc2.setAttribute("SIZE",Vor->DefSize / 10.0);
	elem.appendChild(dc2);
	QDomElement dc3=docu.createElement("TYPO");
	dc3.setAttribute("TIEF",Vor->DVTief);
	dc3.setAttribute("TIEFSC",Vor->DVTiefSc);
	dc3.setAttribute("HOCH",Vor->DVHoch);
	dc3.setAttribute("HOCHSC",Vor->DVHochSc);
	dc3.setAttribute("SMCAPS",Vor->DVKapit);
	dc3.setAttribute("AUTOL", Vor->AutoLine);
	dc3.setAttribute("BASE", Vor->BaseGrid);
	dc3.setAttribute("BASEO", Vor->BaseOffs);
	elem.appendChild(dc3);
	QDomElement dc9=docu.createElement("TOOLS");
	dc9.setAttribute("PEN",Vor->Dpen);
	dc9.setAttribute("BRUSH",Vor->Dbrush);
	dc9.setAttribute("PENLINE",Vor->DpenLine);
	dc9.setAttribute("PENTEXT",Vor->DpenText);
	dc9.setAttribute("TEXTCOL",Vor->DCols);
	dc9.setAttribute("TEXTGAP",Vor->DGap);
	dc9.setAttribute("STIL",Vor->DLineArt);
	dc9.setAttribute("STILLINE",Vor->DLstyleLine);
	dc9.setAttribute("WIDTH",Vor->Dwidth);
	dc9.setAttribute("WIDTHLINE",Vor->DwidthLine);
	dc9.setAttribute("PENSHADE",Vor->Dshade2);
	dc9.setAttribute("LINESHADE",Vor->DshadeLine);
	dc9.setAttribute("BRUSHSHADE",Vor->Dshade);
	dc9.setAttribute("MAGMIN",Vor->MagMin);
	dc9.setAttribute("MAGMAX",Vor->MagMax);
	dc9.setAttribute("MAGSTEP",Vor->MagStep);
	dc9.setAttribute("CPICT",Vor->DbrushPict);
	dc9.setAttribute("PICTSHADE",Vor->ShadePict);
	dc9.setAttribute("PICTSCX",Vor->ScaleX);
	dc9.setAttribute("PICTSCY",Vor->ScaleY);
	dc9.setAttribute("POLYC", Vor->PolyC);
	dc9.setAttribute("POLYF", Vor->PolyF);
	dc9.setAttribute("POLYR", Vor->PolyR);
	dc9.setAttribute("POLYFD", Vor->PolyFd);
	dc9.setAttribute("POLYS", static_cast<int>(Vor->PolyS));
	dc9.setAttribute("PSCALE", static_cast<int>(Vor->ScaleType));
	dc9.setAttribute("PASPECT", static_cast<int>(Vor->AspectRatio));
	elem.appendChild(dc9);
	QDomElement dc4=docu.createElement("MAINWINDOW");
	dc4.setAttribute("XPOS",Vor->MainX);
	dc4.setAttribute("YPOS",Vor->MainY);
	dc4.setAttribute("WIDTH",Vor->MainW);
	dc4.setAttribute("HEIGHT",Vor->MainH);
	elem.appendChild(dc4);
	QDomElement dc5=docu.createElement("TOOLPALETTE");
	dc5.setAttribute("VISIBLE", static_cast<int>(Vor->Werkv));
	dc5.setAttribute("PDFVISIBLE", static_cast<int>(Vor->WerkvP));
	elem.appendChild(dc5);
	QDomElement dc7=docu.createElement("TREEPALETTE");
	dc7.setAttribute("VISIBLE", static_cast<int>(Vor->Tpalv));
	dc7.setAttribute("XPOS",Vor->Tpalx);
	dc7.setAttribute("YPOS",Vor->Tpaly);
	elem.appendChild(dc7);
	QDomElement dc72=docu.createElement("NODEPALETTE");
	dc72.setAttribute("XPOS",Vor->Npalx);
	dc72.setAttribute("YPOS",Vor->Npaly);
	elem.appendChild(dc72);
	QDomElement dc73=docu.createElement("SCRAPBOOK");
	dc73.setAttribute("VISIBLE", static_cast<int>(Vor->SCpalv));
	dc73.setAttribute("XPOS",Vor->SCpalx);
	dc73.setAttribute("YPOS",Vor->SCpaly);
	dc73.setAttribute("WIDTH",Vor->SCpalw);
	dc73.setAttribute("HEIGHT",Vor->SCpalh);
	dc73.setAttribute("PREVIEW",Vor->PSize);
	dc73.setAttribute("SAVE", static_cast<int>(Vor->SaveAtQ));
	elem.appendChild(dc73);
	QDomElement dc74=docu.createElement("LAYERPALETTE");
	dc74.setAttribute("VISIBLE", static_cast<int>(Vor->Lpalv));
	dc74.setAttribute("XPOS",Vor->Lpalx);
	dc74.setAttribute("YPOS",Vor->Lpaly);
	elem.appendChild(dc74);
	QDomElement dc75=docu.createElement("PAGEPALETTE");
	dc75.setAttribute("VISIBLE", static_cast<int>(Vor->Sepalv));
	dc75.setAttribute("XPOS",Vor->Sepalx);
	dc75.setAttribute("YPOS",Vor->Sepaly);
	dc75.setAttribute("THUMBS", static_cast<int>(Vor->SepalT));
	dc75.setAttribute("NAMES", static_cast<int>(Vor->SepalN));
	elem.appendChild(dc75);
	QDomElement dc76=docu.createElement("DOKUMENT");
	dc76.setAttribute("FORMATCODE",Vor->PageFormat);
	dc76.setAttribute("AUSRICHTUNG",Vor->Ausrichtung);
	dc76.setAttribute("BREITE",Vor->PageBreite);
	dc76.setAttribute("HOEHE",Vor->PageHoehe);
	dc76.setAttribute("RANDO",Vor->RandOben);
	dc76.setAttribute("RANDU",Vor->RandUnten);
	dc76.setAttribute("RANDL",Vor->RandLinks);
	dc76.setAttribute("RANDR",Vor->RandRechts);
	dc76.setAttribute("DOPPEL", static_cast<int>(Vor->DoppelSeiten));
	dc76.setAttribute("LINKS", static_cast<int>(Vor->ErsteLinks));
	dc76.setAttribute("AutoSave", static_cast<int>(Vor->AutoSave));
	dc76.setAttribute("AutoSaveTime", Vor->AutoSaveTime);
	elem.appendChild(dc76);
	QDomElement dc77=docu.createElement("BOOKPALETTE");
	dc77.setAttribute("VISIBLE", static_cast<int>(Vor->Bopalv));
	dc77.setAttribute("XPOS",Vor->Bopalx);
	dc77.setAttribute("YPOS",Vor->Bopaly);
	elem.appendChild(dc77);
	QDomElement dc78=docu.createElement("DISTS");
	dc78.setAttribute("VISIBLE", static_cast<int>(Vor->Mapalv));
	dc78.setAttribute("XPOS",Vor->Mapalx);
	dc78.setAttribute("YPOS",Vor->Mapaly);
	elem.appendChild(dc78);
	QDomElement dc8=docu.createElement("MEASUREMENTS");
	dc8.setAttribute("VISIBLE", static_cast<int>(Vor->Mpalv));
	dc8.setAttribute("XPOS",Vor->Mpalx);
	dc8.setAttribute("YPOS",Vor->Mpaly);
	elem.appendChild(dc8);
	QDomElement dc81=docu.createElement("CMS");
	dc81.setAttribute("DPSo", static_cast<int>(Vor->DCMSset.SoftProofOn));
	dc81.setAttribute("DPuse", static_cast<int>(Vor->DCMSset.CMSinUse));
	dc81.setAttribute("DPgam", static_cast<int>(Vor->DCMSset.GamutCheck));
	dc81.setAttribute("DPbla", static_cast<int>(Vor->DCMSset.BlackPoint));
	dc81.setAttribute("DPMo",Vor->DCMSset.DefaultMonitorProfile);
	dc81.setAttribute("DPPr",Vor->DCMSset.DefaultPrinterProfile);
	dc81.setAttribute("DPIn",Vor->DCMSset.DefaultInputProfile);
	dc81.setAttribute("DPIn2",Vor->DCMSset.DefaultInputProfile2);
	dc81.setAttribute("DIPr",Vor->DCMSset.DefaultIntentPrinter);
	dc81.setAttribute("DIMo",Vor->DCMSset.DefaultIntentMonitor);
	dc81.setAttribute("DIMo2",Vor->DCMSset.DefaultIntentMonitor2);
	elem.appendChild(dc81);
	QDomElement dc82=docu.createElement("PRINTER");
	dc82.setAttribute("NAME",Vor->PrinterName);
	dc82.setAttribute("FILE",Vor->PrinterFile);
	dc82.setAttribute("COMMAND",Vor->PrinterCommand);
	dc82.setAttribute("CLIPMARGIN", static_cast<int>(Vor->ClipMargin));
	dc82.setAttribute("GMODE", static_cast<int>(Vor->GCRMode));
	elem.appendChild(dc82);
	QDomElement dc8Pr=docu.createElement("PRINTPREVIEW");
	dc8Pr.setAttribute("Mode", static_cast<int>(Vor->PrPr_Mode));
	dc8Pr.setAttribute("GcrMode", static_cast<int>(Vor->Gcr_Mode));
	dc8Pr.setAttribute("AlphaText", static_cast<int>(Vor->PrPr_AlphaText));
	dc8Pr.setAttribute("AlphaGraphics", static_cast<int>(Vor->PrPr_AlphaGraphics));
	dc8Pr.setAttribute("Transparency", static_cast<int>(Vor->PrPr_Transparency));
	dc8Pr.setAttribute("Cyan", static_cast<int>(Vor->PrPr_C));
	dc8Pr.setAttribute("Magenta", static_cast<int>(Vor->PrPr_M));
	dc8Pr.setAttribute("Yellow", static_cast<int>(Vor->PrPr_Y));
	dc8Pr.setAttribute("Black", static_cast<int>(Vor->PrPr_K));
	elem.appendChild(dc8Pr);
	QDomElement dc8Ex = docu.createElement("EXTERNAL");
	dc8Ex.setAttribute("GIMP", Vor->gimp_exe);
	dc8Ex.setAttribute("GS", Vor->gs_exe);
	dc8Ex.setAttribute("AlphaGraphics", static_cast<int>(Vor->gs_antiGraph));
	dc8Ex.setAttribute("AlphaText", static_cast<int>(Vor->gs_antiText));
	elem.appendChild(dc8Ex);
	QDomElement rde=docu.createElement("HYPHEN");
	rde.setAttribute("LANG", Vor->Language);
	rde.setAttribute("WORDLEN", Vor->MinWordLen);
	rde.setAttribute("HYCOUNT", Vor->HyCount);
	rde.setAttribute("MODE", static_cast<int>(Vor->Automatic));
	rde.setAttribute("INMODE", static_cast<int>(Vor->AutoCheck));
	elem.appendChild(rde);
	CListe::Iterator itc;
	for (itc = Vor->DColors.begin(); itc != Vor->DColors.end(); ++itc)
	{
		QDomElement co=docu.createElement("COLOR");
		co.setAttribute("NAME",itc.key());
		co.setAttribute("RGB",Vor->DColors[itc.key()].getRGBColor().name());
		co.setAttribute("CMYK",Vor->DColors[itc.key()].name());
		elem.appendChild(co);
	}
	for ( SCFontsIterator itf(Vor->AvailFonts); itf.current(); ++itf)
	{
		QDomElement fn=docu.createElement("FONT");
		fn.setAttribute("NAME",itf.currentKey());
		fn.setAttribute("EMBED",static_cast<int>(itf.current()->EmbedPS));
		fn.setAttribute("USE", static_cast<int>(itf.current()->UseFont));
		fn.setAttribute("SUBSET", static_cast<int>(itf.current()->Subset));
		elem.appendChild(fn);
	}
	for (uint rd=0; rd<Vor->RecentDocs.count(); ++rd)
	{
		QDomElement rde=docu.createElement("RECENT");
		rde.setAttribute("NAME",Vor->RecentDocs[rd]);
		elem.appendChild(rde);
	}
	for (uint ksc=0; ksc<68; ++ksc)
	{
		QDomElement kscc=docu.createElement("SHORTCUT");
		kscc.setAttribute("CODE",Vor->KeyActions[ksc].KeyID);
		kscc.setAttribute("NR", ksc);
		elem.appendChild(kscc);
	}
	QMap<QString,QString>::Iterator itfsu;
	for (itfsu = Vor->GFontSub.begin(); itfsu != Vor->GFontSub.end(); ++itfsu)
	{
		QDomElement fosu = docu.createElement("Substitute");
		fosu.setAttribute("Name",itfsu.key());
		fosu.setAttribute("Replace",itfsu.data());
		elem.appendChild(fosu);
	}
	for (uint ccs=0; ccs<Vor->CustomColorSets.count(); ++ccs)
	{
		QDomElement cos=docu.createElement("COLORSET");
		cos.setAttribute("NAME",Vor->CustomColorSets[ccs]);
		elem.appendChild(cos);
	}
	QDomElement cosd=docu.createElement("DCOLORSET");
	cosd.setAttribute("NAME",Vor->DColorSet);
	elem.appendChild(cosd);
	QFile f(ho);
	if(!f.open(IO_WriteOnly))
		return;
	QTextStream s(&f);
	s<<docu.toCString();
	f.close();
}

bool ScriXmlDoc::ReadPref(struct preV *Vorein, QString ho)
{
	QDomDocument docu("scridoc");
	QFile f(ho);
	if(!f.open(IO_ReadOnly))
		return false;
	if(!docu.setContent(&f))
	{
		f.close();
		return false;
	}
	f.close();
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "SCRIBUSRC")
		return false;
	Vorein->DColors.clear();
	CMYKColor lf = CMYKColor();
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		if (dc.tagName()=="GUI")
		{
			Vorein->GUI = dc.attribute("STILT","Default");
			Vorein->Wheelval = QStoInt(dc.attribute("RAD"));
			Vorein->GrabRad = QStoInt(dc.attribute("GRAB","4"));
			Vorein->Einheit = QStoInt(dc.attribute("UNIT","0"));
			Vorein->PagesSbS = QStoInt(dc.attribute("SBS","1"));
			Vorein->AppFontSize = QStoInt(dc.attribute("APF","12"));
			Vorein->RecentDCount = dc.attribute("RCD","5").toUInt();
			Vorein->DocDir = dc.attribute("DOC","");
			Vorein->ProfileDir = dc.attribute("PROFILES","");
			Vorein->ScriptDir = dc.attribute("SCRIPTS","");
			Vorein->TemplateDir = dc.attribute("TEMPLATES","");
			Vorein->GuidesShown = static_cast<bool>(QStoInt(dc.attribute("SHOWGUIDES","1")));
			Vorein->FramesShown = static_cast<bool>(QStoInt(dc.attribute("FRV","1")));
			Vorein->MarginsShown = static_cast<bool>(QStoInt(dc.attribute("SHOWMARGIN","1")));
			Vorein->BaseShown = static_cast<bool>(QStoInt(dc.attribute("SHOWBASE","1")));
			if (dc.hasAttribute("STECOLOR"))
				Vorein->STEcolor = QColor(dc.attribute("STECOLOR"));
			if (dc.hasAttribute("STEFONT"))
				Vorein->STEfont = dc.attribute("STEFONT");
		}
		if (dc.tagName()=="GRID")
		{
			Vorein->DminGrid = QStodouble(dc.attribute("MINOR"));
			Vorein->DmajGrid = QStodouble(dc.attribute("MAJOR"));
			Vorein->DminColor = QColor(dc.attribute("MINORC"));
			Vorein->DmajColor = QColor(dc.attribute("MAJORC"));
			Vorein->Before = static_cast<bool>(QStoInt(dc.attribute("BACKG","1")));
			Vorein->GridShown = static_cast<bool>(QStoInt(dc.attribute("SHOW","0")));
			if (dc.hasAttribute("GuideC"))
				Vorein->guideColor = QColor(dc.attribute("GuideC"));
			if (dc.hasAttribute("GuideZ"))
				Vorein->GuideRad = QStodouble(dc.attribute("GuideZ"));
			if (dc.hasAttribute("BaseC"))
				Vorein->baseColor = QColor(dc.attribute("BaseC"));
		}
		if (dc.tagName()=="PAGE")
		{
			Vorein->DpapColor = QColor(dc.attribute("PAGEC"));
			Vorein->DmargColor = QColor(dc.attribute("MARGC","#0000ff"));
			Vorein->RandFarbig = static_cast<bool>(QStoInt(dc.attribute("RANDF","0")));
			Vorein->PDFTransparency = static_cast<bool>(QStoInt(dc.attribute("TRANS","0")));
			Vorein->DisScale = QStodouble(dc.attribute("DScale","1"));
		}
		if (dc.tagName()=="TYPO")
		{
			Vorein->DVTief = QStoInt(dc.attribute("TIEF"));
			Vorein->DVTiefSc = QStoInt(dc.attribute("TIEFSC"));
			Vorein->DVHoch = QStoInt(dc.attribute("HOCH"));
			Vorein->DVHochSc = QStoInt(dc.attribute("HOCHSC"));
			Vorein->DVKapit = QStoInt(dc.attribute("SMCAPS"));
			Vorein->AutoLine = QStoInt(dc.attribute("AUTOL","20"));
			Vorein->BaseGrid = QStodouble(dc.attribute("BASE","12"));
			Vorein->BaseOffs = QStodouble(dc.attribute("BASEO","0"));
		}
		if (dc.tagName()=="TOOLS")
		{
			Vorein->Dpen = dc.attribute("PEN");
			Vorein->Dbrush = dc.attribute("BRUSH");
			Vorein->DpenLine = dc.attribute("PENLINE");
			Vorein->DpenText = dc.attribute("PENTEXT");
			Vorein->DCols = QStoInt(dc.attribute("TEXTCOL", "1"));
			Vorein->DGap = QStodouble(dc.attribute("TEXTGAP", "0.0"));
			Vorein->DLineArt = QStoInt(dc.attribute("STIL"));
			Vorein->DLstyleLine = QStoInt(dc.attribute("STILLINE"));
			Vorein->Dwidth = QStodouble(dc.attribute("WIDTH"));
			Vorein->DwidthLine = QStodouble(dc.attribute("WIDTHLINE"));
			Vorein->Dshade2 = QStoInt(dc.attribute("PENSHADE"));
			Vorein->DshadeLine = QStoInt(dc.attribute("LINESHADE"));
			Vorein->Dshade = QStoInt(dc.attribute("BRUSHSHADE"));
			Vorein->MagMin = QStoInt(dc.attribute("MAGMIN","10"));
			Vorein->MagMax = QStoInt(dc.attribute("MAGMAX","800"));
			Vorein->MagStep = QStoInt(dc.attribute("MAGSTEP","25"));
			Vorein->DbrushPict = dc.attribute("CPICT");
			Vorein->ShadePict = QStoInt(dc.attribute("PICTSHADE","100"));
			Vorein->ScaleX = QStodouble(dc.attribute("PICTSCX","1"));
			Vorein->ScaleY = QStodouble(dc.attribute("PICTSCY","1"));
			Vorein->ScaleType = static_cast<bool>(QStoInt(dc.attribute("PSCALE", "1")));
			Vorein->AspectRatio = static_cast<bool>(QStoInt(dc.attribute("PASPECT", "0")));
			Vorein->PolyC = QStoInt(dc.attribute("POLYC", "4"));
			Vorein->PolyF = QStodouble(dc.attribute("POLYF", "0.5"));
			Vorein->PolyR = QStodouble(dc.attribute("POLYR", "0"));
			Vorein->PolyFd = QStoInt(dc.attribute("POLYFD", "0"));
			Vorein->PolyS = static_cast<bool>(QStoInt(dc.attribute("POLYS", "0")));
		}
		if (dc.tagName()=="MAINWINDOW")
		{
			Vorein->MainX = QStoInt(dc.attribute("XPOS"));
			Vorein->MainY = QStoInt(dc.attribute("YPOS"));
			Vorein->MainW = QStoInt(dc.attribute("WIDTH"));
			Vorein->MainH = QStoInt(dc.attribute("HEIGHT"));
		}
		if (dc.tagName()=="TOOLPALETTE")
		{
			Vorein->Werkv = static_cast<bool>(QStoInt(dc.attribute("VISIBLE","1")));
			Vorein->WerkvP = static_cast<bool>(QStoInt(dc.attribute("PDFVISIBLE","1")));
		}
		if (dc.tagName()=="TREEPALETTE")
		{
			Vorein->Tpalv = static_cast<bool>(QStoInt(dc.attribute("VISIBLE")));
			Vorein->Tpalx = QStoInt(dc.attribute("XPOS"));
			Vorein->Tpaly = QStoInt(dc.attribute("YPOS"));
		}
		if (dc.tagName()=="LAYERPALETTE")
		{
			Vorein->Lpalv = static_cast<bool>(QStoInt(dc.attribute("VISIBLE")));
			Vorein->Lpalx = QStoInt(dc.attribute("XPOS"));
			Vorein->Lpaly = QStoInt(dc.attribute("YPOS"));
		}
		if (dc.tagName()=="BOOKPALETTE")
		{
			Vorein->Bopalv = static_cast<bool>(QStoInt(dc.attribute("VISIBLE")));
			Vorein->Bopalx = QStoInt(dc.attribute("XPOS"));
			Vorein->Bopaly = QStoInt(dc.attribute("YPOS"));
		}
		if (dc.tagName()=="PAGEPALETTE")
		{
			Vorein->Sepalv = static_cast<bool>(QStoInt(dc.attribute("VISIBLE")));
			Vorein->Sepalx = QStoInt(dc.attribute("XPOS"));
			Vorein->Sepaly = QStoInt(dc.attribute("YPOS"));
			Vorein->SepalT = static_cast<bool>(QStoInt(dc.attribute("THUMBS")));
			Vorein->SepalN = static_cast<bool>(QStoInt(dc.attribute("NAMES")));
		}
		if (dc.tagName()=="SCRAPBOOK")
		{
			Vorein->SCpalv = static_cast<bool>(QStoInt(dc.attribute("VISIBLE")));
			Vorein->SCpalx = QStoInt(dc.attribute("XPOS"));
			Vorein->SCpaly = QStoInt(dc.attribute("YPOS"));
			Vorein->SCpalw = QStoInt(dc.attribute("WIDTH"));
			Vorein->SCpalh = QStoInt(dc.attribute("HEIGHT"));
			Vorein->PSize = QStoInt(dc.attribute("PREVIEW"));
			Vorein->SaveAtQ = static_cast<bool>(QStoInt(dc.attribute("SAVE")));
		}
		if (dc.tagName() == "DOKUMENT")
		{
			Vorein->PageFormat = QStoInt(dc.attribute("FORMATCODE","4"));
			Vorein->Ausrichtung = QStoInt(dc.attribute("AUSRICHTUNG","0"));
			Vorein->PageBreite = QStodouble(dc.attribute("BREITE","595"));
			Vorein->PageHoehe = QStodouble(dc.attribute("HOEHE","842"));
			Vorein->RandOben = QStodouble(dc.attribute("RANDO","9"));
			Vorein->RandUnten = QStodouble(dc.attribute("RANDU","40"));
			Vorein->RandLinks = QStodouble(dc.attribute("RANDL","9"));
			Vorein->RandRechts = QStodouble(dc.attribute("RANDR","9"));
			Vorein->DoppelSeiten = static_cast<bool>(QStoInt(dc.attribute("DOPPEL","0")));
			Vorein->ErsteLinks = static_cast<bool>(QStoInt(dc.attribute("LINKS","0")));
			Vorein->AutoSave = static_cast<bool>(QStoInt(dc.attribute("AutoSave","0")));
			Vorein->AutoSaveTime = QStoInt(dc.attribute("AutoSaveTime","600000"));
		}
		if (dc.tagName()=="CMS")
		{
			Vorein->DCMSset.SoftProofOn = static_cast<bool>(QStoInt(dc.attribute("DPSo","0")));
			Vorein->DCMSset.CMSinUse = static_cast<bool>(QStoInt(dc.attribute("DPuse","0")));
			Vorein->DCMSset.GamutCheck = static_cast<bool>(QStoInt(dc.attribute("DPgam","0")));
			Vorein->DCMSset.BlackPoint = static_cast<bool>(QStoInt(dc.attribute("DPbla","1")));
			Vorein->DCMSset.DefaultMonitorProfile = dc.attribute("DPMo","");
			Vorein->DCMSset.DefaultPrinterProfile = dc.attribute("DPPr","");
			Vorein->DCMSset.DefaultInputProfile = dc.attribute("DPIn","");
			Vorein->DCMSset.DefaultInputProfile2 = dc.attribute("DPIn2","");
			Vorein->DCMSset.DefaultIntentPrinter = QStoInt(dc.attribute("DIPr","0"));
			Vorein->DCMSset.DefaultIntentMonitor = QStoInt(dc.attribute("DIMo","3"));
			Vorein->DCMSset.DefaultIntentMonitor2 = QStoInt(dc.attribute("DIMo2","3"));
		}
		if (dc.tagName()=="NODEPALETTE")
		{
			Vorein->Npalx = QStoInt(dc.attribute("XPOS"));
			Vorein->Npaly = QStoInt(dc.attribute("YPOS"));
		}
		if (dc.tagName()=="SHORTCUT")
			Vorein->KeyActions[QStoInt(dc.attribute("NR"))].KeyID = QStoInt(dc.attribute("CODE"));
		if (dc.tagName()=="RECENT")
			Vorein->RecentDocs.append(dc.attribute("NAME"));
		if (dc.tagName()=="DISTS")
		{
			Vorein->Mapalv = static_cast<bool>(QStoInt(dc.attribute("VISIBLE", "1")));
			Vorein->Mapalx = QStoInt(dc.attribute("XPOS", "0"));
			Vorein->Mapaly = QStoInt(dc.attribute("YPOS", "0"));
		}
		if (dc.tagName()=="MEASUREMENTS")
		{
			Vorein->Mpalv = static_cast<bool>(QStoInt(dc.attribute("VISIBLE", "1")));
			Vorein->Mpalx = QStoInt(dc.attribute("XPOS"));
			Vorein->Mpaly = QStoInt(dc.attribute("YPOS"));
		}
		if (dc.tagName()=="PRINTER")
		{
			Vorein->PrinterName = dc.attribute("NAME");
			Vorein->PrinterFile = dc.attribute("FILE");
			Vorein->PrinterCommand = dc.attribute("COMMAND");
			Vorein->ClipMargin = static_cast<bool>(QStoInt(dc.attribute("CLIPMARGIN", "1")));
			Vorein->GCRMode = static_cast<bool>(QStoInt(dc.attribute("GMODE", "1")));
		}
		if (dc.tagName()=="PRINTPREVIEW")
		{
			Vorein->PrPr_Mode = static_cast<bool>(QStoInt(dc.attribute("Mode", "0")));
			Vorein->Gcr_Mode = static_cast<bool>(QStoInt(dc.attribute("GcrMode", "1")));
			Vorein->PrPr_AlphaText = static_cast<bool>(QStoInt(dc.attribute("AlphaText", "0")));
			Vorein->PrPr_AlphaGraphics = static_cast<bool>(QStoInt(dc.attribute("AlphaGraphics", "0")));
			Vorein->PrPr_Transparency = static_cast<bool>(QStoInt(dc.attribute("Transparency", "0")));
			Vorein->PrPr_C = static_cast<bool>(QStoInt(dc.attribute("Cyan", "1")));
			Vorein->PrPr_M = static_cast<bool>(QStoInt(dc.attribute("Magenta", "1")));
			Vorein->PrPr_Y = static_cast<bool>(QStoInt(dc.attribute("Yellow", "1")));
			Vorein->PrPr_K = static_cast<bool>(QStoInt(dc.attribute("Black", "1")));
		}
		if (dc.tagName()=="EXTERNAL")
		{
			Vorein->gs_exe = dc.attribute("GS", "gs");
			Vorein->gs_antiText = static_cast<bool>(QStoInt(dc.attribute("AlphaText", "0")));
			Vorein->gs_antiGraph = static_cast<bool>(QStoInt(dc.attribute("AlphaGraphics", "0")));
			Vorein->gimp_exe = dc.attribute("GIMP", "gimp");
		}
		if (dc.tagName()=="HYPHEN")
		{
			if (dc.attribute("LANG", "") != "")
				Vorein->Language = dc.attribute("LANG");
			Vorein->MinWordLen = QStoInt(dc.attribute("WORDLEN", "3"));
			Vorein->HyCount = QStoInt(dc.attribute("HYCOUNT", "2"));
			Vorein->Automatic = static_cast<bool>(QStoInt(dc.attribute("MODE", "1")));
			Vorein->AutoCheck = static_cast<bool>(QStoInt(dc.attribute("INMODE", "1")));
		}
		if (dc.tagName()=="FONTS")
		{
			QString tmpf = dc.attribute("FACE");
			if (!Vorein->AvailFonts.find(tmpf))
			{
				DmF *dia = new DmF(0, tmpf, Vorein);
				dia->exec();
				Vorein->DefFont = dia->Ersatz;
				delete dia;
			}
			else
				Vorein->DefFont = dc.attribute("FACE");
			Vorein->DefSize = qRound(QStodouble(dc.attribute("SIZE")) * 10.0);
		}
		if (dc.tagName()=="FONT")
		{
			if (Vorein->AvailFonts.find(dc.attribute("NAME")))
			{
				Vorein->AvailFonts[dc.attribute("NAME")]->EmbedPS =
												 static_cast<bool>(QStoInt(dc.attribute("EMBED")));
				Vorein->AvailFonts[dc.attribute("NAME")]->UseFont &=
												 static_cast<bool>(QStoInt(dc.attribute("USE","1")));
				Vorein->AvailFonts[dc.attribute("NAME")]->Subset =
												 static_cast<bool>(QStoInt(dc.attribute("SUBSET","0")));
			}
		}
		if (dc.tagName()=="COLOR")
		{
			if (dc.hasAttribute("CMYK"))
				lf.setNamedColor(dc.attribute("CMYK"));
			else
				lf.fromQColor(QColor(dc.attribute("RGB")));
		  Vorein->DColors[dc.attribute("NAME")] = lf;
		}
		if (dc.tagName()=="Substitute")
		  Vorein->GFontSub[dc.attribute("Name")] = dc.attribute("Replace");
		if (dc.tagName()=="COLORSET")
			Vorein->CustomColorSets.append(dc.attribute("NAME"));
		if (dc.tagName()=="DCOLORSET")
			Vorein->DColorSet = dc.attribute("NAME");
		DOC=DOC.nextSibling();
	}
	if (Vorein->GUI != "Default")
		qApp->setStyle(QStyleFactory::create(Vorein->GUI));
	QFont apf = qApp->font();
	apf.setPointSize(Vorein->AppFontSize);
	qApp->setFont(apf,true);
	return true;
}

QColor ScriXmlDoc::SetFarbe(ScribusDoc *doc, QString farbe, int shad)
{
	int h, s, v, sneu;
	QColor tmp;
	doc->PageColors[farbe].getRGBColor().rgb(&h, &s, &v);
	if ((h == s) && (s == v))
	{
		doc->PageColors[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = 255 - ((255 - v) * shad / 100);
		tmp.setHsv(h, s, sneu);
	}
	else
	{
		doc->PageColors[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = s * shad / 100;
		tmp.setHsv(h, sneu, v);
	}
	return tmp;
}
