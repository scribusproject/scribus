/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          picstatus.cpp  -  description
                             -------------------
    begin                : Fri Nov 29 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "picstatus.h"

#include <QListWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include <QPixmap>
#include <QFileInfo>
#include <QPainter>
#include <QAction>
#include <QMenu>
#include <cstdio>

#include "commonstrings.h"
#include "effectsdialog.h"
#include "extimageprops.h"
#include "filesearch.h"
#include "pageitem.h"
#include "picsearch.h"
#include "picsearchoptions.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "units.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"


PicItem::PicItem(QListWidget* parent, QString text, QPixmap pix, PageItem* pgItem)
	: QListWidgetItem(pix, text, parent)
{
	PageItemObject = pgItem;
}

PicStatus::PicStatus(QWidget* parent, ScribusDoc *docu) : QDialog( parent )
{
	setupUi(this);
	setModal(true);
	imageViewArea->setIconSize(QSize(128, 128));
	imageViewArea->setContextMenuPolicy(Qt::CustomContextMenu);
	m_Doc = docu;
	currItem = NULL;
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	fillTable();
	workTab->setCurrentIndex(0);
	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(imageViewArea, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(imageSelected(QListWidgetItem*)));
	connect(isPrinting, SIGNAL(clicked()), this, SLOT(PrintPic()));
	connect(isVisibleCheck, SIGNAL(clicked()), this, SLOT(visiblePic()));
	connect(goPageButton, SIGNAL(clicked()), this, SLOT(GotoPic()));
	connect(selectButton, SIGNAL(clicked()), this, SLOT(SelectPic()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(SearchPic()));
	connect(effectsButton, SIGNAL(clicked()), this, SLOT(doImageEffects()));
	connect(buttonLayers, SIGNAL(clicked()), this, SLOT(doImageExtProp()));
	connect(buttonEdit, SIGNAL(clicked()), this, SLOT(doEditImage()));
	connect(imageViewArea, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(slotRightClick()));
}

QPixmap PicStatus::createImgIcon(PageItem* item)
{
	QPainter p;
	QPixmap pm(128, 128);
	QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
	p.begin(&pm);
	p.fillRect(0, 0, 128, 128, imageViewArea->palette().window());
	p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p.setBrush(palette().window());
	p.drawRoundRect(0, 0, 127, 127, 10, 10);
	p.setPen(Qt::NoPen);
	p.setBrush(b);
	p.drawRect(12, 12, 104, 104);
	if (item->PicAvail)
	{
		QImage im2 = item->pixm.scaled(104, 104, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		p.drawImage((104 - im2.width()) / 2 + 12, (104 - im2.height()) / 2 + 12, im2);
	}
	else
	{
		p.setBrush(Qt::NoBrush);
		p.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		p.drawLine(12, 12, 116, 116);
		p.drawLine(12, 116, 116, 12);
	}
	p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p.setBrush(Qt::NoBrush);
	p.drawRect(12, 12, 104, 104);
	p.end();
	return pm;
}

void PicStatus::fillTable()
{
	PageItem *item;
	imageViewArea->clear();
	QListWidgetItem *firstItem=0;
	QListWidgetItem *tempItem=0;

	for (int i = 0; i < m_Doc->MasterItems.count(); ++i)
	{
		item = m_Doc->MasterItems.at(i);
		QFileInfo fi = QFileInfo(item->Pfile);
		if ((item->itemType() == PageItem::ImageFrame) && (!item->asLatexFrame()))
			tempItem = new PicItem(imageViewArea, fi.fileName(), createImgIcon(item), item);
		if (firstItem == 0)
			firstItem = tempItem;
	}

	for (int i = 0; i < m_Doc->Items->count(); ++i)
	{
		item = m_Doc->Items->at(i);
		QFileInfo fi = QFileInfo(item->Pfile);
		if ((item->itemType() == PageItem::ImageFrame) && (!item->asLatexFrame()))
			tempItem = new PicItem(imageViewArea, fi.fileName(), createImgIcon(item), item);
		// if an image is selected in a doc, Manage Pictures should
		// display the selected image and its values
		if (firstItem == 0 || item->isSelected())
			firstItem = tempItem;
	}
	imageViewArea->setCurrentItem(firstItem);
	if (firstItem!=0)
		imageSelected(firstItem);

	// Disable all features when there is no image in the document.
	// It should never be used (see ScribusMainWindow::extrasMenuAboutToShow())
	// but who knows if it can be configured for shortcut or macro...
	imageViewArea->setEnabled(imageViewArea->count() > 0);
	workTab->setEnabled(imageViewArea->count() > 0);
	sortByName();
}

void PicStatus::sortByName()
{
	QListWidgetItem *firstItem = 0;
	QMap<QString, PicItem*> sorted;
	int num = imageViewArea->count();
	if (num != 0)
	{
		firstItem = imageViewArea->currentItem();
		for (int a = num-1; a > -1; --a)
		{
			QListWidgetItem *ite = imageViewArea->takeItem(a);
			PicItem *item = (PicItem*)ite;
			QFileInfo fi = QFileInfo(item->PageItemObject->Pfile);
			sorted.insertMulti(fi.fileName(), item);
		}
		int counter = 0;
		foreach (QString i, sorted.uniqueKeys())
		{
			foreach (PicItem* val, sorted.values(i))
			{
				imageViewArea->insertItem(counter, val);
				counter++;
			}
		}
		imageViewArea->setCurrentItem(firstItem);
		imageSelected(firstItem);
		sortOrder = 0;
	}
}

void PicStatus::sortByPage()
{
	QListWidgetItem *firstItem = 0;
	QMap<int, PicItem*> sorted;
	int num = imageViewArea->count();
	if (num != 0)
	{
		firstItem = imageViewArea->currentItem();
		for (int a = num-1; a > -1; --a)
		{
			QListWidgetItem *ite = imageViewArea->takeItem(a);
			PicItem *item = (PicItem*)ite;
			sorted.insertMulti(item->PageItemObject->OwnPage, item);
		}
		int counter = 0;
		foreach (int i, sorted.uniqueKeys())
		{
			foreach (PicItem* val, sorted.values(i))
			{
				imageViewArea->insertItem(counter, val);
				counter++;
			}
		}
		imageViewArea->setCurrentItem(firstItem);
		imageSelected(firstItem);
		sortOrder = 1;
	}
}

void PicStatus::slotRightClick()
{
	QMenu *pmen = new QMenu();
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	QAction* Act1 = pmen->addAction( tr("Sort by Name"));
	Act1->setCheckable(true);
	QAction* Act2 = pmen->addAction( tr("Sort by Page"));
	Act2->setCheckable(true);
	if (sortOrder == 0)
		Act1->setChecked(true);
	else if (sortOrder == 1)
		Act2->setChecked(true);
	connect(Act1, SIGNAL(triggered()), this, SLOT(sortByName()));
	connect(Act2, SIGNAL(triggered()), this, SLOT(sortByPage()));
	pmen->exec(QCursor::pos());
	delete pmen;
}

void PicStatus::imageSelected(QListWidgetItem *ite)
{
	if (ite != NULL)
	{
		PicItem *item = (PicItem*)ite;
		currItem = item->PageItemObject;
		if (!currItem->OnMasterPage.isEmpty())
			displayPage->setText(currItem->OnMasterPage);
		else
		{
			if (currItem->OwnPage == -1)
				displayPage->setText(  tr("Not on a Page"));
			else
				displayPage->setText(QString::number(currItem->OwnPage + 1));
		}
		displayObjekt->setText(currItem->itemName());
		if (currItem->PicAvail)
		{
			QFileInfo fi = QFileInfo(currItem->Pfile);
			QString ext = fi.suffix().toLower();
			displayName->setText(fi.fileName());
			displayPath->setText(QDir::convertSeparators(fi.path()));
			QString format = "";
			switch (currItem->pixm.imgInfo.type)
			{
				case 0:
					format = tr("JPG");
					break;
				case 1:
					format = tr("TIFF");
					break;
				case 2:
					format = tr("PSD");
					break;
				case 3:
					format = tr("EPS/PS");
					break;
				case 4:
					format = tr("PDF");
					break;
				case 5:
					format = tr("JPG2000");
					break;
				case 6:
					format = ext.toUpper();
					break;
				case 7:
					format = tr("emb. PSD");
					break;
			}
			displayFormat->setText(format);
			QString cSpace;
			if ((extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext)) && (currItem->pixm.imgInfo.type != ImageType7))
				cSpace = tr("Unknown");
			else
				cSpace=colorSpaceText(currItem->pixm.imgInfo.colorspace);
			displayColorspace->setText(cSpace);
			displayDPI->setText(QString("%1 x %2").arg(qRound(currItem->pixm.imgInfo.xres)).arg(qRound(currItem->pixm.imgInfo.yres)));
			displayEffDPI->setText(QString("%1 x %2").arg(qRound(72.0 / currItem->imageXScale())).arg(qRound(72.0 / currItem->imageYScale())));
			displaySizePixel->setText(QString("%1 x %2").arg(currItem->OrigW).arg(currItem->OrigH));
			displayScale->setText(QString("%1 x %2 %").arg(currItem->imageXScale() * 100 / 72.0 * currItem->pixm.imgInfo.xres, 5, 'f', 1).arg(currItem->imageYScale() * 100 / 72.0 * currItem->pixm.imgInfo.yres, 5, 'f', 1));
			displayPrintSize->setText(QString("%1 x %2%3").arg(currItem->OrigW * currItem->imageXScale() * m_Doc->unitRatio(), 7, 'f', 2).arg(currItem->OrigH * currItem->imageXScale() * m_Doc->unitRatio(), 7, 'f', 2).arg(unitGetSuffixFromIndex(m_Doc->unitIndex())));
			isPrinting->setChecked(currItem->printEnabled());
			isVisibleCheck->setChecked(currItem->imageShown());
			buttonEdit->setEnabled(currItem->isRaster);
			effectsButton->setEnabled(currItem->isRaster);
			buttonLayers->setEnabled(currItem->pixm.imgInfo.valid);
			searchButton->setEnabled(true);
		}
		else
		{
			QString trNA = tr("n/a");
			if (!currItem->Pfile.isEmpty())
			{
				QFileInfo fi = QFileInfo(currItem->Pfile);
				displayName->setText(fi.fileName());
				displayPath->setText(QDir::convertSeparators(fi.path()));
				searchButton->setEnabled(true);
			}
			else
			{
				displayName->setText(trNA);
				displayPath->setText(trNA);
				searchButton->setEnabled(false);
			}
			displayFormat->setText(trNA);
			displayColorspace->setText(trNA);
			displayDPI->setText(trNA);
			displayEffDPI->setText(trNA);
			displaySizePixel->setText(trNA);
			displayScale->setText(trNA);
			displayPrintSize->setText(trNA);
			buttonEdit->setEnabled(false);
			effectsButton->setEnabled(false);
			buttonLayers->setEnabled(false);
		}
	}
	else
	{
		currItem = NULL;
		imageViewArea->clearSelection();
	}
}

void PicStatus::PrintPic()
{
	if (currItem != NULL)
		currItem->setPrintEnabled(isPrinting->isChecked());
}

void PicStatus::visiblePic()
{
	if (currItem != NULL)
	{
		currItem->setImageShown(isVisibleCheck->isChecked());
		emit refreshItem(currItem);
	}
}

void PicStatus::GotoPic()
{
	if (currItem != NULL)
	{
		ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
		if (!currItem->OnMasterPage.isEmpty())
			emit selectMasterPage(currItem->OnMasterPage);
		else
			emit selectPage(currItem->OwnPage);
	}
}

void PicStatus::SelectPic()
{
	if (currItem == NULL)
		return;

	ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
	if (currItem->Groups.count() == 0)
		emit selectElement(currItem->OwnPage, currItem->ItemNr, false);
	else
	{
		if (currItem->isGroupControl)
			emit selectElement(currItem->OwnPage, currItem->ItemNr, false);
		else
			emit selectElement(currItem->OwnPage, currItem->ItemNr, true);
	}
}

bool PicStatus::loadPict(const QString & newFilePath)
{
	// Hack to fool the LoadPict function
	currItem->Pfile = newFilePath;
	m_Doc->LoadPict(newFilePath, currItem->ItemNr, true);
	return currItem->PicAvail;
}

void PicStatus::SearchPic()
{
	// no action where is no item selected. It should never happen.
	if (currItem == NULL)
		return;
	static QString lastSearchPath;

	if (lastSearchPath.isEmpty())
		lastSearchPath = displayPath->text();
	PicSearchOptions *dia = new PicSearchOptions(this, displayName->text(), lastSearchPath);
	if (dia->exec())
	{
		lastSearchPath = dia->getLastDirSearched();
		if (dia->getMatches().count() == 0)
		{
			QMessageBox::information(this, tr("Scribus - Image Search"), tr("No images named \"%1\" were found.").arg(dia->getFileName()),
					QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
					QMessageBox::NoButton);
		}
		else
		{
			PicSearch *dia2 = new PicSearch(this, dia->getFileName(), dia->getMatches());
			if (dia2->exec())
			{
				Q_ASSERT(!dia2->currentImage.isEmpty());
				loadPict(dia2->currentImage);
				refreshItem(currItem);
				QFileInfo fi = QFileInfo(currItem->Pfile);
				imageViewArea->currentItem()->setText(fi.fileName());
				imageViewArea->currentItem()->setIcon(createImgIcon(currItem));
				imageSelected(imageViewArea->currentItem());
			}
			delete dia2;
		}
	}
	delete dia;
}

void PicStatus::doImageEffects()
{
	if (currItem == NULL)
		return;

	EffectsDialog* dia = new EffectsDialog(this, currItem, m_Doc);
	if (dia->exec())
	{
		currItem->effectsInUse = dia->effectsList;
		loadPict(currItem->Pfile);
		refreshItem(currItem);
		imageViewArea->currentItem()->setIcon(createImgIcon(currItem));
	}
	delete dia;
}

void PicStatus::doImageExtProp()
{
	if (currItem != NULL)
	{
		ExtImageProps* dia = new ExtImageProps(this, &currItem->pixm.imgInfo, currItem, m_Doc->view());
		dia->exec();
		loadPict(currItem->Pfile);
		refreshItem(currItem);
		imageViewArea->currentItem()->setIcon(createImgIcon(currItem));
		delete dia;
	}
}

void PicStatus::doEditImage()
{
	if (currItem != NULL)
	{
		SelectPic();
		ScCore->primaryMainWindow()->callImageEditor();
	}
}
