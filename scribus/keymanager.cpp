#include "keymanager.h"
#include "keymanager.moc"
#include <qaccel.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qkeysequence.h>
extern QPixmap loadIcon(QString nam);

KeyManager::KeyManager(QWidget* parent, QMap<int,Keys> oldKeyMap)
	: QDialog( parent, "key", true, 0 )
{
	setCaption( tr( "Manage Keyboard Shortcuts" ) );
	setIcon(loadIcon("AppIcon.png"));
	keyMap = oldKeyMap;
	Part1 = "";
	Part2 = "";
	Part3 = "";
	keyCode = 0;
	keyManagerLayout = new QVBoxLayout( this, 11, 6); 

	keyTable = new QTable( oldKeyMap.count(), 2, this, "keyTable" );
	keyTable->setMaximumSize(QSize(500,200));
	for (uint a = 0; a < oldKeyMap.count(); ++a)
	{
		QTableItem *it = new QTableItem(keyTable, QTableItem::Never, oldKeyMap[a].Name);
		keyTable->setItem(a, 0, it);
		QTableItem *it2 = new QTableItem(keyTable, QTableItem::Never, getKeyText(oldKeyMap[a].KeyID));
		keyTable->setItem(a, 1, it2);
	}
	keyTable->setSorting(false);
	keyTable->setSelectionMode(QTable::NoSelection);
	keyTable->setLeftMargin(0);
	keyTable->verticalHeader()->hide();
	header = keyTable->horizontalHeader();
	header->setLabel(0, tr("Action"));
	header->setLabel(1, tr("Current Key"));
	keyTable->adjustColumn(0);
	keyTable->adjustColumn(1);
	keyTable->setColumnMovingEnabled(false);
	keyTable->setRowMovingEnabled(false);
	header->setMovingEnabled(false);
	keyTable->setShowGrid( false );
	keyManagerLayout->addWidget( keyTable );

	keyGroup = new QButtonGroup( this, "keyGroup" );
	keyGroup->setTitle( tr( "Select a Key for this Action" ) );
	keyGroup->setColumnLayout(0, Qt::Vertical );
	keyGroup->layout()->setSpacing( 0 );
	keyGroup->layout()->setMargin( 0 );
	keyGroupLayout = new QGridLayout( keyGroup->layout() );
	keyGroupLayout->setAlignment( Qt::AlignTop );
	keyGroupLayout->setSpacing( 6 );
	keyGroupLayout->setMargin( 11 );

	noKey = new QRadioButton( tr( "&No Key" ), keyGroup, "noKey" );
	keyGroupLayout->addMultiCellWidget( noKey, 0, 1, 0, 1 );
	userDef = new QRadioButton( tr( "&User Defined Key" ), keyGroup, "userDef" );
	keyGroupLayout->addWidget( userDef, 2, 0 );

	keyDisplay = new QLabel( tr( "ALT+SHIFT+T" ), keyGroup, "keyDisplay" );
	keyDisplay->setFrameShape( QLabel::Panel );
	keyDisplay->setFrameShadow( QLabel::Sunken );
	keyDisplay->setAlignment( static_cast<int>( QLabel::AlignCenter ) );

	keyGroupLayout->addMultiCellWidget( keyDisplay, 0, 2, 2, 2 );

	setKeyButton = new QPushButton( tr( "Set &Key" ), keyGroup, "setKeyButton" );
	setKeyButton->setToggleButton(true);

	keyGroupLayout->addMultiCellWidget( setKeyButton, 0, 2, 1, 1, Qt::AlignCenter );
	keyManagerLayout->addWidget( keyGroup );

	okCancelLayout = new QHBoxLayout; 
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );

	okButton = new QPushButton( tr( "&OK" ), this, "okButton" );
	okButton->setDefault( true );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( tr( "&Cancel" ), this, "cancelButton" );
	okCancelLayout->addWidget( cancelButton );
	keyManagerLayout->addLayout( okCancelLayout );

	dispKey(0);

	// signals and slots connections
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( keyTable, SIGNAL(pressed(int, int, int, const QPoint&)), this, SLOT(dispKey(int)));
	connect( noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect( setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
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
	if (setKeyButton->isOn())
	{
		QStringList tl;
		if (keyDisplay->text() != "")
		{
			tl = tl.split("+", keyDisplay->text());
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
				keyCode |= 0x00200000;
				break;
			case Key_Alt:
				Part2 = tr("Alt+");
				keyCode |= 0x00800000;
				break;
			case Key_Control:
				Part1 = tr("Ctrl+");
				keyCode |= 0x00400000;
				break;
			default:
				keyCode |= k->key();
				keyDisplay->setText(getKeyText(keyCode));
				if (checkKey(keyCode))
				{
					QMessageBox::information(this,
											tr("Warning"),
											tr("This Key Sequence is already in use"),
											tr("&OK"), 0, 0, 0, QMessageBox::Ok);
					keyTable->setText(actRow, 1, "");
					keyDisplay->setText("");
					keyMap[actRow].KeyID = 0;
					noKey->setChecked(true);
				}
				else
				{
					keyTable->setText(actRow, 1, getKeyText(keyCode));
					keyMap[actRow].KeyID = keyCode;
					userDef->setChecked(true);
				}
				setKeyButton->setOn(false);
				releaseKeyboard();
		}
	}
	if (setKeyButton->isOn())
		keyDisplay->setText(Part1+Part2+Part3+Part4);
}

void KeyManager::keyReleaseEvent(QKeyEvent *k)
{
	if (setKeyButton->isOn())
	{
		if (keyDisplay->text() != "")
		{
			QStringList tl;
			tl = tl.split("+", keyDisplay->text());
			Part4 = tl[tl.count()-1];
			if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift"))
				Part4 = "";
		}
		else
			Part4 = "";
		if (k->key() == Key_Shift)
		{
			Part3 = "";
			keyCode &= ~0x00200000;
		}
		if (k->key() == Key_Alt)
		{
			Part2 = "";
			keyCode &= ~0x00800000;
		}
		if (k->key() == Key_Control)
		{
			Part1 = "";
			keyCode &= ~0x00400000;
		}
		keyDisplay->setText(Part1+Part2+Part3+Part4);
	}
}

void KeyManager::setKeyText()
{
	if (setKeyButton->isOn())
	{
		keyCode = 0;
		Part1 = "";
		Part2 = "";
		Part3 = "";
		Part4 = "";
		grabKeyboard();
	}
	else
		releaseKeyboard();
}

void KeyManager::dispKey(int row)
{
	keyDisplay->setText(keyTable->text(row, 1));
	if (keyMap[row].KeyID == 0)
		noKey->setChecked(true);
	else
		userDef->setChecked(true);
	actRow = row;
}

void KeyManager::setNoKey()
{
	if (noKey->isChecked())
	{
		keyTable->setText(actRow, 1, "");
		keyDisplay->setText("");
		keyMap[actRow].KeyID = 0;
	}
}

QString KeyManager::getKeyText(int KeyC)
{
	return ((KeyC == 0) ? "" : QString(QKeySequence(KeyC)));
}

bool KeyManager::checkKey(int code)
{
	bool ret = false;
	for (uint a = 0; a < keyMap.count(); ++a)
	{
		if (keyMap[a].KeyID == code)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

const QMap<int, Keys> KeyManager::getNewKeyMap()
{
	return keyMap;
}
