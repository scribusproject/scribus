/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UNICODESEARCH_H
#define UNICODESEARCH_H

#include <QPushButton>
// #include <QTimer>

#include "ui_unicodesearch.h"
#include "scribusapi.h"

class UnicodeSearchModel;
class QSortFilterProxyModel;


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
	*/
	UnicodeSearch(QWidget* parent = 0);
	~UnicodeSearch();

signals:
	//! \brief Emitted when the dialog gets hidden.
	void setVisibleState(bool);
	/*! \brief Emitted when user choses any value.
	\param hex a hex value of glyph - see m_keys.
	*/
	void glyphSelected(const QString & hex);

private:
	//! Searching wrapper
	QSortFilterProxyModel * m_proxyModel;
	//! Data model
	UnicodeSearchModel * m_model;

	//! \brief Overriden hide event. Just emit the setVisibleState(false) here.
	void hideEvent(QHideEvent * e);

	private slots:
		//! Perform the search and switch the focus to the tableView
		void searchEdit_returnPressed();
		//! Handle items from view to promote upstream and close the widget
		void itemChosen(const QModelIndex & index);
};


/*!\brief Qt4 model for QTableView used in UnicodeSearch dialog.
It contains only 2 columns with hex-description pairs/rows of
unicode glyphs (loaded from unicodemap/unicodenameslist.txt.
Hex and descriptions are stored in QMap description+code/code
for performance.
\author Petr Vanek <petr@scribus.info>
*/
class UnicodeSearchModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		UnicodeSearchModel(QObject * parent = 0);
		~UnicodeSearchModel();

		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;

		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

		//! \brief Return hex-key for the row of given index. See m_keys.
		QString hexData(const QModelIndex & index);

	private:

		//! \brief Easier to use QPair-like replacement
		struct UnicodeStruct
		{
			QString hex;
			QString description;
		};
		/*! \brief Internal hex-description mapping.
		It's filled in constructor only once. All searching re-fills
		of the unicodeList are performed on this list (used
		as model's data) */
		QList<UnicodeStruct> m_unicode;

/*		QVariant headerData(int section,
							Qt::Orientation orientation,
							int role = Qt::DisplayRole) const;*/
};

/*! \brief A special widget to cooperate with UnicodeSearch.
Construct a toggle push button. When it's toggled, the search dialog
is shown. It is an "apply" button too.
Search dialog is constructed on demand only.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API UnicodeChooseButton : public QPushButton
{
	Q_OBJECT

public:
	/*! \brief QWidget like constructor.
	\param parent a QWidget parent
	*/
	UnicodeChooseButton(QWidget * parent);
	~UnicodeChooseButton(){};

	virtual void changeEvent(QEvent *e);

signals:
	//! \brief Signal transfering the chosen character as QString
	void chosenUnicode(const QString &);

public slots:
    //! \brief Slot for changing language of GUI
    void languageChange();

private:
	/*! \brief UnicodeSearch reference.
	The dialog is created in "this" constructor. Showing and hiding
	is handled by toggled() signal catched in self_toggled() */
	UnicodeSearch* m_searchDialog;

	/* \brief Hold cache timer.
	It triggers the m_searchDialog deletion when it's no longer used
	by user.
	*/
// 	QTimer * m_cacheTimer;

private slots:
	//! \brief Handle toggle state (show/hide) search dialog.
	void self_toggled(bool);
	//! \brief Handle various signals - user inputs (clicked, return pressed etc.)
	void glyphSelected(const QString & hex);
	// \brief Flush the cached m_searchDialog (destroy it).
// 	void deleteSearchDialog();
};

#endif
