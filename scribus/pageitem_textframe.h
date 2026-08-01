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

#include <QHash>
#include <QRectF>
#include <QString>
#include <QKeyEvent>

#include "scribusapi.h"
#include "pageitem.h"
#include "marks.h"
#include "notesstyles.h"
#include "textframespellchecker.h"

class PageItem_NoteFrame;
class ScPainter;
class ScribusDoc;

typedef QHash<PageItem_NoteFrame*, QList<TextNote *> > NotesInFrameMap;


//cezaryece: I remove static statement and made it public as this function is used also by PageItem_NoteFrame
double calculateLineSpacing (const ParagraphStyle &style, PageItem *item);

class SCRIBUS_API PageItem_TextFrame : public PageItem
{
	Q_OBJECT

public:
	PageItem_TextFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	PageItem_TextFrame(const PageItem & p);
	~PageItem_TextFrame();

	void init();

	PageItem_TextFrame * asTextFrame() override { return this; }
	const PageItem_TextFrame * asTextFrame() const override { return this; }
	bool isTextFrame() const override { return true; }
	bool isTextContainer() const override { return true; }
	bool isTableCellTextFrame() const { return m_isTableCellTextFrame; }
	void setIsTableCellTextFrame(bool isTableCellTextFrame){ m_isTableCellTextFrame = isTableCellTextFrame; }
	/**
	 * Sets the vertical alignment used when laying this frame out, without
	 * recording an undo action.
	 *
	 * For a table cell text frame the alignment is owned by the cell style;
	 * the value held here is a cache refreshed on every relayout rather than a
	 * user edit of the frame, so it must not reach the undo stack. The style or
	 * direct formatting change that caused it carries its own undo entry.
	 * layout() already writes the member directly for the same reason when it
	 * temporarily suppresses alignment on a non-rectangular frame.
	 *
	 * Use PageItem::setVerticalAlignment() for user edits of an ordinary text
	 * frame; on a cell text frame that value would be overwritten by the next
	 * TableCell::updateContent().
	 */
	void setDerivedVerticalAlignment(int val);

	void clearContents() override;
	void truncateContents() override;

	/**
	* \brief Handle keyboard interaction with the text frame while in edit mode
	* @param k key event
	* @param keyRepeat a reference to the keyRepeat property
	*/
	void handleModeEditKey(QKeyEvent *k, bool& keyRepeat) override;
	/**
	 * Returns true if the text cursor is on the first visual line of the
	 * laid-out text. An empty frame (no lines) is treated as on the first
	 * line. Used by table-cell navigation to decide whether Up should
	 * escape to the cell above.
	 */
	bool cursorOnFirstLine() const;
	/**
	 * Returns true if the text cursor is on the last visual line of the
	 * laid-out text. An empty frame (no lines) is treated as on the last
	 * line. Used by table-cell navigation to decide whether Down should
	 * escape to the cell below.
	 */
	bool cursorOnLastLine() const;
	void deleteSelectedTextFromFrame();
	void ExpandSel(int oldPos);
	void deselectAll();

	//for speed up updates when changed was only one frame from chain
	virtual void invalidateLayout(bool wholeChain);
	virtual void invalidateLayout(int firstChar);
	using PageItem::invalidateLayout;
	void layout() override;

	//return true if all previous frames from chain are valid (including that one)
	bool isValidChainFromBegin();
	void setTextAnnotationOpen(bool open);

	double columnWidth();

	//enable/disable marks inserting actions depending on editMode
	void toggleEditModeActions();
	QRegion availableRegion() { return m_availableRegion; }
	int textPositionFromPoint(const QPointF& canvasPoint);

	void replaceSpellingErrorText(const SpellError& error, const QString& suggestion);


protected:
	QRegion calcAvailableRegion();
	QRegion m_availableRegion;
	void DrawObj_Item(ScPainter *p, const QRectF& e) override;
	void DrawObj_Post(ScPainter *p) override;
	void DrawObj_Decoration(ScPainter *p) override;
	//void drawOverflowMarker(ScPainter *p);
	void drawUnderflowMarker(ScPainter *p);
	void drawColumnBorders(ScPainter *p);
	void drawSpellCheckSquiggles(ScPainter* p, const QVector<SpellError>& errors);
	void drawSquiggleLine(ScPainter* p, double x, double y, double width, double fontSize);

	bool unicodeTextEditMode {false};
	int unicodeInputCount {0};
	QString unicodeInputString;

	void drawNoteIcon(ScPainter *p);
	bool createInfoGroup(QFrame *, QGridLayout *) override;
	void applicableActions(QStringList& actionList) override;
	QString infoDescription() const override;
	// Move incomplete lines from the previous frame if needed.
	bool moveLinesFromPreviousFrame ();
	void adjustParagraphEndings ();

private:
	bool cursorBiasBackward {false};
	// If the last paragraph had to be split, this is how many lines of the paragraph are in this frame.
	// Used for orphan/widow control
	int incompleteLines {0};
	// This holds the line splitting positions
	QList<int> incompletePositions;

	void setShadow();
	QString m_currentShadow;
	QMap<QString,StoryText> m_shadows;
	bool checkKeyIsShortcut(QKeyEvent *k);
	QRectF m_origAnnotPos;
	void updateBulletsNum();
	bool m_isTableCellTextFrame { false };

private slots:
	void slotInvalidateLayout(int firstItem, int endItem);
	void slotSpellCheckTextChanged(int firstItem, int endItem);

public:
	//for footnotes/endnotes
	bool hasNoteMark(NotesStyle* NS = nullptr);
	bool hasNoteFrame(NotesStyle* NS, bool inChain = false);
	//bool hasNoteFrame(PageItem_NoteFrame* nF) { return m_notesFramesMap.contains(nF); }
	void delAllNoteFrames(bool doUpdate = false);
	void removeNoteFrame(PageItem_NoteFrame* nF) { m_notesFramesMap.remove(nF); }
	//layout notes frames /updates endnotes frames content if m_Doc->flag_updateEndNotes is set/
	void notesFramesLayout();
	//removing all marsk from text, returns number of removed marks
	int removeMarksFromText(bool doUndo);
	//return note frame for given notes style if current text frame has notes marks with this style
	PageItem_NoteFrame* itemNoteFrame(NotesStyle* nStyle);
	//list all notes frames for current text frame /with endnotes frames if endnotes marks are in that frame/
	QList<PageItem_NoteFrame*> notesFramesList() { return m_notesFramesMap.keys(); }
	//list of notes inserted by current text frame into given noteframe
	QList<TextNote*> notesList(PageItem_NoteFrame* nF) { return m_notesFramesMap.value(nF); }
	//insert note frame to list with empty notes list
	void setNoteFrame(PageItem_NoteFrame* nF);
	void invalidateNotesFrames();

private:
	NotesInFrameMap m_notesFramesMap;
	NotesInFrameMap updateNotesFrames(QMap<int, Mark*> noteMarksPosMap); //update notes frames content
	void updateNotesMarks(NotesInFrameMap notesMap);
	Mark* selectedMark(bool onlySelection = true);
	TextNote* selectedNoteMark(int& foundPos, bool onlySelection = true);
	TextNote* selectedNoteMark(bool onlySelection = true);
protected:
	// set text frame height to last line of text
	double maxY {0.0};
	void setMaxY(double y);

public:
	void setTextFrameHeight();
	double naturalContentHeight();
};

#endif
