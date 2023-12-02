/*************************************************************************************

   Copyright (C) 2019 Martin Reininger <nitramr>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************************/

#include "popup_menu.h"
#include <QEvent>
#include <QBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>

/*!
 * \brief [CTR]
 * \param widget
 */
PopupMenu::PopupMenu(QWidget *contentWidget)
{
	if(contentWidget == nullptr)
		contentWidget = new QWidget();

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(contentWidget);
	layout->setContentsMargins(1,1,1,1);

	QWidget *panel = new QWidget();
	panel->setMinimumSize(contentWidget->size());
	panel->setLayout(layout);

	QWidgetAction * action = new QWidgetAction(this);
	action->setDefaultWidget(panel);

	contentWidget->installEventFilter(this);

	addAction(action);

}

bool PopupMenu::eventFilter(QObject *obj, QEvent *event)
{

	// prevent that menu is close by interact with the background
	if (event->type() == QEvent::MouseButtonPress ||
			event->type() == QEvent::MouseButtonDblClick ||
			event->type() == QEvent::MouseMove) {

		return true;
	}

		return QObject::eventFilter(obj, event);

}

#ifndef Q_OS_MACOS
void PopupMenu::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(rect(), Qt::transparent);

	painter.setBrush( QBrush(palette().color(QPalette::Window)) );
	painter.setPen( QPen(palette().color(QPalette::Mid)) );
	painter.drawRect(this->rect().adjusted(0,0,-1,-1));

}
#endif
