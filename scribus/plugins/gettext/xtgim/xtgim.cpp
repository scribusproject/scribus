/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 /***************************************************************************
  *   Copyright (C) 2009 by Jain Basil Aliyas                               *
  *   mail@jainbasil.net                                                    *
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

#include "xtgim.h"
#include "xtgscanner.h"
#include "xtgdialog.h"

#include <QFile>
#include <QFileInfo>
#include <QTextCodec>

#include "prefsmanager.h"
#include "util.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "styles/paragraphstyle.h"
#include "styles/charstyle.h"


QString FileFormatName()
{
    return QObject::tr("Quark XPress Tags Document");
}

QStringList FileExtensions()
{
	return QStringList("xtg");
}

void GetText2(QString filename, QString encoding, bool textOnly, PageItem *textItem)
{
	XtgIm* xtgim = new XtgIm(filename, encoding, textItem, textOnly);
	delete xtgim;
}

/********************************Class XtgIm***********************************/

XtgIm::XtgIm(QString fileName, QString enc, PageItem *textItem, bool textOnly)
{
	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("XtgIm");
	bool prefix = prefs->getBool("prefix", true);
	bool ask = prefs->getBool("askAgain", true);
	encoding = enc;
	qDebug()<<"Encoding = "<<encoding;
	in_Buffer = "";
	TextWriter *writer = new TextWriter(textItem->doc());
	writer->open(textItem);
	if (!textOnly)
	{
		if (ask)
		{
			XtgDialog* xtgdia = new XtgDialog(prefix);
			if (xtgdia->exec()) {
				prefix = xtgdia->usePrefix();
				prefs->set("prefix", xtgdia->usePrefix());
				prefs->set("askAgain", xtgdia->askAgain());
				delete xtgdia;
			} 
			else {
				delete xtgdia;
				return;
			}
		}
	}
	filename = fileName;
	/**
	This will load the xtg file into a QByte Array which will be used for 
	furthur parsing
	*/
	loadFiletoArray();
	in_Buffer = toUnicode(buffer);
	QString docname = filename.right(filename.length() - filename.lastIndexOf("/") - 1);
	docname = docname.left(docname.lastIndexOf("."));
	XtgScanner *scanner = new XtgScanner(docname,writer,in_Buffer,textOnly,prefix);
	scanner->xtgParse();
	writer->close();
}

/** \brief Load the file into a buffer of type QByteArray */

void XtgIm::loadFiletoArray()
{
	loadRawBytes(filename, buffer);
}

QString XtgIm::toUnicode(const QByteArray& rawText)
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	QString unistr = codec->toUnicode(rawText);
	return unistr;
}

XtgIm::~XtgIm()
{
}
