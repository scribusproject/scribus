/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 /***************************************************************************
  *   Copyright (C) 2004 by Riku Leino                                      *
  *   tsoots@gmail.com                                                      *
  *                                                                         *
  *   This program is free software; you can redistribute it and/or modify  *
  *   it under the terms of the GNU General Public License as published by  *
  *   the Free Software Foundation; either version 2 of the License, or     *
  *   (at your option) any later version.                                   *
  *                                                                         *
  *   This program is distributed in the hope that it will be useful,       *
  *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
  *   GNU General Public License for more details.                          *
  *                                                                         *
  *   You should have received a copy of the GNU General Public License     *
  *   along with this program; if not, write to the                         *
  *   Free Software Foundation, Inc.,                                       *
  *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
  ***************************************************************************/
 
 #include "stylereader.h"
 
 #include <scribusstructs.h>
 #include <gtmeasure.h>
 #include <gtparagraphstyle.h>
 #include <gtframestyle.h>
 #include <gtfont.h>
#include <QByteArray>
 
 StyleReader* StyleReader::sreader = NULL;
 
 extern xmlSAXHandlerPtr sSAXHandler;
 
StyleReader::StyleReader(QString documentName, gtWriter *w,
                         bool textOnly, bool prefix, bool combineStyles)
{
 	sreader      = this;
 	docname      = documentName;
 	readProperties = false;
 	writer       = w;
 	importTextOnly = textOnly;
 	usePrefix    = prefix;
	packStyles   = combineStyles;
 	currentStyle = NULL;
 	parentStyle  = NULL;
 	inList       = false;
 	currentList  = "";
	defaultStyleCreated = false;
}
 
 bool StyleReader::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs) 
 {
 	if (name == "style:default-style")
 		defaultStyle(attrs);
 	else if (name == "style:properties")
 		styleProperties(attrs);
 	else if (name == "style:style")
 	{
		/*if (!defaultStyleCreated)
		{
			gtParagraphStyle* pstyle = new gtParagraphStyle(*(writer->getDefaultStyle()));
			pstyle->setDefaultStyle(true);
			currentStyle = dynamic_cast<gtStyle*>(pstyle);
			currentStyle->setName("default-style");
			defaultStyleCreated = true;
		}*/
 		styleStyle(attrs);
 	}
 	else if (name == "style:tab-stop")
 		tabStop(attrs);
 	else if (name == "text:list-style")
 	{
 		for (int i = 0; i < attrs.count(); ++i)
 			if (attrs.localName(i) == "style:name")
 				currentList = attrs.value(i);
 		inList = true;
 	}
 	else if (((name == "text:list-level-style-bullet") || 
 	          (name == "text:list-level-style-number") ||
 	          (name == "text:list-level-style-image")) && (inList))
 	{
 		QString level = "";
 		for (int i = 0; i < attrs.count(); ++i)
 		{
 			if (attrs.localName(i) == "text:level")
 			{
 				gtStyle *plist;
 				if (attrs.value(i) == "1")
 				{
 					plist = listParents[currentList];
 				}
 				else
 				{
 					int level = (attrs.value(i)).toInt();
 					--level;
 					plist = styles[QString(currentList + "_%1").arg(level)]; 
 				}
 				gtParagraphStyle *pstyle;
 				if (plist == NULL)
 					plist = new gtStyle(*(styles["default-style"]));
 
 				if (plist->target() == "paragraph")
 				{
 					pstyle = dynamic_cast<gtParagraphStyle*>(plist);
					assert(pstyle != NULL);
 					gtParagraphStyle* tmp = new gtParagraphStyle(*pstyle);
 					currentStyle = tmp;
 				}
 				else
 				{
 					gtParagraphStyle* tmp = new gtParagraphStyle(*plist);
 					currentStyle = tmp;
 				}
 				currentStyle->setName(currentList + "_" + attrs.value(i));
 			}
 		}
 		readProperties = true;
 	}
 	else if ((name == "style:drop-cap") && (readProperties))
 	{
 		if (currentStyle->target() == "paragraph")
 		{
 			for (int i = 0; i < attrs.count(); ++i)
 			{
 				if (attrs.localName(i) == "style:lines")
 				{
 					bool ok = false;
 					QString sd = attrs.value(i);
 					int dh = sd.toInt(&ok);
 					if (ok)
 					{
 						gtParagraphStyle* s = dynamic_cast<gtParagraphStyle*>(currentStyle);
						assert(s != NULL);
 						s->setDropCapHeight(dh);
 						s->setDropCap(true);
 					}
 				}
 			}
 		}
 	}
 	else if (name == "style:font-decl")
 	{
 		QString key = "";
 		QString family = "";
 		QString style = "";
 		for (int i = 0; i < attrs.count(); ++i)
 		{
 			if (attrs.localName(i) == "style:name")
 				key = attrs.value(i);
 			else if (attrs.localName(i) == "fo:font-family")
 			{
 				family = attrs.value(i);
 				family = family.remove("'");
 			}
 			else if (attrs.localName(i) == "style:font-style-name")
 				style += attrs.value(i) + " ";
 		}
 		QString name = family + " " + style;
		name = name.simplified();
 		fonts[key] = name;
 	}
 	return true;
 }
 
 void StyleReader::defaultStyle(const QXmlAttributes& attrs)
 {
 	currentStyle = NULL;
 	for (int i = 0; i < attrs.count(); ++i)
 		if (attrs.localName(i) == "style:family")
 			if (attrs.value(i) == "paragraph")
 			{
 				gtParagraphStyle* pstyle = new gtParagraphStyle(*(writer->getDefaultStyle()));
				pstyle->setDefaultStyle(true);
				currentStyle = dynamic_cast<gtStyle*>(pstyle);
				currentStyle->setName("default-style");
 				readProperties = true;
				defaultStyleCreated = true;
 			}
 }
 
 void StyleReader::styleProperties(const QXmlAttributes& attrs)
 {
 	if ((currentStyle == NULL) || (!readProperties))
 		return;
 	gtParagraphStyle* pstyle = NULL;
 	if (currentStyle->target() == "paragraph")
 		pstyle = dynamic_cast<gtParagraphStyle*>(currentStyle);
 	else
 		pstyle = NULL;
 	QString align = NULL;
 	QString force = NULL;
 	bool hasColorTag = false;
 	for (int i = 0; i < attrs.count(); ++i)
 	{
 		if ((attrs.localName(i) == "style:font-name") && (!inList))
 			currentStyle->getFont()->setName(getFont(attrs.value(i)));
 		else if (attrs.localName(i) == "fo:font-size")
 		{
 			double size = 0;
 			double psize = 0;
 			if (parentStyle != NULL)
 				psize = static_cast<double>(parentStyle->getFont()->getSize());
 			else if (styles.contains("default-style"))
 				psize = static_cast<double>(styles["default-style"]->getFont()->getSize());
 
 			psize = psize / 10;
 			size = getSize(attrs.value(i), psize);
 			int nsize = static_cast<int>(size * 10);
 			currentStyle->getFont()->setSize(nsize);
 			if (pstyle)
 				pstyle->setLineSpacing(writer->getPreferredLineSpacing(nsize));
 		}
 		else if ((attrs.localName(i) == "fo:line-height") && (parentStyle != NULL))
 		{
 			gtParagraphStyle* ppstyle;
 			if (parentStyle->target() == "paragraph")
 			{
 				ppstyle = dynamic_cast<gtParagraphStyle*>(parentStyle);
				assert(ppstyle != NULL);
 				ppstyle->setLineSpacing(getSize(attrs.value(i), writer->getPreferredLineSpacing(currentStyle->getFont()->getSize())));
 			}
 		}
 		else if (attrs.localName(i) == "fo:color")
 		{
 			currentStyle->getFont()->setColor(attrs.value(i));
 			hasColorTag = true;
 		}
 		else if ((attrs.localName(i) == "style:use-window-font-color") && (attrs.value(i) == "true"))
 		{
 			currentStyle->getFont()->setColor("Black");
 			hasColorTag = true;
 		}
 		else if ((attrs.localName(i) == "fo:font-weight") && (attrs.value(i) == "bold"))
 			currentStyle->getFont()->setWeight(BOLD);
 		else if ((attrs.localName(i) == "fo:font-style") && (attrs.value(i) == "italic"))
 			currentStyle->getFont()->setSlant(ITALIC);
 		else if ((attrs.localName(i) == "style:text-underline") && (attrs.value(i) != "none"))
 			currentStyle->getFont()->toggleEffect(UNDERLINE);
 		else if ((attrs.localName(i) == "style:text-crossing-out") && (attrs.value(i) != "none"))
 			currentStyle->getFont()->toggleEffect(STRIKETHROUGH);
 		else if ((attrs.localName(i) == "fo:font-variant") && (attrs.value(i) == "small-caps"))
 			currentStyle->getFont()->toggleEffect(SMALL_CAPS);
 		else if ((attrs.localName(i) == "style:text-outline") && (attrs.value(i) == "true"))
 		{
 			currentStyle->getFont()->toggleEffect(OUTLINE);
 			currentStyle->getFont()->setStrokeColor("Black");
 			currentStyle->getFont()->setColor("White");
 		}
 		else if (attrs.localName(i) == "fo:letter-spacing")
 			currentStyle->getFont()->setKerning(static_cast<int>(getSize(attrs.value(i), -1.0)));
 		else if (attrs.localName(i) == "style:text-scale")
 			currentStyle->getFont()->setHscale(static_cast<int>(getSize(attrs.value(i), -1.0)));
 		else if ((attrs.localName(i) == "style:text-position") && 
 		        (((attrs.value(i)).indexOf("sub") != -1) || 
 				(((attrs.value(i)).left(1) == "-") && ((attrs.value(i)).left(1) != "0"))))
 			currentStyle->getFont()->toggleEffect(SUBSCRIPT);
 		else if ((attrs.localName(i) == "style:text-position") && 
 		        (((attrs.value(i)).indexOf("super") != -1) || 
 				(((attrs.value(i)).left(1) != "-") && ((attrs.value(i)).left(1) != "0"))))
 			currentStyle->getFont()->toggleEffect(SUPERSCRIPT);
 		else if ((attrs.localName(i) == "fo:margin-top") && (pstyle != NULL))
 			pstyle->setSpaceAbove(getSize(attrs.value(i)));
 		else if ((attrs.localName(i) == "fo:margin-bottom") && (pstyle != NULL))
 			pstyle->setSpaceBelow(getSize(attrs.value(i)));
 		else if ((attrs.localName(i) == "fo:margin-left") && (pstyle != NULL))
 		{
 			if (inList)
 				pstyle->setIndent(pstyle->getIndent() + getSize(attrs.value(i)));
 			else
 				pstyle->setIndent(getSize(attrs.value(i)));	
 		}
 		else if ((attrs.localName(i) == "text:space-before") && (pstyle != NULL))
 		{
 			if (inList)
 				pstyle->setIndent(pstyle->getIndent() + getSize(attrs.value(i)));
 			else
 				pstyle->setIndent(getSize(attrs.value(i)));
 		}
 		else if ((attrs.localName(i) == "fo:text-indent") && (pstyle != NULL))
 			pstyle->setFirstLineIndent(getSize(attrs.value(i)));
 		else if ((attrs.localName(i) == "fo:text-align") && (pstyle != NULL))
 			align = attrs.value(i);
 		else if ((attrs.localName(i) == "style:justify-single-word") && (pstyle != NULL))
 			force = attrs.value(i);
 	}
	// Qt4 NULL->isNull()
 	if (!align.isNull())
 	{
 		if (align == "end")
 			pstyle->setAlignment(RIGHT);
 		else if (align == "center")
 			pstyle->setAlignment(CENTER);
 		else if (align == "justify")
 		{
 			if (force == "false")
 				pstyle->setAlignment(BLOCK);
 			else
 				pstyle->setAlignment(FORCED);
 		}
 	}
	if (!hasColorTag)
		currentStyle->getFont()->setColor("Black");
 }
 
 void StyleReader::styleStyle(const QXmlAttributes& attrs)
 {
 	QString name = "";
 	QString listName = NULL;
	bool setDefaultStyle = false;
 	bool isParaStyle = false;
 	bool create = true;

	if (!defaultStyleCreated)
	{
		gtParagraphStyle* pstyle = new gtParagraphStyle(*(writer->getDefaultStyle()));
		pstyle->setDefaultStyle(true);
		currentStyle = dynamic_cast<gtStyle*>(pstyle);
		currentStyle->setName("default-style");
		setDefaultStyle     = true;
		defaultStyleCreated = true;
		parentStyle  = currentStyle;
	}

 	for (int i = 0; i < attrs.count(); ++i)
 	{
 		if (attrs.localName(i) == "style:family")
 		{
 			if (attrs.value(i) == "paragraph")
 			{
 				isParaStyle = true;
 				readProperties = true;
 			}
 			else if (attrs.value(i) == "text")
 			{
 				isParaStyle = false;
 				readProperties = true;
 			}
 			else
 			{
 				readProperties = false;
 				return;
 			}
 		}
 		else if (attrs.localName(i) == "style:name")
 			name = attrs.value(i);
 		else if (attrs.localName(i) == "style:parent-style-name")
 		{
 			if (styles.contains(attrs.value(i)))
 				parentStyle = styles[attrs.value(i)];
 			else
 				parentStyle = NULL;
 		}
 		else if (attrs.localName(i) == "style:list-style-name")
 			listName = attrs.value(i);
 	}
 	if ((parentStyle == NULL) && (styles.contains("default-style")))
 		parentStyle = styles["default-style"];
 
 	if (create)
 	{
 		if (parentStyle == NULL)
 		{
 			parentStyle = new gtStyle("tmp-parent");
 		}
 		if (isParaStyle)
 		{
 			gtParagraphStyle *tmpP;
 			if (parentStyle->target() == "paragraph")
 			{
 				tmpP = dynamic_cast<gtParagraphStyle*>(parentStyle);
				assert(tmpP != NULL);
 				gtParagraphStyle* tmp = new gtParagraphStyle(*tmpP);
 // 				tmp->setAutoLineSpacing(true);
 				currentStyle = tmp;
 			}
 			else
 			{
 				gtParagraphStyle* tmp = new gtParagraphStyle(*parentStyle);
 // 				tmp->setAutoLineSpacing(true);
 				currentStyle = tmp;
 			}
 			if (!listName.isNull())
 			{
 				listParents[listName] = currentStyle;
 			}
 		}
 		else
 			currentStyle = new gtStyle(*parentStyle);
 
 		currentStyle->setName(name);
		if (setDefaultStyle)
		{
			gtParagraphStyle* tmp = dynamic_cast<gtParagraphStyle*>(currentStyle);
			if (tmp)
				tmp->setDefaultStyle(true);
		}
 	}
 	else
 		currentStyle = NULL;
 }
 
 void StyleReader::tabStop(const QXmlAttributes& attrs)
 {
 	if (currentStyle->target() == "paragraph")
 	{
 		gtParagraphStyle* pstyle = dynamic_cast<gtParagraphStyle*>(currentStyle);
		assert(pstyle != NULL);
 		QString pos = NULL;
 		QString type = NULL;
 		for (int i = 0; i < attrs.count(); ++i)
 		{
 			if (attrs.localName(i) == "style:position")
 				pos = attrs.value(i);
 			else if (attrs.localName(i) == "style:type")
 				type = attrs.value(i);
 				
 		}
 		if (!pos.isNull())
 		{
 			if (!type.isNull())
 				type = "left";
 			double posd = getSize(pos);
 			if (type == "left")
 				pstyle->setTabValue(posd, LEFT_T);
 			else if (type == "right")
 				pstyle->setTabValue(posd, RIGHT_T);
 			else if (type == "center")
 				pstyle->setTabValue(posd, CENTER_T);
 			else
 				pstyle->setTabValue(posd, CENTER_T);
 		}
 	}
 }
 
 bool StyleReader::endElement(const QString&, const QString&, const QString &name)
 {
 	if ((name == "style:default-style") && (currentStyle != NULL) && (readProperties))
 	{
 		setStyle(currentStyle->getName(), currentStyle);
 		currentStyle = NULL;
 		parentStyle = NULL;
 		readProperties = false;
 	}
 	else if (((name == "style:style") || 
 	          (name == "text:list-level-style-bullet") || 
 			  (name == "text:list-level-style-number") ||
			  (name == "text:list-level-style-image")) && (currentStyle != NULL))
 	{
 		setStyle(currentStyle->getName(), currentStyle);
 		currentStyle = NULL;
 		parentStyle = NULL;
 		readProperties = false;
 	}
 	else if (name == "text:list-style")
 	{
 		inList = false;
 	}

 	return true;
 }
 
 void StyleReader::parse(QString fileName)
 {
#if defined(_WIN32)
	QString fname = QDir::convertSeparators(fileName);
	QByteArray fn = (QSysInfo::WindowsVersion & QSysInfo::WV_NT_based) ? fname.toUtf8() : fname.toLocal8Bit();
#else
	QByteArray fn(fileName.toLocal8Bit());
#endif
 	xmlSAXParseFile(sSAXHandler, fn.data(), 1);
 }

 gtStyle* StyleReader::getDefaultStyle(void)
 {
	 gtStyle* defStyle = writer->getDefaultStyle();
	 StyleMap::Iterator it, itEnd = styles.end();
	 for (it = styles.begin(); it != itEnd; ++it)
	 {
		 gtParagraphStyle *pStyle = dynamic_cast<gtParagraphStyle*> (it.value());
		 if (pStyle && pStyle->isDefaultStyle())
		 {
			 defStyle = pStyle;
			 break;
		 }
	 }
	 return defStyle;
 }
 
 gtStyle* StyleReader::getStyle(const QString& name)
 {
 	if (styles.contains(name))
 	{
 		gtStyle* tmp = styles[name];
 		QString tname = tmp->getName();
 		if ((tname.indexOf(docname) == -1) && (usePrefix))
 			tmp->setName(docname + "_" + tname);
 
 		return tmp;
 	}
 	else
		return getDefaultStyle();
 }
 
 void StyleReader::setStyle(const QString& name, gtStyle* style)
 {
 	gtParagraphStyle *s;
 	QString tname = style->getName();
 	if ((style->target() == "paragraph") && (packStyles))
 	{
 		s = dynamic_cast<gtParagraphStyle*>(style);
		assert(s != NULL);
 		QString nameByAttrs = QString("%1-").arg(s->getSpaceAbove());
 		nameByAttrs += QString("%1-").arg(s->getSpaceBelow());
 		nameByAttrs += QString("%1-").arg(s->getLineSpacing());
 		nameByAttrs += QString("%1-").arg(s->getIndent());
 		nameByAttrs += QString("%1-").arg(s->getFirstLineIndent());
 		nameByAttrs += QString("%1-").arg(s->getAlignment());
 		nameByAttrs += QString("%1-").arg(s->hasDropCap());
 		nameByAttrs += QString("%1-").arg(s->getFont()->getColor());
 		nameByAttrs += QString("%1-").arg(s->getFont()->getStrokeColor());
// TODO is this important ??
/* 		QValueList<double>* tmp = s->getTabValues();
 		for (uint i = 0; i < tmp->count(); ++i)
 		{
 			double td = (*tmp)[i];
 			nameByAttrs += QString("%1-").arg(td);
 		} */
 		if (attrsStyles.contains(nameByAttrs))
 		{
 			tname = attrsStyles[nameByAttrs]->getName();
 			++pstyleCounts[nameByAttrs];
 			style->setName(tname);
 		}
 		else
 		{
 			attrsStyles[nameByAttrs] = style;
 			pstyleCounts[nameByAttrs] = 1;
 			tname = style->getName();
 		}
 	}
 	else if (!packStyles)
 	{
 		attrsStyles[name] = style;
 		pstyleCounts[name] = 1;
 		tname = style->getName();
 	}
 	if (!styles.contains(name))
 	{
 		if ((tname.indexOf(docname) == -1) && (usePrefix))
 			style->setName(docname + "_" + tname);
 		styles[name] = style;
 	}
 }
 
 QString StyleReader::getFont(const QString& key)
 {
 	if (fonts.contains(key))
 		return fonts[key];
 	else
 		return key;
 }
 
 void StyleReader::setupFrameStyle()
 {
 	QString fstyleName = "";
 	int count = 0;
 	CounterMap::Iterator it;
 	for (it = pstyleCounts.begin(); it != pstyleCounts.end(); ++it)
 	{
 		if (it.value() > count)
 		{
 			count = it.value();
 			fstyleName = it.key();
 		}
 	}
 	gtFrameStyle* fstyle;
 	gtParagraphStyle* pstyle = dynamic_cast<gtParagraphStyle*>(attrsStyles[fstyleName]);
 	fstyle = new gtFrameStyle(*pstyle);
 
 	if (!importTextOnly)
 		writer->setFrameStyle(fstyle);
 	delete fstyle;
 }
 
 bool StyleReader::updateStyle(gtStyle* style, gtStyle* parent2Style, const QString& key, const QString& value)
 {
 	gtParagraphStyle* pstyle = NULL;
 	if (style->target() == "paragraph")
 		pstyle = dynamic_cast<gtParagraphStyle*>(style);
 	else
 		pstyle = NULL;
 	QString align = NULL;
 	QString force = NULL;
 
 	if (key == "style:font-name")
 		style->getFont()->setName(getFont(value));
 	else if (key == "fo:font-size")
 	{
 		double size = 0;
 		double psize = 0;
 		if (parent2Style != NULL)
 			psize = static_cast<double>(parent2Style->getFont()->getSize());
 		else if (styles.contains("default-style"))
 			psize = static_cast<double>(styles["default-style"]->getFont()->getSize());
 			psize = psize / 10;
 		size = getSize(value, psize);
 		int nsize = static_cast<int>(size * 10);
 		style->getFont()->setSize(nsize);
 		if (pstyle)
 			pstyle->setLineSpacing(writer->getPreferredLineSpacing(nsize));
 	}
 	else if ((key == "fo:line-height") && (parent2Style != NULL))
 	{
 		gtParagraphStyle* ppstyle;
 		if (parent2Style->target() == "paragraph")
 		{
 			ppstyle = dynamic_cast<gtParagraphStyle*>(parent2Style);
			assert(ppstyle != NULL);
 			ppstyle->setLineSpacing(getSize(value, writer->getPreferredLineSpacing(style->getFont()->getSize())));
 		}
 	}
 	else if (key == "fo:color")
		style->getFont()->setColor(value);
	else if ((key == "style:use-window-font-color") && (value == "true"))
 			style->getFont()->setColor("Black");
 	else if ((key == "fo:font-weight") && (value == "bold"))
 		style->getFont()->setWeight(BOLD);
 	else if ((key == "fo:font-style") && (value == "italic"))
 		style->getFont()->setSlant(ITALIC);
 	else if ((key == "style:text-underline") && (value != "none"))
 		style->getFont()->toggleEffect(UNDERLINE);
 	else if ((key == "style:text-crossing-out") && (value != "none"))
 		style->getFont()->toggleEffect(STRIKETHROUGH);
 	else if ((key == "fo:font-variant") && (value == "small-caps"))
 		style->getFont()->toggleEffect(SMALL_CAPS);
 	else if ((key == "style:text-outline") && (value == "true"))
 	{
 		style->getFont()->toggleEffect(OUTLINE);
 		style->getFont()->setStrokeColor("Black");
 		style->getFont()->setColor("White");
 	}
 	else if (key == "fo:letter-spacing")
 		style->getFont()->setKerning(static_cast<int>(getSize(value, -1.0)));
 	else if (key == "style:text-scale")
 		style->getFont()->setHscale(static_cast<int>(getSize(value, -1.0)));
 	else if ((key == "style:text-position") && 
 	        (((value).indexOf("sub") != -1) || 
 			(((value).left(1) == "-") && ((value).left(1) != "0"))))
 		style->getFont()->toggleEffect(SUBSCRIPT);
 	else if ((key == "style:text-position") && 
 	        (((value).indexOf("super") != -1) || 
 			(((value).left(1) != "-") && ((value).left(1) != "0"))))
 		style->getFont()->toggleEffect(SUPERSCRIPT);
 	else if ((key == "fo:margin-top") && (pstyle != NULL))
 		pstyle->setSpaceAbove(getSize(value));
 	else if ((key == "fo:margin-bottom") && (pstyle != NULL))
 		pstyle->setSpaceBelow(getSize(value));
 	else if ((key == "fo:margin-left") && (pstyle != NULL))
 	{
 		if (inList)
 			pstyle->setIndent(pstyle->getIndent() + getSize(value));
 		else
 			pstyle->setIndent(getSize(value));	
 	}
 	else if ((key == "text:space-before") && (pstyle != NULL))
 	{
 		if (inList)
 			pstyle->setIndent(pstyle->getIndent() + getSize(value));
 		else
 			pstyle->setIndent(getSize(value));	
 	}
 	else if ((key == "fo:text-indent") && (pstyle != NULL))
 		pstyle->setFirstLineIndent(getSize(value));
 	else if ((key == "fo:text-align") && (pstyle != NULL))
 		align = value;
 	else if ((key == "style:justify-single-word") && (pstyle != NULL))
 		force = value;
 
 	if (!align.isNull())
 	{
		assert(pstyle);
 		if (align == "end")
 			pstyle->setAlignment(RIGHT);
 		else if (align == "center")
 			pstyle->setAlignment(CENTER);
 		else if (align == "justify")
 		{
 			if (force != "false")
 				pstyle->setAlignment(FORCED);
 			else
 				pstyle->setAlignment(BLOCK);
 		}
 	}
 	
 	return true;
 }
 
 double StyleReader::getSize(QString s, double parentSize)
 {
 	QString dbl = "0.0";
 	QString lowerValue = s.toLower();
 	double ret = 0.0;
 	if (lowerValue.indexOf("pt") != -1)
 	{
 		dbl = lowerValue.remove("pt");
 		ret = gtMeasure::d2d(dbl.toDouble(), SC_PT);
 	}
 	else if (lowerValue.indexOf("mm") != -1)
 	{
 		dbl = lowerValue.remove("mm");
 		ret = gtMeasure::d2d(dbl.toDouble(), SC_MM);
 	}
 	else if (lowerValue.indexOf("cm") != -1)
 	{
 		dbl = lowerValue.remove("cm");
 		ret = gtMeasure::d2d(dbl.toDouble() * 10, SC_MM);
 	}
 	else if (lowerValue.indexOf("in") != -1)
 	{
 		dbl = lowerValue.remove("inch");
 		dbl = lowerValue.remove("in");
 		ret = gtMeasure::d2d(dbl.toDouble(), SC_IN);
 	}
 	else if (lowerValue.indexOf("pi") != -1)
 	{
 		dbl = lowerValue.remove("pica");
 		dbl = lowerValue.remove("pi");
 		ret = gtMeasure::d2d(dbl.toDouble(), SC_P);
 	}
 	else if (lowerValue.indexOf("c") != -1)
 	{
 		dbl = lowerValue.remove("cicero");
 		dbl = lowerValue.remove("c");
 		ret = gtMeasure::d2d(dbl.toDouble(), SC_C);
 	}
 	else if (lowerValue.indexOf("%") != -1)
 	{
 		dbl = lowerValue.remove("%");
 		double factor = dbl.toDouble();
 		if (parentSize != -1.0)
 		{
 			factor = factor / 100;
 			ret = factor * parentSize;
 		}
 		else
 			ret = factor;
 	}
 	return ret;
 }
 
 StyleReader::~StyleReader()
 {
 	sreader = NULL;
 	StyleMap::Iterator it;
 	for (it = styles.begin(); it != styles.end(); ++it)
 	{
 		if (it.value())
 		{
 			delete it.value();
 			it.value() = NULL;
 		}
 	}
 }
 
 xmlSAXHandler sSAXHandlerStruct = {
 	NULL, // internalSubset,
 	NULL, // isStandalone,
 	NULL, // hasInternalSubset,
 	NULL, // hasExternalSubset,
 	NULL, // resolveEntity,
 	NULL, // getEntity,
 	NULL, // entityDecl,
 	NULL, // notationDecl,
 	NULL, // attributeDecl,
 	NULL, // elementDecl,
 	NULL, // unparsedEntityDecl,
 	NULL, // setDocumentLocator,
 	NULL, // startDocument,
 	NULL, // endDocument,
 	StyleReader::startElement,
 	StyleReader::endElement,
 	NULL, // reference,
 	NULL, // characters
 	NULL, // ignorableWhitespace,
 	NULL, // processingInstruction,
 	NULL, // comment,
 	NULL, // warning,
 	NULL, // error,
 	NULL, // fatalError,
 	NULL, // getParameterEntity,
 	NULL, // cdata,
 	NULL,
 	1
 #ifdef HAVE_XML26
 	,
 	NULL,
 	NULL,
 	NULL,
 	NULL
 #endif
 };
 
 xmlSAXHandlerPtr sSAXHandler = &sSAXHandlerStruct;
 
 void StyleReader::startElement(void*, const xmlChar * fullname, const xmlChar ** atts)
 {
 	QString* name = new QString((const char*) fullname);
 	name = new QString(name->toLower());
 	QXmlAttributes* attrs = new QXmlAttributes();
 	if (atts)
 	{
 		for(const xmlChar** cur = atts; cur && *cur; cur += 2)
 			attrs->append(QString((char*)*cur), NULL, QString((char*)*cur), QString((char*)*(cur + 1)));
 	}
 	sreader->startElement(NULL, NULL, *name, *attrs);
 }
 
 void StyleReader::endElement(void*, const xmlChar * name)
 {
 	QString *nname = new QString((const char*) name);
 	nname = new QString(nname->toLower());
 	sreader->endElement(NULL, NULL, *nname);
 }
 
