
#include <cmath>

#include "pageitem_noteframe.h"

#include "appmodes.h"
#include "pageitem.h"
#include "pageitem_textframe.h"

#include "scribusdoc.h"
#include "textnote.h"
#include "undomanager.h"
#include "util.h"
#include "util_text.h"

#include "text/boxes.h"

PageItem_NoteFrame::PageItem_NoteFrame(NotesStyle *nStyle, ScribusDoc *doc, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline)
	: PageItem_TextFrame(doc, x, y, w, h, w2, fill, outline)
{
	m_nstyle = nStyle;
	itemText.clear();

	m_itemName = generateUniqueCopyName(nStyle->isEndNotes() ? tr("Endnote frame ") + m_nstyle->name() : tr("Footnote frame ") + m_nstyle->name(), false);
	AutoName = false; //endnotes frame will saved with name
	setUName(m_itemName);

	//set default style for note frame
	ParagraphStyle newStyle;
	if (nStyle->notesParStyle().isEmpty() || (nStyle->notesParStyle() == tr("No Style")))
		newStyle.setParent(m_Doc->paragraphStyles()[0].name()); // set default doc style
	else
		newStyle.setParent(nStyle->notesParStyle());
	itemText.blockSignals(true);
	itemText.setDefaultStyle(newStyle);
	itemText.blockSignals(false);

	m_textFlowMode = TextFlowUsesFrameShape;
	setColumns(1);

	m_SizeVLocked = m_nstyle->isAutoNotesHeight();
	m_SizeHLocked = m_nstyle->isAutoNotesWidth();
	m_SizeLocked = m_nstyle->isAutoNotesHeight() && m_nstyle->isAutoNotesWidth();
}

PageItem_NoteFrame::PageItem_NoteFrame(ScribusDoc *doc, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline)
	: PageItem_TextFrame(doc, x, y, w, h, w2, fill, outline)
{
	m_textFlowMode = TextFlowUsesFrameShape;
}

PageItem_NoteFrame::PageItem_NoteFrame(PageItem_TextFrame* inFrame, NotesStyle *nStyle) : PageItem_TextFrame(inFrame->doc(),inFrame->xPos(), inFrame->yPos(),inFrame->width(), inFrame->height(),inFrame->lineWidth(), inFrame->fillColor(), inFrame->lineColor())
{
	m_nstyle = nStyle;
	m_masterFrame = inFrame;

	m_itemName = generateUniqueCopyName(nStyle->isEndNotes() ? tr("Endnote frame ") + m_nstyle->name() : tr("Footnote frame ") + m_nstyle->name(), false);
	AutoName = false;
	setUName(m_itemName);

	//set default style for note frame
	ParagraphStyle newStyle;
	if (nStyle->notesParStyle().isEmpty() || (nStyle->notesParStyle() == tr("No Style")))
	{
		if (nStyle->isEndNotes())
			//set default doc style
			newStyle.setParent(m_Doc->paragraphStyles()[0].name());
		else
		{
			newStyle.setParent(m_masterFrame->itemText.defaultStyle().parent());
			newStyle.applyStyle(m_masterFrame->currentStyle());
		}
	}
	else
		newStyle.setParent(nStyle->notesParStyle());
	itemText.blockSignals(true);
	itemText.setDefaultStyle(newStyle);
	itemText.blockSignals(false);

	double frameHeight = calculateLineSpacing(newStyle, this);
	if (frameHeight == 0.0 && !m_nstyle->isAutoNotesHeight())
		frameHeight = newStyle.charStyle().fontSize()/10;
	m_height = oldHeight = frameHeight;
	oldWidth = m_width;
	oldRot = m_rotation;
	oldXpos = m_xPos;
	m_yPos = oldYpos = m_masterFrame->yPos() + m_masterFrame->height();

	m_textFlowMode = TextFlowUsesFrameShape;
	setColumns(1);

	if (m_nstyle->isAutoWeldNotesFrames() && (m_masterFrame != nullptr))
	{
		addWelded(m_masterFrame);
		m_masterFrame->addWelded(this);
		m_masterFrame->setWeldPoint(0, m_masterFrame->height(), this);
		setWeldPoint(0,0, m_masterFrame);
	}
	if (m_nstyle->isAutoNotesHeight())
		m_SizeVLocked = true;
	else
		m_SizeVLocked = false;
	if (m_nstyle->isAutoNotesWidth())
		m_SizeHLocked = true;
	else
		m_SizeHLocked = false;
	if (m_nstyle->isAutoNotesHeight() && m_nstyle->isAutoNotesWidth())
		m_SizeLocked = true;
	else
		m_SizeLocked = false;
}

void PageItem_NoteFrame::getNamedResources(ResourceCollection& lists) const
{
	PageItem_TextFrame::getNamedResources(lists);

	if (m_nstyle)
		lists.collectNoteStyle(m_nstyle->name());
}

void PageItem_NoteFrame::replaceNamedResources(ResourceCollection& newNames)
{
	PageItem_TextFrame::replaceNamedResources(newNames);

	if (m_nstyle)
	{
		const auto& rsrcNoteStyles = newNames.noteStyles();
		auto it = rsrcNoteStyles.find(m_nstyle->name());
		if ((it != rsrcNoteStyles.end()) && (*it != m_nstyle->name()))
		{
			NotesStyle* newStyle = m_Doc->getNotesStyle(*it);
			setNoteStyle(newStyle);
		}
	}
}

void PageItem_NoteFrame::setNoteStyle(NotesStyle *nStyle, PageItem_TextFrame* master)
{
	m_nstyle = nStyle;
	if (master != nullptr)
		m_masterFrame = master;
	itemText.clear();

	m_itemName = generateUniqueCopyName(m_nstyle->isEndNotes() ? "Endnote frame " + m_nstyle->name() : "Footnote frame " + m_nstyle->name(), false);
	setUName(m_itemName);

	//set default style for note frame
	ParagraphStyle newStyle;
	if (nStyle->notesParStyle().isEmpty() || (nStyle->notesParStyle() == tr("No Style")))
	{
		if (nStyle->isEndNotes() || (m_masterFrame == nullptr))
		{
			//set default doc style
			newStyle.setParent(m_Doc->paragraphStyles()[0].name());
		}
		else if (master != nullptr)
		{
			newStyle.setParent(m_masterFrame->itemText.defaultStyle().parent());
			newStyle.applyStyle(m_masterFrame->currentStyle());
		}
	}
	else
		newStyle.setParent(nStyle->notesParStyle());
	itemText.blockSignals(true);
	itemText.setDefaultStyle(newStyle);
	itemText.blockSignals(false);

	if (m_nstyle->isAutoNotesHeight())
		m_SizeVLocked = true;
	else
		m_SizeVLocked = false;
	if (m_nstyle->isAutoNotesWidth())
		m_SizeHLocked = true;
	else
		m_SizeHLocked = false;
	if (m_nstyle->isAutoNotesHeight() && m_nstyle->isAutoNotesWidth())
		m_SizeLocked = true;
	else
		m_SizeLocked = false;
}

void PageItem_NoteFrame::layout()
{
	if (!invalid || m_notes.isEmpty())
		return;
	if (!m_Doc->flag_layoutNotesFrames)
		return;
	if (itemText.length() == 0)
		return;
	if ((m_masterFrame != nullptr) && m_masterFrame->invalid)
		return;

	//while layouting notes frames undo should be disabled
	UndoManager::instance()->setUndoEnabled(false);

	if (m_nstyle->isAutoNotesWidth() && (m_width != m_masterFrame->width()))
	{
		oldWidth = m_width = m_masterFrame->width();
		updateClip();
	}

	if ((m_Doc->appMode == modeEdit) && isSelected())
		updateNotesText();

	PageItem_TextFrame::layout();
	int oldH = m_height;
	if (notesStyle()->isAutoNotesHeight())
	{
		if (frameOverflows())
		{
			// Increase height while text don`t fit in frame
			double maxH = m_Doc->currentPage()->height() - m_yPos;
			if (maxH <= 0)
				maxH = m_Doc->currentPage()->height();
			while (frameOverflows())
			{
				oldHeight = m_height += 8;
				updateClip(false);
				invalid = true;
				PageItem_TextFrame::layout();
				if (m_height >= maxH)
					break;
			}
		}
		textLayout.box()->moveTo(textLayout.box()->x(), 0);
		oldHeight = m_height = textLayout.box()->naturalHeight() + m_textDistanceMargins.bottom();
		updateConstants();
		updateClip();
		invalid = true;
		PageItem_TextFrame::layout();
	}
	if (oldH != height())
	{
		if (masterFrame() != nullptr)
		{
			foreach(PageItem_NoteFrame* nF, masterFrame()->notesFramesList())
				nF->invalid = true;
		}
	}
	invalid = false;
	m_Doc->regionsChanged()->update(getBoundingRect());
	UndoManager::instance()->setUndoEnabled(true);
}

void PageItem_NoteFrame::insertNote(TextNote *note)
{
	Mark* mrk = note->noteMark();
	if (mrk == nullptr)
	{
		mrk = m_Doc->newMark();
		mrk->setType(MARKNoteFrameType);
		QString label = "NoteFrameMark_" + notesStyle()->name();
		if (notesStyle()->range() == NSRstory)
			label += " in " + note->masterMark()->getItemPtr()->firstInChain()->itemName();
		mrk->label = label + "_" + note->numString();
		mrk->setNotePtr(note);
		getUniqueName(mrk->label, m_Doc->marksLabelsList(MARKNoteFrameType), "_");
		note->setNoteMark(mrk);
	}
	mrk->setItemPtr(this);
	mrk->setString(notesStyle()->prefix() + note->numString() + note->notesStyle()->suffix());

	StoryText story(m_Doc);
	if (!note->saxedText().isEmpty())
		story = desaxeString(m_Doc, note->saxedText());
	story.insertMark(mrk, 0);
	story.setDefaultStyle(itemText.defaultStyle());
//	story.applyCharStyle(0, story.length(), itemText.charStyle());
	if (itemText.length() > 0)
		itemText.insertChars(itemText.length(), SpecialChars::PARSEP);
	itemText.insert(itemText.length(), story);
}

void PageItem_NoteFrame::updateNotes(const QList<TextNote*>& nList, bool clear)
{
	if (nList == m_notes && !clear)
		return;
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc->setNotesChanged(true);
	//itemText.blockSignals(true);

	if (clear)
	{
		itemText.selectAll();
		deleteSelectedTextFromFrame();
		m_notes = nList;
		for (int a=0; a < m_notes.count(); ++a)
			insertNote(m_notes.at(a));
	}
	else
	{
		//just insert new notes into frame notes list
		int count = nList.count();
		if (count > 0)
		{
			for (int i=0; i< count; ++i)
			{
				TextNote* note = nList.at(i);
				if (!m_notes.contains(note))
				{
					m_notes.append(note);
					insertNote(note);
				}
			}
		}
	}
	UndoManager::instance()->setUndoEnabled(true);
	//itemText.blockSignals(false);
	invalid = true;
}

void PageItem_NoteFrame::updateNotesText()
{
	//read texts from notes frame and copy it to note`s data
	if (m_notes.isEmpty() || (itemText.length() == 0))
		return;

	int oldSelStart = itemText.startOfSelection();
	int oldSelLen = itemText.selectionLength();
	int pos = 0;
	int startPos = 0;
	TextNote *note = nullptr;
	Mark* prevMark = nullptr;
	while (pos < itemText.length())
	{
		if (itemText.hasMark(pos, MARKNoteFrameType))
		{
			if (prevMark != nullptr)
			{
				note = prevMark->getNotePtr();
				if (note != nullptr)
				{
					int offset = 0;
					if (itemText.text(pos - 1) == SpecialChars::PARSEP)
						++offset;
					int len = pos - startPos -offset;
					if (len <= 0)
						note->clearSaxedText();
					else
					{
						note->setSaxedText(getItemTextSaxed(startPos, len));
						note->textLen = len;
					}
					itemText.deselectAll();
				}
			}
			prevMark = itemText.mark(pos);
			startPos = pos + 1;
		}
		++pos;
	}
	if (prevMark != nullptr)
	{
		note = prevMark->getNotePtr();
		Q_ASSERT(note != nullptr);
		if (startPos != pos)
		{
			note->setSaxedText(getItemTextSaxed(startPos, pos - startPos));
			note->textLen = pos - startPos;
		}
		else //empty note text (only note marker)
			note->clearSaxedText();
	}
	if (oldSelLen > 0)
		itemText.select(oldSelStart, oldSelLen);
}

void PageItem_NoteFrame::restoreDeleteNoteText(SimpleState *state, bool isUndo)
{
	PageItem::restoreDeleteFrameText(state, isUndo);
	updateNotesText();
}

void PageItem_NoteFrame::restoreDeleteNoteParagraph(SimpleState *state, bool isUndo)
{
	PageItem::restoreDeleteFrameParagraph(state, isUndo);
	updateNotesText();
}

void PageItem_NoteFrame::restoreInsertNoteText(SimpleState *state, bool isUndo)
{
	PageItem::restoreInsertFrameText(state, isUndo);
	updateNotesText();
}

void PageItem_NoteFrame::restoreInsertNoteParagraph(SimpleState *state, bool isUndo)
{
	PageItem::restoreInsertFrameParagraph(state, isUndo);
	updateNotesText();
}

void PageItem_NoteFrame::unWeld(bool doUndo)
{
	if (doUndo)
		PageItem::unWeld();
	else
	{
		for (int a = 0 ; a < weldList.count(); a++)
		{
			WeldingInfo wInf = weldList.at(a);
			PageItem *pIt = wInf.weldItem;
			for (int b = 0 ; b < pIt->weldList.count(); b++)
			{
				WeldingInfo wInf2 = pIt->weldList.at(b);
				PageItem *pIt2 = wInf2.weldItem;
				if (pIt2 == this)
				{
					pIt->weldList.removeAt(b);
					break;
				}
			}
		}
		weldList.clear();
	}
}
