/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "sctablewidget.h"

//Widgets supported
#include <QComboBox>
#include <QCheckBox>
//Other includes

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
	//emulation of cellChanged event from QTableWidget
	if (event->type() == QEvent::FocusOut && obj->isWidgetType())
	{
		int r=-1, c=-1;
		QComboBox* combobox = qobject_cast<QComboBox*>(obj);
		if (combobox)
		{
			if (widgetPositions.contains(combobox))
			{
				r=widgetPositions.value(combobox).first;
				c=widgetPositions.value(combobox).second;
			}
		}
		else
			{
				QCheckBox* checkbox = qobject_cast<QCheckBox*>(obj);
				if (checkbox)
				{
					if (widgetPositions.contains(checkbox))
					{
						r=widgetPositions.value(checkbox).first;
						c=widgetPositions.value(checkbox).second;
					}
				}
			}
		if (r!=-1 && c!=-1)
			emit cellChanged(r,c);
	}
	return QTableWidget::eventFilter(obj, event);
}

void ScTableWidget::setCellWidget(int row, int column, QWidget * widget)
{
	widget->installEventFilter(this);
	QTableWidget::setCellWidget(row, column, widget);
	widgetPositions.insert(widget, QPair<int, int>(row, column));
}

#if QT_VERSION  >= 0x040300
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
#endif
