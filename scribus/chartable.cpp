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
//#include "charselect.h"

#include "chartable.h"
#include "chartable.moc"


#ifdef QT_MAC
CharZoom::CharZoom(QWidget* parent, QPixmap pix, uint val) : QDialog( parent, "Edit", false, WStyle_Customize | WStyle_NoBorder | WType_Popup)
#else
CharZoom::CharZoom(QWidget* parent, QPixmap pix, uint val) : QDialog( parent, "Edit", false, WStyle_Customize | WStyle_NoBorder)
#endif
{
	int newwidth=pix.width()+2;
	int newheight=pix.height()+20;
	resize(newwidth,newheight);
	setMinimumSize(QSize(newwidth,newheight));
	setMaximumSize(QSize(newwidth,newheight));
	pixm = pix;
	QString tmp;
	tmp.sprintf("%04X", val);
	valu = "0x"+tmp;
}

void CharZoom::paintEvent(QPaintEvent *)
{
	QPainter p;
	p.begin(this);
	p.setPen(black);
	p.setBrush(NoBrush);
	p.drawRect(0, 0, width(), height());
	p.drawPixmap(1, 1, pixm);
	p.drawText(5, height()-3, valu);
	p.end();
}

CharTable::CharTable(QWidget* parent, int cols, PageItem* pi, QString font)
	: QTable(parent),
	mPressed(false),
	zoom(0),
	m_Item(pi),
	m_fontInUse(font)
{
	m_characters.clear();
	//watchTimer = new QTimer(this);
//	connect(watchTimer, SIGNAL(timeout()), this, SLOT(showAlternate()));
// 	alternate = false;
	// gui
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
	setDragEnabled(true);
	enableDrops(true);

	connect(this, SIGNAL(dropped(QDropEvent *)), this, SLOT(slotDropped(QDropEvent *)));
}


QRect CharTable::cellGeometry ( int /*row*/, int /*col*/ ) const
{
	int widthHeight = QMAX(18 + qRound(-(*m_Item->doc()->AllFonts)[m_fontInUse].descent() * 18) + 5, 18);
	return QRect(0, 0, widthHeight, widthHeight+20);
}


void CharTable::paintCell( QPainter * qp, int row, int col, const QRect & cr, bool /*selected*/, const QColorGroup & /*cg*/ )
{
	static QPixmap pixm;

	uint cc = row * numCols() + col;
	if (cc >= m_characters.count())
		return;

	QRect sz = cellGeometry(row, col);
	pixm.resize(sz.width(), sz.height());

	ScPainter *p = new ScPainter(&pixm, cr.width(), cr.height());
	p->clear();
	pixm.fill(white);
	QWMatrix chma;
	chma.scale(1.6, 1.6);
	qp->eraseRect(0, 0, cr.width(), cr.height());
	QFont fo = qp->font();
	fo.setPixelSize(9);
	qp->setFont(fo);
	static FPointArray gly;
	ScFace face = (*m_Item->doc()->AllFonts)[m_fontInUse];
	uint gl = face.char2CMap(m_characters[cc]);
	gly = face.glyphOutline(gl);
	if (gly.size() > 4)
	{
		gly.map(chma);
		double ww = sz.width() - face.glyphWidth(gl)*numCols();
		p->translate(ww / 2, 1);
		p->setBrush(black);
		p->setFillMode(1);
		p->setupPolygon(&gly);
		p->fillPath();
		p->end();
		int x = QMAX(0, (cr.width() - sz.width()) / 2);
		qp->drawPixmap(x, 1, pixm);
		QString tmp;
		tmp.sprintf("%04X", m_characters[row*numCols()+col]);
		qp->setPen(black);
		qp->drawText(QRect(2, cr.height()-10, cr.width()-4, 9),Qt::AlignCenter, tmp);
	}
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
		int bh = 48 + qRound(-(*m_Item->doc()->AllFonts)[m_fontInUse].descent() * 48) + 3;
		QPixmap pixm(bh,bh);
		ScPainter *p = new ScPainter(&pixm, bh, bh);
		p->clear();
		pixm.fill(white);
		QWMatrix chma;
		chma.scale(4.8, 4.8);
		ScFace face = (*m_Item->doc()->AllFonts)[m_fontInUse];
		uint gl = face.char2CMap(currentChar);
		FPointArray gly = face.glyphOutline(gl);
		double ww = bh - face.glyphWidth(gl, 48);
		if (gly.size() > 4)
		{
			gly.map(chma);
			p->translate(ww / 2, 1);
			p->setBrush(black);
			p->setFillMode(1);
			p->setupPolygon(&gly);
			p->fillPath();
			p->end();
		}
		delete p;
		zoom = new CharZoom(this, pixm, currentChar);
		zoom->move(m_mousePosition.x()-2, m_mousePosition.y()-2);
		zoom->setModal(false);
		zoom->show();
	}
	if (e->button() == LeftButton)
	{
		selectCells(r, c, r, c);
		cCol = c;
		cRow = r;
	}
	QTable::contentsMousePressEvent(e);
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
		//emit selectChar(rowAt(e->pos().y()), columnAt(e->pos().x()));
		emit selectChar(m_characters[rowAt(e->pos().y()) * numCols() + columnAt(e->pos().x())]);
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
	m_fontInUse = font;
	recalcCellSizes();
}

// D'n'D
void CharTable::enableDrops(bool e)
{
	viewport()->setAcceptDrops(e);
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
	bool ok;
	if ( QTextDrag::decode(evt, label))
	{
		int val = label.toInt(&ok, 10);
		if (ok && !m_characters.contains(val))
		{
			m_characters.append(val);
			recalcCellSizes();
		}
	}
}
