/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "textcontext.h"
#include "pageitem.h"
#include "sctextstruct.h"
#include "style.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "scribusdoc.h"
#include "textnote.h"
#include "util.h"

const ScribusDoc* TextContext::getDoc() const
{
	return m_frame->doc();
}

const CharStyle& TextContext::charStyle() const 
{ 
	static CharStyle emptyCStyle;
	return emptyCStyle; 
}

const ParagraphStyle& TextContext::paragraphStyle() const 
{ 
	static ParagraphStyle emptyPStyle;
	return emptyPStyle; 
}

PageItem* TextContext::object(const InlineFrame& frame)  const
{ 
	return frame.getPageItem(m_frame->doc());
}

QRectF TextContext::getVisualBoundingBox(const InlineFrame& frame)  const
{
	return object(frame)->getVisualBoundingRect();
}
	
QString TextContext::expand(const ExpansionPoint& expansion)
{
	ScribusDoc* doc = m_frame->doc();
	switch (expansion.getType())
	{
		case ExpansionPoint::PageNumber:
			if (m_frame->OwnPage != -1)
			{
				int pageNumber = m_frame->OwnPage;
				if (!m_frame->OnMasterPage.isEmpty() && m_frame->OwnPage >= doc->DocPages.count())
					pageNumber = 0;
				return QString("%1").arg(doc->getSectionPageNumberForPageIndex(pageNumber),
										 doc->getSectionPageNumberWidthForPageIndex(pageNumber),
										 doc->getSectionPageNumberFillCharForPageIndex(pageNumber));
			}
			return "#";
		case ExpansionPoint::PageCount:
			{
				int key = doc->getSectionKeyForPageIndex(m_frame->OwnPage);
				if (key == -1)
					return "%";
				return QString("%1").arg(getStringFromSequence(doc->sections()[key].type, doc->sections()[key].toindex - doc->sections()[key].fromindex + 1));
			}
		case ExpansionPoint::MarkCE:
			{
				Mark* mark = expansion.getMark();
				if (mark != nullptr)
				{
					mark->OwnPage = m_frame->OwnPage;
					//itemPtr and itemName set to this frame only if mark type is different than MARK2ItemType
					if (!mark->isType(MARK2ItemType))
					{
						mark->setItemPtr(m_frame);
						mark->setItemName(m_frame->itemName());
					}

					//anchors and indexes has no visible inserts in text
					if (mark->isType(MARKAnchorType) || mark->isType(MARKIndexType))
						return QString();

					//set note marker charstyle
					if (mark->isNoteType())
					{
						mark->setItemPtr(m_frame);

						const TextNote* note = mark->getNotePtr();
						if (note == nullptr)
							return QString();
					}
					if (!mark->isType(MARKAnchorType) && !mark->isType(MARKIndexType))
						return mark->getString();
				}
			}
			break;
		case ExpansionPoint::Invalid:
		case ExpansionPoint::ListBullet:
		case ExpansionPoint::ListCounter:
		case ExpansionPoint::Note:
		case ExpansionPoint::Anchor:
		case ExpansionPoint::PageRef:
		case ExpansionPoint::Lookup:
		case ExpansionPoint::SectionRef:
			break;
	}
	return QString();
}

const TypoPrefs& TextContext::typographicPrefs() const
{
	return  m_frame->doc()->typographicPrefs();
}
