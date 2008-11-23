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

#include <QObject>
#include <QByteArray>
#include "htmlreader.h"

#include "scribusstructs.h"
#include "gtmeasure.h"

HTMLReader* HTMLReader::hreader = NULL;
bool HTMLReader::elemJustStarted = false;
bool HTMLReader::elemJustFinished = false;

extern htmlSAXHandlerPtr mySAXHandler;

HTMLReader::HTMLReader(gtParagraphStyle *ps, gtWriter *w, bool textOnly)
{
	pstyle = ps;
	defaultColor = ps->getFont()->getColor();
	defaultWeight = ps->getFont()->getWeight();
	defaultSlant = ps->getFont()->getSlant();
	initPStyles();
	inH1 = false;
	inH2 = false;
	inH3 = false;
	inA = false;
	inCode = false;
	inBody = false;
	inPre = false;
	inP = false;
	inCenter = false;
	writer = w;
	href = "";
	extLinks = "";
	extIndex = 1;
	listLevel = -1;
	inOL = false;
	wasInOL = false;
	inUL = false;
	wasInUL = false;
	inLI = false;
	addedLI = false;
	lastCharWasSpace = false;
	noFormatting = textOnly;
	hreader = this;
}

void HTMLReader::initPStyles()
{
	pstylec = new gtParagraphStyle(*pstyle);
	pstylec->setAlignment(CENTER);
	pstylec->setName("HTML_center");
	gtParagraphStyle* pstyleli = new gtParagraphStyle(*pstyle);
	pstyleli->setIndent(pstyleli->getIndent()+50.0);
	pstyleli->setName("HTML_li_level-0");
	listStyles.push_back(pstyleli);
	nextItemNumbers.push_back(1);
	pstyleh4 = new gtParagraphStyle(*pstyle);
	pstyleh4->getFont()->setSize(pstyle->getFont()->getSize() + 10);
	pstyleh4->getFont()->setWeight(BOLD);
	pstyleh4->setSpaceAbove(10.0);
	pstyleh4->setSpaceBelow(5.0);
	pstyleh4->setName("HTML_h4");
	pstyleh3 = new gtParagraphStyle(*pstyle);
	pstyleh3->getFont()->setSize(pstyle->getFont()->getSize() + 20);
	pstyleh3->getFont()->setWeight(BOLD);
	pstyleh3->setSpaceAbove(20.0);
	pstyleh3->setSpaceBelow(10.0);
	pstyleh3->setName("HTML_h3");
	pstyleh2 = new gtParagraphStyle(*pstyle);
	pstyleh2->getFont()->setSize(pstyle->getFont()->getSize() + 40);
	pstyleh2->getFont()->setWeight(BOLD);
	pstyleh2->setSpaceAbove(30.0);
	pstyleh2->setSpaceBelow(20.0);
	pstyleh2->setName("HTML_h2");
	pstyleh1 = new gtParagraphStyle(*pstyle);
	pstyleh1->getFont()->setSize(pstyle->getFont()->getSize() + 60);
	pstyleh1->getFont()->setWeight(BOLD);
	pstyleh1->setSpaceAbove(40.0);
	pstyleh1->setSpaceBelow(30.0);
	pstyleh1->setName("HTML_h1");
	pstylecode = new gtParagraphStyle(*pstyle);
	pstylecode->getFont()->setName("Courier Regular");
	pstylecode->setName("HTML_code");
	pstylep = new gtParagraphStyle(*pstyle);
	pstylep->setSpaceBelow(gtMeasure::i2d(5, SC_MM));
	pstylep->setName("HTML_p");
	pstylepre = new gtParagraphStyle(*pstyle);
	pstylepre->setName("HTML_pre");
}

void HTMLReader::startElement(void*, const xmlChar * fullname, const xmlChar ** atts)
{
	elemJustStarted = true;
	elemJustFinished = false;
	QString name(QString((const char*) fullname).toLower());
	QXmlAttributes attrs;
	if (atts)
	{
		for(const xmlChar** cur = atts; cur && *cur; cur += 2)
			attrs.append(QString((char*)*cur), NULL, QString((char*)*cur), QString((char*)*(cur + 1)));
	}
	hreader->startElement(NULL, NULL, name, attrs);
}

bool HTMLReader::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs) 
{
	if (name == "p")
		inP = true;
	else if (name == "center")
		inCenter = true;
	else if (name == "br")
		writer->append("\n", pstyle);
	else if (name == "a")
	{
		toggleEffect(UNDERLINE);
		setBlueFont();
		for (int i = 0; i < attrs.count(); i++)
		{
			if (attrs.localName(i) == "href")
			{
				href = attrs.value(i);
			}
			inA = true;
		}
	} 
	else if (name == "ul")
	{
		++listLevel;
		if (static_cast<int>(listStyles.size()) < (listLevel + 1))
			createListStyle();
		inUL = true;
		if (inOL)
		{
			inOL = false;
			wasInOL = true;
		}
	}
	else if (name == "ol")
	{
		++listLevel;
		if (static_cast<int>(listStyles.size()) < (listLevel + 1))
			createListStyle();
		inOL = true;
		if (inUL)
		{
			inUL = false;
			wasInUL = true;
		}
	}
	else if (name == "li")
		inLI = true;
	else if (name == "h1")
		inH1 = true;
	else if (name == "h2")
		inH2 = true;
	else if (name == "h3")
		inH3 = true;
	else if (name == "h4")
		inH4 = true;
	else if ((name == "b") || (name == "strong"))
		setBoldFont();
	else if ((name == "i") || (name == "em"))
		setItalicFont();
	else if (name == "code")
		inCode = true;
	else if (name == "body")
		inBody = true;
	else if (name == "pre")
		inPre = true;
	else if (name == "img")
	{
		QString imgline("(img,");
		for (int i = 0; i < attrs.count(); i++)
		{
			if (attrs.localName(i) == "src")
			{
				imgline +=  " src: " + attrs.value(i);
			}
			if (attrs.localName(i) == "alt")
			{
				if (!attrs.value(i).isEmpty())
					imgline += ", alt: " + attrs.value(i);
			}
		}
		imgline += ")\n\n";
		writer->append(imgline, pstyle);
	}
	else if (name == "sub")
		toggleEffect(SUBSCRIPT);
	else if (name == "sup")
		toggleEffect(SUPERSCRIPT);
	else if (name == "del")
		toggleEffect(STRIKETHROUGH);
	else if ((name == "ins" || name == "u") && (!inA))
		toggleEffect(UNDERLINE);
	return true;
}
void HTMLReader::characters(void*, const xmlChar * ch, int len)
{
	QString chars = QString::fromUtf8((const char*) ch, len);
	hreader->characters(chars);
}

bool HTMLReader::characters(const QString &ch) 
{
	if (inBody)
	{
		QString tmp = ch;
		// FIXME : According to html spec, new lines placed just after or just before an element
		// must be ignored, not exactly that, but better than nothing
		if (elemJustStarted  || elemJustFinished)
		{
			while( !tmp.isEmpty() && (tmp[0] == '\r' || tmp[0] == '\n') )
				tmp = tmp.right(tmp.length() - 1);
			elemJustStarted = elemJustFinished = false;
			if (tmp.isEmpty())
				return true;
		}
		QString chl = tmp.left(1), chr = tmp.right(1);
		bool fcis = (chl.length() > 0 && chl[0].isSpace());
		bool lcis = (chr.length() > 0 && chr[0].isSpace());
		if (inPre)
		{
			if (tmp.left(1) == "\n")
				tmp = tmp.right(tmp.length() - 2);
		}
		else
			tmp = tmp.simplified();

		if (!lastCharWasSpace)
			if (fcis)
				tmp = " " + tmp;

		if (lcis)
			tmp = tmp + " ";
		lastCharWasSpace = lcis;
		if ((inLI) && (!addedLI))
		{
			if (inUL)
				tmp = "- " + tmp;
			else if (inOL)
			{
				tmp = QString("%1. ").arg(nextItemNumbers[listLevel]) + tmp;
				++nextItemNumbers[listLevel];
			}
			addedLI = true;
		}

		if (noFormatting)
			writer->appendUnstyled(tmp);
		else if (inP)
			writer->append(tmp, pstylep);
		else if (inLI)
		{
			writer->append(tmp, listStyles[listLevel]);
		}
		else if (inH1)
			writer->append(tmp, pstyleh1);
		else if (inH2)
			writer->append(tmp, pstyleh2);
		else if (inH3)
			writer->append(tmp, pstyleh3);
		else if (inH4)
			writer->append(tmp, pstyleh4);
		else if (inCenter)
			writer->append(tmp, pstylec);
		else if (inCode)
			writer->append(tmp, pstylecode);
		else if (inPre)
			writer->append(tmp, pstylepre);
		else
			writer->append(tmp, pstyle);
	}
	return true;
}

void HTMLReader::endElement(void*, const xmlChar * name)
{
	elemJustStarted = false;
	elemJustFinished = true;
	QString nname(QString((const char*) name).toLower());
	hreader->endElement(NULL, NULL, nname);
}

bool HTMLReader::endElement(const QString&, const QString&, const QString &name)
{
	if (name == "center")
	{
		inCenter = false;
		writer->append("\n");
	}
	else if (name == "p")
	{
		writer->append("\n");
		inP = false;
	}
	else if (name == "a")
	{
		toggleEffect(UNDERLINE);
		if ((!href.isEmpty()) && ((href.indexOf("//") != -1) ||
		    (href.indexOf("mailto:") != -1) || (href.indexOf("www") != -1)))
		{
			href = href.remove("mailto:");
			writer->append(QString(" [%1]").arg(extIndex), pstyle);
			extLinks += QString("[%1] ").arg(extIndex) + href + "\n";
			++extIndex;
		}
		href = "";
		setDefaultColor();
		inA = false;
	}
	else if (name == "ul")
	{
		if (listLevel == 0)
		{
			inUL = false;
			inOL = false;
			wasInUL = false;
			wasInOL = false;
			listLevel = -1;
		}
		else if (wasInOL)
		{
			inUL = false;
			inOL = true;
			wasInOL = false;
			--listLevel;
		}
		else if (wasInUL)
		{
			inUL = true;
			inOL = false;
			wasInUL = false;
			--listLevel;
		}
		else
			--listLevel;
		if (listLevel == -1)
			writer->append("\n");
	}
	else if (name == "ol")
	{
		if (listLevel == 0)
		{
			inUL = false;
			inOL = false;
			wasInUL = false;
			wasInOL = false;
			listLevel = -1;
		}
		else if (wasInUL)
		{
			inOL = false;
			inUL = true;
			wasInUL = false;
			nextItemNumbers[listLevel] = 1;
			--listLevel;
		}
		else if (wasInOL)
		{
			inOL = true;
			inUL = false;
			wasInOL = false;
			nextItemNumbers[listLevel] = 1;
			--listLevel;
		}
		else
		{
			nextItemNumbers[listLevel] = 1;
			--listLevel;
		}
		if (listLevel == -1)
			writer->append("\n");
	}
	else if (name == "li")
	{
		inLI = false;
		addedLI = false;
		writer->append("\n");
	}
	else if (name == "h1")
	{
		inH1 = false;
		writer->append("\n");
	}
	else if (name == "h2")
	{
		inH2 = false;
		writer->append("\n");
	}
	else if (name == "h3")
	{
		inH3 = false;
		writer->append("\n");
	}
	else if (name == "h4")
	{
		inH4 = false;
		writer->append("\n");
	}
	else if ((name == "b") || (name == "strong"))
		unSetBoldFont();
	else if ((name == "i") || (name == "em"))
		unsetItalicFont();
	else if (name == "code")
		inCode = false;
	else if (name == "body")
		inBody = false;
	else if (name == "pre")
	{
		inPre = false;
		writer->append("\n");
	}
	else if (name == "div")
		writer->append("\n");
	else if (name == "sub")
		toggleEffect(SUBSCRIPT);
	else if (name == "sup")
		toggleEffect(SUPERSCRIPT);
	else if (name == "del")
		toggleEffect(STRIKETHROUGH);
	else if ((name == "ins" || name == "u") && (!inA))
		toggleEffect(UNDERLINE);
	return true;
}

void HTMLReader::toggleEffect(FontEffect e)
{
	pstyle->getFont()->toggleEffect(e);
	pstylec->getFont()->toggleEffect(e);
	for (uint i = 0; i < listStyles.size(); ++i)
		listStyles[i]->getFont()->toggleEffect(e);
	pstyleh1->getFont()->toggleEffect(e);
	pstyleh2->getFont()->toggleEffect(e);
	pstyleh3->getFont()->toggleEffect(e);
	pstyleh4->getFont()->toggleEffect(e);
	pstylecode->getFont()->toggleEffect(e);
	pstylep->getFont()->toggleEffect(e);
	pstylepre->getFont()->toggleEffect(e);
}

void HTMLReader::setItalicFont()
{
	pstyle->getFont()->setSlant(ITALIC);
	pstylec->getFont()->setSlant(ITALIC);
	for (uint i = 0; i < listStyles.size(); ++i)
		listStyles[i]->getFont()->setSlant(ITALIC);
	pstyleh1->getFont()->setSlant(ITALIC);
	pstyleh2->getFont()->setSlant(ITALIC);
	pstyleh3->getFont()->setSlant(ITALIC);
	pstyleh4->getFont()->setSlant(ITALIC);
	pstylecode->getFont()->setSlant(ITALIC);
	pstylep->getFont()->setSlant(ITALIC);
	pstylepre->getFont()->setSlant(ITALIC);
}

void HTMLReader::unsetItalicFont()
{
	pstyle->getFont()->setSlant(defaultSlant);
	pstylec->getFont()->setSlant(defaultSlant);
	for (uint i = 0; i < listStyles.size(); ++i)
		listStyles[i]->getFont()->setSlant(defaultSlant);
	pstyleh1->getFont()->setSlant(defaultSlant);
	pstyleh2->getFont()->setSlant(defaultSlant);
	pstyleh3->getFont()->setSlant(defaultSlant);
	pstyleh4->getFont()->setSlant(defaultSlant);
	pstylecode->getFont()->setSlant(defaultSlant);
	pstylep->getFont()->setSlant(defaultSlant);
	pstylepre->getFont()->setSlant(defaultSlant);
}

void HTMLReader::setBlueFont()
{
	pstyle->getFont()->setColor("Blue");
	pstylec->getFont()->setColor("Blue");
	for (uint i = 0; i < listStyles.size(); ++i)
		listStyles[i]->getFont()->setColor("Blue");
	pstyleh1->getFont()->setColor("Blue");
	pstyleh2->getFont()->setColor("Blue");
	pstyleh3->getFont()->setColor("Blue");
	pstyleh4->getFont()->setColor("Blue");
	pstylecode->getFont()->setColor("Blue");
	pstylep->getFont()->setColor("Blue");
	pstylepre->getFont()->setColor("Blue");
}

void HTMLReader::setDefaultColor()
{
	pstyle->getFont()->setColor(defaultColor);
	pstylec->getFont()->setColor(defaultColor);
	for (uint i = 0; i < listStyles.size(); ++i)
		listStyles[i]->getFont()->setColor(defaultColor);
	pstyleh1->getFont()->setColor(defaultColor);
	pstyleh2->getFont()->setColor(defaultColor);
	pstyleh3->getFont()->setColor(defaultColor);
	pstyleh4->getFont()->setColor(defaultColor);
	pstylecode->getFont()->setColor(defaultColor);
	pstylep->getFont()->setColor(defaultColor);
	pstylepre->getFont()->setColor(defaultColor);
}

void HTMLReader::setBoldFont()
{
	pstyle->getFont()->setWeight(BOLD);
	pstylec->getFont()->setWeight(BOLD);
	for (uint i = 0; i < listStyles.size(); ++i)
		listStyles[i]->getFont()->setWeight(BOLD);
	pstylecode->getFont()->setWeight(BOLD);
	pstylep->getFont()->setWeight(BOLD);
	pstylepre->getFont()->setWeight(BOLD);
}

void HTMLReader::unSetBoldFont()
{
	pstyle->getFont()->setWeight(defaultWeight);
	pstylec->getFont()->setWeight(defaultWeight);
	for (uint i = 0; i < listStyles.size(); ++i)
		listStyles[i]->getFont()->setWeight(defaultWeight);
	pstylecode->getFont()->setWeight(REGULAR);
	pstylep->getFont()->setWeight(defaultWeight);
	pstylepre->getFont()->setWeight(defaultWeight);
}

void HTMLReader::parse(QString filename)
{
#if defined(_WIN32)
	QString fname = QDir::convertSeparators(filename);
	QByteArray fn = (QSysInfo::WindowsVersion & QSysInfo::WV_NT_based) ? fname.toUtf8() : fname.toLocal8Bit();
#else
	QByteArray fn(filename.toLocal8Bit());
#endif
	elemJustStarted = elemJustFinished = false;
	htmlSAXParseFile(fn.data(), NULL, mySAXHandler, NULL);
}

void HTMLReader::createListStyle()
{
	gtParagraphStyle* tmpStyle = new gtParagraphStyle(*listStyles[0]);
	tmpStyle->setName(QString("HTML_li_level-%1").arg(listLevel + 1));
	double indent = listStyles[0]->getIndent();
	indent += 25 * (listLevel + 1);
	tmpStyle->setIndent(indent);
	listStyles.push_back(tmpStyle);
	nextItemNumbers.push_back(1);
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
	if (!extLinks.isEmpty())
	{
		writer->append(QObject::tr("\nExternal Links\n"), pstyleh4);
		writer->append(extLinks, pstyle);
	}
	for (uint i = 0; i < listStyles.size(); ++i)
		delete listStyles[i];
	delete pstylec;
	delete pstyleh1;
	delete pstyleh2;
	delete pstyleh3;
	delete pstyleh4;
	delete pstylecode;
	delete pstylep;
	delete pstylepre;
	hreader = NULL;
}

