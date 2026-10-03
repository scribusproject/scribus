#include "propertiespalette_attributes.h"
#include "ui_propertiespalette_attributes.h"

#include "commonstrings.h"
#include "epubdocument.h"
#include "epubreadingorderdialog.h"
#include "iconmanager.h"
#include "pageitem.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"

PropertiesPalette_Attributes::PropertiesPalette_Attributes(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	nameEdit->setFocusPolicy(Qt::ClickFocus);

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));

	connect(nameEdit , SIGNAL(Leaved()) , this, SLOT(handleNewName()));
	connect(noPrint  , SIGNAL(clicked()), this, SLOT(handlePrint()));
	connect(buttonPDFBookmark  , SIGNAL(clicked()), this, SLOT(handlePDFBookmark()));
	connect(buttonPDFAnnotation  , SIGNAL(clicked()), this, SLOT(handlePDFAnnotation()));
	connect(buttonPDFAnnotationSettings  , SIGNAL(clicked()), this, SLOT(handlePDFAnnotationSettings()));
	connect(epubOrderSpin, SIGNAL(valueChanged(int)), this, SLOT(handleEpubOrder(int)));
	connect(epubOrderManageButton, SIGNAL(clicked()), this, SLOT(handleManageEpubOrder()));
	connect(epubImageAltEdit, SIGNAL(editingFinished()), this, SLOT(handleEpubImageAltText()));
	connect(epubImageDecorativeCheck, SIGNAL(clicked(bool)), this, SLOT(handleEpubImageDecorative(bool)));
	connect(epubImageCaptionEdit, SIGNAL(editingFinished()), this, SLOT(handleEpubImageCaption()));
	connect(epubImageCaptionAlignmentCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(handleEpubImageCaptionAlignment(int)));
	connect(epubImageWidthSpin, SIGNAL(valueChanged(int)), this, SLOT(handleEpubImageWidth(int)));
	connect(epubImageFrameCropCheck, SIGNAL(clicked(bool)), this, SLOT(handleEpubImageFrameCrop(bool)));
	epubOrderSpin->setEnabled(false);
	epubOrderManageButton->setEnabled(false);
	epubImageAltLabel->setVisible(false);
	epubImageAltEdit->setVisible(false);
	epubImageDecorativeCheck->setVisible(false);
	epubImageCaptionLabel->setVisible(false);
	epubImageCaptionEdit->setVisible(false);
	epubImageCaptionAlignmentLabel->setVisible(false);
	epubImageCaptionAlignmentCombo->setVisible(false);
	epubImageWidthLabel->setVisible(false);
	epubImageWidthSpin->setVisible(false);
	epubImageFrameCropCheck->setVisible(false);


}

void PropertiesPalette_Attributes::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;
}

void PropertiesPalette_Attributes::setDoc(ScribusDoc *d)
{
	if ((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = nullptr;

	m_haveDoc = true;
	m_haveItem = false;
	epubOrderManageButton->setEnabled(true);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Attributes::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc  = false;
	m_haveItem = false;
	m_doc   = nullptr;
	m_item  = nullptr;
	nameEdit->clear();
	epubOrderSpin->setEnabled(false);
	epubOrderManageButton->setEnabled(false);
	epubImageAltEdit->clear();
	epubImageAltLabel->setVisible(false);
	epubImageAltEdit->setVisible(false);
	epubImageDecorativeCheck->setVisible(false);
	epubImageDecorativeCheck->setChecked(false);
	epubImageCaptionEdit->clear();
	epubImageCaptionLabel->setVisible(false);
	epubImageCaptionEdit->setVisible(false);
	epubImageCaptionAlignmentLabel->setVisible(false);
	epubImageCaptionAlignmentCombo->setVisible(false);
	epubImageWidthLabel->setVisible(false);
	epubImageWidthSpin->setVisible(false);
	epubImageFrameCropCheck->setVisible(false);

	setEnabled(false);
}

void PropertiesPalette_Attributes::unsetItem()
{
	m_haveItem = false;
	m_item     = nullptr;
	handleSelectionChanged();
}


PageItem* PropertiesPalette_Attributes::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;

	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 0)
			currentItem = m_doc->m_Selection->itemAt(0);
	}

	return currentItem;
}

void PropertiesPalette_Attributes::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (!m_doc)
		setDoc(item->doc());

	QSignalBlocker sigName(nameEdit);
	QSignalBlocker sigPrint(noPrint);
	QSignalBlocker sigPDFBookmark(buttonPDFBookmark);
	QSignalBlocker sigPDFAnnotation(buttonPDFAnnotation);
	QSignalBlocker sigEpubOrder(epubOrderSpin);
	QSignalBlocker sigEpubImageAlt(epubImageAltEdit);
	QSignalBlocker sigEpubImageDecorative(epubImageDecorativeCheck);
	QSignalBlocker sigEpubImageCaption(epubImageCaptionEdit);
	QSignalBlocker sigEpubImageCaptionAlignment(epubImageCaptionAlignmentCombo);
	QSignalBlocker sigEpubImageWidth(epubImageWidthSpin);
	QSignalBlocker sigEpubImageFrameCrop(epubImageFrameCropCheck);

	m_haveItem = false;
	m_item = item;

	nameEdit->setText(m_item->itemName());
	noPrint->setChecked(!item->printEnabled());

	buttonPDFBookmark->setChecked(m_item->isPDFBookmark());
	buttonPDFAnnotation->setChecked(m_item->isAnnotation());
	buttonPDFAnnotationSettings->setEnabled(buttonPDFAnnotation->isChecked());
	const bool epubEligible = m_doc->m_Selection->count() == 1 &&
		(EpubDocument::canRankStoryRoot(item) || EpubDocument::canRankImageFrame(item));
	epubOrderSpin->setEnabled(epubEligible);
	const int order = EpubDocument::savedOrder(item);
	epubOrderSpin->setValue(qMax(0, order));
	epubOrderSpin->setToolTip(order == -2
		? tr("The saved EPUB reading order is invalid. Assign a positive rank to repair it.")
		: item->isImageFrame()
			? tr("Set this image's position among text stories and images. Give every item a unique number starting at 1.")
			: tr("Set this text story's position in a reflowable EPUB. Give every story a unique number starting at 1."));
	const bool imageEligible = m_doc->m_Selection->count() == 1 &&
		item->isImageFrame() && !item->isMasterItem();
	const bool decorative = imageEligible && EpubDocument::savedImageDecorative(item);
	epubImageAltLabel->setVisible(imageEligible);
	epubImageAltEdit->setVisible(imageEligible);
	epubImageAltEdit->setEnabled(imageEligible && !decorative);
	epubImageAltEdit->setText(imageEligible ? EpubDocument::savedImageAltText(item) : QString());
	epubImageDecorativeCheck->setVisible(imageEligible);
	epubImageDecorativeCheck->setEnabled(imageEligible);
	epubImageDecorativeCheck->setChecked(decorative);
	epubImageCaptionLabel->setVisible(imageEligible);
	epubImageCaptionEdit->setVisible(imageEligible);
	epubImageCaptionEdit->setEnabled(imageEligible && !decorative);
	epubImageCaptionEdit->setText(imageEligible ? EpubDocument::savedImageCaption(item) : QString());
	epubImageCaptionAlignmentLabel->setVisible(imageEligible);
	epubImageCaptionAlignmentCombo->setVisible(imageEligible);
	epubImageCaptionAlignmentCombo->setEnabled(imageEligible && !decorative && !EpubDocument::savedImageCaption(item).isEmpty());
	epubImageCaptionAlignmentCombo->setCurrentIndex(imageEligible ? qMax(0, EpubDocument::savedImageCaptionAlignment(item)) : 0);
	epubImageWidthLabel->setVisible(imageEligible);
	epubImageWidthSpin->setVisible(imageEligible);
	epubImageWidthSpin->setEnabled(imageEligible);
	epubImageWidthSpin->setValue(imageEligible ? qMax(0, EpubDocument::savedImageWidthPercent(item)) : 0);
	epubImageFrameCropCheck->setVisible(imageEligible);
	epubImageFrameCropCheck->setEnabled(imageEligible);
	epubImageFrameCropCheck->setChecked(imageEligible && EpubDocument::savedUseImageFrameCrop(item));

	m_haveItem = true;

}

void PropertiesPalette_Attributes::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

//	nameEdit->setEnabled(m_doc->m_Selection->count() == 1);

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		nameEdit->setEnabled(false);

		setEnabled(true);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;

		m_haveItem = (itemType!=-1);

		nameEdit->setEnabled(true);

		setEnabled(true);

	}
	if (currItem)
	{
		setCurrentItem(currItem);
	}
	else
	{
		epubOrderSpin->setEnabled(false);
		epubImageAltLabel->setVisible(false);
		epubImageAltEdit->setVisible(false);
		epubImageDecorativeCheck->setVisible(false);
		epubImageCaptionLabel->setVisible(false);
		epubImageCaptionEdit->setVisible(false);
		epubImageCaptionAlignmentLabel->setVisible(false);
		epubImageCaptionAlignmentCombo->setVisible(false);
		epubImageWidthLabel->setVisible(false);
		epubImageWidthSpin->setVisible(false);
		epubImageFrameCropCheck->setVisible(false);
	}
	updateGeometry();
}

void PropertiesPalette_Attributes::toggleLabelVisibility(bool visibility)
{
	labelName->setLabelVisibility(visibility);
	labelExport->setLabelVisibility(visibility);
	labelPDFOptions->setLabelVisibility(visibility);
}

void PropertiesPalette_Attributes::handlePrint()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_TogglePrintEnabled();
}

void PropertiesPalette_Attributes::handleNewName()
{
	if (m_ScMW->scriptIsRunning() || !m_haveDoc || !m_haveItem)
		return;
	QString NameOld = m_item->itemName();
	QString NameNew = nameEdit->text();
	if (NameNew.isEmpty())
	{
		nameEdit->setText(NameOld);
		return;
	}
	bool found = false;
	QList<PageItem*> allItems;
	for (int a = 0; a < m_doc->Items->count(); ++a)
	{
		PageItem *currItem = m_doc->Items->at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			PageItem* item = allItems.at(ii);
			if ((NameNew == item->itemName()) && (item != m_item))
			{
				found = true;
				break;
			}
		}
		allItems.clear();
	}
	if (found)
	{
		ScMessageBox::warning(this, CommonStrings::trWarning, "<qt>"+ tr("Name \"%1\" isn't unique.<br/>Please choose another.").arg(NameNew)+"</qt>");
		nameEdit->setText(NameOld);
		nameEdit->setFocus();
	}
	else
	{
		if (m_item->itemName() != nameEdit->text())
		{
			m_item->setItemName(nameEdit->text());
			m_doc->changed();
		}
	}
}

void PropertiesPalette_Attributes::handlePDFBookmark()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ToggleBookMark();
}

void PropertiesPalette_Attributes::handlePDFAnnotation()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ToggleAnnotation();
}

void PropertiesPalette_Attributes::handlePDFAnnotationSettings()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_ScMW->ModifyAnnot();
}

void PropertiesPalette_Attributes::handleEpubOrder(int rank)
{
	if (!m_haveDoc || !m_haveItem || !m_item || !m_ScMW || m_ScMW->scriptIsRunning() ||
		m_doc->m_Selection->count() != 1)
		return;
	EpubDocument::setSavedOrder(m_item, rank);
}

void PropertiesPalette_Attributes::handleManageEpubOrder()
{
	if (!m_haveDoc || !m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	const QVector<PageItem*> items = EpubDocument::rankableItems(*m_doc);
	QVector<EpubStoryEntry> stories;
	for (const PageItem* item : items)
	{
		const bool isImage = item->isImageFrame();
		const QString preview = isImage ? EpubDocument::savedImageAltText(item)
			: item->itemText.plainText().simplified();
		stories.append({ item->itemName(), preview.left(60), item->OwnPage,
			EpubDocument::savedOrder(item), isImage });
	}
	EpubReadingOrderDialog dialog(stories, this);
	if (dialog.exec() != QDialog::Accepted)
		return;
	QVector<PageItem*> orderedItems;
	for (int index : dialog.orderedIndices())
		orderedItems.append(items.at(index));
	if (!EpubDocument::assignMixedSavedOrder(*m_doc, orderedItems))
		ScMessageBox::warning(this, CommonStrings::trWarning,
			tr("The EPUB reading order could not be saved because the text stories or images changed."));
}

void PropertiesPalette_Attributes::handleEpubImageAltText()
{
	if (!m_haveDoc || !m_haveItem || !m_item || !m_ScMW || m_ScMW->scriptIsRunning() ||
		m_doc->m_Selection->count() != 1)
		return;
	if (!EpubDocument::setSavedImageAltText(m_item, epubImageAltEdit->text()))
	{
		ScMessageBox::warning(this, CommonStrings::trWarning,
			tr("EPUB image description must be valid text, or empty to clear it. Unmark decorative images before adding a description."));
		epubImageAltEdit->setText(EpubDocument::savedImageAltText(m_item));
	}
}

void PropertiesPalette_Attributes::handleEpubImageDecorative(bool decorative)
{
	if (!m_haveDoc || !m_haveItem || !m_item || !m_ScMW || m_ScMW->scriptIsRunning() ||
		m_doc->m_Selection->count() != 1)
		return;
	if (!EpubDocument::setSavedImageDecorative(m_item, decorative))
	{
		ScMessageBox::warning(this, CommonStrings::trWarning,
			tr("Clear this image's EPUB description and visible caption before marking it decorative."));
		QSignalBlocker blocker(epubImageDecorativeCheck);
		epubImageDecorativeCheck->setChecked(EpubDocument::savedImageDecorative(m_item));
	}
	const bool saved = EpubDocument::savedImageDecorative(m_item);
	epubImageAltEdit->setEnabled(!saved);
	epubImageCaptionEdit->setEnabled(!saved);
	epubImageCaptionAlignmentCombo->setEnabled(!saved && !EpubDocument::savedImageCaption(m_item).isEmpty());
}

void PropertiesPalette_Attributes::handleEpubImageCaption()
{
	if (!m_haveDoc || !m_haveItem || !m_item || !m_ScMW || m_ScMW->scriptIsRunning() ||
		m_doc->m_Selection->count() != 1)
		return;
	if (!EpubDocument::setSavedImageCaption(m_item, epubImageCaptionEdit->text()))
	{
		ScMessageBox::warning(this, CommonStrings::trWarning,
			tr("EPUB image caption must be valid text, or empty to clear it. Decorative images cannot have a visible caption."));
		epubImageCaptionEdit->setText(EpubDocument::savedImageCaption(m_item));
	}
	QSignalBlocker blocker(epubImageCaptionAlignmentCombo);
	epubImageCaptionAlignmentCombo->setEnabled(!EpubDocument::savedImageDecorative(m_item) &&
		!EpubDocument::savedImageCaption(m_item).isEmpty());
	epubImageCaptionAlignmentCombo->setCurrentIndex(qMax(0, EpubDocument::savedImageCaptionAlignment(m_item)));
}

void PropertiesPalette_Attributes::handleEpubImageCaptionAlignment(int alignment)
{
	if (!m_haveDoc || !m_haveItem || !m_item || !m_ScMW || m_ScMW->scriptIsRunning() ||
		m_doc->m_Selection->count() != 1)
		return;
	if (!EpubDocument::setSavedImageCaptionAlignment(m_item, alignment))
	{
		ScMessageBox::warning(this, CommonStrings::trWarning,
			tr("Add a valid EPUB image caption before changing its alignment."));
		QSignalBlocker blocker(epubImageCaptionAlignmentCombo);
		epubImageCaptionAlignmentCombo->setCurrentIndex(qMax(0, EpubDocument::savedImageCaptionAlignment(m_item)));
	}
}

void PropertiesPalette_Attributes::handleEpubImageWidth(int widthPercent)
{
	if (!m_haveDoc || !m_haveItem || !m_item || !m_ScMW || m_ScMW->scriptIsRunning() ||
		m_doc->m_Selection->count() != 1)
		return;
	if (!EpubDocument::setSavedImageWidthPercent(m_item, widthPercent))
	{
		ScMessageBox::warning(this, CommonStrings::trWarning,
			tr("EPUB image width must be Auto or between 1% and 100%."));
		QSignalBlocker blocker(epubImageWidthSpin);
		epubImageWidthSpin->setValue(qMax(0, EpubDocument::savedImageWidthPercent(m_item)));
	}
}

void PropertiesPalette_Attributes::handleEpubImageFrameCrop(bool enabled)
{
	if (!m_haveDoc || !m_haveItem || !m_item || !m_ScMW || m_ScMW->scriptIsRunning() ||
		m_doc->m_Selection->count() != 1)
		return;
	if (!EpubDocument::setSavedUseImageFrameCrop(m_item, enabled))
	{
		ScMessageBox::warning(this, CommonStrings::trWarning,
			tr("The EPUB frame-crop choice could not be saved for this image."));
		QSignalBlocker blocker(epubImageFrameCropCheck);
		epubImageFrameCropCheck->setChecked(EpubDocument::savedUseImageFrameCrop(m_item));
	}
}

void PropertiesPalette_Attributes::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PropertiesPalette_Attributes::iconSetChange()
{
	IconManager& im = IconManager::instance();

	QIcon icoPrint;
	icoPrint.addPixmap(im.loadPixmap("no-print"), QIcon::Normal, QIcon::On);
	icoPrint.addPixmap(im.loadPixmap("document-print"), QIcon::Normal, QIcon::Off);
	noPrint->setIcon(icoPrint);

	labelName->setPixmap(im.loadPixmap("name"));
	buttonPDFBookmark->setIcon(im.loadPixmap("pdf-bookmark"));
	buttonPDFAnnotation->setIcon(im.loadPixmap("pdf-annotation-text"));
	buttonPDFAnnotationSettings->setIcon(im.loadIcon("settings"));

}

void PropertiesPalette_Attributes::languageChange()
{
	retranslateUi(this);
	if (m_item)
	{
		const int order = EpubDocument::savedOrder(m_item);
		epubOrderSpin->setToolTip(order == -2
			? tr("The saved EPUB reading order is invalid. Assign a positive rank to repair it.")
			: m_item->isImageFrame()
				? tr("Set this image's position among text stories and images. Give every item a unique number starting at 1.")
				: tr("Set this text story's position in a reflowable EPUB. Give every story a unique number starting at 1."));
	}
}
