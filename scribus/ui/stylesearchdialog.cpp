/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <utility>

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QMainWindow>
#include <QString>

#include "stylesearchdialog.h"
#include "ui_stylesearchdialog.h"
#include "iconmanager.h"

StyleSearchDialog::StyleSearchDialog(QMainWindow *parent, const QList<StyleSearchItem>& styles) :
	QDialog{parent},
	ui{new Ui::StyleSearchDialog},
	styles{styles}
{
	ui->setupUi(this);

	ui->filterLineEdit->installEventFilter(this);
	installEventFilter(this);

	connect(ui->filterLineEdit, &QLineEdit::textChanged,      this, &StyleSearchDialog::updateList);
	connect(this, &StyleSearchDialog::keyArrowUpPressed,     this, &StyleSearchDialog::moveSelectionUp);
	connect(this, &StyleSearchDialog::keyArrowDownPressed,   this, &StyleSearchDialog::moveSelectionDown);
	connect(ui->stylesListWidget, &QListWidget::itemDoubleClicked, this, &QDialog::accept);
}

StyleSearchDialog::~StyleSearchDialog()
{
	delete ui;
}

StyleSearchItem StyleSearchDialog::getStyle() const
{
	if (ui->stylesListWidget->count() == 0)
		return {"", StyleSearchType::paragraph};

	auto item = ui->stylesListWidget->currentItem();
	return {item->text(), static_cast<StyleSearchType>(item->type() - QListWidgetItem::UserType)};
}

/**
 * @brief capture return, arrow keys, and tab
 */
bool StyleSearchDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui->filterLineEdit)
	{
		if (event->type() == QEvent::KeyPress)
		{
			return filterLineEditKeyPress(static_cast<QKeyEvent*>(event));
		}
	}
	return false;
}

bool StyleSearchDialog::filterLineEditKeyPress(QKeyEvent * event)
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

void StyleSearchDialog::moveSelectionUp()
{
	int i = ui->stylesListWidget->currentRow();
	if (i > 0)
		ui->stylesListWidget->setCurrentRow(i - 1);
}

void StyleSearchDialog::moveSelectionDown()
{
	int i = ui->stylesListWidget->currentRow();
	if (i < ui->stylesListWidget->count() - 1)
		ui->stylesListWidget->setCurrentRow(i + 1);
}


/**
 * Fill the list with all styles that match the filter.
 * If the filter contains multiple words, acceppts all styles that
 * contain all the words
 */
void StyleSearchDialog::updateList()
{
	ui->stylesListWidget->clear();

	const auto filter = ui->filterLineEdit->text().trimmed();
	if (filter.isEmpty())
		return;

	IconManager &im = IconManager::instance();
	QIcon iconParagraph;
	QIcon iconCharacter;
	iconParagraph.addPixmap(im.loadPixmap("paragraph-style"));
	iconCharacter.addPixmap(im.loadPixmap("character-style"));
	if (!filter.contains(" "))
	{
		for (auto& style: std::as_const(styles))
		{
			if (style.name.contains(filter, Qt::CaseInsensitive))
			{
				auto qlwi = new QListWidgetItem(
					style.type == StyleSearchType::paragraph ? iconParagraph : iconCharacter,
					style.name,
					nullptr,
					static_cast<int>(QListWidgetItem::UserType) + static_cast<int>(style.type));
				ui->stylesListWidget->addItem(qlwi);
			}
		}
	}

	if (ui->stylesListWidget->count() > 0)
		ui->stylesListWidget->setCurrentRow(0);
}
