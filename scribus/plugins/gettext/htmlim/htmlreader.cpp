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

#include <QByteArray>
#include <QDir>
#include <QObject>

#include "htmlreader.h"
#include "gtmeasure.h"
#include "scribusstructs.h"

HTMLReader* HTMLReader::hreader = nullptr;
bool HTMLReader::elemJustStarted = false;
bool HTMLReader::elemJustFinished = false;

extern htmlSAXHandlerPtr mySAXHandler;

HTMLReader::HTMLReader(gtParagraphStyle *ps, gtWriter *w, bool textOnly)
	: defaultColor(ps->getFont()->getColor()),
	  defaultWeight(ps->getFont()->getWeight()),
	  defaultSlant(ps->getFont()->getSlant()),
	  writer(w),
	  pstyle(ps),
	  noFormatting(textOnly)
{
	initPStyles();
	hreader = this;
}

HTMLReader::~HTMLReader()
{
	if (!extLinks.isEmpty())
	{
		writer->append(QObject::tr("\nExternal Links\n"), pstyleh4.get());
		writer->append(extLinks, pstyle);
	}
	for (gtParagraphStyle* pStyle : listStyles)
		delete pStyle;
	hreader = nullptr;
}

void HTMLReader::initPStyles()
{
	pstylec = std::make_unique<gtParagraphStyle>(*pstyle);
	pstylec->setAlignment(CENTER);
	pstylec->setName("HTML_center");

	auto* pstyleli = new gtParagraphStyle(*pstyle);
	pstyleli->setIndent(pstyleli->getIndent()+50.0);
	pstyleli->setName("HTML_li_level-0");
	listStyles.push_back(pstyleli);
	nextItemNumbers.push_back(1);

	pstyleh6 = std::make_unique<gtParagraphStyle>(*pstyle);
	pstyleh6->getFont()->setSize(pstyle->getFont()->getSize() + 2.5);
	pstyleh6->getFont()->setWeight(BOLD);
	pstyleh6->setSpaceAbove(2.5);
	pstyleh6->setSpaceBelow(1.25);
	pstyleh6->setName("HTML_h6");

	pstyleh5 = std::make_unique<gtParagraphStyle>(*pstyle);
	pstyleh5->getFont()->setSize(pstyle->getFont()->getSize() + 5);
	pstyleh5->getFont()->setWeight(BOLD);
	pstyleh5->setSpaceAbove(5.0);
	pstyleh5->setSpaceBelow(2.5);
	pstyleh5->setName("HTML_h5");

	pstyleh4 = std::make_unique<gtParagraphStyle>(*pstyle);
	pstyleh4->getFont()->setSize(pstyle->getFont()->getSize() + 10);
	pstyleh4->getFont()->setWeight(BOLD);
	pstyleh4->setSpaceAbove(10.0);
	pstyleh4->setSpaceBelow(5.0);
	pstyleh4->setName("HTML_h4");

	pstyleh3 = std::make_unique<gtParagraphStyle>(*pstyle);
	pstyleh3->getFont()->setSize(pstyle->getFont()->getSize() + 20);
	pstyleh3->getFont()->setWeight(BOLD);
	pstyleh3->setSpaceAbove(20.0);
	pstyleh3->setSpaceBelow(10.0);
	pstyleh3->setName("HTML_h3");

	pstyleh2 = std::make_unique<gtParagraphStyle>(*pstyle);
	pstyleh2->getFont()->setSize(pstyle->getFont()->getSize() + 40);
	pstyleh2->getFont()->setWeight(BOLD);
	pstyleh2->setSpaceAbove(30.0);
	pstyleh2->setSpaceBelow(20.0);
	pstyleh2->setName("HTML_h2");

	pstyleh1 = std::make_unique<gtParagraphStyle>(*pstyle);
	pstyleh1->getFont()->setSize(pstyle->getFont()->getSize() + 60);
	pstyleh1->getFont()->setWeight(BOLD);
	pstyleh1->setSpaceAbove(40.0);
	pstyleh1->setSpaceBelow(30.0);
	pstyleh1->setName("HTML_h1");

	pstylecode = std::make_unique<gtParagraphStyle>(*pstyle);
	pstylecode->getFont()->setName("Courier Regular");
	pstylecode->setName("HTML_code");

	pstylep = std::make_unique<gtParagraphStyle>(*pstyle);
	pstylep->setSpaceBelow(gtMeasure::i2d(5, SC_MM));
	pstylep->setName("HTML_p");

	pstylepre = std::make_unique<gtParagraphStyle>(*pstyle);
	pstylepre->setName("HTML_pre");
}

void HTMLReader::startElement(void*, const xmlChar * fullname, const xmlChar ** atts)
{
	elemJustStarted = true;
	elemJustFinished = false;
	QString name(QString((const char*) fullname).toLower());
	HTMLAttributesMap attrs;
	if (atts)
	{
		for (const xmlChar** cur = atts; cur && *cur; cur += 2)
		{
			QString attrName((const char*)*cur);
			QString attrValue((const char*)*(cur + 1));
			attrs[attrName] = attrValue;
		}
	}
	hreader->startElement(name, attrs);
}

bool HTMLReader::startElement(const QString &name, const HTMLAttributesMap &attrs) 
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
		QString hRefVal = attrs.value("href");
		if (!hRefVal.isEmpty())
			href = hRefVal;
		inA = true;
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
	else if (name == "h5")
		inH5 = true;
	else if (name == "h6")
		inH6 = true;
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
		QString srcValue = attrs.value("src");
		if (!srcValue.isEmpty())
		{
			QString attrValue = srcValue;
			if (attrValue.indexOf("data:image") < 0)
				imgline +=  " src: " + attrValue;
			else
			{
				// TODO: correctly embed the image (just putting the source in the
				// text frame crashes scribus for big images; ale/20120808)
				imgline +=  " src: embedded image";
			}
		}
		QString altValue = attrs.value("alt");
		if (!altValue.isEmpty())
			imgline += ", alt: " + altValue;
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
			while (!tmp.isEmpty() && (tmp[0] == '\r' || tmp[0] == '\n'))
				tmp = tmp.right(tmp.length() - 1);
			elemJustStarted = elemJustFinished = false;
			if (tmp.isEmpty())
				return true;
		}
		QString chl(tmp.at(0));
		QString chr(tmp.right(1));
		bool fcis = (!chl.isEmpty() && chl[0].isSpace());
		bool lcis = (!chr.isEmpty() && chr[0].isSpace());
		if (inPre)
		{
			if (tmp.at(0) == "\n")
				tmp = tmp.right(tmp.length() - 2);
		}
		else
			tmp = tmp.simplified();

		if (tmp.isEmpty())
			return true;

		if (!lastCharWasSpace && fcis)
			tmp = " " + tmp;

		if (lcis && !(fcis && tmp.length() <= 1))
			tmp = tmp + " ";
		lastCharWasSpace = lcis;
		if (inLI && !addedLI)
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
			writer->append(tmp, pstylep.get());
		else if (inLI)
		{
			writer->append(tmp, listStyles[listLevel]);
		}
		else if (inH1)
			writer->append(tmp, pstyleh1.get());
		else if (inH2)
			writer->append(tmp, pstyleh2.get());
		else if (inH3)
			writer->append(tmp, pstyleh3.get());
		else if (inH4)
			writer->append(tmp, pstyleh4.get());
		else if (inH5)
			writer->append(tmp, pstyleh5.get());
		else if (inH6)
			writer->append(tmp, pstyleh6.get());
		else if (inCenter)
			writer->append(tmp, pstylec.get());
		else if (inCode)
			writer->append(tmp, pstylecode.get());
		else if (inPre)
			writer->append(tmp, pstylepre.get());
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
	hreader->endElement(nname);
}

bool HTMLReader::endElement(const QString &name)
{
	if (name == "center")
	{
		inCenter = false;
		lastCharWasSpace = true;
		writer->append("\n");
	}
	else if (name == "p")
	{
		inP = false;
		lastCharWasSpace = true;
		writer->append("\n");
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
		{
			lastCharWasSpace = true;
			writer->append("\n");
		}
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
		{
			lastCharWasSpace = true;
			writer->append("\n");
		}
	}
	else if (name == "li")
	{
		inLI = false;
		addedLI = false;
		lastCharWasSpace = true;
		writer->append("\n");
	}
	else if (name == "h1")
	{
		inH1 = false;
		lastCharWasSpace = true;
		writer->append("\n", pstyleh1.get());
	}
	else if (name == "h2")
	{
		inH2 = false;
		lastCharWasSpace = true;
		writer->append("\n", pstyleh2.get());
	}
	else if (name == "h3")
	{
		inH3 = false;
		lastCharWasSpace = true;
		writer->append("\n", pstyleh3.get());
	}
	else if (name == "h4")
	{
		inH4 = false;
		lastCharWasSpace = true;
		writer->append("\n", pstyleh4.get());
	}
	else if (name == "h5")
	{
		inH5 = false;
		lastCharWasSpace = true;
		writer->append("\n", pstyleh5.get());
	}
	else if (name == "h6")
	{
		inH6 = false;
		lastCharWasSpace = true;
		writer->append("\n", pstyleh6.get());
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
		lastCharWasSpace = true;
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
	for (gtParagraphStyle* pGtStyle : listStyles)
		pGtStyle->getFont()->toggleEffect(e);
	pstyleh1->getFont()->toggleEffect(e);
	pstyleh2->getFont()->toggleEffect(e);
	pstyleh3->getFont()->toggleEffect(e);
	pstyleh4->getFont()->toggleEffect(e);
	pstyleh5->getFont()->toggleEffect(e);
	pstyleh6->getFont()->toggleEffect(e);
	pstylecode->getFont()->toggleEffect(e);
	pstylep->getFont()->toggleEffect(e);
	pstylepre->getFont()->toggleEffect(e);
}

void HTMLReader::setItalicFont()
{
	pstyle->getFont()->setSlant(ITALIC);
	pstylec->getFont()->setSlant(ITALIC);
	for (gtParagraphStyle* pGtStyle : listStyles)
		pGtStyle->getFont()->setSlant(ITALIC);
	pstyleh1->getFont()->setSlant(ITALIC);
	pstyleh2->getFont()->setSlant(ITALIC);
	pstyleh3->getFont()->setSlant(ITALIC);
	pstyleh4->getFont()->setSlant(ITALIC);
	pstyleh5->getFont()->setSlant(ITALIC);
	pstyleh6->getFont()->setSlant(ITALIC);
	pstylecode->getFont()->setSlant(ITALIC);
	pstylep->getFont()->setSlant(ITALIC);
	pstylepre->getFont()->setSlant(ITALIC);
}

void HTMLReader::unsetItalicFont()
{
	pstyle->getFont()->setSlant(defaultSlant);
	pstylec->getFont()->setSlant(defaultSlant);
	for (gtParagraphStyle* pGtStyle : listStyles)
		pGtStyle->getFont()->setSlant(defaultSlant);
	pstyleh1->getFont()->setSlant(defaultSlant);
	pstyleh2->getFont()->setSlant(defaultSlant);
	pstyleh3->getFont()->setSlant(defaultSlant);
	pstyleh4->getFont()->setSlant(defaultSlant);
	pstyleh5->getFont()->setSlant(defaultSlant);
	pstyleh6->getFont()->setSlant(defaultSlant);
	pstylecode->getFont()->setSlant(defaultSlant);
	pstylep->getFont()->setSlant(defaultSlant);
	pstylepre->getFont()->setSlant(defaultSlant);
}

void HTMLReader::setBlueFont()
{
	pstyle->getFont()->setColor("Blue");
	pstylec->getFont()->setColor("Blue");
	for (gtParagraphStyle* pGtStyle : listStyles)
		pGtStyle->getFont()->setColor("Blue");
	pstyleh1->getFont()->setColor("Blue");
	pstyleh2->getFont()->setColor("Blue");
	pstyleh3->getFont()->setColor("Blue");
	pstyleh4->getFont()->setColor("Blue");
	pstyleh5->getFont()->setColor("Blue");
	pstyleh6->getFont()->setColor("Blue");
	pstylecode->getFont()->setColor("Blue");
	pstylep->getFont()->setColor("Blue");
	pstylepre->getFont()->setColor("Blue");
}

void HTMLReader::setDefaultColor()
{
	pstyle->getFont()->setColor(defaultColor);
	pstylec->getFont()->setColor(defaultColor);
	for (gtParagraphStyle* pGtStyle : listStyles)
		pGtStyle->getFont()->setColor(defaultColor);
	pstyleh1->getFont()->setColor(defaultColor);
	pstyleh2->getFont()->setColor(defaultColor);
	pstyleh3->getFont()->setColor(defaultColor);
	pstyleh4->getFont()->setColor(defaultColor);
	pstyleh5->getFont()->setColor(defaultColor);
	pstyleh6->getFont()->setColor(defaultColor);
	pstylecode->getFont()->setColor(defaultColor);
	pstylep->getFont()->setColor(defaultColor);
	pstylepre->getFont()->setColor(defaultColor);
}

void HTMLReader::setBoldFont()
{
	pstyle->getFont()->setWeight(BOLD);
	pstylec->getFont()->setWeight(BOLD);
	for (gtParagraphStyle* pGtStyle : listStyles)
		pGtStyle->getFont()->setWeight(BOLD);
	pstylecode->getFont()->setWeight(BOLD);
	pstylep->getFont()->setWeight(BOLD);
	pstylepre->getFont()->setWeight(BOLD);
}

void HTMLReader::unSetBoldFont()
{
	pstyle->getFont()->setWeight(defaultWeight);
	pstylec->getFont()->setWeight(defaultWeight);
	for (gtParagraphStyle* pGtStyle : listStyles)
		pGtStyle->getFont()->setWeight(defaultWeight);
	pstylecode->getFont()->setWeight(REGULAR);
	pstylep->getFont()->setWeight(defaultWeight);
	pstylepre->getFont()->setWeight(defaultWeight);
}

void HTMLReader::parse(const QString& filename)
{
#if defined(_WIN32)
	QString fname = QDir::toNativeSeparators(filename);
	QByteArray fn = fname.toUtf8();
#else
	QByteArray fn(filename.toLocal8Bit());
#endif
	elemJustStarted = elemJustFinished = false;
	htmlSAXParseFile(fn.data(), nullptr, mySAXHandler, nullptr);
}

void HTMLReader::createListStyle()
{
	auto* tmpStyle = new gtParagraphStyle(*listStyles[0]);
	tmpStyle->setName(QString("HTML_li_level-%1").arg(listLevel + 1));
	double indent = listStyles[0]->getIndent();
	indent += 25 * (listLevel + 1);
	tmpStyle->setIndent(indent);
	listStyles.push_back(tmpStyle);
	nextItemNumbers.push_back(1);
}

htmlSAXHandler mySAXHandlerStruct = {
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
	HTMLReader::startElement,
	HTMLReader::endElement,
	nullptr, // reference,
	HTMLReader::characters,
	nullptr, // ignorableWhitespace,
	nullptr, // processingInstruction,
	nullptr, // comment,
	nullptr, // warning,
	nullptr, // error,
	nullptr, // fatalError,
	nullptr, // getParameterEntity,
	nullptr, // cdata,
	nullptr, // externalSubset
	1, // initialized
	nullptr, // _private
	nullptr, // startElementNs
	nullptr, // endElementNs
	nullptr // serror
};

htmlSAXHandlerPtr mySAXHandler = &mySAXHandlerStruct;
