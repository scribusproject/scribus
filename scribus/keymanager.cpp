#include "keymanager.h"
#include "keymanager.moc"
#include <qaccel.h>
#include <qstringlist.h>
#include <qmessagebox.h>
#include <qkeysequence.h>

KeyManager::KeyManager(QWidget* parent, QMap<QString,Keys> oldKeyMap): QWidget( parent, "key" )
{
	keyMap.clear();
	keyMap = oldKeyMap;
	Part1 = "";
	Part2 = "";
	Part3 = "";
	keyCode = 0;
	keyManagerLayout = new QVBoxLayout( this, 11, 6); 
	keyManagerLayout->setAlignment( Qt::AlignTop );

	keyTable = new QTable( oldKeyMap.count(), 2, this, "keyTable" );
	keyTable->setMaximumSize(QSize(500,200));
	uint a=0;
	for (QMap<QString,Keys>::Iterator it=keyMap.begin(); it!=keyMap.end(); ++it)
	{
		QTableItem *item = new QTableItem(keyTable, QTableItem::Never, it.data().cleanMenuText);
		keyTable->setItem(a, 0, item);
		QTableItem *item2 = new QTableItem(keyTable, QTableItem::Never, QString(it.data().keySequence));
		keyTable->setItem(a, 1, item2);
		it.data().tableRow=a;
		++a;
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

	dispKey(0);

	// signals and slots connections
	connect( keyTable, SIGNAL(pressed(int, int, int, const QPoint&)), this, SLOT(dispKey(int)));
	connect( noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect( setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
}

bool KeyManager::event( QEvent* ev )
{
	bool ret = QWidget::event( ev );
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
					keyTable->setText(currRow, 1, "");
					keyDisplay->setText("");
					if (currentKeyMapRow!=NULL)
						currentKeyMapRow.data().keySequence="";
					noKey->setChecked(true);
				}
				else
				{
					QString newKeySequence=QString(QKeySequence(keyCode));
					keyTable->setText(currRow, 1, newKeySequence);
					if (currentKeyMapRow!=NULL)
						currentKeyMapRow.data().keySequence=newKeySequence;
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
	keyTable->setText(row, 1, keyTable->text(row, 1));
	
	for (QMap<QString,Keys>::Iterator it=keyMap.begin(); it!=keyMap.end(); ++it)
	{
		if (it.data().tableRow == row)
		{
			if (it.data().keySequence == "")
				noKey->setChecked(true);
			else
				userDef->setChecked(true);
			currentKeyMapRow=it;	
		}
	}
	currRow = row;
}

void KeyManager::setNoKey()
{
	if (noKey->isChecked())
	{
		keyTable->setText(currRow, 1, "");
		keyDisplay->setText("");
		if (currentKeyMapRow!=NULL)
			currentKeyMapRow.data().keySequence="";
	}
}

QString KeyManager::getKeyText(int KeyC)
{
	return ((KeyC == 0) ? "" : QString(QKeySequence(KeyC)));
}

bool KeyManager::checkKey(int code)
{
	bool ret = false;
	for (QMap<QString,Keys>::Iterator it=keyMap.begin(); it!=keyMap.end(); ++it)
	{
		if (it.data().keySequence == QString(QKeySequence(code)))
		{
			ret = true;
			break;
		}
	}
	return ret;
}

const QMap<QString, Keys> KeyManager::getNewKeyMap()
{
	return keyMap;
}
