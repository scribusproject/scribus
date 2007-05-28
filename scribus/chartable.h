/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARTABLE_H
#define CHARTABLE_H

#include <QAbstractTableModel>
#include <QTableView>
#include "scribusapi.h"

// class PageItem;
class CharZoom;
class ScribusDoc;
class ScFace;

//! \brief A special type for character classes
typedef QList<uint> CharClassDef;

/*! \brief A model (MVC) to handle unicode characters map.
It's a backend for CharTableView - its GUI representation.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API CharTableModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		CharTableModel(QObject *parent = 0, int cols = 4, ScribusDoc * doc = 0, const QString & font = 0);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		//! \brief Get a graphics representation/pixmap of the glyph
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

		void setFontInUse(QString font);
		ScFace fontFace();
		void setCharacters(CharClassDef ch);
		CharClassDef characters() { return m_characters; };

		//! \brief called to erase glyph at index from table.
		bool removeCharacter(int index);

		void setDoc(ScribusDoc *doc);

	public slots:
		/*! \brief appends an unicode char into m_characters list.
		\param s a QString with numerical representation of the character.
		\param base an optional parameter containing base of the numerical converion. See QString::toInt() documentation.
		The base parameter is used mainly in normal code - not in slot calls.
		*/
		void appendUnicode(QString s, uint base = 16);

	private:
		ScribusDoc *m_doc;
		//! \brief Number of the columns for model
		int m_cols;

		QString m_fontInUse;
		CharClassDef m_characters;

		Qt::ItemFlags flags(const QModelIndex &index) const;
};


/*! \brief A visual widget for displaying the unicode glyphs map.
setAcceptDrops() note:
It sets the right-button behaviour too. It enables delete popup
menu when e is true instead of larger preview dialog. The idea:
When user can drop items into it, he could want to delete it too.
\author Petr Vanek <petr@scribus.info>
*/
class CharTableView : public QTableView
{
	Q_OBJECT

	public:
		CharTableView(QWidget * parent = 0);

	signals:
		void selectChar(uint);
		//! \brief When user press the DELETE/BACKSPACE key
		void delChar();

	protected:
		//! \brief Magnify dialog reference
		CharZoom* zoom;
		bool mPressed;
		QPoint mousePos;

		QAction * deleteAct;
		QMenu * actionMenu;

		CharTableModel * model();
		void keyPressEvent(QKeyEvent *k);
		void mouseReleaseEvent(QMouseEvent *m);
		void mousePressEvent(QMouseEvent* e);
		void mouseMoveEvent(QMouseEvent* e);
		// d'n'd
		void dropEvent(QDropEvent *e);
		void dragEnterEvent(QDragEnterEvent * e);
		void dragMoveEvent(QDragMoveEvent *e);

		int currentValue();
		

	private slots:
		void removeCharacter();
};

#endif
