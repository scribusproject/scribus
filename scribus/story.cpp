/***************************************************************************
                          story.cpp  -  description
                             -------------------
    begin                : Tue Nov 11 2003
    copyright            : (C) 2003 by Franz Schmid
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
#include "story.h"
#include "story.moc"
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <qcombobox.h>
#include "serializer.h"
extern QPixmap loadIcon(QString nam);


SEditor::SEditor(QWidget* parent) : QTextEdit(parent)
{
	clines = 0;
	setFrameStyle( QFrame::NoFrame | QFrame::Plain );
//	setTextFormat(Qt::RichText);
}

void SEditor::keyPressEvent(QKeyEvent *k)
{
	int p, i;
	getCursorPosition(&p, &i);
	if ((k->key() == Key_Backspace) && (i == 0))
		{
		emit bsPressed();
		return;
		}
	if ((k->key() == Key_Delete) && (i == static_cast<int>(text().length())))
		{
		emit delPressed();
		return;
		}
	QTextEdit::keyPressEvent(k);
	if (clines != lines())
		emit wrapped();
	clines = lines();
	ensureCursorVisible();
}


STable::STable(QWidget* parent) : QTable(parent)
{
	setShowGrid(false);
}

void STable::keyPressEvent(QKeyEvent *k)
{
	QString tmp;
	int p, i, l, r, c, n;
	QTextEdit *tt;
	n = numRows();
	r = currentRow();
	c = currentColumn();
	if (c == 1)
		{
		tt = (QTextEdit*)cellWidget(r, 1);
		if ((k->key() == Key_Left) || (k->key() == Key_Right) || (k->key() == Key_Down) || (k->key() == Key_Up))
			{
			QTextEdit::CursorAction move;
			tt->getCursorPosition(&p, &i);
			switch (k->key())
				{
				case Key_Left:
					if ((i == 0) && (r > 0))
						{
						tt = (QTextEdit*)cellWidget(r-1, 1);
						setCurrentCell(r-1, 1);
						move = QTextEdit::MoveEnd;
						}
					else
						move = QTextEdit::MoveBackward;
					break;
				case Key_Right:
					if ((i == static_cast<int>(tt->text().length())) && (r < n-1))
						{
						tt = (QTextEdit*)cellWidget(r+1, 1);
						setCurrentCell(r+1, 1);
						move = QTextEdit::MoveLineStart;
						}
					else
						move = QTextEdit::MoveForward;
					break;
				case Key_Up:
					l = tt->lineOfChar(0, i);
					if ((l == 0) && (r > 0))
						{
						tt = (QTextEdit*)cellWidget(r-1, 1);
						setCurrentCell(r-1, 1);
						move = QTextEdit::MoveEnd;
						}
					else
						move = QTextEdit::MoveUp;
					break;
				case Key_Down:
					l = tt->lineOfChar(0, i);
					if ((l == tt->lines()-1) && (r < n-1))
						{
						tt = (QTextEdit*)cellWidget(r+1, 1);
						setCurrentCell(r+1, 1);
						move = QTextEdit::MoveLineStart;
						}
					else
						move = QTextEdit::MoveDown;
					break;
				}
			tt->moveCursor(move, false);
			tt->setFocus();
			updateHeaderStates(); 
			return;
			}
		}
	QTable::keyPressEvent(k);
}

void STable::adjHeight(int r)
{
	QTextEdit *cp = (QTextEdit*)cellWidget(r, 1);
	cp->sync();
	QFontMetrics fm2(cp->currentFont());
	setRowHeight(r, QMAX((fm2.lineSpacing() * (cp->lines()+1)), 24));
	updateHeaderStates();;
}

StoryEditor::StoryEditor(QWidget* parent, ScribusDoc *docc, PageItem *ite) : QDialog(parent, "StoryEditor", false, 0)
{
  uint a;
	int para = 0;
	int pstyle;
	doc = docc;
  QString Dat = "";
	QPtrList<Pti> y = ite->Ptext;
	setCaption( tr( "Story Editor" ) );
	setIcon(loadIcon("AppIcon.png"));
	Form1Layout = new QHBoxLayout( this, 11, 6, "Form1Layout"); 
	edList.clear();
	stList.clear();
	style.append(tr("Left"));
	style.append(tr("Center"));
	style.append(tr("Right"));
	style.append(tr("Block"));
	style.append(tr("Forced"));
	if (doc->Vorlagen.count() > 5)
		{
		for (uint a = 5; a < doc->Vorlagen.count(); ++a)
			{
			style.append(doc->Vorlagen[a].Vname);
			}
		}
	CurrItem = ite;
	table1 = new STable( this );
	table1->setNumCols( 2 );
	table1->horizontalHeader()->setLabel( 0, tr( "Style" ) );
	table1->horizontalHeader()->setLabel( 1, tr( "Text" ) );
	table1->setSelectionMode( QTable::Single );
	table1->setColumnStretchable(0, false);
	table1->setColumnStretchable(1, true);
	Form1Layout->addWidget( table1 );
	resize( QSize(509, 326).expandedTo(minimumSizeHint()) );
	show();
  for (a = 0; a < y.count(); ++a)
  	{
		QString b = y.at(a)->ch;
		pstyle = y.at(a)->cab;
		if (b == QChar(13))
			{
			addPar(para, Dat, pstyle);
			Dat = "";
			para++;
			}
		else
    	Dat += b;
    }
	if (Dat != "")
		addPar(para, Dat, pstyle);
	TextChanged = false;
}

void StoryEditor::updateTextFrame()
{
	bool first = false;
	for (uint a = 0; a < edList.count(); ++a)
		{
		Serializer *ss = new Serializer("");
		QTextEdit *tt = edList.at(a);
		QComboBox *cp = stList.at(a);
		ss->Objekt = tt->text();
		if (a < edList.count()-1)
			ss->Objekt += QChar(10);
		int st = cp->currentItem();
		ss->GetText(CurrItem, st, doc->Vorlagen[st].Font, doc->Vorlagen[st].FontSize, first);
		delete ss;
		first = true;
		}
	if (doc->Trenner->AutoCheck)
		doc->Trenner->slotHyphenate(CurrItem);
	doc->ActPage->RefreshItem(CurrItem);
	TextChanged = false;
}

void StoryEditor::styleChange(int st)
{
	int r = stList.findRef((QComboBox*)sender());
	int align;
	if (st > 4)
		align = doc->Vorlagen[st].Ausri;
	else
		align = st;
	if (r != -1)
		{
		QTextEdit *tt = (QTextEdit*)table1->cellWidget(r, 1);
		switch (align)
			{
			case 0:
				tt->setAlignment(Qt::AlignLeft);
				break;
			case 1:
				tt->setAlignment(Qt::AlignCenter);
				break;
			case 2:
				tt->setAlignment(Qt::AlignRight);
				break;
			case 3:
			case 4:
				tt->setAlignment(Qt::AlignJustify);
				break;
			default:
				break;
			}
		}
}

int StoryEditor::getStyle(int where)
{
	QComboBox *cp = stList.at(where);
	return cp->currentItem();
}

void StoryEditor::addPar(int where, QString text, int sty)
{
	table1->insertRows(where);
	QComboBox *ct = new QComboBox( this );
	ct->insertStringList(style);
	ct->setMaximumSize(200, 24);
	ct->setEditable(false);
  table1->setCellWidget(where, 0, ct);
	SEditor *cp = new SEditor(this);
	stList.insert(where, ct);
	edList.insert(where, cp);
 	table1->setCellWidget(where, 1, cp);
	table1->setCurrentCell(where, 1);
  cp->setText(text);
	table1->adjHeight(where);
	connect(ct, SIGNAL(highlighted(int)), this, SLOT(styleChange(int)));
	ct->setCurrentItem(sty);
	disconnect(ct, SIGNAL(highlighted(int)), this, SLOT(styleChange(int)));
	cp->setFocus();
	cp->setCursorPosition(0, 0);
	connect(cp, SIGNAL(wrapped()), this, SLOT(WrapHandler()));
	connect(cp, SIGNAL(delPressed()), this, SLOT(KeyDel()));
	connect(cp, SIGNAL(bsPressed()), this, SLOT(KeyBS()));
	connect(cp, SIGNAL(returnPressed()), this, SLOT(KeyRet()));
	connect(cp, SIGNAL(clicked(int, int)), this, SLOT(clickAt(int, int)));
	connect(cp, SIGNAL(textChanged()), this, SLOT(modifiedText()));
	connect(ct, SIGNAL(activated(int)), this, SLOT(styleChange(int)));
}

void StoryEditor::modifiedText()
{
	TextChanged = true;
}

void StoryEditor::WrapHandler()
{
	int r = edList.findRef((QTextEdit*)sender());
	if (r != -1)
		table1->adjHeight(r);
}

void StoryEditor::clickAt( int row, int col)
{
	int r = edList.findRef((QTextEdit*)sender());
	if (r != -1)
		{
		table1->setCurrentCell(r, 1);
		table1->cellWidget(r, 1)->setFocus();
		table1->updateHeaderStates();
		}
}

void StoryEditor::KeyDel()
{
	QString tmp, tmp2;
	int r = table1->currentRow();
	QTextEdit *tt = (QTextEdit*)sender();
	tmp = tt->text();
	if (r < table1->numRows()-1)
		{
		int al = tt->alignment();
		QTextEdit *bt = edList.at(r+1);
		tmp2 = bt->text();
		tt->setText(tmp + tmp2);
		tt->setAlignment(al);
		edList.remove(r+1);
		stList.remove(r+1);
		table1->removeRow(r+1);
		table1->setCurrentCell(r, 1);
		table1->adjHeight(r);
		tt->setFocus();
		tt->setCursorPosition(0, tmp.length());
		}
}

void StoryEditor::KeyBS()
{
	QString tmp, tmp2;
	int r = table1->currentRow();
	QTextEdit *tt = (QTextEdit*)sender();
	tmp = tt->text();
	if (r > 0)
		{
		QTextEdit *bt = edList.at(r-1);
		int al = bt->alignment();
		tmp2 = bt->text();
		bt->setText(tmp2 + tmp);
		bt->setAlignment(al);
		edList.remove(r);
		stList.remove(r);
		table1->removeRow(r);
		table1->setCurrentCell(r-1, 1);
		table1->adjHeight(r-1);
		bt->setFocus();
		bt->setCursorPosition(0, tmp2.length());
		}
}

void StoryEditor::KeyRet()
{
	QString tmp, tmp2;
	QString tmp3 = "";
	QTextEdit *tt = (QTextEdit*)sender();
	tmp = tt->text();
	int al = tt->alignment();
	int st = getStyle(table1->currentRow());
	int rPos = tmp.find("\n");
	if (rPos < static_cast<int>(tmp.length()))
		{
		tmp2 = tmp.left(rPos);
		tmp3 = tmp.mid(rPos+1);
		tt->setText(tmp2);
		}
	else
		{
		tmp.remove("\n");
		tt->setText(tmp);
		}
	tt->setAlignment(al);
	table1->adjHeight(table1->currentRow());
	addPar(table1->currentRow()+1, tmp3, st);
}

