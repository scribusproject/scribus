/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARTABLEVIEW_H
#define CHARTABLEVIEW_H

#include <QTableView>

class CharTableModel;
class CharZoom;


/*! \brief A visual widget for displaying the unicode glyphs map.
setAcceptDrops() note:
It sets the right-button behaviour too. It enables delete popup
menu when e is true instead of larger preview dialog. The idea:
When user can drop items into it, he could want to delete it too.
\warning: CharTableModel and CharTableView are designed for 1:1 relations!
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

public slots:
	/*! \brief Handling selection changes from model's internals.
	There can arrive selection changes unrelated to GUI integration. */
	void modelSelectionChanged(QItemSelectionModel * model);

	//! \brief Calculate height of the last row to fit image size
	void resizeLastRow();

protected:
	//! \brief Magnify dialog reference
	CharZoom* zoom;

	QAction * deleteAct;
	QMenu * actionMenu;

	CharTableModel * model();
	void hideEvent(QHideEvent *);
	void keyPressEvent(QKeyEvent *k);
	void mouseReleaseEvent(QMouseEvent *m);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);

	//! \brief Map current QModelIndex into CharClassDef index/position
	int currenCharactersIndex();

	void hideZoomedChar();

	//! \brief Used for computing the items' pixmap size in the model.
	void resizeEvent(QResizeEvent *e);

private slots:
	void removeCharacter();
	void viewDoubleClicked(const QModelIndex & index);
};

#endif
