/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "annot.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGroupBox>
#include <QTextEdit>
#include <QLabel>
#include <QFrame>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QWidget>
#include <QRadioButton>
#include <QList>
#include <QStringList>
#include <QSpacerItem>
#include <QToolTip>
#include <QDateTime>
#include <QPixmap>

#include "annotation.h"
#include "buttonicon.h"
#include "cmsettings.h"
#include "colorcombo.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "editor.h"
#include "iconmanager.h"
#include "navigator.h"
#include "pageitem.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "sccombobox.h"
#include "scfonts.h"
#include "scimage.h"
#include "scribusstructs.h"
#include "scribusview.h"
#include "selfield.h"
#include "ui/customfdialog.h"
#include "ui/propertiespalette_utils.h"
#include "util.h"
#include "util_formats.h"

ScAnnot::ScAnnot(QWidget* parent, PageItem *it, int Seite, int b, int h, ColorList Farben, ScribusView* vie)
		: QDialog( parent )
{
	m_annotation = it->annotation();

	setupUi(this);
	setModal(true);
	setWindowTitle( tr( "Field Properties" ) );
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon.png"));
	item = it;
	Width = b;
	Height = h;
	OriWidth = b;
	OriHeight = h;
	view = vie;
	MaxSeite = Seite;
	QStringList tl;
	dirs = PrefsManager::instance()->prefsFile->getContext("dirs");

	Annotation& annotation = m_annotation;
	if ((annotation.ActionType() == Annotation::Action_GoTo) || (annotation.ActionType() == Annotation::Action_GoToR_FileRel) || (annotation.ActionType() == Annotation::Action_GoToR_FileAbs))
	{
		QString tm = annotation.Action();
		tl = tm.split(" ", QString::SkipEmptyParts);
	}
	else
	{
		tl.append("0");
		tl.append("0");
	}

	if (annotation.Type() != Annotation::RadioButton)
		ComboBox1->setCurrentIndex(annotation.Type()-2);
	else
		ComboBox1->setCurrentIndex(5);

	PropertiesGroup->layout()->setAlignment(Qt::AlignTop);

	Tip->setText(annotation.ToolTip());
	Name->setText(item->itemName());
	OldName = item->itemName();

	TextGroup->layout()->setAlignment(Qt::AlignTop);
	Schrift->setCurrentIndex(annotation.Font());

	BorderGroup->layout()->setAlignment(Qt::AlignTop);

	ColorList::Iterator cit;
	BorderC->setPixmapType(ColorCombo::fancyPixmaps);
	BorderC->setColors(Farben, true);
	if (annotation.borderColor() == CommonStrings::None)
		BorderC->setCurrentIndex(0);
	else
		BorderC->setCurrentText(annotation.borderColor());

	// PFJ - 28/02/04 - Altered to the QString/size_t/for style
	QString borders[] = {CommonStrings::tr_NoneColor, tr("Thin"), tr("Normal"), tr("Wide")};
	size_t bordersArray = sizeof(borders) / sizeof(*borders);
	BorderW->clear();
	for (uint propagate = 0; propagate < bordersArray; ++propagate)
		BorderW->addItem(borders[propagate]);
	BorderW->setCurrentIndex(annotation.Bwid());

	BorderS->setCurrentIndex(annotation.Bsty());

	OtherGroup->layout()->setAlignment(Qt::AlignTop);

	ReadOnly->setEnabled(annotation.Type() != Annotation::Button);
	ReadOnly->setChecked(annotation.Flag() & Annotation::Flag_ReadOnly);
	Required->setEnabled(annotation.Type() != Annotation::Button);
	Required->setChecked(annotation.Flag() & Annotation::Flag_Required);
	NoExport->setEnabled(annotation.Type() != Annotation::Button);
	NoExport->setChecked(annotation.Flag() & Annotation::Flag_NoExport);
	Visib->setCurrentIndex(annotation.Vis());

	TextOptionGroup->layout()->setAlignment(Qt::AlignTop);
	DownT->setText(annotation.Down());
	TextO->setText(annotation.RollOver());

	IconOptionGroup->layout()->setAlignment(Qt::AlignTop);
	UseIcons->setChecked(annotation.UseIcons());
	IconNR->setEnabled(false);
	IconPR->setEnabled(false);
	IconRR->setEnabled(false);
	if (!UseIcons->isChecked())
	{
		IconN->setEnabled(false);
		IconP->setEnabled(false);
		IconR->setEnabled(false);
		NiconPrev->setEnabled(false);
		PiconPrev->setEnabled(false);
		RiconPrev->setEnabled(false);
		PlaceIcon->setEnabled(false);
	}
	else
	{
		QPixmap pmI1;
		ScImage im;
		CMSettings cms(view->Doc, "", Intent_Perceptual);
		cms.allowColorManagement(false);
		if (!item->Pfile.isEmpty())
		{
			im.loadPicture(item->Pfile, 1, cms, ScImage::RGBData, 72);
			pmI1=QPixmap::fromImage(im.qImage());
			NiconPrev->setPixmap(pmI1);
			IconNR->setEnabled(true);
		}
		if (!item->Pfile2.isEmpty())
		{
			im.loadPicture(item->Pfile2, 1, cms, ScImage::RGBData, 72);
			pmI1=QPixmap::fromImage(im.qImage());
			PiconPrev->setPixmap(pmI1);
			IconPR->setEnabled(true);
		}
		if (!item->Pfile3.isEmpty())
		{
			im.loadPicture(item->Pfile3, 1, cms, ScImage::RGBData, 72);
			pmI1=QPixmap::fromImage(im.qImage());
			RiconPrev->setPixmap(pmI1);
			IconRR->setEnabled(true);
		}
	}

	HighlightOptionGroup->layout()->setAlignment(Qt::AlignTop);
	ComboBox7_2->setCurrentIndex(annotation.Feed());

	MultiL->setChecked(annotation.Flag() & Annotation::Flag_Multiline);
	Passwd->setChecked(annotation.Flag() & Annotation::Flag_Password);
	MaxChars->setMinimum(0);
	MaxChars->setMaximum(32768);
	bool setter = annotation.MaxChar() != -1 ? true : false;
	MaxChars->setValue(setter == true ? annotation.MaxChar() : 0);
	Limit->setChecked(setter);
	MaxChars->setEnabled(setter);
	NoScroll->setChecked(annotation.Flag() & Annotation::Flag_DoNotScroll);
	NoSpell->setChecked(annotation.Flag() & Annotation::Flag_DoNotSpellCheck);

	ChkStil->setCurrentIndex(annotation.ChkStil());
	isChkd->setChecked(annotation.IsChk());

	CanEdit->setChecked(annotation.Flag() & Annotation::Flag_Edit);

	int tmpac = annotation.ActionType();
	if (annotation.ActionType() < 0)
		tmpac = 1;
	if ((tmpac == 7) || (tmpac == 9))
		tmpac = 2;
	else if (tmpac == 10)
		tmpac = 6;
	ActionCombo->setCurrentIndex(tmpac);

	QPalette palTxt = EditJava->palette();
	palTxt.setColor(QPalette::Base, palette().color(QPalette::Window));
	if ((annotation.ActionType() == Annotation::Action_JavaScript) || (annotation.AAact()))
		EditJava->setPlainText(annotation.Action());
	ScrEdited = Annotation::Java_ReleaseButton;
	SelAction->setCurrentIndex(0);
	EditJava->setReadOnly(true);
	EditJava->setAutoFillBackground(true);
	EditJava->setPalette(palTxt);
	EditJava->setMinimumSize( QSize( 0, 50 ) );
	EditJava->setFocusPolicy(Qt::NoFocus);

	GroupBoxDestination->layout()->setAlignment(Qt::AlignTop);
	Destfile->setText(annotation.Extern());
	Destfile->setReadOnly(true);
	if (annotation.ActionType() == Annotation::Action_GoToR_FileRel)
		useAbsolute->setChecked(false);
	else if (annotation.ActionType() == Annotation::Action_GoToR_FileAbs)
		useAbsolute->setChecked(true);
	SpinBox11->setMinimum(1);
	SpinBox11->setMaximum(((annotation.ActionType() == Annotation::Action_GoToR_FileRel) || (annotation.ActionType() == Annotation::Action_GoToR_FileAbs)) ? 1000 : Seite);
	SpinBox11->setValue(qMin(annotation.Ziel()+1, Seite));

	if ((annotation.ActionType() == Annotation::Action_GoToR_FileRel) || (annotation.ActionType() == Annotation::Action_GoToR_FileAbs))
		Pg1 = new Navigator( GroupBoxDestination, 100, annotation.Ziel()+1, view, annotation.Extern());
	else
		Pg1 = new Navigator( GroupBoxDestination, 100, qMin(annotation.Ziel(), Seite-1), view);
	Pg1->setMinimumSize(QSize(Pg1->pmx.width(), Pg1->pmx.height()));
	GroupBoxDestinationLayout->addWidget(Pg1, 2, 2, 3, 1);

	SpinBox21->setSuffix( tr( " pt" ) );
	SpinBox21->setMaximum(Width);
	SpinBox21->setValue(tl[0].toInt());
	SpinBox31->setMaximum(Height);
	SpinBox31->setSuffix( tr( " pt" ) );
	SpinBox31->setValue(Height-tl[1].toInt());

	if (annotation.ActionType() == Annotation::Action_SubmitForm)
		SubURL->setText(annotation.Action());
	SelAsHtml->setCurrentIndex(annotation.HTML());

	if (annotation.ActionType() == Annotation::Action_ImportData)
		SubURLa->setText(annotation.Action());

	nameActionCombo->addItem( tr("First Page"), QString("FirstPage"));
	nameActionCombo->addItem( tr("Previous Page"), QString("PrevPage"));
	nameActionCombo->addItem( tr("Next Page"), QString("NextPage"));
	nameActionCombo->addItem( tr("Last Page"), QString("LastPage"));
	nameActionCombo->addItem( tr("Go Back"), QString("GoBack"));
	nameActionCombo->addItem( tr("Go Forward"), QString("GoForward"));
	nameActionCombo->addItem( tr("Go Back Doc"), QString("GoBackDoc"));
	nameActionCombo->addItem( tr("Go Forward Doc"), QString("GoForwardDoc"));
	nameActionCombo->addItem( tr("Go To Page"), QString("GoToPage"));
	nameActionCombo->addItem( tr("Actual Size"), QString("ActualSize"));
	nameActionCombo->addItem( tr("Fit Page"), QString("FitPage"));
	nameActionCombo->addItem( tr("Fit Width"), QString("FitWidth"));
	nameActionCombo->addItem( tr("Fit Height"), QString("FitHeight"));
	nameActionCombo->addItem( tr("Fit Visible"), QString("FitVisible"));
	nameActionCombo->addItem( tr("Single Page"), QString("SinglePage"));
	nameActionCombo->addItem( tr("Two Pages"), QString("TwoPages"));
	nameActionCombo->addItem( tr("One Column"), QString("OneColumn"));
	nameActionCombo->addItem( tr("Two Columns"), QString("TwoColumns"));
	nameActionCombo->addItem( tr("Zoom View In"), QString("ZoomViewIn"));
	nameActionCombo->addItem( tr("Zoom View Out"), QString("ZoomViewOut"));
	nameActionCombo->addItem( tr("Zoom To"), QString("ZoomTo"));
	nameActionCombo->addItem( tr("Print"), QString("Print"));
	nameActionCombo->addItem( tr("Close"), QString("Close"));
	nameActionCombo->addItem( tr("Quit"), QString("Quit"));
	nameActionCombo->addItem( tr("General Prefs"), QString("GeneralPrefs"));
	nameActionCombo->addItem( tr("General Info"), QString("GeneralInfo"));
	nameActionCombo->addItem( tr("Page Setup"), QString("PageSetup"));
	nameActionCombo->addItem( tr("Full Screen"), QString("FullScreen"));
	nameActionCombo->addItem( tr("Show/Hide Bookmarks"), QString("ShowHideBookmarks"));
	nameActionCombo->addItem( tr("Show/Hide Thumbnails"), QString("ShowHideThumbnails"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Editing"), QString("ShowHideToolbarEditing"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Commenting"), QString("ShowHideToolbarCommenting"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Edit"), QString("ShowHideToolbarEdit"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar File"), QString("ShowHideToolbarFile"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Find"), QString("ShowHideToolbarFind"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Forms"), QString("ShowHideToolbarForms"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Measuring"), QString("ShowHideToolbarMeasuring"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Data"), QString("ShowHideToolbarData"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Page Display"), QString("ShowHideToolbarPageDisplay"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Navigation"), QString("ShowHideToolbarNavigation"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Print Production"), QString("ShowHideToolbarPrintProduction"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Redaction"), QString("ShowHideToolbarRedaction"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Basic Tools"), QString("ShowHideToolbarBasicTools"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Tasks"), QString("ShowHideToolbarTasks"));
	nameActionCombo->addItem( tr("Show/Hide Toolbar Typewriter"), QString("ShowHideToolbarTypewriter"));
	nameActionCombo->addItem( tr("Show/Hide Articles"), QString("ShowHideArticles"));
	nameActionCombo->addItem( tr("Show/Hide File Attachment"), QString("ShowHideFileAttachment"));
	nameActionCombo->addItem( tr("Show/Hide Annot Manager"), QString("ShowHideAnnotManager"));
	nameActionCombo->addItem( tr("Show/Hide Fields"), QString("ShowHideFields"));
	nameActionCombo->addItem( tr("Show/Hide Optional Content"), QString("ShowHideOptCont"));
	nameActionCombo->addItem( tr("Show/Hide Model Tree"), QString("ShowHideModelTree"));
	nameActionCombo->addItem( tr("Show/Hide Signatures"), QString("ShowHideSignatures"));

	if ((annotation.ActionType() == Annotation::Action_GoToR_FileRel) || (annotation.ActionType() == Annotation::Action_GoToR_FileAbs))
	{
		LExtern->setChecked(true);
		if (!Destfile->text().isEmpty())
		{
			Width = Pg1->Width;
			Height = Pg1->Height;
		}
		else
		{
			Destfile->setEnabled(false);
			ChFile->setEnabled(false);
			LExtern->setChecked(false);
		}
	}
	else
	{
		Destfile->setEnabled(false);
		ChFile->setEnabled(false);
		LExtern->setChecked(false);
	}

	// Format tab settings
	TxFormat->setCurrentIndex(annotation.Format());

	NumbGroup->layout()->setAlignment( Qt::AlignTop );
	Decim->setMinimum(0);
	Decim->setMaximum(12);
	Decim->setValue(0);
	CurSym->setEnabled(false);
	PreCurr->setEnabled(false);

	FormNum = 0;

	Decim2->setMinimum(0);
	Decim2->setMaximum(12);
	Decim2->setValue(0);

	NumGroup2a->layout()->setAlignment(Qt::AlignTop);

	GroupCust->layout()->setAlignment(Qt::AlignTop);
	EditFormat->setAutoDefault(false);
	if (annotation.Format() != 5)
		EditFormat->setEnabled( false );
	if (annotation.Format() == 5)
		FormatScript->setPlainText( annotation.F_act() );
	FormatScript->setReadOnly(true);
	FormatScript->setAutoFillBackground(true);
	FormatScript->setPalette(palTxt);
	FormatScript->setMaximumSize(QSize(32000,50));
	FormatScript->setFocusPolicy(Qt::NoFocus);
	EditKeystr->setAutoDefault( false );
	if (annotation.Format() != 5)
		EditKeystr->setEnabled( false );
	if (annotation.Format() == 5)
		KeyScript->setPlainText( annotation.K_act() );
	KeyScript->setReadOnly(true);
	KeyScript->setAutoFillBackground(true);
	KeyScript->setPalette(palTxt);
	KeyScript->setMaximumSize(QSize(32000,50));
	KeyScript->setFocusPolicy(Qt::NoFocus);

	DecodeNum();

	TabWidget2->setTabEnabled(TabWidget2->indexOf(tabFormat), false);


	// Value validation settings
	/*ValidateGroup->layout()->setAlignment(Qt::AlignTop);*/
	ValidScript->setMinimumSize( QSize( 0, 50 ) );
	ValidScript->setFocusPolicy(Qt::NoFocus);
	ValidScript->setReadOnly( true );
	ValidScript->setAutoFillBackground(true);
	ValidScript->setPalette(palTxt);
	EditValScript->setAutoDefault( false );

	TabWidget2->setTabEnabled(TabWidget2->indexOf(tabValidate), false);
	SetVali();

	// Value calculation settings
	/*CalcGroup->layout()->setAlignment(Qt::AlignTop);*/
	CalcScript->setMinimumSize( QSize( 0, 50 ) );
	CalcScript->setFocusPolicy(Qt::NoFocus);
	CalcScript->setReadOnly( true );
	CalcScript->setAutoFillBackground(true);
	CalcScript->setPalette(palTxt);

	TabWidget2->setTabEnabled(TabWidget2->indexOf(tabCalculate), false);
	SetCalc();

	if (annotation.Type() == Annotation::RadioButton)
		SetAnnotationType(5);
	else
		SetAnnotationType(annotation.Type()-2);

	PushButton1->setDefault( true );

	connect(PushButton1, SIGNAL(clicked()), this, SLOT(SetValues()));
	connect(PushButton2, SIGNAL(clicked()), this, SLOT(reject()));
	connect(EditFormat, SIGNAL(clicked()), this, SLOT(editFormatSc()));
	connect(EditKeystr, SIGNAL(clicked()), this, SLOT(editKeySc()));
	connect(EditValScript, SIGNAL(clicked()), this, SLOT(editValidSc()));
	connect(EditCalc, SIGNAL(clicked()), this, SLOT(editCalcSc()));
	connect(EditJ, SIGNAL(clicked()), this, SLOT(editJavaSc()));
	connect(SeField, SIGNAL(clicked()), this, SLOT(SelectFelder()));
	connect(Format0c, SIGNAL(activated(const QString&)), this, SLOT(setDateSample(const QString&)));
	connect(TxFormat, SIGNAL(activated(int)), this, SLOT(SetFoScript(int)));
	connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(SetAnnotationType(int)));
	connect(ActionCombo, SIGNAL(activated(int)), this, SLOT(SetActionType(int)));
	connect(SelAction, SIGNAL(activated(int)), this, SLOT(SetActionScript(int)));
	connect(Pg1, SIGNAL(Coords(double, double)), this, SLOT(SetCoords(double, double)));
	connect(SpinBox11, SIGNAL(valueChanged(int)), this, SLOT(SetPage(int)));
	connect(SpinBox21, SIGNAL(valueChanged(int)), this, SLOT(SetCross()));
	connect(SpinBox31, SIGNAL(valueChanged(int)), this, SLOT(SetCross()));
	connect(Limit, SIGNAL(clicked()), this, SLOT(SetLimit()));
	connect(UseCurr, SIGNAL(clicked()), this, SLOT(SetCurr()));
	connect(Format0, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format1, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format2, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format3, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format0a, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format1a, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format2a, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format3a, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format0b, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format1b, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format2b, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(Format3b, SIGNAL(clicked()), this, SLOT(SetFormNum()));
	connect(NoValid, SIGNAL(clicked()), this, SLOT(HandleVali()));
	connect(SimpleValid, SIGNAL(clicked()), this, SLOT(HandleVali()));
	connect(CustomValid, SIGNAL(clicked()), this, SLOT(HandleVali()));
	connect(NoCalc, SIGNAL(clicked()), this, SLOT(HandleCalc()));
	connect(SimpleCalc, SIGNAL(clicked()), this, SLOT(HandleCalc()));
	connect(CustomCalc, SIGNAL(clicked()), this, SLOT(HandleCalc()));
	connect(IconN, SIGNAL(clicked()), this, SLOT(GetNIcon()));
	connect(IconNR, SIGNAL(clicked()), this, SLOT(RemoveNIcon()));
	connect(IconP, SIGNAL(clicked()), this, SLOT(GetPIcon()));
	connect(IconPR, SIGNAL(clicked()), this, SLOT(RemovePIcon()));
	connect(IconR, SIGNAL(clicked()), this, SLOT(GetRIcon()));
	connect(IconRR, SIGNAL(clicked()), this, SLOT(RemoveRIcon()));
	connect(UseIcons, SIGNAL(clicked()), this, SLOT(IconsEin()));
	connect(PlaceIcon, SIGNAL(clicked()), this, SLOT(IPlace()));
	connect(ChFile, SIGNAL(clicked()), this, SLOT(GetFile()));
	connect(LExtern, SIGNAL(clicked()), this, SLOT(SetExternLink()));
	connect(Name, SIGNAL(Leaved()), this, SLOT(NewName()));

	NoSpell->setToolTip( tr( "Flag is ignored for PDF 1.3" ) );
	NoScroll->setToolTip( tr( "Flag is ignored for PDF 1.3" ) );
	CalcFields->setToolTip( tr( "Enter a comma separated list of fields here" ) );
	IconNR->setToolTip( tr("You need at least the Icon for Normal to use Icons for Buttons"));

	SetPage(qMin(SpinBox11->value(), MaxSeite));
	SetCross();
	//resize( minimumSizeHint() );
}

ScAnnot::~ScAnnot()
{}

void ScAnnot::NewName()
{
	QString NameNew = Name->text();
	if (NameNew.isEmpty())
	{
		Name->setText(OldName);
		return;
	}
	bool found = false;
	QList<PageItem*> allItems;
	for (int a = 0; a < view->Doc->Items->count(); ++a)
	{
		PageItem *currItem = view->Doc->Items->at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			PageItem* ite = allItems.at(ii);
			if ((NameNew == ite->itemName()) && (ite != item))
			{
				found = true;
				break;
			}
		}
		allItems.clear();
	}
	if (found)
	{
		Name->setText(OldName);
		Name->setFocus();
	}
}

void ScAnnot::IPlace()
{
	ButtonIcon* dia = new ButtonIcon(this, item);
	if (dia->exec())
	{
		int w = item->pixm.width();
		int h = item->pixm.height();
		double sw = item->width() / w;
		double sh = item->height() / h;
		double sc = qMin(sw, sh);
		if (dia->IcScaleH == 3)
		{
			item->setImageXYScale(1.0, 1.0);
			item->setImageXYOffset((item->width()-w)*dia->IcPlaceX, (item->height()-h)*dia->IcPlaceY);
		}
		else
		{
			if (dia->ScaleH->currentIndex() == 0)
			{
				item->setImageXYScale(sc, sc);
				item->setImageXYOffset(((item->width()-w*sc)/sc)/2.0/ sc, ((item->height()-h*sc)/sc)/2.0/sc);
			}
			else
			{
				item->setImageXYScale(sw, sh);
				item->setImageXYOffset(0.0, 0.0);
			}
		}
		m_annotation.setIPlace(dia->Place->currentIndex());
		m_annotation.setScaleW(dia->ScaleW->currentIndex());
	}
	delete dia;
}

void ScAnnot::RemoveNIcon()
{
	NiconPrev->clear();
	item->Pfile = "";
	item->imageIsAvailable = false;
	IconNR->setEnabled(false);
}

void ScAnnot::RemovePIcon()
{
	PiconPrev->clear();
	item->Pfile2 = "";
	IconPR->setEnabled(false);
}

void ScAnnot::RemoveRIcon()
{
	RiconPrev->clear();
	item->Pfile3 = "";
	IconRR->setEnabled(false);
}

void ScAnnot::IconsEin()
{
	bool setter = !UseIcons->isChecked() ? false : true;
	IconN->setEnabled(setter);
	IconP->setEnabled(setter);
	IconR->setEnabled(setter);

	NiconPrev->setEnabled(setter);
	PiconPrev->setEnabled(setter);
	RiconPrev->setEnabled(setter);
	PlaceIcon->setEnabled(setter);
	IconNR->setEnabled(!item->Pfile.isEmpty() ? true : false);
	IconPR->setEnabled(!item->Pfile2.isEmpty() ? true : false);
	IconRR->setEnabled(!item->Pfile3.isEmpty() ? true : false);
	m_annotation.setUseIcons(UseIcons->isChecked());
}

void ScAnnot::GetNIcon()
{
	QString fileName;
	QString wdir = dirs->get("icon", ".");
	CustomFDialog dia(this, wdir, tr("Open"),
					  tr("Images (*.tif *.png *.jpg *.xpm);;%1;;All Files (*)").arg(FormatsManager::instance()->extensionsForFormat(FormatsManager::EPS)), fdShowPreview | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("icon", fileName.left(fileName.lastIndexOf("/")));
		QPixmap pmI1;
		CMSettings cms(view->Doc, "", Intent_Perceptual);
		cms.allowColorManagement(false);
		item->pixm.loadPicture(fileName, 1, cms, ScImage::RGBData, 72);
		pmI1=QPixmap::fromImage(item->pixm.qImage());
		NiconPrev->setPixmap(pmI1);
		item->Pfile = fileName;
		item->imageIsAvailable = true;
		int w = item->pixm.width();
		int h = item->pixm.height();
		double sw = item->width() / w;
		double sh = item->height() / h;
		double sc = qMin(sw,sh);
		item->setImageXYScale(sc, sc);
		item->setImageXYOffset(((item->width()-(w*sc))/2)/sc, ((item->height()-(h*sc))/2)/sc);
		IconNR->setEnabled(true);
	}
}

void ScAnnot::GetPIcon()
{
	QString fileName;
	QString wdir = dirs->get("icon", ".");
	CustomFDialog dia(this, wdir, tr("Open"),
	                  tr("Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)"), fdShowPreview | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("icon", fileName.left(fileName.lastIndexOf("/")));
		QPixmap pmI1;
		ScImage im;
		CMSettings cms(view->Doc, "", Intent_Perceptual);
		cms.allowColorManagement(false);
		im.loadPicture(fileName, 1, cms, ScImage::RGBData, 72);
		pmI1=QPixmap::fromImage(im.qImage());
		PiconPrev->setPixmap(pmI1);
		item->Pfile2 = fileName;
		IconPR->setEnabled(true);
	}
}

void ScAnnot::GetRIcon()
{
	QString fileName;
	QString wdir = dirs->get("icon", ".");
	CustomFDialog dia(this, wdir, tr("Open"),
	                  tr("Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)"), fdShowPreview | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("icon", fileName.left(fileName.lastIndexOf("/")));
		QPixmap pmI1;
		ScImage im;
		CMSettings cms(view->Doc, "", Intent_Perceptual);
		cms.allowColorManagement(false);
		im.loadPicture(fileName, 1, cms, ScImage::RGBData, 72);
		pmI1=QPixmap::fromImage(im.qImage());
		RiconPrev->setPixmap(pmI1);
		item->Pfile3 = fileName;
		IconRR->setEnabled(true);
	}
}

void ScAnnot::SelectFelder()
{
	SelectFields* dia = new SelectFields(this, CalcFields->text(), item->itemName(), view->Doc, 3);
	if (dia->exec())
		CalcFields->setText(dia->S_Fields);
	delete dia;
}

void ScAnnot::editKeySc()
{
	Editor* dia = new Editor(this, m_annotation.K_act(), view);
	if (dia->exec())
	{
		m_annotation.setK_act(dia->EditTex->toPlainText());
		KeyScript->setPlainText( m_annotation.K_act() );
	}
	delete dia;
}

void ScAnnot::editFormatSc()
{
	Editor* dia = new Editor(this, m_annotation.F_act(), view);
	if (dia->exec())
	{
		m_annotation.setF_act(dia->EditTex->toPlainText());
		FormatScript->setPlainText( m_annotation.F_act() );
	}
	delete dia;
}

void ScAnnot::editValidSc()
{
	Editor* dia = new Editor(this, m_annotation.V_act(), view);
	if (dia->exec())
	{
		m_annotation.setV_act(dia->EditTex->toPlainText());
		ValidScript->setPlainText( m_annotation.V_act() );
	}
	delete dia;
}

void ScAnnot::editCalcSc()
{
	Editor* dia = new Editor(this, m_annotation.C_act(), view);
	if (dia->exec())
	{
		m_annotation.setC_act(dia->EditTex->toPlainText());
		CalcScript->setPlainText( m_annotation.C_act() );
	}
	delete dia;
}

void ScAnnot::editJavaSc()
{
	Editor* dia = new Editor(this, EditJava->toPlainText(), view);
	if (dia->exec())
		EditJava->setPlainText(dia->EditTex->toPlainText());
	delete dia;
}

void ScAnnot::setDateSample(const QString& ds)
{
	QDateTime dt = QDateTime::currentDateTime();
	QString tmp = "";
	if (ds == "m/d")
		tmp = "M/d";
	if (ds == "m/d/yy")
		tmp = "M/d/yy";
	if (ds == "mm/dd/yy")
		tmp = "MM/dd/yy";
	if (ds == "mm/yy")
		tmp = "MM/yy";
	if (ds == "d-mmm")
		tmp = "d-MMM";
	if (ds == "d-mmm-yy")
		tmp = "d-MMM-yy";
	if (ds == "dd-mmm-yy")
		tmp = "dd-MMM-yy";
	if (ds == "dd/mm/yy")
		tmp = "dd/MM/yy";
	if (ds == "dd.mm.yyyy")
		tmp = "dd.MM.yyyy";
	if (ds == "dd/mm/yyyy")
		tmp = "dd/MM/yyyy";
	if (ds == "yy-mm-dd")
		tmp = "yy-MM-dd";
	if (ds == "mmm-yy")
		tmp = "MMM-yy";
	if (ds == "mmmm-yy")
		tmp = "MMMM-yy";
	if (ds == "mmm d, yyyy")
		tmp = "MMM d, yyyy";
	if (ds == "mmmm d, yyyy")
		tmp = "MMMM d, yyyy";
	if (ds == "m/d/yy h:MM tt")
		tmp = "M/d/yy h:mm ap";
	if (ds == "m/d/yy HH:MM")
		tmp = "M/d/yy hh:mm";
	if (ds == "yyyy-mm-dd")
		tmp = "yyyy-MM-dd";
	TextDa1->setText( tr("Example:")+" "+dt.toString(tmp));
}

void ScAnnot::DecodeVali()
{
	QString pfor = m_annotation.V_act();
	int ss = pfor.indexOf("(");
	QString pfo = pfor.mid(ss+1, pfor.length()-ss-2);
	QStringList pfol;
	pfol = pfo.split(",", QString::SkipEmptyParts);
	MinValid->setText(pfol[1].simplified());
	MaxValid->setText(pfol[3].simplified());
}

void ScAnnot::DecodeCalc()
{
	QString tm = "";
	QString tm2;
	QString pfor = m_annotation.C_act();
	int ss = pfor.lastIndexOf("(");
	QString pfo = pfor.mid(ss+1, pfor.length()-ss-3);
	QStringList pfol;
	pfol = pfo.split(",", QString::SkipEmptyParts);
	if (pfol.count() > 1)
	{
		tm2 = pfol[0].simplified();
		tm += tm2.midRef(1, tm2.length()-2);
		for (int cfx = 1; cfx < pfol.count(); ++cfx)
		{
			tm2 = pfol[cfx].simplified();
			tm += ", "+tm2.mid(1, tm2.length()-2);
		}
	}
	CalcFields->setText(tm);
	ss = pfor.indexOf("(");
	pfo = pfor.mid(ss+1, pfor.length()-ss-3);
	pfol = pfo.split(",", QString::SkipEmptyParts);
	/* PFJ - 28/02/04 - Let's get rid of lots of ifs.... */
	QString pf[] = {"\"SUM\"", "\"PRD\"", "\"AVG\"", "\"MIN\"", "\"MAX\""};
	size_t pfTest = sizeof(pf) / sizeof(*pf);
	for (uint test = 0; test < pfTest; ++test)
	{
		if (pfol[0] == pf[test])
			CalcArt->setCurrentIndex(test);
	}
}

void ScAnnot::DecodeNum()
{
	QString pfor = m_annotation.F_act();
	int ss = pfor.indexOf("(");
	QString pfo = pfor.mid(ss+1, pfor.length()-ss-2);
	QStringList pfol;
	pfol = pfo.split(",", QString::SkipEmptyParts);
	if (m_annotation.Format() == 1)
	{
		Decim->setValue(pfol[0].toInt());
		switch (pfol[1].toInt())
		{
			case 0:
				Format0->setChecked(true);
				FormNum = 0;
				break;
			case 1:
				Format1->setChecked(true);
				FormNum = 1;
				break;
			case 2:
				Format2->setChecked(true);
				FormNum = 2;
				break;
			case 3:
				Format3->setChecked(true);
				FormNum = 3;
				break;
		}
		if (pfol[5] == " true")
			PreCurr->setChecked(true);
		else
			PreCurr->setChecked(false);
		if (pfol[4].length() > 2)
		{
			if (PreCurr->isChecked())
				CurSym->setText(pfol[4].mid(2,pfol[4].length()-4));
			else
				CurSym->setText(pfol[4].mid(3,pfol[4].length()-4));
		}
		else
			CurSym->setText("");
		if (!CurSym->text().isEmpty())
		{
			CurSym->setEnabled(true);
			UseCurr->setEnabled(true);
			UseCurr->setChecked(true);
			PreCurr->setEnabled(true);
		}
	}
	if (m_annotation.Format() == 2)
	{
		Decim2->setValue(pfol[0].toInt());
		switch (pfol[1].toInt())
		{
			case 0:
				Format0a->setChecked(true);
				FormNum = 0;
				break;
			case 1:
				Format1a->setChecked(true);
				FormNum = 1;
				break;
			case 2:
				Format2a->setChecked(true);
				FormNum = 2;
				break;
			case 3:
				Format3a->setChecked(true);
				FormNum = 3;
				break;
		}
	}
	if (m_annotation.Format() == 3)
	{
		setCurrentComboItem(Format0c, pfol[0].remove("\""));
		setDateSample(pfol[0]);
	}
	if (m_annotation.Format() == 4)
	{
		switch (pfol[0].toInt())
		{
			case 0:
				Format0b->setChecked(true);
				FormNum = 0;
				break;
			case 1:
				Format1b->setChecked(true);
				FormNum = 1;
				break;
			case 2:
				Format2b->setChecked(true);
				FormNum = 2;
				break;
			case 3:
				Format3b->setChecked(true);
				FormNum = 3;
				break;
		}
	}
}

void ScAnnot::SetFormNum()
{
	switch (m_annotation.Format())
	{
	case 1:
		if (Format0->isChecked())
			FormNum = 0;
		if (Format1->isChecked())
			FormNum = 1;
		if (Format2->isChecked())
			FormNum = 2;
		if (Format3->isChecked())
			FormNum = 3;
		break;
	case 2:
		if (Format0a->isChecked())
			FormNum = 0;
		if (Format1a->isChecked())
			FormNum = 1;
		if (Format2a->isChecked())
			FormNum = 2;
		if (Format3a->isChecked())
			FormNum = 3;
		break;
	case 4:
		if (Format0b->isChecked())
			FormNum = 0;
		if (Format1b->isChecked())
			FormNum = 1;
		if (Format2b->isChecked())
			FormNum = 2;
		if (Format3b->isChecked())
			FormNum = 3;
		break;
	}
}

void ScAnnot::HandleVali()
{
	bool setter = SimpleValid->isChecked() ? true : false;
	MaxValid->setEnabled(setter);
	MinValid->setEnabled(setter);
	EditValScript->setEnabled(false);
	if (CustomValid->isChecked())
		EditValScript->setEnabled(true);
}

void ScAnnot::SetVali()
{
	MaxValid->setEnabled(false);
	MinValid->setEnabled(false);
	EditValScript->setEnabled(false);
	ValidScript->setEnabled(false);
	if (m_annotation.V_act().isEmpty())
		NoValid->setChecked(true);
	else
	{
		if (m_annotation.V_act().startsWith("AFRange_Validate"))
		{
			MaxValid->setEnabled(true);
			MinValid->setEnabled(true);
			SimpleValid->setChecked(true);
			DecodeVali();
		}
		else
		{
			EditValScript->setEnabled(true);
			CustomValid->setChecked(true);
			ValidScript->setPlainText(m_annotation.V_act());
		}
	}
}

void ScAnnot::HandleCalc()
{
	bool setter = SimpleCalc->isChecked() ? true : false;
	CalcFields->setEnabled(setter);
	CalcArt->setEnabled(setter);
	EditCalc->setEnabled(false);
	SeField->setEnabled(setter);
	if (CustomCalc->isChecked())
		EditCalc->setEnabled(true);
}

void ScAnnot::SetCalc()
{
	CalcFields->setEnabled(false);
	CalcArt->setEnabled(false);
	EditCalc->setEnabled(false);
	SeField->setEnabled(false);
	if (m_annotation.C_act().isEmpty())
		NoCalc->setChecked(true);
	else
	{
		if (m_annotation.C_act().startsWith("AFSimple_Calculate"))
		{
			CalcFields->setEnabled(true);
			CalcArt->setEnabled(true);
			SimpleCalc->setChecked(true);
			SeField->setEnabled(true);
			DecodeCalc();
		}
		else
		{
			EditCalc->setEnabled(true);
			CustomCalc->setChecked(true);
			CalcScript->setPlainText(m_annotation.C_act());
		}
	}
}

void ScAnnot::SetCurr()
{
	bool setter = UseCurr->isChecked() ? true : false;
	CurSym->setEnabled(setter);
	PreCurr->setEnabled(setter);
}

void ScAnnot::SetFoScript(int it)
{
	if (it >= 0 && it <=5)
		FoFram->setCurrentIndex(it);
	if (it == 1)
		SetCurr();
	if (it == 5)
	{
		EditFormat->setEnabled( true );
		EditKeystr->setEnabled( true );
//		KeyScript->setPlainText("");
//		FormatScript->setPlainText("");
		KeyScript->setText(m_annotation.K_act());
		FormatScript->setText(m_annotation.F_act());
	}
	m_annotation.setFormat(it);
}

void ScAnnot::SetCoords(double x, double y)
{
	SpinBox21->setValue(static_cast<int>(x*Width));
	SpinBox31->setValue(static_cast<int>(y*Height));
}

void ScAnnot::SetPage(int v)
{
	disconnect(SpinBox11, SIGNAL(valueChanged(int)), this, SLOT(SetPage(int)));
	if ((m_annotation.ActionType() == Annotation::Action_GoToR_FileRel) || (m_annotation.ActionType() == 9))
	{
		if (!Pg1->SetSeite(v, 100, Destfile->text()))
		{
			SpinBox11->setValue(1);
			Pg1->SetSeite(1, 100, Destfile->text());
		}
		Width = Pg1->Width;
		Height = Pg1->Height;
		//		SetCoords(0,0);
	}
	else
	{
		Pg1->SetSeite(qMin(v-1, MaxSeite-1), 100);
		SpinBox11->setValue(qMin(v, MaxSeite));
		Width = OriWidth;
		Height = OriHeight;
		//		SetCoords(0,0);
	}
	SpinBox21->setMaximum(Width);
	SpinBox31->setMaximum(Height);
	connect(SpinBox11, SIGNAL(valueChanged(int)), this, SLOT(SetPage(int)));
}

void ScAnnot::SetCross()
{
	int x,y;
	disconnect(Pg1, SIGNAL(Coords(double, double)), this, SLOT(SetCoords(double, double)));
	x = static_cast<int>(static_cast<double>(SpinBox21->value())/static_cast<double>(Width)*Pg1->pmx.width());
	y = static_cast<int>(static_cast<double>(SpinBox31->value())/static_cast<double>(Height)*Pg1->pmx.height());
	Pg1->drawMark(x, y);
	connect(Pg1, SIGNAL(Coords(double, double)), this, SLOT(SetCoords(double, double)));
}

void ScAnnot::SetValues()
{
	bool AAct = false;
	QString tmp, tmp2;
	QString Nfo("");
	
	Annotation& annotation = item->annotation();
	annotation = m_annotation;

	if (ComboBox1->currentIndex() == 5)
		annotation.setType(Annotation::RadioButton);
	else
		annotation.setType(ComboBox1->currentIndex()+2);
	if (Name->text() != OldName)
		item->setItemName(Name->text());
	annotation.setToolTip(Tip->text());
	annotation.setRollOver(TextO->text());
	annotation.setDown(DownT->text());
	annotation.setBwid(BorderW->currentIndex());
	annotation.setBsty(BorderS->currentIndex());
	annotation.setFeed(ComboBox7_2->currentIndex());
	annotation.setVis(Visib->currentIndex());
	annotation.setFont(Schrift->currentIndex());
	annotation.setFlag(0);
	annotation.setIsChk(isChkd->isChecked());
	annotation.setCheckState(isChkd->isChecked());
	annotation.setChkStil(ChkStil->currentIndex());
	annotation.setBorderColor(BorderC->currentText());
	if (annotation.borderColor() == CommonStrings::tr_NoneColor)
		annotation.setBorderColor(CommonStrings::None);
	Limit->isChecked() ? annotation.setMaxChar(MaxChars->value()) : annotation.setMaxChar(-1);
	if (annotation.Type() == Annotation::Button)
	{
		annotation.addToFlag(Annotation::Flag_PushButton);
		if (item->Pfile.isEmpty())
			annotation.setUseIcons(false);
		annotation.setFormat(0);
		annotation.setF_act(QString());
	}
	else
	{
		annotation.setUseIcons(false);
		if (ReadOnly->isChecked())
			annotation.addToFlag(Annotation::Flag_ReadOnly);
		if (Required->isChecked())
			annotation.addToFlag(Annotation::Flag_Required);
		if (NoExport->isChecked())
			annotation.addToFlag(Annotation::Flag_NoExport);
	}
	if (annotation.Type() == Annotation::RadioButton)
	{
		annotation.addToFlag(Annotation::Flag_Radio | Annotation::Flag_NoToggleToOff);
		annotation.setFormat(0);
		annotation.setF_act(QString());
	}
	if (annotation.Type() == Annotation::Checkbox)
	{
		annotation.setFormat(0);
		annotation.setF_act(QString());
	}
	if (annotation.Type() == Annotation::Combobox)
	{
		annotation.addToFlag(Annotation::Flag_Combo);
		if (CanEdit->isChecked())
			annotation.addToFlag(Annotation::Flag_Edit);
	}
	if (annotation.Type() == Annotation::Listbox)
	{
		annotation.setFormat(0);
		annotation.setF_act(QString());
	}
	if (annotation.Type() == Annotation::Textfield)
	{
		if (MultiL->isChecked())
			annotation.addToFlag(Annotation::Flag_Multiline);
		if (Passwd->isChecked())
			annotation.addToFlag(Annotation::Flag_Password);
		if (NoSpell->isChecked())
			annotation.addToFlag(Annotation::Flag_DoNotSpellCheck);
		if (NoScroll->isChecked())
			annotation.addToFlag(Annotation::Flag_DoNotScroll);
	}
	if ((annotation.Type() == Annotation::Textfield) || (annotation.Type() == Annotation::Combobox))
	{
		if (NoValid->isChecked())
			annotation.setV_act("");
		if (SimpleValid->isChecked())
			annotation.setV_act("AFRange_Validate(true, "+MinValid->text()+", true, "+MaxValid->text()+")");
		if (CustomValid->isChecked())
			annotation.setV_act(ValidScript->toPlainText());
		if (NoCalc->isChecked())
			annotation.setC_act("");
		if (SimpleCalc->isChecked())
		{
			QString tmpCact = "AFSimple_Calculate(";
			switch (CalcArt->currentIndex())
			{
				case 0:
					tmpCact += "\"SUM\", ";
					break;
				case 1:
					tmpCact += "\"PRD\", ";
					break;
				case 2:
					tmpCact += "\"AVG\", ";
					break;
				case 3:
					tmpCact += "\"MIN\", ";
					break;
				case 4:
					tmpCact += "\"MAX\", ";
					break;
			}
			tmpCact += "new Array (";
			QStringList pfol;
			pfol = CalcFields->text().split(",", QString::SkipEmptyParts);
			if (pfol.count() > 1)
			{
				tmpCact += "\""+pfol[0].simplified()+"\"";
				for (int cfx = 1; cfx < pfol.count(); cfx++)
				{
					tmpCact += ", \""+pfol[cfx].simplified()+"\"";
				}
			}
			tmpCact += "))";
			annotation.setC_act(tmpCact);
		}
		if (CustomCalc->isChecked())
			annotation.setC_act(CalcScript->toPlainText());
		switch (TxFormat->currentIndex())
		{
			case 0:
				annotation.setF_act("");
				annotation.setK_act("");
				break;
			case 1:
				Nfo = tmp.setNum(Decim->value())+", "+tmp2.setNum(FormNum)+", 0, 0, \"";
				if (UseCurr->isChecked())
				{
					if (!PreCurr->isChecked())
						Nfo += " ";
					Nfo += CurSym->text().simplified();
					if (PreCurr->isChecked())
						Nfo += " ";
				}
				if (PreCurr->isChecked())
					Nfo += "\", true)";
				else
					Nfo += "\", false)";
				annotation.setF_act("AFNumber_Format("+Nfo);
				annotation.setK_act("AFNumber_Keystroke("+Nfo);
				break;
			case 2:
				Nfo = tmp.setNum(Decim2->value())+", "+tmp.setNum(FormNum)+")";
				annotation.setF_act("AFPercent_Format("+Nfo);
				annotation.setK_act("AFPercent_Keystroke("+Nfo);
				break;
			case 3:
				Nfo = Format0c->currentText()+"\")";
				annotation.setF_act("AFDate_FormatEx(\""+Nfo);
				annotation.setK_act("AFDate_KeystrokeEx(\""+Nfo);
				break;
			case 4:
				Nfo = tmp.setNum(FormNum)+")";
				annotation.setF_act("AFTime_Format("+Nfo);
				annotation.setK_act("AFTime_Keystroke("+Nfo);
				break;
			case 5:
				annotation.setF_act(FormatScript->toPlainText());
				annotation.setK_act(KeyScript->toPlainText());
				break;
		}
	}
	switch (ActionCombo->currentIndex())
	{
		case 0:
			annotation.setActionType(Annotation::Action_None);
			annotation.setAction(QString());
			break;
		case 1:
			annotation.setActionType(Annotation::Action_JavaScript);
			switch (ScrEdited)
			{
				case Annotation::Java_ReleaseButton:
					annotation.setAction(EditJava->toPlainText());
					break;
				case Annotation::Java_PressButton:
					annotation.setD_act(EditJava->toPlainText());
					break;
				case Annotation::Java_EnterWidget:
					annotation.setE_act(EditJava->toPlainText());
					break;
				case Annotation::Java_LeaveWidget:
					annotation.setX_act(EditJava->toPlainText());
					break;
				case Annotation::Java_FocusIn:
					annotation.setFo_act(EditJava->toPlainText());
					break;
				case Annotation::Java_FocusOut:
					annotation.setBl_act(EditJava->toPlainText());
					break;
				case Annotation::Java_SelectionChg:
					annotation.setK_act(EditJava->toPlainText());
					break;
			}
			break;
		case 2:
			if ((LExtern->isChecked()) && (!Destfile->text().isEmpty()))
			{
				annotation.setExtern(Destfile->text());
				if (useAbsolute->isChecked())
					annotation.setActionType(Annotation::Action_GoToR_FileAbs);
				else
					annotation.setActionType(Annotation::Action_GoToR_FileRel);
			}
			else
			{
				annotation.setExtern("");
				annotation.setActionType(Annotation::Action_GoTo);
			}
			annotation.setZiel(SpinBox11->value()-1);
			annotation.setAction(tmp.setNum(SpinBox21->value())+" "+tmp2.setNum(Height-SpinBox31->value())+" 0");
			break;
		case 3:
			annotation.setActionType(Annotation::Action_SubmitForm);
			annotation.setAction(SubURL->text().simplified());
			annotation.setHTML(SelAsHtml->currentIndex());
			break;
		case 4:
			annotation.setActionType(Annotation::Action_ResetForm);
			annotation.setAction(QString());
			break;
		case 5:
			annotation.setActionType(Annotation::Action_ImportData);
			annotation.setAction(SubURLa->text().simplified());
			break;
		case 6:
			annotation.setActionType(Annotation::Action_Named);
			annotation.setAction(nameActionCombo->itemData(nameActionCombo->currentIndex()).toString());
			break;
		default:
			annotation.setActionType(Annotation::Action_None);
			annotation.setAction(QString());
			break;
	}
	if (!annotation.E_act().isEmpty())
		AAct = true;
	if (!annotation.X_act().isEmpty())
		AAct = true;
	if (!annotation.D_act().isEmpty())
		AAct = true;
	if (!annotation.Fo_act().isEmpty())
		AAct = true;
	if (!annotation.Bl_act().isEmpty())
		AAct = true;
	if (!annotation.K_act().isEmpty())
		AAct = true;
	if (!annotation.F_act().isEmpty())
		AAct = true;
	if (!annotation.V_act().isEmpty())
		AAct = true;
	if (!annotation.C_act().isEmpty())
		AAct = true;
	annotation.setAAact(AAct);
	accept();
}

void ScAnnot::SetAnnotationType(int it)
{
	disconnect(ActionCombo, SIGNAL(activated(int)), this, SLOT(SetActionType(int)));
	disconnect(TxFormat, SIGNAL(activated(int)), this, SLOT(SetFoScript(int)));

	Annotation& annotation = m_annotation;
	int tmpac = annotation.ActionType();
	if ((tmpac == 7) || (tmpac == 9))
		tmpac = 2;
	else if (tmpac == 10)
		tmpac = 6;
	int sela = it + 2;
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tabFormat), false);
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tabValidate), false);
	TabWidget2->setTabEnabled(TabWidget2->indexOf(tabCalculate), false);
	EditFormat->setEnabled( false );
	EditKeystr->setEnabled( false );
	CText1->show();
	ChkStil->show();
	SelAction->clear();
	QString tmp_selact[]={tr("Mouse Up"), tr("Mouse Down"), tr("Mouse Enter"),
	                      tr("Mouse Exit"), tr("On Focus"), tr("On Blur")};
	size_t array_sel = sizeof(tmp_selact) / sizeof(*tmp_selact);
	/* PFJ - 28/02/04 - Altered from uint to int and varname */
	for (uint prop = 0; prop < array_sel; ++prop)
		SelAction->addItem(tmp_selact[prop]);
	bool setter;
	switch (sela)
	{
	case 2:
		{
			Fram->setCurrentIndex(3);
			FramOp->setCurrentIndex(sela-2);
			ReadOnly->setEnabled(false);
			ReadOnly->setChecked(false);
			Required->setEnabled(false);
			Required->setChecked(false);
			NoExport->setEnabled(false);
			NoExport->setChecked(false);
			ActionCombo->clear();
			QString tmp_actcom[] = {tr("None", "action"), tr("JavaScript"), tr("Go To"),
									tr("Submit Form"), tr("Reset Form"), tr("Import Data"), tr("Named")};
			size_t array_act = sizeof(tmp_actcom) / sizeof(*tmp_actcom);
			/* PFJ - 28/02/04 - Altered from uint to int and varname */
			for (uint prop = 0; prop < array_act; ++prop)
				ActionCombo->addItem(tmp_actcom[prop]);
			ActionCombo->setCurrentIndex(qMin(tmpac,6));
			setter = ((annotation.ActionType() != Annotation::Action_GoToR_FileRel) && (annotation.ActionType() != Annotation::Action_GoToR_FileAbs)) ? true : false;
			Destfile->setEnabled(setter);
			ChFile->setEnabled(setter);
			SetActionType(tmpac);
			break;
		}
	case 3:
	case 6:
	case 4:
	case 5:
	case 7:
		if ((sela == 3) || (sela == 5))
		{
			TabWidget2->setTabEnabled(TabWidget2->indexOf(tabFormat), true);
			TabWidget2->setTabEnabled(TabWidget2->indexOf(tabValidate), true);
			TabWidget2->setTabEnabled(TabWidget2->indexOf(tabCalculate), true);
			TxFormat->setCurrentIndex(annotation.Format());
			SetFoScript(annotation.Format());
			SetVali();
			SetCalc();
		}
		if (sela == 6)
			SelAction->addItem( tr( "Selection Change" ) );
		ReadOnly->setEnabled(true);
		ReadOnly->setChecked(annotation.Flag() & Annotation::Flag_ReadOnly);
		Required->setChecked(annotation.Flag() & Annotation::Flag_Required);
		NoExport->setChecked(annotation.Flag() & Annotation::Flag_NoExport);
		Fram->setCurrentIndex(3);
		if (sela < 6)
			FramOp->setCurrentIndex(sela-2);
		else if (sela == 6)
			FramOp->setCurrentIndex(3);
		else
		{
			CText1->hide();
			ChkStil->hide();
			FramOp->setCurrentIndex(2);
		}
		ActionCombo->clear();
		ActionCombo->addItem( tr( "None" ) );
		ActionCombo->addItem( tr( "JavaScript" ) );
		ActionCombo->setCurrentIndex(qMin(tmpac, 1));
		SetActionType(tmpac);
		break;
	default:
		Fram->setCurrentIndex(2);
		break;
	}
	MultiL->setChecked(annotation.Flag() & Annotation::Flag_Multiline);
	Passwd->setChecked(annotation.Flag() & Annotation::Flag_Password);
	CanEdit->setChecked(annotation.Flag() & Annotation::Flag_Edit);
	NoSpell->setChecked(annotation.Flag() & Annotation::Flag_DoNotSpellCheck);
	NoScroll->setChecked(annotation.Flag() & Annotation::Flag_DoNotScroll);
	ChkStil->setCurrentIndex(annotation.ChkStil());
	isChkd->setChecked(annotation.IsChk());
	setter = annotation.MaxChar() != -1 ? true : false;
	MaxChars->setValue(setter == true ? annotation.MaxChar() : 0);
	Limit->setChecked(setter);
	MaxChars->setEnabled(setter);
	connect(ActionCombo, SIGNAL(activated(int)), this, SLOT(SetActionType(int)));
	connect(TxFormat, SIGNAL(activated(int)), this, SLOT(SetFoScript(int)));
}

void ScAnnot::SetLimit()
{
	Limit->isChecked() ? MaxChars->setEnabled(true) :MaxChars->setEnabled(false);
}

void ScAnnot::SetExternLink()
{
	disconnect(LExtern, SIGNAL(clicked()), this, SLOT(SetExternLink()));
	bool enable;
	if (!LExtern->isChecked())
	{
		m_annotation.setActionType(Annotation::Action_GoTo);
		enable = false;
		//		Destfile->setEnabled(false);
		//		ChFile->setEnabled(false);
		SetPage(qMin(SpinBox11->value(), MaxSeite));
	}
	else
	{
		if (useAbsolute->isChecked())
			m_annotation.setActionType(Annotation::Action_GoToR_FileAbs);
		else
			m_annotation.setActionType(Annotation::Action_GoToR_FileRel);
		enable = true;
		//		Destfile->setEnabled(true);
		//		ChFile->setEnabled(true);
		if (Destfile->text().isEmpty())
		{
			GetFile();
			if (Destfile->text().isEmpty())
			{
				m_annotation.setActionType(Annotation::Action_GoTo);
				enable = false;
				//				Destfile->setEnabled(false);
				//				ChFile->setEnabled(false);
				LExtern->setChecked(false);
			}
		}
		SetPage(qMin(SpinBox11->value(), MaxSeite));
	}
	Destfile->setEnabled(enable);
	ChFile->setEnabled(enable);
	connect(LExtern, SIGNAL(clicked()), this, SLOT(SetExternLink()));
}

void ScAnnot::SetActionType(int it)
{
	bool setter;
	switch (it)
	{
	case 6:
	{
		Fram2->setCurrentIndex(5);
		int nac = nameActionCombo->findData(m_annotation.Action());
		if (nac >= 0)
			nameActionCombo->setCurrentIndex(nac);
		break;
	}
	case 5:
		Fram2->setCurrentIndex(4);
		SubURLa->setText(m_annotation.Action());
		break;
	case 3:
		Fram2->setCurrentIndex(3);
		SubURL->setText(m_annotation.Action());
		SelAsHtml->setCurrentIndex(m_annotation.HTML());
		break;
	case 2:
		Fram2->setCurrentIndex(2);
		setter = (m_annotation.ActionType() != Annotation::Action_GoToR_FileRel);
		Destfile->setEnabled(setter);
		ChFile->setEnabled(setter);
		SetPage(qMin(SpinBox11->value(), MaxSeite));
		break;
	case 1:
		Fram2->setCurrentIndex(1);
		SelAction->setCurrentIndex(ScrEdited);
		break;
	default:
		Fram2->setCurrentIndex(0);
		break;
	}
}

void ScAnnot::SetActionScript(int it)
{
	Annotation& annotation = m_annotation;
	switch (ScrEdited)
	{
	case Annotation::Java_ReleaseButton:
		annotation.setAction(EditJava->toPlainText());
		break;
	case Annotation::Java_PressButton:
		annotation.setD_act(EditJava->toPlainText());
		break;
	case Annotation::Java_EnterWidget:
		annotation.setE_act(EditJava->toPlainText());
		break;
	case Annotation::Java_LeaveWidget:
		annotation.setX_act(EditJava->toPlainText());
		break;
	case Annotation::Java_FocusIn:
		annotation.setFo_act(EditJava->toPlainText());
		break;
	case Annotation::Java_FocusOut:
		annotation.setBl_act(EditJava->toPlainText());
		break;
	case Annotation::Java_SelectionChg:
		annotation.setK_act(EditJava->toPlainText());
		break;
	}
	switch (it)
	{
	case Annotation::Java_ReleaseButton:
		EditJava->setPlainText(annotation.Action());
		break;
	case Annotation::Java_PressButton:
		EditJava->setPlainText(annotation.D_act());
		break;
	case Annotation::Java_EnterWidget:
		EditJava->setPlainText(annotation.E_act());
		break;
	case Annotation::Java_LeaveWidget:
		EditJava->setPlainText(annotation.X_act());
		break;
	case Annotation::Java_FocusIn:
		EditJava->setPlainText(annotation.Fo_act());
		break;
	case Annotation::Java_FocusOut:
		EditJava->setPlainText(annotation.Bl_act());
		break;
	case Annotation::Java_SelectionChg:
		EditJava->setPlainText(annotation.K_act());
		break;
	}
	ScrEdited = it;
}

void ScAnnot::GetFile()
{
	QString fn;
	QString wdir = dirs->get("annot_getfile", ".");
	CustomFDialog dia(this, wdir, tr("Open"), tr("PDF Files (*.pdf);;All Files (*)"));
	if (!Destfile->text().isEmpty())
		dia.setSelection(Destfile->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		if (!fn.isEmpty())
		{
			dirs->set("annot_getfile", fn.left(fn.lastIndexOf("/")));
			Destfile->setText(fn);
			SpinBox11->setValue(1);
			SpinBox11->setMaximum(1000);
			SetPage(1);
		}
	}
}
