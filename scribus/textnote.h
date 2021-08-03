/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef TEXTNOTE_H
#define TEXTNOTE_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QList>

#include "scribusapi.h"

class Mark;
class NotesStyle;
class ScribusDoc;

class SCRIBUS_API TextNote : public QObject
{
	Q_OBJECT
	friend class ScribusDoc;

private:
	//only ScribusDoc can create and delete notes
	TextNote(NotesStyle *nStyle) : m_notesStyle(nStyle) { }
	~TextNote() {}

public:
	NotesStyle* notesStyle() { return m_notesStyle; }
	const NotesStyle* notesStyle() const { return m_notesStyle; }
	void setNotesStyle (NotesStyle* ns) { m_notesStyle = ns; }

	int num() const { return m_number; }
	void setNum(const int i) { m_number = i; }
	QString numString() const;

	Mark* masterMark() { return m_noteMasterMark; }
	const Mark* masterMark() const { return m_noteMasterMark; }
	void setMasterMark(Mark* m) { m_noteMasterMark = m; }

	Mark* noteMark() { return m_noteFrameMark; }
	const Mark* noteMark() const { return m_noteFrameMark; }
	void setNoteMark(Mark* m) { m_noteFrameMark = m; }

	const QString& saxedText() const { return m_noteSaxedText; }
	void setSaxedText(const QString& string) { m_noteSaxedText = string; }
	void clearSaxedText();

	bool isEndNote() const { return m_notesStyle->isEndNotes(); }
	int textLen {0};

protected:
	NotesStyle *m_notesStyle {nullptr};
	QString m_noteSaxedText;
	Mark *m_noteMasterMark {nullptr};
	Mark *m_noteFrameMark {nullptr};
	int m_number {0};
};

#endif // TEXTNOTE_H
