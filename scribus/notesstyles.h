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
	NotesStyle() : nameStr ("Default"), startNum(1), m_endNotesStyle(false), numeration(), numRange(NSRdocument), prefixStr(""), suffixStr(")"),
		autoNotesHeight(true), autoNotesWidth(true), autoRemoveEmptyNotesFrames(true), autoWeldNotesFrames(true),
		superscriptInNote(true), superscriptInMaster(true), marksCharStyle(""), notesParaStyle("") {}
	~NotesStyle() {}
	bool operator!=(const NotesStyle& n2);

	QString name() const { return nameStr; }
	void setName(const QString s) { nameStr = s; }
	int start() const { return startNum; }
	void setStart(const int i) { startNum = i; }
	void setRange(NumerationRange ns) { numRange = ns; }
	const NumerationRange& range() const { return numRange; }
	QString prefix() const { return prefixStr; }
	void setPrefix (const QString str) { prefixStr = str; }
	QString suffix() const { return suffixStr; }
	void setSuffix (const QString str) { suffixStr = str; }

	QString numString(const int num) const { return numeration.numString(num); }
	void setType(const NumFormat type) { numeration.numFormat = type; }
	const NumFormat& getType() const { return numeration.numFormat; }

	bool isEndNotes() const { return m_endNotesStyle; }
	bool isAutoNotesHeight() const { return autoNotesHeight; }
	void setAutoNotesHeight(bool set) { autoNotesHeight = set; }
	bool isAutoNotesWidth() const { return autoNotesWidth; }
	void setAutoNotesWidth(bool set) { autoNotesWidth = set; }
	bool isAutoRemoveEmptyNotesFrames() const { return autoRemoveEmptyNotesFrames; }
	void setAutoRemoveEmptyNotesFrames(bool set) { autoRemoveEmptyNotesFrames = set; }
	bool isAutoWeldNotesFrames() const { return autoWeldNotesFrames; }
	void setAutoWeldNotesFrames(bool set) { autoWeldNotesFrames = set; }
	bool isSuperscriptInNote() const { return superscriptInNote; }
	void setSuperscriptInNote(bool set) { superscriptInNote = set; }
	bool isSuperscriptInMaster() const { return superscriptInMaster; }
	void setSuperscriptInMaster(bool set) { superscriptInMaster = set; }
	const QString marksChStyle() const { return marksCharStyle; }
	void setMarksCharStyle(const QString styleName) { marksCharStyle = styleName; }
	const QString notesParStyle() const { return notesParaStyle; }
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
	const QString numString();
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
