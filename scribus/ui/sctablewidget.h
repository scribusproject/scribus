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
		ScTableWidget ( QWidget* parent = nullptr );
		ScTableWidget ( int rows, int columns, QWidget* parent = nullptr );
		~ScTableWidget ();
		void setCellWidget ( int row, int column, QWidget * widget );
		void removeCellWidget ( int row, int column );
	protected:
		bool eventFilter(QObject *obj, QEvent *event);

	private:
		QHash<QWidget*, QPair<int, int> > widgetPositions;

	protected slots:
		void comboBoxReceiver(int i);
};

#endif
