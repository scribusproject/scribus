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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "htmlim.h"
//#include "htmlim.moc"
#include <QObject>
#include <QString>
#include <QStringList>

#ifndef HAVE_XML
#error The htmlim plugin requires libxml to build
#endif

#include "scribusstructs.h"
#include "gtparagraphstyle.h" // Style for paragraph based formatting.
#include "gtframestyle.h"

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
	HTMLIm* him = new HTMLIm(filename, encoding, writer, textOnly);
	delete him;
}

/******** Class HTMLIm ************************************/

HTMLIm::HTMLIm(QString fname, QString coding, gtWriter *w, bool textOnly)
{
	m_filename = fname;
	m_encoding = coding;
	m_writer = w;
	gtFrameStyle *fstyle = m_writer->getDefaultStyle();
	m_pstyle = new gtParagraphStyle(*fstyle);
	m_pstyle->setName("HTML_default");
// 	defaultFontSize = pstyle->getFont()->getSize();
	importText(textOnly);
	delete m_pstyle;
}

void HTMLIm::importText(bool textOnly)
{
	HTMLReader* handler = new HTMLReader(m_pstyle, m_writer, textOnly);
	handler->parse(m_filename);
	delete handler;
}

HTMLIm::~HTMLIm()
{

}

