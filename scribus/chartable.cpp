/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <qtable.h>
//#include <qtimer.h>
#include <qdragobject.h>

#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"

#include "chartable.h"
#include "chartable.moc"
#include "charzoom.h"


CharTable::CharTable(QWidget* parent, int cols, ScribusDoc* doc, QString font)
	: QTable(parent),
	mPressed(false),
	zoom(0),
// 	m_Item(pi),
	m_fontInUse(font)
{
	m_characters.clear();
	//watchTimer = new QTimer(this);
//	connect(watchTimer, SIGNAL(timeout()), this, SLOT(showAlternate()));
// 	alternate = false;
	// gui
	m_doc = doc;
	setNumCols(cols);
	setLeftMargin(0);
	verticalHeader()->hide();
	setTopMargin(0);
	horizontalHeader()->hide();
	setSorting(false);
	setSelectionMode(QTable::NoSelection);
	setColumnMovingEnabled(false);
	setRowMovingEnabled(false);
	setReadOnly(true);
//	setDragEnabled(true);
//	enableDrops(true);

//	connect(this, SIGNAL(dropped(QDropEvent *)), this, SLOT(slotDropped(QDropEvent *)));
}

void CharTable::setDoc(ScribusDoc *doc)
{
	bool repaint = (doc == m_doc) ? false : true;

	m_doc = doc;
	if (repaint)
		recalcCellSizes();
}

QRect CharTable::cellGeometry ( int /*row*/, int /*col*/ ) const
{
	if (!m_doc)
		return QRect(0, 0, 1, 1);

	int widthHeight = QMAX(18 + qRound(-(*m_doc->AllFonts)[m_fontInUse].descent() * 18) + 5, 18);
	return QRect(0, 0, widthHeight, widthHeight+20);
}


void CharTable::paintCell( QPainter * qp, int row, int col, const QRect & cr, bool /*selected*/, const QColorGroup & /*cg*/ )
{
	if (!m_doc)
		return;

	static QPixmap pixm;

	uint cc = row * numCols() + col;
	if (cc >= m_characters.count())
		return;

	QRect sz = cellGeometry(row, col);
	pixm.resize(sz.width(), sz.height());

	ScPainter *p = new ScPainter(&pixm, cr.width(), cr.height());
//	qDebug(QString("paintCell: w=%1 h=%2").arg(cr.width()).arg(cr.height()));
	p->clear();
	pixm.fill(white);
//	QWMatrix chma;
//	chma.scale(1.6, 1.6);
	qp->eraseRect(0, 0, cr.width(), cr.height());
	qp->setPen(black);
	static FPointArray gly;
	ScFace face = (*m_doc->AllFonts)[m_fontInUse];
	uint gl = face.char2CMap(m_characters[cc]);
	gly = face.glyphOutline(gl, 1.6);
/*	qDebug(QString("paintCell: face=%1 glyph=%2 size=%3 pt (%4, %5)")
		   .arg(face.scName())
		   .arg(gl)
		   .arg(gly.size())
		   .arg(gly.size()>0? gly.point(0).x() : -999)
		   .arg(gly.size()>0? gly.point(0).y() : -999));
*/
	if (gly.size() >= 4)
	{
//		gly.map(chma);
		double ww = sz.width() - face.glyphWidth(gl, 1.6) * numCols();
		p->translate(ww / 2, 1);
		p->setBrush(black);
		p->setFillMode(1);
		p->setupPolygon(&gly);
		p->fillPath();
		p->end();
		int x = QMAX(0, (cr.width() - sz.width()) / 2);
		qp->drawPixmap(x, 1, pixm);
	}
	QString tmp;
	tmp.sprintf("%04X", m_characters[row*numCols()+col]);
	QFont fo = qp->font();
	fo.setPixelSize(9);
	qp->setFont(fo);
	qp->drawText(QRect(2, cr.height()-10, cr.width()-4, 9),Qt::AlignCenter, tmp);
	qp->setPen(gray);
	qp->drawRect(0, 0, cr.width(), cr.height());
	delete p;
}


void CharTable::keyPressEvent(QKeyEvent *k)
{
	switch (k->key())
	{
		case Key_Backspace:
		case Key_Delete:
			emit delChar();
			break;
		case Key_Insert:
			//emit selectChar(currentRow(), currentColumn());
			emit selectChar(m_characters[currentRow() * numCols() + currentColumn()]);
			break;
	}
	QTable::keyPressEvent(k);
}

void CharTable::contentsMousePressEvent(QMouseEvent* e)
{
	//e->accept();
	uint r = rowAt(e->pos().y());
	uint c = columnAt(e->pos().x());

	mPressed = true;
	m_mousePosition = QCursor::pos();
	uint index = r * numCols() + c;
	int currentChar = -1;
	if (index < m_characters.count())
		currentChar = m_characters[index];

	if (e->button() == RightButton && currentChar > -1)
	{
		//watchTimer->stop();
		// Only non-dropable tables show "magnifier glass"
		if (!viewport()->acceptDrops())
		{
			zoom = new CharZoom(this, currentChar, (*m_doc->AllFonts)[m_fontInUse]);
			zoom->move(m_mousePosition.x()-2, m_mousePosition.y()-2);
			zoom->show();
		}
		else
		{
			// delete popup menu when it accepts drops
			QPopupMenu *pmen = new QPopupMenu();
			int pid = pmen->insertItem( tr("Delete"), this, SLOT(deleteOwnCharacter(int)));
			pmen->setItemParameter(pid, index);
			pmen->exec(QCursor::pos());
			delete pmen;
		}
	}
	if (e->button() == LeftButton)
	{
		selectCells(r, c, r, c);
		cCol = c;
		cRow = r;
	}
	QTable::contentsMousePressEvent(e);
}

void CharTable::deleteOwnCharacter(int index)
{
	if (m_characters.remove(m_characters[index]) > 0)
		recalcCellSizes();
	else
		qDebug("CharTable::deleteOwnCharacter: no char deleted - logical error propably");
}

void CharTable::contentsMouseReleaseEvent(QMouseEvent* e)
{
	e->accept();
	//watchTimer->stop();
	if ((e->button() == RightButton) && (mPressed))
	{
		if (zoom)
		{
			zoom->close();
			delete zoom;
			zoom = 0;
		}
	}
	//if ((e->button() == LeftButton) && (!alternate))
	if (e->button() == LeftButton)
	{
		int index = rowAt(e->pos().y()) * numCols() + columnAt(e->pos().x());
		if (index >= 0)
			emit selectChar(m_characters[index]);
	}
	mPressed = false;
// 	alternate = false;
	QTable::contentsMouseReleaseEvent(e);
}

void CharTable::showAlternate()
{
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
}

void CharTable::recalcCellSizes()
{
	int ab = m_characters.count() / numCols();
	if (m_characters.count() % numCols() != 0)
		ab++;
	setNumRows(ab);

	int cellWidth = width() / numCols();
	int cellHeight = cellWidth;
	for (int d = 0; d < numCols(); ++d)
		setColumnStretchable(d, TRUE);
	for (int d = 0; d < numRows(); ++d)
		setRowHeight(d, cellHeight);
	updateScrollBars();
}

void CharTable::setCharacters(CharClassDef ch)
{
	m_characters.clear();
	m_characters = ch;
	recalcCellSizes();
}

void CharTable::setFontInUse(QString font)
{
	bool repaint = (font == m_fontInUse) ? false : true;
	if (repaint)
	{
		m_fontInUse = font;
		recalcCellSizes();
	}
}

void CharTable::enableDrops(bool e)
{
//	viewport()->setAcceptDrops(e);
}

QDragObject * CharTable::dragObject()
{
	QString s("%1");
	uint val = cRow * numCols() + cCol;
	return new QTextDrag(s.arg(m_characters[val]), this);
}

void CharTable::slotDropped(QDropEvent *evt)
{
	if (evt->source() == this)
		return;

	QString label;
	if ( QTextDrag::decode(evt, label))
		appendUnicode(label, 10);
}

void CharTable::appendUnicode(QString s, uint base)
{
	bool ok;
	int val = s.toInt(&ok, base);
	if (ok && !m_characters.contains(val))
	{
		m_characters.append(val);
		recalcCellSizes();
	}
}
