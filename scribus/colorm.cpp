/****************************************************************************
** Form implementation generated from reading ui file 'Color.ui'
**
** Created: Mon Apr 23 19:09:31 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "colorm.h"
#include "colorm.moc"
#include <qvariant.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <cstdlib>
#include <qcolordialog.h>
#include "customfdialog.h"
#include "dcolor.h"
#include "scribusXml.h"
#include "cmykfw.h"
#include "query.h"
#include "scribus.h"
#include "prefsfile.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

extern QPixmap loadIcon(QString nam);
extern ScribusApp* ScApp;
extern PrefsFile* prefsFile;

Farbmanager::Farbmanager( QWidget* parent, CListe doco, bool HDoc, QString DcolSet, QStringList Cust )
		: QDialog( parent, "dd", true, 0 )
{
	setName( "Farbmanager" );
	HaveDoc = HDoc;
	CColSet = Cust;
	setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
	setSizeGripEnabled(true);
	setCaption( tr( "Colors" ) );
	setIcon(loadIcon("AppIcon.png"));
	Layout2 = new QVBoxLayout( this );
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 11 );

	layout5 = new QHBoxLayout( 0, 0, 6, "layout5");
	layout3 = new QVBoxLayout( 0, 0, 6, "layout3");
	ListBox1 = new QListBox( this, "ListBox1" );
	ListBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, ListBox1->sizePolicy().hasHeightForWidth() ) );
	ListBox1->setMinimumSize( QSize( 164, 228 ) );
	ListBox1->setColumnMode( QListBox::FixedNumber );
	layout5->addWidget( ListBox1 );

	ColorsGroup = new QGroupBox( this, "ColorsGroup" );
	ColorsGroup->setColumnLayout(0, Qt::Vertical );
	ColorsGroup->layout()->setSpacing( 6 );
	ColorsGroup->layout()->setMargin( 11 );
	Layout1 = new QVBoxLayout( ColorsGroup->layout() );
	Layout1->setAlignment( Qt::AlignTop );
	LoadF = new QPushButton( tr( "&Append" ), ColorsGroup, "LoadF" );
	Layout1->addWidget( LoadF );
	NewF = new QPushButton( tr( "&New" ), ColorsGroup, "NewF" );
	Layout1->addWidget( NewF );
	EditF = new QPushButton( tr( "&Edit" ), ColorsGroup, "EditF" );
	EditF->setEnabled( false );
	EditF->setDefault( true );
	Layout1->addWidget( EditF );
	DupF = new QPushButton( tr( "D&uplicate" ), ColorsGroup, "DupF" );
	DupF->setEnabled( false );
	Layout1->addWidget( DupF );
	DelF = new QPushButton( tr( "&Delete" ), ColorsGroup, "DelF" );
	DelF->setEnabled( false );
	Layout1->addWidget( DelF );
	if (HaveDoc)
	{
		DelU = new QPushButton( tr( "&Remove Unused" ), ColorsGroup, "DelU" );
		Layout1->addWidget( DelU );
	}
	layout3->addWidget( ColorsGroup );
	if (!HaveDoc)
	{
		ColsSetGroup = new QGroupBox( this, "ColsSetGroup" );
		ColsSetGroup->setTitle( tr( "Color Sets" ) );
		ColsSetGroup->setColumnLayout(0, Qt::Vertical );
		ColsSetGroup->layout()->setSpacing( 6 );
		ColsSetGroup->layout()->setMargin( 11 );
		ColsSetGroupLayout = new QVBoxLayout( ColsSetGroup->layout() );
		ColsSetGroupLayout->setAlignment( Qt::AlignTop );
		textLabel1 = new QLabel( ColsSetGroup, "textLabel1" );
		textLabel1->setText( tr( "Current Color Set:" ) );
		ColsSetGroupLayout->addWidget( textLabel1 );
		CSets = new QPopupMenu();
		CSets->insertItem("Scribus Small");
		CSets->insertItem("X11 RGB-Set");
		CSets->insertItem("X11 Grey-Set");
		CSets->insertItem("Gnome-Set");
		CSets->insertItem("SVG-Set");
		if (Cust.count() != 0)
		{
			QStringList realEx;
			realEx.clear();
			for (uint m = 0; m < Cust.count(); ++m)
			{
				QString Cpfad = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/"+Cust[m]);
				QFileInfo cfi(Cpfad);
				if (cfi.exists())
				{
					CSets->insertItem(Cust[m]);
					realEx.append(Cust[m]);
				}
			}
			CColSet = realEx;
		}
		LoadColSet = new QToolButton( ColsSetGroup, "LoadColSet" );
		LoadColSet->setPopup(CSets);
		LoadColSet->setPopupDelay(0);
		LoadColSet->setText(DcolSet);
		ColsSetGroupLayout->addWidget( LoadColSet );
		SaveColSet = new QPushButton( tr( "&Save Color Set" ), ColsSetGroup, "SaveColSet" );
		ColsSetGroupLayout->addWidget( SaveColSet );
		layout3->addWidget( ColsSetGroup );
	}
	SaveF = new QPushButton( tr( "&OK" ), this, "SaveF" );
	layout3->addWidget( SaveF );
	CancF = new QPushButton( tr( "&Cancel" ), this, "CancF" );
	CancF->setDefault( true );
	layout3->addWidget( CancF );
	layout5->addLayout( layout3 );
	Layout2->addLayout( layout5 );
	Ersatzliste.clear();
	EditColors = doco;
	updateCList();
	// signals and slots connections
	if (!HaveDoc)
	{
		connect(CSets, SIGNAL(activated(int)), this, SLOT(loadDefaults(int)));
		connect(SaveColSet, SIGNAL( clicked() ), this, SLOT( saveDefaults() ) );
		QToolTip::add( LoadColSet, tr( "Choose a color set to load" ) );
		QToolTip::add( SaveColSet, tr( "Save the current color set" ) );
	}
	else
	{
		connect(DelU, SIGNAL( clicked() ), this, SLOT( delUnused() ) );
		QToolTip::add( DelU, tr( "Remove unused colors from current document's color set" ) );
	}
	QToolTip::add( LoadF, tr( "Append colors to the current set from an existing document" ) );
	QToolTip::add( NewF, tr( "Create a new color within the current set" ) );
	QToolTip::add( EditF, tr( "Edit the currently selected color" ) );
	QToolTip::add( DupF, tr( "Make a copy of the currently selected color" ) );
	QToolTip::add( DelF, tr( "Delete the currently selected color" ) );
	QToolTip::add( SaveF, tr( "Make the current colorset the default color set" ) );
	connect( SaveF, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( CancF, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( NewF, SIGNAL( clicked() ), this, SLOT( neueFarbe() ) );
	connect( EditF, SIGNAL( clicked() ), this, SLOT( editFarbe() ) );
	connect( DupF, SIGNAL( clicked() ), this, SLOT( duplFarbe() ) );
	connect( DelF, SIGNAL( clicked() ), this, SLOT( delFarbe() ) );
	connect( LoadF, SIGNAL( clicked() ), this, SLOT( loadFarben() ) );
	connect( ListBox1, SIGNAL( highlighted(QListBoxItem*) ), this, SLOT( selFarbe(QListBoxItem*) ) );
	connect( ListBox1, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selEditFarbe(QListBoxItem*) ) );
}

void Farbmanager::saveDefaults()
{
	QString Cpfad = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/");
	QString Name = LoadColSet->text();
	Query* dia = new Query(this, "Name", 1, 0, tr("&Name:"), tr("Choose a Name"));
	if ((Name == "Scribus Small") || (Name == "X11 RGB-Set")
	        || (Name == "X11 Grey-Set") || (Name == "Gnome-Set") || (Name == "SVG-Set"))
		dia->setEditText("", false);
	else
		dia->setEditText(Name, false);
	if (dia->exec())
	{
		QString Fname = Cpfad+dia->getEditText();
		LoadColSet->setText(dia->getEditText());
		QFile fx(Fname);
		if (fx.open(IO_WriteOnly))
		{
			QTextStream tsx(&fx);
			QString tmp;
			CListe::Iterator itc;
			tsx << "Color Set:"+dia->getEditText()+"\n";
			int cp, mp, yp, kp;
			for (itc = EditColors.begin(); itc != EditColors.end(); ++itc)
			{
				EditColors[itc.key()].getCMYK(&cp, &mp, &yp, &kp);
				tsx << tmp.setNum(cp) << "\t" ;
				tsx << tmp.setNum(mp) << "\t" ;
				tsx << tmp.setNum(yp) << "\t" ;
				tsx << tmp.setNum(kp) << "\t" ;
				tsx << itc.key() << "\n" ;
			}
			fx.close();
			if (dia->getEditText() != Name)
			{
				CColSet.append(dia->getEditText());
				CSets->insertItem(dia->getEditText());
			}
		}
	}
	delete dia;
}

void Farbmanager::loadDefaults(int id)
{
	int c = CSets->indexOf(id);
	bool cus = false;
	LoadColSet->setText(CSets->text(id));
	EditColors.clear();
	QString Cpfad = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/"+CSets->text(id));
	QString pfadC = LIBDIR;
	QString pfadC2 = pfadC + "rgbscribus.txt";
	switch (c)
	{
	case 0:
		LoadColSet->setText("Scribus Small");
		EditColors.insert("White", CMYKColor(0, 0, 0, 0));
		EditColors.insert("Black", CMYKColor(0, 0, 0, 255));
		EditColors.insert("Blue", CMYKColor(255, 255, 0, 0));
		EditColors.insert("Cyan", CMYKColor(255, 0, 0, 0));
		EditColors.insert("Green", CMYKColor(255, 0, 255, 0));
		EditColors.insert("Red", CMYKColor(0, 255, 255, 0));
		EditColors.insert("Yellow", CMYKColor(0, 0, 255, 0));
		EditColors.insert("Magenta", CMYKColor(0, 255, 0, 0));
		break;
	case 1:
		pfadC2 = pfadC + "rgbscribus.txt";
		break;
	case 2:
		pfadC2 = pfadC + "rgbscribusgreys.txt";
		break;
	case 3:
		pfadC2 = pfadC + "rgbscribusgnome.txt";
		break;
	case 4:
		pfadC2 = pfadC + "rgbsvg.txt";
		break;
	default:
		pfadC2 = Cpfad;
		cus = true;
		break;
	}
	if (c != 0)
	{
		QFile fiC(pfadC2);
		if (fiC.open(IO_ReadOnly))
		{
			QString ColorEn, Cname;
			int Rval, Gval, Bval, Kval;
			QTextStream tsC(&fiC);
			ColorEn = tsC.readLine();
			while (!tsC.atEnd())
			{
				CMYKColor tmp;
				ColorEn = tsC.readLine();
				QTextStream CoE(&ColorEn, IO_ReadOnly);
				CoE >> Rval;
				CoE >> Gval;
				CoE >> Bval;
				if (cus)
				{
					CoE >> Kval;
					Cname = CoE.read().stripWhiteSpace();
					tmp.setColor(Rval, Gval, Bval, Kval);
				}
				else
				{
					Cname = CoE.read().stripWhiteSpace();
					tmp.setColorRGB(Rval, Gval, Bval);
				}
				EditColors.insert(Cname, tmp);
			}
			fiC.close();
		}
		else
		{
			LoadColSet->setText("Scribus Small");
			EditColors.insert("White", CMYKColor(0, 0, 0, 0));
			EditColors.insert("Black", CMYKColor(0, 0, 0, 255));
			EditColors.insert("Blue", CMYKColor(255, 255, 0, 0));
			EditColors.insert("Cyan", CMYKColor(255, 0, 0, 0));
			EditColors.insert("Green", CMYKColor(255, 0, 255, 0));
			EditColors.insert("Red", CMYKColor(0, 255, 255, 0));
			EditColors.insert("Yellow", CMYKColor(0, 0, 255, 0));
			EditColors.insert("Magenta", CMYKColor(0, 255, 0, 0));
		}
	}
	updateCList();
}

void Farbmanager::loadFarben()
{
	QString fileName;
	PrefsContext* dirs = prefsFile->getContext("dirs");
	QString wdir = dirs->get("colors", ".");
#ifdef HAVE_LIBZ
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("colors", fileName.left(fileName.findRev("/")));
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (ss->ReadColors(fileName))
		{
			CListe LColors = ss->Farben;
			CListe::Iterator it;
			for (it = LColors.begin(); it != LColors.end(); ++it)
			{
				if (!EditColors.contains(it.key()))
					EditColors.insert(it.key(), it.data());
			}
			updateCList();
		}
		delete ss;
	}
}

void Farbmanager::delUnused()
{
	PageItem* ite;
	bool found;
	UsedC.clear();
	CListe::Iterator it;
	for (it = EditColors.begin(); it != EditColors.end(); ++it)
	{
		found = false;
		if ((it.key() == ScApp->doc->Dbrush) || (it.key() == ScApp->doc->Dpen) ||
		        (it.key() == ScApp->doc->DbrushPict)
		        || (it.key() == ScApp->doc->DpenLine) || (it.key() == ScApp->doc->DpenText))
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < ScApp->doc->MasterItems.count(); ++c)
		{
			ite = ScApp->doc->MasterItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((ite->PType == 4) || (ite->PType == 8))
			{
				for (uint d=0; d<ite->Ptext.count(); ++d)
				{
					if (it.key() == ite->Ptext.at(d)->ccolor)
						found = true;
					if (it.key() == ite->Ptext.at(d)->cstroke)
						found = true;
					if (found)
						break;
				}
			}
			/* PFJ - 29.02.04 - merged if's to one line */
			if ((it.key() == ite->Pcolor) || (it.key() == ite->Pcolor2))
				found = true;
			if (found)
				break;
		}
		if (found)
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < ScApp->doc->DocItems.count(); ++c)
		{
			ite = ScApp->doc->DocItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((ite->PType == 4) || (ite->PType == 8))
			{
				for (uint d=0; d<ite->Ptext.count(); ++d)
				{
					/* PFJ - 29.02.04 - Merged if's */
					if ((it.key() == ite->Ptext.at(d)->ccolor) ||
							(it.key() == ite->Ptext.at(d)->cstroke))
						found = true;
					if (found)
						break;
				}
			}
			/* PFJ - 29.02.04 - Merged if's */
			if ((it.key() == ite->Pcolor) || (it.key() == ite->Pcolor2))
				found = true;
			if (found)
				break;
		}
		/* PFJ - 29.02.04 - Merged if's */
		if ((it.key() == ScApp->doc->CurrTextFill) ||
		        (it.key() == ScApp->doc->CurrTextStroke))
			found = true;
		if (found)
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
	}
	EditColors = UsedC;
	if (EditColors.count() == 0)
	{
		EditColors.insert("White", CMYKColor(0, 0, 0, 0));
		EditColors.insert("Black", CMYKColor(0, 0, 0, 255));
	}
	updateCList();
}

void Farbmanager::duplFarbe()
{
	QString nam = tr("Copy of %1").arg(sFarbe);
	EditColors.insert(nam, EditColors[sFarbe]);
	updateCList();
}

void Farbmanager::neueFarbe()
{
	CMYKColor tmpFarbe = CMYKColor(0, 0, 0, 0);
	CMYKChoose* dia = new CMYKChoose(this, tmpFarbe, tr("New Color"), &EditColors, CColSet);
	if (dia->exec())
	{
		EditColors.insert(dia->Farbname->text(), dia->Farbe);
		updateCList();
	}
	delete dia;
}

void Farbmanager::editFarbe()
{
	CMYKColor tmpFarbe = EditColors[sFarbe];
	CMYKChoose* dia = new CMYKChoose(this, tmpFarbe, sFarbe, &EditColors, CColSet);
	if (dia->exec())
	{
		EditColors[dia->Farbname->text()] = dia->Farbe;
		if (sFarbe != dia->Farbname->text())
			Ersatzliste.insert(sFarbe, dia->Farbname->text());
		updateCList();
	}
	delete dia;
}

void Farbmanager::delFarbe()
{
	DelColor *dia = new DelColor(this, EditColors, sFarbe, HaveDoc);
	if (dia->exec())
	{
		Ersatzliste.insert(sFarbe, dia->getReplacementColor());
		EditColors.remove(sFarbe);
		updateCList();
	}
	delete dia;
}

void Farbmanager::selFarbe(QListBoxItem *c)
{
	sFarbe = c->text();
	EditF->setEnabled(true);
	DupF->setEnabled(true);
	DelF->setEnabled(EditColors.count() == 1 ? false : true);
}

void Farbmanager::selEditFarbe(QListBoxItem *c)
{
	sFarbe = c->text();
	EditF->setEnabled(true);
	DupF->setEnabled(true);
	DelF->setEnabled(EditColors.count() == 1 ? false : true);
	editFarbe();
}

void Farbmanager::updateCList()
{
	ListBox1->clear();
	CListe::Iterator it;
	QPixmap pm = QPixmap(30, 15);
	for (it = EditColors.begin(); it != EditColors.end(); ++it)
	{
		// if condition 10/21/2004 pv #1191
		if (it.key() != "None" && it.key() != tr("None"))
		{
			pm.fill(EditColors[it.key()].getRGBColor());
			ListBox1->insertItem(pm, it.key());
		}
	}
	DelF->setEnabled(EditColors.count() == 1 ? false : true);
	if (ListBox1->currentItem() == -1)
	{
		DupF->setEnabled(false);
		EditF->setEnabled(false);
		DelF->setEnabled(false);
	}
	ListBox1->setSelected(ListBox1->currentItem(), false);
}

