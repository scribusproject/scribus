/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QHeaderView>
#include <QKeyEvent>
#include <QMenu>

#include "chartablemodel.h"
#include "chartableview.h"
#include "charzoom.h"
#include "fonts/scface.h"


CharTableView::CharTableView(QWidget * parent)
	: QTableView(parent),
	zoom(0),
	mPressed(false)
{
	deleteAct = new QAction( tr("Delete"), this);
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(removeCharacter()));
	actionMenu = new QMenu(this);
	actionMenu->addAction(deleteAct);

	setAcceptDrops(false);
	setDropIndicatorShown(true);

	verticalHeader()->setVisible(false);
	horizontalHeader()->setVisible(false);
	setSelectionMode(QAbstractItemView::SingleSelection);
}

CharTableModel * CharTableView::model()
{
	return qobject_cast<CharTableModel*>(QTableView::model());
}

void CharTableView::removeCharacter()
{
	model()->removeCharacter(deleteAct->data().toInt());
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
			emit selectChar(model()->characters()[currentValue()]);
			break;
	}
	QTableView::keyPressEvent(k);
}

void CharTableView::mousePressEvent(QMouseEvent* e)
{
	if (!currentIndex().isValid())
		return;

	QTableView::mousePressEvent(e);
	mPressed = true;
	mousePos = e->pos();
	int index = currentValue();
	int currentChar = -1;

	if (index < model()->characters().count())
		currentChar = model()->characters()[index];

	if (e->button() == Qt::RightButton && currentChar > -1)
	{
		// Only non-dropable tables show "magnifier glass"
		if (!acceptDrops())
		{
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
	if (mPressed)
		hideZoomedChar();
	if (!currentIndex().isValid())
		return;
	int index = currentValue();
	int currentChar = -1;
	if (index < model()->characters().count())
		currentChar = model()->characters()[index];
	if ((e->buttons() & Qt::LeftButton) && (mPressed) && (currentChar > -1) && ((abs(mousePos.x() - e->pos().x()) > 10) || (abs(mousePos.x() - e->pos().y()) > 10)))
	{
		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData();
		QString s("%1");
		mimeData->setText(s.arg(model()->characters()[currentValue()]));
		drag->setMimeData(mimeData);
		drag->exec(Qt::CopyAction);
		mPressed = false;
	}
	QTableView::mouseMoveEvent(e);
}

void CharTableView::mouseReleaseEvent(QMouseEvent* e)
{
	if (!currentIndex().isValid())
		return;
	if ((e->button() == Qt::RightButton) && mPressed)
		hideZoomedChar();

	if (e->button() == Qt::LeftButton)
	{
		int index = rowAt(e->pos().y()) * model()->columnCount() + columnAt(e->pos().x());
		if (index >= 0 && index < model()->characters().size())
			emit selectChar(model()->characters()[index]);
	}
	mPressed = false;

	QTableView::mouseReleaseEvent(e);
}

void CharTableView::dropEvent(QDropEvent *e)
{
	e->accept();
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		model()->appendUnicode(e->mimeData()->text(), 10);
	}
}

void CharTableView::dragMoveEvent(QDragMoveEvent *e)
{
	e->accept();
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
	}
}

void CharTableView::dragEnterEvent(QDragEnterEvent *e)
{
	e->accept();
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
	}
}

int CharTableView::currentValue()
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
