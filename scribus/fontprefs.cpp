#include "fontprefs.h"
#include "fontprefs.moc"
#include <qhbox.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include "prefscontext.h"
#include "prefsfile.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "prefsmanager.h"
#include "scconfig.h"

extern QPixmap loadIcon(QString nam);
extern ScribusApp *ScApp;

FontPrefs::FontPrefs( QWidget* parent,  SCFonts &flist, bool Hdoc, QString PPath, ScribusDoc* doc ) : QTabWidget( parent, "fpre" )
{
	RList = PrefsManager::instance()->appPrefs.GFontSub;
	HomeP = PPath;
	DocAvail = Hdoc;
	UsedFonts.clear();
	CurrentPath = "";
	docc = doc;
	setMinimumSize(fontMetrics().width( tr( "Available Fonts" )+ tr( "Font Substitutions" )+ tr( "Additional Paths" ))+180, 200);
	tab1 = new QWidget( this, "tab1" );
	tab1Layout = new QVBoxLayout( tab1, 0, 5, "tab1Layout");
	fontList = new QListView(tab1, "fontList" );
	fontList->setAllColumnsShowFocus(true);
	fontList->setShowSortIndicator(true);
	fontList->addColumn( tr("Font Name", "font preview"));
	fontList->addColumn( tr("Use Font", "font preview"));
	fontList->addColumn( tr("Embed in:", "font preview"));
	fontList->addColumn( tr("Subset", "font preview"));
	fontList->setColumnAlignment(3, Qt::AlignCenter);
	fontList->addColumn( tr("Path to Font File", "font preview"));
	SCFontsIterator it(flist);
	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	okIcon = loadIcon("ok.png");
	empty = QPixmap(16,16);
	empty.fill(white);
	for ( ; it.current(); ++it)
	{
		fontSet foS;
		QListViewItem *row = new QListViewItem(fontList);
		row->setText(0, it.currentKey());
		if (it.current()->UseFont)
		{
			UsedFonts.append(it.currentKey());
			foS.FlagUse = true;
			row->setPixmap(1, okIcon);
		}
		else
		{
			foS.FlagUse = false;
			row->setPixmap(1, empty);
		}
		row->setText(2, tr("Postscript"));
		if (it.current()->EmbedPS)
		{
			foS.FlagPS = true;
			row->setPixmap(2, okIcon);
		}
		else
		{
			foS.FlagPS = false;
			row->setPixmap(2, empty);
		}
		Foi::FontType type = it.current()->typeCode;
		if (type == Foi::OTF)
			foS.FlagOTF = true;
		else
			foS.FlagOTF = false;
		if (it.current()->Subset)
		{
			foS.FlagSub = true;
			row->setPixmap(3, okIcon);
		}
		else
		{
			foS.FlagSub = false;
			row->setPixmap(3, empty);
		}
		if (type == Foi::TYPE1)
			row->setPixmap(0, psFont);
		else
		{
			if (type == Foi::TTF)
				row->setPixmap(0, ttfFont);
			if (type == Foi::OTF)
				row->setPixmap(0, otfFont);
		}
		row->setText(4, it.current()->fontPath());
		fontFlags.insert(it.currentKey(), foS);
	}
	fontList->setSorting(0);
	fontList->sort();
	UsedFonts.sort();
	//fontList->setSorting(-1);
	tab1Layout->addWidget( fontList );
	insertTab( tab1, tr( "&Available Fonts" ) );

	tab = new QWidget( this, "tab" );
	tabLayout = new QVBoxLayout( tab, 10, 5, "tabLayout");
	Table3 = new QTable( tab, "Repl" );
	Table3->setSorting(false);
	Table3->setSelectionMode(QTable::SingleRow);
	Table3->setLeftMargin(0);
	Table3->verticalHeader()->hide();
	Table3->setNumCols( 2 );
	Table3->setNumRows(PrefsManager::instance()->appPrefs.GFontSub.count());
	Header2 = Table3->horizontalHeader();
	Header2->setLabel(0, tr("Font Name"));
	Header2->setLabel(1, tr("Replacement"));
	int a = 0;
	QMap<QString,QString>::Iterator itfsu;
	for (itfsu = RList.begin(); itfsu != RList.end(); ++itfsu)
	{
		Table3->setText(a, 0, itfsu.key());
		QComboBox *item = new QComboBox( true, this, "Replace" );
		item->setEditable(false);
		item->insertStringList(UsedFonts);
		item->setCurrentText(itfsu.data());
		Table3->setCellWidget(a, 1, item);
		FlagsRepl.append(item);
		a++;
	}
	Table3->setColumnStretchable(0, true);
	Table3->setColumnStretchable(1, true);
	Table3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabLayout->addWidget( Table3 );
	Layout2a = new QHBoxLayout( 0, 0, 5, "Layout2");
	QSpacerItem* spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2a->addItem( spacer1 );
	DelB = new QPushButton( tr( "&Delete" ), tab, "DelB" );
	DelB->setEnabled(false);
	Layout2a->addWidget( DelB );
	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2a->addItem( spacer2 );
	tabLayout->addLayout( Layout2a );
	insertTab( tab, tr( "Font &Substitutions" ) );

	tab3 = new QWidget( this, "tab3" );
	tab3Layout = new QHBoxLayout( tab3, 10, 5, "tab3Layout");
	// If we're being called for global application preferences, not document
	// preferences, we let the user customize font search paths. Because things
	// go rather badly if paths are changed/removed while a doc is open, the
	// control is also not displayed if there is a document open.
	if (!DocAvail && !ScApp->HaveDoc)
	{
		PathList = new QListBox( tab3, "PathList" );
		readPaths();
		PathList->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		tab3Layout->addWidget( PathList );
		LayoutR = new QVBoxLayout( 0, 0, 5, "LayoutR");
		ChangeB = new QPushButton( tr( "C&hange..." ), tab3, "ChangeB" );
		LayoutR->addWidget( ChangeB );
		AddB = new QPushButton( tr( "A&dd..." ), tab3, "AddB" );
		LayoutR->addWidget( AddB );
		RemoveB = new QPushButton( tr( "&Remove" ), tab3, "RemoveB" );
		LayoutR->addWidget( RemoveB );
		QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
		LayoutR->addItem( spacer_2 );
		tab3Layout->addLayout( LayoutR );
		ChangeB->setEnabled(false);
		RemoveB->setEnabled(false);
		connect(PathList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(SelectPath(QListBoxItem*)));
		connect(AddB, SIGNAL(clicked()), this, SLOT(AddPath()));
		connect(ChangeB, SIGNAL(clicked()), this, SLOT(ChangePath()));
		connect(RemoveB, SIGNAL(clicked()), this, SLOT(DelPath()));
	}
	else
	{
		// Rather than just making the tab vanish when editing doc-specific settings
		// (we don't support per-doc font paths), show a useful explanation.
		QLabel *whyBlankLabel = new QLabel(
				tr("<qt>Font search paths can only be set in Preferences, and only when "
				   "there is no document currently open. Close any open documents, then "
				   "use Edit->Settings to change the font search path.</qt>"),
				tab3, "whyBlankLabel");
		tab3Layout->addWidget( whyBlankLabel );
	}
	insertTab( tab3, tr( "Additional &Paths" ) );

	// signals and slots connections
	connect(Table3, SIGNAL(currentChanged(int, int)), this, SLOT(ReplaceSel(int, int)));
	connect(DelB, SIGNAL(clicked()), this, SLOT(DelEntry()));
	connect(fontList, SIGNAL(clicked(QListViewItem *, const QPoint &, int)), this, SLOT(slotClick(QListViewItem*, const QPoint &, int)));
}

void FontPrefs::restoreDefaults()
{
}

void FontPrefs::slotClick(QListViewItem* ite, const QPoint &, int col)
{
	if (ite == NULL)
		return;
	QString tmp = ite->text(0);
	if ((col == 1) && (!DocAvail))
	{
		fontFlags[tmp].FlagUse = !fontFlags[tmp].FlagUse;
		if (fontFlags[tmp].FlagUse)
			ite->setPixmap(1, okIcon);
		else
			ite->setPixmap(1, empty);
	}
	if (col == 2)
	{
		fontFlags[tmp].FlagPS = !fontFlags[tmp].FlagPS;
		if (fontFlags[tmp].FlagPS)
			ite->setPixmap(2, okIcon);
		else
			ite->setPixmap(2, empty);
	}
	if ((col == 3) && (!fontFlags[tmp].FlagOTF))
	{
		fontFlags[tmp].FlagSub = !fontFlags[tmp].FlagSub;
		if (fontFlags[tmp].FlagSub)
			ite->setPixmap(3, okIcon);
		else
			ite->setPixmap(3, empty);
	}
	UpdateFliste();
}

void FontPrefs::ReplaceSel(int, int)
{
	DelB->setEnabled(true);
}

void FontPrefs::UpdateFliste()
{
	QString tmp;
	UsedFonts.clear();
	SCFontsIterator it(PrefsManager::instance()->appPrefs.AvailFonts);
	for ( ; it.current() ; ++it)
	{
		if (fontFlags[it.currentKey()].FlagUse)
			UsedFonts.append(it.currentKey());
	}
	UsedFonts.sort();
	for (uint b = 0; b < FlagsRepl.count(); ++b)
	{
		tmp = FlagsRepl.at(b)->currentText();
		FlagsRepl.at(b)->clear();
		FlagsRepl.at(b)->insertStringList(UsedFonts);
		if (UsedFonts.contains(tmp) != 0)
			FlagsRepl.at(b)->setCurrentText(tmp);
		else
			FlagsRepl.at(b)->setCurrentItem(0);
	}
}

void FontPrefs::DelEntry()
{
	int r = Table3->currentRow();
	QString tmp = Table3->text(r, 0);
	Table3->removeRow(r);
	FlagsRepl.remove(r);
	RList.remove(tmp);
}

void FontPrefs::readPaths()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	PrefsContext *fontPrefsContext = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *fontPathTable = fontPrefsContext->getTable("ExtraFontDirs");
	PathList->clear();
	for (int i = 0; i < fontPathTable->getRowCount(); ++i)
		PathList->insertItem(fontPathTable->get(i,0));
}

void FontPrefs::writePaths()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	PrefsContext *fontPrefsContext = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *fontPathTable = fontPrefsContext->getTable("ExtraFontDirs");
	fontPathTable->clear();
	for (uint i = 0; i < PathList->count(); ++i)
		fontPathTable->set(i, 0, PathList->text(i));
}

void FontPrefs::SelectPath(QListBoxItem *c)
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
	QString s = QFileDialog::getExistingDirectory(CurrentPath, this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
	{
		dirs->set("fontprefs", s.left(s.findRev("/", -2)));
		if( s.endsWith("/") )
			s = s.left(s.length()-1);
		if (PathList->findItem(s))
			return;
		PathList->insertItem(s);
		writePaths();
		ChangeB->setEnabled(true);
		RemoveB->setEnabled(true);
		CurrentPath = s;
		RebuildDialog();
	}
}

void FontPrefs::ChangePath()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	QString s = QFileDialog::getExistingDirectory(CurrentPath, this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
	{
		s = s.left(s.length()-1);
		if (PathList->findItem(s))
			return;
		PathList->changeItem(s, PathList->currentItem());
		writePaths();
		CurrentPath = s;
		RebuildDialog();
		ChangeB->setEnabled(true);
		RemoveB->setEnabled(true);
	}
}

void FontPrefs::DelPath()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	QFile fx(HomeP+"/scribusfont13.rc");
	if (fx.open(IO_WriteOnly))
	{
		if (PathList->count() == 1)
			PathList->clear();
		else
			PathList->removeItem(PathList->currentItem());
		writePaths();
	}
	else
		return;
	CurrentPath = "";
	RebuildDialog();
	bool setter = PathList->count() > 0 ? true : false;
	ChangeB->setEnabled(setter);
	RemoveB->setEnabled(setter);
}

void FontPrefs::RebuildDialog()
{
	SCFonts* availFonts=&(PrefsManager::instance()->appPrefs.AvailFonts);
	availFonts->clear();
	availFonts->GetFonts(HomeP);
	if (DocAvail)
	{
		for (uint a = 0; a < PathList->count(); ++a)
		{
			availFonts->AddScalableFonts(PathList->text(a)+"/", docc->DocName);
			availFonts->updateFontMap();
		}
	}
	UsedFonts.clear();
	fontFlags.clear();
	fontList->clear();
	SCFontsIterator it(*availFonts);
	for ( ; it.current(); ++it)
	{
		fontSet foS;
		QListViewItem *row = new QListViewItem(fontList);
		row->setText(0, it.currentKey());
		if (it.current()->UseFont)
		{
			UsedFonts.append(it.currentKey());
			foS.FlagUse = true;
			row->setPixmap(1, okIcon);
		}
		else
		{
			foS.FlagUse = false;
			row->setPixmap(1, empty);
		}
		row->setText(2, tr("Postscript"));
		if (it.current()->EmbedPS)
		{
			foS.FlagPS = true;
			row->setPixmap(2, okIcon);
		}
		else
		{
			foS.FlagPS = false;
			row->setPixmap(2, empty);
		}
		Foi::FontType type = it.current()->typeCode;
		if (type == Foi::OTF)
			foS.FlagOTF = true;
		else
			foS.FlagOTF = false;
		if (it.current()->Subset)
		{
			foS.FlagSub = true;
			row->setPixmap(3, okIcon);
		}
		else
		{
			foS.FlagSub = false;
			row->setPixmap(3, empty);
		}
		if (type == Foi::TYPE1)
			row->setPixmap(0, psFont);
		else
		{
			if (type == Foi::TTF)
				row->setPixmap(0, ttfFont);
			if (type == Foi::OTF)
				row->setPixmap(0, otfFont);
		}
		row->setText(4, it.current()->fontPath());
		fontFlags.insert(it.currentKey(), foS);
	}
	//fontList->setSorting(0);
	fontList->sort();
	UsedFonts.sort();
	//fontList->setSorting(-1);
	UpdateFliste();
}
