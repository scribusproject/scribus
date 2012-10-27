#ifndef PAGEITEM_NOTEFRAME_H
#define PAGEITEM_NOTEFRAME_H

#include "pageitem_textframe.h"
#include "notesstyles.h"
#include "undostate.h"

class PageItem_NoteFrame : public PageItem_TextFrame
{
		Q_OBJECT
friend class ScribusDoc;
private:
	PageItem_NoteFrame(NotesStyle *nStyle, ScribusDoc *doc, double x, double y, double w, double h, double w2, QString fill, QString outline);
	PageItem_NoteFrame(ScribusDoc *doc, double x, double y, double w, double h, double w2, QString fill, QString outline);
	PageItem_NoteFrame(PageItem_TextFrame* inFrame, NotesStyle *nStyle);
	~PageItem_NoteFrame() { }
public:
	virtual PageItem_NoteFrame * asNoteFrame() { return this; }
	virtual bool isNoteFrame() const { return true; }
	virtual bool isAutoNoteFrame() const { return m_nstyle->isAutoRemoveEmptyNotesFrames(); }

	//overloaded text frame layouting
	void layout();

	//indicate if noteframe should be deleted
	bool deleteIt;

	//used while reading SLA file
	void setNS(NotesStyle* nStyle, PageItem_TextFrame* master = NULL);
	//returns Notes Style
	NotesStyle* notesStyle() { return m_nstyle; }

	//insert notes content into notesframe
	void updateNotes(QList<TextNote*> nList, bool clear = true);
	//read notes text from notesframe itemText and store it in notes`s saxed text field
	void updateNotesText();

	PageItem_TextFrame* masterFrame() { return m_masterFrame; }
	void setMaster(PageItem* frame) { m_masterFrame = frame->asTextFrame(); }
	bool isEndNotesFrame() { return m_nstyle->isEndNotes(); }
	bool isAutoWelded() { return m_nstyle->isAutoWeldNotesFrames(); }
	bool isAutoHeight() { return m_nstyle->isAutoNotesHeight(); }
	bool isAutoWidth() { return m_nstyle->isAutoNotesWidth(); }

	//return list of notes in noteframe
	QList<TextNote*> notesList() { return l_notes; }
	//remove note from list
	void removeNote(TextNote* note) { l_notes.removeOne(note); }

	void restoreDeleteNoteText(SimpleState *state, bool isUndo);
	void restoreInsertNoteText(SimpleState *state, bool isUndo);
	//overloaded PageItem::unWeld()
	void unWeld(bool doUndo=true);
	
private:
	QList<TextNote*> l_notes;
	NotesStyle* m_nstyle;
	PageItem_TextFrame *m_masterFrame;

	//insert note at end of text in noteframe
	void insertNote(TextNote* note);

//not used???
	//find position of note marker in text
	int findNoteCpos(TextNote* note);
	
};

#endif // PAGEITEM_NOTEFRAME_H
