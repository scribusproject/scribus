#ifndef NOTESSTYLES_H
#define NOTESSTYLES_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QList>
#include "pagestructs.h"
#include "scpage.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "text/storytext.h"
#include "util.h"

class ScribusDoc;
class PageItem_NoteFrame;
class PageItem_TextFrame;

class Numeration
{
public:
	Numeration() : numType(Type_1_2_3) {}
	void setType(NumerationType type) { numType = type; }
	const NumerationType type() { return numType; }
	const QString numString(const int num) { return getStringFromSequence(numType, num); }
private:
	NumerationType numType;
};

typedef enum {
	NSRdocument,
	NSRsection,
	NSRstory,
	NSRpage,
	NSRframe
} NumerationRange;

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
	NotesStyle() : nameStr ("Default"), startNum(1), m_endNotesStyle(false), numeration(), numRange(NSRdocument), prefixStr(""), suffixStr(")"),
		autoNotesHeight(true), autoNotesWidth(true), autoRemoveEmptyNotesFrames(true), autoWeldNotesFrames(true),
		superscriptInNote(true), superscriptInMaster(true), marksCharStyle(""), notesParaStyle("") {}
	~NotesStyle() {}
	bool operator!=(const NotesStyle& n2);

	const QString name() { return nameStr; }
	void setName(const QString s) { nameStr = s; }
	const int start() { return startNum; }
	void setStart(const int i) { startNum = i; }
	void setRange(NumerationRange ns) { numRange = ns; }
	const NumerationRange range() { return numRange; }
	const QString prefix() { return prefixStr; }
	void setPrefix (const QString str) { prefixStr = str; }
	const QString suffix() { return suffixStr; }
	void setSuffix (const QString str) { suffixStr = str; }

	const QString numString(const int num) { return numeration.numString(num); }
	void setType(const NumerationType type) { numeration.setType(type); }
	const NumerationType getType() { return numeration.type(); }

	bool isEndNotes() { return m_endNotesStyle; }
	bool isAutoNotesHeight() { return autoNotesHeight; }
	void setAutoNotesHeight(const bool set) { autoNotesHeight = set; }
	bool isAutoNotesWidth() { return autoNotesWidth; }
	void setAutoNotesWidth(const bool set) { autoNotesWidth = set; }
	bool isAutoRemoveEmptyNotesFrames() { return autoRemoveEmptyNotesFrames; }
	void setAutoRemoveEmptyNotesFrames(const bool set) { autoRemoveEmptyNotesFrames = set; }
	bool isAutoWeldNotesFrames() { return autoWeldNotesFrames; }
	void setAutoWeldNotesFrames(const bool set) { autoWeldNotesFrames = set; }
	bool isSuperscriptInNote() { return superscriptInNote; }
	void setSuperscriptInNote(const bool set) { superscriptInNote = set; }
	bool isSuperscriptInMaster() { return superscriptInMaster; }
	void setSuperscriptInMaster(const bool set) { superscriptInMaster = set; }
	const QString marksChStyle() { return marksCharStyle; }
	void setMarksCharStyle(const QString styleName) { marksCharStyle = styleName; }
	const QString notesParStyle() { return notesParaStyle; }
	void setNotesParStyle(const QString styleName) { notesParaStyle = styleName; }

	void setEndNotes(bool);

private:
	QString nameStr;		//unique name of notes style
	int startNum;			//numeration starts with that number

	bool m_endNotesStyle;		//if not true this is set of footnotes
	Numeration numeration;
	NumerationRange numRange;	//range of numeration for current set
	QString prefixStr;
	QString suffixStr;
	bool autoNotesHeight;				//change height of notes frames to its content automaticaly?
	bool autoNotesWidth;				//change width of notes frames automaticaly if width of master frame changes?
	bool autoRemoveEmptyNotesFrames;
	bool autoWeldNotesFrames;
	bool superscriptInNote;
	bool superscriptInMaster;
	QString marksCharStyle;
	QString notesParaStyle;
};

class SCRIBUS_API TextNote : public QObject
{
    Q_OBJECT
	friend class ScribusDoc;
private:
	//only ScribusDoc can create and delete notes
	TextNote(NotesStyle *nStyle) : m_notesStyle(nStyle), m_noteSaxedText(""), m_noteMasterMark(NULL), m_noteFrameMark(NULL), m_number(0) { }
	~TextNote() {}
public:
	void setNotesStyle (NotesStyle* ns) { m_notesStyle = ns; }
	NotesStyle* notesStyle() { return m_notesStyle; }
	const int num() { return  m_number; }
	void setNum(const int i) { m_number = i; }
	const QString numString() { return notesStyle()->numString(m_number); }
	Mark* masterMark() { return m_noteMasterMark; }
	void setMasterMark(Mark* m) { m_noteMasterMark = m; }
	Mark* noteMark() { return m_noteFrameMark; }
	void setNoteMark(Mark* m) { m_noteFrameMark = m; }
	const QString saxedText() { return m_noteSaxedText; }
	void setSaxedText(const QString string) { m_noteSaxedText = string; }
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
