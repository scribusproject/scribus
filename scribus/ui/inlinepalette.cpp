/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          inlinepalette.cpp  -  description
                             -------------------
    begin                : Tue Mar 27 2012
    copyright            : (C) 2012 by Franz Schmid
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
#include "inlinepalette.h"
#include <QPainter>
#include <QByteArray>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QDrag>
#include <QFileInfo>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMimeData>
#include <QPushButton>
#include <QSignalBlocker>
#include <QVBoxLayout>

#include "appmodes.h"
#include "iconmanager.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scmessagebox.h"
#include "scrspinbox.h"
#include "selection.h"
#include "undomanager.h"
#include "undotransaction.h"
#include "units.h"
#include "util_formats.h"
#include "ui/customfdialog.h"

namespace
{
class AnchorOptionsDialog : public QDialog
{
public:
	explicit AnchorOptionsDialog(const AnchorPosition& position, int unitIndex, QWidget* parent = nullptr)
		: QDialog(parent)
		, m_unitIndex(unitIndex)
	{
		setWindowTitle(tr("Anchored Object Options"));
		setMinimumWidth(420);
		auto* mainLayout = new QVBoxLayout(this);

		auto* modeLayout = new QFormLayout;
		m_mode = new QComboBox(this);
		m_mode->addItem(tr("Inline"), static_cast<int>(AnchorPosition::Mode::Inline));
		m_mode->addItem(tr("Above Line"), static_cast<int>(AnchorPosition::Mode::AboveLine));
		m_mode->addItem(tr("Custom"), static_cast<int>(AnchorPosition::Mode::Custom));
		modeLayout->addRow(tr("Position:"), m_mode);
		mainLayout->addLayout(modeLayout);

		m_positionGroup = new QGroupBox(tr("Position"), this);
		auto* positionLayout = new QFormLayout(m_positionGroup);
		m_horizontalReference = new QComboBox(m_positionGroup);
		m_horizontalReference->addItem(tr("Anchor Character"), static_cast<int>(AnchorPosition::HorizontalReference::AnchorCharacter));
		m_horizontalReference->addItem(tr("Text Column"), static_cast<int>(AnchorPosition::HorizontalReference::TextColumn));
		m_horizontalReference->addItem(tr("Text Frame"), static_cast<int>(AnchorPosition::HorizontalReference::TextFrame));
		m_horizontalReference->addItem(tr("Page"), static_cast<int>(AnchorPosition::HorizontalReference::Page));
		m_horizontalReference->addItem(tr("Spread"), static_cast<int>(AnchorPosition::HorizontalReference::Spread));
		m_horizontalAlignment = new QComboBox(m_positionGroup);
		m_horizontalAlignment->addItem(tr("Left"), static_cast<int>(AnchorPosition::HorizontalAlignment::Left));
		m_horizontalAlignment->addItem(tr("Center"), static_cast<int>(AnchorPosition::HorizontalAlignment::Center));
		m_horizontalAlignment->addItem(tr("Right"), static_cast<int>(AnchorPosition::HorizontalAlignment::Right));
		m_horizontalAlignment->addItem(tr("Spine"), static_cast<int>(AnchorPosition::HorizontalAlignment::Spine));
		m_horizontalAlignment->addItem(tr("Away from Spine"), static_cast<int>(AnchorPosition::HorizontalAlignment::AwayFromSpine));
		m_horizontalAlignment->addItem(tr("Custom"), static_cast<int>(AnchorPosition::HorizontalAlignment::Custom));
		m_verticalReference = new QComboBox(m_positionGroup);
		m_verticalReference->addItem(tr("Anchor Line"), static_cast<int>(AnchorPosition::VerticalReference::AnchorLine));
		m_verticalReference->addItem(tr("Paragraph"), static_cast<int>(AnchorPosition::VerticalReference::Paragraph));
		m_verticalReference->addItem(tr("Text Frame"), static_cast<int>(AnchorPosition::VerticalReference::TextFrame));
		m_verticalReference->addItem(tr("Page"), static_cast<int>(AnchorPosition::VerticalReference::Page));
		m_verticalAlignment = new QComboBox(m_positionGroup);
		m_verticalAlignment->addItem(tr("Top"), static_cast<int>(AnchorPosition::VerticalAlignment::Top));
		m_verticalAlignment->addItem(tr("Center"), static_cast<int>(AnchorPosition::VerticalAlignment::Center));
		m_verticalAlignment->addItem(tr("Bottom"), static_cast<int>(AnchorPosition::VerticalAlignment::Bottom));
		m_verticalAlignment->addItem(tr("Baseline"), static_cast<int>(AnchorPosition::VerticalAlignment::Baseline));
		m_verticalAlignment->addItem(tr("Custom"), static_cast<int>(AnchorPosition::VerticalAlignment::Custom));
		m_xOffset = createDistanceSpinBox();
		m_yOffset = createDistanceSpinBox();
		positionLayout->addRow(tr("Horizontal reference:"), m_horizontalReference);
		positionLayout->addRow(tr("Horizontal alignment:"), m_horizontalAlignment);
		positionLayout->addRow(tr("Vertical reference:"), m_verticalReference);
		positionLayout->addRow(tr("Vertical alignment:"), m_verticalAlignment);
		positionLayout->addRow(tr("X offset:"), m_xOffset);
		positionLayout->addRow(tr("Y offset:"), m_yOffset);
		mainLayout->addWidget(m_positionGroup);

		m_wrapGroup = new QGroupBox(tr("Text Wrap"), this);
		auto* wrapLayout = new QFormLayout(m_wrapGroup);
		m_wrapMode = new QComboBox(m_wrapGroup);
		m_wrapMode->addItem(tr("None"), static_cast<int>(AnchorPosition::WrapMode::None));
		m_wrapMode->addItem(tr("Bounding Box"), static_cast<int>(AnchorPosition::WrapMode::BoundingBox));
		m_wrapMode->addItem(tr("Frame Shape"), static_cast<int>(AnchorPosition::WrapMode::FrameShape));
		m_wrapMode->addItem(tr("Contour"), static_cast<int>(AnchorPosition::WrapMode::Contour));
		m_wrapMode->addItem(tr("Image Clip Path"), static_cast<int>(AnchorPosition::WrapMode::ImageClipPath));
		m_wrapLeft = createDistanceSpinBox(false);
		m_wrapTop = createDistanceSpinBox(false);
		m_wrapRight = createDistanceSpinBox(false);
		m_wrapBottom = createDistanceSpinBox(false);
		wrapLayout->addRow(tr("Wrap shape:"), m_wrapMode);
		wrapLayout->addRow(tr("Left offset:"), m_wrapLeft);
		wrapLayout->addRow(tr("Top offset:"), m_wrapTop);
		wrapLayout->addRow(tr("Right offset:"), m_wrapRight);
		wrapLayout->addRow(tr("Bottom offset:"), m_wrapBottom);
		mainLayout->addWidget(m_wrapGroup);

		m_keepWithinBounds = new QCheckBox(tr("Keep within reference bounds"), this);
		m_lockPosition = new QCheckBox(tr("Prevent manual positioning"), this);
		mainLayout->addWidget(m_keepWithinBounds);
		mainLayout->addWidget(m_lockPosition);
		auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
		connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
		connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
		mainLayout->addWidget(buttons);

		setComboValue(m_mode, static_cast<int>(position.mode));
		setComboValue(m_horizontalReference, static_cast<int>(position.horizontalReference));
		setComboValue(m_verticalReference, static_cast<int>(position.verticalReference));
		setComboValue(m_horizontalAlignment, static_cast<int>(position.horizontalAlignment));
		setComboValue(m_verticalAlignment, static_cast<int>(position.verticalAlignment));
		setComboValue(m_wrapMode, static_cast<int>(position.wrapMode));
		m_xOffset->setValue(position.xOffset, SC_PT);
		m_yOffset->setValue(position.yOffset, SC_PT);
		m_wrapLeft->setValue(position.wrapOffsets.left(), SC_PT);
		m_wrapTop->setValue(position.wrapOffsets.top(), SC_PT);
		m_wrapRight->setValue(position.wrapOffsets.right(), SC_PT);
		m_wrapBottom->setValue(position.wrapOffsets.bottom(), SC_PT);
		m_keepWithinBounds->setChecked(position.keepWithinBounds);
		m_lockPosition->setChecked(position.preventManualPositioning);
		connect(m_mode, &QComboBox::currentIndexChanged, this, [this] { updateEnabledState(); });
		updateEnabledState();
	}

	AnchorPosition position() const
	{
		AnchorPosition result;
		result.mode = static_cast<AnchorPosition::Mode>(m_mode->currentData().toInt());
		result.horizontalReference = static_cast<AnchorPosition::HorizontalReference>(m_horizontalReference->currentData().toInt());
		result.verticalReference = static_cast<AnchorPosition::VerticalReference>(m_verticalReference->currentData().toInt());
		result.horizontalAlignment = static_cast<AnchorPosition::HorizontalAlignment>(m_horizontalAlignment->currentData().toInt());
		result.verticalAlignment = static_cast<AnchorPosition::VerticalAlignment>(m_verticalAlignment->currentData().toInt());
		result.wrapMode = static_cast<AnchorPosition::WrapMode>(m_wrapMode->currentData().toInt());
		result.xOffset = m_xOffset->getValue(SC_PT);
		result.yOffset = m_yOffset->getValue(SC_PT);
		result.wrapOffsets = QMarginsF(m_wrapLeft->getValue(SC_PT), m_wrapTop->getValue(SC_PT),
			m_wrapRight->getValue(SC_PT), m_wrapBottom->getValue(SC_PT));
		result.keepWithinBounds = m_keepWithinBounds->isChecked();
		result.preventManualPositioning = m_lockPosition->isChecked();
		return result;
	}

private:
	ScrSpinBox* createDistanceSpinBox(bool allowNegative = true)
	{
		auto* spin = new ScrSpinBox(allowNegative ? -10000.0 : 0.0, 10000.0, this, m_unitIndex);
		return spin;
	}

	static void setComboValue(QComboBox* combo, int value)
	{
		const int index = combo->findData(value);
		if (index >= 0)
			combo->setCurrentIndex(index);
	}

	void updateEnabledState()
	{
		const auto mode = static_cast<AnchorPosition::Mode>(m_mode->currentData().toInt());
		m_positionGroup->setEnabled(mode != AnchorPosition::Mode::Inline);
		m_wrapGroup->setEnabled(mode == AnchorPosition::Mode::Custom);
		m_keepWithinBounds->setEnabled(mode == AnchorPosition::Mode::Custom);
		m_lockPosition->setEnabled(mode != AnchorPosition::Mode::Inline);
	}

	QComboBox* m_mode { nullptr };
	QGroupBox* m_positionGroup { nullptr };
	QComboBox* m_horizontalReference { nullptr };
	QComboBox* m_horizontalAlignment { nullptr };
	QComboBox* m_verticalReference { nullptr };
	QComboBox* m_verticalAlignment { nullptr };
	ScrSpinBox* m_xOffset { nullptr };
	ScrSpinBox* m_yOffset { nullptr };
	QGroupBox* m_wrapGroup { nullptr };
	QComboBox* m_wrapMode { nullptr };
	ScrSpinBox* m_wrapLeft { nullptr };
	ScrSpinBox* m_wrapTop { nullptr };
	ScrSpinBox* m_wrapRight { nullptr };
	ScrSpinBox* m_wrapBottom { nullptr };
	QCheckBox* m_keepWithinBounds { nullptr };
	QCheckBox* m_lockPosition { nullptr };
	int m_unitIndex { SC_PT };
};
}

InlineView::InlineView(QWidget* parent) : QListWidget(parent)
{
	setDragEnabled(true);
	setViewMode(QListView::IconMode);
	setFlow(QListView::LeftToRight);
	setSortingEnabled(true);
	setWrapping(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setResizeMode(QListView::Adjust);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	delegate = new ScListWidgetDelegate(this, this);
	delegate->setIconOnly(true);
	setItemDelegate(delegate);
	setIconSize(QSize(50, 50));
}

void InlineView::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void InlineView::dragMoveEvent(QDragMoveEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void InlineView::dropEvent(QDropEvent *e)
{
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		if (e->source() == this)
			return;
		QString text = e->mimeData()->text();
		if ((text.startsWith("<SCRIBUSELEM")) || (text.startsWith("<SCRIBUSELEMUTF8")) || (text.startsWith("<ScribusElementUTF8")))
		{
			emit objectDropped(text);
		}
	}
	else
		e->ignore();
}

 void InlineView::startDrag(Qt::DropActions supportedActions)
 {
	QMimeData *mimeData = new QMimeData;
	int id = currentItem()->data(Qt::UserRole).toInt();
	QByteArray data;
	data.setNum(id);
	mimeData->setData("text/inline", data);
	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(currentItem()->icon().pixmap(48, 48));
	drag->exec(Qt::CopyAction);
	clearSelection();
}

InlinePalette::InlinePalette( QWidget* parent) : DockPanelBase("Inline", "panel-inline-items", parent)
{
	setContentsMargins(3, 3, 3, 3);
	setMinimumSize( QSize( 220, 240 ) );
	setObjectName(QString::fromLocal8Bit("Inline"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	InlineViewWidget = new InlineView(this);
	InlineViewWidget->clear();
	setWidget( InlineViewWidget );

	unsetDoc();
	m_scMW  = nullptr;
	currentEditedItem = -1;
	languageChange();
	connect(InlineViewWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleDoubleClick(QListWidgetItem*)));
	connect(InlineViewWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(handleContextMenue(QPoint)));
	connect(InlineViewWidget, SIGNAL(objectDropped(QString)), this, SIGNAL(objectDropped(QString)));
}

void InlinePalette::handleContextMenue(QPoint p)
{
	if (currentEditedItem > 0)
		return;
	QListWidgetItem *item = InlineViewWidget->itemAt(p);
	if (item)
	{
		actItem = item->data(Qt::UserRole).toInt();
		bool txFrame = false;
		if (m_doc->m_Selection->isNotEmpty())
		{
			PageItem* selItem = m_doc->m_Selection->itemAt(0);
			if ((selItem->isTextFrame() || selItem->isTable()))
				txFrame = true;
		}
		QMenu *pmenu = new QMenu();
		if (txFrame)
		{
			QAction* pasteAct = pmenu->addAction( tr("Paste to Item"));
			connect(pasteAct, SIGNAL(triggered()), this, SLOT(handlePasteToItem()));
		}
		PageItem* inlineItem = m_doc->FrameItems.value(actItem, nullptr);
		if ((m_doc->appMode != modeEdit) && (m_doc->appMode != modeEditTable)
			&& (!inlineItem || !inlineItem->isImageFrame()))
		{
			QAction* editAct = pmenu->addAction( tr("Edit Item"));
			connect(editAct, SIGNAL(triggered()), this, SLOT(handleEditItem()));
		}
		if (inlineItem && inlineItem->isImageFrame())
		{
			QAction* editImageAct = pmenu->addAction(tr("Edit Anchored Image..."));
			connect(editImageAct, &QAction::triggered, this, [this] { editAnchoredImage(actItem); });
			QAction* replaceImageAct = pmenu->addAction(tr("Replace Image..."));
			connect(replaceImageAct, &QAction::triggered, this, [this] { replaceAnchoredImage(actItem); });
			QAction* fullImageAct = pmenu->addAction(tr("Edit Full Image Frame..."));
			connect(fullImageAct, &QAction::triggered, this, [this] { emit startEdit(actItem); });
		}
		else if (inlineItem && inlineItem->isTable() && m_doc->appMode == modeEdit)
		{
			QAction* editTableAct = pmenu->addAction(tr("Edit Table..."));
			connect(editTableAct, &QAction::triggered, this, [this] { emit startEdit(actItem); });
		}
		QAction* anchorAct = pmenu->addAction(tr("Anchored Object Options..."));
		connect(anchorAct, &QAction::triggered, this, &InlinePalette::handleAnchorOptions);
		QAction* delAct = pmenu->addAction( tr("Remove Item"));
		connect(delAct, SIGNAL(triggered()), this, SLOT(handleDeleteItem()));
		pmenu->exec(QCursor::pos());
		delete pmenu;
		actItem = -1;
	}
}

void InlinePalette::handlePasteToItem()
{
	PageItem* selItem = m_doc->m_Selection->itemAt(0);
	PageItem_TextFrame *currItem;
	if (selItem->isTable())
		currItem = selItem->asTable()->activeCell().textFrame();
	else
		currItem = selItem->asTextFrame();
	if (currItem->HasSel)
		currItem->deleteSelectedTextFromFrame();
	currItem->itemText.insertObject(actItem);
	if (selItem->isTable())
		selItem->asTable()->update();
	else
		currItem->update();
}

void InlinePalette::handleEditItem()
{
	emit startEdit(actItem);
}

void InlinePalette::handleAnchorOptions()
{
	editAnchorOptions(actItem);
}

void InlinePalette::editAnchorOptions(int itemID)
{
	PageItem* item = m_doc ? m_doc->FrameItems.value(itemID, nullptr) : nullptr;
	if (!item)
		return;

	AnchorOptionsDialog dialog(item->anchorPosition(), m_doc->unitIndex(), this);
	if (dialog.exec() != QDialog::Accepted)
		return;
	const AnchorPosition newPosition = dialog.position();
	if (newPosition == item->anchorPosition())
		return;

	item->setAnchorPosition(newPosition);
	updateItemList();
}

void InlinePalette::replaceAnchoredImage(int itemID)
{
	PageItem* item = m_doc ? m_doc->FrameItems.value(itemID, nullptr) : nullptr;
	if (!item || !item->isImageFrame() || !m_scMW)
		return;
	const QString format = FormatsManager::instance()->fileDialogFormatList(FormatsManager::IMAGESIMGFRAME);
	CustomFDialog fileDialog(this, QFileInfo(item->Pfile).absolutePath(), tr("Replace Anchored Image"), format,
		fdShowPreview | fdExistingFiles | fdDisableOk, contextImages);
	if (fileDialog.exec() != QDialog::Accepted)
		return;
	const QString selected = fileDialog.selectedFiles().value(0);
	if (selected.isEmpty())
		return;
	if (!item->relinkImage(selected, false))
	{
		ScMessageBox::warning(this, tr("Replace Anchored Image"), tr("The selected image could not be loaded. The existing image was kept."));
		return;
	}
	m_doc->invalidateAll();
	m_scMW->view->DrawNew();
	updateItemList();
}

void InlinePalette::editAnchoredImage(int itemID)
{
	PageItem* item = m_doc ? m_doc->FrameItems.value(itemID, nullptr) : nullptr;
	if (!item || !item->isImageFrame() || !m_scMW)
		return;

	QDialog dialog(this);
	dialog.setWindowTitle(tr("Edit Anchored Image"));
	dialog.setMinimumWidth(360);
	auto* mainLayout = new QVBoxLayout(&dialog);
	const double unitRatio = unitGetRatioFromIndex(m_doc->unitIndex());
	const int unitIndex = m_doc->unitIndex();
	const double originalAspect = item->width() > 0.0 ? item->height() / item->width() : 1.0;

	auto* frameGroup = new QGroupBox(tr("Frame Size"), &dialog);
	auto* frameLayout = new QFormLayout(frameGroup);
	auto* frameWidth = new ScrSpinBox(0.1 * unitRatio, 10000.0 * unitRatio, frameGroup, unitIndex);
	auto* frameHeight = new ScrSpinBox(0.1 * unitRatio, 10000.0 * unitRatio, frameGroup, unitIndex);
	frameWidth->setValue(item->width(), SC_PT);
	frameHeight->setValue(item->height(), SC_PT);
	const double initialFrameWidth = frameWidth->value();
	const double initialFrameHeight = frameHeight->value();
	auto* keepProportions = new QCheckBox(tr("Keep frame proportions"), frameGroup);
	keepProportions->setChecked(true);
	frameLayout->addRow(tr("Width:"), frameWidth);
	frameLayout->addRow(tr("Height:"), frameHeight);
	frameLayout->addRow(QString(), keepProportions);
	connect(frameWidth, &QDoubleSpinBox::valueChanged, &dialog, [frameWidth, frameHeight, keepProportions, originalAspect] {
		if (keepProportions->isChecked())
		{
			QSignalBlocker blocker(frameHeight);
			frameHeight->setValue(frameWidth->getValue(SC_PT) * originalAspect, SC_PT);
		}
	});
	connect(frameHeight, &QDoubleSpinBox::valueChanged, &dialog, [frameWidth, frameHeight, keepProportions, originalAspect] {
		if (keepProportions->isChecked() && originalAspect > 0.0)
		{
			QSignalBlocker blocker(frameWidth);
			frameWidth->setValue(frameHeight->getValue(SC_PT) / originalAspect, SC_PT);
		}
	});
	mainLayout->addWidget(frameGroup);

	QString replacementPath;
	auto* sourceGroup = new QGroupBox(tr("Source Image"), &dialog);
	auto* sourceLayout = new QFormLayout(sourceGroup);
	auto* sourceName = new QLabel(item->Pfile.isEmpty() ? tr("No image") : QFileInfo(item->Pfile).fileName(), sourceGroup);
	sourceName->setWordWrap(true);
	sourceLayout->addRow(tr("File:"), sourceName);
	auto* replaceButton = new QPushButton(tr("Replace Image..."), sourceGroup);
	connect(replaceButton, &QPushButton::clicked, &dialog, [&] {
		const QString format = FormatsManager::instance()->fileDialogFormatList(FormatsManager::IMAGESIMGFRAME);
		const QString startDir = QFileInfo(replacementPath.isEmpty() ? item->Pfile : replacementPath).absolutePath();
		CustomFDialog fileDialog(&dialog, startDir, tr("Replace Anchored Image"), format,
			fdShowPreview | fdExistingFiles | fdDisableOk, contextImages);
		if (fileDialog.exec() != QDialog::Accepted)
			return;
		const QString selected = fileDialog.selectedFiles().value(0);
		if (selected.isEmpty())
			return;
		replacementPath = selected;
		sourceName->setText(QFileInfo(selected).fileName());
	});
	sourceLayout->addRow(QString(), replaceButton);
	mainLayout->addWidget(sourceGroup);

	auto* cropGroup = new QGroupBox(tr("Image and Crop"), &dialog);
	auto* cropLayout = new QFormLayout(cropGroup);
	auto* fitToFrame = new QCheckBox(tr("Fit image to frame"), cropGroup);
	fitToFrame->setChecked(item->fitImageToFrame());
	cropLayout->addRow(QString(), fitToFrame);
	const double xres = item->pixm.imgInfo.xres > 0.0 ? item->pixm.imgInfo.xres : 72.0;
	const double yres = item->pixm.imgInfo.yres > 0.0 ? item->pixm.imgInfo.yres : 72.0;
	auto* imageScaleX = new QDoubleSpinBox(cropGroup);
	auto* imageScaleY = new QDoubleSpinBox(cropGroup);
	for (auto* scale : { imageScaleX, imageScaleY })
	{
		scale->setRange(1.0, 30000.0);
		scale->setDecimals(2);
		scale->setSuffix(tr(" %"));
	}
	imageScaleX->setValue(item->imageXScale() * xres / 72.0 * 100.0);
	imageScaleY->setValue(item->imageYScale() * yres / 72.0 * 100.0);
	const double initialImageScaleX = imageScaleX->value();
	const double initialImageScaleY = imageScaleY->value();
	auto* imageOffsetX = new ScrSpinBox(-10000.0 * unitRatio, 10000.0 * unitRatio, cropGroup, unitIndex);
	auto* imageOffsetY = new ScrSpinBox(-10000.0 * unitRatio, 10000.0 * unitRatio, cropGroup, unitIndex);
	imageOffsetX->setValue(item->imageXOffset() * item->imageXScale(), SC_PT);
	imageOffsetY->setValue(item->imageYOffset() * item->imageYScale(), SC_PT);
	const double initialImageOffsetX = imageOffsetX->value();
	const double initialImageOffsetY = imageOffsetY->value();
	cropLayout->addRow(tr("Horizontal scale:"), imageScaleX);
	cropLayout->addRow(tr("Vertical scale:"), imageScaleY);
	cropLayout->addRow(tr("Horizontal crop offset:"), imageOffsetX);
	cropLayout->addRow(tr("Vertical crop offset:"), imageOffsetY);
	auto updateCropEnabled = [=](bool fit) {
		imageScaleX->setEnabled(!fit);
		imageScaleY->setEnabled(!fit);
		imageOffsetX->setEnabled(!fit);
		imageOffsetY->setEnabled(!fit);
	};
	connect(fitToFrame, &QCheckBox::toggled, &dialog, updateCropEnabled);
	updateCropEnabled(fitToFrame->isChecked());
	mainLayout->addWidget(cropGroup);

	AnchorPosition anchor = item->anchorPosition();
	auto* positionGroup = new QGroupBox(tr("Position and Text Wrap"), &dialog);
	auto* positionLayout = new QFormLayout(positionGroup);
	auto* placement = new QComboBox(positionGroup);
	placement->addItem(tr("Inline"), static_cast<int>(AnchorPosition::Mode::Inline));
	placement->addItem(tr("Above Line"), static_cast<int>(AnchorPosition::Mode::AboveLine));
	placement->addItem(tr("Floating"), static_cast<int>(AnchorPosition::Mode::Custom));
	placement->setCurrentIndex(placement->findData(static_cast<int>(anchor.mode)));
	positionLayout->addRow(tr("Placement:"), placement);
	auto* anchorOffsetX = new ScrSpinBox(-10000.0 * unitRatio, 10000.0 * unitRatio, positionGroup, unitIndex);
	auto* anchorOffsetY = new ScrSpinBox(-10000.0 * unitRatio, 10000.0 * unitRatio, positionGroup, unitIndex);
	anchorOffsetX->setValue(anchor.xOffset, SC_PT);
	anchorOffsetY->setValue(anchor.yOffset, SC_PT);
	anchorOffsetY->setToolTip(tr("For Above Line placement, a positive vertical offset moves the image upward."));
	positionLayout->addRow(tr("Horizontal offset:"), anchorOffsetX);
	positionLayout->addRow(tr("Vertical offset:"), anchorOffsetY);
	auto* allowDragging = new QCheckBox(tr("Allow moving by dragging on canvas"), positionGroup);
	allowDragging->setChecked(!anchor.preventManualPositioning);
	positionLayout->addRow(QString(), allowDragging);
	auto updatePositionControls = [&] {
		const bool inlineMode = anchor.mode == AnchorPosition::Mode::Inline;
		anchorOffsetX->setEnabled(!inlineMode);
		anchorOffsetY->setEnabled(!inlineMode);
		allowDragging->setEnabled(!inlineMode);
	};
	connect(placement, &QComboBox::currentIndexChanged, &dialog, [&] {
		const auto mode = static_cast<AnchorPosition::Mode>(placement->currentData().toInt());
		if (anchor.mode == AnchorPosition::Mode::Inline && mode == AnchorPosition::Mode::Custom)
		{
			anchor.horizontalReference = AnchorPosition::HorizontalReference::AnchorCharacter;
			anchor.horizontalAlignment = AnchorPosition::HorizontalAlignment::Custom;
			anchor.verticalReference = AnchorPosition::VerticalReference::AnchorLine;
			anchor.verticalAlignment = AnchorPosition::VerticalAlignment::Baseline;
			anchor.wrapMode = AnchorPosition::WrapMode::BoundingBox;
			anchor.wrapOffsets = QMarginsF(4.0, 4.0, 4.0, 4.0);
		}
		anchor.mode = mode;
		updatePositionControls();
	});
	connect(anchorOffsetX, &QDoubleSpinBox::valueChanged, &dialog, [&] { anchor.xOffset = anchorOffsetX->getValue(SC_PT); });
	connect(anchorOffsetY, &QDoubleSpinBox::valueChanged, &dialog, [&] { anchor.yOffset = anchorOffsetY->getValue(SC_PT); });
	connect(allowDragging, &QCheckBox::toggled, &dialog, [&] (bool allowed) { anchor.preventManualPositioning = !allowed; });
	updatePositionControls();
	auto* positionButton = new QPushButton(tr("Advanced Position and Wrap..."), positionGroup);
	connect(positionButton, &QPushButton::clicked, &dialog, [&] {
		AnchorOptionsDialog options(anchor, unitIndex, &dialog);
		if (options.exec() == QDialog::Accepted)
		{
			anchor = options.position();
			QSignalBlocker placementBlocker(placement);
			QSignalBlocker xBlocker(anchorOffsetX);
			QSignalBlocker yBlocker(anchorOffsetY);
			QSignalBlocker dragBlocker(allowDragging);
			placement->setCurrentIndex(placement->findData(static_cast<int>(anchor.mode)));
			anchorOffsetX->setValue(anchor.xOffset, SC_PT);
			anchorOffsetY->setValue(anchor.yOffset, SC_PT);
			allowDragging->setChecked(!anchor.preventManualPositioning);
			updatePositionControls();
		}
	});
	positionLayout->addRow(QString(), positionButton);
	mainLayout->addWidget(positionGroup);
	auto* fullEditorButton = new QPushButton(tr("Open Full Image Editor (discard changes above)..."), &dialog);
	connect(fullEditorButton, &QPushButton::clicked, &dialog, [&] { dialog.done(2); });
	mainLayout->addWidget(fullEditorButton);
	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
	connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
	mainLayout->addWidget(buttons);
	const int result = dialog.exec();
	if (result == 2)
	{
		m_scMW->editInlineStart(itemID);
		return;
	}
	if (result != QDialog::Accepted)
		return;

	const double newWidth = frameWidth->getValue(SC_PT);
	const double newHeight = frameHeight->getValue(SC_PT);
	const bool geometryChanged = frameWidth->value() != initialFrameWidth || frameHeight->value() != initialFrameHeight;
	const bool fittingChanged = fitToFrame->isChecked() != item->fitImageToFrame();
	const bool scaleXChanged = imageScaleX->value() != initialImageScaleX;
	const bool scaleYChanged = imageScaleY->value() != initialImageScaleY;
	const bool offsetXChanged = imageOffsetX->value() != initialImageOffsetX;
	const bool offsetYChanged = imageOffsetY->value() != initialImageOffsetY;
	const bool cropChanged = !fitToFrame->isChecked()
		&& (scaleXChanged || scaleYChanged || offsetXChanged || offsetYChanged);
	if (!geometryChanged && !fittingChanged && !cropChanged && anchor == item->anchorPosition()
		&& replacementPath.isEmpty())
		return;
	UndoTransaction transaction;
	if (UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction();
	if (!replacementPath.isEmpty() && !item->relinkImage(replacementPath, false))
	{
		ScMessageBox::warning(&dialog, tr("Replace Anchored Image"), tr("The selected image could not be loaded. The existing image was kept."));
		if (transaction)
			transaction.cancel();
		return;
	}
	if (geometryChanged)
	{
		item->setWidthHeight(newWidth, newHeight);
		item->gWidth = newWidth;
		item->gHeight = newHeight;
		item->updateClip();
	}
	if (fittingChanged)
		item->setImageScalingMode(!fitToFrame->isChecked(), item->keepAspectRatio());
	if (fitToFrame->isChecked())
		item->adjustPictScale();
	else if (cropChanged)
	{
		const double currentXRes = item->pixm.imgInfo.xres > 0.0 ? item->pixm.imgInfo.xres : 72.0;
		const double currentYRes = item->pixm.imgInfo.yres > 0.0 ? item->pixm.imgInfo.yres : 72.0;
		const double appliedScaleX = scaleXChanged ? imageScaleX->value() / 100.0 * 72.0 / currentXRes : item->imageXScale();
		const double appliedScaleY = scaleYChanged ? imageScaleY->value() / 100.0 * 72.0 / currentYRes : item->imageYScale();
		item->setImageXYScale(appliedScaleX, appliedScaleY);
		item->setImageXYOffset((scaleXChanged || offsetXChanged) ? imageOffsetX->getValue(SC_PT) / appliedScaleX : item->imageXOffset(),
			(scaleYChanged || offsetYChanged) ? imageOffsetY->getValue(SC_PT) / appliedScaleY : item->imageYOffset());
	}
	item->setAnchorPosition(anchor);
	if (transaction)
		transaction.commit();
	m_doc->invalidateAll();
	m_doc->changed();
	m_doc->changedPagePreview();
	item->update();
	m_scMW->view->DrawNew();
	updateItemList();
}

void InlinePalette::handleDoubleClick(QListWidgetItem *item)
{
	if (item)
		emit startEdit(item->data(Qt::UserRole).toInt());
}

void InlinePalette::handleDeleteItem()
{
	m_doc->removeInlineFrame(actItem);
	QListWidgetItem* item = InlineViewWidget->takeItem(InlineViewWidget->currentRow());
	delete item;
	InlineViewWidget->update();
}

void InlinePalette::editingStart(int itemID)
{
	currentEditedItem = itemID;
	for (int a = 0; a < InlineViewWidget->count(); a++)
	{
		QListWidgetItem* item = InlineViewWidget->item(a);
		if (item)
			item->setFlags(Qt::NoItemFlags);
	}
}

void InlinePalette::editingFinished()
{
	updateItemList();
	currentEditedItem = -1;
}

void InlinePalette::setMainWindow(ScribusMainWindow *mw)
{
	m_scMW = mw;
	if (m_scMW == nullptr)
	{
		InlineViewWidget->clear();
		disconnect(m_scMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
		return;
	}
	connect(m_scMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)), Qt::UniqueConnection);
}

void InlinePalette::setDoc(ScribusDoc *newDoc)
{
	if (m_scMW == nullptr)
		m_doc = nullptr;
	else
		m_doc = newDoc;
	if (m_doc == nullptr)
	{
		InlineViewWidget->clear();
		setEnabled(true);
	}
	else
	{
		setEnabled(!m_doc->drawAsPreview);
		updateItemList();
	}
}

void InlinePalette::unsetDoc()
{
	m_doc = nullptr;
	InlineViewWidget->clear();
	setEnabled(true);
}

void InlinePalette::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqInlinePalUpdate)
		updateItemList();
}

void InlinePalette::updateItemList()
{
	InlineViewWidget->clear();
	InlineViewWidget->setWordWrap(true);
	if (!m_doc)
		return;
	for (auto it = m_doc->FrameItems.cbegin(); it != m_doc->FrameItems.cend(); ++it)
	{
		PageItem *currItem = it.value();
		QPixmap pm = QPixmap::fromImage(currItem->DrawObj_toImage(48));
		QPixmap pm2(50, 50);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill"));
		p.setBrush(b);
		p.drawRect(0, 0, 50, 50);
		p.drawPixmap(25 - pm.width() / 2, 25 - pm.height() / 2, pm);
		p.end();
		QString displayName = currItem->itemName();
		if (currItem->anchorPosition().mode == AnchorPosition::Mode::AboveLine)
			displayName += tr(" — Above Line");
		else if (currItem->anchorPosition().mode == AnchorPosition::Mode::Custom)
			displayName += tr(" — Anchored");
		QListWidgetItem *item = new QListWidgetItem(pm2, displayName, InlineViewWidget);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
		item->setData(Qt::UserRole, currItem->inlineCharID);
	}
}

void InlinePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		DockPanelBase::changeEvent(e);
}

void InlinePalette::languageChange()
{
	setWindowTitle( tr( "Inline Items" ) );
}
