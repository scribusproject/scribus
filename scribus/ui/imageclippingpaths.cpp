/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "imageclippingpaths.h"

#include <QWidget>

#include "pageitem.h"
#include "scpainter.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"

ImageClippingPaths::ImageClippingPaths(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	pathList->setIconSize(QSize(40, 40));

}

void ImageClippingPaths::setCurrentItem(PageItem *item, ScribusView *view )
{
	m_view  = view;
	m_item = item;

	disconnect(pathList, SIGNAL( itemClicked(QListWidgetItem*) ), this, SLOT( selectPath(QListWidgetItem*) ) );
	disconnect(resetPath, SIGNAL(clicked()), this, SLOT(noPath()));

	pathList->clear();

	if (!m_item || !m_item->pixm.imgInfo.valid || m_item->pixm.imgInfo.PDSpathData.count() == 0)
		return;

	ImageInfoRecord *info = &m_item->pixm.imgInfo;
	QSize s = pathList->iconSize();
	int space = 3;

	QMap<QString, FPointArray>::Iterator it;
	for (it = info->PDSpathData.begin(); it != info->PDSpathData.end(); ++it)
	{
		QImage pixm(s.width(), s.height(), QImage::Format_ARGB32_Premultiplied);
		ScPainter *p = new ScPainter(&pixm, s.width(), s.height());
		p->clear();
		p->translate(space, space);
		if (it.key() == info->clipPath)
		{
			pixm.fill(Qt::green);
			p->clear(Qt::green);
		}
		else
			pixm.fill(Qt::white);
		FPointArray Path;
		Path = info->PDSpathData[it.key()].copy();
		FPoint min = getMinClipF(&Path);
		Path.translate(-min.x(), -min.y());
		FPoint max = Path.widthHeight();
		QTransform mm;
		mm.scale( (s.width() - space * 2) / qMax(max.x(), max.y()), (s.height() - space * 2) / qMax(max.x(), max.y()));
		Path.map(mm);
		p->setupPolygon(&Path);
		p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setBrush(Qt::white);
		p->setFillMode(ScPainter::None);
		p->setStrokeMode(ScPainter::Solid);
		p->strokePath();
		p->end();
		delete p;
		QPixmap pm = QPixmap::fromImage(pixm);
		new QListWidgetItem(QIcon(pm), it.key(), pathList);
		if (it.key() == info->usedPath)
		{
			pathList->setCurrentRow(pathList->count() - 1);
			pathList->currentItem()->setSelected(true);
		}
	}

	connect(pathList, SIGNAL( itemClicked(QListWidgetItem*) ), this, SLOT( selectPath(QListWidgetItem*) ) );
	connect(resetPath, SIGNAL(clicked()), this, SLOT(noPath()));

}


void ImageClippingPaths::noPath()
{
	QSignalBlocker list(pathList);
	pathList->clearSelection();

	m_item->imageClip.resize(0);
	m_item->pixm.imgInfo.usedPath.clear();
	m_item->update();

	updatePreview();

}

void ImageClippingPaths::selectPath(QListWidgetItem *c)
{
	if ((c != nullptr))
	{
		m_item->imageClip = m_item->pixm.imgInfo.PDSpathData[c->text()].copy();
		m_item->pixm.imgInfo.usedPath = c->text();
		QTransform cl;
		cl.translate(m_item->imageXOffset()*m_item->imageXScale(), m_item->imageYOffset()*m_item->imageYScale());
		cl.rotate(m_item->imageRotation());
		cl.scale(m_item->imageXScale(), m_item->imageYScale());
		m_item->imageClip.map(cl);
		m_item->update();

		updatePreview();
	}
}

void ImageClippingPaths::updatePreview()
{
	PageItem::TextFlowMode flowMode = m_item->textFlowMode();

	if (pathList->selectedItems().count() == 0 && flowMode == PageItem::TextFlowUsesImageClipping)
		m_item->setTextFlowMode(PageItem::TextFlowDisabled);

	m_view->m_doc->changed();
	m_view->m_doc->changedPagePreview();
}
