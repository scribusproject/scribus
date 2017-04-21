#ifndef NOTESSTYLES_H
#define NOTESSTYLES_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QList>
#include "numeration.h"
#include "pagestructs.h"
#include "scpage.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "text/storytext.h"

class ScribusDoc;
class PageItem_NoteFrame;
class PageItem_TextFrame;

//used for map with endnotes frames maped with range item
typedef union
{
	void* P;
	int sectionIndex;
	ScPage* page;
	PageItem_TextFrame* firstStoryFrame;
} rangeItem;

class SCRIBUS_API NotesStyle
{
public:
	NotesStyle() : m_nameStr ("Default"), m_startNum(1), m_endNotesStyle(false), m_numeration(), m_numRange(NSRdocument), m_suffixStr(")"),
		m_autoNotesHeight(true), m_autoNotesWidth(true), m_autoRemoveEmptyNotesFrames(true), m_autoWeldNotesFrames(true),
		m_superscriptInNote(true), m_superscriptInMaster(true) {}
	~NotesStyle() {}
	bool operator!=(const NotesStyle& n2);

	QString name() const { return m_nameStr; }
	void setName(const QString& s) { m_nameStr = s; }
	int start() const { return m_startNum; }
	void setStart(const int i) { m_startNum = i; }
	void setRange(NumerationRange ns) { m_numRange = ns; }
	const NumerationRange& range() const { return m_numRange; }
	QString prefix() const { return m_prefixStr; }
	void setPrefix (const QString& str) { m_prefixStr = str; }
	QString suffix() const { return m_suffixStr; }
	void setSuffix (const QString& str) { m_suffixStr = str; }

	QString numString(const int num) const { return m_numeration.numString(num); }
	void setType(const NumFormat type) { m_numeration.numFormat = type; }
	const NumFormat& getType() const { return m_numeration.numFormat; }

	bool isEndNotes() const { return m_endNotesStyle; }
	bool isAutoNotesHeight() const { return m_autoNotesHeight; }
	void setAutoNotesHeight(bool set) { m_autoNotesHeight = set; }
	bool isAutoNotesWidth() const { return m_autoNotesWidth; }
	void setAutoNotesWidth(bool set) { m_autoNotesWidth = set; }
	bool isAutoRemoveEmptyNotesFrames() const { return m_autoRemoveEmptyNotesFrames; }
	void setAutoRemoveEmptyNotesFrames(bool set) { m_autoRemoveEmptyNotesFrames = set; }
	bool isAutoWeldNotesFrames() const { return m_autoWeldNotesFrames; }
	void setAutoWeldNotesFrames(bool set) { m_autoWeldNotesFrames = set; }
	bool isSuperscriptInNote() const { return m_superscriptInNote; }
	void setSuperscriptInNote(bool set) { m_superscriptInNote = set; }
	bool isSuperscriptInMaster() const { return m_superscriptInMaster; }
	void setSuperscriptInMaster(bool set) { m_superscriptInMaster = set; }
	const QString marksChStyle() const { return m_marksCharStyle; }
	void setMarksCharStyle(const QString& styleName) { m_marksCharStyle = styleName; }
	const QString notesParStyle() const { return m_notesParaStyle; }
	void setNotesParStyle(const QString& styleName) { m_notesParaStyle = styleName; }

	void setEndNotes(bool);

private:
	QString m_nameStr;		//unique name of notes style
	int m_startNum;			//numeration starts with that number

	bool m_endNotesStyle;		//if not true this is set of footnotes
	Numeration m_numeration;
	NumerationRange m_numRange;	//range of numeration for current set
	QString m_prefixStr;
	QString m_suffixStr;
	bool m_autoNotesHeight;				//change height of notes frames to its content automaticaly?
	bool m_autoNotesWidth;				//change width of notes frames automaticaly if width of master frame changes?
	bool m_autoRemoveEmptyNotesFrames;
	bool m_autoWeldNotesFrames;
	bool m_superscriptInNote;
	bool m_superscriptInMaster;
	QString m_marksCharStyle;
	QString m_notesParaStyle;
};

class SCRIBUS_API TextNote : public QObject
{
    Q_OBJECT
	friend class ScribusDoc;

private:
	//only ScribusDoc can create and delete notes
	TextNote(NotesStyle *nStyle) : textLen(0), m_notesStyle(nStyle), m_noteMasterMark(NULL), m_noteFrameMark(NULL), m_number(0) { }
	~TextNote() {}
public:
	void setNotesStyle (NotesStyle* ns) { m_notesStyle = ns; }
	NotesStyle* notesStyle() { return m_notesStyle; }
	int num() { return  m_number; }
	void setNum(const int i) { m_number = i; }
	const QString numString();
	Mark* masterMark() { return m_noteMasterMark; }
	void setMasterMark(Mark* m) { m_noteMasterMark = m; }
	Mark* noteMark() { return m_noteFrameMark; }
	void setNoteMark(Mark* m) { m_noteFrameMark = m; }
	const QString saxedText() { return m_noteSaxedText; }
	void setSaxedText(const QString& string) { m_noteSaxedText = string; }
	bool isEndNote() { return m_notesStyle->isEndNotes(); }
	int textLen;

protected:
	NotesStyle *m_notesStyle;
	QString m_noteSaxedText;
	Mark *m_noteMasterMark;
	Mark *m_noteFrameMark;
	int m_number;
};

#endif // NOTESSTYLES_H
