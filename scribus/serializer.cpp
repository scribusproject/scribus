/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          serializer.cpp  -  description
                             -------------------
    begin                : Sat May 5 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "serializer.h"
#include "scribusdoc.h"
#include "pageitem.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include "util.h"

Serializer::Serializer(QString name)
{
	Filename = name;
	Objekt = "";
}

QString Serializer::GetObjekt()
{
	return Objekt;
}

void Serializer::PutText(PageItem *Item)
{
	uint a;
	QString Dat = "";
	QPtrList<ScText> y = Item->itemText;
	for (a=0; a<y.count(); ++a)
	{
		QString b = y.at(a)->ch;
		if (b == QChar(13))
			b = "\n";
		Dat += b;
	}
	Objekt = Dat;
}

void Serializer::GetText(PageItem *Item, int Absatz, QString font, int size, bool Append)
{
	ScText *hg;
	PageItem *nextItem;
	PageItem *it = Item;
	ScribusDoc* doku = it->document();
	uint a;
	if (!Append)
	{
		nextItem = Item;
		while (nextItem != 0)
		{
			if (nextItem->BackBox != 0)
				nextItem = nextItem->BackBox;
			else
				break;
		}
		it = nextItem;
		while (nextItem != 0)
		{
			for (ScText *it = nextItem->itemText.first(); it != 0; it = nextItem->itemText.next())
			{
				if ((it->ch == QChar(25)) && (it->cembedded != 0))
				{
					doku->FrameItems.remove(it->cembedded);
					delete it->cembedded;
				}
			}
			nextItem->itemText.clear();
			nextItem->CPos = 0;
			nextItem = nextItem->NextBox;
		}
		doku->updateFrameItems();
	}
	
	const ParagraphStyle& pstyle(doku->docParagraphStyles[Absatz]);
	uint len = Objekt.length();
	for (a=0; a < len; ++a)
	{
		QChar ch(Objekt.at(a));
		uint unicd = ch.unicode();
		if (unicd == 0 || unicd == 13)
			continue;
		if (unicd == 10 || unicd == 5)
			ch = QChar(13);
		hg = new ScText;
		hg->ch = ch;
		if (!pstyle.Font.isEmpty())
		{
			hg->cfont = (*doku->AllFonts)[pstyle.Font];
			hg->csize = pstyle.FontSize;
			hg->cstyle = pstyle.FontEffect;
			hg->ccolor = pstyle.FColor;
			hg->cshade = pstyle.FShade;
			hg->cstroke = pstyle.SColor;
			hg->cshade2 = pstyle.SShade;
			hg->cshadowx = pstyle.txtShadowX;
			hg->cshadowy = pstyle.txtShadowY;
			hg->coutline = pstyle.txtOutline;
			hg->cunderpos = pstyle.txtUnderPos;
			hg->cunderwidth = pstyle.txtUnderWidth;
			hg->cstrikepos = pstyle.txtStrikePos;
			hg->cstrikewidth = pstyle.txtStrikeWidth;
			hg->cscale = pstyle.scaleH;
			hg->cscalev = pstyle.scaleV;
			hg->cbase = pstyle.baseOff;
			hg->cextra = pstyle.kernVal;
		}
		else
		{
			hg->cfont = (*doku->AllFonts)[font];
			hg->ccolor = it->TxtFill;
			hg->cshade = it->ShTxtFill;
			hg->cstroke = it->TxtStroke;
			hg->cshade2 = it->ShTxtStroke;
			hg->csize = size;
			hg->cstyle = it->TxTStyle;
			hg->cshadowx = it->TxtShadowX;
			hg->cshadowy = it->TxtShadowY;
			hg->coutline = it->TxtOutline;
			hg->cunderpos = it->TxtUnderPos;
			hg->cunderwidth = it->TxtUnderWidth;
			hg->cstrikepos = it->TxtStrikePos;
			hg->cstrikewidth = it->TxtStrikeWidth;
			hg->cscale = it->TxtScale;
			hg->cscalev = it->TxtScaleV;
			hg->cbase = it->TxtBase;
			hg->cextra = 0;
		}
		hg->cselect = false;
		hg->cab = Absatz;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		hg->cembedded = 0;
		if (Append)
			it->itemText.insert(it->CPos, hg);
		else
			it->itemText.append(hg);
		it->CPos += 1;
	}
}

bool Serializer::Write(QString Cod)
{
	QTextCodec *codec;
	if (Cod.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(Cod);
	QCString dec = codec->fromUnicode( Objekt );
	QFile f(Filename);
	bool ret = false;
	if (f.open(IO_WriteOnly))
	{
		f.writeBlock(dec, dec.length());
		f.close();
		ret = true;
	}
	return ret;
}

bool Serializer::Read(QString Cod)
{
	QCString file;
	QTextCodec *codec;
	bool tmp = loadRawText(Filename, file);
	if (Cod.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(Cod);
	Objekt = codec->toUnicode( file.data() );
	return tmp;
}
