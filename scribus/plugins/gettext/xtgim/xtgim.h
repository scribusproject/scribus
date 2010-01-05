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

#ifndef XTGIM_H
#define XTGIM_H

#include "pluginapi.h"
#include "pageitem.h"

#include <QByteArray>
#include <QList>
#include <QString>
#include "textwriter.h"

class StyleManager;

extern "C" PLUGIN_API void GetText2(QString filename, QString encoding, bool textOnly, PageItem *textItem);
extern "C" PLUGIN_API QString FileFormatName();
extern "C" PLUGIN_API QStringList FileExtensions();

class XtgIm
{
private:
	QString encoding;
	QString filename;
	QString in_Buffer;
	QByteArray buffer;
	PageItem *textFrame;
	TextWriter *writer;
	void loadFiletoArray();
	QString toUnicode(const QByteArray& rawText);
	StyleManager *StyleMan;
public:
	XtgIm(QString fileName,QString enc,PageItem *textItem,bool textOnly);
	~XtgIm();
};
#endif			/* XTGIM_H */