/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fontprefs.h"
//#include "fontprefs.moc"
#include <q3hbox.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <q3filedialog.h>
#include <qlabel.h>
#include <qcursor.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <QPixmap>
#include <Q3VBoxLayout>

#include "prefscontext.h"
#include "prefsfile.h"
#include "sccombobox.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "colorutil.h"
#include "util.h"

extern QPixmap loadIcon(QString nam);


FontPrefs::FontPrefs( QWidget* parent, bool Hdoc, QString PPath, ScribusDoc* doc ) : QTabWidget( parent, "fpre" )
{
	RList = PrefsManager::instance()->appPrefs.GFontSub;
	HomeP = PPath;
	DocAvail = Hdoc;
	UsedFonts.clear();
	CurrentPath = "";
	docc = doc;
	setMinimumSize(fontMetrics().width( tr( "Available Fonts" )+ tr( "Font Substitutions" )+ tr( "Additional Paths" ))+180, 200);
	tab1 = new QWidget( this, "tab1" );
	tab1Layout = new Q3VBoxLayout( tab1, 0, 5, "tab1Layout");
	fontList = new Q3ListView(tab1, "fontList" );
	fontList->setAllColumnsShowFocus(true);
	fontList->setShowSortIndicator(true);
	fontList->addColumn( tr("Font Name", "font preview"));
	fontList->addColumn( tr("Use Font", "font preview"));
	fontList->setColumnAlignment(1, Qt::AlignCenter);
	fontList->addColumn( tr("Embed in PostScript", "font preview"));
	fontList->setColumnAlignment(2, Qt::AlignCenter);
	fontList->addColumn( tr("Subset", "font preview"));
	fontList->setColumnAlignment(3, Qt::AlignCenter);
	fontList->addColumn( tr("Path to Font File", "font preview"));
	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	substFont = loadIcon("font_subst16.png");

	checkOn = getQCheckBoxPixmap(true, fontList->paletteBackgroundColor());
	checkOff = getQCheckBoxPixmap(false, fontList->paletteBackgroundColor());

	tab1Layout->addWidget( fontList );
	insertTab( tab1, tr( "&Available Fonts" ) );

	tab = new QWidget( this, "tab" );
	tabLayout = new Q3VBoxLayout( tab, 10, 5, "tabLayout");
	Table3 = new Q3Table( tab, "Repl" );
	Table3->setSorting(false);
	Table3->setSelectionMode(Q3Table::SingleRow);
	Table3->setLeftMargin(0);
	Table3->verticalHeader()->hide();
	Table3->setNumCols( 2 );
	Table3->setNumRows(PrefsManager::instance()->appPrefs.GFontSub.count());
	Header2 = Table3->horizontalHeader();
	Header2->setLabel(0, tr("Font Name"));
	Header2->setLabel(1, tr("Replacement"));
// 	int a = 0;
// 	QMap<QString,QString>::Iterator itfsu;
// 	for (itfsu = RList.begin(); itfsu != RList.end(); ++itfsu)
// 	{
// 		Table3->setText(a, 0, itfsu.key());
// 		ScComboBox *item = new ScComboBox( true, this, "Replace" );
// 		item->setEditable(false);
// 		item->insertStringList(UsedFonts);
// 		item->setCurrentText(itfsu.data());
// 		Table3->setCellWidget(a, 1, item);
// 		FlagsRepl.append(item);
// 		a++;
// 	}
	Table3->setColumnStretchable(0, true);
	Table3->setColumnStretchable(1, true);
	Table3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	tabLayout->addWidget( Table3 );
	Layout2a = new Q3HBoxLayout( 0, 0, 5, "Layout2");
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
	tab3Layout = new Q3HBoxLayout( tab3, 10, 5, "tab3Layout");
	// If we're being called for global application preferences, not document
	// preferences, we let the user customize font search paths. Because things
	// go rather badly if paths are changed/removed while a doc is open, the
	// control is also not displayed if there is a document open.
	if (!DocAvail && !ScCore->primaryMainWindow()->HaveDoc)
	{
		PathList = new Q3ListBox( tab3, "PathList" );
		readPaths();
		PathList->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		tab3Layout->addWidget( PathList );
		LayoutR = new Q3VBoxLayout( 0, 0, 5, "LayoutR");
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
		connect(PathList, SIGNAL(highlighted(Q3ListBoxItem*)), this, SLOT(SelectPath(Q3ListBoxItem*)));
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
				tab3, "whyBlankLabel");
		tab3Layout->addWidget( whyBlankLabel );
	}
	insertTab( tab3, tr( "Additional &Paths" ) );

	// signals and slots connections
	connect(Table3, SIGNAL(currentChanged(int, int)), this, SLOT(ReplaceSel(int, int)));
	connect(DelB, SIGNAL(clicked()), this, SLOT(DelEntry()));
	connect(fontList, SIGNAL(clicked(Q3ListViewItem *, const QPoint &, int)),
			this, SLOT(slotClick(Q3ListViewItem*, const QPoint &, int)));
}

void FontPrefs::restoreDefaults()
{
	rebuildDialog();
}

void FontPrefs::slotClick(Q3ListViewItem* ite, const QPoint &, int col)
{
	if (ite == NULL || ite == 0)
		return;
	QString tmp = ite->text(0);
	if ((col == 1) && (!DocAvail))
	{
		fontFlags[tmp].FlagUse = !fontFlags[tmp].FlagUse;
		if (fontFlags[tmp].FlagUse)
			ite->setPixmap(1, checkOn);
		else
			ite->setPixmap(1, checkOff);
	}
	if (col == 2)
	{
		fontFlags[tmp].FlagPS = !fontFlags[tmp].FlagPS;
		if (fontFlags[tmp].FlagPS)
			ite->setPixmap(2, checkOn);
		else
			ite->setPixmap(2, checkOff);
	}
	if ((col == 3) && (!fontFlags[tmp].FlagOTF) && (fontFlags[tmp].FlagNames))
	{
		fontFlags[tmp].FlagSub = !fontFlags[tmp].FlagSub;
		if (fontFlags[tmp].FlagSub)
			ite->setPixmap(3, checkOn);
		else
			ite->setPixmap(3, checkOff);
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
	for ( ; it.hasNext() ; it.next())
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

void FontPrefs::SelectPath(Q3ListBoxItem *c)
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
	QString s = Q3FileDialog::getExistingDirectory(CurrentPath, this, "d", tr("Choose a Directory"), true);
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
		rebuildDialog();
	}
}

void FontPrefs::ChangePath()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	QString s = Q3FileDialog::getExistingDirectory(CurrentPath, this, "d", tr("Choose a Directory"), true);
	if (!s.isEmpty())
	{
		s = s.left(s.length()-1);
		if (PathList->findItem(s))
			return;
		PathList->changeItem(s, PathList->currentItem());
		writePaths();
		CurrentPath = s;
		rebuildDialog();
		ChangeB->setEnabled(true);
		RemoveB->setEnabled(true);
	}
}

void FontPrefs::DelPath()
{
	Q_ASSERT(!DocAvail); // should never be called in doc-specific prefs
	QFile fx(HomeP+"/scribusfont13.rc");
	if (fx.open(QIODevice::WriteOnly))
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
	rebuildDialog();
	bool setter = PathList->count() > 0 ? true : false;
	ChangeB->setEnabled(setter);
	RemoveB->setEnabled(setter);
}

void FontPrefs::rebuildDialog()
{
	SCFonts* availFonts=&(PrefsManager::instance()->appPrefs.AvailFonts);

	/*
	DON'T REMOVE THIS COMMENTS, PLEASE! (Petr)
	It's just a performance vs. functionality test.
	availFonts->clear();
	// FIXME: This is main preformance issue. It's about 90% of all preference reads! - PV
	availFonts->GetFonts(HomeP); */
	/* Are you wondering why this condition? See the comment at
	line #102 (or somewhere near) as reference. Hint: PathList
	is not initialized for example... - PV */
	if (!DocAvail && !ScCore->primaryMainWindow()->HaveDoc)
	{
		for (uint a = 0; a < PathList->count(); ++a)
		{
			availFonts->AddScalableFonts(PathList->text(a)+"/"); //, docc->DocName);
			availFonts->updateFontMap();
		}
	}
	UsedFonts.clear();
	fontFlags.clear();
	fontList->clear();
	SCFontsIterator it(*availFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().isNone())
			continue;
		fontSet foS;
		Q3ListViewItem *row = new Q3ListViewItem(fontList);
		row->setText(0, it.currentKey());

		foS.FlagUse = it.current().usable();
		row->setPixmap(1, foS.FlagUse ? checkOn : checkOff);
		if (foS.FlagUse)
			UsedFonts.append(it.currentKey());

		foS.FlagPS = it.current().embedPs();
		row->setPixmap(2, foS.FlagPS ? checkOn : checkOff);

		foS.FlagSub = it.current().subset();
		row->setPixmap(3, foS.FlagSub ? checkOn : checkOff);

		ScFace::FontType type = it.current().type();
		foS.FlagOTF = (type == ScFace::OTF) ? true : false;
		if (it.current().isReplacement())
			row->setPixmap(0, substFont);
		else if (type == ScFace::TYPE1)
			row->setPixmap(0, psFont);
		else if (type == ScFace::TTF)
			row->setPixmap(0, ttfFont);
		else if (type == ScFace::OTF)
			row->setPixmap(0, otfFont);

		foS.FlagNames = it.current().hasNames();
		row->setText(4, it.current().fontPath());
		fontFlags.insert(it.currentKey(), foS);
	}
	fontList->sort();
	UsedFonts.sort();
	FlagsRepl.clear();
	int a = 0;
	QMap<QString,QString>::Iterator itfsu;
	for (itfsu = RList.begin(); itfsu != RList.end(); ++itfsu)
	{
		Table3->setText(a, 0, itfsu.key());
		ScComboBox *item = new ScComboBox( true, this, "Replace" );
		item->setEditable(false);
		item->insertStringList(UsedFonts);
		item->setCurrentText(itfsu.data());
		Table3->setCellWidget(a, 1, item);
		FlagsRepl.append(item);
		a++;
	}
	UpdateFliste();
}
