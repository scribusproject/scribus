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
#include "util_text.h"
#include "util_math.h"
#include "util_color.h"
#include "scpattern.h"
#include "scxmlstreamwriter.h"

using namespace std;

ScriXmlDoc::ScriXmlDoc()
{
	prefsManager=PrefsManager::instance();
}

bool ScriXmlDoc::attrHasValue(const QXmlStreamAttributes& attrs, const char* attName)
{
	QLatin1String attString(attName);
	bool hasValue = false;
	QXmlStreamAttributes::ConstIterator pIter, pEnd  = attrs.constEnd();
	for (pIter = attrs.constBegin(); pIter != pEnd; ++pIter)
	{
		if (pIter->name() == attString)
		{
			hasValue = true;
			break;
		}
	}
	return hasValue;
}

bool ScriXmlDoc::attrAsBool(const QXmlStreamAttributes& attrs, const char* attName, bool defVal)
{
	QLatin1String attString(attName);
	bool value = defVal;
	QStringRef att = attrs.value(attString);
	if (!att.isEmpty())
		value = static_cast<bool>(QString::fromRawData(att.constData(), att.length()).toInt());
	return value;
}

int ScriXmlDoc::attrAsInt(const QXmlStreamAttributes& attrs, const char* attName, int defVal)
{
	QLatin1String attString(attName);
	int value = defVal;
	QStringRef att = attrs.value(attString);
	if (!att.isEmpty())
		value = QString::fromRawData(att.constData(), att.length()).toInt();
	return value;
}

double ScriXmlDoc::attrAsDbl(const QXmlStreamAttributes& attrs, const char* attName, double defVal)
{
	QLatin1String attString(attName);
	double value = defVal;
	QStringRef att = attrs.value(attString);
	if (!att.isEmpty())
		value = QString::fromRawData(att.constData(), att.length()).toDouble();
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
	OB->firstLineOffsetP = static_cast<FirstLineOffsetPolicy>(attrAsInt(attrs, "FLOP", 0));
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

void ScriXmlDoc::GetItemText(const QXmlStreamAttributes& attrs, StoryText& story, ScribusDoc *doc, LastStyles* last, bool VorLFound, bool impo)
{
	CharStyle newStyle;
	QString pstylename;
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
//		tmpf = ReplacedFonts[tmpf];
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

	if (unknown)
		newStyle.setFont(dummy);
	else
		newStyle.setFont((*doc->AllFonts)[tmpf]);
	
	if (attrHasValue(attrs, "CSIZE"))
		newStyle.setFontSize( qRound(attrAsDbl(attrs, "CSIZE") * 10) );

	if (attrHasValue(attrs, "CCOLOR"))
		newStyle.setFillColor( attrAsString(attrs, "CCOLOR", CommonStrings::None) );

	if (attrHasValue(attrs, "CSHADE"))
		newStyle.setFillShade( attrAsInt(attrs, "CSHADE") );

	if (attrHasValue(attrs, "CEXTRA"))
		newStyle.setTracking( qRound(attrAsDbl(attrs, "CEXTRA") / attrAsDbl(attrs, "CSIZE", 1.0) * 1000.0) );
	else if (attrHasValue(attrs, "CKERN"))
		newStyle.setTracking( attrAsInt(attrs, "CKERN") );

	if (attrHasValue(attrs, "CSTROKE"))
		newStyle.setStrokeColor( attrAsString(attrs, "CSTROKE", CommonStrings::None) );

	if (attrHasValue(attrs, "CSHADE2"))
		newStyle.setStrokeShade( attrAsInt(attrs, "CSHADE2", 100) );

	if (attrHasValue(attrs, "CSCALE"))
	{
		int scaleh = qRound(attrAsDbl(attrs, "CSCALE", 100.0) * 10);
		newStyle.setScaleH(qMin(qMax(scaleh, 100), 4000));
	}

	if (attrHasValue(attrs, "CSCALEV"))
	{
		int scalev = qRound(attrAsDbl(attrs, "CSCALEV", 100.0) * 10);
		newStyle.setScaleV(qMin(qMax(scalev, 100), 4000));
	}

	if (attrHasValue(attrs, "CBASE"))
		newStyle.setBaselineOffset( qRound(attrAsDbl(attrs, "CBASE", 0.0) * 10) );

	if (attrHasValue(attrs, "CSHX"))
		newStyle.setShadowXOffset( qRound(attrAsDbl(attrs, "CSHX", 5.0) * 10) );

	if (attrHasValue(attrs, "CSHY"))
		newStyle.setShadowYOffset( qRound(attrAsDbl(attrs, "CSHY", -5.0) * 10) );

	if (attrHasValue(attrs, "COUT"))
		newStyle.setOutlineWidth( qRound(attrAsDbl(attrs, "COUT", 1.0) * 10) );

	if (attrHasValue(attrs, "CULP"))
		newStyle.setUnderlineOffset( qRound(attrAsDbl(attrs, "CULP", -0.1) * 10) );

	if (attrHasValue(attrs, "CULW"))
		newStyle.setUnderlineWidth( qRound(attrAsDbl(attrs, "CULW", -0.1) * 10) );

	if (attrHasValue(attrs, "CSTP"))
		newStyle.setStrikethruOffset( qRound(attrAsDbl(attrs, "CSTP", -0.1) * 10) );

	if (attrHasValue(attrs, "CSTW"))
		newStyle.setStrikethruWidth( qRound(attrAsDbl(attrs, "CSTW", -0.1) * 10) );

	if (attrHasValue(attrs, "CSTYLE"))
	{
		int cstyle = attrAsInt(attrs, "CSTYLE") & 2047;
		newStyle.setFeatures(static_cast<StyleFlag>(cstyle).featureList());
	}
	
	QString pstyleName = attrAsString(attrs, "PSTYLE", "");
	int calign = attrAsInt(attrs, "CALIGN", -1);	
	int cab    = attrAsInt(attrs, "CAB", -1);
	if (cab >= 5) {
		pstylename = DoVorl[cab];
		calign = -1;
	}
	else if (cab >= 0) {
		pstylename = "";
		calign = cab;
	}

	for (int cxx=0; cxx<tmp2.length(); ++cxx)
	{
		CharStyle style;
		QChar ch = tmp2.at(cxx);
		if (ch == QChar(5))
			ch = SpecialChars::PARSEP;
		if (ch == QChar(4))
			ch = SpecialChars::TAB;

		if (impo && cab >= 0 && VorLFound)
			last->ParaStyle = DoVorl[cab];
		else
			last->ParaStyle = pstylename;

		int pos = story.length();
		story.insertChars(pos, QString(ch));

		if (newStyle != last->Style) {
			story.applyCharStyle(last->StyleStart, pos-last->StyleStart, last->Style);
			last->Style = newStyle;
			last->StyleStart = pos;
		}
		if (ch == SpecialChars::PARSEP)
		{
			ParagraphStyle pstyle;
			if (!pstyleName.isEmpty())
				pstyle.setParent(pstyleName);
			if (calign >= 0)
				pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(calign));
			story.applyStyle(pos, pstyle);
		}
		else
			story.applyCharStyle(pos, 1, style);
	}
	story.applyCharStyle(last->StyleStart, story.length()-last->StyleStart, last->Style);
	ParagraphStyle pstyle;
	if (!last->ParaStyle.isEmpty()) 
	{
		pstyle.setParent( last->ParaStyle );
		story.applyStyle(story.length()-1, pstyle);
	}
	if (calign >= 0)
	{
		pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(calign));
		story.applyStyle(story.length()-1, pstyle);
	}
	last->StyleStart = story.length();
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

void ScriXmlDoc::SetItemProps(ScXmlStreamWriter& writer, ScribusDoc *doc, PageItem* item, const QString& baseDir, bool newFormat)
{
	double xf, yf;
	QString tmp, tmpy;
	if (newFormat)
		writer.writeAttribute("OwnPage", item->OwnPage);
	const ParagraphStyle& defaultStyle = item->itemText.defaultStyle();
	const CharStyle&      charStyle    = item->itemText.defaultStyle().charStyle();
	writer.writeAttribute("PTYPE"    ,item->realItemType());
	writer.writeAttribute("XPOS"     ,item->xPos() - doc->currentPage()->xOffset());
	writer.writeAttribute("YPOS"     ,item->yPos() - doc->currentPage()->yOffset());
	writer.writeAttribute("WIDTH"    ,item->width());
	writer.writeAttribute("HEIGHT"   ,item->height());
	writer.writeAttribute("RADRECT"  ,item->cornerRadius());
	writer.writeAttribute("FRTYPE"   ,item->FrameType);
	writer.writeAttribute("CLIPEDIT" ,item->ClipEdited ? "1" : "0");
	writer.writeAttribute("PWIDTH"   ,item->lineWidth());
	writer.writeAttribute("PCOLOR"   ,item->fillColor());
	writer.writeAttribute("PCOLOR2"  ,item->lineColor());
	writer.writeAttribute("IFONT"    ,charStyle.font().scName());
	writer.writeAttribute("ISIZE"    ,charStyle.fontSize() / 10.0);
	writer.writeAttribute("TXTFILL"  ,charStyle.fillColor());
	writer.writeAttribute("TXTSTROKE",charStyle.strokeColor());
	writer.writeAttribute("TXTSTRSH" ,charStyle.strokeShade());
	writer.writeAttribute("TXTFILLSH",charStyle.fillShade());
	writer.writeAttribute("TXTSCALE" ,charStyle.scaleH() / 10.0);
	writer.writeAttribute("TXTSCALEV",charStyle.scaleV() / 10.0);
	writer.writeAttribute("TXTBASE"  ,charStyle.baselineOffset() / 10.0);
	writer.writeAttribute("TXTSHX"   ,charStyle.shadowXOffset() / 10.0);
	writer.writeAttribute("TXTSHY"   ,charStyle.shadowYOffset() / 10.0);
	writer.writeAttribute("TXTOUT"   ,charStyle.outlineWidth() / 10.0);
	writer.writeAttribute("TXTULP"   ,charStyle.underlineOffset() / 10.0);
	writer.writeAttribute("TXTULW"   ,charStyle.underlineWidth() / 10.0);
	writer.writeAttribute("TXTSTP"   ,charStyle.strikethruOffset() / 10.0);
	writer.writeAttribute("TXTSTW"   ,charStyle.strikethruWidth() / 10.0);
	writer.writeAttribute("TXTKERN"  ,charStyle.tracking());
	writer.writeAttribute("TXTSTYLE" ,charStyle.effects());
	writer.writeAttribute("COLUMNS"  ,item->columns());
	writer.writeAttribute("COLGAP"   ,item->columnGap());
	writer.writeAttribute("NAMEDLST" ,item->NamedLStyle);
	writer.writeAttribute("SHADE"    ,item->fillShade());
	writer.writeAttribute("SHADE2"   ,item->lineShade());
	writer.writeAttribute("GRTYP"    ,item->GrType);
	writer.writeAttribute("ROT"      ,item->rotation());
	writer.writeAttribute("PLINEART" ,item->PLineArt);
	writer.writeAttribute("PLINEEND" ,item->PLineEnd);
	writer.writeAttribute("PLINEJOIN",item->PLineJoin);
	writer.writeAttribute("LINESP"   ,defaultStyle.lineSpacing());
	writer.writeAttribute("LINESPMode",defaultStyle.lineSpacingMode());
	writer.writeAttribute("LOCALSCX" ,item->imageXScale());
	writer.writeAttribute("LOCALSCY" ,item->imageYScale());
	writer.writeAttribute("LOCALX"   ,item->imageXOffset());
	writer.writeAttribute("LOCALY"   ,item->imageYOffset());
	writer.writeAttribute("PICART"   ,item->imageShown() ? "1" : "0");
	writer.writeAttribute("PLTSHOW"  ,item->PoShow ? "1" : "0");
	writer.writeAttribute("BASEOF"   ,item->BaseOffs);
	writer.writeAttribute("textPathType",item->textPathType);
	writer.writeAttribute("textPathFlipped",item->textPathFlipped ? "1" : "0");
	writer.writeAttribute("FLIPPEDH" , item->imageFlippedH() ? "1" : "0");
	writer.writeAttribute("FLIPPEDV" , item->imageFlippedV() ? "1" : "0");
/*	writer.writeAttribute("BBOXX",item->BBoxX);
	writer.writeAttribute("BBOXH",item->BBoxH); */
	writer.writeAttribute("SCALETYPE",item->ScaleType ? "1" : "0");
	writer.writeAttribute("RATIO"    ,item->AspectRatio ? "1" : "0");
	writer.writeAttribute("PRINTABLE",item->printEnabled() ? "1" : "0");
	if(item->isAnnotation())
	{
		writer.writeAttribute("ANNOTATION","1");
		writer.writeAttribute("ANTYPE"  , item->annotation().Type());
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
		writer.writeAttribute("ANZIEL"  , item->annotation().Ziel());
		writer.writeAttribute("ANACTYP" , item->annotation().ActionType());
		writer.writeAttribute("ANTOOLTIP", item->annotation().ToolTip());
		writer.writeAttribute("ANBWID"  , item->annotation().Bwid());
		writer.writeAttribute("ANBSTY"  , item->annotation().Bsty());
		writer.writeAttribute("ANFEED"  , item->annotation().Feed());
		writer.writeAttribute("ANFLAG"  , item->annotation().Flag());
		writer.writeAttribute("ANFONT"  , item->annotation().Font());
		writer.writeAttribute("ANFORMAT", item->annotation().Format());
		writer.writeAttribute("ANROLL"  , item->annotation().RollOver());
		writer.writeAttribute("ANDOWN"  , item->annotation().Down());
		writer.writeAttribute("ANVIS"   , item->annotation().Vis());
		writer.writeAttribute("ANMC"    , item->annotation().MaxChar());
		writer.writeAttribute("ANCHK"   , item->annotation().IsChk() ? "1" : "0");
		writer.writeAttribute("ANAA"    , item->annotation().AAact() ? "1" : "0");
		writer.writeAttribute("ANCHKS"  , item->annotation().ChkStil());
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
	writer.writeAttribute("TEXTFLOWMODE", (int) item->textFlowMode() );
	// Set "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes for compatibility
	// with versions prior to 1.3.4
	writer.writeAttribute("TEXTFLOW" , item->textFlowAroundObject()    ? "1" : "0");
	writer.writeAttribute("TEXTFLOW2", item->textFlowUsesBoundingBox() ? "1" : "0");
	writer.writeAttribute("TEXTFLOW3", item->textFlowUsesContourLine() ? "1" : "0");
	writer.writeAttribute("AUTOTEXT" , item->isAutoText ? "1" : "0");
	writer.writeAttribute("EXTRA"    , item->textToFrameDistLeft());
	writer.writeAttribute("TEXTRA"   , item->textToFrameDistTop());
	writer.writeAttribute("BEXTRA"   , item->textToFrameDistBottom());
	writer.writeAttribute("REXTRA"   , item->textToFrameDistRight());
	writer.writeAttribute("FLOP"	,item->firstLineOffset());
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
	writer.writeAttribute("IRENDER"    , item->IRender);
	writer.writeAttribute("EMBEDDED"   , item->UseEmbedded ? "1" : "0");
	writer.writeAttribute("LOCK"       , item->locked() ? "1" : "0");
	writer.writeAttribute("LOCKR"      , item->sizeLocked() ? "1" : "0");
	writer.writeAttribute("REVERS"     , item->reversed() ? "1" : "0");
	writer.writeAttribute("TransValue" , item->fillTransparency());
	writer.writeAttribute("TransValueS", item->lineTransparency());
	writer.writeAttribute("TransBlend" , item->fillBlendmode());
	writer.writeAttribute("TransBlendS", item->lineBlendmode());
	writer.writeAttribute("isTableItem", static_cast<int>(item->isTableItem));
	writer.writeAttribute("TopLine"    , static_cast<int>(item->TopLine));
	writer.writeAttribute("LeftLine"   , static_cast<int>(item->LeftLine));
	writer.writeAttribute("RightLine"  , static_cast<int>(item->RightLine));
	writer.writeAttribute("BottomLine" , static_cast<int>(item->BottomLine));
	if (item->isTableItem)
	{
		if (item->TopLink != 0)
			writer.writeAttribute("TopLINK", item->TopLink->ItemNr);
		else
			writer.writeAttribute("TopLINK", "-1");
		if (item->LeftLink != 0)
			writer.writeAttribute("LeftLINK", item->LeftLink->ItemNr);
		else
			writer.writeAttribute("LeftLINK", "-1");
		if (item->RightLink != 0)
			writer.writeAttribute("RightLINK", item->RightLink->ItemNr);
		else
			writer.writeAttribute("RightLINK", "-1");
		if (item->BottomLink != 0)
			writer.writeAttribute("BottomLINK", item->BottomLink->ItemNr);
		else
			writer.writeAttribute("BottomLINK", "-1");
		writer.writeAttribute("OwnLINK", item->ItemNr);
	}
	writer.writeAttribute("isGroupControl", item->isGroupControl ? "1" : "0");
	if (item->isGroupControl)
	{
		if (item->groupsLastItem != 0)
			writer.writeAttribute("groupsLastItem", item->groupsLastItem->ItemNr - item->ItemNr);
	}
	writer.writeAttribute("NUMDASH", item->DashValues.count());
	QString dlp = "";
	QVector<double>::Iterator dax;
	for (dax = item->DashValues.begin(); dax != item->DashValues.end(); ++dax)
		dlp += tmp.setNum((*dax)) + " ";
	writer.writeAttribute("DASHS"  , dlp);
	writer.writeAttribute("DASHOFF", item->DashOffset);
	writer.writeAttribute("NUMPO"  , item->PoLine.size());
	QString polp = "";
	for (uint nxx=0; nxx<item->PoLine.size(); ++nxx)
	{
		item->PoLine.point(nxx, &xf, &yf);
		polp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	writer.writeAttribute("POCOOR", polp);
	writer.writeAttribute("NUMCO" , item->ContourLine.size());
	QString colp = "";
	for (uint nxx=0; nxx<item->ContourLine.size(); ++nxx)
	{
		item->ContourLine.point(nxx, &xf, &yf);
		colp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	writer.writeAttribute("COCOOR"  , colp);
	writer.writeAttribute("NUMGROUP", item->Groups.count());
	QString glp = "";
	QStack<int>::Iterator nx;
	for (nx = item->Groups.begin(); nx != item->Groups.end(); ++nx)
		glp += tmp.setNum((*nx)) + " ";
	writer.writeAttribute("GROUPS"  , glp);
	writer.writeAttribute("LANGUAGE", charStyle.language());
	writer.writeAttribute("startArrowIndex", item->startArrowIndex());
	writer.writeAttribute("endArrowIndex"  , item->endArrowIndex());
}

//CB: Private only now
void ScriXmlDoc::GetStyle(QXmlStreamReader &reader, ParagraphStyle &vg, StyleSet<ParagraphStyle>* tempStyles, ScribusDoc* doc, bool fl)
{
	bool fou = false;
	QString tmpf, tmf, tmV;
	double xf, xf2;
	const StyleSet<ParagraphStyle>& docParagraphStyles = tempStyles ? *tempStyles : doc->paragraphStyles();
	QString tagName = reader.name().toString();
	if (tagName != "STYLE" || !reader.isStartElement())
		return;
	QXmlStreamAttributes attrs = reader.attributes();
	vg.setName( attrAsString(attrs, "NAME", "") );
	vg.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(attrAsInt(attrs, "LINESPMode", 0)));
	if (static_cast<bool>(attrAsInt(attrs, "BASE", 0)))
		vg.setLineSpacingMode(ParagraphStyle::BaselineGridLineSpacing);
	vg.setLineSpacing( attrAsDbl(attrs, "LINESP") );
	vg.setLeftMargin( attrAsDbl(attrs, "INDENT", 0.0) );
	vg.setFirstIndent( attrAsDbl(attrs, "FIRST", 0.0) );
	vg.setAlignment(static_cast<ParagraphStyle::AlignmentType>(attrAsInt(attrs, "ALIGN", 0)));
	vg.setGapBefore( attrAsDbl(attrs, "VOR", 0.0) );
	vg.setGapAfter( attrAsDbl(attrs, "NACH", 0.0) );
	vg.setHasDropCap   (static_cast<bool>(attrAsInt(attrs, "DROP", 0)));
	vg.setDropCapLines ( attrAsInt(attrs, "DROPLIN", 2) );
	vg.setDropCapOffset( attrAsDbl(attrs, "DROPDIST", 0.0) );

	tmpf = attrAsString(attrs, "FONT", doc->toolSettings.defFont);
	if (tmpf.isEmpty())
		tmpf = doc->toolSettings.defFont;
	tmf = tmpf;
	if (!DoFonts.contains(tmpf))
		tmpf = AskForFont(prefsManager->appPrefs.AvailFonts, tmpf, doc);
	else
		tmpf = DoFonts[tmf];

	vg.charStyle().setFont(prefsManager->appPrefs.AvailFonts[tmpf]);
	vg.charStyle().setFontSize(qRound(attrAsDbl(attrs, "FONTSIZE", 12.0) * 10.0));
	vg.charStyle().setFeatures(static_cast<StyleFlag>(attrAsInt(attrs, "EFFECT", 0)).featureList());
	vg.charStyle().setFillColor(attrAsString(attrs, "FCOLOR", doc->toolSettings.dBrush));
	vg.charStyle().setFillShade(attrAsInt(attrs, "FSHADE", 100));
	vg.charStyle().setStrokeColor(attrAsString(attrs, "SCOLOR", doc->toolSettings.dPen));
	vg.charStyle().setStrokeShade(attrAsInt(attrs, "SSHADE", 100));
	vg.charStyle().setShadowXOffset(qRound(attrAsDbl(attrs, "TXTSHX", 5.0) * 10));
	vg.charStyle().setShadowYOffset(qRound(attrAsDbl(attrs, "TXTSHY", -5.0) * 10));
	vg.charStyle().setOutlineWidth(qRound(attrAsDbl(attrs, "TXTOUT", 1.0) * 10));
	vg.charStyle().setUnderlineOffset(qRound(attrAsDbl(attrs, "TXTULP", -0.1) * 10));
	vg.charStyle().setUnderlineWidth(qRound(attrAsDbl(attrs, "TXTULW", -0.1) * 10));
	vg.charStyle().setStrikethruOffset(qRound(attrAsDbl(attrs, "TXTSTP", -0.1) * 10));
	vg.charStyle().setStrikethruWidth(qRound(attrAsDbl(attrs, "TXTSTW", -0.1) * 10));
	vg.charStyle().setScaleH(qRound(attrAsDbl(attrs, "SCALEH", 100.0) * 10));
	vg.charStyle().setScaleV(qRound(attrAsDbl(attrs, "SCALEV", 100.0) * 10));
	vg.charStyle().setBaselineOffset(attrAsDbl(attrs, "BASEO", 0.0) * 10);
	vg.charStyle().setTracking(qRound(attrAsDbl(attrs, "KERN", 0.0) * 10));
//	vg.tabValues().clear();

	int numTab = attrAsInt(attrs, "NUMTAB", 0);
	if (numTab != 0)
	{
		QList<ParagraphStyle::TabRecord> tbs;
		ParagraphStyle::TabRecord tb;
		QString tmp = attrAsString(attrs, "TABS", "");
		QTextStream tgv(&tmp, QIODevice::ReadOnly);
		tbs.clear();
		for (int cxv = 0; cxv < numTab; cxv += 2)
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
		while (!reader.atEnd() && !reader.hasError())
		{
//			QXmlStreamReader::TokenType tType = reader.readNext();
			reader.readNext();
			QString tagName = reader.name().toString();
			if (tagName == "STYLE" && reader.isEndElement()) 
				break;
			if (tagName == "Tabs" && reader.isStartElement())
			{
				ParagraphStyle::TabRecord tb;
				QXmlStreamAttributes tabAttrs = reader.attributes();
				tb.tabPosition = attrAsDbl(tabAttrs, "Pos");
				tb.tabType     = attrAsInt(tabAttrs, "Type");
				QString tbCh   = attrAsString(tabAttrs, "Fill","");
				tb.tabFillChar = tbCh.isEmpty() ? QChar() : tbCh.at(0);
				tbs.append(tb);
			}
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
		if (tempStyles)
			tempStyles->create(vg);
		else
		{
			StyleSet<ParagraphStyle> tmp;
			tmp.create(vg);
			doc->redefineStyles(tmp, false);
		}
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
	ff.replace(QChar(0), QChar(32));
	ff.replace("&#x5;", SpecialChars::PARSEP);
	ff.replace("&#x4;", SpecialChars::TAB);

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
		if (tagName=="STYLE")
		{
			GetStyle(sReader, vg, NULL, doc, true);
			VorLFound = true;
		}
		if (tagName=="Pattern")
		{
			ScPattern pat;
//			uint ac = doc->Items->count();
			QString patFile = Fi ? fileName : QString();
			ReadPattern(sReader, doc, view, patFile, GrMax, VorLFound, newVersion);
			
		}
	}

	sReader.clear();
	sReader.addData(ff);

	bool inItem = false;
	bool isGroupControl = false;
	int  groupsLastItem = 0;
	int  itemOwnLink    = 0;
	QString itemClip;
	StoryText  storyText;
	LastStyles lastStyles;
	int LatexDPI;
	bool LatexPream;
	QString LatexConfig;
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
			storyText = StoryText();
			lastStyles = LastStyles();
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
			LatexDPI = attrAsInt (attrs, "LatexDpi", 0);
			LatexPream = attrAsBool(attrs, "LatexUsePreamble", true);
			LatexConfig = attrAsString(attrs, "LatexConfig", "");
		}
		if (inItem && sReader.isStartElement() && tagName == "ITEXT")
		{
			GetItemText(sReader.attributes(), storyText, doc, &lastStyles, VorLFound, true);
//			continue;
		}
		if (inItem && sReader.isStartElement() && tagName == "PARA")
		{
			storyText.insertChars(storyText.length(), SpecialChars::PARSEP);
			ParagraphStyle newStyle;
//			PrefsManager* prefsManager=PrefsManager::instance();
			ReadPStyle(sReader, newStyle, doc);
			storyText.setStyle(storyText.length()-1, newStyle);
			storyText.setCharStyle(storyText.length()-1, 1, lastStyles.Style);
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
			LastStyles lastStyle;
			view->PasteItem(&OB, true, true, false);
			PageItem* Neu = doc->Items->at(doc->Items->count()-1);
			storyText.setDefaultStyle(Neu->itemText.defaultStyle());
			if (Neu->asLatexFrame())
			{
				PageItem_LatexFrame *latexitem = Neu->asLatexFrame();
				latexitem->setConfigFile(LatexConfig);
				latexitem->setDpi(LatexDPI);
				latexitem->setUsePreamble(LatexPream);
			}
			Neu->itemText = storyText;
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

void ScriXmlDoc::ReadPattern(QXmlStreamReader &reader, ScribusDoc *doc, ScribusView *view, const QString& fileName, 
							 int& GrMax, bool styleFound, bool newVersion)
{
	QString tmp, tmpf, tmf;
	QList<PageItem*>  TableItems;
	QMap<int, ImageLoadRequest> loadRequests;
	QMap<int,int> TableID;
	QMap<int,int> arrowID;
	QMap<PageItem*, int> groupID;
	ScImageEffectList imageEffects;
	ScPattern pat;
	struct CopyPasteBuffer OB;
	bool savedAlignGrid   = doc->useRaster;
	bool savedAlignGuides = doc->SnapGuides;
	bool isGroupControl   = false;
	bool isFile = !fileName.isEmpty();
	doc->useRaster  = false;
	doc->SnapGuides = false;
	QXmlStreamAttributes attrs = reader.attributes();
	QString patClipPath;
	QString patName   = attrs.value("Name").toString();
	double  patScaleX = attrAsDbl(attrs, "scaleX", 0.0);
	double  patScaleY = attrAsDbl(attrs, "scaleY", 0.0);
	double  patWidth  = attrAsDbl(attrs, "width", 0.0);
	double  patHeight = attrAsDbl(attrs, "height", 0.0);
	double  patxoffset  = attrAsDbl(attrs, "xoffset", 0.0);
	double  patyoffset = attrAsDbl(attrs, "yoffset", 0.0);
	int     patOwnLink = 0;
	int     groupsLastItem = 0;
	int x = doc->GroupCounter;
	uint    ac = doc->Items->count();
	QString fileDir    = QDir::homePath();
	StoryText  storyText;
	LastStyles lastStyles;
	int LatexDPI;
	bool LatexPream;
	QString LatexConfig;
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.hasError())
			break;
		QString tagName1 = reader.name().toString();
		QXmlStreamAttributes attrs1 = reader.attributes();
		
		if (tagName1 == "PatternItem" && reader.isStartElement())
		{
			loadRequests.clear();
			imageEffects.clear();
			storyText  = StoryText();
			lastStyles = LastStyles();
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
			int numGroup = attrAsInt(attrs1, "NUMGROUP", 0);
			if ((!attrs1.value("GROUPS").isEmpty()) && (numGroup > 0))
			{
				tmp = attrAsString(attrs1, "GROUPS", "");
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
			tmp.clear();
			LatexDPI = attrAsInt (attrs1, "LatexDpi", 0);
			LatexPream = attrAsBool(attrs1, "LatexUsePreamble", true);
			LatexConfig = attrAsString(attrs1, "LatexConfig", "");
		}
		if (tagName1 == "ITEXT" && reader.isStartElement())
		{
			GetItemText(reader.attributes(), storyText, doc, &lastStyles, styleFound, true);
		}
		if (tagName1 == "ImageEffect" && reader.isStartElement())
		{
			struct ImageEffect ef;
			ef.effectParameters = attrAsString(attrs1, "Param", "");
			ef.effectCode       = attrAsInt(attrs1, "Code");
			imageEffects.append(ef);
		}
		if (tagName1 == "PSDLayer" && reader.isStartElement())
		{
			struct ImageLoadRequest loadingInfo;
			loadingInfo.blend   = attrAsString(attrs1, "Blend", "");
			loadingInfo.opacity = attrAsInt(attrs1, "Opacity", 1);
			loadingInfo.visible = attrAsBool(attrs, "Visible", false);
			loadingInfo.useMask = attrAsBool(attrs, "useMask", true);
			loadRequests.insert( attrAsInt(attrs1, "Layer"), loadingInfo);
		}
		if (tagName1 == "CSTOP" && reader.isStartElement())
		{
			QString name = attrs1.value("NAME").toString();
			double ramp  = attrAsDbl(attrs1, "RAMP", 0.0);
			int shade    = attrAsInt(attrs1, "SHADE", 100);
			double opa   = attrAsDbl(attrs1, "TRANS", 1.0);
			OB.fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
			OB.GrColor   = "";
			OB.GrColor2  = "";
		}
		if (tagName1=="Tabs" && reader.isStartElement())
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
		if (isFile && attrAsBool(attrs1, "relativePaths", false))
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
		if (tagName1 == "PatternItem" && reader.isEndElement())
		{
			LastStyles lastStyle;
			view->PasteItem(&OB, true, true, false);
			PageItem* Neu = doc->Items->at(doc->Items->count()-1);
			Neu->setXYPos(Neu->xPos() - doc->currentPage()->xOffset(), Neu->yPos() - doc->currentPage()->yOffset(), true);
			storyText.setDefaultStyle(Neu->itemText.defaultStyle());
			Neu->itemText = storyText;
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
			if (Neu->asLatexFrame())
			{
				PageItem_LatexFrame *latexitem = Neu->asLatexFrame();
				latexitem->setConfigFile(LatexConfig);
				latexitem->setDpi(LatexDPI);
				latexitem->setUsePreamble(LatexPream);
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
		}
		if (tagName1 == "Pattern" && reader.isEndElement())
			break;
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
	doc->useRaster = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	uint ae = doc->Items->count();
	if (ae > ac)
	{
		pat.setDoc(doc);
		PageItem* currItem = doc->Items->at(ac);
		pat.pattern = currItem->DrawObj_toImage();
		pat.scaleX = patScaleX;
		pat.scaleY = patScaleY;
		pat.width  = patWidth;
		pat.height = patHeight;
		pat.xoffset = patxoffset;
		pat.yoffset = patyoffset;
		pat.pattern = pat.pattern.copy(-patxoffset, -patyoffset, pat.width, pat.height);
		for (uint as = ac; as < ae; ++as)
		{
			PageItem* Neu = doc->Items->takeAt(ac);
			Neu->moveBy(pat.xoffset, pat.yoffset, true);
			Neu->gXpos += pat.xoffset;
			Neu->gYpos += pat.yoffset;
			Neu->ItemNr = pat.items.count();
			pat.items.append(Neu);
		}
		if (!doc->docPatterns.contains(patName))
			doc->docPatterns.insert(patName, pat);
	}
	groupID.clear();
	doc->GroupCounter = GrMax + 1;
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
	ScXmlStreamWriter writer(&documentStr);
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
		writer.writeAttribute("W", gw);
		writer.writeAttribute("H", gh);
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
			writer.writeAttribute("W", maxx - minx);
			writer.writeAttribute("H", maxy - miny);
		}
		else
		{
			writer.writeAttribute("W", item->width());
			writer.writeAttribute("H", item->height());
		}
		xp = item->xPos() - doc->currentPage()->xOffset();
		yp = item->yPos() - doc->currentPage()->yOffset();
	}
	writer.writeAttribute("XP", xp);
	writer.writeAttribute("YP", yp);
	writer.writeAttribute("COUNT",   selection->count());
	writer.writeAttribute("Version", QString(VERSION));
	QMap<QString,int>::Iterator itf;
	for (itf = doc->UsedFonts.begin(); itf != doc->UsedFonts.end(); ++itf)
	{
		writer.writeStartElement("FONT");
		writer.writeAttribute("NAME",itf.key());
		writer.writeEndElement();
	}
	ColorList usedColors;
	doc->getUsedColors(usedColors);
	ColorList::Iterator itc;
	for (itc = usedColors.begin(); itc != usedColors.end(); ++itc)
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
	int NewStylesNum = 5;
	UsedStyles.clear();
	UsedMapped2Saved.clear();

	ParagraphStyle vg;
//	if (doc->docParagraphStyles.count() > 5)
	{
		for (int co=0; co < selection->count(); ++co)
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
		QList<int> StyleNumb = UsedStyles.keys();
		qSort(StyleNumb);
		for (int ff = 0; ff < StyleNumb.count(); ++ff)
		{
			int actSt = StyleNumb.at(ff);
			UsedMapped2Saved.insert(actSt, NewStylesNum);
			NewStylesNum++;
			QMap<int, ParagraphStyle>::ConstIterator paraStyle = UsedStyles.find(actSt);
			writer.writeStartElement("STYLE");
			writer.writeAttribute("NAME",  paraStyle->name());
			writer.writeAttribute("ALIGN", static_cast<int>(paraStyle->alignment()));
			writer.writeAttribute("LINESPMode", static_cast<int>(paraStyle->lineSpacingMode()));
			//CB #2738:
			//UsedStyles[actSt].LineSpa is something like this is using automatic from the font:
			//10.34912109375000000000. Default attribute value is then 10.3491 which then becomes //10.34909999999999996589 and then does not compare. This fixes, should we change our
			//default precision?
			writer.writeAttribute("LINESP", QString::number(paraStyle->lineSpacing(),'f',13));
			writer.writeAttribute("INDENT", paraStyle->leftMargin());
			writer.writeAttribute("FIRST",  paraStyle->firstIndent());
			writer.writeAttribute("VOR",    paraStyle->gapBefore());
			writer.writeAttribute("NACH",   paraStyle->gapAfter());
			writer.writeAttribute("FONT",   paraStyle->charStyle().font().scName());
			writer.writeAttribute("FONTSIZE", paraStyle->charStyle().fontSize() / 10.0);
			writer.writeAttribute("DROP",     static_cast<int>(paraStyle->hasDropCap()));
			writer.writeAttribute("DROPLIN",  paraStyle->dropCapLines());
			writer.writeAttribute("DROPDIST", paraStyle->dropCapOffset());
			writer.writeAttribute("EFFECT",   paraStyle->charStyle().effects());
			
			writer.writeAttribute("FCOLOR", paraStyle->charStyle().fillColor());
			writer.writeAttribute("FSHADE", paraStyle->charStyle().fillShade());
			writer.writeAttribute("SCOLOR", paraStyle->charStyle().strokeColor());
			writer.writeAttribute("SSHADE", paraStyle->charStyle().strokeShade());
			writer.writeAttribute("BASE",   static_cast<int>(paraStyle->lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing));
			writer.writeAttribute("TXTSHX", paraStyle->charStyle().shadowXOffset() / 10.0);
			writer.writeAttribute("TXTSHY", paraStyle->charStyle().shadowYOffset() / 10.0);
			writer.writeAttribute("TXTOUT", paraStyle->charStyle().outlineWidth() / 10.0);
			writer.writeAttribute("TXTULP", paraStyle->charStyle().underlineOffset() / 10.0);
			writer.writeAttribute("TXTULW", paraStyle->charStyle().underlineWidth() / 10.0);
			writer.writeAttribute("TXTSTP", paraStyle->charStyle().strikethruOffset() / 10.0);
			writer.writeAttribute("TXTSTW", paraStyle->charStyle().strikethruWidth() / 10.0);
			writer.writeAttribute("SCALEH", paraStyle->charStyle().scaleH() / 10.0);
			writer.writeAttribute("SCALEV", paraStyle->charStyle().scaleV() / 10.0);
			writer.writeAttribute("BASEO",  paraStyle->charStyle().baselineOffset() / 10.0);
			writer.writeAttribute("KERN",   paraStyle->charStyle().tracking() / 10.0);
			for (int a = 0; a < paraStyle->tabValues().count(); ++a)
			{
				writer.writeEmptyElement("Tabs");
				writer.writeAttribute("Type", paraStyle->tabValues().at(a).tabType);
				writer.writeAttribute("Pos",  paraStyle->tabValues().at(a).tabPosition);
				QString tabCh = "";
				if (!paraStyle->tabValues().at(a).tabFillChar.isNull())
					tabCh = QString(paraStyle->tabValues().at(a).tabFillChar);
				writer.writeAttribute("Fill", tabCh);
			}
			writer.writeEndElement();
		}
	}

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
			writer.writeAttribute("Shade"   , (*itMU2).Shade);
			writer.writeAttribute("Dash"    , (*itMU2).Dash);
			writer.writeAttribute("LineEnd" , (*itMU2).LineEnd);
			writer.writeAttribute("LineJoin", (*itMU2).LineJoin);
			writer.writeAttribute("Width"   , (*itMU2).Width);
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
				writer.writeAttribute("NumPoints", itar.value().points.size());
				QString arp = "";
				double xa, ya;
				for (uint nxx = 0; nxx < itar.value().points.size(); ++nxx)
				{
					itar.value().points.point(nxx, &xa, &ya);
					arp += tmp.setNum(xa) + " " + tmpy.setNum(ya) + " ";
				}
				writer.writeAttribute("Points", arp);
				writer.writeAttribute("Name"  , itar.value().name);
				writer.writeAttribute("Index" , itar.key());
				writer.writeEndElement();
			}
		}
	}
	QStringList patterns = doc->getUsedPatternsSelection(selection);
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern& pa = doc->docPatterns[patterns[c]];
		writer.writeStartElement("Pattern");
		writer.writeAttribute ("Name"  , patterns[c]);
		writer.writeAttribute ("scaleX", pa.scaleX);
		writer.writeAttribute ("scaleY", pa.scaleY);
		writer.writeAttribute("width" , pa.width);
		writer.writeAttribute ("height", pa.height);
		writer.writeAttribute("xoffset", pa.xoffset);
		writer.writeAttribute("yoffset", pa.yoffset);
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

void ScriXmlDoc::WriteObject(ScXmlStreamWriter& writer, ScribusDoc *doc, PageItem *item, const QString& baseDir, 
							 QMap<int, int> &UsedMapped2Saved)
{
	QString text, tmp, tmpy;
	QString CurDirP = QDir::currentPath();
	
	const ParagraphStyle& defaultStyle = item->itemText.defaultStyle();

	int textAlignment = item->itemText.defaultStyle().alignment();
	writer.writeAttribute("ALIGN", textAlignment);
	SetItemProps(writer, doc, item, baseDir, false);
	//writer.writeAttribute("LOCK", "0");
	//writer.writeAttribute("XPOS",  item->xPos() - doc->currentPage()->xOffset());
	//writer.writeAttribute("YPOS",  item->yPos() - doc->currentPage()->yOffset());
	writer.writeAttribute("BOOKMARK"   , item->isBookmark ? "1" : "0");
	writer.writeAttribute("fillRule"   , item->fillRule ? "1" : "0");
	writer.writeAttribute("doOverprint", item->doOverprint ? "1" : "0");
	writer.writeAttribute("ImageClip"  , item->pixm.imgInfo.usedPath);

	writer.writeAttribute("NUMTEXT", item->itemText.length());
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
		writer.writeAttribute("GRSTARTX", item->GrStartX);
		writer.writeAttribute("GRSTARTY", item->GrStartY);
		writer.writeAttribute("GRENDX"  , item->GrEndX);
		writer.writeAttribute("GRENDY"  , item->GrEndY);
	}

	if (item->effectsInUse.count() != 0)
	{
		for (int a = 0; a < item->effectsInUse.count(); ++a)
		{
			writer.writeStartElement("ImageEffect");
			writer.writeAttribute("Code" , item->effectsInUse.at(a).effectCode);
			writer.writeAttribute("Param", item->effectsInUse.at(a).effectParameters);
			writer.writeEndElement();
		}
	}
	if (item->itemText.defaultStyle().tabValues().count() != 0)
	{
		for (int a = 0; a < item->itemText.defaultStyle().tabValues().count(); ++a)
		{
			writer.writeStartElement("Tabs");
			writer.writeAttribute("Type", (defaultStyle.tabValues().at(a)).tabType);
			writer.writeAttribute("Pos" , (defaultStyle.tabValues().at(a)).tabPosition);
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
			writer.writeAttribute("Layer"  , it2.key());
			writer.writeAttribute("Visible", it2.value().visible ? "1" : "0");
			writer.writeAttribute("useMask", it2.value().useMask ? "1" : "0");
			writer.writeAttribute("Opacity", it2.value().opacity);
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
			writer.writeAttribute("pScaleX"  , patternScaleX);
			writer.writeAttribute("pScaleY"  , patternScaleY);
			writer.writeAttribute("pOffsetX" , patternOffsetX);
			writer.writeAttribute("pOffsetY" , patternOffsetY);
			writer.writeAttribute("pRotation", patternRotation);
		}
		else
		{
			QList<VColorStop*> cstops = item->fill_gradient.colorStops();
			for (uint cst = 0; cst < item->fill_gradient.Stops(); ++cst)
			{
				writer.writeStartElement("CSTOP");
				writer.writeAttribute("RAMP" , cstops.at(cst)->rampPoint);
				writer.writeAttribute("NAME" , cstops.at(cst)->name);
				writer.writeAttribute("SHADE", cstops.at(cst)->shade);
				writer.writeAttribute("TRANS", cstops.at(cst)->opacity);
				writer.writeEndElement();
			}
			// must be writen before
			// writer.writeAttribute("GRSTARTX", item->GrStartX);
			// writer.writeAttribute("GRSTARTY", item->GrStartY);
			// writer.writeAttribute("GRENDX"  , item->GrEndX);
			// writer.writeAttribute("GRENDY"  , item->GrEndY);
		}
	}
	if (item->asLatexFrame())
	{
		PageItem_LatexFrame *latexitem = item->asLatexFrame();
		writer.writeAttribute("LatexDpi", latexitem->dpi());
		writer.writeAttribute("LatexConfig", latexitem->configFile());
		writer.writeAttribute("LatexUsePreamble", QString::number(static_cast<int>(latexitem->usePreamble())));
		writer.writeTextElement("LATEX-SOURCE", item->asLatexFrame()->formula());
	}
	QDir::setCurrent(CurDirP);
	WriteITEXTs(writer, doc, item);
}

void ScriXmlDoc::WriteITEXTs(ScXmlStreamWriter &writer, ScribusDoc *doc, PageItem *item)
{
	CharStyle lastStyle;
	int lastPos = 0;
	QString text, pstylename;

	for( int k = 0; k < item->itemText.length(); ++k)
	{
		const CharStyle& style(item->itemText.charStyle(k));
		QChar ch = item->itemText.text(k);
		pstylename = QString();
		if (ch == SpecialChars::PARSEP)
			pstylename = item->itemText.paragraphStyle(k).parent();
		if ((style != lastStyle || ch == SpecialChars::PARSEP) && (k - lastPos > 0))
		{
			text = item->itemText.text(lastPos, k - lastPos);
			writer.writeEmptyElement("ITEXT");
			writer.writeAttribute("CH"     ,text);
			WriteLegacyCStyle(writer, lastStyle);
			writer.writeAttribute("PSTYLE" ,pstylename);
			lastPos = k;
			lastStyle = style;
		}
		if (ch == SpecialChars::PARSEP)
		{
			WritePStyle(writer, item->itemText.paragraphStyle(k), "PARA");
			lastPos = k + 1;
		}
	}
	// write pending chars
	if ( item->itemText.length() - lastPos > 0)
	{
		writer.writeEmptyElement("ITEXT");
		text = item->itemText.text(lastPos, item->itemText.length() - lastPos);
		writer.writeAttribute("CH"     ,text);
		WriteLegacyCStyle(writer, lastStyle); 
		writer.writeAttribute("PSTYLE" ,pstylename);
	}
	// paragraphstyle for trailing chars
	if (item->itemText.length() > 0 && item->itemText.text(item->itemText.length()-1) != SpecialChars::PARSEP)
		WritePStyle(writer, item->itemText.paragraphStyle(item->itemText.length()), "PARA");
}

void ScriXmlDoc::WriteLegacyCStyle (ScXmlStreamWriter& writer, const CharStyle& style)
{
	if ( ! style.isInhFont())	
		writer.writeAttribute("CFONT", style.font().scName());
	if ( ! style.isInhFontSize())
		writer.writeAttribute("CSIZE", style.fontSize() / 10.0);
	if ( ! style.isInhFillColor())
		writer.writeAttribute("CCOLOR", style.fillColor());
	if ( ! style.isInhFillShade())
		writer.writeAttribute("CSHADE", style.fillShade());
	if ( ! style.isInhStrokeColor())
		writer.writeAttribute("CSTROKE", style.strokeColor());
	if ( ! style.isInhStrokeShade())
		writer.writeAttribute("CSHADE2", style.strokeShade());
	if ( ! style.isInhShadowXOffset())
		writer.writeAttribute("CSHX", style.shadowXOffset() / 10.0);
	if ( ! style.isInhShadowYOffset())
		writer.writeAttribute("CSHY", style.shadowYOffset() / 10.0);
	if ( ! style.isInhOutlineWidth())
		writer.writeAttribute("COUT", style.outlineWidth() / 10.0);
	if ( ! style.isInhUnderlineOffset())
		writer.writeAttribute("CULP", style.underlineOffset() / 10.0);
	if ( ! style.isInhUnderlineWidth())
		writer.writeAttribute("CULW", style.underlineWidth() / 10.0);
	if ( ! style.isInhStrikethruOffset())
		writer.writeAttribute("CSTP", style.strikethruOffset() / 10.0);
	if ( ! style.isInhStrikethruWidth())
		writer.writeAttribute("CSTW", style.strikethruWidth() / 10.0);
	if ( ! style.isInhScaleH())
		writer.writeAttribute("CSCALE", style.scaleH() / 10.0);
	if ( ! style.isInhScaleV())
		writer.writeAttribute("CSALEV", style.scaleV() / 10.0);
	writer.writeAttribute("CSTYLE", style.effects() & 2047);
}

void ScriXmlDoc::WriteCStyle (ScXmlStreamWriter& writer, const CharStyle& style)
{
	if ( ! style.name().isEmpty() )
		writer.writeAttribute("CNAME", style.name());
	if ( ! style.parent().isEmpty() )
		writer.writeAttribute("CPARENT", style.parent());
	if ( ! style.isInhFont())	
		writer.writeAttribute("FONT", style.font().scName());
	if ( ! style.isInhFontSize())
		writer.writeAttribute("FONTSIZE", style.fontSize() / 10.0);
	if ( ! style.isInhFeatures())
		writer.writeAttribute("FEATURES", style.features().join(" "));
	if ( ! style.isInhFillColor())
		writer.writeAttribute("FCOLOR", style.fillColor());
	if ( ! style.isInhFillShade())
		writer.writeAttribute("FSHADE", style.fillShade());
	if ( ! style.isInhStrokeColor())
		writer.writeAttribute("SCOLOR", style.strokeColor());
	if ( ! style.isInhStrokeShade())
		writer.writeAttribute("SSHADE", style.strokeShade());
	if ( ! style.isInhShadowXOffset())
		writer.writeAttribute("TXTSHX", style.shadowXOffset() / 10.0);
	if ( ! style.isInhShadowYOffset())
		writer.writeAttribute("TXTSHY", style.shadowYOffset() / 10.0);
	if ( ! style.isInhOutlineWidth())
		writer.writeAttribute("TXTOUT", style.outlineWidth() / 10.0);
	if ( ! style.isInhUnderlineOffset())
		writer.writeAttribute("TXTULP", style.underlineOffset() / 10.0);
	if ( ! style.isInhUnderlineWidth())
		writer.writeAttribute("TXTULW", style.underlineWidth() / 10.0);
	if ( ! style.isInhStrikethruOffset())
		writer.writeAttribute("TXTSTP", style.strikethruOffset() / 10.0);
	if ( ! style.isInhStrikethruWidth())
		writer.writeAttribute("TXTSTW", style.strikethruWidth() / 10.0);
	if ( ! style.isInhScaleH())
		writer.writeAttribute("SCALEH", style.scaleH() / 10.0);
	if ( ! style.isInhScaleV())
		writer.writeAttribute("SCALEV", style.scaleV() / 10.0);
	if ( ! style.isInhBaselineOffset())
		writer.writeAttribute("BASEO", style.baselineOffset() / 10.0);
	if ( ! style.isInhTracking())
		writer.writeAttribute("KERN", style.tracking() / 10.0); 
	if ( ! style.isInhWordTracking())
		writer.writeAttribute("wordTrack", style.wordTracking());
	if ( ! style.shortcut().isEmpty() )
		writer.writeAttribute("SHORTCUT", style.shortcut()); // shortcuts won't be inherited
}

void ScriXmlDoc::WritePStyle (ScXmlStreamWriter& writer, const ParagraphStyle& style, const QString& nodeName)
{
//	bool styleHasTabs = (!style.isInhTabValues() && (style.tabValues().count() > 0));
	writer.writeStartElement(nodeName);
	if ( ! style.name().isEmpty() )
		writer.writeAttribute("NAME", style.name());
	if ( ! style.parent().isEmpty())
		writer.writeAttribute("PARENT", style.parent());
	
	if ( ! style.isInhAlignment())
		writer.writeAttribute("ALIGN", style.alignment());
	if ( ! style.isInhLineSpacingMode())
		writer.writeAttribute("LINESPMode", style.lineSpacingMode());
	if ( ! style.isInhLineSpacing())
		writer.writeAttribute("LINESP", style.lineSpacing());
	if ( ! style.isInhLeftMargin())
		writer.writeAttribute("INDENT", style.leftMargin());
	if ( ! style.isInhRightMargin())
		writer.writeAttribute("RMARGIN", style.rightMargin());
	if ( ! style.isInhFirstIndent())
		writer.writeAttribute("FIRST", style.firstIndent());
	if ( ! style.isInhGapBefore())
		writer.writeAttribute("VOR", style.gapBefore());
	if ( ! style.isInhGapAfter())
		writer.writeAttribute("NACH", style.gapAfter());
	if ( ! style.isInhHasDropCap())
		writer.writeAttribute("DROP", static_cast<int>(style.hasDropCap()));
	if ( ! style.isInhDropCapLines())
		writer.writeAttribute("DROPLIN", style.dropCapLines());
	if ( ! style.isInhDropCapOffset())
		writer.writeAttribute("DROPDIST", style.dropCapOffset());
	if ( ! style.isInhOpticalMargins())
		writer.writeAttribute("OpticalMargins", style.opticalMargins());
	if ( ! style.isInhHyphenationMode())
		writer.writeAttribute("HyphenationMode", style.hyphenationMode());
	if ( ! style.isInhMinWordTracking())
		writer.writeAttribute("MinWordTrack", style.minWordTracking());
	if ( ! style.isInhMinGlyphExtension())
		writer.writeAttribute("MinGlyphShrink", style.minGlyphExtension());
	if ( ! style.isInhMaxGlyphExtension())
		writer.writeAttribute("MaxGlyphExtend", style.maxGlyphExtension());

	if ( ! style.shortcut().isEmpty() )
		writer.writeAttribute("PSHORTCUT", style.shortcut()); // shortcuts won't be inherited

	WriteCStyle(writer, style.charStyle());

	if ( ! style.isInhTabValues())
	{
		for (int a = 0; a < style.tabValues().count(); ++a)
		{
			writer.writeEmptyElement("Tabs");
			writer.writeAttribute("Type", (style.tabValues().at(a)).tabType);
			writer.writeAttribute("Pos", (style.tabValues().at(a)).tabPosition);
			QString tabCh = "";
			if (!(style.tabValues().at(a)).tabFillChar.isNull())
				tabCh = QString((style.tabValues().at(a)).tabFillChar);
			writer.writeAttribute("Fill", tabCh);
		}
	}

	writer.writeEndElement();
}

void ScriXmlDoc::ReadCStyle(QXmlStreamReader& reader, CharStyle & newStyle, ScribusDoc *doc)
{
	QXmlStreamAttributes attrs = reader.attributes();

	if ( attrHasValue(attrs, "CNAME"))
		newStyle.setName(attrAsString(attrs, "CNAME", ""));
	if ( attrHasValue(attrs, "CPARENT"))
		newStyle.setParent(attrAsString(attrs, "CPARENT", ""));
	
	if ( attrHasValue(attrs, "FONT"))
		newStyle.setFont(doc->AllFonts->findFont(attrAsString(attrs, "FONT", doc->toolSettings.defFont),doc));
	
	if ( attrHasValue(attrs, "FONTSIZE"))
		newStyle.setFontSize(qRound(attrAsDbl(attrs, "FONTSIZE", 12) * 10));
	
	if ( attrHasValue(attrs, "FCOLOR"))
		newStyle.setFillColor(attrAsString(attrs, "FCOLOR", CommonStrings::None));
	
	if ( attrHasValue(attrs, "KERN"))
		newStyle.setTracking(qRound(attrAsDbl(attrs, "KERN") * 10));
	
	if ( attrHasValue(attrs, "FSHADE"))
		newStyle.setFillShade(attrAsInt(attrs, "FSHADE"));
	
	if ( attrHasValue(attrs, "EFFECTS"))
		newStyle.setFeatures(static_cast<StyleFlag>(attrAsInt(attrs, "EFFECTS")).featureList());
	
	if ( attrHasValue(attrs, "EFFECT"))
		newStyle.setFeatures(static_cast<StyleFlag>(attrAsInt(attrs, "EFFECT")).featureList());
	
	if ( attrHasValue(attrs, "FEATURES"))
		newStyle.setFeatures(attrAsString(attrs, "FEATURES", "").split( " ", QString::SkipEmptyParts));
	
	if ( attrHasValue(attrs, "SCOLOR"))
		newStyle.setStrokeColor(attrAsString(attrs, "SCOLOR", CommonStrings::None));
	
	if ( attrHasValue(attrs, "SSHADE"))
		newStyle.setStrokeShade(attrAsInt(attrs, "SSHADE"));
	
	if ( attrHasValue(attrs, "SCALEH"))
		newStyle.setScaleH(qRound(attrAsDbl(attrs, "SCALEH") * 10));
	
	if ( attrHasValue(attrs, "SCALEV"))
		newStyle.setScaleV(qRound(attrAsDbl(attrs, "SCALEV") * 10));
	
	if ( attrHasValue(attrs, "BASEO"))
		newStyle.setBaselineOffset(qRound(attrAsDbl(attrs, "BASEO") * 10));
	
	if ( attrHasValue(attrs, "TXTSHX"))
		newStyle.setShadowXOffset(qRound(attrAsDbl(attrs, "TXTSHX") * 10));
	
	if ( attrHasValue(attrs, "TXTSHY"))
		newStyle.setShadowYOffset(qRound(attrAsDbl(attrs, "TXTSHY") * 10));
	
	if ( attrHasValue(attrs, "TXTOUT"))
		newStyle.setOutlineWidth(qRound(attrAsDbl(attrs, "TXTOUT") * 10));
	
	if ( attrHasValue(attrs, "TXTULP"))
		newStyle.setUnderlineOffset(qRound(attrAsDbl(attrs, "TXTULP") * 10));
	
	if ( attrHasValue(attrs, "TXTULW"))
		newStyle.setUnderlineWidth(qRound(attrAsDbl(attrs, "TXTULW") * 10));
	
	if ( attrHasValue(attrs, "TXTSTP"))
		newStyle.setStrikethruOffset(qRound(attrAsDbl(attrs, "TXTSTP") * 10));
	
	if ( attrHasValue(attrs, "TXTSTW"))
		newStyle.setStrikethruWidth(qRound(attrAsDbl(attrs, "TXTSTW") * 10));

	if ( attrHasValue(attrs, "SHORTCUT"))
		newStyle.setShortcut(attrAsString(attrs, "SHORTCUT", ""));

	if ( attrHasValue(attrs, "wordTrack"))
		newStyle.setWordTracking(attrAsDbl(attrs, "wordTrack"));
}

void ScriXmlDoc::ReadPStyle(QXmlStreamReader& reader, ParagraphStyle &style, ScribusDoc* doc)
{
	QXmlStreamAttributes attrs = reader.attributes();
	style.erase();
	style.setName( attrAsString(attrs, "NAME", "") );
	style.setParent( attrAsString(attrs, "PARENT", ""));
	if ( attrHasValue(attrs, "LINESPMode"))
		style.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(attrAsInt(attrs, "LINESPMode")));
	if ( attrHasValue(attrs, "LINESP"))
		style.setLineSpacing(attrAsDbl(attrs, "LINESP"));
	if ( attrHasValue(attrs, "INDENT"))
		style.setLeftMargin(attrAsDbl(attrs, "INDENT"));
	if ( attrHasValue(attrs, "RMARGIN"))
		style.setRightMargin(attrAsDbl(attrs, "RMARGIN"));
	if ( attrHasValue(attrs, "FIRST"))
		style.setFirstIndent(attrAsDbl(attrs, "FIRST"));
	if ( attrHasValue(attrs, "ALIGN"))
		style.setAlignment(static_cast<ParagraphStyle::AlignmentType>(attrAsInt(attrs, "ALIGN")));
	if ( attrHasValue(attrs, "VOR"))
		style.setGapBefore(attrAsDbl(attrs, "VOR"));
	if ( attrHasValue(attrs, "NACH"))
		style.setGapAfter(attrAsDbl(attrs, "NACH"));
	if ( attrHasValue(attrs, "DROP"))
		style.setHasDropCap(static_cast<bool>(attrAsInt(attrs, "DROP")));
	if ( attrHasValue(attrs, "DROPLIN"))
		style.setDropCapLines(attrAsInt(attrs, "DROPLIN"));
	if ( attrHasValue(attrs, "DROPDIST"))
		style.setDropCapOffset(attrAsDbl(attrs, "DROPDIST"));
	if ( attrHasValue(attrs, "PSHORTCUT"))
		style.setShortcut(attrAsString(attrs, "PSHORTCUT", ""));

	if ( attrHasValue(attrs, "OpticalMargins"))
		style.setOpticalMargins(attrAsInt(attrs, "OpticalMargins"));
	if ( attrHasValue(attrs, "HyphenationMode"))
		style.setHyphenationMode(attrAsInt(attrs, "HyphenationMode"));
	if ( attrHasValue(attrs, "MinWordTrack"))
		style.setMinWordTracking(attrAsDbl(attrs, "MinWordTrack"));
	if ( attrHasValue(attrs, "NormWordTrack"))
		style.charStyle().setWordTracking(attrAsDbl(attrs, "NormWordTrack"));
	if ( attrHasValue(attrs, "MinGlyphShrink"))
		style.setMinGlyphExtension(attrAsDbl(attrs, "MinGlyphShrink"));
	if ( attrHasValue(attrs, "MaxGlyphExtend"))
		style.setMaxGlyphExtension(attrAsDbl(attrs, "MaxGlyphExtend"));

	ReadCStyle(reader, style.charStyle(), doc);

	QList<ParagraphStyle::TabRecord> tabs;
	while (!reader.atEnd() && !reader.hasError())
	{
//		QXmlStreamReader::TokenType tType = reader.readNext();
		reader.readNext();
		QString tagName = reader.name().toString();
		if (tagName == "PARA" && reader.isEndElement()) 
			break;
		if (tagName == "Tabs" && reader.isStartElement())
		{
			ParagraphStyle::TabRecord tab;
			QXmlStreamAttributes tabAttrs = reader.attributes();
			tab.tabPosition = attrAsDbl(tabAttrs, "Pos");
			tab.tabType     = attrAsInt(tabAttrs, "Type");
			QString tbCh    = attrAsString(tabAttrs, "Fill","");
			tab.tabFillChar = tbCh.isEmpty() ? QChar() : tbCh.at(0);
			tabs.append(tab);
		}
	}
	if (tabs.count() < 0)
		style.setTabValues(tabs);
}

