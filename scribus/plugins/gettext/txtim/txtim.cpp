/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qobject.h>
#include <qcstring.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextcodec.h> 

#include "txtim.h"
#include "scribusstructs.h"

QString FileFormatName()
{
    return QObject::tr("Text Files");
}

QStringList FileExtensions()
{
    return QStringList("txt");
}

void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer)
{
	TxtIm* tim = new TxtIm(filename, encoding, textOnly, writer);
	tim->write();
	delete tim;
}

TxtIm::TxtIm(const QString& fname, const QString& enc, bool textO, gtWriter *w)
{
	filename = fname;
	encoding = enc;
	writer = w;
	textOnly = textO;
	loadText();
	toUnicode();
}

void TxtIm::write()
{
	writer->append(text);
}

void TxtIm::loadText()
{
	text = "";
	QFile f(filename);
	QFileInfo fi(f);
	if (!fi.exists())
		return;
	uint posi;
	QByteArray bb(f.size());
	if (f.open(IO_ReadOnly))
	{
		f.readBlock(bb.data(), f.size());
		f.close();
		for (posi = 0; posi < bb.size(); ++posi)
			text += QChar(bb[posi]);
	}
}

void TxtIm::toUnicode()
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding);
	QString dec = codec->toUnicode( text );
	text = dec;
}

TxtIm::~TxtIm()
{

}
