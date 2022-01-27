/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QDebug>
#include <QMainWindow>
#include <QString>
#include <QEvent>
#include <QKeyEvent>

#include "actionsearchdialog.h"
#include "ui_actionsearchdialog.h"

ActionSearchDialog::ActionSearchDialog(QMainWindow *parent, QList<QString> actionNames) :
	QDialog{parent},
	ui{new Ui::ActionSearchDialog},
	m_actionNames{actionNames}
{
	ui->setupUi(this);

	ui->filterLineEdit->installEventFilter(this);
	installEventFilter(this);

	connect(ui->filterLineEdit, &QLineEdit::textChanged,      this, &ActionSearchDialog::updateList);
	connect(this, &ActionSearchDialog::keyArrowUpPressed,     this, &ActionSearchDialog::moveSelectionUp);
	connect(this, &ActionSearchDialog::keyArrowDownPressed,   this, &ActionSearchDialog::moveSelectionDown);
	connect(ui->actionsListWidget, &QListWidget::itemDoubleClicked, this, &QDialog::accept);
}

ActionSearchDialog::~ActionSearchDialog()
{
	delete ui;
}

QString ActionSearchDialog::getActionName() const
{
	if (ui->actionsListWidget->count() == 0)
		return QString();

	return ui->actionsListWidget->currentItem()->text();
}

/**
 * @brief capture return, arrow keys, and tab
 */
bool ActionSearchDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui->filterLineEdit) {
		if (event->type() == QEvent::KeyPress) {
			return filterLineEditKeyPress(static_cast<QKeyEvent*>(event));
		}
	}
	return false;
}

bool ActionSearchDialog::filterLineEditKeyPress(QKeyEvent * event)
{
	switch (event->key())
	{
		case Qt::Key_Enter:
		case Qt::Key_Return:
			this->accept();
			return true;
		case Qt::Key_Up:
			emit keyArrowUpPressed();
			return true;
		case Qt::Key_Down:
		case Qt::Key_Tab:
			emit keyArrowDownPressed();
			return true;
		default:
			return false;
	}
}

void ActionSearchDialog::moveSelectionUp()
{
	int i = ui->actionsListWidget->currentRow();
	if (i > 0)
	{
		ui->actionsListWidget->setCurrentRow(i - 1);
	}
}

void ActionSearchDialog::moveSelectionDown()
{
	int i = ui->actionsListWidget->currentRow();
	if (i < ui->actionsListWidget->count() - 1)
		ui->actionsListWidget->setCurrentRow(i + 1);
}


/**
 * Fill the list with all actions that match the filter.
 * If the filter contains multiple words, acceppts all actions that
 * contain all the words
 */
void ActionSearchDialog::updateList()
{
	ui->actionsListWidget->clear();

	const auto filter = ui->filterLineEdit->text().trimmed();
	if (filter.isEmpty())
		return;

	if (!filter.contains(" "))
	{
		for (auto& name: qAsConst(m_actionNames))
		{
			if (name.contains(filter, Qt::CaseInsensitive))
			{
				ui->actionsListWidget->addItem(name);
			}
		}
	}
	else
	{
		auto words = filter.split(" ");
		for (auto& name: qAsConst(m_actionNames))
		{
			bool matches(true);
			for (auto& word: qAsConst(words))
			{
				if (!name.contains(word, Qt::CaseInsensitive))
				{
					matches = false;
					break;
				}
			}

			if (!matches)
				continue;
			ui->actionsListWidget->addItem(name);
		}
	}

	if (ui->actionsListWidget->count() > 0)
		ui->actionsListWidget->setCurrentRow(0);
}
