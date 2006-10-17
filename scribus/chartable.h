/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARTABLE_H
#define CHARTABLE_H

#include <qdialog.h>
#include "scribusapi.h"

class QTable;
class PageItem;


//! \brief A special type for character classes
typedef QValueList<uint> CharClassDef;


/*! \brief "A magnifying glass" dialog for CharTable cell.
*/
class SCRIBUS_API CharZoom : public QDialog
{
	Q_OBJECT

	public:
		CharZoom( QWidget* parent, QPixmap pix, uint val);
		~CharZoom() {};
	private:
		void paintEvent(QPaintEvent *);
		QPixmap pixm;
		QString valu;
};


/*! \brief A table widget containing given characters from font */
class SCRIBUS_API CharTable : public QTable
{
    Q_OBJECT

public:
	CharTable(QWidget* parent, int cols, PageItem *pi, QString font);
	~CharTable() {};

	void setFontInUse(QString font);
	void setCharacters(CharClassDef ch);
	//! \brief "Repaint" and refill the table with new font characters
	void recalcCellSizes();

public slots:
	//! \brief No usage now.
	void showAlternate();

signals:
	//! \brief A signal emitted when user selects exact cell
	void selectChar(uint, uint);
	//! \brief When user press the DELETE/BACKSPACE key
	void delChar();

private:
	//! \brief Empty reimplementation of the QTable method due the speedup.
	void resizeData( int ) {};
	//! \brief Reimplementation of the QTable method. Painting of the sample. See Qt docs for more.
	QRect cellGeometry ( int row, int col ) const;
	//! \brief Reimplementation of the QTable method. Painting of the sample. See Qt docs for more.
	void paintCell( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg );
	//! \brief Empty reimplementation of the QTable method due the speedup.
	void paintFocus( QPainter * /*p*/, const QRect & /*cr*/ ) {};

	bool mPressed;
// 	bool alternate;
	//! \brief Magnify dialog reference
	CharZoom* zoom;
	PageItem *m_Item;

protected:
	QString m_fontInUse;
	CharClassDef m_characters;

	virtual void keyPressEvent(QKeyEvent *k);
	virtual void contentsMouseReleaseEvent(QMouseEvent *m);
	virtual void contentsMousePressEvent(QMouseEvent* e);
};

#endif
