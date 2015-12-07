/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QObject>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QTextCodec> 

#include "txtim.h"
#include "scribusstructs.h"
#include "util.h"

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
	m_filename = fname;
	m_encoding = enc;
	m_writer = w;
	m_textOnly = textO;
	loadText();
}

void TxtIm::write()
{
	m_writer->appendUnstyled(m_text);
}

void TxtIm::loadText()
{
	QByteArray rawText;
	if (loadRawText(m_filename, rawText))
		m_text = toUnicode(rawText);
}

QString TxtIm::toUnicode(const QByteArray& rawText)
{
	QTextCodec *codec;
	if (m_encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(m_encoding.toLocal8Bit());
	QString unistr = codec->toUnicode(rawText);
	return unistr;
}

TxtIm::~TxtIm()
{

}
