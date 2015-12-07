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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QObject>
#include <QByteArray>
#include "htmlreader.h"

#include "scribusstructs.h"
#include "gtmeasure.h"

HTMLReader* HTMLReader::m_hreader = NULL;
bool HTMLReader::m_elemJustStarted = false;
bool HTMLReader::m_elemJustFinished = false;

extern htmlSAXHandlerPtr mySAXHandler;

HTMLReader::HTMLReader(gtParagraphStyle *ps, gtWriter *w, bool textOnly)
{
	m_pstyle = ps;
	m_defaultColor = ps->getFont()->getColor();
	m_defaultWeight = ps->getFont()->getWeight();
	m_defaultSlant = ps->getFont()->getSlant();
	initPStyles();
	m_inH1 = false;
	m_inH2 = false;
	m_inH3 = false;
	m_inH4 = false;
	m_inH5 = false;
	m_inH6 = false;
	m_inA = false;
	m_inCode = false;
	m_inBody = false;
	m_inPre = false;
	m_inP = false;
	m_inCenter = false;
	m_writer = w;
	m_href = "";
	m_extLinks = "";
	m_extIndex = 1;
	m_listLevel = -1;
	m_inOL = false;
	m_wasInOL = false;
	m_inUL = false;
	m_wasInUL = false;
	m_inLI = false;
	m_addedLI = false;
	m_lastCharWasSpace = false;
	m_noFormatting = textOnly;
	m_hreader = this;
}

void HTMLReader::initPStyles()
{
	m_pstylec = new gtParagraphStyle(*m_pstyle);
	m_pstylec->setAlignment(CENTER);
	m_pstylec->setName("HTML_center");
	gtParagraphStyle* pstyleli = new gtParagraphStyle(*m_pstyle);
	pstyleli->setIndent(pstyleli->getIndent()+50.0);
	pstyleli->setName("HTML_li_level-0");
	m_listStyles.push_back(pstyleli);
	m_nextItemNumbers.push_back(1);
	m_pstyleh6 = new gtParagraphStyle(*m_pstyle);
	m_pstyleh6->getFont()->setSize(m_pstyle->getFont()->getSize() + 2.5);
	m_pstyleh6->getFont()->setWeight(BOLD);
	m_pstyleh6->setSpaceAbove(2.5);
	m_pstyleh6->setSpaceBelow(1.25);
	m_pstyleh6->setName("HTML_h6");
	m_pstyleh5 = new gtParagraphStyle(*m_pstyle);
	m_pstyleh5->getFont()->setSize(m_pstyle->getFont()->getSize() + 5);
	m_pstyleh5->getFont()->setWeight(BOLD);
	m_pstyleh5->setSpaceAbove(5.0);
	m_pstyleh5->setSpaceBelow(2.5);
	m_pstyleh5->setName("HTML_h5");
	m_pstyleh4 = new gtParagraphStyle(*m_pstyle);
	m_pstyleh4->getFont()->setSize(m_pstyle->getFont()->getSize() + 10);
	m_pstyleh4->getFont()->setWeight(BOLD);
	m_pstyleh4->setSpaceAbove(10.0);
	m_pstyleh4->setSpaceBelow(5.0);
	m_pstyleh4->setName("HTML_h4");
	m_pstyleh3 = new gtParagraphStyle(*m_pstyle);
	m_pstyleh3->getFont()->setSize(m_pstyle->getFont()->getSize() + 20);
	m_pstyleh3->getFont()->setWeight(BOLD);
	m_pstyleh3->setSpaceAbove(20.0);
	m_pstyleh3->setSpaceBelow(10.0);
	m_pstyleh3->setName("HTML_h3");
	m_pstyleh2 = new gtParagraphStyle(*m_pstyle);
	m_pstyleh2->getFont()->setSize(m_pstyle->getFont()->getSize() + 40);
	m_pstyleh2->getFont()->setWeight(BOLD);
	m_pstyleh2->setSpaceAbove(30.0);
	m_pstyleh2->setSpaceBelow(20.0);
	m_pstyleh2->setName("HTML_h2");
	m_pstyleh1 = new gtParagraphStyle(*m_pstyle);
	m_pstyleh1->getFont()->setSize(m_pstyle->getFont()->getSize() + 60);
	m_pstyleh1->getFont()->setWeight(BOLD);
	m_pstyleh1->setSpaceAbove(40.0);
	m_pstyleh1->setSpaceBelow(30.0);
	m_pstyleh1->setName("HTML_h1");
	m_pstylecode = new gtParagraphStyle(*m_pstyle);
	m_pstylecode->getFont()->setName("Courier Regular");
	m_pstylecode->setName("HTML_code");
	m_pstylep = new gtParagraphStyle(*m_pstyle);
	m_pstylep->setSpaceBelow(gtMeasure::i2d(5, SC_MM));
	m_pstylep->setName("HTML_p");
	m_pstylepre = new gtParagraphStyle(*m_pstyle);
	m_pstylepre->setName("HTML_pre");
}

void HTMLReader::startElement(void*, const xmlChar * fullname, const xmlChar ** atts)
{
	m_elemJustStarted = true;
	m_elemJustFinished = false;
	QString name(QString((const char*) fullname).toLower());
	QXmlAttributes attrs;
	if (atts)
	{
		for(const xmlChar** cur = atts; cur && *cur; cur += 2)
			attrs.append(QString((char*)*cur), NULL, QString((char*)*cur), QString((char*)*(cur + 1)));
	}
	m_hreader->startElement(NULL, NULL, name, attrs);
}

bool HTMLReader::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs) 
{
	if (name == "p")
		m_inP = true;
	else if (name == "center")
		m_inCenter = true;
	else if (name == "br")
		m_writer->append("\n", m_pstyle);
	else if (name == "a")
	{
		toggleEffect(UNDERLINE);
		setBlueFont();
		for (int i = 0; i < attrs.count(); i++)
		{
			if (attrs.localName(i) == "href")
			{
				m_href = attrs.value(i);
			}
			m_inA = true;
		}
	} 
	else if (name == "ul")
	{
		++m_listLevel;
		if (static_cast<int>(m_listStyles.size()) < (m_listLevel + 1))
			createListStyle();
		m_inUL = true;
		if (m_inOL)
		{
			m_inOL = false;
			m_wasInOL = true;
		}
	}
	else if (name == "ol")
	{
		++m_listLevel;
		if (static_cast<int>(m_listStyles.size()) < (m_listLevel + 1))
			createListStyle();
		m_inOL = true;
		if (m_inUL)
		{
			m_inUL = false;
			m_wasInUL = true;
		}
	}
	else if (name == "li")
		m_inLI = true;
	else if (name == "h1")
		m_inH1 = true;
	else if (name == "h2")
		m_inH2 = true;
	else if (name == "h3")
		m_inH3 = true;
	else if (name == "h4")
		m_inH4 = true;
	else if (name == "h5")
		m_inH5 = true;
	else if (name == "h6")
		m_inH6 = true;
	else if ((name == "b") || (name == "strong"))
		setBoldFont();
	else if ((name == "i") || (name == "em"))
		setItalicFont();
	else if (name == "code")
		m_inCode = true;
	else if (name == "body")
		m_inBody = true;
	else if (name == "pre")
		m_inPre = true;
	else if (name == "img")
	{
		QString imgline("(img,");
		for (int i = 0; i < attrs.count(); i++)
		{
			if (attrs.localName(i) == "src")
			{
				QString attrValue = attrs.value(i);
				if (attrValue.indexOf("data:image") < 0)
					imgline +=  " src: " + attrValue;
				else
				{
					// TODO: correctly embed the image (just putting the source in the
					// text frame crashes scribus for big images; ale/20120808)
					imgline +=  " src: embedded image";
				}
			}
			if (attrs.localName(i) == "alt")
			{
				if (!attrs.value(i).isEmpty())
					imgline += ", alt: " + attrs.value(i);
			}
		}
		imgline += ")\n\n";
		m_writer->append(imgline, m_pstyle);
	}
	else if (name == "sub")
		toggleEffect(SUBSCRIPT);
	else if (name == "sup")
		toggleEffect(SUPERSCRIPT);
	else if (name == "del")
		toggleEffect(STRIKETHROUGH);
	else if ((name == "ins" || name == "u") && (!m_inA))
		toggleEffect(UNDERLINE);
	return true;
}
void HTMLReader::characters(void*, const xmlChar * ch, int len)
{
	QString chars = QString::fromUtf8((const char*) ch, len);
	m_hreader->characters(chars);
}

bool HTMLReader::characters(const QString &ch) 
{
	if (m_inBody)
	{
		QString tmp = ch;
		// FIXME : According to html spec, new lines placed just after or just before an element
		// must be ignored, not exactly that, but better than nothing
		if (m_elemJustStarted  || m_elemJustFinished)
		{
			while( !tmp.isEmpty() && (tmp[0] == '\r' || tmp[0] == '\n') )
				tmp = tmp.right(tmp.length() - 1);
			m_elemJustStarted = m_elemJustFinished = false;
			if (tmp.isEmpty())
				return true;
		}
		QString chl = tmp.left(1), chr = tmp.right(1);
		bool fcis = (chl.length() > 0 && chl[0].isSpace());
		bool lcis = (chr.length() > 0 && chr[0].isSpace());
		if (m_inPre)
		{
			if (tmp.left(1) == "\n")
				tmp = tmp.right(tmp.length() - 2);
		}
		else
			tmp = tmp.simplified();

		if (!m_lastCharWasSpace)
			if (fcis)
				tmp = " " + tmp;

		if (lcis && !(fcis && tmp.length() <= 1))
			tmp = tmp + " ";
		m_lastCharWasSpace = lcis;
		if ((m_inLI) && (!m_addedLI))
		{
			if (m_inUL)
				tmp = "- " + tmp;
			else if (m_inOL)
			{
				tmp = QString("%1. ").arg(m_nextItemNumbers[m_listLevel]) + tmp;
				++m_nextItemNumbers[m_listLevel];
			}
			m_addedLI = true;
		}

		if (m_noFormatting)
			m_writer->appendUnstyled(tmp);
		else if (m_inP)
			m_writer->append(tmp, m_pstylep);
		else if (m_inLI)
		{
			m_writer->append(tmp, m_listStyles[m_listLevel]);
		}
		else if (m_inH1)
			m_writer->append(tmp, m_pstyleh1);
		else if (m_inH2)
			m_writer->append(tmp, m_pstyleh2);
		else if (m_inH3)
			m_writer->append(tmp, m_pstyleh3);
		else if (m_inH4)
			m_writer->append(tmp, m_pstyleh4);
		else if (m_inH5)
			m_writer->append(tmp, m_pstyleh5);
		else if (m_inH6)
			m_writer->append(tmp, m_pstyleh6);
		else if (m_inCenter)
			m_writer->append(tmp, m_pstylec);
		else if (m_inCode)
			m_writer->append(tmp, m_pstylecode);
		else if (m_inPre)
			m_writer->append(tmp, m_pstylepre);
		else
			m_writer->append(tmp, m_pstyle);
	}
	return true;
}

void HTMLReader::endElement(void*, const xmlChar * name)
{
	m_elemJustStarted = false;
	m_elemJustFinished = true;
	QString nname(QString((const char*) name).toLower());
	m_hreader->endElement(NULL, NULL, nname);
}

bool HTMLReader::endElement(const QString&, const QString&, const QString &name)
{
	if (name == "center")
	{
		m_inCenter = false;
		m_writer->append("\n");
	}
	else if (name == "p")
	{
		m_writer->append("\n");
		m_inP = false;
	}
	else if (name == "a")
	{
		toggleEffect(UNDERLINE);
		if ((!m_href.isEmpty()) && ((m_href.indexOf("//") != -1) ||
		    (m_href.indexOf("mailto:") != -1) || (m_href.indexOf("www") != -1)))
		{
			m_href = m_href.remove("mailto:");
			m_writer->append(QString(" [%1]").arg(m_extIndex), m_pstyle);
			m_extLinks += QString("[%1] ").arg(m_extIndex) + m_href + "\n";
			++m_extIndex;
		}
		m_href = "";
		setDefaultColor();
		m_inA = false;
	}
	else if (name == "ul")
	{
		if (m_listLevel == 0)
		{
			m_inUL = false;
			m_inOL = false;
			m_wasInUL = false;
			m_wasInOL = false;
			m_listLevel = -1;
		}
		else if (m_wasInOL)
		{
			m_inUL = false;
			m_inOL = true;
			m_wasInOL = false;
			--m_listLevel;
		}
		else if (m_wasInUL)
		{
			m_inUL = true;
			m_inOL = false;
			m_wasInUL = false;
			--m_listLevel;
		}
		else
			--m_listLevel;
		if (m_listLevel == -1)
			m_writer->append("\n");
	}
	else if (name == "ol")
	{
		if (m_listLevel == 0)
		{
			m_inUL = false;
			m_inOL = false;
			m_wasInUL = false;
			m_wasInOL = false;
			m_listLevel = -1;
		}
		else if (m_wasInUL)
		{
			m_inOL = false;
			m_inUL = true;
			m_wasInUL = false;
			m_nextItemNumbers[m_listLevel] = 1;
			--m_listLevel;
		}
		else if (m_wasInOL)
		{
			m_inOL = true;
			m_inUL = false;
			m_wasInOL = false;
			m_nextItemNumbers[m_listLevel] = 1;
			--m_listLevel;
		}
		else
		{
			m_nextItemNumbers[m_listLevel] = 1;
			--m_listLevel;
		}
		if (m_listLevel == -1)
			m_writer->append("\n");
	}
	else if (name == "li")
	{
		m_inLI = false;
		m_addedLI = false;
		m_writer->append("\n");
	}
	else if (name == "h1")
	{
		m_inH1 = false;
		m_writer->append("\n", m_pstyleh1);
	}
	else if (name == "h2")
	{
		m_inH2 = false;
		m_writer->append("\n", m_pstyleh2);
	}
	else if (name == "h3")
	{
		m_inH3 = false;
		m_writer->append("\n", m_pstyleh3);
	}
	else if (name == "h4")
	{
		m_inH4 = false;
		m_writer->append("\n", m_pstyleh4);
	}
	else if (name == "h5")
	{
		m_inH5 = false;
		m_writer->append("\n", m_pstyleh5);
	}
	else if (name == "h6")
	{
		m_inH6 = false;
		m_writer->append("\n", m_pstyleh6);
	}
	else if ((name == "b") || (name == "strong"))
		unSetBoldFont();
	else if ((name == "i") || (name == "em"))
		unsetItalicFont();
	else if (name == "code")
		m_inCode = false;
	else if (name == "body")
		m_inBody = false;
	else if (name == "pre")
	{
		m_inPre = false;
		m_writer->append("\n");
	}
	else if (name == "div")
		m_writer->append("\n");
	else if (name == "sub")
		toggleEffect(SUBSCRIPT);
	else if (name == "sup")
		toggleEffect(SUPERSCRIPT);
	else if (name == "del")
		toggleEffect(STRIKETHROUGH);
	else if ((name == "ins" || name == "u") && (!m_inA))
		toggleEffect(UNDERLINE);
	return true;
}

void HTMLReader::toggleEffect(FontEffect e)
{
	m_pstyle->getFont()->toggleEffect(e);
	m_pstylec->getFont()->toggleEffect(e);
	for (uint i = 0; i < m_listStyles.size(); ++i)
		m_listStyles[i]->getFont()->toggleEffect(e);
	m_pstyleh1->getFont()->toggleEffect(e);
	m_pstyleh2->getFont()->toggleEffect(e);
	m_pstyleh3->getFont()->toggleEffect(e);
	m_pstyleh4->getFont()->toggleEffect(e);
	m_pstyleh5->getFont()->toggleEffect(e);
	m_pstyleh6->getFont()->toggleEffect(e);
	m_pstylecode->getFont()->toggleEffect(e);
	m_pstylep->getFont()->toggleEffect(e);
	m_pstylepre->getFont()->toggleEffect(e);
}

void HTMLReader::setItalicFont()
{
	m_pstyle->getFont()->setSlant(ITALIC);
	m_pstylec->getFont()->setSlant(ITALIC);
	for (uint i = 0; i < m_listStyles.size(); ++i)
		m_listStyles[i]->getFont()->setSlant(ITALIC);
	m_pstyleh1->getFont()->setSlant(ITALIC);
	m_pstyleh2->getFont()->setSlant(ITALIC);
	m_pstyleh3->getFont()->setSlant(ITALIC);
	m_pstyleh4->getFont()->setSlant(ITALIC);
	m_pstyleh5->getFont()->setSlant(ITALIC);
	m_pstyleh6->getFont()->setSlant(ITALIC);
	m_pstylecode->getFont()->setSlant(ITALIC);
	m_pstylep->getFont()->setSlant(ITALIC);
	m_pstylepre->getFont()->setSlant(ITALIC);
}

void HTMLReader::unsetItalicFont()
{
	m_pstyle->getFont()->setSlant(m_defaultSlant);
	m_pstylec->getFont()->setSlant(m_defaultSlant);
	for (uint i = 0; i < m_listStyles.size(); ++i)
		m_listStyles[i]->getFont()->setSlant(m_defaultSlant);
	m_pstyleh1->getFont()->setSlant(m_defaultSlant);
	m_pstyleh2->getFont()->setSlant(m_defaultSlant);
	m_pstyleh3->getFont()->setSlant(m_defaultSlant);
	m_pstyleh4->getFont()->setSlant(m_defaultSlant);
	m_pstyleh5->getFont()->setSlant(m_defaultSlant);
	m_pstyleh6->getFont()->setSlant(m_defaultSlant);
	m_pstylecode->getFont()->setSlant(m_defaultSlant);
	m_pstylep->getFont()->setSlant(m_defaultSlant);
	m_pstylepre->getFont()->setSlant(m_defaultSlant);
}

void HTMLReader::setBlueFont()
{
	m_pstyle->getFont()->setColor("Blue");
	m_pstylec->getFont()->setColor("Blue");
	for (uint i = 0; i < m_listStyles.size(); ++i)
		m_listStyles[i]->getFont()->setColor("Blue");
	m_pstyleh1->getFont()->setColor("Blue");
	m_pstyleh2->getFont()->setColor("Blue");
	m_pstyleh3->getFont()->setColor("Blue");
	m_pstyleh4->getFont()->setColor("Blue");
	m_pstyleh5->getFont()->setColor("Blue");
	m_pstyleh6->getFont()->setColor("Blue");
	m_pstylecode->getFont()->setColor("Blue");
	m_pstylep->getFont()->setColor("Blue");
	m_pstylepre->getFont()->setColor("Blue");
}

void HTMLReader::setDefaultColor()
{
	m_pstyle->getFont()->setColor(m_defaultColor);
	m_pstylec->getFont()->setColor(m_defaultColor);
	for (uint i = 0; i < m_listStyles.size(); ++i)
		m_listStyles[i]->getFont()->setColor(m_defaultColor);
	m_pstyleh1->getFont()->setColor(m_defaultColor);
	m_pstyleh2->getFont()->setColor(m_defaultColor);
	m_pstyleh3->getFont()->setColor(m_defaultColor);
	m_pstyleh4->getFont()->setColor(m_defaultColor);
	m_pstyleh5->getFont()->setColor(m_defaultColor);
	m_pstyleh6->getFont()->setColor(m_defaultColor);
	m_pstylecode->getFont()->setColor(m_defaultColor);
	m_pstylep->getFont()->setColor(m_defaultColor);
	m_pstylepre->getFont()->setColor(m_defaultColor);
}

void HTMLReader::setBoldFont()
{
	m_pstyle->getFont()->setWeight(BOLD);
	m_pstylec->getFont()->setWeight(BOLD);
	for (uint i = 0; i < m_listStyles.size(); ++i)
		m_listStyles[i]->getFont()->setWeight(BOLD);
	m_pstylecode->getFont()->setWeight(BOLD);
	m_pstylep->getFont()->setWeight(BOLD);
	m_pstylepre->getFont()->setWeight(BOLD);
}

void HTMLReader::unSetBoldFont()
{
	m_pstyle->getFont()->setWeight(m_defaultWeight);
	m_pstylec->getFont()->setWeight(m_defaultWeight);
	for (uint i = 0; i < m_listStyles.size(); ++i)
		m_listStyles[i]->getFont()->setWeight(m_defaultWeight);
	m_pstylecode->getFont()->setWeight(REGULAR);
	m_pstylep->getFont()->setWeight(m_defaultWeight);
	m_pstylepre->getFont()->setWeight(m_defaultWeight);
}

void HTMLReader::parse(QString filename)
{
#if defined(_WIN32)
	QString fname = QDir::toNativeSeparators(filename);
	QByteArray fn = (QSysInfo::WindowsVersion & QSysInfo::WV_NT_based) ? fname.toUtf8() : fname.toLocal8Bit();
#else
	QByteArray fn(filename.toLocal8Bit());
#endif
	m_elemJustStarted = m_elemJustFinished = false;
	htmlSAXParseFile(fn.data(), NULL, mySAXHandler, NULL);
}

void HTMLReader::createListStyle()
{
	gtParagraphStyle* tmpStyle = new gtParagraphStyle(*m_listStyles[0]);
	tmpStyle->setName(QString("HTML_li_level-%1").arg(m_listLevel + 1));
	double indent = m_listStyles[0]->getIndent();
	indent += 25 * (m_listLevel + 1);
	tmpStyle->setIndent(indent);
	m_listStyles.push_back(tmpStyle);
	m_nextItemNumbers.push_back(1);
}

htmlSAXHandler mySAXHandlerStruct = {
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
	HTMLReader::startElement,
	HTMLReader::endElement,
	NULL, // reference,
	HTMLReader::characters,
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

htmlSAXHandlerPtr mySAXHandler = &mySAXHandlerStruct;

HTMLReader::~HTMLReader()
{
	if (!m_extLinks.isEmpty())
	{
		m_writer->append(QObject::tr("\nExternal Links\n"), m_pstyleh4);
		m_writer->append(m_extLinks, m_pstyle);
	}
	for (uint i = 0; i < m_listStyles.size(); ++i)
		delete m_listStyles[i];
	delete m_pstylec;
	delete m_pstyleh1;
	delete m_pstyleh2;
	delete m_pstyleh3;
	delete m_pstyleh4;
	delete m_pstyleh5;
	delete m_pstyleh6;
	delete m_pstylecode;
	delete m_pstylep;
	delete m_pstylepre;
	m_hreader = NULL;
}

