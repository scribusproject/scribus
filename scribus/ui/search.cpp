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
#include "undomanager.h"
#include "undotransaction.h"
#include "util.h"
#include "util_text.h"

SearchReplace::SearchReplace( QWidget* parent, ScribusDoc *doc, PageItem* item, bool mode )
	: QDialog( parent )
{
	m_item = item;
	m_doc = doc;
	m_itemMode = mode;

	setModal(true);
	setWindowTitle( tr( "Search/Replace" ) );
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));

	SearchReplaceLayout = new QVBoxLayout( this );
	SearchReplaceLayout->setContentsMargins(9, 9, 9, 9);
	SearchReplaceLayout->setSpacing(6);
	SelLayout = new QHBoxLayout;
	SelLayout->setContentsMargins(0, 0, 0, 0);
	SelLayout->setSpacing(6);
	searchGroupBox = new QGroupBox( this );
	searchGroupBox->setTitle( tr( "Search for:" ) );
	SearchLayout = new QGridLayout( searchGroupBox );
	SearchLayout->setContentsMargins(9, 9, 9, 9);
	SearchLayout->setSpacing(6);
	SearchLayout->setAlignment( Qt::AlignTop );
	searchTextCheckBox = new QCheckBox( searchGroupBox );
	searchTextCheckBox->setText( tr( "Text" ) );
	SearchLayout->addWidget( searchTextCheckBox, 0, 0 );
	searchStyleCheckBox = new QCheckBox( searchGroupBox );
	searchStyleCheckBox->setText( tr( "Style" ) );
	SearchLayout->addWidget( searchStyleCheckBox, 1, 0 );
	searchAlignCheckBox = new QCheckBox( searchGroupBox );
	searchAlignCheckBox->setText( tr( "Alignment" ) );
	SearchLayout->addWidget( searchAlignCheckBox, 2, 0 );
	searchFontCheckBox = new QCheckBox( searchGroupBox );
	searchFontCheckBox->setText( tr( "Font" ) );
	SearchLayout->addWidget( searchFontCheckBox, 3, 0 );
	searchSizeCheckBox = new QCheckBox( searchGroupBox );
	searchSizeCheckBox->setText( tr( "Font Size" ) );
	SearchLayout->addWidget( searchSizeCheckBox, 4, 0 );
	searchEffectCheckBox = new QCheckBox( searchGroupBox );
	searchEffectCheckBox->setText( tr( "Font Effects" ) );
	SearchLayout->addWidget( searchEffectCheckBox, 5, 0 );
	searchFillCheckBox = new QCheckBox( searchGroupBox);
	searchFillCheckBox->setText( tr( "Fill Color" ) );
	SearchLayout->addWidget( searchFillCheckBox, 6, 0 );
	searchFillShadeCheckBox = new QCheckBox( searchGroupBox );
	searchFillShadeCheckBox->setText( tr( "Fill Shade" ) );
	SearchLayout->addWidget( searchFillShadeCheckBox, 7, 0 );
	searchStrokeCheckBox = new QCheckBox( searchGroupBox );
	searchStrokeCheckBox->setText( tr( "Stroke Color" ) );
	SearchLayout->addWidget( searchStrokeCheckBox, 8, 0 );
	searchStrokeShadeCheckBox = new QCheckBox( searchGroupBox );
	searchStrokeShadeCheckBox->setText( tr( "Stroke Shade" ) );
	SearchLayout->addWidget( searchStrokeShadeCheckBox, 9, 0 );
	searchTextLineEdit = new QLineEdit( searchGroupBox );
	searchTextLineEdit->setEnabled(false);
	SearchLayout->addWidget( searchTextLineEdit, 0, 1 );
	searchStyleValue = new QComboBox( searchGroupBox );
	searchStyleValue->setEditable(false);
	for (int x = 0; x < doc->paragraphStyles().count(); ++x)
		searchStyleValue->addItem(doc->paragraphStyles()[x].name());
	QListView *tmpView = qobject_cast<QListView*>(searchStyleValue->view()); Q_ASSERT(tmpView);
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	searchStyleValue->setCurrentIndex(findParagraphStyle(doc, doc->currentStyle));
	searchStyleValue->setEnabled(false);
	SearchLayout->addWidget( searchStyleValue, 1, 1 );
	searchAlignValue = new QComboBox( searchGroupBox );
	searchAlignValue->setEditable(false);
	QString tmp_sty[] = { tr("Left"), tr("Center"), tr("Right"), tr("Block"), tr("Forced")};
	size_t ar_sty = sizeof(tmp_sty) / sizeof(*tmp_sty);
	for (uint a = 0; a < ar_sty; ++a)
		searchAlignValue->addItem( tmp_sty[a] );
	tmpView = qobject_cast<QListView*>(searchAlignValue->view()); Q_ASSERT(tmpView);
	tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	searchAlignValue->setEnabled(false);
	SearchLayout->addWidget( searchAlignValue, 2, 1 );
	searchFontValue = new FontCombo(searchGroupBox);
	searchFontValue->setMaximumSize(190, 30);
	setCurrentComboItem(searchFontValue, doc->currentStyle.charStyle().font().scName());
	searchFontValue->setEnabled(false);
	SearchLayout->addWidget( searchFontValue, 3, 1 );
	searchSizeSpinBox = new ScrSpinBox( 0.5, 2048, searchGroupBox, 0 );
	searchSizeSpinBox->setValue( doc->currentStyle.charStyle().fontSize() / 10.0 );
	searchSizeSpinBox->setEnabled(false);
	SearchLayout->addWidget( searchSizeSpinBox, 4, 1 );
	searchStyleEffectsValue = new StyleSelect( searchGroupBox );
	searchStyleEffectsValue->setStyle(0);
	searchStyleEffectsValue->setEnabled(false);
	SearchLayout->addWidget( searchStyleEffectsValue, 5, 1, Qt::AlignLeft );
	searchFillValue = new ColorCombo( searchGroupBox );
	searchFillValue->setEditable(false);
	searchFillValue->setPixmapType(ColorCombo::fancyPixmaps);
	searchFillValue->setColors(doc->PageColors, true);
	searchFillValue->setMinimumWidth(searchFillValue->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(searchFillValue, doc->currentStyle.charStyle().fillColor());
	searchFillValue->setEnabled(false);
	SearchLayout->addWidget( searchFillValue, 6, 1 );
	searchFillShadeValue = new ShadeButton(searchGroupBox);
	searchFillShadeValue->setEnabled(false);
	SearchLayout->addWidget( searchFillShadeValue, 7, 1, Qt::AlignLeft );
	searchStrokeValue = new ColorCombo( searchGroupBox );
	searchStrokeValue->setEditable(false);
	searchStrokeValue->setPixmapType(ColorCombo::fancyPixmaps);
	searchStrokeValue->setColors(doc->PageColors, true);
	searchStrokeValue->view()->setMinimumWidth(searchStrokeValue->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(searchStrokeValue, doc->currentStyle.charStyle().strokeColor());
	searchStrokeValue->setEnabled(false);
	SearchLayout->addWidget( searchStrokeValue, 8, 1 );
	searchStrokeShadeValue =  new ShadeButton(searchGroupBox);
	searchStrokeShadeValue->setEnabled(false);
	SearchLayout->addWidget( searchStrokeShadeValue, 9, 1, Qt::AlignLeft );
	SelLayout->addWidget( searchGroupBox );

	replaceGroupBox = new QGroupBox( this );
	replaceGroupBox->setTitle( tr( "Replace with:" ) );
	ReplaceLayout = new QGridLayout( replaceGroupBox );
	ReplaceLayout->setSpacing(6);
	ReplaceLayout->setContentsMargins(9, 9, 9, 9);
	ReplaceLayout->setAlignment( Qt::AlignTop );
	replaceTextCheckBox = new QCheckBox( replaceGroupBox );
	replaceTextCheckBox->setText( tr( "Text" ) );
	ReplaceLayout->addWidget( replaceTextCheckBox, 0, 0 );
	replaceStyleCheckBox = new QCheckBox( replaceGroupBox );
	replaceStyleCheckBox->setText( tr( "Style" ) );
	ReplaceLayout->addWidget( replaceStyleCheckBox, 1, 0 );
	replaceAlignCheckBox = new QCheckBox( replaceGroupBox );
	replaceAlignCheckBox->setText( tr( "Alignment" ) );
	ReplaceLayout->addWidget( replaceAlignCheckBox, 2, 0 );
	replaceFontCheckBox = new QCheckBox( replaceGroupBox );
	replaceFontCheckBox->setText( tr( "Font" ) );
	ReplaceLayout->addWidget( replaceFontCheckBox, 3, 0 );
	replaceSizeCheckBox = new QCheckBox( replaceGroupBox );
	replaceSizeCheckBox->setText( tr( "Font Size" ) );
	ReplaceLayout->addWidget( replaceSizeCheckBox, 4, 0 );
	replaceEffectCheckBox = new QCheckBox( replaceGroupBox );
	replaceEffectCheckBox->setText( tr( "Font Effects" ) );
	ReplaceLayout->addWidget( replaceEffectCheckBox, 5, 0 );
	replaceFillCheckBox = new QCheckBox( replaceGroupBox );
	replaceFillCheckBox->setText( tr( "Fill Color" ) );
	ReplaceLayout->addWidget( replaceFillCheckBox, 6, 0 );
	replaceFillShadeCheckBox = new QCheckBox( replaceGroupBox );
	replaceFillShadeCheckBox->setText( tr( "Fill Shade" ) );
	ReplaceLayout->addWidget( replaceFillShadeCheckBox, 7, 0 );
	replaceStrokeCheckBox = new QCheckBox( replaceGroupBox );
	replaceStrokeCheckBox->setText( tr( "Stroke Color" ) );
	ReplaceLayout->addWidget( replaceStrokeCheckBox, 8, 0 );
	replaceStrokeShadeCheckBox = new QCheckBox( replaceGroupBox );
	replaceStrokeShadeCheckBox->setText( tr( "Stroke Shade" ) );
	ReplaceLayout->addWidget( replaceStrokeShadeCheckBox, 9, 0 );
	replaceTextLineEdit = new QLineEdit( replaceGroupBox );
	replaceTextLineEdit->setEnabled(false);
	ReplaceLayout->addWidget( replaceTextLineEdit, 0, 1 );
	replaceStyleValue = new QComboBox( replaceGroupBox );
	replaceStyleValue->setEditable(false);
	for (int x = 0; x < doc->paragraphStyles().count(); ++x)
		replaceStyleValue->addItem(doc->paragraphStyles()[x].name());
	tmpView = qobject_cast<QListView*>(replaceStyleValue->view()); Q_ASSERT(tmpView);
	tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	replaceStyleValue->setCurrentIndex(findParagraphStyle(doc, doc->currentStyle));
	replaceStyleValue->setEnabled(false);
	ReplaceLayout->addWidget( replaceStyleValue, 1, 1 );
	replaceAlignValue = new QComboBox( replaceGroupBox );
	replaceAlignValue->setEditable(false);
	for (uint a = 0; a < ar_sty; ++a)
		replaceAlignValue->addItem(tmp_sty[a]);
	tmpView = qobject_cast<QListView*>(replaceAlignValue->view()); Q_ASSERT(tmpView);
	tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	replaceAlignValue->setEnabled(false);
	ReplaceLayout->addWidget( replaceAlignValue, 2, 1 );
	replaceFontValue = new FontCombo(replaceGroupBox);
	replaceFontValue->setMaximumSize(190, 30);
	setCurrentComboItem(replaceFontValue, doc->currentStyle.charStyle().font().scName());
	replaceFontValue->setEnabled(false);
	ReplaceLayout->addWidget( replaceFontValue, 3, 1 );
	replaceSizeSpinBox = new ScrSpinBox( 0.5, 2048, replaceGroupBox, 0 );
	replaceSizeSpinBox->setValue( doc->currentStyle.charStyle().fontSize() / 10.0 );
	replaceSizeSpinBox->setEnabled(false);
	ReplaceLayout->addWidget( replaceSizeSpinBox, 4, 1 );
	replaceStyleEffectsValue = new StyleSelect( replaceGroupBox );
	replaceStyleEffectsValue->setStyle(0);
	replaceStyleEffectsValue->setEnabled(false);
	ReplaceLayout->addWidget( replaceStyleEffectsValue, 5, 1, Qt::AlignLeft );
	replaceFillValue = new ColorCombo( true, replaceGroupBox );
	replaceFillValue->setEditable(false);
	replaceFillValue->setPixmapType(ColorCombo::fancyPixmaps);
	replaceFillValue->setColors(doc->PageColors, true);
	replaceFillValue->view()->setMinimumWidth(replaceFillValue->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(replaceFillValue, doc->currentStyle.charStyle().fillColor());
	replaceFillValue->setEnabled(false);
	ReplaceLayout->addWidget( replaceFillValue, 6, 1 );
	replaceFillShadeValue = new ShadeButton(replaceGroupBox);
	replaceFillShadeValue->setEnabled(false);
	ReplaceLayout->addWidget( replaceFillShadeValue, 7, 1, Qt::AlignLeft );
	replaceStrokeValue = new ColorCombo( true, replaceGroupBox );
	replaceStrokeValue->setEditable(false);
	replaceStrokeValue->setPixmapType(ColorCombo::fancyPixmaps);
	replaceStrokeValue->setColors(doc->PageColors, true);
	replaceStrokeValue->view()->setMinimumWidth(replaceStrokeValue->view()->maximumViewportSize().width() + 24);
	setCurrentComboItem(replaceStrokeValue, doc->currentStyle.charStyle().strokeColor());
	replaceStrokeValue->setEnabled(false);
	ReplaceLayout->addWidget( replaceStrokeValue, 8, 1 );
	replaceStrokeShadeValue = new ShadeButton(replaceGroupBox);
	replaceStrokeShadeValue->setEnabled(false);
	ReplaceLayout->addWidget( replaceStrokeShadeValue, 9, 1, Qt::AlignLeft );
	SelLayout->addWidget( replaceGroupBox );
	SearchReplaceLayout->addLayout( SelLayout );

	OptsLayout = new QHBoxLayout;
	OptsLayout->setSpacing(6);
	OptsLayout->setContentsMargins(0, 0, 0, 0);
	wholeWordCheckBox = new QCheckBox( tr( "&Whole Word" ), this );
	if (mode)
		wholeWordCheckBox->setEnabled(false);
	OptsLayout->addWidget( wholeWordCheckBox );
	ignoreCaseCheckBox = new QCheckBox( tr( "&Ignore Case, Diacritics and Kashida" ), this );
	if (mode)
		ignoreCaseCheckBox->setEnabled(false);
	OptsLayout->addWidget( ignoreCaseCheckBox );
	SearchReplaceLayout->addLayout( OptsLayout );

	ButtonsLayout = new QHBoxLayout;
	ButtonsLayout->setSpacing(6);
	ButtonsLayout->setContentsMargins(0, 0, 0, 0);
	searchButton = new QPushButton( tr( "&Search" ), this );
	searchButton->setDefault( true );
	ButtonsLayout->addWidget( searchButton );
	replaceButton = new QPushButton( tr( "&Replace" ), this );
	replaceButton->setEnabled(false);
	ButtonsLayout->addWidget( replaceButton );
	replaceAllButton = new QPushButton( tr( "Replace &All" ), this );
	replaceAllButton->setEnabled(false);
	ButtonsLayout->addWidget( replaceAllButton );
	clearButton = new QPushButton( tr("C&lear"), this);
	ButtonsLayout->addWidget(clearButton);
	closeButton = new QPushButton( tr( "&Close" ), this );
	ButtonsLayout->addWidget( closeButton );
	SearchReplaceLayout->addLayout( ButtonsLayout );

	resize(minimumSizeHint());

 // signals and slots connections
	connect( closeButton, SIGNAL( clicked() ), this, SLOT( writePrefs() ) );
	connect( searchButton, SIGNAL( clicked() ), this, SLOT( slotSearch() ) );
	connect( replaceButton, SIGNAL( clicked() ), this, SLOT( slotReplace() ) );
	connect( replaceAllButton, SIGNAL( clicked() ), this, SLOT( slotReplaceAll() ) );
	connect( searchTextLineEdit, SIGNAL( textChanged(QString) ), this, SLOT( updateSearchButtonState() ) );
	connect( searchTextCheckBox, SIGNAL( clicked() ), this, SLOT( enableTxSearch() ) );
	connect( searchStyleCheckBox, SIGNAL( clicked() ), this, SLOT( enableStyleSearch() ) );
	connect( searchAlignCheckBox, SIGNAL( clicked() ), this, SLOT( enableAlignSearch() ) );
	connect( searchFontCheckBox, SIGNAL( clicked() ), this, SLOT( enableFontSearch() ) );
	connect( searchSizeCheckBox, SIGNAL( clicked() ), this, SLOT( enableSizeSearch() ) );
	connect( searchEffectCheckBox, SIGNAL( clicked() ), this, SLOT( enableEffSearch() ) );
	connect( searchFillCheckBox, SIGNAL( clicked() ), this, SLOT( enableFillSearch() ) );
	connect( searchFillShadeCheckBox, SIGNAL( clicked() ), this, SLOT( enableFillSSearch() ) );
	connect( searchStrokeShadeCheckBox, SIGNAL( clicked() ), this, SLOT( enableStrokeSSearch() ) );
	connect( searchStrokeCheckBox, SIGNAL( clicked() ), this, SLOT( enableStrokeSearch() ) );
	connect( replaceTextCheckBox, SIGNAL( clicked() ), this, SLOT( enableTxReplace() ) );
	connect( replaceStyleCheckBox, SIGNAL( clicked() ), this, SLOT( enableStyleReplace() ) );
	connect( replaceAlignCheckBox, SIGNAL( clicked() ), this, SLOT( enableAlignReplace() ) );
	connect( replaceFontCheckBox, SIGNAL( clicked() ), this, SLOT( enableFontReplace() ) );
	connect( replaceSizeCheckBox, SIGNAL( clicked() ), this, SLOT( enableSizeReplace() ) );
	connect( replaceEffectCheckBox, SIGNAL( clicked() ), this, SLOT( enableEffReplace() ) );
	connect( replaceFillCheckBox, SIGNAL( clicked() ), this, SLOT( enableFillReplace() ) );
	connect( replaceStrokeCheckBox, SIGNAL( clicked() ), this, SLOT( enableStrokeReplace() ) );
	connect( replaceFillShadeCheckBox, SIGNAL( clicked() ), this, SLOT( enableFillSReplace() ) );
	connect( replaceStrokeShadeCheckBox, SIGNAL( clicked() ), this, SLOT( enableStrokeSReplace() ) );
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));

	//tooltips
	searchButton->setToolTip( tr( "Search for text or formatting in the current text" ) );
	replaceButton->setToolTip( tr( "Replace the searched for formatting with the replacement values" ) );
	replaceAllButton->setToolTip( tr( "Replace all found instances" ) );
	clearButton->setToolTip( tr( "Clear all search and replace options" ) );
	closeButton->setToolTip( tr( "Close search and replace" ) );

 // tab order
	setTabOrder( searchTextCheckBox, searchStyleCheckBox );
	setTabOrder( searchStyleCheckBox, searchFontCheckBox );
	setTabOrder( searchFontCheckBox, searchSizeCheckBox );
	setTabOrder( searchSizeCheckBox, searchEffectCheckBox );
	setTabOrder( searchEffectCheckBox, searchFillCheckBox );
	setTabOrder( searchFillCheckBox, searchStrokeCheckBox );
	setTabOrder( searchStrokeCheckBox, searchTextLineEdit );
	setTabOrder( searchTextLineEdit, searchStyleValue );
	setTabOrder( searchStyleValue, searchAlignValue );
	setTabOrder( searchAlignValue, searchFontValue );
	setTabOrder( searchFontValue, searchSizeSpinBox );
	setTabOrder( searchSizeSpinBox, searchStyleEffectsValue );
	setTabOrder( searchStyleEffectsValue, searchFillValue );
	setTabOrder( searchFillValue, searchStrokeValue );
	setTabOrder( searchStrokeValue, replaceTextCheckBox );
	setTabOrder( replaceTextCheckBox, replaceStyleCheckBox );
	setTabOrder( replaceStyleCheckBox, replaceFontCheckBox );
	setTabOrder( replaceFontCheckBox, replaceSizeCheckBox );
	setTabOrder( replaceSizeCheckBox, replaceEffectCheckBox );
	setTabOrder( replaceEffectCheckBox, replaceFillCheckBox );
	setTabOrder( replaceFillCheckBox, replaceStrokeCheckBox );
	setTabOrder( replaceStrokeCheckBox, replaceTextLineEdit );
	setTabOrder( replaceTextLineEdit, replaceStyleValue );
	setTabOrder( replaceStyleValue, replaceAlignValue );
	setTabOrder( replaceAlignValue, replaceFontValue );
	setTabOrder( replaceFontValue, replaceSizeSpinBox );
	setTabOrder( replaceSizeSpinBox, replaceStyleEffectsValue );
	setTabOrder( replaceStyleEffectsValue, replaceFillValue );
	setTabOrder( replaceFillValue, replaceStrokeValue );
	setTabOrder( replaceStrokeValue, wholeWordCheckBox );
	setTabOrder( wholeWordCheckBox, ignoreCaseCheckBox );
	setTabOrder( ignoreCaseCheckBox, searchButton );
	setTabOrder( searchButton, replaceButton );
	setTabOrder( replaceButton, replaceAllButton );
	setTabOrder( replaceAllButton, closeButton );

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
	if (m_itemMode)
		doSearch_itemMode();
	else if (m_doc->scMW()->CurrStED != nullptr)
		doSearch_storyEdMode();
}

void SearchReplace::doSearch_itemMode()
{
	replaceButton->setEnabled(false);
	replaceAllButton->setEnabled(false);

	m_item->itemText.deselectAll();
	m_item->HasSel = false;

	bool searchForReplace = false;
	bool rep = false;

	m_found = true;

	if ((replaceFillCheckBox->isChecked()) || (replaceStrokeCheckBox->isChecked()) || (replaceStyleCheckBox->isChecked()) || (replaceFontCheckBox->isChecked())
		|| (replaceStrokeShadeCheckBox->isChecked()) || (replaceFillShadeCheckBox->isChecked()) || (replaceSizeCheckBox->isChecked()) || (replaceTextCheckBox->isChecked())
		|| (replaceEffectCheckBox->isChecked()) || (replaceAlignCheckBox->isChecked()))
		rep = true;

	if ((searchFillCheckBox->isChecked()) || (searchStrokeCheckBox->isChecked()) || (searchStyleCheckBox->isChecked()) || (searchFontCheckBox->isChecked())
		|| (searchStrokeShadeCheckBox->isChecked()) || (searchFillShadeCheckBox->isChecked()) || (searchSizeCheckBox->isChecked()) || (searchTextCheckBox->isChecked())
		|| (searchEffectCheckBox->isChecked()) || (searchAlignCheckBox->isChecked()))
		searchForReplace = true;

	int textLen(0);
	int cursorPos = m_item->itemText.cursorPosition();
	int firstChar = doSearch_storyText(m_item->itemText, cursorPos, textLen);

	bool found = (firstChar >= 0);
	if (found)
	{
		m_item->itemText.select(firstChar, textLen);
		m_item->HasSel = true;
		m_item->itemText.setCursorPosition(firstChar + textLen);
		m_matchesFound++;
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
	if (found && searchForReplace)
	{
		if (rep)
		{
			replaceButton->setEnabled(true);
			replaceAllButton->setEnabled(true);
		}
	}
	else
	{
		m_doc->DoDrawing = true;
		m_item->update();
		replaceButton->setEnabled(false);
		replaceAllButton->setEnabled(false);
		m_item->itemText.setCursorPosition(0);
		m_found = false;
	}
}

void SearchReplace::doSearch_storyEdMode()
{
	replaceButton->setEnabled(false);
	replaceAllButton->setEnabled(false);

	bool searchForReplace = false;
	bool rep = false;

	m_found = true;

	if ((replaceFillCheckBox->isChecked()) || (replaceStrokeCheckBox->isChecked()) || (replaceStyleCheckBox->isChecked()) || (replaceFontCheckBox->isChecked())
		|| (replaceStrokeShadeCheckBox->isChecked()) || (replaceFillShadeCheckBox->isChecked()) || (replaceSizeCheckBox->isChecked()) || (replaceTextCheckBox->isChecked())
		|| (replaceEffectCheckBox->isChecked()) || (replaceAlignCheckBox->isChecked()))
		rep = true;

	if ((searchFillCheckBox->isChecked()) || (searchStrokeCheckBox->isChecked()) || (searchStyleCheckBox->isChecked()) || (searchFontCheckBox->isChecked())
		|| (searchStrokeShadeCheckBox->isChecked()) || (searchFillShadeCheckBox->isChecked()) || (searchSizeCheckBox->isChecked()) || (searchTextCheckBox->isChecked())
		|| (searchEffectCheckBox->isChecked()) || (searchAlignCheckBox->isChecked()))
		searchForReplace = true;

	if (m_doc->scMW()->CurrStED == nullptr)
		return;

	SEditor* storyTextEdit = m_doc->scMW()->CurrStED->Editor;
	if (storyTextEdit->StyledText.length() == 0)
		return;
	QTextCursor cursor = storyTextEdit->textCursor();

	int textLen(0);
	int cursorPos = cursor.position();
	int firstChar = doSearch_storyText(storyTextEdit->StyledText, cursorPos, textLen);
	int lastChar = (firstChar >= 0) ? (firstChar + textLen) : 0;

	bool found = (firstChar >= 0);
	if (found)
	{
		cursor.setPosition(firstChar);
		cursor.setPosition(lastChar, QTextCursor::KeepAnchor);
		storyTextEdit->setTextCursor(cursor);
		m_matchesFound++;
	}
	if (found && searchForReplace)
	{
		// m_doc->scMW()->CurrStED->updateProps(); FIXME
		if (rep)
		{
			replaceButton->setEnabled(true);
			replaceAllButton->setEnabled(true);
		}
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

int SearchReplace::doSearch_storyText(const StoryText& storyText, int start, int& textLen)
{
	QString fCol;
	QString sCol;
	QString sFont;
	QString sText;
	int sStyle = 0;
	int sAlign = 0;
	int sSize = 0;
	int sEff = 0;
	int sFillSh = 100;
	int sStrokeSh = 100;
	bool searchForReplace = false;
	bool rep = false;
	bool found = true;

	int maxChar = storyText.length() - 1;
	textLen = 0;

	if ((replaceFillCheckBox->isChecked()) || (replaceStrokeCheckBox->isChecked()) || (replaceStyleCheckBox->isChecked()) || (replaceFontCheckBox->isChecked())
		|| (replaceStrokeShadeCheckBox->isChecked()) || (replaceFillShadeCheckBox->isChecked()) || (replaceSizeCheckBox->isChecked()) || (replaceTextCheckBox->isChecked())
		|| (replaceEffectCheckBox->isChecked()) || (replaceAlignCheckBox->isChecked()))
		rep = true;

	if ((searchFillCheckBox->isChecked()) || (searchStrokeCheckBox->isChecked()) || (searchStyleCheckBox->isChecked()) || (searchFontCheckBox->isChecked())
		|| (searchStrokeShadeCheckBox->isChecked()) || (searchFillShadeCheckBox->isChecked()) || (searchSizeCheckBox->isChecked()) || (searchTextCheckBox->isChecked())
		|| (searchEffectCheckBox->isChecked()) || (searchAlignCheckBox->isChecked()))
		searchForReplace = true;

	if (searchTextCheckBox->isChecked())
		sText = searchTextLineEdit->text();
	if (ignoreCaseCheckBox->isChecked())
		sText = sText.toLower();
	if (searchEffectCheckBox->isChecked())
		sEff = searchStyleEffectsValue->getStyle();
	if (searchFillCheckBox->isChecked())
		fCol = searchFillValue->currentText();
	if (searchFillShadeCheckBox->isChecked())
		sFillSh = searchFillShadeValue->getValue();
	if (searchStrokeCheckBox->isChecked())
		sCol = searchStrokeValue->currentText();
	if (searchStrokeShadeCheckBox->isChecked())
		sStrokeSh = searchStrokeShadeValue->getValue();
	if (searchFontCheckBox->isChecked())
		sFont = searchFontValue->currentText();
	if (searchStyleCheckBox->isChecked())
		sStyle = searchStyleValue->currentIndex();
	if (searchAlignCheckBox->isChecked())
		sAlign = searchAlignValue->currentIndex();
	if (searchSizeCheckBox->isChecked())
		sSize = qRound(searchSizeSpinBox->value() * 10);
	if (sText.length() > 0)
		found = false;

	int firstChar = -1;
	int lastChar = storyText.length();

	if (searchTextCheckBox->isChecked())
	{
		Qt::CaseSensitivity cs = Qt::CaseSensitive;
		if (ignoreCaseCheckBox->isChecked())
			cs = Qt::CaseInsensitive;

		for (int i = start; i < m_item->itemText.length(); ++i)
		{
			i = m_item->itemText.indexOf(sText, i, cs, &textLen);
			found = (i >= 0);
			if (!found)
				break;

			if (wholeWordCheckBox->isChecked() && (i > 0) && storyText.text(i - 1).isLetterOrNumber())
				found = false;
			if (wholeWordCheckBox->isChecked())
			{
				int lastChar = qMin(i + textLen, maxChar);
				found = ((lastChar == maxChar) || !storyText.text(lastChar).isLetterOrNumber());
			}
			if (!found) continue;

			int selStart = i;
			for (int ap = 0; ap < textLen; ++ap)
			{
				const ParagraphStyle& parStyle = storyText.paragraphStyle(selStart + ap);
				const CharStyle& charStyle = storyText.charStyle(selStart + ap);
				if (searchStyleCheckBox->isChecked() && (parStyle.parent() != m_doc->paragraphStyles()[sStyle].name()))
					found = false;
				if (searchAlignCheckBox->isChecked() && (parStyle.alignment() != sAlign))
					found = false;
				if (searchFontCheckBox->isChecked() && (charStyle.font().scName() != sFont))
					found = false;
				if (searchSizeCheckBox->isChecked() && (charStyle.fontSize() != sSize))
					found = false;
				if (searchEffectCheckBox->isChecked() && ((charStyle.effects() & ScStyle_UserStyles) != sEff))
					found = false;
				if (searchFillCheckBox->isChecked() && (charStyle.fillColor() != fCol))
					found = false;
				if (searchFillShadeCheckBox->isChecked() && (charStyle.fillShade() != sFillSh))
					found = false;
				if (searchStrokeCheckBox->isChecked() && (charStyle.strokeColor() != sCol))
					found = false;
				if (searchStrokeShadeCheckBox->isChecked() && (charStyle.strokeShade() != sStrokeSh))
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
		for (int i = start; i < storyText.length(); ++i)
		{
			found = true;
			const ParagraphStyle& parStyle = storyText.paragraphStyle(i);
			const CharStyle& charStyle = storyText.charStyle(i);
			if (searchStyleCheckBox->isChecked() && (parStyle.parent() != m_doc->paragraphStyles()[sStyle].name()))
				found = false;
			if (searchAlignCheckBox->isChecked() && (parStyle.alignment() != sAlign))
				found = false;
			if (searchFontCheckBox->isChecked() && (charStyle.font().scName() != sFont))
				found = false;
			if (searchSizeCheckBox->isChecked() && (charStyle.fontSize() != sSize))
				found = false;
			if (searchEffectCheckBox->isChecked() && ((charStyle.effects() & ScStyle_UserStyles) != sEff))
				found = false;
			if (searchFillCheckBox->isChecked() && (charStyle.fillColor() != fCol))
				found = false;
			if (searchFillShadeCheckBox->isChecked() && (charStyle.fillShade() != sFillSh))
				found = false;
			if (searchStrokeCheckBox->isChecked() && (charStyle.strokeColor() != sCol))
				found = false;
			if (searchStrokeShadeCheckBox->isChecked() && (charStyle.strokeShade() != sStrokeSh))
				found = false;
			if (found && (firstChar < 0))
				firstChar = i;
			else if ((firstChar >= 0) && !found)
			{
				lastChar = i;
				break;
			}
			// When searching paragraph styles break at the end of each found paragraph
			if (searchStyleCheckBox->isChecked() && (firstChar >= 0) && storyText.text(i) == SpecialChars::PARSEP)
			{
				lastChar = i + 1;
				break;
			}
		}
		if (firstChar >= 0)
			textLen = lastChar - firstChar;
	}

	if (firstChar >= 0)
		return firstChar;

	textLen = 0;
	return -1;
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
		UndoTransaction transaction;
		if (UndoManager::undoEnabled())
			transaction = UndoManager::instance()->beginTransaction(m_item->getUName(), m_item->getUPixmap());

		if (replaceTextCheckBox->isChecked())
		{
			QString selectedText = m_item->itemText.selectedText();
			QString repl = replaceTextLineEdit->text();
			if (UndoManager::undoEnabled())
			{
				UndoObject* undoTarget = m_item->isNoteFrame() ? dynamic_cast<UndoObject*>(m_item->doc()) : dynamic_cast<UndoObject*>(m_item);
				if (selectedText.length() > 0)
				{
					auto is = new ScItemState<CharStyle>(Um::DeleteText, "", Um::IDelete);
					int selStart =  m_item->itemText.startOfSelection();
					is->set("DELETE_FRAMETEXT");
					is->set("ETEA",  QString("delete_frametext"));
					is->set("TEXT_STR", selectedText);
					is->set("START", m_item->itemText.startOfSelection());
					is->setItem(m_item->itemText.charStyle(selStart));
					if (m_item->isNoteFrame())
						is->set("noteframeName", m_item->getUName());
					UndoManager::instance()->action(undoTarget, is);
				}
				if (repl.length() > 0)
				{
					int selStart = qMin(m_item->itemText.cursorPosition(), m_item->itemText.startOfSelection());
					auto ss = new SimpleState(Um::InsertText, "", Um::ICreate);
					ss->set("INSERT_FRAMETEXT");
					ss->set("ETEA", QString("insert_frametext"));
					ss->set("TEXT_STR", repl);
					ss->set("START", selStart);
					UndoObject * undoTarget = m_item;
					if (m_item->isNoteFrame())
						ss->set("noteframeName", m_item->getUName());
					UndoManager::instance()->action(undoTarget, ss);
				}
			}
			m_item->itemText.replaceSelection(repl);
		}
		if (replaceStyleCheckBox->isChecked())
		{
			int oldMode = m_doc->appMode;
			m_doc->appMode = modeEdit;
			m_doc->itemSelection_SetNamedParagraphStyle(m_doc->paragraphStyles()[replaceStyleValue->currentIndex()].name());
			m_doc->appMode = oldMode;
		}
		if (replaceAlignCheckBox->isChecked())
		{
			int oldMode = m_doc->appMode;
			m_doc->appMode = modeEdit;
			m_doc->itemSelection_SetAlignment(replaceAlignValue->currentIndex());
			m_doc->appMode = oldMode;
		}
		if (replaceFillCheckBox->isChecked())
			m_doc->itemSelection_SetFillColor(replaceFillValue->currentText());
		if (replaceFillShadeCheckBox->isChecked())
			m_doc->itemSelection_SetFillShade(replaceFillShadeValue->getValue());
		if (replaceStrokeCheckBox->isChecked())
			m_doc->itemSelection_SetStrokeColor(replaceStrokeValue->currentText());
		if (replaceStrokeShadeCheckBox->isChecked())
			m_doc->itemSelection_SetStrokeShade(replaceStrokeShadeValue->getValue());
		if (replaceFontCheckBox->isChecked())
			m_doc->itemSelection_SetFont(replaceFontValue->currentText());
		if (replaceSizeCheckBox->isChecked())
			m_doc->itemSelection_SetFontSize(qRound(replaceSizeSpinBox->value() * 10.0));
		if (replaceEffectCheckBox->isChecked() && (m_item->itemText.hasSelection()))
		{
			int s = replaceStyleEffectsValue->getStyle() & ScStyle_UserStyles;
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

		if (transaction)
			transaction.commit();
	}
	else if (m_doc->scMW()->CurrStED != nullptr)
	{
		StoryEditor* se = m_doc->scMW()->CurrStED;
		if (replaceTextCheckBox->isChecked())
		{
			disconnect(se->Editor, SIGNAL(cursorPositionChanged()), se, SLOT(updateProps()));
			int SelStart = se->Editor->textCursor().selectionStart();
			int SelEnd = se->Editor->textCursor().selectionEnd();
//			se->Editor->insChars(RTextVal->text());
			se->Editor->textCursor().setPosition(SelStart);
			se->Editor->textCursor().setPosition(SelEnd, QTextCursor::KeepAnchor);
			se->Editor->textCursor().removeSelectedText();
//FIXME		se->Editor->setEffects(se->Editor->CurrentEffects);
			QString newText = replaceTextLineEdit->text();
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
		if (replaceStyleCheckBox->isChecked())
			se->newStyle(m_doc->paragraphStyles()[replaceStyleValue->currentIndex()].name());
		if (replaceAlignCheckBox->isChecked())
			se->newAlign(replaceAlignValue->currentIndex());
		if (replaceFillCheckBox->isChecked())
			se->newTxFill(replaceFillValue->currentIndex(), -1);
		if (replaceFillShadeCheckBox->isChecked())
			se->newTxFill(-1, replaceFillShadeValue->getValue());
		if (replaceStrokeCheckBox->isChecked())
			se->newTxStroke(replaceStrokeValue->currentIndex(), -1);
		if (replaceStrokeShadeCheckBox->isChecked())
			se->newTxStroke(-1, replaceStrokeShadeValue->getValue());
		if (replaceFontCheckBox->isChecked())
			se->newTxFont(replaceFontValue->currentText());
		if (replaceSizeCheckBox->isChecked())
			se->newTxSize(replaceSizeSpinBox->value());
		if (replaceEffectCheckBox->isChecked())
			se->newTxStyle(replaceStyleEffectsValue->getStyle());

		QTextCursor textCursor = se->Editor->textCursor();
		int selStart = textCursor.selectionStart();
		int selEnd   = textCursor.selectionEnd();
		int selPos   = qMax(selStart, selEnd);
		textCursor.setPosition(selPos);
		se->Editor->setTextCursor(textCursor);
	}
	replaceButton->setEnabled(false);
	replaceAllButton->setEnabled(false);
	doSearch();
}

int SearchReplace::firstMatchCursorPosition()
{
	return m_firstMatchPosition;
}

void SearchReplace::setSearchedText(const QString& text)
{
	if (searchTextCheckBox->isChecked())
		searchTextLineEdit->setText(text);
}

void SearchReplace::slotReplaceAll()
{
	QScopedValueRollback<bool> replaceAllRollback(m_replacingAll, true);

	if (m_itemMode)
		m_doc->DoDrawing = false;

	UndoTransaction undoTransaction;
	if (m_itemMode && UndoManager::undoEnabled())
		undoTransaction = UndoManager::instance()->beginTransaction(m_item->getUName(), m_item->getUPixmap());
	do
	{
		doReplace();
	}
	while (m_found);

	if (undoTransaction)
		undoTransaction.commit();

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

	ScMessageBox::information(this, tr("Search/Replace"), tr("Search finished. ") + tr("%n match(es) found", "", m_matchesFound));
	m_matchesFound = 0;
}

void SearchReplace::enableTxSearch()
{
	bool setter = searchTextCheckBox->isChecked();
	searchTextLineEdit->setEnabled(setter);
	wholeWordCheckBox->setEnabled(setter);
	ignoreCaseCheckBox->setEnabled(setter);
	if (setter)
		searchTextLineEdit->setFocus();
	updateSearchButtonState();
}

void SearchReplace::enableStyleSearch()
{
	searchStyleValue->setEnabled(searchStyleValue->count() ? searchStyleCheckBox->isChecked() : false);
	updateSearchButtonState();
}

void SearchReplace::enableAlignSearch()
{
	searchAlignValue->setEnabled(searchAlignCheckBox->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableFontSearch()
{
	searchFontValue->setEnabled(searchFontCheckBox->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableSizeSearch()
{
	searchSizeSpinBox->setEnabled(searchSizeCheckBox->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableEffSearch()
{
	searchStyleEffectsValue->setEnabled(searchEffectCheckBox->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableFillSearch()
{
	searchFillValue->setEnabled(searchFillCheckBox->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableFillSSearch()
{
	searchFillShadeValue->setEnabled(searchFillShadeCheckBox->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableStrokeSearch()
{
	searchStrokeValue->setEnabled(searchStrokeCheckBox->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableStrokeSSearch()
{
	searchStrokeShadeValue->setEnabled(searchStrokeShadeCheckBox->isChecked());
	updateSearchButtonState();
}

void SearchReplace::enableTxReplace()
{
	replaceTextLineEdit->setEnabled(replaceTextCheckBox->isChecked());
	if (replaceTextCheckBox->isChecked())
		replaceTextLineEdit->setFocus();
	updateReplaceButtonsState();
}

void SearchReplace::enableStyleReplace()
{
	replaceStyleValue->setEnabled(replaceStyleCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableAlignReplace()
{
	replaceAlignValue->setEnabled(replaceAlignCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableFontReplace()
{
	replaceFontValue->setEnabled(replaceFontCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableSizeReplace()
{
	replaceSizeSpinBox->setEnabled(replaceSizeCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableEffReplace()
{
	replaceStyleEffectsValue->setEnabled(replaceEffectCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableFillReplace()
{
	replaceFillValue->setEnabled(replaceFillCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableFillSReplace()
{
	replaceFillShadeValue->setEnabled(replaceFillShadeCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableStrokeReplace()
{
	replaceStrokeValue->setEnabled(replaceStrokeCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::enableStrokeSReplace()
{
	replaceStrokeShadeValue->setEnabled(replaceStrokeShadeCheckBox->isChecked());
	updateReplaceButtonsState();
}

void SearchReplace::clear()
{
	replaceEffectCheckBox->setChecked(false);
	searchAlignCheckBox->setChecked(false);
	searchEffectCheckBox->setChecked(false);
	searchFillCheckBox->setChecked(false);
	searchFillShadeCheckBox->setChecked(false);
	searchFontCheckBox->setChecked(false);
	searchSizeCheckBox->setChecked(false);
	searchStrokeCheckBox->setChecked(false);
	searchStrokeShadeCheckBox->setChecked(false);
	searchStyleCheckBox->setChecked(false);
	searchTextCheckBox->setChecked(false);
	searchTextLineEdit->setText("");
	int currentParaStyle = findParagraphStyle(m_doc, m_doc->currentStyle);
	searchStyleValue->setCurrentIndex(currentParaStyle);
	replaceAlignValue->setCurrentIndex(m_doc->currentStyle.alignment());
	setCurrentComboItem(searchFontValue, m_doc->currentStyle.charStyle().font().scName());
	setCurrentComboItem(searchFillValue, m_doc->currentStyle.charStyle().fillColor());
	setCurrentComboItem(searchStrokeValue, m_doc->currentStyle.charStyle().strokeColor());
	searchSizeSpinBox->setValue(m_doc->currentStyle.charStyle().fontSize() / 10.0);
	replaceFillCheckBox->setChecked(false);
	replaceFillShadeCheckBox->setChecked(false);
	replaceFontCheckBox->setChecked(false);
	replaceSizeCheckBox->setChecked(false);
	replaceStrokeCheckBox->setChecked(false);
	replaceStrokeShadeCheckBox->setChecked(false);
	replaceStyleCheckBox->setChecked(false);
	replaceTextCheckBox->setChecked(false);
	replaceTextLineEdit->setText("");
	replaceStyleValue->setCurrentIndex(currentParaStyle);
	replaceAlignValue->setCurrentIndex(m_doc->currentStyle.alignment());
	setCurrentComboItem(replaceFontValue, m_doc->currentStyle.charStyle().font().scName());
	setCurrentComboItem(replaceFillValue, m_doc->currentStyle.charStyle().fillColor());
	setCurrentComboItem(replaceStrokeValue, m_doc->currentStyle.charStyle().strokeColor());
	replaceSizeSpinBox->setValue(m_doc->currentStyle.charStyle().fontSize() / 10.0);
	wholeWordCheckBox->setChecked(false);
	ignoreCaseCheckBox->setChecked(false);
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
	if (replaceFillCheckBox->isChecked() || replaceStrokeCheckBox->isChecked() || replaceStyleCheckBox->isChecked() || replaceFontCheckBox->isChecked()  ||
		replaceStrokeShadeCheckBox->isChecked() || replaceFillShadeCheckBox->isChecked() || replaceSizeCheckBox->isChecked() || replaceEffectCheckBox->isChecked() ||
		replaceAlignCheckBox->isChecked())
	{
		replaceEnabled = true;
	}
	replaceEnabled |= replaceTextCheckBox->isChecked();
	if (m_itemMode)
		replaceEnabled &= (m_item->itemText.hasSelection());
	else if (m_doc->scMW()->CurrStED != nullptr)
		replaceEnabled &= m_doc->scMW()->CurrStED->Editor->textCursor().hasSelection();
	else
		replaceEnabled = false;
	replaceEnabled &= m_found;
	replaceButton->setEnabled(replaceEnabled);
	replaceAllButton->setEnabled(replaceEnabled);
}

void SearchReplace::updateSearchButtonState()
{
	bool searchEnabled = false;
	if (searchFillCheckBox->isChecked() || searchStrokeCheckBox->isChecked() || searchStyleCheckBox->isChecked() || searchFontCheckBox->isChecked() ||
		searchStrokeShadeCheckBox->isChecked() || searchFillShadeCheckBox->isChecked() || searchSizeCheckBox->isChecked() || searchEffectCheckBox->isChecked() ||
		searchAlignCheckBox->isChecked())
	{
		searchEnabled = true;
	}
	searchEnabled |= (searchTextCheckBox->isChecked() && !searchTextLineEdit->text().isEmpty());
	searchButton->setEnabled(searchEnabled);
}

void SearchReplace::readPrefs()
{
	searchAlignCheckBox->setChecked(m_prefs->getBool("SAlign", false));
	searchEffectCheckBox->setChecked(m_prefs->getBool("SEffect", false));
	searchFillCheckBox->setChecked(m_prefs->getBool("SFill", false));
	searchFillShadeCheckBox->setChecked(m_prefs->getBool("SFillS", false));
	searchFontCheckBox->setChecked(m_prefs->getBool("SFont", false));
	searchSizeCheckBox->setChecked(m_prefs->getBool("SSize", false));
	searchStrokeCheckBox->setChecked(m_prefs->getBool("SStroke", false));
	searchStrokeShadeCheckBox->setChecked(m_prefs->getBool("SStrokeS", false));
	searchStyleCheckBox->setChecked(m_prefs->getBool("SStyle", false));
	searchTextCheckBox->setChecked(m_prefs->getBool("SText", true));
	replaceEffectCheckBox->setChecked(m_prefs->getBool("REffect", false));
	searchTextLineEdit->setText(m_prefs->get("STextVal", ""));
	int tmp = m_prefs->getInt("SStyleVal", findParagraphStyle(m_doc, m_doc->currentStyle.parent()));
	if (tmp < 0 || tmp >= searchStyleValue->count())
		tmp = 0;
	searchStyleValue->setCurrentIndex(tmp);
	tmp = m_prefs->getInt("SAlignVal", m_doc->currentStyle.alignment());
	if (tmp < 0 || tmp >= searchAlignValue->count())
		tmp = 0;
	searchAlignValue->setCurrentIndex(tmp);
	setCurrentComboItem(searchFontValue, m_prefs->get("SFontVal", m_doc->currentStyle.charStyle().font().scName()));
	setCurrentComboItem(searchFillValue, m_prefs->get("SFillVal", m_doc->currentStyle.charStyle().fillColor()));
	setCurrentComboItem(searchStrokeValue, m_prefs->get("SStrokeVal", m_doc->currentStyle.charStyle().strokeColor()));
	searchSizeSpinBox->setValue(m_prefs->getDouble("SSizeVal", m_doc->currentStyle.charStyle().fontSize() / 10.0));
	replaceAlignCheckBox->setChecked(m_prefs->getBool("RAlign", false));
	replaceFillCheckBox->setChecked(m_prefs->getBool("RFill", false));
	replaceFillShadeCheckBox->setChecked(m_prefs->getBool("RFillS", false));
	replaceFontCheckBox->setChecked(m_prefs->getBool("RFont", false));
	replaceSizeCheckBox->setChecked(m_prefs->getBool("RSize", false));
	replaceStrokeCheckBox->setChecked(m_prefs->getBool("RStroke", false));
	replaceStrokeShadeCheckBox->setChecked(m_prefs->getBool("RStrokeS", false));
	replaceStyleCheckBox->setChecked(m_prefs->getBool("RStyle", false));
	replaceTextCheckBox->setChecked(m_prefs->getBool("RText", true));
	replaceTextLineEdit->setText(m_prefs->get("RTextVal", ""));
	tmp = m_prefs->getInt("RStyleVal", findParagraphStyle(m_doc, m_doc->currentStyle.parent()));
	if (tmp < 0 || tmp >= replaceStyleValue->count())
		tmp = 0;
	replaceStyleValue->setCurrentIndex(tmp);
	tmp = m_prefs->getInt("RAlignVal", m_doc->currentStyle.alignment());
	if (tmp < 0 || tmp >= replaceAlignValue->count())
		tmp = 0;
	replaceAlignValue->setCurrentIndex(tmp);
	setCurrentComboItem(replaceFontValue, m_prefs->get("RFontVal", m_doc->currentStyle.charStyle().font().scName()));
	setCurrentComboItem(replaceFillValue, m_prefs->get("RFillVal", m_doc->currentStyle.charStyle().fillColor()));
	setCurrentComboItem(replaceStrokeValue, m_prefs->get("RStrokeVal", m_doc->currentStyle.charStyle().strokeColor()));
	replaceSizeSpinBox->setValue(m_prefs->getDouble("RSizeVal", m_doc->currentStyle.charStyle().fontSize() / 10.0));
	wholeWordCheckBox->setChecked(m_prefs->getBool("Word", false));
	ignoreCaseCheckBox->setChecked(m_prefs->getBool("CaseIgnore", false));

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

	if (searchTextCheckBox->isChecked() && replaceTextCheckBox->isChecked())
		searchTextLineEdit->setFocus();
}

void SearchReplace::writePrefs()
{
	m_prefs->set("CaseIgnore", ignoreCaseCheckBox->isChecked());
	m_prefs->set("RAlign", replaceAlignCheckBox->isChecked());
	m_prefs->set("RAlignVal", replaceAlignValue->currentIndex());
	m_prefs->set("REffect", replaceEffectCheckBox->isChecked());
	m_prefs->set("RFill", replaceFillCheckBox->isChecked());
	m_prefs->set("RFillS", replaceFillShadeCheckBox->isChecked());
	m_prefs->set("RFillVal", replaceFillValue->currentText());
	m_prefs->set("RFont", replaceFontCheckBox->isChecked());
	m_prefs->set("RFontVal", replaceFontValue->currentText());
	m_prefs->set("RSize", replaceSizeCheckBox->isChecked());
	m_prefs->set("RSizeVal", replaceSizeSpinBox->value());
	m_prefs->set("RStroke", replaceStrokeCheckBox->isChecked());
	m_prefs->set("RStrokeS", replaceStrokeShadeCheckBox->isChecked());
	m_prefs->set("RStrokeVal", replaceStrokeValue->currentText());
	m_prefs->set("RStyle", replaceStyleCheckBox->isChecked());
	m_prefs->set("RStyleVal", replaceStyleValue->currentText());
	m_prefs->set("RText", replaceTextCheckBox->isChecked());
	m_prefs->set("RTextVal", replaceTextLineEdit->text());
	m_prefs->set("SAlign", searchAlignCheckBox->isChecked());
	m_prefs->set("SAlignVal", searchAlignValue->currentIndex());
	m_prefs->set("SEffect", searchEffectCheckBox->isChecked());
	m_prefs->set("SFill", searchFillCheckBox->isChecked());
	m_prefs->set("SFillS", searchFillShadeCheckBox->isChecked());
	m_prefs->set("SFillVal", searchFillValue->currentText());
	m_prefs->set("SFont", searchFontCheckBox->isChecked());
	m_prefs->set("SFontVal", searchFontValue->currentText());
	m_prefs->set("SSize", searchSizeCheckBox->isChecked());
	m_prefs->set("SSizeVal", searchSizeSpinBox->value());
	m_prefs->set("SStroke", searchStrokeCheckBox->isChecked());
	m_prefs->set("SStrokeS", searchStrokeShadeCheckBox->isChecked());
	m_prefs->set("SStrokeVal", searchStrokeValue->currentText());
	m_prefs->set("SStyle", searchStyleCheckBox->isChecked());
	m_prefs->set("SStyleVal", searchStyleValue->currentIndex());
	m_prefs->set("SText", searchTextCheckBox->isChecked());
	m_prefs->set("STextVal", searchTextLineEdit->text());
	m_prefs->set("Word", wholeWordCheckBox->isChecked());
	accept();
}
