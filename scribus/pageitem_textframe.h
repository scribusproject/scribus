/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem.h  -  description
                             -------------------
    copyright            : Scribus Team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PAGEITEMTEXTFRAME_H
#define PAGEITEMTEXTFRAME_H

#include <QMap>
#include <QRectF>
#include <QString>
#include <QKeyEvent>

#include "scribusapi.h"
#include "pageitem.h"

class ScPainter;
class ScribusDoc;

#include "text/nlsconfig.h"

#ifdef NLS_PROTO
#include "text/sctextengine.h"
#include "text/scfont.h"
#include "sctextstruct.h"
class ScText;
class ScStyleRun;
#endif

class SCRIBUS_API PageItem_TextFrame : public PageItem
{
	Q_OBJECT

public:
	PageItem_TextFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
	PageItem_TextFrame(const PageItem & p);
	~PageItem_TextFrame() {};

	virtual PageItem_TextFrame * asTextFrame() { return this; }
	virtual bool isTextFrame() const { return true; }
	
	virtual void clearContents();
	
	/**
	* \brief Handle keyboard interaction with the text frame while in edit mode
	* @param k key event
	* @param keyRepeat a reference to the keyRepeat property
	*/
	virtual void handleModeEditKey(QKeyEvent *k, bool& keyRepeat);
	void deleteSelectedTextFromFrame();
	void setNewPos(int oldPos, int len, int dir);
	void ExpandSel(int dir, int oldPos);
	void deselectAll();
	
	virtual void invalidateLayout();
	virtual void layout();

	double columnWidth();
#ifdef NLS_PROTO
	int firstTextItem() const { return itemText.firstFrameItem; }
	int lastTextItem() const { return itemText.lastFrameItem; }
#endif
	QRegion availableRegion() { return m_availableRegion; }
protected:
	QRegion calcAvailableRegion();
	QRegion m_availableRegion;
	virtual void DrawObj_Item(ScPainter *p, QRectF e);
	virtual void DrawObj_Post(ScPainter *p);
	virtual void DrawObj_Decoration(ScPainter *p);
	//void drawOverflowMarker(ScPainter *p);
	void drawUnderflowMarker(ScPainter *p);
	void drawColumnBorders(ScPainter *p);

#ifdef NLS_PROTO
	void DrawLineItem(ScPainter *p, double width,
	                  ScScriptItem *item, uint itemIndex);
#endif
	
	bool unicodeTextEditMode;
	int unicodeInputCount;
	QString unicodeInputString;
	
	virtual bool createInfoGroup(QFrame *, QGridLayout *);
	virtual void applicableActions(QStringList& actionList);
	virtual QString infoDescription();
	// Move incomplete lines from the previous frame if needed.
	bool moveLinesFromPreviousFrame ();
	void adjustParagraphEndings ();

private:
	bool cursorBiasBackward;
	// If the last paragraph had to be split, this is how many lines of the paragraph are in this frame.
	// Used for orphan/widow control
	int incompleteLines;
	// This holds the line splitting positions
	QList<int> incompletePositions;

	void setShadow();
	QString currentShadow;
	QMap<QString,StoryText> shadows;
	bool checkKeyIsShortcut(QKeyEvent *k);
	
	// set text frame height to last line of text
	double maxY;
	void setMaxY(double y);

public:
	void setTextFrameHeight();

private slots:
	void slotInvalidateLayout();
};

#endif
