/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_fonts.h"
#include "prefsstructs.h"


#include <QComboBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QSpacerItem>
#include <QPixmap>

#include "prefscontext.h"
#include "prefsfile.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "util.h"
#include "util_color.h"
#include "scpaths.h"
#include "fontlistmodel.h"
#include "fontlistview.h"

Prefs_Fonts::Prefs_Fonts(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	m_doc(doc)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Fonts");
	m_icon = "16/preferences-desktop-font.png";

	RList = PrefsManager::instance().appPrefs.fontPrefs.GFontSub;

	setMinimumSize(fontMetrics().horizontalAdvance( tr( "Available Fonts" )+ tr( "Font Substitutions" )+ tr( "Additional Paths" )+ tr( "Rejected Fonts" ))+180, 200);

	fontListTableView->setModel(new FontListModel(fontListTableView, m_doc, true));

	fontSubstitutionsTableWidget->setRowCount(RList.count());
	fontSubstitutionsTableWidget->setColumnCount(2);
	fontSubstitutionsTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Font Name")));
	fontSubstitutionsTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Replacement")));
	fontSubstitutionsTableWidget->setSortingEnabled(false);
	fontSubstitutionsTableWidget->setSelectionBehavior( QAbstractItemView::SelectRows );
	QHeaderView *header = fontSubstitutionsTableWidget->horizontalHeader();
	header->setSectionsMovable(false);
	header->setSectionsClickable(false);
	header->setSectionResizeMode(QHeaderView::Stretch);
	fontSubstitutionsTableWidget->verticalHeader()->hide();
	fontSubstitutionsTableWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));


	// If we're being called for global application preferences, not document
	// preferences, we let the user customize font search paths. Because things
	// go rather badly if paths are changed/removed while a doc is open, the
	// control is also not displayed if there is a document open.
	if (m_doc == nullptr && !ScCore->primaryMainWindow()->HaveDoc)
	{
		whyBlankLabel->resize(0,0);
		whyBlankLabel->hide();
		readPaths();
		changeButton->setEnabled(false);
		removeButton->setEnabled(false);
		connect(pathListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelectPath(QListWidgetItem*)));
		connect(addButton, SIGNAL(clicked()), this, SLOT(AddPath()));
		connect(changeButton, SIGNAL(clicked()), this, SLOT(ChangePath()));
		connect(removeButton, SIGNAL(clicked()), this, SLOT(DelPath()));
	}
	else
	{
		pathListWidget->resize(0,0);
		changeButton->resize(0,0);
		addButton->resize(0,0);
		removeButton->resize(0,0);
		pathListWidget->hide();
		changeButton->hide();
		addButton->hide();
		removeButton->hide();

		// Rather than just making the tab vanish when editing doc-specific settings
		// (we don't support per-doc font paths), show a useful explanation.
		whyBlankLabel->setText("<qt>" +
				tr("Font search paths can only be set in File > Preferences, and only when "
				   "there is no document currently open. Close any open documents, then "
				   "use File > Preferences > Fonts to change the font search path.") + "</qt>");
		additionalPathsButtonsVerticalSpacer->changeSize(0,0);
		//additionalPathsButtonsVerticalSpacer->invalidate();
		additionalPathsLabelVerticalSpacer->changeSize(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding);
		additionalPathsLabelVerticalSpacer->invalidate();
	}

	// signals and slots connections
	connect(fontSubstitutionsTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(ReplaceSel()));
	connect(deleteSubstitutionButton, SIGNAL(clicked()), this, SLOT(DelEntry()));



	//fontListTableView
//fontSubstitutionsTableWidget
}

void Prefs_Fonts::languageChange()
{
	// No need to do anything here, the UI language cannot change while prefs dialog is opened
}

void Prefs_Fonts::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	m_availFonts = prefsData->fontPrefs.AvailFonts;
	fontListTableView->setFonts(m_availFonts);

	FlagsRepl.clear();
	fontSubstitutionsTableWidget->clearContents();
	m_GFontSub = prefsData->fontPrefs.GFontSub;

	int i = 0;
	for (auto itfsu = RList.begin(); itfsu != RList.end(); ++itfsu)
	{
		QTableWidgetItem* tWidgetItem = new QTableWidgetItem(itfsu.key());
		tWidgetItem->setFlags(tWidgetItem->flags() & ~Qt::ItemIsEditable);
		fontSubstitutionsTableWidget->setItem(i, 0, tWidgetItem);
		auto item = new QComboBox(fontSubstitutionsTableWidget);
		fontSubstitutionsTableWidget->setCellWidget(i, 1, item);
		item->setEditable(false);
		item->addItem(itfsu.value());
		setCurrentComboItem(item, itfsu.value());
		FlagsRepl.append(item);
		i++;
	}
	deleteSubstitutionButton->setEnabled(false);

	fontsRejectedTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	auto headerView = fontsRejectedTableWidget->horizontalHeader();
	headerView->resizeSection(0, 150);
	headerView->resizeSection(1, 250);
	headerView->setStretchLastSection(true);
	
	updateFontList();
	updateRejectedFontList();
}

void Prefs_Fonts::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	int fontCount = fontListTableView->model()->rowCount();
	for (int i = 0; i < fontCount; ++i)
	{
		QString fontName = fontListTableView->fontName(i);
		ScFace face = m_availFonts[fontName];

		face.usable(fontListTableView->isFontUsable(i));
		face.subset(fontListTableView->isFontSubsetted(i));
	}

	if (m_doc == nullptr)
		writePaths();

	prefsData->fontPrefs.GFontSub.clear();
	uint a = 0;
	for (QMap<QString,QString>::ConstIterator itfsu = RList.begin(); itfsu != RList.end(); ++itfsu)
		prefsData->fontPrefs.GFontSub[itfsu.key()] = FlagsRepl.at(a++)->currentText();
}

void Prefs_Fonts::changeUnit(ApplicationPrefs *prefsData) const
{
	//Blank, no units on this tab, yet
}

void Prefs_Fonts::ReplaceSel()
{
	int selCount = fontSubstitutionsTableWidget->selectedItems().count();
	deleteSubstitutionButton->setEnabled(selCount > 0);
}

void Prefs_Fonts::updateFontList()
{
	m_usedFonts.clear();
	SCFontsIterator it(m_availFonts);
	for ( ; it.hasNext() ; it.next())
	{
		if (m_availFonts[it.currentKey()].usable())
			m_usedFonts.append(it.currentKey());
	}
	m_usedFonts.sort();

	QString tmp;
	for (int b = 0; b < FlagsRepl.count(); ++b)
	{
		tmp = FlagsRepl.at(b)->currentText();
		FlagsRepl.at(b)->clear();
		FlagsRepl.at(b)->addItems(m_usedFonts);
		if (m_usedFonts.contains(tmp) != 0)
			setCurrentComboItem(FlagsRepl.at(b), tmp);
		else
			FlagsRepl.at(b)->setCurrentIndex(0);
	}
}

void Prefs_Fonts::updateRejectedFontList()
{
	const auto& rejectedFonts = m_availFonts.rejectedFonts;

	fontsRejectedTableWidget->clearContents();
	
	int i = 0;
	for (auto it = rejectedFonts.cbegin(); it != rejectedFonts.cend(); ++it)
	{
		const auto& key = it.key();
		const auto& value = it.value();
		const auto  baseName = QFileInfo(key).baseName();

		fontsRejectedTableWidget->insertRow(i);
		fontsRejectedTableWidget->setItem(i, 0, new QTableWidgetItem(baseName));
		fontsRejectedTableWidget->setItem(i, 1, new QTableWidgetItem(value));
		fontsRejectedTableWidget->setItem(i, 2, new QTableWidgetItem(key));
	}

	fontsRejectedTableWidget->sortItems(0, Qt::AscendingOrder);
}

void Prefs_Fonts::DelEntry()
{
	// This works a because selection mode is "Full rows"
	QList<QTableWidgetItem*> selItems = fontSubstitutionsTableWidget->selectedItems();
	for (int i = 0; i < selItems.count(); ++i)
	{
		QTableWidgetItem* item = selItems.at(i);
		int r = item->row();
		QString tmp = fontSubstitutionsTableWidget->item(r, 0)->text();
		fontSubstitutionsTableWidget->removeRow(r);
		delete FlagsRepl.takeAt(r);
		RList.remove(tmp);
	}
	deleteSubstitutionButton->setEnabled(false);
}

void Prefs_Fonts::readPaths()
{
	Q_ASSERT(m_doc == nullptr); // should never be called in doc-specific prefs
	PrefsContext *fontPrefsContext = PrefsManager::instance().prefsFile->getContext("Fonts");
	PrefsTable *fontPathTable = fontPrefsContext->getTable("ExtraFontDirs");
	pathListWidget->clear();
	for (int i = 0; i < fontPathTable->getRowCount(); ++i)
		pathListWidget->addItem( QDir::toNativeSeparators(fontPathTable->get(i, 0)) );
}

void Prefs_Fonts::writePaths() const
{
	Q_ASSERT(m_doc == nullptr); // should never be called in doc-specific prefs
	PrefsContext *fontPrefsContext = PrefsManager::instance().prefsFile->getContext("Fonts");
	PrefsTable *fontPathTable = fontPrefsContext->getTable("ExtraFontDirs");
	fontPathTable->clear();
	for (int i = 0; i < pathListWidget->count(); ++i)
		fontPathTable->set(i, 0, QDir::fromNativeSeparators(pathListWidget->item(i)->text()));
}

void Prefs_Fonts::SelectPath(QListWidgetItem *c)
{
	if (m_doc == nullptr)
	{
		changeButton->setEnabled(true);
		removeButton->setEnabled(true);
	}
	m_currentPath = c->text();
}

void Prefs_Fonts::AddPath()
{
	Q_ASSERT(m_doc == nullptr); // should never be called in doc-specific prefs
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	m_currentPath = dirs->get("fontprefs", ".");
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), m_currentPath);
	if (s.isEmpty())
		return;

	dirs->set("fontprefs", s.left(s.lastIndexOf("/", -2)));
	if (s.endsWith("/"))
		s.chop(1);
	QString s2 = QDir::toNativeSeparators(s);
	if (pathListWidget->findItems(s2, Qt::MatchExactly).count() != 0)
		return;
	pathListWidget->addItem(s2);
	//writePaths();
	changeButton->setEnabled(false);
	removeButton->setEnabled(false);
	m_currentPath = s;
	QString dir(QDir::fromNativeSeparators(s2));
	m_availFonts.addScalableFonts(dir +"/");
	m_availFonts.updateFontMap();
	m_availFonts.writeFontCache();

	updateFontList();
	updateRejectedFontList();
}

void Prefs_Fonts::ChangePath()
{
	Q_ASSERT(m_doc == nullptr); // should never be called in doc-specific prefs
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), m_currentPath);
	if (s.isEmpty())
		return;

	if (s.endsWith("/"))
		s.chop(1);
	QString s2 = QDir::toNativeSeparators(s);
	if (pathListWidget->findItems(s2, Qt::MatchExactly).count() != 0)
		return;
	QString path = pathListWidget->currentItem()->text();
	SCFontsIterator it(m_availFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().isNone())
			continue;
		QFileInfo fi(it.current().fontFilePath());
		if (fi.absolutePath() == path)
			m_availFonts.remove(it.currentKey());
	}
	pathListWidget->currentItem()->setText(s2);
	//writePaths();
	m_currentPath = s;
	QString dir = QDir::fromNativeSeparators(s2);
	m_availFonts.addScalableFonts(dir +"/");
	m_availFonts.updateFontMap();
	updateFontList();
	updateRejectedFontList();
	changeButton->setEnabled(false);
	removeButton->setEnabled(false);
}

void Prefs_Fonts::DelPath()
{
	Q_ASSERT(m_doc == nullptr); // should never be called in doc-specific prefs
	QFile fx(PrefsManager::instance().preferencesLocation() + "/scribusfont13.rc");
	if (!fx.open(QIODevice::WriteOnly))
		return;

	QString path = pathListWidget->currentItem()->text();
	if (pathListWidget->count() == 1)
		pathListWidget->clear();
	else
		delete pathListWidget->takeItem(pathListWidget->currentRow());
	//writePaths();

	QMutableMapIterator<QString,ScFace> it(m_availFonts);
	while (it.hasNext())
	{
		it.next();
		if (it.value().isNone())
			continue;
		QFileInfo fi(it.value().fontFilePath());
		if (fi.absolutePath() == path)
			m_availFonts.remove(it.key());
	}
	m_availFonts.updateFontMap();
	m_currentPath.clear();
	updateFontList();
	changeButton->setEnabled(false);
	removeButton->setEnabled(false);
}

