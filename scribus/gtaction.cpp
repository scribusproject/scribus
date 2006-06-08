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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "missing.h"
#include "gtaction.h"
#include "mpalette.h"
#include "scribus.h"
#include <qcursor.h>
#include <qstringlist.h>
#include "color.h"
#include "prefsmanager.h"
#include "hyphenator.h"
#include "selection.h"
#include "commonstrings.h"
#include "util.h"

gtAction::gtAction(bool append)
{
	prefsManager=PrefsManager::instance();
	//textFrame = ScMW->view->SelItem.at(0);
	textFrame = ScMW->doc->m_Selection->itemAt(0);
	it = textFrame;
	lastParagraphStyle = -1;
	inPara = false;
	isFirstWrite = true;
	lastCharWasLineChange = false;
	currentFrameStyle = "";
	doAppend = append;
	updateParagraphStyles = false;
	overridePStyleFont = true;
}

gtAction::gtAction(bool append, PageItem* pageitem)
{
	prefsManager=PrefsManager::instance();
	textFrame = pageitem;
	it = textFrame;
	lastParagraphStyle = -1;
	inPara = false;
	isFirstWrite = true;
	lastCharWasLineChange = false;
	currentFrameStyle = "";
	doAppend = append;
	updateParagraphStyles = false;
	overridePStyleFont = true;
}

void gtAction::setProgressInfo()
{
	ScMW->mainWindowStatusLabel->setText(QObject::tr("Importing text"));
	ScMW->mainWindowProgressBar->reset();
	ScMW->mainWindowProgressBar->setTotalSteps(0);
}

void gtAction::setProgressInfoDone()
{
	ScMW->mainWindowStatusLabel->setText("");
	ScMW->mainWindowProgressBar->reset();
}

void gtAction::setInfo(QString infoText)
{
	ScMW->mainWindowStatusLabel->setText(infoText);
}

void gtAction::clearFrame()
{
	textFrame->itemText.clear();
	textFrame->CPos = 0;
	ScMW->doc->updateFrameItems();
}

void gtAction::write(const QString& text, gtStyle *style)
{
	if (isFirstWrite)
	{
		if (!doAppend)
		{
			if (it->NextBox != 0)
			{
				PageItem *nextItem = it->NextBox;
				while (nextItem != 0)
				{
					nextItem->itemText.clear();
					nextItem->CPos = 0;
					nextItem = nextItem->NextBox;
				}
			}
			it->itemText.clear();
			it->CPos = 0;
			ScMW->doc->updateFrameItems();
		}
	}
	int paragraphStyle = -1;
	if (style->target() == "paragraph")
	{
		gtParagraphStyle* pstyle = dynamic_cast<gtParagraphStyle*>(style);
		paragraphStyle = applyParagraphStyle(pstyle);
		if (isFirstWrite)
			inPara = true;
	}
	else if (style->target() == "frame")
	{
		gtFrameStyle* fstyle = dynamic_cast<gtFrameStyle*>(style);
		applyFrameStyle(fstyle);
	}

	if ((inPara) && (!lastCharWasLineChange) && (text.left(1) != "\n") && (lastParagraphStyle != -1))
		paragraphStyle = lastParagraphStyle;


	if (paragraphStyle == -1)
		paragraphStyle = ::findParagraphStyle(ScMW->doc, ScMW->doc->currentStyle);

	gtFont* font = style->getFont();
	QString fontName = validateFont(font)->scName();
	gtFont font2(*font);
	font2.setName(ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().font()->scName());
	QString fontName2 = validateFont(&font2)->scName();
	CharStyle lastStyle;
	int lastStyleStart = 0;
	for (uint a = 0; a < text.length(); ++a)
	{
		CharStyle newStyle;
		if ((text.at(a) == QChar(0)) || (text.at(a) == QChar(13)))
			continue;
		QChar ch = text.at(a);
		if ((ch == QChar(10)) || (ch == QChar(5)))
			ch = QChar(13);
		if ((inPara) && (!overridePStyleFont))
		{
			if (ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().font() == &Foi::NONE)
				newStyle.setFont((*ScMW->doc->AllFonts)[fontName2]);
			else
				newStyle.setFont(ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().font());
			newStyle.setFontSize(ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().fontSize());
			newStyle.setFillColor(ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().fillColor());
			newStyle.setFillShade(ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().fillShade());
			newStyle.setStrokeColor(ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().strokeColor());
			newStyle.setStrokeShade(ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().strokeShade());
			newStyle.setEffects(ScMW->doc->docParagraphStyles[paragraphStyle].charStyle().effects());
		}
		else
		{
			newStyle.setFont((*ScMW->doc->AllFonts)[fontName]);
			newStyle.setFontSize(font->getSize());
			newStyle.setFillColor(parseColor(font->getColor()));
			newStyle.setFillShade(font->getShade());
			newStyle.setStrokeColor(parseColor(font->getStrokeColor()));
			newStyle.setStrokeShade(font->getStrokeShade());
			newStyle.setEffects(static_cast<StyleFlag>(font->getEffectsValue()));
		}
		newStyle.setScaleH(font->getHscale());
		newStyle.setScaleV(1000);
		newStyle.setBaselineOffset(0);
		newStyle.setShadowXOffset(50);
		newStyle.setShadowYOffset(-50);
		newStyle.setOutlineWidth(10);
		newStyle.setUnderlineOffset(-1);
		newStyle.setUnderlineWidth(-1);
		newStyle.setStrikethruOffset(-1);
		newStyle.setStrikethruWidth(-1);
		newStyle.setTracking(font->getKerning());
		int pos = it->itemText.length();
		it->itemText.insertChars(pos, QString(ch));
		if (newStyle != lastStyle) {
			it->itemText.applyStyle(lastStyleStart, pos-lastStyleStart, lastStyle);
			lastStyle = newStyle;
			lastStyleStart = pos;
		}
		if (ch == SpecialChars::PARSEP) {
			it->itemText.applyStyle(pos, ScMW->doc->docParagraphStyles[paragraphStyle]);
		}
	}
	it->itemText.applyStyle(lastStyleStart, it->itemText.length()-lastStyleStart, lastStyle);
	it->itemText.applyStyle(it->itemText.length()-1, ScMW->doc->docParagraphStyles[paragraphStyle]);
	
	lastCharWasLineChange = text.right(1) == "\n";
	inPara = style->target() == "paragraph";
	lastParagraphStyle = paragraphStyle;
	if (isFirstWrite)
		isFirstWrite = false;
}

int gtAction::findParagraphStyle(gtParagraphStyle* pstyle)
{
	return findParagraphStyle(pstyle->getName());
}

int gtAction::findParagraphStyle(const QString& name)
{
	int pstyleIndex = -1;
	for (uint i = 0; i < ScMW->doc->docParagraphStyles.size(); ++i)
	{
		if (ScMW->doc->docParagraphStyles[i].name() == name)
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
		pstyleIndex = ScMW->doc->docParagraphStyles.size() - 1;
	}
	else if (updateParagraphStyles)
	{
		updateParagraphStyle(pstyleIndex, pstyle);
	}
	return pstyleIndex;
}

void gtAction::applyFrameStyle(gtFrameStyle* fstyle)
{
	textFrame->setColumns(fstyle->getColumns());
	textFrame->setColumnGap(fstyle->getColumnsGap());
	textFrame->setFillColor(parseColor(fstyle->getBgColor()));
	textFrame->setFillShade(fstyle->getBgShade());
	textFrame->TabValues = QValueList<ParagraphStyle::TabRecord>(*(fstyle->getTabValues()));

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
	Foi* scfont = validateFont(font);
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
	const CharStyle& style(textFrame->itemText.defaultStyle().charStyle());
	
	font->setName(style.font()->scName());
	font->setSize(style.fontSize());
	font->setColor(style.fillColor());
	font->setShade(style.fillShade());
	font->setStrokeColor(style.strokeColor());
	font->setStrokeShade(style.strokeShade());
	font->setHscale(style.scaleH());
	font->setKerning(0);
}

void gtAction::getFrameStyle(gtFrameStyle *fstyle)
{
	fstyle->setColumns(textFrame->Cols);
	fstyle->setColumnsGap(textFrame->ColGap);
	fstyle->setBgColor(textFrame->fillColor());
	fstyle->setBgShade(textFrame->fillShade());

	const ParagraphStyle& vg(textFrame->itemText.defaultStyle());
	fstyle->setName(vg.name());
	fstyle->setLineSpacing(vg.lineSpacing());
	fstyle->setAlignment(vg.alignment());
	fstyle->setIndent(vg.leftMargin());
	fstyle->setFirstLineIndent(vg.firstIndent());
	fstyle->setSpaceAbove(vg.gapBefore());
	fstyle->setSpaceBelow(vg.gapAfter());
	fstyle->setDropCap(vg.hasDropCap());
	fstyle->setDropCapHeight(vg.dropCapLines());
	fstyle->setAdjToBaseline(vg.useBaselineGrid());

	gtFont font;
	getFrameFont(&font);
	fstyle->setFont(font);
	fstyle->setName("Default frame style");
}

void gtAction::createParagraphStyle(gtParagraphStyle* pstyle)
{
	ScribusDoc* currDoc=textFrame->document();
	if (currDoc->docParagraphStyles.size() > 5)
	{
		for (uint i = 5; i < currDoc->docParagraphStyles.size(); ++i)
		{
			if (currDoc->docParagraphStyles[i].name() == pstyle->getName())
				return;
		}
	}
	gtFont* font = pstyle->getFont();
	ParagraphStyle vg;
	vg.setName(pstyle->getName());
	double linesp;
	if (pstyle->getAutoLineSpacing())
		linesp = getLineSpacing(pstyle->getFont()->getSize());
	else
		linesp = pstyle->getLineSpacing();
	vg.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
	vg.setLineSpacing(linesp);
	vg.setAlignment(pstyle->getAlignment());
	vg.setLeftMargin(pstyle->getIndent());
	vg.setFirstIndent(pstyle->getFirstLineIndent());
	vg.setGapBefore(pstyle->getSpaceAbove());
	vg.setGapAfter(pstyle->getSpaceBelow());
	vg.charStyle().setFont(validateFont(font));
	vg.charStyle().setFontSize(font->getSize());
	vg.tabValues().clear();
	QValueList<ParagraphStyle::TabRecord> *tabs = pstyle->getTabValues();
	for (uint i = 0; i < tabs->size(); ++i)
	{
		ParagraphStyle::TabRecord tmp = (*tabs)[i];
		vg.tabValues().append(tmp);
	}
	vg.setHasDropCap(pstyle->hasDropCap());
	vg.setDropCapLines(pstyle->getDropCapHeight());
	vg.setDropCapOffset(0);
	vg.charStyle().setEffects(static_cast<StyleFlag>(font->getEffectsValue()));
	vg.charStyle().setFillColor(parseColor(font->getColor()));
	vg.charStyle().setFillShade(font->getShade());
	vg.charStyle().setStrokeColor(parseColor(font->getStrokeColor()));
	vg.charStyle().setStrokeShade(font->getStrokeShade());
	vg.setUseBaselineGrid(pstyle->isAdjToBaseline());
	vg.charStyle().setShadowXOffset(50);
	vg.charStyle().setShadowYOffset(-50);
	vg.charStyle().setOutlineWidth(10);
	vg.charStyle().setScaleH(1000);
	vg.charStyle().setScaleV(1000);
	vg.charStyle().setBaselineOffset(0);
	vg.charStyle().setTracking(0);
	vg.charStyle().setUnderlineOffset(ScMW->doc->typographicSettings.valueUnderlinePos);
	vg.charStyle().setUnderlineWidth(ScMW->doc->typographicSettings.valueUnderlineWidth);
	vg.charStyle().setStrikethruOffset(ScMW->doc->typographicSettings.valueStrikeThruPos);
	vg.charStyle().setStrikethruWidth(ScMW->doc->typographicSettings.valueStrikeThruPos);
	textFrame->document()->docParagraphStyles.append(vg);
	ScMW->propertiesPalette->Spal->updateFormatList();
}

void gtAction::removeParagraphStyle(const QString& name)
{
	int index = findParagraphStyle(name);
	if (index != -1)
		removeParagraphStyle(index);
}

void gtAction::removeParagraphStyle(int index)
{
	QValueList<ParagraphStyle>::iterator it = ScMW->doc->docParagraphStyles.at(index);
	ScMW->doc->docParagraphStyles.remove(it);
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
	vg.setName(pstyle->getName());
	double linesp;
	if (pstyle->getAutoLineSpacing())
		linesp = getLineSpacing(pstyle->getFont()->getSize());
	else
		linesp = pstyle->getLineSpacing();
	vg.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
	vg.setLineSpacing(linesp);
	vg.setAlignment(pstyle->getAlignment());
	vg.setLeftMargin(pstyle->getIndent());
	vg.setFirstIndent(pstyle->getFirstLineIndent());
	vg.setGapBefore(pstyle->getSpaceAbove());
	vg.setGapAfter(pstyle->getSpaceBelow());
	vg.charStyle().setFont(validateFont(font));
	vg.charStyle().setFontSize(font->getSize());
	vg.tabValues().clear();
	QValueList<ParagraphStyle::TabRecord> *tabs = pstyle->getTabValues();
	for (uint i = 0; i < tabs->size(); ++i)
	{
		ParagraphStyle::TabRecord tmp = (*tabs)[i];
		vg.tabValues().append(tmp);
	}
	vg.setHasDropCap(pstyle->hasDropCap());
	vg.setDropCapLines(pstyle->getDropCapHeight());
	vg.setDropCapOffset(0);
	vg.charStyle().setEffects(static_cast<StyleFlag>(font->getEffectsValue()));
	vg.charStyle().setFillColor(parseColor(font->getColor()));
	vg.charStyle().setFillShade(font->getShade());
	vg.charStyle().setStrokeColor(parseColor(font->getStrokeColor()));
	vg.charStyle().setStrokeShade(font->getStrokeShade());
	vg.setUseBaselineGrid(pstyle->isAdjToBaseline());
	vg.charStyle().setShadowXOffset(50);
	vg.charStyle().setShadowYOffset(-50);
	vg.charStyle().setOutlineWidth(10);
	vg.charStyle().setScaleH(1000);
	vg.charStyle().setScaleV(1000);
	vg.charStyle().setBaselineOffset(0);
	vg.charStyle().setTracking(0);
	vg.charStyle().setUnderlineOffset(ScMW->doc->typographicSettings.valueUnderlinePos);
	vg.charStyle().setUnderlineWidth(ScMW->doc->typographicSettings.valueUnderlineWidth);
	vg.charStyle().setStrikethruOffset(ScMW->doc->typographicSettings.valueStrikeThruPos);
	vg.charStyle().setStrikethruWidth(ScMW->doc->typographicSettings.valueStrikeThruPos);
	ScMW->doc->docParagraphStyles[pstyleIndex] = vg;
}

Foi* gtAction::validateFont(gtFont* font)
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
		useFont = textFrame->itemText.defaultStyle().charStyle().font()->scName();
	else if (prefsManager->appPrefs.AvailFonts[font->getName()] == 0)
	{
		bool found = false;
		useFont = "";
		QString tmpName = findFontName(font);
		if (tmpName != NULL)
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
				if (tmpName != NULL)
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
				if (tmpName != NULL)
				{
					useFont = tmpName;
					found = true;
				}
				delete tmp;
			}
			if (!found)
			{
				if (!prefsManager->appPrefs.GFontSub.contains(font->getName()))
				{
					MissingFont *dia = new MissingFont(0, useFont, ScMW->doc);
					dia->exec();
					useFont = dia->getReplacementFont();
					prefsManager->appPrefs.GFontSub[font->getName()] = useFont;
					delete dia;
				}
				else
					useFont = prefsManager->appPrefs.GFontSub[font->getName()];
			}
		}
	}

	if(!ScMW->doc->UsedFonts.contains(useFont))
		ScMW->doc->AddFont(useFont);
	return prefsManager->appPrefs.AvailFonts[useFont];
}

QString gtAction::findFontName(gtFont* font)
{
	QString ret = NULL;
	for (uint i = 0; i < static_cast<uint>(gtFont::NAMECOUNT); ++i)
	{
		QString nname = font->getName(i);
		if (prefsManager->appPrefs.AvailFonts[nname] != 0)
		{
			ret = nname;
			break;
		}
	}
	return ret;
}

double gtAction::getLineSpacing(int fontSize)
{
	return ((fontSize / 10.0) * static_cast<double>(ScMW->doc->typographicSettings.autoLineSpacing) / 100) + (fontSize / 10.0);
}

double gtAction::getFrameWidth()
{
	return textFrame->width();
}

QString gtAction::getFrameName()
{
	return QString(textFrame->itemName());
}

bool gtAction::getUpdateParagraphStyles()
{
	return updateParagraphStyles;
}

void gtAction::setUpdateParagraphStyles(bool newUPS)
{
	updateParagraphStyles = newUPS;
}

bool gtAction::getOverridePStyleFont()
{
	return overridePStyleFont;
}
void gtAction::setOverridePStyleFont(bool newOPSF)
{
	overridePStyleFont = newOPSF;
}

QString gtAction::parseColor(const QString &s)
{
	QString ret = CommonStrings::None;
	if (s == CommonStrings::None)
		return ret; // don't want None to become Black or any color
	bool found = false;
	ColorList::Iterator it;
	for (it = ScMW->doc->PageColors.begin(); it != ScMW->doc->PageColors.end(); ++it)
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
			QString parse = s.stripWhiteSpace();
			QStringList colors = QStringList::split( ',', parse );
			QString r = colors[0].right( ( colors[0].length() - 4 ) );
			QString g = colors[1];
			QString b = colors[2].left( ( colors[2].length() - 1 ) );
			if( r.contains( "%" ) )
			{
				r = r.left( r.length() - 1 );
				r = QString::number( static_cast<int>( ( static_cast<double>( 255 * r.toDouble() ) / 100.0 ) ) );
			}
			if( g.contains( "%" ) )
			{
				g = g.left( g.length() - 1 );
				g = QString::number( static_cast<int>( ( static_cast<double>( 255 * g.toDouble() ) / 100.0 ) ) );
			}
			if( b.contains( "%" ) )
			{
				b = b.left( b.length() - 1 );
				b = QString::number( static_cast<int>( ( static_cast<double>( 255 * b.toDouble() ) / 100.0 ) ) );
			}
			c = QColor(r.toInt(), g.toInt(), b.toInt());
		}
		else
		{
			QString rgbColor = s.stripWhiteSpace();
			if( rgbColor.startsWith( "#" ) )
				c.setNamedColor( rgbColor );
			else
				c = parseColorN( rgbColor );
		}
		found = false;
		for (it = ScMW->doc->PageColors.begin(); it != ScMW->doc->PageColors.end(); ++it)
		{
			if (c == ScMW->doc->PageColors[it.key()].getRGBColor())
			{
				ret = it.key();
				found = true;
			}
		}
		if (!found)
		{
			ScColor tmp;
			tmp.fromQColor(c);
			ScMW->doc->PageColors.insert("FromGetText"+c.name(), tmp);
			ScMW->propertiesPalette->updateColorList();
			ret = "FromGetText"+c.name();
		}
	}
	return ret;
}

QColor gtAction::parseColorN(const QString &rgbColor)
{
	int r, g, b;
	keywordToRGB( rgbColor, r, g, b );
	return QColor( r, g, b );
}

void gtAction::finalize()
{
	if (ScMW->doc->docHyphenator->AutoCheck)
		ScMW->doc->docHyphenator->slotHyphenate(textFrame);
	ScMW->view->DrawNew();
	ScMW->slotDocCh();
}

gtAction::~gtAction()
{
	finalize();
}
