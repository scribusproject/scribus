/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2006 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#include "tabkeyboardshortcutswidget.h"

#include <QComboBox>
#include <QDataStream>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeySequence>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTextStream>
#include <QList>
#include <QEvent>

#include "actionmanager.h"
#include "commonstrings.h"
#include "pluginmanager.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scpaths.h"
#include "scplugin.h"
#include "scraction.h"
#include "util.h"
#include "util_icon.h"

TabKeyboardShortcutsWidget::TabKeyboardShortcutsWidget(QMap<QString, Keys> oldKeyMap, QWidget *parent, const char *name)
    :QWidget(parent)
{
	setupUi(this);
	setObjectName(name);
// 	ActionManager::createDefaultMenus();
// 	ActionManager::createDefaultNonMenuActions();
	defMenus=ActionManager::defaultMenus();
	defNonMenuActions=ActionManager::defaultNonMenuActions();

	QVector< QPair<QString, QStringList> >::Iterator itnmenua = defNonMenuActions->begin();
	PluginManager& pluginManager(PluginManager::instance());
	QStringList pluginNames(pluginManager.pluginNames(false));
	ScPlugin* plugin;
	ScActionPlugin* ixplug;
	QString pName;
	for (int i = 0; i < pluginNames.count(); ++i)
	{
		pName = pluginNames.at(i);
		plugin = pluginManager.getPlugin(pName, true);
		Q_ASSERT(plugin); // all the returned names should represent loaded plugins
		if (plugin->inherits("ScActionPlugin"))
		{
			ixplug = dynamic_cast<ScActionPlugin*>(plugin);
			Q_ASSERT(ixplug);
			ScActionPlugin::ActionInfo ai(ixplug->actionInfo());
			itnmenua->second << ai.name;
		}
	}

	Q_CHECK_PTR(defMenus);
	lviToActionMap.clear();
	lviToMenuMap.clear();
	keyTable->clear();
	keyMap.clear();
	keyMap = oldKeyMap;
	Part0 = "";
	Part1 = "";
	Part2 = "";
	Part3 = "";
	Part4 = "";
	keyCode = 0;
	keyDisplay->setMinimumWidth(fontMetrics().width("CTRL+ALT+SHIFT+W"));
	keyDisplay->setText("");

	clearSearchButton->setIcon(loadIcon("clear_right.png"));
	// signals and slots connections
	connect( keyTable, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			 this, SLOT(dispKey(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect( noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect( setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
	connect( loadSetButton, SIGNAL(clicked()), this, SLOT(loadKeySetFile()));
	connect( importSetButton, SIGNAL(clicked()), this, SLOT(importKeySetFile()));
	connect( exportSetButton, SIGNAL(clicked()), this, SLOT(exportKeySetFile()));
	connect( resetSetButton, SIGNAL(clicked()), this, SLOT(resetKeySet()));
	connect( clearSearchButton, SIGNAL(clicked()), this, SLOT(clearSearchString()));
	connect( searchTextLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(applySearch(const QString&)));
}

void TabKeyboardShortcutsWidget::restoreDefaults()
{
	loadableSets->clear();
	loadableSets->addItems(scanForSets());
	insertActions();
	dispKey(0);
}

const QMap<QString, Keys> TabKeyboardShortcutsWidget::getNewKeyMap()
{
	return keyMap;
}

void TabKeyboardShortcutsWidget::setNoKey()
{
	if (noKey->isChecked())
	{
		if (selectedLVI!=0)
		{
			selectedLVI->setText(1,"");
			keyMap[lviToActionMap[selectedLVI]].keySequence=QKeySequence();
		}
		keyDisplay->setText("");
		noKey->setChecked(true);
	}
}

void TabKeyboardShortcutsWidget::loadKeySetFile()
{
	if (keySetList.contains(loadableSets->currentText()))
		importKeySet(keySetList[loadableSets->currentText()]);
}

void TabKeyboardShortcutsWidget::importKeySetFile()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString currentPath = dirs->get("keymapprefs_import", ScPaths::instance().shareDir() + "keysets/");
	QString s = QFileDialog::getOpenFileName(this, tr("Select a Key set file to read"), currentPath, tr("Key Set XML Files (*.xml)"));
	if (!s.isEmpty())
		importKeySet(s);
}
void TabKeyboardShortcutsWidget::exportKeySetFile()
{   
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString currentPath= dirs->get("keymapprefs_export", ".");
	QString s = QFileDialog::getSaveFileName(this, tr("Select a Key set file to save to"), currentPath, tr("Key Set XML Files (*.xml)") );
	if (!s.isEmpty())
		exportKeySet(s);
}

void TabKeyboardShortcutsWidget::importKeySet(QString filename)
{
	searchTextLineEdit->clear();
	QFileInfo fi = QFileInfo(filename);
	if (fi.exists())
	{
		//import the file into qdomdoc
		QDomDocument doc( "keymapentries" );
		QFile file1( filename );
		if ( !file1.open( QIODevice::ReadOnly ) )
			return;
		QTextStream ts(&file1);
		ts.setCodec("UTF-8");
		QString errorMsg;
		int eline;
		int ecol;
		if ( !doc.setContent( ts.readAll(), &errorMsg, &eline, &ecol )) 
		{
			qDebug("%s", QString("Could not open key set file: %1\nError:%2 at line: %3, row: %4").arg(filename).arg(errorMsg).arg(eline).arg(ecol).toAscii().constData());
			file1.close();
			return;
		}
		file1.close();
		//load the file now
		QDomElement docElem = doc.documentElement();
		if (docElem.tagName()=="shortcutset" && docElem.hasAttribute("name"))
		{
			QDomAttr keysetAttr = docElem.attributeNode( "name" );

			//clear current menu entries
			for (QMap<QString,Keys>::Iterator it=keyMap.begin(); it!=keyMap.end(); ++it)
				it.value().keySequence = QKeySequence();
			
			//load in new set
			QDomNode n = docElem.firstChild();
			while( !n.isNull() ) 
			{
				QDomElement e = n.toElement(); // try to convert the node to an element.
				if( !e.isNull() ) 
				{
					if (e.hasAttribute("name")  && e.hasAttribute( "shortcut" )) 
					{
						QDomAttr nameAttr = e.attributeNode( "name" );
						QDomAttr shortcutAttr = e.attributeNode( "shortcut" );
						if (keyMap.contains(nameAttr.value()))
							keyMap[nameAttr.value()].keySequence=QKeySequence(shortcutAttr.value());
					}
				}
				n = n.nextSibling();
			}
		}
	}
	insertActions();
}

bool TabKeyboardShortcutsWidget::exportKeySet(QString filename)
{
	QFileInfo fi = QFileInfo(filename);
	QString exportFileName;
	if (filename.endsWith(".xml"))
		exportFileName = filename;
	else
		exportFileName = filename+".xml";
	if (overwrite(this, exportFileName))
	{
		bool ok;
		QString setName = QInputDialog::getText(this, tr("Export Keyboard Shortcuts to File"), tr("Enter the name of the shortcut set:"), QLineEdit::Normal, QString::null, &ok);
		if (!( ok && !setName.isEmpty()) ) 
			return false;
		
		QDomDocument doc( "keymapentries" );
		QString keyset=QString("<shortcutset name=\"%1\"></shortcutset>").arg(setName);
		doc.setContent(keyset);
		QDomElement keySetElement=doc.documentElement();
		QMap<QString,Keys>::Iterator itEnd=keyMap.end();
		for (QMap<QString,Keys>::Iterator it=keyMap.begin(); it!=itEnd; ++it)
		{
			if (it.value().keySequence.isEmpty() && it.key().isEmpty())
				continue;
			QDomElement function_shortcut=doc.createElement("function");
			function_shortcut.setAttribute("name",it.key());
			function_shortcut.setAttribute("shortcut",getKeyText(it.value().keySequence));
			keySetElement.appendChild(function_shortcut);
		}
		QFile f(filename);
		if(!f.open(QIODevice::WriteOnly))
			return false;
		QDataStream s(&f);
		QByteArray xmltag("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		s.writeRawData(xmltag.data(), xmltag.length());
		QByteArray xmldoc = doc.toByteArray(4);
		s.writeRawData(xmldoc, xmldoc.length());
		f.close();
	}
	return true;
}

void TabKeyboardShortcutsWidget::resetKeySet()
{
	QString location=ScPaths::instance().shareDir();
	QString defaultKeySetFileName=QDir::convertSeparators(location+"keysets/scribus13.xml");
	importKeySet(defaultKeySetFileName);	
}

QStringList TabKeyboardShortcutsWidget::scanForSets()
{
	keySetList.clear();
	QString location=ScPaths::instance().shareDir();
	QString keySetLocation=QDir::convertSeparators(location+"keysets/");
	QDir keySetsDir(keySetLocation, "*.xml", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((keySetsDir.exists()) && (keySetsDir.count() != 0))
	{
		QStringList appNames;
		for (uint fileCounter = 0; fileCounter < keySetsDir.count(); ++fileCounter)
		{
			QString filename=QDir::convertSeparators(location+"keysets/"+keySetsDir[fileCounter]);
			
			QDomDocument doc( "keymapentries" );
			QFile file( filename );
			if ( !file.open( QIODevice::ReadOnly ) )
				continue;
			QString errorMsg;
			int eline;
			int ecol;
			
			if ( !doc.setContent( &file, &errorMsg, &eline, &ecol )) 
			{
				qDebug("%s", QString("Could not open key set file: %1\nError:%2 at line: %3, row: %4").arg(keySetsDir[fileCounter]).arg(errorMsg).arg(eline).arg(ecol).toAscii().constData());
				file.close();
				continue;
			}
			file.close();
		
			QDomElement docElem = doc.documentElement();
			if (docElem.tagName()=="shortcutset" && docElem.hasAttribute("name"))
			{
				QDomAttr nameAttr = docElem.attributeNode( "name" );
				appNames.append(nameAttr.value());
				keySetList.insert(nameAttr.value(), filename);
			}
		}
		return QStringList(appNames);
	}
	return QStringList();
}

QString TabKeyboardShortcutsWidget::getKeyText(int KeyC)
{
	if ((KeyC & ~(Qt::META | Qt::CTRL | Qt::ALT | Qt::SHIFT)) == 0)
		return "";
	// on OSX Qt translates modifiers to forsaken symbols, arrows and the like
	// we prefer plain English
	QString res;
	if ((KeyC & Qt::META) != 0)
		res += "Meta+";
	if ((KeyC & Qt::CTRL) != 0)
		res += "Ctrl+";
	if ((KeyC & Qt::ALT) != 0)
		res += "Alt+";
	if ((KeyC & Qt::SHIFT) != 0)
		res += "Shift+";
	return res + QString(QKeySequence(KeyC & ~(Qt::META | Qt::CTRL | Qt::ALT | Qt::SHIFT)));
}

void TabKeyboardShortcutsWidget::setKeyText()
{
	if (keyTable->currentItem()==0)
	{
		setKeyButton->setChecked(false);
		return;
	}
	if (setKeyButton->isChecked())
	{
		keyCode = 0;
		Part0 = "";
		Part1 = "";
		Part2 = "";
		Part3 = "";
		Part4 = "";
		grabKeyboard();
	}
	else
		releaseKeyboard();
}

void TabKeyboardShortcutsWidget::insertActions()
{
	lviToActionMap.clear();
	lviToMenuMap.clear();
	keyTable->clear();
	bool first, firstMenu=true;
	QTreeWidgetItem *currLVI = NULL;
	QTreeWidgetItem *currMenuLVI = NULL;
	QTreeWidgetItem *prevLVI = NULL;
	QTreeWidgetItem *prevMenuLVI = NULL;
	for (QVector< QPair<QString, QStringList> >::Iterator itmenu = defMenus->begin(); itmenu != defMenus->end(); ++itmenu )
	{
		if (firstMenu)
		{
			currMenuLVI = new QTreeWidgetItem(keyTable);
			firstMenu = false;
		}
		else
			currMenuLVI = new QTreeWidgetItem(keyTable, prevMenuLVI);
		Q_CHECK_PTR(currMenuLVI);
		lviToMenuMap.append(currMenuLVI);
		currMenuLVI->setText(0, itmenu->first);
		currMenuLVI->setExpanded(true);
		prevMenuLVI=currMenuLVI;
		first=true;
		currLVI=0;
		prevLVI=0;
		for ( QStringList::Iterator it = itmenu->second.begin(); it != itmenu->second.end(); ++it )
		{
			if (first)
			{
				currLVI = new QTreeWidgetItem(currMenuLVI);
				first = false;
			}
			else
				currLVI = new QTreeWidgetItem(currMenuLVI, prevLVI);
			Q_CHECK_PTR(currLVI);
			lviToActionMap.insert(currLVI, *it);
			currLVI->setText(0, keyMap[*it].cleanMenuText);
			currLVI->setText(1, keyMap[*it].keySequence);
			prevLVI=currLVI;
		}
	}
	//Non menu actions
	for (QVector< QPair<QString, QStringList> >::Iterator itmenu = defNonMenuActions->begin(); itmenu != defNonMenuActions->end(); ++itmenu )
	{
		if (firstMenu)
		{
			currMenuLVI = new QTreeWidgetItem(keyTable);
			firstMenu = false;
		}
		else
			currMenuLVI = new QTreeWidgetItem(keyTable, prevMenuLVI);
		Q_CHECK_PTR(currMenuLVI);
		lviToMenuMap.append(currMenuLVI);
		currMenuLVI->setText(0, itmenu->first);
		currMenuLVI->setExpanded(true);
		prevMenuLVI=currMenuLVI;
		first=true;
		currLVI=0;
		prevLVI=0;
		for ( QStringList::Iterator it = itmenu->second.begin(); it != itmenu->second.end(); ++it )
		{
			if (first)
			{
				currLVI=new QTreeWidgetItem(currMenuLVI);
				first=false;
			}
			else
				currLVI=new QTreeWidgetItem(currMenuLVI, prevLVI);
			Q_CHECK_PTR(currLVI);
			lviToActionMap.insert(currLVI, *it);
			currLVI->setText(0, keyMap[*it].cleanMenuText);
			currLVI->setText(1, keyMap[*it].keySequence);
			prevLVI=currLVI;
		}
	}
	keyTable->resizeColumnToContents(0);
}

void TabKeyboardShortcutsWidget::applySearch( const QString & newss )
{
	//Must run this as if newss is not empty and we go to the next for loop, the set visible doesnt work
	for (QList<QTreeWidgetItem*>::iterator it=lviToMenuMap.begin(); it!=lviToMenuMap.end(); ++it)
		(*it)->setHidden(false);
	if (newss.isEmpty())
	{			
		for (QMap<QTreeWidgetItem*, QString>::iterator it=lviToActionMap.begin(); it!=lviToActionMap.end(); ++it)
			it.key()->setHidden(false);
		return;
	}
	//Seem to need to do this.. isOpen doesnt seem to do what it says
	for (QMap<QTreeWidgetItem*, QString>::iterator it=lviToActionMap.begin(); it!=lviToActionMap.end(); ++it)
	{
		if (it.key()->text(0).contains(newss, Qt::CaseInsensitive))
			it.key()->setHidden(false);
		else
			it.key()->setHidden(true);
	}
}

void TabKeyboardShortcutsWidget::dispKey(QTreeWidgetItem* qlvi, QTreeWidgetItem*)
{
	if (qlvi!=0 && lviToActionMap.contains(qlvi))
	{
		selectedLVI=qlvi;
		QString actionName=lviToActionMap[qlvi];
		if (actionName.isEmpty())
			return;
		keyDisplay->setText(keyMap[actionName].keySequence);
		if (keyMap[actionName].keySequence.isEmpty())
			noKey->setChecked(true);
		else
			userDef->setChecked(true);
	}
	else
	{
		noKey->setChecked(true);
		keyDisplay->setText("");
		selectedLVI=0;
	}
}

bool TabKeyboardShortcutsWidget::event( QEvent* ev )
{
	bool ret = QWidget::event( ev );
	if ( ev->type() == QEvent::KeyPress )
		keyPressEvent((QKeyEvent*)ev);
	if ( ev->type() == QEvent::KeyRelease )
		keyReleaseEvent((QKeyEvent*)ev);
	return ret;
}

void TabKeyboardShortcutsWidget::keyPressEvent(QKeyEvent *k)
{
	if (setKeyButton->isChecked())
	{
		QStringList tl;
		Part4 = "";
		if (!keyDisplay->text().isEmpty())
		{
			tl = keyDisplay->text().split("+", QString::SkipEmptyParts);
			if (tl.count() > 0)
			{
				Part4 = tl[tl.count()-1];
				if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift") || Part4 == tr("Meta"))
					Part4 = "";
			}
		}		
		switch (k->key())
		{
			case Qt::Key_Meta:
				Part0 = tr("Meta+");
				keyCode |= Qt::META;
				break;
			case Qt::Key_Shift:
				Part3 = tr("Shift+");
				keyCode |= Qt::SHIFT;
				break;
			case Qt::Key_Alt:
				Part2 = tr("Alt+");
				keyCode |= Qt::ALT;
				break;
			case Qt::Key_Control:
				Part1 = tr("Ctrl+");
				keyCode |= Qt::CTRL;
				break;
			default:
				keyCode |= k->key();
				keyDisplay->setText(getKeyText(keyCode));
				releaseKeyboard();
				if (checkKey(keyCode))
				{
					QMessageBox::information(this, CommonStrings::trWarning,
											tr("This key sequence is already in use"),
											CommonStrings::tr_OK);
					selectedLVI->setText(1,keyMap[lviToActionMap[selectedLVI]].keySequence);
					keyDisplay->setText(keyMap[lviToActionMap[selectedLVI]].keySequence);
				}
				else
				{
					QKeySequence newKeySequence(keyCode);
					selectedLVI->setText(1, QString(newKeySequence));
					keyMap[lviToActionMap[selectedLVI]].keySequence=newKeySequence;
					userDef->setChecked(true);
				}
				setKeyButton->setChecked(false);
		}
	}
	if (setKeyButton->isChecked())
		keyDisplay->setText(Part0+Part1+Part2+Part3+Part4);
}

void TabKeyboardShortcutsWidget::keyReleaseEvent(QKeyEvent *k)
{
	if (setKeyButton->isChecked())
	{
		Part4 = "";
		if (!keyDisplay->text().isEmpty())
		{
			QStringList tl;
			tl = keyDisplay->text().split("+", QString::SkipEmptyParts);
			if (tl.count() > 0)
			{
				Part4 = tl[tl.count()-1];
				if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift") || Part4 == tr("Meta"))
					Part4 = "";
			}
		}
		if (k->key() == Qt::Key_Meta)
		{
			Part0 = "";
			keyCode &= ~Qt::META;
		}
		if (k->key() == Qt::Key_Shift)
		{
			Part3 = "";
			keyCode &= ~Qt::SHIFT;
		}
		if (k->key() == Qt::Key_Alt)
		{
			Part2 = "";
			keyCode &= ~Qt::ALT;
		}
		if (k->key() == Qt::Key_Control)
		{
			Part1 = "";
			keyCode &= ~Qt::CTRL;
		}
		keyDisplay->setText(Part0+Part1+Part2+Part3+Part4);
	}
}

bool TabKeyboardShortcutsWidget::checkKey(int code)
{
	bool ret = false;
	QKeySequence key = QKeySequence(code);
	for (QMap<QString,Keys>::Iterator it=keyMap.begin(); it!=keyMap.end(); ++it)
	{
		if (key.matches(it.value().keySequence) != QKeySequence::NoMatch)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void TabKeyboardShortcutsWidget::clearSearchString( )
{
	searchTextLineEdit->clear();
}
