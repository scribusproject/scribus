/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARTABLE_H
#define CHARTABLE_H

#include <qdialog.h>
//Added by qt3to4:
#include <QDropEvent>
#include <QMouseEvent>
#include <Q3ValueList>
#include <Q3Table>
#include <QKeyEvent>
#include "scribusapi.h"

class PageItem;
class CharZoom;

//! \brief A special type for character classes
typedef Q3ValueList<uint> CharClassDef;


/*! \brief A table widget containing given characters from font */
class SCRIBUS_API CharTable : public Q3Table
{
    Q_OBJECT

public:
	CharTable(QWidget* parent, int cols, ScribusDoc *doc, QString font);
	~CharTable() {};

	void setFontInUse(QString font);
	void setCharacters(CharClassDef ch);
	CharClassDef characters() { return m_characters; };
	//! \brief "Repaint" and refill the table with new font characters
	void recalcCellSizes();
	/*! \brief Set the widget to accept/reject drop events.
	It sets the right-button behaviour too. It enables delete popup
	menu when e is true instead of larger preview dialog. The idea:
	When user can drop items into it, he could want to delete it too. */
	void enableDrops(bool e);
	void setDoc(ScribusDoc *doc);

public slots:
	//! \brief No usage now.
	void showAlternate();
	/*! \brief appends an unicode char into m_characters list.
	\param s a QString with numerical representation of the character.
	\param base an optional parameter containing base of the numerical converion. See QString::toInt() documentation.
	The base parameter is used mainly in normal code - not in slot calls.
	*/
	void appendUnicode(QString s, uint base = 16);

signals:
	void selectChar(uint);
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
// 	PageItem *m_Item;
	ScribusDoc *m_doc;
	//! \brief True when its in dragging mode
	bool dragging;
	//! \brief current column
	uint cCol;
	//! \brief current row
	uint cRow;

private slots:
	void slotDropped(QDropEvent *evt);
	/*! \brief Delete a character from this table on given position.
	Delete an item from m_characters at given index. See enableDrops()
	for more info. */
	void deleteOwnCharacter(int index);

protected:
	QString m_fontInUse;
	CharClassDef m_characters;
	QPoint m_mousePosition;

	void keyPressEvent(QKeyEvent *k);
	void contentsMouseReleaseEvent(QMouseEvent *m);
	void contentsMousePressEvent(QMouseEvent* e);
	// d'n'd
	Q3DragObject * dragObject();
};

#endif
