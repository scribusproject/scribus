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

#ifndef SXWIM_H
#define SXWIM_H

#include <qstring.h>
#include <gtwriter.h>
#include "config.h"

extern "C" void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" QString FileFormatName();

extern "C" QStringList FileExtensions();

#ifdef HAVE_XML

const QString STYLE   = "styles.xml";
const QString CONTENT = "content.xml";

class SxwIm
{
private:
	gtWriter* writer;
	QString encoding;
	QString filename;
	QString stylePath;
	QString contentPath;
	bool decompress();
public:
	SxwIm(QString fileName, QString encoding, gtWriter* w, bool textOnly);
	~SxwIm();
};

#endif // HAVE_XML

#endif // SXWIM_H
