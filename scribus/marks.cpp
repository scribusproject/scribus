#include <QDebug>
#include <QString>

#include "marks.h"

Mark::Mark(const Mark& other)
	: label(other.label),
	  OwnPage(other.OwnPage),
	  m_type(other.m_type),
	  m_data(other.m_data)
{

}

void Mark::setValues(const QString& l, int p, MarkType t, const MarkData& d)
{
	label = l;
	OwnPage = p;
	m_type = t;
	m_data = d;
}

void Mark::getMark(QString& l, MarkType& t) const
{
	l = m_data.destmarkName;
	t = m_data.destmarkType;
}

void Mark::setMark(Mark* mP)
{
	if (mP == nullptr)
	{
		m_data.destmarkName = "";
		m_data.destmarkType = MARKNoType;
	}
	else
	{
		m_data.destmarkName = mP->label;
		m_data.destmarkType = mP->getType();
	}
}

void Mark::setMark(const QString& l, MarkType t)
{
	m_data.destmarkName = l;
	m_data.destmarkType = t;
}

const QString Mark::getString() const
{
	return m_data.strtxt;
}

void Mark::setString(const QString& str)
{
	m_data.strtxt = str;
}

void Mark::clearString()
{
	m_data.strtxt.clear();
}

void Mark::setNotePtr(TextNote* note)
{
	m_data.notePtr = note;
}

bool Mark::hasItemPtr() const
{
	return m_data.itemPtr != nullptr;
}

bool Mark::hasString() const
{
	return !m_data.strtxt.isEmpty();
}

bool Mark::hasMark() const
{
	return !m_data.destmarkName.isEmpty();
}

bool Mark::isUnique() const
{
	return ((m_type != MARKVariableTextType) && (m_type != MARKIndexType) && (m_type != MARKBullNumType));
}

bool Mark::isNoteType() const
{
	return ((m_type == MARKNoteMasterType) || (m_type == MARKNoteFrameType));
}

bool Mark::isType(const MarkType t) const
{
	return (m_type == t);
}

BulNumMark::BulNumMark()
{
	label = "BullNumMark";
	m_type = MARKBullNumType;
}
