/***************************************************************************
 *   Riku Leino, riku.leino@gmail.com                                          *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef NFTDIALOG_H
#define NFTDIALOG_H

#include <utility>
#include <vector>
#include <qcursor.h>
#include <qdialog.h>
#include <qframe.h>
#include <qiconview.h> 
#include <qiconset.h>
#include <qiconview.h> 
#include <qlabel.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qpixmap.h> 
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qtextbrowser.h> 
#include <qtoolbutton.h>
#include "nftsettings.h"

typedef std::pair<nfttemplate*, QIconViewItem*> ListItem;

class nftdialog: public QDialog
{
	Q_OBJECT

private:
	nftsettings* settings;
	QListBox* categoryList;
	QIconView* tnailGrid;
	QFrame* detailBar;
	QTextBrowser* infoLabel;
	QIconView* imgLabel;
	QTextBrowser* aboutLabel;
	QWidget* tmpSpacer;
	QToolButton* imgButton;
	QToolButton* infoButton;
	QToolButton* aboutButton;
	QPushButton* settingsButton;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QBoxLayout* mainlo;
	QBoxLayout* detaillo;
	QBoxLayout* dtbarlo;
	QPopupMenu* popUp;
	std::vector<ListItem*> iconItems;
	void setupCategories();
	void setupListItems();
	void setupAbout();
public:
	nfttemplate* currentTemplate;
	nftdialog(QWidget* parent, QString lang, QString templateDir);
	~nftdialog();
private slots: 
	void setTNails();
	void infoToggle();
	void imgToggle();
	void aboutToggle();
	void setInfo(QIconViewItem* item);
	void exitOK();
	void exitCancel();
	void getCurrentTemplate(QIconViewItem* item);
	void showPopup(QIconViewItem* item, const QPoint& point);
	void removeTemplate();
};

#endif
