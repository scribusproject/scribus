#include "fontprefs.h"
#include "fontprefs.moc"
#include <qhbox.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include "prefscontext.h"
#include "prefsfile.h"
#include "scribusdoc.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

extern QPixmap loadIcon(QString nam);
extern PrefsFile* prefsFile;

FontPrefs::FontPrefs( QWidget* parent,  SCFonts &flist, bool Hdoc, ApplicationPrefs *prefs, QString PPath, ScribusDoc* doc ) : QTabWidget( parent, "fpre" )
{
	Prefs = prefs;
	RList = Prefs->GFontSub;
	HomeP = PPath;
	DocAvail = Hdoc;
	UsedFonts.clear();
	CurrentPath = "";
	docc = doc;
	setMinimumSize(fontMetrics().width( tr( "Available Fonts" )+ tr( "Font Substitutions" )+ tr( "Additional Paths" ))+180, 200);
	tab1 = new QWidget( this, "tab1" );
	tab1Layout = new QVBoxLayout( tab1, 10, 5, "tab1Layout");
	Table1 = new QTable( tab1, "FONT" );
	Table1->setNumRows( flist.count() );
	Table1->setNumCols( 6 );
	Table1->setSorting(true);
	SCFontsIterator it(flist);
	int a = 0;
	for ( ; it.current(); ++it)
	{
		Table1->setText(a, 0, it.currentKey());
		QCheckBox *cp = new QCheckBox(this, "use");
		cp->setText(" ");
		cp->setChecked(it.current()->UseFont);
		if (it.current()->UseFont)
			UsedFonts.append(it.currentKey());
		cp->setEraseColor(white);
		if (Hdoc)
			cp->setEnabled(false);
		connect(cp, SIGNAL(clicked()), this, SLOT(UpdateFliste()));
		FlagsUse.append(cp);
		Table1->setCellWidget(a, 1, cp);
		QCheckBox *cp2 = new QCheckBox(this, "ps");
		cp2->setText( tr("Postscript"));
		cp2->setChecked(it.current()->EmbedPS);
		cp2->setEraseColor(white);
		FlagsPS.append(cp2);
		Table1->setCellWidget(a, 2, cp2);
		QFileInfo fi = QFileInfo(it.current()->Datei);
		QString ext = fi.extension(false).lower();
		QCheckBox *cp3 = new QCheckBox(this, "su");
		cp3->setText( tr("Yes"));
		if (ext == "otf")
		{
			cp3->setChecked(true); 			// Open Type Fonts are always Subsetted
			cp3->setEnabled(false);
		}
		else
			cp3->setChecked(it.current()->Subset);
		cp3->setEraseColor(white);
		FlagsSub.append(cp3);
		Table1->setCellWidget(a, 3, cp3);
		if ((ext == "pfa") || (ext == "pfb"))
			Table1->setPixmap(a, 4, loadIcon("font_type1.png"));
		else
		{
			if (ext == "ttf")
				Table1->setPixmap(a, 4, loadIcon("font_truetype.png"));
			if (ext == "otf")
				Table1->setPixmap(a, 4, loadIcon("font_otf.png"));
		}
		Table1->setText(a, 5, it.current()->Datei);
		++a;
	}
	UsedFonts.sort();
	Table1->sortColumn(0, true, true);
	Table1->setSorting(false);
	Table1->setSelectionMode(QTable::NoSelection);
	Table1->setLeftMargin(0);
	Table1->verticalHeader()->hide();
	Header = Table1->horizontalHeader();
	QString tmp_head[] = { tr("Font Name"), tr("Use Font"), tr("Embed in:"),
	                       tr("Subset"), tr("Type"), tr("Path to Font File")};
	size_t array_head = sizeof(tmp_head) / sizeof(*tmp_head);
	for (uint a = 0; a < array_head; ++a)
		Header->setLabel(a, tmp_head[a]);
	Table1->adjustColumn(0);
	Table1->adjustColumn(1);
	Table1->setColumnWidth(2, 110);
	Table1->adjustColumn(3);
	Table1->adjustColumn(4);
	Table1->adjustColumn(5);
	Table1->setColumnMovingEnabled(false);
	Table1->setRowMovingEnabled(false);
	Table1->setColumnReadOnly(1, true);
	Table1->setColumnReadOnly(2, true);
	Table1->setColumnReadOnly(3, true);
	Header->setMovingEnabled(false);
	tab1Layout->addWidget( Table1 );
	insertTab( tab1, tr( "&Available Fonts" ) );

	tab = new QWidget( this, "tab" );
	tabLayout = new QVBoxLayout( tab, 10, 5, "tabLayout");
	Table3 = new QTable( tab, "Repl" );
	Table3->setSorting(false);
	Table3->setSelectionMode(QTable::SingleRow);
	Table3->setLeftMargin(0);
	Table3->verticalHeader()->hide();
	Table3->setNumCols( 2 );
	Table3->setNumRows(Prefs->GFontSub.count());
	Header2 = Table3->horizontalHeader();
	Header2->setLabel(0, tr("Font Name"));
	Header2->setLabel(1, tr("Replacement"));
	a = 0;
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
	PathList = new QListBox( tab3, "PathList" );
	ReadPath();
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
	insertTab( tab3, tr( "Additional &Paths" ) );
//	if (Hdoc)
//		setTabEnabled(tab3, false);
	ChangeB->setEnabled(false);
	RemoveB->setEnabled(false);

	// signals and slots connections
	connect(Table3, SIGNAL(currentChanged(int, int)), this, SLOT(ReplaceSel(int, int)));
	connect(DelB, SIGNAL(clicked()), this, SLOT(DelEntry()));
	connect(PathList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(SelectPath(QListBoxItem*)));
	connect(AddB, SIGNAL(clicked()), this, SLOT(AddPath()));
	connect(ChangeB, SIGNAL(clicked()), this, SLOT(ChangePath()));
	connect(RemoveB, SIGNAL(clicked()), this, SLOT(DelPath()));
}

void FontPrefs::ReplaceSel(int, int)
{
	DelB->setEnabled(true);
}

void FontPrefs::UpdateFliste()
{
	QString tmp;
	UsedFonts.clear();
	uint a = 0;
	SCFontsIterator it(Prefs->AvailFonts);
	for ( ; it.current() ; ++it)
	{
		if (FlagsUse.at(a)->isChecked())
			UsedFonts.append(it.currentKey());
		++a;
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

void FontPrefs::ReadPath()
{
	QFile fx(HomeP+"/scribusfont13.rc");
	QString ExtraPath = "";
	ExtraFonts.clear();
	PathList->clear();
	if ( fx.exists() )
	{
		if (fx.open(IO_ReadOnly))
		{
			QTextStream tsx(&fx);
			ExtraPath = tsx.read();
			fx.close();
			ExtraFonts = QStringList::split("\n",ExtraPath);
		}
		PathList->insertStringList(ExtraFonts);
	}
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
	PrefsContext* dirs = prefsFile->getContext("dirs");
	CurrentPath = dirs->get("fontprefs", ".");
	QString s = QFileDialog::getExistingDirectory(CurrentPath, this, "d", tr("Choose a Directory"), true);
	if (s != "")
	{
		dirs->set("fontprefs", s.left(s.findRev("/", -2)));
		s = s.left(s.length()-1);
		if (PathList->findItem(s))
			return;
		if (!DocAvail)
		{
			QFile fx(HomeP+"/scribusfont13.rc");
			if (fx.open(IO_WriteOnly))
			{
				QTextStream tsx(&fx);
				for (uint a = 0; a < PathList->count(); ++a)
					tsx << PathList->text(a) << "\n" ;
				tsx << s;
				fx.close();
			}
			else
				return;
			ChangeB->setEnabled(true);
			RemoveB->setEnabled(true);
		}
		PathList->insertItem(s);
		CurrentPath = s;
		RebuildDialog();
	}
}

void FontPrefs::ChangePath()
{
	QString s = QFileDialog::getExistingDirectory(CurrentPath, this, "d", tr("Choose a Directory"), true);
	if (s != "")
	{
		s = s.left(s.length()-1);
		if (PathList->findItem(s))
			return;
		QFile fx(HomeP+"/scribusfont13.rc");
		if (fx.open(IO_WriteOnly))
		{
			PathList->changeItem(s, PathList->currentItem());
			QTextStream tsx(&fx);
			for (uint a = 0; a < PathList->count(); ++a)
				tsx << PathList->text(a) << "\n" ;
			fx.close();
		}
		else
			return;
		CurrentPath = s;
		RebuildDialog();
		ChangeB->setEnabled(true);
		RemoveB->setEnabled(true);
	}
}

void FontPrefs::DelPath()
{
	QFile fx(HomeP+"/scribusfont13.rc");
	if (fx.open(IO_WriteOnly))
	{
		if (PathList->count() == 1)
			PathList->clear();
		else
			PathList->removeItem(PathList->currentItem());
		QTextStream tsx(&fx);
		for (uint a = 0; a < PathList->count(); ++a)
			tsx << PathList->text(a) << "\n" ;
		fx.close();
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
	Prefs->AvailFonts.clear();
	Prefs->AvailFonts.GetFonts(HomeP);
	if (DocAvail)
	{
		for (uint a = 0; a < PathList->count(); ++a)
		{
			if (ExtraFonts.contains(PathList->text(a)) == 0)
				Prefs->AvailFonts.AddScalableFonts(PathList->text(a)+"/", docc->DocName);
		}
	}
	UsedFonts.clear();
	FlagsUse.clear();
	FlagsPS.clear();
	FlagsSub.clear();
//	emit ReReadPrefs();
	Table1->setNumRows( Prefs->AvailFonts.count() );
	Table1->setNumCols( 6 );
	Table1->setSorting(true);
	SCFontsIterator it(Prefs->AvailFonts);
	int a = 0;
	for ( ; it.current(); ++it)
	{
		Table1->setText(a, 0, it.currentKey());
		QCheckBox *cp = new QCheckBox(this, "use");
		cp->setText(" ");
		cp->setChecked(it.current()->UseFont);
		if (it.current()->UseFont)
			UsedFonts.append(it.currentKey());
		cp->setEraseColor(white);
		if (DocAvail)
			cp->setEnabled(false);
		connect(cp, SIGNAL(clicked()), this, SLOT(UpdateFliste()));
		FlagsUse.append(cp);
		Table1->setCellWidget(a, 1, cp);
		QCheckBox *cp2 = new QCheckBox(this, "ps");
		cp2->setText( tr("Postscript"));
		cp2->setChecked(it.current()->EmbedPS);
		cp2->setEraseColor(white);
		FlagsPS.append(cp2);
		Table1->setCellWidget(a, 2, cp2);
		QFileInfo fi = QFileInfo(it.current()->Datei);
		QString ext = fi.extension(false).lower();
		QCheckBox *cp3 = new QCheckBox(this, "su");
		cp3->setText( tr("Yes"));
		if (ext == "otf")
		{
			cp3->setChecked(true); 			// Open Type Fonts are always Subsetted
			cp3->setEnabled(false);
		}
		else
			cp3->setChecked(it.current()->Subset);
		cp3->setEraseColor(white);
		FlagsSub.append(cp3);
		Table1->setCellWidget(a, 3, cp3);
		if ((ext == "pfa") || (ext == "pfb"))
			Table1->setPixmap(a, 4, loadIcon("font_type1.png"));
		else
		{
			if (ext == "ttf")
				Table1->setPixmap(a, 4, loadIcon("font_truetype.png"));
			if (ext == "otf")
				Table1->setPixmap(a, 4, loadIcon("font_otf.png"));
		}
		Table1->setText(a, 5, it.current()->Datei);
		a++;
	}
	Table1->sortColumn(0, true, true);
	UsedFonts.sort();
	Table1->setSorting(false);
	UpdateFliste();
}
