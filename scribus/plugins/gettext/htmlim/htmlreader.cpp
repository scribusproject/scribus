/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/

#include "htmlreader.h"

using namespace std;

HTMLReader::HTMLReader(gtParagraphStyle *ps, gtWriter *w)
{
	pstyle = ps;
	defaultColor = ps->getFont()->getColor();
	initPStyles();
	inH1 = false;
	inH2 = false;
	inH3 = false;
	inA = false;
	inCode = false;
	writer = w;
	href = "";
	inOL = false;
	nextItemNumber = 1;
	inUL = false;
	inLI = false;
	addedLI = false;
}

void HTMLReader::initPStyles()
{
	pstylec = new gtParagraphStyle(*pstyle);
	pstylec->setAlignment(CENTER);
	pstylec->setName("HTML_center");
	pstyleli = new gtParagraphStyle(*pstyle);
	pstyleli->setIndent(pstyleli->getIndent()+50.0);
	pstyleli->setName("HTML_li");
	pstyleh3 = new gtParagraphStyle(*pstyle);
	pstyleh3->getFont()->setSize(pstyle->getFont()->getSize() + 20);
	pstyleh3->setSpaceAbove(20.0);
	pstyleh3->setSpaceBelow(10.0);
	pstyleh3->setName("HTML_h3");
	pstyleh2 = new gtParagraphStyle(*pstyle);
	pstyleh2->getFont()->setSize(pstyle->getFont()->getSize() + 40);
	pstyleh2->setSpaceAbove(30.0);
	pstyleh2->setSpaceBelow(20.0);
	pstyleh2->setName("HTML_h2");
	pstyleh1 = new gtParagraphStyle(*pstyle);
	pstyleh1->getFont()->setSize(pstyle->getFont()->getSize() + 60);
	pstyleh2->setSpaceAbove(40.0);
	pstyleh2->setSpaceBelow(30.0);
	pstyleh1->setName("HTML_h1");
}

bool HTMLReader::startDocument()
{
	return true;
}

bool HTMLReader::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs) 
{
	if (name == "center")
		inCenter = true;
	else if (name == "br")
		writer->append("\n", pstyle);
// 	else if (name == "p")
// 		writer->append("\n", pstyle);
	else if (name == "a")
	{
		writer->append(" ", pstyle);
		toggleUnderlining();
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
	else if ((name == "b") || (name == "strong"))
		setBoldFont();

	return true;
}

bool HTMLReader::characters(const QString &ch) 
{
	QString tmp = "";
	bool add;
	for (uint i = 0; i < ch.length(); ++i)
	{
		add = false;
		if ((ch.at(i) != ' ') && (ch.at(i) != '\n') && (ch.at(i) != '\t'))
			add = true;
		if (i <= ch.length() - 2)
			if ((ch.at(i) == ' ') && ((ch.at(i + 1) != ' ') && (ch.at(i + 1) != '\t') && (ch.at(i + 1) != '\n')))
				add = true;

		if (add)
			tmp += ch.at(i);
	}
	if ((inLI) && (!addedLI))
	{
		if (inUL)
		{
			if (tmp.at(0) != '\n')
				tmp = "\n- " + tmp;
			else
				tmp = "- " + tmp;
		}
		else if (inOL)
		{
			if (tmp.at(0) != '\n')
				tmp = QString("\n%1. ").arg(nextItemNumber) + tmp;
			else
				tmp = QString("%1. ").arg(nextItemNumber) + tmp;
			++nextItemNumber;
		}
		addedLI = true;
	}
	
	if (inLI)
		writer->append(tmp, pstyleli);
	else if (inH1)
		writer->append(tmp, pstyleh1);
	else if (inH2)
		writer->append(tmp, pstyleh2);
	else if (inH3)
		writer->append(tmp, pstyleh3);
	else if (inCenter)
		writer->append(tmp, pstylec);
	else
		writer->append(tmp, pstyle);
	return true;
}

bool HTMLReader::endElement(const QString&, const QString&, const QString &name)
{
	if (name == "center")
		inCenter = false;
	if (name == "p")
		writer->append("\n");
	if (name == "a")
	{
		toggleUnderlining();
		if ((href != "") && (href.find("http") != -1))
			writer->append(" ("+href+")", pstyle);
		href = "";
		setDefaultFont();
		inA = false;
	}
	if (name == "ul")
		inUL = false;
	if (name == "ol")
	{
		inOL = false;
		nextItemNumber = 1;
	}
	if (name == "li")
	{
		inLI = false;
		addedLI = false;
		writer->append("\n", pstyleli);
	}
	if (name == "h1")
	{
		inH1 = false;
		writer->append("\n", pstyleh1);
	}
	if (name == "h2")
	{
		inH2 = false;
		writer->append("\n", pstyleh2);
	}
	if (name == "h3")
	{
		inH3 = false;
		writer->append("\n", pstyleh3);
	}
	if ((name == "b") || (name == "strong"))
		unSetBoldFont();
	return true;
}

bool HTMLReader::endDocument()
{
	writer->append("\n");
	return true;
}

void HTMLReader::toggleUnderlining()
{
	pstyle->getFont()->toggleEffect(UNDERLINE);
	pstylec->getFont()->toggleEffect(UNDERLINE);
	pstyleli->getFont()->toggleEffect(UNDERLINE);
	pstyleh1->getFont()->toggleEffect(UNDERLINE);
	pstyleh2->getFont()->toggleEffect(UNDERLINE);
	pstyleh3->getFont()->toggleEffect(UNDERLINE);
	
}

void HTMLReader::setBlueFont()
{
	pstyle->getFont()->setColor("Blue");
	pstylec->getFont()->setColor("Blue");
	pstyleli->getFont()->setColor("Blue");
	pstyleh1->getFont()->setColor("Blue");
	pstyleh2->getFont()->setColor("Blue");
	pstyleh3->getFont()->setColor("Blue");
}

void HTMLReader::setDefaultFont()
{
	pstyle->getFont()->setColor(defaultColor);
	pstylec->getFont()->setColor(defaultColor);
	pstyleli->getFont()->setColor(defaultColor);
	pstyleh1->getFont()->setColor(defaultColor);
	pstyleh2->getFont()->setColor(defaultColor);
	pstyleh3->getFont()->setColor(defaultColor);
}

void HTMLReader::setBoldFont()
{
	pstyle->getFont()->setWeight(BOLD);
	pstylec->getFont()->setWeight(BOLD);
	pstyleli->getFont()->setWeight(BOLD);
	pstyleh1->getFont()->setWeight(BOLD);
	pstyleh2->getFont()->setWeight(BOLD);
	pstyleh3->getFont()->setWeight(BOLD);
}

void HTMLReader::unSetBoldFont()
{
	pstyle->getFont()->setWeight(REGULAR);
	pstylec->getFont()->setWeight(REGULAR);
	pstyleli->getFont()->setWeight(REGULAR);
	pstyleh1->getFont()->setWeight(REGULAR);
	pstyleh2->getFont()->setWeight(REGULAR);
	pstyleh3->getFont()->setWeight(REGULAR);
}

HTMLReader::~HTMLReader()
{
	delete pstylec;
	delete pstyleli;
	delete pstyleh1;
	delete pstyleh2;
	delete pstyleh3;
}
