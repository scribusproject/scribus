/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/

#include "htmlreader.h"

#ifdef HAVE_XML

#include <gtmeasure.h>

HTMLReader* HTMLReader::hreader = NULL;

extern htmlSAXHandlerPtr mySAXHandler;

HTMLReader::HTMLReader(gtParagraphStyle *ps, gtWriter *w)
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
	inOL = false;
	nextItemNumber = 1;
	inUL = false;
	inLI = false;
	addedLI = false;
	lastCharWasSpace = false;
	hreader = this;
}

void HTMLReader::initPStyles()
{
	pstylec = new gtParagraphStyle(*pstyle);
	pstylec->setAlignment(CENTER);
	pstylec->setName("HTML_center");
	pstyleli = new gtParagraphStyle(*pstyle);
	pstyleli->setIndent(pstyleli->getIndent()+50.0);
	pstyleli->setName("HTML_li");
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
	pstylep->setSpaceBelow(gtMeasure::i2d(5, MM));
	pstylep->setName("HTML_p");
	pstylepre = new gtParagraphStyle(*pstyle);
	pstylepre->setName("HTML_pre");
}

void HTMLReader::startElement(void *user_data, const xmlChar * fullname, const xmlChar ** atts)
{
	QString* name = new QString((const char*) fullname);
	name = new QString(name->lower());
	QXmlAttributes* attrs = new QXmlAttributes();
	if (atts)
	{
		for(const xmlChar** cur = atts; cur && *cur; cur += 2)
			attrs->append(QString((char*)*cur), NULL, QString((char*)*cur), QString((char*)*(cur + 1)));
	}
	hreader->startElement(NULL, NULL, *name, *attrs);
	
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
		inUL = true;
	else if (name == "ol")
		inOL = true;
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
				if (attrs.value(i) != "")
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
	else if ((name == "ins") && (!inA))
		toggleEffect(UNDERLINE);
	return true;
}
void HTMLReader::characters(void *user_data, const xmlChar * ch, int len)
{
	QString chars = QString::fromUtf8((const char*) ch, len);
	hreader->characters(chars);
}

bool HTMLReader::characters(const QString &ch) 
{
	if (inBody)
	{
		QString tmp = "";
		bool add = true;
		bool fcis = ch.left(1) == " ";
		bool lcis = ch.right(1) == " ";
		if (inPre)
			tmp = ch;
		else
		{
			for (uint i = 0; i < ch.length(); ++i)
			{
				if (ch.at(i) != "\n")
					tmp += ch.at(i);
			}
			tmp = tmp.simplifyWhiteSpace();
		}
		if (!lastCharWasSpace)
			if (fcis)
				tmp = " " + tmp;
		
		if (lcis)
			tmp = tmp + " ";
		bool lastCharWasSpace = lcis;
		if ((inLI) && (!addedLI))
		{
			if (inUL)
				tmp = "- " + tmp;
			else if (inOL)
			{
				tmp = QString("%1. ").arg(nextItemNumber) + tmp;
				++nextItemNumber;
			}
			addedLI = true;
		}

		if (inP)
			writer->append(tmp, pstylep);
		else if (inLI)
			writer->append(tmp, pstyleli);
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

void HTMLReader::endElement(void *user_data, const xmlChar * name)
{
	QString *nname = new QString((const char*) name);
	nname = new QString(nname->lower());
	hreader->endElement(NULL, NULL, *nname);
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
		if ((href != "") || (href.find("//") != -1) || (href.find("mailto:") != -1))
		{
			href = href.remove("mailto:");
			writer->append(" ("+href+")", pstyle);
		}
		href = "";
		setDefaultColor();
		inA = false;
	}
	else if (name == "ul")
	{
		inUL = false;
		writer->append("\n");
	}
	else if (name == "ol")
	{
		inOL = false;
		nextItemNumber = 1;
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
	else if ((name == "ins") && (!inA))
		toggleEffect(UNDERLINE);
	return true;
}

void HTMLReader::toggleEffect(FontEffect e)
{
	pstyle->getFont()->toggleEffect(e);
	pstylec->getFont()->toggleEffect(e);
	pstyleli->getFont()->toggleEffect(e);
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
	pstyleli->getFont()->setSlant(ITALIC);
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
	pstyleli->getFont()->setSlant(defaultSlant);
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
	pstyleli->getFont()->setColor("Blue");
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
	pstyleli->getFont()->setColor(defaultColor);
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
	pstyleli->getFont()->setWeight(BOLD);
	pstylecode->getFont()->setWeight(BOLD);
	pstylep->getFont()->setWeight(BOLD);
	pstylepre->getFont()->setWeight(BOLD);
}

void HTMLReader::unSetBoldFont()
{
	pstyle->getFont()->setWeight(defaultWeight);
	pstylec->getFont()->setWeight(defaultWeight);
	pstyleli->getFont()->setWeight(defaultWeight);
	pstylecode->getFont()->setWeight(REGULAR);
	pstylep->getFont()->setWeight(defaultWeight);
	pstylepre->getFont()->setWeight(defaultWeight);
}

void HTMLReader::parse(QString filename)
{
// 	xmlSAXUserParseFile(debugSAXHandler, NULL, filename.latin1());
	htmlSAXParseFile(filename.latin1(), NULL, mySAXHandler, NULL); 
}

htmlSAXHandler mySAXHandlerStruct = {
	NULL, // internalSubsetDebug,
	NULL, // isStandaloneDebug,
	NULL, // hasInternalSubsetDebug,
	NULL, // hasExternalSubsetDebug,
	NULL, // resolveEntityDebug,
	NULL, // getEntityDebug,
	NULL, // entityDeclDebug,
	NULL, // notationDeclDebug,
	NULL, // attributeDeclDebug,
	NULL, // elementDeclDebug,
	NULL, // unparsedEntityDeclDebug,
	NULL, // setDocumentLocatorDebug,
	NULL, // startDocument,
	NULL, // endDocument,
	HTMLReader::startElement,
	HTMLReader::endElement,
	NULL, // referenceDebug,
	HTMLReader::characters,
	NULL, // ignorableWhitespaceDebug,
	NULL, // processingInstructionDebug,
	NULL, // commentDebug,
	NULL, // warningDebug,
	NULL, // errorDebug,
	NULL, // fatalErrorDebug,
	NULL, // getParameterEntityDebug,
	NULL, // cdataDebug,
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
	delete pstylec;
	delete pstyleli;
	delete pstyleh1;
	delete pstyleh2;
	delete pstyleh3;
	delete pstylecode;
	delete pstylep;
	delete pstylepre;
	hreader = NULL;
}

#endif // HAVE_XML
