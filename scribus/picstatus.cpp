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
#include <q3table.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qtoolbutton.h>
#include <qstringlist.h>
#include <q3textstream.h>
#include <qtooltip.h>
#include <qcursor.h>
#include <qpainter.h>
//Added by qt3to4:
#include <QPixmap>
#include <cstdio>

#include "picstatus.h"
//#include "picstatus.moc"
#include "picsearch.h"
#include "picsearchoptions.h"
#include "scribusdoc.h"
#include "pageitem.h"
#include "filesearch.h"
#include "scribuscore.h"
#include "commonstrings.h"
#include "effectsdialog.h"
#include "extimageprops.h"

extern QPixmap loadIcon(QString nam);

PicItem::PicItem(Q3IconView* parent, QString text, QPixmap pix, PageItem* pgItem) : Q3IconViewItem(parent, text, pix)
{
	PageItemObject = pgItem;
}

PicStatus::PicStatus(QWidget* parent, ScribusDoc *docu) : PicStatusBase( parent, "PicStatus", true, 0 )
{
	m_Doc = docu;
	currItem = NULL;
	setIcon(loadIcon("AppIcon.png"));
	fillTable();
	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(imageViewArea, SIGNAL(currentChanged(Q3IconViewItem *)), this, SLOT(imageSelected(Q3IconViewItem *)));
	connect(imageViewArea, SIGNAL(clicked(Q3IconViewItem*)), this, SLOT(imageSelected(Q3IconViewItem*)));
	connect(isPrinting, SIGNAL(clicked()), this, SLOT(PrintPic()));
	connect(isVisible, SIGNAL(clicked()), this, SLOT(visiblePic()));
	connect(goPageButton, SIGNAL(clicked()), this, SLOT(GotoPic()));
	connect(selectButton, SIGNAL(clicked()), this, SLOT(SelectPic()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(SearchPic()));
	connect(effectsButton, SIGNAL(clicked()), this, SLOT(doImageEffects()));
	connect(buttonLayers, SIGNAL(clicked()), this, SLOT(doImageExtProp()));
	connect(buttonEdit, SIGNAL(clicked()), this, SLOT(doEditImage()));
}

QPixmap PicStatus::createImgIcon(PageItem* item)
{
	QPainter p;
	QPixmap pm(128, 128);
	QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
	p.begin(&pm);
	p.fillRect(0, 0, 128, 128, imageViewArea->paletteBackgroundColor());
	p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p.setBrush(paletteBackgroundColor());
	p.drawRoundRect(0, 0, 128, 128, 10, 10);
	p.setPen(Qt::NoPen);
	p.setBrush(b);
	p.drawRect(12, 12, 104, 104);
	if (item->PicAvail)
	{
		QImage im2 = item->pixm.smoothScale(104, 104, QImage::ScaleMin);
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
	QPixmap pm(128, 128);
	QPainter p;
	imageViewArea->clear();
	for (item = m_Doc->MasterItems.first(); item; item = m_Doc->MasterItems.next())
	{
		QFileInfo fi = QFileInfo(item->Pfile);
		if (item->itemType() == PageItem::ImageFrame)
		{
			PicItem *ite =  new PicItem(imageViewArea, fi.fileName(), createImgIcon(item), item);
			ite->setDragEnabled(false);
		}
	}
	for (item = m_Doc->Items->first(); item; item = m_Doc->Items->next())
	{
		QFileInfo fi = QFileInfo(item->Pfile);
		if (item->itemType() == PageItem::ImageFrame)
		{
			PicItem *ite =  new PicItem(imageViewArea, fi.fileName(), createImgIcon(item), item);
			ite->setDragEnabled(false);
		}
	}
}

void PicStatus::imageSelected(Q3IconViewItem *ite)
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
			QString ext = fi.extension(false).lower();
			displayName->setText(fi.fileName());
			displayPath->setText(QDir::convertSeparators(fi.dirPath()));
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
					format = ext.upper();
					break;
				case 7:
					format = tr("emb. PSD");
					break;
			}
			displayFormat->setText(format);
			QString cSpace;
			if (((ext == "pdf") || (ext == "eps") || (ext == "epsi") || (ext == "ps")) && (currItem->pixm.imgInfo.type != 7))
				cSpace = tr("Unknown");
			else
			{
				switch (currItem->pixm.imgInfo.colorspace)
				{
					case 0:
						cSpace = tr("RGB");
						break;
					case 1:
						cSpace = tr("CMYK");
						break;
					case 2:
						cSpace = tr("Grayscale");
						break;
					case 3:
						cSpace = tr("Duotone");
						break;
				}
			}
			displayColorspace->setText(cSpace);
			displayDPI->setText(QString("%1 x %2").arg(qRound(currItem->pixm.imgInfo.xres)).arg(qRound(currItem->pixm.imgInfo.yres)));
			displayEffDPI->setText(QString("%1 x %2").arg(qRound(72.0 / currItem->imageXScale())).arg(qRound(72.0 / currItem->imageYScale())));
			displaySizePixel->setText(QString("%1 x %2").arg(currItem->OrigW).arg(currItem->OrigH));
			displayScale->setText(QString("%1 x %2 %").arg(currItem->imageXScale() * 100 / 72.0 * currItem->pixm.imgInfo.xres, 5, 'f', 1).arg(currItem->imageYScale() * 100 / 72.0 * currItem->pixm.imgInfo.yres, 5, 'f', 1));
			displayPrintSize->setText(QString("%1 x %2%3").arg(currItem->OrigW * currItem->imageXScale() * m_Doc->unitRatio(), 7, 'f', 2).arg(currItem->OrigH * currItem->imageXScale() * m_Doc->unitRatio(), 7, 'f', 2).arg(unitGetSuffixFromIndex(m_Doc->unitIndex())));
			isPrinting->setChecked(currItem->printEnabled());
			isVisible->setChecked(currItem->imageShown());
			buttonEdit->setEnabled(true);
			effectsButton->setEnabled(true);
			buttonLayers->setEnabled(true);
		}
		else
		{
			if (!currItem->Pfile.isEmpty())
			{
				QFileInfo fi = QFileInfo(currItem->Pfile);
				displayName->setText(fi.fileName());
				displayPath->setText(QDir::convertSeparators(fi.dirPath()));
			}
			else
			{
				displayName->setText( tr("n/a"));
				displayPath->setText( tr("n/a"));
			}
			displayFormat->setText( tr("n/a"));
			displayColorspace->setText( tr("n/a"));
			displayDPI->setText( tr("n/a"));
			displayEffDPI->setText( tr("n/a"));
			displaySizePixel->setText( tr("n/a"));
			displayScale->setText( tr("n/a"));
			displayPrintSize->setText( tr("n/a"));
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
		currItem->setImageShown(isVisible->isChecked());
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
	if (currItem != NULL)
	{
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
	PicSearchOptions *dia = new PicSearchOptions(this, displayName->text(), displayPath->text());
	if (dia->exec())
	{
		if (dia->m_matches.count() == 0)
		{
			QMessageBox::information(this, tr("Scribus - Image Search"), tr("No images named \"%1\" were found.").arg(dia->m_fileName),
					QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
					QMessageBox::NoButton);
		}
		else
		{
			PicSearch *dia2 = new PicSearch(this, dia->m_fileName, dia->m_matches);
			if (dia2->exec())
			{
				Q_ASSERT(!dia2->currentImage.isEmpty());
				loadPict(dia2->currentImage);
				refreshItem(currItem);
				QFileInfo fi = QFileInfo(currItem->Pfile);
				imageViewArea->currentItem()->setText(fi.fileName());
				imageViewArea->currentItem()->setPixmap(createImgIcon(currItem));
				imageSelected(imageViewArea->currentItem());
			}
			delete dia2;
		}
	}
	delete dia;
}

void PicStatus::doImageEffects()
{
	if (currItem != NULL)
	{
		EffectsDialog* dia = new EffectsDialog(this, currItem, m_Doc);
		if (dia->exec())
		{
			currItem->effectsInUse = dia->effectsList;
			loadPict(currItem->Pfile);
			refreshItem(currItem);
			imageViewArea->currentItem()->setPixmap(createImgIcon(currItem));
		}
		delete dia;
	}
}

void PicStatus::doImageExtProp()
{
	if (currItem != NULL)
	{
		ExtImageProps* dia = new ExtImageProps(this, &currItem->pixm.imgInfo, currItem, m_Doc->view());
		dia->exec();
		loadPict(currItem->Pfile);
		refreshItem(currItem);
		imageViewArea->currentItem()->setPixmap(createImgIcon(currItem));
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
