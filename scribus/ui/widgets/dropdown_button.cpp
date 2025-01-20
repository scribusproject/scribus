#include <QMenu>
#include "dropdown_button.h"

DropdownButton::DropdownButton(QWidget *parent) : QToolButton(parent)
{
	setPopupMode(QToolButton::InstantPopup);
}

void DropdownButton::setCurrentIndex(int index)
{
	m_index = index;

	if (menu())
		indexChanged(menu()->actions().at(qBound(0, index, menu()->actions().size())));
}

void DropdownButton::setMenu(QMenu *menu)
{
	//menu->setFont(font());
	QToolButton::setMenu(menu);
	connect(menu, &QMenu::triggered, this, &DropdownButton::indexChanged, Qt::UniqueConnection);
}

void DropdownButton::clear()
{
	if (menu())
		menu()->clear();
}

QAction *DropdownButton::addAction(const QIcon &icon, const QString &text)
{
	if (menu())
		return menu()->addAction(icon, text);

	QMenu *m = new QMenu();
	setMenu(m);
	return m->addAction(icon, text);
}

void DropdownButton::indexChanged(QAction *action)
{
	if (!action->icon().isNull())
		setIcon(action->icon());

	if (menu())
	{
		m_index = menu()->actions().indexOf(action);
		emit currentIndexChanged(m_index);
		emit activated(m_index);
	}
}
