/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCFILEWIDGET_H
#define SCFILEWIDGET_H

#include <QFileDialog>

#include "scribusapi.h"

/*! \brief Simple wrapper simulating the QFileDialog as a common widget.
Usage is simple - everything is like in QFileDialog except it is not
a dialog, it's a widget. It can be used for extension features etc.
\note Original QFileDialog OK/Cancel buttons are hidden. You must
write your own dialog accept/reject handling.
There is one good idea - it's quite good to handle original dialog
accepted() signal on your own to prevent ugly widget hidding when
user e.g. doubleclicks the "open" file (in the parent (new) dialog):
connect(fileWidget, SIGNAL(accepted()), this, SLOT(accept()));
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API ScFileWidget : public QFileDialog
{
	Q_OBJECT

	public:
		ScFileWidget(QWidget * parent);

		QString selectedFile();

};

#endif
