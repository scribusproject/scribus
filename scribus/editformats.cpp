#include "editformats.h"
#include "editformats.moc"
#include "edit1format.h"
#include <qmessagebox.h>
#include "customfdialog.h"
#include "scribusXml.h"
extern QPixmap loadIcon(QString nam);

StilFormate::StilFormate( QWidget* parent, ScribusDoc *doc, preV *avail)
    : QDialog( parent, "Formate", true, 0)
{
    resize( 327, 260 );
    setCaption( tr( "Edit Styles" ) );
  	setIcon(loadIcon("AppIcon.png"));
	fon = avail;
	Docu = doc;
    StilFormateLayout = new QHBoxLayout( this ); 
    StilFormateLayout->setSpacing( 5 );
    StilFormateLayout->setMargin( 10 );

    ListBox1 = new QListBox( this, "ListBox1" );
    ListBox1->setMinimumSize( QSize( 200, 240 ) );
    StilFormateLayout->addWidget( ListBox1 );

    Layout15 = new QVBoxLayout; 
    Layout15->setSpacing( 6 );
    Layout15->setMargin( 0 );

    LoadS = new QPushButton( this, "LoadF" );
    LoadS->setText( tr( "Append" ) );
    Layout15->addWidget( LoadS );

    NewB = new QPushButton( this, "NewB" );
    NewB->setText( tr( "New" ) );
    Layout15->addWidget( NewB );

    EditB = new QPushButton( this, "EditB" );
    EditB->setText( tr( "Edit" ) );
    EditB->setDefault( true );
    EditB->setEnabled(false);
    Layout15->addWidget( EditB );

    DublicateB = new QPushButton( this, "DublicateB" );
    DublicateB->setText( tr( "Duplicate" ) );
    DublicateB->setEnabled(false);
    Layout15->addWidget( DublicateB );

    DeleteB = new QPushButton( this, "DeleteB" );
    DeleteB->setText( tr( "Delete" ) );
    DeleteB->setEnabled(false);
    Layout15->addWidget( DeleteB );

    SaveB = new QPushButton( this, "SaveB" );
    SaveB->setText( tr( "Save" ) );
    Layout15->addWidget( SaveB );

    ExitB = new QPushButton( this, "ExitB" );
    ExitB->setText( tr( "Leave" ) );
    Layout15->addWidget( ExitB );

    CancelB = new QPushButton( this, "CancelB" );
    CancelB->setText( tr( "Cancel" ) );
    Layout15->addWidget( CancelB );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout15->addItem( spacer );
    StilFormateLayout->addLayout( Layout15 );

    // signals and slots connections
    connect(CancelB, SIGNAL(clicked()), this, SLOT(reject()));
    connect(SaveB, SIGNAL(clicked()), this, SLOT(saveIt()));
    connect(ExitB, SIGNAL(clicked()), this, SLOT(accept()));
    connect(EditB, SIGNAL(clicked()), this, SLOT(editFormat()));
    connect(NewB, SIGNAL(clicked()), this, SLOT(neuesFormat()));
    connect(LoadS, SIGNAL(clicked()), this, SLOT(loadStyles()));
    connect(DublicateB, SIGNAL(clicked()), this, SLOT(dupFormat()));
    connect(DeleteB, SIGNAL(clicked()), this, SLOT(deleteFormat()));
    connect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
    TempVorl = doc->Vorlagen;
    UpdateFList();
}

void StilFormate::saveIt()
{
	emit saveStyle(this);
}

void StilFormate::selFormat(QListBoxItem *c)
{
	for (uint x = 5; x < TempVorl.count(); ++x)
	{
		if (TempVorl[x].Vname == c->text())
		{
			sFnumber = x;
			break;
		}
	}
    EditB->setEnabled(true);
    DublicateB->setEnabled(true);
    DeleteB->setEnabled(true);
}

void StilFormate::dupFormat()
{
   	struct StVorL sty;
   	sty.Vname = tr("Copy of %1").arg(TempVorl[sFnumber].Vname);
   	sty.LineSpa = TempVorl[sFnumber].LineSpa;
   	sty.Ausri = TempVorl[sFnumber].Ausri;
   	sty.Indent = TempVorl[sFnumber].Indent;
   	sty.First = TempVorl[sFnumber].First;
   	sty.Avor = TempVorl[sFnumber].Avor;
   	sty.Anach = TempVorl[sFnumber].Anach;
   	sty.Font = TempVorl[sFnumber].Font;
   	sty.FontSize = TempVorl[sFnumber].FontSize;
   	sty.TabValues = TempVorl[sFnumber].TabValues;
   	sty.Drop = TempVorl[sFnumber].Drop;
   	sty.DropLin = TempVorl[sFnumber].DropLin;
   	sty.FontEffect = TempVorl[sFnumber].FontEffect;
	sty.FColor = TempVorl[sFnumber].FColor;
	sty.FShade = TempVorl[sFnumber].FShade;
	sty.SColor = TempVorl[sFnumber].SColor;
	sty.SShade = TempVorl[sFnumber].SShade;
   	TempVorl.append(sty);
   	sFnumber = TempVorl.count()-1;
	EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true, fon,
									static_cast<double>(Docu->AutoLine), Docu->Einheit, Docu);
	if (!dia2->exec())
		TempVorl.remove(TempVorl.fromLast());
   	delete dia2;
	UpdateFList();
}

void StilFormate::neuesFormat()
{
   	struct StVorL sty;
   	sty.Vname = tr("New Style");
   	sty.LineSpa = ((Docu->Dsize / 10.0) * static_cast<double>(Docu->AutoLine) / 100) + (Docu->Dsize / 10.0);
   	sty.Ausri = 0;
   	sty.Indent = 0;
   	sty.First = 0;
   	sty.Avor = 0;
   	sty.Anach = 0;
	sty.Font = Docu->Dfont;
	sty.FontSize = Docu->Dsize;
	sty.TabValues.clear();
   	sty.Drop = false;
   	sty.DropLin = 2;
	sty.FontEffect = 0;
	sty.FColor = Docu->Dbrush;
	sty.FShade = Docu->Dshade;
	sty.SColor = Docu->Dpen;
	sty.SShade = Docu->Dshade2;
   	TempVorl.append(sty);
   	sFnumber = TempVorl.count()-1;
	EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true, fon,
									static_cast<double>(Docu->AutoLine), Docu->Einheit, Docu);
	if (!dia2->exec())
		TempVorl.remove(TempVorl.fromLast());
   	delete dia2;
	UpdateFList();
}

void StilFormate::editFormat()
{
	EditStyle* dia = new EditStyle(this, &TempVorl[sFnumber], TempVorl, false, fon,
									static_cast<double>(Docu->AutoLine), Docu->Einheit, Docu);
	dia->exec();
	delete dia;
	UpdateFList();
}

void StilFormate::deleteFormat()
{
	int exit=QMessageBox::warning(this,
  								tr("Warning"),
                                tr("Do you really want do delete this Style?"),
                                tr("No"),
                                tr("Yes"),
                                0, QMessageBox::No, QMessageBox::Yes);
	/* PFJ - 29.02.04 - Altered to use the correct QMessageBox value. It was 1 */
	if (exit == QMessageBox::Yes)
	{
		ListBox1->removeItem(sFnumber);
		TempVorl.remove(TempVorl.at(sFnumber));
		UpdateFList();
	}
}

void StilFormate::loadStyles()
{
	QString fileName;
#ifdef HAVE_LIBZ
	CustomFDialog dia(this, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	CustomFDialog dia(this, tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
  if (!fileName.isEmpty())
  	{
  		ScriXmlDoc *ss = new ScriXmlDoc();
		ss->Vorlagen.clear();
		for (uint x = 5; x < TempVorl.count(); ++x)
			ss->Vorlagen.append(TempVorl[x]);
		uint old = TempVorl.count()-5;
  		if (ss->ReadStyles(fileName, Docu, fon))
  		{
			if (ss->Vorlagen.count() > old)
			{
				for (uint xx=old; xx<ss->Vorlagen.count(); ++xx)
				{
				   	struct StVorL sty;
				   	sty.Vname = ss->Vorlagen[xx].Vname;
			   		sty.LineSpa = ss->Vorlagen[xx].LineSpa;
				   	sty.Ausri = ss->Vorlagen[xx].Ausri;
				   	sty.Indent = ss->Vorlagen[xx].Indent;
				   	sty.First = ss->Vorlagen[xx].First;
			   		sty.Avor = ss->Vorlagen[xx].Avor;
				   	sty.Anach = ss->Vorlagen[xx].Anach;
				   	sty.Font = ss->Vorlagen[xx].Font;
				   	sty.FontSize = ss->Vorlagen[xx].FontSize;
   					sty.TabValues = ss->Vorlagen[xx].TabValues;
   					sty.Drop = TempVorl[xx].Drop;
  	 				sty.DropLin = TempVorl[xx].DropLin;
  	 				sty.FontEffect = TempVorl[xx].FontEffect;
					sty.FColor = TempVorl[xx].FColor;
					sty.FShade = TempVorl[xx].FShade;
					sty.SColor = TempVorl[xx].SColor;
					sty.SShade = TempVorl[xx].SShade;
				   	TempVorl.append(sty);
				}
			}
			UpdateFList();
  		}
		delete ss;
  	}
}

void StilFormate::UpdateFList()
{
	
	if (TempVorl.count() < 6)
		return;
	ListBox1->clear();
	for (uint x = 5; x < TempVorl.count(); ++x)
		ListBox1->insertItem(TempVorl[x].Vname);
	if (ListBox1->currentItem() == -1)
   	{
	   	DublicateB->setEnabled(false);
   		EditB->setEnabled(false);
		DeleteB->setEnabled(false);
   	}
	/* PFJ - 29.02.04 - Altered from TRUE to true */
	ListBox1->sort( true );
	ListBox1->setSelected(ListBox1->currentItem(), false);
}
