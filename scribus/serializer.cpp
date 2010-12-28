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
#include "scribusstructs.h"
#include "pageitem.h"
#include "sccolor.h"
#include "scpattern.h"
#include "util.h"
#include "util_math.h"
#include "util_color.h"
#include "resourcecollection.h"

#include "desaxe/simple_actions.h"
#include "desaxe/saxXML.h"
#include "desaxe/uniqueid.h"

#include <QTextCodec>
#include <QList>
#include <QDebug>

using namespace desaxe;


struct Collection
{
	QList<PageItem*> items;
	ColorList colors;
	StyleSet<ParagraphStyle> pstyles;
	StyleSet<CharStyle> cstyles;
	QMap<QString,multiLine> lstyles;
	QList<QString> fonts;
	QList<QString> patterns;

	void collectItem(PageItem* p)              { items.append(p); }
	void collectColor(QString name, ScColor c) { colors[name] = c; }
	void collectStyle(ParagraphStyle* style)   { if (style && !style->name().isEmpty()) pstyles.append(style); }
	void collectCharStyle(CharStyle* style)    { if (style && !style->name().isEmpty()) cstyles.append(style); }
	void collectLineStyle(QString name, multiLine& m) { lstyles[name] = m; }
	void collectFont(QString name)             { fonts.append(name); }
	void collectPattern(QString name)          { patterns.append(name); }
};


class CollectColor_body : public Action_body
{
	void begin (const Xml_string& tagname, Xml_attr attr)
	{
		m_name = attr["name"];
	}
	
	void end (const Xml_string& tagname)
	{
//		qDebug() << QString("collect %1").arg(tagname);
		Collection* coll = this->dig->lookup<Collection>("<collection>");
		ScColor* color = this->dig->top<ScColor>();
		coll->collectColor(m_name, *color);
	}
private:
	QString m_name;
};

class CollectColor : public MakeAction<CollectColor_body>
{};

class CollectMultiLine_body : public Action_body
{
	void begin (const Xml_string& tagname, Xml_attr attr)
	{
		m_name = attr["Name"];
	}
	
	void end (const Xml_string& tagname)
	{
//		qDebug() << QString("collect %1").arg(tagname);
		Collection* coll  = this->dig->lookup<Collection>("<collection>");
		multiLine*  mline = this->dig->top<multiLine>();
		coll->collectLineStyle(m_name, *mline);
	}
private:
	QString m_name;
};

class CollectMultiLine : public MakeAction<CollectMultiLine_body>
{};

class CollectSingleLine_body : public Action_body
{
	void begin (const Xml_string& tagname, Xml_attr attr)
	{
	}
	
	void end (const Xml_string& tagname)
	{
//		qDebug() << QString("collect %1").arg(tagname);
//		Collection* coll  = this->dig->lookup<Collection>("<collection>");
		multiLine*  mline = this->dig->lookup<multiLine>("<multiline>");
		SingleLine* sline = this->dig->top<SingleLine>();
		mline->append(*sline);
	}
};

class CollectSingleLine : public MakeAction<CollectSingleLine_body>
{};

class MergeColors_body : public Action_body
{
	void end (const Xml_string& tagname)
	{
//		qDebug() << QString("collect %1").arg(tagname);
		Collection* coll = this->dig->lookup<Collection>("<collection>");
		ScribusDoc* doc  = this->dig->lookup<ScribusDoc>("<scribusdoc>");
		doc->PageColors.addColors(coll->colors, false);
	}
};

class MergeColors : public MakeAction<MergeColors_body>
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

	addRule("/SCRIBUSFRAGMENT/MultiLine", Factory<multiLine>());
	addRule("/SCRIBUSFRAGMENT/MultiLine", Store<multiLine>("<multiline>"));
	addRule("/SCRIBUSFRAGMENT/MultiLine", CollectMultiLine());

	addRule("/SCRIBUSFRAGMENT/MultiLine/SubLine", Factory<SingleLine>());
	addRule("/SCRIBUSFRAGMENT/MultiLine/SubLine", SetAttributeWithConversion<SingleLine, const QString&> ( &SingleLine::setColor, "Color", &parse<const Xml_string&>, "Black"));
	addRule("/SCRIBUSFRAGMENT/MultiLine/SubLine", SetAttributeWithConversion<SingleLine, int>( &SingleLine::setShade, "Shade", &parseInt ));
	addRule("/SCRIBUSFRAGMENT/MultiLine/SubLine", SetAttributeWithConversion<SingleLine, int>( &SingleLine::setDash , "Dash", &parseInt ));
	addRule("/SCRIBUSFRAGMENT/MultiLine/SubLine", SetAttributeWithConversion<SingleLine, int>( &SingleLine::setLineEnd , "LineEnd", &parseInt ));
	addRule("/SCRIBUSFRAGMENT/MultiLine/SubLine", SetAttributeWithConversion<SingleLine, int>( &SingleLine::setLineJoin, "LineJoin", &parseInt ));
	addRule("/SCRIBUSFRAGMENT/MultiLine/SubLine", SetAttributeWithConversion<SingleLine, double>( &SingleLine::setLineWidth, "Width", &parseDouble ));
	addRule("/SCRIBUSFRAGMENT/MultiLine/SubLine", CollectSingleLine());

	addRule("/SCRIBUSFRAGMENT/font", SetAttribute<Collection, QString>( & Collection::collectFont, "name"));

	PageItem::desaxeRules("", *this, "item");
	addRule("/SCRIBUSFRAGMENT/item", SetterP<Collection,PageItem>( & Collection::collectItem ));
}

bool Serializer::writeWithEncoding(const QString& filename, const QString& encoding, 
								   const QString& txt)
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	QByteArray dec = codec->fromUnicode( txt );
	QFile f(filename);
	if (f.open(QIODevice::WriteOnly))
	{
		f.write(dec, dec.length());
		f.close();
		return true;
	}
	return false;
}

bool Serializer::readWithEncoding(const QString& filename, const QString& encoding, 
								  QString &txt)
{
	QByteArray file;
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	if (loadRawText(filename, file))
	{
		txt = codec->toUnicode( file.data() );
		return true;
	}
	return false;
}
