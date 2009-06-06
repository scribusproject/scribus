/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMenu>

#include "chartablemodel.h"
#include "chartableview.h"
#include "ui/charzoom.h"
#include "fonts/scface.h"


CharTableView::CharTableView(QWidget * parent)
		: QTableView(parent),
		zoom(0)
{
	deleteAct = new QAction( tr("Delete"), this);
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(removeCharacter()));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
	        this, SLOT(viewDoubleClicked(const QModelIndex &)));

	actionMenu = new QMenu(this);
	actionMenu->addAction(deleteAct);

	setAcceptDrops(false);
	setDropIndicatorShown(true);

	verticalHeader()->setVisible(false);
	horizontalHeader()->setVisible(false);
}

CharTableModel * CharTableView::model()
{
	return qobject_cast<CharTableModel*>(QTableView::model());
}

void CharTableView::removeCharacter()
{
	model()->removeCharacter(deleteAct->data().toInt());
}

void CharTableView::modelSelectionChanged(QItemSelectionModel * model)
{
	setSelectionModel(model);
}

void CharTableView::resizeLastRow()
{
	setRowHeight(model()->rowCount()-1, width() / model()->columnCount() + 5);
}

void CharTableView::keyPressEvent(QKeyEvent *k)
{
	switch (k->key())
	{
	case Qt::Key_Backspace:
	case Qt::Key_Delete:
		emit delChar();
		break;
	case Qt::Key_Insert:
		// safely emit selectChar(model()->characters()[currenCharactersIndex()]);
		viewDoubleClicked(QModelIndex());
		break;
	}
	QTableView::keyPressEvent(k);
}

void CharTableView::mousePressEvent(QMouseEvent* e)
{
	QTableView::mousePressEvent(e);

	int index = currenCharactersIndex();
	int currentChar = -1;

	if (index < model()->characters().count())
		currentChar = model()->characters()[index];

	if (e->button() == Qt::RightButton && currentChar > -1)
	{
		// Only non-dropable tables show "magnifier glass"
		if (!acceptDrops())
		{
			hideZoomedChar();
			zoom = new CharZoom(this, currentChar, model()->fontFace());
			zoom->move(e->globalPos().x()-2, e->globalPos().y()-2);
			zoom->show();
		}
		else
		{
			deleteAct->setData(index);
			actionMenu->popup(e->globalPos());
		}
	}
}

void CharTableView::mouseMoveEvent(QMouseEvent* e)
{
	// HACK to prevent strange Qt4 cursor behaviour after dropping. It's examined by Trolltech now - PV.
	// It's the one and only reason why to include QApplication here.
		// Fixed at least in Qt-4.4.2
//	QApplication::restoreOverrideCursor();
	hideZoomedChar();
	QTableView::mouseMoveEvent(e);
}

void CharTableView::mouseReleaseEvent(QMouseEvent* e)
{
	hideZoomedChar();
	QTableView::mouseReleaseEvent(e);
}

void CharTableView::viewDoubleClicked(const QModelIndex & /*index*/)
{
	if (model()->characters().count() > currenCharactersIndex())
		emit selectChar(model()->characters()[currenCharactersIndex()]);
}

int CharTableView::currenCharactersIndex()
{
	return currentIndex().row() * model()->columnCount() + currentIndex().column();
}

void CharTableView::hideZoomedChar()
{
	if (zoom)
	{
		zoom->close();
		delete zoom;
		zoom = 0;
	}
}

void CharTableView::hideEvent(QHideEvent * e)
{
	hideZoomedChar();
	QTableView::hideEvent(e);
}

void CharTableView::resizeEvent(QResizeEvent *e)
{
	QTableView::resizeEvent(e);
	if (model())
	{
		model()->setViewWidth(e->size().width());
		// The resizeColumnsToContents() method won't work here.
		// It doesn't handle cells without any content. And it creates
		// larger columns than required. Dunno why.
		for (int i = 0; i < model()->columnCount(); ++i)
			setColumnWidth(i, e->size().width() / model()->columnCount());
		for (int i = 0; i < model()->rowCount(); ++i)
			setRowHeight(i, e->size().width() / model()->columnCount() + 5);
	}
}
