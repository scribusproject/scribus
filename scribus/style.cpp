/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */


#include <qvaluelist.h>
#include <qobject.h>
#include "sctextstruct.h"
#include "scfonts.h"


const QString CharStyle::NOCOLOR; 
const QString CharStyle::NOLANG; 

StyleFlag& operator&= (StyleFlag& left, StyleFlag right){        
	int result = static_cast<int>(left) & static_cast<int>(right);        
	left = static_cast<StyleFlag>(result);
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
	int result = static_cast<int>(left) & static_cast<int>(right);        
	return static_cast<StyleFlag>(result);
}

StyleFlag operator| (StyleFlag left, StyleFlag right)
{        
	int result = static_cast<int>(left) | static_cast<int>(right);
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


void CharStyle::applyCharStyle(const CharStyle & other)
{
	if (other.parent() != NULL)
		setParent(other.parent());
	if (other.csize != NOVALUE)
		csize = other.csize;
	if (other.cshade != NOVALUE)
		cshade = other.cshade;
	if (other.cshade2 != NOVALUE)
		cshade2 = other.cshade2;
	if (other.cstyle != ScStyle_None)
		cstyle = static_cast<StyleFlag>((cstyle & ~ScStyle_UserStyles) | (other.cstyle & ScStyle_UserStyles));
	if (other.cscale != NOVALUE)
		cscale = other.cscale;
	if (other.cscalev != NOVALUE)
		cscalev = other.cscalev;
	if (other.cbase != NOVALUE)
		cbase = other.cbase;
	if (other.cshadowx != NOVALUE)
		cshadowx = other.cshadowx;
	if (other.cshadowy != NOVALUE)
		cshadowy = other.cshadowy;
	if (other.coutline != NOVALUE)
		coutline = other.coutline;
	if (other.cunderpos != NOVALUE)
		cunderpos = other.cunderpos;
	if (other.cunderwidth != NOVALUE)
		cunderwidth = other.cunderwidth;
	if (other.cstrikepos != NOVALUE)
		cstrikepos = other.cstrikepos;
	if (other.cstrikewidth != NOVALUE)
		cstrikewidth = other.cstrikewidth;
	if (other.cextra != NOVALUE)
		cextra = other.cextra;
	if (!other.cfont.isNone())
		cfont = other.cfont;
	if (other.ccolor != NOCOLOR)
		ccolor = other.ccolor;
	if (other.cstroke != NOCOLOR)
		cstroke = other.cstroke;
	if (other.language_ != NOLANG)
		language_ = other.language_;
}


void CharStyle::eraseCharStyle(const CharStyle & other)
{
	if (other.parent() == parent())
		setParent(NULL);
	if (other.csize == csize)
		csize = NOVALUE;
	if (other.cshade == cshade)
		cshade = NOVALUE;
	if (other.cshade2 == cshade2)
		cshade2 = NOVALUE;
	if ((other.cstyle  & ScStyle_UserStyles) == (cstyle & ScStyle_UserStyles))
		cstyle = ScStyle_None;
	if (other.cscale == cscale)
		cscale = NOVALUE;
	if (other.cscalev == cscalev)
		cscalev = NOVALUE;
	if (other.cbase == cbase)
		cbase = NOVALUE;
	if (other.cshadowx == cshadowx)
		cshadowx = NOVALUE;
	if (other.cshadowy == cshadowy)
		cshadowy = NOVALUE;
	if (other.coutline == coutline)
		coutline = NOVALUE;
	if (other.cunderpos == cunderpos)
		cunderpos = NOVALUE;
	if (other.cunderwidth == cunderwidth)
		cunderwidth = NOVALUE;
	if (other.cstrikepos == cstrikepos)
		cstrikepos = NOVALUE;
	if (other.cstrikewidth == cstrikewidth)
		cstrikewidth = NOVALUE;
	if (other.cextra == cextra)
		cextra = NOVALUE;
	if (other.cfont == cfont)
		cfont = ScFace::none();
	if (other.ccolor == ccolor)
		ccolor = NOCOLOR;
	if (other.cstroke == cstroke)
		cstroke = NOCOLOR;
	if (other.language_ == language_)
		language_ = NOLANG;
}

bool CharStyle::equiv(const Style & other) const
{
	const CharStyle * oth = dynamic_cast<const CharStyle*> ( & other );
	return  (oth &&
			 csize == oth->csize &&
			 cshade == oth->cshade &&
			 cshade2 == oth->cshade2 &&
			 cstyle == oth->cstyle &&
			 cscale == oth->cscale &&
			 cscalev == oth->cscalev &&
			 cbase == oth->cbase &&
			 cshadowx == oth->cshadowx &&
			 cshadowy == oth->cshadowy &&
			 coutline == oth->coutline &&
			 cunderpos == oth->cunderpos &&
			 cunderwidth == oth->cunderwidth &&
			 cstrikepos == oth->cstrikepos &&
			 cstrikewidth == oth->cstrikewidth &&
			 cextra == oth->cextra &&
			 cfont == oth->cfont &&
			 ccolor == oth->ccolor &&
			 cstroke == oth->cstroke &&	
			 language_ == oth->language_ );	
}


QString CharStyle::displayName() const
{
	if ( hasName() || !hasParent() )
		return name();
//	else if ( inheritsAll() )
//		return parent()->displayName();
	else 
		return parent()->displayName() + "+";
}

/*
bool CharStyle::definesAll() const
{
	return definesLineSpacing() && 
	definesLeftMargin() && 
	definesRightMargin() && 
	definesFirstIndent() &&
	definesAlignment() && 
	definesGapBefore()  &&
	definesLineSpacingMode()  && 
	definesGapAfter()  && 
	definesHasDropCap() && 
	definesDropCapOffset() && 
	definesDropCapLines() && 
	definesUseBaselineGrid() && 
	charStyle().definesAll() ;
	
}

bool CharStyle::inheritsAll() const
{
	return inheritsLineSpacing() && 
	inheritsLeftMargin() && 
	inheritsRightMargin() && 
	inheritsFirstIndent() &&
	inheritsAlignment() && 
	inheritsGapBefore()  &&
	inheritsLineSpacingMode()  && 
	inheritsGapAfter()  && 
	inheritsHasDropCap() && 
	inheritsDropCapOffset() && 
	inheritsDropCapLines() && 
	inheritsUseBaselineGrid() && 
	charStyle().inheritsAll() ;
}
*/

ParagraphStyle::ParagraphStyle() : Style(), cstyle(),
		LineSpaMode(static_cast<LineSpacingMode>(NOVALUE)),
		LineSpa(NOVALUE - 0.1),
		textAlignment(NOVALUE),
		Indent(NOVALUE - 0.1),
		rightMargin_(NOVALUE - 0.1),
		First(NOVALUE - 0.1),
		gapBefore_(NOVALUE - 0.1),
		gapAfter_(NOVALUE - 0.1),
		TabValues(),
		haveTabs(false),
		Drop(NOVALUE),
		DropLin(NOVALUE),
		DropDist(NOVALUE - 0.1),
		BaseAdj(NOVALUE)
{}


ParagraphStyle::ParagraphStyle(const ParagraphStyle& other) : Style(other), cstyle(other.charStyle()),
		LineSpaMode(other.LineSpaMode),
		LineSpa(other.LineSpa),
		textAlignment(other.textAlignment),
		Indent(other.Indent),
		rightMargin_(other.rightMargin_),
		First(other.First),
		gapBefore_(other.gapBefore_),
		gapAfter_(other.gapAfter_),
		TabValues(other.TabValues),
		haveTabs(other.haveTabs),
		Drop(other.Drop),
		DropLin(other.DropLin),
		DropDist(other.DropDist),
		BaseAdj(other.BaseAdj)
{}

QString ParagraphStyle::displayName() const
{
	if ( hasName() || !hasParent() )
		return name();
	//	else if ( inheritsAll() )
	//		return parent()->displayName();
	else 
		return parent()->displayName() + "+";
}

static bool sameTabs(const QValueList<ParagraphStyle::TabRecord>& tabs, const QValueList<ParagraphStyle::TabRecord>& other)
{
	ParagraphStyle::TabRecord tb;
	bool tabEQ = false;
	if ((other.count() == 0) && (tabs.count() == 0))
		tabEQ = true;
	else
	{
		for (uint t1 = 0; t1 < other.count(); t1++)
		{
			tb.tabPosition = other[t1].tabPosition;
			tb.tabType = other[t1].tabType;
			tb.tabFillChar = other[t1].tabFillChar;
			for (uint t2 = 0; t2 < tabs.count(); t2++)
			{
				ParagraphStyle::TabRecord tb2;
				tb2.tabPosition = tabs[t2].tabPosition;
				tb2.tabType = tabs[t2].tabType;
				tb2.tabFillChar = tabs[t2].tabFillChar;
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
	return tabEQ;
}


bool ParagraphStyle::equiv(const Style& other) const
{
	const ParagraphStyle* oth = dynamic_cast<const ParagraphStyle*> ( & other );
	return oth &&
		sameTabs(tabValues(), oth->tabValues()) &&
		(lineSpacing() == oth->lineSpacing()) && 
		(leftMargin() == oth->leftMargin()) && 
		(rightMargin() == oth->rightMargin()) && 
		(firstIndent() == oth->firstIndent()) &&
		(alignment() == oth->alignment()) && 
		(gapBefore() == oth->gapBefore()) &&
		(lineSpacingMode() == oth->lineSpacingMode()) && 
		(gapAfter() == oth->gapAfter()) && 
		(hasDropCap() == oth->hasDropCap()) && 
		(dropCapOffset() == oth->dropCapOffset()) && 
		(dropCapLines() == oth->dropCapLines()) && 
		(useBaselineGrid() == oth->useBaselineGrid()) && 
		(charStyle() == oth->charStyle());
}	



ParagraphStyle& ParagraphStyle::operator=(const ParagraphStyle& other) 
{
	setParent(other.parent());
	cstyle = other.charStyle();
	LineSpaMode = other.LineSpaMode;
	LineSpa = (other.LineSpa);
	textAlignment = (other.textAlignment);
	Indent = (other.Indent);
	rightMargin_ = (other.rightMargin_);
	First = (other.First);
	gapBefore_ = (other.gapBefore_);
	gapAfter_ = (other.gapAfter_);
	TabValues = (other.TabValues);
	haveTabs = other.haveTabs;
	Drop = (other.Drop);
	DropLin = (other.DropLin);
	DropDist = (other.DropDist);
	BaseAdj = (other.BaseAdj);
	return *this;
}


void ParagraphStyle::applyStyle(const ParagraphStyle& other) 
{
	if (other.parent() != NULL)
		setParent(other.parent());
	cstyle.applyCharStyle(other.charStyle());
	if (other.LineSpaMode != NOVALUE)
		LineSpaMode = other.LineSpaMode;
	if (other.LineSpa >= 0)
		LineSpa = (other.LineSpa);
	if (other.textAlignment != NOVALUE)
		textAlignment = (other.textAlignment);
	if (other.Indent >= NOVALUE) 
		Indent = (other.Indent);
	if (other.rightMargin_ >= NOVALUE)
		rightMargin_ = (other.rightMargin_);
	if (other.First >= NOVALUE)
		First = (other.First);
	if (other.gapBefore_ >= NOVALUE)
		gapBefore_ = (other.gapBefore_);
	if (other.gapAfter_ >= NOVALUE)
		gapAfter_ = (other.gapAfter_);
	if (other.haveTabs) {
		TabValues = other.TabValues;
		haveTabs = true;
	}
	if (other.Drop != NOVALUE)
		Drop = (other.Drop);
	if (other.DropLin != NOVALUE)
		DropLin = (other.DropLin);
	if (other.DropDist >= NOVALUE)
		DropDist = (other.DropDist);
	if (other.BaseAdj != NOVALUE)
		BaseAdj = (other.BaseAdj);
}


void ParagraphStyle::eraseStyle(const ParagraphStyle& other) 
{
	if (other.parent() == parent())
		setParent(NULL);
	cstyle.eraseCharStyle(other.charStyle());
	if (other.LineSpaMode == LineSpaMode)
		LineSpaMode = static_cast<LineSpacingMode>(NOVALUE);
	if (other.LineSpa == LineSpa)
		LineSpa = NOVALUE - 0.1;
	if (other.textAlignment == textAlignment)
		textAlignment = NOVALUE;
	if (other.Indent == Indent) 
		Indent = NOVALUE - 0.1;
	if (other.rightMargin_ == rightMargin_)
		rightMargin_ = NOVALUE - 0.1;
	if (other.First == First)
		First = NOVALUE - 0.1;
	if (other.gapBefore_ == gapBefore_)
		gapBefore_ = NOVALUE - 0.1;
	if (other.gapAfter_ == gapAfter_)
		gapAfter_ = NOVALUE - 0.1;
	if (sameTabs(other.TabValues, TabValues)) {
		TabValues.clear();
		haveTabs = false;
	}
	if (other.Drop == Drop)
		Drop = NOVALUE;
	if (other.DropLin == DropLin)
		DropLin = NOVALUE;
	if (other.DropDist == DropDist)
		DropDist = NOVALUE - 0.1;
	if (other.BaseAdj == BaseAdj)
		BaseAdj = NOVALUE;
}


