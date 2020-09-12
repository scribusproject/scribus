/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "search.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QScopedValueRollback>

#include "appmodes.h"
#include "canvas.h"
#include "colorcombo.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "fontcombo.h"
#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scpage.h"
#include "scribus.h"
#include "scribusview.h"
#include "scrspinbox.h"
#include "selection.h"
#include "shadebutton.h"
#include "styleselect.h"
#include "ui/storyeditor.h"
#include "util.h"
#include "util_text.h"

SearchReplace::SearchReplace( QWidget* parent, ScribusDoc *doc, PageItem* ite, bool mode )
	: QDialog( parent )
{
	m_item = ite;
	m_doc = doc;
	m_itemMode = mode;

	setModal(true);
	setWindowTitle( tr( "Search/Replace" ) );
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));

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
	SStyle->setText( tr( "Style" ) );
	SearchLayout->addWidget( SStyle, 1, 0 );
	SAlign = new QCheckBox( Search );
	SAlign->setText( tr( "Alignment" ) );
	SearchLayout->addWidget( SAlign, 2, 0 );
	SFont = new QCheckBox( Search );
	SFont->setText( tr( "Font" ) );
	SearchLayout->addWidget( SFont, 3, 0 );
	SSize = new QCheckBox( Search );
	SSize->setText( tr( "Font Size" ) );
	SearchLayout->addWidget( SSize, 4, 0 );
	SEffect = new QCheckBox( Search );
	SEffect->setText( tr( "Font Effects" ) );
	SearchLayout->addWidget( SEffect, 5, 0 );
	SFill = new QCheckBox( Search);
	SFill->setText( tr( "Fill Color" ) );
	SearchLayout->addWidget( SFill, 6, 0 );
	SFillS = new QCheckBox( Search );
	SFillS->setText( tr( "Fill Shade" ) );
	SearchLayout->addWidget( SFillS, 7, 0 );
	SStroke = new QCheckBox( Search );
	SStroke->setText( tr( "Stroke Color" ) );
	SearchLayout->addWidget( SStroke, 8, 0 );
	SStrokeS = new QCheckBox( Search );
	SStrokeS->setText( tr( "Stroke Shade" ) );
	SearchLayout->addWidget( SStrokeS, 9, 0 );
	STextVal = new QLineEdit( Search );
	STextVal->setEnabled(false);
	SearchLayout->addWidget( STextVal, 0, 1 );
	SStyleVal = new QComboBox( Search );
	SStyleVal->setEditable(false);
	for (int x = 0; x < doc->paragraphStyles().count(); ++x)
		SStyleVal->addItem(doc->paragraphStyles()[x].name());
	QListView *tmpView = dynamic_cast<QListView*>(SStyleVal->view()); Q_ASSERT(tmpView);
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	SStyleVal->setCurrentIndex(findParagraphStyle(doc, doc->currentStyle));
	SStyleVal->setEnabled(false);
	SearchLayout->addWidget( SStyleVal, 1, 1 );
	SAlignVal = new QComboBox( Search );
	SAlignVal->setEditable(false);
	QString tmp_sty[] = { tr("Left"), tr("Center"), tr("Right"), tr("Block"), tr("Forced")};
	size_t ar_sty = sizeof(tmp_sty) / sizeof(*tmp_sty);
	for (uint a = 0; a < ar_sty; ++a)
		SAlignVal->addItem( tmp_sty[a] );
	tmpView = dynamic_cast<QListView*>(SAlignVal->view()); Q_ASSERT(tmpView);
	tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	SAlignVal->setEnabled(false);
	SearchLayout->addWidget( SAlignVal, 2, 1 );
	SFontVal = new FontCombo(Search);
	SFontVal->setMaximumSize(190, 30);
	setCurrentComboItem(SFontVal, doc->currentStyle.charStyle().font().scName());
	SFontVal->setEnabled(false);
	SearchLayout->addWidget( SFontVal, 3, 1 );
	SSizeVal = new ScrSpinBox( 0.5, 2048, Search, 0 );
	SSizeVal->setValue( doc->currentStyle.charStyle().fontSize() / 10.0 );
	SSizeVal->setEnabled(false);
	SearchLayout->addWidget( SSizeVal, 4, 1 );
	SEffVal = new StyleSelect( Search );
	SEffVal->setStyle(0);
	SEffVal->setEnabled(false);
	SearchLayout->addWidget( SEffVal, 5, 1, Qt::AlignLeft );
	SFillVal = new ColorCombo( Search );
	SFillVal->setEditable(false);
	SFillVal->setPixmapType(ColorCombo::fancyPixmaps);
	SFillVal->setColors(doc->PageColors, true);
	SFillVal->setMinimumWidth(SFillVal->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(SFillVal, doc->currentStyle.charStyle().fillColor());
	SFillVal->setEnabled(false);
	SearchLayout->addWidget( SFillVal, 6, 1 );
	SFillSVal = new ShadeButton(Search);
	SFillSVal->setEnabled(false);
	SearchLayout->addWidget( SFillSVal, 7, 1, Qt::AlignLeft );
	SStrokeVal = new ColorCombo( Search );
	SStrokeVal->setEditable(false);
	SStrokeVal->setPixmapType(ColorCombo::fancyPixmaps);
	SStrokeVal->setColors(doc->PageColors, true);
	SStrokeVal->view()->setMinimumWidth(SStrokeVal->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(SStrokeVal, doc->currentStyle.charStyle().strokeColor());
	SStrokeVal->setEnabled(false);
	SearchLayout->addWidget( SStrokeVal, 8, 1 );
	SStrokeSVal =  new ShadeButton(Search);
	SStrokeSVal->setEnabled(false);
	SearchLayout->addWidget( SStrokeSVal, 9, 1, Qt::AlignLeft );
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
	RStyle->setText( tr( "Style" ) );
	ReplaceLayout->addWidget( RStyle, 1, 0 );
	RAlign = new QCheckBox( Replace );
	RAlign->setText( tr( "Alignment" ) );
	ReplaceLayout->addWidget( RAlign, 2, 0 );
	RFont = new QCheckBox( Replace );
	RFont->setText( tr( "Font" ) );
	ReplaceLayout->addWidget( RFont, 3, 0 );
	RSize = new QCheckBox( Replace );
	RSize->setText( tr( "Font Size" ) );
	ReplaceLayout->addWidget( RSize, 4, 0 );
	REffect = new QCheckBox( Replace );
	REffect->setText( tr( "Font Effects" ) );
	ReplaceLayout->addWidget( REffect, 5, 0 );
	RFill = new QCheckBox( Replace );
	RFill->setText( tr( "Fill Color" ) );
	ReplaceLayout->addWidget( RFill, 6, 0 );
	RFillS = new QCheckBox( Replace );
	RFillS->setText( tr( "Fill Shade" ) );
	ReplaceLayout->addWidget( RFillS, 7, 0 );
	RStroke = new QCheckBox( Replace );
	RStroke->setText( tr( "Stroke Color" ) );
	ReplaceLayout->addWidget( RStroke, 8, 0 );
	RStrokeS = new QCheckBox( Replace );
	RStrokeS->setText( tr( "Stroke Shade" ) );
	ReplaceLayout->addWidget( RStrokeS, 9, 0 );
	RTextVal = new QLineEdit( Replace );
	RTextVal->setEnabled(false);
	ReplaceLayout->addWidget( RTextVal, 0, 1 );
	RStyleVal = new QComboBox( Replace );
	RStyleVal->setEditable(false);
	for (int x = 0; x < doc->paragraphStyles().count(); ++x)
		RStyleVal->addItem(doc->paragraphStyles()[x].name());
	tmpView = dynamic_cast<QListView*>(RStyleVal->view()); Q_ASSERT(tmpView);
	tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	RStyleVal->setCurrentIndex(findParagraphStyle(doc, doc->currentStyle));
	RStyleVal->setEnabled(false);
	ReplaceLayout->addWidget( RStyleVal, 1, 1 );
	RAlignVal = new QComboBox( Replace );
	RAlignVal->setEditable(false);
	for (uint a = 0; a < ar_sty; ++a)
		RAlignVal->addItem(tmp_sty[a]);
	tmpView = dynamic_cast<QListView*>(RAlignVal->view()); Q_ASSERT(tmpView);
	tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	RAlignVal->setEnabled(false);
	ReplaceLayout->addWidget( RAlignVal, 2, 1 );
	RFontVal = new FontCombo(Replace);
	RFontVal->setMaximumSize(190, 30);
	setCurrentComboItem(RFontVal, doc->currentStyle.charStyle().font().scName());
	RFontVal->setEnabled(false);
	ReplaceLayout->addWidget( RFontVal, 3, 1 );
	RSizeVal = new ScrSpinBox( 0.5, 2048, Replace, 0 );
	RSizeVal->setValue( doc->currentStyle.charStyle().fontSize() / 10.0 );
	RSizeVal->setEnabled(false);
	ReplaceLayout->addWidget( RSizeVal, 4, 1 );
	REffVal = new StyleSelect( Replace );
	REffVal->setStyle(0);
	REffVal->setEnabled(false);
	ReplaceLayout->addWidget( REffVal, 5, 1, Qt::AlignLeft );
	RFillVal = new ColorCombo( true, Replace );
	RFillVal->setEditable(false);
	RFillVal->setPixmapType(ColorCombo::fancyPixmaps);
	RFillVal->setColors(doc->PageColors, true);
	RFillVal->view()->setMinimumWidth(RFillVal->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(RFillVal, doc->currentStyle.charStyle().fillColor());
	RFillVal->setEnabled(false);
	ReplaceLayout->addWidget( RFillVal, 6, 1 );
	RFillSVal = new ShadeButton(Replace);
	RFillSVal->setEnabled(false);
	ReplaceLayout->addWidget( RFillSVal, 7, 1, Qt::AlignLeft );
	RStrokeVal = new ColorCombo( true, Replace );
	RStrokeVal->setEditable(false);
	RStrokeVal->setPixmapType(ColorCombo::fancyPixmaps);
	RStrokeVal->setColors(doc->PageColors, true);
	RStrokeVal->view()->setMinimumWidth(RStrokeVal->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(RStrokeVal, doc->currentStyle.charStyle().strokeColor());
	RStrokeVal->setEnabled(false);
	ReplaceLayout->addWidget( RStrokeVal, 8, 1 );
	RStrokeSVal = new ShadeButton(Replace);;
	RStrokeSVal->setEnabled(false);
	ReplaceLayout->addWidget( RStrokeSVal, 9, 1, Qt::AlignLeft );
	SelLayout->addWidget( Replace );
	SearchReplaceLayout->addLayout( SelLayout );

	OptsLayout = new QHBoxLayout;
	OptsLayout->setSpacing( 5 );
	OptsLayout->setMargin( 0 );
	Word = new QCheckBox( tr( "&Whole Word" ), this );
	if (mode)
		Word->setEnabled(false);
	OptsLayout->addWidget( Word );
	CaseIgnore = new QCheckBox( tr( "&Ignore Case, Diacritics and Kashida" ), this );
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
	connect( STextVal, SIGNAL( textChanged(QString) ), this, SLOT( updateSearchButtonState() ) );
	connect( SText, SIGNAL( clicked() ), this, SLOT( enableTxSearch() ) );
	connect( SStyle, SIGNAL( clicked() ), this, SLOT( enableStyleSearch() ) );
	connect( SAlign, SIGNAL( clicked() ), this, SLOT( enableAlignSearch() ) );
	connect( SFont, SIGNAL( clicked() ), this, SLOT( enableFontSearch() ) );
	connect( SSize, SIGNAL( clicked() ), this, SLOT( enableSizeSearch() ) );
	connect( SEffect, SIGNAL( clicked() ), this, SLOT( enableEffSearch() ) );
	connect( SFill, SIGNAL( clicked() ), this, SLOT( enableFillSearch() ) );
	connect( SFillS, SIGNAL( clicked() ), this, SLOT( enableFillSSearch() ) );
	connect( SStrokeS, SIGNAL( clicked() ), this, SLOT( enableStrokeSSearch() ) );
	connect( SStroke, SIGNAL( clicked() ), this, SLOT( enableStrokeSearch() ) );
	connect( RText, SIGNAL( clicked() ), this, SLOT( enableTxReplace() ) );
	connect( RStyle, SIGNAL( clicked() ), this, SLOT( enableStyleReplace() ) );
	connect( RAlign, SIGNAL( clicked() ), this, SLOT( enableAlignReplace() ) );
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
	setTabOrder( SStyleVal, SAlignVal );
	setTabOrder( SAlignVal, SFontVal );
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
	setTabOrder( RStyleVal, RAlignVal );
	setTabOrder( RAlignVal, RFontVal );
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

	m_prefs = PrefsManager::instance().prefsFile->getContext("SearchReplace");
	readPrefs();
}

void SearchReplace::slotSearch()
{
	doSearch();

	if (m_itemMode)
		m_item->update();
	if (!m_found)
		showNotFoundMessage();
}

void SearchReplace::doSearch()
{
	int maxChar = m_item->itemText.length() - 1;
	DoReplace->setEnabled(false);
	AllReplace->setEnabled(false);
	if (m_itemMode)
	{
		m_item->itemText.deselectAll();
		m_item->HasSel = false;
	}

	QString fCol = "";
	QString sCol = "";
	QString sFont = "";
	QString sText = "";
	int sStyle = 0;
	int sAlign = 0;
	int sSize = 0;
	int sEff = 0;
	int sFillSh = 100;
	int sStrokeSh = 100;
	bool searchForReplace = false;
	bool rep = false;
	bool found = true;

	m_found = true;

	if ((RFill->isChecked()) || (RStroke->isChecked()) || (RStyle->isChecked()) || (RFont->isChecked())
		|| (RStrokeS->isChecked()) || (RFillS->isChecked()) || (RSize->isChecked()) || (RText->isChecked())
		|| (REffect->isChecked())  || (RAlign->isChecked()))
		rep = true;
	if ((SFill->isChecked()) || (SStroke->isChecked()) || (SStyle->isChecked()) || (SFont->isChecked())
			|| (SStrokeS->isChecked()) || (SFillS->isChecked()) || (SSize->isChecked()) || (SText->isChecked())
			|| (SEffect->isChecked())  || (SAlign->isChecked()))
		searchForReplace = true;
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
	if (SAlign->isChecked())
		sAlign = SAlignVal->currentIndex();
	if (SSize->isChecked())
		sSize = qRound(SSizeVal->value() * 10);
	if (sText.length() > 0)
		found = false;
	
	int a, textLen(0);
	int cursorPos = m_item->itemText.cursorPosition();
	m_replStart = cursorPos;

	if (m_itemMode)
	{
		Qt::CaseSensitivity cs = Qt::CaseSensitive;
		if (CaseIgnore->isChecked())
			cs = Qt::CaseInsensitive;

		for (a = cursorPos; a < m_item->itemText.length(); ++a)
		{
			found = true;
			if (SText->isChecked())
			{
				a = m_item->itemText.indexOf(sText, a, cs, &textLen);
				found = (a >= 0);
				if (!found) break;

				if (Word->isChecked() && (a > 0) && m_item->itemText.text(a - 1).isLetterOrNumber())
					found = false;
				if (Word->isChecked())
				{
					int lastChar = qMin(a + textLen, maxChar);
					found = ((lastChar == maxChar) || !m_item->itemText.text(lastChar).isLetterOrNumber());
				}
				if (!found) continue;
			}
			if (SSize->isChecked())
			{
				if (m_item->itemText.charStyle(a).fontSize() != sSize)
					found = false;
			}
			if (SFont->isChecked())
			{
				if (m_item->itemText.charStyle(a).font().scName() != sFont)
					found = false;
			}

			if (SStyle->isChecked())
			{
				if (m_item->itemText.paragraphStyle(a).parent() != m_doc->paragraphStyles()[sStyle].name())
					found = false;
			}

			if (SAlign->isChecked())
			{
				if (m_item->itemText.paragraphStyle(a).alignment() != sAlign)
					found = false;
			}
			if (SStroke->isChecked())
			{
				if (m_item->itemText.charStyle(a).strokeColor() != sCol)
					found = false;
			}
			if (SStrokeS->isChecked())
			{
				if (m_item->itemText.charStyle(a).strokeShade() != sStrokeSh)
					found = false;
			}
			if (SFillS->isChecked())
			{
				if (m_item->itemText.charStyle(a).fillShade() != sFillSh)
					found = false;
			}
			if (SEffect->isChecked())
			{
				if ((m_item->itemText.charStyle(a).effects() & ScStyle_UserStyles) != sEff)
					found = false;
			}
			if (SFill->isChecked())
			{
				if (m_item->itemText.charStyle(a).fillColor() != fCol)
					found = false;
			}
			if (found && searchForReplace)
			{
				m_item->itemText.select(a, textLen);
				m_item->HasSel = true;
				if (rep)
				{
					DoReplace->setEnabled(true);
					AllReplace->setEnabled(true);
				}
				m_item->itemText.setCursorPosition(a + textLen);

				if (!SText->isChecked())
					break;

				m_replStart = a;
				break;
			}
			if (SText->isChecked())
			{
				for (int xx = m_replStart; xx < a + 1; ++xx)
					m_item->itemText.select(qMin(xx, maxChar), 1, false);
				m_item->HasSel = false;
			}
		}
		if (found && !m_replacingAll)
		{
			QPointF textCanvasPos;
			int foundPos = m_item->itemText.cursorPosition();
			if (m_item->itemText.selectionLength() > 0)
				foundPos = m_item->itemText.startOfSelection();
			bool cPosFound = m_doc->textCanvasPosition(m_item, foundPos, textCanvasPos);
			if (cPosFound)
			{
				QRectF updateRect;
				PageItem* textItem = m_item->frameOfChar(foundPos);
				if (textItem != m_item)
				{
					updateRect = m_item->getVisualBoundingRect();
					updateRect = updateRect.united(textItem->getVisualBoundingRect());
					int selLength = m_item->itemText.selectionLength();
					m_item->itemText.deselectAll();
					m_item->HasSel = false;
					m_doc->m_Selection->delaySignalsOn();
					m_doc->m_Selection->removeItem(m_item);
					m_doc->m_Selection->addItem(textItem);
					m_item = textItem;
					m_item->itemText.deselectAll();
					if (selLength > 0)
						m_item->itemText.select(foundPos, selLength);
					m_item->itemText.setCursorPosition(foundPos + selLength);
					m_item->HasSel = true;
					m_doc->m_Selection->delaySignalsOff();
				}
				QRectF visibleCanvasRect = m_doc->view()->visibleCanvasRect();
				if (!visibleCanvasRect.contains(textCanvasPos))
					m_doc->view()->setCanvasCenterPos(textCanvasPos.x(), textCanvasPos.y());
				if (!updateRect.isEmpty())
					m_doc->regionsChanged()->update(updateRect.adjusted(-10.0, -10.0, 10.0, 10.0));
			}
		}
		if ((!found) || (a == m_item->itemText.length()))
		{
			m_doc->DoDrawing = true;
			m_item->update();
			DoReplace->setEnabled(false);
			AllReplace->setEnabled(false);
			m_item->itemText.setCursorPosition(0);
			m_found = false;
		}
	}
	else if (m_doc->scMW()->CurrStED != nullptr)
	{
		found = false;
		SEditor* storyTextEdit = m_doc->scMW()->CurrStED->Editor;
		if (storyTextEdit->StyledText.length() == 0)
			return;

		QTextCursor cursor = storyTextEdit->textCursor();
		int position  = cursor.position();
		StoryText& styledText = storyTextEdit->StyledText;
		int firstChar = -1, lastChar = styledText.length();
		if (SText->isChecked())
		{
			Qt::CaseSensitivity cs = Qt::CaseSensitive;
			if (CaseIgnore->isChecked())
				cs = Qt::CaseInsensitive;

			for (int i = position; i < styledText.length(); ++i)
			{
				i = styledText.indexOf(sText, i, cs, &textLen);
				found = (i >= 0);
				if (!found)
					break;

				if (Word->isChecked() && (i > 0) && styledText.text(i - 1).isLetterOrNumber())
					found = false;
				if (Word->isChecked())
				{
					int lastChar = qMin(i + textLen, maxChar);
					found = ((lastChar == maxChar) || !styledText.text(lastChar).isLetterOrNumber());
				}
				if (!found) continue;

				int selStart = i;
				for (int ap = 0; ap < textLen; ++ap)
				{
					const ParagraphStyle& parStyle = storyTextEdit->StyledText.paragraphStyle(selStart + ap);
					const CharStyle& charStyle = storyTextEdit->StyledText.charStyle(selStart + ap);
					if (SSize->isChecked() && (charStyle.fontSize() != sSize))
						found = false;
					if (SFont->isChecked() && (charStyle.font().scName() != sFont))
						found = false;
					if (SStyle->isChecked() && (parStyle.parent() != m_doc->paragraphStyles()[sStyle].name()))
						found = false;
					if (SAlign->isChecked() && (parStyle.alignment() != sAlign))
						found = false;
					if (SFill->isChecked() && (charStyle.fillColor() != fCol))
						found = false;
					if (SStroke->isChecked() && (charStyle.strokeColor() != sCol))
						found = false;
					if (SStrokeS->isChecked() && (charStyle.strokeShade() != sStrokeSh))
						found = false;
					if (SFillS->isChecked() && (charStyle.fillShade() != sFillSh))
						found = false;
					if (SEffect->isChecked() && ((charStyle.effects() & ScStyle_UserStyles) != sEff))
						found = false;
				}

				if (found)
				{
					firstChar = i;
					lastChar = i + textLen;
					break;
				}
			}
		}
		else
		{
			for (int i = position; i < styledText.length(); ++i)
			{
				found = true;
				const ParagraphStyle& parStyle = storyTextEdit->StyledText.paragraphStyle(i);
				const CharStyle& charStyle = styledText.charStyle(i);
				if (SSize->isChecked() && (charStyle.fontSize() != sSize))
					found = false;
				if (SFont->isChecked() && (charStyle.font().scName() != sFont))
					found = false;
				if (SStyle->isChecked() && (parStyle.parent() != m_doc->paragraphStyles()[sStyle].name()))
					found = false;
				if (SAlign->isChecked() && (parStyle.alignment() != sAlign))
					found = false;
				if (SFill->isChecked() && (charStyle.fillColor() != fCol))
					found = false;
				if (SFillS->isChecked() && (charStyle.fillShade() != sFillSh))
					found = false;
				if (SStroke->isChecked() && (charStyle.strokeColor() != sCol))
					found = false;
				if (SStrokeS->isChecked() && (charStyle.strokeShade() != sStrokeSh))
					found = false;
				if (SEffect->isChecked() && ((charStyle.effects() & ScStyle_UserStyles) != sEff))
					found = false;
				if (found && (firstChar < 0))
					firstChar = i;
				else if ((firstChar >= 0) && !found)
				{
					lastChar = i;
					break;
				}
				// When searching paragraph styles break at the end of each found paragraph
				if (SStyle->isChecked() && (firstChar >= 0) && styledText.text(i) == SpecialChars::PARSEP)
				{
					lastChar = i;
					break;
				}
			}
		}
		found = (firstChar >= 0);
		if (found)
		{
			cursor.setPosition(firstChar);
			cursor.setPosition(lastChar, QTextCursor::KeepAnchor);
			storyTextEdit->setTextCursor(cursor);
		}
		if (found && searchForReplace)
		{
			// m_doc->scMW()->CurrStED->updateProps(); FIXME
			if (rep)
			{
				DoReplace->setEnabled(true);
				AllReplace->setEnabled(true);
			}
			m_matchesFound++;
			m_firstMatchPosition = storyTextEdit->textCursor().selectionStart();
		}
		else
		{
			m_found = false;
			QTextCursor cursor = storyTextEdit->textCursor();
			cursor.clearSelection();
			cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
			storyTextEdit->setTextCursor(cursor);
		}
	}
}

void SearchReplace::slotReplace()
{
	doReplace();

	if (m_itemMode)
		m_item->update();
	if (!m_found)
		showNotFoundMessage();
}

void SearchReplace::doReplace()
{
	if (m_itemMode)
	{
		if (RText->isChecked())
		{
			QString repl = RTextVal->text();
			m_item->itemText.replaceSelection(repl);
		}
		if (RStyle->isChecked())
		{
			int oldMode = m_doc->appMode;
			m_doc->appMode = modeEdit;
			m_doc->itemSelection_SetNamedParagraphStyle(m_doc->paragraphStyles()[RStyleVal->currentIndex()].name());
			m_doc->appMode = oldMode;
		}
		if (RAlign->isChecked())
		{
			int oldMode = m_doc->appMode;
			m_doc->appMode = modeEdit;
			m_doc->itemSelection_SetAlignment(RAlignVal->currentIndex());
			m_doc->appMode = oldMode;
		}
		if (RFill->isChecked())
			m_doc->itemSelection_SetFillColor(RFillVal->currentText());
		if (RFillS->isChecked())
			m_doc->itemSelection_SetFillShade(RFillSVal->getValue());
		if (RStroke->isChecked())
			m_doc->itemSelection_SetStrokeColor(RStrokeVal->currentText());
		if (RStrokeS->isChecked())
			m_doc->itemSelection_SetStrokeShade(RStrokeSVal->getValue());
		if (RFont->isChecked())
			m_doc->itemSelection_SetFont(RFontVal->currentText());
		if (RSize->isChecked())
			m_doc->itemSelection_SetFontSize(qRound(RSizeVal->value() * 10.0));
		if (REffect->isChecked() && (m_item->itemText.hasSelection()))
		{
			int s = REffVal->getStyle() & ScStyle_UserStyles;
			m_doc->currentStyle.charStyle().setFeatures(static_cast<StyleFlag>(s).featureList()); // ???
			for (int i = 0; i < m_item->itemText.length(); ++i)
			{
				if (m_item->itemText.selected(i))
				{
					CharStyle newFeatures;
					newFeatures.setFeatures(static_cast<StyleFlag>(s).featureList());
					m_item->itemText.applyCharStyle(i, 1, newFeatures);
				}
			}
		}
		m_item->itemText.deselectAll();
	}
	else if (m_doc->scMW()->CurrStED != nullptr)
	{
		StoryEditor* se = m_doc->scMW()->CurrStED;
		if (RText->isChecked())
		{
			disconnect(se->Editor, SIGNAL(cursorPositionChanged()), se, SLOT(updateProps()));
			int SelStart = se->Editor->textCursor().selectionStart();
			int SelEnd = se->Editor->textCursor().selectionEnd();
//			se->Editor->insChars(RTextVal->text());
			se->Editor->textCursor().setPosition(SelStart);
			se->Editor->textCursor().setPosition(SelEnd, QTextCursor::KeepAnchor);
			se->Editor->textCursor().removeSelectedText();
//FIXME		se->Editor->setEffects(se->Editor->CurrentEffects);
			QString newText = RTextVal->text();
			se->Editor->insertPlainText(newText);
			if (newText.length() > 0)
			{
				QTextCursor textCursor = se->Editor->textCursor();
				textCursor.setPosition(SelStart);
				textCursor.setPosition(SelStart + newText.length(), QTextCursor::KeepAnchor);
				se->Editor->setTextCursor(textCursor);
			}
			connect(se->Editor, SIGNAL(cursorPositionChanged()), se, SLOT(updateProps()));
//			se->newAlign(se->Editor->currentParaStyle);
		}
		if (RStyle->isChecked())
			se->newStyle(m_doc->paragraphStyles()[RStyleVal->currentIndex()].name());
		if (RAlign->isChecked())
			se->newAlign(RAlignVal->currentIndex());
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

		QTextCursor textCursor = se->Editor->textCursor();
		int selStart = textCursor.selectionStart();
		int selEnd   = textCursor.selectionEnd();
		int selPos   = qMax(selStart, selEnd);
		textCursor.setPosition(selPos);
		se->Editor->setTextCursor(textCursor);
	}
	DoReplace->setEnabled(false);
	AllReplace->setEnabled(false);
	doSearch();
}

int SearchReplace::firstMatchCursorPosition()
{
	return m_firstMatchPosition;
}

void SearchReplace::setSearchedText(const QString& text)
{
	if (SText->isChecked())
		STextVal->setText(text);
}

void SearchReplace::slotReplaceAll()
{
	QScopedValueRollback<bool> replaceAllRollback(m_replacingAll, true);

	if (m_itemMode)
		m_doc->DoDrawing = false;

	do
	{
		doReplace();
	}
	while (m_found);

	if (m_itemMode)
	{
		m_doc->DoDrawing = true;
		if (m_item->isTextFrame())
			m_item->asTextFrame()->invalidateLayout(true);
		m_doc->regionsChanged()->update(QRectF());
	}

	showNotFoundMessage();
}

void SearchReplace::showNotFoundMessage()
{
	if (m_found)
		return;

	if (m_itemMode)
		ScMessageBox::information(this, tr("Search/Replace"), tr("Search finished"));
	else
	{
		ScMessageBox::information(this, tr("Search/Replace"),
					tr("Search finished, found %1 matches").arg(m_matchesFound));
		m_matchesFound = 0;
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
	updateSearchButtonState();
}

void SearchReplace::enableStyleSearch()
{
	SStyleVal->setEnabled(SStyleVal->count() ? SStyle->isChecked() : false);
	updateSearchButtonState();
}

void SearchReplace::enableAlignSearch()
{
	SAlignVal->setEnabled(SAlign->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableFontSearch()
{
	SFontVal->setEnabled(SFont->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableSizeSearch()
{
	SSizeVal->setEnabled(SSize->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableEffSearch()
{
	SEffVal->setEnabled(SEffect->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableFillSearch()
{
	SFillVal->setEnabled(SFill->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableFillSSearch()
{
	SFillSVal->setEnabled(SFillS->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableStrokeSearch()
{
	SStrokeVal->setEnabled(SStroke->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableStrokeSSearch()
{
	SStrokeSVal->setEnabled(SStrokeS->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableTxReplace()
{
	RTextVal->setEnabled(RText->isChecked());
	if (RText->isChecked())
		RTextVal->setFocus();
	updateReplaceButtonsState();
}

void SearchReplace::enableStyleReplace()
{
	RStyleVal->setEnabled(RStyle->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableAlignReplace()
{
	RAlignVal->setEnabled(RAlign->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableFontReplace()
{
	RFontVal->setEnabled(RFont->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableSizeReplace()
{
	RSizeVal->setEnabled(RSize->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableEffReplace()
{
	REffVal->setEnabled(REffect->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableFillReplace()
{
	RFillVal->setEnabled(RFill->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableFillSReplace()
{
	RFillSVal->setEnabled(RFillS->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableStrokeReplace()
{
	RStrokeVal->setEnabled(RStroke->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableStrokeSReplace()
{
	RStrokeSVal->setEnabled(RStrokeS->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::clear()
{
	SAlign->setChecked(false);
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
	int currentParaStyle = findParagraphStyle(m_doc, m_doc->currentStyle);
	SStyleVal->setCurrentIndex(currentParaStyle);
	RAlignVal->setCurrentIndex(m_doc->currentStyle.alignment());
	setCurrentComboItem(SFontVal, m_doc->currentStyle.charStyle().font().scName());
	setCurrentComboItem(SFillVal, m_doc->currentStyle.charStyle().fillColor());
	setCurrentComboItem(SStrokeVal, m_doc->currentStyle.charStyle().strokeColor());
	SSizeVal->setValue(m_doc->currentStyle.charStyle().fontSize() / 10.0);
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
	RAlignVal->setCurrentIndex(m_doc->currentStyle.alignment());
	setCurrentComboItem(RFontVal, m_doc->currentStyle.charStyle().font().scName());
	setCurrentComboItem(RFillVal, m_doc->currentStyle.charStyle().fillColor());
	setCurrentComboItem(RStrokeVal, m_doc->currentStyle.charStyle().strokeColor());
	RSizeVal->setValue(m_doc->currentStyle.charStyle().fontSize() / 10.0);
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

void SearchReplace::updateReplaceButtonsState()
{
	bool replaceEnabled = false;
	if (RFill->isChecked() || RStroke->isChecked() || RStyle->isChecked() || RFont->isChecked()  ||
		RStrokeS->isChecked() || RFillS->isChecked() || RSize->isChecked() || REffect->isChecked() ||
		RAlign->isChecked())
	{
		replaceEnabled = true;
	}
	replaceEnabled |= RText->isChecked();
	if (m_itemMode)
		replaceEnabled &= (m_item->itemText.hasSelection());
	else if (m_doc->scMW()->CurrStED != nullptr)
		replaceEnabled &= m_doc->scMW()->CurrStED->Editor->textCursor().hasSelection();
	else
		replaceEnabled = false;
	replaceEnabled &= m_found;
	DoReplace->setEnabled(replaceEnabled);
	AllReplace->setEnabled(replaceEnabled);
}

void SearchReplace::updateSearchButtonState()
{
	bool searchEnabled = false;
	if (SFill->isChecked() || SStroke->isChecked() || SStyle->isChecked() || SFont->isChecked() ||
		SStrokeS->isChecked() || SFillS->isChecked() || SSize->isChecked() || SEffect->isChecked() ||
		SAlign->isChecked())
	{
		searchEnabled = true;
	}
	searchEnabled |= (SText->isChecked() && !STextVal->text().isEmpty());
	DoSearch->setEnabled(searchEnabled);
}

void SearchReplace::readPrefs()
{
	SStroke->setChecked(m_prefs->getBool("SStroke", false));
	SFill->setChecked(m_prefs->getBool("SFill", false));
	SStrokeS->setChecked(m_prefs->getBool("SStrokeS", false));
	SFillS->setChecked(m_prefs->getBool("SFillS", false));
	SSize->setChecked(m_prefs->getBool("SSize", false));
	SFont->setChecked(m_prefs->getBool("SFont", false));
	SStyle->setChecked(m_prefs->getBool("SStyle", false));
	SAlign->setChecked(m_prefs->getBool("SAlign", false));
	SText->setChecked(m_prefs->getBool("SText", true));
	SEffect->setChecked(m_prefs->getBool("SEffect", false));
	REffect->setChecked(m_prefs->getBool("REffect", false));
	STextVal->setText(m_prefs->get("STextVal", ""));
	int tmp = m_prefs->getInt("SStyleVal", findParagraphStyle(m_doc, m_doc->currentStyle));
	if (tmp < 0 || tmp >= SStyleVal->count())
		tmp = 0;
	SStyleVal->setCurrentIndex(tmp);
	tmp = m_prefs->getInt("SAlignVal", m_doc->currentStyle.alignment());
	if (tmp < 0 || tmp >= SAlignVal->count())
		tmp = 0;
	SAlignVal->setCurrentIndex(tmp);
	setCurrentComboItem(SFontVal, m_prefs->get("SFontVal", m_doc->currentStyle.charStyle().font().scName()));
	setCurrentComboItem(SFillVal, m_prefs->get("SFillVal", m_doc->currentStyle.charStyle().fillColor()));
	setCurrentComboItem(SStrokeVal, m_prefs->get("SStrokeVal", m_doc->currentStyle.charStyle().strokeColor()));
	SSizeVal->setValue(m_prefs->getDouble("SSizeVal", m_doc->currentStyle.charStyle().fontSize() / 10.0));
	RStroke->setChecked(m_prefs->getBool("RStroke", false));
	RStrokeS->setChecked(m_prefs->getBool("RStrokeS", false));
	RFill->setChecked(m_prefs->getBool("RFill", false));
	RFillS->setChecked(m_prefs->getBool("RFillS", false));
	RSize->setChecked(m_prefs->getBool("RSize", false));
	RFont->setChecked(m_prefs->getBool("RFont", false));
	RStyle->setChecked(m_prefs->getBool("RStyle", false));
	RAlign->setChecked(m_prefs->getBool("RAlign", false));
	RText->setChecked(m_prefs->getBool("RText", true));
	RTextVal->setText(m_prefs->get("RTextVal", ""));
	tmp = m_prefs->getInt("RStyleVal", findParagraphStyle(m_doc, m_doc->currentStyle));
	if (tmp < 0 || tmp >= RStyleVal->count())
		tmp = 0;
	RStyleVal->setCurrentIndex(tmp);
	tmp = m_prefs->getInt("RAlignVal", m_doc->currentStyle.alignment());
	if (tmp < 0 || tmp >= RAlignVal->count())
		tmp = 0;
	RAlignVal->setCurrentIndex(tmp);
	setCurrentComboItem(RFontVal, m_prefs->get("RFontVal", m_doc->currentStyle.charStyle().font().scName()));
	setCurrentComboItem(RFillVal, m_prefs->get("RFillVal", m_doc->currentStyle.charStyle().fillColor()));
	setCurrentComboItem(RStrokeVal, m_prefs->get("RStrokeVal", m_doc->currentStyle.charStyle().strokeColor()));
	RSizeVal->setValue(m_prefs->getDouble("RSizeVal", m_doc->currentStyle.charStyle().fontSize() / 10.0));
	Word->setChecked(m_prefs->getBool("Word", false));
	CaseIgnore->setChecked(m_prefs->getBool("CaseIgnore", false));

	enableTxSearch();
	enableStyleSearch();
	enableAlignSearch();
	enableFontSearch();
	enableSizeSearch();
	enableEffSearch();
	enableFillSearch();
	enableFillSSearch();
	enableStrokeSearch();
	enableStrokeSSearch();
	enableTxReplace();
	enableStyleReplace();
	enableAlignReplace();
	enableFontReplace();
	enableSizeReplace();
	enableEffReplace();
	enableFillReplace();
	enableFillSReplace();
	enableStrokeReplace();
	enableStrokeSReplace();

	if (SText->isChecked() && RText->isChecked())
		STextVal->setFocus();
}

void SearchReplace::writePrefs()
{
	m_prefs->set("SStroke", SStroke->isChecked());
	m_prefs->set("SFill", SFill->isChecked());
	m_prefs->set("SStrokeS", SStrokeS->isChecked());
	m_prefs->set("SFillS", SFillS->isChecked());
	m_prefs->set("SSize", SSize->isChecked());
	m_prefs->set("SFont", SFont->isChecked());
	m_prefs->set("SStyle", SStyle->isChecked());
	m_prefs->set("SAlign", SAlign->isChecked());
	m_prefs->set("SText", SText->isChecked());
	m_prefs->set("SEffect", SEffect->isChecked());
	m_prefs->set("REffect", REffect->isChecked());
	m_prefs->set("STextVal", STextVal->text());
	m_prefs->set("SStyleVal", SStyleVal->currentIndex());
	m_prefs->set("SAlignVal", SAlignVal->currentIndex());
	m_prefs->set("SFontVal", SFontVal->currentText());
	m_prefs->set("SSizeVal", SSizeVal->value());
	m_prefs->set("SFillVal", SFillVal->currentText());
	m_prefs->set("SStrokeVal", SStrokeVal->currentText());
	m_prefs->set("RStroke", RStroke->isChecked());
	m_prefs->set("RStrokeS", RStrokeS->isChecked());
	m_prefs->set("RFill", RFill->isChecked());
	m_prefs->set("RFillS", RFillS->isChecked());
	m_prefs->set("RSize", RSize->isChecked());
	m_prefs->set("RFont", RFont->isChecked());
	m_prefs->set("RStyle", RStyle->isChecked());
	m_prefs->set("RAlign", RAlign->isChecked());
	m_prefs->set("RText", RText->isChecked());
	m_prefs->set("RTextVal", RTextVal->text());
	m_prefs->set("RStyleVal", RStyleVal->currentText());
	m_prefs->set("RAlignVal", RAlignVal->currentIndex());
	m_prefs->set("RFontVal", RFontVal->currentText());
	m_prefs->set("RSizeVal", RSizeVal->value());
	m_prefs->set("RFillVal", RFillVal->currentText());
	m_prefs->set("RStrokeVal", RStrokeVal->currentText());
	m_prefs->set("Word", Word->isChecked());
	m_prefs->set("CaseIgnore", CaseIgnore->isChecked());
	accept();
}
