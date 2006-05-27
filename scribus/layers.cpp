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
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qvaluelist.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qcolordialog.h>

#include "scmessagebox.h"
#include "sccombobox.h"
#include "scribus.h"

#include "layers.h"
#include "layers.moc"
#ifdef HAVE_CAIRO
#include <cairo.h>
#endif

extern QPixmap loadIcon(QString nam);



LayerTable::LayerTable(QWidget* parent) : QTable(parent)
{
}

void LayerTable::endEdit ( int row, int col, bool accept, bool replace )
{
	QTable::EditMode ed = editMode();
	QString oldCont = text(row, col);
	QTable::endEdit(row, col, accept, replace);
	QString newCont = item(row, col)->text();
	bool realAccept = true;
	int b = numRows();
	for (int a = 0; a < b; ++a)
	{
		if (a != row)
		{
			if (newCont == text(a, 6))
				realAccept = false;
		}
	}
	if (newCont.isEmpty())
		realAccept = false;
	if (realAccept)
	{
		if (ed != QTable::NotEditing)
			emit updtName(row);
	}
	else
	{
		setText(row, col, oldCont);
		emit updtName(row);
	}
}

LayerPalette::LayerPalette(QWidget* parent)
		: ScrPaletteBase( parent, "Layers", false, 0 )
{
	setIcon(loadIcon("AppIcon.png"));
	LayerPaletteLayout = new QVBoxLayout( this, 10, 5, "LayerPaletteLayout");

	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");
	textLabel1 = new QLabel( this, "textLabel1" );
	layout1->addWidget( textLabel1 );
	blendMode = new ScComboBox( false, this, "blendMode" );
	layout1->addWidget( blendMode );
	textLabel2 = new QLabel( this, "textLabel2" );
	textLabel2->setText( tr( "Opacity:" ) );
	layout1->addWidget( textLabel2 );
	opacitySpinBox = new QSpinBox( this, "opacitySpinBox" );
	opacitySpinBox->setMinValue(0);
	opacitySpinBox->setMaxValue(100);
	opacitySpinBox->setLineStep(10);
	opacitySpinBox->setSuffix( tr(" %"));
	opacitySpinBox->setFocusPolicy(QWidget::ClickFocus);
	layout1->addWidget( opacitySpinBox );
	LayerPaletteLayout->addLayout( layout1 );
#ifndef HAVE_CAIRO
	blendMode->hide();
	textLabel1->hide();
	textLabel2->hide();
	opacitySpinBox->hide();
#else
#if CAIRO_VERSION < CAIRO_VERSION_ENCODE(1, 1, 8)
	blendMode->hide();
	textLabel1->hide();
#endif
#endif

	Table = new LayerTable( this );
	Table->setNumRows( 0 );
	Table->setNumCols( 7 );
	QHeader *header = Table->horizontalHeader();
	header->setLabel(0, "");
	header->setLabel(1, loadIcon("16/show-object.png"), "");
	header->setLabel(2, loadIcon("16/document-print.png"), "");
	header->setLabel(3, loadIcon("16/lock.png"), "");
	header->setLabel(4, loadIcon("16/layer-flow-around.png"), "");
	header->setLabel(5, loadIcon("layer-outline.png"), "");
	Table->setColumnReadOnly(0, true);
	Table->setColumnReadOnly(1, true);
	Table->setColumnReadOnly(2, true);
	Table->setColumnReadOnly(3, true);
	Table->setColumnReadOnly(4, true);
	Table->setColumnReadOnly(5, true);
	Table->setColumnWidth(0, 24);
	Table->setColumnWidth(1, 24);
	Table->setColumnWidth(2, 24);
	Table->setColumnWidth(3, 24);
	Table->setColumnWidth(4, 24);
	Table->setColumnWidth(5, 24);
	header->setResizeEnabled(false, 0);
	header->setResizeEnabled(false, 1);
	header->setResizeEnabled(false, 2);
	header->setResizeEnabled(false, 3);
	header->setResizeEnabled(false, 4);
	header->setResizeEnabled(false, 5);
	header->setResizeEnabled(true, 6);
	Table->setRowMovingEnabled(false);
	Table->setSorting(false);
	Table->setSelectionMode( QTable::SingleRow );
	Table->setFocusStyle( QTable::FollowStyle );
	Header = Table->verticalHeader();
	Header->setMovingEnabled(false);
	Header->setResizeEnabled(false);
	LayerPaletteLayout->addWidget( Table );

	Layout1 = new QHBoxLayout( 0, 0, 0, "Layout1");
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );

	newLayerButton = new QPushButton( this, "newLayerButton" );
	newLayerButton->setMinimumSize( QSize( 50, 24 ) );
	newLayerButton->setMaximumSize( QSize( 50, 24 ) );
	newLayerButton->setText( "" );
	newLayerButton->setPixmap(loadIcon("Newlayer.png"));
	Layout1->addWidget( newLayerButton );

	deleteLayerButton = new QPushButton( this, "deleteLayerButton" );
	deleteLayerButton->setMinimumSize( QSize( 50, 24 ) );
	deleteLayerButton->setMaximumSize( QSize( 50, 24 ) );
	deleteLayerButton->setText( "" );
	deleteLayerButton->setPixmap(loadIcon("Deletelayer.png"));
	Layout1->addWidget( deleteLayerButton );

	raiseLayerButton = new QPushButton( this, "raiseLayerButton" );
	raiseLayerButton->setMinimumSize( QSize( 50, 24 ) );
	raiseLayerButton->setMaximumSize( QSize( 50, 24 ) );
	raiseLayerButton->setText( "" );
	raiseLayerButton->setPixmap(loadIcon("Raiselayer.png"));
	Layout1->addWidget( raiseLayerButton );

	lowerLayerButton = new QPushButton( this, "lowerLayerButton" );
	lowerLayerButton->setMinimumSize( QSize( 50, 24 ) );
	lowerLayerButton->setMaximumSize( QSize( 50, 24 ) );
	lowerLayerButton->setText( "" );
	lowerLayerButton->setPixmap(loadIcon("Lowerlayer.png"));
	Layout1->addWidget( lowerLayerButton );

	LayerPaletteLayout->addLayout( Layout1 );
	ClearInhalt();
	languageChange();
	
	connect(newLayerButton, SIGNAL(clicked()), this, SLOT(addLayer()));
	connect(deleteLayerButton, SIGNAL(clicked()), this, SLOT(removeLayer()));
	connect(raiseLayerButton, SIGNAL(clicked()), this, SLOT(upLayer()));
	connect(lowerLayerButton, SIGNAL(clicked()), this, SLOT(downLayer()));
//	connect(Table, SIGNAL(valueChanged(int, int)), this, SLOT(changeName(int, int)));
	connect(Table, SIGNAL(updtName(int)), this, SLOT(updateName(int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));

}

void LayerPalette::updateName(int r)
{
	changeName(r, 6);
	ScMW->changeLayer(ScMW->doc->activeLayer());
}

void LayerPalette::ClearInhalt()
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	int b = Table->numRows();
	for (int a = 0; a < b; ++a)
		Table->removeRow(0);
	flagsPrintable.clear();
	flagsVisible.clear();
	flagsFlow.clear();
	flagsOutline.clear();
	flagsMarker.clear();
	newLayerButton->setEnabled(false);
	deleteLayerButton->setEnabled(false);
	raiseLayerButton->setEnabled(false);
	lowerLayerButton->setEnabled(false);
}

void LayerPalette::setLayers(QValueList<Layer> *layin, int act)
{
	layers = layin;
	rebuildList();
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	markActiveLayer(act);
	newLayerButton->setEnabled(true);
	deleteLayerButton->setEnabled(true);
	raiseLayerButton->setEnabled(true);
	lowerLayerButton->setEnabled(true);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::rebuildList()
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	flagsPrintable.clear();
	flagsVisible.clear();
	flagsFlow.clear();
	flagsOutline.clear();
	flagsMarker.clear();
	QString tmp;
	QValueList<Layer>::iterator it;
	int layerCount=ScMW->doc->layerCount();
	Table->setNumRows(layerCount);
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		int layerNumber=(*it).LNr;
		//TODO once "layers" is not set anymore, need to get layer number differently
		int layerLevel=ScMW->doc->layerLevelFromNumber(layerNumber);
		int row=layerCount-layerLevel-1;
		Table->setText(row, 6, ScMW->doc->layerName(layerNumber));
		QToolButton *pb = new QToolButton(this, tmp.setNum(layerLevel));
		pb->setAutoRaise(true);
		pb->setText( "" );
		QPixmap pm(20,15);
		pm.fill(ScMW->doc->layerMarker(layerNumber));
		QIconSet ic;
		ic.setPixmap(pm, QIconSet::Small, QIconSet::Normal);
		ic.setPixmap(pm, QIconSet::Small, QIconSet::Active);
		pb->setIconSet(ic);
		Table->setCellWidget(row, 0, pb);
		flagsMarker.append(pb);
		connect(pb, SIGNAL(clicked()), this, SLOT(markLayer()));
		QCheckBox *cp2 = new QCheckBox(this, tmp.setNum(layerLevel));
		cp2->setChecked(ScMW->doc->layerVisible(layerNumber));
		flagsVisible.append(cp2);
		connect(cp2, SIGNAL(clicked()), this, SLOT(visibleLayer()));
		Table->setCellWidget(row, 1, cp2);
		QCheckBox *cp = new QCheckBox(this, tmp.setNum(layerLevel));
		cp->setChecked(ScMW->doc->layerPrintable(layerNumber));
		Table->setCellWidget(row, 2, cp);
		flagsPrintable.append(cp);
		connect(cp, SIGNAL(clicked()), this, SLOT(printLayer()));
		QCheckBox *cp3 = new QCheckBox(this, tmp.setNum(layerLevel));
		cp3->setChecked(ScMW->doc->layerLocked(layerNumber));
		flagsLocked.append(cp3);
		connect(cp3, SIGNAL(clicked()), this, SLOT(lockLayer()));
		Table->setCellWidget(row, 3, cp3);
		QCheckBox *cp4 = new QCheckBox(this, tmp.setNum(layerLevel));
		cp4->setChecked(ScMW->doc->layerFlow(layerNumber));
		flagsFlow.append(cp4);
		connect(cp4, SIGNAL(clicked()), this, SLOT(flowToggleLayer()));
		Table->setCellWidget(row, 4, cp4);
		QCheckBox *cp5 = new QCheckBox(this, tmp.setNum(layerLevel));
		cp5->setChecked(ScMW->doc->layerOutline(layerNumber));
		flagsOutline.append(cp5);
		connect(cp5, SIGNAL(clicked()), this, SLOT(outlineToggleLayer()));
		Table->setCellWidget(row, 5, cp5);
		Header->setLabel(row, tmp.setNum(layerLevel));
	}
	Table->setColumnStretchable(6, true);
	Table->adjustColumn(6);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::addLayer()
{
	ScMW->doc->addLayer(QString::null, true);
	rebuildList();
	markActiveLayer();
	ScMW->changeLayer(ScMW->doc->activeLayer());
	ScMW->slotDocCh();
}

void LayerPalette::removeLayer()
{
	int layerCount=ScMW->doc->layerCount();
	if (layerCount < 2)
		return;
	int level = layerCount-1-Table->currentRow();
	int layerNumber=ScMW->doc->layerNumberFromLevel(level);
	bool delToo = false;
	if (ScMW->doc->layerContainsItems(layerNumber))
	{
		int scmReturn=ScMessageBox::warning(this, tr("Delete Layer"),
									tr("Do you want to delete all objects on this layer too?"),
									QMessageBox::Yes,
									QMessageBox::No,
									QMessageBox::Cancel | QMessageBox::Default | QMessageBox::Escape);
		if (scmReturn == QMessageBox::Cancel)
			return;
		if (scmReturn == QMessageBox::Yes)
			delToo = true;
	}

	if (!ScMW->doc->deleteLayer(layerNumber, delToo))
		return;
	
	rebuildList();
	markActiveLayer();
	ScMW->changeLayer(ScMW->doc->activeLayer());
	ScMW->slotDocCh();
}

void LayerPalette::upLayer()
{
	int layerCount=ScMW->doc->layerCount();
	if ((layerCount < 2) || (Table->currentRow() == 0))
		return;
	int layerLevel = layerCount-1-Table->currentRow();
	ScMW->doc->raiseLayerByLevel(layerLevel);
	rebuildList();
	markActiveLayer();
	ScMW->changeLayer(ScMW->doc->activeLayer());
	emit LayerChanged();
	ScMW->slotDocCh();
}

void LayerPalette::downLayer()
{
	int layerCount=ScMW->doc->layerCount();
	if ((layerCount < 2) || (Table->currentRow() == static_cast<int>(layerCount) - 1))
		return;
	int layerLevel = layerCount-1-Table->currentRow();
	ScMW->doc->lowerLayerByLevel(layerLevel);
	rebuildList();
	ScMW->changeLayer(ScMW->doc->activeLayer());
	emit LayerChanged();
	markActiveLayer();
	ScMW->slotDocCh();
}

void LayerPalette::changeName(int row, int col)
{
	if (col == 6)
	{
		int layerLevel = ScMW->doc->layerCount()-1-row;
		int layerNumber=ScMW->doc->layerNumberFromLevel(layerLevel);
		if (layerNumber!=-1)
		{
			if (ScMW->doc->changeLayerName(layerNumber, Table->text(row, col)))
				ScMW->slotDocCh();
		}
	}
}

void LayerPalette::visibleLayer()
{
	int level = QString(sender()->name()).toInt();
	int layerNumber=ScMW->doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (senderBox->isA("QCheckBox"))
	{
		ScMW->doc->setLayerVisible(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		emit LayerChanged();
		ScMW->slotDocCh();
	}
}

void LayerPalette::printLayer()
{
	int level = QString(sender()->name()).toInt();
	int layerNumber=ScMW->doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (senderBox->isA("QCheckBox"))
	{
		ScMW->doc->setLayerPrintable(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		ScMW->slotDocCh();
	}
}

void LayerPalette::lockLayer()
{
	int level = QString(sender()->name()).toInt();
	int layerNumber=ScMW->doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (senderBox->isA("QCheckBox"))
	{
		ScMW->doc->setLayerLocked(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		ScMW->slotDocCh();
	}
}

void LayerPalette::flowToggleLayer()
{
	int level = QString(sender()->name()).toInt();
	int layerNumber=ScMW->doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (senderBox->isA("QCheckBox"))
	{
		ScMW->doc->setLayerFlow(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		emit LayerChanged();
		ScMW->slotDocCh();
	}
}

void LayerPalette::outlineToggleLayer()
{
	int level = QString(sender()->name()).toInt();
	int layerNumber=ScMW->doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (senderBox->isA("QCheckBox"))
	{
		ScMW->doc->setLayerOutline(layerNumber,((QCheckBox*)(senderBox))->isChecked());
		emit LayerChanged();
		ScMW->slotDocCh();
	}
}

void LayerPalette::markLayer()
{
	int level = QString(sender()->name()).toInt();
	int layerNumber=ScMW->doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	const QObject* senderBox=sender();
	if (senderBox->isA("QToolButton"))
	{
		QColor neu = QColor();
		neu = QColorDialog::getColor(ScMW->doc->layerMarker(layerNumber), this);
		QPixmap pm(20,15);
		pm.fill(neu);
		QIconSet ic;
		ic.setPixmap(pm, QIconSet::Small, QIconSet::Normal);
		ic.setPixmap(pm, QIconSet::Small, QIconSet::Active);
		((QToolButton*)(senderBox))->setIconSet(ic);
		ScMW->doc->setLayerMarker(layerNumber,neu);
		emit LayerChanged();
		ScMW->slotDocCh();
	}
}

void LayerPalette::changeOpacity()
{
	ScMW->doc->setLayerTransparency(ScMW->doc->activeLayer(), opacitySpinBox->value() / 100.0);
	emit LayerChanged();
	ScMW->slotDocCh();
}

void LayerPalette::changeBlendMode(int blend)
{
	ScMW->doc->setLayerBlendMode(ScMW->doc->activeLayer(), blend);
	emit LayerChanged();
	ScMW->slotDocCh();
}

void LayerPalette::markActiveLayer(int layerNumber)
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	int layerToMark=layerNumber;
	if (layerNumber==-1)
		layerToMark=ScMW->doc->activeLayer();
	Table->setCurrentCell(ScMW->doc->layerCount()-1-ScMW->doc->layerLevelFromNumber(layerToMark), 6);
	opacitySpinBox->setValue(qRound(ScMW->doc->layerTransparency(layerToMark) * 100));
	blendMode->setCurrentItem(ScMW->doc->layerBlendMode(layerToMark));
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::setActiveLayer(int row)
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	int layerNumber=ScMW->doc->layerNumberFromLevel(ScMW->doc->layerCount()-1-row);
	bool found=ScMW->doc->setActiveLayer(layerNumber);
	if (found)
	{
		ScMW->changeLayer(ScMW->doc->activeLayer());
		opacitySpinBox->setValue(qRound(ScMW->doc->layerTransparency(ScMW->doc->activeLayer()) * 100));
		blendMode->setCurrentItem(ScMW->doc->layerBlendMode(ScMW->doc->activeLayer()));
	}
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOpacity()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void LayerPalette::languageChange()
{
	setCaption( tr( "Layers" ) );
	textLabel1->setText( tr( "Blend Mode:" ) );
	blendMode->clear();
	blendMode->insertItem( tr("Normal"));
	blendMode->insertItem( tr("Darken"));
	blendMode->insertItem( tr("Lighten"));
	blendMode->insertItem( tr("Multiply"));
	blendMode->insertItem( tr("Screen"));
	blendMode->insertItem( tr("Overlay"));
	blendMode->insertItem( tr("Hard Light"));
	blendMode->insertItem( tr("Soft Light"));
	blendMode->insertItem( tr("Difference"));
	blendMode->insertItem( tr("Exlusion"));
	blendMode->insertItem( tr("Color Dodge"));
	blendMode->insertItem( tr("Color Burn"));
	blendMode->insertItem( tr("Overprint"));
	textLabel2->setText( tr( "Opacity:" ) );
	opacitySpinBox->setSuffix( tr(" %"));
	Table->horizontalHeader()->setLabel(6, tr("Name"));
	QToolTip::remove( newLayerButton );
	QToolTip::remove( deleteLayerButton );
	QToolTip::remove( raiseLayerButton );
	QToolTip::remove( lowerLayerButton );
	QToolTip::add( newLayerButton, tr( "Add a new layer" ) );
	QToolTip::add( deleteLayerButton, tr( "Delete layer" ) );
	QToolTip::add( raiseLayerButton, tr( "Raise layer" ) );
	QToolTip::add( lowerLayerButton, tr( "Lower layer" ) );
}

