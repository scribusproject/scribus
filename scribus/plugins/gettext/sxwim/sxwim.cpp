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

#include "sxwim.h"
#include <qstringlist.h>

#ifdef HAVE_XML

#include <prefsfile.h>
#include <prefscontext.h>
#include <prefstable.h>
#include "sxwunzip.h"
#include "stylereader.h"
#include "contentreader.h"
#include "sxwdia.h"

extern PrefsFile* prefsFile;

QString FileFormatName()
{
    return QObject::tr("OpenOffice.org Writer Documents");
}

QStringList FileExtensions()
{
	return QStringList("sxw");
}

void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer)
{
	SxwIm* sim = new SxwIm(filename, encoding, writer, textOnly);
	delete sim;
}

/********** Class SxwIm ************************************************************/

SxwIm::SxwIm(QString fileName, QString enc, gtWriter* w, bool textOnly)
{
	PrefsContext* prefs = prefsFile->getPluginContext("SxwIm");
	bool update = prefs->getBool("update", true);
	bool prefix = prefs->getBool("prefix", true);
	bool ask = prefs->getBool("askAgain", true);
	bool pack = prefs->getBool("pack", true);
	encoding = enc;
	if (!textOnly)
	{
		if (ask)
		{
			SxwDialog* sxwdia = new SxwDialog(update, prefix, pack);
			sxwdia->exec();
			update = sxwdia->shouldUpdate();
			prefix = sxwdia->usePrefix();
			pack = sxwdia->packStyles();
			prefs->set("update", update);
			prefs->set("prefix", sxwdia->usePrefix());
			prefs->set("askAgain", sxwdia->askAgain());
			prefs->set("pack", sxwdia->packStyles());
			delete sxwdia;
		}
	}
	filename = fileName;
	writer = w;
	writer->setUpdateParagraphStyles(update);
	SxwUnzip* sun = new SxwUnzip(fileName);
	stylePath   = sun->getFile(STYLE);
	contentPath = sun->getFile(CONTENT);
	delete sun;
	if ((stylePath != NULL) && (contentPath != NULL))
	{
		QString docname = filename.right(filename.length() - filename.findRev("/") - 1);
		docname = docname.left(docname.findRev("."));
		StyleReader *sreader = new StyleReader(docname, writer, textOnly, prefix, pack);
		sreader->parse(stylePath);
		ContentReader *creader = new ContentReader(docname, sreader, writer, textOnly);
		creader->parse(contentPath);
		delete sreader;
		delete creader;
		QFile f1(stylePath);
		f1.remove();
		QFile f2(contentPath);
		f2.remove();
	}
	else if ((stylePath == NULL) && (contentPath != NULL))
	{
		QFile f2(contentPath);
		f2.remove();
	}
	else if ((stylePath != NULL) && (contentPath == NULL))
	{
		QFile f1(stylePath);
		f1.remove();
	}
}

SxwIm::~SxwIm()
{

}

#endif
