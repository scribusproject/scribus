/****************************************************************************
**
**
** Created: Son Jun 2 11:23:14 2002
**      by:  Franz Schmid
**
**
****************************************************************************/
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qvaluelist.h>

#include "scribus.h"
#include "undomanager.h"
#include "undostate.h"

#include "layers.h"
#include "layers.moc"

extern QPixmap loadIcon(QString nam);
extern ScribusApp* ScApp;


LayerTable::LayerTable(QWidget* parent) : QTable(parent)
{
}

/*
void LayerTable::keyPressEvent(QKeyEvent *k)
{
	QTable::keyPressEvent(k);
}
*/

void LayerTable::endEdit ( int row, int col, bool accept, bool replace )
{
	QTable::EditMode ed = editMode();
	QTable::endEdit(row, col, accept, replace);
	if (ed != QTable::NotEditing)
		emit updtName(row);
}

LayerPalette::LayerPalette(QWidget* parent)
		: ScrPaletteBase( parent, "Layers", false, 0 )
{
	setIcon(loadIcon("AppIcon.png"));
	setCaption( tr( "Layers" ) );
	LayerPaletteLayout = new QVBoxLayout( this, 10, 5, "LayerPaletteLayout");

	Table = new LayerTable( this );
	Table->setNumRows( 0 );
	Table->setNumCols( 3 );
	QHeader *header = Table->horizontalHeader();
	header->setLabel(0, loadIcon("Layervisible.xpm"), "");
	header->setLabel(1, loadIcon("DateiPrint16.png"), "");
	header->setLabel(2, tr("Name"));
	Table->setColumnReadOnly(0, true);
	Table->setColumnReadOnly(1, true);
	Table->setColumnWidth(0, 24);
	Table->setColumnWidth(1, 24);
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

	undoManager = UndoManager::instance();
}
/*
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
*/
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
		Table->setText(layers->count()-(*it).Level-1, 2, (*it).Name);
		QCheckBox *cp = new QCheckBox(this, tmp.setNum((*it).Level));
		cp->setChecked((*it).isPrintable);
		Table->setCellWidget(layers->count()-(*it).Level-1, 1, cp);
		FlagsPrint.append(cp);
		connect(cp, SIGNAL(clicked()), this, SLOT(printLayer()));
		QCheckBox *cp2 = new QCheckBox(this, tmp.setNum((*it).Level));
		cp2->setChecked((*it).isViewable);
		FlagsSicht.append(cp2);
		connect(cp2, SIGNAL(clicked()), this, SLOT(visibleLayer()));
		Table->setCellWidget(layers->count()-(*it).Level-1, 0, cp2);
		Header->setLabel(layers->count()-(*it).Level-1, tmp.setNum((*it).Level));
	}
	Table->setColumnStretchable(2, true);
	Table->adjustColumn(2);
	connect(Table, SIGNAL(currentChanged(int, int)), this, SLOT(setActiveLayer(int)));
}

void LayerPalette::addLayer()
{
	QString tmp;
	struct Layer ll;
	ll.LNr = layers->last().LNr + 1;
	ll.Level = layers->count();
	ll.Name = tr("New Layer")+" "+tmp.setNum(ll.LNr);
	ll.isViewable = true;
	ll.isPrintable = true;
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
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::RaiseLayer, "", Um::IUp);
		ss->set("UP_LAYER", "up_layer");
		ss->set("ACTIVE", *Activ);
		undoManager->action(this, ss, ScApp->doc->DocName, Um::IDocument);
	}
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
	if ((layers->count() < 2) || (Table->currentRow() == static_cast<int>(layers->count()) - 1))
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LowerLayer, "", Um::IDown);
		ss->set("DOWN_LAYER", "down_layer");
		ss->set("ACTIVE", *Activ);
		undoManager->action(this, ss, ScApp->doc->DocName, Um::IDocument);
	}
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
	
	if (col == 2)
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
			(*it).isViewable = it2.current()->isChecked();
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
			bool printable =  it2.current()->isChecked();
			if (UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(printable ? Um::PrintLayer : Um::DoNotPrintLayer,
						                          "", Um::IPrint);
				ss->set("PRINT_LAYER", "print_layer");
				ss->set("ACTIVE", (*it).LNr);
				ss->set("PRINT", printable);
				undoManager->action(this, ss, ScApp->doc->DocName, Um::IDocument);
			}
			(*it).isPrintable = printable;
			ScApp->slotDocCh();
		}
	}
}

void LayerPalette::printLayer(int layerNr, bool isPrintable)
{
	QValueList<Layer>::iterator it;
	QPtrListIterator<QCheckBox> it2(FlagsPrint);
	for (it = layers->begin(); it != layers->end(); ++it, ++it2)
	{
		if ((*it).LNr == layerNr)
		{
			(*it).isPrintable = isPrintable;
			(*it2)->setChecked(isPrintable);
			ScApp->slotDocCh();
		}
	}
}

void LayerPalette::MarkActiveLayer(int l)
{
	QValueList<Layer>::iterator it;
	for (it = layers->begin(); it != layers->end(); ++it)
	{
		if ((*it).LNr == l)
			break;
	}
	Table->setCurrentCell(layers->count()-1-(*it).Level, 2);
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

void LayerPalette::restore(UndoState *state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (ss)
	{
		if (ss->contains("UP_LAYER"))
		{
			MarkActiveLayer(ss->getInt("ACTIVE"));
			if (isUndo)
				downLayer();
			else
				upLayer();
		}
		else if (ss->contains("DOWN_LAYER"))
		{
			MarkActiveLayer(ss->getInt("ACTIVE"));
			if (isUndo)
				upLayer();
			else
				downLayer();
		}
		else if (ss->contains("PRINT_LAYER"))
		{
			bool print = ss->getBool("PRINT");
			printLayer(ss->getInt("ACTIVE"), isUndo ? !print : print);
		}
	}
}
