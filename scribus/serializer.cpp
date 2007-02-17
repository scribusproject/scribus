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
#include "desaxe/simple_actions.h"
#include "desaxe/saxXML.h"

using namespace desaxe;


Serializer::Serializer(ScribusDoc& doc) : Digester(), m_Doc(doc)
{
	// register desaxe rules for styles, colors and elems
}


void Serializer::serializeObjects(const Selection& selection, SaxHandler&)
{
	// write object to tmpfile
	SaxXML tmpfile("tmp-scribus1.xml", true);
	tmpfile.beginDoc();
	selection.itemAt(0)->saxx(tmpfile);
	tmpfile.endDoc();

	// run object thru deserializer
	Digester serializer;
	PageItem::desaxeRules("**", serializer);
	serializer.addRule("**/item", Result<PageItem>());
	serializer.push(selection.itemAt(0)->doc());
	serializer.beginDoc();
	selection.itemAt(0)->saxx(serializer);
	serializer.endDoc();

	// write deserialized object to disk
	SaxXML tmpfile2("tmp-scribus2.xml", true);
	tmpfile2.beginDoc();
	PageItem* des = serializer.result<PageItem>();
	des->saxx(tmpfile2);
	tmpfile2.endDoc();
}


Selection Serializer::deserializeObjects(const QString & xml)
{
	return *m_Doc.m_Selection;
}

Selection Serializer::deserializeObjects(const QFile & xml)
{
	return *m_Doc.m_Selection;
}


bool Serializer::writeWithEncoding(const QString& filename, const QString& encoding, 
								   const QString& txt)
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding);
	QCString dec = codec->fromUnicode( txt );
	QFile f(filename);
	if (f.open(IO_WriteOnly))
	{
		f.writeBlock(dec, dec.length());
		f.close();
		return true;
	}
	return false;
}


bool Serializer::readWithEncoding(const QString& filename, const QString& encoding, 
								  QString &txt)
{
	QCString file;
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding);
	if (loadRawText(filename, file))
	{
		txt = codec->toUnicode( file.data() );
		return true;
	}
	return false;
}
