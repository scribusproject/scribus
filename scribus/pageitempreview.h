//
// C++ Interface: pageitempreview
//
// Description: 
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef PAGEITEMPREVIEW_H
#define PAGEITEMPREVIEW_H

#include <QThread>
#include <QMutex>
#include <QImage>

class PageItem;
class PageItemPreview : public QThread
{
	public:
		PageItemPreview(PageItem* pi);
		~PageItemPreview();
		
		bool isReady() const;
		QImage * getImage();
		
	private:
		PageItem * m_pageitem;
		bool m_completed;
		QImage m_image;
		
		QMutex m_mutex;
		
		void run();
};

#endif // PAGEITEMPREVIEW_H

