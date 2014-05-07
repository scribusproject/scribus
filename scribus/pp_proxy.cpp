/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pp_proxy.h"
#include "scribus.h"
#include "pageitem.h"

PP_Proxy::PP_Proxy(QObject *parent)
	: QObject(parent),
	  m_pageItem(NULL),
	  m_scribusDoc(NULL)
{
}

void PP_Proxy::setPageItem(PageItem *item)
{
	m_pageItem=item;
	if (m_scribusDoc!=m_pageItem->doc())
		m_scribusDoc=m_pageItem->doc();
}

void PP_Proxy::setDoc(ScribusDoc *doc)
{
	m_scribusDoc=doc;
	connectDoc();
}

void PP_Proxy::connectDoc()
{
	//connect(m_scribusDoc, SIGNAL(blah()), this, SLOT(blah2()));
}

void PP_Proxy::connectItem()
{
	//connect(m_pageItem, SIGNAL(blah()), this, SLOT(blah2()));
}

void PP_Proxy::publishDocChange()
{

}

void PP_Proxy::publishItemChange()
{

}

