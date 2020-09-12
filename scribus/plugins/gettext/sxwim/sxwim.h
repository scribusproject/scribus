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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#ifndef SXWIM_H
#define SXWIM_H

#include <QString>
#include <gtwriter.h>
#include "scconfig.h"
#include "pluginapi.h"

extern "C" PLUGIN_API void GetText(const QString& filename, const QString& encoding, bool textOnly, gtWriter *writer);

extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();

const QString STYLE   = "styles.xml";
const QString CONTENT = "content.xml";

class SxwIm
{
public:
	SxwIm(const QString& fileName, const QString& encoding, gtWriter* w, bool textOnly);
	~SxwIm();

private:
	gtWriter* writer { nullptr };
	QString encoding;
	QString filename;
	QString stylePath;
	QString contentPath;

	bool decompress();
};

#endif // SXWIM_H
