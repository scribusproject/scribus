/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARTABLE_H
#define CHARTABLE_H

#include <QAbstractTableModel>
#include "scribusapi.h"

// class PageItem;
class CharZoom;
class ScribusDoc;

//! \brief A special type for character classes
typedef QList<uint> CharClassDef;

class SCRIBUS_API CharTableModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		CharTableModel(QObject *parent = 0, int cols = 4, ScribusDoc * doc = 0, const QString & font = 0);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		void setFontInUse(QString font);
		void setCharacters(CharClassDef ch);
		CharClassDef characters() { return m_characters; };

		/*! \brief Set the widget to accept/reject drop events.
		It sets the right-button behaviour too. It enables delete popup
		menu when e is true instead of larger preview dialog. The idea:
		When user can drop items into it, he could want to delete it too. */
		void enableDrops(bool e);
		void setDoc(ScribusDoc *doc);

	public slots:
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

	int m_cols;

private slots:
// 	void slotDropped(QDropEvent *evt);
	/*! \brief Delete a character from this table on given position.
	Delete an item from m_characters at given index. See enableDrops()
	for more info. */
// 	void deleteOwnCharacter(int index);

protected:
	QString m_fontInUse;
	CharClassDef m_characters;
// 	QPoint m_mousePosition;

// 	void keyPressEvent(QKeyEvent *k);
// 	void contentsMouseReleaseEvent(QMouseEvent *m);
// 	void contentsMousePressEvent(QMouseEvent* e);
	// d'n'd
// 	Q3DragObject * dragObject();
};

#endif
