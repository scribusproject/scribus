/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QDropEvent>
#include <QKeyEvent>
#include <QMimeData>

#include "filedialogeventcatcher.h"

FileDialogEventCatcher::FileDialogEventCatcher(QObject* parent)
	: QObject(parent)
{
}

bool FileDialogEventCatcher::eventFilter(QObject *o, QEvent *e)
{
	if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
		switch (keyEvent->key())
		{
			case Qt::Key_Escape:
				emit escapePressed();
				return true;
			case Qt::Key_D:
				if (keyEvent->modifiers() & Qt::ControlModifier)
				{
					emit desktopPressed();
					return true;
				}
				break;
			case Qt::Key_H:
				if (keyEvent->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier))
				{
					emit homePressed();
					return true;
				}
				break;
			case Qt::Key_Up:
				if (keyEvent->modifiers() & Qt::ControlModifier)
				{
					emit parentPressed();
					return true;
				}
				break;
			case Qt::Key_Down:
				if (keyEvent->modifiers() & Qt::ControlModifier)
				{
					emit enterSelectedPressed();
					return true;
				}
				break;
		}
	}
	else
	if (e->type() == QEvent::DragEnter)
	{
		e->accept();
		return true;
	}
	else
	if (e->type() == QEvent::Drop)
	{
		QDropEvent *dropEvent = static_cast<QDropEvent *>(e);
		if (dropEvent)
		{
			if ( dropEvent->mimeData()->hasFormat("text/uri-list"))
			{
				QString fileUrl;
				QList<QUrl> fileUrls = dropEvent->mimeData()->urls();
				if (fileUrls.count()>0)
				{
					fileUrl = fileUrls[0].toLocalFile();
					if (fileUrls[0].isLocalFile())
					{
						emit dropLocation(fileUrl);
					}
				}
			}
		}
		e->accept();
		return true;
	}
	return QObject::eventFilter(o, e);
}
