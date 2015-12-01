#include "notesstyles.h"
#include "marks.h"
#include "util.h"

void NotesStyle::setEndNotes(bool setendnotes)
{
	if (m_endNotesStyle != setendnotes)
	{
		m_endNotesStyle = setendnotes;
	}
}

bool NotesStyle::operator!=(const NotesStyle& n2)
{
	return ((m_nameStr != n2.m_nameStr) || (m_startNum != n2.m_startNum) || (m_endNotesStyle != n2.m_endNotesStyle) ||
			(getType() != static_cast<NotesStyle>(n2).getType()) || (m_numRange != n2.m_numRange) ||
			(m_prefixStr != n2.m_prefixStr) || (m_suffixStr != n2.m_suffixStr) ||
			(m_autoNotesHeight != n2.m_autoNotesHeight) || (m_autoNotesWidth != n2.m_autoNotesWidth) ||
			(m_autoRemoveEmptyNotesFrames != n2.m_autoRemoveEmptyNotesFrames) || (m_autoWeldNotesFrames != n2.m_autoWeldNotesFrames) ||
			(m_superscriptInMaster != n2.m_superscriptInMaster) || (m_superscriptInNote != n2.m_superscriptInNote) ||
			(m_marksCharStyle != n2.m_marksCharStyle) || (m_notesParaStyle != n2.m_notesParaStyle)
			);
}


const QString TextNote::numString()
{
	return notesStyle()->numString(m_number);
}
