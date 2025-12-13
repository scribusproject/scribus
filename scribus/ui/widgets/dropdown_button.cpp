#include <QMenu>
#include "dropdown_button.h"
#include "scribusapp.h"

DropdownButton::DropdownButton(QWidget *parent) : QToolButton(parent)
{
	setPopupMode(QToolButton::InstantPopup);
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void DropdownButton::setCurrentIndex(int index)
{
	m_index = index;

	if (!menu())
		return;

	if (menu()->actions().empty())
		return;

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

void DropdownButton::iconSetChange()
{
	this->blockSignals(true);
	setCurrentIndex(m_index);
	this->blockSignals(false);
}
