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
#include <qtooltip.h>
#include <qpixmap.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qhbox.h>
#include <qcolordialog.h>
#include <qfontdialog.h>
#include <qcursor.h>
#include "serializer.h"
#include "customfdialog.h"
#include "search.h"
#include "scribus.h"
#include "prefscontext.h"
#include "prefsfile.h"

extern PrefsFile* prefsFile;
extern QPixmap loadIcon(QString nam);
extern ScribusApp* ScApp;

SideBar::SideBar(QWidget *pa) : QLabel(pa)
{
	setEraseColor(QColor(255,255,255));
	offs = 0;
	editor = 0;
	noUpdt = true;
	inRep = false;
	setMinimumWidth(fontMetrics().width( tr("No Style") )+30);
}

void SideBar::mouseReleaseEvent(QMouseEvent *m)
{
	CurrentPar = editor->paragraphAt(QPoint(2, m->y()+offs));
	pmen = new QPopupMenu();
	Spalette* Spal = new Spalette(this);
	Spal->setFormats(editor->doc);
	if ((CurrentPar < static_cast<int>(editor->StyledText.count())) && (editor->StyledText.count() != 0))
	{
		if (editor->StyledText.at(CurrentPar)->count() > 0)
			Spal->setFormat(editor->StyledText.at(CurrentPar)->at(0)->cab);
		else
			Spal->setFormat(0);
	}
	else
		Spal->setFormat(0);
	connect(Spal, SIGNAL(newStyle(int)), this, SLOT(setPStyle(int)));
	pmen->insertItem(Spal);
	pmen->exec(QCursor::pos());
	delete pmen;
}

void SideBar::setPStyle(int s)
{
	emit ChangeStyle(CurrentPar, s);
	pmen->activateItemAt(0);
}

void SideBar::paintEvent(QPaintEvent *e)
{
	inRep = true;
	QLabel::paintEvent(e);
	QPainter p;
	p.begin(this);
	if ((editor != 0) && (noUpdt))
	{
		int st = editor->CurrentABStil;
		for (int pa = 0; pa < editor->paragraphs(); ++pa)
		{
			QRect re = editor->paragraphRect(pa);
			if (!re.isValid())
				break;
			re.setWidth(width()-5);
			re.moveBy(5, 0);
			if (((re.y()+re.height())-offs < height()) && ((re.y()+re.height())-offs > 0))
				p.drawLine(0, (re.y()+re.height())-offs, width()-1, (re.y()+re.height())-offs);
			if ((re.y()-offs < height()) && (re.y()-offs > 0))
			{
				re.setY(re.y()-offs);
				if ((pa < static_cast<int>(editor->StyledText.count())) && (editor->StyledText.count() != 0))
				{
					if (editor->StyledText.at(pa)->count() > 0)
					{
						st = editor->StyledText.at(pa)->at(0)->cab;
						if (st < 5)
							p.drawText(re, Qt::AlignLeft | Qt::AlignTop, tr("No Style"));
						else
							p.drawText(re, Qt::AlignLeft | Qt::AlignTop, editor->doc->Vorlagen[st].Vname);
					}
					else
					{
						st = editor->ParagStyles[pa];
						if (st < 5)
							p.drawText(re, Qt::AlignLeft | Qt::AlignTop, tr("No Style"));
						else
							p.drawText(re, Qt::AlignLeft | Qt::AlignTop, editor->doc->Vorlagen[st].Vname);
					}
				}
				else
				{
					st = editor->CurrentABStil;
					if (st < 5)
						p.drawText(re, Qt::AlignLeft | Qt::AlignTop, tr("No Style"));
					else
						p.drawText(re, Qt::AlignLeft | Qt::AlignTop, editor->doc->Vorlagen[st].Vname);
				}
			}
		}
	}
	p.end();
	inRep = false;
}

void SideBar::doMove(int, int y)
{
	offs = y;
	if (!inRep)
		update();
}

void SideBar::doRepaint()
{
	if (!inRep)
		update();
}

void SideBar::setRepaint(bool r)
{
	noUpdt = r;
}

SEditor::SEditor(QWidget* parent, ScribusDoc *docc) : QTextEdit(parent)
{
	doc = docc;
	wasMod = false;
	StoredSel = false;
	StyledText.clear();
	StyledText.setAutoDelete(true);
	ParagStyles.clear();
	cBuffer.setAutoDelete(true);
	cBuffer.clear();
	setUndoRedoEnabled(true);
	setUndoDepth(0);
	setTextFormat(Qt::PlainText);
	viewport()->setAcceptDrops(false);
	ClipData = 0;
	UniCinp = false;
	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
	connect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(SelClipChange()));
}

void SEditor::keyPressEvent(QKeyEvent *k)
{
	emit SideBarUp(false);
	int p, i;
	getCursorPosition(&p, &i);
	int KeyMod;
	switch (k->state())
	{
	case ShiftButton:
		KeyMod = 0x00200000;
		break;
	case AltButton:
		KeyMod = 0x00800000;
		break;
	case ControlButton:
		KeyMod = 0x00400000;
		break;
	default:
		KeyMod = 0;
		break;
	}
	if ((k->key() + KeyMod) == ScApp->Prefs.KeyActions[60].KeyID)
	{
		insChars(QString(QChar(30)));
		insert("#");
		emit SideBarUp(true);
		return;
	}
	switch (k->state())
	{
		case ControlButton:
		case ControlButton|ShiftButton:
		case ControlButton|Keypad:
		case ControlButton|ShiftButton|Keypad:
			break;
		case NoButton:
		case Keypad:
		case ShiftButton:
			if (UniCinp)
			{
				int conv = 0;
				bool ok = false;
				UniCinS += k->text();
				conv = UniCinS.toInt(&ok, 16);
				if (!ok)
				{
					UniCinp = false;
					UniCinC = 0;
					UniCinS = "";
					return;
				}
				UniCinC++;
				if (UniCinC == 4)
				{
					UniCinp = false;
					UniCinC = 0;
					UniCinS = "";
					if (ok)
					{
						if (conv < 31)
							conv = 32;
						insChars(QString(QChar(conv)));
 						insert(QString(QChar(conv)));
						emit SideBarUp(true);
						return;
					}
				}
				else
				{
					emit SideBarUp(true);
					return;
				}
			}
			wasMod = false;
			switch (k->key())
			{
				case Key_Shift:
				case Key_Control:
				case Key_Alt:
					wasMod = true;
					break;
				case Key_F12:
					UniCinp = true;
					UniCinC = 0;
					UniCinS = "";
					return;
					break;
				case Key_Delete:
					if (!hasSelectedText())
					{
						ChList *chars = StyledText.at(p);
						if (i < static_cast<int>(chars->count()))
							chars->remove(i);
						else
						{
							if (p < static_cast<int>(StyledText.count()-1))
							{
								struct PtiSmall *hg;
								ChList *chars2 = StyledText.at(p+1);
								int a = static_cast<int>(chars2->count());
								if (a > 0)
								{
									int ca;
									if (chars->count() > 0)
										ca = chars->at(0)->cab;
									else
										ca = CurrentABStil;
									for (int s = 0; s < a; ++s)
									{
										hg = chars2->take(0);
										hg->cab = ca;
										chars->append(hg);
									}
								}
								StyledText.remove(p+1);
								ParagStyles.remove(ParagStyles.at(p+1));
							}
						}
					}
					else
						deleteSel();
					break;
				case Key_Backspace:
					if (!hasSelectedText())
					{
						if (p >= static_cast<int>(StyledText.count()))
							break;
						ChList *chars = StyledText.at(p);
						if (i > 0)
							chars->remove(i-1);
						else
						{
							if (p > 0)
							{
								struct PtiSmall *hg;
								ChList *chars2 = StyledText.at(p-1);
								int a = static_cast<int>(chars->count());
								if (a > 0)
								{
									int ca;
									if (chars2->count() > 0)
										ca = chars2->at(0)->cab;
									else
										ca = chars->at(0)->cab;
									for (int s = 0; s < a; ++s)
									{
										hg = chars->take(0);
										hg->cab = ca;
										chars2->append(hg);
									}
								}
								StyledText.remove(p);
								ParagStyles.remove(ParagStyles.at(p));
							}
						}
					}
					else
						deleteSel();
					break;
				case Key_Return:
				case Key_Enter:
					{
						if (hasSelectedText())
							deleteSel();
						ChList *chars;
						chars = new ChList;
						chars->setAutoDelete(true);
						chars->clear();
						if (StyledText.count() != 0)
						{
							if (p >= static_cast<int>(StyledText.count()))
							{
								StyledText.append(chars);
								ParagStyles.append(CurrentABStil);
							}
							else
							{
							ChList *chars2 = StyledText.at(p);
							int a = static_cast<int>(chars2->count());
							for (int s = i; s < a; ++s)
							{
								chars->append(chars2->take(i));
							}
							StyledText.insert(p+1, chars);
							ParagStyles.insert(ParagStyles.at(p+1), CurrentABStil);
							}
						}
						else
						{
							StyledText.append(chars);
							ParagStyles.append(CurrentABStil);
						}
					}
					break;
				case Key_Left:
				case Key_Right:
				case Key_Prior:
				case Key_Next:
				case Key_Up:
				case Key_Down:
				case Key_Home:
				case Key_End:
					break;
				default:
					if (k->text() != "")
						insChars(k->text());
					break;
			}
			break;
		default:
			break;
	}
	QTextEdit::keyPressEvent(k);
	emit SideBarUp(true);
	emit SideBarUpdate();
}

void SEditor::focusOutEvent(QFocusEvent *e)
{
	if (hasSelectedText())
	{
		getSelection(&SelParaStart, &SelCharStart, &SelParaEnd, &SelCharEnd);
		StoredSel = true;
	}
	else
		StoredSel = false;
	QTextEdit::focusOutEvent(e);
}

void SEditor::insChars(QString t)
{
	int p, i, p2, ccab;
	if (hasSelectedText())
		deleteSel();
	getCursorPosition(&p, &i);
	ChList *chars;
	p2 = p;
	if ((p >= static_cast<int>(StyledText.count())) || (StyledText.count() == 0))
	{
		chars = new ChList;
		chars->setAutoDelete(true);
		chars->clear();
		p2 = static_cast<int>(StyledText.count());
		StyledText.append(chars);
		ParagStyles.append(CurrentABStil);
	}
	else
		chars = StyledText.at(p);
	if (chars->count() != 0)
		ccab = chars->at(0)->cab;
	else
		ccab = CurrentABStil;
	for (uint a = 0; a < t.length(); ++a)
	{
		if (t[a] == QChar(13))
		{
			ChList *chars2;
			chars2 = new ChList;
			chars2->setAutoDelete(true);
			chars2->clear();
			if (p2 >= static_cast<int>(StyledText.count()))
			{
				StyledText.append(chars2);
				ParagStyles.append(ccab);
			}
			else
			{
				int a = static_cast<int>(chars->count());
				for (int s = i; s < a; ++s)
				{
					chars2->append(chars->take(i));
				}
				StyledText.insert(p2+1, chars2);
				ParagStyles.insert(ParagStyles.at(p2+1), ccab);
			}
			p2++;
			chars = StyledText.at(p2);
			i = 0;
		}
		else
		{
			struct PtiSmall *hg;
			hg = new PtiSmall;
			hg->ch = t[a];
			hg->ccolor = CurrTextFill;
			hg->cshade = CurrTextFillSh;
			hg->cstroke = CurrTextStroke;
			hg->cshade2 = CurrTextStrokeSh;
			hg->cfont = CurrFont;
			hg->csize = CurrFontSize;
			hg->cstyle = CurrentStyle;
			hg->cab = ccab;
			hg->cextra = CurrTextKern;
			hg->cscale = CurrTextScale;
			chars->insert(i, hg);
			i++;
		}
	}
}

void SEditor::insStyledText()
{
	if (cBuffer.count() == 0)
		return;
	int p, i, p2, ccab;
	if (hasSelectedText())
		deleteSel();
	getCursorPosition(&p, &i);
	ChList *chars;
	p2 = p;
	if ((p >= static_cast<int>(StyledText.count())) || (StyledText.count() == 0))
	{
		chars = new ChList;
		chars->setAutoDelete(true);
		chars->clear();
		p2 = static_cast<int>(StyledText.count());
		StyledText.append(chars);
		ParagStyles.append(CurrentABStil);
	}
	else
		chars = StyledText.at(p);
	if (chars->count() != 0)
		ccab = chars->at(0)->cab;
	else
		ccab = CurrentABStil;
	for (uint a = 0; a < cBuffer.count()-1; ++a)
	{
		struct PtiSmall *hg;
		if (cBuffer.at(a)->ch == QChar(13))
		{
			ChList *chars2;
			chars2 = new ChList;
			chars2->setAutoDelete(true);
			chars2->clear();
			if (p2 >= static_cast<int>(StyledText.count()))
			{
				StyledText.append(chars2);
				ParagStyles.append(ccab);
			}
			else
			{
				int a = static_cast<int>(chars->count());
				for (int s = i; s < a; ++s)
				{
					chars2->append(chars->take(i));
				}
				StyledText.insert(p2+1, chars2);
				ParagStyles.insert(ParagStyles.at(p2+1), ccab);
			}
			p2++;
			chars = StyledText.at(p2);
			i = 0;
		}
		else
		{
			hg = new PtiSmall;
			hg->ch = cBuffer.at(a)->ch;
			hg->ccolor = cBuffer.at(a)->ccolor;
			hg->cshade = cBuffer.at(a)->cshade;
			hg->cstroke = cBuffer.at(a)->cstroke;
			hg->cshade2 = cBuffer.at(a)->cshade2;
			hg->cfont = cBuffer.at(a)->cfont;
			hg->csize = cBuffer.at(a)->csize;
			hg->cstyle = cBuffer.at(a)->cstyle;
			hg->cab = ccab;
			hg->cextra = cBuffer.at(a)->cextra;
			hg->cscale = cBuffer.at(a)->cscale;
			chars->insert(i, hg);
			i++;
		}
	}
}

void SEditor::copyStyledText()
{
	int PStart, PEnd, SelStart, SelEnd, start, end;
	ChList *chars;
	struct PtiSmall *hg;
	cBuffer.clear();
	getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
	for (int pa = PStart; pa < PEnd+1; ++pa)
	{
		chars = StyledText.at(pa);
		if (pa == PStart)
			start = SelStart;
		else
			start = 0;
		if (pa == PEnd)
			end = SelEnd;
		else
			end = chars->count();
		for (int ca = start; ca < end; ++ca)
		{
			hg = new PtiSmall;
			hg->ch = chars->at(ca)->ch;
			hg->cfont = chars->at(ca)->cfont;
			hg->csize = chars->at(ca)->csize;
			hg->ccolor = chars->at(ca)->ccolor;
			hg->cshade = chars->at(ca)->cshade;
			hg->cstroke = chars->at(ca)->cstroke;
			hg->cshade2 = chars->at(ca)->cshade2;
			hg->cscale = chars->at(ca)->cscale;
			hg->cstyle = chars->at(ca)->cstyle;
			hg->cab = chars->at(ca)->cab;
			hg->cextra = chars->at(ca)->cextra;
			cBuffer.append(hg);
		}
		hg = new PtiSmall;
		hg->ch = QChar(13);
		hg->cfont = "";
		hg->csize = 1;
		hg->ccolor = "";
		hg->cshade = 1;
		hg->cstroke = "";
		hg->cshade2 = 1;
		hg->cscale = 0;
		hg->cstyle = 0;
		hg->cab = 0;
		hg->cextra = 0;
		cBuffer.append(hg);
	}
}

void SEditor::saveItemText(PageItem* b)
{
	ChList *chars;
	b->CPos = 0;
	b->Ptext.clear();
	uint c = 0;
	for (uint p = 0; p < StyledText.count(); ++p)
	{
		if (p != 0)
		{
			c = StyledText.at(p-1)->count()-1;
			struct Pti *hg;
			hg = new Pti;
			hg->ch = QChar(13);
			chars = StyledText.at(p-1);
			if (chars->count() != 0)
			{
				hg->cfont = chars->at(c)->cfont;
				hg->csize = chars->at(c)->csize;
				hg->ccolor = chars->at(c)->ccolor;
				hg->cshade = chars->at(c)->cshade;
				hg->cstroke = chars->at(c)->cstroke;
				hg->cshade2 = chars->at(c)->cshade2;
				hg->cscale = chars->at(c)->cscale;
				hg->cstyle = chars->at(c)->cstyle;
				hg->cextra = chars->at(c)->cextra;
			}
			else
			{
				hg->ccolor = CurrTextFill;
				hg->cshade = CurrTextFillSh;
				hg->cstroke = CurrTextStroke;
				hg->cshade2 = CurrTextStrokeSh;
				hg->cfont = CurrFont;
				hg->csize = CurrFontSize;
				hg->cstyle = CurrentStyle;
				hg->cextra = CurrTextKern;
				hg->cscale = CurrTextScale;
				if (doc->Vorlagen[ParagStyles[p-1]].Font != "")
				{
					hg->cfont = doc->Vorlagen[ParagStyles[p-1]].Font;
					hg->csize = doc->Vorlagen[ParagStyles[p-1]].FontSize;
				}
			}
			hg->cab = ParagStyles[p-1];
			hg->cselect = false;
			hg->xp = 0;
			hg->yp = 0;
			hg->PRot = 0;
			hg->PtransX = 0;
			hg->PtransY = 0;
			b->Ptext.append(hg);
		}
		chars = StyledText.at(p);
		for (uint c = 0; c < chars->count(); ++c)
		{
			struct Pti *hg;
			hg = new Pti;
			hg->ch = chars->at(c)->ch;
			hg->cfont = chars->at(c)->cfont;
			hg->csize = chars->at(c)->csize;
			hg->ccolor = chars->at(c)->ccolor;
			hg->cshade = chars->at(c)->cshade;
			hg->cstroke = chars->at(c)->cstroke;
			hg->cshade2 = chars->at(c)->cshade2;
			hg->cscale = chars->at(c)->cscale;
			hg->cstyle = chars->at(c)->cstyle;
			hg->cab = chars->at(c)->cab;
			hg->cextra = chars->at(c)->cextra;
			hg->cselect = false;
			hg->xp = 0;
			hg->yp = 0;
			hg->PRot = 0;
			hg->PtransX = 0;
			hg->PtransY = 0;
			b->Ptext.append(hg);
		}
	}
}

void SEditor::setAlign(int style)
{
	int align = 0;
	if (style > 4)
		align = doc->Vorlagen[style].Ausri;
	else
		align = style;
	switch (align)
	{
	case 0:
		setAlignment(Qt::AlignLeft);
		break;
	case 1:
		setAlignment(Qt::AlignCenter);
		break;
	case 2:
		setAlignment(Qt::AlignRight);
		break;
	case 3:
	case 4:
		setAlignment(Qt::AlignJustify);
		break;
	default:
		break;
	}
}

void SEditor::loadItemText(PageItem* b)
{
	struct PtiSmall *hg;
	QString Text = "";
	QString Ccol = "";
	int Csha = 100;
	int Csty = 0;
	int Ali = 0;
	PageItem *nb = b;
	StyledText.clear();
	ParagStyles.clear();
	ChList *chars;
	chars = new ChList;
	chars->setAutoDelete(true);
	chars->clear();
	while (nb != 0)
	{
		if (nb->BackBox != 0)
			nb = nb->BackBox;
		else
			break;
	}
	if (nb != 0)
	{
		if (nb->Ptext.count() != 0)
		{
			Ccol = nb->Ptext.at(0)->ccolor;
			Csha = nb->Ptext.at(0)->cshade;
			Csty = nb->Ptext.at(0)->cstyle;
			Ali = nb->Ptext.at(0)->cab;
		}
		else
		{
			Ccol = b->TxtFill;
			Csha = b->ShTxtFill;
			Csha = b->TxTStyle;
			Ali = b->Ausrich;
		}
		setAlign(Ali);
		setFarbe(Ccol, Csha);
		setStyle(Csty);
	}
	while (nb != 0)
	{
		for (uint a = 0; a < nb->Ptext.count(); ++a)
		{
			if (nb->Ptext.at(a)->ch == QChar(13))
			{
				StyledText.append(chars);
				ParagStyles.append(Ali);
				chars = new ChList;
				chars->setAutoDelete(true);
				chars->clear();
				Text += "\n";
			}
			else
			{
				hg = new PtiSmall;
				hg->ch = nb->Ptext.at(a)->ch;
				hg->cfont = nb->Ptext.at(a)->cfont;
				hg->csize = nb->Ptext.at(a)->csize;
				hg->ccolor = nb->Ptext.at(a)->ccolor;
				hg->cshade = nb->Ptext.at(a)->cshade;
				hg->cstroke = nb->Ptext.at(a)->cstroke;
				hg->cshade2 = nb->Ptext.at(a)->cshade2;
				hg->cscale = nb->Ptext.at(a)->cscale;
				hg->cstyle = nb->Ptext.at(a)->cstyle;
				hg->cab = nb->Ptext.at(a)->cab;
				hg->cextra = nb->Ptext.at(a)->cextra;
				if ((Ccol == hg->ccolor) && (Ali == hg->cab) && (Csha == hg->cshade) && (Csty == hg->cstyle))
				{
					if (hg->ch == QChar(30))
					{
						setFarbe(Ccol, Csha);
						setAlign(Ali);
						setStyle(Csty);
						insert(Text);
						insert("#");
						Text = "";
						chars->append(hg);
						continue;
					}
					else
						Text += hg->ch;
				}
				else
				{
					setFarbe(Ccol, Csha);
					setAlign(Ali);
					setStyle(Csty);
					insert(Text);
					Text = hg->ch;
					Ccol = hg->ccolor;
					Csha = hg->cshade;
					Csty = hg->cstyle;
					Ali = hg->cab;
				}
				chars->append(hg);
			}
		}
		nb = nb->NextBox;
	}
	setAlign(Ali);
	setFarbe(Ccol, Csha);
	setStyle(Csty);
	insert(Text);
	if (chars->count() != 0)
	{
		StyledText.append(chars);
		ParagStyles.append(Ali);
	}
	if (StyledText.count() != 0)
		emit setProps(0, 0);
	setCursorPosition(0, 0);
}

void SEditor::loadText(QString tx, PageItem* b)
{
	struct PtiSmall *hg;
	QString Text = "";
	StyledText.clear();
	ParagStyles.clear();
	ChList *chars;
	chars = new ChList;
	chars->setAutoDelete(true);
	chars->clear();
	setAlign(b->Ausrich);
	setFarbe(b->TxtFill, b->ShTxtFill);
	setStyle(b->TxTStyle);
	for (uint a = 0; a < tx.length(); ++a)
	{
		if (tx[a] == QChar(13))
		{
			StyledText.append(chars);
			ParagStyles.append(b->Ausrich);
			chars = new ChList;
			chars->setAutoDelete(true);
			chars->clear();
			Text += "\n";
		}
		else
		{
			hg = new PtiSmall;
			hg->ch = tx[a];
			hg->cfont = b->IFont;
			hg->csize = b->ISize;
			hg->ccolor = b->TxtFill;
			hg->cshade = b->ShTxtFill;
			hg->cstroke = b->TxtStroke;
			hg->cshade2 = b->ShTxtStroke;
			hg->cscale = b->TxtScale;
			hg->cstyle = b->TxTStyle;
			hg->cab = b->Ausrich;
			hg->cextra = 0;
			Text += hg->ch;
			chars->append(hg);
		}
	}
	insert(Text);
	if (chars->count() != 0)
	{
		StyledText.append(chars);
		ParagStyles.append(b->Ausrich);
	}
	if (StyledText.count() != 0)
		emit setProps(0, 0);
	setCursorPosition(0, 0);
}

void SEditor::updateAll()
{
	if (StyledText.count() == 0)
		return;
	int p, i;
	getCursorPosition(&p, &i);
	clear();
	struct PtiSmall *hg;
	QString Text = "";
	QString Ccol = "";
	int Csha;
	int Csty;
	int Ali = 0;
	ChList *chars = StyledText.at(0);
	if (chars->count() != 0)
	{
		Ccol = chars->at(0)->ccolor;
		Csha = chars->at(0)->cshade;
		Csty = chars->at(0)->cstyle;
		Ali = chars->at(0)->cab;
	}
	else
	{
		Ccol = CurrTextFill;
		Csha = CurrTextFillSh;
		Csty = CurrentStyle;
		Ali = CurrentABStil;
	}
	setAlign(Ali);
	setFarbe(Ccol, Csha);
	setStyle(Csty);
	for (uint pa = 0; pa < StyledText.count(); ++pa)
	{
		chars = StyledText.at(pa);
		if ((chars->count() == 0) && (pa < StyledText.count()-1))
		{
			Text += "\n";
			continue;
		}
		for (uint a = 0; a < chars->count(); ++a)
		{
			hg = chars->at(a);
			if ((Ccol == hg->ccolor) && (Ali == hg->cab) && (Csha == hg->cshade) && (Csty == hg->cstyle))
			{
				if (hg->ch == QChar(30))
				{
					setFarbe(Ccol, Csha);
					setAlign(Ali);
					setStyle(Csty);
					insert(Text);
					insert("#");
					Text = "";
					continue;
				}
				else
					Text += hg->ch;
			}
			else
			{
				setFarbe(Ccol, Csha);
				setAlign(Ali);
				setStyle(Csty);
				insert(Text);
				Text = hg->ch;
				Ccol = hg->ccolor;
				Csha = hg->cshade;
				Csty = hg->cstyle;
				Ali = hg->cab;
			}
		}
		if (pa < StyledText.count()-1)
			Text += "\n";
	}
	setAlign(Ali);
	setFarbe(Ccol, Csha);
	setStyle(Csty);
	insert(Text);
	setCursorPosition(p, i);
}

void SEditor::updateFromChars(int pa)
{
	ChList *chars = StyledText.at(pa);
	if (chars->count() == 0)
		return;
	int SelStart = 0;
	int SelEnd = 0;
	int p, i;
	getCursorPosition(&p, &i);
	removeSelection();
	QString Ccol = chars->at(0)->ccolor;
	int Csha = chars->at(0)->cshade;
	int Csty = chars->at(0)->cstyle;
	for (uint a = 0; a < chars->count(); ++a)
	{
		if ((Ccol == chars->at(a)->ccolor) && (Csha == chars->at(a)->cshade) && (Csty == chars->at(a)->cstyle))
			SelEnd++;
		else
		{
			setSelection(pa, SelStart, pa, SelEnd);
			setFarbe(Ccol, Csha);
			setStyle(Csty);
			removeSelection();
			Ccol = chars->at(a)->ccolor;
			Csha = chars->at(a)->cshade;
			Csty = chars->at(a)->cstyle;
			SelStart = SelEnd;
			SelEnd++;
		}
	}
	setSelection(pa, SelStart, pa, SelEnd);
	setFarbe(Ccol, Csha);
	setStyle(Csty);
	removeSelection();
	setAlign(chars->at(0)->cab);
	setCursorPosition(p, i);
}

/* updates the internal PtiSmall structure, to be useable for all members of the PtiSmall struct
   there is a code as first parameter to indicate which member should be updated.
	0 = Fill Colour and Fill Shade
	1 = Stroke Colour and Stroke Shade
	2 = Font
	3 = Font Size
	4 = Character Style
	5 = Character Scaling
	6 = Kerning
 */
void SEditor::updateSel(int code, struct PtiSmall *hg)
{
	int PStart, PEnd, SelStart, SelEnd, start, end;
	ChList *chars;
	if (StoredSel)
	{
		setSelection(SelParaStart, SelCharStart, SelParaEnd, SelCharEnd);
		StoredSel = false;
	}
	getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
	for (int pa = PStart; pa < PEnd+1; ++pa)
	{
		chars = StyledText.at(pa);
		if (pa == PStart)
			start = SelStart;
		else
			start = 0;
		if (pa == PEnd)
			end = SelEnd;
		else
			end = chars->count();
		for (int ca = start; ca < end; ++ca)
		{
			switch (code)
			{
				case 0:
					chars->at(ca)->ccolor = hg->ccolor;
					chars->at(ca)->cshade = hg->cshade;
					break;
				case 1:
					chars->at(ca)->cstroke = hg->cstroke;
					chars->at(ca)->cshade2 = hg->cshade2;
					break;
				case 2:
					chars->at(ca)->cfont = hg->cfont;
					break;
				case 3:
					chars->at(ca)->csize = hg->csize;
					break;
				case 4:
					chars->at(ca)->cstyle &= ~127;
					chars->at(ca)->cstyle |= hg->cstyle;
					break;
				case 5:
					chars->at(ca)->cscale = hg->cscale;
					break;
				case 6:
					chars->at(ca)->cextra = hg->cextra;
					break;
				default:
					break;
			}
		}
	}
}

void SEditor::deleteSel()
{
	int PStart, PEnd, SelStart, SelEnd, start, end;
	ChList *chars = NULL;
	getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
	if (PStart == PEnd)
	{
		chars = StyledText.at(PStart);
		for (int a = 0; a < SelEnd-SelStart; ++a)
		{
			chars->remove(SelStart);
		}
	}
	else
	{
		for (int pa = PStart; pa < PEnd+1; ++pa)
		{
			bool noChar = false;
			if (pa >= static_cast<int>(StyledText.count()))
				noChar = true;
			else
				chars = StyledText.at(pa);
			if (pa == PStart)
				start = SelStart;
			else
				start = 0;
			if (pa == PEnd)
				end = SelEnd;
			else
			{
				if (noChar)
					end = 0;
				else
					end = chars->count();
			}
			if (!noChar)
			{
				for (int ca = 0; ca < end-start; ++ca)
				{
					chars->remove(start);
				}
			}
		}
		if (PEnd-PStart > 1)
		{
			for (int pa2 = 0; pa2 < PEnd - PStart - 1; ++pa2)
			{
				if (PStart+1 < static_cast<int>(StyledText.count()))
				{
					StyledText.remove(PStart+1);
					ParagStyles.remove(ParagStyles.at(PStart+1));
				}
			}
		}
		if (PStart+1 < static_cast<int>(StyledText.count()))
		{
			struct PtiSmall *hg;
			ChList *chars2 = StyledText.at(PStart+1);
			chars = StyledText.at(PStart);
			int a = static_cast<int>(chars2->count());
			if (a > 0)
			{
				int ca;
				if (chars->count() > 0)
					ca = chars->at(0)->cab;
				else
					ca = CurrentABStil;
				for (int s = 0; s < a; ++s)
				{
					hg = chars2->take(0);
					hg->cab = ca;
					chars->append(hg);
				}
			}
			StyledText.remove(PStart+1);
			ParagStyles.remove(ParagStyles.at(PStart+1));
		}
	}
	setCursorPosition(PStart, SelStart);
}

void SEditor::setStyle(int Csty)
{
	if (Csty & 8)
		setUnderline(true);
	else
		setUnderline(false);
	QFont f = currentFont();
	if (Csty & 16)
		f.setStrikeOut(true);
	else
		f.setStrikeOut(false);
	setCurrentFont(f);
	if (Csty & 1)
		setVerticalAlignment(AlignSuperScript);
	else if (Csty & 2)
		setVerticalAlignment(AlignSubScript);
	else
		setVerticalAlignment(AlignNormal);
}

void SEditor::setFarbe(QString farbe, int shad)
{
	int h, s, v, sneu;
	QColor tmp;
	doc->PageColors[farbe].getRGBColor().rgb(&h, &s, &v);
	if ((h == s) && (s == v))
	{
		doc->PageColors[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = 255 - ((255 - v) * shad / 100);
		tmp.setHsv(h, s, sneu);
	}
	else
	{
		doc->PageColors[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = s * shad / 100;
		tmp.setHsv(h, sneu, v);
	}
	setColor(tmp);
}

void SEditor::copy()
{
	emit SideBarUp(false);
	if ((hasSelectedText()) && (selectedText() != ""))
	{
		disconnect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
		disconnect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(SelClipChange()));
		tBuffer = selectedText();
		copyStyledText();
		QApplication::clipboard()->setText(tBuffer, QClipboard::Clipboard);
		ClipData = 1;
		connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
		connect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(SelClipChange()));
		emit PasteAvail();
	}
	emit SideBarUp(true);
}

void SEditor::cut()
{
	copy();
	emit SideBarUp(false);
	if (hasSelectedText())
	{
		deleteSel();
		removeSelectedText();
	}
	emit SideBarUp(true);
	emit SideBarUpdate();
}

void SEditor::paste()
{
	emit SideBarUp(false);
	int p, i;
	QString data = "";
	getCursorPosition(&p, &i);
	if (ClipData == 1)
		insStyledText();
	else
	{
		QString data = QApplication::clipboard()->text(QClipboard::Selection);
		if (data.isNull())
			data = QApplication::clipboard()->text(QClipboard::Clipboard);
		if (!data.isNull())
		{
			data.replace(QRegExp("\r"), "");
			data.replace(QRegExp("\n"), QChar(13));
			insChars(data);
			ClipData = 2;
			emit PasteAvail();
		}
		else
		{
			emit SideBarUp(true);
			return;
		}
	}
	updateAll();
	emit SideBarUp(true);
	emit SideBarUpdate();
}

QPopupMenu* SEditor::createPopupMenu(const QPoint & pos)
{
	QPopupMenu *p = QTextEdit::createPopupMenu(pos);
	p->removeItemAt(0);
	p->removeItemAt(0);
	p->removeItemAt(0);
	p->removeItemAt(3);
	return p;
}

void SEditor::SelClipChange()
{
	ClipData = 3;
	emit PasteAvail();
}

void SEditor::ClipChange()
{
	ClipData = 2;
	emit PasteAvail();
}

/* Toolbar for Fill Colour */
SToolBColorF::SToolBColorF(QMainWindow* parent, ScribusDoc *doc) : QToolBar( tr("Fill Color Settings"), parent)
{
	FillIcon = new QLabel( "", this, "FillIcon" );
	FillIcon->setPixmap(loadIcon("fill.png"));
	FillIcon->setScaledContents( false );
	TxFill = new QComboBox( true, this, "TxFill" );
	TxFill->setEditable(false);
	PM2 = new ShadeButton(this);
	TxFill->clear();
	CListe::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	TxFill->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		TxFill->insertItem(pm, it.key());
	}
	TxFill->listBox()->setMinimumWidth(TxFill->listBox()->maxItemWidth()+24);
	QToolTip::add( TxFill, tr( "Color of text fill" ) );
	QToolTip::add( PM2, tr( "Saturation of color of text fill" ) );
	connect(TxFill, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	connect(PM2, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
}

void SToolBColorF::SetColor(int c)
{
	disconnect(TxFill, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	TxFill->setCurrentItem(c);
	connect(TxFill, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
}

void SToolBColorF::SetShade(int s)
{
	disconnect(PM2, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
	PM2->setValue(s);
	connect(PM2, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
}

void SToolBColorF::newShadeHandler()
{
	emit NewColor(TxFill->currentItem(), PM2->getValue());
}

/* Toolbar for Stroke Colour */
SToolBColorS::SToolBColorS(QMainWindow* parent, ScribusDoc *doc) : QToolBar( tr("Stroke Color Settings"), parent)
{
	StrokeIcon = new QLabel( "", this, "StrokeIcon" );
	StrokeIcon->setPixmap(loadIcon("Stiftalt.xpm"));
	StrokeIcon->setScaledContents( false );
	TxStroke = new QComboBox( true, this, "TxStroke" );
	TxStroke->setEditable(false);
	PM1 = new ShadeButton(this);
	TxStroke->clear();
	CListe::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	TxStroke->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		TxStroke->insertItem(pm, it.key());
	}
	TxStroke->listBox()->setMinimumWidth(TxStroke->listBox()->maxItemWidth()+24);
	QToolTip::add( TxStroke, tr( "Color of text stroke" ) );
	QToolTip::add( PM1, tr( "Saturation of color of text stroke" ) );
	connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	connect(PM1, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
}

void SToolBColorS::SetColor(int c)
{
	disconnect(TxStroke, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	TxStroke->setCurrentItem(c);
	connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
}

void SToolBColorS::SetShade(int s)
{
	disconnect(PM1, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
	PM1->setValue(s);
	connect(PM1, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
}

void SToolBColorS::newShadeHandler()
{
	emit NewColor(TxStroke->currentItem(), PM1->getValue());
}

/* Toolbar for Character Style Settings */
SToolBStyle::SToolBStyle(QMainWindow* parent) : QToolBar( tr("Character Settings"), parent)
{
	SeStyle = new StyleSelect(this);
	kerningLabel = new QLabel( tr( "Kerning:" ), this, "kerningLabel" );
	Extra = new MSpinBox( this, 1 );
	Extra->setValues( -300, 300, 10, 0);
	Extra->setSuffix( tr( " pt" ) );
	QToolTip::add( Extra, tr( "Manual Kerning" ) );
	connect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
	connect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
}

void SToolBStyle::newKernHandler()
{
	emit NewKern(Extra->value());
}

void SToolBStyle::SetStyle(int s)
{
	disconnect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
	SeStyle->setStyle(s);
	connect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
}

void SToolBStyle::SetKern(double k)
{
	disconnect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
	Extra->setValue(k);
	connect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
}

/* Toolbar for alignment of Paragraphs */
SToolBAlign::SToolBAlign(QMainWindow* parent) : QToolBar( tr("Style Settings"), parent)
{
	GroupAlign = new AlignSelect(this);
	Spal = new Spalette(this);
	QToolTip::add( Spal, tr( "Style of current paragraph" ) );
	connect(Spal, SIGNAL(newStyle(int)), this, SLOT(newStyleHandler(int )));
	connect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(NewAlign(int )));
}

void SToolBAlign::newStyleHandler(int s)
{
	if (s != 0)
		GroupAlign->setEnabled(false);
	else
		GroupAlign->setEnabled(true);
	emit newStyle(s);
}

void SToolBAlign::SetAlign(int s)
{
	disconnect(Spal, SIGNAL(newStyle(int)), this, SLOT(newStyleHandler(int )));
	disconnect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(NewAlign(int )));
	if (s < 5)
	{
		GroupAlign->setEnabled(true);
		GroupAlign->setStyle(s);
	}
	else
		GroupAlign->setEnabled(false);
	Spal->setFormat(s);
	connect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(NewAlign(int )));
	connect(Spal, SIGNAL(newStyle(int)), this, SLOT(newStyleHandler(int )));
}

/* Toolbar for Font related Settings */
SToolBFont::SToolBFont(QMainWindow* parent) : QToolBar( tr("Font Settings"), parent)
{
	Fonts = new FontCombo(this, &ScApp->Prefs);
	Fonts->setMaximumSize(190, 30);
	Size = new MSpinBox( 1, 1024, this, 1 );
	Size->setPrefix( "" );
	Size->setSuffix( tr( " pt" ) );
	ChScale = new QSpinBox( this, "ChScale" );
	ChScale->setMaxValue( 400 );
	ChScale->setMinValue( 25 );
	ChScale->setValue( 100 );
	ChScale->setSuffix( tr( " %" ) );
	QToolTip::add( Fonts, tr( "Font of selected text" ) );
	QToolTip::add( Size, tr( "Font Size" ) );
	QToolTip::add( ChScale, tr( "Scaling width of characters" ) );
	connect(ChScale, SIGNAL(valueChanged(int)), this, SIGNAL(NewScale(int)));
	connect(Fonts, SIGNAL(activated(const QString &)), this, SIGNAL(NewFont(const QString &)));
	connect(Size, SIGNAL(valueChanged(int)), this, SLOT(newSizeHandler()));
}

void SToolBFont::SetFont(QString f)
{
	disconnect(Fonts, SIGNAL(activated(const QString &)), this, SIGNAL(NewFont(const QString &)));
	Fonts->setCurrentText(f);
	connect(Fonts, SIGNAL(activated(const QString &)), this, SIGNAL(NewFont(const QString &)));
}

void SToolBFont::SetSize(double s)
{
	disconnect(Size, SIGNAL(valueChanged(int)), this, SLOT(newSizeHandler()));
	Size->setValue(s);
	connect(Size, SIGNAL(valueChanged(int)), this, SLOT(newSizeHandler()));
}

void SToolBFont::SetScale(int s)
{
	disconnect(ChScale, SIGNAL(valueChanged(int)), this, SIGNAL(NewScale(int)));
	ChScale->setValue(s);
	connect(ChScale, SIGNAL(valueChanged(int)), this, SIGNAL(NewScale(int)));
}

void SToolBFont::newSizeHandler()
{
	emit NewSize(Size->value());
}

/* Main Story Editor Class */
StoryEditor::StoryEditor(QWidget* parent, ScribusDoc *docc, PageItem *ite) 
									: QMainWindow(parent, "StoryEditor", WShowModal | WType_Dialog)
{
	setCaption( tr( "Story Editor" ) );
	setIcon(loadIcon("AppIcon.png"));
	doc = docc;
	QHBox* vb = new QHBox( this );
	StoryEd2Layout = new QHBoxLayout( 0, 5, 5, "StoryEd2Layout");
/* Setting up Menu Bar */
	fmenu = new QPopupMenu();
	fmenu->insertItem(loadIcon("editdelete.png"), tr("&New"), this, SLOT(Do_new()), CTRL+Key_N);
	fid52 = fmenu->insertItem(loadIcon("reload16.png"),  tr("&Reload Text from Frame"), this, SLOT(slotFileRevert()));
	fmenu->insertSeparator();
	fmenu->insertItem(loadIcon("DateiSave16.png"), tr("&Save to File..."), this, SLOT(SaveTextFile()));
	fmenu->insertItem(loadIcon("DateiOpen16.png"), tr("&Load from File..."), this, SLOT(LoadTextFile()));
	fmenu->insertItem(tr("Save &Document"), this, SLOT(Do_saveDocument()), CTRL+Key_S);
	fmenu->insertSeparator();
	/* changes to fit the #662 bug 05/28/04 petr vanek */
	fmenu->insertItem(loadIcon("ok.png"),  tr("&Update Text Frame and Exit"), this, SLOT(Do_leave2()));
	fmenu->insertItem(loadIcon("exit.png"),  tr("&Exit Without Updating Text Frame"), this, SLOT(Do_leave()));
	/* end of changes */
	emenu = new QPopupMenu();
	emenu->insertItem( tr("Select &All"), this, SLOT(Do_selectAll()), CTRL+Key_A);
	Mcopy = emenu->insertItem(loadIcon("editcut.png"), tr("Cu&t"), this, SLOT(Do_cut()), CTRL+Key_X);
	Mcut = emenu->insertItem(loadIcon("editcopy.png"), tr("&Copy"), this, SLOT(Do_copy()), CTRL+Key_C);
	Mpaste = emenu->insertItem(loadIcon("editpaste.png"), tr("&Paste"), this, SLOT(Do_paste()), CTRL+Key_V);
	Mdel = emenu->insertItem(loadIcon("editdelete.png"), tr("C&lear"), this, SLOT(Do_del()), Key_Delete);
	emenu->insertSeparator();
	emenu->insertItem(loadIcon("find16.png"),  tr("&Search/Replace..."), this, SLOT(SearchText()));
	emenu->insertItem( tr("&Insert Special..."), this , SLOT(Do_insSp()));
	emenu->insertSeparator();
	emenu->insertItem( tr("&Edit Styles..."), this , SLOT(slotEditStyles()));
	emenu->insertItem( tr("&Fonts Preview..."), this , SLOT(Do_fontPrev()));
	Mupdt = emenu->insertItem(loadIcon("compfile16.png"),  tr("&Update Text Frame"), this, SLOT(updateTextFrame()), CTRL+Key_U);
	settingsMenu = new QPopupMenu();
	settingsMenu->insertItem( tr("&Background..."), this , SLOT(setBackPref()));
	settingsMenu->insertItem( tr("&Display Font..."), this , SLOT(setFontPref()));
	smartSel = settingsMenu->insertItem( tr("&Smart text selection"), this, SLOT(ToggleSmart()));
	smartSelection = false;
	settingsMenu->setItemChecked(smartSel, smartSelection);
	menuBar()->insertItem( tr("&File"), fmenu);
	menuBar()->insertItem( tr("&Edit"), emenu);
	menuBar()->insertItem( tr("&Settings"), settingsMenu );

/* Setting up Toolbars */
	FileTools = new QToolBar( tr("File"), this);
	DatNeu = new QToolButton(loadIcon("editdelete.png"), tr("Clear all Text"), QString::null, this, SLOT(Do_new()), FileTools);
	DatOpe = new QToolButton(loadIcon("DateiOpen.xpm"), tr("Load Text from File"), QString::null, this, SLOT(LoadTextFile()), FileTools);
	DatSav = new QToolButton(loadIcon("DateiSave2.png"), tr("Save Text to File"), QString::null, this, SLOT(SaveTextFile()), FileTools);
	DatClo = new QToolButton(loadIcon("DateiClose.png"), tr("Update Text Frame and Exit"), QString::null, this, SLOT(Do_leave2()), FileTools);
	DatCan = new QToolButton(loadIcon("exit22.png"), tr("Exit Without Updating Text Frame"), QString::null, this, SLOT(Do_leave()), FileTools);
	DatRel = new QToolButton(loadIcon("reload.png"), tr("Reload Text from Frame"), QString::null, this, SLOT(slotFileRevert()), FileTools);
	DatUpdt = new QToolButton(loadIcon("compfile.png"), tr("Update Text Frame"), QString::null, this, SLOT(updateTextFrame()), FileTools);
	DatFin = new QToolButton(loadIcon("find.png"), tr("Search/Replace"), QString::null, this, SLOT(SearchText()), FileTools);
	DatUpdt->setEnabled(false);
	DatRel->setEnabled(false);
	setDockEnabled(FileTools, DockLeft, false);
	setDockEnabled(FileTools, DockRight, false);
	setDockEnabled(FileTools, DockBottom, false);
	FontTools = new SToolBFont(this);
	setDockEnabled(FontTools, DockLeft, false);
	setDockEnabled(FontTools, DockRight, false);
	setDockEnabled(FontTools, DockBottom, false);
	AlignTools = new SToolBAlign(this);
	setDockEnabled(AlignTools, DockLeft, false);
	setDockEnabled(AlignTools, DockRight, false);
	setDockEnabled(AlignTools, DockBottom, false);
	AlignTools->Spal->setFormats(doc);
	StyleTools = new SToolBStyle(this);
	setDockEnabled(StyleTools, DockLeft, false);
	setDockEnabled(StyleTools, DockRight, false);
	setDockEnabled(StyleTools, DockBottom, false);
	StrokeTools = new SToolBColorS(this, doc);
	setDockEnabled(StrokeTools, DockLeft, false);
	setDockEnabled(StrokeTools, DockRight, false);
	setDockEnabled(StrokeTools, DockBottom, false);
	StrokeTools->TxStroke->setEnabled(false);
	StrokeTools->PM1->setEnabled(false);
	FillTools = new SToolBColorF(this, doc);
	setDockEnabled(FillTools, DockLeft, false);
	setDockEnabled(FillTools, DockRight, false);
	setDockEnabled(FillTools, DockBottom, false);

	EdSplit = new QSplitter(vb);
/* SideBar Widget */
	EditorBar = new SideBar(EdSplit);
/* Editor Widget, subclass of QTextEdit */
	Editor = new SEditor(EdSplit, docc);
	StoryEd2Layout->addWidget( EdSplit );
	
/* Setting up Status Bar */
	ButtonGroup1 = new QButtonGroup( statusBar(), "ButtonGroup1" );
	ButtonGroup1->setFrameShape( QButtonGroup::NoFrame );
	ButtonGroup1->setFrameShadow( QButtonGroup::Plain );
	ButtonGroup1->setTitle("");
	ButtonGroup1->setExclusive( true );
	ButtonGroup1->setColumnLayout(0, Qt::Vertical );
	ButtonGroup1->layout()->setSpacing( 0 );
	ButtonGroup1->layout()->setMargin( 0 );
	ButtonGroup1Layout = new QGridLayout( ButtonGroup1->layout() );
	ButtonGroup1Layout->setAlignment( Qt::AlignTop );
	ButtonGroup1Layout->setSpacing( 2 );
	ButtonGroup1Layout->setMargin( 0 );
	WordCT1 = new QLabel(ButtonGroup1, "wt");
	WordCT1->setText( tr("Current Paragraph:"));
	ButtonGroup1Layout->addMultiCellWidget( WordCT1, 0, 0, 0, 3 );
	WordCT = new QLabel(ButtonGroup1, "wt");
	WordCT->setText( tr("Words: "));
	ButtonGroup1Layout->addWidget( WordCT, 1, 0 );
	WordC = new QLabel(ButtonGroup1, "wc");
	ButtonGroup1Layout->addWidget( WordC, 1, 1 );
	CharCT = new QLabel(ButtonGroup1, "ct");
	CharCT->setText( tr("Chars: "));
	ButtonGroup1Layout->addWidget( CharCT, 1, 2 );
	CharC = new QLabel(ButtonGroup1, "cc");
	ButtonGroup1Layout->addWidget( CharC, 1, 3 );
	statusBar()->addWidget(ButtonGroup1, 1, true);
	ButtonGroup2 = new QButtonGroup( statusBar(), "ButtonGroup2" );
	ButtonGroup2->setFrameShape( QButtonGroup::NoFrame );
	ButtonGroup2->setFrameShadow( QButtonGroup::Plain );
	ButtonGroup2->setTitle("");
	ButtonGroup2->setExclusive( true );
	ButtonGroup2->setColumnLayout(0, Qt::Vertical );
	ButtonGroup2->layout()->setSpacing( 0 );
	ButtonGroup2->layout()->setMargin( 0 );
	ButtonGroup2Layout = new QGridLayout( ButtonGroup2->layout() );
	ButtonGroup2Layout->setAlignment( Qt::AlignTop );
	ButtonGroup2Layout->setSpacing( 2 );
	ButtonGroup2Layout->setMargin( 0 );
	WordCT3 = new QLabel(ButtonGroup2, "wt");
	WordCT3->setText( tr("Totals:"));
	ButtonGroup2Layout->addMultiCellWidget( WordCT3, 0, 0, 0, 5 );
	ParCT = new QLabel(ButtonGroup2, "pt");
	ParCT->setText( tr("Paragraphs: "));
	ButtonGroup2Layout->addWidget( ParCT, 1, 0 );
	ParC = new QLabel(ButtonGroup2, "pc");
	ButtonGroup2Layout->addWidget( ParC, 1, 1 );
	WordCT2 = new QLabel(ButtonGroup2, "wt");
	WordCT2->setText( tr("Words: "));
	ButtonGroup2Layout->addWidget( WordCT2, 1, 2 );
	WordC2 = new QLabel(ButtonGroup2, "wc");
	ButtonGroup2Layout->addWidget( WordC2, 1, 3 );
	CharCT2 = new QLabel(ButtonGroup2, "ct");
	CharCT2->setText( tr("Chars: "));
	ButtonGroup2Layout->addWidget( CharCT2, 1, 4 );
	CharC2 = new QLabel(ButtonGroup2, "cc");
	ButtonGroup2Layout->addWidget( CharC2, 1, 5 );
	statusBar()->addWidget(ButtonGroup2, 1, true);
	setCentralWidget( vb );
/* Final polishment */
	fmenu->setItemEnabled(fid52, 0);
	emenu->setItemEnabled(Mcopy, 0);
	emenu->setItemEnabled(Mcut, 0);
	emenu->setItemEnabled(Mpaste, 0);
	emenu->setItemEnabled(Mdel, 0);
	emenu->setItemEnabled(Mupdt, 0);
	resize( QSize(660, 500).expandedTo(minimumSizeHint()) );
	Editor->setPaper(ScApp->Prefs.STEcolor);
	QFont fo;
	fo.fromString(ScApp->Prefs.STEfont);
	Editor->setFont(fo);
	CurrItem = ite;
	firstSet = false;
	Editor->loadItemText(ite);
	updateProps(0,0);
	updateStatus();
	TextChanged = false;
	EditorBar->setFrameStyle(Editor->frameStyle());
	EditorBar->setLineWidth(Editor->lineWidth());
	EditorBar->editor = Editor;
	connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
	connect(Editor, SIGNAL(clicked(int, int)), this, SLOT(updateProps(int, int)));
	connect(Editor, SIGNAL(setProps(int, int)), this, SLOT(updateProps(int, int)));
	connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	connect(Editor, SIGNAL(copyAvailable(bool)), this, SLOT(CopyAvail(bool )));
	connect(Editor, SIGNAL(PasteAvail()), this, SLOT(PasteAvail()));
	connect(Editor, SIGNAL(contentsMoving(int, int)), EditorBar, SLOT(doMove(int, int )));
	connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
	connect(Editor, SIGNAL(SideBarUp(bool )), EditorBar, SLOT(setRepaint(bool )));
	connect(Editor, SIGNAL(SideBarUpdate( )), EditorBar, SLOT(doRepaint()));
	// 10/12/2004 - pv - #1203: wrong selection on double click
	connect(Editor, SIGNAL(doubleClicked(int, int)), this, SLOT(doubleClick(int, int)));
	connect(EditorBar, SIGNAL(ChangeStyle(int, int )), this, SLOT(changeAlignSB(int, int )));
	connect(AlignTools, SIGNAL(newStyle(int)), this, SLOT(newAlign(int)));
	connect(AlignTools, SIGNAL(NewAlign(int)), this, SLOT(newAlign(int)));
	connect(FillTools, SIGNAL(NewColor(int, int)), this, SLOT(newTxFill(int, int)));
	connect(StrokeTools, SIGNAL(NewColor(int, int)), this, SLOT(newTxStroke(int, int)));
	connect(FontTools, SIGNAL(NewSize(double )), this, SLOT(newTxSize(double)));
	connect(FontTools, SIGNAL(NewFont(const QString& )), this, SLOT(newTxFont(const QString& )));
	connect(FontTools, SIGNAL(NewScale(int )), this, SLOT(newTxScale(int )));
	connect(StyleTools, SIGNAL(NewKern(double )), this, SLOT(newTxKern(double )));
	connect(StyleTools, SIGNAL(newStyle(int )), this, SLOT(newTxStyle(int )));
	Editor->setFocus();
}
 
/** 10/12/2004 - pv - #1203: wrong selection on double click
Catch the double click signal - cut the wrong selection (with
whitespaces on the tail) - select only one word - return
controlling back to story editor - have rest */
void StoryEditor::doubleClick(int para, int position)
{
	int paraFrom, indexFrom, paraTo, indexTo;
	QString selText = Editor->selectedText();
	if (selText.length() == 0 || !smartSelection)
	{
		updateProps(para, position);
		return;
	}
	Editor->getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
	selText =  selText.stripWhiteSpace();
	Editor->setSelection(paraFrom, indexFrom, paraFrom, indexFrom + selText.length());
	updateProps(para, position);
}

void StoryEditor::ToggleSmart()
{
	smartSelection = !smartSelection;
	settingsMenu->setItemChecked(smartSel, smartSelection);
}

int StoryEditor::exec()
{
	clearWFlags( WDestructiveClose );
	setWFlags( WShowModal );
	result = 0;
	show();
	Editor->setFocus();
	qApp->enter_loop();
	clearWFlags( WShowModal );
	return result;
}

void StoryEditor::closeEvent(QCloseEvent *)
{
	if (TextChanged)
	{
		int t = QMessageBox::warning(this, tr("Warning"),
									tr("Do you want to save your changes?"),
									QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
		if (t == QMessageBox::Yes)
			result = QDialog::Accepted;
		else if (t == QMessageBox::Cancel)
			return;
		else if (t == QMessageBox::No)
			result = QDialog::Rejected;
	}
	else
		result = QDialog::Rejected;
	hide();
	qApp->exit_loop();
}

void StoryEditor::setBackPref()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(Editor->paper().color(), this);
	if (neu.isValid())
	{
		Editor->setPaper(neu);
		ScApp->Prefs.STEcolor = neu;
	}
}

void StoryEditor::setFontPref()
{
	Editor->setFont( QFontDialog::getFont( 0, Editor->font() ) );
	ScApp->Prefs.STEfont = Editor->font().toString();
	EditorBar->doRepaint();
}

void StoryEditor::newTxFill(int c, int s)
{
	if (c != -1)
		Editor->CurrTextFill = FillTools->TxFill->text(c);
	if (s != -1)
		Editor->CurrTextFillSh = s;
	Editor->setFarbe(Editor->CurrTextFill, Editor->CurrTextFillSh);
	struct PtiSmall hg;
	hg.ccolor = Editor->CurrTextFill;
	hg.cshade = Editor->CurrTextFillSh;
	Editor->updateSel(0, &hg);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxStroke(int c, int s)
{
	if (c != -1)
		Editor->CurrTextStroke = StrokeTools->TxStroke->text(c);
	if (s != -1)
		Editor->CurrTextStrokeSh = s;
	struct PtiSmall hg;
	hg.cstroke = Editor->CurrTextStroke;
	hg.cshade2 = Editor->CurrTextStrokeSh;
	Editor->updateSel(1, &hg);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxFont(const QString &f)
{
	Editor->CurrFont = f;
	struct PtiSmall hg;
	hg.cfont = Editor->CurrFont;
	Editor->updateSel(2, &hg);
	if(!doc->UsedFonts.contains(Editor->CurrFont))
		doc->AddFont(Editor->CurrFont, ScApp->Prefs.AvailFonts[Editor->CurrFont]->Font);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxSize(double s)
{
	Editor->CurrFontSize = qRound(s * 10.0);
	struct PtiSmall hg;
	hg.csize = Editor->CurrFontSize;
	Editor->updateSel(3, &hg);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxStyle(int s)
{
	Editor->CurrentStyle = s;
	struct PtiSmall hg;
	hg.cstyle = Editor->CurrentStyle;
	Editor->updateSel(4, &hg);
	Editor->setStyle(s);
	if (s & 4)
	{
		StrokeTools->TxStroke->setEnabled(true);
		StrokeTools->PM1->setEnabled(true);
	}
	else
	{
		StrokeTools->TxStroke->setEnabled(false);
		StrokeTools->PM1->setEnabled(false);
	}
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxScale(int s)
{
	Editor->CurrTextScale = s;
	struct PtiSmall hg;
	hg.cscale = Editor->CurrTextScale;
	Editor->updateSel(5, &hg);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxKern(double s)
{
	Editor->CurrTextKern = s;
	struct PtiSmall hg;
	hg.cextra = Editor->CurrTextKern;
	Editor->updateSel(6, &hg);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::updateProps(int p, int ch)
{
	CListe::Iterator it;
	int c = 0;
	struct PtiSmall *hg;
	SEditor::ChList *chars;
	if (Editor->wasMod)
		return;
	if ((p >= static_cast<int>(Editor->StyledText.count())) || (Editor->StyledText.count() == 0) || (!firstSet))
	{
		if (!firstSet)
		{
			Editor->CurrTextFill = CurrItem->TxtFill;
			Editor->CurrTextFillSh = CurrItem->ShTxtFill;
			Editor->CurrTextStroke = CurrItem->TxtStroke;
			Editor->CurrTextStrokeSh = CurrItem->ShTxtStroke;
			Editor->CurrFont = CurrItem->IFont;
			Editor->CurrFontSize = CurrItem->ISize;
			Editor->CurrentStyle = CurrItem->TxTStyle;
			Editor->CurrentABStil = CurrItem->Ausrich;
			Editor->CurrTextKern = CurrItem->ExtraV;
			Editor->CurrTextScale = CurrItem->TxtScale;
			c = 0;
			StrokeTools->SetShade(CurrItem->ShTxtStroke);
			FillTools->SetShade(CurrItem->ShTxtFill);
			QString b = CurrItem->TxtFill;
			if ((b != "None") && (b != ""))
			{
				c++;
				for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
				{
					if (it.key() == b)
						break;
					c++;
				}
			}
			FillTools->SetColor(c);
			c = 0;
			b = CurrItem->TxtStroke;
			if ((b != "None") && (b != ""))
			{
				c++;
				for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
				{
					if (it.key() == b)
						break;
					c++;
				}
			}
			StrokeTools->SetColor(c);
			AlignTools->SetAlign(CurrItem->Ausrich);
			StyleTools->SetKern(CurrItem->ExtraV);
			StyleTools->SetStyle(CurrItem->TxTStyle);
			FontTools->SetSize(CurrItem->ISize / 10.0);
			FontTools->SetFont(CurrItem->IFont);
			FontTools->SetScale(CurrItem->TxtScale);
		}
		if (Editor->CurrentStyle & 4)
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		Editor->setAlign(Editor->CurrentABStil);
		Editor->setStyle(Editor->CurrentStyle);
		Editor->setFarbe(Editor->CurrTextFill, Editor->CurrTextFillSh);
		firstSet = true;
		return;
	}
	chars = Editor->StyledText.at(p);
	Editor->CurrentABStil = Editor->ParagStyles[p];
	if (chars->count() == 0)
	{
		if (Editor->CurrentABStil > 4)
		{
			Editor->CurrFont = doc->Vorlagen[Editor->CurrentABStil].Font;
			Editor->CurrFontSize = doc->Vorlagen[Editor->CurrentABStil].FontSize;
			Editor->CurrentStyle = doc->Vorlagen[Editor->CurrentABStil].FontEffect;
			Editor->CurrTextFill = doc->Vorlagen[Editor->CurrentABStil].FColor;
			Editor->CurrTextFillSh = doc->Vorlagen[Editor->CurrentABStil].FShade;
			Editor->CurrTextStroke = doc->Vorlagen[Editor->CurrentABStil].SColor;
			Editor->CurrTextStrokeSh = doc->Vorlagen[Editor->CurrentABStil].SShade;
		}
		Editor->setAlign(Editor->CurrentABStil);
		Editor->setStyle(Editor->CurrentStyle);
		Editor->setFarbe(Editor->CurrTextFill, Editor->CurrTextFillSh);
	}
	else
	{
		if (Editor->hasSelectedText())
		{
			int PStart, PEnd, SelStart, SelEnd;
			Editor->getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
			if ((SelStart > -1) && (SelStart < static_cast<int>(chars->count())))
				hg = chars->at(SelStart);
			else
				hg = chars->at(QMIN(QMAX(ch-1, 0), static_cast<int>(chars->count())-1));
		}
		else
			hg = chars->at(QMIN(QMAX(ch-1, 0), static_cast<int>(chars->count())-1));
		Editor->CurrTextFill = hg->ccolor;
		Editor->CurrTextFillSh = hg->cshade;
		Editor->CurrTextStroke = hg->cstroke;
		Editor->CurrTextStrokeSh = hg->cshade2;
		Editor->CurrFont = hg->cfont;
		Editor->CurrFontSize = hg->csize;
		Editor->CurrentStyle = hg->cstyle & 127;
		Editor->CurrTextKern = hg->cextra;
		Editor->CurrTextScale = hg->cscale;
	}
	StrokeTools->SetShade(Editor->CurrTextStrokeSh);
	FillTools->SetShade(Editor->CurrTextFillSh);
	QString b = Editor->CurrTextFill;
	if ((b != "None") && (b != ""))
	{
		c++;
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			if (it.key() == b)
				break;
			c++;
		}
	}
	FillTools->SetColor(c);
	c = 0;
	b = Editor->CurrTextStroke;
	if ((b != "None") && (b != ""))
	{
		c++;
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			if (it.key() == b)
				break;
			c++;
		}
	}
	StrokeTools->SetColor(c);
	if (Editor->CurrentStyle & 4)
	{
		StrokeTools->TxStroke->setEnabled(true);
		StrokeTools->PM1->setEnabled(true);
	}
	else
	{
		StrokeTools->TxStroke->setEnabled(false);
		StrokeTools->PM1->setEnabled(false);
	}
	StyleTools->SetKern(Editor->CurrTextKern);
	StyleTools->SetStyle(Editor->CurrentStyle);
	FontTools->SetSize(Editor->CurrFontSize / 10.0);
	FontTools->SetFont(Editor->CurrFont);
	FontTools->SetScale(Editor->CurrTextScale);
	AlignTools->SetAlign(Editor->CurrentABStil);
	updateStatus();
}

void StoryEditor::updateStatus()
{
	QString tmp;
	int p, i;
	Editor->getCursorPosition(&p, &i);
	ParC->setText(tmp.setNum(Editor->StyledText.count()));
	QRegExp rx( "(\\w+)\\b" );
	int pos = 0;
	int counter = 0;
	int counter1 = 0;
	int counter2 = 0;
	while ( pos >= 0 )
	{
		pos = rx.search( Editor->text(p), pos );
		if ( pos > -1 )
		{
			counter++;
			pos += rx.matchedLength();
		}
	}
	WordC->setText(tmp.setNum(counter));
	CharC->setText(tmp.setNum(Editor->text(p).length()-1));
	for (uint a = 0; a < Editor->StyledText.count(); ++a)
	{
		int pos = 0;
		while ( pos >= 0 )
		{
			pos = rx.search( Editor->text(a), pos );
			if ( pos > -1 )
			{
				counter2++;
				pos += rx.matchedLength();
			}
		}
		counter1 += Editor->text(a).length();
	}
	WordC2->setText(tmp.setNum(counter2));
	CharC2->setText(tmp.setNum(Editor->length()));
}

void StoryEditor::Do_insSp()
{
	ScApp->DLLinput = Editor->CurrFont;
	ScApp->DLLReturn = "";
	if (ScApp->DLLexists(1))
	{
		ScApp->CallDLL( 1 );
		Editor->insChars(ScApp->DLLReturn);
		Editor->insert(ScApp->DLLReturn);
	}
	ScApp->DLLinput = "";
	ScApp->DLLReturn = "";
}

void StoryEditor::Do_fontPrev()
{
	ScApp->DLLinput = Editor->CurrFont;
	ScApp->DLLReturn = "";
	if (ScApp->DLLexists(2))
	{
		ScApp->CallDLL( 2 );
		if (ScApp->DLLReturn != "")
		{
			newTxFont(ScApp->DLLReturn);
			FontTools->SetFont(ScApp->DLLReturn);
		}
	}
	ScApp->DLLinput = "";
	ScApp->DLLReturn = "";
}

void StoryEditor::Do_leave2()
{
	result = QDialog::Accepted;
	hide();
	qApp->exit_loop();
}

void StoryEditor::Do_leave()
{
	if (TextChanged)
	{
		int t = QMessageBox::warning(this, tr("Warning"),
		                             tr("Do you really want to lose all your Changes?"),
		                             QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);
		if (t == QMessageBox::No)
			return;
	}
	result = QDialog::Rejected;
	hide();
	qApp->exit_loop();
}
 
/*! Saves the document with editation continued. Signal called from menu.
  05/28/04 petr vanek
  */
void StoryEditor::Do_saveDocument()
{
	updateTextFrame();
	ScApp->slotFileSave();
}

bool StoryEditor::Do_new()
{
	if (Editor->length() != 0)
	{
		int t = QMessageBox::warning(this, tr("Warning"),
	                             tr("Do you really want to clear all your Text?"),
	                             QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);
		if (t == QMessageBox::No)
			return false;
	}
	Editor->StyledText.clear();
	Editor->ParagStyles.clear();
	Editor->clear();
	Editor->setUndoRedoEnabled(false);
	Editor->setUndoRedoEnabled(true);
	Editor->setCursorPosition(0, 0);
	emenu->setItemEnabled(Mcopy, 0);
	emenu->setItemEnabled(Mcut, 0);
	emenu->setItemEnabled(Mdel, 0);
	fmenu->setItemEnabled(fid52, 0);
	TextChanged = false;
	updateProps(0, 0);
	updateStatus();
	return true;
}

void StoryEditor::slotFileRevert()
{
	if (Do_new())
	{
		Editor->loadItemText(CurrItem);
		updateStatus();
	}
}

void StoryEditor::Do_selectAll()
{
	if (Editor->StyledText.count() == 0)
		return;
	if (Editor->StyledText.count() > 1)
		Editor->setSelection(0, 0, Editor->StyledText.count()-1, Editor->StyledText.at(Editor->StyledText.count()-1)->count());
	else
		Editor->setSelection(0, 0, 0, Editor->StyledText.at(0)->count());
}

void StoryEditor::Do_copy()
{
	Editor->copy();
}

void StoryEditor::Do_paste()
{
	Editor->paste();
}

void StoryEditor::Do_cut()
{
	Editor->cut();
}

void StoryEditor::Do_del()
{
	if (Editor->StyledText.count() == 0)
		return;
	EditorBar->setRepaint(false);
	if (Editor->hasSelectedText())
	{
		Editor->deleteSel();
		Editor->removeSelectedText();
	}
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
}

void StoryEditor::CopyAvail(bool u)
{
	emenu->setItemEnabled(Mcopy, u);
	emenu->setItemEnabled(Mcut, u);
	emenu->setItemEnabled(Mdel, u);
	if (Editor->tBuffer.length() != 0)
		emenu->setItemEnabled(Mpaste, 1);
	else
		emenu->setItemEnabled(Mpaste, 0);
}

void StoryEditor::PasteAvail()
{
	emenu->setItemEnabled(Mpaste, 1);
}

void StoryEditor::updateTextFrame()
{
	PageItem *nb = CurrItem;
	while (nb != 0)
	{
		if (nb->BackBox != 0)
			nb = nb->BackBox;
		else
			break;
	}
	PageItem* nb2 = nb;
	while (nb2 != 0)
	{
		nb2->Ptext.clear();
		nb2->CPos = 0;
		nb2->Dirty = false;
		nb2 = nb2->NextBox;
	}
	Editor->saveItemText(nb);
	if (doc->Trenner->AutoCheck)
	{
		if (doc->Trenner->Language != nb->Language)
			doc->Trenner->slotNewDict(nb->Language);
		doc->Trenner->slotHyphenate(nb);
	}
	bool rep = doc->RePos;
	doc->RePos = true;
	QPixmap pgPix(1, 1);
	ScPainter *painter = new ScPainter(&pgPix, 1, 1);
	painter->translate(0.5, 0.5);
	nb->DrawObj(painter, QRect(0, 0, 1, 1));
	painter->end();
	delete painter;
	doc->RePos = rep;
	nb2 = nb;
	while (nb2 != 0)
	{
		nb2->Dirty = false;
		nb2 = nb2->NextBox;
	}
	ScApp->view->DrawNew();
	TextChanged = false;
	emenu->setItemEnabled(Mupdt, 0);
	fmenu->setItemEnabled(fid52, 0);
	DatUpdt->setEnabled(false);
	DatRel->setEnabled(false);
	emit DocChanged();
}

void StoryEditor::SearchText()
{
	EditorBar->setRepaint(false);
	SearchReplace* dia = new SearchReplace(this, doc, &ScApp->Prefs, CurrItem, false);
	dia->exec();
	delete dia;
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
}

void StoryEditor::slotEditStyles()
{
	EditorBar->setRepaint(false);
	int p, i;
	Editor->getCursorPosition(&p, &i);
	disconnect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	disconnect(AlignTools, SIGNAL(newStyle(int)), this, SLOT(newAlign(int)));
	disconnect(AlignTools, SIGNAL(NewAlign(int)), this, SLOT(newAlign(int)));
	emit EditSt();
	AlignTools->Spal->setFormats(doc);
	AlignTools->SetAlign(Editor->CurrentABStil);
	connect(AlignTools, SIGNAL(newStyle(int)), this, SLOT(newAlign(int)));
	connect(AlignTools, SIGNAL(NewAlign(int)), this, SLOT(newAlign(int)));
	Editor->setCursorPosition(p, i);
	updateProps(p, i);
	connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
}

void StoryEditor::newAlign(int st)
{
	Editor->CurrentABStil = st;
	changeAlign(st);
}

void StoryEditor::changeAlignSB(int pa, int align)
{
	Editor->CurrentABStil = align;
	if (Editor->StyledText.count() != 0)
	{
		disconnect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
		SEditor::ChList *chars;
		(*Editor->ParagStyles.at(pa)) = Editor->CurrentABStil;
		if (Editor->StyledText.at(pa)->count() > 0)
		{
			chars = Editor->StyledText.at(pa);
			for (uint s = 0; s < chars->count(); ++s)
			{
				if (Editor->CurrentABStil > 4)
				{
					if (doc->Vorlagen[Editor->CurrentABStil].Font != "")
					{
						chars->at(s)->cfont = doc->Vorlagen[Editor->CurrentABStil].Font;
						chars->at(s)->csize = doc->Vorlagen[Editor->CurrentABStil].FontSize;
						chars->at(s)->cstyle &= ~127;
						chars->at(s)->cstyle |= doc->Vorlagen[Editor->CurrentABStil].FontEffect;
						chars->at(s)->ccolor = doc->Vorlagen[Editor->CurrentABStil].FColor;
						chars->at(s)->cshade = doc->Vorlagen[Editor->CurrentABStil].FShade;
						chars->at(s)->cstroke = doc->Vorlagen[Editor->CurrentABStil].SColor;
						chars->at(s)->cshade2 = doc->Vorlagen[Editor->CurrentABStil].SShade;
					}
				}
				if ((Editor->CurrentABStil < 5) && (chars->at(s)->cab > 4))
				{
					chars->at(s)->ccolor = CurrItem->TxtFill;
					chars->at(s)->cshade = CurrItem->ShTxtFill;
					chars->at(s)->cstroke = CurrItem->TxtStroke;
					chars->at(s)->cshade2 = CurrItem->ShTxtStroke;
					chars->at(s)->cfont = CurrItem->IFont;
					chars->at(s)->csize = CurrItem->ISize;
					chars->at(s)->cstyle &= ~127;
					chars->at(s)->cstyle |= CurrItem->TxTStyle;
				}
				chars->at(s)->cab = Editor->CurrentABStil;
			}
			Editor->updateFromChars(pa);
		}
		Editor->setCursorPosition(pa, 0);
		updateProps(pa, 0);
		Editor->ensureCursorVisible();
		connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	}
	else
	{
		if (Editor->CurrentABStil > 4)
		{
			if (doc->Vorlagen[Editor->CurrentABStil].Font != "")
			{
				Editor->CurrFont = doc->Vorlagen[Editor->CurrentABStil].Font;
				Editor->CurrFontSize = doc->Vorlagen[Editor->CurrentABStil].FontSize;
				Editor->CurrentStyle = doc->Vorlagen[Editor->CurrentABStil].FontEffect;
				Editor->CurrTextFill = doc->Vorlagen[Editor->CurrentABStil].FColor;
				Editor->CurrTextFillSh = doc->Vorlagen[Editor->CurrentABStil].FShade;
				Editor->CurrTextStroke = doc->Vorlagen[Editor->CurrentABStil].SColor;
				Editor->CurrTextStrokeSh = doc->Vorlagen[Editor->CurrentABStil].SShade;
			}
		}
		else
		{
			Editor->CurrTextFill = CurrItem->TxtFill;
			Editor->CurrTextFillSh = CurrItem->ShTxtFill;
			Editor->CurrTextStroke = CurrItem->TxtStroke;
			Editor->CurrTextStrokeSh = CurrItem->ShTxtStroke;
			Editor->CurrFont = CurrItem->IFont;
			Editor->CurrFontSize = CurrItem->ISize;
			Editor->CurrentStyle = CurrItem->TxTStyle;
			Editor->CurrTextKern = CurrItem->ExtraV;
			Editor->CurrTextScale = CurrItem->TxtScale;
		}
		Editor->setStyle(Editor->CurrentStyle);
		if (Editor->CurrentStyle & 4)
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		Editor->setFarbe(Editor->CurrTextFill, Editor->CurrTextFillSh);
		Editor->setCursorPosition(0, 0);
		updateProps(0, 0);
	}
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::changeAlign(int )
{
	int p, i;
	bool sel = false;
	Editor->getCursorPosition(&p, &i);
	if (Editor->StyledText.count() != 0)
	{
		disconnect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
		int PStart, PEnd, SelStart, SelEnd, PStart2, PEnd2, SelStart2, SelEnd2;
		SEditor::ChList *chars;
		if (Editor->hasSelectedText())
		{
			Editor->getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
			PStart2 = PStart;
			PEnd2 = PEnd;
			SelStart2 = SelStart;
			SelEnd2 = SelEnd;
			sel = true;
		}
		else
		{
			PStart = p;
			PEnd = p;
		}
		for (int pa = PStart; pa < QMIN(PEnd+1, static_cast<int>(Editor->StyledText.count())); ++pa)
		{
			(*Editor->ParagStyles.at(pa)) = Editor->CurrentABStil;
			if (Editor->StyledText.at(pa)->count() > 0)
			{
				chars = Editor->StyledText.at(pa);
				for (uint s = 0; s < chars->count(); ++s)
				{
					if (Editor->CurrentABStil > 4)
					{
						if (doc->Vorlagen[Editor->CurrentABStil].Font != "")
						{
							chars->at(s)->cfont = doc->Vorlagen[Editor->CurrentABStil].Font;
							chars->at(s)->csize = doc->Vorlagen[Editor->CurrentABStil].FontSize;
							chars->at(s)->cstyle &= ~127;
							chars->at(s)->cstyle |= doc->Vorlagen[Editor->CurrentABStil].FontEffect;
							chars->at(s)->ccolor = doc->Vorlagen[Editor->CurrentABStil].FColor;
							chars->at(s)->cshade = doc->Vorlagen[Editor->CurrentABStil].FShade;
							chars->at(s)->cstroke = doc->Vorlagen[Editor->CurrentABStil].SColor;
							chars->at(s)->cshade2 = doc->Vorlagen[Editor->CurrentABStil].SShade;
						}
					}
					if ((Editor->CurrentABStil < 5) && (chars->at(s)->cab > 4))
					{
						chars->at(s)->ccolor = CurrItem->TxtFill;
						chars->at(s)->cshade = CurrItem->ShTxtFill;
						chars->at(s)->cstroke = CurrItem->TxtStroke;
						chars->at(s)->cshade2 = CurrItem->ShTxtStroke;
						chars->at(s)->cfont = CurrItem->IFont;
						chars->at(s)->csize = CurrItem->ISize;
						chars->at(s)->cstyle &= ~127;
						chars->at(s)->cstyle |= CurrItem->TxTStyle;
					}
					chars->at(s)->cab = Editor->CurrentABStil;
				}
			Editor->updateFromChars(pa);
			}
		}
		if (sel)
			Editor->setSelection(PStart2, SelStart2, PEnd2, SelEnd2);
		Editor->setCursorPosition(p, i);
		Editor->ensureCursorVisible();
		updateProps(p, i);
		connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	}
	else
	{
		if (Editor->CurrentABStil > 4)
		{
			if (doc->Vorlagen[Editor->CurrentABStil].Font != "")
			{
				Editor->CurrFont = doc->Vorlagen[Editor->CurrentABStil].Font;
				Editor->CurrFontSize = doc->Vorlagen[Editor->CurrentABStil].FontSize;
				Editor->CurrentStyle = doc->Vorlagen[Editor->CurrentABStil].FontEffect;
				Editor->CurrTextFill = doc->Vorlagen[Editor->CurrentABStil].FColor;
				Editor->CurrTextFillSh = doc->Vorlagen[Editor->CurrentABStil].FShade;
				Editor->CurrTextStroke = doc->Vorlagen[Editor->CurrentABStil].SColor;
				Editor->CurrTextStrokeSh = doc->Vorlagen[Editor->CurrentABStil].SShade;
			}
		}
		else
		{
			Editor->CurrTextFill = CurrItem->TxtFill;
			Editor->CurrTextFillSh = CurrItem->ShTxtFill;
			Editor->CurrTextStroke = CurrItem->TxtStroke;
			Editor->CurrTextStrokeSh = CurrItem->ShTxtStroke;
			Editor->CurrFont = CurrItem->IFont;
			Editor->CurrFontSize = CurrItem->ISize;
			Editor->CurrentStyle = CurrItem->TxTStyle;
			Editor->CurrTextKern = CurrItem->ExtraV;
			Editor->CurrTextScale = CurrItem->TxtScale;
		}
		Editor->setStyle(Editor->CurrentStyle);
		if (Editor->CurrentStyle & 4)
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		Editor->setFarbe(Editor->CurrTextFill, Editor->CurrTextFillSh);
		Editor->setCursorPosition(0, 0);
		updateProps(0, 0);
	}
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::modifiedText()
{
	TextChanged = true;
	firstSet = true;
	emenu->setItemEnabled(Mupdt, 1);
	fmenu->setItemEnabled(fid52, 1);
	if (Editor->tBuffer.length() != 0)
		emenu->setItemEnabled(Mpaste, 1);
	else
		emenu->setItemEnabled(Mpaste, 0);
	DatUpdt->setEnabled(true);
	DatRel->setEnabled(true);
	updateStatus();
}

void StoryEditor::LoadTextFile()
{
	if (Do_new())
	{
		EditorBar->setRepaint(false);
		QString LoadEnc = "";
		QString fileName = "";
		PrefsContext* dirs = prefsFile->getContext("dirs");
		QString wdir = dirs->get("story_load", ScApp->Prefs.DocDir);
		CustomFDialog dia(this, wdir, tr("Open"), tr("Text Files (*.txt);;All Files(*)"), false, true, false, true);
		if (dia.exec() != QDialog::Accepted)
			return;
		LoadEnc = dia.TxCodeM->currentText();
		fileName =  dia.selectedFile();
		if (!fileName.isEmpty())
		{
			dirs->set("story_load", fileName.left(fileName.findRev("/")));
			Serializer *ss = new Serializer(fileName);
			if (ss->Read(LoadEnc))
			{
				QString data = ss->GetObjekt();
				data.replace(QRegExp("\r"), "");
				data.replace(QRegExp("\n"), QChar(13));
				Editor->loadText(data, CurrItem);
				emenu->setItemEnabled(Mpaste, 0);
				emenu->setItemEnabled(Mcopy, 0);
				emenu->setItemEnabled(Mcut, 0);
				emenu->setItemEnabled(Mdel, 0);
				delete ss;
			}
		}
		EditorBar->setRepaint(true);
		EditorBar->doRepaint();
	}
}

void StoryEditor::SaveTextFile()
{
	QString LoadEnc = "";
	QString fileName = "";
	PrefsContext* dirs = prefsFile->getContext("dirs");
	QString wdir = dirs->get("story_save", ScApp->Prefs.DocDir);
	CustomFDialog dia(this, wdir, tr("Save as"), tr("Text Files (*.txt);;All Files(*)"), false, false, false, true);
	if (dia.exec() != QDialog::Accepted)
		return;
	LoadEnc = dia.TxCodeM->currentText();
	fileName =  dia.selectedFile();
	if (!fileName.isEmpty())
	{
		dirs->set("story_save", fileName.left(fileName.findRev("/")));
		Serializer *ss = new Serializer(fileName);
		ss->Objekt = Editor->text();
		ss->Write(LoadEnc);
		delete ss;
	}
}
