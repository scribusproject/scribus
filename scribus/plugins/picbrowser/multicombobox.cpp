/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "multicombobox.h"


multiComboboxModel::multiComboboxModel ( QObject* parent ) : QStandardItemModel ( parent )
{
	insertColumn ( 0 );
}

Qt::ItemFlags multiComboboxModel::flags ( const QModelIndex& index ) const
{
	return QStandardItemModel::flags ( index ) | Qt::ItemIsUserCheckable;
}


multiView::multiView ( QWidget* parent ) : QListView ( parent )
{
}


multiView::multiView ( multiCombobox* parent ) : QListView ( parent )
{
	parentMcb = parent;
}


bool multiView::eventFilter ( QObject* object, QEvent* event )
{
	if ( event->type() == QEvent::MouseButtonRelease )
	{
		QMouseEvent* mouse = static_cast<QMouseEvent*> ( event );
		QModelIndex index = indexAt ( mouse->pos() );

		if ( index.isValid() )
		{
			QModelIndexList selectedList = selectedIndexes();

			for ( int i = 0 ; i < selectedList.size() ; ++i )
			{
				parentMcb->switchCheckstate ( selectedList.at ( i ).row() );
			}
			return true;
		}
	}
	else if ( event->type() == QEvent::KeyPress )
	{
		QKeyEvent* key = static_cast<QKeyEvent*> ( event );

		if ( ( key->key() == Qt::Key_Return ) || ( key->key() == Qt::Key_Enter ) )
		{
			QModelIndexList selectedList = selectedIndexes();

			for ( int i = 0 ; i < selectedList.size() ; ++i )
			{
				parentMcb->switchCheckstate ( selectedList.at ( i ).row() );
			}

			return true;
		}
	}

	return false;
}



multiCombobox::multiCombobox ( QWidget *parent ) : QComboBox ( parent )
{
	mcbModel = new multiComboboxModel ( this );
	mcbView = new multiView ( this );

	setModel ( mcbModel );
	setView ( mcbView );

	mcbView->installEventFilter ( mcbView );
	mcbView->viewport()->installEventFilter ( mcbView );

//setEditable(true);
//lineEdit()->setReadOnly(true);
//lineEdit()->setEnabled(false);
	view()->setSelectionMode ( QAbstractItemView::ExtendedSelection );
}


void multiCombobox::setCheckstate ( int index, int checked )
{
	int itemsCount = count();

	if ( index < 0 || index >= itemsCount )
	{
		return;
	}

	if ( checked == 1 )
	{
		QComboBox::setItemData ( index, Qt::Checked, Qt::CheckStateRole );
	}
	else if ( checked == 2 )
	{
		QComboBox::setItemData ( index, Qt::PartiallyChecked, Qt::CheckStateRole );
	}
	else
	{
		QComboBox::setItemData ( index, Qt::Unchecked, Qt::CheckStateRole );
	}
}


int multiCombobox::checkstate ( int index )
{
	int itemsCount = count();

	if ( index < 0 || index >= itemsCount )
	{
		return false;
	}

	QVariant var = QComboBox::itemData ( index, Qt::CheckStateRole );

	if ( var == Qt::Checked )
	{
		return 1;
	}
	else if ( var == Qt::PartiallyChecked )
	{
		return 2;
	}
	else
	{
		return 0;
	}
}


void multiCombobox::switchCheckstate ( int row )
{
	if ( checkstate ( row ) )
	{
		setCheckstate ( row, 0 );
	}
	else
	{
		setCheckstate ( row, 1 );
	}

	emit checkstateChanged ( row );
}


int multiCombobox::addItem ( QString text, int checked )
{
	QComboBox::addItem ( text );

	int itemsCount = count();

	setCheckstate ( itemsCount-1, checked );

	return itemsCount-1;
}
