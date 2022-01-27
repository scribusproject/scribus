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

#ifndef GTACTION_H
#define GTACTION_H

#include <QColor>
#include <QTextCodec>
#include <QMap>
#include <QString>

#include "scribusapi.h"

class PageItem;
class PrefsManager;
class ScFace;
class CharStyle;
class ParagraphStyle;
class ScribusDoc;
class ScribusMainWindow;
class StoryText;
class TextNote;

class UndoManager;

class gtStyle;
class gtParagraphStyle;
class gtFont;
class gtFrameStyle;

typedef QMap<QString, QString> FontFamilyMap;

class SCRIBUS_API gtAction
{
public:
	gtAction(bool append, PageItem *pageitem);
	~gtAction();

	void setProgressInfo();
	void setProgressInfoDone();
	void setInfo(const QString& infoText);
	double getLineSpacing(int fontSize);
	void clearFrame();
	void getFrameFont(gtFont *font);
	void getFrameStyle(gtFrameStyle *fstyle);
	void write(const QString& text, gtStyle *style, bool isNote);
	void writeUnstyled(const QString& text, bool isNote);
	void applyFrameStyle(gtFrameStyle* fstyle);
	void createParagraphStyle(gtParagraphStyle* pstyle);
	void setCharStyleAttributes(gtFont *font, CharStyle& style);
	void setParaStyleAttributes(gtParagraphStyle *pstyle, ParagraphStyle& style);
	void updateParagraphStyle(const QString& pstyleName, gtParagraphStyle* pstyle);
	void removeParagraphStyle(const QString& name);
	void removeParagraphStyle(int index);
	double getFrameWidth();
	QString getFrameName();
	bool getUpdateParagraphStyles();
	void setUpdateParagraphStyles(bool newUPS);
	bool getOverridePStyleFont();
	void setOverridePStyleFont(bool newOPSF);

private:
	PrefsManager& m_prefsManager;
	UndoManager* m_undoManager { nullptr };
	ScribusMainWindow* m_ScMW { nullptr };
	PageItem *m_textFrame { nullptr };
	PageItem *m_it { nullptr };
	int m_lastParagraphStyle { -1 };
	bool m_inPara { false };
	bool m_isFirstWrite { true };
	bool m_doAppend;
	bool m_lastCharWasLineChange { false };
	bool m_updateParagraphStyles { false };
	/* If paragraph style is used should the font style of the gtpstyle be used 
	   or should writer respect the font set in the real paragraph style
	*/
	bool m_overridePStyleFont { true };
	QString m_currentFrameStyle;
	FontFamilyMap m_families;

	StoryText* m_noteStory { nullptr };
	TextNote* m_note { nullptr };

	int findParagraphStyle(const QString& name);
	int findParagraphStyle(gtParagraphStyle* pstyle);
	int applyParagraphStyle(gtParagraphStyle* pstyle);

	ScFace  validateFont(gtFont* font);
	QString findFontName(gtFont* font);
	void    updateParagraphStyle(int pstyleIndex, gtParagraphStyle* pstyle);
	QString parseColor(const QString &s);
	void    finalize();
};

#endif
