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

void Mark::setValues(const QString& l, int p, MarkType t, MarkData d)
{
	label = l;
	OwnPage = p;
	typ = t;
	data = d;
}

void Mark::getMark(QString& l, MarkType& t)
{
	l = data.destmarkName;
	t = data.destmarkType;
}

void Mark::setMark(const QString& l, MarkType t)
{
	data.destmarkName = l;
	data.destmarkType = t;
}

const QString Mark::getString()
{
	return data.strtxt;
}

void Mark::setString(const QString str)
{
	data.strtxt = str;
}

void Mark::setNotePtr(TextNote* note)
{
	data.notePtr = note;
}

bool Mark::hasItemPtr()
{
	return data.itemPtr != NULL;
}

bool Mark::hasString()
{
	return !data.strtxt.isEmpty();
}

bool Mark::hasMark()
{
	return data.destmarkName != "";
}

bool Mark::isUnique()
{
	return ((typ != MARKVariableTextType) && (typ != MARKIndexType) && (typ != MARKBullNumType));
}

bool Mark::isNoteType()
{
	return ((typ == MARKNoteMasterType) || (typ==MARKNoteFrameType));
}

bool Mark::isType(const MarkType t)
{
	return t==typ;
}

BulNumMark::BulNumMark()
	: Mark()
{
	label = "BullNumMark";
	typ = MARKBullNumType;
}
