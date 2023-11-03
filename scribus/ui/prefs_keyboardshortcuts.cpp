/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QDomDocument>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "actionmanager.h"
#include "api/api_application.h"
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


Prefs_KeyboardShortcuts::Prefs_KeyboardShortcuts(QWidget* parent, ScribusDoc* /*doc*/)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Keyboard Shortcuts");
	m_icon = "16/preferences-desktop-keyboard-shortcuts.png";

	defMenus = ActionManager::defaultMenus();
	defNonMenuActions = ActionManager::defaultNonMenuActions();

	QVector< QPair<QString, QStringList> >::Iterator itnmenua = defNonMenuActions->begin();
	PluginManager& pluginManager(PluginManager::instance());
	QStringList pluginNames(pluginManager.pluginNames(false));
	ScPlugin* plugin = nullptr;
	ScActionPlugin* ixplug = nullptr;
	QString pName;
	for (int i = 0; i < pluginNames.count(); ++i)
	{
		pName = pluginNames.at(i);
		plugin = pluginManager.getPlugin(pName, true);
		Q_ASSERT(plugin); // all the returned names should represent loaded plugins
		if (plugin->inherits("ScActionPlugin"))
		{
			ixplug = qobject_cast<ScActionPlugin*>(plugin);
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
	keyDisplay->setMinimumWidth(fontMetrics().horizontalAdvance("CTRL+ALT+SHIFT+W"));
	keyDisplay->setText("");
	selectedLVI = nullptr;

	clearSearchButton->setIcon(IconManager::instance().loadIcon("clear_right.png"));
	// signals and slots connections
	connect( keyTable, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(dispKey(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect( noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect( setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
	connect( loadSetButton, SIGNAL(clicked()), this, SLOT(loadKeySetFile()));
	connect( importSetButton, SIGNAL(clicked()), this, SLOT(importKeySetFile()));
	connect( exportSetButton, SIGNAL(clicked()), this, SLOT(exportKeySetFile()));
	connect( resetSetButton, SIGNAL(clicked()), this, SLOT(resetKeySet()));
	connect( clearSearchButton, SIGNAL(clicked()), this, SLOT(clearSearchString()));
	connect( searchTextLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(applySearch(const QString&)));

}

Prefs_KeyboardShortcuts::~Prefs_KeyboardShortcuts() = default;

void Prefs_KeyboardShortcuts::languageChange()
{
	// No need to do anything here, the UI language cannot change while prefs dialog is opened
}

void Prefs_KeyboardShortcuts::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	keyMap = prefsData->keyShortcutPrefs.KeyActions;
	loadableSets->clear();
	loadableSets->addItems(scanForSets());
	insertActions();
	dispKey(nullptr);
}

void Prefs_KeyboardShortcuts::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->keyShortcutPrefs.KeyActions = keyMap;
}

void Prefs_KeyboardShortcuts::setNoKey()
{
	if (noKey->isChecked())
	{
		if (selectedLVI != nullptr)
		{
			selectedLVI->setText(1, "");
			keyMap[lviToActionMap[selectedLVI]].keySequence = QKeySequence();
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
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString currentPath = dirs->get("keymapprefs_import", ScPaths::instance().shareDir() + "keysets/");
	QString s = QFileDialog::getOpenFileName(this, tr("Select a Key set file to read"), currentPath, tr("Key Set XML Files (*.xml)"));
	if (!s.isEmpty())
		importKeySet(s);
}
void Prefs_KeyboardShortcuts::exportKeySetFile()
{
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString currentPath= dirs->get("keymapprefs_export", ".");
	QString s = QFileDialog::getSaveFileName(this, tr("Select a Key set file to save to"), currentPath, tr("Key Set XML Files (*.xml)") );
	if (!s.isEmpty())
		exportKeySet(s);
}

void Prefs_KeyboardShortcuts::importKeySet(const QString& filename)
{
	searchTextLineEdit->clear();

	QFileInfo fi(filename);
	if (!fi.exists())
		return;

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
		qDebug("%s", QString("Could not open key set file: %1\nError:%2 at line: %3, row: %4").arg(filename, errorMsg).arg(eline).arg(ecol).toLatin1().constData());
		file1.close();
		return;
	}
	file1.close();

	//load the file now
	QDomElement docElem = doc.documentElement();
	if (docElem.tagName() == "shortcutset" && docElem.hasAttribute("name"))
	{
		QDomAttr keysetAttr = docElem.attributeNode( "name" );

		//clear current menu entries
		for (QMap<QString,Keys>::Iterator it = keyMap.begin(); it != keyMap.end(); ++it)
			it.value().keySequence = QKeySequence();

		//load in new set
		for (QDomNode n = docElem.firstChild(); !n.isNull(); n = n.nextSibling())
		{
			QDomElement e = n.toElement();
			if (e.isNull())
				continue;
			if (e.hasAttribute("name") && e.hasAttribute("shortcut"))
			{
				QDomAttr nameAttr = e.attributeNode("name");
				QDomAttr shortcutAttr = e.attributeNode("shortcut");
				if (keyMap.contains(nameAttr.value()))
					keyMap[nameAttr.value()].keySequence = QKeySequence(shortcutAttr.value());
			}
		}
	}

	insertActions();
}

bool Prefs_KeyboardShortcuts::exportKeySet(const QString& filename)
{
	QString exportFileName;
	if (filename.endsWith(".xml"))
		exportFileName = filename;
	else
		exportFileName = filename+".xml";
	if (overwrite(this, exportFileName))
	{
		bool ok;
		QString setName = QInputDialog::getText(this, tr("Export Keyboard Shortcuts to File"), tr("Enter the name of the shortcut set:"), QLineEdit::Normal, QString(), &ok);
		if (!( ok && !setName.isEmpty()) )
			return false;

		QDomDocument doc( "keymapentries" );
		QString keyset = QString("<shortcutset name=\"%1\"></shortcutset>").arg(setName);
		doc.setContent(keyset);
		QDomElement keySetElement = doc.documentElement();
		QMap<QString, Keys>::Iterator itEnd = keyMap.end();
		for (QMap<QString, Keys>::Iterator it = keyMap.begin(); it != itEnd; ++it)
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
	QString location = ScPaths::instance().shareDir();
	QString defaultKeySetFileName = QDir::toNativeSeparators(location + "keysets/scribus15.xml");
	importKeySet(defaultKeySetFileName);
}

QStringList Prefs_KeyboardShortcuts::scanForSets()
{
	keySetList.clear();
	QString location(ScPaths::instance().shareDir());
	QDir keySetsDir(QDir::toNativeSeparators(location+"keysets/"), "*.xml", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((!keySetsDir.exists()) || (keySetsDir.count() <= 0))
		return QStringList();

	QStringList appNames;
	for (uint fileCounter = 0; fileCounter < keySetsDir.count(); ++fileCounter)
	{
		QString filename(QDir::toNativeSeparators(location+"keysets/"+keySetsDir[fileCounter]));

		QDomDocument doc( "keymapentries" );
		QFile file( filename );
		if (!file.open( QIODevice::ReadOnly))
			continue;
		QString errorMsg;
		int eline;
		int ecol;

		if (!doc.setContent( &file, &errorMsg, &eline, &ecol ))
		{
			qDebug("%s", QString("Could not open key set file: %1\nError:%2 at line: %3, row: %4").arg(keySetsDir[fileCounter], errorMsg).arg(eline).arg(ecol).toLatin1().constData());
			file.close();
			continue;
		}
		file.close();

		QDomElement docElem = doc.documentElement();
		if (docElem.tagName() == "shortcutset" && docElem.hasAttribute("name"))
		{
			QDomAttr nameAttr = docElem.attributeNode( "name" );
			if(nameAttr.value().contains(ScribusAPI::getVersionScribus().remove(".svn")))
				appNames.prepend(nameAttr.value());
			else
				appNames.append(nameAttr.value());
			keySetList.insert(nameAttr.value(), filename);
		}
	}
	return QStringList(appNames);
}

QString Prefs_KeyboardShortcuts::getKeyText(const QKeySequence& KeyC)
{
	return KeyC.toString();
}

QString Prefs_KeyboardShortcuts::getTrKeyText(const QKeySequence& KeyC)
{
	return KeyC.toString(QKeySequence::NativeText);
}

void Prefs_KeyboardShortcuts::setKeyText()
{
	if (keyTable->currentItem() == nullptr)
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
	bool first = true;
	bool firstMenu = true;
	QTreeWidgetItem* currLVI = nullptr;
	QTreeWidgetItem* currMenuLVI = nullptr;
	QTreeWidgetItem* prevLVI = nullptr;
	QTreeWidgetItem* prevMenuLVI = nullptr;

	lviToActionMap.clear();
	lviToMenuMap.clear();
	keyTable->clear();

	for (int i = 0; i < defMenus->count(); ++i)
	{
		const QPair<QString, QStringList> &actionStrings = defMenus->at(i);
		if (firstMenu)
		{
			currMenuLVI = new QTreeWidgetItem(keyTable);
			firstMenu = false;
		}
		else
			currMenuLVI = new QTreeWidgetItem(keyTable, prevMenuLVI);
		Q_CHECK_PTR(currMenuLVI);
		lviToMenuMap.append(currMenuLVI);
		currMenuLVI->setText(0, actionStrings.first);
		currMenuLVI->setExpanded(true);
		currMenuLVI->setFlags(Qt::ItemIsEnabled);
		prevMenuLVI = currMenuLVI;
		first = true;
		currLVI = nullptr;
		prevLVI = nullptr;
		for (int j = 0; j < actionStrings.second.count(); ++j)
		{
			QString actionName = actionStrings.second.at(j);
			if (!keyMap.contains(actionName))
			{
				qDebug() << "The action " << actionName << " is not defined in shortcut map";
				continue;
			}
			const Keys &actionKeys = keyMap[actionName];
			if (actionKeys.cleanMenuText.isEmpty())
				continue;
			if (first)
			{
				currLVI = new QTreeWidgetItem(currMenuLVI);
				first = false;
			}
			else
				currLVI = new QTreeWidgetItem(currMenuLVI, prevLVI);
			Q_CHECK_PTR(currLVI);
			lviToActionMap.insert(currLVI, actionName);
			currLVI->setText(0, actionKeys.cleanMenuText);
			currLVI->setText(1, actionKeys.keySequence.toString(QKeySequence::NativeText));
			prevLVI = currLVI;
		}
	}
	//Non menu actions
	for (int i = 0; i < defNonMenuActions->count(); ++i)
	{
		const QPair<QString, QStringList> &actionStrings = defNonMenuActions->at(i);
		if (firstMenu)
		{
			currMenuLVI = new QTreeWidgetItem(keyTable);
			firstMenu = false;
		}
		else
			currMenuLVI = new QTreeWidgetItem(keyTable, prevMenuLVI);
		Q_CHECK_PTR(currMenuLVI);
		lviToMenuMap.append(currMenuLVI);
		currMenuLVI->setText(0, actionStrings.first);
		currMenuLVI->setExpanded(true);
		currMenuLVI->setFlags(Qt::ItemIsEnabled);
		prevMenuLVI = currMenuLVI;
		first = true;
		currLVI = nullptr;
		prevLVI = nullptr;
		for (int j = 0; j < actionStrings.second.count(); ++j)
		{
			QString actionName = actionStrings.second.at(j);
			if (!keyMap.contains(actionName))
			{
				qDebug() << "The action " << actionName << " is not defined in shortcut map";
				continue;
			}
			const Keys &actionKeys = keyMap[actionName];
			if (actionKeys.cleanMenuText.isEmpty())
				continue;
			if (first)
			{
				currLVI = new QTreeWidgetItem(currMenuLVI);
				first = false;
			}
			else
				currLVI = new QTreeWidgetItem(currMenuLVI, prevLVI);
			Q_CHECK_PTR(currLVI);
			lviToActionMap.insert(currLVI, actionName);
			currLVI->setText(0, actionKeys.cleanMenuText);
			currLVI->setText(1, actionKeys.keySequence.toString(QKeySequence::NativeText));
			prevLVI = currLVI;
		}
	}
	keyTable->resizeColumnToContents(0);
}

void Prefs_KeyboardShortcuts::applySearch( const QString & newss )
{
	//Must run this as if newss is not empty and we go to the next for loop, the set visible doesn't work
	for (auto it = lviToMenuMap.begin(); it != lviToMenuMap.end(); ++it)
		(*it)->setHidden(false);
	if (newss.isEmpty())
	{
		for (auto it = lviToActionMap.begin(); it != lviToActionMap.end(); ++it)
			it.key()->setHidden(false);
		return;
	}
	//Seem to need to do this.. isOpen doesn't seem to do what it says
	for (auto it = lviToActionMap.begin(); it != lviToActionMap.end(); ++it)
	{
		if (it.key()->text(0).contains(newss, Qt::CaseInsensitive))
			it.key()->setHidden(false);
		else
			it.key()->setHidden(true);
	}
}

void Prefs_KeyboardShortcuts::dispKey(QTreeWidgetItem* qlvi, QTreeWidgetItem*)
{
	if (setKeyButton->isChecked())
	{
		releaseKeyboard();
		setKeyButton->setChecked(false);
	}
	if (qlvi != nullptr && lviToActionMap.contains(qlvi))
	{
		selectedLVI = qlvi;
		QString actionName = lviToActionMap[qlvi];
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
		selectedLVI = nullptr;
	}
	noKey->setEnabled(selectedLVI != nullptr);
	userDef->setEnabled(selectedLVI != nullptr);
	setKeyButton->setEnabled(selectedLVI != nullptr);
	keyDisplay->setEnabled(selectedLVI != nullptr);
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
	if (!setKeyButton->isChecked())
		return;

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
			if (selectedLVI)
			{
				QString actionName = lviToActionMap[selectedLVI];
				if (checkKey(keyCode))
				{
					ScMessageBox::information(this, CommonStrings::trWarning, tr("The %1 key sequence is already in use by \"%2\"").arg(getTrKeyText(keyCode),getAction(keyCode)));
					selectedLVI->setText(1,keyMap[actionName].keySequence.toString(QKeySequence::NativeText));
					keyDisplay->setText(keyMap[actionName].keySequence.toString(QKeySequence::NativeText));
				}
				else
				{
					QKeySequence newKeySequence(keyCode);
					selectedLVI->setText(1, newKeySequence.toString(QKeySequence::NativeText));
					keyMap[actionName].keySequence = newKeySequence;
					userDef->setChecked(true);
				}
			}
			setKeyButton->setChecked(false);
	}

	if (setKeyButton->isChecked())
		keyDisplay->setText(getTrKeyText(keyCode));
}

void Prefs_KeyboardShortcuts::keyReleaseEvent(QKeyEvent *k)
{
	if (!setKeyButton->isChecked())
		return;

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

QString Prefs_KeyboardShortcuts::getAction(int code)
{
	QKeySequence key(code);
	for (QMap<QString,Keys>::Iterator it = keyMap.begin(); it != keyMap.end(); ++it)
	{
		if (key.matches(it.value().keySequence) != QKeySequence::NoMatch)
			return it->cleanMenuText;
	}
	return QString();
}

bool Prefs_KeyboardShortcuts::checkKey(int code)
{
	QKeySequence key(code);
	for (QMap<QString,Keys>::Iterator it = keyMap.begin(); it != keyMap.end(); ++it)
	{
		if (key.matches(it.value().keySequence) != QKeySequence::NoMatch)
			return true;
	}
	return false;
}

void Prefs_KeyboardShortcuts::clearSearchString( )
{
	searchTextLineEdit->clear();
}

