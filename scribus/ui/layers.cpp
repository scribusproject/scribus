/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/****************************************************************************
**
**
** Created: Son Jun 2 11:23:14 2002
**      by:  Franz Schmid
**
**
****************************************************************************/

#include "layers.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QSignalBlocker>
#include <QSpacerItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

#include "iconmanager.h"
#include "sclayer.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"
#include "ui/scrspinbox.h"
#include "undomanager.h"

LayerPalette::LayerPalette(QWidget* parent) : ScDockPalette(parent, "Layers", Qt::WindowFlags()), m_Doc(nullptr)
{
	setObjectName(QString::fromLocal8Bit("Layers"));
	setMinimumSize( QSize(220, 240) );
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	containerWidget = new QWidget(this);
	LayerPaletteLayout = new QVBoxLayout();
	LayerPaletteLayout->setContentsMargins(3, 3, 3, 3);
	LayerPaletteLayout->setSpacing(3);

	layout1 = new QHBoxLayout;
	layout1->setContentsMargins(0, 0, 0, 0);
	layout1->setSpacing(3);
	textLabel1 = new QLabel( this);
	layout1->addWidget( textLabel1 );
	blendMode = new QComboBox( this);
	layout1->addWidget( blendMode );
	textLabel2 = new QLabel( this);
	textLabel2->setText( tr( "Opacity:" ) );
	layout1->addWidget( textLabel2 );
	opacitySpinBox = new ScrSpinBox( this);
	opacitySpinBox->setMinimum(0);
	opacitySpinBox->setMaximum(100);
	opacitySpinBox->setSingleStep(10);
	opacitySpinBox->setDecimals(0);
	opacitySpinBox->setSuffix( tr(" %"));
	opacitySpinBox->setFocusPolicy(Qt::ClickFocus);
	layout1->addWidget( opacitySpinBox );
	LayerPaletteLayout->addLayout( layout1 );

	IconManager& im = IconManager::instance();
	Table = new QTableWidget(0, 8, this );
	Table->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
	Table->setHorizontalHeaderItem(1, new QTableWidgetItem(im.loadIcon("16/show-object.png"), ""));
	Table->setHorizontalHeaderItem(2, new QTableWidgetItem(im.loadIcon("16/document-print.png"), ""));
	Table->setHorizontalHeaderItem(3, new QTableWidgetItem(im.loadIcon("16/lock.png"), ""));
	Table->setHorizontalHeaderItem(4, new QTableWidgetItem(im.loadIcon("16/layer-flow-around.png"), ""));
	Table->setHorizontalHeaderItem(5, new QTableWidgetItem(im.loadIcon("layer-outline.png"), ""));
	Table->setHorizontalHeaderItem(6, new QTableWidgetItem(im.loadIcon("16/pointer.png"), ""));
	Table->setHorizontalHeaderItem(7, new QTableWidgetItem( tr("Name")));

	QHeaderView *header = Table->horizontalHeader();
	header->setStretchLastSection(true);
	header->setSectionsClickable(false );
	header->setSectionsMovable( false );
	header->setSectionResizeMode(QHeaderView::Fixed);
	header->setHighlightSections(false);
	header->setMaximumSectionSize(24);

	Table->setColumnWidth(0, 24);
	Table->setColumnWidth(1, 24);
	Table->setColumnWidth(2, 24);
	Table->setColumnWidth(3, 24);
	Table->setColumnWidth(4, 24);
	Table->setColumnWidth(5, 24);
	Table->setColumnWidth(6, 24);
	Table->setSortingEnabled(false);
	Table->setSelectionBehavior( QAbstractItemView::SelectRows );
	QHeaderView *Header = Table->verticalHeader();
	Header->setSectionsMovable( false );
	Header->setSectionResizeMode(QHeaderView::Fixed);
	Header->hide();
	LayerPaletteLayout->addWidget( Table );

	Layout1 = new QHBoxLayout;
	Layout1->setContentsMargins(0, 0, 0, 0);
	Layout1->setSpacing(3);
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );

	newLayerButton = new QToolButton( this );
	newLayerButton->setIconSize(QSize(16, 16));
	newLayerButton->setMinimumSize( QSize( 48, 0 ) );
	newLayerButton->setMaximumSize( QSize( 48, 16777215 ) );
	newLayerButton->setText( "" );
	newLayerButton->setIcon(IconManager::instance().loadIcon("16/list-add.png"));
	Layout1->addWidget( newLayerButton );

	deleteLayerButton = new QToolButton( this );
	deleteLayerButton->setIconSize(QSize(16, 16));
	deleteLayerButton->setMinimumSize( QSize( 48, 0 ) );
	deleteLayerButton->setMaximumSize( QSize( 48, 16777215 ) );
	deleteLayerButton->setText( "" );
	deleteLayerButton->setIcon(IconManager::instance().loadIcon("16/list-remove.png"));
	Layout1->addWidget( deleteLayerButton );
	
	duplicateLayerButton = new QToolButton( this );
	duplicateLayerButton->setIconSize(QSize(16, 16));
	duplicateLayerButton->setMinimumSize( QSize( 48, 0 ) );
	duplicateLayerButton->setMaximumSize( QSize( 48, 16777215 ) );
	duplicateLayerButton->setText( "" );
	duplicateLayerButton->setIcon(IconManager::instance().loadIcon("16/edit-copy.png"));
	Layout1->addWidget( duplicateLayerButton );

	raiseLayerButton = new QToolButton( this );
	raiseLayerButton->setIconSize(QSize(16, 16));
	raiseLayerButton->setMinimumSize( QSize( 48, 0 ) );
	raiseLayerButton->setMaximumSize( QSize( 48, 16777215 ) );
	raiseLayerButton->setText( "" );
	raiseLayerButton->setIcon(IconManager::instance().loadIcon("16/go-up.png"));
	Layout1->addWidget( raiseLayerButton );

	lowerLayerButton = new QToolButton( this );
	lowerLayerButton->setIconSize(QSize(16, 16));
	lowerLayerButton->setMinimumSize( QSize( 48, 0 ) );
	lowerLayerButton->setMaximumSize( QSize( 48, 16777215 ) );
	lowerLayerButton->setText( "" );
	lowerLayerButton->setIcon(IconManager::instance().loadIcon("16/go-down.png"));
	Layout1->addWidget( lowerLayerButton );

	LayerPaletteLayout->addLayout( Layout1 );
	containerWidget->setLayout( LayerPaletteLayout );
	setWidget( containerWidget );
	clearContent();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect(newLayerButton, SIGNAL(clicked()), this, SLOT(addLayer()));
	connect(duplicateLayerButton, SIGNAL(clicked()), this, SLOT(dupLayer()));
	connect(deleteLayerButton, SIGNAL(clicked()), this, SLOT(removeLayer()));
	connect(raiseLayerButton, SIGNAL(clicked()), this, SLOT(upLayer()));
	connect(lowerLayerButton, SIGNAL(clicked()), this, SLOT(downLayer()));
	connect(Table, SIGNAL(cellChanged(int,int)), this, SLOT(changeName(int,int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	connect(header, SIGNAL(sectionClicked(int)), this, SLOT(toggleAllfromHeader(int)));
}

void LayerPalette::installEventFilter(QObject *obj)
{
	ScDockPalette::installEventFilter(obj);
	Table->installEventFilter(obj);
}

void LayerPalette::clearContent()
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(cellClicked(int,int)), this, SLOT(setActiveLayer(int,int)));
	disconnect(Table, SIGNAL(cellChanged(int,int)), this, SLOT(changeName(int,int)));
	Table->clearContents();
	Table->setRowCount(0);
	m_Doc = nullptr;
	newLayerButton->setEnabled(false);
	deleteLayerButton->setEnabled(false);
	raiseLayerButton->setEnabled(false);
	lowerLayerButton->setEnabled(false);
}

void LayerPalette::setDoc(ScribusDoc* doc)
{
	m_Doc = doc;
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(cellClicked(int,int)), this, SLOT(setActiveLayer(int,int)));
	disconnect(Table, SIGNAL(cellChanged(int,int)), this, SLOT(changeName(int,int)));
	if (!m_Doc)
	{
		layers = nullptr;
		newLayerButton->setEnabled(false);
		duplicateLayerButton->setEnabled(false);
		deleteLayerButton->setEnabled(false);
		raiseLayerButton->setEnabled(false);
		lowerLayerButton->setEnabled(false);
		markActiveLayer(0);
	}
	layers = &m_Doc->Layers;
	rebuildList();

	markActiveLayer(m_Doc->activeLayer());
	newLayerButton->setEnabled(true);
	duplicateLayerButton->setEnabled(true);

	connect(Table, SIGNAL(cellChanged(int,int)), this, SLOT(changeName(int,int)));
	connect(Table, SIGNAL(cellClicked(int,int)), this, SLOT(setActiveLayer(int,int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::rebuildList()
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(cellClicked(int,int)), this, SLOT(setActiveLayer(int,int)));
	disconnect(Table, SIGNAL(cellChanged(int,int)), this, SLOT(changeName(int,int)));
	QString tmp;
	int layerCount = m_Doc->layerCount();
	Table->setRowCount(layerCount);
	for (auto it = layers->begin(); it != layers->end(); ++it)
	{
		int layerID = it->ID;
		//TODO once "layers" is not set anymore, need to get layer number differently
		int layerLevel = m_Doc->layerLevelFromID(layerID);
		tmp = tmp.setNum(layerLevel);
		int row = layerCount-layerLevel - 1;
		QToolButton *pb = new QToolButton(this);
		pb->setObjectName(tmp);
		pb->setAutoRaise(true);
		pb->setText( "" );
		QPixmap pm(20,15);
		pm.fill(m_Doc->layerMarker(layerID));
		pb->setIcon(pm);
		Table->setCellWidget(row, 0, pb);
		connect(pb, SIGNAL(clicked()), this, SLOT(markLayer()));

		QWidget *cw1 = new QWidget(Table);
		QCheckBox *cp1 = new QCheckBox(cw1);
		cp1->setObjectName(tmp);
		QHBoxLayout *cl1 = new QHBoxLayout(cw1);
		cl1->addWidget(cp1);
		cl1->setAlignment(Qt::AlignCenter);
		cl1->setContentsMargins(0, 0, 0, 0);
		Table->setCellWidget(row, 1, cw1);
		cp1->setChecked(m_Doc->layerVisible(layerID));
		connect(cp1, SIGNAL(clicked()), this, SLOT(visibleLayer()));

		QWidget *cw2 = new QWidget(Table);
		QCheckBox *cp2 = new QCheckBox(cw2);
		cp2->setObjectName(tmp);
		cp2->setChecked(m_Doc->layerPrintable(layerID));
		QHBoxLayout *cl2 = new QHBoxLayout(cw2);
		cl2->addWidget(cp2);
		cl2->setAlignment(Qt::AlignCenter);
		cl2->setContentsMargins(0, 0, 0, 0);
		Table->setCellWidget(row, 2, cw2);
		connect(cp2, SIGNAL(clicked()), this, SLOT(printLayer()));

		QWidget *cw3 = new QWidget(Table);
		QCheckBox *cp3 = new QCheckBox(cw3);
		cp3->setObjectName(tmp);
		cp3->setChecked(m_Doc->layerLocked(layerID));
		QHBoxLayout *cl3 = new QHBoxLayout(cw3);
		cl3->addWidget(cp3);
		cl3->setAlignment(Qt::AlignCenter);
		cl3->setContentsMargins(0, 0, 0, 0);
		Table->setCellWidget(row, 3, cw3);
		connect(cp3, SIGNAL(clicked()), this, SLOT(lockLayer()));

		QWidget *cw4 = new QWidget(Table);
		QCheckBox *cp4 = new QCheckBox(cw4);
		cp4->setObjectName(tmp);
		cp4->setChecked(m_Doc->layerFlow(layerID));
		QHBoxLayout *cl4 = new QHBoxLayout(cw4);
		cl4->addWidget(cp4);
		cl4->setAlignment(Qt::AlignCenter);
		cl4->setContentsMargins(0, 0, 0, 0);
		Table->setCellWidget(row, 4, cw4);
		connect(cp4, SIGNAL(clicked()), this, SLOT(flowToggleLayer()));

		QWidget *cw5 = new QWidget(Table);
		QCheckBox *cp5 = new QCheckBox(cw5);
		cp5->setObjectName(tmp);
		cp5->setChecked(m_Doc->layerOutline(layerID));
		QHBoxLayout *cl5 = new QHBoxLayout(cw5);
		cl5->addWidget(cp5);
		cl5->setAlignment(Qt::AlignCenter);
		cl5->setContentsMargins(0, 0, 0, 0);
		Table->setCellWidget(row, 5, cw5);
		connect(cp5, SIGNAL(clicked()), this, SLOT(outlineToggleLayer()));

		QWidget *cw6 = new QWidget(Table);
		QCheckBox *cp6 = new QCheckBox(cw6);
		cp6->setObjectName(tmp);
		cp6->setChecked(m_Doc->layerSelectable(layerID));
		QHBoxLayout *cl6 = new QHBoxLayout(cw6);
		cl6->addWidget(cp6);
		cl6->setAlignment(Qt::AlignCenter);
		cl6->setContentsMargins(0, 0, 0, 0);
		Table->setCellWidget(row, 6, cw6);
		connect(cp6, SIGNAL(clicked()), this, SLOT(selectToggleLayer()));

		Table->setItem(row, 7, new QTableWidgetItem(m_Doc->layerName(layerID)));
	}
	connect(Table, SIGNAL(cellChanged(int,int)), this, SLOT(changeName(int,int)));
	connect(Table, SIGNAL(cellClicked(int,int)), this, SLOT(setActiveLayer(int,int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::addLayer()
{
	if (!m_Doc)
		return;
	m_Doc->addLayer(QString(), true);
	rebuildList();
	markActiveLayer();
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
}

void LayerPalette::dupLayer()
{
	if (!m_Doc)
		return;
	UndoTransaction copyTransaction;
	if (UndoManager::undoEnabled())
		copyTransaction = UndoManager::instance()->beginTransaction("", Um::ILayerAction, Um::DuplicateLayer.arg(m_Doc->activeLayerName()) , "", Um::ICreate);
	
	int current = m_Doc->activeLayer();
	
	m_Doc->addLayer(QString(), true);
	rebuildList();
	markActiveLayer();
	m_Doc->copyLayer(current, m_Doc->activeLayer());
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
	if (copyTransaction)
		copyTransaction.commit();
}

void LayerPalette::removeLayer()
{
	if (!m_Doc)
		return;
	int layerCount = m_Doc->layerCount();
	if (layerCount < 2)
		return;
	int level = layerCount-1-Table->currentRow();
	int layerID = m_Doc->layerIDFromLevel(level);
	bool delToo = false;
	if (m_Doc->layerContainsItems(layerID))
	{
		int scmReturn = ScMessageBox::warning(this, tr("Delete Layer"),
									tr("Do you want to delete all objects on this layer too?"),
									QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
									QMessageBox::No,	// GUI default
									QMessageBox::Yes);	// batch default
		if (scmReturn == QMessageBox::Cancel)
			return;
		delToo = (scmReturn != QMessageBox::No);
	}

	if (!m_Doc->deleteLayer(layerID, delToo))
		return;

	rebuildList();
	markActiveLayer();
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
}

void LayerPalette::upLayer()
{
	if (!m_Doc)
		return;
	int layerCount = m_Doc->layerCount();
	if ((layerCount < 2) || (Table->currentRow() == 0))
		return;
	int layerLevel = layerCount-1-Table->currentRow();
	m_Doc->raiseLayerByLevel(layerLevel);
	rebuildList();
	markActiveLayer();
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
	emit LayerChanged();
	m_Doc->changed();
}

void LayerPalette::downLayer()
{
	if (!m_Doc)
		return;
	int layerCount = m_Doc->layerCount();
	if ((layerCount < 2) || (Table->currentRow() == static_cast<int>(layerCount) - 1))
		return;
	int layerLevel = layerCount-1-Table->currentRow();
	m_Doc->lowerLayerByLevel(layerLevel);
	rebuildList();
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
	emit LayerChanged();
	markActiveLayer();
	m_Doc->changed();
}

void LayerPalette::changeName(int row, int col)
{
	if (!m_Doc)
		return;
	if (col == 7)
	{
		int layerLevel = m_Doc->layerCount()-1-row;
		int layerID = m_Doc->layerIDFromLevel(layerLevel);
		if (layerID != -1)
			m_Doc->changeLayerName(layerID, Table->item(row, col)->text());
	}
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
}

void LayerPalette::visibleLayer()
{
	if (!m_Doc)
		return;
	int level = sender()->objectName().toInt();
	int layerID = m_Doc->layerIDFromLevel(level);
	if (layerID == -1)
		return;
	const QObject* senderBox = sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		bool isLayerVisible = ((const QCheckBox*)(senderBox))->isChecked();
		m_Doc->setLayerVisible(layerID, isLayerVisible);
		if (!isLayerVisible)
			m_Doc->m_Selection->removeItemsOfLayer(layerID);
		setActiveLayer(Table->currentRow(), -1);
		emit LayerChanged();
	}
}

void LayerPalette::printLayer()
{
	if (!m_Doc)
		return;
	int level = sender()->objectName().toInt();
	int layerID = m_Doc->layerIDFromLevel(level);
	if (layerID == -1)
		return;
	const QObject* senderBox = sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		m_Doc->setLayerPrintable(layerID, ((const QCheckBox*)(senderBox))->isChecked());
		setActiveLayer(Table->currentRow(), -1);
		emit LayerChanged();
	}
}

void LayerPalette::lockLayer()
{
	if (!m_Doc)
		return;
	int level = sender()->objectName().toInt();
	int layerID = m_Doc->layerIDFromLevel(level);
	if (layerID == -1)
		return;
	const QObject* senderBox = sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		bool isLayerLocked = ((const QCheckBox*)(senderBox))->isChecked();
		m_Doc->setLayerLocked(layerID, isLayerLocked);
		if (isLayerLocked)
			m_Doc->m_Selection->removeItemsOfLayer(layerID);
		deleteLayerButton->setEnabled(!isLayerLocked);
		setActiveLayer(Table->currentRow(), -1);
	}
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
}

void LayerPalette::flowToggleLayer()
{
	if (!m_Doc)
		return;
	int level = sender()->objectName().toInt();
	int layerID = m_Doc->layerIDFromLevel(level);
	if (layerID == -1)
		return;
	const QObject* senderBox = sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		m_Doc->setLayerFlow(layerID, ((const QCheckBox*)(senderBox))->isChecked());
		emit LayerChanged();
		setActiveLayer(Table->currentRow(), -1);
	}
}

void LayerPalette::outlineToggleLayer()
{
	if (!m_Doc)
		return;
	int level = sender()->objectName().toInt();
	int layerID = m_Doc->layerIDFromLevel(level);
	if (layerID == -1)
		return;
	const QObject* senderBox = sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		m_Doc->setLayerOutline(layerID, ((const QCheckBox*)(senderBox))->isChecked());
		emit LayerChanged();
		setActiveLayer(Table->currentRow(), -1);
	}
}

void LayerPalette::selectToggleLayer()
{
	if (!m_Doc)
		return;
	int level = sender()->objectName().toInt();
	int layerID = m_Doc->layerIDFromLevel(level);
	if (layerID == -1)
		return;
	const QObject* senderBox = sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		bool isLayerSelectable = ((const QCheckBox*)(senderBox))->isChecked();
		m_Doc->setLayerSelectable(layerID, isLayerSelectable);
		emit LayerChanged();
		setActiveLayer(Table->currentRow(), -1);
	}
}

void LayerPalette::toggleAllfromHeader(int index)
{
	if (!m_Doc)
		return;

	int layerCount = m_Doc->layerCount();
	for (auto it = layers->begin(); it != layers->end(); ++it)
	{
		int row = layerCount - m_Doc->layerLevelFromID(it->ID) - 1;
		if (index == 1)
		{
			(dynamic_cast<QCheckBox*>(Table->cellWidget(row, 1)))->toggle();
			it->isViewable = ((QCheckBox*)(Table->cellWidget(row, 1)))->isChecked();
		}
		if (index == 2)
		{
			(dynamic_cast<QCheckBox*>(Table->cellWidget(row, 2)))->toggle();
			it->isPrintable = ((QCheckBox*)(Table->cellWidget(row, 2)))->isChecked();
		}
		if (index == 3)
		{
			(dynamic_cast<QCheckBox*>(Table->cellWidget(row, 3)))->toggle();
			it->isEditable = !((QCheckBox*)(Table->cellWidget(row, 3)))->isChecked();
		}
		if (index == 4)
		{
			(dynamic_cast<QCheckBox*>(Table->cellWidget(row, 4)))->toggle();
			it->flowControl = ((QCheckBox*)(Table->cellWidget(row, 4)))->isChecked();
		}
		else if (index == 5)
		{
			(dynamic_cast<QCheckBox*>(Table->cellWidget(row, 5)))->toggle();
			it->outlineMode = ((QCheckBox*)(Table->cellWidget(row, 5)))->isChecked();
		}
	}
	emit LayerChanged();
}

void LayerPalette::markLayer()
{
	if (!m_Doc)
		return;
	int level = sender()->objectName().toInt();
	int layerID = m_Doc->layerIDFromLevel(level);
	if (layerID == -1)
		return;
	const QObject* senderBox = sender();
	if (strcmp(senderBox->metaObject()->className(), "QToolButton") == 0)
	{
		QColor newColor(QColorDialog::getColor(m_Doc->layerMarker(layerID), this));
		if (newColor.isValid())
		{
			QPixmap pm(20,15);
			pm.fill(newColor);
			((QToolButton*)(senderBox))->setIcon(pm);
			m_Doc->setLayerMarker(layerID, newColor);
			m_Doc->scMW()->updateLayerMenu();
			m_Doc->scMW()->rebuildLayersList();
			emit LayerChanged();
		}
		setActiveLayer(Table->currentRow(), -1);
	}
}

void LayerPalette::changeOpacity()
{
	if (!m_Doc)
		return;
	m_Doc->setLayerTransparency(m_Doc->activeLayer(), opacitySpinBox->value() / 100.0);
	emit LayerChanged();
}

void LayerPalette::changeBlendMode(int blend)
{
	if (!m_Doc)
		return;
	m_Doc->setLayerBlendMode(m_Doc->activeLayer(), blend);
	emit LayerChanged();
}

void LayerPalette::markActiveLayer(int layerID)
{
	if (!m_Doc)
		return;
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(cellClicked(int,int)), this, SLOT(setActiveLayer(int,int)));
	int layerToMark = layerID;
	if (layerID == -1)
		layerToMark = m_Doc->activeLayer();
	Table->setCurrentCell(m_Doc->layerCount()-1-m_Doc->layerLevelFromID(layerToMark), 6);
	opacitySpinBox->setValue(qRound(m_Doc->layerTransparency(layerToMark) * 100));
	blendMode->setCurrentIndex(m_Doc->layerBlendMode(layerToMark));
	deleteLayerButton->setEnabled(m_Doc->layerCount() > 1 && !m_Doc->layerLocked( m_Doc->activeLayer() ));
		
	if (layers->count() > 1)
	{
		raiseLayerButton->setEnabled(Table->currentRow() != 0);
		lowerLayerButton->setEnabled(Table->currentRow() != Table->rowCount() - 1);
	}
	else
	{
		raiseLayerButton->setEnabled(false);
		lowerLayerButton->setEnabled(false);
	}
	connect(Table, SIGNAL(cellClicked(int,int)), this, SLOT(setActiveLayer(int,int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::setActiveLayer(int row, int col)
{
	if (!m_Doc)
		return;
	if (col != 7)
	{
		markActiveLayer(-1);
		return;
	}
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	int layerID = m_Doc->layerIDFromLevel(m_Doc->layerCount() - 1 - row);
	bool found = m_Doc->setActiveLayer(layerID);
	if (found)
	{
		m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
		opacitySpinBox->setValue(qRound(m_Doc->layerTransparency(m_Doc->activeLayer()) * 100));
		blendMode->setCurrentIndex(m_Doc->layerBlendMode(m_Doc->activeLayer()));
		deleteLayerButton->setEnabled(m_Doc->layerCount() > 1 && !m_Doc->layerLocked( m_Doc->activeLayer() ));
		if (layers->count() > 1)
		{
			raiseLayerButton->setEnabled(Table->currentRow()!= 0);
			lowerLayerButton->setEnabled(Table->currentRow() != Table->rowCount() - 1);
		}
		else
		{
			raiseLayerButton->setEnabled(false);
			lowerLayerButton->setEnabled(false);
		}
	}
	connect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		ScDockPalette::changeEvent(e);
}

void LayerPalette::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();

	Table->horizontalHeaderItem(1)->setIcon(iconManager.loadIcon("16/show-object.png"));
	Table->horizontalHeaderItem(2)->setIcon(iconManager.loadIcon("16/document-print.png"));
	Table->horizontalHeaderItem(3)->setIcon(iconManager.loadIcon("16/lock.png"));
	Table->horizontalHeaderItem(4)->setIcon(iconManager.loadIcon("16/layer-flow-around.png"));
	Table->horizontalHeaderItem(5)->setIcon(iconManager.loadIcon("layer-outline.png"));
	Table->horizontalHeaderItem(6)->setIcon(iconManager.loadIcon("16/pointer.png"));

	newLayerButton->setIcon(iconManager.loadIcon("16/list-add.png"));
	deleteLayerButton->setIcon(iconManager.loadIcon("16/list-remove.png"));
	duplicateLayerButton->setIcon(iconManager.loadIcon("16/edit-copy.png"));
	raiseLayerButton->setIcon(iconManager.loadIcon("16/go-up.png"));
	lowerLayerButton->setIcon(iconManager.loadIcon("16/go-down.png"));
}

void LayerPalette::languageChange()
{
	setWindowTitle( tr( "Layers" ) );
	textLabel1->setText( tr( "Blend Mode:" ) );

	int  oldBlendMode = blendMode->currentIndex();
	QSignalBlocker signalBlocker(blendMode);
	blendMode->clear();
	blendMode->addItem( tr("Normal"));
	blendMode->addItem( tr("Darken"));
	blendMode->addItem( tr("Lighten"));
	blendMode->addItem( tr("Multiply"));
	blendMode->addItem( tr("Screen"));
	blendMode->addItem( tr("Overlay"));
	blendMode->addItem( tr("Hard Light"));
	blendMode->addItem( tr("Soft Light"));
	blendMode->addItem( tr("Difference"));
	blendMode->addItem( tr("Exclusion"));
	blendMode->addItem( tr("Color Dodge"));
	blendMode->addItem( tr("Color Burn"));
	blendMode->addItem( tr("Hue"));
	blendMode->addItem( tr("Saturation"));
	blendMode->addItem( tr("Color"));
	blendMode->addItem( tr("Luminosity"));

	blendMode->setCurrentIndex(oldBlendMode);

	textLabel2->setText( tr( "Opacity:" ) );
	opacitySpinBox->setSuffix( tr(" %"));
	Table->horizontalHeaderItem(7)->setText( tr("Name"));
	newLayerButton->setToolTip("");
	duplicateLayerButton->setToolTip("");
	deleteLayerButton->setToolTip("");
	raiseLayerButton->setToolTip("");
	lowerLayerButton->setToolTip("");
	blendMode->setToolTip("");
	opacitySpinBox->setToolTip("");
	newLayerButton->setToolTip( tr( "Add a New Layer" ) );
	duplicateLayerButton->setToolTip( tr( "Duplicate the Current Layer" ) );
	deleteLayerButton->setToolTip( tr( "Delete Layer" ) );
	raiseLayerButton->setToolTip( tr( "Raise Layer" ) );
	lowerLayerButton->setToolTip( tr( "Lower Layer" ) );
	blendMode->setToolTip( tr("Selects the Blendmode, works only in PDF 1.4"));
	opacitySpinBox->setToolTip( tr("Layer Transparency, works only in PDF 1.4 and SVG"));
	Table->horizontalHeaderItem(0)->setToolTip("<qt>" + tr("Color of the Layer Indicator. Each layer has a color assigned to display on the canvas when layer indicators are enabled. You can double click to edit the color.") + "</qt>");
	Table->horizontalHeaderItem(1)->setToolTip("<qt>" + tr("Make Layer Visible. Uncheck to hide the layer from the display.") + "</qt>" );
	Table->horizontalHeaderItem(2)->setToolTip("<qt>" + tr("Print Layer. Uncheck to disable printing.") + "</qt>" );
	Table->horizontalHeaderItem(3)->setToolTip("<qt>" + tr("Lock or Unlock Layer. Unchecked is unlocked.") + "</qt>" );
	Table->horizontalHeaderItem(4)->setToolTip("<qt>" + tr("Make text in lower layers flow around objects. Enabling this forces text in lower layers to flow around objects of the layer for which this option has been enabled.") + "</qt>" );
	Table->horizontalHeaderItem(5)->setToolTip("<qt>" + tr("Outline Mode. Toggles the 'wireframe' display of objects to speed the display of very complex objects.") + "</qt>" ) ;
	Table->horizontalHeaderItem(6)->setToolTip("<qt>" + tr("Select Objects on Layer. Check to enable selecting.") + "</qt>" );
	Table->horizontalHeaderItem(7)->setToolTip("<qt>" + tr("Name of the Layer. Double click on the name of a layer to edit the name.") + "</qt>" );
}

