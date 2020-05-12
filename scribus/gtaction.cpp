/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include <QCursor>
#include <QList>
#include <QProgressBar>
#include <QStringList>

#include "gtaction.h"
#include "gtfont.h"
#include "gtstyle.h"
#include "gtparagraphstyle.h"
#include "gtframestyle.h"

#include "commonstrings.h"
#include "hyphenator.h"
#include "marks.h"
#include "notesstyles.h"
#include "pageitem_textframe.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "selection.h"
#include "sccolorengine.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "undomanager.h"
#include "util.h"
#include "util_text.h"
#include "ui/contentpalette.h"
#include "ui/missing.h"

gtAction::gtAction(bool append, PageItem* pageitem) :
	m_prefsManager(PrefsManager::instance())
{	m_textFrame = pageitem;
	m_ScMW=m_textFrame->doc()->scMW();
	m_it = m_textFrame;
	m_lastParagraphStyle = -1;
	m_inPara = false;
	m_isFirstWrite = true;
	m_lastCharWasLineChange = false;
	m_currentFrameStyle = "";
	m_doAppend = append;
	m_updateParagraphStyles = false;
	m_overridePStyleFont = true;
	m_undoManager = UndoManager::instance();
	m_noteStory = nullptr;
	m_note = nullptr;
}

void gtAction::setProgressInfo()
{
	m_ScMW->setStatusBarInfoText(QObject::tr("Importing text"));
	m_ScMW->mainWindowProgressBar->reset();
	m_ScMW->mainWindowProgressBar->setMaximum(0); // 0 shows a busy progressbar
}

void gtAction::setProgressInfoDone()
{
	m_ScMW->setStatusBarInfoText("");
	m_ScMW->mainWindowProgressBar->reset();
	m_ScMW->mainWindowProgressBar->setMaximum(1);
}

void gtAction::setInfo(const QString& infoText)
{
	m_ScMW->setStatusBarInfoText(infoText);
}

void gtAction::clearFrame()
{
	m_textFrame->itemText.clear();
}

void gtAction::writeUnstyled(const QString& text, bool isNote)
{
	UndoTransaction activeTransaction;
	if (m_isFirstWrite && m_it->itemText.length() > 0)
	{
		if (!m_doAppend)
		{
			if (UndoManager::undoEnabled())
				activeTransaction = m_undoManager->beginTransaction(Um::Selection, Um::IGroup, Um::ImportText, "", Um::IDelete);
			if (m_it->nextInChain() != nullptr)
			{
				PageItem *nextItem = m_it->nextInChain();
				while (nextItem != nullptr)
				{
					nextItem->itemText.selectAll();
					nextItem->asTextFrame()->deleteSelectedTextFromFrame();
					nextItem = nextItem->nextInChain();
				}
			}
			m_it->itemText.selectAll();
			m_it->asTextFrame()->deleteSelectedTextFromFrame();
		}
	}

	QChar ch0(0), ch5(5), ch10(10), ch13(13);
	QString textStr = text;
	textStr.remove(ch0);
	textStr.remove(ch13);
	textStr.replace(ch10,ch13);
	textStr.replace(ch5,ch13);
	textStr.replace(QString(0x2028),SpecialChars::LINEBREAK);
	textStr.replace(QString(0x2029),SpecialChars::PARSEP);
	if (isNote)
	{
		if (m_note == nullptr)
		{
			m_note = m_it->m_Doc->newNote(m_it->m_Doc->m_docNotesStylesList.at(0));
			Q_ASSERT(m_noteStory == nullptr);
			m_noteStory = new StoryText(m_it->m_Doc);
		}
		if (textStr == SpecialChars::OBJECT)
		{
			NotesStyle* nStyle = m_note->notesStyle();
			QString label = "NoteMark_" + nStyle->name();
			if (nStyle->range() == NSRstory)
				label += " in " + m_it->firstInChain()->itemName();
			if (m_it->m_Doc->getMark(label + "_1", MARKNoteMasterType) != nullptr)
				getUniqueName(label,m_it->m_Doc->marksLabelsList(MARKNoteMasterType), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
			else
				label = label + "_1";
			Mark* mrk = m_it->m_Doc->newMark();
			mrk->label = label;
			mrk->setType(MARKNoteMasterType);
			mrk->setNotePtr(m_note);
			m_note->setMasterMark(mrk);
			if (m_noteStory->text(m_noteStory->length() -1) == SpecialChars::PARSEP)
				m_noteStory->removeChars(m_noteStory->length() -1, 1);
			m_note->setSaxedText(saxedText(m_noteStory));
			mrk->clearString();
			mrk->OwnPage = m_it->OwnPage;
			m_it->itemText.insertMark(mrk);
			if (UndoManager::undoEnabled())
			{
				ScItemsState* is = new ScItemsState(UndoManager::InsertNote);
				is->set("ETEA", mrk->label);
				is->set("MARK", QString("new"));
				is->set("label", mrk->label);
				is->set("type", (int) MARKNoteMasterType);
				is->set("strtxt", QString(""));
				is->set("nStyle", nStyle->name());
				is->set("at", m_it->itemText.cursorPosition() -1);
				is->insertItem("inItem", m_it);
				m_undoManager->action(m_it->m_Doc, is);
			}
			m_note = nullptr;
			delete m_noteStory;
		}
		else
			m_noteStory->insertChars(m_noteStory->length(), textStr);
	}
	else
	{
		int pos = m_it->itemText.length();
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::AppendText,"",Um::ICreate);
			ss->set("INSERT_FRAMETEXT");
			ss->set("TEXT_STR",textStr);
			ss->set("START", pos);
			m_undoManager->action(m_it, ss);
		}
		m_it->itemText.insertChars(pos, textStr);
	}
	m_lastCharWasLineChange = text.right(1) == "\n";
	m_isFirstWrite = false;
	if (activeTransaction)
	{
		activeTransaction.commit();
	}
}

void gtAction::write(const QString& text, gtStyle *style, bool isNote)
{
	if (m_isFirstWrite)
	{
		if (!m_doAppend)
		{
			if (m_it->nextInChain() != nullptr)
			{
				PageItem *nextItem = m_it->nextInChain();
				while (nextItem != nullptr)
				{
					nextItem->itemText.clear();
					nextItem = nextItem->nextInChain();
				}
			}
			m_it->itemText.clear();
		}
	}
	int paragraphStyle = -1;
	if (style->target() == "paragraph")
	{
		gtParagraphStyle* pstyle = dynamic_cast<gtParagraphStyle*>(style);
		assert(pstyle != nullptr);
		paragraphStyle = applyParagraphStyle(pstyle);
		if (m_isFirstWrite)
			m_inPara = true;
	}
	else if (style->target() == "frame")
	{
		gtFrameStyle* fstyle = dynamic_cast<gtFrameStyle*>(style);
		assert(fstyle != nullptr);
		applyFrameStyle(fstyle);
	}

	if ((m_inPara) && (!m_lastCharWasLineChange) && (text.left(1) != "\n") && (m_lastParagraphStyle != -1))
		paragraphStyle = m_lastParagraphStyle;

	if (paragraphStyle == -1)
		paragraphStyle = 0; //::findParagraphStyle(textFrame->doc(), textFrame->doc()->currentStyle);

	const ParagraphStyle& paraStyle = m_textFrame->doc()->paragraphStyles()[paragraphStyle];

	gtFont* font = style->getFont();
//	QString fontName = validateFont(font).scName();
	CharStyle lastStyle, newStyle;
	int lastStyleStart = 0;
	
	if ((m_inPara) && (!m_overridePStyleFont))
	{
		if (paraStyle.charStyle().font().isNone())
		{
			gtFont font2(*font);
			font2.setName(paraStyle.charStyle().font().scName());
			QString fontName2 = validateFont(&font2).scName();
			newStyle.setFont((*m_textFrame->doc()->AllFonts)[fontName2]);
		}
	}
	else
	{
		setCharStyleAttributes(font, newStyle);
	}
	/*newStyle.eraseCharStyle(paraStyle.charStyle());*/

	lastStyle = newStyle;
	lastStyleStart = m_it->itemText.length();
	StoryText* story = nullptr;
	if (isNote)
	{
		if (m_noteStory == nullptr)
		{
			m_note = m_it->m_Doc->newNote(m_it->m_Doc->m_docNotesStylesList.at(0));
			m_noteStory = new StoryText(m_it->m_Doc);
		}
		story = m_noteStory;
	}
	else
		story = &m_it->itemText;

	QChar ch0(0), ch5(5), ch10(10), ch13(13); 
	for (int a = 0; a < text.length(); ++a)
	{
		if ((text.at(a) == ch0) || (text.at(a) == ch13))
			continue;
		QChar ch = text.at(a);
		if ((ch == ch10) || (ch == ch5))
			ch = ch13;
		
		int pos = story->length();
		if (isNote && ch == SpecialChars::OBJECT)
		{
			NotesStyle* nStyle = m_note->notesStyle();
			QString label = "NoteMark_" + nStyle->name();
			if (nStyle->range() == NSRstory)
				label += " in " + m_it->firstInChain()->itemName();
			if (m_it->m_Doc->getMark(label + "_1", MARKNoteMasterType) != nullptr)
				getUniqueName(label,m_it->m_Doc->marksLabelsList(MARKNoteMasterType), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
			else
				label = label + "_1";
			Mark* mrk = m_it->m_Doc->newMark();
			mrk->label = label;
			mrk->setType(MARKNoteMasterType);
			mrk->setNotePtr(m_note);
			m_note->setMasterMark(mrk);
			mrk->clearString();
			mrk->OwnPage = m_it->OwnPage;
			m_it->itemText.insertMark(mrk);
			story->applyCharStyle(lastStyleStart, story->length()-lastStyleStart, lastStyle);
			if (paraStyle.hasName())
			{
				ParagraphStyle pStyle;
				pStyle.setParent(paraStyle.name());
				story->applyStyle(qMax(0,story->length()-1), pStyle);
			}
			else
				story->applyStyle(qMax(0,story->length()-1), paraStyle);
			
			m_lastCharWasLineChange = text.right(1) == "\n";
			m_inPara = style->target() == "paragraph";
			m_lastParagraphStyle = paragraphStyle;
			if (m_isFirstWrite)
				m_isFirstWrite = false;
			if (story->text(pos -1) == SpecialChars::PARSEP)
				story->removeChars(pos-1, 1);
			m_note->setSaxedText(saxedText(story));
			m_note = nullptr;
			delete m_noteStory;
			m_noteStory = nullptr;
			return;
		}
		story->insertChars(pos, QString(ch));
		if (ch == SpecialChars::PARSEP) 
		{
			if (paraStyle.hasName())
			{
				ParagraphStyle pstyle;
				pstyle.setParent(paraStyle.name());
				story->applyStyle(pos, pstyle);
			}
			else
				story->applyStyle(pos, paraStyle);
		}
	}
	story->applyCharStyle(lastStyleStart, story->length()-lastStyleStart, lastStyle);
	if (paraStyle.hasName())
	{
		ParagraphStyle pStyle;
		pStyle.setParent(paraStyle.name());
		story->applyStyle(qMax(0,story->length()-1), pStyle);
	}
	else
		story->applyStyle(qMax(0,story->length()-1), paraStyle);
	
	m_lastCharWasLineChange = text.right(1) == "\n";
	m_inPara = style->target() == "paragraph";
	m_lastParagraphStyle = paragraphStyle;
	if (m_isFirstWrite)
		m_isFirstWrite = false;
}

int gtAction::findParagraphStyle(gtParagraphStyle* pstyle)
{
	return findParagraphStyle(pstyle->getName());
}

int gtAction::findParagraphStyle(const QString& name)
{
	int pstyleIndex = -1;
	for (int i = 0; i < m_textFrame->doc()->paragraphStyles().count(); ++i)
	{
		if (m_textFrame->doc()->paragraphStyles()[i].name() == name)
		{
			pstyleIndex = i;
			break;
		}
	}
	return pstyleIndex;
}

int gtAction::applyParagraphStyle(gtParagraphStyle* pstyle)
{
	int pstyleIndex = findParagraphStyle(pstyle);
	if (pstyleIndex == -1)
	{
		createParagraphStyle(pstyle);
		pstyleIndex = m_textFrame->doc()->paragraphStyles().count() - 1;
	}
	else if (m_updateParagraphStyles)
	{
		updateParagraphStyle(pstyleIndex, pstyle);
	}
	return pstyleIndex;
}

void gtAction::applyFrameStyle(gtFrameStyle* fstyle)
{
	m_textFrame->setColumns(fstyle->getColumns());
	m_textFrame->setColumnGap(fstyle->getColumnsGap());
	m_textFrame->setFillColor(parseColor(fstyle->getBgColor()));
	m_textFrame->setFillShade(fstyle->getBgShade());
	ParagraphStyle newTabs(m_textFrame->itemText.defaultStyle());
	newTabs.setTabValues(QList<ParagraphStyle::TabRecord>(*(fstyle->getTabValues())));
	m_textFrame->itemText.setDefaultStyle(newTabs);

// 	gtParagraphStyle* pstyle = new gtParagraphStyle(*fstyle);
// 	int pstyleIndex = findParagraphStyle(pstyle);
// 	if (pstyleIndex == -1)
// 		pstyleIndex = 0;
// 	textFrame->Doc->currentParaStyle = pstyleIndex;

/* FIXME
	double linesp;
	if (fstyle->getAutoLineSpacing())
		linesp = getLineSpacing(fstyle->getFont()->getSize());
	else
		linesp = fstyle->getLineSpacing();
	textFrame->setLineSpacing(linesp);
	textFrame->setLineSpacingMode(0);
	gtFont* font = fstyle->getFont();
	Scface* scfont = validateFont(font);
	textFrame->setFont(scfont->scName());
	textFrame->setFontSize(font->getSize());
	textFrame->TxtFill = parseColor(font->getColor());
	textFrame->ShTxtFill = font->getShade();
	textFrame->TxtStroke = parseColor(font->getStrokeColor());
	textFrame->ShTxtStroke = font->getStrokeShade();
	textFrame->TxtScale = font->getHscale();
	textFrame->TxtScaleV = 1000;
	textFrame->TxtBase = 0;
	textFrame->TxtShadowX = 50;
	textFrame->TxtShadowY = -50;
	textFrame->TxtOutline = 10;
	textFrame->TxtUnderPos = -1;
	textFrame->TxtUnderWidth = -1;
	textFrame->TxtStrikePos = -1;
	textFrame->TxtStrikeWidth = -1;
	textFrame->ExtraV = font->getKerning();
	*/
}

void gtAction::getFrameFont(gtFont *font)
{
	const CharStyle& style(m_textFrame->itemText.defaultStyle().charStyle());
	
	if (!style.isInhFont())
		font->setName(style.font().scName());
	if (!style.isInhFontSize())
		font->setSize(style.fontSize());
	if (!style.isInhFillColor())
		font->setColor(style.fillColor());
	if (!style.isInhFillShade())
		font->setShade(qRound(style.fillShade()));
	if (!style.isInhStrokeColor())
		font->setStrokeColor(style.strokeColor());
	if (!style.isInhStrokeShade())
		font->setStrokeShade(qRound(style.strokeShade()));
	if (!style.isInhScaleH())
		font->setHscale(qRound(style.scaleH()));
	font->setKerning(0);
}

void gtAction::getFrameStyle(gtFrameStyle *fstyle)
{
	fstyle->setColumns(m_textFrame->m_columns);
	fstyle->setColumnsGap(m_textFrame->m_columnGap);
	fstyle->setBgColor(m_textFrame->fillColor());
	fstyle->setBgShade(qRound(m_textFrame->fillShade()));

	const ParagraphStyle& vg(m_textFrame->itemText.defaultStyle());
	fstyle->setName(vg.name());
	fstyle->setLineSpacing(vg.lineSpacing());
	fstyle->setAdjToBaseline(vg.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing);

	if (!vg.isInhAlignment())
		fstyle->setAlignment(vg.alignment());
	if (!vg.isInhLeftMargin())
		fstyle->setIndent(vg.leftMargin());
	if (!vg.isInhFirstIndent())
		fstyle->setFirstLineIndent(vg.firstIndent());
	if (!vg.isInhGapBefore())
		fstyle->setSpaceAbove(vg.gapBefore());
	if (!vg.isInhGapAfter())
		fstyle->setSpaceBelow(vg.gapAfter());
	if (!vg.isInhHasDropCap())
		fstyle->setDropCap(vg.hasDropCap());
	if (!vg.isInhDropCapLines())
		fstyle->setDropCapHeight(vg.dropCapLines());
	if (!vg.isInhHasBullet())
		fstyle->setBullet(vg.hasBullet(), vg.bulletStr());
	if (!vg.isInhHasNum())
		fstyle->setNum(vg.hasNum(),vg.numFormat(),vg.numLevel(), vg.numStart(), vg.numPrefix(), vg.numSuffix());

	gtFont font;
	getFrameFont(&font);
	fstyle->setFont(font);
	fstyle->setName("Default frame style");
}

void gtAction::createParagraphStyle(gtParagraphStyle* pstyle)
{
	ScribusDoc* currDoc=m_textFrame->doc();
	for (int i = 0; i < currDoc->paragraphStyles().count(); ++i)
	{
		if (currDoc->paragraphStyles()[i].name() == pstyle->getName())
			return;
	}
	gtFont* font = pstyle->getFont();
	
	ParagraphStyle vg;
	setParaStyleAttributes(pstyle, vg);
	setCharStyleAttributes(font, vg.charStyle());

	// Maybe set those attributes when target is the frame
	/*vg.charStyle().setShadowXOffset(50);
	vg.charStyle().setShadowYOffset(-50);
	vg.charStyle().setOutlineWidth(10);
	vg.charStyle().setScaleH(1000);
	vg.charStyle().setScaleV(1000);
	vg.charStyle().setBaselineOffset(0);
	vg.charStyle().setTracking(0);
	vg.charStyle().setUnderlineOffset(textFrame->doc()->typographicSettings.valueUnderlinePos);
	vg.charStyle().setUnderlineWidth(textFrame->doc()->typographicSettings.valueUnderlineWidth);
	vg.charStyle().setStrikethruOffset(textFrame->doc()->typographicSettings.valueStrikeThruPos);
	vg.charStyle().setStrikethruWidth(textFrame->doc()->typographicSettings.valueStrikeThruPos);*/

	StyleSet<ParagraphStyle> tmp;
	tmp.create(vg);
	m_textFrame->doc()->redefineStyles(tmp, false);
	
	m_ScMW->contentPalette->updateTextStyles();
}

void gtAction:: setCharStyleAttributes(gtFont *font, CharStyle& style)
{
	int flags = font->getFlags();
	style.erase();

	if ((flags & gtFont::familyWasSet) || (flags & gtFont::weightWasSet) || (flags & gtFont::slantWasSet))
		style.setFont(validateFont(font));
	if (flags & gtFont::sizeWasSet)
		style.setFontSize(font->getSize());
	if (flags & gtFont::effectWasSet)
		style.setFeatures(static_cast<StyleFlag>(font->getEffectsValue()).featureList());
	if (flags & gtFont::fillColorWasSet)
		style.setFillColor(parseColor(font->getColor()));
	if (flags & gtFont::fillShadeWasSet)
		style.setFillShade(font->getShade());
	if (flags & gtFont::strokeColorWasSet)
		style.setStrokeColor(parseColor(font->getStrokeColor()));
	if (flags & gtFont::strokeShadeWasSet)
		style.setStrokeShade(font->getStrokeShade());
	if (flags & gtFont::hscaleWasSet)
		style.setScaleH(font->getHscale());
	if (flags & gtFont::kerningWasSet)
		style.setTracking(font->getKerning());
}

void gtAction::setParaStyleAttributes(gtParagraphStyle *pstyle, ParagraphStyle& style)
{
	double linesp;
	int flags = pstyle->getFlags();
	style.erase();

	style.setName(pstyle->getName());
	if (pstyle->getAutoLineSpacing())
		linesp = getLineSpacing(pstyle->getFont()->getSize());
	else
		linesp = pstyle->getLineSpacing();
	style.setLineSpacingMode(pstyle->isAdjToBaseline() ? ParagraphStyle::BaselineGridLineSpacing : ParagraphStyle::FixedLineSpacing);
	style.setLineSpacing(linesp);

	if (flags & gtParagraphStyle::alignmentWasSet)
		style.setAlignment(static_cast<ParagraphStyle::AlignmentType>(pstyle->getAlignment()));
	if (flags & gtParagraphStyle::indentWasSet)
		style.setLeftMargin(pstyle->getIndent());
	if (flags & gtParagraphStyle::firstIndentWasSet)
		style.setFirstIndent(pstyle->getFirstLineIndent());
	if (flags & gtParagraphStyle::spaceAboveWasSet)
		style.setGapBefore(pstyle->getSpaceAbove());
	if (flags & gtParagraphStyle::spaceBelowWasSet)
		style.setGapAfter(pstyle->getSpaceBelow());
	if (flags & gtParagraphStyle::tabValueWasSet)
		style.setTabValues(*pstyle->getTabValues());
	if (flags & gtParagraphStyle::dropCapWasSet)
		style.setHasDropCap(pstyle->hasDropCap());
	if (flags & gtParagraphStyle::dropCapHeightWasSet)
		style.setDropCapLines(pstyle->getDropCapHeight());
	if (flags & gtParagraphStyle::bulletWasSet)
	{
		style.setHasBullet(pstyle->hasBullet());
		style.setBulletStr(pstyle->getBullet());
	}
	if (flags & gtParagraphStyle::numWasSet)
	{
		style.setHasNum(pstyle->hasNum());
		style.setNumName(pstyle->getName());
		style.setNumFormat(pstyle->getNumFormat());
		style.setNumLevel(pstyle->getNumLevel());
		style.setNumStart(pstyle->getNumStart());
		style.setNumHigher(true);
		style.setNumOther(true);
		style.setNumPrefix(pstyle->getNumPrefix());
		style.setNumSuffix(pstyle->getNumSuffix());
	}
	
	/*vg.setDropCapOffset(0);*/
}

void gtAction::removeParagraphStyle(const QString& name)
{
	int index = findParagraphStyle(name);
	if (index != -1)
		removeParagraphStyle(index);
}

void gtAction::removeParagraphStyle(int index)
{
	QMap<QString, QString> map;
	map[m_textFrame->doc()->paragraphStyles()[index].name()] = "";
	m_textFrame->doc()->replaceStyles(map);
}

void gtAction::updateParagraphStyle(const QString&, gtParagraphStyle* pstyle)
{
	int pstyleIndex = findParagraphStyle(pstyle->getName());
	if (pstyleIndex != -1)
		updateParagraphStyle(pstyleIndex, pstyle);
}

void gtAction::updateParagraphStyle(int pstyleIndex, gtParagraphStyle* pstyle)
{
	gtFont* font = pstyle->getFont();
	ParagraphStyle vg;

	setParaStyleAttributes(pstyle, vg);
	setCharStyleAttributes(font, vg.charStyle());

	// Maybe set those attributes when target is the frame
	/*vg.charStyle().setShadowXOffset(50);
	vg.charStyle().setShadowYOffset(-50);
	vg.charStyle().setOutlineWidth(10);
	vg.charStyle().setScaleH(1000);
	vg.charStyle().setScaleV(1000);
	vg.charStyle().setBaselineOffset(0);
	vg.charStyle().setTracking(0);
	vg.charStyle().setUnderlineOffset(textFrame->doc()->typographicSettings.valueUnderlinePos);
	vg.charStyle().setUnderlineWidth(textFrame->doc()->typographicSettings.valueUnderlineWidth);
	vg.charStyle().setStrikethruOffset(textFrame->doc()->typographicSettings.valueStrikeThruPos);
	vg.charStyle().setStrikethruWidth(textFrame->doc()->typographicSettings.valueStrikeThruPos);*/

	StyleSet<ParagraphStyle> tmp;
	tmp.create(vg);
	m_textFrame->doc()->redefineStyles(tmp, false);
	if (vg.name() != m_textFrame->doc()->paragraphStyles()[pstyleIndex].name())
	{
		QMap<QString, QString> map;
		map[m_textFrame->doc()->paragraphStyles()[pstyleIndex].name()] = vg.name();
		m_textFrame->doc()->replaceStyles(map);
	}
}

ScFace gtAction::validateFont(gtFont* font)
{
	// Dirty hack for family Times New Roman
	if (font->getFamily() == "Times New")
	{
		font->setFamily("Times New Roman");
		if (font->getWeight() == "Roman")
			font->setWeight("Regular");
	}

	QString useFont = font->getName();
	if ((useFont.isNull()) || (useFont.isEmpty()))
		useFont = m_textFrame->itemText.defaultStyle().charStyle().font().scName();
	else if (m_prefsManager.appPrefs.fontPrefs.AvailFonts[font->getName()].isNone())
	{
		bool found = false;
		// Do not empty otherwise user may be asked to replace an empty font 
		// by font replacement dialog
		// useFont = "";
		QString tmpName = findFontName(font);
		if (!tmpName.isEmpty())
		{
			useFont = tmpName;
			found = true;
		}
		if (!found)
		{
			if (font->getSlant() == gtFont::fontSlants[ITALIC])
			{
				gtFont* tmp = new gtFont(*font);
				tmp->setSlant(OBLIQUE);
				tmpName = findFontName(tmp);
				if (!tmpName.isEmpty())
				{
					useFont = tmpName;
					found = true;
				}
				delete tmp;
			}
			else if (font->getSlant() == gtFont::fontSlants[OBLIQUE])
			{
				gtFont* tmp = new gtFont(*font);
				tmp->setSlant(ITALIC);
				tmpName = findFontName(tmp);
				if (!tmpName.isEmpty())
				{
					useFont = tmpName;
					found = true;
				}
				delete tmp;
			}
			if (!found)
			{
				if (!m_prefsManager.appPrefs.fontPrefs.GFontSub.contains(font->getName()))
				{
					MissingFont *dia = new MissingFont(nullptr, useFont, m_textFrame->doc());
					dia->exec();
					useFont = dia->getReplacementFont();
					m_prefsManager.appPrefs.fontPrefs.GFontSub[font->getName()] = useFont;
					delete dia;
				}
				else
					useFont = m_prefsManager.appPrefs.fontPrefs.GFontSub[font->getName()];
			}
		}
	}

	if(!m_textFrame->doc()->UsedFonts.contains(useFont))
		m_textFrame->doc()->AddFont(useFont);
	return m_prefsManager.appPrefs.fontPrefs.AvailFonts[useFont];
}

QString gtAction::findFontName(gtFont* font)
{
	QString ret;
	for (uint i = 0; i < static_cast<uint>(gtFont::NAMECOUNT); ++i)
	{
		QString nname = font->getName(i);
		if (! m_prefsManager.appPrefs.fontPrefs.AvailFonts[nname].isNone())
		{
			ret = nname;
			break;
		}
	}
	return ret;
}

double gtAction::getLineSpacing(int fontSize)
{
	return ((fontSize / 10.0) * (static_cast<double>(m_textFrame->doc()->typographicPrefs().autoLineSpacing) / 100));
}

double gtAction::getFrameWidth()
{
	return m_textFrame->width();
}

QString gtAction::getFrameName()
{
	return QString(m_textFrame->itemName());
}

bool gtAction::getUpdateParagraphStyles()
{
	return m_updateParagraphStyles;
}

void gtAction::setUpdateParagraphStyles(bool newUPS)
{
	m_updateParagraphStyles = newUPS;
}

bool gtAction::getOverridePStyleFont()
{
	return m_overridePStyleFont;
}
void gtAction::setOverridePStyleFont(bool newOPSF)
{
	m_overridePStyleFont = newOPSF;
}

QString gtAction::parseColor(const QString &s)
{
	QString ret = CommonStrings::None;
	if (s == CommonStrings::None)
		return ret; // don't want None to become Black or any color
	bool found = false;
	ColorList::Iterator it;
	for (it = m_textFrame->doc()->PageColors.begin(); it != m_textFrame->doc()->PageColors.end(); ++it)
	{
		if (it.key() == s)
		{
			ret = it.key();
			found = true;
		}
	}
	if (!found)
	{
		QColor c;
		if( s.startsWith( "rgb(" ) )
		{
			QString parse = s.trimmed();
			QStringList colors = parse.split(',', QString::SkipEmptyParts);
			QString r = colors[0].right( ( colors[0].length() - 4 ) );
			QString g = colors[1];
			QString b = colors[2].left( ( colors[2].length() - 1 ) );
			if( r.contains( "%" ) )
			{
				r.chop(1);
				r = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(r) ) / 100.0 ) ) );
			}
			if( g.contains( "%" ) )
			{
				g.chop(1);
				g = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(g) ) / 100.0 ) ) );
			}
			if( b.contains( "%" ) )
			{
				b.chop(1);
				b = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(b) ) / 100.0 ) ) );
			}
			c = QColor(r.toInt(), g.toInt(), b.toInt());
		}
		else
			c.setNamedColor(s.trimmed());
		found = false;
		for (it = m_textFrame->doc()->PageColors.begin(); it != m_textFrame->doc()->PageColors.end(); ++it)
		{
			if (c == ScColorEngine::getRGBColor(it.value(), m_textFrame->doc()))
			{
				ret = it.key();
				found = true;
			}
		}
		if (!found)
		{
			ScColor tmp;
			tmp.fromQColor(c);
			m_textFrame->doc()->PageColors.insert("FromGetText"+c.name(), tmp);
			m_ScMW->contentPalette->updateColorList();
			ret = "FromGetText"+c.name();
		}
	}
	return ret;
}

void gtAction::finalize()
{
	if (m_textFrame->doc()->docHyphenator->AutoCheck)
		m_textFrame->doc()->docHyphenator->slotHyphenate(m_textFrame);
	m_textFrame->doc()->regionsChanged()->update(QRectF());
	m_textFrame->doc()->changed();
}

gtAction::~gtAction()
{
	finalize();
}
