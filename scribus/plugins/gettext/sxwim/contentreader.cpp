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

#include "contentreader.h"
#include "gtparagraphstyle.h"

#ifdef HAVE_XML

#include "scribusstructs.h"
#include <QByteArray>

ContentReader* ContentReader::creader = nullptr;

extern xmlSAXHandlerPtr cSAXHandler;

ContentReader::ContentReader(const QString& documentName, StyleReader *s, gtWriter *w, bool textOnly)
{
	creader = this;
	docname = documentName;
	sreader = s;
	writer  = w;
	importTextOnly = textOnly;
}

bool ContentReader::startElement(const QString &name, const SXWAttributesMap &attrs) 
{
	if ((name == "text:p") || (name == "text:h"))
	{
		++append;
		QString name = attrs.value("text:style-name");
		if (!name.isEmpty())
			styleNames.push_back(name);
		if (!inList)
		{
			pstyle = sreader->getStyle(name);
			currentStyle = pstyle;
		}
		else
		{
			gtStyle *tmp = sreader->getStyle(getName());
			if ((tmp->getName()).indexOf("default-style") != -1)
				getStyle();
			else
				currentStyle = tmp;
		}
	}
	else if (name == "text:span")
	{
		inSpan = true;
		QString styleName = attrs.value("text:style-name");
		if (!styleName.isEmpty())
		{
			currentStyle = sreader->getStyle(styleName);
			styleNames.push_back(styleName);
		}
		gtStyle *tmp = sreader->getStyle(getName());
		if ((tmp->getName()).indexOf("default-style") != -1)
			getStyle();
		else
			currentStyle = tmp;
	}
	else if ((name == "text:unordered-list") || (name == "text:ordered-list"))
	{
		inList = true;
		++listLevel;
		if (static_cast<int>(listIndex2.size()) < listLevel)
			listIndex2.push_back(0);
		QString styleName = attrs.value("text:style-name");
		if (!styleName.isEmpty())
			currentList = styleName;
		currentStyle = sreader->getStyle(QString(currentList + "_%1").arg(listLevel));
		styleNames.clear();
		styleNames.push_back(QString(currentList + "_%1").arg(listLevel));
		if (name == "text:ordered-list")
			isOrdered2.push_back(true);
		else
			isOrdered2.push_back(false);
	}
	else if (name == "text:list-item")
	{
		bool isOrdered = false;
		int levelIndex = listLevel - 1;
		if (levelIndex >= 0 && levelIndex < static_cast<int>(isOrdered2.size()))
			isOrdered = isOrdered2[listLevel - 1];
		if (isOrdered)
		{
			++listIndex;
			++listIndex2[listLevel - 1];
			if (listLevel == 1)
				write(QString("%1. ").arg(listIndex2[listLevel - 1]));
			else
				write(QString("%1. ").arg(listIndex2[listLevel - 1]));
		}
		else
			write("- ");
	}
	else if (name == "text:note")
		inNote = true;
	else if (name == "text:note-body")
		inNoteBody = true;
	else if (name == "style:style")
	{
		QString sname = "";
		bool isTextStyle = false;
		for (auto attr = attrs.cbegin(); attr != attrs.cend(); ++attr)
		{
			QString attrName = attr.key();
			if (attrName == "style:name")
				sname = attr.value();
			else if ((attrName == "style:family") && (attr.value() == "text"))
				isTextStyle = true;
		}
		if (isTextStyle)
		{
			tName = sname;
			inT = true;
		}
	}
	else if ((name == "style:properties") && (inT))
	{
		Properties p;
		for (auto attr = attrs.cbegin(); attr != attrs.cend(); ++attr)
		{
			std::pair<QString, QString> pair(attr.key(), attr.value());
			p.push_back(pair);
		}
		tmap[tName] = p;
	}
	else if (name == "text:s")
	{
		int count = 1;
		QString textC = attrs.value("text:c");
		if (!textC.isEmpty())
		{
			bool ok = false;
			int tmpcount = textC.toInt(&ok);
			if (ok)
				count = tmpcount;
		}
		for (int i = 0; i < count; ++i)
			write(" ");
	}
	return true;
}

bool ContentReader::characters(const QString &ch) 
{
	QString tmp = ch;
	tmp = tmp.remove("\n");
	tmp = tmp.remove(""); // Remove all OO.o hyphenation chars
	// Unneeded as scribus now also use standard unicode non-breakable space
	// tmp = tmp.replace(QChar(160), SpecialChars::NBSPACE); // replace OO.o nbsp with Scribus nbsp
	if (append > 0)
		write(tmp);
	return true;
}

bool ContentReader::endElement(const QString &name)
{
	if ((name == "text:p") || (name == "text:h"))
	{
		write("\n");
		--append;
		if (inList || inNote || inNoteBody)
		{
			if (!styleNames.empty())
				styleNames.pop_back();
		}
		else
			styleNames.clear();
	}
	else if (name == "text:span")
	{
		inSpan = false;
		currentStyle = pstyle;
		if (!styleNames.empty())
			styleNames.pop_back();	
		currentStyle = sreader->getStyle(getName());
	}
	else if (name == "text:note")
		inNote = false;
	else if (name == "text:note-body")
		inNoteBody = false;
	else if (name == "text:line-break")
		write(SpecialChars::LINEBREAK);
	else if (name == "text:tab-stop")
		write("\t");
	else if ((name == "text:unordered-list") || (name == "text:ordered-list"))
	{
		--listLevel;
		styleNames.clear();
		if (listLevel == 0)
		{
			inList = false;
			listIndex2.clear();
		}
		else
		{
			currentStyle = sreader->getStyle(QString(currentList + "_%1").arg(listLevel));
			styleNames.push_back(QString(currentList + "_%1").arg(listLevel));
		}
	}
	else if ((name == "style:style") && (inT))
	{
		inT = false;
		tName = "";
	}
	return true;
}

void ContentReader::write(const QString& text)
{
	if (!inNote && !inNoteBody) // Disable notes import for now
	{
		if (importTextOnly)
			writer->appendUnstyled(text);
		else if (inSpan)
			writer->append(text, currentStyle, false);
		else
			writer->append(text, currentStyle);
	}
	lastStyle = currentStyle;
}

void ContentReader::parse(const QString& fileName)
{
	sreader->parse(fileName);
#if defined(_WIN32)
	QString fname = QDir::toNativeSeparators(fileName);
	QByteArray fn = (QSysInfo::WindowsVersion & QSysInfo::WV_NT_based) ? fname.toUtf8() : fname.toLocal8Bit();
#else
	QByteArray fn(fileName.toLocal8Bit());
#endif
	xmlSAXParseFile(cSAXHandler, fn.data(), 1);
}

xmlSAXHandler cSAXHandlerStruct = {
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
	ContentReader::startElement,
	ContentReader::endElement,
	nullptr, // reference,
	ContentReader::characters,
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

xmlSAXHandlerPtr cSAXHandler = &cSAXHandlerStruct;

void ContentReader::startElement(void*, const xmlChar *fullname, const xmlChar ** atts)
{
	QString name(QString((const char*) fullname).toLower());
	SXWAttributesMap attrs;
	for (const xmlChar** cur = atts; cur && *cur; cur += 2)
	{
		QString attrName((char*)*cur);
		QString attrValue((char*)*(cur + 1));
		attrs[attrName] = attrValue;
	}
	creader->startElement(name, attrs);
}

void ContentReader::characters(void*, const xmlChar *ch, int len)
{
	QString chars = QString::fromUtf8((const char*) ch, len);
	creader->characters(chars);
}

void ContentReader::endElement(void*, const xmlChar *name)
{
	QString nname(QString((const char*) name).toLower());
	creader->endElement(nname);
}

QString ContentReader::getName()
{
	QString s = "";
	for (uint i = 0; i < styleNames.size(); ++i)
		s += styleNames[i];
	return s;
}

void ContentReader::getStyle()
{
	gtStyle *style = nullptr;
	gtStyle *tmp = nullptr;
	if (styleNames.empty())
		style = sreader->getStyle("default-style");
	else
		style = sreader->getStyle(styleNames[0]);
	assert (style != nullptr);
	gtParagraphStyle* par = dynamic_cast<gtParagraphStyle*>(style);
	if (par)
		tmp = new gtParagraphStyle(*par);
	else
		tmp = new gtStyle(*style);
	for (uint i = 1; i < styleNames.size(); ++i)
	{
		Properties& p = tmap[styleNames[i]];
		for (uint j = 0; j < p.size(); ++j)
			sreader->updateStyle(tmp, sreader->getStyle(styleNames[i - 1]), p[j].first, p[j].second);
	}
	currentStyle = tmp;
	sreader->setStyle(getName(), tmp);
}

ContentReader::~ContentReader()
{
	creader = nullptr;
	delete defaultStyle;
}

#endif // HAVE_XML
