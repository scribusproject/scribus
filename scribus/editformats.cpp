#include "editformats.h"
#include "editformats.moc"
#include "edit1format.h"
#include <qmessagebox.h>
#include <qheader.h>

#include "scribusdoc.h"
#include "customfdialog.h"
#include "prefsfile.h"
#include "scribusXml.h"

extern QPixmap loadIcon(QString nam);
extern PrefsFile* prefsFile;

ChooseStyles::ChooseStyles( QWidget* parent, QValueList<ParagraphStyle> *styleList, QValueList<ParagraphStyle> *styleOld)
 : QDialog( parent, "ChooseStyles", true, 0 )
{
	setCaption( tr( "Choose Styles" ) );
	setIcon(loadIcon("AppIcon.png"));
	ChooseStylesLayout = new QVBoxLayout( this, 10, 5, "ChooseStylesLayout");
	StyleView = new QListView( this, "StyleView" );
	StyleView->clear();
	StyleView->addColumn( tr( "Available Styles" ) );
	StyleView->header()->setClickEnabled( false, StyleView->header()->count() - 1 );
	StyleView->header()->setResizeEnabled( false, StyleView->header()->count() - 1 );
	StyleView->setSorting(-1);
	int counter = 5;
	bool tabEQ = false;
	for (uint x = 5; x < styleList->count(); ++x)
	{
		struct ParagraphStyle vg;
		struct ParagraphStyle vg2;
		vg = (*styleList)[x];
		bool found = false;
		for (uint xx=0; xx<styleOld->count(); ++xx)
		{
			vg2 = (*styleOld)[xx];
			if (vg.Vname == vg2.Vname)
			{
				struct PageItem::TabRecord tb;
				tabEQ = false;
				if ((vg2.TabValues.count() == 0) && (vg.TabValues.count() == 0))
					tabEQ = true;
				else
				{
					for (uint t1 = 0; t1 < vg2.TabValues.count(); t1++)
					{
						tb.tabPosition = vg2.TabValues[t1].tabPosition;
						tb.tabType = vg2.TabValues[t1].tabType;
						tb.tabFillChar = vg2.TabValues[t1].tabFillChar;
						for (uint t2 = 0; t2 < vg.TabValues.count(); t2++)
						{
							struct PageItem::TabRecord tb2;
							tb2.tabPosition = vg.TabValues[t2].tabPosition;
							tb2.tabType = vg.TabValues[t2].tabType;
							tb2.tabFillChar = vg.TabValues[t2].tabFillChar;
							if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
							{
								tabEQ = true;
								break;
							}
						}
						if (tabEQ)
							break;
					}
				}
				if ((vg.LineSpa == vg2.LineSpa) && (vg.Indent == vg2.Indent) && (vg.First == vg2.First) &&
					(vg.textAlignment == vg2.textAlignment) && (vg.gapBefore == vg2.gapBefore) &&
					(vg.LineSpaMode == vg2.LineSpaMode) && (vg.gapAfter == vg2.gapAfter) && (vg.Font == vg2.Font) && (tabEQ)
					&& (vg.Drop == vg2.Drop) && (vg.DropDist == vg2.DropDist) && (vg.DropLin == vg2.DropLin) && (vg.FontEffect == vg2.FontEffect) &&
					(vg.FColor == vg2.FColor) && (vg.FShade == vg2.FShade) && (vg.SColor == vg2.SColor) &&
					(vg.txtShadowX == vg2.txtShadowX) && (vg.txtShadowY == vg2.txtShadowY) &&
					(vg.txtOutline == vg2.txtOutline) && (vg.txtUnderPos == vg2.txtUnderPos) && (vg.txtUnderWidth == vg2.txtUnderWidth) &&
					(vg.scaleH == vg2.scaleH) && (vg.scaleV == vg2.scaleV) && (vg.baseOff == vg2.baseOff) && (vg.kernVal == vg2.kernVal) &&
					(vg.txtStrikePos == vg2.txtStrikePos) && (vg.txtStrikeWidth == vg2.txtStrikeWidth) &&
					(vg.SShade == vg2.SShade) && (vg.BaseAdj == vg2.BaseAdj) && (vg.FontSize == vg2.FontSize))
				{
					found = true;
				}
				else
				{
					vg.Vname = "Copy of "+vg2.Vname;
					found = false;
				}
				break;
			}
		}
		if (!found)
		{
			QCheckListItem *item = new QCheckListItem (StyleView, vg.Vname, QCheckListItem::CheckBox);
			item->setOn(true);
			storedStyles.insert(item, counter);
		}
		counter++;
	}
	StyleView->setSorting(0);
	ChooseStylesLayout->addWidget( StyleView );
	layout2 = new QHBoxLayout( 0, 0, 5, "layout2");
	QSpacerItem* spacer1 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer1 );
	OkButton = new QPushButton( this, "OkButton" );
	OkButton->setText( tr( "OK" ) );
	layout2->addWidget( OkButton );
	CancelButton = new QPushButton( this, "CancelButton" );
	CancelButton->setText( tr( "Cancel" ) );
	layout2->addWidget( CancelButton );
	ChooseStylesLayout->addLayout( layout2 );
	resize(230, 280);
	clearWState( WState_Polished );
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
}

StilFormate::StilFormate( QWidget* parent, ScribusDoc *doc, ApplicationPrefs *avail) : QDialog( parent, "Formate", true, 0)
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

	LoadS = new QPushButton( tr( "&Append" ), this, "LoadF" );
	Layout15->addWidget( LoadS );

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

	ExitB = new QPushButton( tr( "&OK" ), this, "ExitB" );
	Layout15->addWidget( ExitB );

	CancelB = new QPushButton( tr( "&Cancel" ), this, "CancelB" );
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
	connect( ListBox1, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selEditFormat(QListBoxItem*) ) );
	TempVorl = doc->docParagraphStyles;
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

void StilFormate::selEditFormat(QListBoxItem *c)
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
	editFormat();
}

void StilFormate::dupFormat()
{
	struct ParagraphStyle sty;
	sty.Vname = tr("Copy of %1").arg(TempVorl[sFnumber].Vname);
	sty.LineSpaMode = TempVorl[sFnumber].LineSpaMode;
	sty.LineSpa = TempVorl[sFnumber].LineSpa;
	sty.textAlignment = TempVorl[sFnumber].textAlignment;
	sty.Indent = TempVorl[sFnumber].Indent;
	sty.First = TempVorl[sFnumber].First;
	sty.gapBefore = TempVorl[sFnumber].gapBefore;
	sty.gapAfter = TempVorl[sFnumber].gapAfter;
	sty.Font = TempVorl[sFnumber].Font;
	sty.FontSize = TempVorl[sFnumber].FontSize;
	sty.TabValues = TempVorl[sFnumber].TabValues;
	sty.Drop = TempVorl[sFnumber].Drop;
	sty.DropLin = TempVorl[sFnumber].DropLin;
	sty.DropDist = TempVorl[sFnumber].DropDist;
	sty.FontEffect = TempVorl[sFnumber].FontEffect;
	sty.FColor = TempVorl[sFnumber].FColor;
	sty.FShade = TempVorl[sFnumber].FShade;
	sty.SColor = TempVorl[sFnumber].SColor;
	sty.SShade = TempVorl[sFnumber].SShade;
	sty.BaseAdj = TempVorl[sFnumber].BaseAdj;
	sty.txtShadowX = TempVorl[sFnumber].txtShadowX;
	sty.txtShadowY = TempVorl[sFnumber].txtShadowY;
	sty.txtOutline = TempVorl[sFnumber].txtOutline;
	sty.txtUnderPos = TempVorl[sFnumber].txtUnderPos;
	sty.txtUnderWidth = TempVorl[sFnumber].txtUnderWidth;
	sty.txtStrikePos = TempVorl[sFnumber].txtStrikePos;
	sty.txtStrikeWidth = TempVorl[sFnumber].txtStrikeWidth;
	sty.scaleH = TempVorl[sFnumber].scaleH;
	sty.scaleV = TempVorl[sFnumber].scaleV;
	sty.baseOff = TempVorl[sFnumber].baseOff;
	sty.kernVal = TempVorl[sFnumber].kernVal;
	TempVorl.append(sty);
	sFnumber = TempVorl.count()-1;
	EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true, fon,
	                                static_cast<double>(Docu->typographicSetttings.autoLineSpacing), Docu->docUnitIndex, Docu);
	if (!dia2->exec())
		TempVorl.remove(TempVorl.fromLast());
	delete dia2;
	UpdateFList();
}

void StilFormate::neuesFormat()
{
	struct ParagraphStyle sty;
	sty.Vname = tr("New Style");
	sty.LineSpaMode = 0;
	sty.LineSpa = ((Docu->toolSettings.defSize / 10.0) * static_cast<double>(Docu->typographicSetttings.autoLineSpacing) / 100) + (Docu->toolSettings.defSize / 10.0);
	sty.textAlignment = 0;
	sty.Indent = 0;
	sty.First = 0;
	sty.gapBefore = 0;
	sty.gapAfter = 0;
	sty.Font = Docu->toolSettings.defFont;
	sty.FontSize = Docu->toolSettings.defSize;
	sty.TabValues.clear();
	sty.Drop = false;
	sty.DropLin = 2;
	sty.DropDist = 0;
	sty.FontEffect = 0;
	sty.FColor = Docu->toolSettings.dBrush;
	sty.FShade = Docu->toolSettings.dShade;
	sty.SColor = Docu->toolSettings.dPen;
	sty.SShade = Docu->toolSettings.dShade2;
	sty.BaseAdj = false;
	sty.txtShadowX = 50;
	sty.txtShadowY = -50;
	sty.txtOutline = 10;
	sty.txtUnderPos = Docu->typographicSetttings.valueUnderlinePos;
	sty.txtUnderWidth = Docu->typographicSetttings.valueUnderlineWidth;
	sty.txtStrikePos = Docu->typographicSetttings.valueStrikeThruPos;
	sty.txtStrikeWidth = Docu->typographicSetttings.valueStrikeThruPos;
	sty.scaleH = 1000;
	sty.scaleV = 1000;
	sty.baseOff = 0;
	sty.kernVal = 0;
	TempVorl.append(sty);
	sFnumber = TempVorl.count()-1;
	EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true, fon, static_cast<double>(Docu->typographicSetttings.autoLineSpacing), Docu->docUnitIndex, Docu);
	if (!dia2->exec())
		TempVorl.remove(TempVorl.fromLast());
	delete dia2;
	UpdateFList();
}

void StilFormate::editFormat()
{
	EditStyle* dia = new EditStyle(this, &TempVorl[sFnumber], TempVorl, false, fon,
	                               static_cast<double>(Docu->typographicSetttings.autoLineSpacing), Docu->docUnitIndex, Docu);
	dia->exec();
	delete dia;
	UpdateFList();
}

void StilFormate::deleteFormat()
{
	int exit=QMessageBox::warning(this,
	                              tr("Warning"),
	                              tr("Do you really want to delete this Style?"),
	                              tr("No"),
	                              tr("Yes"),
	                              0, 0, 0);
	/* PFJ - 29.02.04 - Altered to use the correct QMessageBox value. It was 1 */
	/* FS - 13.03.04 the 1 is correct in this version of QMessageBox, it returns the Nr of the clicked Button either 0 or 1 or 2 */
	if (exit == 1)
	{
		ListBox1->removeItem(sFnumber);
		TempVorl.remove(TempVorl.at(sFnumber));
		UpdateFList();
	}
}

void StilFormate::loadStyles()
{
	PrefsContext* dirs = prefsFile->getContext("dirs");
	QString wdir = dirs->get("editformats", ".");
#ifdef HAVE_LIBZ
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	if (dia.exec() == QDialog::Accepted)
	{
		QString selectedFile = dia.selectedFile();
		dirs->set("editformats", selectedFile.left(selectedFile.findRev("/")));
		QValueList<ParagraphStyle> TempVorl2;
		for (uint x = 0; x < 5; ++x)
		{
			TempVorl2.append(TempVorl[x]);
		}
		Docu->loadStylesFromFile(selectedFile, &TempVorl2);
		ChooseStyles* dia2 = new ChooseStyles(this, &TempVorl2, &TempVorl);
		if (dia2->exec())
		{
			QStringList neededColors;
			neededColors.clear();
			QMap<QCheckListItem*, int>::Iterator it;
			for (it = dia2->storedStyles.begin(); it != dia2->storedStyles.end(); ++it)
			{
				struct ParagraphStyle sty;
				if (it.key()->isOn())
				{
					sty = TempVorl2[it.data()];
					TempVorl.append(sty);
					if ((!Docu->PageColors.contains(sty.SColor)) && (!neededColors.contains(sty.SColor)))
						neededColors.append(sty.SColor);
					if ((!Docu->PageColors.contains(sty.FColor)) && (!neededColors.contains(sty.FColor)))
						neededColors.append(sty.FColor);
				}
			}
			if (!neededColors.isEmpty())
			{
				ScriXmlDoc *ss = new ScriXmlDoc();
				if (ss->ReadColors(selectedFile))
				{
					ColorList LColors = ss->Farben;
					ColorList::Iterator itc;
					for (itc = LColors.begin(); itc != LColors.end(); ++itc)
					{
						if (neededColors.contains(itc.key()))
							Docu->PageColors.insert(itc.key(), itc.data());
					}
				}
				delete ss;
			}
		}
		UpdateFList();
	}
	else
		return;
}

void StilFormate::UpdateFList()
{
	ListBox1->clear();
	if (TempVorl.count() < 6)
		return;
	for (uint x = 5; x < TempVorl.count(); ++x)
		ListBox1->insertItem(TempVorl[x].Vname);
	if (ListBox1->currentItem() == -1)
	{
		DublicateB->setEnabled(false);
		EditB->setEnabled(false);
		DeleteB->setEnabled(false);
	}
	ListBox1->sort( true );
	ListBox1->setSelected(ListBox1->currentItem(), false);
}
