/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@welho.com                                                      *
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

#include <qcolor.h>
#include <qtextcodec.h>
#include <qvaluelist.h>
#include <qmap.h>
#include <qstring.h>
#include "pageitem.h"
#include "gtfont.h"
#include "gtframestyle.h"
#include "gtparagraphstyle.h"
#include "gtstyle.h"

typedef QMap<QString, QString> FontFamilyMap;

class gtAction
{
private:
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
	QString validateFont(gtFont* font);
	QString findFontName(gtFont* font);
	void updateParagraphStyle(int pstyleIndex, gtParagraphStyle* pstyle);
	QString parseColor(const QString &s);
	QColor parseColorN(const QString &rgbColor);
	void finalize();
public:
	gtAction(bool append);
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
	void applyFrameStyle(gtFrameStyle* fstyle);
	void createParagraphStyle(gtParagraphStyle* pstyle);
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
