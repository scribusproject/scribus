/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QHeaderView>
#include <QAction>
#include <QMenu>

#include "scribusdoc.h"
#include "scpainter.h"
#include "chartable.h"
#include "charzoom.h"


CharTableModel::CharTableModel(QObject *parent, int cols, ScribusDoc * doc, const QString & font)
    : QAbstractTableModel(parent),
	m_doc(doc),
	m_cols(cols),
	m_fontInUse(font)
{
	m_characters.clear();
}

int CharTableModel::rowCount(const QModelIndex & /* parent */) const
{
    return m_characters.count() / m_cols;
}

int CharTableModel::columnCount(const QModelIndex & /* parent */) const
{
    return m_cols;
}

QVariant CharTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || m_doc == 0)
		return QVariant();

	int ix = index.row() * m_cols + index.column();
	int currentChar = -1;
	if (ix < m_characters.count())
		currentChar = m_characters[ix];
	if (currentChar == -1)
		return QVariant();

	// pixmap
	if (role == Qt::DecorationRole)
	{
		// it could be optimized maybe. But it's much faster
		// than qt3 anyway.
		QPixmap pixm(30, 50);
		QImage pix(30, 50, QImage::Format_ARGB32);
		ScPainter *p = new ScPainter(&pix, 30, 50);
		p->clear();
		pixm.fill(Qt::white);
		QMatrix chma;
// 		chma.scale(4.8, 4.8);
		chma.scale(2.0, 2.0);

		ScFace face = (*m_doc->AllFonts)[m_fontInUse];
		uint gl = face.char2CMap(currentChar);
		FPointArray gly = face.glyphOutline(gl);
		if (gly.size() > 4)
		{
			gly.map(chma);
			p->translate(5, 10);
			p->setBrush(Qt::black);
			p->setFillMode(1);
			p->setupPolygon(&gly);
			p->fillPath();
			p->end();
		}
		delete p;
		pixm.convertFromImage(pix);
		return QVariant(pixm);
	}
	// trash
	return QVariant();
}

void CharTableModel::setDoc(ScribusDoc *doc)
{
	bool repaint = (doc == m_doc) ? false : true;

	m_doc = doc;
	if (repaint)
		reset();
}

ScFace CharTableModel::fontFace()
{
	return (*m_doc->AllFonts)[m_fontInUse];
}

void CharTableModel::setCharacters(CharClassDef ch)
{
	m_characters.clear();
	m_characters = ch;
	reset();
}

void CharTableModel::setFontInUse(QString font)
{
	if (font != m_fontInUse)
	{
		m_fontInUse = font;
		reset();
	}
}

void CharTableModel::appendUnicode(QString s, uint base)
{
	bool ok;
	int val = s.toInt(&ok, base);
	if (ok && !m_characters.contains(val))
	{
		m_characters.append(val);
		reset();
	}
}

bool CharTableModel::removeCharacter(int index)
{
	if (index >= 0 && index < m_characters.size())
	{
		m_characters.removeAt(index);
		reset();
		return true;
	}
	qDebug("CharTable::deleteOwnCharacter: no char deleted - logical error propably");
	return false;
}

Qt::ItemFlags CharTableModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
	if (index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	return Qt::ItemIsDropEnabled | defaultFlags;
}



CharTableView::CharTableView(QWidget * parent)
	: QTableView(parent),
	zoom(0),
	mPressed(false)
{
	deleteAct = new QAction(tr("Delete"), this);
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

	mPressed = true;

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

	if (e->button() == Qt::LeftButton && currentChar > -1)
	{
		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData();
		QString s("%1");
		mimeData->setText(s.arg(model()->characters()[currentValue()]));
		drag->setMimeData(mimeData);
		drag->start(Qt::CopyAction);
	}
	QTableView::mousePressEvent(e);
}

void CharTableView::removeCharacter()
{
	model()->removeCharacter(deleteAct->data().toInt());
}

void CharTableView::mouseReleaseEvent(QMouseEvent* e)
{
	if (!currentIndex().isValid())
		return;
	if ((e->button() == Qt::RightButton) && (mPressed))
	{
		if (zoom)
		{
			zoom->close();
			delete zoom;
			zoom = 0;
		}
	}

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
	if (e->mimeData()->hasText())
	{
		model()->appendUnicode(e->mimeData()->text(), 10);
		e->acceptProposedAction();
		e->accept();
	}
}

void CharTableView::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasText())
		e->acceptProposedAction();
}

int CharTableView::currentValue()
{
	return currentIndex().row() * model()->columnCount() + currentIndex().column();
}
