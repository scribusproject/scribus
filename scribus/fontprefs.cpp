/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fontprefs.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
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


FontPrefs::FontPrefs( QWidget* parent, bool Hdoc, QString PPath, ScribusDoc* doc ) : QTabWidget( parent )
{
	RList = PrefsManager::instance()->appPrefs.GFontSub;
	HomeP = PPath;
	DocAvail = Hdoc;
	UsedFonts.clear();
	CurrentPath = "";
	docc = doc;
	setMinimumSize(fontMetrics().width( tr( "Available Fonts" )+ tr( "Font Substitutions" )+ tr( "Additional Paths" ))+180, 200);
	tab1 = new QWidget( this );
	tab1Layout = new QVBoxLayout( tab1 );
	tab1Layout->setMargin(10);
	tab1Layout->setSpacing(5);

	fontList = new FontListView(tab1);
	fontList->setModel(new FontListModel(fontList));

	tab1Layout->addWidget( fontList );
	addTab( tab1, tr( "&Available Fonts" ) );

	tab = new QWidget( this );
	tabLayout = new QVBoxLayout( tab );
	tabLayout->setMargin(10);
	tabLayout->setSpacing(5);

	Table3 = new QTableWidget(0, 2, tab );
	Table3->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Font Name")));
	Table3->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Replacement")));
	Table3->setSortingEnabled(false);
	Table3->setSelectionBehavior( QAbstractItemView::SelectRows );
	QHeaderView *header = Table3->horizontalHeader();
	header->setMovable(false);
	header->setClickable(false);
	header->setResizeMode(QHeaderView::Stretch);
	Table3->verticalHeader()->hide();
	Table3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabLayout->addWidget( Table3 );
	Layout2a = new QHBoxLayout;
	Layout2a->setMargin(0);
	Layout2a->setSpacing(5);
	QSpacerItem* spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2a->addItem( spacer1 );
	DelB = new QPushButton( tr( "&Delete" ), tab );
	DelB->setEnabled(false);
	Layout2a->addWidget( DelB );
	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2a->addItem( spacer2 );
	tabLayout->addLayout( Layout2a );
	addTab( tab, tr( "Font &Substitutions" ) );

	tab3 = new QWidget( this );
	tab3Layout = new QHBoxLayout( tab3 );
	tab3Layout->setMargin(10);
	tab3Layout->setSpacing(5);
	// If we're being called for global application preferences, not document
	// preferences, we let the user customize font search paths. Because things
	// go rather badly if paths are changed/removed while a doc is open, the
	// control is also not displayed if there is a document open.
	if (!DocAvail && !ScCore->primaryMainWindow()->HaveDoc)
	{
		PathList = new QListWidget( tab3 );
		readPaths();
		PathList->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		tab3Layout->addWidget( PathList );
		LayoutR = new QVBoxLayout;
		LayoutR->setMargin(0);
		LayoutR->setSpacing(5);
		ChangeB = new QPushButton( tr( "C&hange..." ), tab3 );
		LayoutR->addWidget( ChangeB );
		AddB = new QPushButton( tr( "A&dd..." ), tab3 );
		LayoutR->addWidget( AddB );
		RemoveB = new QPushButton( tr( "&Remove" ), tab3 );
		LayoutR->addWidget( RemoveB );
		QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
		LayoutR->addItem( spacer_2 );
		tab3Layout->addLayout( LayoutR );
		ChangeB->setEnabled(false);
		RemoveB->setEnabled(false);
		connect(PathList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelectPath(QListWidgetItem*)));
		connect(AddB, SIGNAL(clicked()), this, SLOT(AddPath()));
		connect(ChangeB, SIGNAL(clicked()), this, SLOT(ChangePath()));
		connect(RemoveB, SIGNAL(clicked()), this, SLOT(DelPath()));
	}
	else
	{
		// Rather than just making the tab vanish when editing doc-specific settings
		// (we don't support per-doc font paths), show a useful explanation.
		QLabel *whyBlankLabel = new QLabel("<qt>" +
				tr("Font search paths can only be set in File > Preferences, and only when "
				   "there is no document currently open. Close any open documents, then "
				   "use File > Preferences > Fonts to change the font search path.") + "</qt>",
				tab3);
		whyBlankLabel->setWordWrap(true);
		tab3Layout->addWidget( whyBlankLabel );
	}
	addTab( tab3, tr( "Additional &Paths" ) );

	// signals and slots connections
	connect(Table3, SIGNAL(cellClicked(int, int)), this, SLOT(ReplaceSel(int, int)));
	connect(DelB, SIGNAL(clicked()), this, SLOT(DelEntry()));
}

void FontPrefs::restoreDefaults()
{
	rebuildDialog();
}

void FontPrefs::ReplaceSel(int, int)
{
	DelB->setEnabled(true);
}

void FontPrefs::UpdateFliste()
{
	QString tmp;
	UsedFonts.clear();
	SCFonts fonts = PrefsManager::instance()->appPrefs.AvailFonts;
	SCFontsIterator it(fonts);
	for ( ; it.hasNext() ; it.next())
	{
		if (fonts[it.currentKey()].usable())
			UsedFonts.append(it.currentKey());
	}
	UsedFonts.sort();

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

void FontPrefs::DelEntry()
{
	int r = Table3->currentRow();
	QString tmp = Table3->item(r, 0)->text();
	Table3->removeRow(r);
	delete FlagsRepl.takeAt(r);
	RList.remove(tmp);
	DelB->setEnabled(false);
}

void FontPrefs::readPaths()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	PrefsContext *fontPrefsContext = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *fontPathTable = fontPrefsContext->getTable("ExtraFontDirs");
	PathList->clear();
	for (int i = 0; i < fontPathTable->getRowCount(); ++i)
		PathList->addItem( QDir::convertSeparators(fontPathTable->get(i,0)) );
}

void FontPrefs::writePaths()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	PrefsContext *fontPrefsContext = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *fontPathTable = fontPrefsContext->getTable("ExtraFontDirs");
	fontPathTable->clear();
	for (int i = 0; i < PathList->count(); ++i)
		fontPathTable->set(i, 0, QDir::fromNativeSeparators(PathList->item(i)->text()));
}

void FontPrefs::SelectPath(QListWidgetItem *c)
{
	if (!DocAvail)
	{
		ChangeB->setEnabled(true);
		RemoveB->setEnabled(true);
	}
	CurrentPath = c->text();
}

void FontPrefs::AddPath()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	CurrentPath = dirs->get("fontprefs", ".");
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), CurrentPath);
	if (!s.isEmpty())
	{
		dirs->set("fontprefs", s.left(s.lastIndexOf("/", -2)));
		if( s.endsWith("/") )
			s = s.left(s.length()-1);
		QString s2 = QDir::convertSeparators(s);
		if (PathList->findItems(s2, Qt::MatchExactly).count() != 0)
			return;
		PathList->addItem(s2);
		writePaths();
		ChangeB->setEnabled(false);
		RemoveB->setEnabled(false);
		CurrentPath = s;
		SCFonts* availFonts=&(PrefsManager::instance()->appPrefs.AvailFonts);
		QString dir = QDir::fromNativeSeparators(s2);
		availFonts->AddScalableFonts(dir +"/");
		availFonts->updateFontMap();
		rebuildDialog();
	}
}

void FontPrefs::ChangePath()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	QString s = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), CurrentPath);
	if (!s.isEmpty())
	{
		if( s.endsWith("/") )
			s = s.left(s.length()-1);
		QString s2 = QDir::convertSeparators(s2);
		if (PathList->findItems(s2, Qt::MatchExactly).count() != 0)
			return;
		QString path = PathList->currentItem()->text();
		SCFonts* availFonts=&(PrefsManager::instance()->appPrefs.AvailFonts);
		SCFontsIterator it(*availFonts);
		for ( ; it.hasNext(); it.next())
		{
			if (it.current().isNone())
				continue;
			QFileInfo fi(it.current().fontFilePath());
			if (fi.absolutePath() == path)
				availFonts->remove(it.currentKey());
		}
		PathList->currentItem()->setText(s2);
		writePaths();
		CurrentPath = s;
		QString dir = QDir::fromNativeSeparators(s2);
		availFonts->AddScalableFonts(dir +"/");
		availFonts->updateFontMap();
		rebuildDialog();
		ChangeB->setEnabled(false);
		RemoveB->setEnabled(false);
	}
}

void FontPrefs::DelPath()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	QFile fx(HomeP+"/scribusfont13.rc");
	QString path = PathList->currentItem()->text();
	if (fx.open(QIODevice::WriteOnly))
	{
		if (PathList->count() == 1)
			PathList->clear();
		else
			delete PathList->takeItem(PathList->currentRow());
		writePaths();
	}
	else
		return;
	SCFonts* availFonts=&(PrefsManager::instance()->appPrefs.AvailFonts);
	SCFontsIterator it(*availFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().isNone())
			continue;
		QFileInfo fi(it.current().fontFilePath());
		if (fi.absolutePath() == path)
			availFonts->remove(it.currentKey());
	}
	availFonts->updateFontMap();
	CurrentPath = "";
	rebuildDialog();
	ChangeB->setEnabled(false);
	RemoveB->setEnabled(false);
}

void FontPrefs::rebuildDialog()
{
// 	SCFonts* availFonts=&(PrefsManager::instance()->appPrefs.AvailFonts);
	fontList->setFonts(PrefsManager::instance()->appPrefs.AvailFonts);
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
	Table3->clearContents();
	Table3->setRowCount(PrefsManager::instance()->appPrefs.GFontSub.count());
	int a = 0;
	QMap<QString,QString>::Iterator itfsu;
	for (itfsu = RList.begin(); itfsu != RList.end(); ++itfsu)
	{
		QTableWidgetItem* tWidgetItem = new QTableWidgetItem(itfsu.key());
		tWidgetItem->setFlags(tWidgetItem->flags() & ~Qt::ItemIsEditable);
		Table3->setItem(a, 0, tWidgetItem);
		ScComboBox *item = new ScComboBox(Table3);
		item->setEditable(false);
		item->addItems(UsedFonts);
		setCurrentComboItem(item, itfsu.value());
		Table3->setCellWidget(a, 1, item);
		FlagsRepl.append(item);
		a++;
	}
	UpdateFliste();
}
