/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STYLESEARCHDIALOG_H
#define STYLESEARCHDIALOG_H

class QKeyEvent;
class QMainWindow;
class QString;
class QEvent;

#include <QDialog>
#include <QList>
#include "stylesearch.h"

namespace Ui { class StyleSearchDialog; }

class StyleSearchDialog : public QDialog
{
	Q_OBJECT

public:
	explicit StyleSearchDialog(QMainWindow *parent, const QList<StyleSearchItem> &styleNames);
	~StyleSearchDialog();

	StyleSearchItem getStyle() const;

protected:
	bool eventFilter(QObject *obj, QEvent *ev) override;

private:
	Ui::StyleSearchDialog *ui { nullptr };
	QList<StyleSearchItem> styles;

	bool filterLineEditKeyPress(QKeyEvent * event);

private slots:
	void moveSelectionUp();
	void moveSelectionDown();
	void updateList();

signals:
	void keyArrowUpPressed();
	void keyArrowDownPressed();
};

#endif
