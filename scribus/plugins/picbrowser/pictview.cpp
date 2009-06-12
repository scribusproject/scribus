//
// C++ Implementation: pictview
//
// Description: 
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "pictview.h"
#include "previewimage.h"

#include <QMimeData>
#include <QDrag>

#include <QDebug>

PictView::PictView(QWidget * parent )
	:QListView(parent)
{
	setViewMode ( QListView::IconMode );
	setUniformItemSizes ( true );
	setContextMenuPolicy ( Qt::CustomContextMenu );
	setSelectionMode ( QAbstractItemView::SingleSelection );
	setMovement ( QListView::Static);
	setResizeMode ( QListView::Adjust );
	setSelectionRectVisible( false );
}

void PictView::SetModel(PreviewImagesModel * pm)
{
	this->setModel(pm);
}

void PictView::SetIconSize(const QSize & s)
{
	this->setIconSize(s);
}

void PictView::SetGridSize(const QSize & s)
{
	this->setGridSize(s);
}

QItemSelectionModel * PictView::SelectionModel() const
{
	return this->selectionModel(); 
}

void PictView::startDrag(Qt::DropActions supportedActions)
{
	QModelIndex mdx(currentIndex());
	QModelIndexList mil;
	if(mdx.isValid())
	{
		mil << mdx;
		PreviewImagesModel * pim(static_cast<PreviewImagesModel*>(model()));
		QMimeData *md = pim->mimeData(mil);
		QDrag *drag = new QDrag(this);
		drag->setMimeData(md);
		QIcon icn(pim->data(mdx, Qt::DecorationRole).value<QIcon>());
		if(!icn.isNull())
		{
			drag->setPixmap(icn.pixmap(64,64));
		}
		drag->start();
	}

}

