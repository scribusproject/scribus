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

ContentReader* ContentReader::m_creader = NULL;

extern xmlSAXHandlerPtr cSAXHandler;

ContentReader::ContentReader(QString documentName, StyleReader *s, gtWriter *w, bool textOnly)
{
	m_creader = this;
	m_docname = documentName;
	m_sreader = s;
	m_writer  = w;
	m_importTextOnly = textOnly;
	m_defaultStyle = NULL;
	m_currentStyle = NULL;
	m_inList = false;
	m_inNote = false;
	m_inNoteBody = false;
	m_inSpan = false;
	m_append    = 0;
	m_listIndex = 0;
	m_listLevel = 0;
	m_currentList = "";
	m_inT = false;
	m_tName = "";
}

bool ContentReader::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs) 
{
	if ((name == "text:p") || (name == "text:h"))
	{
		++m_append;
		QString name = "";
		for (int i = 0; i < attrs.count(); ++i)
		{
			if (attrs.localName(i) == "text:style-name")
			{
				name = attrs.value(i);
				m_styleNames.push_back(attrs.value(i));
			}
		}
		if (!m_inList)
		{
			m_pstyle = m_sreader->getStyle(name);
			m_currentStyle = m_pstyle;
		}
		else
		{
			gtStyle *tmp = m_sreader->getStyle(getName());
			if ((tmp->getName()).indexOf("default-style") != -1)
				getStyle();
			else
				m_currentStyle = tmp;
		}
	}
	else if (name == "text:span")
	{
		m_inSpan = true;
		QString styleName = "";
		for (int i = 0; i < attrs.count(); ++i)
		{
			if (attrs.localName(i) == "text:style-name")
			{
				m_currentStyle = m_sreader->getStyle(attrs.value(i));
				styleName = attrs.value(i);
				m_styleNames.push_back(styleName);
			}
		}
		gtStyle *tmp = m_sreader->getStyle(getName());
		if ((tmp->getName()).indexOf("default-style") != -1)
			getStyle();
		else
			m_currentStyle = tmp;
	}
	else if ((name == "text:unordered-list") || (name == "text:ordered-list"))
	{
		m_inList = true;
		++m_listLevel;
		if (static_cast<int>(m_listIndex2.size()) < m_listLevel)
			m_listIndex2.push_back(0);
		for (int i = 0; i < attrs.count(); ++i)
		{
			if (attrs.localName(i) == "text:style-name")
				m_currentList = attrs.value(i);
		}
		m_currentStyle = m_sreader->getStyle(QString(m_currentList + "_%1").arg(m_listLevel));
		m_styleNames.clear();
		m_styleNames.push_back(QString(m_currentList + "_%1").arg(m_listLevel));
		if (name == "text:ordered-list")
			m_isOrdered2.push_back(true);
		else
			m_isOrdered2.push_back(false);
	}
	else if (name == "text:list-item")
	{
		bool isOrdered = false;
		int levelIndex = m_listLevel - 1;
		if (levelIndex >= 0 && levelIndex < static_cast<int>(m_isOrdered2.size()))
			isOrdered = m_isOrdered2[m_listLevel - 1];
		if (isOrdered)
		{
			++m_listIndex;
			++m_listIndex2[m_listLevel - 1];
			if (m_listLevel == 1)
				write(QString("%1. ").arg(m_listIndex2[m_listLevel - 1]));
			else
				write(QString("%1. ").arg(m_listIndex2[m_listLevel - 1]));
		}
		else
			write("- ");
	}
	else if (name == "text:note")
		m_inNote = true;
	else if (name == "text:note-body")
		m_inNoteBody = true;
	else if (name == "style:style")
	{
		QString sname = "";
		bool isTextStyle = false;
		for (int i = 0; i < attrs.count(); ++i)
		{
			if (attrs.localName(i) == "style:name")
				sname = attrs.value(i);
			else if ((attrs.localName(i) == "style:family") && (attrs.value(i) == "text"))
				isTextStyle = true;
		}
		if (isTextStyle)
		{
			m_tName = sname;
			m_inT = true;
		}
	}
	else if ((name == "style:properties") && (m_inT))
	{
		Properties p;
		for (int i = 0; i < attrs.count(); ++i)
		{
			std::pair<QString, QString> pair(attrs.localName(i), attrs.value(i));
			p.push_back(pair);
		}
		m_tmap[m_tName] = p;
	}
	else if (name == "text:s")
	{
		int count = 1;
		for (int i = 0; i < attrs.count(); ++i)
		{
			if (attrs.localName(i) == "text:c")
			{
				bool ok = false;
				int tmpcount = (attrs.value(i)).toInt(&ok);
				if (ok)
					count = tmpcount;
			}
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
	if (m_append > 0)
		write(tmp);
	return true;
}

bool ContentReader::endElement(const QString&, const QString&, const QString &name)
{
	if ((name == "text:p") || (name == "text:h"))
	{
		write("\n");
		--m_append;
		if (m_inList || m_inNote || m_inNoteBody)
		{
			if (m_styleNames.size() != 0)
				m_styleNames.pop_back();
		}
		else
			m_styleNames.clear();
	}
	else if (name == "text:span")
	{
		m_inSpan = false;
		m_currentStyle = m_pstyle;
		if (m_styleNames.size() != 0)
			m_styleNames.pop_back();	
		m_currentStyle = m_sreader->getStyle(getName());
	}
	else if (name == "text:note")
		m_inNote = false;
	else if (name == "text:note-body")
		m_inNoteBody = false;
	else if (name == "text:line-break")
		write(SpecialChars::LINEBREAK);
	else if (name == "text:tab-stop")
		write("\t");
	else if ((name == "text:unordered-list") || (name == "text:ordered-list"))
	{
		--m_listLevel;
		m_styleNames.clear();
		if (m_listLevel == 0)
		{
			m_inList = false;
			m_listIndex2.clear();
		}
		else
		{
			m_currentStyle = m_sreader->getStyle(QString(m_currentList + "_%1").arg(m_listLevel));
			m_styleNames.push_back(QString(m_currentList + "_%1").arg(m_listLevel));
		}
	}
	else if ((name == "style:style") && (m_inT))
	{
		m_inT = false;
		m_tName = "";
	}
	return true;
}

void ContentReader::write(const QString& text)
{
	if (!m_inNote && !m_inNoteBody) // Disable notes import for now
	{
		if (m_importTextOnly)
			m_writer->appendUnstyled(text);
		else if (m_inSpan)
			m_writer->append(text, m_currentStyle, false);
		else
			m_writer->append(text, m_currentStyle);
	}
	m_lastStyle = m_currentStyle;
}

void ContentReader::parse(QString fileName)
{
	m_sreader->parse(fileName);
#if defined(_WIN32)
	QString fname = QDir::toNativeSeparators(fileName);
	QByteArray fn = (QSysInfo::WindowsVersion & QSysInfo::WV_NT_based) ? fname.toUtf8() : fname.toLocal8Bit();
#else
	QByteArray fn(fileName.toLocal8Bit());
#endif
	xmlSAXParseFile(cSAXHandler, fn.data(), 1);
}

xmlSAXHandler cSAXHandlerStruct = {
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
	ContentReader::startElement,
	ContentReader::endElement,
	NULL, // reference,
	ContentReader::characters,
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

xmlSAXHandlerPtr cSAXHandler = &cSAXHandlerStruct;

void ContentReader::startElement(void*, const xmlChar *fullname, const xmlChar ** atts)
{
	QString name(QString((const char*) fullname).toLower());
	QXmlAttributes attrs;
	if (atts)
	{
		for(const xmlChar** cur = atts; cur && *cur; cur += 2)
			attrs.append(QString((char*)*cur), NULL, QString((char*)*cur), QString((char*)*(cur + 1)));
	}
	m_creader->startElement(NULL, NULL, name, attrs);
}

void ContentReader::characters(void*, const xmlChar *ch, int len)
{
	QString chars = QString::fromUtf8((const char*) ch, len);
	m_creader->characters(chars);
}

void ContentReader::endElement(void*, const xmlChar *name)
{
	QString nname(QString((const char*) name).toLower());
	m_creader->endElement(NULL, NULL, nname);
}

QString ContentReader::getName()
{
	QString s = "";
	for (uint i = 0; i < m_styleNames.size(); ++i)
		s += m_styleNames[i];
	return s;
}

void ContentReader::getStyle()
{
	gtStyle *style = NULL, *tmp = NULL;
	if (m_styleNames.size() == 0)
		style = m_sreader->getStyle("default-style");
	else
		style = m_sreader->getStyle(m_styleNames[0]);
	assert (style != NULL);
	gtParagraphStyle* par = dynamic_cast<gtParagraphStyle*>(style);
	if (par)
		tmp = new gtParagraphStyle(*par);
	else
		tmp = new gtStyle(*style);
	for (uint i = 1; i < m_styleNames.size(); ++i)
	{
		Properties& p = m_tmap[m_styleNames[i]];
		for (uint j = 0; j < p.size(); ++j)
			m_sreader->updateStyle(tmp, m_sreader->getStyle(m_styleNames[i - 1]), p[j].first, p[j].second);
	}
	m_currentStyle = tmp;
	m_sreader->setStyle(getName(), tmp);
}

ContentReader::~ContentReader()
{
	m_creader = NULL;
	delete m_defaultStyle;
}

#endif // HAVE_XML
