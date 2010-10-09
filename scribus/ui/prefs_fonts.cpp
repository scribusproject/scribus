/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_fonts.h"
#include "prefsstructs.h"


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
#include "sccombobox.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "util.h"
#include "util_color.h"
#include "util_icon.h"
#include "scpaths.h"
#include "fontlistmodel.h"
#include "fontlistview.h"

Prefs_Fonts::Prefs_Fonts(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	m_doc(doc)
{
	setupUi(this);

	RList = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub;
	UsedFonts.clear();
	CurrentPath = "";
	setMinimumSize(fontMetrics().width( tr( "Available Fonts" )+ tr( "Font Substitutions" )+ tr( "Additional Paths" ))+180, 200);

	fontListTableView->setModel(new FontListModel(fontListTableView));

	fontSubstitutionsTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Font Name")));
	fontSubstitutionsTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Replacement")));
	fontSubstitutionsTableWidget->setSortingEnabled(false);
	fontSubstitutionsTableWidget->setSelectionBehavior( QAbstractItemView::SelectRows );
	QHeaderView *header = fontSubstitutionsTableWidget->horizontalHeader();
	header->setMovable(false);
	header->setClickable(false);
	header->setResizeMode(QHeaderView::Stretch);
	fontSubstitutionsTableWidget->verticalHeader()->hide();
	fontSubstitutionsTableWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));


	// If we're being called for global application preferences, not document
	// preferences, we let the user customize font search paths. Because things
	// go rather badly if paths are changed/removed while a doc is open, the
	// control is also not displayed if there is a document open.
	if (m_doc==0 && !ScCore->primaryMainWindow()->HaveDoc)
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
	connect(fontSubstitutionsTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(ReplaceSel(int, int)));
	connect(deleteSubstitutionButton, SIGNAL(clicked()), this, SLOT(DelEntry()));



	//fontListTableView
//fontSubstitutionsTableWidget
}

Prefs_Fonts::~Prefs_Fonts()
{
}

void Prefs_Fonts::languageChange()
{
}

void Prefs_Fonts::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	// 	SCFonts* availFonts=&(PrefsManager::instance()->appPrefs.AvailFonts);
	m_availFonts=prefsData->fontPrefs.AvailFonts;
		fontListTableView->setFonts(m_availFonts);
		/*
		DON'T REMOVE THIS COMMENTS, PLEASE! (Petr)
		It's just a performance vs. functionality test.
		availFonts->clear();
		// FIXME: This is main preformance issue. It's about 90% of all preference reads! - PV
		availFonts->GetFonts(HomeP); */
		/* Are you wondering why this condition? See the comment at
		line #102 (or somewhere near) as reference. Hint: PathList
		is not initialized for example... - PV */
	/*	if (!DocAvail && !ScCore->primaryMainWindow()->HaveDoc)
		{
			for (uint a = 0; a < PathList->count(); ++a)
			{
				QString dir = ScPaths::separatorsToSlashes(PathList->text(a));
				availFonts->AddScalableFonts(dir +"/"); //, docc->DocName);
				availFonts->updateFontMap();
			}
		} */
	// 	UsedFonts.clear();
	// 	fontFlags.clear();
	// 	fontList->clear();
	// 	SCFontsIterator it(*availFonts);
	// 	for ( ; it.hasNext(); it.next())
	// 	{
	// 		if (it.current().isNone())
	// 			continue;
	// 		fontSet foS;
	// 		QTreeWidgetItem *row = new QTreeWidgetItem(fontList);
	// 		row->setText(0, it.currentKey());

	// 		foS.FlagUse = it.current().usable();
	// 		row->setIcon(1, foS.FlagUse ? checkOn : checkOff);
	// 		if (foS.FlagUse)
	// 			UsedFonts.append(it.currentKey());

	// 		foS.FlagPS = it.current().embedPs();
	// 		row->setIcon(2, foS.FlagPS ? checkOn : checkOff);

	// 		foS.FlagSub = it.current().subset();
	// 		row->setIcon(3, foS.FlagSub ? checkOn : checkOff);

	// 		ScFace::FontType type = it.current().type();
	// 		foS.FlagOTF = (type == ScFace::OTF) ? true : false;
	// 		if (it.current().isReplacement())
	// 			row->setIcon(0, substFont);
	// 		else if (type == ScFace::TYPE1)
	// 			row->setIcon(0, psFont);
	// 		else if (type == ScFace::TTF)
	// 			row->setIcon(0, ttfFont);
	// 		else if (type == ScFace::OTF)
	// 			row->setIcon(0, otfFont);

	// 		foS.FlagNames = it.current().hasNames();
	// 		row->setText(4, it.current().fontPath());
	// 		fontFlags.insert(it.currentKey(), foS);
	// 	}
	// 	fontList->sortByColumn(0, Qt::AscendingOrder);
	// 	fontList->resizeColumnToContents(0);
	// 	fontList->resizeColumnToContents(4);
	// 	UsedFonts.sort();
		FlagsRepl.clear();
		fontSubstitutionsTableWidget->clearContents();
		m_GFontSub=prefsData->fontPrefs.GFontSub;
		fontSubstitutionsTableWidget->setRowCount(m_GFontSub.count());
		int a = 0;
		QMap<QString,QString>::Iterator itfsu;
		for (itfsu = RList.begin(); itfsu != RList.end(); ++itfsu)
		{
			QTableWidgetItem* tWidgetItem = new QTableWidgetItem(itfsu.key());
			tWidgetItem->setFlags(tWidgetItem->flags() & ~Qt::ItemIsEditable);
			fontSubstitutionsTableWidget->setItem(a, 0, tWidgetItem);
			ScComboBox *item = new ScComboBox(fontSubstitutionsTableWidget);
			fontSubstitutionsTableWidget->setCellWidget(a, 1, item);
			item->setEditable(false);
			item->addItem(itfsu.value());
			setCurrentComboItem(item, itfsu.value());
			FlagsRepl.append(item);
			a++;
		}
		updateFontList();
}

void Prefs_Fonts::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	if (m_doc==0)
		writePaths();
	prefsData->fontPrefs.GFontSub.clear();
	uint a = 0;
	for (QMap<QString,QString>::ConstIterator itfsu = RList.begin(); itfsu != RList.end(); ++itfsu)
		prefsData->fontPrefs.GFontSub[itfsu.key()] = FlagsRepl.at(a++)->currentText();

}

void Prefs_Fonts::ReplaceSel(int, int)
{
	deleteSubstitutionButton->setEnabled(true);
}

void Prefs_Fonts::updateFontList()
{
	UsedFonts.clear();
	SCFontsIterator it(m_availFonts);
	for ( ; it.hasNext() ; it.next())
	{
		if (m_availFonts[it.currentKey()].usable())
			UsedFonts.append(it.currentKey());
	}
	UsedFonts.sort();
	QString tmp;
	for (int b = 0; b < FlagsRepl.count(); ++b)
	{
		tmp = FlagsRepl.at(b)->currentText();
		FlagsRepl.at(b)->clear();
		FlagsRepl.at(b)->addItems(UsedFonts);
		if (UsedFonts.contains(tmp) != 0)
			setCurrentComboItem(FlagsRepl.at(b), tmp);
		else
			FlagsRepl.at(b)->setCurrentIndex(0);
	}
}

void Prefs_Fonts::DelEntry()
{
	int r = fontSubstitutionsTableWidget->currentRow();
	QString tmp = fontSubstitutionsTableWidget->item(r, 0)->text();
	fontSubstitutionsTableWidget->removeRow(r);
	delete FlagsRepl.takeAt(r);
	RList.remove(tmp);
	deleteSubstitutionButton->setEnabled(false);
}

void Prefs_Fonts::readPaths()
{
	Q_ASSERT(m_doc==0); // should never be called in doc-specific prefs
	PrefsContext *fontPrefsContext = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *fontPathTable = fontPrefsContext->getTable("ExtraFontDirs");
	pathListWidget->clear();
	for (int i = 0; i < fontPathTable->getRowCount(); ++i)
		pathListWidget->addItem( QDir::convertSeparators(fontPathTable->get(i,0)) );
}

void Prefs_Fonts::writePaths() const
{
	Q_ASSERT(m_doc==0); // should never be called in doc-specific prefs
	PrefsContext *fontPrefsContext = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *fontPathTable = fontPrefsContext->getTable("ExtraFontDirs");
	fontPathTable->clear();
	for (int i = 0; i < pathListWidget->count(); ++i)
		fontPathTable->set(i, 0, QDir::fromNativeSeparators(pathListWidget->item(i)->text()));
}

void Prefs_Fonts::SelectPath(QListWidgetItem *c)
{
	if (m_doc==0)
	{
		changeButton->setEnabled(true);
		removeButton->setEnabled(true);
	}
	CurrentPath = c->text();
}

void Prefs_Fonts::AddPath()
{
	Q_ASSERT(m_doc==0); // should never be called in doc-specific prefs
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	CurrentPath = dirs->get("fontprefs", ".");
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), CurrentPath);
	if (!s.isEmpty())
	{
		dirs->set("fontprefs", s.left(s.lastIndexOf("/", -2)));
		if( s.endsWith("/") )
			s.chop(1);
		QString s2 = QDir::convertSeparators(s);
		if (pathListWidget->findItems(s2, Qt::MatchExactly).count() != 0)
			return;
		pathListWidget->addItem(s2);
		//writePaths();
		changeButton->setEnabled(false);
		removeButton->setEnabled(false);
		CurrentPath = s;
		QString dir(QDir::fromNativeSeparators(s2));
		m_availFonts.AddScalableFonts(dir +"/");
		m_availFonts.updateFontMap();
		updateFontList();
	}
}

void Prefs_Fonts::ChangePath()
{
	Q_ASSERT(m_doc==0); // should never be called in doc-specific prefs
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), CurrentPath);
	if (!s.isEmpty())
	{
		if( s.endsWith("/") )
			s.chop(1);
		QString s2 = QDir::convertSeparators(s);
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
		CurrentPath = s;
		QString dir = QDir::fromNativeSeparators(s2);
		m_availFonts.AddScalableFonts(dir +"/");
		m_availFonts.updateFontMap();
		updateFontList();
		changeButton->setEnabled(false);
		removeButton->setEnabled(false);
	}
}

void Prefs_Fonts::DelPath()
{
	Q_ASSERT(m_doc==0); // should never be called in doc-specific prefs
	QFile fx(PrefsManager::instance()->preferencesLocation()+"/scribusfont13.rc");
	QString path = pathListWidget->currentItem()->text();
	if (fx.open(QIODevice::WriteOnly))
	{
		if (pathListWidget->count() == 1)
			pathListWidget->clear();
		else
			delete pathListWidget->takeItem(pathListWidget->currentRow());
		//writePaths();
	}
	else
		return;
	SCFontsIterator it(m_availFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().isNone())
			continue;
		QFileInfo fi(it.current().fontFilePath());
		if (fi.absolutePath() == path)
			m_availFonts.remove(it.currentKey());
	}
	m_availFonts.updateFontMap();
	CurrentPath = "";
	updateFontList();
	changeButton->setEnabled(false);
	removeButton->setEnabled(false);
}

