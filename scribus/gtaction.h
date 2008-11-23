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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef GTACTION_H
#define GTACTION_H

#include <QColor>
#include <QTextCodec>
#include <QMap>
#include <QString>

class PageItem;
class PrefsManager;
class ScFace;

#include "gtfont.h"
#include "gtframestyle.h"
#include "gtparagraphstyle.h"
#include "gtstyle.h"
#include "scribusapi.h"

class CharStyle;
class ParagraphStyle;
class ScribusDoc;
class ScribusMainWindow;

typedef QMap<QString, QString> FontFamilyMap;

class SCRIBUS_API gtAction
{
private:
	ScribusMainWindow* m_ScMW;
	PageItem *textFrame;
	PageItem *it;
	int lastParagraphStyle;
	bool inPara;
	bool isFirstWrite;
	bool doAppend;
	bool lastCharWasLineChange;
	bool updateParagraphStyles;
	/* If paragraph style is used should the font style of the gtpstyle be used 
	   or should writer respect the font set in the real paragraph style
	*/
	bool overridePStyleFont;
	QString currentFrameStyle;
	FontFamilyMap families;

	int findParagraphStyle(const QString& name);
	int findParagraphStyle(gtParagraphStyle* pstyle);
	int applyParagraphStyle(gtParagraphStyle* pstyle);

	ScFace  validateFont(gtFont* font);
	QString findFontName(gtFont* font);
	void    updateParagraphStyle(int pstyleIndex, gtParagraphStyle* pstyle);
	QString parseColor(const QString &s);
	QColor  parseColorN(const QString &rgbColor);
	void finalize();
	PrefsManager *prefsManager;
public:
//	gtAction(bool append);
	gtAction(bool append, PageItem *pageitem);
	~gtAction();
	void setTextFrame(PageItem* frame);
	void setProgressInfo();
	void setProgressInfoDone();
	void setInfo(QString infoText);
	double getLineSpacing(int fontSize);
	void clearFrame();
	void getFrameFont(gtFont *font);
	void getFrameStyle(gtFrameStyle *fstyle);
	void write(const QString& text, gtStyle *style);
	void writeUnstyled(const QString& text);
	void applyFrameStyle(gtFrameStyle* fstyle);
	void createParagraphStyle(gtParagraphStyle* pstyle);
	void setCharStyleAttributes(gtFont *font, CharStyle& style);
	void setParaStyleAttributes(gtParagraphStyle *gtstyle, ParagraphStyle& style);
	void updateParagraphStyle(const QString& pstyleName, gtParagraphStyle* pstyle);
	void removeParagraphStyle(const QString& name);
	void removeParagraphStyle(int index);
	double getFrameWidth();
	QString getFrameName();
	bool getUpdateParagraphStyles();
	void setUpdateParagraphStyles(bool newUPS);
	bool getOverridePStyleFont();
	void setOverridePStyleFont(bool newOPSF);
};

#endif
