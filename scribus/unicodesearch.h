/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UNICODESEARCH_H
#define UNICODESEARCH_H

#include <qvariant.h>
#include <qdialog.h>
#include <qpushbutton.h>

#include "unicodesearchbase.h"


/*! \brief Special "search for unicode character" dialog.
The search string entered by user is searched in character
description and in the hex representation (as string) too.
See directory ./unicodemap/ for more info
\author Petr Vanek <petr@scribus.info>
*/
class UnicodeSearch : public UnicodeSearchBase
{
	Q_OBJECT

public:
	/*! \brief QDialog like constructor
	\param parent standard qt widget
	\param name string name
	\param modal is modal flag */
	UnicodeSearch(QWidget* parent = 0, const char* name = 0, bool modal = false);
	~UnicodeSearch(){};
	/*! \brief Check if is the m_unicodeMap filled and perform data read if it's empty.
	It's in separate method to keep the constructor lightweight. This method is
	called first time user requests the dialog. */
	void checkForUpdate();

signals:
	//! \brief Emitted when the dialog gets hidden.
	void setVisibleState(bool);

private:
	/*! \brief Internal hex-description mapping.
	It's filled in constructor only once. All searching re-fills
	of the unicodeList are performed on this map */
	QMap<QString,QString> m_unicodeMap;

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

private slots:
	//! \brief Start search with user input.
	void searchEdit_returnPressed();
};


/*! \brief A special widget to cooperate with UnicodeSearch.
Construct a toggle push button. When it's toggled, the search dialog
is shown. There is a "apply" button too.
\author Petr Vanek <petr@scribus.info>
*/
class UnicodeChooseButton : public QWidget
{
	Q_OBJECT

public:
	/*! \brief QWidget like constructor.
	\param parent a QWidget parent
	\param name string name of the widget
	*/
	UnicodeChooseButton(QWidget * parent, const char * name = 0);
	~UnicodeChooseButton(){};

signals:
	void clicked();

private:
	/*! \brief UnicodeSearch reference.
	The dialog is created in "this" constructor. Showing and hiding
	is handled by toggled() signal catched in self_toggled() */
	UnicodeSearch* m_searchDialog;
	//! \brief A button to open/close m_searchDialog.
	QPushButton* searchButton;
	//! \brief A button for selected character operations.
	QPushButton* selectButton;

private slots:
	//! \brief Handle toggle state (show/hide) search dialog.
	void self_toggled(bool);
	//! \brief emit the local clicked() signal.
	void selectButton_clicked();

};


#endif
