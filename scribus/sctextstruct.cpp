/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <qvaluelist.h>
#include "sctextstruct.h"
#include "scfonts.h"

StyleFlag& operator&= (StyleFlag& left, StyleFlag right){        int result = static_cast<int>(left) & static_cast<int>(right);        left = static_cast<StyleFlag>(result);
        return left;
}
StyleFlag& operator|= (StyleFlag& left, StyleFlag right)
{
        int result = static_cast<int>(left) | static_cast<int>(right);
        left = static_cast<StyleFlag>(result);
        return left;
}
StyleFlag operator& (StyleFlag left, StyleFlag right)
{
        int result = static_cast<int>(left) & static_cast<int>(right);        return static_cast<StyleFlag>(result);
}

StyleFlag operator| (StyleFlag left, StyleFlag right)
{        int result = static_cast<int>(left) | static_cast<int>(right);
        return static_cast<StyleFlag>(result);
}

StyleFlag operator^ (StyleFlag left, StyleFlag right){
        int result = static_cast<int>(left) ^ static_cast<int>(right);
        return static_cast<StyleFlag>(result);
}

StyleFlag operator~ (StyleFlag arg)
{
        int result = ~ static_cast<int>(arg);
        return static_cast<StyleFlag>(result);
}

ParagraphStyle::ParagraphStyle() : 
	Vname(),
	pparent_(NULL),
	LineSpaMode(FixedLineSpacing),
	LineSpa(0),
	textAlignment(0),
	Indent(0),
	rightMargin_(0),
	First(0),
	gapBefore_(0),
	gapAfter_(0),
	TabValues(), 
	Drop(false),
	DropLin(2),
	DropDist(0),
	BaseAdj(false)
{}

bool ParagraphStyle::equiv(const ParagraphStyle& other) const
{
	ParagraphStyle::TabRecord tb;
	bool tabEQ = false;
	if ((other.tabValues().count() == 0) && (tabValues().count() == 0))
		tabEQ = true;
	else
	{
		for (uint t1 = 0; t1 < other.tabValues().count(); t1++)
		{
			tb.tabPosition = other.tabValues()[t1].tabPosition;
			tb.tabType = other.tabValues()[t1].tabType;
			tb.tabFillChar = other.tabValues()[t1].tabFillChar;
			for (uint t2 = 0; t2 < tabValues().count(); t2++)
			{
				ParagraphStyle::TabRecord tb2;
				tb2.tabPosition = tabValues()[t2].tabPosition;
				tb2.tabType = tabValues()[t2].tabType;
				tb2.tabFillChar = tabValues()[t2].tabFillChar;
				if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
				{
					tabEQ = true;
					break;
				}
			}
			if (tabEQ)
				break;
		}
	}
	return tabEQ &&
		(lineSpacing() == other.lineSpacing()) && 
		(leftMargin() == other.leftMargin()) && 
		(rightMargin() == other.rightMargin()) && 
		(firstIndent() == other.firstIndent()) &&
		(alignment() == other.alignment()) && 
		(gapBefore() == other.gapBefore()) &&
		(lineSpacingMode() == other.lineSpacingMode()) && 
		(gapAfter() == other.gapAfter()) && 
		(hasDropCap() == other.hasDropCap()) && 
		(dropCapOffset() == other.dropCapOffset()) && 
		(dropCapLines() == other.dropCapLines()) && 
		(useBaselineGrid() == other.useBaselineGrid()) && 
		(charStyle() == other.charStyle());
}	

#define tr QString

QString CharStyle::asString() const
{
	QString result;
	if (cfont != NULL)
		result += tr("font %1 ").arg(cfont->scName());
	if (csize != NOVALUE)
		result += tr("size %1 ").arg(csize);
	if (cstyle != NOVALUE)
		result += tr("+style ");
	if (ccolor != NOCOLOR || cstroke != NOCOLOR || cshade != NOVALUE || cshade2 != NOVALUE)
		result += tr("+color ");
	if (cunderwidth != NOVALUE || cunderpos != NOVALUE)
		result += cunderwidth > 0 ? tr("+underline ") : tr("-underline ");
	if (cstrikewidth != NOVALUE || cstrikepos != NOVALUE)
		result += cstrikewidth > 0 ? tr("+strikeout ") : tr("-strikeout ");
	if (cshadowx != NOVALUE || cshadowy != NOVALUE)
		result += cshadowx != 0 && cshadowy != 0? tr("+shadow ") : tr("-shadow ");
	if (coutline != NOVALUE)
		result += coutline > 0 ? tr("+outline ") : tr("-outline ");
	if (cextra != NOVALUE)
		result += cextra > 0 ? tr("+tracking %1").arg(cextra) : tr("-tracking ");
	if (cbase != NOVALUE)
		result += tr("+baseline %1").arg(cbase);
	if (cscale != NOVALUE || cscalev != NOVALUE)
		result += tr("+stretch ");
	if (cparent_ != NULL)
		result += tr("parent= %1").arg(cparent_->cname_=="" ? tr("unnamed") : cparent_->cname_);
	return result.stripWhiteSpace();
}
