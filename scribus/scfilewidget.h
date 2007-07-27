/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCFILEWIDGET_H
#define SCFILEWIDGET_H

#include <QFileDialog>


/*! \brief Simple wrapper simulating the QFileDialog as a common widget.
Usage is simple - everything is like in QFileDialog except it is not
a dialog, it's a widget. It can be used for extension features etc.
\note Original QFileDialog OK/Cancel buttons are hidden. You must
write your own dialog accept/reject handling.
\author Petr Vanek <petr@scribus.info>
*/
class ScFileWidget : public QFileDialog
{
	Q_OBJECT

	public:
		ScFileWidget(QWidget * parent);

		QString selectedFile();

};

#endif
