/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "keymanager.h"
#include "keymanager.moc"
#include <qaccel.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qkeysequence.h>
extern QPixmap loadIcon(QString nam);

KeyManager::KeyManager(QWidget* parent, QMap<int,Keys> Ke)
	: QDialog( parent, "key", true, 0 )
{
	setCaption( tr( "Manage Keyboard Shortcuts" ) );
	setIcon(loadIcon("AppIcon.png"));
	KK = Ke;
	Part1 = "";
	Part2 = "";
	Part3 = "";
	Kcode = 0;
	KeyManagerLayout = new QVBoxLayout( this ); 
	KeyManagerLayout->setSpacing( 6 );
	KeyManagerLayout->setMargin( 11 );

	TastenT = new QTable( this, "TastenT" );
	TastenT->setMaximumSize(QSize(500,200));
	TastenT->setNumCols( 2 );
	TastenT->setNumRows( Ke.count() );
	for (uint a = 0; a < Ke.count(); ++a)
	{
		QTableItem *it = new QTableItem(TastenT, QTableItem::Never, Ke[a].Name);
		TastenT->setItem(a, 0, it);
		QTableItem *it2 = new QTableItem(TastenT, QTableItem::Never, GetKeyText(Ke[a].KeyID));
		TastenT->setItem(a, 1, it2);
	}
	TastenT->setSorting(false);
	TastenT->setSelectionMode(QTable::NoSelection);
	TastenT->setLeftMargin(0);
	TastenT->verticalHeader()->hide();
	Header = TastenT->horizontalHeader();
	Header->setLabel(0, tr("Action"));
	Header->setLabel(1, tr("Current Key"));
	TastenT->adjustColumn(0);
	TastenT->adjustColumn(1);
	TastenT->setColumnMovingEnabled(false);
	TastenT->setRowMovingEnabled(false);
	Header->setMovingEnabled(false);
	TastenT->setShowGrid( false );
	KeyManagerLayout->addWidget( TastenT );

	KeyGroup = new QButtonGroup( this, "KeyGroup" );
	KeyGroup->setTitle( tr( "Select a Key for this Action" ) );
	KeyGroup->setColumnLayout(0, Qt::Vertical );
	KeyGroup->layout()->setSpacing( 0 );
	KeyGroup->layout()->setMargin( 0 );
	KeyGroupLayout = new QGridLayout( KeyGroup->layout() );
	KeyGroupLayout->setAlignment( Qt::AlignTop );
	KeyGroupLayout->setSpacing( 6 );
	KeyGroupLayout->setMargin( 11 );

	NoKey = new QRadioButton( tr( "&No Key" ), KeyGroup, "NoKey" );

	KeyGroupLayout->addMultiCellWidget( NoKey, 0, 1, 0, 1 );

	UserDef = new QRadioButton( tr( "&User Defined Key" ), KeyGroup, "UserDef" );
	KeyGroupLayout->addWidget( UserDef, 2, 0 );

	Tdisplay = new QLabel( KeyGroup, "Tdisplay" );
	Tdisplay->setFrameShape( QLabel::Panel );
	Tdisplay->setFrameShadow( QLabel::Sunken );
	Tdisplay->setText( tr( "ALT+SHIFT+T" ) );
	Tdisplay->setAlignment( static_cast<int>( QLabel::AlignCenter ) );

	KeyGroupLayout->addMultiCellWidget( Tdisplay, 0, 2, 2, 2 );

	SetKey = new QPushButton( tr( "Set &Key" ), KeyGroup, "SetKey" );
	SetKey->setToggleButton(true);

	KeyGroupLayout->addMultiCellWidget( SetKey, 0, 2, 1, 1, Qt::AlignCenter );
	KeyManagerLayout->addWidget( KeyGroup );

	Layout4 = new QHBoxLayout; 
	Layout4->setSpacing( 6 );
	Layout4->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout4->addItem( spacer );

	OKButton = new QPushButton( tr( "&OK" ), this, "OKButton" );
	OKButton->setDefault( true );
	Layout4->addWidget( OKButton );
	CancelB = new QPushButton( tr( "&Cancel" ), this, "CancelB" );
	Layout4->addWidget( CancelB );

	KeyManagerLayout->addLayout( Layout4 );
	DispKey(0);
	// signals and slots connections
	connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect(TastenT, SIGNAL(pressed(int, int, int, const QPoint&)), this, SLOT(DispKey(int)));
	connect(NoKey, SIGNAL(clicked()), this, SLOT(SetNoKey()));
	connect(SetKey, SIGNAL(clicked()), this, SLOT(SetKeyText()));
}

bool KeyManager::event( QEvent* ev )
{
	bool ret = QDialog::event( ev );
	if ( ev->type() == QEvent::KeyPress )
		keyPressEvent((QKeyEvent*)ev);
	if ( ev->type() == QEvent::KeyRelease )
		keyReleaseEvent((QKeyEvent*)ev);
	return ret;
}

void KeyManager::keyPressEvent(QKeyEvent *k)
{
	if (SetKey->isOn())
	{
		QStringList tl;
		if (Tdisplay->text() != "")
		{
			tl = tl.split("+", Tdisplay->text());
			Part4 = tl[tl.count()-1];
			if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift"))
				Part4 = "";
		}
		else
			Part4 = "";
		switch (k->key())
		{
			case Key_Shift:
				Part3 = tr("Shift+");
				Kcode |= 0x00200000;
				break;
			case Key_Alt:
				Part2 = tr("Alt+");
				Kcode |= 0x00800000;
				break;
			case Key_Control:
				Part1 = tr("Ctrl+");
				Kcode |= 0x00400000;
				break;
			default:
				Kcode |= k->key();
				Tdisplay->setText(GetKeyText(Kcode));
				if (CheckKey(Kcode))
				{
					QMessageBox::information(this,
											tr("Warning"),
											tr("This Key Sequence is already in use"),
											tr("&OK"), 0, 0, 0, QMessageBox::Ok);
					TastenT->setText(ActRow, 1, "");
					Tdisplay->setText("");
					KK[ActRow].KeyID = 0;
					NoKey->setChecked(true);
				}
				else
				{
					TastenT->setText(ActRow, 1, GetKeyText(Kcode));
					KK[ActRow].KeyID = Kcode;
					UserDef->setChecked(true);
				}
				SetKey->setOn(false);
				releaseKeyboard();
		}
	}
	if (SetKey->isOn())
		Tdisplay->setText(Part1+Part2+Part3+Part4);
}

void KeyManager::keyReleaseEvent(QKeyEvent *k)
{
	if (SetKey->isOn())
	{
		if (Tdisplay->text() != "")
		{
			QStringList tl;
			tl = tl.split("+", Tdisplay->text());
			Part4 = tl[tl.count()-1];
			if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift"))
				Part4 = "";
		}
		else
			Part4 = "";
		if (k->key() == Key_Shift)
		{
			Part3 = "";
			Kcode &= ~0x00200000;
		}
		if (k->key() == Key_Alt)
		{
			Part2 = "";
			Kcode &= ~0x00800000;
		}
		if (k->key() == Key_Control)
		{
			Part1 = "";
			Kcode &= ~0x00400000;
		}
		Tdisplay->setText(Part1+Part2+Part3+Part4);
	}
}

void KeyManager::SetKeyText()
{
	if (SetKey->isOn())
	{
		Kcode = 0;
		Part1 = "";
		Part2 = "";
		Part3 = "";
		Part4 = "";
		grabKeyboard();
	}
	else
		releaseKeyboard();
}

void KeyManager::DispKey(int r)
{
	Tdisplay->setText(TastenT->text(r, 1));
	if (KK[r].KeyID == 0)
		NoKey->setChecked(true);
	else
		UserDef->setChecked(true);
	ActRow = r;
}

void KeyManager::SetNoKey()
{
	if (NoKey->isChecked())
	{
		TastenT->setText(ActRow, 1, "");
		Tdisplay->setText("");
		KK[ActRow].KeyID = 0;
	}
}

QString KeyManager::GetKeyText(int KeyC)
{
	if (KeyC == 0)
		return "";
	else
		return QString(QKeySequence(KeyC));
}

bool KeyManager::CheckKey(int Code)
{
	bool ret = false;
	for (uint a = 0; a < KK.count(); ++a)
	{
		if (KK[a].KeyID == Code)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

