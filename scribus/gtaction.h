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

#include <qtextcodec.h>
#include <qvaluelist.h>
#include "scribus.h"
#include "scribusdoc.h"
#include "missing.h"
#include "mpalette.h"
#include "gtfont.h"
#include "gtframestyle.h"
#include "gtparagraphstyle.h"
#include "gtstyle.h"

class gtAction
{
private:
	PageItem *textFrame;
	PageItem *it;
	int lastParagraphStyle;
	bool lastCharWasLineChange;
	QString currentFrameStyle;
	int findParagraphStyle(gtParagraphStyle* pstyle);
	int applyParagraphStyle(gtParagraphStyle* pstyle);
	void applyFrameStyle(gtFrameStyle* fstyle);
	QString validateFont(gtFont* font);
	void finalize();
public:
	gtAction(bool append);
	~gtAction();
	void setTextFrame(PageItem* frame);
	void setProgressInfo();
	void setProgressInfoDone();
	void setInfo(QString infoText);
	void clearFrame();
	void getFrameFont(gtFont *font);
	void getFrameStyle(gtFrameStyle *fstyle);
	void write(QString text, gtStyle *style);
	void createParagraphStyle(gtParagraphStyle* pstyle);
	double getFrameWidth();
	QString getFrameName();
};

#endif
