/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef GUIDESVIEWL_H
#define GUIDESVIEWL_H

#include <QTableView>

/*! \brief A view for guides list.
It appends only the "Delete" key handling as for issue #7765 to the QTableView
\author Petr Vanek <petr@scribus.info>
*/
class GuidesView : public QTableView
{
	Q_OBJECT

	public:
		GuidesView(QWidget * parent = 0);

	signals:
		void deleteKeyPressed();

	protected:
		void keyPressEvent(QKeyEvent * event);
};

#endif
