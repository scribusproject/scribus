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

void Mark::getDestMark(QString& l, MarkType& t) const
{
	l = m_data.destMarkName;
	t = m_data.destMarkType;
}

void Mark::setDestMark(Mark* mP)
{
	if (mP == nullptr)
	{
		m_data.destMarkName.clear();
		m_data.destMarkType = MARKNoType;
	}
	else
	{
		m_data.destMarkName = mP->label;
		m_data.destMarkType = mP->getType();
	}
}

void Mark::setDestMark(const QString& l, MarkType t)
{
	m_data.destMarkName = l;
	m_data.destMarkType = t;
}

void Mark::clearString()
{
	m_data.text.clear();
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
	return !m_data.text.isEmpty();
}

bool Mark::hasMark() const
{
	return !m_data.destMarkName.isEmpty();
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
