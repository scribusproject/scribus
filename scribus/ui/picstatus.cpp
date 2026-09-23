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

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHash>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QMultiMap>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QProgressDialog>
#include <QScopedPointer>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTimer>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "effectsdialog.h"
#include "embeddedimageextractor.h"
#include "extimageprops.h"
#include "iconmanager.h"
#include "imagealphacontour.h"
#include "imagecmykconversion.h"
#include "imagecmykbatch.h"
#include "imagelinkreplacement.h"
#include "imagelinkmatcher.h"
#include "pageitem.h"
#include "picsearch.h"
#include "picsearchoptions.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "undotransaction.h"
#include "units.h"
#include "util_color.h"
#include "util_formats.h"



PicItem::PicItem(QListWidget* parent, const QString& text, const QPixmap& pix, PageItem* pgItem)
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
	m_lastExtractionDirectory = m_Doc->hasName
		? QFileInfo(m_Doc->documentFileName()).absolutePath() : QDir::homePath();
	setWindowIcon(IconManager::instance().loadIcon("app-icon"));
	fillTable();
	workTab->setCurrentIndex(0);
	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(imageViewArea, SIGNAL(itemSelectionChanged()), this, SLOT(newImageSelected()));
	connect(linkStatusFilter, &QComboBox::currentIndexChanged, this, &PicStatus::applyImageFilters);
	connect(imageFilterText, &QLineEdit::textChanged, this, &PicStatus::applyImageFilters);
	connect(isPrinting, SIGNAL(clicked()), this, SLOT(PrintPic()));
	connect(isVisibleCheck, SIGNAL(clicked()), this, SLOT(visiblePic()));
	connect(goPageButton, SIGNAL(clicked()), this, SLOT(GotoPic()));
	connect(selectButton, SIGNAL(clicked()), this, SLOT(SelectPic()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(SearchPic()));
	connect(relinkFolderButton, SIGNAL(clicked()), this, SLOT(relinkMissingImages()));
	connect(mapFolderButton, &QPushButton::clicked, this, &PicStatus::mapMissingImageFolder);
	connect(extractSelectedButton, &QPushButton::clicked, this, &PicStatus::extractSelectedEmbeddedImage);
	connect(extractAllButton, &QPushButton::clicked, this, &PicStatus::extractAllEmbeddedImages);
	connect(fileManagerButton, SIGNAL(clicked()), this, SLOT(FileManager()));
	connect(effectsButton, SIGNAL(clicked()), this, SLOT(doImageEffects()));
	connect(buttonLayers, SIGNAL(clicked()), this, SLOT(doImageExtProp()));
	connect(buttonEdit, SIGNAL(clicked()), this, SLOT(doEditImage()));
	connect(imageViewArea, &QListWidget::customContextMenuRequested, this, &PicStatus::slotRightClick);
}

QPixmap PicStatus::createImgIcon(PageItem* item)
{
	QPainter p;
	QPixmap pm(128, 128);
	QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill"));
	p.begin(&pm);
	p.fillRect(0, 0, 128, 128, imageViewArea->palette().window());
	p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p.setBrush(palette().window());
	p.drawRoundedRect(0, 0, 127, 127, 10, 10, Qt::RelativeSize);
	p.setPen(Qt::NoPen);
	p.setBrush(b);
	p.drawRect(12, 12, 104, 104);
	if (item->imageIsAvailable && QFile::exists(item->externalFile()))
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

void PicStatus::enableWidgets(bool enabled)
{
	isPrinting->setEnabled(enabled);
	isVisibleCheck->setEnabled(enabled);
	goPageButton->setEnabled(enabled);
	selectButton->setEnabled(enabled);
	searchButton->setEnabled(enabled);
	fileManagerButton->setEnabled(enabled);
	effectsButton->setEnabled(enabled);
	buttonLayers->setEnabled(enabled);
	buttonEdit->setEnabled(enabled);
	extractSelectedButton->setEnabled(enabled && currItem && currItem->isImageInline()
		&& currItem->imageIsAvailable && QFileInfo::exists(currItem->Pfile));
}

void PicStatus::fillTable()
{
	PageItem *item;
	imageViewArea->clear();
	QListWidgetItem *firstItem = nullptr;
	QListWidgetItem *tempItem = nullptr;

	QList<PageItem*> allItems;
	for (int i = 0; i < m_Doc->MasterItems.count(); ++i)
	{
		PageItem *currItem = m_Doc->MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			item = allItems.at(ii);
			if ((item->itemType() != PageItem::ImageFrame) || item->isLatexFrame())
				continue;
			QFileInfo fi(item->Pfile);
			QString Iname;
			if (item->isInlineImage)
				Iname = tr("Embedded Image");
			else if (item->Pfile.isEmpty())
				Iname = tr("Empty Image Frame");
			else
				Iname = fi.fileName();
			tempItem = new PicItem(imageViewArea, Iname, createImgIcon(item), item);
			if (firstItem == nullptr)
				firstItem = tempItem;
		}
		allItems.clear();
	}
	allItems.clear();
	for (int i = 0; i < m_Doc->DocItems.count(); ++i)
	{
		PageItem *currItem = m_Doc->DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			item = allItems.at(ii);
			if ((item->itemType() != PageItem::ImageFrame) || item->isLatexFrame())
				continue;
			QFileInfo fi(item->Pfile);
			QString Iname;
			if (item->isInlineImage)
				Iname = tr("Embedded Image");
			else if (item->Pfile.isEmpty())
				Iname = tr("Empty Image Frame");
			else
				Iname = fi.fileName();
			tempItem = new PicItem(imageViewArea, Iname, createImgIcon(item), item);
			// if an image is selected in a doc, Manage Pictures should
			// display the selected image and its values
			if (firstItem == nullptr || item->isSelected())
				firstItem = tempItem;
		}
		allItems.clear();
	}
	imageViewArea->setCurrentItem(firstItem);
	if (firstItem != nullptr)
		imageSelected(firstItem);

	// Disable all features when there is no image in the document.
	// It should never be used (see ScribusMainWindow::extrasMenuAboutToShow())
	// but who knows if it can be configured for shortcut or macro...
	imageViewArea->setEnabled(imageViewArea->count() > 0);
	workTab->setEnabled(imageViewArea->count() > 0);
	bool hasMissingImages = false;
	for (int i = 0; i < imageViewArea->count(); ++i)
	{
		const auto *imageItem = static_cast<PicItem*>(imageViewArea->item(i));
		const PageItem *pageItem = imageItem->PageItemObject;
		if (!pageItem->imageIsAvailable && !pageItem->isImageInline() && !pageItem->Pfile.isEmpty())
		{
			hasMissingImages = true;
			break;
		}
	}
	relinkFolderButton->setEnabled(hasMissingImages);
	mapFolderButton->setEnabled(hasMissingImages);
	extractAllButton->setEnabled(!embeddedImageItems().isEmpty());
	if (sortOrder == 0)
		sortByName();
	else
		sortByPage();
	if (imageViewArea->count() == 0)
		applyImageFilters();
}

void PicStatus::applyImageFilters()
{
	const QString query = imageFilterText->text().trimmed();
	const int filter = linkStatusFilter->currentIndex();
	QListWidgetItem* selected = imageViewArea->currentItem();
	QListWidgetItem* firstVisible = nullptr;
	int visible = 0;
	{
		// Hiding a selected row must not leave actions targeting an invisible frame.
		const QSignalBlocker blocker(imageViewArea);
		for (int i = 0; i < imageViewArea->count(); ++i)
		{
			auto* row = static_cast<PicItem*>(imageViewArea->item(i));
			const PageItem* item = row->PageItemObject;
			const bool embedded = item->isImageInline();
			const bool empty = !embedded && item->Pfile.isEmpty();
			const bool missing = !embedded && !empty && !item->imageIsAvailable;
			const bool available = !embedded && !empty && item->imageIsAvailable;
			const bool statusMatches = filter == 0 || (filter == 1 && missing)
				|| (filter == 2 && available) || (filter == 3 && embedded) || (filter == 4 && empty);
			const bool textMatches = query.isEmpty() || row->text().contains(query, Qt::CaseInsensitive)
				|| item->Pfile.contains(query, Qt::CaseInsensitive)
				|| QDir::toNativeSeparators(item->Pfile).contains(query, Qt::CaseInsensitive)
				|| item->itemName().contains(query, Qt::CaseInsensitive);
			const bool show = statusMatches && textMatches;
			row->setHidden(!show);
			if (!show && selected == row)
				selected = nullptr;
			if (show)
			{
				++visible;
				if (!firstVisible)
					firstVisible = row;
			}
		}
		if (!selected)
			selected = firstVisible;
		imageViewArea->clearSelection();
		imageViewArea->setCurrentItem(selected);
		if (selected)
			selected->setSelected(true);
	}
	imageCountLabel->setText(visible == 0 ? tr("No images match these filters (%1 total).")
		.arg(imageViewArea->count()) : tr("%1 of %2 images").arg(visible).arg(imageViewArea->count()));
	imageViewArea->setEnabled(visible > 0);
	workTab->setEnabled(selected != nullptr);
	imageSelected(selected);
}

void PicStatus::sortByName()
{
	QMultiMap<QString, PicItem*> sorted;

	int num = imageViewArea->count();
	if (num == 0)
		return;

	auto firstItem = imageViewArea->currentItem();
	for (int i = num - 1; i > -1; --i)
	{
		QListWidgetItem *ite = imageViewArea->takeItem(i);
		PicItem *item = (PicItem*) ite;
		QFileInfo fi(item->PageItemObject->Pfile);
		sorted.insert(fi.fileName(), item);
	}

	int counter = 0;
	foreach (const QString& i, sorted.uniqueKeys())
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
	applyImageFilters();
}

void PicStatus::sortByPage()
{
	QMultiMap<int, PicItem*> sorted;

	int num = imageViewArea->count();
	if (num == 0)
		return;

	auto firstItem = imageViewArea->currentItem();
	for (int a = num-1; a > -1; --a)
	{
		QListWidgetItem *ite = imageViewArea->takeItem(a);
		PicItem *item = (PicItem*)ite;
		sorted.insert(item->PageItemObject->OwnPage, item);
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
	applyImageFilters();
}

void PicStatus::slotRightClick(const QPoint& position)
{
	if (QListWidgetItem* clicked = imageViewArea->itemAt(position))
		imageViewArea->setCurrentItem(clicked);
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
	pmen->addSeparator();
	QAction* replaceAll = pmen->addAction(tr("Replace All Uses of This Image..."));
	replaceAll->setEnabled(currItem && !currItem->isImageInline() && !currItem->Pfile.isEmpty());
	connect(replaceAll, &QAction::triggered, this, &PicStatus::replaceSelectedImageEverywhere);
	QAction* alphaContour = pmen->addAction(tr("Generate Image Contour..."));
	alphaContour->setEnabled(currItem && currItem->imageIsAvailable && currItem->isRaster);
	connect(alphaContour, &QAction::triggered, this, &PicStatus::generateSelectedImageContour);
	QAction* exportCMYK = pmen->addAction(tr("Export CMYK TIFF Copy..."));
	exportCMYK->setEnabled(currItem && currItem->imageIsAvailable && currItem->isRaster
		&& currItem->pixm.imgInfo.colorspace == ColorSpaceRGB && m_Doc->HasCMS
		&& m_Doc->DocPrinterProf && m_Doc->DocPrinterProf.colorSpace() == ColorSpace_Cmyk);
	connect(exportCMYK, &QAction::triggered, this, &PicStatus::exportSelectedCMYKCopy);
	QAction* batchCMYK = pmen->addAction(tr("Batch Export RGB Images to CMYK..."));
	batchCMYK->setEnabled(m_Doc->HasCMS && m_Doc->DocPrinterProf
		&& m_Doc->DocPrinterProf.colorSpace() == ColorSpace_Cmyk);
	connect(batchCMYK, &QAction::triggered, this, &PicStatus::batchExportCMYKCopies);
	pmen->exec(QCursor::pos());
	delete pmen;
}

void PicStatus::batchExportCMYKCopies()
{
	const QString directory = QFileDialog::getExistingDirectory(this, tr("Choose CMYK Output Folder"),
		m_lastExtractionDirectory);
	if (directory.isEmpty())
		return;
	QDialog dialog(this);
	dialog.setWindowTitle(tr("Batch Export RGB Images to CMYK"));
	auto* layout = new QVBoxLayout(&dialog);
	auto* form = new QFormLayout();
	auto* inputProfile = new QComboBox(&dialog);
	inputProfile->addItem(tr("Each image's existing profile"), QString());
	for (const QString& name : ScCore->InputProfiles.keys())
		inputProfile->addItem(name, name);
	form->addRow(tr("RGB source profile:"), inputProfile);
	auto* outputProfile = new QComboBox(&dialog);
	outputProfile->addItem(tr("Document CMYK output profile"), QString());
	for (const QString& name : ScCore->PrinterProfiles.keys())
		outputProfile->addItem(name, name);
	form->addRow(tr("CMYK output profile:"), outputProfile);
	auto* intent = new QComboBox(&dialog);
	intent->addItem(tr("Document/image default"), -1);
	intent->addItem(tr("Perceptual"), static_cast<int>(Intent_Perceptual));
	intent->addItem(tr("Relative colorimetric"), static_cast<int>(Intent_Relative_Colorimetric));
	intent->addItem(tr("Saturation"), static_cast<int>(Intent_Saturation));
	intent->addItem(tr("Absolute colorimetric"), static_cast<int>(Intent_Absolute_Colorimetric));
	form->addRow(tr("Rendering intent:"), intent);
	auto* blackPoint = new QComboBox(&dialog);
	blackPoint->addItem(tr("Document default"), -1);
	blackPoint->addItem(tr("Enabled"), 1);
	blackPoint->addItem(tr("Disabled"), 0);
	form->addRow(tr("Black-point compensation:"), blackPoint);
	layout->addLayout(form);
	auto* backup = new QCheckBox(tr("Copy original images into an originals folder"), &dialog);
	backup->setChecked(true);
	layout->addWidget(backup);
	auto* relink = new QCheckBox(tr("Relink eligible frames to the exported TIFFs (undoable)"), &dialog);
	layout->addWidget(relink);
	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
	buttons->button(QDialogButtonBox::Ok)->setText(tr("Preview and Export"));
	layout->addWidget(buttons);
	connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
	if (dialog.exec() != QDialog::Accepted)
		return;
	ImageCMYKBatchOptions options;
	options.color.sourceProfileName = inputProfile->currentData().toString();
	options.color.destinationProfileName = outputProfile->currentData().toString();
	if (intent->currentData().toInt() >= 0)
		options.color.renderingIntent = static_cast<eRenderIntent>(intent->currentData().toInt());
	if (blackPoint->currentData().toInt() >= 0)
		options.color.blackPointCompensation = blackPoint->currentData().toBool();
	options.copyOriginals = backup->isChecked();
	options.relink = relink->isChecked();
	options.dryRun = true;
	const ImageCMYKBatchResult preview = runImageCMYKBatch(m_Doc, directory, options);
	if (!preview.error.isEmpty())
	{
		ScMessageBox::warning(this, tr("Batch CMYK Export"), preview.error);
		return;
	}
	if (preview.ready == 0)
	{
		ScMessageBox::information(this, tr("Batch CMYK Export"), tr("No eligible RGB raster image frames were found."));
		return;
	}
	if (QMessageBox::question(this, tr("Batch CMYK Export"),
		tr("%1 image frame(s) appear eligible. %2 frame(s) will be skipped.\n\n"
		   "Files will be written to %3. Originals will not be overwritten. Continue?")
			.arg(preview.ready).arg(preview.entries.size() - preview.ready)
			.arg(QDir::toNativeSeparators(directory)),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
		return;
	options.dryRun = false;
	const ImageCMYKBatchResult result = runImageCMYKBatch(m_Doc, directory, options);
	fillTable();
	ScMessageBox::information(this, tr("Batch CMYK Export"),
		tr("Exported: %1\nRelinked: %2\nFailed: %3\nReport: %4\n\n%5")
			.arg(result.exported).arg(result.relinked).arg(result.failed)
			.arg(QDir::toNativeSeparators(result.reportPath), result.error));
}

void PicStatus::replaceSelectedImageEverywhere()
{
	if (!currItem || currItem->isImageInline() || currItem->Pfile.isEmpty())
		return;
	const QString source = currItem->Pfile;
	const QString replacement = QFileDialog::getOpenFileName(this, tr("Choose Replacement Image"),
		QFileInfo(source).absolutePath(), tr("All Files (*)"));
	if (replacement.isEmpty())
		return;
	bool scopeAccepted = false;
	const QStringList scopes { tr("Entire document and master pages"),
		tr("Current document page only"), tr("Master pages only") };
	const QString chosenScope = QInputDialog::getItem(this, tr("Replacement Scope"),
		tr("Replace links on:"), scopes, 0, false, &scopeAccepted);
	if (!scopeAccepted)
		return;
	const ImageLinkReplacementScope scope = chosenScope == scopes.at(1)
		? ImageLinkReplacementScope::CurrentPage : chosenScope == scopes.at(2)
			? ImageLinkReplacementScope::MasterPages : ImageLinkReplacementScope::EntireDocument;
	const ImageLinkReplacementResult preview = replaceImageLinks(m_Doc, source, replacement, scope, true);
	if (preview.matched == 0 || QDir::cleanPath(QFileInfo(source).absoluteFilePath())
		== QDir::cleanPath(QFileInfo(replacement).absoluteFilePath()))
		return;
	QString previewDetails;
	for (const ImageLinkReplacementEntry& entry : preview.entries)
		previewDetails += tr("Page %1: %2\n").arg(entry.page, entry.frame);
	QMessageBox confirmation(this);
	confirmation.setWindowTitle(tr("Replace Image Links"));
	confirmation.setText(
		tr("Replace %n external frame(s) linked to:\n%1\n\nWith:\n%2\n\n"
		   "Embedded images are excluded. "
		   "The changes can be undone together.", nullptr, preview.matched)
			.arg(QDir::toNativeSeparators(source), QDir::toNativeSeparators(replacement)));
	confirmation.setDetailedText(previewDetails);
	confirmation.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	confirmation.setDefaultButton(QMessageBox::No);
	if (confirmation.exec() != QMessageBox::Yes)
		return;
	const ImageLinkReplacementResult result = replaceImageLinks(m_Doc, source, replacement, scope, false);
	fillTable();
	QString reportMessage;
	if (QMessageBox::question(this, tr("Replacement Report"),
		tr("Save a JSON report of the frames and replacement results?"),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
	{
		const QString reportPath = QFileDialog::getSaveFileName(this, tr("Save Replacement Report"),
			QFileInfo(source).absolutePath() + QDir::separator() + QStringLiteral("image-replacement-report.json"),
			tr("JSON Report (*.json)"));
		if (!reportPath.isEmpty() && !writeImageLinkReplacementReport(reportPath, source, replacement, result))
			reportMessage = tr("\nThe report could not be saved.");
	}
	ScMessageBox::information(this, tr("Replace All Uses of This Image"),
		tr("Matched: %1\nReplaced: %2\nCould not load: %3\n\nOriginal image files were not changed.%4")
			.arg(result.matched).arg(result.replaced).arg(result.failed).arg(reportMessage));
}

void PicStatus::generateSelectedImageContour()
{
	if (!currItem || !currItem->imageIsAvailable || !currItem->isRaster)
		return;
	QDialog options(this);
	options.setWindowTitle(tr("Generate Image Contour"));
	auto* layout = new QVBoxLayout(&options);
	auto* form = new QFormLayout();
	auto* source = new QComboBox(&options);
	source->addItem(tr("Transparency (alpha)"), static_cast<int>(ImageContourSource::Alpha));
	source->addItem(tr("Embedded clipping path"), static_cast<int>(ImageContourSource::ImageClippingPath));
	source->addItem(tr("Dark areas (luminance)"), static_cast<int>(ImageContourSource::Luminance));
	source->addItem(tr("Contrast against corners"), static_cast<int>(ImageContourSource::ContrastEdge));
	form->addRow(tr("Source:"), source);
	auto* threshold = new QSpinBox(&options);
	threshold->setRange(1, 255);
	threshold->setValue(128);
	threshold->setToolTip(tr("Opacity, darkness or background-contrast threshold, depending on source."));
	form->addRow(tr("Threshold:"), threshold);
	auto* smoothing = new QSpinBox(&options);
	smoothing->setRange(0, 50);
	smoothing->setSuffix(tr(" pt"));
	form->addRow(tr("Corner smoothing:"), smoothing);
	auto* simplification = new QDoubleSpinBox(&options);
	simplification->setRange(0, 8);
	simplification->setDecimals(1);
	simplification->setToolTip(tr("Reduce source detail before tracing; 0 keeps the most detail."));
	form->addRow(tr("Simplification:"), simplification);
	auto* padding = new QDoubleSpinBox(&options);
	padding->setRange(0, 1000);
	padding->setDecimals(1);
	padding->setSuffix(tr(" pt"));
	padding->setToolTip(tr("Extra distance between the visible image and surrounding text."));
	form->addRow(tr("Text clearance:"), padding);
	layout->addLayout(form);
	auto* preview = new QLabel(&options);
	preview->setFixedSize(260, 205);
	preview->setAlignment(Qt::AlignCenter);
	layout->addWidget(preview, 0, Qt::AlignHCenter);
	auto* wrap = new QCheckBox(tr("Enable text flow around the generated contour"), &options);
	wrap->setChecked(true);
	layout->addWidget(wrap);
	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &options);
	buttons->button(QDialogButtonBox::Ok)->setText(tr("Generate"));
	auto currentOptions = [&]() {
		ImageContourOptions value;
		value.source = static_cast<ImageContourSource>(source->currentData().toInt());
		value.threshold = threshold->value();
		value.smoothing = smoothing->value();
		value.simplification = simplification->value();
		value.padding = padding->value();
		value.enableWrap = wrap->isChecked();
		return value;
	};
	auto refreshPreview = [&]() {
		QPainterPath contour;
		QString previewError;
		const bool valid = buildImageContour(currItem, currentOptions(), &contour, &previewError);
		buttons->button(QDialogButtonBox::Ok)->setEnabled(valid);
		if (!valid)
		{
			preview->setPixmap(QPixmap());
			preview->setText(previewError);
			return;
		}
		QPixmap picture(preview->size());
		picture.fill(preview->palette().window().color());
		QPainter painter(&picture);
		painter.setRenderHint(QPainter::Antialiasing);
		const double scale = qMin(240.0 / currItem->width(), 180.0 / currItem->height());
		painter.translate((picture.width() - currItem->width() * scale) / 2,
			(picture.height() - currItem->height() * scale) / 2);
		painter.scale(scale, scale);
		painter.setPen(QPen(Qt::gray, 1.0 / scale));
		painter.drawRect(QRectF(0, 0, currItem->width(), currItem->height()));
		painter.setPen(QPen(QColor(0, 130, 160), 2.0 / scale));
		painter.setBrush(QColor(0, 130, 160, 45));
		painter.drawPath(contour);
		preview->setPixmap(picture);
	};
	connect(source, &QComboBox::currentIndexChanged, &options, refreshPreview);
	connect(threshold, &QSpinBox::valueChanged, &options, refreshPreview);
	connect(smoothing, &QSpinBox::valueChanged, &options, refreshPreview);
	connect(simplification, &QDoubleSpinBox::valueChanged, &options, refreshPreview);
	connect(padding, &QDoubleSpinBox::valueChanged, &options, refreshPreview);
	refreshPreview();
	connect(buttons, &QDialogButtonBox::accepted, &options, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &options, &QDialog::reject);
	layout->addWidget(buttons);
	if (options.exec() != QDialog::Accepted)
		return;
	QString error;
	if (!generateImageContour(currItem, currentOptions(), &error))
		ScMessageBox::warning(this, tr("Generate Image Contour"), error);
	else
		fillTable();
}

void PicStatus::exportSelectedCMYKCopy()
{
	if (!currItem || !currItem->imageIsAvailable || !currItem->isRaster
		|| currItem->pixm.imgInfo.colorspace != ColorSpaceRGB || !m_Doc->HasCMS
		|| !m_Doc->DocPrinterProf || m_Doc->DocPrinterProf.colorSpace() != ColorSpace_Cmyk)
		return;

	const QFileInfo sourceInfo(currItem->Pfile);
	const QString suggestedPath = sourceInfo.absolutePath() + QDir::separator()
		+ sourceInfo.completeBaseName() + QStringLiteral("-CMYK.tif");
	const QString destination = QFileDialog::getSaveFileName(this, tr("Export CMYK TIFF Copy"),
		suggestedPath, tr("TIFF Image (*.tif *.tiff)"));
	if (destination.isEmpty())
		return;
	QString error;
	if (!exportImageAsCMYKCopy(currItem, destination, &error))
	{
		ScMessageBox::warning(this, tr("Export CMYK TIFF Copy"), error);
		return;
	}
	if (!currItem->isImageInline()
		&& QMessageBox::question(this, tr("Export CMYK TIFF Copy"),
			tr("Relink this frame to the new CMYK TIFF? The original file is kept, and the relink can be undone."),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
	{
		if (!loadPict(currItem, destination, true, true))
			ScMessageBox::warning(this, tr("Export CMYK TIFF Copy"),
				tr("The CMYK TIFF was saved, but the frame could not be relinked. Its original image remains in place."));
		fillTable();
		return;
	}
	ScMessageBox::information(this, tr("Export CMYK TIFF Copy"),
		tr("CMYK TIFF saved. The original image and this frame's link were not changed."));
}

void PicStatus::newImageSelected()
{
	QList<QListWidgetItem*> items = imageViewArea->selectedItems();
	imageSelected((items.count() > 0) ? items.at(0) : nullptr);
}

void PicStatus::imageSelected(QListWidgetItem *ite)
{
	if (ite == nullptr)
	{
		currItem = nullptr;
		enableWidgets(false);
		isPrinting->setChecked(false);
		isVisibleCheck->setChecked(false);
		for (QLabel* label : { displayName, displayPath, displayFormat, displayColorspace,
			displayDPI, displayEffDPI, displaySizePixel, displayScale, displayPrintSize,
			displayPage, displayObjekt })
			label->clear();
		return;
	}

	enableWidgets(true);

	PicItem *item = (PicItem*) ite;
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
	if (currItem->imageIsAvailable)
	{
		QFileInfo fi(currItem->Pfile);
		QString ext = fi.suffix().toLower();
		if (currItem->isInlineImage)
		{
			displayName->setText( tr("Embedded Image"));
			displayPath->setText("");
			searchButton->setEnabled(false);
			fileManagerButton->setEnabled(false);
		}
		else
		{
			displayName->setText(fi.fileName());
			displayPath->setText(QDir::toNativeSeparators(fi.path()));
			searchButton->setEnabled(true);
			fileManagerButton->setEnabled(true);
		}
		QString format;
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
			cSpace = colorSpaceText(currItem->pixm.imgInfo.colorspace);
		displayColorspace->setText(cSpace);
		displayDPI->setText(QString("%1 x %2").arg(currItem->pixm.imgInfo.xres).arg(currItem->pixm.imgInfo.yres));
		displayEffDPI->setText(QString("%1 x %2").arg(qRound(72.0 / currItem->imageXScale())).arg(qRound(72.0 / currItem->imageYScale())));
		displaySizePixel->setText(QString("%1 x %2").arg(currItem->OrigW).arg(currItem->OrigH));
		displayScale->setText(QString("%1 x %2 %").arg(currItem->imageXScale() * 100 / 72.0 * currItem->pixm.imgInfo.xres, 5, 'f', 1).arg(currItem->imageYScale() * 100 / 72.0 * currItem->pixm.imgInfo.yres, 5, 'f', 1));
		displayPrintSize->setText(QString("%1 x %2%3").arg(currItem->OrigW * currItem->imageXScale() * m_Doc->unitRatio(), 7, 'f', 2).arg(currItem->OrigH * currItem->imageXScale() * m_Doc->unitRatio(), 7, 'f', 2).arg(unitGetSuffixFromIndex(m_Doc->unitIndex())));
		isPrinting->setChecked(currItem->printEnabled());
		isVisibleCheck->setChecked(currItem->imageVisible());
		buttonEdit->setEnabled(currItem->isRaster);
		effectsButton->setEnabled(currItem->isRaster);
		buttonLayers->setEnabled(currItem->pixm.imgInfo.valid);
	}
	else
	{
		QString trNA = tr("n/a");
		if (!currItem->Pfile.isEmpty())
		{
			QFileInfo fi(currItem->Pfile);
			displayName->setText(fi.fileName());
			displayPath->setText(QDir::toNativeSeparators(fi.path()));
			searchButton->setEnabled(true);
			fileManagerButton->setEnabled(true);
		}
		else
		{
			displayName->setText(trNA);
			displayPath->setText(trNA);
			searchButton->setEnabled(false);
			fileManagerButton->setEnabled(false);
		}
		displayFormat->setText(trNA);
		displayColorspace->setText(trNA);
		displayDPI->setText(trNA);
		displayEffDPI->setText(trNA);
		displaySizePixel->setText(trNA);
		displayScale->setText(trNA);
		displayPrintSize->setText(trNA);
		isPrinting->setChecked(currItem->printEnabled());
		isVisibleCheck->setChecked(currItem->imageVisible());
		buttonEdit->setEnabled(false);
		effectsButton->setEnabled(false);
		buttonLayers->setEnabled(false);
	}
}

void PicStatus::PrintPic()
{
	if (currItem != nullptr)
		currItem->setPrintEnabled(isPrinting->isChecked());
}

void PicStatus::visiblePic()
{
	if (currItem == nullptr)
		return;
	currItem->setImageVisible(isVisibleCheck->isChecked());
}

void PicStatus::GotoPic()
{
	if (currItem == nullptr)
		return;

	if (currItem->OnMasterPage.isEmpty() && m_Doc->masterPageMode())
		ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
	if (!currItem->OnMasterPage.isEmpty())
		emit selectMasterPage(currItem->OnMasterPage);
	else
		emit selectPage(currItem->OwnPage);

	emit selectElementByItem(currItem, true, 1);
}

void PicStatus::SelectPic()
{
	if (currItem == nullptr)
		return;

	if (currItem->OnMasterPage.isEmpty() && m_Doc->masterPageMode())
		ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
	else
		if (!currItem->OnMasterPage.isEmpty() && !m_Doc->masterPageMode())
			emit selectMasterPage(currItem->OnMasterPage);

	emit selectElementByItem(currItem, true, 1);
}

bool PicStatus::loadPict(PageItem* item, const QString & newFilePath, bool showMsg,
	bool useNewEmbeddedProfile)
{
	bool masterPageMode = !item->OnMasterPage.isEmpty();
	bool oldMasterPageMode = m_Doc->masterPageMode();
	if (masterPageMode != oldMasterPageMode)
		m_Doc->setMasterPageMode(masterPageMode);
	const bool loaded = item->relinkImage(newFilePath, showMsg, useNewEmbeddedProfile);
	if (masterPageMode != oldMasterPageMode)
		m_Doc->setMasterPageMode(oldMasterPageMode);
	return loaded;
}

void PicStatus::relinkMissingImages()
{
	relinkMissingImagesFromFolder(false);
}

void PicStatus::mapMissingImageFolder()
{
	relinkMissingImagesFromFolder(true);
}

QList<PageItem*> PicStatus::embeddedImageItems() const
{
	QList<PageItem*> items;
	for (int i = 0; i < imageViewArea->count(); ++i)
	{
		const auto *imageItem = static_cast<PicItem*>(imageViewArea->item(i));
		PageItem *pageItem = imageItem->PageItemObject;
		if (pageItem && pageItem->isImageInline() && pageItem->imageIsAvailable
			&& QFileInfo::exists(pageItem->Pfile))
			items.append(pageItem);
	}
	return items;
}

void PicStatus::extractSelectedEmbeddedImage()
{
	if (!currItem || !currItem->isImageInline() || !currItem->imageIsAvailable
		|| !QFileInfo::exists(currItem->Pfile))
		return;

	const QString defaultName = suggestedEmbeddedImageFileName(currItem->itemName(), currItem->Pfile, 1);
	const QString destination = QFileDialog::getSaveFileName(this, tr("Extract Embedded Image"),
		QDir(m_lastExtractionDirectory).filePath(defaultName), tr("All Files (*)"));
	if (destination.isEmpty())
		return;
	m_lastExtractionDirectory = QFileInfo(destination).absolutePath();

	QString error;
	if (!copyEmbeddedImageBytes(currItem->Pfile, destination, true, &error))
	{
		ScMessageBox::warning(this, tr("Extract Embedded Image"), error);
		return;
	}

	const auto relink = QMessageBox::question(this, tr("Extract Embedded Image"),
		tr("The embedded image was extracted successfully.\n\n"
		   "Relink this frame to the extracted file? The relinking can be undone without deleting the extracted file."),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (relink == QMessageBox::Yes && !currItem->relinkExtractedImage(destination))
	{
		ScMessageBox::warning(this, tr("Extract Embedded Image"),
			tr("The image was extracted, but the frame could not be relinked. It remains embedded."));
		return;
	}
	fillTable();
}

void PicStatus::extractAllEmbeddedImages()
{
	const QList<PageItem*> images = embeddedImageItems();
	if (images.isEmpty())
		return;

	const QString directory = QFileDialog::getExistingDirectory(this, tr("Extract All Embedded Images"),
		m_lastExtractionDirectory);
	if (directory.isEmpty())
		return;
	m_lastExtractionDirectory = directory;

	QDialog options(this);
	options.setWindowTitle(tr("Extract All Embedded Images"));
	auto *layout = new QVBoxLayout(&options);
	auto *description = new QLabel(tr("Extract %n embedded image(s) to:\n%1", nullptr, images.size())
		.arg(QDir::toNativeSeparators(directory)), &options);
	description->setWordWrap(true);
	layout->addWidget(description);
	auto *conflictLabel = new QLabel(tr("If a filename already exists:"), &options);
	layout->addWidget(conflictLabel);
	auto *conflictBox = new QComboBox(&options);
	conflictBox->setAccessibleName(tr("Existing file handling"));
	conflictBox->addItem(tr("Keep both files (add a number)"), static_cast<int>(ImageExtractionConflict::KeepBoth));
	conflictBox->addItem(tr("Skip the embedded image"), static_cast<int>(ImageExtractionConflict::Skip));
	conflictBox->addItem(tr("Replace the existing file"), static_cast<int>(ImageExtractionConflict::Replace));
	layout->addWidget(conflictBox);
	auto *relinkBox = new QCheckBox(tr("Relink frames to the extracted files"), &options);
	relinkBox->setToolTip(tr("Relinking is grouped into one undo step. Undo does not delete extracted files."));
	layout->addWidget(relinkBox);
	auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &options);
	buttons->button(QDialogButtonBox::Ok)->setText(tr("Extract"));
	connect(buttons, &QDialogButtonBox::accepted, &options, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &options, &QDialog::reject);
	layout->addWidget(buttons);
	if (options.exec() != QDialog::Accepted)
		return;

	const auto conflict = static_cast<ImageExtractionConflict>(conflictBox->currentData().toInt());
	const bool relinkFrames = relinkBox->isChecked();
	QSet<QString> reservedPaths;
	UndoTransaction transaction;
	if (relinkFrames && UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction(Um::SelectionGroup, Um::IGroup,
			tr("Relink extracted images"), QString(), Um::IGetImage);

	int extracted = 0;
	int relinked = 0;
	int relinkFailed = 0;
	int renamed = 0;
	int skipped = 0;
	int failed = 0;
	for (int i = 0; i < images.size(); ++i)
	{
		PageItem *pageItem = images.at(i);
		const QString fileName = suggestedEmbeddedImageFileName(pageItem->itemName(), pageItem->Pfile, i + 1);
		const ImageExtractionPath output = resolveImageExtractionPath(directory, fileName, conflict, &reservedPaths);
		if (output.skipped)
		{
			++skipped;
			continue;
		}
		QString error;
		if (!copyEmbeddedImageBytes(pageItem->Pfile, output.path,
			conflict == ImageExtractionConflict::Replace, &error))
		{
			++failed;
			continue;
		}
		++extracted;
		if (output.renamed)
			++renamed;
		if (relinkFrames)
		{
			if (pageItem->relinkExtractedImage(output.path, false))
				++relinked;
			else
				++relinkFailed;
		}
	}
	if (transaction)
	{
		if (relinked > 0)
			transaction.commit();
		else
			transaction.cancel();
	}

	fillTable();
	ScMessageBox::information(this, tr("Extract All Embedded Images"),
		tr("Extracted: %1\nRelinked: %2\nRenamed to avoid conflicts: %3\nSkipped: %4\nFailed to extract: %5\nFailed to relink: %6\n\n"
		   "Undoing relinking will not delete extracted files.")
			.arg(extracted).arg(relinked).arg(renamed).arg(skipped).arg(failed).arg(relinkFailed));
}

void PicStatus::relinkMissingImagesFromFolder(bool mapFolder)
{
	QStringList missingPaths;
	for (int i = 0; i < imageViewArea->count(); ++i)
	{
		const auto *imageItem = static_cast<PicItem*>(imageViewArea->item(i));
		const PageItem *pageItem = imageItem->PageItemObject;
		if (!pageItem->imageIsAvailable && !pageItem->isImageInline() && !pageItem->Pfile.isEmpty())
			missingPaths.append(pageItem->Pfile);
	}
	missingPaths.removeDuplicates();
	if (missingPaths.isEmpty())
		return;

	QString sourceDirectory;
	if (mapFolder)
	{
		const QStringList sourceFolders = imageLinkSourceFolders(missingPaths);
		bool accepted = false;
		sourceDirectory = QInputDialog::getItem(this, tr("Map Moved Image Folder"),
			tr("Original image folder (it does not need to exist):\n"
			   "Choose or enter the old root folder. Subfolder paths will be preserved."),
			sourceFolders, 0, true, &accepted);
		if (!accepted || sourceDirectory.isEmpty())
			return;
		for (qsizetype i = missingPaths.size(); i > 0; --i)
		{
			if (imageLinkRelativePath(missingPaths.at(i - 1), sourceDirectory).isEmpty())
				missingPaths.removeAt(i - 1);
		}
		if (missingPaths.isEmpty())
		{
			ScMessageBox::information(this, tr("Map Moved Image Folder"),
				tr("No missing image links belong to this folder. Choose an original folder listed in the document."));
			return;
		}
	}

	static QString lastRelinkDirectory;
	if (lastRelinkDirectory.isEmpty())
		lastRelinkDirectory = m_Doc->hasName ? QFileInfo(m_Doc->documentFileName()).absolutePath() : QDir::homePath();
	const QString directory = QFileDialog::getExistingDirectory(this,
		mapFolder ? tr("Choose New Location of Image Folder") : tr("Find Missing Images in Folder"), lastRelinkDirectory);
	if (directory.isEmpty())
		return;
	lastRelinkDirectory = directory;

	ImageLinkSearchTask search(this, missingPaths, directory, true, sourceDirectory);
	QProgressDialog progress(tr("Scanning folders for missing images..."), tr("Cancel"), 0, 0, this);
	progress.setWindowTitle(tr("Relink Missing Images"));
	progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumDuration(0);
	progress.setAutoClose(false);
	progress.setAutoReset(false);
	connect(&progress, &QProgressDialog::canceled, &search, &DeferredTask::cancel);
	connect(&search, &DeferredTask::finished, &progress, &QProgressDialog::accept);
	connect(&search, &DeferredTask::aborted, &progress,
		[&progress](bool) { progress.reject(); });
	QTimer progressUpdate;
	connect(&progressUpdate, &QTimer::timeout, &progress, [&search, &progress]() {
		progress.setLabelText(PicStatus::tr("Scanning folders for missing images...\n%1 files checked")
			.arg(search.scannedFileCount()));
	});
	progressUpdate.start(100);
	search.start();
	progress.exec();
	progressUpdate.stop();
	if (!search.isFinished())
		return;

	const auto matches = search.matches();
	QHash<QString, QStringList> candidatesByPath;
	int ambiguous = 0;
	int ambiguousResolved = 0;
	int notFound = 0;
	for (const ImageLinkMatch& match : matches)
	{
		candidatesByPath.insert(match.linkPath, match.candidatePaths);
		if (match.isAmbiguous())
		{
			++ambiguous;
			PicSearch resolver(this, QDir::toNativeSeparators(match.linkPath),
				match.candidatePaths, true, true);
			if (resolver.exec() == QDialog::Accepted)
			{
				candidatesByPath.insert(match.linkPath, { resolver.getSelectedImage() });
				++ambiguousResolved;
			}
		}
		else if (match.candidatePaths.isEmpty())
			++notFound;
	}

	if (mapFolder)
	{
		QDialog review(this);
		review.setObjectName(QStringLiteral("imageFolderMappingReview"));
		review.setWindowTitle(tr("Review Image Folder Mapping"));
		review.resize(850, 420);
		auto* layout = new QVBoxLayout(&review);
		auto* description = new QLabel(tr("From: %1\nTo: %2\n\n"
			"Only missing images with a matching subfolder path will be relinked. "
			"The changes can be undone together.")
			.arg(QDir::toNativeSeparators(sourceDirectory), QDir::toNativeSeparators(directory)), &review);
		description->setTextFormat(Qt::PlainText);
		description->setWordWrap(true);
		layout->addWidget(description);
		auto* paths = new QTreeWidget(&review);
		paths->setHeaderLabels({ tr("Original Link"), tr("Replacement"), tr("Status") });
		paths->setRootIsDecorated(false);
		paths->setTextElideMode(Qt::ElideMiddle);
		paths->header()->setStretchLastSection(false);
		paths->header()->setSectionResizeMode(0, QHeaderView::Stretch);
		paths->header()->setSectionResizeMode(1, QHeaderView::Stretch);
		paths->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
		int ready = 0;
		for (const ImageLinkMatch& match : matches)
		{
			const QStringList candidates = candidatesByPath.value(match.linkPath);
			const bool selected = candidates.size() == 1;
			if (selected)
				++ready;
			auto* row = new QTreeWidgetItem(paths, { QDir::toNativeSeparators(match.linkPath),
				selected ? QDir::toNativeSeparators(candidates.first()) : QString(),
				selected ? tr("Ready") : candidates.isEmpty() ? tr("Not found") : tr("Skipped") });
			row->setToolTip(0, row->text(0));
			row->setToolTip(1, row->text(1));
		}
		layout->addWidget(paths);
		auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &review);
		buttons->button(QDialogButtonBox::Ok)->setText(tr("Relink %n Image Link(s)", nullptr, ready));
		buttons->button(QDialogButtonBox::Ok)->setEnabled(ready > 0);
		connect(buttons, &QDialogButtonBox::accepted, &review, &QDialog::accept);
		connect(buttons, &QDialogButtonBox::rejected, &review, &QDialog::reject);
		layout->addWidget(buttons);
		if (review.exec() != QDialog::Accepted)
			return;
	}

	UndoTransaction transaction;
	if (UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction(Um::SelectionGroup, Um::IGroup,
			tr("Relink missing images"), QString(), Um::IGetImage);
	int relinked = 0;
	int failed = 0;
	for (int i = 0; i < imageViewArea->count(); ++i)
	{
		auto *imageItem = static_cast<PicItem*>(imageViewArea->item(i));
		PageItem *pageItem = imageItem->PageItemObject;
		if (pageItem->imageIsAvailable || pageItem->isImageInline() || pageItem->Pfile.isEmpty())
			continue;
		const QString linkPath = QDir::cleanPath(QFileInfo(pageItem->Pfile).absoluteFilePath());
		const QStringList candidates = candidatesByPath.value(linkPath);
		if (candidates.size() != 1)
			continue;
		if (loadPict(pageItem, candidates.first(), false))
		{
			++relinked;
			imageItem->setText(QFileInfo(pageItem->Pfile).fileName());
			imageItem->setIcon(createImgIcon(pageItem));
		}
		else
			++failed;
	}
	if (transaction)
	{
		if (relinked > 0)
			transaction.commit();
		else
			transaction.cancel();
	}

	fillTable();
	ScMessageBox::information(this, tr("Relink Missing Images"),
		tr("Relinked: %1\nAmbiguous matches resolved: %2\nAmbiguous matches skipped: %3\nNot found: %4\nCould not load: %5")
			.arg(relinked).arg(ambiguousResolved).arg(ambiguous - ambiguousResolved).arg(notFound).arg(failed),
		QMessageBox::Ok | QMessageBox::Default | QMessageBox::Escape,
		QMessageBox::NoButton);
}

void PicStatus::SearchPic()
{
	// no action where is no item selected. It should never happen.
	if (currItem == nullptr)
		return;
	static QString lastSearchPath;

	if (lastSearchPath.isEmpty())
		lastSearchPath = displayPath->text();

	QScopedPointer<PicSearchOptions> dia(new PicSearchOptions(this, displayName->text(), lastSearchPath));
	if (dia->exec() != QDialog::Accepted)
		return;

	lastSearchPath = dia->getLastDirSearched();
	if (dia->getMatches().count() == 0)
	{
		ScMessageBox::information(this, tr("Scribus - Image Search"), tr("No images named \"%1\" were found.").arg(dia->getFileName()),
				QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
				QMessageBox::NoButton);
		return;
	}

	auto item = static_cast<PicItem*>(imageViewArea->currentItem());
	bool brokenLink = !(item->PageItemObject->imageIsAvailable);

	QScopedPointer<PicSearch> dia2(new PicSearch(this, dia->getFileName(), dia->getMatches(), brokenLink));
	if (dia2->exec() != QDialog::Accepted)
		return;

	QFileInfo source(currItem->Pfile);
	UndoTransaction transaction;
	if (dia2->isApplyToMatchingImages() && UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction(Um::SelectionGroup, Um::IGroup,
			tr("Relink images"), QString(), Um::IGetImage);

	if (!loadPict(currItem, dia2->getSelectedImage()))
	{
		if (transaction)
			transaction.cancel();
		ScMessageBox::warning(this, tr("Scribus - Image Search"),
			tr("The selected replacement image could not be loaded. The original link was kept."),
			QMessageBox::Ok | QMessageBox::Default | QMessageBox::Escape,
			QMessageBox::NoButton);
		return;
	}
	QFileInfo target(currItem->Pfile);
	item->setText(target.fileName());
	item->setIcon(createImgIcon(currItem));
	imageSelected(imageViewArea->currentItem());

	if (dia2->isApplyToMatchingImages())
		relinkMatchingImages(source, target, brokenLink);
	if (transaction)
		transaction.commit();
	applyImageFilters();
}

void PicStatus::FileManager()
{
	if (currItem == nullptr)
		return;
	QFileInfo fi(currItem->Pfile);
	QString path = fi.canonicalPath();
	if (path.isEmpty())
		return;
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void PicStatus::doImageEffects()
{
	if (currItem == nullptr)
		return;

	EffectsDialog* dia = new EffectsDialog(this, currItem, m_Doc);
	if (dia->exec())
	{
		currItem->effectsInUse = dia->effectsList;
		loadPict(currItem, currItem->Pfile);
		imageViewArea->currentItem()->setIcon(createImgIcon(currItem));
	}
	delete dia;
}

void PicStatus::doImageExtProp()
{
	if (currItem == nullptr)
		return;

	ExtImageProps dia(this, currItem, m_Doc->view());
	if (dia.exec())
	{
		loadPict(currItem, currItem->Pfile);
		imageViewArea->currentItem()->setIcon(createImgIcon(currItem));
	}
}

void PicStatus::doEditImage()
{
	if (currItem == nullptr)
		return;
	SelectPic();
	ScCore->primaryMainWindow()->callImageEditor();
}

/**
 * Relink all matching images.
 * Images match if they have the same path as the pattern.
 * If the "pattern" was a broken link, only images with broken links will match.
 */
void PicStatus::relinkMatchingImages(const QFileInfo& source, const QFileInfo& target, bool brokenLink)
{
	for (int i = 0; i < imageViewArea->count(); i++)
	{
		auto item = static_cast<PicItem*>(imageViewArea->item(i));

		if (brokenLink && item->PageItemObject->imageIsAvailable)
			continue;

		QFileInfo fi(item->PageItemObject->Pfile);
		if (fi.path() != source.path())
			continue;
		if (fi.fileName() == target.fileName())
			continue;

		loadPict(item->PageItemObject, QDir(target.path()).filePath(fi.fileName()));
		item->setText(fi.fileName());
		item->setIcon(createImgIcon(item->PageItemObject));
	}
}
