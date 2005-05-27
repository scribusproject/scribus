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

extern ScribusApp* ScApp;

gtAction::gtAction(bool append)
{
	textFrame = ScApp->view->SelItem.at(0);
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
	textFrame->itemText.clear();
	textFrame->CPos = 0;
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
		paragraphStyle = ScApp->doc->currentParaStyle;

	gtFont* font = style->getFont();
	QString fontName = validateFont(font);
	gtFont* font2 = new gtFont(*font);
	font2->setName(ScApp->doc->docParagraphStyles[paragraphStyle].Font);
	QString fontName2 = validateFont(font2);
	for (uint a = 0; a < text.length(); ++a)
	{
		if ((text.at(a) == QChar(0)) || (text.at(a) == QChar(13)))
			continue;
		struct ScText *hg = new ScText;
		hg->ch = text.at(a);
		if ((hg->ch == QChar(10)) || (hg->ch == QChar(5)))
			hg->ch = QChar(13);
		if ((inPara) && (!overridePStyleFont))
		{
			if (ScApp->doc->docParagraphStyles[paragraphStyle].Font == "")
				hg->cfont = (*ScApp->doc->AllFonts)[fontName2];
			else
				hg->cfont = (*ScApp->doc->AllFonts)[ScApp->doc->docParagraphStyles[paragraphStyle].Font];
			hg->csize = ScApp->doc->docParagraphStyles[paragraphStyle].FontSize;
			hg->ccolor = ScApp->doc->docParagraphStyles[paragraphStyle].FColor;
			hg->cshade = ScApp->doc->docParagraphStyles[paragraphStyle].FShade;
			hg->cstroke = ScApp->doc->docParagraphStyles[paragraphStyle].SColor;
			hg->cshade2 = ScApp->doc->docParagraphStyles[paragraphStyle].SShade;
			hg->cstyle = ScApp->doc->docParagraphStyles[paragraphStyle].FontEffect;
		}
		else
		{
			hg->cfont = (*ScApp->doc->AllFonts)[fontName];
			hg->csize = font->getSize();
			hg->ccolor = parseColor(font->getColor());
			hg->cshade = font->getShade();
			hg->cstroke = parseColor(font->getStrokeColor());
			hg->cshade2 = font->getStrokeShade();
			hg->cstyle = font->getEffectsValue();
		}
		hg->cscale = font->getHscale();
		hg->cscalev = 1000;
		hg->cbase = 0;
		hg->cshadowx = 50;
		hg->cshadowy = -50;
		hg->coutline = 10;
		hg->cunderpos = -1;
		hg->cunderwidth = -1;
		hg->cstrikepos = -1;
		hg->cstrikewidth = -1;
		hg->cextra = font->getKerning();
		hg->cselect = false;
		hg->cab = paragraphStyle;
		hg->xp = 0;
		hg->yp = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		it->itemText.append(hg);
	}
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
	for (uint i = 0; i < ScApp->doc->docParagraphStyles.size(); ++i)
	{
		if (ScApp->doc->docParagraphStyles[i].Vname == name)
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
		pstyleIndex = ScApp->doc->docParagraphStyles.size() - 1;
	}
	else if (updateParagraphStyles)
	{
		updateParagraphStyle(pstyleIndex, pstyle);
	}
	return pstyleIndex;
}

void gtAction::applyFrameStyle(gtFrameStyle* fstyle)
{
	textFrame->Cols = fstyle->getColumns();
	textFrame->ColGap = fstyle->getColumnsGap();
	textFrame->setFillColor(parseColor(fstyle->getBgColor()));
	textFrame->setFillShade(fstyle->getBgShade());
	textFrame->TabValues = QValueList<double>(*(fstyle->getTabValues()));
	
// 	gtParagraphStyle* pstyle = new gtParagraphStyle(*fstyle);
// 	int pstyleIndex = findParagraphStyle(pstyle);
// 	if (pstyleIndex == -1)
// 		pstyleIndex = 0;
// 	textFrame->Doc->currentParaStyle = pstyleIndex;

	double linesp;
	if (fstyle->getAutoLineSpacing())
		linesp = getLineSpacing(fstyle->getFont()->getSize());
	else
		linesp = fstyle->getLineSpacing();
	textFrame->LineSp = linesp;

	gtFont* font = fstyle->getFont();
	QString fontName = validateFont(font);
	textFrame->IFont = fontName;
	textFrame->ISize = font->getSize();
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
	fstyle->setBgColor(textFrame->fillColor());
	fstyle->setBgShade(textFrame->fillShade());
	
	struct ParagraphStyle vg = textFrame->Doc->docParagraphStyles[textFrame->Doc->currentParaStyle];
	fstyle->setName(vg.Vname);
	fstyle->setLineSpacing(vg.LineSpa);
	fstyle->setAlignment(vg.textAlignment);
	fstyle->setIndent(vg.Indent);
	fstyle->setFirstLineIndent(vg.First);
	fstyle->setSpaceAbove(vg.gapBefore);
	fstyle->setSpaceBelow(vg.gapAfter);
	fstyle->setDropCap(vg.Drop);
	fstyle->setDropCapHeight(vg.DropLin);
	fstyle->setAdjToBaseline(vg.BaseAdj);
	
	gtFont font;
	getFrameFont(&font);
	fstyle->setFont(font);
	fstyle->setName("Default frame style");
}

void gtAction::createParagraphStyle(gtParagraphStyle* pstyle)
{
	if (textFrame->Doc->docParagraphStyles.size() > 5)
	{
		for (uint i = 5; i < textFrame->Doc->docParagraphStyles.size(); ++i)
		{
			if (textFrame->Doc->docParagraphStyles[i].Vname == pstyle->getName())
				return;
		}
	}
	gtFont* font = pstyle->getFont();
	struct ParagraphStyle vg;
	vg.Vname = pstyle->getName();
	double linesp;
	if (pstyle->getAutoLineSpacing())
		linesp = getLineSpacing(pstyle->getFont()->getSize());
	else
		linesp = pstyle->getLineSpacing();
	vg.LineSpa = linesp;
	vg.textAlignment = pstyle->getAlignment();
	vg.Indent = pstyle->getIndent();
	vg.First = pstyle->getFirstLineIndent();
	vg.gapBefore = pstyle->getSpaceAbove();
	vg.gapAfter = pstyle->getSpaceBelow();
	vg.Font = validateFont(font);
	vg.FontSize = font->getSize();
	vg.TabValues.clear();
	QValueList<double> *tabs = pstyle->getTabValues();
	for (uint i = 0; i < tabs->size(); ++i)
	{
		double tmp = (*tabs)[i];
		vg.TabValues.append(tmp);
	}
	vg.Drop = pstyle->hasDropCap();
	vg.DropLin = pstyle->getDropCapHeight();
	vg.FontEffect = font->getEffectsValue();
	vg.FColor = parseColor(font->getColor());
	vg.FShade = font->getShade();
	vg.SColor = parseColor(font->getStrokeColor());
	vg.SShade = font->getStrokeShade();
	vg.BaseAdj = pstyle->isAdjToBaseline();
	textFrame->Doc->docParagraphStyles.append(vg);
	ScApp->propertiesPalette->Spal->updateFormatList();
}

void gtAction::removeParagraphStyle(const QString& name)
{
	int index = findParagraphStyle(name);
	if (index != -1)
		removeParagraphStyle(index);
}

void gtAction::removeParagraphStyle(int index)
{
	QValueList<ParagraphStyle>::iterator it = ScApp->doc->docParagraphStyles.at(index);
	ScApp->doc->docParagraphStyles.remove(it);
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
	struct ParagraphStyle vg;
	vg.Vname = pstyle->getName();
	double linesp;
	if (pstyle->getAutoLineSpacing())
		linesp = getLineSpacing(pstyle->getFont()->getSize());
	else
		linesp = pstyle->getLineSpacing();
	vg.LineSpa = linesp;
	vg.textAlignment = pstyle->getAlignment();
	vg.Indent = pstyle->getIndent();
	vg.First = pstyle->getFirstLineIndent();
	vg.gapBefore = pstyle->getSpaceAbove();
	vg.gapAfter = pstyle->getSpaceBelow();
	vg.Font = validateFont(font);
	vg.FontSize = font->getSize();
	vg.TabValues.clear();
	QValueList<double> *tabs = pstyle->getTabValues();
	for (uint i = 0; i < tabs->size(); ++i)
	{
		double tmp = (*tabs)[i];
		vg.TabValues.append(tmp);
	}
	vg.Drop = pstyle->hasDropCap();
	vg.DropLin = pstyle->getDropCapHeight();
	vg.FontEffect = font->getEffectsValue();
	vg.FColor = parseColor(font->getColor());
	vg.FShade = font->getShade();
	vg.SColor = parseColor(font->getStrokeColor());
	vg.SShade = font->getStrokeShade();
	vg.BaseAdj = pstyle->isAdjToBaseline();
	ScApp->doc->docParagraphStyles[pstyleIndex] = vg;
}

QString gtAction::validateFont(gtFont* font)
{
	// Dirty hack for family Times New Roman
	if (font->getFamily() == "Times New")
	{
		font->setFamily("Times New Roman");
		if (font->getWeight() == "Roman")
			font->setWeight("Regular");
	}

	QString useFont = font->getName();
	if ((useFont == NULL) || (useFont == ""))
		useFont = textFrame->IFont;
	else if (ScApp->Prefs.AvailFonts[font->getName()] == 0)
	{
		bool found = false;
		useFont == NULL;
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
				if (!ScApp->Prefs.GFontSub.contains(font->getName()))
				{
					MissingFont *dia = new MissingFont(0, useFont, &ScApp->Prefs, ScApp->doc);
					dia->exec();
					useFont = dia->getReplacementFont();
					ScApp->Prefs.GFontSub[font->getName()] = useFont;
					delete dia;
				}
				else
					useFont = ScApp->Prefs.GFontSub[font->getName()];
			}
		}
	}

	if(!ScApp->doc->UsedFonts.contains(useFont))
		ScApp->doc->AddFont(useFont, ScApp->Prefs.AvailFonts[useFont]->Font);
	return useFont;
}

QString gtAction::findFontName(gtFont* font)
{
	QString ret = NULL;
	for (uint i = 0; i < static_cast<uint>(gtFont::NAMECOUNT); ++i)
	{
		QString nname = font->getName(i);
		if (ScApp->Prefs.AvailFonts[nname] != 0)
		{
			ret = nname;
			break;
		}
	}
	return ret;
}

double gtAction::getLineSpacing(int fontSize)
{
	return ((fontSize / 10.0) * static_cast<double>(ScApp->doc->typographicSetttings.autoLineSpacing) / 100) + (fontSize / 10.0);
}

double gtAction::getFrameWidth()
{
	return textFrame->Width;
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
	QString ret = "None";
	if (s == "None")
		return ret; // don't want None to become Black or any color
	bool found = false;
	ColorList::Iterator it;
	for (it = ScApp->doc->PageColors.begin(); it != ScApp->doc->PageColors.end(); ++it)
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
		for (it = ScApp->doc->PageColors.begin(); it != ScApp->doc->PageColors.end(); ++it)
		{
			if (c == ScApp->doc->PageColors[it.key()].getRGBColor())
			{
				ret = it.key();
				found = true;
			}
		}
		if (!found)
		{
			CMYKColor tmp;
			tmp.fromQColor(c);
			ScApp->doc->PageColors.insert("FromGetText"+c.name(), tmp);
			ScApp->propertiesPalette->Cpal->SetColors(ScApp->doc->PageColors);
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
	if (ScApp->doc->docHyphenator->AutoCheck)
		ScApp->doc->docHyphenator->slotHyphenate(textFrame);
	ScApp->view->DrawNew();
	ScApp->slotDocCh();
}

gtAction::~gtAction()
{
	finalize();
}
