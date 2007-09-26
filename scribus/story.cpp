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

#include <QApplication>
#include <QCloseEvent>
#include <QColorDialog>
#include <QComboBox>
#include <QCursor>
#include <QDesktopWidget>
#include <QEvent>
#include <QFocusEvent>
#include <QFontDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHideEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QList>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPalette>
#include <QPixmap>
#include <QRegExp>
#include <QShowEvent>
#include <QTextBlock>
#include <QTextCodec>
#include <QTextLayout>
#include <QToolTip>

#include "actionmanager.h"
#include "alignselect.h"
#include "charselect.h"
#include "colorcombo.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "fontcombo.h"
#include "menumanager.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "pluginmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scfonts.h"
#include "scmessagebox.h"
#include "scplugin.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scrspinbox.h"
#include "search.h"
#include "serializer.h"
#include "shadebutton.h"
#include "spalette.h"
#include "story.h"
#include "styleitem.h"
#include "stylemanager.h"
#include "styleselect.h"
#include "text/nlsconfig.h"
#include "util.h"
#include "util_icon.h"
/*
SideBar::SideBar(QWidget *pa) : QLabel(pa)
{
	QPalette pal;
	pal.setColor(QPalette::Window, QColor(255,255,255));
	setAutoFillBackground(true);
	setPalette(pal);
	offs = 0;
	editor = 0;
	noUpdt = true;
	inRep = false;
	pmen = new QMenu(this);
	setMinimumWidth(fontMetrics().width( tr("No Style") )+30);
}

void SideBar::mouseReleaseEvent(QMouseEvent *m)
{
	int p = editor->cursorForPosition(QPoint(2, m->y()+offs)).position();
	CurrentPar = editor->StyledText.nrOfParagraph(p);
	int pos = editor->StyledText.startOfParagraph(p);

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
	connect(paraStyleCombo, SIGNAL(newStyle(const QString&)), this, SLOT(setPStyle(const QString&)));
	pmen->clear();
	paraStyleAct = new QWidgetAction(this);
	paraStyleAct->setDefaultWidget(paraStyleCombo);
	pmen->addAction(paraStyleAct);
	pmen->addAction( tr("Edit Styles..."), this, SLOT(editStyles()));
	pmen->exec(QCursor::pos());
}

void SideBar::editStyles()
{
	emit sigEditStyles();
}

void SideBar::setPStyle(const QString& name)
{
	emit ChangeStyle(CurrentPar, name);
	pmen->close();
}

void SideBar::paintEvent(QPaintEvent *e)
{
	inRep = true;
	QLabel::paintEvent(e);
	QList<QRect> paraList;
	QTextBlock tb = editor->document()->begin();
	while (tb.isValid())
	{
		paraList.append(tb.layout()->boundingRect().toRect());
		tb = tb.next();
	}
	QPainter p;
	p.begin(this);
	if ((editor != 0) && (noUpdt))
	{
		int gesY = 0;
		for (uint pa = 0; pa < editor->StyledText.nrOfParagraphs(); ++pa)
		{
			QRect re = paraList[pa];
			if (!re.isValid())
				break;
			re.setWidth(width()-5);
			re.translate(5, 0);
			if (((re.y()+re.height())-offs + gesY < height()) && ((re.y()+re.height())-offs + gesY >= 0))
				p.drawLine(0, (re.y()+re.height())-offs + gesY, width()-1, (re.y()+re.height())-offs + gesY);
			if ((re.y()-offs + gesY < height()) && (re.y()-offs + gesY >= 0))
			{
				re.moveTop(gesY-offs);
				QString parname = editor->StyledText.paragraphStyle(editor->StyledText.startOfParagraph(pa)).parent();
				if (parname.isEmpty())
					parname = tr("No Style");
				p.drawText(re, Qt::AlignLeft | Qt::AlignTop, parname);
			}
			gesY += re.height();
		}
	}
	p.end();
	inRep = false;
}

void SideBar::doMove(int, int y)
{
	offs -= y;
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
*/
SEditor::SEditor(QWidget* parent, ScribusDoc *docc, StoryEditor* parentSE) : QTextEdit(parent)
{
	setCurrentDocument(docc);
	parentStoryEditor=parentSE;
	wasMod = false;
	StoredSel = false;
	SelCharStart = 0;
	SelParaStart = 0;
	StyledText.clear();
	cBuffer.clear();
	document()->setUndoRedoEnabled(true);
	viewport()->setAcceptDrops(false);
	ClipData = 0;
	unicodeTextEditMode = false;
	setAutoFillBackground(true);
	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
}

void SEditor::setCurrentDocument(ScribusDoc *docc)
{
	doc = docc;
	StyledText = StoryText(docc);
}

void SEditor::inputMethodEvent(QInputMethodEvent *event)
{
	QString uc = event->commitString();
	if ((!uc.isEmpty()) && ((*doc->AllFonts)[CurrFont].canRender(uc[0])))
	{
		insChars(event->commitString());
		QTextEdit::inputMethodEvent(event);
//		emit SideBarUp(true);
//		emit SideBarUpdate();
	}
}

void SEditor::keyPressEvent(QKeyEvent *k)
{
//	emit SideBarUp(false);
	int pos = textCursor().position();
	int keyMod=0;
	if (k->modifiers() & Qt::ShiftModifier)
		keyMod |= Qt::SHIFT;
	if (k->modifiers() & Qt::ControlModifier)
		keyMod |= Qt::CTRL;
	if (k->modifiers() & Qt::AltModifier)
		keyMod |= Qt::ALT;

	QString uc = k->text();
	if ((k->modifiers() == Qt::ControlModifier) ||
		(k->modifiers() && (Qt::ControlModifier | Qt::ShiftModifier)) ||
		(k->modifiers() && (Qt::ControlModifier | Qt::KeypadModifier)) ||
		(k->modifiers() && (Qt::ControlModifier | Qt::ShiftModifier | Qt::KeypadModifier))
	   )
	{
		switch (k->key())
		{
			case Qt::Key_Delete:
				moveCursor(QTextCursor::NextWord, QTextCursor::KeepAnchor);
				deleteSel();
				break;
			case Qt::Key_Backspace:
				moveCursor(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
				deleteSel();
				break;
			case Qt::Key_K:
				moveCursor(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
				deleteSel();
				break;
			case Qt::Key_D:
				moveCursor(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
				deleteSel();
				textCursor().removeSelectedText();
				break;
			case Qt::Key_H:
				moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
				deleteSel();
				textCursor().removeSelectedText();
				break;
			case Qt::Key_X:
				cut();
				return;
				break;
			case Qt::Key_V:
				paste();
				return;
				break;
			case Qt::Key_Y:
			case Qt::Key_Z:
//				emit SideBarUp(true);
				return;
				break;
			case Qt::Key_C:
				copyStyledText();
				break;
		}
	}
	if ((k->modifiers() == Qt::NoModifier) ||
		(k->modifiers() == Qt::KeypadModifier) ||
		(k->modifiers() == Qt::ShiftModifier) ||
		(k->modifiers() && (Qt::ControlModifier | Qt::AltModifier)) ||
		(k->modifiers() && (Qt::ControlModifier | Qt::AltModifier | Qt::ShiftModifier)) // Shift + AltGr on Windows for polish characters
	   )
	{
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
					insertPlainText(QString(QChar(conv)));
//					emit SideBarUp(true);
//					emit SideBarUpdate();
					return;
				}
			}
			else
			{
//				emit SideBarUp(true);
//				emit SideBarUpdate();
				return;
			}
		}
		wasMod = false;
		switch (k->key())
		{
			case Qt::Key_Escape:
				k->ignore();
				break;
			case Qt::Key_Shift:
			case Qt::Key_Control:
			case Qt::Key_Alt:
				wasMod = true;
				break;
			case Qt::Key_F12:
				unicodeTextEditMode = true;
				unicodeInputCount = 0;
				unicodeInputString = "";
				return;
				break;
			case Qt::Key_Delete:
				if (!textCursor().hasSelection())
				{
					if (pos < StyledText.length())
						StyledText.removeChars(pos, 1);
				}
				else
					deleteSel();
				break;
			case Qt::Key_Backspace:
				if (!textCursor().hasSelection())
				{
					if (pos > 0)
						StyledText.removeChars(pos-1, 1);
				}
				else
					deleteSel();
				break;
			case Qt::Key_Return:
			case Qt::Key_Enter:
				{
					if (textCursor().hasSelection())
					{
						pos = StyledText.startOfSelection();
						deleteSel();
					}
					StyledText.insertChars(pos, SpecialChars::PARSEP, true);
				}
				break;
			case Qt::Key_Left:
			case Qt::Key_Right:
			case Qt::Key_PageUp:
			case Qt::Key_PageDown:
			case Qt::Key_Up:
			case Qt::Key_Down:
			case Qt::Key_Home:
			case Qt::Key_End:
				break;
			default:
				if ((!k->text().isEmpty()) && ((*doc->AllFonts)[CurrFont].canRender(uc[0])))
				{
					insChars(k->text());
					QTextEdit::keyPressEvent(k);
//					emit SideBarUp(true);
//					emit SideBarUpdate();
				}
				return;
				break;
		}
	}
	QTextEdit::keyPressEvent(k);
//	emit SideBarUp(true);
//	emit SideBarUpdate();
}

void SEditor::focusOutEvent(QFocusEvent *e)
{
	if (textCursor().hasSelection())
	{
		SelParaStart = textCursor().selectionStart();
		SelParaEnd = textCursor().selectionEnd();
		StoredSel = true;
	}
	else
	{
		SelParaStart = textCursor().position();
		StoredSel = false;
	}
	QTextEdit::focusOutEvent(e);
}

void SEditor::focusInEvent(QFocusEvent *e)
{
	if (StoredSel)
	{
		textCursor().setPosition(SelParaStart);
		textCursor().setPosition(SelParaEnd, QTextCursor::KeepAnchor);
		StoredSel = false;
	}
	else
	{
		textCursor().setPosition(SelParaStart);
		StoredSel = false;
	}
		
	QTextEdit::focusInEvent(e);
}

void SEditor::insChars(QString t)
{
	if (textCursor().hasSelection())
		deleteSel();
	int pos = qMin(textCursor().position(), StyledText.length());
	StyledText.insertChars(pos, t, true);
}

void SEditor::insStyledText()
{
	if (cBuffer.length() == 0)
		return;
	if (textCursor().hasSelection())
		deleteSel();
	int pos = qMin(textCursor().position(), StyledText.length());
	StyledText.insert(pos, cBuffer);
}

void SEditor::copyStyledText()
{
	int start = textCursor().selectionStart();
	int end = textCursor().selectionEnd();
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
	currItem->itemText.setDefaultStyle(StyledText.defaultStyle());
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
	SelParaStart = 0;
	while (currItem->CPos >= (SelCharStart = currItem->itemText.endOfParagraph(SelParaStart))
		   && SelParaStart < npars)
		++SelParaStart;
	if (currItem->CPos < SelCharStart)
		SelCharStart = currItem->CPos;
	SelCharStart -= currItem->itemText.startOfParagraph(SelParaStart);
	StoredSel = false;
	//qDebug("SE::loadItemText: cursor");
//	setCursorPosition(SelParaStart, SelCharStart);
	emit setProps(SelParaStart, SelCharStart);
	SelParaStart = 0;
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
	//qDebug("SE::loadText: cursor");
	textCursor().setPosition(0);
}

void SEditor::updateAll()
{
	clear();
	if (StyledText.length() == 0)
		return;
	setUpdatesEnabled(false);
	int pos = textCursor().position();
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
				insertPlainText(Text);
				Text = "";
				Csty = cstyle.effects();
			}

			if (ch == SpecialChars::OBJECT)
			{
				setFarbe(true);
				insertPlainText("@");
				setFarbe(false);
			}
			else if (ch == SpecialChars::PAGENUMBER)
			{
				setFarbe(true);
				insertPlainText("#");
				setFarbe(false);
			}
			else if (ch == SpecialChars::NBSPACE)
			{
				setFarbe(true);
				insertPlainText("_");
				setFarbe(false);
			}
			else if (ch == SpecialChars::FRAMEBREAK)
			{
				setFarbe(true);
				insertPlainText("|");
				setFarbe(false);
			}
			else if (ch == SpecialChars::COLBREAK)
			{
				setFarbe(true);
				insertPlainText("^");
				setFarbe(false);
			}
			else if (ch == SpecialChars::NBHYPHEN)
			{
				setFarbe(true);
				insertPlainText("=");
				setFarbe(false);
			}
			else if (ch == SpecialChars::LINEBREAK)
			{
				setFarbe(true);
				insertPlainText("*");
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
	insertPlainText(Text);
	textCursor().setPosition(pos);
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
	int SelStart = start;
	int SelEnd = 0;
	int pos = textCursor().position();
	textCursor().clearSelection();
	int Csty = StyledText.charStyle(start).effects();
	for (int a = start; a < end; ++a)
	{
		if (Csty == StyledText.charStyle(a).effects())
			SelEnd++;
		else
		{
			textCursor().setPosition(SelStart);
			textCursor().setPosition(SelEnd, QTextCursor::KeepAnchor);
			setStyle(Csty);
			textCursor().clearSelection();
			Csty = StyledText.charStyle(a).effects();
			SelStart = SelEnd;
			SelEnd++;
		}
	}
	textCursor().setPosition(SelStart);
	textCursor().setPosition(SelEnd, QTextCursor::KeepAnchor);
	setStyle(Csty);
	textCursor().clearSelection();
	setAlign(StyledText.paragraphStyle(start).alignment());
	setUpdatesEnabled(true);
	textCursor().setPosition(pos);
}

/* updates the internal StyledText structure, applies 'newStyle' to the selection */
void SEditor::updateSel(const ParagraphStyle& newStyle)
{
	int PStart, PEnd, SelStart, SelEnd, start;
	if (StoredSel)
	{
		textCursor().setPosition(SelParaStart);
		textCursor().setPosition(SelParaEnd, QTextCursor::KeepAnchor);
		StoredSel = false;
	}
	SelStart = textCursor().selectionStart();
	PStart = StyledText.nrOfParagraph(SelStart);
	SelEnd = textCursor().selectionEnd();
	PEnd = StyledText.nrOfParagraph(SelEnd);
	for (int pa = PStart; pa <= PEnd; ++pa)
	{
		start = StyledText.startOfParagraph(pa);
		StyledText.applyStyle(start, newStyle);
	}
}

void SEditor::updateSel(const CharStyle& newStyle)
{
	if (StoredSel)
	{
		textCursor().setPosition(SelParaStart);
		textCursor().setPosition(SelParaEnd, QTextCursor::KeepAnchor);
		StoredSel = false;
	}
	int start = textCursor().selectionStart();
	int end = textCursor().selectionEnd();
	if (start >= 0 && start < end)
		StyledText.applyCharStyle(start, end-start, newStyle);
}


void SEditor::deleteSel()
{
	int start = textCursor().selectionStart();
	int end = textCursor().selectionEnd();
	if (end > start)
		StyledText.removeChars(start, end-start);
	textCursor().setPosition(start);
	StoredSel = false;
}

void SEditor::setStyle(int Csty)
{
	QTextCharFormat charF;
	if (Csty & 8)
		charF.setFontUnderline(true);
	else
		charF.setFontUnderline(false);
	if (Csty & 16)
		charF.setFontStrikeOut(true);
	else
		charF.setFontStrikeOut(false);
	if (Csty & 1)
		charF.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
	else if (Csty & 2)
		charF.setVerticalAlignment(QTextCharFormat::AlignSubScript);
	else
		charF.setVerticalAlignment(QTextCharFormat::AlignNormal);
	textCursor().setCharFormat(charF);
}

void SEditor::setFarbe(bool marker)
{
	QColor tmp;
	if (marker)
		tmp = QColor(Qt::red);
	else
		tmp = QColor(Qt::black);
	setTextColor(tmp);
}

void SEditor::copy()
{
//	emit SideBarUp(false);
	if ((textCursor().hasSelection()) && (!textCursor().selectedText().isEmpty()))
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
//	emit SideBarUp(true);
}

void SEditor::cut()
{
	copy();
//	emit SideBarUp(false);
	if (textCursor().hasSelection())
	{
		deleteSel();
		textCursor().removeSelectedText();
	}
//	emit SideBarUp(true);
//	emit SideBarUpdate();
}

void SEditor::paste()
{
//	emit SideBarUp(false);
//	int currentPara, currentCharPos;
	QString data = "";
	int newParaCount, lengthLastPara;
	int advanceLen = 0;
//	bool inserted=false;
//	getCursorPosition(&currentPara, &currentCharPos);
	int pos = textCursor().position();
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
			newParaCount=data.count("\n");
			lengthLastPara=data.length()-data.lastIndexOf("\n");
			data.replace(QRegExp("\n"), SpecialChars::PARSEP);
//			inserted=true;
			advanceLen = data.length() - newParaCount;
			insChars(data);
			ClipData = 2;
			emit PasteAvail();
		}
		else
		{
//			emit SideBarUp(true);
			return;
		}
	}
	updateAll();
	setUpdatesEnabled(false);
	//qDebug("SE::paste: cursor");
//	setCursorPosition(currentPara, currentCharPos);
	textCursor().setPosition(pos);
	for (int a = 0; a < advanceLen; ++a)
	{
		moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
	}
	setUpdatesEnabled(true);
//	if (inserted)
//		setCursorPosition(currentPara+newParaCount,(newParaCount==0?currentCharPos:0)+lengthLastPara-1);
	repaint();
//	emit SideBarUp(true);
//	emit SideBarUpdate();
}
/*
Q_3PopupMenu* SEditor::createPopupMenu(const QPoint & pos)
{
	Q_3PopupMenu *p = Q_3TextEdit::createPopupMenu(pos);
	p->removeItemAt(0);
	p->removeItemAt(0);
	p->removeItemAt(0);
	p->removeItemAt(3);
	return p;
}
*/
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

void SEditor::scrollContentsBy(int dx, int dy)
{
	emit contentsMoving(dx, dy);
	QTextEdit::scrollContentsBy (dx, dy);
}

/* Toolbar for Fill Colour */
SToolBColorF::SToolBColorF(QMainWindow* parent, ScribusDoc *doc) : QToolBar( tr("Fill Color Settings"), parent)
{
	FillIcon = new QLabel(this);
	FillIcon->setPixmap(loadIcon("16/color-fill.png"));
	FillIcon->setScaledContents( false );
	fillIconAction=addWidget(FillIcon);
	fillIconAction->setVisible(true);
	TxFill = new ColorCombo(false, this);
	txFillAction=addWidget(TxFill);
	txFillAction->setVisible(true);
	PM2 = new ShadeButton(this);
	pm2Action=addWidget(PM2);
	pm2Action->setVisible(true);
	
	setCurrentDocument(doc);
	//TxFill->listBox()->setMinimumWidth(TxFill->listBox()->maxItemWidth()+24);
	connect(TxFill, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	connect(PM2, SIGNAL(clicked()), this, SLOT(newShadeHandler()));

	languageChange();
}

void SToolBColorF::languageChange()
{
	TxFill->setToolTip("");
	PM2->setToolTip("");
	TxFill->setToolTip( tr( "Color of text fill" ));
	PM2->setToolTip( tr( "Saturation of color of text fill" ));
}

void SToolBColorF::setCurrentDocument(ScribusDoc *doc)
{
	if (doc!=NULL)
		TxFill->updateBox(doc->PageColors, ColorCombo::smallPixmaps, true);
	else
	{
		TxFill->clear();
		TxFill->addItem(CommonStrings::tr_NoneColor);
	}
//	if (doc!=NULL)
//		TxFill->insertItems(doc->PageColors, ColorCombo::smallPixmaps);
	resize(minimumSizeHint());
}

void SToolBColorF::SetColor(int c)
{
	disconnect(TxFill, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	TxFill->setCurrentIndex(c);
	connect(TxFill, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
}

void SToolBColorF::SetShade(double s)
{
	disconnect(PM2, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
	PM2->setValue(qRound(s));
	connect(PM2, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
}

void SToolBColorF::newShadeHandler()
{
	emit NewColor(TxFill->currentIndex(), PM2->getValue());
}

/* Toolbar for Stroke Colour */
SToolBColorS::SToolBColorS(QMainWindow* parent, ScribusDoc *doc) : QToolBar( tr("Stroke Color Settings"), parent)
{
	StrokeIcon = new QLabel( "", this );
	StrokeIcon->setPixmap(loadIcon("16/color-stroke.png"));
	StrokeIcon->setScaledContents( false );
	strokeIconAction=addWidget(StrokeIcon);
	strokeIconAction->setVisible(true);
	TxStroke = new ColorCombo(false, this);
	txStrokeAction=addWidget(TxStroke);
	txStrokeAction->setVisible(true);
	PM1 = new ShadeButton(this);
	pm1Action=addWidget(PM1);
	pm1Action->setVisible(true);
	setCurrentDocument(doc);
	//TxStroke->listBox()->setMinimumWidth(TxStroke->listBox()->maxItemWidth()+24);
	connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	connect(PM1, SIGNAL(clicked()), this, SLOT(newShadeHandler()));

	languageChange();
}

void SToolBColorS::languageChange()
{
	TxStroke->setToolTip("");
	PM1->setToolTip("");
	TxStroke->setToolTip( tr("Color of text stroke"));
	PM1->setToolTip( tr("Saturation of color of text stroke"));
}

void SToolBColorS::setCurrentDocument(ScribusDoc *doc)
{
	TxStroke->clear();
	TxStroke->addItem(CommonStrings::tr_NoneColor);
	if (doc!=NULL)
		TxStroke->insertItems(doc->PageColors, ColorCombo::smallPixmaps);
	resize(minimumSizeHint());
}

void SToolBColorS::SetColor(int c)
{
	disconnect(TxStroke, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
	TxStroke->setCurrentIndex(c);
	connect(TxStroke, SIGNAL(activated(int)), this, SLOT(newShadeHandler()));
}

void SToolBColorS::SetShade(double s)
{
	disconnect(PM1, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
	PM1->setValue(qRound(s));
	connect(PM1, SIGNAL(clicked()), this, SLOT(newShadeHandler()));
}

void SToolBColorS::newShadeHandler()
{
	emit NewColor(TxStroke->currentIndex(), PM1->getValue());
}

/* Toolbar for Character Style Settings */
SToolBStyle::SToolBStyle(QMainWindow* parent) : QToolBar( tr("Character Settings"), parent)
{
	SeStyle = new StyleSelect(this);
	seStyleAction=addWidget(SeStyle);
	seStyleAction->setVisible(true);
	trackingLabel = new QLabel( this );
	trackingLabel->setText("");
	trackingLabel->setPixmap(loadIcon("textkern.png"));
	trackingLabelAction=addWidget(trackingLabel);
	trackingLabelAction->setVisible(true);
	Extra = new ScrSpinBox( this, 1 );
	Extra->setValues( -300, 300, 2, 0);
	Extra->setSuffix( tr( " %" ) );
	extraAction=addWidget(Extra);
	extraAction->setVisible(true);

	connect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
	connect(Extra, SIGNAL(valueChanged(double)), this, SLOT(newKernHandler()));
	connect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowHandler()));
	connect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowHandler()));
	connect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newOutlineHandler()));
	connect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newUnderlineHandler()));
	connect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newUnderlineHandler()));
	connect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newStrikeHandler()));
	connect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newStrikeHandler()));

	languageChange();
}

void SToolBStyle::languageChange()
{
	Extra->setToolTip("");
	Extra->setToolTip( tr( "Manual Tracking" ));
}

void SToolBStyle::newStrikeHandler()
{
	double x = SeStyle->StrikeVal->LPos->value() * 10.0;
	double y = SeStyle->StrikeVal->LWidth->value() * 10.0;
// 	emit newUnderline(x, y);
	emit newStrike(x, y);
}

void SToolBStyle::newUnderlineHandler()
{
	double x = SeStyle->UnderlineVal->LPos->value() * 10.0;
	double y = SeStyle->UnderlineVal->LWidth->value() * 10.0;
	emit newUnderline(x, y);
}

void SToolBStyle::newOutlineHandler()
{
	double x = SeStyle->OutlineVal->LWidth->value() * 10.0;
	emit newOutline(x);
}

void SToolBStyle::newShadowHandler()
{
	double x = SeStyle->ShadowVal->Xoffset->value() * 10.0;
	double y = SeStyle->ShadowVal->Yoffset->value() * 10.0;
	emit NewShadow(x, y);
}

void SToolBStyle::newKernHandler()
{
	emit NewKern(Extra->value() * 10.0);
}

void SToolBStyle::setOutline(double x)
{
	disconnect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newOutlineHandler()));
	SeStyle->OutlineVal->LWidth->setValue(x / 10.0);
	connect(SeStyle->OutlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newOutlineHandler()));
}

void SToolBStyle::setStrike(double p, double w)
{
	disconnect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newStrikeHandler()));
	disconnect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newStrikeHandler()));
	SeStyle->StrikeVal->LWidth->setValue(w / 10.0);
	SeStyle->StrikeVal->LPos->setValue(p / 10.0);
	connect(SeStyle->StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newStrikeHandler()));
	connect(SeStyle->StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newStrikeHandler()));
}

void SToolBStyle::setUnderline(double p, double w)
{
	disconnect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newUnderlineHandler()));
	disconnect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newUnderlineHandler()));
	SeStyle->UnderlineVal->LWidth->setValue(w / 10.0);
	SeStyle->UnderlineVal->LPos->setValue(p / 10.0);
	connect(SeStyle->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(newUnderlineHandler()));
	connect(SeStyle->UnderlineVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(newUnderlineHandler()));
}

void SToolBStyle::SetShadow(double x, double y)
{
	disconnect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowHandler()));
	disconnect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowHandler()));
	SeStyle->ShadowVal->Xoffset->setValue(x / 10.0);
	SeStyle->ShadowVal->Yoffset->setValue(y / 10.0);
	connect(SeStyle->ShadowVal->Xoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowHandler()));
	connect(SeStyle->ShadowVal->Yoffset, SIGNAL(valueChanged(double)), this, SLOT(newShadowHandler()));
}

void SToolBStyle::SetStyle(int s)
{
	disconnect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
	SeStyle->setStyle(s);
	connect(SeStyle, SIGNAL(State(int)), this, SIGNAL(newStyle(int)));
}

void SToolBStyle::SetKern(double k)
{
	disconnect(Extra, SIGNAL(valueChanged(double)), this, SLOT(newKernHandler()));
	Extra->setValue(k / 10.0);
	connect(Extra, SIGNAL(valueChanged(double)), this, SLOT(newKernHandler()));
}

/* Toolbar for alignment of Paragraphs */
SToolBAlign::SToolBAlign(QMainWindow* parent) : QToolBar( tr("Style Settings"), parent)
{
	GroupAlign = new AlignSelect(this);
	groupAlignAction=addWidget(GroupAlign);
	groupAlignAction->setVisible(true);
	paraStyleCombo = new ParaStyleComboBox(this);
	paraStyleComboAction=addWidget(paraStyleCombo);
	paraStyleComboAction->setVisible(true);
	connect(paraStyleCombo, SIGNAL(newStyle(const QString&)), this, SIGNAL(newParaStyle(const QString& )));
	connect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(newAlign(int )));

	languageChange();
}

void SToolBAlign::languageChange()
{
	paraStyleCombo->setToolTip("");
	paraStyleCombo->setToolTip( tr("Style of current paragraph"));
}


void SToolBAlign::SetAlign(int s)
{
	disconnect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(newAlign(int )));
	GroupAlign->setStyle(s);
	connect(GroupAlign, SIGNAL(State(int)), this, SIGNAL(newAlign(int )));
}

void SToolBAlign::SetParaStyle(QString s)
{
	disconnect(paraStyleCombo, SIGNAL(newStyle(const QString&)), this, SIGNAL(newParaStyle(const QString& )));
	paraStyleCombo->setFormat(s);
	connect(paraStyleCombo, SIGNAL(newStyle(const QString&)), this, SIGNAL(newParaStyle(const QString& )));
}


/* Toolbar for Font related Settings */
SToolBFont::SToolBFont(QMainWindow* parent) : QToolBar( tr("Font Settings"), parent)
{
	Fonts = new FontCombo(this);
	Fonts->setMaximumSize(190, 30);
	fontsAction=addWidget(Fonts);
	fontsAction->setVisible(true);
	Size = new ScrSpinBox( 0.5, 2048, this, 1 );
	PrefsManager* prefsManager = PrefsManager::instance();
	Size->setPrefix( "" );
	Size->setSuffix( tr( " pt" ) );
	Size->setValue(prefsManager->appPrefs.toolSettings.defSize / 10.0);
	sizeAction=addWidget(Size);
	sizeAction->setVisible(true);
	ScaleTxt = new QLabel("", this);
	ScaleTxt->setPixmap(loadIcon("textscaleh.png"));
	scaleTxtAction=addWidget(ScaleTxt);
	scaleTxtAction->setVisible(true);
	ChScale = new ScrSpinBox( 10, 400,  this, 1 );
	ChScale->setValue( 100 );
	ChScale->setSuffix( tr( " %" ) );
	chScaleAction=addWidget(ChScale);
	chScaleAction->setVisible(true);
	ScaleTxtV = new QLabel("", this);
	ScaleTxtV->setPixmap(loadIcon("textscalev.png"));
	scaleTxtVAction=addWidget(ScaleTxtV);
	scaleTxtVAction->setVisible(true);
	ChScaleV = new ScrSpinBox( 10, 400, this, 1 );
	ChScaleV->setValue( 100 );
	ChScaleV->setSuffix( tr( " %" ) );
	chScaleVAction=addWidget(ChScaleV);
	chScaleVAction->setVisible(true);

	connect(ChScale, SIGNAL(valueChanged(double)), this, SIGNAL(newScale(double)));
	connect(ChScaleV, SIGNAL(valueChanged(double)), this, SIGNAL(newScaleV(double)));
	connect(Fonts, SIGNAL(activated(const QString &)), this, SIGNAL(NewFont(const QString &)));
	connect(Size, SIGNAL(valueChanged(double)), this, SIGNAL(NewSize(double)));
}

void SToolBFont::languageChange()
{
	Fonts->setToolTip("");
	Size->setToolTip("");
	ChScale->setToolTip("");
	ChScaleV->setToolTip("");
	Fonts->setToolTip( tr("Font of selected text"));
	Size->setToolTip( tr("Font Size"));
	ChScale->setToolTip( tr("Scaling width of characters"));
	ChScaleV->setToolTip( tr("Scaling height of characters"));
}

void SToolBFont::SetFont(QString f)
{
	disconnect(Fonts, SIGNAL(activated(const QString &)), this, SIGNAL(NewFont(const QString &)));
	setCurrentComboItem(Fonts, f);
	connect(Fonts, SIGNAL(activated(const QString &)), this, SIGNAL(NewFont(const QString &)));
}

void SToolBFont::SetSize(double s)
{
	disconnect(Size, SIGNAL(valueChanged(double)), this, SLOT(newSizeHandler()));
	Size->setValue(s / 10.0);
	connect(Size, SIGNAL(valueChanged(double)), this, SLOT(newSizeHandler()));
}

void SToolBFont::SetScale(double s)
{
	disconnect(ChScale, SIGNAL(valueChanged(double)), this, SIGNAL(newScale(double)));
	ChScale->setValue(s / 10.0);
	connect(ChScale, SIGNAL(valueChanged(double)), this, SIGNAL(newScale(double)));
}

void SToolBFont::SetScaleV(double s)
{
	disconnect(ChScaleV, SIGNAL(valueChanged(double)), this, SIGNAL(newScaleV(double)));
	ChScaleV->setValue(s / 10.0);
	connect(ChScaleV, SIGNAL(valueChanged(double)), this, SIGNAL(newScaleV(double)));
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
StoryEditor::StoryEditor(QWidget* parent) : QMainWindow(parent, Qt::Window), // WType_Dialog) //WShowModal |
	activFromApp(true),
	currDoc(NULL),
	currItem(NULL),
	textChanged(false),
	firstSet(false),
	blockUpdate(false),
	CurrPara(0),
	CurrChar(0),
	charSelect(NULL),
	charSelectUsed(false)
{
	prefsManager=PrefsManager::instance();
#ifdef Q_WS_MAC
	noIcon = loadIcon("noicon.xpm");
#endif
	buildGUI();
	/*
	//Editor->loadItemText(ite);
	updateProps(0,0);
	updateStatus();
	*/
	Editor->setFocus();
	Editor->setFarbe(false);
	loadPrefs();
}

StoryEditor::~StoryEditor()
{
	savePrefs();
}

void StoryEditor::showEvent(QShowEvent *)
{
	charSelect = new CharSelect(this);
	charSelect->userTableModel()->setCharacters( ScCore->primaryMainWindow()->charPalette->userTableModel()->characters());
	connect(charSelect, SIGNAL(insertSpecialChar()), this, SLOT(slot_insertSpecialChar()));
	connect(charSelect, SIGNAL(insertUserSpecialChar(QChar)), this, SLOT(slot_insertUserSpecialChar(QChar)));
}

void StoryEditor::hideEvent(QHideEvent *)
{
	if (charSelectUsed)
		ScCore->primaryMainWindow()->charPalette->userTableModel()->setCharacters( charSelect->userTableModel()->characters());
	if (charSelect->isVisible())
		charSelect->close();
	disconnect(charSelect, SIGNAL(insertSpecialChar()), this, SLOT(slot_insertSpecialChar()));
	disconnect(charSelect, SIGNAL(insertUserSpecialChar(QChar)), this, SLOT(slot_insertUserSpecialChar(QChar)));
	delete charSelect;
	charSelect = NULL;
}

void StoryEditor::savePrefs()
{
	// save prefs
	QRect geo = geometry();
	prefs->set("left", geo.left());
	prefs->set("top", geo.top());
	prefs->set("width", width());
	prefs->set("height", height());
//	QList<int> splitted = EdSplit->sizes();
//	prefs->set("side", splitted[0]);
//	prefs->set("main", splitted[1]);
	prefs->set("winstate", QString(saveState().toBase64()));
}

void StoryEditor::loadPrefs()
{
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("StoryEditor");
	int vleft   = qMax(-80, prefs->getInt("left", 10));
#if defined(Q_OS_MAC) || defined(_WIN32)
	int vtop    = qMax(64, prefs->getInt("top", 10));
#else
	int vtop    = qMax(-80, prefs->getInt("top", 10));
#endif
	int vwidth  = qMax(600, prefs->getInt("width", 600));
	int vheight = qMax(400, prefs->getInt("height", 400));
	// Check values against current screen size
	QRect scr = QApplication::desktop()->screen()->geometry();
	QSize gStrut = QApplication::globalStrut();
	if ( vleft >= scr.width() )
		vleft = 0;
	if ( vtop >= scr.height() )
		vtop = 64;
	if ( vwidth >= scr.width() )
		vwidth = qMax( gStrut.width(), scr.width() - vleft );
	if ( vheight >= scr.height() )
		vheight = qMax( gStrut.height(), scr.height() - vtop );
	setGeometry(vleft, vtop, vwidth, vheight);
	QByteArray state = "";
	state = prefs->get("winstate","").toAscii();
	if (!state.isEmpty())
		restoreState(QByteArray::fromBase64(state));
/*	int side = prefs->getInt("side", -1);
	int txtarea = prefs->getInt("main", -1);
	if ((side != -1) && (txtarea != -1))
	{
		QList<int> splitted;
		splitted.append(side);
		splitted.append(txtarea);
		EdSplit->setSizes(splitted);
	} */
}

void StoryEditor::initActions()
{
	//File Menu
	seActions.insert("fileNew", new ScrAction(loadIcon("16/document-new.png"), loadIcon("22/document-new.png"), "", Qt::CTRL+Qt::Key_N, this));
	seActions.insert("fileRevert", new ScrAction(loadIcon("reload16.png"), loadIcon("reload.png"), "", QKeySequence(), this));
	seActions.insert("fileSaveToFile", new ScrAction(loadIcon("16/document-save.png"), loadIcon("22/document-save.png"), "", QKeySequence(), this));
	seActions.insert("fileLoadFromFile", new ScrAction(loadIcon("16/document-open.png"),  loadIcon("22/document-open.png"), "", QKeySequence(), this));
	seActions.insert("fileSaveDocument", new ScrAction("", Qt::CTRL+Qt::Key_S, this));
	seActions.insert("fileUpdateAndExit", new ScrAction(loadIcon("ok.png"), loadIcon("ok22.png"), "", Qt::CTRL+Qt::Key_W,  this));
	seActions.insert("fileExit", new ScrAction(loadIcon("exit.png"), loadIcon("exit22.png"), "", QKeySequence(),  this));

	connect( seActions["fileNew"], SIGNAL(triggered()), this, SLOT(Do_new()) );
	connect( seActions["fileRevert"], SIGNAL(triggered()), this, SLOT(slotFileRevert()) );
	connect( seActions["fileSaveToFile"], SIGNAL(triggered()), this, SLOT(SaveTextFile()) );
	connect( seActions["fileLoadFromFile"], SIGNAL(triggered()), this, SLOT(LoadTextFile()) );
	connect( seActions["fileSaveDocument"], SIGNAL(triggered()), this, SLOT(Do_saveDocument()) );
	connect( seActions["fileUpdateAndExit"], SIGNAL(triggered()), this, SLOT(Do_leave2()) );
	connect( seActions["fileExit"], SIGNAL(triggered()), this, SLOT(Do_leave()) );

	//Edit Menu
	seActions.insert("editSelectAll", new ScrAction(loadIcon("16/edit-select-all.png"), QPixmap(), "", Qt::CTRL+Qt::Key_A, this));
	seActions.insert("editCut", new ScrAction(loadIcon("16/edit-cut.png"), QPixmap(), "", Qt::CTRL+Qt::Key_X, this));
	seActions.insert("editCopy", new ScrAction(loadIcon("16/edit-copy.png"), QPixmap(), "", Qt::CTRL+Qt::Key_C, this));
	seActions.insert("editPaste", new ScrAction(loadIcon("16/edit-paste.png"), QPixmap(), "", Qt::CTRL+Qt::Key_V, this));
	seActions.insert("editClear", new ScrAction(loadIcon("16/edit-delete.png"), QPixmap(), "", Qt::Key_Delete, this));
	seActions.insert("editSearchReplace", new ScrAction(loadIcon("16/edit-find-replace.png"), QPixmap(), "", QKeySequence(), this));
	seActions.insert("editEditStyle", new ScrAction("", QKeySequence(), this));
	seActions.insert("editFontPreview", new ScrAction("", QKeySequence(), this));
	seActions.insert("editUpdateFrame", new ScrAction(loadIcon("compfile16.png"),loadIcon("compfile.png"), "", Qt::CTRL+Qt::Key_U, this));

	connect( seActions["editSelectAll"], SIGNAL(triggered()), this, SLOT(Do_selectAll()) );
	connect( seActions["editCut"], SIGNAL(triggered()), this, SLOT(Do_cut()) );
	connect( seActions["editCopy"], SIGNAL(triggered()), this, SLOT(Do_copy()) );
	connect( seActions["editPaste"], SIGNAL(triggered()), this, SLOT(Do_paste()) );
	connect( seActions["editClear"], SIGNAL(triggered()), this, SLOT(Do_del()) );
	connect( seActions["editSearchReplace"], SIGNAL(triggered()), this, SLOT(SearchText()) );
	connect( seActions["editEditStyle"], SIGNAL(triggered()), this, SLOT(slotEditStyles()) );
	connect( seActions["editFontPreview"], SIGNAL(triggered()), this, SLOT(Do_fontPrev()) );
	connect( seActions["editUpdateFrame"], SIGNAL(triggered()), this, SLOT(updateTextFrame()) );

	//Insert Menu
	seActions.insert("insertGlyph", new ScrAction(QPixmap(), QPixmap(), "", QKeySequence(), this));
	connect( seActions["insertGlyph"], SIGNAL(triggered()), this, SLOT(Do_insSp()) );

	//Settings Menu
	seActions.insert("settingsBackground", new ScrAction("", QKeySequence(), this));
	seActions.insert("settingsDisplayFont", new ScrAction("", QKeySequence(), this));
	seActions.insert("settingsSmartTextSelection", new ScrAction("", QKeySequence(), this));
	smartSelection = false;
	seActions["settingsSmartTextSelection"]->setChecked(false);
	seActions["settingsSmartTextSelection"]->setToggleAction(true);

	connect( seActions["settingsBackground"], SIGNAL(triggered()), this, SLOT(setBackPref()) );
	connect( seActions["settingsDisplayFont"], SIGNAL(triggered()), this, SLOT(setFontPref()) );
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
	seMenuMgr->createMenu("InsertChar", tr("Character"), "Insert");
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
	seMenuMgr->createMenu("InsertQuote", tr("Quote"), "Insert");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteApostrophe"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteStraight"], "InsertQuote");
	seMenuMgr->addMenuSeparator("InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteSingleLeft"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteSingleRight"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteDoubleLeft"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteDoubleRight"], "InsertQuote");
	seMenuMgr->addMenuSeparator("InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteSingleReversed"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteDoubleReversed"], "InsertQuote");
	seMenuMgr->addMenuSeparator("InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteLowSingleComma"], "InsertQuote");
	seMenuMgr->addMenuItem(seActions["unicodeQuoteLowDoubleComma"], "InsertQuote");
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
	seMenuMgr->createMenu("InsertSpace", tr("Spaces && Breaks"), "Insert");
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
	seMenuMgr->createMenu("InsertLigature", tr("Ligature"), "Insert");
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

	setWindowIcon(loadIcon("AppIcon.png"));
	StoryEd2Layout = new QHBoxLayout;
	StoryEd2Layout->setSpacing( 5 );
	StoryEd2Layout->setMargin( 5 );

/* Setting up Toolbars */
	FileTools = new QToolBar(this);
	FileTools->setIconSize(QSize(16,16));
	FileTools->setObjectName("File");
	FileTools->addAction(seActions["fileNew"]);
	FileTools->addAction(seActions["fileLoadFromFile"]);
	FileTools->addAction(seActions["fileSaveToFile"]);
	FileTools->addAction(seActions["fileUpdateAndExit"]);
	FileTools->addAction(seActions["fileExit"]);
	FileTools->addAction(seActions["fileRevert"]);
	FileTools->addAction(seActions["editUpdateFrame"]);
	FileTools->addAction(seActions["editSearchReplace"]);

	FileTools->setAllowedAreas(Qt::LeftToolBarArea);
	FileTools->setAllowedAreas(Qt::RightToolBarArea);
	FileTools->setAllowedAreas(Qt::BottomToolBarArea);
	FileTools->setAllowedAreas(Qt::TopToolBarArea);
	FontTools = new SToolBFont(this);
	FontTools->setIconSize(QSize(16,16));
	FontTools->setObjectName("Font");
	FontTools->setAllowedAreas(Qt::LeftToolBarArea);
	FontTools->setAllowedAreas(Qt::RightToolBarArea);
	FontTools->setAllowedAreas(Qt::BottomToolBarArea);
	FontTools->setAllowedAreas(Qt::TopToolBarArea);
	AlignTools = new SToolBAlign(this);
	AlignTools->setIconSize(QSize(16,16));
	AlignTools->setObjectName("Align");
	AlignTools->setAllowedAreas(Qt::LeftToolBarArea);
	AlignTools->setAllowedAreas(Qt::RightToolBarArea);
	AlignTools->setAllowedAreas(Qt::BottomToolBarArea);
	AlignTools->setAllowedAreas(Qt::TopToolBarArea);
	AlignTools->paraStyleCombo->setDoc(currDoc);
	StyleTools = new SToolBStyle(this);
	StyleTools->setIconSize(QSize(16,16));
	StyleTools->setObjectName("Style");
	StyleTools->setAllowedAreas(Qt::LeftToolBarArea);
	StyleTools->setAllowedAreas(Qt::RightToolBarArea);
	StyleTools->setAllowedAreas(Qt::BottomToolBarArea);
	StyleTools->setAllowedAreas(Qt::TopToolBarArea);
	StrokeTools = new SToolBColorS(this, currDoc);
	StrokeTools->setIconSize(QSize(16,16));
	StrokeTools->setObjectName("Strok");
	StrokeTools->setAllowedAreas(Qt::LeftToolBarArea);
	StrokeTools->setAllowedAreas(Qt::RightToolBarArea);
	StrokeTools->setAllowedAreas(Qt::BottomToolBarArea);
	StrokeTools->setAllowedAreas(Qt::TopToolBarArea);
	StrokeTools->TxStroke->setEnabled(false);
	StrokeTools->PM1->setEnabled(false);
	FillTools = new SToolBColorF(this, currDoc);
	FillTools->setIconSize(QSize(16,16));
	FillTools->setObjectName("Fill");
	FillTools->setAllowedAreas(Qt::LeftToolBarArea);
	FillTools->setAllowedAreas(Qt::RightToolBarArea);
	FillTools->setAllowedAreas(Qt::BottomToolBarArea);
	FillTools->setAllowedAreas(Qt::TopToolBarArea);
	
	addToolBar(FileTools);
	addToolBar(FontTools);
	addToolBar(AlignTools);
	addToolBar(StyleTools);
	addToolBar(StrokeTools);
	addToolBar(FillTools);

//	EdSplit = new QSplitter(this);
/* SideBar Widget */
//	EditorBar = new SideBar(EdSplit);
/* Editor Widget, subclass of QTextEdit */
	Editor = new SEditor(this, currDoc, this);
	StoryEd2Layout->addWidget( Editor );

/* Setting up Status Bar */
	ButtonGroup1 = new QFrame( statusBar() );
	ButtonGroup1->setFrameShape( QFrame::NoFrame );
	ButtonGroup1->setFrameShadow( QFrame::Plain );
	ButtonGroup1Layout = new QGridLayout( ButtonGroup1 );
	ButtonGroup1Layout->setAlignment( Qt::AlignTop );
	ButtonGroup1Layout->setSpacing( 2 );
	ButtonGroup1Layout->setMargin( 0 );
	WordCT1 = new QLabel(ButtonGroup1);
	ButtonGroup1Layout->addWidget( WordCT1, 0, 0, 1, 3 );
	WordCT = new QLabel(ButtonGroup1);
	ButtonGroup1Layout->addWidget( WordCT, 1, 0 );
	WordC = new QLabel(ButtonGroup1);
	ButtonGroup1Layout->addWidget( WordC, 1, 1 );
	CharCT = new QLabel(ButtonGroup1);
	ButtonGroup1Layout->addWidget( CharCT, 1, 2 );
	CharC = new QLabel(ButtonGroup1);
	ButtonGroup1Layout->addWidget( CharC, 1, 3 );
	statusBar()->addPermanentWidget(ButtonGroup1, 1);
	ButtonGroup2 = new QFrame( statusBar() );
	ButtonGroup2->setFrameShape( QFrame::NoFrame );
	ButtonGroup2->setFrameShadow( QFrame::Plain );
	ButtonGroup2Layout = new QGridLayout( ButtonGroup2 );
	ButtonGroup2Layout->setAlignment( Qt::AlignTop );
	ButtonGroup2Layout->setSpacing( 2 );
	ButtonGroup2Layout->setMargin( 0 );
	WordCT3 = new QLabel(ButtonGroup2);
	ButtonGroup2Layout->addWidget( WordCT3, 0, 0, 1, 5 );
	ParCT = new QLabel(ButtonGroup2);
	ButtonGroup2Layout->addWidget( ParCT, 1, 0 );
	ParC = new QLabel(ButtonGroup2);
	ButtonGroup2Layout->addWidget( ParC, 1, 1 );
	WordCT2 = new QLabel(ButtonGroup2);
	ButtonGroup2Layout->addWidget( WordCT2, 1, 2 );
	WordC2 = new QLabel(ButtonGroup2);
	ButtonGroup2Layout->addWidget( WordC2, 1, 3 );
	CharCT2 = new QLabel(ButtonGroup2);
	ButtonGroup2Layout->addWidget( CharCT2, 1, 4 );
	CharC2 = new QLabel(ButtonGroup2);
	ButtonGroup2Layout->addWidget( CharC2, 1, 5 );
	statusBar()->addPermanentWidget(ButtonGroup2, 1);
	setCentralWidget( Editor );
	//Final setup
	resize( QSize(660, 500).expandedTo(minimumSizeHint()) );
	if (prefsManager==NULL)
		sDebug(QString("%1").arg("prefsmgr null"));

//	Editor->setPaper(prefsManager->appPrefs.STEcolor);
	QFont fo;
	fo.fromString(prefsManager->appPrefs.STEfont);
	Editor->setFont(fo);
//	EditorBar->setFrameStyle(Editor->frameStyle());
//	EditorBar->setLineWidth(Editor->lineWidth());
//	EditorBar->editor = Editor;
	Editor->installEventFilter(this);
	languageChange();
}

void StoryEditor::languageChange()
{
	setWindowTitle( tr( "Story Editor" ) );
	//File Menu
	seMenuMgr->setText("File", tr("&File"));
	seActions["fileNew"]->setText( tr("Clear All Text"));
	seActions["fileRevert"]->setTexts( tr("&Reload Text from Frame"));
	seActions["fileSaveToFile"]->setTexts( tr("&Save to File..."));
	seActions["fileLoadFromFile"]->setTexts( tr("&Load from File..."));
	seActions["fileSaveDocument"]->setTexts( tr("Save &Document"));
	seActions["fileUpdateAndExit"]->setTexts( tr("&Update Text Frame and Exit"));
	seActions["fileExit"]->setTexts( tr("&Exit Without Updating Text Frame"));
	//Edit Menu
	seMenuMgr->setText("Edit", tr("&Edit"));
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
	seMenuMgr->setText("Insert", tr("&Insert"));
	seMenuMgr->setText("InsertChar", tr("Character"));
	seMenuMgr->setText("InsertQuote", tr("Quote"));
	seMenuMgr->setText("InsertSpace", tr("Space"));
	seActions["insertGlyph"]->setTexts( tr("&Insert Glyph..."));

	//Settings Menu
	seMenuMgr->setText("Settings", tr("&Settings"));
	seActions["settingsBackground"]->setTexts( tr("&Background..."));
	seActions["settingsDisplayFont"]->setTexts( tr("&Display Font..."));
	seActions["settingsSmartTextSelection"]->setTexts( tr("&Smart text selection"));

	//Unicode Actions
	ActionManager::languageChangeUnicodeActions(&seActions);

	FileTools->setWindowTitle( tr("File"));

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
//	disconnect(EditorBar,0,0,0);
	disconnect(AlignTools,0,0,0);
	disconnect(FillTools,0,0,0);
	disconnect(FontTools,0,0,0);
	disconnect(StrokeTools,0,0,0);
	disconnect(StyleTools,0,0,0);
}

void StoryEditor::connectSignals()
{
	connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
//	connect(Editor, SIGNAL(clicked(int, int)), this, SLOT(updateProps(int, int)));
	connect(Editor, SIGNAL(setProps(int, int)), this, SLOT(updateProps(int, int)));
	connect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
	connect(Editor, SIGNAL(copyAvailable(bool)), this, SLOT(CopyAvail(bool )));
	connect(Editor, SIGNAL(PasteAvail()), this, SLOT(PasteAvail()));
//	connect(Editor, SIGNAL(contentsMoving(int, int)), EditorBar, SLOT(doMove(int, int )));
//	connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
//	connect(Editor, SIGNAL(SideBarUp(bool )), EditorBar, SLOT(setRepaint(bool )));
//	connect(Editor, SIGNAL(SideBarUpdate( )), EditorBar, SLOT(doRepaint()));
	// 10/12/2004 - pv - #1203: wrong selection on double click
//	connect(Editor, SIGNAL(doubleClicked(int, int)), this, SLOT(doubleClick(int, int)));
//	connect(EditorBar, SIGNAL(ChangeStyle(int, const QString& )), this, SLOT(changeStyleSB(int, const QString&)));
//	connect(EditorBar, SIGNAL(sigEditStyles()), this, SLOT(slotEditStyles()));
	connect(AlignTools, SIGNAL(newParaStyle(const QString&)), this, SLOT(newStyle(const QString&)));
	connect(AlignTools, SIGNAL(newAlign(int)), this, SLOT(newAlign(int)));
	connect(FillTools, SIGNAL(NewColor(int, int)), this, SLOT(newTxFill(int, int)));
	connect(StrokeTools, SIGNAL(NewColor(int, int)), this, SLOT(newTxStroke(int, int)));
	connect(FontTools, SIGNAL(NewSize(double )), this, SLOT(newTxSize(double)));
	connect(FontTools, SIGNAL(NewFont(const QString& )), this, SLOT(newTxFont(const QString& )));
	connect(FontTools, SIGNAL(newScale(double )), this, SLOT(newTxScale()));
	connect(FontTools, SIGNAL(newScaleV(double )), this, SLOT(newTxScaleV()));
	connect(StyleTools, SIGNAL(NewKern(double )), this, SLOT(newTxKern(double )));
	connect(StyleTools, SIGNAL(newStyle(int )), this, SLOT(newTxStyle(int )));
	connect(StyleTools, SIGNAL(NewShadow(double, double)), this, SLOT(newShadowOffs(double, double)));
	connect(StyleTools, SIGNAL(newOutline(double )), this, SLOT(newTxtOutline(double )));
	connect(StyleTools, SIGNAL(newUnderline(double, double)), this, SLOT(newTxtUnderline(double, double)));
	connect(StyleTools, SIGNAL(newStrike(double, double )), this, SLOT(newTxtStrike(double, double)));
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
		setWindowTitle( tr("Story Editor - %1").arg(currItem->itemName()));
		firstSet = false;
		FontTools->Fonts->RebuildList(currDoc, currItem->isAnnotation());
		Editor->loadItemText(currItem);
//		Editor->getCursorPosition(&CurrPara, &CurrChar);
		Editor->repaint();
//		EditorBar->setRepaint(true);
//		EditorBar->doRepaint();
		updateProps(0,0);
		updateStatus();
		connectSignals();
	}
	else
	{
		Editor->StyledText.clear();
		Editor->clear();
		setWindowTitle( tr( "Story Editor" ));
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
	int indexFrom=0, indexTo=0;
	QString selText = Editor->textCursor().selectedText();
	if (selText.length() == 0 || !smartSelection)
	{
		updateProps(para, position);
		return;
	}
	indexFrom = Editor->textCursor().selectionStart();
	indexTo = Editor->textCursor().selectionEnd();
	selText =  selText.trimmed();
	Editor->textCursor().clearSelection();
	Editor->textCursor().setPosition(indexFrom);
	Editor->textCursor().setPosition(indexFrom + selText.length(), QTextCursor::KeepAnchor);
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
	setCurrentDocumentAndItem(NULL, NULL);
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
//		Editor->getCursorPosition(&CurrPara, &CurrChar);
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
				Editor->textCursor().setPosition(0);
				seActions["fileRevert"]->setEnabled(false);
				seActions["editCopy"]->setEnabled(false);
				seActions["editCut"]->setEnabled(false);
				seActions["editClear"]->setEnabled(false);
				textChanged = false;
				FontTools->Fonts->RebuildList(currDoc, currItem->isAnnotation());
				Editor->loadItemText(currItem);
//				Editor->getCursorPosition(&CurrPara, &CurrChar);
				updateStatus();
				textChanged = false;
				Editor->repaint();
//				EditorBar->doMove(0, Editor->contentsY());
//				EditorBar->setRepaint(true);
//				EditorBar->doRepaint();
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
	neu = QColorDialog::getColor(Editor->palette().color(QPalette::Base), this);
	if (neu.isValid())
	{
		QPalette pal;
		pal.setColor(QPalette::Active, QPalette::Base, neu);
		pal.setColor(QPalette::Inactive, QPalette::Base, neu);
		pal.setColor(QPalette::Disabled, QPalette::Base, neu);
		Editor->setPalette(pal);
		prefsManager->appPrefs.STEcolor = neu;
	}
	blockUpdate = false;
}

void StoryEditor::setFontPref()
{
	blockUpdate = true;
	Editor->setFont( QFontDialog::getFont( 0, Editor->font(), this ) );
	prefsManager->appPrefs.STEfont = Editor->font().toString();
//	EditorBar->doRepaint();
	blockUpdate = false;
}

void StoryEditor::newTxFill(int c, int s)
{
	if (c != -1)
		Editor->CurrTextFill = FillTools->TxFill->itemText(c);
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
		Editor->CurrTextStroke = StrokeTools->TxStroke->itemText(c);
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
	charStyle.setFeatures(Editor->CurrentStyle.featureList());
	Editor->updateSel(charStyle);
	Editor->setStyle(s);
	if ((s & ScStyle_Outline) || (s & ScStyle_Shadowed))
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

void StoryEditor::newTxScale()
{
	int ss = qRound(FontTools->ChScale->value() * 10);
	Editor->CurrTextScale = ss;
	CharStyle charStyle;
	charStyle.setScaleH(Editor->CurrTextScale);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxScaleV()
{
	int ss = qRound(FontTools->ChScaleV->value() * 10);
	Editor->CurrTextScaleV = ss;
	CharStyle charStyle;
	charStyle.setScaleV(Editor->CurrTextScaleV);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxKern(double s)
{
	Editor->CurrTextKern = s;
	CharStyle charStyle;
	charStyle.setTracking(Editor->CurrTextKern);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newShadowOffs(double x, double y)
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

void StoryEditor::newTxtOutline(double o)
{
	Editor->CurrTextOutline = o;
	CharStyle charStyle;
	charStyle.setOutlineWidth(Editor->CurrTextOutline);
	Editor->updateSel(charStyle);
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::newTxtUnderline(double p, double w)
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

void StoryEditor::newTxtStrike(double p, double w)
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

void StoryEditor::updateProps()
{
	QTextCursor cur = Editor->textCursor();
	updateProps(cur);
}

void StoryEditor::updateProps(QTextCursor &cur)
{
	int pos = cur.position();
	int para = Editor->StyledText.nrOfParagraph(pos);
	int start = Editor->StyledText.startOfParagraph(para);
	updateProps(para, pos - start);
}

void StoryEditor::updateProps(int p, int ch)
{
	ColorList::Iterator it;
	int c = 0;
	if (Editor->wasMod)
		return;
	if ((p >= static_cast<int>(Editor->StyledText.nrOfParagraphs())) || (Editor->StyledText.length() == 0) || (!firstSet))
	{
		int pos = Editor->StyledText.startOfParagraph(p) + ch;
		if (!firstSet)
		{
			const CharStyle& curstyle(pos < Editor->StyledText.length()? currItem->itemText.charStyle(pos) : currItem->itemText.defaultStyle().charStyle());
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
			AlignTools->SetParaStyle(Editor->currentParaStyle);
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
		if ((Editor->CurrentStyle & ScStyle_Outline) || (Editor->CurrentStyle & ScStyle_Shadowed))
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
		if (Editor->textCursor().hasSelection())
//		{
//			int PStart=0, PEnd=0, SelStart=0, SelEnd=0;
			start = Editor->textCursor().selectionStart();
//			Editor->getSelection(&PStart, &SelStart, &PEnd, &SelEnd);
//			start = Editor->StyledText.startOfParagraph(PStart);
//			if (SelStart >= 0 && start + SelStart < Editor->StyledText.endOfParagraph(PStart))
//				start = qMin(start + SelStart, Editor->StyledText.endOfParagraph(PStart)-1);
//			else
//				start = qMin(start + qMax(ch-1, 0), Editor->StyledText.endOfParagraph(p)-1);
//		}
		else
			start = qMin(Editor->StyledText.startOfParagraph(p) + qMax(ch-1, 0), Editor->StyledText.endOfParagraph(p)-1);
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
	if ((Editor->CurrentStyle & ScStyle_Outline) || (Editor->CurrentStyle & ScStyle_Shadowed))
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
	AlignTools->SetParaStyle(Editor->currentParaStyle);
	updateUnicodeActions();
	updateStatus();
}

void StoryEditor::updateStatus()
{
	QString tmp;
	int p = Editor->StyledText.nrOfParagraph(Editor->textCursor().position());
	int start = Editor->StyledText.startOfParagraph(p);
	int end = Editor->StyledText.endOfParagraph(p);

	ParC->setText(tmp.setNum(Editor->StyledText.nrOfParagraphs()));
	CharC2->setText(tmp.setNum(Editor->StyledText.length()));

	CharC->setText(tmp.setNum(end - start));
	QRegExp rx( "(\\w+)\\b" );
	const QString& txt(Editor->StyledText.text(0, Editor->StyledText.length()));
	int pos = 1;
	int counter = end > start? 1 : 0;
	int counter2 = Editor->StyledText.length() > 0? 1 : 0;
	while ( pos >= 0 )
	{
		pos = rx.indexIn( txt, pos );
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
	//ScCore->primaryMainWindow()->charPalette->show();
	charSelectUsed = true;
	if (charSelect->isVisible())
		return;
	charSelect->setEnabled(true, 0);
	charSelect->setDoc(currDoc);
	charSelect->show();
}

void StoryEditor::slot_insertSpecialChar()
{
	blockUpdate = true;
	if (!charSelect->getCharacters().isEmpty())
	{
		Editor->insChars(charSelect->getCharacters());
		Editor->insertPlainText(charSelect->getCharacters());
	}
	blockUpdate = false;
}

void StoryEditor::slot_insertUserSpecialChar(QChar c)
{
	blockUpdate = true;
	Editor->insChars(c);
	Editor->insertPlainText(c);
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
		                             QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
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
	if (!Editor->document()->isEmpty())
	{
		blockUpdate = true;
		int t = ScMessageBox::warning(this, CommonStrings::trWarning,
	                             tr("Do you really want to clear all your text?"),
	                             QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
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
	//qDebug("SE::Do_new: cursor");
	Editor->textCursor().setPosition(0);
	seActions["fileRevert"]->setEnabled(false);
	seActions["editCopy"]->setEnabled(false);
	seActions["editCut"]->setEnabled(false);
	seActions["editClear"]->setEnabled(false);
//	textChanged = false;
//	EditorBar->setRepaint(true);
//	EditorBar->doRepaint();
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
//		Editor->getCursorPosition(&CurrPara, &CurrChar);
		updateStatus();
//		EditorBar->setRepaint(true);
//		EditorBar->doRepaint();
		Editor->repaint();
	}
}

void StoryEditor::Do_selectAll()
{
	if (Editor->StyledText.length() == 0)
		return;
	Editor->selectAll();
/*	int lastPar = Editor->StyledText.nrOfParagraphs()-1;
	if (lastPar > 0)
	{
		int lastParStart = Editor->StyledText.startOfParagraph(lastPar);
		int lastParEnd = Editor->StyledText.endOfParagraph(lastPar);
		Editor->setSelection(0, 0, lastPar, lastParEnd - lastParStart);
	}
	else
	{
		Editor->setSelection(0, 0, 0, Editor->StyledText.length());
	} */
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
//	EditorBar->setRepaint(false);
	if (Editor->textCursor().hasSelection())
	{
		Editor->deleteSel();
		Editor->textCursor().removeSelectedText();
	}
//	EditorBar->setRepaint(true);
//	EditorBar->doRepaint();
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
#if 0
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
#endif
	if (currItem->asTextFrame())
		currItem->asTextFrame()->invalidateLayout();
	PageItem* nb2 = nextItem;
	nb2->itemText.clear();
#if 0
	if (currItem->asTextFrame())
	{
		while (nb2 != 0)
		{
		for (int j = nb2->firstInFrame(); j <= nb2->lastInFrame(); ++j)
		{
			if ((nb2->itemText.text(j) == SpecialChars::OBJECT) && (nb2->itemText.item(j)->cembedded != 0))
			{
				QList<PageItem*> emG;
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
			nb2->CPos = 0;
			nb2->Dirty = false;
			nb2 = nb2->nextInChain();
		}
	}
#endif
	Editor->saveItemText(nextItem);
#if 0
	QList<PageItem*> FrameItemsDel;
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
#endif
	currDoc->updateFrameItems();
#if 0
	if (currItem->asTextFrame())
	{
		nextItem->layout();
		nb2 = nextItem;
		while (nb2 != 0)
		{
			nb2->Dirty = false;
			nb2 = nb2->nextInChain();
		}
	}
#endif
	ScCore->primaryMainWindow()->view->DrawNew();
	textChanged = false;
	seActions["fileRevert"]->setEnabled(false);
	seActions["editUpdateFrame"]->setEnabled(false);
	emit DocChanged();
}

void StoryEditor::SearchText()
{
	blockUpdate = true;
//	EditorBar->setRepaint(false);
	SearchReplace* dia = new SearchReplace(this, currDoc, currItem, false);
	dia->exec();
	delete dia;
	qApp->processEvents();
	blockUpdate = false;
//	EditorBar->setRepaint(true);
//	EditorBar->doRepaint();
}

void StoryEditor::slotEditStyles()
{
	ScCore->primaryMainWindow()->styleMgr()->exec(this);
}

void StoryEditor::newAlign(int st)
{
	Editor->CurrAlign = st;
	changeAlign(st);
}


void StoryEditor::newStyle(const QString& name)
{
	Editor->currentParaStyle = name;
	changeStyle();
}


void StoryEditor::changeStyleSB(int pa, const QString& name)
{
	Editor->currentParaStyle = name;
	ParagraphStyle newStyle;
	newStyle.setParent(Editor->currentParaStyle);

	if (Editor->StyledText.length() != 0)
	{
		disconnect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		disconnect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
//		disconnect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));

/*		qDebug(QString("changeStyleSB: pa=%2, start=%2, new=%3 %4")
			   .arg(pa)
			   .arg(Editor->StyledText.startOfParagraph(pa))
			   .arg(newStyle.parent())
			   .arg(newStyle.hasParent()));
*/
		Editor->StyledText.applyStyle(Editor->StyledText.startOfParagraph(pa), newStyle);

		Editor->updateFromChars(pa);
//		Editor->setCursorPosition(pa, 0);
//		updateProps(pa, 0);
		Editor->ensureCursorVisible();
//		EditorBar->doRepaint();
		connect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
//		connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
	}
	else
	{
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().font().scName();
		Editor->CurrFontSize = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fontSize();
		Editor->CurrentStyle = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().effects();
		Editor->CurrTextFill = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fillColor();
		Editor->CurrTextFillSh = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fillShade();
		Editor->CurrTextStroke = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strokeColor();
		Editor->CurrTextStrokeSh = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strokeShade();
		Editor->CurrTextShadowX = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().shadowXOffset();
		Editor->CurrTextShadowY = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().shadowYOffset();
		Editor->CurrTextOutline = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().outlineWidth();
		Editor->CurrTextUnderPos = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().underlineOffset();
		Editor->CurrTextUnderWidth = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().underlineWidth();
		Editor->CurrTextStrikePos = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strikethruOffset();
		Editor->CurrTextStrikeWidth = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strikethruWidth();

		Editor->setStyle(Editor->CurrentStyle);
		if ((Editor->CurrentStyle & ScStyle_Outline) || (Editor->CurrentStyle & ScStyle_Shadowed))
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		//qDebug("SE::changeStyleSB: cursor");
		Editor->textCursor().setPosition(0);
		updateProps(0, 0);
	}

	Editor-> repaint();
	modifiedText();
	Editor->setFocus();
}

void StoryEditor::changeStyle()
{
	int p = 0;
	bool sel = false;
	ParagraphStyle newStyle;
	newStyle.setParent(Editor->currentParaStyle);

	int pos = Editor->textCursor().position();
	p = Editor->StyledText.nrOfParagraph(pos);
	if (Editor->StyledText.length() != 0)
	{
		disconnect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		disconnect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
//		disconnect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
		int PStart = 0;
		int PEnd = 0;
		int SelStart = 0;
		int SelEnd = 0;
		if (Editor->textCursor().hasSelection())
		{
			PStart = Editor->StyledText.nrOfParagraph(Editor->textCursor().selectionStart());
			PEnd = Editor->StyledText.nrOfParagraph(Editor->textCursor().selectionEnd());
			SelStart = Editor->textCursor().selectionStart();
			SelEnd = Editor->textCursor().selectionEnd();
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
		Editor->textCursor().setPosition(pos);
		if (sel)
		{
			Editor->textCursor().setPosition(SelStart);
			Editor->textCursor().setPosition(SelEnd, QTextCursor::KeepAnchor);
		}
		Editor->ensureCursorVisible();
		updateProps();
//		EditorBar->doRepaint();
		connect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
//		connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
	}
	else
	{
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().font().scName();
		Editor->CurrFontSize = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fontSize();
		Editor->CurrentStyle = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().effects();
		Editor->CurrTextFill = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fillColor();
		Editor->CurrTextFillSh = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fillShade();
		Editor->CurrTextStroke = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strokeColor();
		Editor->CurrTextStrokeSh = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strokeShade();
		Editor->CurrTextShadowX = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().shadowXOffset();
		Editor->CurrTextShadowY = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().shadowYOffset();
		Editor->CurrTextOutline = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().outlineWidth();
		Editor->CurrTextUnderPos = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().underlineOffset();
		Editor->CurrTextUnderWidth = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().underlineWidth();
		Editor->CurrTextStrikePos = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strikethruOffset();
		Editor->CurrTextStrikeWidth = currDoc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strikethruWidth();

		Editor->setStyle(Editor->CurrentStyle);
		if ((Editor->CurrentStyle & ScStyle_Outline) || (Editor->CurrentStyle & ScStyle_Shadowed))
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		Editor->textCursor().setPosition(0);
		updateProps(0, 0);
	}
	modifiedText();
	Editor-> repaint();
	Editor->setFocus();
}


void StoryEditor::changeAlign(int )
{
	int p = 0;
	bool sel = false;
	ParagraphStyle newStyle;
	newStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(Editor->CurrAlign));

	int pos = Editor->textCursor().position();
	p = Editor->StyledText.nrOfParagraph(pos);
	if (Editor->StyledText.length() != 0)
	{
		disconnect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		disconnect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
//		disconnect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
		int PStart = 0;
		int PEnd = 0;
		int SelStart = 0;
		int SelEnd = 0;
		if (Editor->textCursor().hasSelection())
		{
			PStart = Editor->StyledText.nrOfParagraph(Editor->textCursor().selectionStart());
			PEnd = Editor->StyledText.nrOfParagraph(Editor->textCursor().selectionEnd());
			SelStart = Editor->textCursor().selectionStart();
			SelEnd = Editor->textCursor().selectionEnd();
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
		Editor->textCursor().setPosition(pos);
		if (sel)
		{
			Editor->textCursor().setPosition(SelStart);
			Editor->textCursor().setPosition(SelEnd, QTextCursor::KeepAnchor);
		}
		Editor->ensureCursorVisible();
		updateProps();
//		EditorBar->doRepaint();
		connect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
//		connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
	}
	modifiedText();
	Editor->repaint();
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
//		EditorBar->setRepaint(false);
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
			dirs->set("story_load", fileName.left(fileName.lastIndexOf("/")));
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
//		EditorBar->setRepaint(true);
//		EditorBar->doRepaint();
	}
	Editor-> repaint();
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
		dirs->set("story_save", fileName.left(fileName.lastIndexOf("/")));
		Serializer::writeWithEncoding(fileName, LoadEnc, Editor->toPlainText());
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
	Editor->insertPlainText(guiInsertString);
	if (setColor)
		Editor->setFarbe(false);
	modifiedText();
//	EditorBar->setRepaint(true);
//	EditorBar->doRepaint();
}

void StoryEditor::updateUnicodeActions()
{
	if (Editor->prevFont!=Editor->CurrFont)
		ScCore->primaryMainWindow()->actionManager->enableUnicodeActions(&seActions, true, Editor->CurrFont);
}
