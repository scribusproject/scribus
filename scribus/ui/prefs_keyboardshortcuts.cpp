/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDomDocument>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "actionmanager.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "pluginmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefsstructs.h"
#include "scpaths.h"
#include "scplugin.h"
#include "scraction.h"
#include "ui/prefs_keyboardshortcuts.h"
#include "ui/scmessagebox.h"
#include "util.h"


Prefs_KeyboardShortcuts::Prefs_KeyboardShortcuts(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	defMenus=ActionManager::defaultMenus();
	defNonMenuActions=ActionManager::defaultNonMenuActions();

	QVector< QPair<QString, QStringList> >::Iterator itnmenua = defNonMenuActions->begin();
	PluginManager& pluginManager(PluginManager::instance());
	QStringList pluginNames(pluginManager.pluginNames(false));
	ScPlugin* plugin = NULL;
	ScActionPlugin* ixplug = NULL;
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
	keyCode = 0;
	keyDisplay->setMinimumWidth(fontMetrics().width("CTRL+ALT+SHIFT+W"));
	keyDisplay->setText("");
	selectedLVI = NULL;

	clearSearchButton->setIcon(IconManager::instance()->loadIcon("clear_right.png"));
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

Prefs_KeyboardShortcuts::~Prefs_KeyboardShortcuts()
{
}

void Prefs_KeyboardShortcuts::languageChange()
{
}

void Prefs_KeyboardShortcuts::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	keyMap=prefsData->keyShortcutPrefs.KeyActions;
	loadableSets->clear();
	loadableSets->addItems(scanForSets());
	insertActions();
	dispKey(0);
}

void Prefs_KeyboardShortcuts::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->keyShortcutPrefs.KeyActions=keyMap;
}

void Prefs_KeyboardShortcuts::setNoKey()
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

void Prefs_KeyboardShortcuts::loadKeySetFile()
{
	if (keySetList.contains(loadableSets->currentText()))
		importKeySet(keySetList[loadableSets->currentText()]);
}

void Prefs_KeyboardShortcuts::importKeySetFile()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString currentPath = dirs->get("keymapprefs_import", ScPaths::instance().shareDir() + "keysets/");
	QString s = QFileDialog::getOpenFileName(this, tr("Select a Key set file to read"), currentPath, tr("Key Set XML Files (*.xml)"));
	if (!s.isEmpty())
		importKeySet(s);
}
void Prefs_KeyboardShortcuts::exportKeySetFile()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString currentPath= dirs->get("keymapprefs_export", ".");
	QString s = QFileDialog::getSaveFileName(this, tr("Select a Key set file to save to"), currentPath, tr("Key Set XML Files (*.xml)") );
	if (!s.isEmpty())
		exportKeySet(s);
}

void Prefs_KeyboardShortcuts::importKeySet(QString filename)
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
			qDebug("%s", QString("Could not open key set file: %1\nError:%2 at line: %3, row: %4").arg(filename).arg(errorMsg).arg(eline).arg(ecol).toLatin1().constData());
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

bool Prefs_KeyboardShortcuts::exportKeySet(QString filename)
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
			QDomElement function_shortcut = doc.createElement("function");
			function_shortcut.setAttribute("name", it.key());
			function_shortcut.setAttribute("shortcut", getKeyText(it.value().keySequence));
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

void Prefs_KeyboardShortcuts::resetKeySet()
{
	QString location=ScPaths::instance().shareDir();
	QString defaultKeySetFileName=QDir::toNativeSeparators(location+"keysets/scribus15.xml");
	importKeySet(defaultKeySetFileName);
}

QStringList Prefs_KeyboardShortcuts::scanForSets()
{
	keySetList.clear();
	QString location=ScPaths::instance().shareDir();
	QString keySetLocation=QDir::toNativeSeparators(location+"keysets/");
	QDir keySetsDir(keySetLocation, "*.xml", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((keySetsDir.exists()) && (keySetsDir.count() != 0))
	{
		QStringList appNames;
		for (uint fileCounter = 0; fileCounter < keySetsDir.count(); ++fileCounter)
		{
			QString filename=QDir::toNativeSeparators(location+"keysets/"+keySetsDir[fileCounter]);

			QDomDocument doc( "keymapentries" );
			QFile file( filename );
			if ( !file.open( QIODevice::ReadOnly ) )
				continue;
			QString errorMsg;
			int eline;
			int ecol;

			if ( !doc.setContent( &file, &errorMsg, &eline, &ecol ))
			{
				qDebug("%s", QString("Could not open key set file: %1\nError:%2 at line: %3, row: %4").arg(keySetsDir[fileCounter]).arg(errorMsg).arg(eline).arg(ecol).toLatin1().constData());
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

QString Prefs_KeyboardShortcuts::getKeyText(QKeySequence KeyC)
{
	return KeyC.toString();
}

QString Prefs_KeyboardShortcuts::getTrKeyText(QKeySequence KeyC)
{
	return KeyC.toString(QKeySequence::NativeText);
}

void Prefs_KeyboardShortcuts::setKeyText()
{
	if (keyTable->currentItem()==0)
	{
		setKeyButton->setChecked(false);
		return;
	}
	if (setKeyButton->isChecked())
	{
		keyCode = 0;
		grabKeyboard();
	}
	else
		releaseKeyboard();
}

void Prefs_KeyboardShortcuts::insertActions()
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
		currMenuLVI->setFlags(Qt::ItemIsEnabled);
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
			currLVI->setText(1, keyMap[*it].keySequence.toString(QKeySequence::NativeText));
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
		currMenuLVI->setFlags(Qt::ItemIsEnabled);
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
			currLVI->setText(1, keyMap[*it].keySequence.toString(QKeySequence::NativeText));
			prevLVI=currLVI;
		}
	}
	keyTable->resizeColumnToContents(0);
}

void Prefs_KeyboardShortcuts::applySearch( const QString & newss )
{
	//Must run this as if newss is not empty and we go to the next for loop, the set visible doesn't work
	for (QList<QTreeWidgetItem*>::iterator it=lviToMenuMap.begin(); it!=lviToMenuMap.end(); ++it)
		(*it)->setHidden(false);
	if (newss.isEmpty())
	{
		for (QMap<QTreeWidgetItem*, QString>::iterator it=lviToActionMap.begin(); it!=lviToActionMap.end(); ++it)
			it.key()->setHidden(false);
		return;
	}
	//Seem to need to do this.. isOpen doesn't seem to do what it says
	for (QMap<QTreeWidgetItem*, QString>::iterator it=lviToActionMap.begin(); it!=lviToActionMap.end(); ++it)
	{
		if (it.key()->text(0).contains(newss, Qt::CaseInsensitive))
			it.key()->setHidden(false);
		else
			it.key()->setHidden(true);
	}
}

void Prefs_KeyboardShortcuts::dispKey(QTreeWidgetItem* qlvi, QTreeWidgetItem*)
{
	if (qlvi!=0 && lviToActionMap.contains(qlvi))
	{
		selectedLVI=qlvi;
		QString actionName=lviToActionMap[qlvi];
		if (actionName.isEmpty())
			return;
		keyDisplay->setText(keyMap[actionName].keySequence.toString(QKeySequence::NativeText));
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

bool Prefs_KeyboardShortcuts::event( QEvent* ev )
{
	bool ret = QWidget::event( ev );
	if ( ev->type() == QEvent::KeyPress )
		keyPressEvent((QKeyEvent*)ev);
	if ( ev->type() == QEvent::KeyRelease )
		keyReleaseEvent((QKeyEvent*)ev);
	return ret;
}

void Prefs_KeyboardShortcuts::keyPressEvent(QKeyEvent *k)
{
	if (setKeyButton->isChecked())
	{
		switch (k->key())
		{
			case Qt::Key_Meta:
				keyCode |= Qt::META;
				break;
			case Qt::Key_Shift:
				keyCode |= Qt::SHIFT;
				break;
			case Qt::Key_Alt:
				keyCode |= Qt::ALT;
				break;
			case Qt::Key_Control:
				keyCode |= Qt::CTRL;
				break;
			default:
				keyCode |= k->key();
				keyDisplay->setText(getTrKeyText(keyCode));
				releaseKeyboard();
				if (checkKey(keyCode))
				{
					ScMessageBox::information(this, CommonStrings::trWarning,
											tr("The %1 key sequence is already in use by \"%2\"")
												.arg(getTrKeyText(keyCode))
												.arg(getAction(keyCode)));
					selectedLVI->setText(1,keyMap[lviToActionMap[selectedLVI]].keySequence.toString(QKeySequence::NativeText));
					keyDisplay->setText(keyMap[lviToActionMap[selectedLVI]].keySequence.toString(QKeySequence::NativeText));
				}
				else
				{
					QKeySequence newKeySequence(keyCode);
					selectedLVI->setText(1, newKeySequence.toString(QKeySequence::NativeText));
					keyMap[lviToActionMap[selectedLVI]].keySequence=newKeySequence;
					userDef->setChecked(true);
				}
				setKeyButton->setChecked(false);
		}
	}
	if (setKeyButton->isChecked())
		keyDisplay->setText(getTrKeyText(keyCode));
}

void Prefs_KeyboardShortcuts::keyReleaseEvent(QKeyEvent *k)
{
	if (setKeyButton->isChecked())
	{
		if (k->key() == Qt::Key_Meta)
			keyCode &= ~Qt::META;
		if (k->key() == Qt::Key_Shift)
			keyCode &= ~Qt::SHIFT;
		if (k->key() == Qt::Key_Alt)
			keyCode &= ~Qt::ALT;
		if (k->key() == Qt::Key_Control)
			keyCode &= ~Qt::CTRL;
		keyDisplay->setText(getTrKeyText(keyCode));
	}
}

QString Prefs_KeyboardShortcuts::getAction(int code)
{
	QString ret;
	QKeySequence key = QKeySequence(code);
	for (QMap<QString,Keys>::Iterator it=keyMap.begin(); it!=keyMap.end(); ++it)
	{
		if (key.matches(it.value().keySequence) != QKeySequence::NoMatch)
		{
			ret = it->cleanMenuText;
			break;
		}
	}
	return ret;
}

bool Prefs_KeyboardShortcuts::checkKey(int code)
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

void Prefs_KeyboardShortcuts::clearSearchString( )
{
	searchTextLineEdit->clear();
}

