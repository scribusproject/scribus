/***************************************************************************
                          conswin.cpp  -  description
                             -------------------
    begin                : Mon Okt 14 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "conswin.h"
#include "conswin.moc"

ConsWin::ConsWin(QWidget* parent) : QTextEdit(parent)
{
	setTextFormat(Qt::PlainText);
	setText(">>>");
	Prompt = ">>>";
	moveCursor(QTextEdit::MoveEnd, false);
	LastComm = "";
}

void ConsWin::keyPressEvent(QKeyEvent *k)
{
	int p, i;
	getCursorPosition(&p, &i);
	if ((k->key() == Key_Left) || (k->key() == Key_Backspace))
		{
		if (i < 4)
			return;
		}
	if (k->key() == Key_Delete)
		{
		if (p != paragraphs()-1)
			return;
		}
	if ((k->key() == Key_Home) || (k->key() == Key_Prior) || (k->key() == Key_Next) || (k->key() == Key_Backtab)) 
		return;
	if ((k->key() == Key_Return) || (k->key() == Key_Enter))
		{
		if ((text(p).startsWith(">>>")) || (text(p).startsWith("...")))
			{
			LastComm = text(p).remove(0, 3);
			LastComm = LastComm.mid(0, LastComm.length()-1);
			}
		else
			LastComm = "";
		if (p == paragraphs()-1)
			emit returnPressed();
		else
			{
			removeParagraph(paragraphs()-1);
			insertParagraph(Prompt+LastComm, -1);
			moveCursor(QTextEdit::MoveEnd, false);
			moveCursor(QTextEdit::MoveBackward, false);
			del();
			}
		return;
		}
	QTextEdit::keyPressEvent(k);
	getCursorPosition(&p, &i);
	if (((text(p).startsWith(">>>")) || (text(p).startsWith("..."))) && (i < 3))
		setCursorPosition(p, 3);
}

void ConsWin::contentsMousePressEvent(QMouseEvent *m)
{
	int p, i;
	QTextEdit::contentsMousePressEvent(m);
	i = charAt(m->pos(), &p);
	if (((text(p).startsWith(">>>")) || (text(p).startsWith("..."))) && (i < 3))
		setCursorPosition(p, 3);
	else
		setCursorPosition(p, i);
	return;
}
