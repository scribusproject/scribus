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
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qvaluelist.h>
#include <qtooltip.h>
#include <qcheckbox.h>

#include "scmessagebox.h"
#include "scribus.h"

#include "layers.h"
#include "layers.moc"

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
			if (newCont == text(a, 3))
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

	Table = new LayerTable( this );
	Table->setNumRows( 0 );
	Table->setNumCols( 4 );
	QHeader *header = Table->horizontalHeader();
	header->setLabel(0, loadIcon("Layervisible.xpm"), "");
	header->setLabel(1, loadIcon("DateiPrint16.png"), "");
	header->setLabel(2, loadIcon("locked.png"), "");
	Table->setColumnReadOnly(0, true);
	Table->setColumnReadOnly(1, true);
	Table->setColumnReadOnly(2, true);
	Table->setColumnWidth(0, 24);
	Table->setColumnWidth(1, 24);
	Table->setColumnWidth(2, 24);
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

}

void LayerPalette::updateName(int r)
{
	changeName(r, 3);
	ScMW->changeLayer(ScMW->doc->activeLayer());
}

void LayerPalette::ClearInhalt()
{
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	int b = Table->numRows();
	for (int a = 0; a < b; ++a)
		Table->removeRow(0);
	flagsPrintable.clear();
	flagsVisible.clear();
	newLayerButton->setEnabled(false);
	deleteLayerButton->setEnabled(false);
	raiseLayerButton->setEnabled(false);
	lowerLayerButton->setEnabled(false);
}

void LayerPalette::setLayers(QValueList<Layer> *layin, int act)
{
	layers = layin;
	rebuildList();
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	markActiveLayer(act);
	newLayerButton->setEnabled(true);
	deleteLayerButton->setEnabled(true);
	raiseLayerButton->setEnabled(true);
	lowerLayerButton->setEnabled(true);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
}

void LayerPalette::rebuildList()
{
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	flagsPrintable.clear();
	flagsVisible.clear();
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
		Table->setText(row, 3, ScMW->doc->layerName(layerNumber));
		QCheckBox *cp = new QCheckBox(this, tmp.setNum(layerLevel));
		cp->setChecked(ScMW->doc->layerPrintable(layerNumber));
		Table->setCellWidget(row, 1, cp);
		flagsPrintable.append(cp);
		connect(cp, SIGNAL(clicked()), this, SLOT(printLayer()));
		QCheckBox *cp2 = new QCheckBox(this, tmp.setNum(layerLevel));
		cp2->setChecked(ScMW->doc->layerVisible(layerNumber));
		flagsVisible.append(cp2);
		connect(cp2, SIGNAL(clicked()), this, SLOT(visibleLayer()));
		Table->setCellWidget(row, 0, cp2);
		QCheckBox *cp3 = new QCheckBox(this, tmp.setNum(layerLevel));
		cp3->setChecked(ScMW->doc->layerLocked(layerNumber));
		flagsLocked.append(cp3);
		connect(cp3, SIGNAL(clicked()), this, SLOT(lockLayer()));
		Table->setCellWidget(row, 2, cp3);
		Header->setLabel(row, tmp.setNum(layerLevel));
	}
	Table->setColumnStretchable(3, true);
	Table->adjustColumn(3);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
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
	if (col == 3)
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

void LayerPalette::markActiveLayer(int layerNumber)
{
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	int layerToMark=layerNumber;
	if (layerNumber==-1)
		layerToMark=ScMW->doc->activeLayer();
	Table->setCurrentCell(ScMW->doc->layerCount()-1-ScMW->doc->layerLevelFromNumber(layerToMark), 2);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
}

void LayerPalette::setActiveLayer(int row)
{
	int layerNumber=ScMW->doc->layerNumberFromLevel(ScMW->doc->layerCount()-1-row);
	bool found=ScMW->doc->setActiveLayer(layerNumber);
	if (found)
		ScMW->changeLayer(ScMW->doc->activeLayer());
}

void LayerPalette::languageChange()
{
	setCaption( tr( "Layers" ) );
	Table->horizontalHeader()->setLabel(3, tr("Name"));
	QToolTip::remove( newLayerButton );
	QToolTip::remove( deleteLayerButton );
	QToolTip::remove( raiseLayerButton );
	QToolTip::remove( lowerLayerButton );
	QToolTip::add( newLayerButton, tr( "Add a new layer" ) );
	QToolTip::add( deleteLayerButton, tr( "Delete layer" ) );
	QToolTip::add( raiseLayerButton, tr( "Raise layer" ) );
	QToolTip::add( lowerLayerButton, tr( "Lower layer" ) );
}

