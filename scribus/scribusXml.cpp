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
//#include "scribusXml.moc"

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

bool ScriXmlDoc::attrAsBool(const QXmlStreamAttributes& attrs, const QString& attName, bool defVal)
{
	bool value = defVal;
	QStringRef att = attrs.value(attName);
	if (!att.isEmpty())
		value = static_cast<bool>(att.toString().toInt());
	return value;
}

int ScriXmlDoc::attrAsInt(const QXmlStreamAttributes& attrs, const QString& attName, int defVal)
{
	int value = defVal;
	QStringRef att = attrs.value(attName);
	if (!att.isEmpty())
		value = att.toString().toInt();
	return value;
}

double ScriXmlDoc::attrAsDbl(const QXmlStreamAttributes& attrs, const QString& attName, double defVal)
{
	double value = defVal;
	QStringRef att = attrs.value(attName);
	if (!att.isEmpty())
		value = att.toString().toDouble();
	return value;
}

QString ScriXmlDoc::attrAsString (const QXmlStreamAttributes& attrs, const QString& attName, const QString& defVal)
{
	QStringRef att = attrs.value(attName);
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
		OB->m_annotation.setExtern(efp.absFilePath());
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
	OB->Pfile      = Relative2Path( attrAsString(attrs, "PFILE" ,""), baseDir);
	OB->Pfile2     = Relative2Path( attrAsString(attrs, "PFILE2",""), baseDir);
	OB->Pfile3     = Relative2Path( attrAsString(attrs, "PFILE3",""), baseDir);
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

void ScriXmlDoc::SetItemProps(QDomElement *ob, PageItem* item, const QString& baseDir, bool newFormat)
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
	ob->setAttribute("textPathType", item->textPathType);
	ob->setAttribute("textPathFlipped", static_cast<int>(item->textPathFlipped));
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
	if (((item->asImageFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
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
	ob->setAttribute("LANGUAGE", item->itemText.defaultStyle().charStyle().language());
	ob->setAttribute("startArrowIndex", item->startArrowIndex());
	ob->setAttribute("endArrowIndex", item->endArrowIndex());
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
		fileDir = QFileInfo(fileName).dirPath(true);
	}
	else
	{
		ff = fileName;
	}

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
			int     patOwnLink, groupsLastItem;
			QString patText   = sReader.readElementText();
			QXmlStreamReader sReader1(patText);
			while(!sReader1.atEnd() && !sReader1.hasError())
			{
 				QXmlStreamReader::TokenType tType = sReader1.readNext();
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
					struct ScImage::imageEffect ef;
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
						QString test = QDir::cleanPath(QDir::convertSeparators(pfi.dirPath(true)+"/"+OB.Pfile));
						QFileInfo pfi2(test);
						OB.Pfile = pfi2.absFilePath();
					}
					if (!OB.Pfile2.isEmpty())
					{
						QFileInfo pfi(fileName);
						QString test = QDir::cleanPath(QDir::convertSeparators(pfi.dirPath(true)+"/"+OB.Pfile2));
						QFileInfo pfi2(test);
						OB.Pfile2 = pfi2.absFilePath();
					}
					if (!OB.Pfile3.isEmpty())
					{
						QFileInfo pfi(fileName);
						QString test = QDir::cleanPath(QDir::convertSeparators(pfi.dirPath(true)+"/"+OB.Pfile3));
						QFileInfo pfi2(test);
						OB.Pfile3 = pfi2.absFilePath();
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
 		QXmlStreamReader::TokenType tType = sReader.readNext();
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
			continue;
		}
		if (inItem && sReader.isStartElement() && tagName == "ImageEffect")
		{
			struct ScImage::imageEffect ef;
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
				QString test = QDir::cleanPath(QDir::convertSeparators(pfi.dirPath(true)+"/"+OB.Pfile));
				QFileInfo pfi2(test);
				OB.Pfile = pfi2.absFilePath();
			}
			if (!OB.Pfile2.isEmpty())
			{
				QFileInfo pfi(fileName);
				QString test = QDir::cleanPath(QDir::convertSeparators(pfi.dirPath(true)+"/"+OB.Pfile2));
				QFileInfo pfi2(test);
				OB.Pfile2 = pfi2.absFilePath();
			}
			if (!OB.Pfile3.isEmpty())
			{
				QFileInfo pfi(fileName);
				QString test = QDir::cleanPath(QDir::convertSeparators(pfi.dirPath(true)+"/"+OB.Pfile3));
				QFileInfo pfi2(test);
				OB.Pfile3 = pfi2.absFilePath();
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
				groupID.insert(Neu, groupsLastItem +Neu->ItemNr );
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
			it.key()->groupsLastItem = doc->Items->at(it.data());
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
	QDomDocument docu("scribus");
	QString st="<SCRIBUSELEMUTF8></SCRIBUSELEMUTF8>";
	QString baseDir = QDir::homePath();
	docu.setContent(st);
	QDomElement elem=docu.documentElement();
	item = selection->itemAt(0);
	QList<uint> ELL;
	for (int cor=0; cor<selection->count(); ++cor)
		ELL.append(selection->itemAt(cor)->ItemNr);
	qSort(ELL);
	if (selection->isMultipleSelection())
	{
		double gx, gy, gw, gh;
		selection->getGroupRect(&gx, &gy, &gw, &gh);
		xp = gx - doc->currentPage()->xOffset();
		yp = gy - doc->currentPage()->yOffset();
		elem.setAttribute("W", gw);
		elem.setAttribute("H", gh);
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
			elem.setAttribute("W", maxx - minx);
			elem.setAttribute("H", maxy - miny);
		}
		else
		{
			elem.setAttribute("W", item->width());
			elem.setAttribute("H", item->height());
		}
		xp = item->xPos() - doc->currentPage()->xOffset();
		yp = item->yPos() - doc->currentPage()->yOffset();
	}
	elem.setAttribute("XP", xp);
	elem.setAttribute("YP", yp);
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
	QStringList patterns = doc->getUsedPatternsSelection();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern& pa = doc->docPatterns[patterns[c]];
		QDomElement pat = docu.createElement("Pattern");
		pat.setAttribute("Name", patterns[c]);
		pat.setAttribute("scaleX", pa.scaleX);
		pat.setAttribute("scaleY", pa.scaleY);
		pat.setAttribute("width", pa.width);
		pat.setAttribute("height", pa.height);
		for (int o = 0; o < pa.items.count(); o++)
		{
			QDir::setCurrent(QDir::homePath());
			item = pa.items.at(o);
			QDomElement ob = docu.createElement("PatternItem");
			WriteObject(doc, docu, ob, baseDir, UsedMapped2Saved, item);
			pat.appendChild(ob);
		}
		elem.appendChild(pat);
	}
	for (int co=0; co<selection->count(); ++co)
	{
		QDir::setCurrent(QDir::homePath());
		item = doc->Items->at(ELL[co]);
		QDomElement ob = docu.createElement("ITEM");
		WriteObject(doc, docu, ob, baseDir, UsedMapped2Saved, item);
		elem.appendChild(ob);
	}
	return docu.toString();
}

void ScriXmlDoc::WriteObject(ScribusDoc *doc, QDomDocument &docu, QDomElement &ob, const QString& baseDir, 
							 QMap<int, int> &UsedMapped2Saved, PageItem *item)
{
	int tst, tst2;
	double te, tsh, tshs, te2, tsh2, tshs2;
	QString text, tf, tf2, tc, tc2, tcs, tcs2, tmp, tmpy;
	double ts, ts2, tsc, tsc2, tscv, tscv2, tb, tb2, tsx, tsx2, tsy, tsy2, tout, tout2, tulp, tulp2, tulw, tulw2, tstp, tstp2, tstw, tstw2;
	QString CurDirP = QDir::currentPath();

	int textAlignment = item->itemText.defaultStyle().alignment();
	ob.setAttribute("ALIGN",textAlignment);
	SetItemProps(&ob, item, baseDir, false);
	ob.setAttribute("LOCK", 0);
	ob.setAttribute("XPOS",item->xPos() - doc->currentPage()->xOffset());
	ob.setAttribute("YPOS",item->yPos() - doc->currentPage()->yOffset());
	ob.setAttribute("BOOKMARK", item->isBookmark ? 1 : 0);
	ob.setAttribute("fillRule", static_cast<int>(item->fillRule));
	ob.setAttribute("doOverprint", static_cast<int>(item->doOverprint));
	ob.setAttribute("ImageClip", item->pixm.imgInfo.usedPath);
	if (item->effectsInUse.count() != 0)
	{
		for (int a = 0; a < item->effectsInUse.count(); ++a)
		{
			QDomElement imeff = docu.createElement("ImageEffect");
			imeff.setAttribute("Code", item->effectsInUse.at(a).effectCode);
			imeff.setAttribute("Param", item->effectsInUse.at(a).effectParameters);
			ob.appendChild(imeff);
		}
	}
	if (item->itemText.defaultStyle().tabValues().count() != 0)
	{
		for (int a = 0; a < item->itemText.defaultStyle().tabValues().count(); ++a)
		{
			QDomElement tabs = docu.createElement("Tabs");
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
			QDomElement psd = docu.createElement("PSDLayer");
			psd.setAttribute("Layer",it2.key());
			psd.setAttribute("Visible", static_cast<int>(it2.data().visible));
			psd.setAttribute("useMask", static_cast<int>(it2.data().useMask));
			psd.setAttribute("Opacity", it2.data().opacity);
			psd.setAttribute("Blend", it2.data().blend);
			ob.appendChild(psd);
		}
	}
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
		QString pstylename = "";
		if (ch == SpecialChars::PARSEP)
			pstylename = item->itemText.paragraphStyle(k).parent();
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
			it.setAttribute("PSTYLE",pstylename);
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
		it.setAttribute("CH",text);
		it.setAttribute("CSIZE",ts);
		it.setAttribute("CFONT",tf);
		it.setAttribute("CCOLOR",tc);
		it.setAttribute("CKERN",te);
		it.setAttribute("CSHADE",tsh);
		it.setAttribute("CSTYLE",tst);
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
		it.setAttribute("PSTYLE",pstylename);
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
}
