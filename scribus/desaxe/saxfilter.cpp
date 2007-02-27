
#include "saxfilter.h"


void SaxFilter::beginDoc()
{
	m_delegate->beginDoc();
}

void SaxFilter::endDoc()
{
	m_delegate->endDoc();
}

void SaxFilter::begin(Xml_string tag, Xml_attr attr)
{
	m_delegate->begin(tag, attr);
}

void SaxFilter::end(Xml_string tag)
{
	m_delegate->end(tag);
}

void SaxFilter::chars(Xml_string text)
{
	m_delegate->chars(text);
}


KeepOpen::KeepOpen(SaxHandler* delegate) : SaxFilter(delegate), m_level(0) {}

void KeepOpen::begin(Xml_string tag, Xml_attr attr)
{
	++m_level;
	if (m_level == 1)
		m_tag = tag;
	SaxFilter::begin(tag, attr);
}

void KeepOpen::end(Xml_string tag)
{
	--m_level;
	if (m_level > 0)
		SaxFilter::end(tag);
}

Xml_string KeepOpen::openTag()
{
	return m_tag;
}

RenameElem::RenameElem(SaxHandler* delegate, Xml_string oldname, Xml_string newname)
: SaxFilter(delegate), m_level(0), m_old(oldname), m_new(newname) {}

RenameElem::RenameElem(SaxHandler* delegate, Xml_string newname)
: SaxFilter(delegate), m_level(0), m_old(""), m_new(newname) {}

void RenameElem::begin(Xml_string tag, Xml_attr attr)
{
	++m_level;
	if (m_level == 1 || m_old == tag)
		SaxFilter::begin(m_new, attr);
	else
		SaxFilter::begin(tag, attr);
}

void RenameElem::end(Xml_string tag)
{
	if (m_level == 1 || m_old == tag)
		SaxFilter::end(m_new);
	else
		SaxFilter::end(tag);
	--m_level;
}


AddAttributes::AddAttributes(SaxHandler* delegate, Xml_attr add) 
: SaxFilter(delegate), m_oneshot(false), m_attributes(add) {}


void AddAttributes::begin(Xml_string tag, Xml_attr attr)
{
	if ( !m_oneshot )
	{
		m_oneshot = true;
		Xml_attr::iterator it;
		for (it = attr.begin(); it != attr.end(); ++it)
			m_attributes[Xml_key(it)] = Xml_data(it);
		SaxFilter::begin(tag, m_attributes);
	}
	else
		SaxFilter::begin(tag, attr);
}

