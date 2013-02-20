/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QKeyEvent>

#include "scescapecatcher.h"

ScEscapeCatcher::ScEscapeCatcher(QObject* parent)
	: QObject(parent)
{
}

bool ScEscapeCatcher::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
		if (keyEvent->key() == Qt::Key_Escape)
		{
			emit escapePressed();
			return true;
		}
	}
	return QObject::eventFilter(o, e);
}
