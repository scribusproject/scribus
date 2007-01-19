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
#include "text/nlsconfig.h"

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
	int a;
	QString Dat = "";
	for (a=0; a < Item->itemText.length(); ++a)
	{
		QString b = Item->itemText.text(a, 1);
		if (b == QChar(13))
			b = "\n";
		Dat += b;
	}
	Objekt = Dat;
}

void Serializer::GetText(PageItem *Item, int Absatz, QString font, int size, bool Append)
{
//	PageItem *nextItem;
	PageItem *it = Item;
	ScribusDoc* doku = it->doc();
	if (Absatz < 0 || Absatz > signed(doku->paragraphStyles().count()))
		Absatz = 0;
	
	if (!Append)
	{
		it->itemText.clear();
		it->CPos = 0;
	}
	QString txt = Objekt.remove(QChar(0)).remove(QChar(13));
	txt = txt.replace(QChar(10), QChar(13)).replace(QChar(5), QChar(13));
	uint insPos = Append? it->CPos : it->itemText.length();
	it->itemText.insertChars(insPos, txt);
	ParagraphStyle newStyle;
	CharStyle newCharStyle;
	if ( Absatz > 0 )
		newStyle.setParent(doku->paragraphStyles()[Absatz].name());
	if ( !font.isEmpty() )
		newCharStyle.setFont((*doku->AllFonts)[font]);
	if ( size > 0 )
		newCharStyle.setFontSize(size);
	
	if (Append)
	{
		it->itemText.applyCharStyle(insPos, txt.length(), newCharStyle);
		for (uint i=0; i < it->itemText.nrOfParagraphs(); ++i) {
			uint pos = it->itemText.startOfParagraph(i);
			if (pos >= insPos + txt.length())
				break;
			if (pos >= insPos)
				it->itemText.applyStyle(insPos, newStyle);
		}
		it->CPos = insPos + txt.length();
	}
	else if ( Absatz >= 0 || !font.isEmpty() || size > 0)
	{
		newStyle.charStyle() = newCharStyle;
		it->itemText.setDefaultStyle(newStyle);
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
