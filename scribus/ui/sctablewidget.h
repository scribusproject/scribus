/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCTABLEWIDGET_H
#define SCTABLEWIDGET_H

#include <QTableWidget>
#include <QEvent>
#include <QHash>
#include <QPair>


class ScTableWidget : public QTableWidget
{
	Q_OBJECT

	public:
		ScTableWidget ( QWidget * parent = 0 );
		ScTableWidget ( int rows, int columns, QWidget * parent = 0 );
		~ScTableWidget ();
		void setCellWidget ( int row, int column, QWidget * widget );
#if QT_VERSION  >= 0x040300
		void removeCellWidget ( int row, int column );
#endif
	protected:
		bool eventFilter(QObject *obj, QEvent *event);

	private:
		QHash<QWidget*, QPair<int, int> > widgetPositions;
};

#endif
