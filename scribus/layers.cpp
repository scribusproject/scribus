/****************************************************************************
**
**
** Created: Son Jun 2 11:23:14 2002
**      by:  Franz Schmid
**
**
****************************************************************************/
#include <qmessagebox.h>
#include "layers.h"
#include "layers.moc"
#include "scribus.h"

extern QPixmap loadIcon(QString nam);
extern ScribusApp* ScApp;


LayerTable::LayerTable(QWidget* parent) : QTable(parent)
{}

void LayerTable::keyPressEvent(QKeyEvent *k)
{
	if (k->key() == Key_F10)
		emit ToggleAllPalettes();
	if (k->key() == Key_Escape)
		emit Schliessen();
	QTable::keyPressEvent(k);
}

void LayerTable::endEdit ( int row, int col, bool accept, bool replace )
{
	QTable::EditMode ed = editMode();
	QTable::endEdit(row, col, accept, replace);
	if (ed != QTable::NotEditing)
		emit updtName(row);
}

LayerPalette::LayerPalette(QWidget* parent)
		: QDialog( parent, "Layers", false, 0 )
{
	setIcon(loadIcon("AppIcon.png"));
	setCaption( tr( "Layers" ) );
	LayerPaletteLayout = new QVBoxLayout( this, 10, 5, "LayerPaletteLayout");

	Table = new LayerTable( this );
	Table->setNumRows( 0 );
	Table->setNumCols( 3 );
	Table->setTopMargin(0);
	Table->horizontalHeader()->hide();
	Table->setRowMovingEnabled(false);
	Table->setSorting(false);
	Table->setSelectionMode( QTable::SingleRow );
	Table->setFocusStyle( QTable::FollowStyle );
	Table->setColumnReadOnly(1, true);
	Table->setColumnReadOnly(2, true);
	Table->setColumnWidth(1, 55);
	Table->setColumnWidth(2, 55);
	Header = Table->verticalHeader();
	Header->setMovingEnabled(false);
	Header->setResizeEnabled(false);
	LayerPaletteLayout->addWidget( Table );

	Layout1 = new QHBoxLayout( 0, 0, 0, "Layout1");
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );

	NewLayer = new QPushButton( this, "NewLayer" );
	NewLayer->setMinimumSize( QSize( 50, 24 ) );
	NewLayer->setMaximumSize( QSize( 50, 24 ) );
	NewLayer->setText( "" );
	NewLayer->setPixmap(loadIcon("Newlayer.png"));
	QToolTip::add( NewLayer, tr( "Add a new Layer" ) );
	Layout1->addWidget( NewLayer );

	DeleteLayer = new QPushButton( this, "DeleteLayer" );
	DeleteLayer->setMinimumSize( QSize( 50, 24 ) );
	DeleteLayer->setMaximumSize( QSize( 50, 24 ) );
	DeleteLayer->setText( "" );
	DeleteLayer->setPixmap(loadIcon("Deletelayer.png"));
	QToolTip::add( DeleteLayer, tr( "Delete Layer" ) );
	Layout1->addWidget( DeleteLayer );

	RaiseLayer = new QPushButton( this, "RaiseLayer" );
	RaiseLayer->setMinimumSize( QSize( 50, 24 ) );
	RaiseLayer->setMaximumSize( QSize( 50, 24 ) );
	RaiseLayer->setText( "" );
	RaiseLayer->setPixmap(loadIcon("Raiselayer.png"));
	QToolTip::add( RaiseLayer, tr( "Raise Layer" ) );
	Layout1->addWidget( RaiseLayer );

	LowerLayer = new QPushButton( this, "LowerLayer" );
	LowerLayer->setMinimumSize( QSize( 50, 24 ) );
	LowerLayer->setMaximumSize( QSize( 50, 24 ) );
	LowerLayer->setText( "" );
	LowerLayer->setPixmap(loadIcon("Lowerlayer.png"));
	QToolTip::add( LowerLayer, tr( "Lower Layer" ) );
	Layout1->addWidget( LowerLayer );

	LayerPaletteLayout->addLayout( Layout1 );
	ClearInhalt();
	connect(NewLayer, SIGNAL(clicked()), this, SLOT(addLayer()));
	connect(DeleteLayer, SIGNAL(clicked()), this, SLOT(removeLayer()));
	connect(RaiseLayer, SIGNAL(clicked()), this, SLOT(upLayer()));
	connect(LowerLayer, SIGNAL(clicked()), this, SLOT(downLayer()));
	connect(Table, SIGNAL(valueChanged(int, int)), this, SLOT(changeName(int, int)));
	connect(Table, SIGNAL(updtName(int)), this, SLOT(updateName(int)));
}

void LayerPalette::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}

void LayerPalette::reject()
{
	emit Schliessen();
	QDialog::reject();
}

void LayerPalette::updateName(int r)
{
	changeName(r, 0);
	emit LayerActivated(*Activ);
}

void LayerPalette::ClearInhalt()
{
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	int b = Table->numRows();
	for (int a = 0; a < b; ++a)
		Table->removeRow(0);
	FlagsPrint.clear();
	FlagsSicht.clear();
	NewLayer->setEnabled(false);
	DeleteLayer->setEnabled(false);
	RaiseLayer->setEnabled(false);
	LowerLayer->setEnabled(false);
}

void LayerPalette::setLayers(QValueList<Layer> *layin, int *act)
{
	layers = layin;
	Activ = act;
	rebuildList();
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	MarkActiveLayer(*Activ);
	NewLayer->setEnabled(true);
	DeleteLayer->setEnabled(true);
	RaiseLayer->setEnabled(true);
	LowerLayer->setEnabled(true);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
}

void LayerPalette::rebuildList()
{
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	FlagsPrint.clear();
	FlagsSicht.clear();
	QString tmp;
	QValueList<Layer>::iterator it;
	Table->setNumRows(layers->count());
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		Table->setText(layers->count()-(*it).Level-1, 0, (*it).Name);
		QCheckBox *cp = new QCheckBox(this, tmp.setNum((*it).Level));
		cp->setPixmap(loadIcon("DateiPrint16.png"));
		cp->setChecked((*it).Drucken);
		Table->setCellWidget(layers->count()-(*it).Level-1, 1, cp);
		FlagsPrint.append(cp);
		connect(cp, SIGNAL(clicked()), this, SLOT(printLayer()));
		QCheckBox *cp2 = new QCheckBox(this, tmp.setNum((*it).Level));
		cp2->setPixmap(loadIcon("Layervisible.xpm"));
		cp2->setChecked((*it).Sichtbar);
		FlagsSicht.append(cp2);
		connect(cp2, SIGNAL(clicked()), this, SLOT(visibleLayer()));
		Table->setCellWidget(layers->count()-(*it).Level-1, 2, cp2);
		Header->setLabel(layers->count()-(*it).Level-1, tmp.setNum((*it).Level));
	}
	Table->setColumnStretchable(0, true);
	Table->adjustColumn(0);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
}

void LayerPalette::addLayer()
{
	QString tmp;
	struct Layer ll;
	ll.LNr = layers->last().LNr + 1;
	ll.Level = layers->count();
	ll.Name = tr("New Layer")+" "+tmp.setNum(ll.LNr);
	ll.Sichtbar = true;
	ll.Drucken = true;
	layers->append(ll);
	rebuildList();
	*Activ = ll.LNr;
	MarkActiveLayer(*Activ);
	emit LayerActivated(*Activ);
	ScApp->slotDocCh();
}

void LayerPalette::removeLayer()
{
	if (layers->count() < 2)
		return;
	bool delToo = false;
	int t = QMessageBox::warning(this, tr("Delete Layer"),
	                             tr("Do you want to delete all Objects on this Layer too?"),
	                             QMessageBox::No | QMessageBox::Default | QMessageBox::Escape,
	                             QMessageBox::Yes,
	                             QMessageBox::NoButton);
	if (t == QMessageBox::Yes)
		delToo = true;
	int num = layers->count()-1-Table->currentRow();
	QValueList<Layer>::iterator it2;
	for (it2 = layers->begin(); it2 != layers->end(); ++it2)
	{
		if ((*it2).Level == num)
			break;
	}
	int num2 = (*it2).LNr;
	if (!num2)
		return;
	layers->remove
	(it2);
	QValueList<Layer>::iterator it;
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		if ((*it).Level > num)
			(*it).Level -= 1;
	}
	rebuildList();
	emit LayerRemoved(num2, delToo);
	*Activ = 0;
	MarkActiveLayer(*Activ);
	emit LayerActivated(*Activ);
	ScApp->slotDocCh();
}

void LayerPalette::upLayer()
{
	if ((layers->count() < 2) || (Table->currentRow() == 0))
		return;
	int num = layers->count()-1-Table->currentRow();
	QValueList<Layer>::iterator it;
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		if ((*it).Level == num+1)
			break;
	}
	QValueList<Layer>::iterator it2;
	for (it2 = layers->begin(); it2 != layers->end(); ++it2)
	{
		if ((*it2).Level == num)
			break;
	}
	(*it2).Level += 1;
	(*it).Level -= 1;
	rebuildList();
	emit LayerChanged();
	MarkActiveLayer(*Activ);
	ScApp->slotDocCh();
}

void LayerPalette::downLayer()
{
	if ((layers->count() < 2) || (Table->currentRow() == static_cast<int>(layers->count()) - 1)
	   )
		return;
	int num = layers->count()-1-Table->currentRow();
	QValueList<Layer>::iterator it;
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		if ((*it).Level == num-1)
			break;
	}
	QValueList<Layer>::iterator it2;
	for (it2 = layers->begin(); it2 != layers->end(); ++it2)
	{
		if ((*it2).Level == num)
			break;
	}
	(*it2).Level -= 1;
	(*it).Level += 1;
	rebuildList();
	emit LayerChanged();
	MarkActiveLayer(*Activ);
	ScApp->slotDocCh();
}

void LayerPalette::changeName(int row, int col)
{
	if (col == 0)
	{
		int num = layers->count()-1-row;
		QValueList<Layer>::iterator it;
		for (it = layers->begin(); it != layers->end(); ++it)
		{
			if ((*it).Level == num)
			{
				(*it).Name = Table->text(row, col);
				ScApp->slotDocCh();
			}
		}
	}
}

void LayerPalette::visibleLayer()
{
	int num = QString(sender()->name()).toInt();
	QValueList<Layer>::iterator it;
	QPtrListIterator<QCheckBox> it2(FlagsSicht);
	for (it = layers->begin(); it != layers->end(); ++it, ++it2)
	{
		if ((*it).Level == num)
		{
			(*it).Sichtbar = it2.current()->isChecked();
			emit LayerChanged();
			ScApp->slotDocCh();
		}
	}
}

void LayerPalette::printLayer()
{
	int num = QString(sender()->name()).toInt();
	QValueList<Layer>::iterator it;
	QPtrListIterator<QCheckBox> it2(FlagsPrint);
	for (it = layers->begin(); it != layers->end(); ++it, ++it2)
	{
		if ((*it).Level == num)
		{
			(*it).Drucken = it2.current()->isChecked();
			ScApp->slotDocCh();
		}
	}
}

void LayerPalette::MarkActiveLayer(int l)
{
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	QValueList<Layer>::iterator it;
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		if ((*it).LNr == l)
			break;
	}
	Table->setCurrentCell(layers->count()-1-(*it).Level, 0);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
}

void LayerPalette::setActiveLayer(int row)
{
	QValueList<Layer>::iterator it;
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		if ((*it).Level == static_cast<int>(layers->count())-1-row)
			break;
	}
	*Activ = (*it).LNr;
	emit LayerActivated(*Activ);
}
