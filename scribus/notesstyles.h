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

//used for map with endnotes frames mapped with range item
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
	NotesStyle() {}
	~NotesStyle() {}

	bool operator!=(const NotesStyle& n2) const;

	const QString& name() const { return m_nameStr; }
	void setName(const QString& s) { m_nameStr = s; }

	int start() const { return m_startNum; }
	void setStart(int i) { m_startNum = i; }

	void setRange(NumerationRange ns) { m_numRange = ns; }
	const NumerationRange& range() const { return m_numRange; }

	const QString& prefix() const { return m_prefixStr; }
	void setPrefix (const QString& str) { m_prefixStr = str; }

	const QString& suffix() const { return m_suffixStr; }
	void setSuffix (const QString& str) { m_suffixStr = str; }

	QString numString(int num) const { return m_numeration.numString(num); }
	void setType(NumFormat type) { m_numeration.numFormat = type; }
	const NumFormat& getType() const { return m_numeration.numFormat; }

	bool isEndNotes() const { return m_endNotesStyle; }
	void setEndNotes(bool isEndnoteStyle);
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
	const QString& marksChStyle() const { return m_marksCharStyle; }
	void setMarksCharStyle(const QString& styleName) { m_marksCharStyle = styleName; }
	const QString& notesParStyle() const { return m_notesParaStyle; }
	void setNotesParStyle(const QString& styleName) { m_notesParaStyle = styleName; }

private:
	QString m_nameStr {"Default"}; //unique name of notes style
	int m_startNum {1}; //numeration starts with that number

	bool m_endNotesStyle {false}; //if not true this is set of footnotes
	Numeration m_numeration;
	NumerationRange m_numRange {NSRdocument}; //range of numeration for current set
	QString m_prefixStr;
	QString m_suffixStr {")"};
	bool m_autoNotesHeight {true}; //change height of notes frames to its content automatically?
	bool m_autoNotesWidth {true}; //change width of notes frames automatically if width of master frame changes?
	bool m_autoRemoveEmptyNotesFrames {true};
	bool m_autoWeldNotesFrames {true};
	bool m_superscriptInNote {true};
	bool m_superscriptInMaster {true};
	QString m_marksCharStyle;
	QString m_notesParaStyle;
};


#endif // NOTESSTYLES_H
