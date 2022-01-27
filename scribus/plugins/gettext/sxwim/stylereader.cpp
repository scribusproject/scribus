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
  *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
  ***************************************************************************/

#include "stylereader.h"

#include <QByteArray>

#include <scribusstructs.h>
#include <gtmeasure.h>
#include <gtparagraphstyle.h>
#include <gtframestyle.h>
#include <gtfont.h>

StyleReader* StyleReader::sreader = nullptr;

extern xmlSAXHandlerPtr sSAXHandler;

StyleReader::StyleReader(const QString& documentName, gtWriter *w, bool textOnly, bool prefix, bool combineStyles)
{
	sreader      = this;
	docname      = documentName;
	writer       = w;
	importTextOnly = textOnly;
	usePrefix    = prefix;
	packStyles   = combineStyles;
}

StyleReader::~StyleReader()
{
	sreader = nullptr;
	StyleMap::Iterator it;
	for (it = styles.begin(); it != styles.end(); ++it)
	{
		if (it.value())
		{
			delete it.value();
			it.value() = nullptr;
		}
	}
}

void StyleReader::startElement(void*, const xmlChar * fullname, const xmlChar ** atts)
{
	QString name = QString((const char*) fullname).toLower();
	SXWAttributesMap attrs;
	for (const xmlChar** cur = atts; cur && *cur; cur += 2)
	{
		QString attrName((char*)*cur);
		QString attrValue((char*)*(cur + 1));
		attrs[attrName] = attrValue;
	}
	sreader->startElement(name, attrs);
}

bool StyleReader::startElement(const QString &name, const SXWAttributesMap &attrs)
{
	if (name == "style:default-style")
		defaultStyle(attrs);
	else if (name == "style:properties")
		styleProperties(attrs);
	else if (name == "style:style")
	{
		/*if (!defaultStyleCreated)
		{
			gtParagraphStyle* pstyle = new gtParagraphStyle(writer->getDefaultStyle()->asGtParagraphStyle());
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
		currentList = attrs.value("style:name");
		inList = true;
	}
	else if (((name == "text:list-level-style-bullet") ||
			  (name == "text:list-level-style-number") ||
			  (name == "text:list-level-style-image")) && (inList))
	{
		QString textLevel = attrs.value("text:level");
		if (!textLevel.isEmpty())
		{
			gtStyle *plist;
			if (textLevel == "1")
			{
				plist = listParents[currentList];
			}
			else
			{
				int level = textLevel.toInt();
				--level;
				plist = styles[QString(currentList + "_%1").arg(level)];
			}
			gtParagraphStyle *pstyle;
			if (plist == nullptr)
				plist = new gtStyle(*(styles["default-style"]));

			if (plist->target() == "paragraph")
			{
				pstyle = dynamic_cast<gtParagraphStyle*>(plist);
				assert(pstyle != nullptr);
				gtParagraphStyle* tmp = new gtParagraphStyle(*pstyle);
				currentStyle = tmp;
			}
			else
			{
				gtParagraphStyle* tmp = new gtParagraphStyle(*plist);
				currentStyle = tmp;
			}
			currentStyle->setName(currentList + "_" + textLevel);
		}
		readProperties = true;
	}
	else if ((name == "style:drop-cap") && (readProperties))
	{
		if (currentStyle->target() == "paragraph")
		{
			QString styleLines = attrs.value( "style:lines");
			if (!styleLines.isEmpty())
			{
				bool ok = false;
				QString sd = styleLines;
				int dh = sd.toInt(&ok);
				if (ok)
				{
					gtParagraphStyle* s = dynamic_cast<gtParagraphStyle*>(currentStyle);
					assert(s != nullptr);
					s->setDropCapHeight(dh);
					s->setDropCap(true);
				}
			}
		}
	}
	else if (name == "style:font-decl")
	{
		QString key;
		QString family;
		QString style;
		for (auto attr = attrs.cbegin(); attr != attrs.cend(); ++attr)
		{
			QString attrName = attr.key();
			if (attrName == "style:name")
				key = attr.value();
			else if (attrName == "fo:font-family")
			{
				family = attr.value();
				family = family.remove("'");
			}
			else if (attrName == "style:font-style-name")
				style += attr.value() + " ";
		}
		QString name = family + " " + style;
		name = name.simplified();
		fonts[key] = name;
	}
	return true;
}

void StyleReader::defaultStyle(const SXWAttributesMap& attrs)
{
	currentStyle = nullptr;
	QString styleFamily = attrs.value("style:family");
	if (styleFamily == "paragraph")
	{
		gtParagraphStyle* pstyle = new gtParagraphStyle(writer->getDefaultStyle()->asGtParagraphStyle());
		pstyle->setDefaultStyle(true);
		currentStyle = dynamic_cast<gtStyle*>(pstyle);
		currentStyle->setName("default-style");
		readProperties = true;
		defaultStyleCreated = true;
	}
}

void StyleReader::styleProperties(const SXWAttributesMap& attrs)
{
	if ((currentStyle == nullptr) || (!readProperties))
		return;

	gtParagraphStyle* pstyle = nullptr;
	if (currentStyle->target() == "paragraph")
		pstyle = dynamic_cast<gtParagraphStyle*>(currentStyle);
	else
		pstyle = nullptr;

	QString align;
	QString force;
	bool hasColorTag = false;
	for (auto attr = attrs.begin(); attr != attrs.end(); ++attr)
	{
		QString attrName = attr.key();
		QString attrValue = attr.value();
		if ((attrName == "style:font-name") && (!inList))
			currentStyle->getFont()->setName(getFont(attrValue));
		else if (attrName == "fo:font-size")
		{
			double size = 0.0;
			double psize = 0.0;
			if (parentStyle != nullptr)
				psize = static_cast<double>(parentStyle->getFont()->getSize());
			else if (styles.contains("default-style"))
				psize = static_cast<double>(styles["default-style"]->getFont()->getSize());

			psize = psize / 10;
			size = getSize(attrValue, psize);
			int nsize = static_cast<int>(size * 10);
			currentStyle->getFont()->setSize(nsize);
			if (pstyle)
				pstyle->setLineSpacing(writer->getPreferredLineSpacing(nsize));
		}
		else if ((attrName == "fo:line-height") && (parentStyle != nullptr))
		{
			gtParagraphStyle* ppstyle;
			if (parentStyle->target() == "paragraph")
			{
				ppstyle = dynamic_cast<gtParagraphStyle*>(parentStyle);
				assert(ppstyle != nullptr);
				ppstyle->setLineSpacing(getSize(attrValue, writer->getPreferredLineSpacing(currentStyle->getFont()->getSize())));
			}
		}
		else if (attrName == "fo:color")
		{
			currentStyle->getFont()->setColor(attrValue);
			hasColorTag = true;
		}
		else if ((attrName == "style:use-window-font-color") && (attrValue == "true"))
		{
			currentStyle->getFont()->setColor("Black");
			hasColorTag = true;
		}
		else if ((attrName == "fo:font-weight") && (attrValue == "bold"))
			currentStyle->getFont()->setWeight(BOLD);
		else if ((attrName == "fo:font-style") && (attrValue == "italic"))
			currentStyle->getFont()->setSlant(ITALIC);
		else if ((attrName == "style:text-underline") && (attrValue != "none"))
			currentStyle->getFont()->toggleEffect(UNDERLINE);
		else if ((attrName == "style:text-crossing-out") && (attrValue != "none"))
			currentStyle->getFont()->toggleEffect(STRIKETHROUGH);
		else if ((attrName == "fo:font-variant") && (attrValue == "small-caps"))
			currentStyle->getFont()->toggleEffect(SMALL_CAPS);
		else if ((attrName == "style:text-outline") && (attrValue == "true"))
		{
			currentStyle->getFont()->toggleEffect(OUTLINE);
			currentStyle->getFont()->setStrokeColor("Black");
			currentStyle->getFont()->setColor("White");
		}
		else if (attrName == "fo:letter-spacing")
			currentStyle->getFont()->setKerning(static_cast<int>(getSize(attrValue, -1.0)));
		else if (attrName == "style:text-scale")
			currentStyle->getFont()->setHscale(static_cast<int>(getSize(attrValue, -1.0)));
		else if ((attrName == "style:text-position") &&
				 (((attrValue).indexOf("sub") != -1) ||
				  (((attrValue).at(0) == "-") && ((attrValue).at(0) != "0"))))
			currentStyle->getFont()->toggleEffect(SUBSCRIPT);
		else if ((attrName == "style:text-position") &&
				 (((attrValue).indexOf("super") != -1) ||
				  (((attrValue).at(0) != "-") && ((attrValue).at(0) != "0"))))
			currentStyle->getFont()->toggleEffect(SUPERSCRIPT);
		else if ((attrName == "fo:margin-top") && (pstyle != nullptr))
			pstyle->setSpaceAbove(getSize(attrValue));
		else if ((attrName == "fo:margin-bottom") && (pstyle != nullptr))
			pstyle->setSpaceBelow(getSize(attrValue));
		else if ((attrName == "fo:margin-left") && (pstyle != nullptr))
		{
			if (inList)
				pstyle->setIndent(pstyle->getIndent() + getSize(attrValue));
			else
				pstyle->setIndent(getSize(attrValue));
		}
		else if ((attrName == "text:space-before") && (pstyle != nullptr))
		{
			if (inList)
				pstyle->setIndent(pstyle->getIndent() + getSize(attrValue));
			else
				pstyle->setIndent(getSize(attrValue));
		}
		else if ((attrName == "fo:text-indent") && (pstyle != nullptr))
			pstyle->setFirstLineIndent(getSize(attrValue));
		else if ((attrName == "fo:text-align") && (pstyle != nullptr))
			align = attrValue;
		else if ((attrName == "style:justify-single-word") && (pstyle != nullptr))
			force = attrValue;
	}
	if (!align.isEmpty() && (pstyle != nullptr))
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

void StyleReader::styleStyle(const SXWAttributesMap& attrs)
{
	QString name;
	QString listName;
	bool setDefaultStyle = false;
	bool isParaStyle = false;
	bool create = true;

	if (!defaultStyleCreated)
	{
		gtParagraphStyle* pstyle = new gtParagraphStyle(writer->getDefaultStyle()->asGtParagraphStyle());
		pstyle->setDefaultStyle(true);
		currentStyle = dynamic_cast<gtStyle*>(pstyle);
		currentStyle->setName("default-style");
		setDefaultStyle     = true;
		defaultStyleCreated = true;
		parentStyle  = currentStyle;
	}

	for (auto attr = attrs.begin(); attr != attrs.end(); ++attr)
	{
		QString attrName = attr.key();
		QString attrValue = attr.value();
		if (attrName == "style:family")
		{
			if (attrValue == "paragraph")
			{
				isParaStyle = true;
				readProperties = true;
			}
			else if (attrValue == "text")
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
		else if (attrName == "style:name")
			name = attrValue;
		else if (attrName == "style:parent-style-name")
		{
			if (styles.contains(attrValue))
				parentStyle = styles[attrValue];
			else
				parentStyle = nullptr;
		}
		else if (attrName == "style:list-style-name")
			listName = attrValue;
	}
	if ((parentStyle == nullptr) && (styles.contains("default-style")))
		parentStyle = styles["default-style"];

	if (create)
	{
		if (parentStyle == nullptr)
		{
			parentStyle = new gtStyle("tmp-parent");
		}
		if (isParaStyle)
		{
			gtParagraphStyle *tmpP;
			if (parentStyle->target() == "paragraph")
			{
				tmpP = dynamic_cast<gtParagraphStyle*>(parentStyle);
				assert(tmpP != nullptr);
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
			if (!listName.isEmpty())
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
		currentStyle = nullptr;
}

void StyleReader::tabStop(const SXWAttributesMap& attrs)
{
	if (currentStyle->target() == "paragraph")
	{
		gtParagraphStyle* pstyle = dynamic_cast<gtParagraphStyle*>(currentStyle);
		assert(pstyle != nullptr);
		QString pos = attrs.value("style:position");
		QString type = attrs.value("style:type");
		if (!pos.isEmpty())
		{
			if (!type.isEmpty())
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

void StyleReader::endElement(void*, const xmlChar * name)
{
	QString nname = QString((const char*) name).toLower();
	sreader->endElement(nname);
}

bool StyleReader::endElement(const QString &name)
{
	if ((name == "style:default-style") && (currentStyle != nullptr) && (readProperties))
	{
		setStyle(currentStyle->getName(), currentStyle);
		currentStyle = nullptr;
		parentStyle = nullptr;
		readProperties = false;
	}
	else if (((name == "style:style") ||
			  (name == "text:list-level-style-bullet") ||
			  (name == "text:list-level-style-number") ||
			  (name == "text:list-level-style-image")) && (currentStyle != nullptr))
	{
		setStyle(currentStyle->getName(), currentStyle);
		currentStyle = nullptr;
		parentStyle = nullptr;
		readProperties = false;
	}
	else if (name == "text:list-style")
	{
		inList = false;
	}

	return true;
}

void StyleReader::parse(const QString& fileName)
{
#if defined(_WIN32)
	QString fname = QDir::toNativeSeparators(fileName);
	QByteArray fn = (QSysInfo::WindowsVersion & QSysInfo::WV_NT_based) ? fname.toUtf8() : fname.toLocal8Bit();
#else
	QByteArray fn(fileName.toLocal8Bit());
#endif
	xmlSAXParseFile(sSAXHandler, fn.data(), 1);
}

gtStyle* StyleReader::getDefaultStyle()
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
	return getDefaultStyle();
}

void StyleReader::setStyle(const QString& name, gtStyle* style)
{
	gtParagraphStyle *s;
	QString tname = style->getName();
	if ((style->target() == "paragraph") && (packStyles))
	{
		s = dynamic_cast<gtParagraphStyle*>(style);
		assert(s != nullptr);
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
	return key;
}

void StyleReader::setupFrameStyle()
{
	QString fstyleName;
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

	gtParagraphStyle* pstyle = dynamic_cast<gtParagraphStyle*>(attrsStyles[fstyleName]);
	gtFrameStyle* fstyle = new gtFrameStyle(*pstyle);

	if (!importTextOnly)
		writer->setFrameStyle(fstyle);
	delete fstyle;
}

bool StyleReader::updateStyle(gtStyle* style, gtStyle* parent2Style, const QString& key, const QString& value)
{
	gtParagraphStyle* pstyle = nullptr;
	if (style->target() == "paragraph")
		pstyle = dynamic_cast<gtParagraphStyle*>(style);
	else
		pstyle = nullptr;
	QString align;
	QString force;

	if (key == "style:font-name")
		style->getFont()->setName(getFont(value));
	else if (key == "fo:font-size")
	{
		double size = 0;
		double psize = 0;
		if (parent2Style != nullptr)
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
	else if ((key == "fo:line-height") && (parent2Style != nullptr))
	{
		gtParagraphStyle* ppstyle;
		if (parent2Style->target() == "paragraph")
		{
			ppstyle = dynamic_cast<gtParagraphStyle*>(parent2Style);
			assert(ppstyle != nullptr);
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
			 ((value.indexOf("sub") != -1) ||
			  ((value.at(0) == "-") && (value.at(0) != "0"))))
		style->getFont()->toggleEffect(SUBSCRIPT);
	else if ((key == "style:text-position") &&
			 ((value.indexOf("super") != -1) ||
			  ((value.at(0) != "-") && (value.at(0) != "0"))))
		style->getFont()->toggleEffect(SUPERSCRIPT);
	else if ((key == "fo:margin-top") && (pstyle != nullptr))
		pstyle->setSpaceAbove(getSize(value));
	else if ((key == "fo:margin-bottom") && (pstyle != nullptr))
		pstyle->setSpaceBelow(getSize(value));
	else if ((key == "fo:margin-left") && (pstyle != nullptr))
	{
		if (inList)
			pstyle->setIndent(pstyle->getIndent() + getSize(value));
		else
			pstyle->setIndent(getSize(value));
	}
	else if ((key == "text:space-before") && (pstyle != nullptr))
	{
		if (inList)
			pstyle->setIndent(pstyle->getIndent() + getSize(value));
		else
			pstyle->setIndent(getSize(value));
	}
	else if ((key == "fo:text-indent") && (pstyle != nullptr))
		pstyle->setFirstLineIndent(getSize(value));
	else if ((key == "fo:text-align") && (pstyle != nullptr))
		align = value;
	else if ((key == "style:justify-single-word") && (pstyle != nullptr))
		force = value;

	if (!align.isEmpty() && (pstyle != nullptr))
	{
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

double StyleReader::getSize(const QString& s, double parentSize)
{
	QString dbl("0.0");
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

xmlSAXHandler sSAXHandlerStruct = {
	nullptr, // internalSubset,
	nullptr, // isStandalone,
	nullptr, // hasInternalSubset,
	nullptr, // hasExternalSubset,
	nullptr, // resolveEntity,
	nullptr, // getEntity,
	nullptr, // entityDecl,
	nullptr, // notationDecl,
	nullptr, // attributeDecl,
	nullptr, // elementDecl,
	nullptr, // unparsedEntityDecl,
	nullptr, // setDocumentLocator,
	nullptr, // startDocument,
	nullptr, // endDocument,
	StyleReader::startElement,
	StyleReader::endElement,
	nullptr, // reference,
	nullptr, // characters
	nullptr, // ignorableWhitespace,
	nullptr, // processingInstruction,
	nullptr, // comment,
	nullptr, // warning,
	nullptr, // error,
	nullptr, // fatalError,
	nullptr, // getParameterEntity,
	nullptr, // cdata,
	nullptr,
	1
	#ifdef HAVE_XML26
	,
	nullptr,
	nullptr,
	nullptr,
	nullptr
	#endif
};

xmlSAXHandlerPtr sSAXHandler = &sSAXHandlerStruct;

