/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/

#ifndef NFTWIDGET_H
#define NFTWIDGET_H

#include <utility>
#include <vector>

#include "ui_nftwidget.h"
#include "nftsettings.h"

class QAction;


typedef std::pair<nfttemplate*, QListWidgetItem*> ListItem;


class nftwidget: public QWidget, public Ui::nftwidget
{
	Q_OBJECT

private:
	nftsettings* settings;
	QAction * removeAction;
	QAction * openAction;
	std::vector<ListItem*> iconItems;
	void setupCategories();
	void setupListItems();
	void setupAbout();
public:
	nfttemplate* currentDocumentTemplate;
	nftwidget(QWidget* parent);
	void setupSettings(QString lang);
	~nftwidget();
private slots: 
	void setInfo();
	void setThumbnails();
	void getCurrentDocumentTemplate(QListWidgetItem* item);
	void removeTemplate();
signals:
	void leaveOK();
	void ButtonBoxEnable(bool);
};

#endif
