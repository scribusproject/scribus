/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HYASK_H
#define HYASK_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

class WortEdit : public QLineEdit
{
	Q_OBJECT
public:
	WortEdit(QWidget* parent);
	~WortEdit() {};
	void keyPressEvent(QKeyEvent *k);
};

class HyAsk : public QDialog
{
	Q_OBJECT

public:
	HyAsk( QWidget* parent, QString HWort );
	~HyAsk() {};

	WortEdit* Wort;
	QPushButton* OK;
	QPushButton* Skip;
	QPushButton* Cancel;
	int xpos;
	int ypos;

public slots:
	void Check();
	void DoSkip();
	void accept();
	void reject();

protected:
	QVBoxLayout* HyAskLayout;
	QHBoxLayout* Layout1;
};

#endif // HYASK_H
