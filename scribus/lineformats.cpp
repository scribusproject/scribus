#include "lineformats.h"
#include "lineformats.moc"

#include <qmessagebox.h>
#include <qpainter.h>

#include "customfdialog.h"
#include "scribusXml.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "multiline.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

LineFormate::LineFormate( QWidget* parent, ScribusDoc *doc)
		: QDialog( parent, "Formate", true, 0)
{
	resize( 327, 260 );
	setCaption( tr( "Edit Line Styles" ) );
	setIcon(loadIcon("AppIcon.png"));
	Docu = doc;
	TempStyles = doc->MLineStyles;
	StilFormateLayout = new QHBoxLayout( this );
	StilFormateLayout->setSpacing( 5 );
	StilFormateLayout->setMargin( 10 );

	ListBox1 = new QListBox( this, "ListBox1" );
	ListBox1->setMinimumSize( QSize( 200, 240 ) );
	StilFormateLayout->addWidget( ListBox1 );

	Layout15 = new QVBoxLayout;
	Layout15->setSpacing( 6 );
	Layout15->setMargin( 0 );

	LoadLS = new QPushButton( tr( "&Import" ), this, "LoadF" );
	Layout15->addWidget( LoadLS );

	NewB = new QPushButton( tr( "&New" ), this, "NewB" );
	Layout15->addWidget( NewB );

	EditB = new QPushButton( tr( "&Edit" ), this, "EditB" );
	EditB->setDefault( true );
	EditB->setEnabled(false);
	Layout15->addWidget( EditB );

	DublicateB = new QPushButton( tr( "D&uplicate" ), this, "DublicateB" );
	DublicateB->setEnabled(false);
	Layout15->addWidget( DublicateB );

	DeleteB = new QPushButton( tr( "&Delete" ), this, "DeleteB" );
	DeleteB->setEnabled(false);
	Layout15->addWidget( DeleteB );

	SaveB = new QPushButton( tr( "&Save" ), this, "SaveB" );
	Layout15->addWidget( SaveB );

	ExitB = new QPushButton( CommonStrings::tr_OK, this, "ExitB" );
	Layout15->addWidget( ExitB );

	CancelB = new QPushButton( CommonStrings::tr_Cancel, this, "CancelB" );
	Layout15->addWidget( CancelB );
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout15->addItem( spacer );
	StilFormateLayout->addLayout( Layout15 );

	// signals and slots connections
	connect(CancelB, SIGNAL(clicked()), this, SLOT(reject()));
	connect(SaveB, SIGNAL(clicked()), this, SLOT(saveIt()));
	connect(ExitB, SIGNAL(clicked()), this, SLOT(accept()));
	connect(LoadLS, SIGNAL(clicked()), this, SLOT(loadLStyles()));
	connect(EditB, SIGNAL(clicked()), this, SLOT(editFormat()));
	connect(NewB, SIGNAL(clicked()), this, SLOT(neuesFormat()));
	connect(DublicateB, SIGNAL(clicked()), this, SLOT(dupFormat()));
	connect(DeleteB, SIGNAL(clicked()), this, SLOT(deleteFormat()));
	connect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
	connect( ListBox1, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selEditFormat(QListBoxItem*) ) );
	if (ListBox1->count() > 0)
    	sFnumber = TempStyles.begin().key();
	else
		sFnumber = "";
	Replacement.clear();
	UpdateFList();
}

void LineFormate::saveIt()
{
	emit saveStyle(this);
}

void LineFormate::selFormat(QListBoxItem *c)
{
	sFnumber = c->text();
	EditB->setEnabled(true);
	DublicateB->setEnabled(true);
	DeleteB->setEnabled(true);
}

void LineFormate::selEditFormat(QListBoxItem *c)
{
	sFnumber = c->text();
	EditB->setEnabled(true);
	DublicateB->setEnabled(true);
	DeleteB->setEnabled(true);
	editFormat();
}

void LineFormate::dupFormat()
{
	multiLine ml;
	ml = TempStyles[sFnumber];
	MultiLine* dia = new MultiLine(this, Docu, ml, tr("Copy of %1").arg(sFnumber), &TempStyles);
	if (dia->exec())
	{
		TempStyles.insert(dia->SName->text(), dia->TempVorl);
		sFnumber = dia->SName->text();
	}
	UpdateFList();
}

void LineFormate::neuesFormat()
{
	struct SingleLine sl;
	sl.Color = "Black";
	sl.Shade = 100;
	sl.Dash = SolidLine;
	sl.LineEnd = FlatCap;
	sl.LineJoin = MiterJoin;
	sl.Width = 1.0;
	multiLine ml;
	ml.push_back(sl);
	MultiLine* dia = new MultiLine(this, Docu, ml, tr("New Style"), &TempStyles);
	if (dia->exec())
	{
		TempStyles.insert(dia->SName->text(), dia->TempVorl);
		sFnumber = dia->SName->text();
	}
	UpdateFList();
}

void LineFormate::editFormat()
{
	MultiLine* dia = new MultiLine(this, Docu, TempStyles[sFnumber], sFnumber, &TempStyles);
	if (dia->exec())
	{
		if (sFnumber != dia->SName->text())
		{
			Replacement.insert(sFnumber, dia->SName->text());
			TempStyles.remove(sFnumber);
			TempStyles.insert(dia->SName->text(), dia->TempVorl);
			sFnumber = dia->SName->text();
		}
		else
			TempStyles[sFnumber] = dia->TempVorl;
	}
	delete dia;
	UpdateFList();
}

void LineFormate::deleteFormat()
{
	int exit=QMessageBox::warning(this,
	                              tr("Warning"),
	                              tr("Do you really want to delete this Style?"),
	                              tr("&No"),
	                              tr("&Yes"),
	                              0, 0, 0);
	/* PFJ - 29.02.04 - Changed from 1 to QMessageBox::Yes */
	/* FS - 12.05.04 the 1 is correct in this version of QMessageBox, it returns the Nr of the clicked Button either 0 or 1 or 2 */
	if (exit == 1)
	{
		Replacement.insert(sFnumber, "");
		TempStyles.remove(sFnumber);
		UpdateFList();
	}
}

void LineFormate::loadLStyles()
{
	QString fileName;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("lineformats", ".");
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
		dirs->set("lineformats", fileName.left(fileName.findRev("/")));
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (ss->ReadLStyles(fileName, &TempStyles))
			UpdateFList();
		delete ss;
	}
}

void LineFormate::UpdateFList()
{
	disconnect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
	disconnect( ListBox1, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selEditFormat(QListBoxItem*) ) );
	ListBox1->clear();
	QMap<QString,multiLine>::Iterator it;
	for (it = TempStyles.begin(); it != TempStyles.end(); ++it)
	{
		QPixmap pm = QPixmap(37, 37);
		pm.fill(white);
		QPainter p;
		p.begin(&pm);
		QColor tmpf;
		int h, s, v, sneu;
		multiLine ml = it.data();
		for (int its = ml.size()-1; its > -1; its--)
		{
			Docu->PageColors[ml[its].Color].getRawRGBColor().rgb(&h, &s, &v);
			if ((h == s) && (s == v))
			{
				Docu->PageColors[ml[its].Color].getRawRGBColor().hsv(&h, &s, &v);
				sneu = 255 - ((255 - v) * ml[its].Shade / 100);
				tmpf.setHsv(h, s, sneu);
			}
			else
			{
				Docu->PageColors[ml[its].Color].getRawRGBColor().hsv(&h, &s, &v);
				sneu = s * ml[its].Shade / 100;
				tmpf.setHsv(h, sneu, v);
			}
			p.setPen(QPen(tmpf,
							QMAX(static_cast<int>(ml[its].Width), 1),
							static_cast<PenStyle>(ml[its].Dash),
							static_cast<PenCapStyle>(ml[its].LineEnd),
							static_cast<PenJoinStyle>(ml[its].LineJoin)));
			p.drawLine(0, 18, 37, 18);
			}
		p.end();
		ListBox1->insertItem(pm, it.key());
	}
	if (ListBox1->count() > 0)
		ListBox1->setSelected(ListBox1->findItem(sFnumber), true);
	bool setter = ListBox1->count() == 0 ? true : false;
	if (setter == true)
		sFnumber = "";
	DublicateB->setEnabled(!setter);
	EditB->setEnabled(!setter);
	DeleteB->setEnabled(!setter);
	ListBox1->sort( true );
	connect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
	connect( ListBox1, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selEditFormat(QListBoxItem*) ) );
}
