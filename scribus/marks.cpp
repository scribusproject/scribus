#include <QDebug>
#include <QString>

#include "marks.h"

Mark::Mark(const Mark& other)
	: label(other.label),
	  OwnPage(other.OwnPage),
	  typ(other.typ),
	  data(other.data)
{

}

void Mark::setValues(const QString& l, int p, MarkType t, const MarkData& d)
{
	label = l;
	OwnPage = p;
	typ = t;
	data = d;
}

void Mark::getMark(QString& l, MarkType& t) const
{
	l = data.destmarkName;
	t = data.destmarkType;
}

void Mark::setMark(Mark* mP)
{
	if (mP == nullptr)
	{
		data.destmarkName = "";
		data.destmarkType = MARKNoType;
	}
	else
	{
		data.destmarkName = mP->label;
		data.destmarkType = mP->getType();
	}
}

void Mark::setMark(const QString& l, MarkType t)
{
	data.destmarkName = l;
	data.destmarkType = t;
}

const QString Mark::getString() const
{
	return data.strtxt;
}

void Mark::setString(const QString& str)
{
	data.strtxt = str;
}

void Mark::setNotePtr(TextNote* note)
{
	data.notePtr = note;
}

bool Mark::hasItemPtr() const
{
	return data.itemPtr != nullptr;
}

bool Mark::hasString() const
{
	return !data.strtxt.isEmpty();
}

bool Mark::hasMark() const
{
	return data.destmarkName != "";
}

bool Mark::isUnique() const
{
	return ((typ != MARKVariableTextType) && (typ != MARKIndexType) && (typ != MARKBullNumType));
}

bool Mark::isNoteType() const
{
	return ((typ == MARKNoteMasterType) || (typ == MARKNoteFrameType));
}

bool Mark::isType(const MarkType t) const
{
	return t == typ;
}

BulNumMark::BulNumMark()
{
	label = "BullNumMark";
	typ = MARKBullNumType;
}
