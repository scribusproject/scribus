/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHECKDOCUMENT_H
#define CHECKDOCUMENT_H

#include <QMap>

class QEvent;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class QVBoxLayout;

#include "scribusapi.h"
#include "scrpalettebase.h"

class ScribusDoc;
class ScComboBox;


/*! \brief Preflight Verifier GUI (P.V.)
A tool to check document for errors (in P.V. profiles)
which can be set up in Preferences dialog.
*/
class SCRIBUS_API CheckDocument : public ScrPaletteBase
{
	Q_OBJECT

public:
	CheckDocument( QWidget* parent, bool modal );
	~CheckDocument() {};
	
	virtual void changeEvent(QEvent *e);

	/*! \brief State of the P.V. */
	enum CheckMode { checkNULL, checkPDF, checkEPS, checkPrint, checkPrintPreview };

	/*! \brief Clean the list view tree and reset the P.V. attributes. */
	void clearErrorList();
	/*! \brief Get all possible errors for given document
	\param doc a reference to the ScribusDoc */
	void buildErrorList(ScribusDoc *doc);
	/*! \brief Enable/disable "ignore" button and noButton property
	\param state true to enable the button */
	void setIgnoreEnabled(bool state);
	/*! \brief Get the state of the "ignore" button
	\retval true on button is visible */
	bool isIgnoreEnabled();

	//! \brief Current state of P.V.
	CheckMode checkMode;

public slots:
	/*! \brief Called when is selected a new item in error list.
	\param ite and item */
	void slotSelect(QTreeWidgetItem* ite);
	/*!\brief Do a manual rechecking. */
	void doReScan();
	/*! \brief Process error checking itself.
	\param name a QString with P.V. profile name */
	void newScan(const QString& name);

signals:
	//void rescan();
	//! \brief Signal emitted when user selects any page item in error list.
	void selectElement(int, int);
	//! \brief Signal emitted when user selects any page in error list.
	void selectPage(int);
	//! \brief Signal emitted when user selects any master page in error list.
	void selectMasterPage(QString);
	//! \brief Signal emitted when user selects any master page item in error list.
	void selectMasterPageElement(QString, int);
	//void selectNormal();
	//! \brief Signal emitted when user press the "ignore errors" button.
	void ignoreAllErrors();

protected:
	QVBoxLayout* checkDocumentLayout;
	QHBoxLayout* layout1;
	QHBoxLayout* layout2;

protected slots:
	virtual void languageChange();

private:
	//! \brief Mappping Page Item - item nr.
	QMap<QTreeWidgetItem*, int> itemMap;
	//! \brief Mappping Page - page nr.
	QMap<QTreeWidgetItem*, int> pageMap;
	//! \brief Mappping Master Page - MP nr.
	QMap<QTreeWidgetItem*, QString> masterPageMap;
	//! \brief Mappping MP Item - MP item nr.
	QMap<QTreeWidgetItem*, int> masterPageItemMap;

	//! \brief a reference to the current document
	ScribusDoc* m_Doc;
	//! \brief Icon for fatal error
	QPixmap graveError;
	//! \brief Icon for warning
	QPixmap onlyWarning;
	//! \brief Icon for OK
	QPixmap noErrors;

	//! \brief Strings for common texts in GUI dialog tree
	QString missingGlyph;
	QString textOverflow;
	QString notOnPage;
	QString missingImg;
	QString lowDPI;
	QString highDPI;
	QString transpar;
	QString annot;
	QString rasterPDF;
	QString isGIF;
	QString isGIFtoolTip;
	QString WrongFont;

	//! \brief Flag if is ignore button shown. true = hidden, false = shown.
	bool noButton;
	ScComboBox* curCheckProfile;
	QLabel* textLabel1;
	QTreeWidget* reportDisplay;
	QPushButton* ignoreErrors;
	QPushButton* reScan;
};

#endif // CHECKDOCUMENT_H
