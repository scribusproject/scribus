

ParagraphStyle::ParagraphStyle() : Style(), cstyle()
{
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = attr_DEFAULT; \
	inh_##attr_NAME = true;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}


ParagraphStyle::ParagraphStyle(const ParagraphStyle& other) : Style(other), cstyle(other.charStyle())
{
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}


QString ParagraphStyle::displayName() const
{
	if ( hasName() || !hasParent() || !m_base)
		return name();
	//	else if ( inheritsAll() )
	//		return parent()->displayName();
	else 
		return (parentStyle())->displayName() + "+";
}

namespace {
bool operator==(const QValueList<ParagraphStyle::TabRecord>& tabs, const QValueList<ParagraphStyle::TabRecord>& other)
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

} // namespace

bool ParagraphStyle::equiv(const Style& other) const
{
	const ParagraphStyle* oth = dynamic_cast<const ParagraphStyle*> ( & other );
	return  oth &&
		parent == oth->parent() &&
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		inh_##attr_NAME == oth->inh_##attr_NAME &&\
		(inh_##attr_NAME || m_##attr_NAME == oth->m_##attr_NAME) &&
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
		;
}	



ParagraphStyle& ParagraphStyle::operator=(const ParagraphStyle& other) 
{
	static_cast<Style&>(*this) = static_cast<const Style&>(other);
	cstyle = other.charStyle();
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = oth->m_##attr_NAME; \
	inh_##attr_NAME = oth->inh_##attr_NAME);
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	return *this;
}



void ParagraphStyle::update(StyleBase* base)
{
	Style::update(base);
	const ParagraphStyle * oth = dynamic_cast<const ParagraphStyle*> ( parentStyle() );
	if (oth) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		if (inh_##attr_NAME) \
			m_##attr_NAME = oth->##attr_GETTER;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	}
}



void ParagraphStyle::applyStyle(const ParagraphStyle& other) 
{
	Style::applyStyle(other);
	cstyle.applyCharStyle(other.charStyle());
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (! other.inh_##attr_NAME) \
		set##attr_NAME(other.m_##attr_NAME);
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}


void ParagraphStyle::eraseStyle(const ParagraphStyle& other) 
{
	Style::eraseStyle(other);
	cstyle.eraseCharStyle(other.charStyle());
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME && m_##attr_NAME == other.##attr_GETTER()) \
		reset##attr_NAME();
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}

