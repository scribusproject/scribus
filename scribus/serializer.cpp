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
#include <q3textstream.h>
#include <qtextcodec.h>
//Added by qt3to4:
#include <Q3CString>
#include <Q3ValueList>
#include <Q3PtrList>
#include "sccolor.h"
#include "util.h"
#include "resourcecollection.h"

#include "desaxe/simple_actions.h"
#include "desaxe/saxXML.h"
#include "desaxe/uniqueid.h"


using namespace desaxe;


struct Collection
{
	Q3PtrList<PageItem> items;
	ColorList colors;
	StyleSet<ParagraphStyle> pstyles;
	StyleSet<CharStyle> cstyles;
	Q3ValueList<QString> fonts;
	Q3ValueList<QString> patterns;

	void collectItem(PageItem* p)              { items.append(p); }
	void collectColor(QString name, ScColor c) { colors[name] = c; }
	void collectStyle(ParagraphStyle* style)   { if (style && !style->name().isEmpty()) pstyles.append(style); }
	void collectCharStyle(CharStyle* style)    { if (style && !style->name().isEmpty()) cstyles.append(style); }
	void collectFont(QString name)             { fonts.append(name); }
	void collectPattern(QString name)          { patterns.append(name); }
};


class CollectColor_body : public Action_body
{
	void begin (const Xml_string tagname, Xml_attr attr)
	{
		m_name = attr["name"];
	}
	
	void end (const Xml_string tagname)
	{
//		qDebug(QString("collect %1").arg(tagname));
		Collection* coll = this->dig->top<Collection>(1);
		ScColor* color = this->dig->top<ScColor>();
		coll->collectColor(m_name, *color);
	}
private:
	QString m_name;
};

class CollectColor : public MakeAction<CollectColor_body>
{};


Serializer::Serializer(ScribusDoc& doc) : Digester(), m_Doc(doc)
{
	// register desaxe rules for styles, colors and elems
	addRule("/SCRIBUSFRAGMENT", Factory<Collection>());
	addRule("/SCRIBUSFRAGMENT", Store<Collection>("<collection>"));
	
	addRule("/SCRIBUSFRAGMENT/color", Factory<ScColor>());
	addRule("/SCRIBUSFRAGMENT/color", SetAttribute<ScColor, QString>( &ScColor::setNamedColor, "RGB" ));
	addRule("/SCRIBUSFRAGMENT/color", SetAttribute<ScColor, QString>( &ScColor::setNamedColor, "CMYK" ));
	addRule("/SCRIBUSFRAGMENT/color", SetAttributeWithConversion<ScColor, bool>( &ScColor::setSpotColor, "Spot", &parseBool ));
	addRule("/SCRIBUSFRAGMENT/color", SetAttributeWithConversion<ScColor, bool>( &ScColor::setRegistrationColor, "Register", &parseBool ));
	addRule("/SCRIBUSFRAGMENT/color", CollectColor());

	CharStyle::desaxeRules("/SCRIBUSFRAGMENT/", *this);
	addRule("/SCRIBUSFRAGMENT/charstyle", SetterP<Collection, CharStyle>( & Collection::collectCharStyle ));
	
	ParagraphStyle::desaxeRules("/SCRIBUSFRAGMENT/", *this);
	addRule("/SCRIBUSFRAGMENT/style", SetterP<Collection, ParagraphStyle>( & Collection::collectStyle ));

	addRule("/SCRIBUSFRAGMENT/font", SetAttribute<Collection, QString>( & Collection::collectFont, "name"));

	PageItem::desaxeRules("", *this, "item");
	addRule("/SCRIBUSFRAGMENT/item", SetterP<Collection,PageItem>( & Collection::collectItem ));
}


void Serializer::serializeObjects(const Selection& selection, SaxHandler& outputhandler)
{
	Xml_attr attr;
	UniqueID handler( & outputhandler );
	handler.beginDoc();
	handler.begin("SCRIBUSFRAGMENT", attr);
	ScribusDoc* doc = selection.itemAt(0)->doc();
	
	
	QMap<QString,int>::Iterator itf;
	for (itf = doc->UsedFonts.begin(); itf != doc->UsedFonts.end(); ++itf)
	{
		attr["name"] = itf.key();
		handler.beginEnd("font", attr);
	}
	
	ColorList usedColors;
	doc->getUsedColors(usedColors, false);
	ColorList::Iterator itc;
	for (itc = usedColors.begin(); itc != usedColors.end(); ++itc)
	{
		Xml_attr cattr;
		cattr["name"] = itc.key();
		if (doc->PageColors[itc.key()].getColorModel() == colorModelRGB)
			cattr["RGB"] = doc->PageColors[itc.key()].nameRGB();
		else
			cattr["CMYK"] = doc->PageColors[itc.key()].nameCMYK();
		cattr["Spot"] = toXMLString(doc->PageColors[itc.key()].isSpotColor());
		cattr["Register"] = toXMLString(doc->PageColors[itc.key()].isRegistrationColor());
		handler.beginEnd("color", cattr);
	}
	
	ResourceCollection lists;
	for (uint i=0; i < doc->Items->count(); ++i)
		doc->Items->at(i)->getNamedResources(lists);
	
	Q3ValueList<QString>::Iterator it;
	Q3ValueList<QString> names = lists.styleNames();
	for (it = names.begin(); it != names.end(); ++it)
		doc->paragraphStyles()[*it].saxx(handler);

	names = lists.charStyleNames();
	for (it = names.begin(); it != names.end(); ++it)
		doc->charStyles()[*it].saxx(handler);
	
	names = lists.lineStyleNames();
	for (it = names.begin(); it != names.end(); ++it)
	{
		Xml_attr multiattr;
		multiattr["Name"] = *it;
		handler.begin("MultiLine", multiattr);		
		multiLine ml = doc->MLineStyles[*it];
		
		Q3ValueVector<SingleLine>::Iterator itMU2;
		for (itMU2 = ml.begin(); itMU2 != ml.end(); ++itMU2)
		{
			Xml_attr lineattr;
			lineattr["Color"] = (*itMU2).Color;
			lineattr["Shade"] = (*itMU2).Shade;
			lineattr["Dash"] = (*itMU2).Dash;
			lineattr["LineEnd"] = (*itMU2).LineEnd;
			lineattr["LineJoin"] = (*itMU2).LineJoin;
			lineattr["Width"] = (*itMU2).Width;
			handler.beginEnd("SubLine", lineattr);
		}
		handler.end("MultiLine");
	}

	/*	names = lists.patterns();
	for (it = names.begin(); it != names.end(); ++it)
		doc->patterns[*it].saxx(handler);
*/
	
	for (uint i=0; i < doc->Items->count(); ++i)
	{
		int k = selection.findItem(doc->Items->at(i));
		if (k >=0)
			doc->Items->at(i)->saxx(handler);
	}

	handler.end("SCRIBUSFRAGMENT");
	handler.endDoc();
}


Selection Serializer::deserializeObjects(const Q3CString & xml)
{
	store<ScribusDoc>("<scribusdoc>", &m_Doc);

	parseMemory(xml, xml.length());

	return importCollection();
}

Selection Serializer::deserializeObjects(const QFile & file)
{
	store<ScribusDoc>("<scribusdoc>", &m_Doc);

	QFileInfo fi(file);
	parseFile(fi.filePath());

	return importCollection();
}


Selection Serializer::importCollection()
{	
	Collection* coll = lookup<Collection>("<collection>");
	Selection result( &m_Doc, false);
//	qDebug(QString("deserialize: collection %1 doc %2").arg((ulong)coll).arg((ulong)&m_Doc));
	if (coll == NULL)
		qDebug("deserialize: no objects collected");
	else
	{
		QMap<QString,QString> newNames;
		for (uint i = 0; i < coll->pstyles.count(); ++i)  // FIXME:  QValueList<QString> StyleSet::names()
		{
			QString oldName = coll->pstyles[i].name();
			QString newName = oldName;
			int counter = 0;
			while (m_Doc.paragraphStyles().find(newName) >= 0)
				newName = (QObject::tr("Copy of %1 (%2)")).arg(oldName).arg(++counter);
			newNames[oldName] = newName;
		}
		//ResourceCollection::makeUnique(newNames, mDoc->paragraphStyles().names());
		coll->pstyles.rename(newNames);
		m_Doc.redefineStyles(coll->pstyles, false);
		
		newNames.clear();
		for (uint i = 0; i < coll->cstyles.count(); ++i)  
		{
			QString oldName = coll->cstyles[i].name();
			QString newName = oldName;
			int counter = 0;
			while (m_Doc.charStyles().find(newName) >= 0)
				newName = (QObject::tr("Copy of %1 (%2)")).arg(oldName).arg(++counter);
			newNames[oldName] = newName;
		}
		coll->cstyles.rename(newNames);
		m_Doc.redefineCharStyles(coll->cstyles, false);
		
		//TODO: patterns
		//TODO: fonts
		//TODO: linestyles
		
		Q3PtrList<PageItem>* objects = &(coll->items);
		
//		qDebug(QString("deserialize: objects %1").arg((ulong)objects));
		
		for (uint i=0; i < objects->count(); ++i)
		{
//			qDebug(QString("deserialized item: %1,%2").arg(objects->at(i)->xPos()).arg(objects->at(i)->yPos()));
			PageItem* currItem = objects->at(i);
			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
			result.addItem(currItem);
		}
//		qDebug(QString("deserialize: %1 objects, colors %2 %3").arg(objects->count()).arg((ulong)&(m_Doc.PageColors)).arg((ulong)&(coll->colors)));		
		m_Doc.PageColors.addColors(coll->colors, false);
//		qDebug(QString("deserialize: delete collection... %1").arg(result.count()));
		delete coll;
	}
//	qDebug(QString("deserialize done: %1 items").arg(result.count()));
	return result;
}


bool Serializer::writeWithEncoding(const QString& filename, const QString& encoding, 
								   const QString& txt)
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding);
	Q3CString dec = codec->fromUnicode( txt );
	QFile f(filename);
	if (f.open(QIODevice::WriteOnly))
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
	Q3CString file;
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
