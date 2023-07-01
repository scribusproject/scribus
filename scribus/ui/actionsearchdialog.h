/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ACTIONSEARCHDIALOG_H
#define ACTIONSEARCHDIALOG_H

class QKeyEvent;
class QMainWindow;
class QString;
class QEvent;

#include <QDialog>
#include <QList>

namespace Ui { class ActionSearchDialog; }

class ActionSearchDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ActionSearchDialog(QMainWindow *parent, QList<QString> actionNames);
	~ActionSearchDialog();

	QString getActionName() const;

protected:
	bool eventFilter(QObject *obj, QEvent *ev) override;

private:
	Ui::ActionSearchDialog *ui { nullptr };
	QList<QString> m_actionNames;

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
