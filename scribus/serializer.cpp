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
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>
extern bool loadText(QString nam, QString *Buffer);
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
  bool Uni = false;
  QString Dat = "";
	QPtrList<Pti> y = Item->Ptext;
  for (a=0; a<y.count(); ++a)
  	{
		QString b = y.at(a)->ch;
		if (b == QChar(13))
			b = "\n";
		if (b[0].unicode() > 255)
			Uni = true;
    Dat += b;
    }
  if (Uni)
		Objekt = Dat.utf8();
	else
		Objekt = Dat;
}

void Serializer::GetText(PageItem *Item, int Absatz, bool Append)
{
	struct Pti *hg;
	PageItem *nb;
	uint a;
	if (!Append)
		{
		if (Item->NextBox != 0)
			{
			nb = Item->NextBox;
			while (nb != 0)
				{
				nb->Ptext.clear();
				nb->CPos = 0;
				nb->Dirty = true;
				nb = nb->NextBox;
				}
			}
		Item->Ptext.clear();
		Item->CPos = 0;
		}
	for (a=0; a<Objekt.length(); ++a)
		{
		hg = new Pti;
		hg->ch = Objekt.at(a);
		if (hg->ch == QChar(10)) { hg->ch = QChar(13); }
		if (hg->ch == QChar(5)) { hg->ch = QChar(13); }
		if (hg->ch == QChar(9)) { hg->ch = " "; }
		hg->cfont = Item->IFont;
		hg->csize = Item->ISize;
		hg->ccolor = Item->Pcolor2;
		hg->cextra = 0;
		hg->cshade = Item->Shade2;
		hg->cselect = false;
		hg->cstyle = 0;
 		hg->cab = Absatz;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		if (Append)
 			Item->Ptext.insert(Item->CPos, hg);
		else
			Item->Ptext.append(hg);
 		Item->CPos += 1;
		}
}

bool Serializer::Write()
{
	QFile f(Filename);
	bool ret = false;
	if (f.open(IO_WriteOnly))
		{
		QTextStream t(&f);
		t.writeRawBytes(Objekt, Objekt.length());
		f.close();
		ret = true;
		}
	else
		{
		ret = false;
		}
	return ret;
}

bool Serializer::Read()
{
	bool tmp = loadText(Filename, &Objekt);
	QTextCodec* codec = QTextCodec::codecForContent(Objekt, Objekt.length());
	if (codec)
		{
		if (QString(codec->name()) == "UTF-8")
			{
			QString dec = QString::fromUtf8(Objekt);
			Objekt = dec;
			}
		}	
	return tmp;
}
