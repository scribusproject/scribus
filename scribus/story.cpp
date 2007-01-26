/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include "colorcombo.h"
#include "scfonts.h"
#include "story.h"
#include "story.moc"
#include <qtooltip.h>
#include <qpixmap.h>
#include <qcombobox.h>
#include <qregexp.h>
#include <qhbox.h>
#include <qcolordialog.h>
#include <qfontdialog.h>
#include <qcursor.h>
#include <qtextcodec.h>

#include "actionmanager.h"
#include "alignselect.h"
#include "charselect.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "editformats.h"
#include "fontcombo.h"
#include "menumanager.h"
#include "mspinbox.h"
#include "pageitem.h"
#include "pluginmanager.h"
#include "prefscontext.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "scmessagebox.h"
#include "scraction.h"
#include "scribuscore.h"
#include "search.h"
#include "serializer.h"
#include "shadebutton.h"
#include "spalette.h"
#include "styleselect.h"
#include "util.h"
#include "scplugin.h"
#include "text/nlsconfig.h"

extern QPixmap loadIcon(QString nam);


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
	int p=0, i=0;
	editor->getCursorPosition(&p, &i);
	int pos = editor->StyledText.startOfParagraph(p) + i;
	pmen = new QPopupMenu();
	ParaStyleComboBox* paraStyleCombo = new ParaStyleComboBox(this);
	paraStyleCombo->setDoc(editor->doc);
	if ((CurrentPar < static_cast<int>(editor->StyledText.nrOfParagraphs())) && (editor->StyledText.length() != 0))
	{
		int len = editor->StyledText.endOfParagraph(CurrentPar) - editor->StyledText.startOfParagraph(CurrentPar);
		if (len > 0)
			paraStyleCombo->setFormat(editor->StyledText.paragraphStyle(pos).displayName());
		else
			paraStyleCombo->setFormat("");
	}
	else
		paraStyleCombo->setFormat("");
	connect(paraStyleCombo, SIGNAL(newStyle(int)), this, SLOT(setPStyle(int)));
	pmen->insertItem(paraStyleCombo);
	pmen->insertItem( tr("Edit Styles..."), this, SLOT(editStyles()));
	pmen->exec(QCursor::pos());
	delete pmen;
}

void SideBar::editStyles()
{
	emit sigEditStyles();
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
		for (uint pa = 0; pa < editor->StyledText.nrOfParagraphs(); ++pa)
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
				QString parname = editor->StyledText.paragraphStyle(editor->StyledText.startOfParagraph(pa)).parent();
				if (parname.isEmpty())
					parname = tr("No Style");
				p.drawText(re, Qt::AlignLeft | Qt::AlignTop, parname);
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

SEditor::SEditor(QWidget* parent, ScribusDoc *docc, StoryEditor* parentSE) : QTextEdit(parent)
{
	setCurrentDocument(docc);
	parentStoryEditor=parentSE;
	wasMod = false;
	StoredSel = false;
	StyledText.clear();
	cBuffer.clear();
	setUndoRedoEnabled(true);
	setUndoDepth(0);
	setTextFormat(Qt::PlainText);
	viewport()->setAcceptDrops(false);
	ClipData = 0;
	unicodeTextEditMode = false;
	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
//	connect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(SelClipChange()));
}

void SEditor::setCurrentDocument(ScribusDoc *docc)
{
	doc = docc;
	StyledText = StoryText(docc);
}

void SEditor::imEndEvent(QIMEvent *e)
{
	QString uc = e->text();
	if ((!uc.isEmpty()) && ((*doc->AllFonts)[CurrFont].canRender(uc[0])))
	{
		insChars(e->text());
		QTextEdit::imEndEvent(e);
		emit SideBarUp(true);
		emit SideBarUpdate();
	}
}

void SEditor::keyPressEvent(QKeyEvent *k)
{
	emit SideBarUp(false);
	int p=0, i=0;
	getCursorPosition(&p, &i);
	int pos = StyledText.startOfParagraph(p) + i;
	int keyMod=0;
	if (k->state() & ShiftButton)
		keyMod |= SHIFT;
	if (k->state() & ControlButton)
		keyMod |= CTRL;
	if (k->state() & AltButton)
		keyMod |= ALT;

	QString uc = k->text();
	switch (k->state())
	{
		case ControlButton:
		case ControlButton|ShiftButton:
		case ControlButton|Keypad:
		case ControlButton|ShiftButton|Keypad:
			switch (k->key())
			{
				case Key_Delete:
					moveCursor(QTextEdit::MoveWordForward, true);
					deleteSel();
					break;
				case Key_Backspace:
					moveCursor(QTextEdit::MoveWordBackward, true);
					deleteSel();
					break;
				case Key_K:
					moveCursor(QTextEdit::MoveLineEnd, true);
					deleteSel();
					break;
				case Key_D:
					moveCursor(QTextEdit::MoveForward, true);
					deleteSel();
					break;
				case Key_H:
					moveCursor(QTextEdit::MoveBackward, true);
					deleteSel();
					break;
				case Key_X:
					cut();
					return;
					break;
				case Key_V:
					paste();
					return;
					break;
				case Key_Y:
				case Key_Z:
					emit SideBarUp(true);
					return;
					break;
				case Key_C:
					copyStyledText();
					break;
			}
			break;
		case NoButton:
		case Keypad:
		case ShiftButton:
		case ControlButton|AltButton:
		case ControlButton|AltButton|ShiftButton: // Shift + AltGr on Windows for polish characters
			if (unicodeTextEditMode)
			{
				int conv = 0;
				bool ok = false;
				unicodeInputString += k->text();
				conv = unicodeInputString.toInt(&ok, 16);
				if (!ok)
				{
					unicodeTextEditMode = false;
					unicodeInputCount = 0;
					unicodeInputString = "";
					return;
				}
				unicodeInputCount++;
				if (unicodeInputCount == 4)
				{
					unicodeTextEditMode = false;
					unicodeInputCount = 0;
					unicodeInputString = "";
					if (ok)
					{
						if (conv < 31)
							conv = 32;
						insChars(QString(QChar(conv)));
 						insert(QString(QChar(conv)));
						emit SideBarUp(true);
						emit SideBarUpdate();
						return;
					}
				}
				else
				{
					emit SideBarUp(true);
					emit SideBarUpdate();
					return;
				}
			}
			wasMod = false;
			switch (k->key())
			{
				case Key_Escape:
					k->ignore();
					break;
				case Key_Shift:
				case Key_Control:
				case Key_Alt:
					wasMod = true;
					break;
				case Key_F12:
					unicodeTextEditMode = true;
					unicodeInputCount = 0;
					unicodeInputString = "";
					return;
					break;
				case Key_Delete:
					if (!hasSelectedText())
					{
						if (pos < StyledText.length())
							StyledText.removeChars(pos, 1);
					}
					else
						deleteSel();
					break;
				case Key_Backspace:
					if (!hasSelectedText())
					{
						if (pos > 0)
							StyledText.removeChars(pos-1, 1);
					}
					else
						deleteSel();
					break;
				case Key_Return:
				case Key_Enter:
					{
						if (hasSelectedText()) {
							pos = StyledText.startOfSelection();
							deleteSel();
						}
						StyledText.insertChars(pos, SpecialChars::PARSEP);
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
					if ((!k->text().isEmpty()) && ((*doc->AllFonts)[CurrFont].canRender(uc[0])))
					{
						insChars(k->text());
						QTextEdit::keyPressEvent(k);
						emit SideBarUp(true);
						emit SideBarUpdate();
					}
					return;
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
	else {
		StoredSel = false;
	}
	QTextEdit::focusOutEvent(e);
}

void SEditor::focusInEvent(QFocusEvent *e)
{
	if (StoredSel)
	{
		setSelection(SelParaStart, SelCharStart, SelParaEnd, SelCharEnd);
		StoredSel = false;
	}
	QTextEdit::focusInEvent(e);
}

void SEditor::insChars(QString t)
{
	int p=0, i=0;
	if (hasSelectedText())
		deleteSel();
	getCursorPosition(&p, &i);
	int pos = QMIN(StyledText.startOfParagraph(p) + i, StyledText.length());
	StyledText.insertChars(pos, t);
}

void SEditor::insStyledText()
{
	if (cBuffer.length() == 0)
		return;
	int p=0, i=0;
	if (hasSelectedText())
		deleteSel();
	getCursorPosition(&p, &i);
	int pos = QMIN(StyledText.startOfParagraph(p) + i, StyledText.length());
	StyledText.insert(pos, cBuffer);
}

void SEditor::copyStyledText()
{
	int PStart, PEnd, SelStart, SelEnd, start, end;
	getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
	start = StyledText.startOfParagraph(PStart) + SelStart;
	end = StyledText.startOfParagraph(PEnd) + SelEnd;
	if (start < 0 || end <= start)
		return;
	StyledText.select(start, end-start);
	cBuffer.clear();
	cBuffer.insert(0, StyledText, true);
}

void SEditor::saveItemText(PageItem *currItem)
{
	currItem->CPos = 0;
	currItem->itemText.clear();
	currItem->itemText.append(StyledText);
/* uh... FIXME
		if (ch == SpecialChars::OBJECT)
			{
				PageItem* embedded = chars->at(c)->cembedded;
				currItem->doc()->FrameItems.append(embedded);
				if (embedded->Groups.count() != 0)
				{
					for (uint ga=0; ga<FrameItems.count(); ++ga)
					{
						if (FrameItems.at(ga)->Groups.count() != 0)
						{
							if (FrameItems.at(ga)->Groups.top() == embedded->Groups.top())
							{
								if (FrameItems.at(ga)->ItemNr != embedded->ItemNr)
								{
									if (currItem->doc()->FrameItems.find(FrameItems.at(ga)) == -1)
										currItem->doc()->FrameItems.append(FrameItems.at(ga));
								}
							}
						}
					}
				}
				currItem->itemText.insertObject(pos, embedded);
			}
*/
}

void SEditor::setAlign(int align)
{
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


void SEditor::loadItemText(PageItem *currItem)
{
	StyledText.clear();
	FrameItems.clear();
	StyledText.append(currItem->itemText);
	updateAll();
	int npars = currItem->itemText.nrOfParagraphs();
	int currPar = 0;
	int currChar;
	while (currItem->CPos >= (currChar = currItem->itemText.endOfParagraph(currPar))
		   && currPar < npars)
		++currPar;
	if (currItem->CPos < currChar)
		currChar = currItem->CPos;
	currChar -= currItem->itemText.startOfParagraph(currPar);
	setCursorPosition(currPar, currChar);
	emit setProps(currPar, currChar);
}

void SEditor::loadText(QString tx, PageItem *currItem)
{
	setUpdatesEnabled(false);
	QString Text = "";
	StyledText.clear();
	StyledText.setDefaultStyle(currItem->itemText.defaultStyle());
	StyledText.insertChars(0, tx);
	updateAll();
	if (StyledText.length() != 0)
		emit setProps(0, 0);
	setCursorPosition(0, 0);
}

void SEditor::updateAll()
{
	clear();
	if (StyledText.length() == 0)
		return;
	setUpdatesEnabled(false);
	int p=0, i=0;
	getCursorPosition(&p, &i);
	QString Text = "";
	QString chars;
	int Csty = StyledText.charStyle(0).effects();
	int Ali = StyledText.paragraphStyle(0).alignment();
	setAlign(Ali);
	setStyle(Csty);
	for (uint pa = 0; pa < StyledText.nrOfParagraphs(); ++pa)
	{
		int start = StyledText.startOfParagraph(pa);
		int end = StyledText.endOfParagraph(pa);
		const ParagraphStyle& pstyle(StyledText.paragraphStyle(start));
		Ali = pstyle.alignment();
		setAlign(Ali);
		if (start >= end && pa < StyledText.nrOfParagraphs()-1)
		{
			Text += "\n";
			continue;
		}
		for (int a = start; a < end; ++a)
		{
			const CharStyle& cstyle(StyledText.charStyle(a));
			const QChar ch = StyledText.text(a);
			if (Csty != cstyle.effects() ||
				ch == SpecialChars::OBJECT ||
				ch == SpecialChars::PAGENUMBER ||
				ch == SpecialChars::NBSPACE ||
				ch == SpecialChars::FRAMEBREAK ||
				ch == SpecialChars::COLBREAK ||
				ch == SpecialChars::NBHYPHEN ||
				ch == SpecialChars::LINEBREAK)
			{
				setAlign(Ali);
				setStyle(Csty);
				insert(Text);
				Text = "";
				Csty = cstyle.effects();
			}

			if (ch == SpecialChars::OBJECT)
			{
				setFarbe(true);
				insert("@");
				setFarbe(false);
			}
			else if (ch == SpecialChars::PAGENUMBER)
			{
				setFarbe(true);
				insert("#");
				setFarbe(false);
			}
			else if (ch == SpecialChars::NBSPACE)
			{
				setFarbe(true);
				insert("_");
				setFarbe(false);
			}
			else if (ch == SpecialChars::FRAMEBREAK)
			{
				setFarbe(true);
				insert("|");
				setFarbe(false);
			}
			else if (ch == SpecialChars::COLBREAK)
			{
				setFarbe(true);
				insert("^");
				setFarbe(false);
			}
			else if (ch == SpecialChars::NBHYPHEN)
			{
				setFarbe(true);
				insert("=");
				setFarbe(false);
			}
			else if (ch == SpecialChars::LINEBREAK)
			{
				setFarbe(true);
				insert("*");
				setFarbe(false);
			}
			else
				Text += ch;
		}
		if (pa < StyledText.nrOfParagraphs()-1)
			Text += "\n";
	}
	setAlign(Ali);
	setStyle(Csty);
	insert(Text);
	setUpdatesEnabled(true);
	//CB Removed to fix 2083 setCursorPosition(p, i);
}


void SEditor::updateFromChars(int pa)
{
	int start = StyledText.startOfParagraph(pa);
	int end = StyledText.endOfParagraph(pa);
	if (start >= end)
		return;
	setUpdatesEnabled(false);
	int SelStart = 0;
	int SelEnd = 0;
	int p=0, i=0;
	getCursorPosition(&p, &i);
	removeSelection();
	int Csty = StyledText.charStyle(start).effects();
	for (int a = start; a < end; ++a)
	{
		if (Csty == StyledText.charStyle(a).effects())
			SelEnd++;
		else
		{
			setSelection(pa, SelStart, pa, SelEnd);
			setStyle(Csty);
			removeSelection();
			Csty = StyledText.charStyle(a).effects();
			SelStart = SelEnd;
			SelEnd++;
		}
	}
	setSelection(pa, SelStart, pa, SelEnd);
	setStyle(Csty);
	removeSelection();
	setAlign(StyledText.paragraphStyle(start).alignment());
	setUpdatesEnabled(true);
	setCursorPosition(p, i);
}

/* updates the internal StyledText structure, applies 'newStyle' to the selection */
void SEditor::updateSel(const ParagraphStyle& newStyle)
{
	int PStart, PEnd, SelStart, SelEnd, start;
	if (StoredSel)
	{
		setSelection(SelParaStart, SelCharStart, SelParaEnd, SelCharEnd);
		StoredSel = false;
	}
	getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
	for (int pa=PStart; pa <= PEnd; ++pa)
	{
		start = StyledText.startOfParagraph(PStart) + SelStart;
		StyledText.applyStyle(start, newStyle);
	}
}

void SEditor::updateSel(const CharStyle& newStyle)
{
	int PStart, PEnd, SelStart, SelEnd, start, end;
	if (StoredSel)
	{
		setSelection(SelParaStart, SelCharStart, SelParaEnd, SelCharEnd);
		StoredSel = false;
	}
	getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
	start = StyledText.startOfParagraph(PStart) + SelStart;
	end = StyledText.startOfParagraph(PEnd) + SelEnd;
	if (start >= 0 && start < end)
		StyledText.applyCharStyle(start, end-start, newStyle);
}


void SEditor::deleteSel()
{
	int PStart, PEnd, SelStart, SelEnd, start, end;
	getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
	start = StyledText.startOfParagraph(PStart) + SelStart;
	end = StyledText.startOfParagraph(PEnd) + SelEnd;
	if (end > start)
		StyledText.removeChars(start, end-start);
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

void SEditor::setFarbe(bool marker)
{
	QColor tmp;
	if (marker)
		tmp = QColor(red);
	else
		tmp = QColor(black);
	setColor(tmp);
}

void SEditor::copy()
{
	emit SideBarUp(false);
	if ((hasSelectedText()) && (!selectedText().isEmpty()))
	{
		disconnect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
//		disconnect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(SelClipChange()));
		copyStyledText();
		QApplication::clipboard()->setText(tBuffer, QClipboard::Clipboard);
		ClipData = 1;
		connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
//		connect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(SelClipChange()));
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
	int currentPara, currentCharPos;
	QString data = "";
	int newParaCount, lengthLastPara;
	int advanceLen = 0;
//	bool inserted=false;
	getCursorPosition(&currentPara, &currentCharPos);
	if (ClipData == 1)
	{
		advanceLen = cBuffer.length();
		insStyledText();
	}
	else
	{
//		QString data = QApplication::clipboard()->text(QClipboard::Selection);
//		if (data.isNull())
		data = QApplication::clipboard()->text(QClipboard::Clipboard);
		if (!data.isNull())
		{
			data.replace(QRegExp("\r"), "");
			newParaCount=data.contains("\n");
			lengthLastPara=data.length()-data.findRev("\n");
			data.replace(QRegExp("\n"), SpecialChars::PARSEP);
//			inserted=true;
			advanceLen = data.length() - newParaCount;
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
	setUpdatesEnabled(false);
	setCursorPosition(currentPara, currentCharPos);
	for (int a = 0; a < advanceLen; ++a)
	{
		moveCursor(QTextEdit::MoveForward, false);
	}
	setUpdatesEnabled(true);
//	if (inserted)
//		setCursorPosition(currentPara+newParaCount,(newParaCount==0?currentCharPos:0)+lengthLastPara-1);
	sync();
	repaintContents();
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
	FillIcon->setPixmap(loadIcon("16/color-fill.png"));
	FillIcon->setScaledContents( false );
	TxFill = new ColorCombo( false, this, "TxFill" );
	PM2 = new ShadeButton(this);
	setCurrentDocument(doc);
	//TxFill->listBox()->setMinimumWidth(TxFill->listBox()->maxItemWidth()+24);
	connect(TxFill, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	connect(PM2, SIGNAL(clicked()), this, SLOT(newShadeHandler()));

	languageChange();
}

void SToolBColorF::languageChange()
{
	QToolTip::remove(TxFill);
	QToolTip::remove(PM2);
	QToolTip::add(TxFill, tr( "Color of text fill" ));
	QToolTip::add(PM2, tr( "Saturation of color of text fill" ));



}

void SToolBColorF::setCurrentDocument(ScribusDoc *doc)
{
	TxFill->clear();
	TxFill->insertItem(CommonStrings::NoneColor);
	if (doc!=NULL)
		TxFill->insertItems(doc->PageColors, ColorCombo::smallPixmaps);
	resize(minimumSizeHint());
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
	StrokeIcon->setPixmap(loadIcon("16/color-stroke.png"));
	StrokeIcon->setScaledContents( false );
	TxStroke = new ColorCombo( false, this, "TxStroke" );
	PM1 = new ShadeButton(this);
	setCurrentDocument(doc);
	//TxStroke->listBox()->setMinimumWidth(TxStroke->listBox()->maxItemWidth()+24);
	connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	connect(PM1, SIGNAL(clicked()), this, SLOT(newShadeHandler()));

	languageChange();
}

void SToolBColorS::languageChange()
{
	QToolTip::remove(TxStroke);
	QToolTip::remove(PM1);
	QToolTip::add(TxStroke, tr("Color of text stroke"));
	QToolTip::add(PM1, tr("Saturation of color of text stroke"));
}

void SToolBColorS::setCurrentDocument(ScribusDoc *doc)
{
	TxStroke->clear();
	TxStroke->insertItem(CommonStrings::NoneColor);
	if (doc!=NULL)
		TxStroke->insertItems(doc->PageColors, ColorCombo::smallPixmaps);
	resize(minimumSizeHint());
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
	trackingLabel = new QLabel( this, "trackingLabel" );
	trackingLabel->setText("");
	trackingLabel->setPixmap(loadIcon("textkern.png"));
	Extra = new MSpinBox( this, 1 );
	Extra->setValues( -300, 300, 10, 0);
	Extra->setSuffix( tr( " %" ) );

	connect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
	connect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
	connect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowHandler()));
	connect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowHandler()));
	connect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newOutlineHandler()));
	connect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newUnderlineHandler()));
	connect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newUnderlineHandler()));
	connect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newStrikeHandler()));
	connect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newStrikeHandler()));

	languageChange();
}

void SToolBStyle::languageChange()
{
	QToolTip::remove(Extra);
	QToolTip::add(Extra, tr( "Manual Tracking" ));
}

void SToolBStyle::newStrikeHandler()
{
	int x = qRound(SeStyle->StrikeVal->LPos->value() * 10.0);
	int y = qRound(SeStyle->StrikeVal->LWidth->value() * 10.0);
	emit newUnderline(x, y);
}

void SToolBStyle::newUnderlineHandler()
{
	int x = qRound(SeStyle->UnderlineVal->LPos->value() * 10.0);
	int y = qRound(SeStyle->UnderlineVal->LWidth->value() * 10.0);
	emit newUnderline(x, y);
}

void SToolBStyle::newOutlineHandler()
{
	int x = qRound(SeStyle->OutlineVal->LWidth->value() * 10.0);
	emit newOutline(x);
}

void SToolBStyle::newShadowHandler()
{
	int x = qRound(SeStyle->ShadowVal->Xoffset->value() * 10.0);
	int y = qRound(SeStyle->ShadowVal->Yoffset->value() * 10.0);
	emit NewShadow(x, y);
}

void SToolBStyle::newKernHandler()
{
	emit NewKern(qRound(Extra->value() * 10.0));
}

void SToolBStyle::setOutline(int x)
{
	disconnect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newOutlineHandler()));
	SeStyle->OutlineVal->LWidth->setValue(x / 10.0);
	connect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newOutlineHandler()));
}

void SToolBStyle::setStrike(int p, int w)
{
	disconnect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newStrikeHandler()));
	disconnect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newStrikeHandler()));
	SeStyle->StrikeVal->LWidth->setValue(w / 10.0);
	SeStyle->StrikeVal->LPos->setValue(p / 10.0);
	connect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newStrikeHandler()));
	connect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newStrikeHandler()));
}

void SToolBStyle::setUnderline(int p, int w)
{
	disconnect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newUnderlineHandler()));
	disconnect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newUnderlineHandler()));
	SeStyle->UnderlineVal->LWidth->setValue(w / 10.0);
	SeStyle->UnderlineVal->LPos->setValue(p / 10.0);
	connect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(newUnderlineHandler()));
	connect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(newUnderlineHandler()));
}

void SToolBStyle::SetShadow(int x, int y)
{
	disconnect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowHandler()));
	disconnect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowHandler()));
	SeStyle->ShadowVal->Xoffset->setValue(x / 10.0);
	SeStyle->ShadowVal->Yoffset->setValue(y / 10.0);
	connect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowHandler()));
	connect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(int)), this, SLOT(newShadowHandler()));
}

void SToolBStyle::SetStyle(int s)
{
	disconnect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
	SeStyle->setStyle(s);
	connect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
}

void SToolBStyle::SetKern(int k)
{
	disconnect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
	Extra->setValue(k / 10.0);
	connect(Extra, SIGNAL(valueChanged(int)), this, SLOT(newKernHandler()));
}

/* Toolbar for alignment of Paragraphs */
SToolBAlign::SToolBAlign(QMainWindow* parent) : QToolBar( tr("Style Settings"), parent)
{
	GroupAlign = new AlignSelect(this);
	paraStyleCombo = new ParaStyleComboBox(this);
	connect(paraStyleCombo, SIGNAL(newStyle(int)), this, SIGNAL(newParaStyle(int )));
	connect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(newAlign(int )));

	languageChange();
}

void SToolBAlign::languageChange()
{
	QToolTip::remove(paraStyleCombo);
	QToolTip::add(paraStyleCombo, tr("Style of current paragraph"));
}


void SToolBAlign::SetAlign(int s)
{
	disconnect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(newAlign(int )));
	GroupAlign->setStyle(s);
	connect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(newAlign(int )));
}

void SToolBAlign::SetParaStyle(int s)
{
	disconnect(paraStyleCombo, SIGNAL(newStyle(int)), this, SIGNAL(newParaStyle(int )));
	paraStyleCombo->selFormat(s);
	connect(paraStyleCombo, SIGNAL(newStyle(int)), this, SIGNAL(newParaStyle(int )));
}


/* Toolbar for Font related Settings */
SToolBFont::SToolBFont(QMainWindow* parent) : QToolBar( tr("Font Settings"), parent)
{
	Fonts = new FontCombo(this);
	Fonts->setMaximumSize(190, 30);
	Size = new MSpinBox( 0.5, 2048, this, 1 );
	PrefsManager* prefsManager = PrefsManager::instance();
	Size->setPrefix( "" );
	Size->setSuffix( tr( " pt" ) );
	Size->setValue(prefsManager->appPrefs.toolSettings.defSize / 10.0);
	ScaleTxt = new QLabel("", this, "ScaleTxt" );
	ScaleTxt->setPixmap(loadIcon("textscaleh.png"));
	ChScale = new MSpinBox( 10, 400,  this, 1 );
	ChScale->setValue( 100 );
	ChScale->setSuffix( tr( " %" ) );
	ScaleTxtV = new QLabel("", this, "ScaleTxtV" );
	ScaleTxtV->setPixmap(loadIcon("textscalev.png"));
	ChScaleV = new MSpinBox( 10, 400, this, 1 );
	ChScaleV->setValue( 100 );
	ChScaleV->setSuffix( tr( " %" ) );

	connect(ChScale, SIGNAL(valueChanged(int)), this, SIGNAL(NewScale(int)));
	connect(ChScaleV, SIGNAL(valueChanged(int)), this, SIGNAL(NewScaleV(int)));
	connect(Fonts, SIGNAL(activated(const QString &)), this, SIGNAL(NewFont(const QString &)));
	connect(Size, SIGNAL(valueChanged(int)), this, SLOT(newSizeHandler()));
}

void SToolBFont::languageChange()
{
	QToolTip::remove(Fonts);
	QToolTip::remove(Size);
	QToolTip::remove(ChScale);
	QToolTip::remove(ChScaleV);
	QToolTip::add(Fonts, tr("Font of selected text"));
	QToolTip::add(Size, tr("Font Size"));
	QToolTip::add(ChScale, tr("Scaling width of characters"));
	QToolTip::add(ChScaleV, tr("Scaling height of characters"));
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
	Size->setValue(s / 10.0);
	connect(Size, SIGNAL(valueChanged(int)), this, SLOT(newSizeHandler()));
}

void SToolBFont::SetScale(int s)
{
	disconnect(ChScale, SIGNAL(valueChanged(int)), this, SIGNAL(NewScale(int)));
	ChScale->setValue(s / 10.0);
	connect(ChScale, SIGNAL(valueChanged(int)), this, SIGNAL(NewScale(int)));
}

void SToolBFont::SetScaleV(int s)
{
	disconnect(ChScaleV, SIGNAL(valueChanged(int)), this, SIGNAL(NewScaleV(int)));
	ChScaleV->setValue(s / 10.0);
	connect(ChScaleV, SIGNAL(valueChanged(int)), this, SIGNAL(NewScaleV(int)));
}

void SToolBFont::newSizeHandler()
{
	emit NewSize(Size->value());
}

/* Main Story Editor Class */
// StoryEditor::StoryEditor(QWidget* parent, ScribusDoc *docc, PageItem *ite)
// 	: QMainWindow(parent, "StoryEditor", WType_TopLevel) //  WType_Dialog) //WShowModal |
// {
// 	prefsManager=PrefsManager::instance();
// 	currDoc = docc;
// 	seMenuMgr=NULL;
// 	buildGUI();
// 	currItem = ite;
// // 	charSelect = NULL;
// 	firstSet = false;
// 	activFromApp = true;
// 	Editor->loadItemText(ite);
// 	Editor->getCursorPosition(&CurrPara, &CurrChar);
// 	EditorBar->setRepaint(true);
// 	EditorBar->doRepaint();
// 	updateProps(CurrPara, CurrChar);
// 	updateStatus();
// 	textChanged = false;
// 	disconnectSignals();
// 	connectSignals();
// 	Editor->setFocus();
// 	Editor->setFarbe(false);
// 	blockUpdate = false;
// 	loadPrefs();
// 	// hack to keep charPalette visible. See destructor too - PV
// 	ScCore->primaryMainWindow()->charPalette->reparent(this, QPoint(0, 0));
// }

/* Main Story Editor Class, no current document */
StoryEditor::StoryEditor(QWidget* parent) : QMainWindow(parent, "StoryEditor", WType_TopLevel) // WType_Dialog) //WShowModal |
{
	prefsManager=PrefsManager::instance();
	currDoc = NULL;
	currItem = NULL;
// 	charSelect = NULL;
	#ifdef Q_WS_MAC
	noIcon = loadIcon("noicon.xpm");
	#endif
	buildGUI();
	CurrPara = 0;
	CurrChar = 0;
	firstSet = false;
	activFromApp = true;
	/*
	//Editor->loadItemText(ite);
	updateProps(0,0);
	updateStatus();
	*/
	textChanged = false;
	Editor->setFocus();
	Editor->setFarbe(false);
	blockUpdate = false;
	loadPrefs();
}

StoryEditor::~StoryEditor()
{
	savePrefs();
	connect(ScCore->primaryMainWindow()->charPalette,
			SIGNAL(insertSpecialChar()),
			ScCore->primaryMainWindow()->charPalette,
			SLOT(slot_insertSpecialChar()));
	connect(ScCore->primaryMainWindow()->charPalette,
			SIGNAL(insertUserSpecialChar(QChar)),
			ScCore->primaryMainWindow()->charPalette,
			SLOT(slot_insertUserSpecialChar(QChar)));
}

void StoryEditor::savePrefs()
{
	// save prefs
	QRect geo = geometry();
	prefs->set("left", geo.left());
	prefs->set("top", geo.top());
	prefs->set("width", width());
	prefs->set("height", height());
	QValueList<int> splitted = EdSplit->sizes();
	prefs->set("side", splitted[0]);
	prefs->set("main", splitted[1]);
}

void StoryEditor::loadPrefs()
{
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("StoryEditor");
	int vleft   = QMAX(-80, prefs->getInt("left", 10));
#if defined(QT_MAC) || defined(_WIN32)
	int vtop    = QMAX(64, prefs->getInt("top", 10));
#else
	int vtop    = QMAX(-80, prefs->getInt("top", 10));
#endif
	int vwidth  = QMAX(600, prefs->getInt("width", 600));
	int vheight = QMAX(400, prefs->getInt("height", 400));
	// Check values against current screen size
	QRect scr = QApplication::desktop()->screen()->geometry();
	QSize gStrut = QApplication::globalStrut();
	if ( vleft >= scr.width() )
		vleft = 0;
	if ( vtop >= scr.height() )
		vtop = 64;
	if ( vwidth >= scr.width() )
		vwidth = QMAX( gStrut.width(), scr.width() - vleft );
	if ( vheight >= scr.height() )
		vheight = QMAX( gStrut.height(), scr.height() - vtop );
	setGeometry(vleft, vtop, vwidth, vheight);
	int side = prefs->getInt("side", -1);
	int txtarea = prefs->getInt("main", -1);
	if ((side != -1) && (txtarea != -1))
	{
		QValueList<int> splitted;
		splitted.append(side);
		splitted.append(txtarea);
		EdSplit->setSizes(splitted);
	}
}

void StoryEditor::initActions()
{
	//File Menu
	seActions.insert("fileNew", new ScrAction(QIconSet(loadIcon("editdelete.png"), loadIcon("editdelete.png")), "", CTRL+Key_N, this, "fileNew"));
	seActions.insert("fileRevert", new ScrAction(QIconSet(loadIcon("reload16.png"), loadIcon("reload.png")), "", QKeySequence(), this, "fileRevert"));
	seActions.insert("fileSaveToFile", new ScrAction(QIconSet(loadIcon("16/document-save.png"), loadIcon("22/document-save.png")), "", QKeySequence(), this, "fileSaveToFile"));
	seActions.insert("fileLoadFromFile", new ScrAction(QIconSet(loadIcon("16/document-open.png"),  loadIcon("22/document-open.png")), "", QKeySequence(), this, "fileLoadFromFile"));
	seActions.insert("fileSaveDocument", new ScrAction("", CTRL+Key_S, this, "fileSaveDocument"));
	seActions.insert("fileUpdateAndExit", new ScrAction(QIconSet(loadIcon("ok.png"), loadIcon("ok22.png")), "", CTRL+Key_W,  this, "fileUpdateAndExit"));
	seActions.insert("fileExit", new ScrAction(QIconSet(loadIcon("exit.png"), loadIcon("exit22.png")), "", QKeySequence(),  this, "fileExit"));

	connect( seActions["fileNew"], SIGNAL(activated()), this, SLOT(Do_new()) );
	connect( seActions["fileRevert"], SIGNAL(activated()), this, SLOT(slotFileRevert()) );
	connect( seActions["fileSaveToFile"], SIGNAL(activated()), this, SLOT(SaveTextFile()) );
	connect( seActions["fileLoadFromFile"], SIGNAL(activated()), this, SLOT(LoadTextFile()) );
	connect( seActions["fileSaveDocument"], SIGNAL(activated()), this, SLOT(Do_saveDocument()) );
	connect( seActions["fileUpdateAndExit"], SIGNAL(activated()), this, SLOT(Do_leave2()) );
	connect( seActions["fileExit"], SIGNAL(activated()), this, SLOT(Do_leave()) );

	//Edit Menu
	seActions.insert("editSelectAll", new ScrAction(QIconSet(noIcon), "", CTRL+Key_A, this, "editSelectAll"));
	seActions.insert("editCut", new ScrAction(QIconSet(loadIcon("editcut.png")), "", CTRL+Key_X, this, "editCut"));
	seActions.insert("editCopy", new ScrAction(QIconSet(loadIcon("editcopy.png")), "", CTRL+Key_C, this, "editCopy"));
	seActions.insert("editPaste", new ScrAction(QIconSet(loadIcon("editpaste.png")), "", CTRL+Key_V, this, "editPaste"));
	seActions.insert("editClear", new ScrAction(QIconSet(loadIcon("editdelete.png")), "", Key_Delete, this, "editClear"));
	seActions.insert("editSearchReplace", new ScrAction(QIconSet(loadIcon("find16.png")), "", QKeySequence(), this, "editSearchReplace"));
	seActions.insert("editEditStyle", new ScrAction(QIconSet(noIcon), "", QKeySequence(), this, "editEditStyle"));
	seActions.insert("editFontPreview", new ScrAction(QIconSet(noIcon), "", QKeySequence(), this, "editFontPreview"));
	seActions.insert("editUpdateFrame", new ScrAction(QIconSet(loadIcon("compfile16.png"),loadIcon("compfile.png")), "", CTRL+Key_U, this, "editUpdateFrame"));

	connect( seActions["editSelectAll"], SIGNAL(activated()), this, SLOT(Do_selectAll()) );
	connect( seActions["editCut"], SIGNAL(activated()), this, SLOT(Do_cut()) );
	connect( seActions["editCopy"], SIGNAL(activated()), this, SLOT(Do_copy()) );
	connect( seActions["editPaste"], SIGNAL(activated()), this, SLOT(Do_paste()) );
	connect( seActions["editClear"], SIGNAL(activated()), this, SLOT(Do_del()) );
	connect( seActions["editSearchReplace"], SIGNAL(activated()), this, SLOT(SearchText()) );
	connect( seActions["editEditStyle"], SIGNAL(activated()), this, SLOT(slotEditStyles()) );
	connect( seActions["editFontPreview"], SIGNAL(activated()), this, SLOT(Do_fontPrev()) );
	connect( seActions["editUpdateFrame"], SIGNAL(activated()), this, SLOT(updateTextFrame()) );

	//Insert Menu
	seActions.insert("insertGlyph", new ScrAction(QIconSet(noIcon), "", QKeySequence(), this, "insertGlyph"));
	connect( seActions["insertGlyph"], SIGNAL(activated()), this, SLOT(Do_insSp()) );

	//Settings Menu
	seActions.insert("settingsBackground", new ScrAction(QIconSet(noIcon), "", QKeySequence(), this, "settingsBackground"));
	seActions.insert("settingsDisplayFont", new ScrAction(QIconSet(noIcon), "", QKeySequence(), this, "settingsDisplayFont"));
	seActions.insert("settingsSmartTextSelection", new ScrAction(QIconSet(noIcon), "", QKeySequence(), this, "settingsSmartTextSelection"));
	smartSelection = false;
	seActions["settingsSmartTextSelection"]->setOn(false);
	seActions["settingsSmartTextSelection"]->setToggleAction(true);

	connect( seActions["settingsBackground"], SIGNAL(activated()), this, SLOT(setBackPref()) );
	connect( seActions["settingsDisplayFont"], SIGNAL(activated()), this, SLOT(setFontPref()) );
	connect( seActions["settingsSmartTextSelection"], SIGNAL(toggled(bool)), this, SLOT(setSmart(bool)) );


	seActions["fileRevert"]->setEnabled(false);
	seActions["editCopy"]->setEnabled(false);
	seActions["editCut"]->setEnabled(false);
	seActions["editPaste"]->setEnabled(false);
	seActions["editClear"]->setEnabled(false);
	seActions["editUpdateFrame"]->setEnabled(false);
}

void StoryEditor::buildMenus()
{
	seMenuMgr = new MenuManager(this->menuBar(), this->menuBar());
	seMenuMgr->createMenu("File", tr("&File"));
	seMenuMgr->addMenuItem(seActions["fileNew"], "File");
	seMenuMgr->addMenuItem(seActions["fileRevert"], "File");
	seMenuMgr->addMenuSeparator("File");
	seMenuMgr->addMenuItem(seActions["fileSaveToFile"], "File");
	seMenuMgr->addMenuItem(seActions["fileLoadFromFile"], "File");
	seMenuMgr->addMenuItem(seActions["fileSaveDocument"], "File");
	seMenuMgr->addMenuSeparator("File");
	seMenuMgr->addMenuItem(seActions["fileUpdateAndExit"], "File");
	seMenuMgr->addMenuItem(seActions["fileExit"], "File");
	seMenuMgr->createMenu("Edit", tr("&Edit"));
	seMenuMgr->addMenuItem(seActions["editSelectAll"], "Edit");
	seMenuMgr->addMenuItem(seActions["editCut"], "Edit");
	seMenuMgr->addMenuItem(seActions["editCopy"], "Edit");
	seMenuMgr->addMenuItem(seActions["editPaste"], "Edit");
	seMenuMgr->addMenuItem(seActions["editClear"], "Edit");
	seMenuMgr->addMenuSeparator("Edit");
	seMenuMgr->addMenuItem(seActions["editSearchReplace"], "Edit");
	seMenuMgr->addMenuSeparator("Edit");
	seMenuMgr->addMenuItem(seActions["editEditStyle"], "Edit");
	seMenuMgr->addMenuItem(seActions["editFontPreview"], "Edit");
	seMenuMgr->addMenuItem(seActions["editUpdateFrame"], "Edit");
	seMenuMgr->createMenu("Insert", tr("&Insert"));
	seMenuMgr->addMenuItem(seActions["insertGlyph"], "Insert");
	seMenuMgr->createMenu("InsertChar", QPixmap(noIcon), tr("Character"), "Insert");
	seMenuMgr->addMenuItem(seActions["unicodePageNumber"], "InsertChar");
	//seMenuMgr->addMenuItem(seActions["unicodeSmartHyphen"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeNonBreakingHyphen"], "InsertChar");
	seMenuMgr->addMenuSeparator("InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeCopyRight"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeRegdTM"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeTM"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeSolidus"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeBullet"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeMidpoint"], "InsertChar");
	seMenuMgr->addMenuSeparator("InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeDashEm"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeDashEn"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeDashFigure"], "InsertChar");
	seMenuMgr->addMenuItem(seActions["unicodeDashQuotation"], "InsertChar");
	seMenuMgr->createMenu("InsertQuote", QPixmap(noIcon), tr("Quote"), "Insert");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteApostrophe"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteStraight"], "InsertQuote");
	seMenuMgr->addMenuSeparator("InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteSingleLeft"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteSingleRight"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteDoubleLeft"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteDoubleRight"], "InsertQuote");
	seMenuMgr->addMenuSeparator("InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteLowSingleComma"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteLowDoubleComma"], "InsertQuote");
	seMenuMgr->addMenuSeparator("InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteSingleReversed"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteDoubleReversed"], "InsertQuote");
	seMenuMgr->addMenuSeparator("InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteSingleLeftGuillemet"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteSingleRightGuillemet"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteDoubleLeftGuillemet"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteDoubleRightGuillemet"], "InsertQuote");
	seMenuMgr->addMenuSeparator("InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteCJKSingleLeft"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteCJKSingleRight"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteCJKDoubleLeft"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteCJKDoubleRight"], "InsertQuote");
	seMenuMgr->createMenu("InsertSpace", QPixmap(noIcon), tr("Spaces && Breaks"), "Insert");
	seMenuMgr->addMenuItem(seActions["unicodeNonBreakingSpace"], "InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeSpaceEN"], "InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeSpaceEM"], "InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeSpaceThin"], "InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeSpaceThick"], "InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeSpaceMid"], "InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeSpaceHair"], "InsertSpace");
	seMenuMgr->addMenuSeparator("InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeNewLine"], "InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeFrameBreak"], "InsertSpace");
	seMenuMgr->addMenuItem(seActions["unicodeColumnBreak"], "InsertSpace");
	seMenuMgr->createMenu("InsertLigature", QPixmap(noIcon), tr("Ligature"), "Insert");
	seMenuMgr->addMenuItem(seActions["unicodeLigature_ff"], "InsertLigature");
	seMenuMgr->addMenuItem(seActions["unicodeLigature_fi"], "InsertLigature");
	seMenuMgr->addMenuItem(seActions["unicodeLigature_fl"], "InsertLigature");
	seMenuMgr->addMenuItem(seActions["unicodeLigature_ffi"], "InsertLigature");
	seMenuMgr->addMenuItem(seActions["unicodeLigature_ffl"], "InsertLigature");
	seMenuMgr->addMenuItem(seActions["unicodeLigature_ft"], "InsertLigature");
	seMenuMgr->addMenuItem(seActions["unicodeLigature_st"], "InsertLigature");

	seMenuMgr->createMenu("Settings", tr("&Settings"));
	seMenuMgr->addMenuItem(seActions["settingsBackground"], "Settings");
	seMenuMgr->addMenuItem(seActions["settingsDisplayFont"], "Settings");
	seMenuMgr->addMenuItem(seActions["settingsSmartTextSelection"], "Settings");

	seMenuMgr->addMenuToMenuBar("File");
	seMenuMgr->addMenuToMenuBar("Edit");
	seMenuMgr->addMenuToMenuBar("Insert");
	seMenuMgr->addMenuToMenuBar("Settings");
}

void StoryEditor::buildGUI()
{
	unicodeCharActionNames.clear();
	seActions.clear();
	initActions();
	ActionManager::initUnicodeActions(&seActions, this, &unicodeCharActionNames);
	seActions["unicodeSmartHyphen"]->setEnabled(false);//CB TODO doesnt work in SE yet.
	buildMenus();

	setIcon(loadIcon("AppIcon.png"));
	QHBox* vb = new QHBox( this );
	StoryEd2Layout = new QHBoxLayout( 0, 5, 5, "StoryEd2Layout");

/* Setting up Toolbars */
	FileTools = new QToolBar(this);
	seActions["fileNew"]->addTo(FileTools);
	seActions["fileLoadFromFile"]->addTo(FileTools);
	seActions["fileSaveToFile"]->addTo(FileTools);
	seActions["fileUpdateAndExit"]->addTo(FileTools);
	seActions["fileExit"]->addTo(FileTools);
	seActions["fileRevert"]->addTo(FileTools);
	seActions["editUpdateFrame"]->addTo(FileTools);
	seActions["editSearchReplace"]->addTo(FileTools);

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
	AlignTools->paraStyleCombo->setDoc(currDoc);
	StyleTools = new SToolBStyle(this);
	setDockEnabled(StyleTools, DockLeft, false);
	setDockEnabled(StyleTools, DockRight, false);
	setDockEnabled(StyleTools, DockBottom, false);
	StrokeTools = new SToolBColorS(this, currDoc);
	setDockEnabled(StrokeTools, DockLeft, false);
	setDockEnabled(StrokeTools, DockRight, false);
	setDockEnabled(StrokeTools, DockBottom, false);
	StrokeTools->TxStroke->setEnabled(false);
	StrokeTools->PM1->setEnabled(false);
	FillTools = new SToolBColorF(this, currDoc);
	setDockEnabled(FillTools, DockLeft, false);
	setDockEnabled(FillTools, DockRight, false);
	setDockEnabled(FillTools, DockBottom, false);

	EdSplit = new QSplitter(vb);
/* SideBar Widget */
	EditorBar = new SideBar(EdSplit);
/* Editor Widget, subclass of QTextEdit */
	Editor = new SEditor(EdSplit, currDoc, this);
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
	ButtonGroup1Layout->addMultiCellWidget( WordCT1, 0, 0, 0, 3 );
	WordCT = new QLabel(ButtonGroup1, "wt");
	ButtonGroup1Layout->addWidget( WordCT, 1, 0 );
	WordC = new QLabel(ButtonGroup1, "wc");
	ButtonGroup1Layout->addWidget( WordC, 1, 1 );
	CharCT = new QLabel(ButtonGroup1, "ct");
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
	ButtonGroup2Layout->addMultiCellWidget( WordCT3, 0, 0, 0, 5 );
	ParCT = new QLabel(ButtonGroup2, "pt");
	ButtonGroup2Layout->addWidget( ParCT, 1, 0 );
	ParC = new QLabel(ButtonGroup2, "pc");
	ButtonGroup2Layout->addWidget( ParC, 1, 1 );
	WordCT2 = new QLabel(ButtonGroup2, "wt");
	ButtonGroup2Layout->addWidget( WordCT2, 1, 2 );
	WordC2 = new QLabel(ButtonGroup2, "wc");
	ButtonGroup2Layout->addWidget( WordC2, 1, 3 );
	CharCT2 = new QLabel(ButtonGroup2, "ct");
	ButtonGroup2Layout->addWidget( CharCT2, 1, 4 );
	CharC2 = new QLabel(ButtonGroup2, "cc");
	ButtonGroup2Layout->addWidget( CharC2, 1, 5 );
	statusBar()->addWidget(ButtonGroup2, 1, true);
	setCentralWidget( vb );
	//Final setup
	resize( QSize(660, 500).expandedTo(minimumSizeHint()) );
	if (prefsManager==NULL)
		sDebug(QString("%1").arg("prefsmgr null"));

	Editor->setPaper(prefsManager->appPrefs.STEcolor);
	QFont fo;
	fo.fromString(prefsManager->appPrefs.STEfont);
	Editor->setFont(fo);
	EditorBar->setFrameStyle(Editor->frameStyle());
	EditorBar->setLineWidth(Editor->lineWidth());
	EditorBar->editor = Editor;
	Editor->installEventFilter(this);
	languageChange();
}

void StoryEditor::languageChange()
{
	setCaption( tr( "Story Editor" ) );
	//File Menu
	seMenuMgr->setMenuText("File", tr("&File"));
	seActions["fileNew"]->setMenuText( tr("&New"));
	seActions["fileNew"]->setText( tr("Clear All Text"));
	seActions["fileRevert"]->setTexts( tr("&Reload Text from Frame"));
	seActions["fileSaveToFile"]->setTexts( tr("&Save to File..."));
	seActions["fileLoadFromFile"]->setTexts( tr("&Load from File..."));
	seActions["fileSaveDocument"]->setTexts( tr("Save &Document"));
	seActions["fileUpdateAndExit"]->setTexts( tr("&Update Text Frame and Exit"));
	seActions["fileExit"]->setTexts( tr("&Exit Without Updating Text Frame"));
	//Edit Menu
	seMenuMgr->setMenuText("Edit", tr("&Edit"));
	seActions["editSelectAll"]->setTexts( tr("Select &All"));
	seActions["editCut"]->setTexts( tr("Cu&t"));
	seActions["editCopy"]->setTexts( tr("&Copy"));
	seActions["editPaste"]->setTexts( tr("&Paste"));
	seActions["editClear"]->setTexts( tr("C&lear"));
	seActions["editSearchReplace"]->setTexts( tr("&Search/Replace..."));
	seActions["editEditStyle"]->setTexts( tr("&Edit Styles..."));
	seActions["editFontPreview"]->setTexts( tr("&Fonts Preview..."));
	seActions["editUpdateFrame"]->setTexts( tr("&Update Text Frame"));

	//Insert menu
	seMenuMgr->setMenuText("Insert", tr("&Insert"));
	seMenuMgr->setMenuText("InsertChar", tr("Character"));
	seMenuMgr->setMenuText("InsertQuote", tr("Quote"));
	seMenuMgr->setMenuText("InsertSpace", tr("Space"));
	seActions["insertGlyph"]->setTexts( tr("&Insert Glyph..."));

	//Settings Menu
	seMenuMgr->setMenuText("Settings", tr("&Settings"));
	seActions["settingsBackground"]->setTexts( tr("&Background..."));
	seActions["settingsDisplayFont"]->setTexts( tr("&Display Font..."));
	seActions["settingsSmartTextSelection"]->setTexts( tr("&Smart text selection"));

	//Unicode Actions
	ActionManager::languageChangeUnicodeActions(&seActions);

	FileTools->setLabel( tr("File"));

	WordCT1->setText( tr("Current Paragraph:"));
	WordCT->setText( tr("Words: "));
	CharCT->setText( tr("Chars: "));
	WordCT3->setText( tr("Totals:"));
	ParCT->setText( tr("Paragraphs: "));
	WordCT2->setText( tr("Words: "));
	CharCT2->setText( tr("Chars: "));

}

void StoryEditor::disconnectSignals()
{
	disconnect(Editor,0,0,0);
	disconnect(EditorBar,0,0,0);
	disconnect(AlignTools,0,0,0);
	disconnect(FillTools,0,0,0);
	disconnect(FontTools,0,0,0);
	disconnect(StrokeTools,0,0,0);
	disconnect(StyleTools,0,0,0);
}

void StoryEditor::connectSignals()
{
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
	connect(EditorBar, SIGNAL(ChangeStyle(int, int )), this, SLOT(changeStyleSB(int, int )));
	connect(EditorBar, SIGNAL(sigEditStyles()), this, SLOT(slotEditStyles()));
	connect(AlignTools, SIGNAL(newParaStyle(int)), this, SLOT(newStyle(int)));
	connect(AlignTools, SIGNAL(newAlign(int)), this, SLOT(newAlign(int)));
	connect(FillTools, SIGNAL(NewColor(int, int)), this, SLOT(newTxFill(int, int)));
	connect(StrokeTools, SIGNAL(NewColor(int, int)), this, SLOT(newTxStroke(int, int)));
	connect(FontTools, SIGNAL(NewSize(double )), this, SLOT(newTxSize(double)));
	connect(FontTools, SIGNAL(NewFont(const QString& )), this, SLOT(newTxFont(const QString& )));
	connect(FontTools, SIGNAL(NewScale(int )), this, SLOT(newTxScale(int )));
	connect(FontTools, SIGNAL(NewScaleV(int )), this, SLOT(newTxScaleV(int )));
	connect(StyleTools, SIGNAL(NewKern(int )), this, SLOT(newTxKern(int )));
	connect(StyleTools, SIGNAL(newStyle(int )), this, SLOT(newTxStyle(int )));
	connect(StyleTools, SIGNAL(NewShadow(int, int)), this, SLOT(newShadowOffs(int, int)));
	connect(StyleTools, SIGNAL(newOutline(int )), this, SLOT(newTxtOutline(int )));
	connect(StyleTools, SIGNAL(newUnderline(int, int)), this, SLOT(newTxtUnderline(int, int)));
	connect(StyleTools, SIGNAL(newStrike(int, int )), this, SLOT(newTxtStrike(int, int)));
	// PV - char palette
	connect(ScCore->primaryMainWindow()->charPalette, SIGNAL(insertSpecialChar()), this, SLOT(slot_insertSpecialChar()));
	disconnect(ScCore->primaryMainWindow()->charPalette, SIGNAL(insertSpecialChar()), ScCore->primaryMainWindow()->charPalette, SLOT(slot_insertSpecialChar()));
	connect(ScCore->primaryMainWindow()->charPalette, SIGNAL(insertUserSpecialChar(QChar)), this, SLOT(slot_insertUserSpecialChar(QChar)));
	disconnect(ScCore->primaryMainWindow()->charPalette, SIGNAL(insertUserSpecialChar(QChar)), ScCore->primaryMainWindow()->charPalette, SLOT(slot_insertUserSpecialChar(QChar)));
}

void StoryEditor::setCurrentDocumentAndItem(ScribusDoc *doc, PageItem *item)
{
	disconnectSignals();
	currDoc=doc;
	textChanged=false;
	AlignTools->paraStyleCombo->setDoc(currDoc);
	StrokeTools->setCurrentDocument(currDoc);
	FillTools->setCurrentDocument(currDoc);
	Editor->setCurrentDocument(currDoc);
	currItem = item;
	if (currItem!=NULL)
	{
		setCaption( tr("Story Editor - %1").arg(currItem->itemName()));
		firstSet = false;
		Editor->loadItemText(currItem);
		Editor->getCursorPosition(&CurrPara, &CurrChar);
		Editor->sync();
		Editor->repaintContents();
		EditorBar->setRepaint(true);
		EditorBar->doRepaint();
		updateProps(0,0);
		updateStatus();
		connectSignals();
	}
	else
	{
		Editor->StyledText.clear();
		Editor->clear();
		setCaption( tr( "Story Editor" ));
	}
	QString data = QApplication::clipboard()->text(QClipboard::Clipboard);
	if (!data.isNull())
		seActions["editPaste"]->setEnabled(true);
}

/** 10/12/2004 - pv - #1203: wrong selection on double click
Catch the double click signal - cut the wrong selection (with
whitespaces on the tail) - select only one word - return
controlling back to story editor - have rest */
void StoryEditor::doubleClick(int para, int position)
{
	int paraFrom=0, indexFrom=0, paraTo=0, indexTo=0;
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

void StoryEditor::setSmart(bool newSmartSelection)
{
	smartSelection = newSmartSelection;
}

void StoryEditor::closeEvent(QCloseEvent *)
{
	if (textChanged)
	{
		blockUpdate = true;
		int t = ScMessageBox::warning(this, CommonStrings::trWarning,
									tr("Do you want to save your changes?"),
									QMessageBox::Yes|QMessageBox::Default,
									QMessageBox::No,
									QMessageBox::Cancel|QMessageBox::Escape);
		qApp->processEvents();
		if (t == QMessageBox::Yes)
		{
			updateTextFrame();
			result = QDialog::Accepted;
		}
		else if (t == QMessageBox::Cancel)
		{
			blockUpdate = false;
			return;
		}
		else if (t == QMessageBox::No)
			result = QDialog::Rejected;
	}
	else
		result = QDialog::Rejected;
	setCurrentDocumentAndItem(currDoc, NULL);
	savePrefs();
// 	if (charSelect != NULL)
// 		charSelect->close();
	hide();
	blockUpdate = false;
}

void StoryEditor::keyPressEvent (QKeyEvent * e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else
	{
		activFromApp = false;
		return QMainWindow::keyReleaseEvent(e);
	}
}

bool StoryEditor::eventFilter( QObject* ob, QEvent* ev )
{
	if ( ev->type() == QEvent::WindowDeactivate )
	{
		if ((currItem!=NULL) && (!blockUpdate))
			updateTextFrame();
		activFromApp = false;
		Editor->getCursorPosition(&CurrPara, &CurrChar);
	}
	if ( ev->type() == QEvent::WindowActivate )
	{
		if ((!activFromApp) && (!textChanged) && (!blockUpdate))
		{
			activFromApp = true;
			if (currItem!=NULL)
			{
				disconnectSignals();
				Editor->setUndoRedoEnabled(false);
				Editor->setUndoRedoEnabled(true);
//				Editor->setCursorPosition(0, 0);
				seActions["fileRevert"]->setEnabled(false);
				seActions["editCopy"]->setEnabled(false);
				seActions["editCut"]->setEnabled(false);
				seActions["editClear"]->setEnabled(false);
				textChanged = false;
				Editor->loadItemText(currItem);
				Editor->getCursorPosition(&CurrPara, &CurrChar);
				updateStatus();
				textChanged = false;
				Editor->sync();
				Editor->repaintContents();
				EditorBar->doMove(0, Editor->contentsY());
				EditorBar->setRepaint(true);
				EditorBar->doRepaint();
				connectSignals();
			}
		}
	}
	return QMainWindow::eventFilter(ob, ev);
}

void StoryEditor::setBackPref()
{
	blockUpdate = true;
	QColor neu = QColor();
	neu = QColorDialog::getColor(Editor->paper().color(), this);
	if (neu.isValid())
	{
		Editor->setPaper(neu);
		prefsManager->appPrefs.STEcolor = neu;
	}
	blockUpdate = false;
}

void StoryEditor::setFontPref()
{
	blockUpdate = true;
	Editor->setFont( QFontDialog::getFont( 0, Editor->font(), this ) );
	prefsManager->appPrefs.STEfont = Editor->font().toString();
	EditorBar->doRepaint();
	blockUpdate = false;
}

void StoryEditor::newTxFill(int c, int s)
{
	if (c != -1)
		Editor->CurrTextFill = FillTools->TxFill->text(c);
	if (s != -1)
		Editor->CurrTextFillSh = s;
	CharStyle charStyle;
	charStyle.setFillColor(Editor->CurrTextFill);
	charStyle.setFillShade(Editor->CurrTextFillSh);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxStroke(int c, int s)
{
	if (c != -1)
		Editor->CurrTextStroke = StrokeTools->TxStroke->text(c);
	if (s != -1)
		Editor->CurrTextStrokeSh = s;
	CharStyle charStyle;
	charStyle.setStrokeColor(Editor->CurrTextStroke);
	charStyle.setStrokeShade(Editor->CurrTextStrokeSh);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxFont(const QString &f)
{
	if(!currDoc->UsedFonts.contains(f)) {
		if (!currDoc->AddFont(f)) {
//, prefsManager->appPrefs.AvailFonts[f]->Font)) {
			FontTools->Fonts->RebuildList(currDoc);
			return;
		};
	}
	Editor->prevFont = Editor->CurrFont;
	Editor->CurrFont = f;
	updateUnicodeActions();
	CharStyle charStyle;
	charStyle.setFont((*currDoc->AllFonts)[Editor->CurrFont]);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxSize(double s)
{
	Editor->CurrFontSize = qRound(s * 10.0);
	CharStyle charStyle;
	charStyle.setFontSize(Editor->CurrFontSize);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxStyle(int s)
{
	Editor->CurrentStyle = static_cast<StyleFlag>(s);
	CharStyle charStyle;
	charStyle.setEffects(Editor->CurrentStyle);
	Editor->updateSel(charStyle);
	Editor->setStyle(s);
	if (s & ScStyle_Outline)
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

void StoryEditor::newTxScale(int )
{
	int ss = qRound(FontTools->ChScale->value() * 10);
	Editor->CurrTextScale = ss;
	CharStyle charStyle;
	charStyle.setScaleH(Editor->CurrTextScale);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxScaleV(int )
{
	int ss = qRound(FontTools->ChScaleV->value() * 10);
	Editor->CurrTextScaleV = ss;
	CharStyle charStyle;
	charStyle.setScaleV(Editor->CurrTextScaleV);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxKern(int s)
{
	Editor->CurrTextKern = s;
	CharStyle charStyle;
	charStyle.setTracking(Editor->CurrTextKern);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newShadowOffs(int x, int y)
{
	CharStyle charStyle;
	charStyle.setShadowXOffset(x);
	charStyle.setShadowYOffset(y);
	Editor->CurrTextShadowX = x;
	Editor->CurrTextShadowY = y;
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxtOutline(int o)
{
	Editor->CurrTextOutline = o;
	CharStyle charStyle;
	charStyle.setOutlineWidth(Editor->CurrTextOutline);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxtUnderline(int p, int w)
{
	CharStyle charStyle;
	charStyle.setUnderlineOffset(p);
	charStyle.setUnderlineWidth(w);
	Editor->CurrTextUnderPos = p;
	Editor->CurrTextUnderWidth = w;
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxtStrike(int p, int w)
{
	CharStyle charStyle;
	charStyle.setStrikethruOffset(p);
	charStyle.setStrikethruWidth(w);
	Editor->CurrTextStrikePos = p;
	Editor->CurrTextStrikeWidth = w;
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::updateProps(int p, int ch)
{
	ColorList::Iterator it;
	int c = 0;
	if (Editor->wasMod)
		return;
	if ((p >= static_cast<int>(Editor->StyledText.nrOfParagraphs())) || (Editor->StyledText.length() == 0) || (!firstSet))
	{
		if (false && !firstSet)
		{
			const CharStyle& curstyle(currItem->itemText.defaultStyle().charStyle());
			Editor->CurrTextFill = curstyle.fillColor();
			Editor->CurrTextFillSh = curstyle.fillShade();
			Editor->CurrTextStroke = curstyle.strokeColor();
			Editor->CurrTextStrokeSh = curstyle.strokeShade();
			Editor->prevFont = Editor->CurrFont;
			Editor->CurrFont = curstyle.font().scName();
			Editor->CurrFontSize = curstyle.fontSize();
			Editor->CurrentStyle = curstyle.effects();
			Editor->currentParaStyle = curstyle.parent();
			Editor->CurrTextKern = curstyle.tracking();
			Editor->CurrTextScale = curstyle.scaleH();
			Editor->CurrTextScaleV = curstyle.scaleV();
			Editor->CurrTextBase = curstyle.baselineOffset();
			Editor->CurrTextShadowX = curstyle.shadowXOffset();
			Editor->CurrTextShadowY = curstyle.shadowYOffset();
			Editor->CurrTextOutline = curstyle.outlineWidth();
			Editor->CurrTextUnderPos = curstyle.underlineOffset();
			Editor->CurrTextUnderWidth = curstyle.underlineWidth();
			Editor->CurrTextStrikePos = curstyle.strikethruOffset();
			Editor->CurrTextStrikeWidth = curstyle.strikethruWidth();
			c = 0;
			StrokeTools->SetShade(Editor->CurrTextStrokeSh);
			FillTools->SetShade(Editor->CurrTextFillSh);
			QString b = Editor->CurrTextFill;
			if ((b != CommonStrings::None) && (!b.isEmpty()))
			{
				c++;
				for (it = currDoc->PageColors.begin(); it != currDoc->PageColors.end(); ++it)
				{
					if (it.key() == b)
						break;
					c++;
				}
			}
			FillTools->SetColor(c);
			c = 0;
			b = Editor->CurrTextStroke;
			if ((b != CommonStrings::None) && (!b.isEmpty()))
			{
				c++;
				for (it = currDoc->PageColors.begin(); it != currDoc->PageColors.end(); ++it)
				{
					if (it.key() == b)
						break;
					c++;
				}
			}
			StrokeTools->SetColor(c);
			AlignTools->SetAlign(Editor->CurrAlign);
			StyleTools->SetKern(Editor->CurrTextKern);
			StyleTools->SetStyle(Editor->CurrentStyle);
			StyleTools->SetShadow(Editor->CurrTextShadowX, Editor->CurrTextShadowY);
			StyleTools->setOutline(Editor->CurrTextOutline);
			StyleTools->setUnderline(Editor->CurrTextUnderPos, Editor->CurrTextUnderWidth);
			StyleTools->setStrike(Editor->CurrTextStrikePos, Editor->CurrTextStrikeWidth);
			FontTools->SetSize(Editor->CurrFontSize);
			FontTools->SetFont(Editor->CurrFont);
			FontTools->SetScale(Editor->CurrTextScale);
			FontTools->SetScaleV(Editor->CurrTextScaleV);
		}
		if (Editor->CurrentStyle & ScStyle_Outline)
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		Editor->setStyle(Editor->CurrentStyle);
		firstSet = true;
		updateUnicodeActions();
		return;
	}
	int parStart = Editor->StyledText.startOfParagraph(p);
	const ParagraphStyle& parStyle(Editor->StyledText.paragraphStyle(parStart));
	Editor->currentParaStyle = parStyle.displayName();
	if (Editor->StyledText.endOfParagraph(p) <= parStart)
	{
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = parStyle.charStyle().font().scName();
		Editor->CurrFontSize = parStyle.charStyle().fontSize();
		Editor->CurrentStyle = parStyle.charStyle().effects();
		Editor->CurrTextFill = parStyle.charStyle().fillColor();
		Editor->CurrTextFillSh = parStyle.charStyle().fillShade();
		Editor->CurrTextStroke = parStyle.charStyle().strokeColor();
		Editor->CurrTextStrokeSh = parStyle.charStyle().strokeShade();
		Editor->CurrTextShadowX = parStyle.charStyle().shadowXOffset();
		Editor->CurrTextShadowY = parStyle.charStyle().shadowYOffset();
		Editor->CurrTextOutline = parStyle.charStyle().outlineWidth();
		Editor->CurrTextUnderPos = parStyle.charStyle().underlineOffset();
		Editor->CurrTextUnderWidth = parStyle.charStyle().underlineWidth();
		Editor->CurrTextStrikePos = parStyle.charStyle().strikethruOffset();
		Editor->CurrTextStrikeWidth = parStyle.charStyle().strikethruWidth();
		Editor->setAlign(Editor->CurrAlign);
		Editor->setStyle(Editor->CurrentStyle);
	}
	else
	{
		int start;
		if (Editor->hasSelectedText())
		{
			int PStart=0, PEnd=0, SelStart=0, SelEnd=0;
			Editor->getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
			start = Editor->StyledText.startOfParagraph(PStart);
			if (SelStart >= 0 && start + SelStart < Editor->StyledText.endOfParagraph(PStart))
				start = QMIN(start + SelStart, Editor->StyledText.endOfParagraph(PStart)-1);
			else
				start = QMIN(start + QMAX(ch-1, 0), Editor->StyledText.endOfParagraph(p)-1);
		}
		else
			start = QMIN(Editor->StyledText.startOfParagraph(p) + QMAX(ch-1, 0), Editor->StyledText.endOfParagraph(p)-1);
		if (start >= Editor->StyledText.length())
			start = Editor->StyledText.length() - 1;
		if (start < 0)
			start = 0;
		const CharStyle& charStyle(Editor->StyledText.charStyle(start));
		Editor->CurrTextFill = charStyle.fillColor();
		Editor->CurrTextFillSh = charStyle.fillShade();
		Editor->CurrTextStroke = charStyle.strokeColor();
		Editor->CurrTextStrokeSh = charStyle.strokeShade();
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = charStyle.font().scName();
		Editor->CurrFontSize = charStyle.fontSize();
		Editor->CurrentStyle = charStyle.effects() & static_cast<StyleFlag>(1919);
		Editor->CurrTextKern = charStyle.tracking();
		Editor->CurrTextScale = charStyle.scaleH();
		Editor->CurrTextScaleV = charStyle.scaleV();
		Editor->CurrTextBase = charStyle.baselineOffset();
		Editor->CurrTextShadowX = charStyle.shadowXOffset();
		Editor->CurrTextShadowY = charStyle.shadowYOffset();
		Editor->CurrTextOutline = charStyle.outlineWidth();
		Editor->CurrTextUnderPos = charStyle.underlineOffset();
		Editor->CurrTextUnderWidth = charStyle.underlineWidth();
		Editor->CurrTextStrikePos = charStyle.strikethruOffset();
		Editor->CurrTextStrikeWidth = charStyle.strikethruWidth();
	}
	StrokeTools->SetShade(Editor->CurrTextStrokeSh);
	FillTools->SetShade(Editor->CurrTextFillSh);
	QString b = Editor->CurrTextFill;
	if ((b != CommonStrings::None) && (!b.isEmpty()))
	{
		c++;
		for (it = currDoc->PageColors.begin(); it != currDoc->PageColors.end(); ++it)
		{
			if (it.key() == b)
				break;
			c++;
		}
	}
	FillTools->SetColor(c);
	c = 0;
	b = Editor->CurrTextStroke;
	if ((b != CommonStrings::None) && (!b.isEmpty()))
	{
		c++;
		for (it = currDoc->PageColors.begin(); it != currDoc->PageColors.end(); ++it)
		{
			if (it.key() == b)
				break;
			c++;
		}
	}
	StrokeTools->SetColor(c);
	if (Editor->CurrentStyle & ScStyle_Outline)
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
	StyleTools->SetShadow(Editor->CurrTextShadowX, Editor->CurrTextShadowY);
	StyleTools->setOutline(Editor->CurrTextOutline);
	StyleTools->setUnderline(Editor->CurrTextUnderPos, Editor->CurrTextUnderWidth);
	StyleTools->setStrike(Editor->CurrTextStrikePos, Editor->CurrTextStrikeWidth);
	FontTools->SetSize(Editor->CurrFontSize);
	FontTools->SetFont(Editor->CurrFont);
	FontTools->SetScale(Editor->CurrTextScale);
	FontTools->SetScaleV(Editor->CurrTextScaleV);
	AlignTools->SetAlign(Editor->CurrAlign);
	updateUnicodeActions();
	updateStatus();
}

void StoryEditor::updateStatus()
{
	QString tmp;
	int p=0, i=0;
	Editor->getCursorPosition(&p, &i);
	int start = Editor->StyledText.startOfParagraph(p);
	int end = Editor->StyledText.endOfParagraph(p);

	ParC->setText(tmp.setNum(Editor->StyledText.nrOfParagraphs()));
	CharC->setText(tmp.setNum(end - start));
	CharC2->setText(tmp.setNum(Editor->StyledText.length()));

	QRegExp rx( "(\\w+)\\b" );
	const QString& txt(Editor->StyledText.text(0, Editor->StyledText.length()));
	int pos = 1;
	int counter = end > start? 1 : 0;
	int counter2 = Editor->StyledText.length() > 0? 1 : 0;
	while ( pos >= 0 )
	{
		pos = rx.search( txt, pos );
		if ( pos > -1 )
		{
			if (pos > start && pos < end)
				counter++;

			counter2++;
			pos += rx.matchedLength();
		}
	}

	WordC->setText(tmp.setNum(counter));
	WordC2->setText(tmp.setNum(counter2));
}

void StoryEditor::Do_insSp()
{
	ScCore->primaryMainWindow()->charPalette->show();
}

void StoryEditor::slot_insertSpecialChar()
{
	blockUpdate = true;
	if (!ScCore->primaryMainWindow()->charPalette->getCharacters().isEmpty())
	{
		Editor->insChars(ScCore->primaryMainWindow()->charPalette->getCharacters());
		Editor->insert(ScCore->primaryMainWindow()->charPalette->getCharacters());
	}
	blockUpdate = false;
}

void StoryEditor::slot_insertUserSpecialChar(QChar c)
{
	blockUpdate = true;
	Editor->insChars(c);
	Editor->insert(c);
	blockUpdate = false;
}

void StoryEditor::Do_fontPrev()
{
	blockUpdate = true;
	QString retval;
	ScActionPlugin* plugin;
	bool result = false;

	if (PluginManager::instance().DLLexists("fontpreview"))
	{
		plugin = dynamic_cast<ScActionPlugin*>(PluginManager::instance().getPlugin("fontpreview", false));
		if (plugin)
			result = plugin->run(this, currDoc, Editor->CurrFont);
		if (result)
		{
			retval = plugin->runResult();
			if (!retval.isEmpty())
			{
				newTxFont(retval);
				FontTools->SetFont(retval);
			}
		}
	}
	blockUpdate = false;
}

void StoryEditor::Do_leave2()
{
	updateTextFrame();
	result = QDialog::Accepted;
	setCurrentDocumentAndItem(currDoc, NULL);
	hide();
	blockUpdate = false;
}

void StoryEditor::Do_leave()
{
	if (textChanged)
	{
		blockUpdate = true;
		int t = ScMessageBox::warning(this, CommonStrings::trWarning,
		                             tr("Do you really want to lose all your changes?"),
		                             QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
		qApp->processEvents();
		if (t == QMessageBox::No)
		{
			blockUpdate = false;
			return;
		}
	}
	result = QDialog::Rejected;
	setCurrentDocumentAndItem(currDoc, NULL);
	hide();
	blockUpdate = false;
}

void StoryEditor::Do_saveDocument()
{
	blockUpdate = true;
	if (ScCore->primaryMainWindow()->slotFileSave())
		updateTextFrame();
	blockUpdate = false;
}

bool StoryEditor::Do_new()
{
	if (Editor->length() != 0)
	{
		blockUpdate = true;
		int t = ScMessageBox::warning(this, CommonStrings::trWarning,
	                             tr("Do you really want to clear all your text?"),
	                             QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
		qApp->processEvents();
		if (t == QMessageBox::No)
		{
			blockUpdate = false;
			return false;
		}
	}
	Editor->StyledText.clear();
	Editor->clear();
	Editor->setUndoRedoEnabled(false);
	Editor->setUndoRedoEnabled(true);
	Editor->setCursorPosition(0, 0);
	seActions["fileRevert"]->setEnabled(false);
	seActions["editCopy"]->setEnabled(false);
	seActions["editCut"]->setEnabled(false);
	seActions["editClear"]->setEnabled(false);
//	textChanged = false;
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
	updateProps(0, 0);
	updateStatus();
	blockUpdate = false;
	return true;
}

void StoryEditor::slotFileRevert()
{
	if (Do_new())
	{
		Editor->loadItemText(currItem);
		Editor->getCursorPosition(&CurrPara, &CurrChar);
		updateStatus();
		EditorBar->setRepaint(true);
		EditorBar->doRepaint();
		Editor->sync();
		Editor->repaintContents();
	}
}

void StoryEditor::Do_selectAll()
{
	if (Editor->StyledText.length() == 0)
		return;

	int lastPar = Editor->StyledText.nrOfParagraphs()-1;
	if (lastPar > 0)
	{
		int lastParStart = Editor->StyledText.startOfParagraph(lastPar);
		int lastParEnd = Editor->StyledText.endOfParagraph(lastPar);
		Editor->setSelection(0, 0, lastPar, lastParEnd - lastParStart);
	}
	else
	{
		Editor->setSelection(0, 0, 0, Editor->StyledText.length());
	}
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
	if (Editor->StyledText.length() == 0)
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
	seActions["editCopy"]->setEnabled(u);
	seActions["editCut"]->setEnabled(u);
	seActions["editClear"]->setEnabled(u);
//	seActions["editCopy"]->setEnabled(Editor->tBuffer.length() != 0);
}

void StoryEditor::PasteAvail()
{
	seActions["editPaste"]->setEnabled(true);
}

void StoryEditor::updateTextFrame()
{
	//Return immediately if we dont have to update the frame
	if (!textChanged)
		return;
	PageItem *nextItem = currItem;
	if (currItem->asTextFrame())
	{
		while (nextItem != 0)
		{
			if (nextItem->prevInChain() != 0)
				nextItem = nextItem->prevInChain();
			else
				break;
		}
	}
	PageItem* nb2 = nextItem;
	nb2->itemText.clear();
	if (currItem->asTextFrame())
	{
		while (nb2 != 0)
		{
#if 0
		for (int j = nb2->firstInFrame(); j <= nb2->lastInFrame(); ++j)
		{
			if ((nb2->itemText.text(j) == SpecialChars::OBJECT) && (nb2->itemText.item(j)->cembedded != 0))
			{
				QPtrList<PageItem> emG;
				emG.clear();
				emG.append(nb2->itemText.item(j)->cembedded);
				if (nb2->itemText.item(j)->cembedded->Groups.count() != 0)
				{
					for (uint ga=0; ga<Editor->FrameItems.count(); ++ga)
					{
						if (Editor->FrameItems.at(ga)->Groups.count() != 0)
						{
							if (Editor->FrameItems.at(ga)->Groups.top() == nb2->itemText.item(j)->cembedded->Groups.top())
							{
								if (Editor->FrameItems.at(ga)->ItemNr != nb2->itemText.item(j)->cembedded->ItemNr)
								{
									if (emG.find(Editor->FrameItems.at(ga)) == -1)
										emG.append(Editor->FrameItems.at(ga));
								}
							}
						}
					}
				}
				for (uint em = 0; em < emG.count(); ++em)
				{
					currDoc->FrameItems.remove(emG.at(em));
				}
			}
		}
		nb2->itemText.clear();
#endif
			nb2->CPos = 0;
			nb2->Dirty = false;
			nb2 = nb2->nextInChain();
		}
	}
	Editor->saveItemText(nextItem);
	QPtrList<PageItem> FrameItemsDel;
	FrameItemsDel.setAutoDelete(true);
	for (uint a = 0; a < Editor->FrameItems.count(); ++a)
	{
		if (currDoc->FrameItems.findRef(Editor->FrameItems.at(a)) == -1)
			FrameItemsDel.append(Editor->FrameItems.at(a));
	}
	for (uint a = 0; a < FrameItemsDel.count(); ++a)
	{
		Editor->FrameItems.remove(FrameItemsDel.at(a));
	}
	FrameItemsDel.clear();
	currDoc->updateFrameItems();
	if (currItem->asTextFrame())
	{
		dynamic_cast<PageItem_TextFrame*>(nextItem)->layout();
		nb2 = nextItem;
		while (nb2 != 0)
		{
			nb2->Dirty = false;
			nb2 = nb2->nextInChain();
		}
	}
	ScCore->primaryMainWindow()->view->DrawNew();
	textChanged = false;
	seActions["fileRevert"]->setEnabled(false);
	seActions["editUpdateFrame"]->setEnabled(false);
	emit DocChanged();
}

void StoryEditor::SearchText()
{
	blockUpdate = true;
	EditorBar->setRepaint(false);
	SearchReplace* dia = new SearchReplace(this, currDoc, currItem, false);
	dia->exec();
	delete dia;
	qApp->processEvents();
	blockUpdate = false;
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
}

void StoryEditor::slotEditStyles()
{
	blockUpdate = true;
	EditorBar->setRepaint(false);
	int p=0, i=0;
	Editor->getCursorPosition(&p, &i);
	disconnect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	disconnect(AlignTools, SIGNAL(newParaStyle(int)), this, SLOT(newStyle(int)));
	disconnect(AlignTools, SIGNAL(newAlign(int)), this, SLOT(newAlign(int)));
	//emit EditSt();

	StilFormate *dia = new StilFormate(this, currDoc);
	connect(dia, SIGNAL(saveStyle(StilFormate *)), ScCore->primaryMainWindow(), SLOT(saveStyles(StilFormate *)));
	if (dia->exec())
		ScCore->primaryMainWindow()->saveStyles(dia);
	disconnect(dia, SIGNAL(saveStyle(StilFormate *)), ScCore->primaryMainWindow(), SLOT(saveStyles(StilFormate *)));
	delete dia;

	AlignTools->paraStyleCombo->setDoc(currDoc);
	AlignTools->SetAlign(Editor->CurrAlign);
	AlignTools->SetParaStyle(currItem->doc()->paragraphStyles().find(Editor->currentParaStyle));
	connect(AlignTools, SIGNAL(newParaStyle(int)), this, SLOT(newStyle(int)));
	connect(AlignTools, SIGNAL(newAlign(int)), this, SLOT(newAlign(int)));
	Editor->setCursorPosition(p, i);
	updateProps(p, i);
	connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	Editor->sync();
	Editor-> repaintContents();
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
	qApp->processEvents();
	blockUpdate = false;
}

void StoryEditor::newAlign(int st)
{
	Editor->CurrAlign = st;
	changeAlign(st);
}


void StoryEditor::newStyle(int st)
{
	Editor->currentParaStyle = currDoc->paragraphStyles()[st].name();
	changeStyle(st);
}


void StoryEditor::changeStyleSB(int pa, int st)
{
	Editor->currentParaStyle = currDoc->paragraphStyles()[st].name();
	ParagraphStyle newStyle;
	newStyle.setParent(Editor->currentParaStyle);

	if (Editor->StyledText.length() != 0)
	{
		disconnect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
/*		qDebug(QString("changeStyleSB: pa=%2, start=%2, new=%3 %4")
			   .arg(pa)
			   .arg(Editor->StyledText.startOfParagraph(pa))
			   .arg(newStyle.parent())
			   .arg(newStyle.hasParent()));
*/
		Editor->StyledText.applyStyle(Editor->StyledText.startOfParagraph(pa), newStyle);

		Editor->updateFromChars(pa);
		Editor->setCursorPosition(pa, 0);
		updateProps(pa, 0);
		Editor->ensureCursorVisible();
		connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	}
	else
	{
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().font().scName();
		Editor->CurrFontSize = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().fontSize();
		Editor->CurrentStyle = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().effects();
		Editor->CurrTextFill = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().fillColor();
		Editor->CurrTextFillSh = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().fillShade();
		Editor->CurrTextStroke = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().strokeColor();
		Editor->CurrTextStrokeSh = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().strokeShade();
		Editor->CurrTextShadowX = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().shadowXOffset();
		Editor->CurrTextShadowY = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().shadowYOffset();
		Editor->CurrTextOutline = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().outlineWidth();
		Editor->CurrTextUnderPos = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().underlineOffset();
		Editor->CurrTextUnderWidth = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().underlineWidth();
		Editor->CurrTextStrikePos = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().strikethruOffset();
		Editor->CurrTextStrikeWidth = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().strikethruWidth();

		Editor->setStyle(Editor->CurrentStyle);
		if (Editor->CurrentStyle & ScStyle_Outline)
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		Editor->setCursorPosition(0, 0);
		updateProps(0, 0);
	}

	Editor->sync();
	Editor-> repaintContents();
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::changeStyle(int )
{
	int p=0, i=0;
	bool sel = false;
	ParagraphStyle newStyle;
	newStyle.setParent(Editor->currentParaStyle);

	Editor->getCursorPosition(&p, &i);
	if (Editor->StyledText.length() != 0)
	{
		disconnect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
		int PStart = 0;
		int PEnd = 0;
		int SelStart = 0;
		int SelEnd = 0;
		int PStart2 = 0;
		int PEnd2 = 0;
		int SelStart2 = 0;
		int SelEnd2 = 0;
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
		for (int pa = PStart; pa <= PEnd; ++pa)
		{
			Editor->StyledText.applyStyle(Editor->StyledText.startOfParagraph(pa), newStyle);
			Editor->updateFromChars(pa);
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
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().font().scName();
		Editor->CurrFontSize = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().fontSize();
		Editor->CurrentStyle = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().effects();
		Editor->CurrTextFill = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().fillColor();
		Editor->CurrTextFillSh = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().fillShade();
		Editor->CurrTextStroke = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().strokeColor();
		Editor->CurrTextStrokeSh = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().strokeShade();
		Editor->CurrTextShadowX = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().shadowXOffset();
		Editor->CurrTextShadowY = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().shadowYOffset();
		Editor->CurrTextOutline = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().outlineWidth();
		Editor->CurrTextUnderPos = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().underlineOffset();
		Editor->CurrTextUnderWidth = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().underlineWidth();
		Editor->CurrTextStrikePos = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().strikethruOffset();
		Editor->CurrTextStrikeWidth = currDoc->paragraphStyles()[Editor->currentParaStyle].charStyle().strikethruWidth();

		Editor->setStyle(Editor->CurrentStyle);
		if (Editor->CurrentStyle & ScStyle_Outline)
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		Editor->setCursorPosition(0, 0);
		updateProps(0, 0);
	}
	modifiedText();
	Editor->sync();
	Editor-> repaintContents();
	Editor->setFocus();
}


void StoryEditor::changeAlign(int )
{
	int p=0, i=0;
	bool sel = false;
	ParagraphStyle newStyle;
	newStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(Editor->CurrAlign));

	Editor->getCursorPosition(&p, &i);
	if (Editor->StyledText.length() != 0)
	{
		disconnect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
		int PStart = 0;
		int PEnd = 0;
		int SelStart = 0;
		int SelEnd = 0;
		int PStart2 = 0;
		int PEnd2 = 0;
		int SelStart2 = 0;
		int SelEnd2 = 0;
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
		for (int pa = PStart; pa <= PEnd; ++pa)
		{
			Editor->StyledText.applyStyle(Editor->StyledText.startOfParagraph(pa), newStyle);
			Editor->updateFromChars(pa);
		}
		if (sel)
			Editor->setSelection(PStart2, SelStart2, PEnd2, SelEnd2);
		Editor->setCursorPosition(p, i);
		Editor->ensureCursorVisible();
		updateProps(p, i);
		connect(Editor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateProps(int, int)));
	}
	modifiedText();
	Editor->sync();
	Editor->repaintContents();
	Editor->setFocus();
}


void StoryEditor::modifiedText()
{
	textChanged = true;
	firstSet = true;
	seActions["fileRevert"]->setEnabled(true);
	seActions["editUpdateFrame"]->setEnabled(true);
//	seActions["editPaste"]->setEnabled(Editor->tBuffer.length() != 0);
	updateStatus();
}

void StoryEditor::LoadTextFile()
{
	if (Do_new())
	{
		EditorBar->setRepaint(false);
		QString LoadEnc = "";
		QString fileName = "";
		PrefsContext* dirs = prefsManager->prefsFile->getContext("dirs");
		QString wdir = dirs->get("story_load", prefsManager->documentDir());
		CustomFDialog dia(this, wdir, tr("Open"), tr("Text Files (*.txt);;All Files(*)"), fdExistingFiles | fdShowCodecs);
		if (dia.exec() != QDialog::Accepted)
			return;
		LoadEnc = dia.TxCodeM->currentText();
		if (LoadEnc == "UTF-16")
			LoadEnc = "ISO-10646-UCS-2";
		fileName =  dia.selectedFile();
		if (!fileName.isEmpty())
		{
			dirs->set("story_load", fileName.left(fileName.findRev("/")));
			// time to retire...
			/*
			Serializer *ss = new Serializer(fileName);
			if (ss->Read(LoadEnc))
			{
				QString data = ss->GetObjekt();
				data.replace(QRegExp("\r"), "");
				data.replace(QRegExp("\n"), QChar(13));
				Editor->loadText(data, currItem);
				seActions["editPaste"]->setEnabled(false);
				seActions["editCopy"]->setEnabled(false);
				seActions["editCut"]->setEnabled(false);
				seActions["editClear"]->setEnabled(false);
			}
			delete ss;
			 */
			QString txt;
			if (Serializer::readWithEncoding(fileName, LoadEnc, txt))
			{
				txt.replace(QRegExp("\r"), "");
				txt.replace(QRegExp("\n"), QChar(13));
				Editor->loadText(txt, currItem);
				seActions["editPaste"]->setEnabled(false);
				seActions["editCopy"]->setEnabled(false);
				seActions["editCut"]->setEnabled(false);
				seActions["editClear"]->setEnabled(false);
			}
		}
		EditorBar->setRepaint(true);
		EditorBar->doRepaint();
	}
	Editor->sync();
	Editor-> repaintContents();
}

void StoryEditor::SaveTextFile()
{
	blockUpdate = true;
	QString LoadEnc = "";
	QString fileName = "";
	PrefsContext* dirs = prefsManager->prefsFile->getContext("dirs");
	QString wdir = dirs->get("story_save", prefsManager->appPrefs.DocDir);
	CustomFDialog dia(this, wdir, tr("Save as"), tr("Text Files (*.txt);;All Files(*)"), fdShowCodecs);
	qApp->processEvents();
	if (dia.exec() != QDialog::Accepted)
	{
		blockUpdate = false;
		return;
	}
	LoadEnc = dia.TxCodeM->currentText();
	if (LoadEnc == "UTF-16")
		LoadEnc = "ISO-10646-UCS-2";
	fileName =  dia.selectedFile();
	if (!fileName.isEmpty())
	{
		dirs->set("story_save", fileName.left(fileName.findRev("/")));
		Serializer::writeWithEncoding(fileName, LoadEnc, Editor->text());
		/*
		Serializer *ss = new Serializer(fileName);
		ss->Objekt = Editor->text();
		ss->Write(LoadEnc);
		delete ss;
		 */
	}
	blockUpdate = false;
}

bool StoryEditor::textDataChanged() const
{
	return textChanged;
}

PageItem* StoryEditor::currentItem() const
{
	return currItem;
}

ScribusDoc* StoryEditor::currentDocument() const
{
	return currDoc;
}

void StoryEditor::specialActionKeyEvent(const QString& /*actionName*/, int unicodevalue)
{
	Editor->insChars(QString(QChar(unicodevalue)));
	QString guiInsertString=QChar(unicodevalue);
	bool setColor=false;
	if (unicodevalue == seActions["unicodePageNumber"]->actionInt())
	{
		setColor=true;
		guiInsertString="#";
	}
	if (unicodevalue == seActions["unicodeNonBreakingSpace"]->actionInt())
	{
		setColor=true;
		guiInsertString="_";
	}
	if (unicodevalue == seActions["unicodeFrameBreak"]->actionInt())
	{
		setColor=true;
		guiInsertString="|";
	}
	if (unicodevalue == seActions["unicodeNewLine"]->actionInt())
	{
		setColor=true;
		guiInsertString="*";
	}
	if (unicodevalue == seActions["unicodeColumnBreak"]->actionInt())
	{
		setColor=true;
		guiInsertString="^";
	}
	if (unicodevalue == seActions["unicodeNonBreakingHyphen"]->actionInt())
	{
		setColor=true;
		guiInsertString="=";
	}
	if (setColor)
		Editor->setFarbe(true);
	Editor->insert(guiInsertString);
	if (setColor)
		Editor->setFarbe(false);
	modifiedText();
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
}

void StoryEditor::updateUnicodeActions()
{
	if (Editor->prevFont!=Editor->CurrFont)
		ScCore->primaryMainWindow()->actionManager->enableUnicodeActions(&seActions, true, Editor->CurrFont);
}
