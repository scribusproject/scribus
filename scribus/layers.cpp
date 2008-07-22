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
#include <QEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

#include "sccombobox.h"
#include "scribus.h"
#include "undomanager.h"
#include "util_icon.h"

LayerPalette::LayerPalette(QWidget* parent) : ScrPaletteBase( parent, "Layers", false, 0 ), m_Doc(0)
{
	LayerPaletteLayout = new QVBoxLayout( this );
	LayerPaletteLayout->setMargin(2);
	LayerPaletteLayout->setSpacing(2);

	layout1 = new QHBoxLayout;
	layout1->setMargin(0);
	layout1->setSpacing(2);
	textLabel1 = new QLabel( this);
	layout1->addWidget( textLabel1 );
	blendMode = new ScComboBox( this);
	layout1->addWidget( blendMode );
	textLabel2 = new QLabel( this);
	textLabel2->setText( tr( "Opacity:" ) );
	layout1->addWidget( textLabel2 );
	opacitySpinBox = new QSpinBox( this);
	opacitySpinBox->setMinimum(0);
	opacitySpinBox->setMaximum(100);
	opacitySpinBox->setSingleStep(10);
	opacitySpinBox->setSuffix( tr(" %"));
	opacitySpinBox->setFocusPolicy(Qt::ClickFocus);
	layout1->addWidget( opacitySpinBox );
	LayerPaletteLayout->addLayout( layout1 );

	Table = new QTableWidget(0, 7, this );
	Table->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
	Table->setHorizontalHeaderItem(1, new QTableWidgetItem(QIcon(loadIcon("16/show-object.png")), ""));
	Table->setHorizontalHeaderItem(2, new QTableWidgetItem(QIcon(loadIcon("16/document-print.png")), ""));
	Table->setHorizontalHeaderItem(3, new QTableWidgetItem(QIcon(loadIcon("16/lock.png")), ""));
	Table->setHorizontalHeaderItem(4, new QTableWidgetItem(QIcon(loadIcon("16/layer-flow-around.png")), ""));
	Table->setHorizontalHeaderItem(5, new QTableWidgetItem(QIcon(loadIcon("layer-outline.png")), ""));
	Table->setHorizontalHeaderItem(6, new QTableWidgetItem( tr("Name")));

	QHeaderView *header = Table->horizontalHeader();
	header->setStretchLastSection(true);
	header->setMovable(false);
	header->setClickable(true);
	header->setHighlightSections(false);
	header->setResizeMode(QHeaderView::Fixed);

	Table->setColumnWidth(0, 24);
	Table->setColumnWidth(1, 24);
	Table->setColumnWidth(2, 24);
	Table->setColumnWidth(3, 24);
	Table->setColumnWidth(4, 24);
	Table->setColumnWidth(5, 24);
	Table->setSortingEnabled(false);
	Table->setSelectionBehavior( QAbstractItemView::SelectRows );
	QHeaderView *Header = Table->verticalHeader();
	Header->setMovable(false);
	Header->setResizeMode(QHeaderView::Fixed);
	Header->hide();
	LayerPaletteLayout->addWidget( Table );

	Layout1 = new QHBoxLayout;
	Layout1->setMargin(0);
	Layout1->setSpacing(0);
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );

	newLayerButton = new QPushButton( this );
	newLayerButton->setMinimumSize( QSize( 50, 24 ) );
	newLayerButton->setMaximumSize( QSize( 50, 24 ) );
	newLayerButton->setText( "" );
	newLayerButton->setIcon(QIcon(loadIcon("16/list-add.png")));
	Layout1->addWidget( newLayerButton );

	deleteLayerButton = new QPushButton( this );
	deleteLayerButton->setMinimumSize( QSize( 50, 24 ) );
	deleteLayerButton->setMaximumSize( QSize( 50, 24 ) );
	deleteLayerButton->setText( "" );
	deleteLayerButton->setIcon(QIcon(loadIcon("16/list-remove.png")));
	Layout1->addWidget( deleteLayerButton );
	
	duplicateLayerButton = new QPushButton( this );
	duplicateLayerButton->setMinimumSize( QSize( 50, 24 ) );
	duplicateLayerButton->setMaximumSize( QSize( 50, 24 ) );
	duplicateLayerButton->setText( "" );
	duplicateLayerButton->setIcon(QIcon(loadIcon("16/edit-copy.png")));
	Layout1->addWidget( duplicateLayerButton );

	raiseLayerButton = new QPushButton( this );
	raiseLayerButton->setMinimumSize( QSize( 50, 24 ) );
	raiseLayerButton->setMaximumSize( QSize( 50, 24 ) );
	raiseLayerButton->setText( "" );
	raiseLayerButton->setIcon(QIcon(loadIcon("16/go-up.png")));
	Layout1->addWidget( raiseLayerButton );

	lowerLayerButton = new QPushButton( this );
	lowerLayerButton->setMinimumSize( QSize( 50, 24 ) );
	lowerLayerButton->setMaximumSize( QSize( 50, 24 ) );
	lowerLayerButton->setText( "" );
	lowerLayerButton->setIcon(QIcon(loadIcon("16/go-down.png")));
	Layout1->addWidget( lowerLayerButton );

	LayerPaletteLayout->addLayout( Layout1 );
	ClearInhalt();
	languageChange();

	connect(newLayerButton, SIGNAL(clicked()), this, SLOT(addLayer()));
	connect(duplicateLayerButton, SIGNAL(clicked()), this, SLOT(dupLayer()));
	connect(deleteLayerButton, SIGNAL(clicked()), this, SLOT(removeLayer()));
	connect(raiseLayerButton, SIGNAL(clicked()), this, SLOT(upLayer()));
	connect(lowerLayerButton, SIGNAL(clicked()), this, SLOT(downLayer()));
	connect(Table, SIGNAL(cellChanged(int, int)), this, SLOT(changeName(int, int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	connect(header, SIGNAL(sectionClicked(int)), this, SLOT(toggleAllfromHeader(int)));
}

void LayerPalette::ClearInhalt()
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(cellActivated(int, int)), this, SLOT(setActiveLayer(int)));
	disconnect(Table, SIGNAL(cellChanged(int, int)), this, SLOT(changeName(int, int)));
	Table->clearContents();
	Table->setRowCount(0);
	m_Doc = NULL;
	newLayerButton->setEnabled(false);
	deleteLayerButton->setEnabled(false);
	raiseLayerButton->setEnabled(false);
	lowerLayerButton->setEnabled(false);
}

void LayerPalette::setDoc(ScribusDoc* doc)
{
	m_Doc=doc;
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(cellClicked(int, int)), this, SLOT(setActiveLayer(int)));
	disconnect(Table, SIGNAL(cellChanged(int, int)), this, SLOT(changeName(int, int)));
	if (!m_Doc)
	{
		layers=0;
		newLayerButton->setEnabled(false);
		duplicateLayerButton->setEnabled(false);
		deleteLayerButton->setEnabled(false);
		raiseLayerButton->setEnabled(false);
		lowerLayerButton->setEnabled(false);
		markActiveLayer(0);
	}
	layers=&m_Doc->Layers;
	rebuildList();

	markActiveLayer(m_Doc->activeLayer());
	newLayerButton->setEnabled(true);
	duplicateLayerButton->setEnabled(true);

	connect(Table, SIGNAL(cellChanged(int, int)), this, SLOT(changeName(int, int)));
	connect(Table, SIGNAL(cellClicked(int, int)), this, SLOT(setActiveLayer(int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::rebuildList()
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(cellClicked(int, int)), this, SLOT(setActiveLayer(int)));
	disconnect(Table, SIGNAL(cellChanged(int, int)), this, SLOT(changeName(int, int)));
	QString tmp;
	ScLayers::iterator it;
	int layerCount = m_Doc->layerCount();
	Table->setRowCount(layerCount);
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		int layerNumber=(*it).LNr;
		//TODO once "layers" is not set anymore, need to get layer number differently
		int layerLevel = m_Doc->layerLevelFromNumber(layerNumber);
		tmp = tmp.setNum(layerLevel);
		int row = layerCount-layerLevel-1;
		QToolButton *pb = new QToolButton(this);
		pb->setObjectName(tmp);
		pb->setAutoRaise(true);
		pb->setText( "" );
		QPixmap pm(20,15);
		pm.fill(m_Doc->layerMarker(layerNumber));
		pb->setIcon(pm);
		Table->setCellWidget(row, 0, pb);
		connect(pb, SIGNAL(clicked()), this, SLOT(markLayer()));
		QCheckBox *cp2 = new QCheckBox(this);
		cp2->setObjectName(tmp);
		connect(cp2, SIGNAL(clicked()), this, SLOT(visibleLayer()));
		Table->setCellWidget(row, 1, cp2);
		cp2->setChecked(m_Doc->layerVisible(layerNumber));
		QCheckBox *cp = new QCheckBox(this);
		cp->setObjectName(tmp);
		Table->setCellWidget(row, 2, cp);
		cp->setChecked(m_Doc->layerPrintable(layerNumber));
		connect(cp, SIGNAL(clicked()), this, SLOT(printLayer()));
		QCheckBox *cp3 = new QCheckBox(this);
		cp3->setObjectName(tmp);
		connect(cp3, SIGNAL(clicked()), this, SLOT(lockLayer()));
		Table->setCellWidget(row, 3, cp3);
		cp3->setChecked(m_Doc->layerLocked(layerNumber));
		QCheckBox *cp4 = new QCheckBox(this);
		cp4->setObjectName(tmp);
		connect(cp4, SIGNAL(clicked()), this, SLOT(flowToggleLayer()));
		Table->setCellWidget(row, 4, cp4);
		cp4->setChecked(m_Doc->layerFlow(layerNumber));
		QCheckBox *cp5 = new QCheckBox(this);
		cp5->setObjectName(tmp);
		connect(cp5, SIGNAL(clicked()), this, SLOT(outlineToggleLayer()));
		Table->setCellWidget(row, 5, cp5);
		cp5->setChecked(m_Doc->layerOutline(layerNumber));
		Table->setItem(row, 6, new QTableWidgetItem(m_Doc->layerName(layerNumber)));
	}
	connect(Table, SIGNAL(cellChanged(int, int)), this, SLOT(changeName(int, int)));
	connect(Table, SIGNAL(cellClicked(int, int)), this, SLOT(setActiveLayer(int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::addLayer()
{
	m_Doc->addLayer(QString::null, true);
	rebuildList();
	markActiveLayer();
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
	m_Doc->changed();
}

void LayerPalette::dupLayer()
{
	UndoTransaction copyTransaction( UndoManager::instance()->beginTransaction("", Um::ILayerAction, Um::DuplicateLayer.arg(m_Doc->activeLayerName()) , "", Um::ICreate) );
	
	int current = m_Doc->activeLayer();
	
	m_Doc->addLayer(QString::null, true);
	rebuildList();
	markActiveLayer();
	m_Doc->copyLayer(current, m_Doc->activeLayer());
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
	m_Doc->changed();
	
	copyTransaction.commit();
}

void LayerPalette::removeLayer()
{
	int layerCount=m_Doc->layerCount();
	if (layerCount < 2)
		return;
	int level = layerCount-1-Table->currentRow();
	int layerNumber=m_Doc->layerNumberFromLevel(level);
	bool delToo = false;
	if (m_Doc->layerContainsItems(layerNumber))
	{
		int scmReturn = QMessageBox::warning(this, tr("Delete Layer"),
									tr("Do you want to delete all objects on this layer too?"),
									QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
		if (scmReturn == QMessageBox::Cancel)
			return;
		delToo = (scmReturn != QMessageBox::No);
	}

	if (!m_Doc->deleteLayer(layerNumber, delToo))
		return;

	rebuildList();
	markActiveLayer();
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
	m_Doc->changed();
}

void LayerPalette::upLayer()
{
	int layerCount=m_Doc->layerCount();
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
	int layerCount=m_Doc->layerCount();
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
	if (col == 6)
	{
		int layerLevel = m_Doc->layerCount()-1-row;
		int layerNumber=m_Doc->layerNumberFromLevel(layerLevel);
		if (layerNumber!=-1)
			m_Doc->changeLayerName(layerNumber, Table->item(row, col)->text());
	}
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
}

void LayerPalette::visibleLayer()
{
	int level = QString(sender()->objectName()).toInt();
	int layerNumber=m_Doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		m_Doc->setLayerVisible(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		setActiveLayer(Table->currentRow());
		emit LayerChanged();
	}
}

void LayerPalette::printLayer()
{
	int level = QString(sender()->objectName()).toInt();
	int layerNumber=m_Doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		m_Doc->setLayerPrintable(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		setActiveLayer(Table->currentRow());
	}
}

void LayerPalette::lockLayer()
{
	int level = QString(sender()->objectName()).toInt();
	int layerNumber=m_Doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		m_Doc->setLayerLocked(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		deleteLayerButton->setEnabled(!((QCheckBox*)(senderBox))->isChecked());
		setActiveLayer(Table->currentRow());
	}
	m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
}

void LayerPalette::flowToggleLayer()
{
	int level = QString(sender()->objectName()).toInt();
	int layerNumber=m_Doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
		m_Doc->setLayerFlow(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		setActiveLayer(Table->currentRow());
}

void LayerPalette::outlineToggleLayer()
{
	int level = QString(sender()->objectName()).toInt();
	int layerNumber=m_Doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (strcmp(senderBox->metaObject()->className(), "QCheckBox") == 0)
	{
		m_Doc->setLayerOutline(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		emit LayerChanged();
		setActiveLayer(Table->currentRow());
	}
}

void LayerPalette::toggleAllfromHeader(int index)
{
	if (m_Doc)
	{
		ScLayers::iterator it;
		int layerCount = m_Doc->layerCount();
		for (it = layers->begin(); it != layers->end(); ++it)
		{
			int row = layerCount - m_Doc->layerLevelFromNumber((*it).LNr) - 1;
			if (index == 1)
			{
				((QCheckBox*)(Table->cellWidget(row, 1)))->toggle();
				(*it).isViewable = ((QCheckBox*)(Table->cellWidget(row, 1)))->isChecked();
			}
			if (index == 2)
			{
				((QCheckBox*)(Table->cellWidget(row, 2)))->toggle();
				(*it).isPrintable = ((QCheckBox*)(Table->cellWidget(row, 2)))->isChecked();
			}
			if (index == 3)
			{
				((QCheckBox*)(Table->cellWidget(row, 3)))->toggle();
				(*it).isEditable = ((QCheckBox*)(Table->cellWidget(row, 3)))->isChecked();
			}
			if (index == 4)
			{
				((QCheckBox*)(Table->cellWidget(row, 4)))->toggle();
				(*it).flowControl = ((QCheckBox*)(Table->cellWidget(row, 4)))->isChecked();
			}
			else if (index == 5)
			{
				((QCheckBox*)(Table->cellWidget(row, 5)))->toggle();
				(*it).outlineMode = ((QCheckBox*)(Table->cellWidget(row, 5)))->isChecked();
			}
		}
		emit LayerChanged();
	}
}

void LayerPalette::markLayer()
{
	int level = QString(sender()->objectName()).toInt();
	int layerNumber=m_Doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (strcmp(senderBox->metaObject()->className(), "QToolButton") == 0)
	{
		QColor neu = QColor();
		neu = QColorDialog::getColor(m_Doc->layerMarker(layerNumber), this);
		if (neu.isValid())
		{
			QPixmap pm(20,15);
			pm.fill(neu);
			((QToolButton*)(senderBox))->setIcon(pm);
			m_Doc->setLayerMarker(layerNumber,neu);
			emit LayerChanged();
		}
		setActiveLayer(Table->currentRow());
	}
}

void LayerPalette::changeOpacity()
{
	m_Doc->setLayerTransparency(m_Doc->activeLayer(), opacitySpinBox->value() / 100.0);
	emit LayerChanged();
}

void LayerPalette::changeBlendMode(int blend)
{
	m_Doc->setLayerBlendMode(m_Doc->activeLayer(), blend);
	emit LayerChanged();
}

void LayerPalette::markActiveLayer(int layerNumber)
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(cellClicked(int, int)), this, SLOT(setActiveLayer(int)));
	int layerToMark=layerNumber;
	if (layerNumber==-1)
		layerToMark=m_Doc->activeLayer();
	Table->setCurrentCell(m_Doc->layerCount()-1-m_Doc->layerLevelFromNumber(layerToMark), 6);
	opacitySpinBox->setValue(qRound(m_Doc->layerTransparency(layerToMark) * 100));
	blendMode->setCurrentIndex(m_Doc->layerBlendMode(layerToMark));
	deleteLayerButton->setEnabled(m_Doc->layerCount()>1 && !m_Doc->layerLocked( m_Doc->activeLayer() ));
		
	if (layers->count()>1)
	{
		raiseLayerButton->setEnabled(Table->currentRow()!=0);
		lowerLayerButton->setEnabled(Table->currentRow()!=Table->rowCount()-1);
	}
	else
	{
		raiseLayerButton->setEnabled(false);
		lowerLayerButton->setEnabled(false);
	}
	connect(Table, SIGNAL(cellClicked(int, int)), this, SLOT(setActiveLayer(int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::setActiveLayer(int row)
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	int layerNumber=m_Doc->layerNumberFromLevel(m_Doc->layerCount()-1-row);
	bool found=m_Doc->setActiveLayer(layerNumber);
	if (found)
	{
		m_Doc->scMW()->changeLayer(m_Doc->activeLayer());
		opacitySpinBox->setValue(qRound(m_Doc->layerTransparency(m_Doc->activeLayer()) * 100));
		blendMode->setCurrentIndex(m_Doc->layerBlendMode(m_Doc->activeLayer()));
		deleteLayerButton->setEnabled(m_Doc->layerCount()>1 && !m_Doc->layerLocked( m_Doc->activeLayer() ));
		if (layers->count()>1)
		{
			raiseLayerButton->setEnabled(Table->currentRow()!= 0);
			lowerLayerButton->setEnabled(Table->currentRow()!= Table->rowCount()-1);
		}
		else
		{
			raiseLayerButton->setEnabled(false);
			lowerLayerButton->setEnabled(false);
		}
	}
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void LayerPalette::languageChange()
{
	setWindowTitle( tr( "Layers" ) );
	textLabel1->setText( tr( "Blend Mode:" ) );
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
	textLabel2->setText( tr( "Opacity:" ) );
	opacitySpinBox->setSuffix( tr(" %"));
	Table->horizontalHeaderItem(6)->setText( tr("Name"));
	newLayerButton->setToolTip("");
	duplicateLayerButton->setToolTip("");
	deleteLayerButton->setToolTip("");
	raiseLayerButton->setToolTip("");
	lowerLayerButton->setToolTip("");
	newLayerButton->setToolTip( tr( "Add a new layer" ) );
	duplicateLayerButton->setToolTip( tr( "Duplicates the current layer" ) );
	deleteLayerButton->setToolTip( tr( "Delete layer" ) );
	raiseLayerButton->setToolTip( tr( "Raise layer" ) );
	lowerLayerButton->setToolTip( tr( "Lower layer" ) );
	Table->horizontalHeaderItem(0)->setToolTip("<qt>" + tr("Color of the Layer Indicator - Each layer has a color assigned to display on the canvas when layer indicators are enabled. You can double click to edit the color. ") + "</qt>");
	Table->horizontalHeaderItem(1)->setToolTip("<qt>" + tr("Make Layer Visible - Uncheck to hide the layer from the display ") + "</qt>" );
	Table->horizontalHeaderItem(2)->setToolTip("<qt>" + tr("Print Layer - Uncheck to disable printing. ") + "</qt>" );
	Table->horizontalHeaderItem(3)->setToolTip("<qt>" + tr("Lock or Unlock Layer - Unchecked is unlocked ") + "</qt>" );
	Table->horizontalHeaderItem(4)->setToolTip("<qt>" + tr("Text flows around objects in lower Layers - Enabling this forces text frames to flow around other objects, even in layers below") + "</qt>" );
	Table->horizontalHeaderItem(5)->setToolTip("<qt>" + tr("Outline Mode - Toggles the 'wireframe' display of objects to speed the display of very complex objects.") + "</qt>" ) ;
	Table->horizontalHeaderItem(6)->setToolTip("<qt>" + tr("Name of the Layer - Double clicking on the name of a layer enabled editing") + "</qt>" );
}


