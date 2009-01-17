/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "search.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QMessageBox>

#include "colorlistbox.h"
#include "commonstrings.h"
#include "colorcombo.h"
#include "fontcombo.h"
#include "scrspinbox.h"
#include "page.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "shadebutton.h"
#include "storyeditor.h"
#include "styleselect.h"
#include "util.h"
#include "util_icon.h"
#include "util_text.h"
#include "text/nlsconfig.h"

SearchReplace::SearchReplace( QWidget* parent, ScribusDoc *doc, PageItem* ite, bool mode )
	: QDialog( parent ),
	matchesFound(0)
{
	setModal(true);
	setWindowTitle( tr( "Search/Replace" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	ColorList::Iterator it;
	Item = ite;
	Doc = doc;
	NotFound = false;
	SMode = mode;
	SearchReplaceLayout = new QVBoxLayout( this );
	SearchReplaceLayout->setMargin(10);
	SearchReplaceLayout->setSpacing(5);
	SelLayout = new QHBoxLayout;
	SelLayout->setMargin(0);
	SelLayout->setSpacing(5);
	Search = new QGroupBox( this );
	Search->setTitle( tr( "Search for:" ) );
	SearchLayout = new QGridLayout( Search );
	SearchLayout->setMargin(5);
	SearchLayout->setSpacing(2);
	SearchLayout->setAlignment( Qt::AlignTop );
	SText = new QCheckBox( Search );
	SText->setText( tr( "Text" ) );
	SearchLayout->addWidget( SText, 0, 0 );
	SStyle = new QCheckBox( Search );
	SStyle->setText( tr( "Alignment" ) );
	SearchLayout->addWidget( SStyle, 1, 0 );
	SFont = new QCheckBox( Search );
	SFont->setText( tr( "Font" ) );
	SearchLayout->addWidget( SFont, 2, 0 );
	SSize = new QCheckBox( Search );
	SSize->setText( tr( "Font Size" ) );
	SearchLayout->addWidget( SSize, 3, 0 );
	SEffect = new QCheckBox( Search );
	SEffect->setText( tr( "Font Effects" ) );
	SearchLayout->addWidget( SEffect );
	SFill = new QCheckBox( Search);
	SFill->setText( tr( "Fill Color" ) );
	SearchLayout->addWidget( SFill );
	SFillS = new QCheckBox( Search );
	SFillS->setText( tr( "Fill Shade" ) );
	SearchLayout->addWidget( SFillS );
	SStroke = new QCheckBox( Search );
	SStroke->setText( tr( "Stroke Color" ) );
	SearchLayout->addWidget( SStroke );
	SStrokeS = new QCheckBox( Search );
	SStrokeS->setText( tr( "Stroke Shade" ) );
	SearchLayout->addWidget( SStrokeS, 8, 0 );
	STextVal = new QLineEdit( Search );
	STextVal->setEnabled(false);
	SearchLayout->addWidget( STextVal, 0, 1 );
	SStyleVal = new QComboBox( Search );
	SStyleVal->setEditable(false);
	QString tmp_sty[] = { tr("Left"), tr("Center"), tr("Right"), tr("Block"), tr("Forced")};
	size_t ar_sty = sizeof(tmp_sty) / sizeof(*tmp_sty);
	for (uint a = 0; a < ar_sty; ++a)
		SStyleVal->addItem(tmp_sty[a]);
//	if (doc->docParagraphStyles.count() >5)
	{
		for (int x = 0; x < doc->paragraphStyles().count(); ++x)
			SStyleVal->addItem(doc->paragraphStyles()[x].name());
	}
	QListView *tmpView = dynamic_cast<QListView*>(SStyleVal->view()); Q_ASSERT(tmpView);
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	SStyleVal->setCurrentIndex(findParagraphStyle(doc, doc->currentStyle));
	SStyleVal->setEnabled(false);
	SearchLayout->addWidget( SStyleVal, 1, 1 );
	SFontVal = new FontCombo(Search);
	SFontVal->setMaximumSize(190, 30);
	setCurrentComboItem(SFontVal, doc->currentStyle.charStyle().font().scName());
	SFontVal->setEnabled(false);
	SearchLayout->addWidget( SFontVal, 2, 1 );
	SSizeVal = new ScrSpinBox( 0.5, 2048, Search, 1 );
	SSizeVal->setValue( doc->currentStyle.charStyle().fontSize() / 10.0 );
	SSizeVal->setEnabled(false);
	SearchLayout->addWidget( SSizeVal, 3, 1 );
	SEffVal = new StyleSelect( Search );
	SEffVal->setStyle(0);
	SEffVal->setEnabled(false);
	SearchLayout->addWidget( SEffVal, 4, 1, Qt::AlignLeft );
	SFillVal = new ColorCombo( Search );
	SFillVal->setEditable(false);
	SFillVal->updateBox(doc->PageColors, ColorCombo::fancyPixmaps, true);
	SFillVal->setMinimumWidth(SFillVal->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(SFillVal, doc->currentStyle.charStyle().fillColor());
	SFillVal->setEnabled(false);
	SearchLayout->addWidget( SFillVal, 5, 1 );
	SFillSVal = new ShadeButton(Search);
	SFillSVal->setEnabled(false);
	SearchLayout->addWidget( SFillSVal, 6, 1, Qt::AlignLeft );
	SStrokeVal = new ColorCombo( Search );
	SStrokeVal->setEditable(false);
	SStrokeVal->updateBox(doc->PageColors, ColorCombo::fancyPixmaps, true);
	SStrokeVal->view()->setMinimumWidth(SStrokeVal->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(SStrokeVal, doc->currentStyle.charStyle().strokeColor());
	SStrokeVal->setEnabled(false);
	SearchLayout->addWidget( SStrokeVal, 7, 1 );
	SStrokeSVal =  new ShadeButton(Search);
	SStrokeSVal->setEnabled(false);
	SearchLayout->addWidget( SStrokeSVal, 8, 1, Qt::AlignLeft );
	SelLayout->addWidget( Search );

	Replace = new QGroupBox( this );
	Replace->setTitle( tr( "Replace with:" ) );
	ReplaceLayout = new QGridLayout( Replace );
	ReplaceLayout->setSpacing( 2 );
	ReplaceLayout->setMargin( 5 );
	ReplaceLayout->setAlignment( Qt::AlignTop );
	RText = new QCheckBox( Replace );
	RText->setText( tr( "Text" ) );
	ReplaceLayout->addWidget( RText, 0, 0 );
	RStyle = new QCheckBox( Replace );
	RStyle->setText( tr( "Alignment" ) );
	ReplaceLayout->addWidget( RStyle, 1, 0 );
	RFont = new QCheckBox( Replace );
	RFont->setText( tr( "Font" ) );
	ReplaceLayout->addWidget( RFont, 2, 0 );
	RSize = new QCheckBox( Replace );
	RSize->setText( tr( "Font Size" ) );
	ReplaceLayout->addWidget( RSize, 3, 0 );
	REffect = new QCheckBox( Replace );
	REffect->setText( tr( "Font Effects" ) );
	ReplaceLayout->addWidget( REffect, 4, 0 );
	RFill = new QCheckBox( Replace );
	RFill->setText( tr( "Fill Color" ) );
	ReplaceLayout->addWidget( RFill, 5, 0 );
	RFillS = new QCheckBox( Replace );
	RFillS->setText( tr( "Fill Shade" ) );
	ReplaceLayout->addWidget( RFillS, 6, 0 );
	RStroke = new QCheckBox( Replace );
	RStroke->setText( tr( "Stroke Color" ) );
	ReplaceLayout->addWidget( RStroke, 7, 0 );
	RStrokeS = new QCheckBox( Replace );
	RStrokeS->setText( tr( "Stroke Shade" ) );
	ReplaceLayout->addWidget( RStrokeS, 8, 0 );
	RTextVal = new QLineEdit( Replace );
	RTextVal->setEnabled(false);
	ReplaceLayout->addWidget( RTextVal, 0, 1 );
	RStyleVal = new QComboBox( Replace );
	RStyleVal->setEditable(false);
	for (uint a = 0; a < ar_sty; ++a)
		RStyleVal->addItem(tmp_sty[a]);
//	if (doc->docParagraphStyles.count() > 5)
	{
		for (int x = 0; x < doc->paragraphStyles().count(); ++x)
			RStyleVal->addItem(doc->paragraphStyles()[x].name());
	}
	tmpView = dynamic_cast<QListView*>(RStyleVal->view()); Q_ASSERT(tmpView);
	tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	RStyleVal->setCurrentIndex(findParagraphStyle(doc, doc->currentStyle));
	RStyleVal->setEnabled(false);
	ReplaceLayout->addWidget( RStyleVal, 1, 1 );
	RFontVal = new FontCombo(Replace);
	RFontVal->setMaximumSize(190, 30);
	setCurrentComboItem(RFontVal, doc->currentStyle.charStyle().font().scName());
	RFontVal->setEnabled(false);
	ReplaceLayout->addWidget( RFontVal, 2, 1 );
	RSizeVal = new ScrSpinBox( 0.5, 2048, Replace, 1 );
	RSizeVal->setValue( doc->currentStyle.charStyle().fontSize() / 10.0 );
	RSizeVal->setEnabled(false);
	ReplaceLayout->addWidget( RSizeVal, 3, 1 );
	REffVal = new StyleSelect( Replace );
	REffVal->setStyle(0);
	REffVal->setEnabled(false);
	ReplaceLayout->addWidget( REffVal, 4, 1, Qt::AlignLeft );
	RFillVal = new ColorCombo( true, Replace );
	RFillVal->setEditable(false);
	RFillVal->updateBox(doc->PageColors, ColorCombo::fancyPixmaps, true);
	RFillVal->view()->setMinimumWidth(RFillVal->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(RFillVal, doc->currentStyle.charStyle().fillColor());
	RFillVal->setEnabled(false);
	ReplaceLayout->addWidget( RFillVal, 5, 1 );
	RFillSVal = new ShadeButton(Replace);
	RFillSVal->setEnabled(false);
	ReplaceLayout->addWidget( RFillSVal, 6, 1, Qt::AlignLeft );
	RStrokeVal = new ColorCombo( true, Replace );
	RStrokeVal->setEditable(false);
	RStrokeVal->updateBox(doc->PageColors, ColorCombo::fancyPixmaps, true);
	RStrokeVal->view()->setMinimumWidth(RStrokeVal->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(RStrokeVal, doc->currentStyle.charStyle().strokeColor());
	RStrokeVal->setEnabled(false);
	ReplaceLayout->addWidget( RStrokeVal, 7, 1 );
	RStrokeSVal = new ShadeButton(Replace);;
	RStrokeSVal->setEnabled(false);
	ReplaceLayout->addWidget( RStrokeSVal, 8, 1, Qt::AlignLeft );
	SelLayout->addWidget( Replace );
	SearchReplaceLayout->addLayout( SelLayout );

	OptsLayout = new QHBoxLayout;
	OptsLayout->setSpacing( 5 );
	OptsLayout->setMargin( 0 );
	Word = new QCheckBox( tr( "&Whole Word" ), this );
	if (mode)
		Word->setEnabled(false);
	OptsLayout->addWidget( Word );
	CaseIgnore = new QCheckBox( tr( "&Ignore Case" ), this );
	if (mode)
		CaseIgnore->setEnabled(false);
	OptsLayout->addWidget( CaseIgnore );
	SearchReplaceLayout->addLayout( OptsLayout );

	ButtonsLayout = new QHBoxLayout;
	ButtonsLayout->setSpacing( 5 );
	ButtonsLayout->setMargin( 0 );
	DoSearch = new QPushButton( tr( "&Search" ), this );
	DoSearch->setDefault( true );
	ButtonsLayout->addWidget( DoSearch );
	DoReplace = new QPushButton( tr( "&Replace" ), this );
	DoReplace->setEnabled(false);
	ButtonsLayout->addWidget( DoReplace );
	AllReplace = new QPushButton( tr( "Replace &All" ), this );
	AllReplace->setEnabled(false);
	ButtonsLayout->addWidget( AllReplace );
	clearButton = new QPushButton( tr("C&lear"), this);
	ButtonsLayout->addWidget(clearButton);
	Leave = new QPushButton( tr( "&Close" ), this );
	ButtonsLayout->addWidget( Leave );
	SearchReplaceLayout->addLayout( ButtonsLayout );

	resize(minimumSizeHint());

 // signals and slots connections
	connect( Leave, SIGNAL( clicked() ), this, SLOT( writePrefs() ) );
	connect( DoSearch, SIGNAL( clicked() ), this, SLOT( slotSearch() ) );
	connect( DoReplace, SIGNAL( clicked() ), this, SLOT( slotReplace() ) );
	connect( AllReplace, SIGNAL( clicked() ), this, SLOT( slotReplaceAll() ) );
	connect( SText, SIGNAL( clicked() ), this, SLOT( enableTxSearch() ) );
	connect( SStyle, SIGNAL( clicked() ), this, SLOT( enableStyleSearch() ) );
	connect( SFont, SIGNAL( clicked() ), this, SLOT( enableFontSearch() ) );
	connect( SSize, SIGNAL( clicked() ), this, SLOT( enableSizeSearch() ) );
	connect( SEffect, SIGNAL( clicked() ), this, SLOT( enableEffSearch() ) );
	connect( SFill, SIGNAL( clicked() ), this, SLOT( enableFillSearch() ) );
	connect( SFillS, SIGNAL( clicked() ), this, SLOT( enableFillSSearch() ) );
	connect( SStrokeS, SIGNAL( clicked() ), this, SLOT( enableStrokeSSearch() ) );
	connect( SStroke, SIGNAL( clicked() ), this, SLOT( enableStrokeSearch() ) );
	connect( RText, SIGNAL( clicked() ), this, SLOT( enableTxReplace() ) );
	connect( RStyle, SIGNAL( clicked() ), this, SLOT( enableStyleReplace() ) );
	connect( RFont, SIGNAL( clicked() ), this, SLOT( enableFontReplace() ) );
	connect( RSize, SIGNAL( clicked() ), this, SLOT( enableSizeReplace() ) );
	connect( REffect, SIGNAL( clicked() ), this, SLOT( enableEffReplace() ) );
	connect( RFill, SIGNAL( clicked() ), this, SLOT( enableFillReplace() ) );
	connect( RStroke, SIGNAL( clicked() ), this, SLOT( enableStrokeReplace() ) );
	connect( RFillS, SIGNAL( clicked() ), this, SLOT( enableFillSReplace() ) );
	connect( RStrokeS, SIGNAL( clicked() ), this, SLOT( enableStrokeSReplace() ) );
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
	
	//tooltips
	DoSearch->setToolTip( tr( "Search for text or formatting in the current text" ) );
	DoReplace->setToolTip( tr( "Replace the searched for formatting with the replacement values" ) );
	AllReplace->setToolTip( tr( "Replace all found instances" ) );
	clearButton->setToolTip( tr( "Clear all search and replace options" ) );
	Leave->setToolTip( tr( "Close search and replace" ) );

 // tab order
	setTabOrder( SText, SStyle );
	setTabOrder( SStyle, SFont );
	setTabOrder( SFont, SSize );
	setTabOrder( SSize, SEffect );
	setTabOrder( SEffect, SFill );
	setTabOrder( SFill, SStroke );
	setTabOrder( SStroke, STextVal );
	setTabOrder( STextVal, SStyleVal );
	setTabOrder( SStyleVal, SFontVal );
	setTabOrder( SFontVal, SSizeVal );
	setTabOrder( SSizeVal, SEffVal );
	setTabOrder( SEffVal, SFillVal );
	setTabOrder( SFillVal, SStrokeVal );
	setTabOrder( SStrokeVal, RText );
	setTabOrder( RText, RStyle );
	setTabOrder( RStyle, RFont );
	setTabOrder( RFont, RSize );
	setTabOrder( RSize, REffect );
	setTabOrder( REffect, RFill );
	setTabOrder( RFill, RStroke );
	setTabOrder( RStroke, RTextVal );
	setTabOrder( RTextVal, RStyleVal );
	setTabOrder( RStyleVal, RFontVal );
	setTabOrder( RFontVal, RSizeVal );
	setTabOrder( RSizeVal, REffVal );
	setTabOrder( REffVal, RFillVal );
	setTabOrder( RFillVal, RStrokeVal );
	setTabOrder( RStrokeVal, Word );
	setTabOrder( Word, CaseIgnore );
	setTabOrder( CaseIgnore, DoSearch );
	setTabOrder( DoSearch, DoReplace );
	setTabOrder( DoReplace, AllReplace );
	setTabOrder( AllReplace, Leave );

	prefs = PrefsManager::instance()->prefsFile->getContext("SearchReplace");
	readPrefs();
}

void SearchReplace::slotSearch()
{
//	if (SMode)
//		Doc->view()->slotDoCurs(false);
	slotDoSearch();
	if (SMode)
	{
//		Doc->view()->slotDoCurs(true);
		Item->update();
	}
}

void SearchReplace::slotDoSearch()
{
	int maxChar = Item->itemText.length() - 1;
	DoReplace->setEnabled(false);
	AllReplace->setEnabled(false);
	if (SMode)
	{
		Item->itemText.deselectAll();
		Item->HasSel = false;
	}
	QString fCol = "";
	QString sCol = "";
	QString sFont = "";
	QString sText = "";
	NotFound = true;
	int sStyle = 0;
	int sSize = 0;
	int sEff = 0;
	int sFillSh = 100;
	int sStrokeSh = 100;
	bool rep = false;
	bool found = true;
	if ((RFill->isChecked()) || (RStroke->isChecked()) || (RStyle->isChecked()) || (RFont->isChecked())
		|| (RStrokeS->isChecked()) || (RFillS->isChecked()) || (RSize->isChecked()) || (RText->isChecked())
		|| (REffect->isChecked()))
		rep = true;
	if (SText->isChecked())
		sText = STextVal->text();
	if (CaseIgnore->isChecked())
		sText = sText.toLower();
	if (SEffect->isChecked())
		sEff = SEffVal->getStyle();
	if (SFill->isChecked())
		fCol = SFillVal->currentText();
	if (SFillS->isChecked())
		sFillSh = SFillSVal->getValue();
	if (SStroke->isChecked())
		sCol = SStrokeVal->currentText();
	if (SStrokeS->isChecked())
		sStrokeSh = SStrokeSVal->getValue();
	if (SFont->isChecked())
		sFont = SFontVal->currentText();
	if (SStyle->isChecked())
		sStyle = SStyleVal->currentIndex();
	if (SSize->isChecked())
		sSize = qRound(SSizeVal->value() * 10);
	if (sText.length() > 0)
		found = false;
	int inde = 0;
	uint as = Item->CPos;
	ReplStart = as;
	int a;
	if (SMode)
	{
		for (a = as; a < Item->itemText.length(); ++a)
		{
			if (SText->isChecked())
			{
				QString chstr = Item->itemText.text(a,1);
				if (CaseIgnore->isChecked())
					chstr = chstr.toLower();
				found = chstr == sText.mid(inde, 1) ? true : false;
				if ((Word->isChecked()) && (inde == 0) && (chstr[0].isSpace()))
					found = true;
			}
			else
				found = true;
			if (SSize->isChecked())
			{
				if (Item->itemText.charStyle(a).fontSize() != sSize)
					found = false;
			}
			if (SFont->isChecked())
			{
				if (Item->itemText.charStyle(a).font().scName() != sFont)
					found = false;
			}
#ifndef NLS_PROTO
			if (SStyle->isChecked())
			{
//				if (Item->itemText.item(a)->cab != sStyle)
//					found = false;
			}
#endif
			if (SStroke->isChecked())
			{
				if (Item->itemText.charStyle(a).strokeColor() != sCol)
					found = false;
			}
			if (SStrokeS->isChecked())
			{
				if (Item->itemText.charStyle(a).strokeShade() != sStrokeSh)
					found = false;
			}
			if (SFillS->isChecked())
			{
				if (Item->itemText.charStyle(a).fillShade() != sFillSh)
					found = false;
			}
			if (SEffect->isChecked())
				{
				if ((Item->itemText.charStyle(a).effects() & 1919) != sEff)
					found = false;
				}
			if (SFill->isChecked())
			{
				if (Item->itemText.charStyle(a).fillColor() != fCol)
					found = false;
			}
			if (found)
			{
				Item->itemText.select(a,1);
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
					if ((Word->isChecked()) && (inde == 1) && (Item->itemText.text(a).isSpace()))
					{
						inde--;
						Item->itemText.select(a, 1, false);
					}
					if ( Word->isChecked()  &&  inde == sText.length()  &&
						! Item->itemText.text(qMin(a+1, maxChar)).isSpace() )
					{
						for (int xx = ReplStart; xx < a+1; ++xx)
							Item->itemText.select(qMin(xx, maxChar), 1, false);
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
					for (int xx = ReplStart; xx < a+1; ++xx)
						Item->itemText.select(qMin(xx, maxChar), 1, false);
					Item->HasSel = false;
				}
				inde = 0;
			}
		}
		if ((!found) || (a == Item->itemText.length()))
		{
			Doc->DoDrawing = true;
			Item->update();
			DoReplace->setEnabled(false);
			AllReplace->setEnabled(false);
			QMessageBox::information(this, tr("Search/Replace"), tr("Search finished"), CommonStrings::tr_OK);
			Item->CPos = 0;
			NotFound = false;
		}
	}
	else
	{
		if (Doc->scMW()->CurrStED != NULL)
		{
			found = false;
			SEditor* storyTextEdit = Doc->scMW()->CurrStED->Editor;
			if (storyTextEdit->StyledText.length() != 0)
			{
				if (SText->isChecked())
				{
					QTextDocument::FindFlags flags;
					if (!CaseIgnore->isChecked())
						flags |= QTextDocument::FindCaseSensitively;
					if (Word->isChecked())
						flags |= QTextDocument::FindWholeWords;
					do
					{
						found = storyTextEdit->find(sText, flags);
						if (!found) break;
						QTextCursor cursor = storyTextEdit->textCursor();
						int selStart = cursor.selectionStart();
						for (int ap = 0; ap < sText.length(); ++ap)
						{
							const CharStyle& charStyle = storyTextEdit->StyledText.charStyle(selStart + ap);
							if (SSize->isChecked() && (charStyle.fontSize() != sSize))
								found = false;
							if (SFont->isChecked() && (charStyle.font().scName() != sFont))
								found = false;
//							if (SStyle->isChecked() && (hg->cab != sStyle))
//								found = false;
							if (SFill->isChecked() && (charStyle.fillColor() != fCol))
								found = false;
							if (SStroke->isChecked() && (charStyle.strokeColor() != sCol))
								found = false;
							if (SStrokeS->isChecked() && (charStyle.strokeShade() != sStrokeSh))
								found = false;
							if (SFillS->isChecked() && (charStyle.fillShade() != sFillSh))
								found = false;
							if (SEffect->isChecked() && ((charStyle.effects() & 1919) != sEff))
								found = false;
						}
					} while(!found);
				}
				else
				{
					QTextCursor cursor = storyTextEdit->textCursor();
					int position = cursor.position();
					StoryText& styledText = storyTextEdit->StyledText;
					for (int i = position; i < styledText.length(); ++i)
					{
						found = true;
						const CharStyle& charStyle = styledText.charStyle(i);
						if (SSize->isChecked() && (charStyle.fontSize() != sSize))
							found = false;
						if (SFont->isChecked() && (charStyle.font().scName() != sFont))
							found = false;
//						if (SStyle->isChecked() && (hg->cab != sStyle))
//							found = false;
						if (SFill->isChecked() && (charStyle.fillColor() != fCol))
							found = false;
						if (SFillS->isChecked() && (charStyle.fillShade() != sFillSh))
							found = false;
						if (SStroke->isChecked() && (charStyle.strokeColor() != sCol))
							found = false;
						if (SStrokeS->isChecked() && (charStyle.strokeShade() != sStrokeSh))
							found = false;
						if (SEffect->isChecked() && ((charStyle.effects() & 1919) != sEff))
							found = false;
						if (found)
						{
							cursor.setPosition(i + 1, QTextCursor::KeepAnchor);
							storyTextEdit->setTextCursor(cursor);
							break;
						}
					}
				}
				if (found)
				{
					// Doc->scMW()->CurrStED->updateProps(); FIXME
					if (rep)
					{
						DoReplace->setEnabled(true);
						AllReplace->setEnabled(true);
					}
					matchesFound++;
				}
				else
				{
					QMessageBox::information(this, tr("Search/Replace"),
							tr("Search finished, found %1 matches").arg(matchesFound),
							CommonStrings::tr_OK);
					matchesFound = 0;
					NotFound = false;
					QTextCursor cursor = storyTextEdit->textCursor();
					cursor.clearSelection();
					cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
					storyTextEdit->setTextCursor(cursor);
				}
			}
		}
	}
}

void SearchReplace::slotReplace()
{
//	if (SMode)
//		Doc->view()->slotDoCurs(false);
	slotDoReplace();
	if (SMode)
	{
//		Doc->view()->slotDoCurs(true);
		Item->update();
	}
}

void SearchReplace::slotDoReplace()
{
	if (SMode)
	{
		QString repl, sear;
		int cs, cx;
// 		ScText *hg;
		if (RText->isChecked())
		{
			repl = RTextVal->text();
			sear = STextVal->text();
			if (sear.length() == repl.length())
			{
				for (cs = 0; cs < sear.length(); ++cs)
					Item->itemText.replaceChar(ReplStart+cs, repl[cs]);
			}
			else
			{
				if (sear.length() < repl.length())
				{
					for (cs = 0; cs < sear.length(); ++cs)
						Item->itemText.replaceChar(ReplStart+cs, repl[cs]);
					for (cx = cs; cx < repl.length(); ++cx)
						Item->itemText.insertChars(ReplStart+cx, repl.mid(cx,1), true); 
					// FIXME:NLS also replace styles!!
					Item->CPos = ReplStart+cx;
				}
				else
				{
					for (cs = 0; cs < repl.length(); ++cs)
						Item->itemText.replaceChar(ReplStart+cs, repl[cs]);
					Item->itemText.removeChars(ReplStart+cs, sear.length() - cs);
					Item->CPos = ReplStart+cs;
				}
			}
		}
		if (RStyle->isChecked())
			Doc->itemSelection_SetNamedParagraphStyle(Doc->paragraphStyles()[RStyleVal->currentIndex()].name());
		if (RFill->isChecked())
			Doc->itemSelection_SetFillColor(RFillVal->currentText());
		if (RFillS->isChecked())
			Doc->itemSelection_SetFillShade(RFillSVal->getValue());
		if (RStroke->isChecked())
			Doc->itemSelection_SetStrokeColor(RStrokeVal->currentText());
		if (RStrokeS->isChecked())
			Doc->itemSelection_SetStrokeShade(RStrokeSVal->getValue());
		if (RFont->isChecked())
			Doc->itemSelection_SetFont(RFontVal->currentText());
		if (RSize->isChecked())
			Doc->itemSelection_SetFontSize(qRound(RSizeVal->value() * 10.0));
		if (REffect->isChecked())
		{
#ifndef NLS_PROTO
			int s = REffVal->getStyle();
			Doc->currentStyle.charStyle().setFeatures(static_cast<StyleFlag>(s).featureList()); // ???
			for (int a = 0; a < Item->itemText.length(); ++a)
			{
				if (Item->itemText.selected(a))
				{
					StyleFlag fl = Item->itemText.item(a)->effects();
					fl &= static_cast<StyleFlag>(~1919);
					fl |= static_cast<StyleFlag>(s);
					Item->itemText.item(a)->setFeatures(fl.featureList());
				}
			}
#endif
		}
		Item->itemText.deselectAll();
	}
	else
	{
		if (Doc->scMW()->CurrStED != NULL)
		{
			StoryEditor* se=Doc->scMW()->CurrStED;
			if (RStyle->isChecked())
				se->newAlign(RStyleVal->currentIndex());
			if (RFill->isChecked())
				se->newTxFill(RFillVal->currentIndex(), -1);
			if (RFillS->isChecked())
				se->newTxFill(-1, RFillSVal->getValue());
			if (RStroke->isChecked())
				se->newTxStroke(RStrokeVal->currentIndex(), -1);
			if (RStrokeS->isChecked())
				se->newTxStroke(-1, RStrokeSVal->getValue());
			if (RFont->isChecked())
				se->newTxFont(RFontVal->currentText());
			if (RSize->isChecked())
				se->newTxSize(RSizeVal->value());
			if (REffect->isChecked())
				se->newTxStyle(REffVal->getStyle());
			if (RText->isChecked())
			{
				disconnect(se->Editor, SIGNAL(cursorPositionChanged(int, int)), se, SLOT(updateProps(int, int)));
				int SelStart, SelEnd;
				SelStart = se->Editor->textCursor().selectionStart();
				SelEnd = se->Editor->textCursor().selectionEnd();
//					se->Editor->insChars(RTextVal->text());
				se->Editor->textCursor().setPosition(SelStart);
				se->Editor->textCursor().setPosition(SelEnd, QTextCursor::KeepAnchor);
				se->Editor->textCursor().removeSelectedText();
//FIXME				se->Editor->setStyle(se->Editor->CurrentStyle);
				se->Editor->insertPlainText(RTextVal->text());
				connect(se->Editor, SIGNAL(cursorPositionChanged(int, int)), se, SLOT(updateProps(int, int)));
//				se->newAlign(se->Editor->currentParaStyle);
			}
		}
	}
	DoReplace->setEnabled(false);
	AllReplace->setEnabled(false);
	slotDoSearch();
}

void SearchReplace::slotReplaceAll()
{
	if (SMode)
	{
//		Doc->view()->slotDoCurs(false);
		Doc->DoDrawing = false;
	}
	do
	{
		slotDoReplace();
//		slotDoSearch();
	}
	while (NotFound);
	if (SMode)
	{
		Doc->DoDrawing = true;
//		Doc->view()->slotDoCurs(true);
		Item->update();
	}
}

void SearchReplace::enableTxSearch()
{
	bool setter = SText->isChecked();
	STextVal->setEnabled(setter);
	Word->setEnabled(setter);
	CaseIgnore->setEnabled(setter);
	if (setter)
		STextVal->setFocus();
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

void SearchReplace::enableEffSearch()
{
	SEffVal->setEnabled(SEffect->isChecked());
}

void SearchReplace::enableFillSearch()
{
	SFillVal->setEnabled(SFill->isChecked());
}

void SearchReplace::enableFillSSearch()
{
	SFillSVal->setEnabled(SFillS->isChecked());
}

void SearchReplace::enableStrokeSearch()
{
	SStrokeVal->setEnabled(SStroke->isChecked());
}

void SearchReplace::enableStrokeSSearch()
{
	SStrokeSVal->setEnabled(SStrokeS->isChecked());
}

void SearchReplace::enableTxReplace()
{
	RTextVal->setEnabled(RText->isChecked());
	if (RText->isChecked())
		RTextVal->setFocus();
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

void SearchReplace::enableEffReplace()
{
	REffVal->setEnabled(REffect->isChecked());
}

void SearchReplace::enableFillReplace()
{
	RFillVal->setEnabled(RFill->isChecked());
}

void SearchReplace::enableFillSReplace()
{
	RFillSVal->setEnabled(RFillS->isChecked());
}

void SearchReplace::enableStrokeReplace()
{
	RStrokeVal->setEnabled(RStroke->isChecked());
}

void SearchReplace::enableStrokeSReplace()
{
	RStrokeSVal->setEnabled(RStrokeS->isChecked());
}

void SearchReplace::clear()
{

	SStroke->setChecked(false);
	SFill->setChecked(false);
	SStrokeS->setChecked(false);
	SFillS->setChecked(false);
	SSize->setChecked(false);
	SFont->setChecked(false);
	SStyle->setChecked(false);
	SText->setChecked(false);
	SEffect->setChecked(false);
	REffect->setChecked(false);
	STextVal->setText("");
	int currentParaStyle = findParagraphStyle(Doc, Doc->currentStyle);
	SStyleVal->setCurrentIndex(currentParaStyle);
	setCurrentComboItem(SFontVal, Doc->currentStyle.charStyle().font().scName());
	setCurrentComboItem(SFillVal, Doc->currentStyle.charStyle().fillColor());
	setCurrentComboItem(SStrokeVal, Doc->currentStyle.charStyle().strokeColor());
	SSizeVal->setValue(Doc->currentStyle.charStyle().fontSize() / 10.0);
	RStroke->setChecked(false);
	RStrokeS->setChecked(false);
	RFill->setChecked(false);
	RFillS->setChecked(false);
	RSize->setChecked(false);
	RFont->setChecked(false);
	RStyle->setChecked(false);
	RText->setChecked(false);
	RTextVal->setText("");
	RStyleVal->setCurrentIndex(currentParaStyle);
	setCurrentComboItem(RFontVal, Doc->currentStyle.charStyle().font().scName());
	setCurrentComboItem(RFillVal, Doc->currentStyle.charStyle().fillColor());
	setCurrentComboItem(RStrokeVal, Doc->currentStyle.charStyle().strokeColor());
	RSizeVal->setValue(Doc->currentStyle.charStyle().fontSize() / 10.0);
	Word->setChecked(false);
	CaseIgnore->setChecked(false);
	enableTxSearch();
	enableStyleSearch();
	enableFontSearch();
	enableSizeSearch();
	enableEffSearch();
	enableFillSearch();
	enableFillSSearch();
	enableStrokeSearch();
	enableStrokeSSearch();
	enableTxReplace();
	enableStyleReplace();
	enableFontReplace();
	enableSizeReplace();
	enableEffReplace();
	enableFillReplace();
	enableFillSReplace();
	enableStrokeReplace();
	enableStrokeSReplace();
}

void SearchReplace::readPrefs()
{
	SStroke->setChecked(prefs->getBool("SStroke", false));
	SFill->setChecked(prefs->getBool("SFill", false));
	SStrokeS->setChecked(prefs->getBool("SStrokeS", false));
	SFillS->setChecked(prefs->getBool("SFillS", false));
	SSize->setChecked(prefs->getBool("SSize", false));
	SFont->setChecked(prefs->getBool("SFont", false));
	SStyle->setChecked(prefs->getBool("SStyle", false));
	SText->setChecked(prefs->getBool("SText", false));
	SEffect->setChecked(prefs->getBool("SEffect", false));
	REffect->setChecked(prefs->getBool("REffect", false));
	STextVal->setText(prefs->get("STextVal", ""));
	int tmp = prefs->getInt("SStyleVal", findParagraphStyle(Doc, Doc->currentStyle));
	if (tmp < 0 || tmp >= SStyleVal->count())
		SStyleVal->setCurrentIndex(0);
	else
		SStyleVal->setCurrentIndex(tmp);

	setCurrentComboItem(SFontVal, prefs->get("SFontVal", Doc->currentStyle.charStyle().font().scName()));
	setCurrentComboItem(SFillVal, prefs->get("SFillVal", Doc->currentStyle.charStyle().fillColor()));
	setCurrentComboItem(SStrokeVal, prefs->get("SStrokeVal", Doc->currentStyle.charStyle().strokeColor()));
	SSizeVal->setValue(prefs->getDouble("SSizeVal", Doc->currentStyle.charStyle().fontSize() / 10.0));
	RStroke->setChecked(prefs->getBool("RStroke", false));
	RStrokeS->setChecked(prefs->getBool("RStrokeS", false));
	RFill->setChecked(prefs->getBool("RFill", false));
	RFillS->setChecked(prefs->getBool("RFillS", false));
	RSize->setChecked(prefs->getBool("RSize", false));
	RFont->setChecked(prefs->getBool("RFont", false));
	RStyle->setChecked(prefs->getBool("RStyle", false));
	RText->setChecked(prefs->getBool("RText", false));
	RTextVal->setText(prefs->get("RTextVal", ""));
	tmp = prefs->getInt("RStyleVal", findParagraphStyle(Doc, Doc->currentStyle));
	if (tmp < 0 || tmp >= RStyleVal->count())
		RStyleVal->setCurrentIndex(0);
	else
		RStyleVal->setCurrentIndex(tmp);
	setCurrentComboItem(RFontVal, prefs->get("RFontVal", Doc->currentStyle.charStyle().font().scName()));
	setCurrentComboItem(RFillVal, prefs->get("RFillVal", Doc->currentStyle.charStyle().fillColor()));
	setCurrentComboItem(RStrokeVal, prefs->get("RStrokeVal", Doc->currentStyle.charStyle().strokeColor()));
	RSizeVal->setValue(prefs->getDouble("RSizeVal", Doc->currentStyle.charStyle().fontSize() / 10.0));
	Word->setChecked(prefs->getBool("Word", false));
	CaseIgnore->setChecked(prefs->getBool("CaseIgnore", false));
	enableTxSearch();
	enableStyleSearch();
	enableFontSearch();
	enableSizeSearch();
	enableEffSearch();
	enableFillSearch();
	enableFillSSearch();
	enableStrokeSearch();
	enableStrokeSSearch();
	enableTxReplace();
	enableStyleReplace();
	enableFontReplace();
	enableSizeReplace();
	enableEffReplace();
	enableFillReplace();
	enableFillSReplace();
	enableStrokeReplace();
	enableStrokeSReplace();
}

void SearchReplace::writePrefs()
{
	prefs->set("SStroke", SStroke->isChecked());
	prefs->set("SFill", SFill->isChecked());
	prefs->set("SStrokeS", SStrokeS->isChecked());
	prefs->set("SFillS", SFillS->isChecked());
	prefs->set("SSize", SSize->isChecked());
	prefs->set("SFont", SFont->isChecked());
	prefs->set("SStyle", SStyle->isChecked());
	prefs->set("SText", SText->isChecked());
	prefs->set("SEffect", SEffect->isChecked());
	prefs->set("REffect", REffect->isChecked());
	prefs->set("STextVal", STextVal->text());
	prefs->set("SStyleVal", SStyleVal->currentIndex());
	prefs->set("SFontVal", SFontVal->currentText());
	prefs->set("SSizeVal", SSizeVal->value());
	prefs->set("SFillVal", SFillVal->currentText());
	prefs->set("SStrokeVal", SStrokeVal->currentText());
	prefs->set("RStroke", RStroke->isChecked());
	prefs->set("RStrokeS", RStrokeS->isChecked());
	prefs->set("RFill", RFill->isChecked());
	prefs->set("RFillS", RFillS->isChecked());
	prefs->set("RSize", RSize->isChecked());
	prefs->set("RFont", RFont->isChecked());
	prefs->set("RStyle", RStyle->isChecked());
	prefs->set("RText", RText->isChecked());
	prefs->set("RTextVal", RTextVal->text());
	prefs->set("RStyleVal", RStyleVal->currentText());
	prefs->set("RFontVal", RFontVal->currentText());
	prefs->set("RSizeVal", RSizeVal->value());
	prefs->set("RFillVal", RFillVal->currentText());
	prefs->set("RStrokeVal", RStrokeVal->currentText());
	prefs->set("Word", Word->isChecked());
	prefs->set("CaseIgnore", CaseIgnore->isChecked());
	accept();
}
