/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Okt 03 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "rtfim.h"

#include <QByteArray>
#include <QBuffer>
#include <QFile>
#include "scribusdoc.h"
#include "third_party/rtf-qt/rtfreader.h"
#include "third_party/rtf-qt/sladocumentrtfoutput.h"


QString FileFormatName()
{
    return QObject::tr("RTF Document");
}

QStringList FileExtensions()
{
	return QStringList("rtf");
}

void GetText2(QString filename, QString encoding, bool textOnly, bool prefix, bool append, PageItem *textItem)
{
	QFile f(filename);
	if (f.open(QIODevice::ReadOnly))
	{
		QByteArray daten = f.readAll();
		f.close();
		QBuffer buffer(&daten);
		buffer.open(QIODevice::ReadOnly);
		RtfReader::SlaDocumentRtfOutput *output = new RtfReader::SlaDocumentRtfOutput(textItem, textItem->doc(), prefix);
		RtfReader::Reader reader;
		if (!append)
		{
			QString pStyleD = CommonStrings::DefaultParagraphStyle;
			ParagraphStyle newStyle;
			newStyle.setDefaultStyle(false);
			newStyle.setParent(pStyleD);
			textItem->itemText.clear();
			textItem->itemText.setDefaultStyle(newStyle);
		}
		reader.parseFromDeviceTo(&buffer, output);
		textItem->itemText.trim();
		textItem->itemText.invalidateLayout();
		delete output;
	}
}
