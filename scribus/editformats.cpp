/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "editformats.h"
#include "editformats.moc"
#include "edit1format.h"
#include <qmessagebox.h>
#include <qheader.h>

#include "commonstrings.h"
#include "scribusdoc.h"
#include "customfdialog.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "scribusXml.h"
#include "page.h"
#include "sccombobox.h"
#include "util.h"

extern QPixmap loadIcon(QString nam);


DelStyle::DelStyle(QWidget* parent, QValueList<ParagraphStyle> sty, QString styleName)
		: QDialog( parent, "DelStyle", true, 0 )
{
	setName( "DelStyle" );
	setCaption( tr( "Delete Style" ) );
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this, 10, 5 );
	delStyleLayout = new QGridLayout;
	delStyleLayout->setSpacing( 5 );
	delStyleLayout->setMargin( 5 );
	deleteLabel = new QLabel( tr( "Delete Style:" ), this, "deleteLabel" );
	delStyleLayout->addWidget( deleteLabel, 0, 0 );
	styleToDelLabel = new QLabel( styleName, this, "colorToDelLabel" );
	delStyleLayout->addWidget( styleToDelLabel, 0, 1 );
	replaceLabel = new QLabel( tr( "Replace With:" ), this, "replaceLabel" );
	delStyleLayout->addWidget( replaceLabel, 1, 0 );
	replacementStyleData = new ScComboBox(false, this);
	replacementStyleData->insertItem( tr("No Style"));

	// sort the names in language specific order (PV)
	QStringList existingStyles;
	for (uint x = 5; x < sty.count(); ++x)
	{
		if (sty[x].name() != styleName)
			existingStyles.append(sty[x].name());
	}
	existingStyles = sortQStringList(existingStyles);
	replacementStyleData->insertStringList(existingStyles);

	delStyleLayout->addWidget( replacementStyleData, 1, 1 );
	replacementStyle = replacementStyleData->text(0);
	dialogLayout->addLayout( delStyleLayout );
	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "PushButton13" );
	cancelButton->setDefault( true );
	okCancelLayout->addWidget( cancelButton );
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( replacementStyleData, SIGNAL(activated(int)), this, SLOT( ReplaceStyle(int) ) );
}

void DelStyle::ReplaceStyle(int id)
{
	replacementStyle = replacementStyleData->text(id);
}

const QString DelStyle::getReplacementStyle()
{
	return replacementStyle;
}

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
		ParagraphStyle vg;
		ParagraphStyle vg2;
		vg = (*styleList)[x];
		bool found = false;
		for (uint xx=0; xx<styleOld->count(); ++xx)
		{
			vg2 = (*styleOld)[xx];
			if (vg.name() == vg2.name())
			{
				if (vg.equiv(vg2))
				{
					found = true;
				}
				else
				{
					vg.setName("Copy of "+vg2.name());
					found = false;
				}
				break;
			}
		}
		if (!found)
		{
			QCheckListItem *item = new QCheckListItem (StyleView, vg.name(), QCheckListItem::CheckBox);
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
	OkButton = new QPushButton( CommonStrings::tr_OK, this, "OkButton" );
	layout2->addWidget( OkButton );
	CancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "CancelButton" );
	layout2->addWidget( CancelButton );
	ChooseStylesLayout->addLayout( layout2 );
	resize(230, 280);
	clearWState( WState_Polished );
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
}

StilFormate::StilFormate( QWidget* parent, ScribusDoc *doc) : QDialog( parent, "Formate", true, 0)
{
	resize( 327, 260 );
	setCaption( tr( "Edit Styles" ) );
	setIcon(loadIcon("AppIcon.png"));
	Docu = doc;
	ReplaceList.clear();
	StilFormateLayout = new QHBoxLayout( this );
	StilFormateLayout->setSpacing( 5 );
	StilFormateLayout->setMargin( 10 );

	ListBox1 = new QListBox( this, "ListBox1" );
	ListBox1->setMinimumSize( QSize( 200, 240 ) );
	StilFormateLayout->addWidget( ListBox1 );

	Layout15 = new QVBoxLayout;
	Layout15->setSpacing( 6 );
	Layout15->setMargin( 0 );

	LoadS = new QPushButton( tr( "&Import" ), this, "LoadF" );
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

	SaveB = new QPushButton( CommonStrings::tr_Save, this, "SaveB" );
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
	connect(EditB, SIGNAL(clicked()), this, SLOT(editFormat()));
	connect(NewB, SIGNAL(clicked()), this, SLOT(neuesFormat()));
	connect(LoadS, SIGNAL(clicked()), this, SLOT(loadStyles()));
	connect(DublicateB, SIGNAL(clicked()), this, SLOT(dupFormat()));
	connect(DeleteB, SIGNAL(clicked()), this, SLOT(deleteFormat()));
	connect(ListBox1, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
	connect(ListBox1, SIGNAL(selected(QListBoxItem*)), this, SLOT(selEditFormat(QListBoxItem*)));
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
		if (TempVorl[x].name() == c->text())
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
		if (TempVorl[x].name() == c->text())
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
	ParagraphStyle sty; // = TempVorl[sFnumber];
	sty.setName(tr("Copy of %1").arg(TempVorl[sFnumber].name()));
	sty.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(TempVorl[sFnumber].lineSpacingMode()));
	sty.setLineSpacing(TempVorl[sFnumber].lineSpacing());
	sty.setAlignment(TempVorl[sFnumber].alignment());
	sty.setLeftMargin(TempVorl[sFnumber].leftMargin());
	sty.setRightMargin(TempVorl[sFnumber].rightMargin());
	sty.setFirstIndent(TempVorl[sFnumber].firstIndent());
	sty.setGapBefore(TempVorl[sFnumber].gapBefore());
	sty.setGapAfter(TempVorl[sFnumber].gapAfter());
	sty.setHasDropCap(TempVorl[sFnumber].hasDropCap());
	sty.setDropCapLines(TempVorl[sFnumber].dropCapLines());
	sty.setDropCapOffset(TempVorl[sFnumber].dropCapOffset());
	sty.setUseBaselineGrid(TempVorl[sFnumber].useBaselineGrid());
	sty.tabValues() = TempVorl[sFnumber].tabValues();
	sty.charStyle() = TempVorl[sFnumber].charStyle();
	TempVorl.append(sty);
	sFnumber = TempVorl.count()-1;
	EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true,
	                                static_cast<double>(Docu->typographicSettings.autoLineSpacing), Docu->unitIndex(), Docu);
	if (!dia2->exec())
		TempVorl.remove(TempVorl.fromLast());
	delete dia2;
	UpdateFList();
}

void StilFormate::neuesFormat()
{
	int selectedIndex=ListBox1->currentItem();
	int topIndex=ListBox1->topItem();
	ParagraphStyle sty;
	sty.setName(tr("New Style"));
	sty.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(0));
	sty.setLineSpacing(((Docu->toolSettings.defSize / 10.0) * 
			static_cast<double>(Docu->typographicSettings.autoLineSpacing) / 100) 
			  + (Docu->toolSettings.defSize / 10.0));
	sty.setAlignment(0);
	sty.setLeftMargin(0);
	sty.setRightMargin(0);
	sty.setFirstIndent(0);
	sty.setGapBefore(0);
	sty.setGapAfter(0);
	sty.charStyle() = CharStyle(PrefsManager::instance()->appPrefs.AvailFonts[Docu->toolSettings.defFont],
								Docu->toolSettings.defSize);
	sty.tabValues().clear();
	sty.setHasDropCap(false);
	sty.setDropCapLines(2);
	sty.setDropCapOffset(0);
	sty.charStyle().ccolor = Docu->toolSettings.dBrush;
	sty.charStyle().cshade = Docu->toolSettings.dShade;
	sty.charStyle().cstroke = Docu->toolSettings.dPen;
	sty.charStyle().cshade2 = Docu->toolSettings.dShade2;
	sty.setUseBaselineGrid(false);
	sty.charStyle().cshadowx = 50;
	sty.charStyle().cshadowy = -50;
	sty.charStyle().coutline = 10;
	sty.charStyle().cunderpos = Docu->typographicSettings.valueUnderlinePos;
	sty.charStyle().cunderwidth = Docu->typographicSettings.valueUnderlineWidth;
	sty.charStyle().cstrikepos = Docu->typographicSettings.valueStrikeThruPos;
	sty.charStyle().cstrikewidth = Docu->typographicSettings.valueStrikeThruPos;
	sty.charStyle().cscale = 1000;
	sty.charStyle().cscalev = 1000;
	sty.charStyle().cbase = 0;
	sty.charStyle().cextra = 0;
	TempVorl.append(sty);
	sFnumber = TempVorl.count()-1;
	EditStyle* dia2 = new EditStyle(this, &TempVorl[sFnumber], TempVorl, true,  static_cast<double>(Docu->typographicSettings.autoLineSpacing), Docu->unitIndex(), Docu);
	if (!dia2->exec())
		TempVorl.remove(TempVorl.fromLast());
	delete dia2;
	UpdateFList();
	ListBox1->setSelected(selectedIndex, true);
	ListBox1->setTopItem(topIndex);
}

void StilFormate::editFormat()
{
	int selectedIndex=ListBox1->currentItem();
	int topIndex=ListBox1->topItem();
	EditStyle* dia = new EditStyle(this, &TempVorl[sFnumber], TempVorl, false,
	                               static_cast<double>(Docu->typographicSettings.autoLineSpacing), Docu->unitIndex(), Docu);
	dia->exec();
	delete dia;
	UpdateFList();
	ListBox1->setSelected(selectedIndex, true);
	ListBox1->setTopItem(topIndex);
}

void StilFormate::deleteFormat()
{
/*	int exit=QMessageBox::warning(this,
	                              CommonStrings::trWarning,
	                              tr("Do you really want to delete this style?"),
	                              tr("No"),
	                              tr("Yes"),
	                              0, 0, 0); */
	/* PFJ - 29.02.04 - Altered to use the correct QMessageBox value. It was 1 */
	/* FS - 13.03.04 the 1 is correct in this version of QMessageBox, it returns the Nr of the clicked Button either 0 or 1 or 2 */
//	if (exit == 1)
	int selectedIndex=ListBox1->currentItem();
	int topIndex=ListBox1->topItem();
	DelStyle *dia = new DelStyle(this, TempVorl, TempVorl[sFnumber].name());
	if (dia->exec())
	{
		if (ReplaceList.values().contains(TempVorl[sFnumber].name()))
		{
			QMap<QString,QString>::Iterator it;
			for (it = ReplaceList.begin(); it != ReplaceList.end(); ++it)
			{
				if (it.data() == TempVorl[sFnumber].name())
					it.data() = dia->getReplacementStyle();
			}
		}
		ReplaceList.insert(TempVorl[sFnumber].name(), dia->getReplacementStyle());
		ListBox1->removeItem(sFnumber);
		TempVorl.remove(TempVorl.at(sFnumber));
		UpdateFList();
	}
	delete dia;
	int listBoxCount=ListBox1->count();
	if (listBoxCount>selectedIndex)
		ListBox1->setSelected(selectedIndex, true);
	if (listBoxCount>topIndex)
		ListBox1->setTopItem(topIndex);
}

void StilFormate::loadStyles()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
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
				ParagraphStyle sty;
				if (it.key()->isOn())
				{
					sty = TempVorl2[it.data()];
					TempVorl.append(sty);
					if ((!Docu->PageColors.contains(sty.charStyle().cstroke)) && (!neededColors.contains(sty.charStyle().cstroke)))
						neededColors.append(sty.charStyle().cstroke);
					if ((!Docu->PageColors.contains(sty.charStyle().ccolor)) && (!neededColors.contains(sty.charStyle().ccolor)))
						neededColors.append(sty.charStyle().ccolor);
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
		ListBox1->insertItem(TempVorl[x].name());
	if (ListBox1->currentItem() == -1)
	{
		DublicateB->setEnabled(false);
		EditB->setEnabled(false);
		DeleteB->setEnabled(false);
	}
	ListBox1->sort( true );
	ListBox1->setSelected(ListBox1->currentItem(), false);
}
