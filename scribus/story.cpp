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
#include "serializer.h"
#include "customfdialog.h"
#include "search.h"
#include "scribus.h"
extern ScribusApp* ScApp;

extern QPixmap loadIcon(QString nam);

SEditor::SEditor(QWidget* parent, ScribusDoc *docc) : QTextEdit(parent)
{
	doc = docc;
	StyledText.clear();
	StyledText.setAutoDelete(true);
	setUndoRedoEnabled(true);
	setUndoDepth(0);
	setTextFormat(Qt::PlainText);
}

void SEditor::keyPressEvent(QKeyEvent *k)
{
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
						return;
					}
				}
				else
					return;
			}
			switch (k->key())
			{
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
							}
						}
					}
					else
						deleteSel();
					break;
				case Key_Backspace:
					if (!hasSelectedText())
					{
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
							ChList *chars2 = StyledText.at(p);
							int a = static_cast<int>(chars2->count());
							for (int s = i; s < a; ++s)
							{
								chars->append(chars2->take(i));
							}
							StyledText.insert(p+1, chars);
						}
						else
							StyledText.append(chars);
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
}

void SEditor::insChars(QString t)
{
	int p, i;
	getCursorPosition(&p, &i);
	if (hasSelectedText())
		deleteSel();
	ChList *chars;
	if ((p >= static_cast<int>(StyledText.count())) || (StyledText.count() == 0))
	{
		chars = new ChList;
		chars->setAutoDelete(true);
		chars->clear();
		StyledText.append(chars);
	}
	else
		chars = StyledText.at(p);
	for (uint a = 0; a < t.length(); ++a)
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
		hg->cab = CurrentABStil;
		hg->cextra = CurrTextKern;
		hg->cscale = CurrTextScale;
		chars->insert(i, hg);
		i++;
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
				hg->cab = chars->at(c)->cab;
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
				hg->cab = CurrentABStil;
				hg->cextra = CurrTextKern;
				hg->cscale = CurrTextScale;
				if (doc->Vorlagen[CurrentABStil].Font != "")
				{
					hg->cfont = doc->Vorlagen[CurrentABStil].Font;
					hg->csize = doc->Vorlagen[CurrentABStil].FontSize;
				}
			}
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
	int Csha;
	int Csty;
	int Ali = 0;
	PageItem *nb = b;
	StyledText.clear();
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
		StyledText.append(chars);
	if (StyledText.count() != 0)
		emit setProps(0, 0);
	setCursorPosition(0, 0);
}

void SEditor::loadText(QString tx, PageItem* b)
{
	struct PtiSmall *hg;
	QString Text = "";
	StyledText.clear();
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
		StyledText.append(chars);
	if (StyledText.count() != 0)
		emit setProps(0, 0);
	setCursorPosition(0, 0);
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
			SelEnd++;
			SelStart = SelEnd;
		}
	}
	setSelection(pa, SelStart, pa, SelEnd);
	setFarbe(Ccol, Csha);
	setStyle(Csty);
	removeSelection();
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
	ChList *chars;
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
			chars = StyledText.at(pa);
			if (pa == PStart)
				start = SelStart;
			else
				start = 0;
			if (pa == PEnd)
				end = SelEnd;
			else
				end = chars->count();
			for (int ca = 0; ca < end-start; ++ca)
			{
				chars->remove(start);
			}
		}
		if (PEnd-PStart > 1)
		{
			for (int pa2 = 0; pa2 < PEnd - PStart - 1; ++pa2)
			{
				StyledText.remove(PStart+1);
			}
		}
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
	}
	setCursorPosition(PStart, SelStart);
}

void SEditor::setStyle(int Csty)
{
	if (Csty & 8)
		setUnderline(true);
	else
		setUnderline(false);
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

void SEditor::cut()
{
}

void SEditor::paste()
{
}

/* Toolbar for Fill Colour */
SToolBColorF::SToolBColorF(QMainWindow* parent, ScribusDoc *doc) : QToolBar( tr("F&ill Color Settings"), parent)
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
SToolBColorS::SToolBColorS(QMainWindow* parent, ScribusDoc *doc) : QToolBar( tr("&Stroke Color Settings"), parent)
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
SToolBStyle::SToolBStyle(QMainWindow* parent) : QToolBar( tr("&Character Settings"), parent)
{
	SeStyle = new StyleSelect(this);
	kerningLabel = new QLabel( tr( "Kerning:" ), this, "kerningLabel" );
	Extra = new MSpinBox( this, 1 );
	Extra->setValues( -300, 300, 10, 0);
	Extra->setSuffix( tr( " pt" ) );
	QToolTip::add( Extra, tr( "Manual Kerning" ) );
	connect(SeStyle, SIGNAL(State(int)), this, SIGNAL(NewStyle(int)));
	connect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
}

void SToolBStyle::newKernHandler()
{
	emit NewKern(Extra->value());
}

void SToolBStyle::SetStyle(int s)
{
	disconnect(SeStyle, SIGNAL(State(int)), this, SIGNAL(NewStyle(int)));
	SeStyle->setStyle(s);
	connect(SeStyle, SIGNAL(State(int)), this, SIGNAL(NewStyle(int)));
}

void SToolBStyle::SetKern(double k)
{
	disconnect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
	Extra->setValue(k);
	connect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
}

/* Toolbar for alignment of Paragraphs */
SToolBAlign::SToolBAlign(QMainWindow* parent) : QToolBar( tr("St&yle Settings"), parent)
{
	GroupAlign = new AlignSelect(this);
	Spal = new Spalette(this);
	QToolTip::add( Spal, tr( "Style of current paragraph" ) );
	connect(Spal, SIGNAL(activated(int)), this, SLOT(newStyleHandler(int )));
	connect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(NewAlign(int )));
}

void SToolBAlign::newStyleHandler(int s)
{
	if (s != 0)
		GroupAlign->setEnabled(false);
	else
		GroupAlign->setEnabled(true);
	emit NewStyle(s);
}

void SToolBAlign::SetAlign(int s)
{
	disconnect(Spal, SIGNAL(activated(int)), this, SIGNAL(NewStyle(int )));
	disconnect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(NewAlign(int )));
	if (s < 5)
	{
		GroupAlign->setEnabled(true);
		GroupAlign->setStyle(s);
	}
	else
		GroupAlign->setEnabled(false);
	Spal->setFormat(s);
	connect(Spal, SIGNAL(activated(int)), this, SIGNAL(NewStyle(int )));
	connect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(NewAlign(int )));
}

/* Toolbar for Font related Settings */
SToolBFont::SToolBFont(QMainWindow* parent) : QToolBar( tr("&Font Settings"), parent)
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
	fmenu->insertItem(loadIcon("DateiNeu16.png"), tr("&New"), this, SLOT(Do_new()), CTRL+Key_N);
	fid52 = fmenu->insertItem( tr("&Reload Text from Frame"), this, SLOT(slotFileRevert()));
	fmenu->insertSeparator();
	fmenu->insertItem(loadIcon("DateiSave16.png"), tr("&Save to File..."), this, SLOT(SaveTextFile()));
	fmenu->insertItem(loadIcon("DateiOpen16.png"), tr("&Load from File..."), this, SLOT(LoadTextFile()));
	fmenu->insertItem(tr("Save &Document"), this, SLOT(Do_saveDocument()), CTRL+Key_S);
	fmenu->insertSeparator();
	/* changes to fit the #662 bug 05/28/04 petr vanek */
	fmenu->insertItem( tr("&Update Text Frame and Exit"), this, SLOT(Do_leave2()));
	fmenu->insertItem( tr("&Exit Without Updating Text Frame"), this, SLOT(Do_leave()));
	/* end of changes */
	emenu = new QPopupMenu();
	Mcopy = emenu->insertItem(loadIcon("editcut.png"), tr("Cu&t"), this, SLOT(Do_cut()), CTRL+Key_X);
	Mcut = emenu->insertItem(loadIcon("editcopy.png"), tr("&Copy"), this, SLOT(Do_copy()), CTRL+Key_C);
	Mpaste = emenu->insertItem(loadIcon("editpaste.png"), tr("&Paste"), this, SLOT(Do_paste()), CTRL+Key_V);
	Mdel = emenu->insertItem(loadIcon("editdelete.png"), tr("C&lear"), this, SLOT(Do_del()), CTRL+Key_V);
	emenu->insertSeparator();
//	int sr = emenu->insertItem( tr("&Search/Replace..."), this, SLOT(SearchText()));
	emenu->insertItem( tr("&Insert Special..."), this , SLOT(Do_insSp()));
//	emenu->setItemEnabled(sr, 0);
	emenu->insertSeparator();
//	emenu->insertItem( tr("&Edit Styles..."), this , SLOT(slotEditStyles()));
	Mupdt = emenu->insertItem( tr("&Update Text Frame"), this, SLOT(updateTextFrame()));
//	menuBar = new QMenuBar(this);
	menuBar()->insertItem( tr("&File"), fmenu);
	menuBar()->insertItem( tr("&Edit"), emenu);

/* Setting up Toolbars */
	FontTools = new SToolBFont(this);
	setDockEnabled(FontTools, DockLeft, false);
	setDockEnabled(FontTools, DockRight, false);
	setDockEnabled(FontTools, DockBottom, false);
	AlignTools = new SToolBAlign(this);
	setDockEnabled(AlignTools, DockLeft, false);
	setDockEnabled(AlignTools, DockRight, false);
	setDockEnabled(AlignTools, DockBottom, false);
	AlignTools->Spal->SetFormats(doc);
	StyleTools = new SToolBStyle(this);
	setDockEnabled(StyleTools, DockLeft, false);
	setDockEnabled(StyleTools, DockRight, false);
	setDockEnabled(StyleTools, DockBottom, false);
	StrokeTools = new SToolBColorS(this, doc);
	setDockEnabled(StrokeTools, DockLeft, false);
	setDockEnabled(StrokeTools, DockRight, false);
	setDockEnabled(StrokeTools, DockBottom, false);
	StrokeTools->setEnabled(false);
	FillTools = new SToolBColorF(this, doc);
	setDockEnabled(FillTools, DockLeft, false);
	setDockEnabled(FillTools, DockRight, false);
	setDockEnabled(FillTools, DockBottom, false);

/* Editor Widget, subclass of QTextEdit */
	Editor = new SEditor(vb, docc);
	StoryEd2Layout->addWidget( Editor );
	
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
	CurrItem = ite;
	Editor->loadItemText(ite);
	updateProps(0,0);
	updateStatus();
	Editor->setFocus();
	TextChanged = false;
	connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
	connect(Editor, SIGNAL(clicked(int, int)), this, SLOT(updateStatus()));
	connect(Editor, SIGNAL(setProps(int, int)), this, SLOT(updateProps(int, int)));
	connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	connect(AlignTools, SIGNAL(NewStyle(int)), this, SLOT(styleChange(int)));
	connect(AlignTools, SIGNAL(NewAlign(int)), this, SLOT(newAlign(int)));
	connect(FillTools, SIGNAL(NewColor(int, int)), this, SLOT(newTxFill(int, int)));
	connect(StrokeTools, SIGNAL(NewColor(int, int)), this, SLOT(newTxStroke(int, int)));
	connect(FontTools, SIGNAL(NewSize(double )), this, SLOT(newTxSize(double)));
	connect(FontTools, SIGNAL(NewFont(const QString& )), this, SLOT(newTxFont(const QString& )));
	connect(FontTools, SIGNAL(NewScale(int )), this, SLOT(newTxScale(int )));
	connect(StyleTools, SIGNAL(NewKern(double )), this, SLOT(newTxKern(double )));
	connect(StyleTools, SIGNAL(NewStyle(int )), this, SLOT(newTxStyle(int )));
}

int StoryEditor::exec()
{
	clearWFlags( WDestructiveClose );
	setWFlags( WShowModal );
	result = 0;
	show();
	qApp->enter_loop();
	clearWFlags( WShowModal );
	return result;
}

void StoryEditor::closeEvent(QCloseEvent *event)
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

void StoryEditor::newTxFill(int c, int s)
{
	Editor->CurrTextFill = FillTools->TxFill->text(c);
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
	Editor->CurrTextStroke = StrokeTools->TxStroke->text(c);
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
		StrokeTools->setEnabled(true);
	else
		StrokeTools->setEnabled(false);
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
	SEditor::ChList *chars;
	if ((p >= static_cast<int>(Editor->StyledText.count())) || (Editor->StyledText.count() == 0))
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
		if (CurrItem->TxTStyle & 4)
			StrokeTools->setEnabled(true);
		else
			StrokeTools->setEnabled(false);
		AlignTools->SetAlign(CurrItem->Ausrich);
		StyleTools->SetKern(CurrItem->ExtraV);
		StyleTools->SetStyle(CurrItem->TxTStyle);
		FontTools->SetSize(CurrItem->ISize / 10.0);
		FontTools->SetFont(CurrItem->IFont);
		FontTools->SetScale(CurrItem->TxtScale);
		return;
	}
	chars = Editor->StyledText.at(p);
	if (chars->count() == 0)
	{
		Editor->setColor(doc->PageColors[CurrItem->TxtFill].getRGBColor());
		Editor->CurrTextFill = CurrItem->TxtFill;
		Editor->setAlignment(Qt::AlignLeft);
		AlignTools->SetAlign(0);
		Editor->CurrentABStil = 0;
		return;
	}
	struct PtiSmall *hg;
	if (Editor->hasSelectedText())
	{
		int PStart, PEnd, SelStart, SelEnd;
		Editor->getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
		if ((SelStart != -1) && (SelStart < static_cast<int>(chars->count())))
			hg = chars->at(SelStart);
		else
			hg = chars->at(QMIN(QMAX(ch-1, 0), chars->count()-1));
	}
	else
		hg = chars->at(QMIN(QMAX(ch-1, 0), chars->count()-1));
	Editor->CurrTextFill = hg->ccolor;
	Editor->CurrTextFillSh = hg->cshade;
	Editor->CurrTextStroke = hg->cstroke;
	Editor->CurrTextStrokeSh = hg->cshade2;
	Editor->CurrFont = hg->cfont;
	Editor->CurrFontSize = hg->csize;
	Editor->CurrentStyle = hg->cstyle & 127;
	Editor->CurrentABStil = hg->cab;
	Editor->CurrTextKern = hg->cextra;
	Editor->CurrTextScale = hg->cscale;
	StrokeTools->SetShade(hg->cshade2);
	FillTools->SetShade(hg->cshade);
	QString b = hg->ccolor;
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
	b = hg->cstroke;
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
	if (hg->cstyle & 4)
		StrokeTools->setEnabled(true);
	else
		StrokeTools->setEnabled(false);
	AlignTools->SetAlign(hg->cab);
	StyleTools->SetKern(hg->cextra);
	StyleTools->SetStyle(hg->cstyle & 127);
	FontTools->SetSize(hg->csize / 10.0);
	FontTools->SetFont(hg->cfont);
	FontTools->SetScale(hg->cscale);
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
	ScApp->CallDLL( tr("&Insert Special") );
	Editor->insChars(ScApp->DLLReturn);
	Editor->insert(ScApp->DLLReturn);
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
	int t = QMessageBox::warning(this, tr("Warning"),
	                             tr("Do you really want to clear all your Text?"),
	                             QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);
	if (t == QMessageBox::No)
		return false;
	Editor->StyledText.clear();
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

void StoryEditor::Do_copy()
{
/*	SEditor *cp = dynamic_cast<SEditor*>(table1->cellWidget(table1->currentRow(), 1));
	cp->copy();
	table1->adjHeight(table1->currentRow()); */
}

void StoryEditor::Do_paste()
{
/*	SEditor *cp = dynamic_cast<SEditor*>(table1->cellWidget(table1->currentRow(), 1));
	cp->paste();
	table1->adjHeight(table1->currentRow()); */
}

void StoryEditor::Do_cut()
{
/*	SEditor *cp = dynamic_cast<SEditor*>(table1->cellWidget(table1->currentRow(), 1));
	cp->cut();
	table1->adjHeight(table1->currentRow()); */
}

void StoryEditor::Do_del()
{
/*	SEditor *cp = dynamic_cast<SEditor*>(table1->cellWidget(table1->currentRow(), 1));
	cp->del();
	table1->adjHeight(table1->currentRow()); */
}

void StoryEditor::CopyAvail(bool u)
{
	emenu->setItemEnabled(Mcopy, u);
	emenu->setItemEnabled(Mcut, u);
	emenu->setItemEnabled(Mdel, u);
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
	Editor->saveItemText(nb);
	while (nb != 0)
	{
		if (doc->Trenner->AutoCheck)
		{
			if (doc->Trenner->Language != nb->Language)
				doc->Trenner->slotNewDict(nb->Language);
			doc->Trenner->slotHyphenate(nb);
		}
		bool savre = doc->RePos;
		doc->RePos = true;
		QPixmap pgPix(1, 1);
		ScPainter *painter = new ScPainter(&pgPix, 1, 1);
		painter->translate(0.5, 0.5);
		nb->DrawObj(painter, QRect(0, 0, 1, 1));
		painter->end();
		delete painter;
		doc->RePos = savre;
		nb->OwnPage->RefreshItem(nb);
		nb = nb->NextBox;
	}
	TextChanged = false;
	emenu->setItemEnabled(Mupdt, 0);
	fmenu->setItemEnabled(fid52, 0);
	emit DocChanged();
}

void StoryEditor::SearchText()
{
/*	SearchReplace* dia = new SearchReplace(this, doc, 0, CurrItem, false);
	dia->exec();
	delete dia; */
}

void StoryEditor::slotEditStyles()
{
/*	int sty;
	QComboBox *ct;
	emit EditSt();
	style.clear();
	if (doc->Vorlagen.count() > 5)
	{
		for (uint a = 5; a < doc->Vorlagen.count(); ++a)
			style.append(doc->Vorlagen[a].Vname);
	}
	style.sort();
	QString tmp[] = { tr("Forced"), tr("Block"), tr("Right"), tr("Center"), tr("Left") };
	size_t ar = sizeof(tmp) / sizeof(*tmp);
	for (uint a = 0; a < ar; ++a)
		style.prepend(tmp[a]);
	for (uint b = 0; b < stList.count(); ++b)
	{
		ct = stList.at(b);
		sty = ct->currentItem();
		QString StText = ct->text(sty);
		int align = 0;
		if (sty > 4)
		{
			for (uint x = 5; x < style.count(); ++x)
			{
				if (style[x] == StText)
				{
					align = x;
					break;
				}
			}
		}
		else
			align = sty;
		disconnect(ct, SIGNAL(activated(int)), this, SLOT(styleChange(int)));
		ct->clear();
		ct->insertStringList(style);
		connect(ct, SIGNAL(highlighted(int)), this, SLOT(styleChange(int)));
		ct->setCurrentItem(align);
		disconnect(ct, SIGNAL(highlighted(int)), this, SLOT(styleChange(int)));
		connect(ct, SIGNAL(activated(int)), this, SLOT(styleChange(int)));
	} */
}

void StoryEditor::styleChange(int st)
{
	if (st == 0)
		Editor->CurrentABStil = 0;
	else
		Editor->CurrentABStil = st+4;
	int align = 0;
	if (st > 0)
	{
		for (uint x = 5; x < doc->Vorlagen.count(); ++x)
		{
			if (doc->Vorlagen[x].Vname == AlignTools->Spal->text(st))
			{
				align = doc->Vorlagen[x].Ausri;
				break;
			}
		}
	}
	else
		align = 0;
	changeAlign(align);
}

void StoryEditor::newAlign(int st)
{
	Editor->CurrentABStil = st;
	changeAlign(st);
}

void StoryEditor::changeAlign(int align)
{
	switch (align)
	{
	case 0:
		Editor->setAlignment(Qt::AlignLeft);
		break;
	case 1:
		Editor->setAlignment(Qt::AlignCenter);
		break;
	case 2:
		Editor->setAlignment(Qt::AlignRight);
		break;
	case 3:
	case 4:
		Editor->setAlignment(Qt::AlignJustify);
		break;
	default:
		break;
	}
	int p, i;
	Editor->getCursorPosition(&p, &i);
	if (Editor->StyledText.at(p)->count() > 0)
	{
		for (uint s = 0; s < Editor->StyledText.at(p)->count(); ++s)
		{
			if (Editor->CurrentABStil > 4)
			{
				if (doc->Vorlagen[Editor->CurrentABStil].Font != "")
				{
					Editor->StyledText.at(p)->at(s)->cfont = doc->Vorlagen[Editor->CurrentABStil].Font;
					Editor->StyledText.at(p)->at(s)->csize = doc->Vorlagen[Editor->CurrentABStil].FontSize;
					Editor->StyledText.at(p)->at(s)->cstyle &= ~127;
					Editor->StyledText.at(p)->at(s)->cstyle |= doc->Vorlagen[Editor->CurrentABStil].FontEffect;
					Editor->StyledText.at(p)->at(s)->ccolor = doc->Vorlagen[Editor->CurrentABStil].FColor;
					Editor->StyledText.at(p)->at(s)->cshade = doc->Vorlagen[Editor->CurrentABStil].FShade;
					Editor->StyledText.at(p)->at(s)->cstroke = doc->Vorlagen[Editor->CurrentABStil].SColor;
					Editor->StyledText.at(p)->at(s)->cshade2 = doc->Vorlagen[Editor->CurrentABStil].SShade;
				}
			}
			if ((Editor->CurrentABStil < 5) && (Editor->StyledText.at(p)->at(s)->cab > 4))
			{
				Editor->StyledText.at(p)->at(s)->ccolor = CurrItem->TxtFill;
				Editor->StyledText.at(p)->at(s)->cshade = CurrItem->ShTxtFill;
				Editor->StyledText.at(p)->at(s)->cstroke = CurrItem->TxtStroke;
				Editor->StyledText.at(p)->at(s)->cshade2 = CurrItem->ShTxtStroke;
				Editor->StyledText.at(p)->at(s)->cfont = CurrItem->IFont;
				Editor->StyledText.at(p)->at(s)->csize = CurrItem->ISize;
				Editor->StyledText.at(p)->at(s)->cstyle &= ~127;
				Editor->StyledText.at(p)->at(s)->cstyle |= CurrItem->TxTStyle;
			}
			Editor->StyledText.at(p)->at(s)->cab = Editor->CurrentABStil;
		}
		disconnect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
		Editor->updateFromChars(p);
		connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
		updateProps(p, i);
	}
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::modifiedText()
{
	TextChanged = true;
	emenu->setItemEnabled(Mupdt, 1);
	fmenu->setItemEnabled(fid52, 1);
	updateStatus();
}

void StoryEditor::LoadTextFile()
{
	if (Do_new())
	{
		QString LoadEnc = "";
		QString fileName = "";
		CustomFDialog dia(this, tr("Open"), tr("Text Files (*.txt);;All Files(*)"), false, true, false, true);
		if (dia.exec() != QDialog::Accepted)
			return;
		LoadEnc = dia.TxCodeM->currentText();
		fileName =  dia.selectedFile();
		if (!fileName.isEmpty())
		{
			Serializer *ss = new Serializer(fileName);
			if (ss->Read(LoadEnc))
			{
				QString data = ss->GetObjekt();
				QString Dat = "";
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
	}
}

void StoryEditor::SaveTextFile()
{
	QString LoadEnc = "";
	QString fileName = "";
	CustomFDialog dia(this, tr("Save as"), tr("Text Files (*.txt);;All Files(*)"), false, false, false, true);
	if (dia.exec() != QDialog::Accepted)
		return;
	LoadEnc = dia.TxCodeM->currentText();
	fileName =  dia.selectedFile();
	if (!fileName.isEmpty())
	{
		Serializer *ss = new Serializer(fileName);
		ss->Objekt = Editor->text();
		ss->Write(LoadEnc);
		delete ss;
	}
}
