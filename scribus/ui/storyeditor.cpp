/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  story.cpp  -  description
							 -------------------
	begin			   : Tue Nov 11 2003
	copyright		   : (C) 2003 by Franz Schmid
	email			   : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *																	   *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or   *
 *   (at your option) any later version.								   *
 *																	   *
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
#include <QPair>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPalette>
#include <QPixmap>
#include <QRegExp>
#include <QShowEvent>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCodec>
#include <QTextLayout>
#include <QToolTip>

#include "actionmanager.h"
#include "alignselect.h"
#include "colorcombo.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "fontcombo.h"
#include "iconmanager.h"
#include "loremipsum.h"
#include "menumanager.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "pluginmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scfonts.h"
#include "scplugin.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "scrspinbox.h"
#include "search.h"
#include "serializer.h"
#include "shadebutton.h"
#include "storyeditor.h"
#include "styleitem.h"
#include "styleselect.h"
#include "ui/charselect.h"
#include "ui/customfdialog.h"
#include "ui/scmessagebox.h"
#include "ui/spalette.h"
#include "ui/stylemanager.h"
#include "units.h"
#include "util.h"
#include "util_debug.h"


class StyledTextMimeData : public QMimeData
{
protected:
	StoryText   m_styledText;
	ScGuardedPtr<ScribusDoc> m_styledTextDoc;

public:
	const StoryText&  styledText(void) const { return m_styledText; }
	const ScribusDoc* document(void)   const { return m_styledTextDoc; }

	void  setStyledText(const StoryText& text, ScribusDoc* doc)
	{
		QByteArray styledTextData (sizeof(void*), 0);
		m_styledText.clear();
		m_styledText.insert(0, text, true);
		m_styledTextDoc = doc->guardedPtr();
		styledTextData.setNum((quintptr)((quintptr*) &m_styledText));
		setData("application/x-scribus-styledtext", styledTextData);
	};
};

SideBar::SideBar(QWidget *pa) : QLabel(pa)
{
	QPalette pal;
	pal.setColor(QPalette::Window, QColor(255,255,255));
	setAutoFillBackground(true);
	setPalette(pal);
	offs = 0;
	currentPar = 0;
	editor = 0;
	noUpdt = true;
	inRep = false;
	pmen = new QMenu(this);
	paraStyleAct = NULL;
	setMinimumWidth(fontMetrics().width( tr("No Style") )+30);
}

void SideBar::mouseReleaseEvent(QMouseEvent *m)
{
	QPoint globalPos = m->globalPos();
	QPoint viewPos   = editor->viewport()->mapFromGlobal(globalPos);
	int p = editor->cursorForPosition(QPoint(2, viewPos.y())).position();
	currentPar = editor->StyledText.nrOfParagraph(p);
	int pos = editor->StyledText.startOfParagraph( editor->StyledText.nrOfParagraph(p) );

	pmen->clear();

	QString styleName = "";
	ParaStyleComboBox* paraStyleCombo = new ParaStyleComboBox(pmen);
	paraStyleCombo->setDoc(editor->doc);
	if ((currentPar < static_cast<int>(editor->StyledText.nrOfParagraphs())) && (editor->StyledText.length() != 0))
	{
		int len = editor->StyledText.endOfParagraph(currentPar) - editor->StyledText.startOfParagraph(currentPar);
		if (len > 0)
			styleName = editor->StyledText.paragraphStyle(pos).parent(); //FIXME ParaStyleComboBox and use localized style name
	}
	paraStyleCombo->setFormat(styleName);
	connect(paraStyleCombo, SIGNAL(newStyle(const QString&)), this, SLOT(setPStyle(const QString&)));
	
	paraStyleAct = new QWidgetAction(pmen);
	paraStyleAct->setDefaultWidget(paraStyleCombo);
	pmen->addAction(paraStyleAct);
	pmen->exec(QCursor::pos());
}

//void SideBar::editStyles()
//{
//	emit sigEditStyles();
//}

void SideBar::setPStyle(const QString& name)
{
	emit ChangeStyle(currentPar, name);
	pmen->close();
}

void SideBar::paintEvent(QPaintEvent *e)
{
	inRep = true;
	QLabel::paintEvent(e);
	QPair<int, int> paraInfo;
	QList< QPair<int,int> > paraList;
	if (editor != NULL)
	{
		QRect  edRect = editor->viewport()->rect();
		QPoint pt1 = edRect.topLeft(), pt2 = edRect.bottomRight();
		QTextCursor cur1 = editor->cursorForPosition(pt1);
		QTextCursor cur2 = editor->cursorForPosition(pt2);
		int pos1 = cur1.position(), pos2 = cur2.position();
		pos1 = editor->StyledText.prevParagraph(pos1);
		pos1 = (pos1 == 0) ? 0 : (pos1 + 1);
		pos2 = editor->StyledText.nextParagraph(pos2);
		while ((pos1 <= pos2) && (pos1 < editor->StyledText.length()))
		{
			paraInfo.first = pos1;
			if (editor->StyledText.text(pos1) == SpecialChars::PARSEP)
			{
				paraInfo.second = pos1;
				pos1 += 1;
			}
			else
			{
				pos1 = editor->StyledText.nextParagraph(pos1) + 1;
				paraInfo.second = qMax(0, qMin(pos1 - 1, editor->StyledText.length() - 1));
			}
			paraList.append(paraInfo);
		}
	}
	QPainter p;
	p.begin(this);
	if ((editor != NULL) && (noUpdt))
	{
		QString trNoStyle = tr("No Style");
		for (int pa = 0; pa < paraList.count(); ++pa)
		{
			QPair<int,int> paraInfo = paraList[pa];
			// Draw paragraph style name first
			QTextCursor cur(editor->document());
			cur.setPosition(paraInfo.first);
			QTextBlock blockStart = cur.block();
			QTextLine  lineStart  = blockStart.layout()->lineForTextPosition(paraInfo.first - blockStart.position());
			if (lineStart.isValid())
			{
				QPointF blockPos = blockStart.layout()->position();
				QRect re = lineStart.rect().translated(0, blockPos.y()).toRect();
				re.setWidth(width()-5);
				re.setHeight(re.height()-2);
				re.translate(5, 2-offs);
				if ((re.top() < height()) && (re.top() >= 0))
				{
					QString parname = editor->StyledText.paragraphStyle(paraInfo.first).parent();
					if (parname.isEmpty())
						parname = trNoStyle;
					p.drawText(re, Qt::AlignLeft | Qt::AlignTop, parname);
				}
			}
			// Draw paragraph separation line
			cur.setPosition(paraInfo.second);
			QTextBlock blockEnd = cur.block();
			QTextLine  lineEnd  = blockEnd.layout()->lineForTextPosition(paraInfo.second - blockEnd.position());
			if (lineEnd.isValid())
			{
				QPointF blockPos = blockEnd.layout()->position();
				QRect re = lineEnd.rect().translated(0, 2 + blockPos.y()).toRect();
				if ((re.bottom() - offs < height()) && (re.bottom() - offs >= 0))
					p.drawLine(0, re.bottom()-offs, width()-1, re.bottom() - offs);
			}
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

SEditor::SEditor(QWidget* parent, ScribusDoc *docc, StoryEditor* parentSE) : QTextEdit(parent)
{
	setCurrentDocument(docc);
	parentStoryEditor=parentSE;
	wasMod = false;
	SelCharStart = 0;
	StyledText.clear();
	document()->setUndoRedoEnabled(true);
	viewport()->setAcceptDrops(false);
	unicodeTextEditMode = false;
	blockContentsChangeHook = 0;
	setAutoFillBackground(true);
	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
	connect(this->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(handleContentsChange(int, int, int)));
	SuspendContentsChange = 0;
}

void SEditor::setCurrentDocument(ScribusDoc *docc)
{
	doc = docc;
	StyledText = StoryText(docc);
}

void SEditor::inputMethodEvent(QInputMethodEvent *event)
{
	QString uc = event->commitString();
	SuspendContentsChange = 1;	// prevent our handler from doing anything
	bool changed = false;
	int pos;
	if(textCursor().hasSelection())
	{
		pos =  textCursor().selectionStart();
		StyledText.removeChars(pos, textCursor().selectionEnd() - pos);
		changed = true;
	}
	pos = -1;
	if(!uc.isEmpty())
	{
		if ((*doc->AllFonts)[CurrFont].canRender(uc[0]))
		{
			pos = textCursor().hasSelection() ? textCursor().selectionStart() : textCursor().position();
			pos = qMin(pos, StyledText.length());
		}
		else
		{
			event->setCommitString("");
		}
	}
	QTextEdit::inputMethodEvent(event);
	SuspendContentsChange = 0;
	if(pos >= 0)
	{
		handleContentsChange(pos, 0, uc.length());
		changed = true;
	}
	if(changed)
	{
		emit SideBarUp(true);
		emit SideBarUpdate();
	}
/*	if ((!uc.isEmpty()) && ((*doc->AllFonts)[CurrFont].canRender(uc[0])))
	{
		// Should be processed by the handleContentsChange slot
		// insertCharsInternal(event->commitString());
		QTextEdit::inputMethodEvent(event);
		emit SideBarUp(true);
		emit SideBarUpdate();
	} */
}

void SEditor::keyPressEvent(QKeyEvent *k)
{
	emit SideBarUp(false);
// 	int pos = textCursor().position();
	int keyMod=0;
	if (k->modifiers() & Qt::ShiftModifier)
		keyMod |= Qt::SHIFT;
	if (k->modifiers() & Qt::ControlModifier)
		keyMod |= Qt::CTRL;
	if (k->modifiers() & Qt::AltModifier)
		keyMod |= Qt::ALT;

	if(ScCore->primaryMainWindow()->actionManager->compareKeySeqToShortcut(k->key(), k->modifiers(), "specialUnicodeSequenceBegin"))
	{
		unicodeTextEditMode = true;
		unicodeInputCount = 0;
		unicodeInputString = "";
		return;
	}

	QString uc = k->text();
	if ((k->modifiers() == Qt::ControlModifier) ||
		(k->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) ||
		(k->modifiers() == (Qt::ControlModifier | Qt::KeypadModifier)) ||
		(k->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier | Qt::KeypadModifier))
	   )
	{
		switch (k->key())
		{
			case Qt::Key_K:
				moveCursor(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
				textCursor().removeSelectedText();
				break;
			case Qt::Key_D:
				moveCursor(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
				textCursor().removeSelectedText();
				break;
			case Qt::Key_H:
				moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
				textCursor().removeSelectedText();
				break;
			/*case Qt::Key_X:
				cut();
				return;
				break;
			case Qt::Key_V:
				paste();
				return;
				break;*/
			case Qt::Key_Y:
			case Qt::Key_Z:
				emit SideBarUp(true);
				return;
				break;
		}
	}
	if ((k->modifiers() == Qt::NoModifier) ||
		(k->modifiers() == Qt::KeypadModifier) ||
		(k->modifiers() == Qt::ShiftModifier) ||
		(k->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) ||
		(k->modifiers() == (Qt::ControlModifier | Qt::AltModifier | Qt::ShiftModifier)) // Shift + AltGr on Windows for polish characters
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
					insertPlainText(QString(QChar(conv)));
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
			case Qt::Key_Escape:
				k->ignore();
				break;
			case Qt::Key_Shift:
			case Qt::Key_Control:
			case Qt::Key_Alt:
				wasMod = true;
				break;
			case Qt::Key_Return:
			case Qt::Key_Enter:
				if (k->modifiers() == Qt::ShiftModifier)
					insertChars(SpecialChars::LINEBREAK, k->text());
				else
					insertChars(SpecialChars::PARSEP, k->text());
				emit SideBarUp(true);
				emit SideBarUpdate();
				return;
				break;
			case Qt::Key_Delete:
			case Qt::Key_Backspace:
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
					QTextEdit::keyPressEvent(k);
					emit SideBarUp(true);
					emit SideBarUpdate();
				}
				return;
				break;
		}
	}
	QTextEdit::keyPressEvent(k);
	emit SideBarUp(true);
	emit SideBarUpdate();
}

void SEditor::handleContentsChange(int position, int charsRemoved, int charsAdded)
{
	// As of Qt 4.7.4, Cococa-QTextEdit output of input method is broken.
	// We need a workaround to avoit the bug.
	if(SuspendContentsChange != 0)
		return;
	if (blockContentsChangeHook <= 0)
	{
		if (charsRemoved > 0 && StyledText.length() > 0)
			StyledText.removeChars(position, charsRemoved);
		if (charsAdded > 0)
		{
			QTextCursor cursor = textCursor();
			cursor.setPosition(position);
			cursor.setPosition(position + charsAdded, QTextCursor::KeepAnchor);
			QString addedChars = cursor.selectedText();
			if (addedChars.length() > 0)
				StyledText.insertChars(position, addedChars, true);
			//qDebug("handleContentsChange : - %01d, + %01d, len %01d", charsRemoved, charsAdded, addedChars.length());
		}	
	}
}

void SEditor::focusOutEvent(QFocusEvent *e)
{
	QTextCursor tc(textCursor());
	if (tc.hasSelection())
	{
		QPair<int, int> selPair = qMakePair(tc.selectionStart(), tc.selectionEnd());
		SelStack.push(selPair);
	}
	else
	{
		QPair<int, int> selPair = qMakePair(tc.position(), -1);
		SelStack.push(selPair);
	}
	QTextEdit::focusOutEvent(e);
}

void SEditor::focusInEvent(QFocusEvent *e)
{
	if (SelStack.count() > 0)
	{
		QTextCursor tc(textCursor());
		QPair<int, int> selPair = SelStack.pop();
		tc.setPosition(qMin(selPair.first, StyledText.length()));
		if (selPair.second >= 0)
			tc.setPosition(selPair.second, QTextCursor::KeepAnchor);
		setTextCursor(tc);
	}
	QTextEdit::focusInEvent(e);
}

void SEditor::insertChars(const QString& text)
{
	if (textCursor().hasSelection())
		textCursor().removeSelectedText();
	++blockContentsChangeHook;
	QTextCursor c(textCursor());
	int pos = qMin(c.position(), StyledText.length());
	StyledText.insertChars(pos, text, true);
// 	insertPlainText(text);
	insertUpdate(pos, text.length());
	c.setPosition(pos + text.length());
	setTextCursor(c);
	setColor(false); // HACK to force normal edit color
	--blockContentsChangeHook;
}

void SEditor::insertChars(const QString& styledText, const QString& editText)
{
	if ((styledText.length() == editText.length()) && !styledText.isEmpty())
	{
		if (textCursor().hasSelection())
			textCursor().removeSelectedText();

		++blockContentsChangeHook;
		QTextCursor cursor(textCursor());
		int pos = qMin(cursor.position(), StyledText.length());
		StyledText.insertChars(pos, styledText, true);
 		insertPlainText(editText);
		cursor.setPosition(pos + editText.length());
		setTextCursor(cursor);
		--blockContentsChangeHook;
	}
}

void SEditor::insertCharsInternal(const QString& t)
{
	if (textCursor().hasSelection())
		deleteSel();
	int pos = textCursor().hasSelection() ? textCursor().selectionStart() : textCursor().position();
	pos = qMin(pos, StyledText.length());
	insertCharsInternal(t, pos);
}

void SEditor::insertCharsInternal(const QString& t, int pos)
{
	if (textCursor().hasSelection())
		textCursor().removeSelectedText();
	int oldLength = StyledText.length();
	StyledText.insertChars(pos, t, true);
	int newLength = StyledText.length();
	insertUpdate(pos, newLength - oldLength);
}

void SEditor::insertStyledText(const StoryText& styledText)
{
	if (styledText.length() == 0)
		return;
	int pos = textCursor().hasSelection() ? textCursor().selectionStart() : textCursor().position();
	pos = qMin(pos, StyledText.length());
	insertStyledText(styledText, pos);
}

void SEditor::insertStyledText(const StoryText& styledText, int pos)
{
	if (styledText.length() == 0)
		return;
	if (textCursor().hasSelection())
		textCursor().removeSelectedText();
	int oldLength = StyledText.length();
	StyledText.insert(pos, styledText);
	int newLength = StyledText.length();
	insertUpdate(pos, newLength - oldLength);
}

void SEditor::saveItemText(PageItem *currItem)
{
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
	QTextCursor tCursor = this->textCursor();
	setAlign(tCursor, align);
}

void SEditor::setAlign(QTextCursor& tCursor, int align)
{
	++blockContentsChangeHook;
	QTextBlockFormat blockFormat;
	switch (align)
	{
	case 0:
		blockFormat.setAlignment(Qt::AlignLeft);
		break;
	case 1:
		blockFormat.setAlignment(Qt::AlignCenter);
		break;
	case 2:
		blockFormat.setAlignment(Qt::AlignRight);
		break;
	case 3:
	case 4:
		blockFormat.setAlignment(Qt::AlignJustify);
		break;
	default:
		break;
	}
	tCursor.setBlockFormat(blockFormat);
	--blockContentsChangeHook;
}


void SEditor::loadItemText(PageItem *currItem)
{
	setTextColor(Qt::black);
	StyledText.clear();
	FrameItems.clear();
	StyledText.setDefaultStyle(currItem->itemText.defaultStyle());
	StyledText.append(currItem->itemText);
	updateAll();
	int npars = currItem->itemText.nrOfParagraphs();
	int newSelParaStart = 0;
	while (currItem->itemText.cursorPosition() >= (SelCharStart = currItem->itemText.endOfParagraph(newSelParaStart)) && newSelParaStart < npars)
		++newSelParaStart;
	if (currItem->itemText.cursorPosition() < SelCharStart)
		SelCharStart = currItem->itemText.cursorPosition();
	SelCharStart -= currItem->itemText.startOfParagraph(newSelParaStart);
	if (SelStack.count())
		SelStack.top().second = -1;
	//qDebug() << "SE::loadItemText: cursor";
//	setCursorPosition(SelParaStart, SelCharStart);
	emit setProps(newSelParaStart, SelCharStart);
	//SelParaStart = 0;
}

void SEditor::loadText(QString tx, PageItem *currItem)
{
	setTextColor(Qt::black);
	setUpdatesEnabled(false);
	QString Text = "";
	StyledText.clear();
	StyledText.setDefaultStyle(currItem->itemText.defaultStyle());
	StyledText.insertChars(0, tx);
	updateAll();
	if (StyledText.length() != 0)
		emit setProps(0, 0);
	//qDebug() << "SE::loadText: cursor";
	textCursor().setPosition(0);
}

void SEditor::updateAll()
{
	++blockContentsChangeHook;
	clear();
	insertUpdate(0, StyledText.length());
	--blockContentsChangeHook;
}

void SEditor::insertUpdate(int position, int len)
{
	if (StyledText.length() == 0 || len == 0)
		return;
	QString chars, text = "";
	++blockContentsChangeHook;
	setUpdatesEnabled(false);
	this->blockSignals(true);
	//prevent layout of QTextDocument while updating
	this->textCursor().beginEditBlock();
	int cursorPos = textCursor().position();
	int scrollPos = verticalScrollBar()->value();
	int end  = qMin(StyledText.length(), position + len);
	int cSty = StyledText.charStyle(position).effects();
	int pAli = StyledText.paragraphStyle(position).alignment();
	setAlign(pAli);
	setEffects(cSty);
	for (int pos = position; pos < end; ++pos)
	{
		const CharStyle& cstyle(StyledText.charStyle(pos));
		const QChar ch = StyledText.text(pos);
		if (ch == SpecialChars::PARSEP)
		{
			text += "\n";
			const ParagraphStyle& pstyle(StyledText.paragraphStyle(pos));
			pAli = pstyle.alignment();
			setAlign(pAli);
			setEffects(cSty);
			insertPlainText(text);
			cSty = cstyle.effects();
			text = "";
			continue;
		}
		if (cSty != cstyle.effects() ||
				ch == SpecialChars::OBJECT ||
				ch == SpecialChars::PAGENUMBER ||
				ch == SpecialChars::PAGECOUNT ||
				ch == SpecialChars::NBSPACE ||
				ch == SpecialChars::FRAMEBREAK ||
				ch == SpecialChars::COLBREAK ||
				ch == SpecialChars::NBHYPHEN ||
				ch == SpecialChars::LINEBREAK)
		{
			setAlign(pAli);
			setEffects(cSty);
			insertPlainText(text);
			cSty = cstyle.effects();
			text = "";
		}
		if (ch == SpecialChars::OBJECT)
		{
			setColor(true);
			insertPlainText("@");
			setColor(false);
		}
		else if (ch == SpecialChars::PAGENUMBER)
		{
			setColor(true);
			insertPlainText("#");
			setColor(false);
		}
		else if (ch == SpecialChars::PAGECOUNT)
		{
			setColor(true);
			insertPlainText("%");
			setColor(false);
		}
		else if (ch == SpecialChars::NBSPACE)
		{
			setColor(true);
			insertPlainText("_");
			setColor(false);
		}
		else if (ch == SpecialChars::FRAMEBREAK)
		{
			setColor(true);
			insertPlainText("|");
			setColor(false);
		}
		else if (ch == SpecialChars::COLBREAK)
		{
			setColor(true);
			insertPlainText("^");
			setColor(false);
		}
		else if (ch == SpecialChars::NBHYPHEN)
		{
			setColor(true);
			insertPlainText("=");
			setColor(false);
		}
		else if (ch == SpecialChars::LINEBREAK)
		{
			setColor(true);
			insertPlainText("*");
			setColor(false);
		}
		else
			text += ch;
	}
	if (position < end)
	{
		const ParagraphStyle& pstyle(StyledText.paragraphStyle(end - 1));
		setAlign(pstyle.alignment());
	}
	setEffects(cSty);
	insertPlainText(text);
	QTextCursor tCursor = textCursor();
	tCursor.setPosition(cursorPos);
	setTextCursor(tCursor);
	verticalScrollBar()->setValue(scrollPos);
	this->textCursor().endEditBlock();
	this->blockSignals(false);
	setUpdatesEnabled(true);
	--blockContentsChangeHook;
	emit textChanged();
	//CB Removed to fix 2083 setCursorPosition(p, i);	
}


void SEditor::updateFromChars(int pa)
{
	int start = StyledText.startOfParagraph(pa);
	int end   = StyledText.endOfParagraph(pa);
	if (start >= end)
		return;
	setUpdatesEnabled(false);
	int SelStart = start;
	int SelEnd   = start;
	int pos = textCursor().position();
	textCursor().clearSelection();
	int effects = StyledText.charStyle(start).effects();
	for (int a = start; a < end; ++a)
	{
		if (effects == StyledText.charStyle(a).effects())
			SelEnd++;
		else
		{
			textCursor().setPosition(SelStart);
			textCursor().setPosition(SelEnd, QTextCursor::KeepAnchor);
			setEffects(effects);
			textCursor().clearSelection();
			effects = StyledText.charStyle(a).effects();
			SelStart = SelEnd;
			SelEnd++;
		}
	}
	QTextCursor tCursor = textCursor();
	tCursor.setPosition(SelStart);
	tCursor.setPosition(SelEnd, QTextCursor::KeepAnchor);
	setEffects(tCursor, effects);
	setAlign(tCursor, StyledText.paragraphStyle(start).alignment());
	tCursor.clearSelection();
	setUpdatesEnabled(true);
	tCursor = textCursor();
	tCursor.setPosition(pos);
	setTextCursor(tCursor);
}

/* updates the internal StyledText structure, applies 'newStyle' to the selection */
void SEditor::updateSel(const ParagraphStyle& newStyle)
{
	int PStart, PEnd, SelStart, SelEnd, start;
	if (SelStack.count())
	{
		QTextCursor tc(textCursor());
		QPair<int, int> selPair = SelStack.pop();
		if (selPair.second >= 0)
		{
			tc.setPosition(selPair.first);
			tc.setPosition(selPair.second, QTextCursor::KeepAnchor);
			setTextCursor(tc);
		}
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
	if (SelStack.count())
	{
		QTextCursor tc(textCursor());
		QPair<int, int> selPair = SelStack.pop();
		if (selPair.second >= 0)
		{
			tc.setPosition(selPair.first);
			tc.setPosition(selPair.second, QTextCursor::KeepAnchor);
			setTextCursor(tc);
		}
	}
	int start = textCursor().selectionStart();
	int end = textCursor().selectionEnd();
	if (start >= 0 && start < end)
		StyledText.applyCharStyle(start, end-start, newStyle);
}


void SEditor::deleteSel()
{
	int start = textCursor().selectionStart();
	int end   = textCursor().selectionEnd();
	if (end > start)
		StyledText.removeChars(start, end-start);
	textCursor().setPosition(start);
	SelStack.clear();
}

void SEditor::setEffects(int effects)
{
	QTextCursor tCursor = textCursor();
	setEffects(tCursor, effects);
	setTextCursor(tCursor);
}

void SEditor::setEffects(QTextCursor& tCursor, int effects)
{
	++blockContentsChangeHook;
	QTextCharFormat charF;
	if (effects & 8)
		charF.setFontUnderline(true);
	else
		charF.setFontUnderline(false);
	if (effects & 16)
		charF.setFontStrikeOut(true);
	else
		charF.setFontStrikeOut(false);
	if (effects & 1)
		charF.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
	else if (effects & 2)
		charF.setVerticalAlignment(QTextCharFormat::AlignSubScript);
	else
		charF.setVerticalAlignment(QTextCharFormat::AlignNormal);
	tCursor.setCharFormat(charF);
	--blockContentsChangeHook;
}

void SEditor::setColor(bool marker)
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
	emit SideBarUp(false);
	if ((textCursor().hasSelection()) && (!textCursor().selectedText().isEmpty()))
	{
		disconnect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
		QMimeData* mimeData = createMimeDataFromSelection();
		QApplication::clipboard()->setMimeData(mimeData, QClipboard::Clipboard);
		connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(ClipChange()));
		emit PasteAvail();
	}
	emit SideBarUp(true);
}

void SEditor::cut()
{
	copy();
	emit SideBarUp(false);
	if (textCursor().hasSelection())
		textCursor().removeSelectedText();
	emit SideBarUp(true);
	emit SideBarUpdate();
}

void SEditor::paste()
{
	emit SideBarUp(false);
	bool useMimeStyledText = false;
	// int  newParaCount, lengthLastPara,
	int advanceLen = 0;
	int  pos = textCursor().hasSelection() ? textCursor().selectionStart() : textCursor().position();
	const QMimeData* mimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);
	if (mimeData->hasFormat("application/x-scribus-styledtext"))
	{
		const StyledTextMimeData* styledData = dynamic_cast<const StyledTextMimeData*>(mimeData);
		if (styledData)
			useMimeStyledText = (styledData->document() == doc);
	}
	if (useMimeStyledText)
	{
		const StyledTextMimeData* styledData = dynamic_cast<const StyledTextMimeData*>(mimeData);
		if (styledData)
		{
			const StoryText& styledText = styledData->styledText();
			advanceLen = styledText.length();
			insertStyledText(styledText, pos);
		}
	}
	else
	{
		QString data = QApplication::clipboard()->text(QClipboard::Clipboard);
		if (!data.isEmpty())
		{
			data.replace(QRegExp("\r"), "");
		//	newParaCount=data.count("\n");
		//	lengthLastPara=data.length()-data.lastIndexOf("\n");
			data.replace(QRegExp("\n"), SpecialChars::PARSEP);
//			inserted=true;
			advanceLen = data.length() /*- newParaCount*/;
			insertCharsInternal(data, pos);
			emit PasteAvail();
		}
		else
		{
			emit SideBarUp(true);
			return;
		}
	}
	setUpdatesEnabled(false);
	//qDebug() << "SE::paste: cursor";
//	setCursorPosition(currentPara, currentCharPos);
	QTextCursor tCursor = textCursor();
	tCursor.setPosition(pos + advanceLen);
	setTextCursor(tCursor);
	/*for (int a = 0; a < advanceLen; ++a)
	{
		moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
	}*/
	setUpdatesEnabled(true);
//	if (inserted)
//		setCursorPosition(currentPara+newParaCount,(newParaCount==0?currentCharPos:0)+lengthLastPara-1);
	repaint();
	emit SideBarUp(true);
	emit SideBarUpdate();
}

bool SEditor::canInsertFromMimeData( const QMimeData * source ) const
{
	if (source->hasText() || source->hasFormat("application/x-scribus-styledtext"))
		return true;
	return false;
}

QMimeData* SEditor::createMimeDataFromSelection () const
{
	StyledTextMimeData* mimeData = new StyledTextMimeData();
	int start = textCursor().selectionStart();
	int end   = textCursor().selectionEnd();
	if (start < 0 || end <= start)
		return mimeData;
	StoryText* that = const_cast<StoryText*> (&StyledText);
	that->select(start, end-start);
	QString selectedText = textCursor().selectedText();
	selectedText.replace(QChar(0x2029), QChar('\n'));
	mimeData->setText(selectedText);
	mimeData->setStyledText(*that, doc);
	return mimeData;
}

void SEditor::insertFromMimeData ( const QMimeData * source )
{
	paste();
}

void SEditor::SelClipChange()
{
	emit PasteAvail();
}

void SEditor::ClipChange()
{
	emit PasteAvail();
}

void SEditor::scrollContentsBy(int dx, int dy)
{
	emit contentsMoving(dx, dy);
	QTextEdit::scrollContentsBy(dx, dy);
}

/* Toolbar for Fill Colour */
SToolBColorF::SToolBColorF(QMainWindow* parent, ScribusDoc *doc) : QToolBar( tr("Fill Color Settings"), parent)
{
	FillIcon = new QLabel(this);
	FillIcon->setPixmap(IconManager::instance()->loadPixmap("16/color-fill.png"));
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

void SToolBColorF::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
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
	StrokeIcon->setPixmap(IconManager::instance()->loadPixmap("16/color-stroke.png"));
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

void SToolBColorS::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
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
	trackingLabel->setPixmap(IconManager::instance()->loadPixmap("textkern.png"));
	trackingLabelAction=addWidget(trackingLabel);
	trackingLabelAction->setVisible(true);
	Extra = new ScrSpinBox( this, SC_PERCENT );
	Extra->setValues( -300, 300, 2, 0);
	Extra->setSuffix( unitGetSuffixFromIndex(SC_PERCENT) );
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

void SToolBStyle::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
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

void SToolBAlign::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
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
	Size = new ScrSpinBox( 0.5, 2048, this, SC_POINTS );
	PrefsManager* prefsManager = PrefsManager::instance();
	Size->setSuffix( unitGetSuffixFromIndex(SC_POINTS) );
	Size->setValue(prefsManager->appPrefs.itemToolPrefs.textSize / 10.0);
	sizeAction=addWidget(Size);
	sizeAction->setVisible(true);
	ScaleTxt = new QLabel("", this);
	ScaleTxt->setPixmap(IconManager::instance()->loadPixmap("textscaleh.png"));
	scaleTxtAction=addWidget(ScaleTxt);
	scaleTxtAction->setVisible(true);
	ChScale = new ScrSpinBox( 10, 400,  this, SC_PERCENT );
	ChScale->setValue( 100 );
	ChScale->setSuffix( unitGetSuffixFromIndex(SC_PERCENT) );
	chScaleAction=addWidget(ChScale);
	chScaleAction->setVisible(true);
	ScaleTxtV = new QLabel("", this);
	ScaleTxtV->setPixmap(IconManager::instance()->loadPixmap("textscalev.png"));
	scaleTxtVAction=addWidget(ScaleTxtV);
	scaleTxtVAction->setVisible(true);
	ChScaleV = new ScrSpinBox( 10, 400, this, SC_PERCENT );
	ChScaleV->setValue( 100 );
	ChScaleV->setSuffix( unitGetSuffixFromIndex(SC_PERCENT) );
	chScaleVAction=addWidget(ChScaleV);
	chScaleVAction->setVisible(true);

	connect(ChScale, SIGNAL(valueChanged(double)), this, SIGNAL(newScale(double)));
	connect(ChScaleV, SIGNAL(valueChanged(double)), this, SIGNAL(newScaleV(double)));
	connect(Fonts, SIGNAL(activated(const QString &)), this, SIGNAL(NewFont(const QString &)));
	connect(Size, SIGNAL(valueChanged(double)), this, SIGNAL(NewSize(double)));
}

void SToolBFont::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
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
// 	m_doc = docc;
// 	seMenuMgr=NULL;
// 	buildGUI();
// 	currItem = ite;
// // 	charSelect = NULL;
// 	m_firstSet = false;
// 	activFromApp = true;
// 	Editor->loadItemText(ite);
// 	Editor->getCursorPosition(&m_currPara, &m_currChar);
// 	EditorBar->setRepaint(true);
// 	EditorBar->doRepaint();
// 	updateProps(m_currPara, m_currChar);
// 	updateStatus();
// 	m_textChanged = false;
// 	disconnectSignals();
// 	connectSignals();
// 	Editor->setFocus();
// 	Editor->setColor(false);
// 	m_blockUpdate = false;
// 	loadPrefs();
// 	// hack to keep charPalette visible. See destructor too - PV
// 	ScCore->primaryMainWindow()->charPalette->reparent(this, QPoint(0, 0));
// }

/* Main Story Editor Class, no current document */
StoryEditor::StoryEditor(QWidget* parent) : QMainWindow(parent, Qt::Window), // WType_Dialog) //WShowModal |
	activFromApp(true),
	m_doc(NULL),
	m_item(NULL),
	m_textChanged(false),
	m_firstSet(false),
	m_blockUpdate(false),
//	m_currPara(0),
//	m_currChar(0),
	charSelect(NULL),
	charSelectUsed(false)
{
	m_spellActive=false;
	prefsManager=PrefsManager::instance();
#ifdef Q_OS_MAC
	noIcon = IconManager::instance()->loadPixmap("noicon.xpm");
#endif
	buildGUI();
	/*
	//Editor->loadItemText(ite);
	updateProps(0,0);
	updateStatus();
	*/
	Editor->setFocus();
	Editor->setColor(false);
	loadPrefs();
}

StoryEditor::~StoryEditor()
{
	savePrefs();
}

void StoryEditor::showEvent(QShowEvent *)
{
	loadPrefs();
	charSelect = new CharSelect(this);
	charSelect->userTableModel()->setCharactersAndFonts(ScCore->primaryMainWindow()->charPalette->userTableModel()->characters(), ScCore->primaryMainWindow()->charPalette->userTableModel()->fonts());
	connect(charSelect, SIGNAL(insertSpecialChar()), this, SLOT(slot_insertSpecialChar()));
	connect(charSelect, SIGNAL(insertUserSpecialChar(QChar, QString)), this, SLOT(slot_insertUserSpecialChar(QChar, QString)));

	m_smartSelection = prefsManager->appPrefs.storyEditorPrefs.smartTextSelection;
	seActions["settingsSmartTextSelection"]->setChecked(m_smartSelection);
}

void StoryEditor::hideEvent(QHideEvent *)
{
	if (charSelect)
	{
		if (charSelectUsed)
			ScCore->primaryMainWindow()->charPalette->userTableModel()->setCharactersAndFonts(charSelect->userTableModel()->characters(), charSelect->userTableModel()->fonts());
		if (charSelect->isVisible())
			charSelect->close();
		disconnect(charSelect, SIGNAL(insertSpecialChar()),
					this, SLOT(slot_insertSpecialChar()));
		disconnect(charSelect, SIGNAL(insertUserSpecialChar(QChar, QString)),
					this, SLOT(slot_insertUserSpecialChar(QChar, QString)));
		delete charSelect;
		charSelect = NULL;
	}
	savePrefs();
}

void StoryEditor::savePrefs()
{
	// save prefs
	QRect geo = geometry();
	prefs->set("left", geo.left());
	prefs->set("top", geo.top());
	prefs->set("width", width());
	prefs->set("height", height());
	QList<int> splitted = EdSplit->sizes();
	prefs->set("side", splitted[0]);
	prefs->set("main", splitted[1]);
	prefs->set("winstate", QString(saveState().toBase64()));
}

void StoryEditor::loadPrefs()
{
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("StoryEditor");
	int vleft   = qMax(-80, prefs->getInt("left", 10));
#if defined(Q_OS_MAC) || defined(_WIN32)
	int vtop	= qMax(64, prefs->getInt("top", 10));
#else
	int vtop	= qMax(-80, prefs->getInt("top", 10));
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
	state = prefs->get("winstate","").toLatin1();
	if (!state.isEmpty())
		restoreState(QByteArray::fromBase64(state));
	int side = prefs->getInt("side", -1);
	int txtarea = prefs->getInt("main", -1);
	if ((side != -1) && (txtarea != -1))
	{
		QList<int> splitted;
		splitted.append(side);
		splitted.append(txtarea);
		EdSplit->setSizes(splitted);
	}
	setupEditorGUI();
}

void StoryEditor::initActions()
{
	IconManager* im = IconManager::instance();
	//File Menu
	seActions.insert("fileNew", new ScrAction(im->loadPixmap("16/document-new.png"), im->loadPixmap("22/document-new.png"), "", Qt::CTRL+Qt::Key_N, this));
	seActions.insert("fileRevert", new ScrAction(im->loadPixmap("reload16.png"), im->loadPixmap("reload.png"), "", QKeySequence(), this));
	seActions.insert("fileSaveToFile", new ScrAction(im->loadPixmap("16/document-save.png"), im->loadPixmap("22/document-save.png"), "", QKeySequence(), this));
	seActions.insert("fileLoadFromFile", new ScrAction(im->loadPixmap("16/document-open.png"),  im->loadPixmap("22/document-open.png"), "", QKeySequence(), this));
	seActions.insert("fileSaveDocument", new ScrAction("", Qt::CTRL+Qt::Key_S, this));
	seActions.insert("fileUpdateAndExit", new ScrAction(im->loadPixmap("ok.png"), im->loadPixmap("ok22.png"), "", Qt::CTRL+Qt::Key_W,  this));
	seActions.insert("fileExit", new ScrAction(im->loadPixmap("exit.png"), im->loadPixmap("exit22.png"), "", QKeySequence(),  this));

	connect( seActions["fileNew"], SIGNAL(triggered()), this, SLOT(Do_new()) );
	connect( seActions["fileRevert"], SIGNAL(triggered()), this, SLOT(slotFileRevert()) );
	connect( seActions["fileSaveToFile"], SIGNAL(triggered()), this, SLOT(SaveTextFile()) );
	connect( seActions["fileLoadFromFile"], SIGNAL(triggered()), this, SLOT(LoadTextFile()) );
	connect( seActions["fileSaveDocument"], SIGNAL(triggered()), this, SLOT(Do_saveDocument()) );
	connect( seActions["fileUpdateAndExit"], SIGNAL(triggered()), this, SLOT(Do_leave2()) );
	connect( seActions["fileExit"], SIGNAL(triggered()), this, SLOT(Do_leave()) );

	//Edit Menu
	seActions.insert("editCut", new ScrAction(im->loadPixmap("16/edit-cut.png"), QPixmap(), "", Qt::CTRL+Qt::Key_X, this));
	seActions.insert("editCopy", new ScrAction(im->loadPixmap("16/edit-copy.png"), QPixmap(), "", Qt::CTRL+Qt::Key_C, this));
	seActions.insert("editPaste", new ScrAction(im->loadPixmap("16/edit-paste.png"), QPixmap(), "", Qt::CTRL+Qt::Key_V, this));
	seActions.insert("editClear", new ScrAction(im->loadPixmap("16/edit-delete.png"), QPixmap(), "", Qt::Key_Delete, this));
	seActions.insert("editSelectAll", new ScrAction(im->loadPixmap("16/edit-select-all.png"), QPixmap(), "", Qt::CTRL+Qt::Key_A, this));
	seActions.insert("editSearchReplace", new ScrAction(im->loadPixmap("16/edit-find-replace.png"), QPixmap(), "", Qt::CTRL+Qt::Key_F, this));
	//seActions.insert("editEditStyle", new ScrAction("", QKeySequence(), this));
	seActions.insert("editFontPreview", new ScrAction("", QKeySequence(), this));
	seActions.insert("editUpdateFrame", new ScrAction(im->loadPixmap("compfile16.png"),im->loadPixmap("compfile.png"), "", Qt::CTRL+Qt::Key_U, this));

	connect( seActions["editCut"], SIGNAL(triggered()), this, SLOT(Do_cut()) );
	connect( seActions["editCopy"], SIGNAL(triggered()), this, SLOT(Do_copy()) );
	connect( seActions["editPaste"], SIGNAL(triggered()), this, SLOT(Do_paste()) );
	connect( seActions["editClear"], SIGNAL(triggered()), this, SLOT(Do_del()) );
	connect( seActions["editSelectAll"], SIGNAL(triggered()), this, SLOT(Do_selectAll()) );
	connect( seActions["editSearchReplace"], SIGNAL(triggered()), this, SLOT(SearchText()) );
	//connect( seActions["editEditStyle"], SIGNAL(triggered()), this, SLOT(slotEditStyles()) );
	connect( seActions["editFontPreview"], SIGNAL(triggered()), this, SLOT(Do_fontPrev()) );
	connect( seActions["editUpdateFrame"], SIGNAL(triggered()), this, SLOT(updateTextFrame()) );

	//Insert Menu
	seActions.insert("insertGlyph", new ScrAction(QPixmap(), QPixmap(), "", QKeySequence(), this));
	connect( seActions["insertGlyph"], SIGNAL(triggered()), this, SLOT(Do_insSp()) );
	seActions.insert("insertSampleText", new ScrAction(QPixmap(), QPixmap(), "", QKeySequence(), this));
	connect(seActions["insertSampleText"], SIGNAL(triggered()), this, SLOT(insertSampleText()));

	//Settings Menu
	seActions.insert("settingsBackground", new ScrAction("", QKeySequence(), this));
	seActions.insert("settingsDisplayFont", new ScrAction("", QKeySequence(), this));
	seActions.insert("settingsSmartTextSelection", new ScrAction("", QKeySequence(), this));
	seActions["settingsSmartTextSelection"]->setChecked(m_smartSelection);
	seActions["settingsSmartTextSelection"]->setToggleAction(true);

	connect( seActions["settingsBackground"], SIGNAL(triggered()), this, SLOT(setBackPref()) );
	connect( seActions["settingsDisplayFont"], SIGNAL(triggered()), this, SLOT(setFontPref()) );
	connect( seActions["settingsSmartTextSelection"], SIGNAL(toggled(bool)), this, SLOT(setSmart(bool)) );


//	seActions["fileRevert"]->setEnabled(false);
//	seActions["editCopy"]->setEnabled(false);
//	seActions["editCut"]->setEnabled(false);
//	seActions["editPaste"]->setEnabled(false);
//	seActions["editClear"]->setEnabled(false);
//	seActions["editUpdateFrame"]->setEnabled(false);
}

void StoryEditor::buildMenus()
{
	seMenuMgr = new MenuManager(this->menuBar(), this->menuBar());
	seMenuMgr->createMenu("File", tr("&File"));
	seMenuMgr->addMenuItemString("fileNew", "File");
	seMenuMgr->addMenuItemString("fileRevert", "File");
	seMenuMgr->addMenuItemString("SEPARATOR", "File");
	seMenuMgr->addMenuItemString("fileSaveToFile", "File");
	seMenuMgr->addMenuItemString("fileLoadFromFile", "File");
	seMenuMgr->addMenuItemString("fileSaveDocument", "File");
	seMenuMgr->addMenuItemString("SEPARATOR", "File");
	seMenuMgr->addMenuItemString("fileUpdateAndExit", "File");
	seMenuMgr->addMenuItemString("fileExit", "File");
	seMenuMgr->createMenu("Edit", tr("&Edit"));
	seMenuMgr->addMenuItemString("editCut", "Edit");
	seMenuMgr->addMenuItemString("editCopy", "Edit");
	seMenuMgr->addMenuItemString("editPaste", "Edit");
	seMenuMgr->addMenuItemString("editClear", "Edit");
	seMenuMgr->addMenuItemString("SEPARATOR", "Edit");
	seMenuMgr->addMenuItemString("editSelectAll", "Edit");
	seMenuMgr->addMenuItemString("SEPARATOR", "Edit");
	seMenuMgr->addMenuItemString("editSearchReplace", "Edit");
	seMenuMgr->addMenuItemString("SEPARATOR", "Edit");
//	seMenuMgr->addMenuItemString("editEditStyle", "Edit");
	seMenuMgr->addMenuItemString("editFontPreview", "Edit");
	seMenuMgr->addMenuItemString("editUpdateFrame", "Edit");
	seMenuMgr->addMenuItemString("SEPARATOR", "Edit");
	seMenuMgr->addMenuItemString("settingsSmartTextSelection", "Edit");
	seMenuMgr->createMenu("Insert", tr("&Insert"));
	seMenuMgr->addMenuItemString("insertGlyph", "Insert");
	seMenuMgr->createMenu("InsertChar", tr("Character"), "Insert");
	seMenuMgr->addMenuItemString("InsertChar", "Insert");
	seMenuMgr->addMenuItemString("unicodePageNumber", "InsertChar");
	seMenuMgr->addMenuItemString("unicodePageCount", "InsertChar");
	//seMenuMgr->addMenuItemString("unicodeSoftHyphen", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeNonBreakingHyphen", "InsertChar");
	seMenuMgr->addMenuItemString("SEPARATOR", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeCopyRight", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeRegdTM", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeTM", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeSolidus", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeBullet", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeMidpoint", "InsertChar");
	seMenuMgr->addMenuItemString("SEPARATOR", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeDashEm", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeDashEn", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeDashFigure", "InsertChar");
	seMenuMgr->addMenuItemString("unicodeDashQuotation", "InsertChar");
	seMenuMgr->createMenu("InsertQuote", tr("Quote"), "Insert");
	seMenuMgr->addMenuItemString("InsertQuote", "Insert");
	seMenuMgr->addMenuItemString("unicodeQuoteApostrophe", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteStraight", "InsertQuote");
	seMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteSingleLeft", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteSingleRight", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteDoubleLeft", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteDoubleRight", "InsertQuote");
	seMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteSingleReversed", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteDoubleReversed", "InsertQuote");
	seMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteLowSingleComma", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteLowDoubleComma", "InsertQuote");
	seMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteSingleLeftGuillemet", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteSingleRightGuillemet", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteDoubleLeftGuillemet", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteDoubleRightGuillemet", "InsertQuote");
	seMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteCJKSingleLeft", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteCJKSingleRight", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteCJKDoubleLeft", "InsertQuote");
	seMenuMgr->addMenuItemString("unicodeQuoteCJKDoubleRight", "InsertQuote");
	seMenuMgr->createMenu("InsertSpace", tr("Spaces && Breaks"), "Insert");
	seMenuMgr->addMenuItemString("InsertSpace", "Insert");
	seMenuMgr->addMenuItemString("unicodeNonBreakingSpace", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeSpaceEN", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeSpaceEM", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeSpaceThin", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeSpaceThick", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeSpaceMid", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeSpaceHair", "InsertSpace");
	seMenuMgr->addMenuItemString("SEPARATOR", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeNewLine", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeFrameBreak", "InsertSpace");
	seMenuMgr->addMenuItemString("unicodeColumnBreak", "InsertSpace");
	seMenuMgr->createMenu("InsertLigature", tr("Ligature"), "Insert");
	seMenuMgr->addMenuItemString("InsertLigature", "Insert");
	seMenuMgr->addMenuItemString("unicodeLigature_ff", "InsertLigature");
	seMenuMgr->addMenuItemString("unicodeLigature_fi", "InsertLigature");
	seMenuMgr->addMenuItemString("unicodeLigature_fl", "InsertLigature");
	seMenuMgr->addMenuItemString("unicodeLigature_ffi", "InsertLigature");
	seMenuMgr->addMenuItemString("unicodeLigature_ffl", "InsertLigature");
	seMenuMgr->addMenuItemString("unicodeLigature_ft", "InsertLigature");
	seMenuMgr->addMenuItemString("unicodeLigature_st", "InsertLigature");
	seMenuMgr->addMenuItemString("insertSampleText", "Insert");

	seMenuMgr->createMenu("Settings", tr("&Settings"));
	seMenuMgr->addMenuItemString("settingsBackground", "Settings");
	seMenuMgr->addMenuItemString("settingsDisplayFont", "Settings");
//	seMenuMgr->addMenuItemString("settingsSmartTextSelection", "Settings");

	seMenuMgr->addMenuStringToMenuBar("File");
	seMenuMgr->addMenuItemStringstoMenuBar("File", seActions);
	seMenuMgr->addMenuStringToMenuBar("Edit");
	seMenuMgr->addMenuItemStringstoMenuBar("Edit", seActions);
	seMenuMgr->addMenuStringToMenuBar("Insert");
	seMenuMgr->addMenuItemStringstoMenuBar("Insert", seActions);
	seMenuMgr->addMenuStringToMenuBar("Settings");
	seMenuMgr->addMenuItemStringstoMenuBar("Settings", seActions);
	
	PluginManager::instance().setupPluginActions(this);
	PluginManager::instance().languageChange();
}

void StoryEditor::buildGUI()
{
	unicodeCharActionNames.clear();
	seActions.clear();
	m_smartSelection = prefsManager->appPrefs.storyEditorPrefs.smartTextSelection;
	initActions();
	ActionManager::initUnicodeActions(&seActions, this, &unicodeCharActionNames);
	seActions["unicodeSoftHyphen"]->setEnabled(false);//CB TODO doesn't work in SE yet.
	buildMenus();

	setWindowIcon(IconManager::instance()->loadPixmap("AppIcon.png"));
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
	AlignTools->paraStyleCombo->setDoc(m_doc);
	StyleTools = new SToolBStyle(this);
	StyleTools->setIconSize(QSize(16,16));
	StyleTools->setObjectName("Style");
	StyleTools->setAllowedAreas(Qt::LeftToolBarArea);
	StyleTools->setAllowedAreas(Qt::RightToolBarArea);
	StyleTools->setAllowedAreas(Qt::BottomToolBarArea);
	StyleTools->setAllowedAreas(Qt::TopToolBarArea);
	StrokeTools = new SToolBColorS(this, m_doc);
	StrokeTools->setIconSize(QSize(16,16));
	StrokeTools->setObjectName("Strok");
	StrokeTools->setAllowedAreas(Qt::LeftToolBarArea);
	StrokeTools->setAllowedAreas(Qt::RightToolBarArea);
	StrokeTools->setAllowedAreas(Qt::BottomToolBarArea);
	StrokeTools->setAllowedAreas(Qt::TopToolBarArea);
	StrokeTools->TxStroke->setEnabled(false);
	StrokeTools->PM1->setEnabled(false);
	FillTools = new SToolBColorF(this, m_doc);
	FillTools->setIconSize(QSize(16,16));
	FillTools->setObjectName("Fill");
	FillTools->setAllowedAreas(Qt::LeftToolBarArea);
	FillTools->setAllowedAreas(Qt::RightToolBarArea);
	FillTools->setAllowedAreas(Qt::BottomToolBarArea);
	FillTools->setAllowedAreas(Qt::TopToolBarArea);
	
	addToolBar(FileTools);
	addToolBarBreak();
	addToolBar(FontTools);
	addToolBar(AlignTools);
	addToolBarBreak();
	addToolBar(StyleTools);
	addToolBar(StrokeTools);
	addToolBar(FillTools);

	EdSplit = new QSplitter(this);
/* SideBar Widget */
	EditorBar = new SideBar(this);
	EdSplit->addWidget(EditorBar);
/* Editor Widget, subclass of QTextEdit */
	Editor = new SEditor(this, m_doc, this);
	EdSplit->addWidget(Editor);
	StoryEd2Layout->addWidget( EdSplit );

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
	setCentralWidget( EdSplit );
	//Final setup
	resize( QSize(660, 500).expandedTo(minimumSizeHint()) );
	if (prefsManager==NULL)
		sDebug(QString("%1").arg("prefsmgr null"));

	EditorBar->editor = Editor;
	Editor->installEventFilter(this);
	languageChange();
	ActionManager::setActionTooltips(&seActions);
}

void StoryEditor::setupEditorGUI()
{
	QFont fo;
	fo.fromString(prefsManager->appPrefs.storyEditorPrefs.guiFont);
	Editor->setFont(fo);
	QPalette pal;
	QColor newColor(prefsManager->appPrefs.storyEditorPrefs.guiFontColorBackground);
	pal.setColor(QPalette::Active, QPalette::Base, newColor);
	pal.setColor(QPalette::Inactive, QPalette::Base, newColor);
	pal.setColor(QPalette::Disabled, QPalette::Base, newColor);
	Editor->setPalette(pal);
	EditorBar->setFrameStyle(Editor->frameStyle());
	EditorBar->setLineWidth(Editor->lineWidth());
}

void StoryEditor::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
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
	seActions["editCut"]->setTexts( tr("Cu&t"));
	seActions["editCopy"]->setTexts( tr("&Copy"));
	seActions["editPaste"]->setTexts( tr("&Paste"));
	seActions["editClear"]->setTexts( tr("C&lear"));
	seActions["editSelectAll"]->setTexts( tr("Select &All"));
	seActions["editSearchReplace"]->setTexts( tr("&Search/Replace..."));
//	seActions["editEditStyle"]->setTexts( tr("&Edit Styles..."));
	seActions["editFontPreview"]->setTexts( tr("&Fonts Preview..."));
	seActions["editUpdateFrame"]->setTexts( tr("&Update Text Frame"));

	//Insert menu
	seActions["insertGlyph"]->setTexts( tr("&Glyph..."));
	seMenuMgr->setText("Insert", tr("&Insert"));
	seMenuMgr->setText("InsertChar", tr("Character"));
	seMenuMgr->setText("InsertQuote", tr("Quote"));
	seMenuMgr->setText("InsertSpace", tr("Space"));
	seActions["insertSampleText"]->setTexts( tr("&Sample Text"));

	//Settings Menu
	seMenuMgr->setText("Settings", tr("&Settings"));
	seActions["settingsBackground"]->setTexts( tr("&Background..."));
	seActions["settingsDisplayFont"]->setTexts( tr("&Display Font..."));
	seActions["settingsSmartTextSelection"]->setTexts( tr("&Smart Text Selection"));

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
	disconnect(Editor->document(), 0,0,0);
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
//	connect(Editor, SIGNAL(clicked(int, int)), this, SLOT(updateProps(int, int)));
	connect(Editor, SIGNAL(setProps(int, int)), this, SLOT(updateProps(int, int)));
	connect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
	connect(Editor, SIGNAL(copyAvailable(bool)), this, SLOT(CopyAvail(bool )));
	connect(Editor, SIGNAL(PasteAvail()), this, SLOT(PasteAvail()));
	connect(Editor, SIGNAL(contentsMoving(int, int)), EditorBar, SLOT(doMove(int, int )));
	connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
	connect(Editor, SIGNAL(SideBarUp(bool )), EditorBar, SLOT(setRepaint(bool )));
	connect(Editor, SIGNAL(SideBarUpdate( )), EditorBar, SLOT(doRepaint()));
	connect(Editor->document(), SIGNAL(contentsChange(int, int, int)), Editor, SLOT(handleContentsChange(int, int, int)));
	Editor->SuspendContentsChange = 0;
	// 10/12/2004 - pv - #1203: wrong selection on double click
//	connect(Editor, SIGNAL(doubleClicked(int, int)), this, SLOT(doubleClick(int, int)));
	connect(EditorBar, SIGNAL(ChangeStyle(int, const QString& )), this, SLOT(changeStyleSB(int, const QString&)));
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
	m_doc=doc;
	m_textChanged=false;
	AlignTools->paraStyleCombo->setDoc(m_doc);
	FillTools->setCurrentDocument(m_doc);
	StrokeTools->setCurrentDocument(m_doc);
	Editor->setCurrentDocument(m_doc);
	StyleTools->SetStyle(0);
	m_item = item;
	if (m_item != NULL)
	{
		setWindowTitle( tr("Story Editor - %1").arg(m_item->itemName()));
		m_firstSet = false;
		FontTools->Fonts->RebuildList(m_doc, m_item->isAnnotation());
		Editor->loadItemText(m_item);
		Editor->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
		Editor->repaint();
		EditorBar->offs = 0;
//		Editor->textCursor().setPosition(0);
//		Editor->ensureCursorVisible();
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
		setWindowTitle( tr( "Story Editor" ));
	}
	QString data = QApplication::clipboard()->text(QClipboard::Clipboard);
	if (!data.isNull())
		seActions["editPaste"]->setEnabled(true);
}

void StoryEditor::setSpellActive(bool ssa)
{
	m_spellActive=ssa;
}

/** 10/12/2004 - pv - #1203: wrong selection on double click
Catch the double click signal - cut the wrong selection (with
whitespaces on the tail) - select only one word - return
controlling back to story editor - have rest */
void StoryEditor::doubleClick(int para, int position)
{
	int indexFrom=0; //, indexTo=0;
	QString selText = Editor->textCursor().selectedText();
	if (selText.length() == 0 || !m_smartSelection)
	{
		updateProps(para, position);
		return;
	}
	indexFrom = Editor->textCursor().selectionStart();
//	indexTo = Editor->textCursor().selectionEnd();
	selText =  selText.trimmed();
	Editor->textCursor().clearSelection();
	Editor->textCursor().setPosition(indexFrom);
	Editor->textCursor().setPosition(indexFrom + selText.length(), QTextCursor::KeepAnchor);
	updateProps(para, position);
}

void StoryEditor::setSmart(bool newSmartSelection)
{
	m_smartSelection = newSmartSelection;
}

void StoryEditor::closeEvent(QCloseEvent *e)
{
	if (m_textChanged)
	{
		m_blockUpdate = true;
		int t = ScMessageBox::warning(this, CommonStrings::trWarning,
									tr("Do you want to save your changes?"),
									QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
									QMessageBox::No);
		qApp->processEvents();
		if (t == QMessageBox::Yes)
		{
			updateTextFrame();
			m_result = QDialog::Accepted;
		}
		else if (t == QMessageBox::Cancel)
		{
			e->ignore();
			m_blockUpdate = false;
			return;
		}
		else if (t == QMessageBox::No)
			m_result = QDialog::Rejected;
	}
	else
		m_result = QDialog::Rejected;
	setCurrentDocumentAndItem(NULL, NULL);
	savePrefs();
// 	if (charSelect != NULL)
// 		charSelect->close();
	hide();
	m_blockUpdate = false;
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
	if (!m_spellActive)
	{
		if ( ev->type() == QEvent::WindowDeactivate )
		{
			if ((m_item != NULL) && (!m_blockUpdate))
				updateTextFrame();
			activFromApp = false;
	//		Editor->getCursorPosition(&m_currPara, &m_currChar);
		}
		if ( ev->type() == QEvent::WindowActivate )
		{
			if ((!activFromApp) && (!m_textChanged) && (!m_blockUpdate))
			{
				activFromApp = true;
				if (m_item != NULL)
				{
					//set to false otherwise some dialog properties won't be set correctly
					if (m_item->itemText.length() == 0)
						m_firstSet = false; 
					disconnectSignals();
					Editor->setUndoRedoEnabled(false);
					Editor->setUndoRedoEnabled(true);
					Editor->textCursor().setPosition(0);
					seActions["fileRevert"]->setEnabled(false);
					seActions["editCopy"]->setEnabled(false);
					seActions["editCut"]->setEnabled(false);
					seActions["editClear"]->setEnabled(false);
					m_textChanged = false;
					FontTools->Fonts->RebuildList(m_doc, m_item->isAnnotation());
					Editor->loadItemText(m_item);
	//				Editor->getCursorPosition(&m_currPara, &m_currChar);
					updateStatus();
					m_textChanged = false;
					//Editor->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
					Editor->repaint();
					EditorBar->offs = 0;
	//				EditorBar->doMove(0, Editor->contentsY());
					EditorBar->setRepaint(true);
					EditorBar->doRepaint();
					updateProps(0, 0);
					connectSignals();
				}
			}
		}
	}
	return QMainWindow::eventFilter(ob, ev);
}

void StoryEditor::setBackPref()
{
	m_blockUpdate = true;
	QColor newColor(QColorDialog::getColor(Editor->palette().color(QPalette::Base), this));
	if (newColor.isValid())
	{
		QPalette pal;
		pal.setColor(QPalette::Active, QPalette::Base, newColor);
		pal.setColor(QPalette::Inactive, QPalette::Base, newColor);
		pal.setColor(QPalette::Disabled, QPalette::Base, newColor);
		Editor->setPalette(pal);
		prefsManager->appPrefs.storyEditorPrefs.guiFontColorBackground = newColor;
	}
	m_blockUpdate = false;
}

void StoryEditor::setFontPref()
{
	m_blockUpdate = true;
	Editor->setFont( QFontDialog::getFont( 0, Editor->font(), this ) );
	prefsManager->appPrefs.storyEditorPrefs.guiFont = Editor->font().toString();
	EditorBar->doRepaint();
	m_blockUpdate = false;
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
	if(!m_doc->UsedFonts.contains(f)) {
		if (!m_doc->AddFont(f)) {
//, prefsManager->appPrefs.AvailFonts[f]->Font)) {
			FontTools->Fonts->RebuildList(m_doc);
			return;
		};
	}
	Editor->prevFont = Editor->CurrFont;
	Editor->CurrFont = f;
	updateUnicodeActions();
	CharStyle charStyle;
	charStyle.setFont((*m_doc->AllFonts)[Editor->CurrFont]);
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
	Editor->CurrentEffects = static_cast<StyleFlag>(s);
	CharStyle charStyle;
	charStyle.setFeatures(Editor->CurrentEffects.featureList());
	Editor->updateSel(charStyle);
	Editor->setEffects(s);
	bool setter=((s & ScStyle_Outline) || (s & ScStyle_Shadowed));
	StrokeTools->TxStroke->setEnabled(setter);
	StrokeTools->PM1->setEnabled(setter);
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
	if (Editor->wasMod)
		return;
	ColorList::Iterator it;
	int c = 0;

	if ((p >= static_cast<int>(Editor->StyledText.nrOfParagraphs())) || (Editor->StyledText.length() == 0) || (!m_firstSet))
	{
		int pos = Editor->StyledText.startOfParagraph(p) + ch;
		if (!m_firstSet)
		{
			const CharStyle& curstyle(pos < Editor->StyledText.length()? m_item->itemText.charStyle(pos) : m_item->itemText.defaultStyle().charStyle());
			const ParagraphStyle parStyle(pos < Editor->StyledText.length()? m_item->itemText.paragraphStyle(pos) : m_item->itemText.defaultStyle());
			Editor->currentParaStyle = parStyle.parent();
			Editor->CurrAlign = parStyle.alignment();
			Editor->CurrTextFill = curstyle.fillColor();
			Editor->CurrTextFillSh = curstyle.fillShade();
			Editor->CurrTextStroke = curstyle.strokeColor();
			Editor->CurrTextStrokeSh = curstyle.strokeShade();
			Editor->prevFont = Editor->CurrFont;
			Editor->CurrFont = curstyle.font().scName();
			Editor->CurrFontSize = curstyle.fontSize();
			Editor->CurrentEffects = curstyle.effects();
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
				for (it = m_doc->PageColors.begin(); it != m_doc->PageColors.end(); ++it)
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
				for (it = m_doc->PageColors.begin(); it != m_doc->PageColors.end(); ++it)
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
			StyleTools->SetStyle(Editor->CurrentEffects);
			StyleTools->SetShadow(Editor->CurrTextShadowX, Editor->CurrTextShadowY);
			StyleTools->setOutline(Editor->CurrTextOutline);
			StyleTools->setUnderline(Editor->CurrTextUnderPos, Editor->CurrTextUnderWidth);
			StyleTools->setStrike(Editor->CurrTextStrikePos, Editor->CurrTextStrikeWidth);
			FontTools->SetSize(Editor->CurrFontSize);
			FontTools->SetFont(Editor->CurrFont);
			FontTools->SetScale(Editor->CurrTextScale);
			FontTools->SetScaleV(Editor->CurrTextScaleV);
		}
		if ((Editor->CurrentEffects & ScStyle_Outline) || (Editor->CurrentEffects & ScStyle_Shadowed))
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		Editor->setEffects(Editor->CurrentEffects);
		m_firstSet = true;
		updateUnicodeActions();
		return;
	}
	int parStart = Editor->StyledText.startOfParagraph(p);
	const ParagraphStyle& parStyle(Editor->StyledText.paragraphStyle(parStart));
	Editor->currentParaStyle = parStyle.parent(); //FIXME ParaStyleComboBox and use localized style name
	if (Editor->StyledText.endOfParagraph(p) <= parStart)
	{
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = parStyle.charStyle().font().scName();
		Editor->CurrFontSize = parStyle.charStyle().fontSize();
		Editor->CurrentEffects = parStyle.charStyle().effects();
		Editor->CurrTextFill   = parStyle.charStyle().fillColor();
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
		Editor->setEffects(Editor->CurrentEffects);
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
		const ParagraphStyle& paraStyle(Editor->StyledText.paragraphStyle(start));
		const CharStyle& charStyle(Editor->StyledText.charStyle(start));
		Editor->CurrAlign = paraStyle.alignment();
		Editor->CurrTextFill = charStyle.fillColor();
		Editor->CurrTextFillSh = charStyle.fillShade();
		Editor->CurrTextStroke = charStyle.strokeColor();
		Editor->CurrTextStrokeSh = charStyle.strokeShade();
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = charStyle.font().scName();
		Editor->CurrFontSize = charStyle.fontSize();
		Editor->CurrentEffects = charStyle.effects() & static_cast<StyleFlag>(ScStyle_UserStyles);
		Editor->CurrTextKern   = charStyle.tracking();
		Editor->CurrTextScale  = charStyle.scaleH();
		Editor->CurrTextScaleV = charStyle.scaleV();
		Editor->CurrTextBase   = charStyle.baselineOffset();
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
		for (it = m_doc->PageColors.begin(); it != m_doc->PageColors.end(); ++it)
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
		for (it = m_doc->PageColors.begin(); it != m_doc->PageColors.end(); ++it)
		{
			if (it.key() == b)
				break;
			c++;
		}
	}
	StrokeTools->SetColor(c);
	if ((Editor->CurrentEffects & ScStyle_Outline) || (Editor->CurrentEffects & ScStyle_Shadowed))
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
	StyleTools->SetStyle(Editor->CurrentEffects);
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

	int paragraphCount = Editor->StyledText.nrOfParagraphs();
	ParC->setText(tmp.setNum(paragraphCount));
	CharC->setText(tmp.setNum(end - start));

	QRegExp rx( "(\\w+)\\b" );
	const QString& txt(Editor->StyledText.text(0, Editor->StyledText.length()));
	int counter  = 0;
	int counter2 = 0;

	int realCharCount = Editor->StyledText.length();
	int parSepCount   = paragraphCount;
	if ((realCharCount > 0) && (!txt.endsWith(SpecialChars::PARSEP)))
		parSepCount -= 1;
	CharC2->setText(tmp.setNum(realCharCount - parSepCount));

	int pos = rx.indexIn(txt, 0);
	while (pos >= 0)
	{
		if (pos >= start && pos < end)
			counter++;

		counter2++;
		pos += rx.matchedLength();
		pos  = rx.indexIn(txt, pos);
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
	charSelect->setDoc(m_doc);
	charSelect->show();
}

void StoryEditor::insertSampleText()
{
	LoremManager dia(m_doc, this);
	if (!dia.exec())
		return;
	m_blockUpdate = true;
	Editor->insertChars(dia.loremIpsum());
	m_blockUpdate = false;
}

void StoryEditor::slot_insertSpecialChar()
{
	m_blockUpdate = true;
	if (!charSelect->getCharacters().isEmpty())
		Editor->insertPlainText(charSelect->getCharacters());
	m_blockUpdate = false;
}

void StoryEditor::slot_insertUserSpecialChar(QChar c, QString)
{
	m_blockUpdate = true;
	Editor->insertPlainText(c);
	m_blockUpdate = false;
}

void StoryEditor::Do_fontPrev()
{
	m_blockUpdate = true;
	QString retval;
	ScActionPlugin* plugin;
	bool result = false;

	if (PluginManager::instance().DLLexists("fontpreview"))
	{
		plugin = dynamic_cast<ScActionPlugin*>(PluginManager::instance().getPlugin("fontpreview", false));
		if (plugin)
			result = plugin->run(this, m_doc, Editor->CurrFont);
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
	m_blockUpdate = false;
}

void StoryEditor::Do_leave2()
{
	updateTextFrame();
	m_result = QDialog::Accepted;
	setCurrentDocumentAndItem(m_doc, NULL);
	hide();
	m_blockUpdate = false;
}

void StoryEditor::Do_leave()
{
	if (m_textChanged)
	{
		m_blockUpdate = true;
		int t = ScMessageBox::warning(this, CommonStrings::trWarning,
									 tr("Do you really want to lose all your changes?"),
									 QMessageBox::Yes | QMessageBox::No,
									 QMessageBox::No,	// GUI default
									 QMessageBox::Yes);	// batch default
		qApp->processEvents();
		if (t == QMessageBox::No)
		{
			m_blockUpdate = false;
			return;
		}
	}
	m_result = QDialog::Rejected;
	setCurrentDocumentAndItem(m_doc, NULL);
	hide();
	m_blockUpdate = false;
}

void StoryEditor::Do_saveDocument()
{
	m_blockUpdate = true;
	if (ScCore->primaryMainWindow()->slotFileSave())
		updateTextFrame();
	m_blockUpdate = false;
}

bool StoryEditor::Do_new()
{
	if (!Editor->document()->isEmpty())
	{
		m_blockUpdate = true;
		int t = ScMessageBox::warning(this, CommonStrings::trWarning,
								 tr("Do you really want to clear all your text?"),
								 QMessageBox::Yes | QMessageBox::No,
								 QMessageBox::No,	// GUI default
								 QMessageBox::Yes);	// batch default
		qApp->processEvents();
		if (t == QMessageBox::No)
		{
			m_blockUpdate = false;
			return false;
		}
	}
	Editor->clear();
	Editor->setUndoRedoEnabled(false);
	Editor->setUndoRedoEnabled(true);
	//qDebug() << "SE::Do_new: cursor";
	Editor->textCursor().setPosition(0);
	seActions["fileRevert"]->setEnabled(false);
	seActions["editCopy"]->setEnabled(false);
	seActions["editCut"]->setEnabled(false);
	seActions["editClear"]->setEnabled(false);
//	m_textChanged = false;
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
	updateProps(0, 0);
	updateStatus();
	m_blockUpdate = false;
	return true;
}

void StoryEditor::slotFileRevert()
{
	if (Do_new())
	{
		Editor->loadItemText(m_item);
//		Editor->getCursorPosition(&m_currPara, &m_currChar);
		updateStatus();
		EditorBar->setRepaint(true);
		EditorBar->doRepaint();
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
	EditorBar->setRepaint(false);
	if (Editor->textCursor().hasSelection())
		Editor->textCursor().removeSelectedText();
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
	//Return immediately if we don't have to update the frame
	if (!m_textChanged)
		return;
	PageItem *nextItem = m_item;
//#if 0
	if (m_item->asTextFrame())
	{
		while (nextItem != 0)
		{
			if (nextItem->prevInChain() != 0)
				nextItem = nextItem->prevInChain();
			else
				break;
		}
	}
//#endif
	m_item->invalidateLayout();
	PageItem* nb2 = nextItem;
	nb2->itemText.clear();
#if 0
	if (m_item->asTextFrame())
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
					m_doc->FrameItems.remove(emG.at(em));
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
	// #9180 : force relayout here, it appears that relayout is sometime disabled
	// to speed up selection, but re layout() cannot be avoided here
	if (nextItem->asTextFrame())
		nextItem->asTextFrame()->invalidateLayout(true);
	nextItem->layout();
#if 0
	QList<PageItem*> FrameItemsDel;
	FrameItemsDel.setAutoDelete(true);
	for (uint a = 0; a < Editor->FrameItems.count(); ++a)
	{
		if (m_doc->FrameItems.findRef(Editor->FrameItems.at(a)) == -1)
			FrameItemsDel.append(Editor->FrameItems.at(a));
	}
	for (uint a = 0; a < FrameItemsDel.count(); ++a)
	{
		Editor->FrameItems.remove(FrameItemsDel.at(a));
	}
	FrameItemsDel.clear();
#endif
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
	m_textChanged = false;
	seActions["fileRevert"]->setEnabled(false);
	seActions["editUpdateFrame"]->setEnabled(false);
	emit DocChanged();
}

void StoryEditor::SearchText()
{
	m_blockUpdate = true;
	EditorBar->setRepaint(false);
	SearchReplace* dia = new SearchReplace(this, m_doc, m_item, false);
	dia->exec();
	int pos = dia->firstMatchCursorPosition();
	if (pos >= 0)
	{
		QTextCursor tCursor = Editor->textCursor();
		tCursor.setPosition(pos);
		Editor->setTextCursor(tCursor);
		Editor->SelStack.push(qMakePair(pos, -1));
	}
	delete dia;
	qApp->processEvents();
	m_blockUpdate = false;
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
}

//void StoryEditor::slotEditStyles()
//{
//	ScCore->primaryMainWindow()->styleMgr()->exec(this);
//}

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
		disconnect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
		int cursorPos = Editor->textCursor().position();
		int scrollPos = Editor->verticalScrollBar()->value();

/*		qDebug() << QString("changeStyleSB: pa=%2, start=%2, new=%3 %4")
			   .arg(pa)
			   .arg(Editor->StyledText.startOfParagraph(pa))
			   .arg(newStyle.parent())
			   .arg(newStyle.hasParent());
*/
		int paraStart= Editor->StyledText.startOfParagraph(pa);
		if (name.isEmpty()) // erase parent style
		{
			newStyle = Editor->StyledText.paragraphStyle(paraStart);
			newStyle.setParent(name);
			Editor->StyledText.setStyle(paraStart, newStyle);
		}
		else
			Editor->StyledText.applyStyle(paraStart, newStyle, true);

		Editor->updateFromChars(pa);
		QTextCursor tCursor = Editor->textCursor();
		tCursor.setPosition(cursorPos);
		Editor->setTextCursor(tCursor);
		Editor->verticalScrollBar()->setValue(scrollPos);

		EditorBar->doRepaint();
		connect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
		connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
	}
	else
	{
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().font().scName();
		Editor->CurrFontSize   = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fontSize();
		Editor->CurrentEffects = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().effects();
		Editor->CurrTextFill   = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fillColor();
		Editor->CurrTextFillSh = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fillShade();
		Editor->CurrTextStroke = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strokeColor();
		Editor->CurrTextStrokeSh = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strokeShade();
		Editor->CurrTextShadowX = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().shadowXOffset();
		Editor->CurrTextShadowY = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().shadowYOffset();
		Editor->CurrTextOutline = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().outlineWidth();
		Editor->CurrTextUnderPos = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().underlineOffset();
		Editor->CurrTextUnderWidth = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().underlineWidth();
		Editor->CurrTextStrikePos = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strikethruOffset();
		Editor->CurrTextStrikeWidth = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strikethruWidth();

		Editor->setEffects(Editor->CurrentEffects);
		if ((Editor->CurrentEffects & ScStyle_Outline) || (Editor->CurrentEffects & ScStyle_Shadowed))
		{
			StrokeTools->TxStroke->setEnabled(true);
			StrokeTools->PM1->setEnabled(true);
		}
		else
		{
			StrokeTools->TxStroke->setEnabled(false);
			StrokeTools->PM1->setEnabled(false);
		}
		//qDebug() << "SE::changeStyleSB: cursor";
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
		int scrollPos = Editor->verticalScrollBar()->value();
		disconnect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		disconnect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
		disconnect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
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
		QTextCursor textCursor = Editor->textCursor();
		textCursor.setPosition(sel ? SelStart : pos);
		if (sel)
			textCursor.setPosition(SelEnd, QTextCursor::KeepAnchor);
		Editor->setTextCursor(textCursor);
		Editor->verticalScrollBar()->setValue(scrollPos);
		updateProps();
		EditorBar->doRepaint();
		connect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
		connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
	}
	else
	{
		Editor->prevFont = Editor->CurrFont;
		Editor->CurrFont = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().font().scName();
		Editor->CurrFontSize   = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fontSize();
		Editor->CurrentEffects = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().effects();
		Editor->CurrTextFill   = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fillColor();
		Editor->CurrTextFillSh = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().fillShade();
		Editor->CurrTextStroke = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strokeColor();
		Editor->CurrTextStrokeSh = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strokeShade();
		Editor->CurrTextShadowX = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().shadowXOffset();
		Editor->CurrTextShadowY = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().shadowYOffset();
		Editor->CurrTextOutline = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().outlineWidth();
		Editor->CurrTextUnderPos = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().underlineOffset();
		Editor->CurrTextUnderWidth = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().underlineWidth();
		Editor->CurrTextStrikePos = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strikethruOffset();
		Editor->CurrTextStrikeWidth = m_doc->paragraphStyles().get(Editor->currentParaStyle).charStyle().strikethruWidth();

		Editor->setEffects(Editor->CurrentEffects);
		if ((Editor->CurrentEffects & ScStyle_Outline) || (Editor->CurrentEffects & ScStyle_Shadowed))
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
		disconnect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
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
		QTextCursor textCursor = Editor->textCursor();
		textCursor.setPosition(sel ? SelStart : pos);
		if (sel)
			textCursor.setPosition(SelEnd, QTextCursor::KeepAnchor);
		Editor->setTextCursor(textCursor);
		Editor->ensureCursorVisible();
		updateProps();
		EditorBar->doRepaint();
		connect(Editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateProps()));
		connect(Editor, SIGNAL(textChanged()), this, SLOT(modifiedText()));
		connect(Editor, SIGNAL(textChanged()), EditorBar, SLOT(doRepaint()));
	}
	modifiedText();
	Editor->repaint();
	Editor->setFocus();
}


void StoryEditor::modifiedText()
{
	m_textChanged = true;
	m_firstSet = true;
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
		CustomFDialog dia(this, wdir, tr("Open"), tr("Text Files (*.txt);;All Files (*)"), fdExistingFiles | fdShowCodecs);
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
				Editor->loadText(txt, m_item);
				seActions["editPaste"]->setEnabled(false);
				seActions["editCopy"]->setEnabled(false);
				seActions["editCut"]->setEnabled(false);
				seActions["editClear"]->setEnabled(false);
			}
		}
		EditorBar->setRepaint(true);
		EditorBar->doRepaint();
	}
	Editor-> repaint();
}

void StoryEditor::SaveTextFile()
{
	m_blockUpdate = true;
	QString LoadEnc = "";
	QString fileName = "";
	PrefsContext* dirs = prefsManager->prefsFile->getContext("dirs");
	QString wdir = dirs->get("story_save", prefsManager->appPrefs.pathPrefs.documents);
	CustomFDialog dia(this, wdir, tr("Save as"), tr("Text Files (*.txt);;All Files (*)"), fdShowCodecs|fdHidePreviewCheckBox);
	qApp->processEvents();
	if (dia.exec() != QDialog::Accepted)
	{
		m_blockUpdate = false;
		return;
	}
	LoadEnc = dia.TxCodeM->currentText();
	if (LoadEnc == "UTF-16")
		LoadEnc = "ISO-10646-UCS-2";
	fileName =  dia.selectedFile();
	if (!fileName.isEmpty())
	{
		dirs->set("story_save", fileName.left(fileName.lastIndexOf("/")));
		Serializer::writeWithEncoding(fileName, LoadEnc, Editor->StyledText.plainText());
	}
	m_blockUpdate = false;
}

bool StoryEditor::textDataChanged() const
{
	return m_textChanged;
}

PageItem* StoryEditor::currentItem() const
{
	return m_item;
}

ScribusDoc* StoryEditor::currentDocument() const
{
	return m_doc;
}

void StoryEditor::specialActionKeyEvent(int unicodevalue)
{
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
		Editor->setColor(true);
	Editor->insertChars(QString(QChar(unicodevalue)), guiInsertString);
	if (setColor)
		Editor->setColor(false);
	modifiedText();
	EditorBar->setRepaint(true);
	EditorBar->doRepaint();
}

void StoryEditor::updateUnicodeActions()
{
	if (Editor->prevFont!=Editor->CurrFont)
		ScCore->primaryMainWindow()->actionManager->enableUnicodeActions(&seActions, true, Editor->CurrFont);
}
