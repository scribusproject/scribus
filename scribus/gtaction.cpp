/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@welho.com                                                      *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "gtaction.h"

extern ScribusApp* ScApp;

gtAction::gtAction(bool append)
{
	textFrame = ScApp->doc->ActPage->SelItem.at(0);
	it = textFrame;
	lastParagraphStyle = -1;
	lastCharWasLineChange = false;
	if (it->NextBox != 0)
	{
		PageItem *nb = it->NextBox;
		while (nb != 0)
		{
			nb->Ptext.clear();
			nb->CPos = 0;
			nb->Dirty = true;
			nb = nb->NextBox;
		}
	}
	it->Ptext.clear();
	it->CPos = 0;
}

void gtAction::setProgressInfo()
{
	ScApp->FMess->setText(QObject::tr("Importing text"));
	ScApp->FProg->reset();
	ScApp->FProg->setTotalSteps(0);
}

void gtAction::setProgressInfoDone()
{
	ScApp->FMess->setText("");
	ScApp->FProg->reset();
}

void gtAction::setInfo(QString infoText)
{
	ScApp->FMess->setText(infoText);
}

void gtAction::clearFrame()
{
	textFrame->Ptext.clear();
	textFrame->CPos = 0;
	textFrame->Dirty = true;
}

void gtAction::write(QString text, gtStyle *style)
{
	int paragraphStyle = 0;
	if (style->target() == "paragraph")
	{
		gtParagraphStyle* pstyle = dynamic_cast<gtParagraphStyle*>(style);
		paragraphStyle = applyParagraphStyle(pstyle);
	}
	else if (style->target() == "frame")
	{
		gtFrameStyle* fstyle = dynamic_cast<gtFrameStyle*>(style);
		applyFrameStyle(fstyle);
	}
	gtFont* font = style->getFont();
	validateFont(font);
	for (uint a = 0; a < text.length(); ++a)
	{
		if ((text.at(a) == QChar(0)) || (text.at(a) == QChar(13)))
			continue;
		struct Pti *hg = new Pti;
		hg->ch = text.at(a);
		if ((hg->ch == QChar(10)) || (hg->ch == QChar(5)))
			hg->ch = QChar(13);
		hg->cfont = font->getName();
		hg->csize = font->getSize();
		hg->ccolor = font->getColor();
		hg->cshade = font->getShade();
		hg->cstroke = font->getStrokeColor();
		hg->cshade2 = font->getStrokeShade();
		hg->cscale = font->getHscale();
		hg->cextra = font->getKerning();
		hg->cselect = false;
		hg->cstyle = font->getEffectsValue();
		if ((paragraphStyle == -1) || ((a == 0) && (text.at(0) == '\n')))
		{
			if (lastParagraphStyle == -1)
				hg->cab = ScApp->doc->CurrentABStil;
			else
				hg->cab = lastParagraphStyle;
		}
		else
			hg->cab = paragraphStyle;
		hg->xp = 0;
		hg->yp = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		it->Ptext.append(hg);
		lastParagraphStyle = hg->cab;
	}
}

int gtAction::findParagraphStyle(gtParagraphStyle* pstyle)
{
	int pstyleIndex = -1;
	for (uint i = 0; i < ScApp->doc->Vorlagen.size(); ++i)
	{
		if (ScApp->doc->Vorlagen[i].Vname == pstyle->getName())
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
		pstyleIndex = ScApp->doc->Vorlagen.size() - 1;
	}

	return pstyleIndex;
}

void gtAction::applyFrameStyle(gtFrameStyle* fstyle)
{
	if (fstyle->getName() != currentFrameStyle)
	{
		// @todo Set the default options for the text frame
		gtParagraphStyle* pstyle = new gtParagraphStyle(*fstyle);
		int pstyleIndex = findParagraphStyle(pstyle);
		if (pstyleIndex == -1)
			pstyleIndex = 0;
		textFrame->Doc->CurrentABStil = pstyleIndex;
		gtFont* font = fstyle->getFont();
		textFrame->IFont = font->getName();
		textFrame->ISize = font->getSize();
		textFrame->TxtFill = font->getColor();
		textFrame->ShTxtFill = font->getShade();
		textFrame->TxtStroke = font->getStrokeColor();
		textFrame->ShTxtStroke = font->getStrokeShade();
		textFrame->TxtScale = font->getHscale();
	}
}

void gtAction::getFrameFont(gtFont *font)
{
	font->setName(textFrame->IFont);
	font->setSize(textFrame->ISize);
	font->setColor(textFrame->TxtFill);
	font->setShade(textFrame->ShTxtFill);
	font->setStrokeColor(textFrame->TxtStroke);
	font->setStrokeShade(textFrame->ShTxtStroke);
	font->setHscale(textFrame->TxtScale);
	font->setKerning(0);
}

void gtAction::getFrameStyle(gtFrameStyle *fstyle)
{
	fstyle->setColumns(textFrame->Cols);
	fstyle->setColumnsGap(textFrame->ColGap);
	fstyle->setBgColor(textFrame->Pcolor);
	fstyle->setBgShade(textFrame->Shade);
	
	struct StVorL vg = textFrame->Doc->Vorlagen[textFrame->Doc->CurrentABStil];
	fstyle->setName(vg.Vname);
	fstyle->setLineSpacing(vg.LineSpa);
	fstyle->setAlignment(vg.Ausri);
	fstyle->setIndent(vg.Indent);
	fstyle->setFirstLineIndent(vg.First);
	fstyle->setSpaceAbove(vg.Avor);
	fstyle->setSpaceBelow(vg.Anach);
	fstyle->setDropCap(vg.Drop);
	fstyle->setDropCapHeight(vg.DropLin);
	fstyle->setAdjToBaseline(vg.BaseAdj);
	
	gtFont font;
	getFrameFont(&font);
	fstyle->setFont(font);
}

void gtAction::createParagraphStyle(gtParagraphStyle* pstyle)
{
	if (textFrame->Doc->Vorlagen.size() > 5)
	{
		for (uint i = 5; i < textFrame->Doc->Vorlagen.size(); ++i)
		{
			if (textFrame->Doc->Vorlagen[i].Vname == pstyle->getName())
				return;
		}
	} 
	gtFont* font = pstyle->getFont();
	struct StVorL vg;
	vg.Vname = pstyle->getName();
	vg.LineSpa = pstyle->getLineSpacing();
	vg.Ausri = pstyle->getAlignment();
	vg.Indent = pstyle->getIndent();
	vg.First = pstyle->getFirstLineIndent();
	vg.Avor = pstyle->getSpaceAbove();
	vg.Anach = pstyle->getSpaceBelow();
	vg.Font = font->getName();
	vg.FontSize = font->getSize();
	vg.TabValues.clear();
	vg.Drop = pstyle->hasDropCap();
	vg.DropLin = pstyle->getDropCapHeight();
	vg.FontEffect = font->getEffectsValue();
	vg.FColor = font->getColor();
	vg.FShade = font->getShade();
	vg.SColor = font->getStrokeColor();
	vg.SShade = font->getStrokeShade();
	vg.BaseAdj = pstyle->isAdjToBaseline();
	textFrame->Doc->Vorlagen.append(vg);
	ScApp->Mpal->Spal->updateFList();
}

bool gtAction::validateFont(gtFont* font)
{
	if (ScApp->Prefs.AvailFonts[font->getName()] == 0)
	{
		// @todo show the subst dialog and add font
	}
	else
	{
		ScApp->SetNewFont(font->getName());
	}
	if(!ScApp->doc->UsedFonts.contains(font->getName()))
		ScApp->doc->AddFont(font->getName(), ScApp->Prefs.AvailFonts[font->getName()]->Font);
	return false;
}

void gtAction::finalize()
{
	if (ScApp->doc->Trenner->AutoCheck)
		ScApp->doc->Trenner->slotHyphenate(textFrame);
	ScApp->doc->ActPage->update();
	ScApp->slotDocCh();
}

gtAction::~gtAction()
{
	finalize();
}
