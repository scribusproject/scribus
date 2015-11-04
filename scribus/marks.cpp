#include <QDebug>
#include <QString>

#include "marks.h"

const QString Mark::getString()
{
	return data.strtxt;
}

void Mark::setString(const QString str)
{
	data.strtxt = str;
}
