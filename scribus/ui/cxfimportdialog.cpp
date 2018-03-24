/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDialog>
#include <QListWidgetItem>

#include "cxfimportdialog.h"
#include "iconmanager.h"

CxfImportDialog::CxfImportDialog(QWidget* parent)
               : QDialog(parent)
{
	setupUi(this);
	setDefaultPriorities();
	
	buttonUp->setIcon(IconManager::instance()->loadIcon("16/go-up.png"));
	buttonUp->setEnabled(false);
	buttonDown->setIcon(IconManager::instance()->loadIcon("16/go-down.png"));
	buttonDown->setEnabled(false);

	connect(buttonUp, SIGNAL(clicked()), this, SLOT(moveUp()));
	connect(buttonDown, SIGNAL(clicked()), this, SLOT(moveDown()));
	connect(colorspaceListBox, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(select(QListWidgetItem*)));
}

void CxfImportDialog::insertColorspaceLab()
{
	QListWidgetItem* itemLab = new QListWidgetItem( tr("Lab"), colorspaceListBox);
	itemLab->setData(Qt::UserRole, QVariant((int) ColorSpace_Lab));
}

void CxfImportDialog::insertColorspaceRGB()
{
	QListWidgetItem* itemRGB = new QListWidgetItem( tr("RGB"), colorspaceListBox);
	itemRGB->setData(Qt::UserRole, QVariant((int) ColorSpace_Rgb));
}

void CxfImportDialog::insertColorspaceCMYK()
{
	QListWidgetItem* itemCMYK = new QListWidgetItem( tr("CMYK"), colorspaceListBox);
	itemCMYK->setData(Qt::UserRole, QVariant((int) ColorSpace_Cmyk));
}

QList<eColorSpaceType> CxfImportDialog::priorities() const
{
	QList<eColorSpaceType> priorities;

	for (int i = 0; i < colorspaceListBox->count(); ++i)
	{
		QListWidgetItem* item = colorspaceListBox->item(i);
		eColorSpaceType colorspace = (eColorSpaceType) item->data(Qt::UserRole).toInt();
		priorities.append(colorspace);
	}

	return priorities;
}

void CxfImportDialog::setDefaultPriorities()
{
	colorspaceListBox->clear();

	insertColorspaceLab();
	insertColorspaceCMYK();
	insertColorspaceRGB();
}

void CxfImportDialog::setPriorities(const QList<eColorSpaceType>& priorities)
{
	colorspaceListBox->clear();

	for (int i = 0; i < priorities.count(); ++i)
	{
		eColorSpaceType colorspace = priorities.at(i);
		if (colorspace == ColorSpace_Lab)
			insertColorspaceLab();
		else if (colorspace == ColorSpace_Cmyk)
			insertColorspaceCMYK();
		else if (colorspace == ColorSpace_Rgb)
			insertColorspaceRGB();
	}

	if (colorspaceListBox->count() < 2)
		setDefaultPriorities();
}

void CxfImportDialog::moveUp()
{
	int curr = colorspaceListBox->currentRow();
	if (curr == 0)
		return;
	QListWidgetItem *it = colorspaceListBox->takeItem(curr);
	colorspaceListBox->insertItem(curr-1, it);
	colorspaceListBox->setCurrentItem(it);
}

void CxfImportDialog::moveDown()
{
	int curr = colorspaceListBox->currentRow();
	if (curr == static_cast<int>(colorspaceListBox->count())-1)
		return;
	QListWidgetItem *it = colorspaceListBox->takeItem(curr);
	colorspaceListBox->insertItem(curr+1, it);
	colorspaceListBox->setCurrentItem(it);
}

void CxfImportDialog::select(QListWidgetItem* item)
{
	colorspaceListBox->setCurrentItem(item);
	int curr = colorspaceListBox->currentRow();
	if (curr == 0)
	{
		buttonUp->setEnabled(false);
		buttonDown->setEnabled(true);
	}
	else if (curr == colorspaceListBox->count() - 1)
	{
		buttonUp->setEnabled(true);
		buttonDown->setEnabled(false);
	}
	else
	{
		buttonUp->setEnabled(true);
		buttonDown->setEnabled(true);
	}
}
