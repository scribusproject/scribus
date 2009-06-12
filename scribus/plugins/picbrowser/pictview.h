//
// C++ Interface: pictview
//
// Description: 
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef PICTVIEW_H
#define PICTVIEW_H

#include <QListView>

class PreviewImagesModel;

class PictView : public QListView
{
	public:
		PictView(QWidget * parent );
		
		void SetModel(PreviewImagesModel * pm);
		QItemSelectionModel *SelectionModel() const;
		void SetIconSize(const QSize& s);
		void SetGridSize(const QSize& s);
		
	protected:
		void startDrag( Qt::DropActions supportedActions );
		
};

#endif // PICTVIEW_H

