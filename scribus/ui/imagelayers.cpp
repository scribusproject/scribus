/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "imagelayers.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QTabWidget>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QToolTip>
#include <QPainter>
#include <QWidget>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>

#include "pageitem.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "iconmanager.h"
#include "ui/propertiespalette.h"
#include "util.h"

ImageLayers::ImageLayers(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	blendMode->clear();
	blendMode->addItem( tr("Normal"), "norm");
	blendMode->addItem( tr("Darken"), "dark");
	blendMode->addItem( tr("Lighten"), "lite");
	blendMode->addItem( tr("Hue"), "hue ");
	blendMode->addItem( tr("Saturation"), "sat ");
	blendMode->addItem( tr("Color"), "colr");
	blendMode->addItem( tr("Luminosity"), "lum ");
	blendMode->addItem( tr("Multiply"), "mul ");
	blendMode->addItem( tr("Screen"), "scrn");
	blendMode->addItem( tr("Dissolve"), "diss");
	blendMode->addItem( tr("Overlay"), "over");
	blendMode->addItem( tr("Hard Light"), "hLit");
	blendMode->addItem( tr("Soft Light"), "sLit");
	blendMode->addItem( tr("Difference"), "diff");
	blendMode->addItem( tr("Exclusion"), "smud");
	blendMode->addItem( tr("Color Dodge"), "div ");
	blendMode->addItem( tr("Color Burn"), "idiv");
	blendMode->addItem( tr("Plus"), "plus");
	blendMode->addItem( tr("Destination In"), "dsti");
	blendMode->addItem( tr("Destination Out"), "dsto");

	opacitySpinBox->setMinimum(0);
	opacitySpinBox->setDecimals(0);
	opacitySpinBox->setMaximum(100);
	opacitySpinBox->setSingleStep(10);
	opacitySpinBox->setSuffix( tr(" %"));

	blendMode->setCurrentIndex(0);

	layerTable->setColumnCount(3);
	layerTable->setHorizontalHeaderItem(0, new QTableWidgetItem(IconManager::instance().loadIcon("16/show-object.png"), ""));
	layerTable->setHorizontalHeaderItem(1, new QTableWidgetItem(IconManager::instance().loadIcon("mask"), ""));
	layerTable->setHorizontalHeaderItem(2, new QTableWidgetItem( tr("Name")));
	layerTable->setColumnWidth(0, 64);
	layerTable->setColumnWidth(1, 64);
	QHeaderView* headerH = layerTable->horizontalHeader();
	headerH->setStretchLastSection(true);
	headerH->setSectionsClickable(false );
	headerH->setSectionsMovable( false );
	headerH->setSectionResizeMode(QHeaderView::Fixed);
	layerTable->setSortingEnabled(false);
	layerTable->setSelectionBehavior(QTableWidget::SelectRows);
	QHeaderView *Header = layerTable->verticalHeader();
	Header->setSectionsMovable( false );
	Header->setSectionResizeMode(QHeaderView::Fixed);
	Header->hide();

}

void ImageLayers::setCurrentItem(PageItem *item, ScribusView *view )
{
	bool sameItem = (m_item == item);

	m_view  = view;
	m_item = item;

	disconnect(layerTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectLayer()));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLayerInfo()));
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(updateLayerInfo()));

	if (!m_item || !m_item->pixm.imgInfo.valid || m_item->pixm.imgInfo.layerInfo.count() == 0)
	{
		currentLayer = -1;
		opacitySpinBox->setEnabled(false);
		blendMode->setEnabled(false);
		blendMode->setCurrentIndex(0);
		layerTable->clearContents();
		layerTable->clearSelection();
		originalRequestProps.clear();
		return;
	}

	ImageInfoRecord *info = &m_item->pixm.imgInfo;
	QList<PSDLayer>::iterator it;
	uint counter = 0;

	if (!sameItem || originalRequestProps.count() == 0)
	{
		layerTable->clearSelection();
		originalRequestProps = info->RequestProps;
		currentLayer = -1;
		layerTable->setRowCount(info->layerInfo.count());

		for (it = info->layerInfo.begin(); it != info->layerInfo.end(); ++it)
		{
			addListItem(it, counter, info);
			counter++;
		}

		selectLayer();
	}
	// Update list only if there are changes
	else if (sameItem && info->RequestProps != originalRequestProps/* && originalRequestProps.count() != 0*/)
	{
		originalRequestProps = info->RequestProps;

		for (it = info->layerInfo.begin(); it != info->layerInfo.end(); ++it)
		{
			updateListItem(it, counter, info);
			counter++;
		}

		selectLayer();
	}

	connect(layerTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectLayer()));
	connect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLayerInfo()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(updateLayerInfo()));

}

void ImageLayers::selectLayer()
{
	QSignalBlocker sigOpacity(opacitySpinBox);
	QSignalBlocker sigBlendMode(blendMode);

	QModelIndexList selectedRows = layerTable->selectionModel()->selectedRows();
	if (selectedRows.count() <= 0)
	{
		currentLayer = -1;
		opacitySpinBox->setEnabled(false);
		opacitySpinBox->setValue(100);
		blendMode->setEnabled(false);
		blendMode->setCurrentIndex(0);
		return;
	}

	int selectedRow = selectedRows.at(0).row();
	currentLayer = layerTable->rowCount() - selectedRow - 1;

	ImageInfoRecord *info = &m_item->pixm.imgInfo;

	if ((info->isRequest) && (info->RequestProps.contains(currentLayer)))
	{
		opacitySpinBox->setValue(qRound(info->RequestProps[currentLayer].opacity / 255.0 * 100));
		setCurrentComboItemFromData(blendMode, info->RequestProps[currentLayer].blend);
	}
	else
	{
		opacitySpinBox->setValue(qRound(info->layerInfo[currentLayer].opacity / 255.0 * 100));
		setCurrentComboItemFromData(blendMode, info->layerInfo[currentLayer].blend);
	}
	opacitySpinBox->setEnabled(true);
	blendMode->setEnabled(true);
}

void ImageLayers::addListItem(QList<PSDLayer>::iterator it, int layerID, ImageInfoRecord *info)
{
	QPalette palette;
	palette.setColor(backgroundRole(), Qt::white);

	// Layer
	QCheckBox *cp = new QCheckBox();
	cp->setPalette(palette);
	QPixmap pm;
	pm = QPixmap::fromImage(it->thumb);
	cp->setIcon(pm);
	layerTable->setCellWidget(tableRow(layerID), 0, cp);
	if ((info->isRequest) && (info->RequestProps.contains(layerID)))
		cp->setChecked(info->RequestProps[layerID].visible);
	else
		cp->setChecked(!(it->flags & 2));

	connect(cp, SIGNAL(toggled(bool)), this, SLOT(updateLayerInfo()));

	// Layer Mask
	if (!it->thumb_mask.isNull())
	{
		QCheckBox *cp2 = new QCheckBox();
		cp2->setPalette(palette);
		QPixmap pm2;
		pm2 = QPixmap::fromImage(it->thumb_mask);
		cp2->setIcon(pm2);
		layerTable->setCellWidget(tableRow(layerID), 1, cp2);

		if ((info->isRequest) && (info->RequestProps.contains(layerID)))
			cp2->setChecked(info->RequestProps[layerID].useMask);
		else
			cp2->setChecked(true);

		connect(cp2, SIGNAL(toggled(bool)), this, SLOT(updateLayerInfo()));
	}
	else
		layerTable->setCellWidget(tableRow(layerID), 1, nullptr);

	// Layer Name
	QTableWidgetItem *tW = new QTableWidgetItem(it->layerName);
	tW->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	layerTable->setItem(tableRow(layerID), 2, tW);
	layerTable->setRowHeight(tableRow(layerID), 40);
}

void ImageLayers::updateListItem(QList<PSDLayer>::iterator it, int layerID, ImageInfoRecord *info)
{
	// layer
	QCheckBox *cbLayer = dynamic_cast<QCheckBox*>(layerTable->cellWidget(tableRow(layerID), 0));
	if (cbLayer)
	{
		QSignalBlocker sigLayer(cbLayer);
		if ((info->isRequest) && (info->RequestProps.contains(layerID)))
			cbLayer->setChecked(info->RequestProps[layerID].visible);
		else
			cbLayer->setChecked(!(it->flags & 2));
	}

	// Mask
	if (!it->thumb_mask.isNull())
	{
		QCheckBox *cbMask = dynamic_cast<QCheckBox*>(layerTable->cellWidget(tableRow(layerID), 1));

		if (!cbMask)
			return;

		QSignalBlocker sigMask(cbMask);
		if ((info->isRequest) && (info->RequestProps.contains(layerID)))
			cbMask->setChecked(info->RequestProps[layerID].useMask);
		else
			cbMask->setChecked(true);
	}
}

void ImageLayers::updateLayerInfo()
{
	struct ImageLoadRequest loadingInfo;
	bool isRequest = m_item->pixm.imgInfo.isRequest;
	for (int r = 0; r < layerTable->rowCount(); ++r)
	{
		int layerIndex = tableRow(r);

		if (currentLayer == layerIndex)
		{
			loadingInfo.blend = blendMode->currentData().toString();
			loadingInfo.opacity = qRound(opacitySpinBox->value() / 100.0 * 255);
		}
		else if ((isRequest) && (m_item->pixm.imgInfo.RequestProps.contains(layerIndex)))
		{
			loadingInfo.blend = m_item->pixm.imgInfo.RequestProps[layerIndex].blend;
			loadingInfo.opacity = m_item->pixm.imgInfo.RequestProps[layerIndex].opacity;
		}
		else
		{
			loadingInfo.blend = m_item->pixm.imgInfo.layerInfo[layerIndex].blend;
			loadingInfo.opacity = m_item->pixm.imgInfo.layerInfo[layerIndex].opacity;
		}

		QCheckBox *cbLayer = dynamic_cast<QCheckBox*>(layerTable->cellWidget(r, 0));
		QCheckBox *cbMask = dynamic_cast<QCheckBox*>(layerTable->cellWidget(r, 1));
		loadingInfo.visible = (cbLayer) ? cbLayer->checkState() : true;
		loadingInfo.useMask = (cbMask) ? cbMask->checkState() : true;

		m_item->pixm.imgInfo.RequestProps.insert(layerIndex, loadingInfo);
	}
	m_item->pixm.imgInfo.isRequest = true;

	m_view->m_doc->loadPict(m_item->Pfile, m_item, true);
	m_item->update();
}

