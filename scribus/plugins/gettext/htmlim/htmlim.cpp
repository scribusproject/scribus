#include "htmlim.h"
#include <qstring.h>
#include <qstringlist.h>

#ifdef HAVE_XML

#include <gtparagraphstyle.h> // Style for paragraph based formatting.
#include <gtframestyle.h>

QString FileFormatName()
{
    return QObject::tr("HTML Files");
}

QStringList FileExtensions()
{
    QStringList endings(QObject::tr("html"));
	endings << QString("htm");
	return endings;
}

void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer)
{
	HTMLIm* him = new HTMLIm(filename, writer);
	delete him;
}

/******** Class HTMLIm ************************************/

HTMLIm::HTMLIm(QString fname, gtWriter *w)
{
	filename = fname;
	writer = w;
	gtFrameStyle *fstyle = writer->getDefaultStyle();
	pstyle = new gtParagraphStyle(*fstyle);
	pstyle->setName("HTML_default");
	defaultFontSize = pstyle->getFont()->getSize();
	importText();
	delete pstyle;
}

void HTMLIm::importText()
{
	HTMLReader* handler = new HTMLReader(pstyle, writer);
	handler->parse(filename);
	delete handler;
}

HTMLIm::~HTMLIm()
{

}

#endif // HAVE_XML
