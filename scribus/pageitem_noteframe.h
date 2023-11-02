#ifndef PAGEITEM_NOTEFRAME_H
#define PAGEITEM_NOTEFRAME_H

#include "pageitem_textframe.h"
#include "notesstyles.h"
#include "undostate.h"

class SCRIBUS_API PageItem_NoteFrame : public PageItem_TextFrame
{
	Q_OBJECT
	friend class ScribusDoc;

private:
	PageItem_NoteFrame(NotesStyle *nStyle, ScribusDoc *doc, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	PageItem_NoteFrame(ScribusDoc *doc, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
	PageItem_NoteFrame(PageItem_TextFrame* inFrame, NotesStyle *nStyle);
	~PageItem_NoteFrame() { }

public:
	PageItem_NoteFrame * asNoteFrame() override { return this; }
	const PageItem_NoteFrame * asNoteFrame()  const override { return this; }
	bool isNoteFrame() const override { return true; }
	bool isAutoNoteFrame() const override { return m_nstyle->isAutoRemoveEmptyNotesFrames(); }
	bool isTextContainer() const override { return true; }

	//overloaded text frame layouting
	void layout() override;

	void getNamedResources(ResourceCollection& lists) const override;
	void replaceNamedResources(ResourceCollection& newNames) override;

	//indicate if noteframe should be deleted
	bool deleteIt { false };

	//used while reading SLA file
	void setNoteStyle(NotesStyle* nStyle, PageItem_TextFrame* master = nullptr);
	//returns Notes Style
	NotesStyle* notesStyle() { return m_nstyle; }

	//return list of notes in noteframe
	const QList<TextNote*>& notesList() const { return m_notes; }
	//remove note from list
	void removeNote(TextNote* note) { m_notes.removeOne(note); }

	//insert notes content into notesframe
	void updateNotes(const QList<TextNote*>& nList, bool clear = true);
	//read notes text from notesframe itemText and store it in notes`s saxed text field
	void updateNotesText();

	PageItem_TextFrame* masterFrame() { return m_masterFrame; }
	void setMaster(PageItem* frame) { m_masterFrame = frame->asTextFrame(); }
	bool isEndNotesFrame() const { return m_nstyle->isEndNotes(); }
	bool isAutoWelded()  const { return m_nstyle->isAutoWeldNotesFrames(); }
	bool isAutoHeight()  const { return m_nstyle->isAutoNotesHeight(); }
	bool isAutoWidth()  const { return m_nstyle->isAutoNotesWidth(); }

	void restoreDeleteNoteText(SimpleState *state, bool isUndo);
	void restoreDeleteNoteParagraph(SimpleState *state, bool isUndo);
	void restoreInsertNoteText(SimpleState *state, bool isUndo);
	void restoreInsertNoteParagraph(SimpleState *state, bool isUndo);
	//overloaded PageItem::unWeld()
	void unWeld(bool doUndo = true);
	
private:
	QList<TextNote*> m_notes;
	NotesStyle* m_nstyle {nullptr};
	PageItem_TextFrame *m_masterFrame {nullptr};

	//insert note at end of text in noteframe
	void insertNote(TextNote* note);
};

#endif // PAGEITEM_NOTEFRAME_H
