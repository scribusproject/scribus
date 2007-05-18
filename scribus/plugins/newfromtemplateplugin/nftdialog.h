/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/

#ifndef NFTDIALOG_H
#define NFTDIALOG_H

#include <utility>
#include <vector>

#include "ui_nftdialog.h"
#include "nftsettings.h"

typedef std::pair<nfttemplate*, QListWidgetItem*> ListItem;

class nftdialog: public QDialog, public Ui::nftdialog
{
	Q_OBJECT

private:
	nftsettings* settings;
// 	Q3ListBox* categoryList;
// 	Q3IconView* tnailGrid;
// 	Q3Frame* detailBar;
// 	Q3TextBrowser* infoLabel;
// 	Q3IconView* imgLabel;
// 	Q3TextBrowser* aboutLabel;
// 	QWidget* tmpSpacer;
// 	QToolButton* imgButton;
// 	QToolButton* infoButton;
// 	QToolButton* aboutButton;
// 	QPushButton* settingsButton;
// 	QPushButton* okButton;
// 	QPushButton* cancelButton;
// 	Q3BoxLayout* mainlo;
// 	Q3BoxLayout* detaillo;
// 	Q3BoxLayout* dtbarlo;
// 	Q3PopupMenu* popUp;
	std::vector<ListItem*> iconItems;
	void setupCategories();
	void setupListItems();
	void setupAbout();
public:
	nfttemplate* currentDocumentTemplate;
	nftdialog(QWidget* parent, QString lang, QString templateDir);
	~nftdialog();
private slots: 
	void setTNails();
// 	void infoToggle();
// 	void imgToggle();
// 	void aboutToggle();
	void setInfo();
// 	void exitOK();
// 	void exitCancel();
	void getCurrentDocumentTemplate(QListWidgetItem* item);
// 	void showPopup(QListWidgetItem* item, const QPoint& point);
	void removeTemplate();
};

#endif
