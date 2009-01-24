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
	for (int i=0; i < doc->Items->count(); ++i)
		doc->Items->at(i)->getNamedResources(lists);
	
	QList<QString>::Iterator it;
	QList<QString> names = lists.styleNames();
	for (it = names.begin(); it != names.end(); ++it)
		doc->paragraphStyles().get(*it).saxx(handler);

	names = lists.charStyleNames();
	for (it = names.begin(); it != names.end(); ++it)
		doc->charStyles().get(*it).saxx(handler);
	
	names = lists.lineStyleNames();
	for (it = names.begin(); it != names.end(); ++it)
	{
		Xml_attr multiattr;
		multiattr["Name"] = *it;
		handler.begin("MultiLine", multiattr);		
		multiLine ml = doc->MLineStyles[*it];
		
		multiLine::Iterator itMU2;
		for (itMU2 = ml.begin(); itMU2 != ml.end(); ++itMU2)
		{
			Xml_attr lineattr;
			lineattr["Color"] = (*itMU2).Color;
			lineattr["Shade"] = toXMLString((*itMU2).Shade);
			lineattr["Dash"] = toXMLString((*itMU2).Dash);
			lineattr["LineEnd"] = toXMLString((*itMU2).LineEnd);
			lineattr["LineJoin"] = toXMLString((*itMU2).LineJoin);
			lineattr["Width"] = toXMLString((*itMU2).Width);
			handler.beginEnd("SubLine", lineattr);
		}
		handler.end("MultiLine");
	}

	/*	names = lists.patterns();
	for (it = names.begin(); it != names.end(); ++it)
		doc->patterns[*it].saxx(handler);
*/
/*
	QStringList patterns = doc->getUsedPatternsSelection((Selection*)&selection);
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern& pa = doc->docPatterns[patterns[c]];
		Xml_attr cattr;
		cattr["Name"] = patterns[c];
		cattr["scaleX"] = toXMLString(pa.scaleX);
		cattr["scaleY"] = toXMLString(pa.scaleY);
		cattr["width"] = toXMLString(pa.width);
		cattr["height"] = toXMLString(pa.height);
		cattr["xoffset"] = toXMLString(pa.xoffset);
		cattr["yoffset"] = toXMLString(pa.yoffset);
		handler.begin("Pattern", cattr);
		for (int o = 0; o < pa.items.count(); o++)
		{
			pa.items.at(o)->saxx(handler);
		}
		handler.end("Pattern");
	}
*/
	for (int i=0; i < doc->Items->count(); ++i)
	{
		int k = selection.findItem(doc->Items->at(i));
		if (k >=0)
			doc->Items->at(i)->saxx(handler);
	}

	handler.end("SCRIBUSFRAGMENT");
	handler.endDoc();
}


Selection Serializer::cloneObjects(const Selection& objects)
{
	backUpColors = m_Doc.PageColors;
	store<ScribusDoc>("<scribusdoc>", &m_Doc);

	serializeObjects(objects, *this);
	
	return importCollection();
}
	

Selection Serializer::deserializeObjects(const QByteArray & xml)
{
	backUpColors = m_Doc.PageColors;
	store<ScribusDoc>("<scribusdoc>", &m_Doc);

	parseMemory(xml, xml.length());

	return importCollection();
}

Selection Serializer::deserializeObjects(const QFile & file)
{
	backUpColors = m_Doc.PageColors;
	store<ScribusDoc>("<scribusdoc>", &m_Doc);

	QFileInfo fi(file);
	parseFile(fi.filePath());

	return importCollection();
}


Selection Serializer::importCollection()
{
	Collection* coll = lookup<Collection>("<collection>");
	Selection result( &m_Doc, false);
//	qDebug() << QString("deserialize: collection %1 doc %2").arg((ulong)coll).arg((ulong)&m_Doc);
	if (coll == NULL)
		qDebug() << "deserialize: no objects collected";
	else
	{
		QMap<QString,QString> newNames;

		//TODO: fonts

		do {
			newNames.clear();
			for (int i = 0; i < coll->cstyles.count(); ++i)
			{
				QString oldName = coll->cstyles[i].name();
				int oldIndex = m_Doc.charStyles().find(oldName);
				if (oldIndex >= 0 && m_Doc.charStyle(oldName) == coll->cstyles[i])
					continue;
				QString newName = oldName;
				if (oldIndex >= 0 && !newNames.contains(oldName))
				{
					int counter = 0;
					while (m_Doc.charStyles().find(newName) >= 0)
						newName = (QObject::tr("Copy of %1 (%2)")).arg(oldName).arg(++counter);
					newNames[oldName] = newName;
				}
			}
			
			coll->cstyles.rename(newNames);
		}
		while (newNames.count() > 0);
		m_Doc.redefineCharStyles(coll->cstyles, false);		

		do {
			newNames.clear();
			for (int i = 0; i < coll->pstyles.count(); ++i)  // FIXME:  QValueList<QString> StyleSet::names()
			{
				QString oldName = coll->pstyles[i].name();
				int oldIndex = m_Doc.paragraphStyles().find(oldName);
//				qDebug() << QString("comparing %1 (old %2 new %3): parent '%4'='%5' cstyle %6 equiv %7").arg(oldName).arg(oldIndex).arg(i)
//					   .arg(oldIndex>=0? m_Doc.paragraphStyle(oldName).parent() : "?").arg(coll->pstyles[i].parent())
//					   .arg(oldIndex>=0? m_Doc.paragraphStyle(oldName).charStyle() == coll->pstyles[i].charStyle() : false)
//					   .arg(oldIndex>=0? m_Doc.paragraphStyle(oldName).equiv(coll->pstyles[i]) : false);
			
				if (oldIndex >= 0 && coll->pstyles[i] == m_Doc.paragraphStyle(oldName) )
					continue;
				QString newName = oldName;
				if (oldIndex >= 0 && !newNames.contains(oldName))
				{
					int counter = 0;
					while (m_Doc.paragraphStyles().find(newName) >= 0)
						newName = (QObject::tr("Copy of %1 (%2)")).arg(oldName).arg(++counter);
					newNames[oldName] = newName;
				}
			}
			coll->pstyles.rename(newNames);
		}
		while(newNames.count() > 0);
		
		m_Doc.redefineStyles(coll->pstyles, false);		

		//TODO: linestyles : this is temporary code until MultiLine is replaced by LineStyle
		QMap<QString,multiLine>::Iterator mlit;
		for (mlit = coll->lstyles.begin(); mlit != coll->lstyles.end(); ++mlit)
		{
			multiLine& ml = mlit.value();
			QString    oldName = mlit.key();
			QString    newName = mlit.key();
			QMap<QString,multiLine>::ConstIterator mlitd = m_Doc.MLineStyles.find(oldName);
			if (mlitd != m_Doc.MLineStyles.end() && ml != mlitd.value())
			{
				int counter = 0;
				while (m_Doc.MLineStyles.contains(newName))
					newName = (QObject::tr("Copy of %1 (%2)")).arg(oldName).arg(++counter);
			}
			m_Doc.MLineStyles.insert(newName, ml);
		}

		//TODO: patterns
		
		QList<PageItem*>* objects = &(coll->items);
		m_Doc.PageColors = backUpColors;
		m_Doc.PageColors.addColors(coll->colors, false);
		int maxG = m_Doc.GroupCounter;
		for (int i=0; i < objects->count(); ++i)
		{
			PageItem* currItem = objects->at(i);
			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
			currItem->setFillQColor();
			currItem->setLineQColor();
			result.addItem(currItem);
			if (currItem->Groups.count() != 0)
			{
				QStack<int> groups;
				for (int i=0; i < currItem->groups().count(); ++i)
				{
					int newGroup = m_Doc.GroupCounter + currItem->groups()[i] - 1;
					groups.append(newGroup);
				}
				currItem->setGroups(groups);
				maxG = qMax(maxG, currItem->Groups.top()+1);
			}
		}
		m_Doc.GroupCounter = maxG;
		updateGradientColors(coll->colors);
		delete coll;
	}
	return result;
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

void Serializer::updateGradientColors(const ColorList& colors)
{
	VColorStop* grStop;
	uint itemsCount = m_Doc.Items->count();
	for (uint c=0; c < itemsCount; ++c)
	{
		PageItem *ite = m_Doc.Items->at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			grStop = cstops.at(cst);
			if (colors.contains(grStop->name))
				grStop->color = SetColor(&m_Doc, grStop->name, grStop->shade);
		}
	}
	uint masterItemsCount =  m_Doc.MasterItems.count();
	for (uint c=0; c < masterItemsCount; ++c)
	{
		PageItem *ite = m_Doc.MasterItems.at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			grStop = cstops.at(cst);
			if (colors.contains(grStop->name))
				grStop->color = SetColor(&m_Doc, grStop->name, grStop->shade);
		}
	}
	uint frameItemsCount = m_Doc.FrameItems.count();
	for (uint c=0; c < frameItemsCount; ++c)
	{
		PageItem *ite = m_Doc.FrameItems.at(c);
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			grStop = cstops.at(cst);
			if (colors.contains(grStop->name))
				grStop->color = SetColor(&m_Doc, grStop->name, grStop->shade);
		}
	}
	QStringList patterns =m_Doc.docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern& pa = m_Doc.docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *ite = pa.items.at(o);
			QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				grStop = cstops.at(cst);
				if (colors.contains(grStop->name))
					grStop->color = SetColor(&m_Doc, grStop->name, grStop->shade);
			}
		}
	}
}
