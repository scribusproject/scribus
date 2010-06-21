/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "saxfilter.h"

void SaxFilter::beginDoc()
{
	m_delegate->beginDoc();
}

void SaxFilter::endDoc()
{
	m_delegate->endDoc();
}

void SaxFilter::begin(const Xml_string& tag, Xml_attr attr)
{
	m_delegate->begin(tag, attr);
}

void SaxFilter::end(const Xml_string& tag)
{
	m_delegate->end(tag);
}

void SaxFilter::chars(const Xml_string& text)
{
	m_delegate->chars(text);
}


KeepOpen::KeepOpen(SaxHandler* delegate) : SaxFilter(delegate), m_level(0) {}

void KeepOpen::begin(const Xml_string& tag, Xml_attr attr)
{
	++m_level;
	if (m_level == 1)
		m_tag = tag;
	SaxFilter::begin(tag, attr);
}

void KeepOpen::end(const Xml_string& tag)
{
	--m_level;
	if (m_level > 0)
		SaxFilter::end(tag);
}

Xml_string KeepOpen::openTag()
{
	return m_tag;
}

RenameElem::RenameElem(SaxHandler* delegate, const Xml_string& oldname, const Xml_string& newname)
: SaxFilter(delegate), m_level(0), m_old(oldname), m_new(newname) {}

RenameElem::RenameElem(SaxHandler* delegate, const Xml_string& newname)
: SaxFilter(delegate), m_level(0), m_old(""), m_new(newname) {}

void RenameElem::begin(const Xml_string& tag, Xml_attr attr)
{
	++m_level;
	if (m_level == 1 || m_old == tag)
		SaxFilter::begin(m_new, attr);
	else
		SaxFilter::begin(tag, attr);
}

void RenameElem::end(const Xml_string& tag)
{
	if (m_level == 1 || m_old == tag)
		SaxFilter::end(m_new);
	else
		SaxFilter::end(tag);
	--m_level;
}


AddAttributes::AddAttributes(SaxHandler* delegate, Xml_attr add) 
: SaxFilter(delegate), m_oneshot(false), m_attributes(add) {}


void AddAttributes::begin(const Xml_string& tag, Xml_attr attr)
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

