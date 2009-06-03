/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PICSTATUS_H
#define PICSTATUS_H

#include "scribusapi.h"
#include "ui_picstatus.h"
#include <QListWidgetItem>

class ScribusDoc;
class PageItem;

class SCRIBUS_API PicItem : public QListWidgetItem
{
public:
	PicItem(QListWidget* parent, QString text, QPixmap pix, PageItem* pgItem);
	~PicItem() {};
	PageItem *PageItemObject;
};


/*! \brief Constructs a Dialog, which list all Images in the current Document.
In this Dialog it is possible to search for missing Images. */
class SCRIBUS_API PicStatus : public QDialog, Ui::PicStatus
{ 
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\brief Constructs a Dialog, which list all Images in the current Document. In this Dialog it is possible
		to search for missing Images.
	\param parent Pointer to parent window
	\param docu Pointer to the current Document
	 */
	PicStatus(QWidget* parent, ScribusDoc *docu);
	~PicStatus() {};

	QPixmap createImgIcon(PageItem* item);

	/*! \brief A slot called when there is a request to re-fill the table
	of images. It clears the table at first. Then it iterates through
	all items in MasterItems and Items too. */
	void fillTable();

private slots:
	void sortByName();
	void sortByPage();
	void slotRightClick();
	void imageSelected(QListWidgetItem *ite);
	/*!
	\author Franz Schmid
	\brief Enables or disables printing of the selected Image.
	*/
	void PrintPic();
	/*!
	\author Franz Schmid
	\brief Enables or disables viewing of the selected Image.
	*/
	void visiblePic();
	/*!
	\author Franz Schmid
	\brief Moves to the Page containing the selected Image.
	 */
	void GotoPic();

	/*!
	\author Franz Schmid
	\brief Selects the object containing the selected Image.
	 */
	void SelectPic();
	/*!
	\author Franz Schmid
	\brief Searches for the given Picture. Displays a Dialog when more than one Picture is found.
	*/
	void SearchPic();
	void doImageEffects();
	void doImageExtProp();
	void doEditImage();

signals:
	void selectPage(int);
	void selectMasterPage(QString);
	void selectElement(int, int, bool);
	void refreshItem(PageItem*);

protected:
	/*! \brief Load the image specified into the PageItem
	\param newFilePath a file path */
	bool loadPict(const QString & newFilePath);

private:
	ScribusDoc *m_Doc;
	PageItem *currItem;
	int sortOrder;

};
#endif
