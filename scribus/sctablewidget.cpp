/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "sctablewidget.h"

#include <QComboBox>
#include <QMap>
#include <QPair>

ScTableWidget::ScTableWidget(QWidget *parent) : QTableWidget(parent)
{
}

ScTableWidget::ScTableWidget( int rows, int columns, QWidget *parent) : QTableWidget(rows, columns, parent)
{
}

ScTableWidget::~ScTableWidget()
{
}

bool ScTableWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::FocusIn && obj->isWidgetType())
	{
		QWidget* widget = qobject_cast<QWidget*>(obj);
		if (widget)
		{
			QTableWidgetItem* item = itemAt(widget->pos());
			if (item)
			{
				setCurrentItem(item);
			}
		}
	}
	if (event->type() == QEvent::FocusOut && obj->isWidgetType())
	{
		QComboBox* combobox = qobject_cast<QComboBox*>(obj);
		if (combobox)
		{
			int r=0, c=0;
			if (widgetPositions.contains(combobox))
			{
				r=widgetPositions.value(combobox).first;
				c=widgetPositions.value(combobox).second;
			}
// 			qDebug(QString("- %1 %2 %3").arg(combobox->currentText()).arg(r).arg(c));
			emit cellChanged(r,c);
		}
	}
	return QTableWidget::eventFilter(obj, event);
}

void ScTableWidget::setCellWidget(int row, int column, QWidget * widget)
{
	widget->installEventFilter(this);
	QTableWidget::setCellWidget(row, column, widget);
	widgetPositions.insert(widget, QPair<int, int>(row, column));
}

void ScTableWidget::removeCellWidget ( int row, int column )
{
	QTableWidget::removeCellWidget(row, column);
	QHash<QWidget*, QPair<int, int> >::const_iterator i = widgetPositions.constBegin();
	QWidget* t=NULL;
	while (i != widgetPositions.constEnd())
	{
		if (i.value().first==row && i.value().second==column)
		{
			t=i.key();
			break;
		}
		++i;
	}
	if (t!=NULL)
		widgetPositions.remove(t);
}

