#include "search.h"
#include "search.moc"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>
#include "mspinbox.h"
#include "scribusdoc.h"
#include "fontcombo.h"
#include "page.h"
extern QPixmap loadIcon(QString nam);


SearchReplace::SearchReplace( QWidget* parent, ScribusDoc *doc, preV *Prefs, PageItem* ite )
							: QDialog( parent, "SearchReplace", true, 0 )

{
    setCaption( tr( "Search/Replace" ) );
	setIcon(loadIcon("AppIcon.png"));
	QPixmap pm;
	pm = QPixmap(30, 15);
	CListe::Iterator it;
	Item = ite;
	Doc = doc;
	NotFound = false;
    SearchReplaceLayout = new QVBoxLayout( this, 10, 5, "SearchReplaceLayout"); 

    SelLayout = new QHBoxLayout( 0, 0, 6, "SelLayout"); 

    Search = new QGroupBox( this, "Search" );
    Search->setTitle( tr( "Search for:" ) );
    Search->setColumnLayout(0, Qt::Vertical );
    Search->layout()->setSpacing( 2 );
    Search->layout()->setMargin( 5 );
    SearchLayout = new QGridLayout( Search->layout() );
    SearchLayout->setAlignment( Qt::AlignTop );

    SText = new QCheckBox( Search, "SText" );
    SText->setText( tr( "Text" ) );
    SearchLayout->addWidget( SText, 0, 0 );

    SStyle = new QCheckBox( Search, "SStyle" );
    SStyle->setText( tr( "Paragraph Style" ) );
    SearchLayout->addWidget( SStyle, 1, 0 );

    SFont = new QCheckBox( Search, "SFont" );
    SFont->setText( tr( "Font" ) );
    SearchLayout->addWidget( SFont, 2, 0 );

    SSize = new QCheckBox( Search, "SSize" );
    SSize->setText( tr( "Font Size" ) );
    SearchLayout->addWidget( SSize, 3, 0 );

    SFill = new QCheckBox( Search, "SFill" );
    SFill->setText( tr( "Fill Color" ) );
    SearchLayout->addWidget( SFill, 4, 0 );

    SStroke = new QCheckBox( Search, "SStroke" );
    SStroke->setText( tr( "Stroke Color" ) );
    SearchLayout->addWidget( SStroke, 5, 0 );

    STextVal = new QLineEdit( Search, "STextVal" );
	STextVal->setEnabled(false);
    SearchLayout->addWidget( STextVal, 0, 1 );

    SStyleVal = new QComboBox( true, Search, "SStyleVal" );
	SStyleVal->setEditable(false);
	SStyleVal->insertItem( tr("Left"));
	SStyleVal->insertItem( tr("Center"));
	SStyleVal->insertItem( tr("Right"));
	SStyleVal->insertItem( tr("Block"));
	SStyleVal->insertItem( tr("Forced"));
	if (doc->Vorlagen.count() > 5)
		{
		for (uint x = 5; x < doc->Vorlagen.count(); ++x)
			{
			SStyleVal->insertItem(doc->Vorlagen[x].Vname);
			}
		}
	SStyleVal->setCurrentItem(doc->CurrentABStil);
	SStyleVal->setEnabled(false);
    SearchLayout->addWidget( SStyleVal, 1, 1 );

    SFontVal = new FontCombo(Search, Prefs);
	SFontVal->setMaximumSize(190, 30);
	SFontVal->setCurrentText(doc->CurrFont);
	SFontVal->setEnabled(false);
    SearchLayout->addWidget( SFontVal, 2, 1 );

    SSizeVal = new MSpinBox( Search, 1 );
    SSizeVal->setSuffix( tr( " pt" ) );
    SSizeVal->setMinValue( 1 );
    SSizeVal->setMaxValue( 1024 );
    SSizeVal->setValue( doc->CurrFontSize / 10.0 );
	SSizeVal->setEnabled(false);
    SearchLayout->addWidget( SSizeVal, 3, 1 );

    SFillVal = new QComboBox( true, Search, "SFillVal" );
	SFillVal->setEditable(false);
	SFillVal->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		SFillVal->insertItem(pm, it.key());
		}
	SFillVal->setCurrentText(doc->CurrTextFill);
	SFillVal->setEnabled(false);
    SearchLayout->addWidget( SFillVal, 4, 1 );

    SStrokeVal = new QComboBox( true, Search, "SStrokeVal" );
	SStrokeVal->setEditable(false);
	SStrokeVal->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		SStrokeVal->insertItem(pm, it.key());
		}
	SStrokeVal->setCurrentText(doc->CurrTextStroke);
	SStrokeVal->setEnabled(false);
    SearchLayout->addWidget( SStrokeVal, 5, 1 );

    SelLayout->addWidget( Search );

    Replace = new QGroupBox( this, "Replace" );
    Replace->setTitle( tr( "Replace with:" ) );
    Replace->setColumnLayout(0, Qt::Vertical );
    Replace->layout()->setSpacing( 2 );
    Replace->layout()->setMargin( 5 );
    ReplaceLayout = new QGridLayout( Replace->layout() );
    ReplaceLayout->setAlignment( Qt::AlignTop );

    RText = new QCheckBox( Replace, "RText" );
    RText->setText( tr( "Text" ) );
    ReplaceLayout->addWidget( RText, 0, 0 );

    RStyle = new QCheckBox( Replace, "RStyle" );
    RStyle->setText( tr( "Paragraph Style" ) );
    ReplaceLayout->addWidget( RStyle, 1, 0 );

    RFont = new QCheckBox( Replace, "RFont" );
    RFont->setText( tr( "Font" ) );
    ReplaceLayout->addWidget( RFont, 2, 0 );

    RSize = new QCheckBox( Replace, "RSize" );
    RSize->setText( tr( "Font Size" ) );
    ReplaceLayout->addWidget( RSize, 3, 0 );

    RFill = new QCheckBox( Replace, "RFill" );
    RFill->setText( tr( "Fill Color" ) );
    ReplaceLayout->addWidget( RFill, 4, 0 );

    RStroke = new QCheckBox( Replace, "RStroke" );
    RStroke->setText( tr( "Stroke Color" ) );
    ReplaceLayout->addWidget( RStroke, 5, 0 );

    RTextVal = new QLineEdit( Replace, "RTextVal" );
	RTextVal->setEnabled(false);
    ReplaceLayout->addWidget( RTextVal, 0, 1 );

    RStyleVal = new QComboBox( true, Replace, "RStyleVal" );
	RStyleVal->setEditable(false);
	RStyleVal->insertItem( tr("Left"));
	RStyleVal->insertItem( tr("Center"));
	RStyleVal->insertItem( tr("Right"));
	RStyleVal->insertItem( tr("Block"));
	RStyleVal->insertItem( tr("Forced"));
	if (doc->Vorlagen.count() > 5)
		{
		for (uint x = 5; x < doc->Vorlagen.count(); ++x)
			{
			RStyleVal->insertItem(doc->Vorlagen[x].Vname);
			}
		}
	RStyleVal->setCurrentItem(doc->CurrentABStil);
	RStyleVal->setEnabled(false);
    ReplaceLayout->addWidget( RStyleVal, 1, 1 );

    RFontVal = new FontCombo(Replace, Prefs);
	RFontVal->setMaximumSize(190, 30);
	RFontVal->setCurrentText(doc->CurrFont);
	RFontVal->setEnabled(false);
    ReplaceLayout->addWidget( RFontVal, 2, 1 );

    RSizeVal = new MSpinBox( Replace, 1 );
    RSizeVal->setSuffix( tr( " pt" ) );
    RSizeVal->setMinValue( 1 );
    RSizeVal->setMaxValue( 1024 );
    RSizeVal->setValue( doc->CurrFontSize / 10.0 );
	RSizeVal->setEnabled(false);
    ReplaceLayout->addWidget( RSizeVal, 3, 1 );

    RFillVal = new QComboBox( true, Replace, "RFillVal" );
	RFillVal->setEditable(false);
	RFillVal->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		RFillVal->insertItem(pm, it.key());
		}
	RFillVal->setCurrentText(doc->CurrTextFill);
	RFillVal->setEnabled(false);
    ReplaceLayout->addWidget( RFillVal, 4, 1 );

    RStrokeVal = new QComboBox( true, Replace, "RStrokeVal" );
	RStrokeVal->setEditable(false);
	RStrokeVal->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		RStrokeVal->insertItem(pm, it.key());
		}
	RStrokeVal->setCurrentText(doc->CurrTextStroke);
	RStrokeVal->setEnabled(false);
    ReplaceLayout->addWidget( RStrokeVal, 5, 1 );

    SelLayout->addWidget( Replace );
    SearchReplaceLayout->addLayout( SelLayout );

    OptsLayout = new QHBoxLayout( 0, 0, 6, "OptsLayout"); 

    Word = new QCheckBox( this, "Word" );
	Word->setEnabled(false);
    Word->setText( tr( "Whole Word" ) );
    OptsLayout->addWidget( Word );

    CaseIgnore = new QCheckBox( this, "CaseIgnore" );
    CaseIgnore->setText( tr( "Ignore Case" ) );
	CaseIgnore->setEnabled(false);
    OptsLayout->addWidget( CaseIgnore );

    SearchReplaceLayout->addLayout( OptsLayout );

    ButtonsLayout = new QHBoxLayout( 0, 0, 4, "ButtonsLayout"); 

    DoSearch = new QPushButton( this, "DoSearch" );
    DoSearch->setText( tr( "Search" ) );
    DoSearch->setDefault( TRUE );
    ButtonsLayout->addWidget( DoSearch );

    DoReplace = new QPushButton( this, "DoReplace" );
    DoReplace->setText( tr( "Replace" ) );
	DoReplace->setEnabled(false);
    ButtonsLayout->addWidget( DoReplace );

    AllReplace = new QPushButton( this, "DoReplace" );
    AllReplace->setText( tr( "Replace All" ) );
	AllReplace->setEnabled(false);
    ButtonsLayout->addWidget( AllReplace );

    Leave = new QPushButton( this, "Leave" );
    Leave->setText( tr( "Leave" ) );
    ButtonsLayout->addWidget( Leave );
    SearchReplaceLayout->addLayout( ButtonsLayout );
//    resize( QSize(570, 311).expandedTo(minimumSizeHint()) );
    resize(minimumSizeHint());

    // signals and slots connections
    connect( Leave, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( DoSearch, SIGNAL( clicked() ), this, SLOT( slotSearch() ) );
    connect( DoReplace, SIGNAL( clicked() ), this, SLOT( slotReplace() ) );
    connect( AllReplace, SIGNAL( clicked() ), this, SLOT( slotReplaceAll() ) );
    connect( SText, SIGNAL( clicked() ), this, SLOT( enableTxSearch() ) );
    connect( SStyle, SIGNAL( clicked() ), this, SLOT( enableStyleSearch() ) );
    connect( SFont, SIGNAL( clicked() ), this, SLOT( enableFontSearch() ) );
    connect( SSize, SIGNAL( clicked() ), this, SLOT( enableSizeSearch() ) );
    connect( SFill, SIGNAL( clicked() ), this, SLOT( enableFillSearch() ) );
    connect( SStroke, SIGNAL( clicked() ), this, SLOT( enableStrokeSearch() ) );
    connect( RText, SIGNAL( clicked() ), this, SLOT( enableTxReplace() ) );
    connect( RStyle, SIGNAL( clicked() ), this, SLOT( enableStyleReplace() ) );
    connect( RFont, SIGNAL( clicked() ), this, SLOT( enableFontReplace() ) );
    connect( RSize, SIGNAL( clicked() ), this, SLOT( enableSizeReplace() ) );
    connect( RFill, SIGNAL( clicked() ), this, SLOT( enableFillReplace() ) );
    connect( RStroke, SIGNAL( clicked() ), this, SLOT( enableStrokeReplace() ) );

    // tab order
    setTabOrder( SText, SStyle );
    setTabOrder( SStyle, SFont );
    setTabOrder( SFont, SSize );
    setTabOrder( SSize, SFill );
    setTabOrder( SFill, SStroke );
    setTabOrder( SStroke, STextVal );
    setTabOrder( STextVal, SStyleVal );
    setTabOrder( SStyleVal, SFontVal );
    setTabOrder( SFontVal, SSizeVal );
    setTabOrder( SSizeVal, SFillVal );
    setTabOrder( SFillVal, SStrokeVal );
    setTabOrder( SStrokeVal, RText );
    setTabOrder( RText, RStyle );
    setTabOrder( RStyle, RFont );
    setTabOrder( RFont, RSize );
    setTabOrder( RSize, RFill );
    setTabOrder( RFill, RStroke );
    setTabOrder( RStroke, RTextVal );
    setTabOrder( RTextVal, RStyleVal );
    setTabOrder( RStyleVal, RFontVal );
    setTabOrder( RFontVal, RSizeVal );
    setTabOrder( RSizeVal, RFillVal );
    setTabOrder( RFillVal, RStrokeVal );
    setTabOrder( RStrokeVal, Word );
    setTabOrder( Word, CaseIgnore );
    setTabOrder( CaseIgnore, DoSearch );
    setTabOrder( DoSearch, DoReplace );
    setTabOrder( DoReplace, AllReplace );
    setTabOrder( AllReplace, Leave );
}

void SearchReplace::slotSearch()
{
	Doc->ActPage->slotDoCurs(false);
	DoReplace->setEnabled(false);
	AllReplace->setEnabled(false);
	for (uint a = 0; a < Item->MaxChars; ++a)
		{
		Item->Ptext.at(a)->cselect = false;
		}
	Item->HasSel = false;
	QString fCol = "";
	QString sCol = "";
	QString sFont = "";
	QString sText = "";
	NotFound = true;
	int sStyle = 0;
	int sSize = 0;
	bool rep = false;
	bool found = true;
	if ((RFill->isChecked()) || (RStroke->isChecked())  || (RStyle->isChecked())  || (RFont->isChecked())
		|| (RSize->isChecked())  || (RText->isChecked()))
		rep = true;
	if (SText->isChecked())
		sText = STextVal->text();
	if (CaseIgnore->isChecked())
		sText = sText.lower();
	if (SFill->isChecked())
		fCol = SFillVal->currentText();
	if (SStroke->isChecked())
		sCol = SStrokeVal->currentText();
	if (SFont->isChecked())
		sFont = SFontVal->currentText();
	if (SStyle->isChecked())
		sStyle = SStyleVal->currentItem();
	if (SSize->isChecked())
		sSize = qRound(SSizeVal->value() * 10);
	if (sText.length() > 0)
		found = false;
	uint inde = 0;
	uint as = Item->CPos;
	ReplStart = as;
	for (uint a = as; a < Item->MaxChars; ++a)
		{
		if (SText->isChecked())
			{
			QString chx = Item->Ptext.at(a)->ch;
			if (CaseIgnore->isChecked())
				chx = chx.lower();
			if (chx == sText.mid(inde,1))
				found = true;
			else
				found = false;
			if ((Word->isChecked()) && (inde == 0) && (chx[0].isSpace()))
				found = true;
			}
		else
			found = true;
		if (SSize->isChecked())
			{
			if (Item->Ptext.at(a)->csize != sSize)
				found = false;
			}
		if (SFont->isChecked())
			{
			if (Item->Ptext.at(a)->cfont != sFont)
				found = false;
			}
		if (SStyle->isChecked())
			{
			if (Item->Ptext.at(a)->cab != sStyle)
				found = false;
			}
		if (SStroke->isChecked())
			{
			if (Item->Ptext.at(a)->cstroke != sCol)
				found = false;
			}
		if (SFill->isChecked())
			{			
			if (Item->Ptext.at(a)->ccolor != fCol)
				found = false;
			}
		if (found)
			{
			Item->Ptext.at(a)->cselect = true;
			Item->HasSel = true;
			if (rep)
				{
				DoReplace->setEnabled(true);
				AllReplace->setEnabled(true);
				}
			Item->CPos = a+1;
			if (SText->isChecked())
				{
				if (inde == 0)
					ReplStart = a;
				inde++;
				if ((Word->isChecked()) && (inde == 1) && (Item->Ptext.at(a)->ch[0].isSpace()))
					{
					inde--;
					Item->Ptext.at(a)->cselect = false;
					}
				if ((Word->isChecked()) && (inde == sText.length()) && (!Item->Ptext.at(QMIN(Item->MaxChars-1,a+1))->ch[0].isSpace()))
					{
					for (uint xx = ReplStart; xx < a+1; ++xx)
						{
						Item->Ptext.at(QMIN(xx,Item->MaxChars-1))->cselect = false;
						}
					Item->HasSel = false;
					inde = 0;
					found = false;
					}
				else
					{
					if (inde == sText.length())
						break;
					}
				}
			else
				break;
			}
		else
			{
			if (SText->isChecked())
				{
				for (uint xx = ReplStart; xx < a+1; ++xx)
					{
					Item->Ptext.at(QMIN(xx,Item->MaxChars-1))->cselect = false;
					}
				Item->HasSel = false;
				}
			inde = 0;
			}
		}
	if (!found)
		{
		Doc->DoDrawing = true;
		Doc->ActPage->RefreshItem(Item, true);
		DoReplace->setEnabled(false);
		AllReplace->setEnabled(false);
		QMessageBox::information(this, tr("Search/Replace"), tr("Search finished"), tr("OK"));
		Item->CPos = 0;
		NotFound = false;
		}
	Doc->ActPage->slotDoCurs(true);
	Doc->ActPage->RefreshItem(Item, true);
}

void SearchReplace::slotReplace()
{
	QString repl, sear;
	uint cs, cx;
	struct Pti *hg;
	if (RText->isChecked())
		{
		repl = RTextVal->text();
		sear = STextVal->text();
		if (sear.length() == repl.length())
			{
			for (cs = 0; cs < sear.length(); ++cs)
				{
				Item->Ptext.at(ReplStart+cs)->ch = repl[cs];
				}
			}
		else
			{
			if (sear.length() < repl.length())
				{
				for (cs = 0; cs < sear.length(); ++cs)
					{
					Item->Ptext.at(ReplStart+cs)->ch = repl[cs];
					}
				for (cx = cs; cx < repl.length(); ++cx)
					{
					hg = new Pti;
					hg->ch = repl[cx];
					if (RSize->isChecked())
						hg->csize = qRound(RSizeVal->value() * 10.0);
					else
 						hg->csize = Doc->CurrFontSize;
					if (RFill->isChecked())
						hg->ccolor = RFillVal->currentText();
					else
						hg->ccolor = Doc->CurrTextFill;
					hg->cshade = Doc->CurrTextFillSh;
					if (RStroke->isChecked())
						hg->cstroke = RStrokeVal->currentText();
					else
						hg->cstroke = Doc->CurrTextStroke;
					hg->cshade2 = Doc->CurrTextStrokeSh;
					hg->cscale = Doc->CurrTextScale;
 					hg->cselect = true;
 					hg->cstyle = Doc->CurrentStyle;
					if (RStyle->isChecked())
						hg->cab = RStyleVal->currentItem();
					else
 						hg->cab = Doc->CurrentABStil;
					if (Doc->Vorlagen[hg->cab].Font != "")
						{
						hg->cfont = Doc->Vorlagen[hg->cab].Font;
						hg->csize = Doc->Vorlagen[hg->cab].FontSize;
						}
					if (RFont->isChecked())
						hg->cfont = RFontVal->currentText();
					else
 						hg->cfont = Doc->CurrFont;
 					hg->cextra = 0;
 					hg->xp = 0;
 					hg->yp = 0;
					hg->PRot = 0;
					hg->PtransX = 0;
					hg->PtransY = 0;
 					Item->Ptext.insert(ReplStart+cx, hg);     
					}
				Item->CPos = ReplStart+cx;
				}
			else
				{
				for (cs = 0; cs < repl.length(); ++cs)
					{
					Item->Ptext.at(ReplStart+cs)->ch = repl[cs];
					}
				for (uint cxx = cs; cxx < sear.length(); ++cxx)
					{
 					Item->Ptext.remove(ReplStart+cs);
					}
				Item->CPos = ReplStart+cs;
				}
			}
		}
	if (RStyle->isChecked())
		emit NewAbs(RStyleVal->currentItem());
	if (RFill->isChecked())
		Doc->ActPage->ItemTextBrush(RFillVal->currentText());
	if (RStroke->isChecked())
		Doc->ActPage->ItemTextPen(RStrokeVal->currentText());
	if (RFont->isChecked())
		emit NewFont(RFontVal->currentText());
	if (RSize->isChecked())
		Doc->ActPage->chFSize(qRound(RSizeVal->value() * 10.0));
	DoReplace->setEnabled(false);
	AllReplace->setEnabled(false);
	for (uint a = 0; a < Item->Ptext.count(); ++a)
		{
		Item->Ptext.at(a)->cselect = false;
		}
	Doc->ActPage->slotDoCurs(true);
	Doc->ActPage->RefreshItem(Item, true);
}

void SearchReplace::slotReplaceAll()
{
	Doc->DoDrawing = false;
	do
		{
		slotReplace();
		slotSearch();
		}
	while (NotFound);
	Doc->DoDrawing = true;
}

void SearchReplace::enableTxSearch()
{
	bool setter = SText->isChecked();
	STextVal->setEnabled(setter);
	Word->setEnabled(setter);
	CaseIgnore->setEnabled(setter);
}

void SearchReplace::enableStyleSearch()
{
	SStyleVal->setEnabled(SStyle->isChecked());
}

void SearchReplace::enableFontSearch()
{
	SFontVal->setEnabled(SFont->isChecked());
}

void SearchReplace::enableSizeSearch()
{
	SSizeVal->setEnabled(SSize->isChecked());
}

void SearchReplace::enableFillSearch()
{
	SFillVal->setEnabled(SFill->isChecked());
}

void SearchReplace::enableStrokeSearch()
{
	SStrokeVal->setEnabled(SStroke->isChecked());
}

void SearchReplace::enableTxReplace()
{
	RTextVal->setEnabled(RText->isChecked());
}

void SearchReplace::enableStyleReplace()
{
	RStyleVal->setEnabled(RStyle->isChecked());
}

void SearchReplace::enableFontReplace()
{
	RFontVal->setEnabled(RFont->isChecked());
}

void SearchReplace::enableSizeReplace()
{
	RSizeVal->setEnabled(RSize->isChecked());
}

void SearchReplace::enableFillReplace()
{
	RFillVal->setEnabled(RFill->isChecked());
}

void SearchReplace::enableStrokeReplace()
{
	RStrokeVal->setEnabled(RStroke->isChecked());
}

