/****************************************************************************
** Form implementation generated from reading ui file 'tabmanager.ui'
**
** Created: Don Jan 1 12:57:21 2004
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "tabmanager.h"
#include "tabmanager.moc"
#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "mspinbox.h"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

TabManager::TabManager( QWidget* parent, int dEin, QValueList<double> inTab) : QDialog( parent, "TabManager", true, 0 )
{
	setCaption( tr( "Manage Tabulators" ) );
	setIcon(loadIcon("AppIcon.png"));
	tmpTab = inTab;
	selTab = -1;
	TabManagerLayout = new QVBoxLayout( this, 5, 5, "TabManagerLayout"); 
	layout9 = new QHBoxLayout( 0, 0, 5, "layout9"); 

	TabList = new QListBox( this, "TabList" );
	layout9->addWidget( TabList );
	layout8 = new QVBoxLayout( 0, 0, 5, "layout8"); 
	layout7 = new QGridLayout( 0, 1, 1, 0, 5, "layout7"); 

	Text1 = new QLabel( this, "Text1" );
	Text1->setText( tr( "Position:" ) );
	layout7->addWidget( Text1, 0, 0 );

	Text2 = new QLabel( this, "Text2" );
	Text2->setText( tr( "Alignment:" ) );
	layout7->addWidget( Text2, 1, 0 );

	Align = new QComboBox( true, this, "Align" );
	Align->clear();
	QString tmp_ali[] = { tr("Left"), tr("Right"), tr("Full Stop"), tr("Comma"), tr("Center")};
	size_t ar_ali = sizeof(tmp_ali) / sizeof(*tmp_ali);
	for (uint a = 0; a < ar_ali; ++a)
		Align->insertItem(tmp_ali[a]);
	Align->setEditable(false);
	Align->setEnabled(false);
	layout7->addWidget( Align, 1, 1 );

	Position = new MSpinBox( this, 1 );
	Position->setSuffix( tr( " pt" ) );
	Position->setMaxValue( 30000 );
	Position->setMinValue( 0 );
	Position->setValue(0);
	Position->setEnabled(false);

	layout7->addWidget( Position, 0, 1 );
	layout8->addLayout( layout7 );
	QSpacerItem* spacer = new QSpacerItem( 20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout8->addItem( spacer );
	layout9->addLayout( layout8 );
	TabManagerLayout->addLayout( layout9 );

	layout10 = new QGridLayout( 0, 1, 1, 0, 5, "layout10"); 
	OKButton = new QPushButton( this, "OKButton" );
	OKButton->setText( tr( "OK" ) );
	OKButton->setDefault( TRUE );
	layout10->addWidget( OKButton, 1, 0 );

	CancelButton = new QPushButton( this, "CancelButton" );
	CancelButton->setText( tr( "Cancel" ) );
	layout10->addWidget( CancelButton, 1, 1 );

	DelButton = new QPushButton( this, "DelButton" );
	DelButton->setText( tr( "Remove" ) );
	DelButton->setEnabled(false);
	layout10->addWidget( DelButton, 0, 1 );

	AddButton = new QPushButton( this, "AddButton" );
	AddButton->setText( tr( "Add" ) );
	layout10->addWidget( AddButton, 0, 0 );
	TabManagerLayout->addLayout( layout10 );
	resize( QSize(321, 229).expandedTo(minimumSizeHint()) );
	UnitChange(dEin);
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( CancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(TabList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selIte(QListBoxItem*)));
	connect(Position, SIGNAL(valueChanged(int)), this, SLOT(PValChange()));
	connect(DelButton, SIGNAL(clicked()), this, SLOT(DelTab()));
	connect(AddButton, SIGNAL(clicked()), this, SLOT(AddTab()));
	connect(Align, SIGNAL(activated(int)), this, SLOT(ChangeType()));
	UpdateTabL();
}

void TabManager::DelTab()
{
	QValueList<double>::Iterator it;
	it = tmpTab.at(selTab);
	it = tmpTab.remove(it);
	tmpTab.remove(it);
	selTab = tmpTab.isEmpty() ? -1 : static_cast<int>(tmpTab.count() - 2);
	UpdateTabL();
}

void TabManager::AddTab()
{
	tmpTab.prepend(0);
	tmpTab.prepend(0);
	selTab = 0;
	UpdateTabL();
}

void TabManager::ChangeType()
{
	tmpTab[selTab] = static_cast<double>(Align->currentItem());
}

void TabManager::selIte(QListBoxItem *c)
{
	disconnect(Position, SIGNAL(valueChanged(int)), this, SLOT(PValChange()));
	disconnect(Align, SIGNAL(activated(int)), this, SLOT(ChangeType()));
	selTab = c->listBox()->currentItem() * 2;
	DelButton->setEnabled(true);
	Position->setEnabled(true);
	Align->setEnabled(true);
	Position->setValue(tmpTab[selTab+1] * UmReFaktor);
	Align->setCurrentItem(static_cast<int>(tmpTab[selTab]));
	connect(Position, SIGNAL(valueChanged(int)), this, SLOT(PValChange()));
	connect(Align, SIGNAL(activated(int)), this, SLOT(ChangeType()));
}

void TabManager::PValChange()
{
	double CurX = Position->value() / UmReFaktor;
	int gg = static_cast<int>(tmpTab.count()-1);
	int g = gg;
	double type = tmpTab[selTab];
	QValueList<double>::Iterator it;
	it = tmpTab.at(selTab);
	it = tmpTab.remove(it);
	tmpTab.remove(it);
	for (int yg = static_cast<int>(tmpTab.count()-1); yg > 0; yg -= 2)
	{
		if (CurX < tmpTab[yg])
			g = yg;
	}
	selTab = g-1;
	if (gg == g)
	{
		tmpTab.append(type);
		tmpTab.append(CurX);
		selTab = static_cast<int>(tmpTab.count()-2);
	}
	else
	{
		it = tmpTab.at(selTab);
		it = tmpTab.insert(it, CurX);
		tmpTab.insert(it, type);
	}
	UpdateTabL();
}

void TabManager::UnitChange(int dEin)
{
	Position->setDecimals(10);
	char *tmp[] = {" pt", " mm", " in", " p"};
	Einheit = dEin;
	QString ein = tr(tmp[dEin]);
	if (dEin == 2)
		Position->setDecimals(10000);
	Position->setSuffix(ein);
  if (!tmpTab.isEmpty() || (selTab != -1))
		Position->setValue(tmpTab[selTab+1] * UmReFaktor);
	else
		Position->setValue(0);
}

void TabManager::UpdateTabL()
{
	disconnect(TabList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selIte(QListBoxItem*)));
	disconnect(Position, SIGNAL(valueChanged(int)), this, SLOT(PValChange()));
	disconnect(Align, SIGNAL(activated(int)), this, SLOT(ChangeType()));
	TabList->clear();
	char *tp[] = {" pt", " mm", " in", " p"};
	QString ein = tr(tp[Einheit]), tmp;
	for (uint a = 0; a < tmpTab.count(); a += 2)
		TabList->insertItem(tmp.setNum(qRound(tmpTab[a+1]*UmReFaktor*Position->Decimals)/
							static_cast<double>(Position->Decimals))+ein);
	if (tmpTab.isEmpty())
		selTab = -1;
	if (selTab != -1)
	{
		Position->setEnabled(true);
		Align->setEnabled(true);
		Position->setValue(tmpTab[selTab+1] * UmReFaktor);
		Align->setCurrentItem(static_cast<int>(tmpTab[selTab]));
		TabList->setCurrentItem(selTab / 2);
	}
	else
	{
		Position->setValue(0);
		Align->setCurrentItem(0);
		Position->setEnabled(false);
		Align->setEnabled(false);
	}
	connect(TabList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selIte(QListBoxItem*)));
	connect(Position, SIGNAL(valueChanged(int)), this, SLOT(PValChange()));
	connect(Align, SIGNAL(activated(int)), this, SLOT(ChangeType()));
}
