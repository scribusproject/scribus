/****************************************************************************
**
**
** Created: Son Jun 2 11:23:14 2002
**      by:  Franz Schmid
**
**
****************************************************************************/
#include "layers.h"
#include "layers.moc"

extern QPixmap loadIcon(QString nam);

LayerPalette::LayerPalette(QWidget* parent)
    : QDialog( parent, "Layers", false, 0 )
{
  	setIcon(loadIcon("AppIcon.xpm"));
    setCaption( tr( "Layers" ) );
    LayerPaletteLayout = new QVBoxLayout( this, 10, 5, "LayerPaletteLayout");

    Table = new QTable( this, "Table" );
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

    NewLayer = new QPushButton( this, "NewLayer" );
    NewLayer->setMinimumSize( QSize( 50, 24 ) );
    NewLayer->setMaximumSize( QSize( 50, 24 ) );
    NewLayer->setText( tr( "" ) );
    NewLayer->setPixmap(loadIcon("Newlayer.png"));
    QToolTip::add( NewLayer, tr( "Add a new Layer" ) );
    Layout1->addWidget( NewLayer );

    DeleteLayer = new QPushButton( this, "DeleteLayer" );
    DeleteLayer->setMinimumSize( QSize( 50, 24 ) );
    DeleteLayer->setMaximumSize( QSize( 50, 24 ) );
    DeleteLayer->setText( tr( "" ) );
    DeleteLayer->setPixmap(loadIcon("Deletelayer.png"));
    QToolTip::add( DeleteLayer, tr( "Delete Layer" ) );
    Layout1->addWidget( DeleteLayer );

    RaiseLayer = new QPushButton( this, "RaiseLayer" );
    RaiseLayer->setMinimumSize( QSize( 50, 24 ) );
    RaiseLayer->setMaximumSize( QSize( 50, 24 ) );
    RaiseLayer->setText( tr( "" ) );
    RaiseLayer->setPixmap(loadIcon("Raiselayer.png"));
    QToolTip::add( RaiseLayer, tr( "Raise Layer" ) );
    Layout1->addWidget( RaiseLayer );

    LowerLayer = new QPushButton( this, "LowerLayer" );
    LowerLayer->setMinimumSize( QSize( 50, 24 ) );
    LowerLayer->setMaximumSize( QSize( 50, 24 ) );
    LowerLayer->setText( tr( "" ) );
    LowerLayer->setPixmap(loadIcon("Lowerlayer.png"));
    QToolTip::add( LowerLayer, tr( "Lower Layer" ) );
    Layout1->addWidget( LowerLayer );

    CloseB = new QPushButton( this, "Closeb" );
    CloseB->setMinimumSize( QSize( 50, 24 ) );
    CloseB->setMaximumSize( QSize( 50, 24 ) );
    CloseB->setText( tr( "" ) );
    CloseB->setPixmap(loadIcon("DateiClos16.png"));
    QToolTip::add( CloseB, tr( "Closes this Dialog" ) );
    Layout1->addWidget( CloseB );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );
    LayerPaletteLayout->addLayout( Layout1 );
		ClearInhalt();
		connect(NewLayer, SIGNAL(clicked()), this, SLOT(addLayer()));
		connect(DeleteLayer, SIGNAL(clicked()), this, SLOT(removeLayer()));
		connect(RaiseLayer, SIGNAL(clicked()), this, SLOT(upLayer()));
		connect(LowerLayer, SIGNAL(clicked()), this, SLOT(downLayer()));
		connect(Table, SIGNAL(valueChanged(int, int)), this, SLOT(changeName(int, int)));
    connect(CloseB, SIGNAL(clicked()), this, SLOT(Verlassen()));
}

void LayerPalette::closeEvent(QCloseEvent *ce)
{	
	emit Schliessen();
	ce->accept();
}

void LayerPalette::Verlassen()
{
	close();
}

void LayerPalette::ClearInhalt()
{
	disconnect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
	int b = Table->numRows()-1;
	if (b > 0)
		{
		for (int a = b; a > 0; a--)
			{
			Table->removeRow(a);
			}
		Table->clearCell(0, 0);
		Table->clearCellWidget(0, 1);
		Table->clearCellWidget(0, 2);
		}
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
	for (uint l = 0; l < layers->count(); ++l)
		{
		it = layers->at(l);
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
	ll.LNr = layers->count();
	ll.Level = layers->count();
	ll.Name = tr("New Layer")+" "+tmp.setNum(ll.Level);
	ll.Sichtbar = true;
	ll.Drucken = true;
	layers->append(ll);
	rebuildList();
	*Activ = ll.LNr;
	MarkActiveLayer(*Activ);
	emit LayerActivated(*Activ);
}

void LayerPalette::removeLayer()
{
	if (layers->count() < 2)
		return;
	int num = layers->count()-1-Table->currentRow();
	QValueList<Layer>::iterator it2;
	for (uint l = 0; l < layers->count(); ++l)
		{
		it2 = layers->at(l);
		if ((*it2).Level == num)
			break;
		}
	int num2 = (*it2).LNr;
	layers->remove(it2);
	QValueList<Layer>::iterator it;
	for (uint l = 0; l < layers->count(); ++l)
		{
		it = layers->at(l);
		if ((*it).Level > num)
			(*it).Level -= 1;
		if ((*it).LNr > num2)
			(*it).LNr -= 1;
		}
	rebuildList();
	emit LayerRemoved(num);
	*Activ = 0;
	MarkActiveLayer(*Activ);
	emit LayerActivated(*Activ);
}

void LayerPalette::upLayer()
{
	if (layers->count() < 2)
		return;
	if (Table->currentRow() == 0)
		return;
	int num = layers->count()-1-Table->currentRow();
	QValueList<Layer>::iterator it;
	for (uint l = 0; l < layers->count(); ++l)
		{
		it = layers->at(l);
		if ((*it).Level == num+1)
			break;
		}
	QValueList<Layer>::iterator it2;
	for (uint l = 0; l < layers->count(); ++l)
		{
		it2 = layers->at(l);
		if ((*it2).Level == num)
			break;
		}
	(*it2).Level += 1;
	(*it).Level -= 1;
	rebuildList();
	emit LayerChanged();
	MarkActiveLayer(*Activ);
}

void LayerPalette::downLayer()
{
	if (layers->count() < 2)
		return;
	if (Table->currentRow() == static_cast<int>(layers->count())-1)
		return;
	int num = layers->count()-1-Table->currentRow();
	QValueList<Layer>::iterator it;
	for (uint l = 0; l < layers->count(); ++l)
		{
		it = layers->at(l);
		if ((*it).Level == num-1)
			break;
		}
	QValueList<Layer>::iterator it2;
	for (uint l = 0; l < layers->count(); ++l)
		{
		it2 = layers->at(l);
		if ((*it2).Level == num)
			break;
		}
	(*it2).Level -= 1;
	(*it).Level += 1;
	rebuildList();
	emit LayerChanged();
	MarkActiveLayer(*Activ);
}

void LayerPalette::changeName(int row, int col)
{
	if (col == 0)
		(*layers->at(layers->count()-row-1)).Name = Table->text(row, col);
}

void LayerPalette::visibleLayer()
{
	int num = QString(sender()->name()).toInt();
	QValueList<Layer>::iterator it;
	for (uint l = 0; l < layers->count(); ++l)
		{
		it = layers->at(l);
		if ((*it).Level == num)
			{
			(*it).Sichtbar = FlagsSicht.at(l)->isChecked();
			emit LayerChanged();
			}
		}
}

void LayerPalette::printLayer()
{
	int num = QString(sender()->name()).toInt();
	QValueList<Layer>::iterator it;
	for (uint l = 0; l < layers->count(); ++l)
		{
		it = layers->at(l);
		if ((*it).Level == num)
			{
			(*it).Drucken = FlagsPrint.at(l)->isChecked();
			}
		}
}

void LayerPalette::MarkActiveLayer(int l)
{
	QValueList<Layer>::iterator it;
	for (uint la = 0; la < layers->count(); ++la)
		{
		it = layers->at(la);
		if ((*it).LNr == l)
			break;
		}
	Table->setCurrentCell(layers->count()-1-(*it).Level, 0);
}

void LayerPalette::setActiveLayer(int row)
{
	QValueList<Layer>::iterator it;
	for (uint la = 0; la < layers->count(); ++la)
		{
		it = layers->at(la);
		if ((*it).Level == static_cast<int>(layers->count())-1-row)
			break;
		}
	*Activ = (*it).LNr;
	emit LayerActivated(*Activ);
}
