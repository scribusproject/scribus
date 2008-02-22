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

#include <QtAlgorithms>
#include <QApplication>
#include <QByteArray>
#include <QCursor>
#include <QDir>
#include <QFile>
#include <QRegExp>
#include <QTextCodec>
#include <QTextStream>
#include <QXmlSimpleReader>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

#include <cstdlib>
#include <cmath>
#include "missing.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_latexframe.h"
#include "units.h"
#include "prefsmanager.h"
#include "scribusview.h"
#include "commonstrings.h"
#include "text/nlsconfig.h"

#include <iostream>

#include "scconfig.h"

#include <zlib.h>
#include "util.h"
#include "util_math.h"
#include "util_color.h"
#include "scpattern.h"

using namespace std;

ScriXmlDoc::ScriXmlDoc()
{
	prefsManager=PrefsManager::instance();
}

bool ScriXmlDoc::attrAsBool(const QXmlStreamAttributes& attrs, const char* attName, bool defVal)
{
	QLatin1String attString(attName);
	bool value = defVal;
	QStringRef att = attrs.value(attString);
	if (!att.isEmpty())
		value = static_cast<bool>(att.toString().toInt());
	return value;
}

int ScriXmlDoc::attrAsInt(const QXmlStreamAttributes& attrs, const char* attName, int defVal)
{
	QLatin1String attString(attName);
	int value = defVal;
	QStringRef att = attrs.value(attString);
	if (!att.isEmpty())
		value = att.toString().toInt();
	return value;
}

double ScriXmlDoc::attrAsDbl(const QXmlStreamAttributes& attrs, const char* attName, double defVal)
{
	QLatin1String attString(attName);
	double value = defVal;
	QStringRef att = attrs.value(attString);
	if (!att.isEmpty())
		value = att.toString().toDouble();
	return value;
}

QString ScriXmlDoc::attrAsString (const QXmlStreamAttributes& attrs, const char* attName, const QString& defVal)
{
	QLatin1String attString(attName);
	QStringRef att = attrs.value(attString);
	if (!att.isNull())
		return att.toString();
	return defVal;
}

void ScriXmlDoc::GetItemProps(const QXmlStreamAttributes& attrs, struct CopyPasteBuffer *OB, const QString& baseDir, bool newVersion)
{
	QString tmp;
	int x, y;
	double xf, yf, xf2;
	OB->PType   = static_cast<PageItem::ItemType>(attrAsInt(attrs, "PTYPE"));
	OB->Width   = attrAsDbl(attrs, "WIDTH", 0.0);
	OB->Height  = attrAsDbl(attrs, "HEIGHT", 0.);
	OB->RadRect = attrAsDbl(attrs, "RADRECT", 0.0);
	OB->ClipEdited = attrAsInt(attrs, "CLIPEDIT", 0);
	OB->FrameType  = attrAsInt(attrs, "FRTYPE", 0);
	OB->Pwidth     = attrAsDbl(attrs, "PWIDTH");
	OB->Pcolor     = attrAsString(attrs, "PCOLOR", CommonStrings::None);
	if ((!newVersion) && (OB->PType == 4))
	{
		OB->TxtFill = attrAsString(attrs, "PCOLOR2", CommonStrings::None);
		OB->Pcolor2 = CommonStrings::None;
	}
	else
	{
		OB->Pcolor2 = attrAsString(attrs, "PCOLOR2", CommonStrings::None);
		OB->TxtFill = attrAsString(attrs, "TXTFILL", "Black");
	}
	OB->Shade      = attrAsInt(attrs, "SHADE");
	OB->Shade2     = attrAsInt(attrs, "SHADE2");
	OB->FillRule   = attrAsInt(attrs, "fillRule", 1);
	OB->TxtStroke  = attrAsString(attrs, "TXTSTROKE", CommonStrings::None);
	OB->ShTxtFill  = attrAsInt(attrs, "TXTFILLSH", 100);
	OB->ShTxtStroke= attrAsInt(attrs, "TXTSTRSH", 100);
	OB->TxtScale   = qRound(attrAsDbl(attrs, "TXTSCALE", 100.0) * 10);
	OB->TxtScaleV  = qRound(attrAsDbl(attrs, "TXTSCALEV", 100.0) * 10);
	OB->TxTBase    = qRound(attrAsDbl(attrs, "TXTBASE", 0) * 10);
	OB->TxTStyle   = attrAsInt(attrs, "TXTSTYLE", 0);
	OB->TxtShadowX = qRound(attrAsDbl(attrs, "TXTSHX", 5.0) * 10);
	OB->TxtShadowY = qRound(attrAsDbl(attrs, "TXTSHY", -5.0)* 10);
	OB->TxtOutline = qRound(attrAsDbl(attrs, "TXTOUT", 1.0) * 10);
	OB->TxtUnderPos   = qRound(attrAsDbl(attrs, "TXTULP", -0.1) * 10);
	OB->TxtUnderWidth = qRound(attrAsDbl(attrs, "TXTULW", -0.1) * 10);
	OB->TxtStrikePos  = qRound(attrAsDbl(attrs, "TXTSTP", -0.1) * 10);
	OB->TxtStrikeWidth= qRound(attrAsDbl(attrs, "TXTSTW", -0.1) * 10);
	OB->Cols   = attrAsInt(attrs, "COLUMNS", 1);
	OB->ColGap = attrAsDbl(attrs, "COLGAP" , 0.0);
	OB->GrType = attrAsInt(attrs, "GRTYP"  , 0);
	OB->fill_gradient.clearStops();
	if (OB->GrType != 0)
	{
		if (OB->GrType == 8)
		{
			OB->pattern = attrAsString(attrs, "pattern", "");
			OB->patternScaleX   = attrAsDbl(attrs, "pScaleX", 100.0);
			OB->patternScaleY   = attrAsDbl(attrs, "pScaleY", 100.0);
			OB->patternOffsetX  = attrAsDbl(attrs, "pOffsetX", 0.0);
			OB->patternOffsetY  = attrAsDbl(attrs, "pOffsetY", 0.0);
			OB->patternRotation = attrAsDbl(attrs, "pRotation", 0.0);
		}
		else
		{
			OB->GrStartX = attrAsDbl(attrs, "GRSTARTX", 0.0);
			OB->GrStartY = attrAsDbl(attrs, "GRSTARTY", 0.0);
			OB->GrEndX   = attrAsDbl(attrs, "GRENDX", 0.0);
			OB->GrEndY   = attrAsDbl(attrs, "GRENDY", 0.0);
			OB->GrColor  = attrAsString(attrs, "GRCOLOR", "Black");
			if (OB->GrColor.isEmpty())
				OB->GrColor = "Black";
			OB->GrColor2 = attrAsString(attrs, "GRCOLOR2","Black");
			if (OB->GrColor2.isEmpty())
				OB->GrColor2 = "Black";
			OB->GrShade  = attrAsInt(attrs, "GRSHADE", 100);
			OB->GrShade2 = attrAsInt(attrs, "GRSHADE2", 100);
		}
	}
	OB->Rot        = attrAsDbl(attrs, "ROT", 0.0);
	OB->PLineArt   = Qt::PenStyle    ( attrAsInt(attrs, "PLINEART", 0) );
	OB->PLineEnd   = Qt::PenCapStyle ( attrAsInt(attrs, "PLINEEND", 0) );
	OB->PLineJoin  = Qt::PenJoinStyle( attrAsInt(attrs, "PLINEJOIN", 0));
	OB->LineSp     = attrAsDbl(attrs, "LINESP");
	OB->LineSpMode = attrAsInt(attrs, "LINESPMode", 0);
	OB->LocalScX   = attrAsDbl(attrs, "LOCALSCX");
	OB->LocalScY   = attrAsDbl(attrs, "LOCALSCY");
	OB->LocalX     = attrAsDbl(attrs, "LOCALX");
	OB->LocalY     = attrAsDbl(attrs, "LOCALY");
	OB->PicArt     = attrAsInt(attrs, "PICART");
	OB->flippedH   = attrAsInt(attrs, "FLIPPEDH") % 2;
	OB->flippedV   = attrAsInt(attrs, "FLIPPEDV") % 2;
/*	OB->BBoxX=obj->attribute("BBOXX").toDouble();
	OB->BBoxH=obj->attribute("BBOXH").toDouble(); */
	OB->ScaleType  = attrAsInt(attrs, "SCALETYPE", 1);
	OB->AspectRatio= attrAsInt(attrs, "RATIO", 0);
	OB->isPrintable= attrAsInt(attrs, "PRINTABLE");
	OB->m_isAnnotation = attrAsInt(attrs, "ANNOTATION", 0);
	OB->m_annotation.setType  (attrAsInt(attrs, "ANTYPE", 0));
	OB->m_annotation.setAction(attrAsString(attrs, "ANACTION",""));
	OB->m_annotation.setE_act (attrAsString(attrs, "ANEACT",""));
	OB->m_annotation.setX_act (attrAsString(attrs, "ANXACT",""));
	OB->m_annotation.setD_act (attrAsString(attrs, "ANDACT",""));
	OB->m_annotation.setFo_act(attrAsString(attrs, "ANFOACT",""));
	OB->m_annotation.setBl_act(attrAsString(attrs, "ANBLACT",""));
	OB->m_annotation.setK_act (attrAsString(attrs, "ANKACT",""));
	OB->m_annotation.setF_act (attrAsString(attrs, "ANFACT",""));
	OB->m_annotation.setV_act (attrAsString(attrs, "ANVACT",""));
	OB->m_annotation.setC_act (attrAsString(attrs, "ANCACT",""));
	OB->m_annotation.setActionType(attrAsInt(attrs, "ANACTYP", 0));
	OB->m_annotation.setExtern    (attrAsString(attrs, "ANEXTERN",""));
	if ((!OB->m_annotation.Extern().isEmpty()) && (OB->m_annotation.ActionType() != 8))
	{
		QFileInfo efp(OB->m_annotation.Extern());
		OB->m_annotation.setExtern(efp.absoluteFilePath());
	}
	OB->m_annotation.setZiel (attrAsInt(attrs, "ANZIEL", 0));
	OB->AnName = attrAsString(attrs, "ANNAME","");
	OB->m_annotation.setToolTip (attrAsString(attrs, "ANTOOLTIP",""));
	OB->m_annotation.setRollOver(attrAsString(attrs, "ANROLL",""));
	OB->m_annotation.setDown  (attrAsString(attrs, "ANDOWN",""));
	OB->m_annotation.setBwid  (attrAsInt(attrs, "ANBWID", 1));
	OB->m_annotation.setBsty  (attrAsInt(attrs, "ANBSTY", 0));
	OB->m_annotation.setFeed  (attrAsInt(attrs, "ANFEED", 1));
	OB->m_annotation.setFlag  (attrAsInt(attrs, "ANFLAG", 0));
	OB->m_annotation.setFont  (attrAsInt(attrs, "ANFONT", 4));
	OB->m_annotation.setFormat(attrAsInt(attrs, "ANFORMAT", 0));
	OB->m_annotation.setVis   (attrAsInt(attrs, "ANVIS", 0));
	OB->m_annotation.setIsChk (attrAsBool(attrs, "ANCHK", false));
	OB->m_annotation.setAAact (attrAsBool(attrs, "ANAA", false));
	OB->m_annotation.setHTML  (attrAsBool(attrs, "ANHTML", false));
	OB->m_annotation.setUseIcons(attrAsBool(attrs, "ANICON", false));
	OB->m_annotation.setChkStil (attrAsInt(attrs, "ANCHKS", 0));
	OB->m_annotation.setMaxChar (attrAsInt(attrs, "ANMC", -1));
	OB->m_annotation.setBorderColor(attrAsString(attrs, "ANBCOL",CommonStrings::None));
	OB->m_annotation.setIPlace  (attrAsInt(attrs, "ANPLACE", 1));
	OB->m_annotation.setScaleW  (attrAsInt(attrs, "ANSCALE", 0));
	if (attrAsInt(attrs, "TRANSPARENT", 0) == 1)
		OB->Pcolor = CommonStrings::None;
	OB->textAlignment = attrAsInt(attrs, "ALIGN", 0);
	if (!attrs.value("TEXTFLOWMODE").isEmpty() )
		OB->TextflowMode = (PageItem::TextFlowMode) attrAsInt(attrs, "TEXTFLOWMODE", 0);
	else if (attrAsInt(attrs, "TEXTFLOW", 0))
	{
		if (attrAsInt(attrs, "TEXTFLOW2", 0))
			OB->TextflowMode = PageItem::TextFlowUsesBoundingBox;
		else if (attrAsInt(attrs, "TEXTFLOW3", 0))
			OB->TextflowMode = PageItem::TextFlowUsesContourLine;
		else
			OB->TextflowMode = PageItem::TextFlowUsesFrameShape;	
	}
	else
		OB->TextflowMode = PageItem::TextFlowDisabled;
	OB->Extra    = attrAsDbl(attrs, "EXTRA", 0.0);
	OB->TExtra   = attrAsDbl(attrs, "TEXTRA", 1.0);
	OB->BExtra   = attrAsDbl(attrs, "BEXTRA", 1.0);
	OB->RExtra   = attrAsDbl(attrs, "REXTRA", 1.0);
	OB->PoShow   = attrAsInt(attrs, "PLTSHOW", 0);
	OB->BaseOffs = attrAsDbl(attrs, "BASEOF", 0.0);
	OB->textPathType    = attrAsInt (attrs, "textPathType", 0);
	OB->textPathFlipped = attrAsBool(attrs, "textPathFlipped", false);
	OB->ISize      = qRound(attrAsDbl(attrs, "ISIZE", 12.0) * 10);
	if (!attrs.value("EXTRAV").isEmpty())
		OB->ExtraV = qRound( attrAsDbl(attrs, "EXTRAV", 0.0) / attrAsDbl(attrs, "ISIZE", 12.0) * 1000.0);
	else
		OB->ExtraV = attrAsInt(attrs, "TXTKERN");
/*	OB->Pfile      = Relative2Path( attrAsString(attrs, "PFILE" ,""), baseDir);
	OB->Pfile2     = Relative2Path( attrAsString(attrs, "PFILE2",""), baseDir);
	OB->Pfile3     = Relative2Path( attrAsString(attrs, "PFILE3",""), baseDir); */
	OB->Pfile      = attrAsString(attrs, "PFILE" ,"");
	OB->Pfile2     = attrAsString(attrs, "PFILE2","");
	OB->Pfile3     = attrAsString(attrs, "PFILE3","");
	OB->IProfile   = attrAsString(attrs, "PRFILE","");
	OB->EmProfile  = attrAsString(attrs, "EPROF","");
	OB->IRender    = attrAsInt (attrs, "IRENDER", 1);
	OB->UseEmbedded= attrAsInt (attrs, "EMBEDDED", 1);
	OB->Locked       = attrAsBool(attrs, "LOCK", false);
	OB->LockRes      = attrAsBool(attrs, "LOCKR", false);
	OB->Reverse      = attrAsBool(attrs, "REVERS", false);
	OB->isTableItem  = attrAsBool(attrs, "isTableItem", false);
	OB->TopLine      = attrAsBool(attrs, "TopLine", false);
	OB->LeftLine     = attrAsBool(attrs, "LeftLine", false);
	OB->RightLine    = attrAsBool(attrs, "RightLine", false);
	OB->BottomLine   = attrAsBool(attrs, "BottomLine", false);
	OB->TopLinkID    = attrAsInt(attrs, "TopLINK", -1);
	OB->LeftLinkID   = attrAsInt(attrs, "LeftLINK", -1);
	OB->RightLinkID  = attrAsInt(attrs, "RightLINK", -1);
	OB->BottomLinkID = attrAsInt(attrs, "BottomLINK", -1);
	OB->Transparency = attrAsDbl(attrs, "TransValue", 0.0);
	if (!attrs.value("TransValueS").isEmpty())
		OB->TranspStroke = attrAsDbl(attrs, "TransValueS", 0.0);
	else
		OB->TranspStroke = OB->Transparency;
	OB->TransBlend  = attrAsInt(attrs, "TransBlend", 0);
	OB->TransBlendS = attrAsInt(attrs, "TransBlendS", 0);
	tmp = "";
	int numClip = attrAsInt(attrs, "NUMCLIP", 0);
	if (numClip > 0)
	{
		OB->Clip.resize(numClip);
		tmp = attrAsString(attrs, "CLIPCOOR", "");
		QTextStream fc(&tmp, QIODevice::ReadOnly);
		for (int c=0; c < numClip; ++c)
		{
			fc >> x;
			fc >> y;
			OB->Clip.setPoint(c, x, y);
		}
	}
	else
		OB->Clip.resize(0);
	tmp = "";
	int numPo = attrAsInt(attrs, "NUMPO", 0);
	if (numPo > 0 )
	{
		OB->PoLine.resize(numPo);
		tmp = attrAsString(attrs, "POCOOR", "");
		QTextStream fp(&tmp, QIODevice::ReadOnly);
		for (int cx=0; cx < numPo; ++cx)
		{
			fp >> xf;
			fp >> yf;
			OB->PoLine.setPoint(cx, xf, yf);
		}
	}
	else
		OB->PoLine.resize(0);
	tmp = "";
	int numCo = attrAsInt(attrs, "NUMCO", 0); 
	if (numCo > 0)
	{
		OB->ContourLine.resize(numCo);
		tmp = attrAsString(attrs, "COCOOR", "");
		QTextStream fp(&tmp, QIODevice::ReadOnly);
		for (int cx=0; cx < numCo; ++cx)
		{
			fp >> xf;
			fp >> yf;
			OB->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		OB->ContourLine.resize(0);
	tmp = "";
	int numTab = attrAsInt(attrs, "NUMTAB", 0); 
	if (numTab > 0)
	{
		ParagraphStyle::TabRecord tb;
		tmp = attrAsString(attrs, "TABS", "");
		QTextStream tgv(&tmp, QIODevice::ReadOnly);
		OB->TabValues.clear();
		for (int cxv = 0; cxv < numTab; cxv += 2)
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
	int numDash = attrAsInt(attrs, "NUMDASH", 0); 
	if (numDash > 0)
	{
		tmp = attrAsString(attrs, "DASHS", "");
		QTextStream dgv(&tmp, QIODevice::ReadOnly);
		OB->DashValues.clear();
		for (int cxv = 0; cxv < numDash; ++cxv)
		{
			dgv >> xf;
			OB->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		OB->DashValues.clear();
	OB->DashOffset = attrAsDbl(attrs, "DASHOFF", 0.0);
}

void ScriXmlDoc::GetItemText(const QXmlStreamAttributes& attrs, ScribusDoc *doc, bool VorLFound, bool impo, PageItem* obj)
{
	QString tmp2, tmf, tmpf, tmp3;
	tmp2 = attrAsString(attrs, "CH", "");
	tmp2.replace(QRegExp("\r"), QChar(5));
	tmp2.replace(QRegExp("\n"), QChar(5));
	tmp2.replace(QRegExp("\t"), QChar(4));
	tmpf = attrAsString(attrs, "CFONT", doc->toolSettings.defFont);
	bool unknown = false;
	ScFace dummy = ScFace::none();
	if ((!prefsManager->appPrefs.AvailFonts.contains(tmpf)) || (!prefsManager->appPrefs.AvailFonts[tmpf].usable()))
	{
		bool isThere = false;
		for (int dl = 0; dl < dummyScFaces.count(); ++dl)
		{
			if (dummyScFaces.at(dl).scName() == tmpf)
			{
				isThere = true;
				dummy = dummyScFaces.at(dl);
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
	int size = qRound(attrAsDbl(attrs, "CSIZE") * 10);
	QString fcolor = attrAsString(attrs, "CCOLOR", CommonStrings::None);
	int extra;
	if (!attrs.value("CEXTRA").isEmpty())
		extra = qRound(attrAsDbl(attrs, "CEXTRA") / attrAsDbl(attrs, "CSIZE", 1.0) * 1000.0);
	else
		extra = attrAsInt(attrs, "CKERN");
	int shade  = attrAsInt(attrs, "CSHADE");
	int cstyle = attrAsInt(attrs, "CSTYLE") & 2047;
	QString stroke = attrAsString(attrs, "CSTROKE", CommonStrings::None);
	int shade2 = attrAsInt(attrs, "CSHADE2", 100);
	int scale  = qRound(attrAsDbl(attrs, "CSCALE", 100.0) * 10);
	int scalev = qRound(attrAsDbl(attrs, "CSCALEV", 100.0) * 10);
	int base   = qRound(attrAsDbl(attrs, "CBASE", 0.0) * 10);
	int shX    = qRound(attrAsDbl(attrs, "CSHX", 5.0) * 10);
	int shY    = qRound(attrAsDbl(attrs, "CSHY", -5.0) * 10);
	int outL   = qRound(attrAsDbl(attrs, "COUT", 1.0) * 10);
	int ulp    = qRound(attrAsDbl(attrs, "CULP", -0.1) * 10);
	int ulw    = qRound(attrAsDbl(attrs, "CULW", -0.1) * 10);
	int stp    = qRound(attrAsDbl(attrs, "CSTP", -0.1) * 10);
	int stw    = qRound(attrAsDbl(attrs, "CSTW", -0.1) * 10);
	QString pstyleName = attrAsString(attrs, "PSTYLE", "");
	for (int cxx=0; cxx<tmp2.length(); ++cxx)
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
		style.setFeatures(static_cast<StyleFlag>(cstyle).featureList());
/*
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
*/		
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
		if (ch == SpecialChars::PARSEP)
		{
			ParagraphStyle pstyle;
			pstyle.setParent(pstyleName);
			obj->itemText.applyStyle(pos, pstyle);
		}
		else
			obj->itemText.applyCharStyle(pos, 1, style);
	}
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
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			MissingFont *dia = new MissingFont(0, tmpf, doc);
			dia->exec();
			tmpf = dia->getReplacementFont();
			delete dia;
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
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

void ScriXmlDoc::SetItemProps(QXmlStreamWriter& writer, ScribusDoc *doc, PageItem* item, const QString& baseDir, bool newFormat)
{
	double xf, yf;
	QString tmp, tmpy;
	if (newFormat)
		writer.writeAttribute("OwnPage", QString::number(item->OwnPage));
	const ParagraphStyle& defaultStyle = item->itemText.defaultStyle();
	const CharStyle&      charStyle    = item->itemText.defaultStyle().charStyle();
	writer.writeAttribute("PTYPE"    ,QString::number(item->realItemType()));
	writer.writeAttribute("XPOS"     ,QString::number(item->xPos() - doc->currentPage()->xOffset()));
	writer.writeAttribute("YPOS"     ,QString::number(item->yPos() - doc->currentPage()->yOffset()));
	writer.writeAttribute("WIDTH"    ,QString::number(item->width()));
	writer.writeAttribute("HEIGHT"   ,QString::number(item->height()));
	writer.writeAttribute("RADRECT"  ,QString::number(item->cornerRadius()));
	writer.writeAttribute("FRTYPE"   ,QString::number(item->FrameType));
	writer.writeAttribute("CLIPEDIT" ,item->ClipEdited ? "1" : "0");
	writer.writeAttribute("PWIDTH"   ,QString::number(item->lineWidth()));
	writer.writeAttribute("PCOLOR"   ,item->fillColor());
	writer.writeAttribute("PCOLOR2"  ,item->lineColor());
	writer.writeAttribute("TXTFILL"  ,charStyle.fillColor());
	writer.writeAttribute("TXTSTROKE",charStyle.strokeColor());
	writer.writeAttribute("TXTSTRSH" ,QString::number(charStyle.strokeShade()));
	writer.writeAttribute("TXTFILLSH",QString::number(charStyle.fillShade()));
	writer.writeAttribute("TXTSCALE" ,QString::number(charStyle.scaleH() / 10.0));
	writer.writeAttribute("TXTSCALEV",QString::number(charStyle.scaleV() / 10.0));
	writer.writeAttribute("TXTBASE"  ,QString::number(charStyle.baselineOffset() / 10.0));
	writer.writeAttribute("TXTSHX"   ,QString::number(charStyle.shadowXOffset() / 10.0));
	writer.writeAttribute("TXTSHY"   ,QString::number(charStyle.shadowYOffset() / 10.0));
	writer.writeAttribute("TXTOUT"   ,QString::number(charStyle.outlineWidth() / 10.0));
	writer.writeAttribute("TXTULP"   ,QString::number(charStyle.underlineOffset() / 10.0));
	writer.writeAttribute("TXTULW"   ,QString::number(charStyle.underlineWidth() / 10.0));
	writer.writeAttribute("TXTSTP"   ,QString::number(charStyle.strikethruOffset() / 10.0));
	writer.writeAttribute("TXTSTW"   ,QString::number(charStyle.strikethruWidth() / 10.0));
	writer.writeAttribute("TXTSTYLE" ,QString::number(charStyle.effects()));
	writer.writeAttribute("COLUMNS"  ,QString::number(item->columns()));
	writer.writeAttribute("COLGAP"   ,QString::number(item->columnGap()));
	writer.writeAttribute("NAMEDLST" ,item->NamedLStyle);
	writer.writeAttribute("SHADE"    ,QString::number(item->fillShade()));
	writer.writeAttribute("SHADE2"   ,QString::number(item->lineShade()));
	writer.writeAttribute("GRTYP"    ,QString::number(item->GrType));
	writer.writeAttribute("ROT"      ,QString::number(item->rotation()));
	writer.writeAttribute("PLINEART" ,QString::number(item->PLineArt));
	writer.writeAttribute("PLINEEND" ,QString::number(item->PLineEnd));
	writer.writeAttribute("PLINEJOIN",QString::number(item->PLineJoin));
	writer.writeAttribute("LINESP"   ,QString::number(defaultStyle.lineSpacing()));
	writer.writeAttribute("LINESPMode",QString::number(defaultStyle.lineSpacingMode()));
	writer.writeAttribute("TXTKERN"  ,QString::number(charStyle.tracking()));
	writer.writeAttribute("LOCALSCX" ,QString::number(item->imageXScale()));
	writer.writeAttribute("LOCALSCY" ,QString::number(item->imageYScale()));
	writer.writeAttribute("LOCALX"   ,QString::number(item->imageXOffset()));
	writer.writeAttribute("LOCALY"   ,QString::number(item->imageYOffset()));
	writer.writeAttribute("PICART"   ,item->imageShown() ? "1" : "0");
	writer.writeAttribute("PLTSHOW"  ,item->PoShow ? "1" : "0");
	writer.writeAttribute("BASEOF"   ,QString::number(item->BaseOffs));
	writer.writeAttribute("textPathType",QString::number(item->textPathType));
	writer.writeAttribute("textPathFlipped",item->textPathFlipped ? "1" : "0");
	writer.writeAttribute("FLIPPEDH" , item->imageFlippedH() ? "1" : "0");
	writer.writeAttribute("FLIPPEDV" , item->imageFlippedV() ? "1" : "0");
/*	writer.writeAttribute("BBOXX",item->BBoxX);
	writer.writeAttribute("BBOXH",item->BBoxH); */
	writer.writeAttribute("IFONT"    ,charStyle.font().scName());
	writer.writeAttribute("ISIZE"    ,QString::number(charStyle.fontSize() / 10.0) );
	writer.writeAttribute("SCALETYPE",item->ScaleType ? "1" : "0");
	writer.writeAttribute("RATIO"    ,item->AspectRatio ? "1" : "0");
	writer.writeAttribute("PRINTABLE",item->printEnabled() ? "1" : "0");
	if(item->isAnnotation())
	{
		writer.writeAttribute("ANNOTATION","1");
		writer.writeAttribute("ANTYPE"  , QString::number(item->annotation().Type()));
		writer.writeAttribute("ANACTION", item->annotation().Action());
		writer.writeAttribute("ANEACT"  , item->annotation().E_act());
		writer.writeAttribute("ANXACT"  , item->annotation().X_act());
		writer.writeAttribute("ANDACT"  , item->annotation().D_act());
		writer.writeAttribute("ANFOACT" , item->annotation().Fo_act());
		writer.writeAttribute("ANBLACT" , item->annotation().Bl_act());
		writer.writeAttribute("ANKACT"  , item->annotation().K_act());
		writer.writeAttribute("ANFACT"  , item->annotation().F_act());
		writer.writeAttribute("ANVACT"  , item->annotation().V_act());
		writer.writeAttribute("ANCACT"  , item->annotation().C_act());
		if (item->annotation().ActionType() == 8)
			writer.writeAttribute("ANEXTERN", item->annotation().Extern());
		else
			writer.writeAttribute("ANEXTERN", Path2Relative(item->annotation().Extern(), baseDir));
		writer.writeAttribute("ANZIEL"  , QString::number(item->annotation().Ziel()));
		writer.writeAttribute("ANACTYP" , QString::number(item->annotation().ActionType()));
		writer.writeAttribute("ANTOOLTIP", item->annotation().ToolTip());
		writer.writeAttribute("ANBWID"  , QString::number(item->annotation().Bwid()));
		writer.writeAttribute("ANBSTY"  , QString::number(item->annotation().Bsty()));
		writer.writeAttribute("ANFEED"  , QString::number(item->annotation().Feed()));
		writer.writeAttribute("ANFLAG"  , QString::number(item->annotation().Flag()));
		writer.writeAttribute("ANFONT"  , QString::number(item->annotation().Font()));
		writer.writeAttribute("ANFORMAT", QString::number(item->annotation().Format()));
		writer.writeAttribute("ANROLL"  , item->annotation().RollOver());
		writer.writeAttribute("ANDOWN"  , item->annotation().Down());
		writer.writeAttribute("ANVIS"   , QString::number(item->annotation().Vis()));
		writer.writeAttribute("ANMC"    , QString::number(item->annotation().MaxChar()));
		writer.writeAttribute("ANCHK"   , item->annotation().IsChk() ? "1" : "0");
		writer.writeAttribute("ANAA"    , item->annotation().AAact() ? "1" : "0");
		writer.writeAttribute("ANCHKS"  , QString::number(item->annotation().ChkStil()));
		writer.writeAttribute("ANBCOL"  , item->annotation().borderColor());
		writer.writeAttribute("ANHTML"  , item->annotation().HTML() ? "1" : "0");
		writer.writeAttribute("ANICON"  , item->annotation().UseIcons() ? "1" : "0");
		writer.writeAttribute("ANPLACE" , item->annotation().IPlace() ? "1" : "0");
		writer.writeAttribute("ANSCALE" , item->annotation().ScaleW() ? "1" : "0");
	}
	else
		writer.writeAttribute("ANNOTATION","0");
	writer.writeAttribute("ANNAME", !item->AutoName ? item->itemName() : QString(""));
	// "TEXTFLOWMODE" succeed to "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes
	writer.writeAttribute("TEXTFLOWMODE", QString::number((int) item->textFlowMode()) );
	// Set "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes for compatibility
	// with versions prior to 1.3.4
	writer.writeAttribute("TEXTFLOW" , item->textFlowAroundObject()    ? "1" : "0");
	writer.writeAttribute("TEXTFLOW2", item->textFlowUsesBoundingBox() ? "1" : "0");
	writer.writeAttribute("TEXTFLOW3", item->textFlowUsesContourLine() ? "1" : "0");
	writer.writeAttribute("AUTOTEXT" , item->isAutoText ? "1" : "0");
	writer.writeAttribute("EXTRA"    , QString::number(item->textToFrameDistLeft()));
	writer.writeAttribute("TEXTRA"   , QString::number(item->textToFrameDistTop()));
	writer.writeAttribute("BEXTRA"   , QString::number(item->textToFrameDistBottom()));
	writer.writeAttribute("REXTRA"   , QString::number(item->textToFrameDistRight()));
	if (((item->asImageFrame() && !item->asLatexFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
		writer.writeAttribute("PFILE",Path2Relative(item->Pfile, baseDir));
	if (!item->Pfile2.isEmpty())
		writer.writeAttribute("PFILE2",Path2Relative(item->Pfile2, baseDir));
	else
		writer.writeAttribute("PFILE2","");
	if (!item->Pfile3.isEmpty())
		writer.writeAttribute("PFILE3",Path2Relative(item->Pfile3, baseDir));
	else
		writer.writeAttribute("PFILE3","");
	writer.writeAttribute("PRFILE"     , item->IProfile);
	writer.writeAttribute("EPROF"      , item->EmProfile);
	writer.writeAttribute("IRENDER"    , QString::number(item->IRender));
	writer.writeAttribute("EMBEDDED"   , item->UseEmbedded ? "1" : "0");
	writer.writeAttribute("LOCK"       , item->locked() ? "1" : "0");
	writer.writeAttribute("LOCKR"      , item->sizeLocked() ? "1" : "0");
	writer.writeAttribute("REVERS"     , item->reversed() ? "1" : "0");
	writer.writeAttribute("TransValue" , QString::number(item->fillTransparency()));
	writer.writeAttribute("TransValueS", QString::number(item->lineTransparency()));
	writer.writeAttribute("TransBlend" , QString::number(item->fillBlendmode()));
	writer.writeAttribute("TransBlendS", QString::number(item->lineBlendmode()));
	writer.writeAttribute("isTableItem", QString::number(static_cast<int>(item->isTableItem)));
	writer.writeAttribute("TopLine"    , QString::number(static_cast<int>(item->TopLine)));
	writer.writeAttribute("LeftLine"   , QString::number(static_cast<int>(item->LeftLine)));
	writer.writeAttribute("RightLine"  , QString::number(static_cast<int>(item->RightLine)));
	writer.writeAttribute("BottomLine" , QString::number(static_cast<int>(item->BottomLine)));
	if (item->asLatexFrame()) {
		PageItem_LatexFrame *latexitem = item->asLatexFrame();
		//NOTE: Even though these settings are written they can't be read back
		// because CopyPasteBuffer does not support them
		writer.writeAttribute("LatexDpi", QString::number(latexitem->dpi()));
		writer.writeAttribute("LatexConfig", latexitem->configFile());
		writer.writeAttribute("LatexUsePreamble",
			QString::number(static_cast<int>(latexitem->usePreamble())));
	}
	if (item->isTableItem)
	{
		if (item->TopLink != 0)
			writer.writeAttribute("TopLINK", QString::number(item->TopLink->ItemNr));
		else
			writer.writeAttribute("TopLINK", "-1");
		if (item->LeftLink != 0)
			writer.writeAttribute("LeftLINK", QString::number(item->LeftLink->ItemNr));
		else
			writer.writeAttribute("LeftLINK", "-1");
		if (item->RightLink != 0)
			writer.writeAttribute("RightLINK", QString::number(item->RightLink->ItemNr));
		else
			writer.writeAttribute("RightLINK", "-1");
		if (item->BottomLink != 0)
			writer.writeAttribute("BottomLINK", QString::number(item->BottomLink->ItemNr));
		else
			writer.writeAttribute("BottomLINK", "-1");
		writer.writeAttribute("OwnLINK", QString::number(item->ItemNr));
	}
	writer.writeAttribute("isGroupControl", item->isGroupControl ? "1" : "0");
	if (item->isGroupControl)
	{
		if (item->groupsLastItem != 0)
			writer.writeAttribute("groupsLastItem", QString::number(item->groupsLastItem->ItemNr - item->ItemNr));
	}
	writer.writeAttribute("NUMDASH", QString::number(item->DashValues.count()));
	QString dlp = "";
	QList<double>::Iterator dax;
	for (dax = item->DashValues.begin(); dax != item->DashValues.end(); ++dax)
		dlp += tmp.setNum((*dax)) + " ";
	writer.writeAttribute("DASHS"  , dlp);
	writer.writeAttribute("DASHOFF", QString::number(item->DashOffset));
	writer.writeAttribute("NUMPO"  , QString::number(item->PoLine.size()));
	QString polp = "";
	for (uint nxx=0; nxx<item->PoLine.size(); ++nxx)
	{
		item->PoLine.point(nxx, &xf, &yf);
		polp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	writer.writeAttribute("POCOOR", polp);
	writer.writeAttribute("NUMCO" , QString::number(item->ContourLine.size()));
	QString colp = "";
	for (uint nxx=0; nxx<item->ContourLine.size(); ++nxx)
	{
		item->ContourLine.point(nxx, &xf, &yf);
		colp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	writer.writeAttribute("COCOOR"  , colp);
	writer.writeAttribute("NUMGROUP", QString::number(item->Groups.count()));
	QString glp = "";
	QStack<int>::Iterator nx;
	for (nx = item->Groups.begin(); nx != item->Groups.end(); ++nx)
		glp += tmp.setNum((*nx)) + " ";
	writer.writeAttribute("GROUPS"  , glp);
	writer.writeAttribute("LANGUAGE", charStyle.language());
	writer.writeAttribute("startArrowIndex", QString::number(item->startArrowIndex()));
	writer.writeAttribute("endArrowIndex"  , QString::number(item->endArrowIndex()));
}

//CB: Private only now
void ScriXmlDoc::GetStyle(QDomElement &pg, ParagraphStyle &vg, StyleSet<ParagraphStyle> & docParagraphStyles, ScribusDoc* doc, bool fl)
{
	bool fou;
	QString tmpf, tmf, tmV;
	double xf, xf2;
	fou = false;
	vg.setName(pg.attribute("NAME"));
	vg.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(pg.attribute("LINESPMode", "0").toInt()));
	vg.setLineSpacing(pg.attribute("LINESP").toDouble());
	vg.setLeftMargin(pg.attribute("INDENT", "0").toDouble());
	vg.setFirstIndent(pg.attribute("FIRST", "0").toDouble());
	vg.setAlignment(static_cast<ParagraphStyle::AlignmentType>(pg.attribute("ALIGN").toInt()));
	vg.setGapBefore(pg.attribute("VOR", "0").toDouble());
	vg.setGapAfter(pg.attribute("NACH", "0").toDouble());
	tmpf = pg.attribute("FONT", doc->toolSettings.defFont);
	if (tmpf.isEmpty())
		tmpf = doc->toolSettings.defFont;
	tmf = tmpf;
	if (!DoFonts.contains(tmpf))
		tmpf = AskForFont(prefsManager->appPrefs.AvailFonts, tmpf, doc);
	else
		tmpf = DoFonts[tmf];
	vg.charStyle().setFont(prefsManager->appPrefs.AvailFonts[tmpf]);
	vg.charStyle().setFontSize(qRound(pg.attribute("FONTSIZE", "12").toDouble() * 10.0));
	vg.setHasDropCap(static_cast<bool>(pg.attribute("DROP", "0").toInt()));
	vg.setDropCapLines(pg.attribute("DROPLIN", "2").toInt());
	vg.setDropCapOffset(pg.attribute("DROPDIST", "0").toDouble());
	vg.charStyle().setFeatures(static_cast<StyleFlag>(pg.attribute("EFFECT", "0").toInt()).featureList());
	vg.charStyle().setFillColor(pg.attribute("FCOLOR", doc->toolSettings.dBrush));
	vg.charStyle().setFillShade(pg.attribute("FSHADE", "100").toInt());
	vg.charStyle().setStrokeColor(pg.attribute("SCOLOR", doc->toolSettings.dPen));
	vg.charStyle().setStrokeShade(pg.attribute("SSHADE", "100").toInt());

	if (static_cast<bool>(pg.attribute("BASE", "0").toInt()))
		vg.setLineSpacingMode(ParagraphStyle::BaselineGridLineSpacing);
	vg.charStyle().setShadowXOffset(qRound(pg.attribute("TXTSHX", "5").toDouble() * 10));
	vg.charStyle().setShadowYOffset(qRound(pg.attribute("TXTSHY", "-5").toDouble() * 10));
	vg.charStyle().setOutlineWidth(qRound(pg.attribute("TXTOUT", "1").toDouble() * 10));
	vg.charStyle().setUnderlineOffset(qRound(pg.attribute("TXTULP", "-0.1").toDouble() * 10));
	vg.charStyle().setUnderlineWidth(qRound(pg.attribute("TXTULW", "-0.1").toDouble() * 10));
	vg.charStyle().setStrikethruOffset(qRound(pg.attribute("TXTSTP", "-0.1").toDouble() * 10));
	vg.charStyle().setStrikethruWidth(qRound(pg.attribute("TXTSTW", "-0.1").toDouble() * 10));
	vg.charStyle().setScaleH(qRound(pg.attribute("SCALEH", "100").toDouble() * 10));
	vg.charStyle().setScaleV(qRound(pg.attribute("SCALEV", "100").toDouble() * 10));
	vg.charStyle().setBaselineOffset(qRound(pg.attribute("BASEO", "0").toDouble() * 10));
	vg.charStyle().setTracking(qRound(pg.attribute("KERN", "0").toDouble() * 10));
//	vg.tabValues().clear();
	if ((pg.hasAttribute("NUMTAB")) && (pg.attribute("NUMTAB", "0").toInt() != 0))
	{
		QList<ParagraphStyle::TabRecord> tbs;
		ParagraphStyle::TabRecord tb;
		QString tmp = pg.attribute("TABS");
		QTextStream tgv(&tmp, QIODevice::ReadOnly);
		tbs.clear();
		for (int cxv = 0; cxv < pg.attribute("NUMTAB", "0").toInt(); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			tbs.append(tb);
		}
		vg.setTabValues(tbs);
		tmp = "";
	}
	else
	{
		QList<ParagraphStyle::TabRecord> tbs;
		QDomNode IT = pg.firstChild();
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
			IT=IT.nextSibling();
		}
		vg.setTabValues(tbs);
	}
	for (int xx=0; xx<docParagraphStyles.count(); ++xx)
	{
		if (vg.name() == docParagraphStyles[xx].name())
		{
			//Compare the attributes of the pasted styles vs existing ones
			if (vg.equiv(docParagraphStyles[xx]))
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
				vg.setName("Copy of "+docParagraphStyles[xx].name());
				fou = false;
			}
			break;
		}
	}
	if (!fou)
	{
		for (int xx=0; xx< docParagraphStyles.count(); ++xx)
		{
			if (vg.equiv(docParagraphStyles[xx]))
			{
				vg.setName(docParagraphStyles[xx].name());
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
		docParagraphStyles.create(vg);
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
	if (isFile)
	{
		QByteArray f;
		if (!loadRawText(file, f))
			return false;
		if (f.left(16) == "<SCRIBUSELEMUTF8")
			ff = QString::fromUtf8(f.data());
		else
			ff = f;
	}
	else
		ff  = file;

	bool succeed = false;
	QStringRef tName;
	QXmlStreamReader sReader(ff);
	QXmlStreamReader::TokenType tType;
	while (!sReader.atEnd() && !sReader.hasError())
	{
		tType = sReader.readNext();
		if (tType == QXmlStreamReader::StartElement)
		{
			tName = sReader.name();
			if ((tName == "SCRIBUSELEM") || (tName == "SCRIBUSELEMUTF8"))
			{
				QXmlStreamAttributes attrs;
				QString attx = attrs.value("XP").toString();
				QString atty = attrs.value("YP").toString();
				QString attw = attrs.value("W").toString();
				QString atth = attrs.value("H").toString();
				*x = (attx.isEmpty()) ? 0.0 : attx.toDouble();
				*y = (atty.isEmpty()) ? 0.0 : atty.toDouble();
				*w = (attw.isEmpty()) ? 0.0 : attw.toDouble();
				*h = (atth.isEmpty()) ? 0.0 : atth.toDouble();
				succeed = true;
			}
		}
	}
	return (succeed && !sReader.hasError());
}

bool ScriXmlDoc::ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, double Xp, double Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub, ScribusView *view)
{
	QString ff;
	struct CopyPasteBuffer OB;
	ParagraphStyle vg;
	QString tmp, tmpf, tmf;
	QMap<int,int> TableID;
	QMap<int,int> arrowID;
	QMap<PageItem*, int> groupID;
	QMap<int, ImageLoadRequest> loadRequests;
	QList<QXmlStreamAttributes> iTextElems;
	QList<PageItem*>  TableItems;
	ScImageEffectList imageEffects;
	bool VorLFound = false;
	bool newVersion = false;
	double GrX = 0.0, GrY = 0.0;
	int x, GrMax = doc->GroupCounter;
	ScColor lf = ScColor();

	QString fileDir = QDir::homePath();
	if (Fi)
	{
		QByteArray f;
		if (!loadRawText(fileName, f))
			return false;
		if (f.left(16) == "<SCRIBUSELEMUTF8")
			ff = QString::fromUtf8(f.data());
		else
			ff = f;
		fileDir = QFileInfo(fileName).absolutePath();
	}
	else
	{
		ff = fileName;
	}

	// In case ff contains some old broken scribus xml
	ff.replace(QChar(5), SpecialChars::PARSEP);
	ff.replace(QChar(4), SpecialChars::TAB);

	bool isScribusElem = false;
	QXmlStreamReader     sReader(ff);
	QXmlStreamAttributes attrs;
	
	while (!sReader.atEnd() && !sReader.hasError())
	{
		if (sReader.readNext() == QXmlStreamReader::StartElement)
		{
			QStringRef tName = sReader.name();
			if (tName == "SCRIBUSELEM" || tName == "SCRIBUSELEMUTF8")
			{
				QXmlStreamAttributes attrs = sReader.attributes();
				if (!loc)
				{
					GrX = attrs.value("XP").toString().toDouble();
					GrY = attrs.value("YP").toString().toDouble();
				}
				if (!attrs.value("Version").isEmpty())
					newVersion = true;
				isScribusElem = true;
				break;
			}
		}
	}
	if (!isScribusElem || sReader.hasError())
		return false;

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
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(QDir::homePath());
	int startNumArrows = doc->arrowStyles.count();
	while (!sReader.atEnd() && !sReader.hasError())
	{
		if (sReader.readNext() != QXmlStreamReader::StartElement)
			continue;
		QString tagName = sReader.name().toString();
		attrs           = sReader.attributes();
		if( tagName == "Arrows")
		{
			QString attIndex = attrs.value("Index").toString();
			if (attIndex.toInt() > startNumArrows)
			{
				struct ArrowDesc arrow;
				double xa, ya;
				arrow.name  = attrs.value("Name").toString();
				arrow.userArrow = true;
				QString tmp = attrs.value("Points").toString();
				QTextStream fp(&tmp, QIODevice::ReadOnly);
				uint numPoints = attrs.value("NumPoints").toString().toUInt();
				for (uint cx = 0; cx < numPoints; ++cx)
				{
					fp >> xa;
					fp >> ya;
					arrow.points.addPoint(xa, ya);
				}
				doc->arrowStyles.append(arrow);
				arrowID.insert(attIndex.toInt(), doc->arrowStyles.count());
			}
			else
				arrowID.insert(attIndex.toInt(), attIndex.toInt());
		}
		if ( tagName == "FONT" )
		{
			QString attName = attrs.value("NAME").toString();
			tmpf = attName;
			if ((!avail.contains(tmpf)) || (!avail[tmpf].usable()))
			{
				if (!FontSub.contains(tmpf) || (!avail[FontSub[tmpf]].usable()))
				{
					MissingFont *dia = new MissingFont(0, tmpf, doc);
					dia->exec();
					tmpf = dia->getReplacementFont();
					FontSub[attName] = tmpf;
					delete dia;
				}
				else
					tmpf = FontSub[tmpf];
			}
//			fo = avail[tmpf]->Font;
//			fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
			if(!doc->UsedFonts.contains(tmpf))
				doc->AddFont(tmpf);
			DoFonts[attName] = tmpf;
		}
		// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
		if ( tagName=="COLOR" && attrs.value("NAME")!=CommonStrings::None )
		{
			QString colName = attrs.value("NAME").toString();
			if (!attrs.value("CMYK").isEmpty())
				lf.setNamedColor(attrs.value("CMYK").toString());
			else
				lf.fromQColor(QColor(attrs.value("RGB").toString()));
			lf.setSpotColor( attrAsBool(attrs, "Spot", false) );
			lf.setRegistrationColor( attrAsBool(attrs, "Register", false) );
			if (!doc->PageColors.contains(colName))
			  	doc->PageColors[colName] = lf;
		}
		if (tagName=="MultiLine")
		{
			multiLine ml;
			SingleLine sl;
			QString  mlName = attrs.value("Name").toString();
			while(!(sReader.isEndElement() && sReader.name() == tagName))
			{
				if (sReader.readNext() != QXmlStreamReader::StartElement)
					continue;
				QString tagName1 = sReader.name().toString();
				QXmlStreamAttributes attrs1 = sReader.attributes();
				if (tagName1 == "SubLine")
				{
					sl.Color    = attrs1.value("Color").toString();
					sl.Dash     = attrAsInt(attrs1, "Dash");
					sl.LineEnd  = attrAsInt(attrs1, "LineEnd");
					sl.LineJoin = attrAsInt(attrs1, "LineJoin");
					sl.Shade    = attrAsInt(attrs1, "Shade");
					sl.Width    = attrAsDbl(attrs1, "Width");
					ml.push_back(sl);
				}
			}
			if (!doc->MLineStyles.contains(mlName))
				doc->MLineStyles.insert(mlName, ml);
		}
		/*if(pg.tagName()=="STYLE")
		{
			GetStyle(pg, vg, doc->docParagraphStyles, doc, true);
			VorLFound = true;
		}*/
		if (tagName=="Pattern")
		{
			ScPattern pat;
			uint ac = doc->Items->count();
			bool savedAlignGrid   = doc->useRaster;
			bool savedAlignGuides = doc->SnapGuides;
			bool isGroupControl   = false;
			doc->useRaster  = false;
			doc->SnapGuides = false;
			QString patClipPath;
			QString patName   = attrs.value("Name").toString();
			double  patScaleX = attrAsDbl(attrs, "scaleX", 0.0);
			double  patScaleY = attrAsDbl(attrs, "scaleY", 0.0);
			double  patWidth  = attrAsDbl(attrs, "width", 0.0);
			double  patHeight = attrAsDbl(attrs, "height", 0.0);
			int     patOwnLink = 0;
			int groupsLastItem = 0;
			QString patText   = sReader.readElementText();
			QXmlStreamReader sReader1(patText);
			while(!sReader1.atEnd() && !sReader1.hasError())
			{
				sReader1.readNext();
				if (sReader1.hasError())
					break;
				QString tagName1 = sReader1.name().toString();
				QXmlStreamAttributes attrs1 = sReader1.attributes();
				
				if (tagName1 == "PatternItem" && sReader1.isStartElement())
				{
					iTextElems.clear();
					loadRequests.clear();
					imageEffects.clear();
					GetItemProps(attrs1, &OB, fileDir, newVersion);
					patClipPath    = attrs1.value("ImageClip").toString();
					patOwnLink     = attrAsInt(attrs1, "OwnLINK", 0);
					isGroupControl = attrAsBool(attrs1, "isGroupControl", false);
					groupsLastItem = attrAsInt(attrs1, "groupsLastItem", 0);
					OB.Xpos = attrAsDbl(attrs1, "XPOS") + doc->currentPage()->xOffset();
					OB.Ypos = attrAsDbl(attrs1, "YPOS") + doc->currentPage()->yOffset();
					OB.startArrowIndex = arrowID[ attrAsInt(attrs1, "startArrowIndex", 0) ];
					OB.endArrowIndex   = arrowID[ attrAsInt(attrs1, "endArrowIndex", 0)];
					OB.isBookmark      = attrAsInt(attrs1, "BOOKMARK");
					OB.NamedLStyle     = attrs1.value("NAMEDLST").toString();
					if (!doc->MLineStyles.contains(OB.NamedLStyle))
						OB.NamedLStyle = "";
					OB.itemText        = "";
					OB.textAlignment   = attrAsInt(attrs1, "ALIGN");
					tmf = attrAsString(attrs1, "IFONT", doc->toolSettings.defFont);
					if (tmf.isEmpty())
						tmf = doc->toolSettings.defFont;
					if (DoFonts[tmf].isEmpty())
						OB.IFont = doc->toolSettings.defFont;
					else
						OB.IFont = DoFonts[tmf];
					OB.LayerNr  = 0;
					OB.Language = attrAsString(attrs1, "LANGUAGE", doc->Language);
					tmp = "";
					if ( (!attrs1.value("GROUPS").isEmpty()) && (attrAsInt(attrs1, "GROUSPS", 0) != 0))
					{
						tmp = attrAsString(attrs1, "GROUPS", "");
						QTextStream fg(&tmp, QIODevice::ReadOnly);
						OB.Groups.clear();
						int numGroups = attrAsInt(attrs1, "NUMGROUP", 0);
						for (int cx = 0; cx < numGroups; ++cx)
						{
							fg >> x;
							OB.Groups.push(x+doc->GroupCounter);
							GrMax = qMax(GrMax, x+doc->GroupCounter);
						}
						tmp.clear();
					}
					else
						OB.Groups.clear();
					tmp.clear();
				}
				if (tagName1 == "ITEXT" && sReader1.isStartElement())
				{
					iTextElems.append( sReader1.attributes() );
					continue;
				}
				if (tagName1 == "ImageEffect" && sReader1.isStartElement())
				{
					struct ImageEffect ef;
					ef.effectParameters = attrAsString(attrs1, "Param", "");
					ef.effectCode       = attrAsInt(attrs1, "Code");
					imageEffects.append(ef);
				}
				if (tagName1 == "PSDLayer" && sReader1.isStartElement())
				{
					struct ImageLoadRequest loadingInfo;
					loadingInfo.blend   = attrAsString(attrs1, "Blend", "");
					loadingInfo.opacity = attrAsInt(attrs1, "Opacity", 1);
					loadingInfo.visible = attrAsBool(attrs, "Visible", false);
					loadingInfo.useMask = attrAsBool(attrs, "useMask", true);
					loadRequests.insert( attrAsInt(attrs1, "Layer"), loadingInfo);
				}
				if (tagName1 == "CSTOP" && sReader1.isStartElement())
				{
					QString name = attrs1.value("NAME").toString();
					double ramp  = attrAsDbl(attrs1, "RAMP", 0.0);
					int shade    = attrAsInt(attrs1, "SHADE", 100);
					double opa   = attrAsDbl(attrs1, "TRANS", 1.0);
					OB.fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
					OB.GrColor   = "";
					OB.GrColor2  = "";
				}
				if (tagName1=="Tabs" && sReader1.isStartElement())
				{
					ParagraphStyle::TabRecord tb;
					tb.tabPosition  = attrAsDbl(attrs1, "Pos");
					tb.tabType      = attrAsInt(attrs1, "Type");
					QStringRef tbCh = attrs1.value("Fill");
					if (tbCh.isEmpty())
						tb.tabFillChar = QChar();
					else
						tb.tabFillChar = tbCh.at(0);
					OB.TabValues.append(tb);
				}
				if (Fi && attrAsBool(attrs1, "relativePaths", false))
				{
					if (!OB.Pfile.isEmpty())
					{
						QFileInfo pfi(fileName);
						QString test = QDir::cleanPath(QDir::convertSeparators(pfi.absolutePath()+"/"+OB.Pfile));
						QFileInfo pfi2(test);
						OB.Pfile = pfi2.absoluteFilePath();
					}
					if (!OB.Pfile2.isEmpty())
					{
						QFileInfo pfi(fileName);
						QString test = QDir::cleanPath(QDir::convertSeparators(pfi.absolutePath()+"/"+OB.Pfile2));
						QFileInfo pfi2(test);
						OB.Pfile2 = pfi2.absoluteFilePath();
					}
					if (!OB.Pfile3.isEmpty())
					{
						QFileInfo pfi(fileName);
						QString test = QDir::cleanPath(QDir::convertSeparators(pfi.absolutePath()+"/"+OB.Pfile3));
						QFileInfo pfi2(test);
						OB.Pfile3 = pfi2.absoluteFilePath();
					}
				}
				if (tagName1 == "PatternItem" && sReader1.isEndElement())
				{
					view->PasteItem(&OB, true, true, false);
					PageItem* Neu = doc->Items->at(doc->Items->count()-1);
					Neu->setXYPos(Neu->xPos() - doc->currentPage()->xOffset(), Neu->yPos() - doc->currentPage()->yOffset(), true);
					for (int iElem = 0; iElem < iTextElems.count(); ++iElem)
						GetItemText(iTextElems.at(iElem), doc, VorLFound, true, Neu);
					Neu->effectsInUse = imageEffects;
					Neu->pixm.imgInfo.RequestProps = loadRequests;
					Neu->pixm.imgInfo.isRequest    = (loadRequests.count() > 0);
					if ((Neu->effectsInUse.count() != 0) || (Neu->pixm.imgInfo.RequestProps.count() != 0))
						doc->LoadPict(Neu->Pfile, Neu->ItemNr, true);
					if (Neu->pixm.imgInfo.PDSpathData.contains(patClipPath))
					{
						Neu->imageClip = Neu->pixm.imgInfo.PDSpathData[patClipPath].copy();
						Neu->pixm.imgInfo.usedPath = patClipPath;
						QMatrix cl;
						cl.translate(Neu->imageXOffset()*Neu->imageXScale(), Neu->imageYOffset()*Neu->imageYScale());
						cl.scale(Neu->imageXScale(), Neu->imageYScale());
						Neu->imageClip.map(cl);
					}
					if (Neu->isTableItem)
					{
						TableItems.append(Neu);
						TableID.insert(patOwnLink, Neu->ItemNr);
					}
					Neu->isGroupControl = isGroupControl;
					if (Neu->isGroupControl)
						groupID.insert(Neu, groupsLastItem + Neu->ItemNr);
					if (Neu->asPathText())
						Neu->updatePolyClip();
					imageEffects.clear();
					loadRequests.clear();
					iTextElems.clear();
				}
			}
			doc->useRaster = savedAlignGrid;
			doc->SnapGuides = savedAlignGuides;
			uint ae = doc->Items->count();
			if (ae > ac)
			{
				pat.setDoc(doc);
				PageItem* currItem = doc->Items->at(ac);
				pat.pattern = currItem->DrawObj_toImage();
				for (uint as = ac; as < ae; ++as)
				{
					PageItem* Neu = doc->Items->takeAt(ac);
					Neu->ItemNr = pat.items.count();
					pat.items.append(Neu);
				}
				pat.scaleX = patScaleX;
				pat.scaleY = patScaleY;
				pat.width  = patWidth;
				pat.height = patHeight;
				if (!doc->docPatterns.contains(patName))
					doc->docPatterns.insert(patName, pat);
			}
		}
	}

	sReader.clear();
	sReader.addData(ff);

	bool inItem = false;
	bool isGroupControl = false;
	int  groupsLastItem = 0;
	int  itemOwnLink    = 0;
	QString itemClip;
	while(!sReader.atEnd() && !sReader.hasError())
	{
		sReader.readNext();
		QString tagName = sReader.name().toString();
		QXmlStreamAttributes attrs = sReader.attributes();

		if (sReader.isStartElement() && tagName == "ITEM")
		{
			inItem = true;
			imageEffects.clear();
			loadRequests.clear();
			iTextElems.clear();
			GetItemProps(attrs, &OB, fileDir, newVersion);
			OB.Xpos = Xp + attrAsDbl(attrs, "XPOS", 0.0) - GrX;
			OB.Ypos = Yp + attrAsDbl(attrs, "YPOS", 0.0) - GrY;
			OB.startArrowIndex = arrowID[ attrAsInt(attrs, "startArrowIndex", 0) ];
			OB.endArrowIndex   = arrowID[ attrAsInt(attrs, "endArrowIndex", 0)];
			OB.isBookmark      = attrAsInt(attrs, "BOOKMARK");
			OB.NamedLStyle     = attrAsString(attrs, "NAMEDLST", "");
			isGroupControl     = attrAsBool(attrs, "isGroupControl", false);
			groupsLastItem     = attrAsInt (attrs, "groupsLastItem", 0);
			itemOwnLink        = attrAsInt (attrs, "OwnLINK", 0);
			itemClip           = attrAsString(attrs, "ImageClip", "");
			if (!doc->MLineStyles.contains(OB.NamedLStyle))
				OB.NamedLStyle = "";
			OB.itemText        = "";
			OB.textAlignment   = attrAsInt(attrs, "ALIGN", 0);
			tmf = attrAsString(attrs, "IFONT", doc->toolSettings.defFont);
			if (tmf.isEmpty())
				tmf = doc->toolSettings.defFont;
			if (DoFonts[tmf].isEmpty())
				OB.IFont = doc->toolSettings.defFont;
			else
				OB.IFont = DoFonts[tmf];
			OB.LayerNr   = 0;
			OB.Language  = attrAsString(attrs, "LANGUAGE", doc->Language);
			tmp = "";
			int numGroup = attrAsInt(attrs, "NUMGROUP", 0);
			if ((!attrs.value("GROUPS").isEmpty()) && (numGroup > 0))
			{
				tmp = attrAsString(attrs, "GROUPS", "");
				QTextStream fg(&tmp, QIODevice::ReadOnly);
				OB.Groups.clear();
				for (int cx = 0; cx < numGroup; ++cx)
				{
					fg >> x;
					OB.Groups.push(x+doc->GroupCounter);
					GrMax = qMax(GrMax, x+doc->GroupCounter);
				}
				tmp = "";
			}
			else
				OB.Groups.clear();
			tmp = "";
		}
		if (inItem && sReader.isStartElement() && tagName == "ITEXT")
		{
			iTextElems.append(sReader.attributes());
//			continue;
		}
		if (inItem && sReader.isStartElement() && tagName == "LATEX-SOURCE")
		{
			OB.itemText = sReader.readElementText();
		}
		if (inItem && sReader.isStartElement() && tagName == "ImageEffect")
		{
			struct ImageEffect ef;
			ef.effectParameters = attrAsString(attrs, "Param", "");
			ef.effectCode       = attrAsInt(attrs, "Code");
			imageEffects.append(ef);
		}
		if (inItem && sReader.isStartElement() && tagName == "PSDLayer")
		{
			struct ImageLoadRequest loadingInfo;
			loadingInfo.blend   = attrAsString(attrs, "Blend", "");
			loadingInfo.opacity = attrAsInt(attrs, "Opacity", 1);
			loadingInfo.visible = attrAsBool(attrs, "Visible", true);
			loadingInfo.useMask = attrAsBool(attrs, "useMask", true);
			loadRequests.insert( attrAsInt(attrs, "Layer"), loadingInfo);
		}
		if (inItem && sReader.isStartElement() && tagName == "CSTOP")
		{
			QString name = attrAsString(attrs, "NAME", "");
			double ramp  = attrAsDbl(attrs, "RAMP", 0.0);
			int shade    = attrAsInt(attrs, "SHADE", 100);
			double opa   = attrAsDbl(attrs, "TRANS", 1.0);
			OB.fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
			OB.GrColor   = "";
			OB.GrColor2  = "";
		}
		if (inItem && sReader.isStartElement() && tagName == "Tabs")
		{
			ParagraphStyle::TabRecord tb;
			tb.tabPosition  = attrAsDbl(attrs, "Pos", 0.0);
			tb.tabType      = attrAsInt(attrs,"Type");
			QStringRef tbCh = attrs.value("Fill");
			if (tbCh.isEmpty())
				tb.tabFillChar = QChar();
			else
				tb.tabFillChar = tbCh.at(0);
			OB.TabValues.append(tb);
		}
		if (inItem && Fi && attrAsBool(attrs, "relativePaths", false))
		{
			if (!OB.Pfile.isEmpty())
			{
				QFileInfo pfi(fileName);
				QString test = QDir::cleanPath(QDir::convertSeparators(pfi.absolutePath()+"/"+OB.Pfile));
				QFileInfo pfi2(test);
				OB.Pfile = pfi2.absoluteFilePath();
			}
			if (!OB.Pfile2.isEmpty())
			{
				QFileInfo pfi(fileName);
				QString test = QDir::cleanPath(QDir::convertSeparators(pfi.absolutePath()+"/"+OB.Pfile2));
				QFileInfo pfi2(test);
				OB.Pfile2 = pfi2.absoluteFilePath();
			}
			if (!OB.Pfile3.isEmpty())
			{
				QFileInfo pfi(fileName);
				QString test = QDir::cleanPath(QDir::convertSeparators(pfi.absolutePath()+"/"+OB.Pfile3));
				QFileInfo pfi2(test);
				OB.Pfile3 = pfi2.absoluteFilePath();
			}
		}
		if (sReader.isEndElement() && (tagName == "ITEM"))
		{
			view->PasteItem(&OB, true, true, false);
			PageItem* Neu = doc->Items->at(doc->Items->count()-1);
			for (int iElem = 0; iElem < iTextElems.count(); ++iElem)
				GetItemText(iTextElems.at(iElem), doc, VorLFound, true, Neu);
			Neu->effectsInUse = imageEffects;
			Neu->pixm.imgInfo.RequestProps = loadRequests;
			Neu->pixm.imgInfo.isRequest    = (loadRequests.count() > 0);
			if ((Neu->effectsInUse.count() != 0) || (Neu->pixm.imgInfo.RequestProps.count() != 0))
				doc->LoadPict(Neu->Pfile, Neu->ItemNr, true);
			if (Neu->pixm.imgInfo.PDSpathData.contains(itemClip))
			{
				Neu->imageClip = Neu->pixm.imgInfo.PDSpathData[itemClip].copy();
				Neu->pixm.imgInfo.usedPath = itemClip;
				QMatrix cl;
				cl.translate(Neu->imageXOffset()*Neu->imageXScale(), Neu->imageYOffset()*Neu->imageYScale());
				cl.scale(Neu->imageXScale(), Neu->imageYScale());
				Neu->imageClip.map(cl);
			}
			if (Neu->isTableItem)
			{
				TableItems.append(Neu);
				TableID.insert(itemOwnLink, Neu->ItemNr);
			}
			Neu->isGroupControl = isGroupControl;
			if (Neu->isGroupControl)
				groupID.insert(Neu, groupsLastItem + Neu->ItemNr );
			if (Neu->asPathText())
				Neu->updatePolyClip();
			imageEffects.clear();
			loadRequests.clear();
			iTextElems.clear();
			inItem = false;
		}
	}
	if (TableItems.count() != 0)
	{
		for (int ttc = 0; ttc < TableItems.count(); ++ttc)
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
	if (groupID.count() != 0)
	{
		QMap<PageItem*, int>::Iterator it;
		for (it = groupID.begin(); it != groupID.end(); ++it)
		{
			if (it.value() >= doc->Items->count())
			{
				it.key()->isGroupControl = false;
				continue;
			}
			it.key()->groupsLastItem = doc->Items->at(it.value());
		}
	}
	doc->GroupCounter = GrMax + 1;
	QDir::setCurrent(CurDirP);
	return (!sReader.hasError());
}

QString ScriXmlDoc::WriteElem(ScribusDoc *doc, ScribusView *view, Selection* selection)
{
	if (selection->count()==0)
		return "";
	double xp, yp;
	QString tmp, tmpy;
	PageItem *item;
	//QDomDocument docu("scribus");
	//QString st="<SCRIBUSELEMUTF8></SCRIBUSELEMUTF8>";.
	//docu.setContent(st);
	QString documentStr;
	QString baseDir = QDir::homePath();
	//QDomElement elem=docu.documentElement();
	item = selection->itemAt(0);
	QList<uint> ELL;
	for (int cor=0; cor<selection->count(); ++cor)
		ELL.append(selection->itemAt(cor)->ItemNr);
	qSort(ELL);
	documentStr.reserve(524288);
	QXmlStreamWriter writer(&documentStr);
//	writer.writeStartDocument();
//	don't use that function here, as it inserts an unneeded version info into that xml, which
//	breaks our reading code at several places.
	writer.writeStartElement("SCRIBUSELEMUTF8");
	if (selection->isMultipleSelection())
	{
		double gx, gy, gw, gh;
		selection->getGroupRect(&gx, &gy, &gw, &gh);
		xp = gx - doc->currentPage()->xOffset();
		yp = gy - doc->currentPage()->yOffset();
		writer.writeAttribute("W", QString::number(gw));
		writer.writeAttribute("H", QString::number(gh));
	}
	else
	{
		if (item->rotation() != 0)
		{
			double minx = 99999.9;
			double miny = 99999.9;
			double maxx = -99999.9;
			double maxy = -99999.9;
			double xpo = item->xPos() - doc->currentPage()->xOffset();
			double ypo = item->yPos() - doc->currentPage()->yOffset();
			FPointArray pb(4);
			pb.setPoint(0, FPoint(xpo, ypo));
			pb.setPoint(1, FPoint(item->width(), 0.0, xpo, ypo, item->rotation(), 1.0, 1.0));
			pb.setPoint(2, FPoint(item->width(), item->height(), xpo, ypo, item->rotation(), 1.0, 1.0));
			pb.setPoint(3, FPoint(0.0, item->height(), xpo, ypo, item->rotation(), 1.0, 1.0));
			for (uint pc = 0; pc < 4; ++pc)
			{
				minx = qMin(minx, pb.point(pc).x());
				miny = qMin(miny, pb.point(pc).y());
				maxx = qMax(maxx, pb.point(pc).x());
				maxy = qMax(maxy, pb.point(pc).y());
			}
			writer.writeAttribute("W", QString::number(maxx - minx));
			writer.writeAttribute("H", QString::number(maxy - miny));
		}
		else
		{
			writer.writeAttribute("W", QString::number(item->width()));
			writer.writeAttribute("H", QString::number(item->height()));
		}
		xp = item->xPos() - doc->currentPage()->xOffset();
		yp = item->yPos() - doc->currentPage()->yOffset();
	}
	writer.writeAttribute("XP", QString::number(xp));
	writer.writeAttribute("YP", QString::number(yp));
	writer.writeAttribute("COUNT",   QString::number(selection->count()));
	writer.writeAttribute("Version", QString(VERSION));
	QMap<QString,int>::Iterator itf;
	for (itf = doc->UsedFonts.begin(); itf != doc->UsedFonts.end(); ++itf)
	{
		writer.writeStartElement("FONT");
		writer.writeAttribute("NAME",itf.key());
		writer.writeEndElement();
	}
	ColorList::Iterator itc;
	for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
	{
		writer.writeStartElement("COLOR");
		writer.writeAttribute("NAME",itc.key());
		if (itc.value().getColorModel() == colorModelRGB)
			writer.writeAttribute("RGB" , itc.value().nameRGB());
		else
			writer.writeAttribute("CMYK", itc.value().nameCMYK());
		writer.writeAttribute("Spot"    , itc.value().isSpotColor() ? "1" : "0");
		writer.writeAttribute("Register", itc.value().isRegistrationColor() ? "1" : "0");
		writer.writeEndElement();
	}
	QMap<int, ParagraphStyle> UsedStyles;
	QMap<int, int> UsedMapped2Saved;
//	int NewStylesNum = 5;
	UsedStyles.clear();
	UsedMapped2Saved.clear();
/*
	ParagraphStyle vg;
//	if (doc->docParagraphStyles.count() > 5)
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
			fo.setAttribute("BASE", static_cast<int>(UsedStyles[actSt].lineSpacingMode() == ParagraphStyle::BaselineGridSpacing));
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
 */
	QMap<QString,multiLine>::Iterator itMU;
	for (itMU = doc->MLineStyles.begin(); itMU != doc->MLineStyles.end(); ++itMU)
	{
		writer.writeStartElement("MultiLine");
		writer.writeAttribute("Name",itMU.key());
		multiLine ml = itMU.value();
		multiLine::iterator itMU2;
		for (itMU2 = ml.begin(); itMU2 != ml.end(); ++itMU2)
		{
			writer.writeStartElement("SubLine");
			writer.writeAttribute("Color"   , (*itMU2).Color);
			writer.writeAttribute("Shade"   , QString::number((*itMU2).Shade));
			writer.writeAttribute("Dash"    , QString::number((*itMU2).Dash));
			writer.writeAttribute("LineEnd" , QString::number((*itMU2).LineEnd));
			writer.writeAttribute("LineJoin", QString::number((*itMU2).LineJoin));
			writer.writeAttribute("Width"   , QString::number((*itMU2).Width));
			writer.writeEndElement();
		}
		writer.writeEndElement();
	}
	QMap<int, ArrowDesc> usedArrows;
	QMap<int, ArrowDesc>::Iterator itar;
	struct ArrowDesc arrow;
	//for (uint co=0; co<Selitems->count(); ++co)
	for (int co=0; co<selection->count(); ++co)
	{
		item = doc->Items->at(ELL[co]);
		int startIndex = item->startArrowIndex();
		int endIndex = item->endArrowIndex();
		if (startIndex != 0)
		{
			arrow.points = doc->arrowStyles.at(startIndex-1).points.copy();
			arrow.name = doc->arrowStyles.at(startIndex-1).name;
			usedArrows.insert(startIndex, arrow);
		}
		if (endIndex != 0)
		{
			arrow.points = doc->arrowStyles.at(endIndex-1).points.copy();
			arrow.name = doc->arrowStyles.at(endIndex-1).name;
			usedArrows.insert(endIndex, arrow);
		}
	}
	if (usedArrows.count() != 0)
	{
		for (itar = usedArrows.begin(); itar != usedArrows.end(); ++itar)
		{
			if (itar.value().userArrow)
			{
				writer.writeStartElement("Arrows");
				writer.writeAttribute("NumPoints", QString::number(itar.value().points.size()));
				QString arp = "";
				double xa, ya;
				for (uint nxx = 0; nxx < itar.value().points.size(); ++nxx)
				{
					itar.value().points.point(nxx, &xa, &ya);
					arp += tmp.setNum(xa) + " " + tmpy.setNum(ya) + " ";
				}
				writer.writeAttribute("Points", arp);
				writer.writeAttribute("Name"  , itar.value().name);
				writer.writeAttribute("Index" , QString::number(itar.key()));
				writer.writeEndElement();
			}
		}
	}
	QStringList patterns = doc->getUsedPatternsSelection();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern& pa = doc->docPatterns[patterns[c]];
		writer.writeStartElement("Pattern");
		writer.writeAttribute ("Name"  , patterns[c]);
		writer.writeAttribute ("scaleX", QString::number(pa.scaleX));
		writer.writeAttribute ("scaleY", QString::number(pa.scaleY));
		writer.writeAttribute("width" , QString::number(pa.width));
		writer.writeAttribute ("height", QString::number(pa.height));
		for (int o = 0; o < pa.items.count(); o++)
		{
			QDir::setCurrent(QDir::homePath());
			item = pa.items.at(o);
			writer.writeStartElement("PatternItem");
			WriteObject(writer, doc, item, baseDir, UsedMapped2Saved);
			writer.writeEndElement();
		}
		writer.writeEndElement();
	}
	for (int co=0; co<selection->count(); ++co)
	{
		QDir::setCurrent(QDir::homePath());
		item = doc->Items->at(ELL[co]);
		writer.writeStartElement("ITEM");
		WriteObject(writer, doc, item, baseDir, UsedMapped2Saved);
		writer.writeEndElement();
	}
	writer.writeEndElement();
	writer.writeEndDocument();
	return documentStr;
}

void ScriXmlDoc::WriteObject(QXmlStreamWriter& writer, ScribusDoc *doc, PageItem *item, const QString& baseDir, 
							 QMap<int, int> &UsedMapped2Saved)
{
	int tst, tst2;
	double te, tsh, tshs, te2, tsh2, tshs2;
	QString text, tf, tf2, tc, tc2, tcs, tcs2, tmp, tmpy;
	double ts, ts2, tsc, tsc2, tscv, tscv2, tb, tb2, tsx, tsx2, tsy, tsy2, tout, tout2, tulp, tulp2, tulw, tulw2, tstp, tstp2, tstw, tstw2;
	QString CurDirP = QDir::currentPath();
	
	const ParagraphStyle& defaultStyle = item->itemText.defaultStyle();

	int textAlignment = item->itemText.defaultStyle().alignment();
	writer.writeAttribute("ALIGN", QString::number(textAlignment));
	SetItemProps(writer, doc, item, baseDir, false);
	//writer.writeAttribute("LOCK", "0");
	//writer.writeAttribute("XPOS",  QString::number(item->xPos() - doc->currentPage()->xOffset()));
	//writer.writeAttribute("YPOS",  QString::number(item->yPos() - doc->currentPage()->yOffset()));
	writer.writeAttribute("BOOKMARK"   , item->isBookmark ? "1" : "0");
	writer.writeAttribute("fillRule"   , item->fillRule ? "1" : "0");
	writer.writeAttribute("doOverprint", item->doOverprint ? "1" : "0");
	writer.writeAttribute("ImageClip"  , item->pixm.imgInfo.usedPath);

	writer.writeAttribute("NUMTEXT", QString::number(item->itemText.length()));
	QString txnu = "";
	for(int kt = 0; kt < item->itemText.length(); ++kt)
#ifndef NLS_PROTO
		txnu += tmp.setNum(item->itemText.item(kt)->glyph.xoffset) + " " + tmpy.setNum(item->itemText.item(kt)->glyph.yoffset) + " ";
#else
		txnu += "0 0 ";
#endif
	writer.writeAttribute("TEXTCOOR", txnu);
	writer.writeAttribute("BACKITEM", "-1");
	writer.writeAttribute("BACKPAGE", "-1");
	writer.writeAttribute("NEXTITEM", "-1");
	writer.writeAttribute("NEXTPAGE", "-1");

	if (item->GrType != 0 && item->GrType != 8)
	{
		writer.writeAttribute("GRSTARTX", QString::number(item->GrStartX));
		writer.writeAttribute("GRSTARTY", QString::number(item->GrStartY));
		writer.writeAttribute("GRENDX"  , QString::number(item->GrEndX));
		writer.writeAttribute("GRENDY"  , QString::number(item->GrEndY));
	}

	if (item->effectsInUse.count() != 0)
	{
		for (int a = 0; a < item->effectsInUse.count(); ++a)
		{
			writer.writeStartElement("ImageEffect");
			writer.writeAttribute("Code" , QString::number(item->effectsInUse.at(a).effectCode));
			writer.writeAttribute("Param", item->effectsInUse.at(a).effectParameters);
			writer.writeEndElement();
		}
	}
	if (item->itemText.defaultStyle().tabValues().count() != 0)
	{
		for (int a = 0; a < item->itemText.defaultStyle().tabValues().count(); ++a)
		{
			writer.writeStartElement("Tabs");
			writer.writeAttribute("Type", QString::number((defaultStyle.tabValues().at(a)).tabType));
			writer.writeAttribute("Pos" , QString::number((defaultStyle.tabValues().at(a)).tabPosition));
			QString tabCh = "";
			if (!(defaultStyle.tabValues().at(a)).tabFillChar.isNull())
				tabCh = QString((defaultStyle.tabValues().at(a)).tabFillChar);
			writer.writeAttribute("Fill", tabCh);
			writer.writeEndElement();
		}
	}
	if (((item->asImageFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()) && (item->pixm.imgInfo.layerInfo.count() != 0) && (item->pixm.imgInfo.isRequest))
	{
		QMap<int, ImageLoadRequest>::iterator it2;
		for (it2 = item->pixm.imgInfo.RequestProps.begin(); it2 != item->pixm.imgInfo.RequestProps.end(); ++it2)
		{
			writer.writeStartElement("PSDLayer");
			writer.writeAttribute("Layer"  , QString::number(it2.key()));
			writer.writeAttribute("Visible", it2.value().visible ? "1" : "0");
			writer.writeAttribute("useMask", it2.value().useMask ? "1" : "0");
			writer.writeAttribute("Opacity", QString::number(it2.value().opacity) );
			writer.writeAttribute("Blend"  , it2.value().blend);
			writer.writeEndElement();
		}
	}
	if (item->GrType != 0)
	{
		if (item->GrType == 8)
		{
			writer.writeAttribute("pattern", item->pattern());
			double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
			item->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
			writer.writeAttribute("pScaleX"  , QString::number(patternScaleX));
			writer.writeAttribute("pScaleY"  , QString::number(patternScaleY));
			writer.writeAttribute("pOffsetX" , QString::number(patternOffsetX));
			writer.writeAttribute("pOffsetY" , QString::number(patternOffsetY));
			writer.writeAttribute("pRotation", QString::number(patternRotation));
		}
		else
		{
			QList<VColorStop*> cstops = item->fill_gradient.colorStops();
			for (uint cst = 0; cst < item->fill_gradient.Stops(); ++cst)
			{
				writer.writeStartElement("CSTOP");
				writer.writeAttribute("RAMP" , QString::number(cstops.at(cst)->rampPoint));
				writer.writeAttribute("NAME" , cstops.at(cst)->name);
				writer.writeAttribute("SHADE", QString::number(cstops.at(cst)->shade));
				writer.writeAttribute("TRANS", QString::number(cstops.at(cst)->opacity));
				writer.writeEndElement();
			}
			// must be writen before
			// writer.writeAttribute("GRSTARTX", QString::number(item->GrStartX));
			// writer.writeAttribute("GRSTARTY", QString::number(item->GrStartY));
			// writer.writeAttribute("GRENDX"  , QString::number(item->GrEndX));
			// writer.writeAttribute("GRENDY"  , QString::number(item->GrEndY));
		}
	}
	if (item->asLatexFrame()) {
		writer.writeTextElement("LATEX-SOURCE", item->asLatexFrame()->formula());
	}
	QDir::setCurrent(CurDirP);
	for(int k=0;k<item->itemText.length();++k)
	{
		const CharStyle& style4(item->itemText.charStyle(k));
		QChar ch = item->itemText.text(k);
		writer.writeStartElement("ITEXT");
		ts = style4.fontSize() / 10.0;
		tf = style4.font().scName();
		tc = style4.fillColor();
		te = style4.tracking();
		tsh = style4.fillShade();
		tst = style4.effects() & 2047;
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
		// QChar(5) and QChar(4) trigger QXmlStreamReader errors
		/*if (ch == SpecialChars::PARSEP)
			text = QChar(5);
		else if (ch == SpecialChars::TAB)
			text = QChar(4);
		else
			text = ch;*/
		text = ch;
		QString pstylename = "";
		if (ch == SpecialChars::PARSEP)
			pstylename = item->itemText.paragraphStyle(k).parent();
		++k;
		if (k == item->itemText.length())
		{
			writer.writeAttribute("CH"     , text);
			writer.writeAttribute("CSIZE"  , QString::number(ts));
			writer.writeAttribute("CFONT"  , tf);
			writer.writeAttribute("CCOLOR" , tc);
			writer.writeAttribute("CKERN"  , QString::number(te));
			writer.writeAttribute("CSHADE" , QString::number(tsh));
			writer.writeAttribute("CSTYLE" , QString::number(tst));
			writer.writeAttribute("CSTROKE", tcs);
			writer.writeAttribute("CSHADE2", QString::number(tshs));
			writer.writeAttribute("CSCALE" , QString::number(tsc));
			writer.writeAttribute("CSCALEV", QString::number(tscv));
			writer.writeAttribute("CBASE"  , QString::number(tb));
			writer.writeAttribute("CSHX"   , QString::number(tsx));
			writer.writeAttribute("CSHY"   , QString::number(tsy));
			writer.writeAttribute("COUT"   , QString::number(tout));
			writer.writeAttribute("CULP"   , QString::number(tulp));
			writer.writeAttribute("CULW"   , QString::number(tulw));
			writer.writeAttribute("CSTP"   , QString::number(tstp));
			writer.writeAttribute("CSTW"   , QString::number(tstw));
			writer.writeAttribute("PSTYLE" , pstylename);
			writer.writeEndElement();
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
		QString pstylename2 = "";
		if (ch == SpecialChars::PARSEP)
			pstylename2 = item->itemText.paragraphStyle(k).parent();
		while ((ts2 == ts)
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
						&& (tst2 == tst)
			   && (pstylename == pstylename2))
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
			pstylename2 = "";
			if (ch == SpecialChars::PARSEP)
				pstylename2 = item->itemText.paragraphStyle(k).parent();
		}
		writer.writeAttribute("CH"     ,text);
		writer.writeAttribute("CSIZE"  ,QString::number(ts));
		writer.writeAttribute("CFONT"  ,tf);
		writer.writeAttribute("CCOLOR" ,tc);
		writer.writeAttribute("CKERN"  ,QString::number(te));
		writer.writeAttribute("CSHADE" ,QString::number(tsh));
		writer.writeAttribute("CSTYLE" ,QString::number(tst));
		writer.writeAttribute("CSTROKE",tcs);
		writer.writeAttribute("CSHADE2",QString::number(tshs));
		writer.writeAttribute("CSCALE" ,QString::number(tsc));
		writer.writeAttribute("CSCALEV",QString::number(tscv));
		writer.writeAttribute("CBASE"  ,QString::number(tb));
		writer.writeAttribute("CSHX"   ,QString::number(tsx));
		writer.writeAttribute("CSHY"   ,QString::number(tsy));
		writer.writeAttribute("COUT"   ,QString::number(tout));
		writer.writeAttribute("CULP"   ,QString::number(tulp));
		writer.writeAttribute("CULW"   ,QString::number(tulw));
		writer.writeAttribute("CSTP"   ,QString::number(tstp));
		writer.writeAttribute("CSTW"   ,QString::number(tstw));
		writer.writeAttribute("PSTYLE" ,pstylename);
		k--;
		writer.writeEndElement();
	}
}
