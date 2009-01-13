//
// C++ Implementation: pageitempreview
//
// Description: 
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "pageitempreview.h"
#include "pageitem.h"

#include <QMutexLocker>

PageItemPreview::PageItemPreview(PageItem * pi)
	:m_pageitem(pi), m_completed(false)
{
	start();
}

PageItemPreview::~ PageItemPreview()
{
}

bool PageItemPreview::isReady() const
{
	return m_completed;
}

QImage * PageItemPreview::getImage()
{
	if(m_mutex.tryLock())
	{
		m_mutex.unlock();
		if(m_completed)
		{
			return &m_image;
		}
	}
	return 0;
}

void PageItemPreview::run()
{
	QMutexLocker locker(&m_mutex);
	
	m_image = m_pageitem->DrawObj_toImage();
	m_completed = true;
	
}
