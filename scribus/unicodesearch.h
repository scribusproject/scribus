/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UNICODESEARCH_H
#define UNICODESEARCH_H

#include <QVariant>
#include <QDialog>
#include <QPushButton>
#include <QHideEvent>

#include "ui_unicodesearch.h"
#include "scribusapi.h"
#include "fonts/scface.h"

class CharZoom;

/*! \brief Special "search for unicode character" dialog.
The search string entered by user is searched in character
description and in the hex representation (as string) too.
See directory ./unicodemap/ for more info
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API UnicodeSearch : public QDialog, public Ui::UnicodeSearch
{
	Q_OBJECT

public:
	/*! \brief QDialog like constructor
	\param parent standard qt widget
	\param name string name
	\param modal is modal flag */
	UnicodeSearch(QWidget* parent = 0);
	~UnicodeSearch(){};
	/*! \brief Check if is the m_unicodeMap filled and perform data read if it's empty.
	It's in separate method to keep the constructor lightweight. This method is
	called first time user requests the dialog. */
	void checkForUpdate();
	//! \brief Set currently used font
	void setFont(ScFace f) { m_font = f; };

signals:
	//! \brief Emitted when the dialog gets hidden.
	void setVisibleState(bool);

private:
	/*! \brief Internal hex-description mapping.
	It's filled in constructor only once. All searching re-fills
	of the unicodeList are performed on this map */
	QMap<QString,QString> m_unicodeMap;

	//! \brief A reference to the CharZoom dialog shown/closed in unicodeList_mouseButtonPressed() slot.
	CharZoom * m_zoom;
	//! \brief Currently used font
	ScFace m_font;

	//! \brief All items from m_unicodeMap into unicodeList.
	void query();
	/*! \brief Fill unicodeList with all occurences of "filter" substrings.
	\param filter a mask. Search is perfomed like "*foo*" in descriptions and hex representations too.
	*/
	void query(QString filter);
	//! \brief Open the file with unicode map and fill the m_unicodeMap.
	void readUnicodeMap();
	//! \brief Overriden hide event. Just emit the setVisibleState(false) here.
	void hideEvent(QHideEvent * e);
	//! \brief filter mouse clicks so we can get our glyph zoom view
// 	bool eventFilter(QObject *obj, QEvent *event);

private slots:
	//! \brief Start search with user input.
	void searchEdit_returnPressed();
	//! \brief Handle the m_zoom dialog
	void unicodeList_mouseButtonPressed(QTableWidgetItem* item);
};


/*! \brief A special widget to cooperate with UnicodeSearch.
Construct a toggle push button. When it's toggled, the search dialog
is shown. It is an "apply" button too.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API UnicodeChooseButton : public QPushButton
{
	Q_OBJECT

public:
	/*! \brief QWidget like constructor.
	\param parent a QWidget parent
	\param name string name of the widget
	*/
	UnicodeChooseButton(QWidget * parent);
	~UnicodeChooseButton(){};
	//! \brief Set currently used font. Passed in the m_searchDialog here only.
	void setFont(ScFace f) { m_searchDialog->setFont(f); };

signals:
	//! \brief Signal transfering the chosen character as QString
	void chosenUnicode(QString);

public slots:
    //! \brief Slot for changing language of GUI
    void languageChange();

private:
	/*! \brief UnicodeSearch reference.
	The dialog is created in "this" constructor. Showing and hiding
	is handled by toggled() signal catched in self_toggled() */
	UnicodeSearch* m_searchDialog;

private slots:
	//! \brief Handle toggle state (show/hide) search dialog.
	void self_toggled(bool);
	//! \brief Handle various signals - user inputs (clicked, return pressed etc.)
	void unicodeList_chosen(QTableWidgetItem *);
	void unicodeList_chosen(int row, int column);

};

#endif
