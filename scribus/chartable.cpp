/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

// #include <q3table.h>
// //#include <qtimer.h>
// #include <q3dragobject.h>
// //Added by qt3to4:
// #include <QDropEvent>
// #include <QPixmap>
// #include <QMouseEvent>
// #include <Q3PopupMenu>
// #include <QKeyEvent>

#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"

#include "chartable.h"
#include "charzoom.h"


CharTableModel::CharTableModel(QObject *parent, int cols, ScribusDoc * doc, const QString & font)
    : QAbstractTableModel(parent),
	mPressed(false),
	zoom(0),
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

	// pixmap
	if (role == Qt::DecorationRole)
	{
		// it could be optimized maybe. But it's much faster
		// than qt3 anyway.
		int ix = index.row() * m_cols + index.column();
		int currentChar = -1;
		if (ix < m_characters.count())
			currentChar = m_characters[ix];
		if (currentChar == -1)
			return QVariant();

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
	// "zoom" preview
	if (role == Qt::ToolTipRole)
	{
		// TODO: CharZoom ************************************************
	}
	// trash
	return QVariant();
}

QVariant CharTableModel::headerData(int /* section */,
                                Qt::Orientation /* orientation */,
                                int role) const
{
//     if (role == Qt::SizeHintRole)
//         return QSize(1, 1);
    return QVariant();
}


void CharTableModel::setDoc(ScribusDoc *doc)
{
	bool repaint = (doc == m_doc) ? false : true;

	m_doc = doc;
	if (repaint)
		reset();
}

// void CharTable::keyPressEvent(QKeyEvent *k)
// {
// 	switch (k->key())
// 	{
// 		case Qt::Key_Backspace:
// 		case Qt::Key_Delete:
// 			emit delChar();
// 			break;
// 		case Qt::Key_Insert:
// 			//emit selectChar(currentRow(), currentColumn());
// 			emit selectChar(m_characters[currentRow() * numCols() + currentColumn()]);
// 			break;
// 	}
// 	Q3Table::keyPressEvent(k);
// }
// 
// void CharTable::contentsMousePressEvent(QMouseEvent* e)
// {
// 	//e->accept();
// 	uint r = rowAt(e->pos().y());
// 	uint c = columnAt(e->pos().x());
// 
// 	mPressed = true;
// 	m_mousePosition = QCursor::pos();
// 	int index = r * numCols() + c;
// 	int currentChar = -1;
// 	if (index < m_characters.count())
// 		currentChar = m_characters[index];
// 
// 	if (e->button() == Qt::RightButton && currentChar > -1)
// 	{
// 		//watchTimer->stop();
// 		// Only non-dropable tables show "magnifier glass"
// 		if (!viewport()->acceptDrops())
// 		{
// 			zoom = new CharZoom(this, currentChar, (*m_doc->AllFonts)[m_fontInUse]);
// 			zoom->move(m_mousePosition.x()-2, m_mousePosition.y()-2);
// 			zoom->show();
// 		}
// 		else
// 		{
// 			// delete popup menu when it accepts drops
// 			Q3PopupMenu *pmen = new Q3PopupMenu();
// 			int pid = pmen->insertItem( tr("Delete"), this, SLOT(deleteOwnCharacter(int)));
// 			pmen->setItemParameter(pid, index);
// 			pmen->exec(QCursor::pos());
// 			delete pmen;
// 		}
// 	}
// 	if (e->button() == Qt::LeftButton)
// 	{
// 		selectCells(r, c, r, c);
// 		cCol = c;
// 		cRow = r;
// 	}
// 	Q3Table::contentsMousePressEvent(e);
// }

// void CharTable::deleteOwnCharacter(int index)
// {
// 	if (m_characters.remove(m_characters[index]) > 0)
// 	{
// 		recalcCellSizes();
// 		// WTF? why it does not repaint?
// 		repaint(0, 0, width(), height(), true);
// 	}
// 	else
// 		qDebug("CharTable::deleteOwnCharacter: no char deleted - logical error propably");
// }

// void CharTable::contentsMouseReleaseEvent(QMouseEvent* e)
// {
// 	e->accept();
// 	//watchTimer->stop();
// 	if ((e->button() == Qt::RightButton) && (mPressed))
// 	{
// 		if (zoom)
// 		{
// 			zoom->close();
// 			delete zoom;
// 			zoom = 0;
// 		}
// 	}
// 	//if ((e->button() == Qt::LeftButton) && (!alternate))
// 	if (e->button() == Qt::LeftButton)
// 	{
// 		int index = rowAt(e->pos().y()) * numCols() + columnAt(e->pos().x());
// 		if (index >= 0)
// 			emit selectChar(m_characters[index]);
// 	}
// 	mPressed = false;
// // 	alternate = false;
// 	Q3Table::contentsMouseReleaseEvent(e);
// }

// void CharTable::showAlternate()
// {
/*	watchTimer->stop();
	alternate = true;
	QString font;
	QString chToIns = "";
	font = par->fontInUse;
	uint baseChar = rowA*16+colA;
	if (baseChar < maxCount)
	{
		QPopupMenu *pmen = new QPopupMenu();
		chToIns = QChar(par->characters[baseChar]);
		pmen->insertItem(FontSample((*m_Item->doc()->AllFonts)[font], 20, chToIns, paletteBackgroundColor(), true));
		if ((*m_Item->doc()->AllFonts)[font]->CharWidth.contains(par->characters[baseChar] + 0xF720))
		{
			chToIns = QChar(par->characters[baseChar] + 0xF720);
			pmen->insertItem(FontSample((*m_Item->doc()->AllFonts)[font], 20, chToIns, paletteBackgroundColor(), true));
		}
		int re = pmen->indexOf(pmen->exec(QCursor::pos()));
		delete pmen;
	} */
// }

// void CharTable::recalcCellSizes()
// {
// 	int ab = m_characters.count() / numCols();
// 	if (m_characters.count() % numCols() != 0)
// 		ab++;
// 	setNumRows(ab);
// 
// 	int cellWidth = width() / numCols();
// 	int cellHeight = cellWidth;
// 	for (int d = 0; d < numCols(); ++d)
// 		setColumnStretchable(d, TRUE);
// 	for (int d = 0; d < numRows(); ++d)
// 		setRowHeight(d, cellHeight);
// 	updateScrollBars();
// }

void CharTableModel::setCharacters(CharClassDef ch)
{
	m_characters.clear();
	m_characters = ch;
// 	recalcCellSizes();
	reset();
}

void CharTableModel::setFontInUse(QString font)
{
	bool repaint = (font == m_fontInUse) ? false : true;
	if (repaint)
	{
		m_fontInUse = font;
// 		recalcCellSizes();
		reset();
	}
}

// void CharTable::enableDrops(bool e)
// {
// 	viewport()->setAcceptDrops(e);
// }
// 
// Q3DragObject * CharTable::dragObject()
// {
// 	QString s("%1");
// 	uint val = cRow * numCols() + cCol;
// 	return new Q3TextDrag(s.arg(m_characters[val]), this);
// }
// 
// void CharTable::slotDropped(QDropEvent *evt)
// {
// 	if (evt->source() == this)
// 		return;
// 
// 	QString label;
// 	if ( Q3TextDrag::decode(evt, label))
// 		appendUnicode(label, 10);
// }
// 
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
