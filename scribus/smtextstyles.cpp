/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smtextstyles.h"
#include "smtextstyles.moc"
#include "util.h"
#include "mspinbox.h"
#include "alignselect.h"
#include "tabruler.h"
#include "fontcombo.h"
#include "styleselect.h"
#include "sccombobox.h"
#include "shadebutton.h"
#include "commonstrings.h"
#include "style.h"
#include "scribusdoc.h"
#include "selection.h"

#include <qgroupbox.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qpopupmenu.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qframe.h>
#include <qpixmap.h>
#include <qtabwidget.h>

SMPStyleWidget::SMPStyleWidget()
{
	setupDistances();
	setupDropCaps();
	setupTabs();
	setupCharStyle();
}

void SMPStyleWidget::setupDistances()
{
	distancesBox->setColumnLayout(0, Qt::Vertical );
	distancesBox->layout()->setSpacing( 0 );
	distancesBox->layout()->setMargin( 0 );
	distancesBoxLayout = new QGridLayout( distancesBox->layout() );
	distancesBoxLayout->setAlignment( Qt::AlignTop );
	distancesBoxLayout->setSpacing( 5 );
	distancesBoxLayout->setMargin( 10 );

	lineSpacingMode_ = new QComboBox(distancesBox, "linespacingCombo");
	lineSpacingMode_->insertItem(tr("Fixed Linespacing"));
	lineSpacingMode_->insertItem(tr("Automatic Linespacing"));
	lineSpacingMode_->insertItem(tr("Align to Baseline Grid"));
	distancesBoxLayout->addMultiCellWidget(lineSpacingMode_, 0, 0, 1, 3);

	pixmapLabel0 = new QLabel(distancesBox, "pixmapLabel0");
	pixmapLabel0->setMinimumSize(QSize(22,22));
	pixmapLabel0->setMaximumSize(QSize(22,22));
	pixmapLabel0->setPixmap(loadIcon("linespacing2.png"));
	distancesBoxLayout->addWidget(pixmapLabel0, 1, 0);

	lineSpacing_ = new MSpinBox(1, 300, distancesBox, 1 );
	lineSpacing_->setSuffix(tr( " pt" ));
	distancesBoxLayout->addWidget(lineSpacing_, 1,1);

// 	pixmapLabel1 = new QLabel(distancesBox, "pixmapLabel2" );
// 	pixmapLabel1->setMinimumSize(QSize(22, 22));
// 	pixmapLabel1->setMaximumSize(QSize(22, 22));
// 	pixmapLabel1->setPixmap(loadIcon("textbase.png"));
// 	distancesBoxLayout->addWidget(pixmapLabel1, 2, 0);
// 
// 	fontBase = new MSpinBox( -100, 100, distancesBox, 1 );
// 	fontBase->setSuffix( tr( " %" ) );
// 	distancesBoxLayout->addWidget( fontBase, 2, 1 );

	pixmapLabel3 = new QLabel( "", distancesBox, "TextLabel1_2_2" );
	pixmapLabel3->setPixmap( loadIcon("above.png") );
	distancesBoxLayout->addWidget( pixmapLabel3, 1, 2 );

	spaceAbove_ = new MSpinBox( 0, 300, distancesBox, 1 );
	spaceAbove_->setSuffix( tr( " pt" ) );
	distancesBoxLayout->addWidget( spaceAbove_, 1, 3 );

	pixmapLabel4 = new QLabel( "", distancesBox, "TextLabel1_2_3" );
	pixmapLabel4->setPixmap( loadIcon("below.png") );
	distancesBoxLayout->addWidget( pixmapLabel4, 2, 2 );

	spaceBelow_ = new MSpinBox( 0, 300, distancesBox, 1 );
	spaceBelow_->setSuffix( tr( " pt" ) );
	distancesBoxLayout->addWidget( spaceBelow_, 2, 3 );

	alignement_ = new AlignSelect(distancesBox);
	distancesBoxLayout->addMultiCellWidget(alignement_, 3,3,1,3);
}

void SMPStyleWidget::setupDropCaps()
{
	dropCapsBox->setCheckable( true );
	dropCapsBox->setColumnLayout(0, Qt::Vertical );
	dropCapsBox->layout()->setSpacing( 5 );
	dropCapsBox->layout()->setMargin( 10 );

	dropCapsBoxLayout = new QGridLayout(dropCapsBox->layout());
	dropCapsBoxLayout->setAlignment(Qt::AlignTop);

	dropCapLines_ = new QSpinBox(dropCapsBox, "DropLines");
	dropCapLines_->setMinValue(2);
	dropCapLines_->setMaxValue(20);
	capLabel1 = new QLabel(dropCapLines_, tr("&Lines:"), dropCapsBox, "CapLabel");
	dropCapsBoxLayout->addWidget(capLabel1, 0, 0);
	dropCapsBoxLayout->addWidget(dropCapLines_, 0, 1);

	dropCapOffset_ = new MSpinBox(-3000, 3000, dropCapsBox, 1);
	dropCapOffset_->setSuffix(tr(" pt"));
	capLabel2 = new QLabel(dropCapLines_, tr("Distance from Text:"), dropCapsBox, "CapLabel2");
	dropCapsBoxLayout->addWidget(capLabel2, 1, 0);
	dropCapsBoxLayout->addWidget(dropCapOffset_, 1, 1);
}

void SMPStyleWidget::setupTabs()
{
	tabsBox->setColumnLayout(0, Qt::Vertical );
	tabsBox->layout()->setSpacing( 0 );
	tabsBox->layout()->setMargin( 0 );

	tabsBoxLayout = new QVBoxLayout(tabsBox->layout());
	tabsBoxLayout->setAlignment( Qt::AlignTop );
	tabsBoxLayout->setSpacing( 5 );
	tabsBoxLayout->setMargin( 10 );

	tabList_ = new Tabruler(tabsBox);
	tabsBoxLayout->addWidget( tabList_ );
}

void SMPStyleWidget::setupCharStyle()
{
	characterBox->setColumns(3);
	characterBox->setInsideMargin(5);
	characterBox->setInsideSpacing(5);
	cpage = new SMCStylePage(characterBox);
}

void SMPStyleWidget::show(ParagraphStyle &pstyle)
{
	lineSpacingMode_->setCurrentItem(pstyle.lineSpacingMode());
	lineSpacing_->setValue(pstyle.lineSpacing());
	alignement_->setStyle(pstyle.alignment());
	tabList_->setFirstLineData(pstyle.firstIndent());
	tabList_->setLeftIndentData(pstyle.leftMargin());
	tabList_->setRightIndentData(pstyle.rightMargin());
	spaceAbove_->setValue(pstyle.gapBefore());
	spaceBelow_->setValue(pstyle.gapAfter());
	dropCapsBox->setChecked(pstyle.hasDropCap());
	dropCapLines_->setValue(pstyle.dropCapLines());
	dropCapOffset_->setValue(pstyle.dropCapOffset());

//  ASK Avox!
// 	bool useBaselineGrid() const { return BaseAdj==NOVALUE && parent()? inh().useBaselineGrid() : BaseAdj > 0; }

	cpage->show(pstyle.charStyle());
}

SMPStyleWidget::~SMPStyleWidget()
{
	
}

/******************************************************************************/
/******************************************************************************/

SMParagraphStyle::SMParagraphStyle() : StyleItem(), pwidget_(0), doc_(NULL)
{

}

QTabWidget* SMParagraphStyle::widget()
{
	if (!pwidget_)
	{
		pwidget_ = new SMPStyleWidget();
		Q_CHECK_PTR(pwidget_);
	}

	return pwidget_->tabWidget;
}

QString SMParagraphStyle::typeName()
{
	return tr("Paragraph Styles");
}

void SMParagraphStyle::currentDoc(ScribusDoc *doc)
{
	Q_ASSERT(doc);
	doc_ = doc;
}

QValueList<StyleName> SMParagraphStyle::styles()
{
	QValueList<StyleName> tmpList;

	if (!doc_)
		return tmpList; // no doc available

	StyleSet<ParagraphStyle> &tmp = doc_->docParagraphStyles;
	for (uint i = 0; i < tmp.count(); ++i)
	{
		if (tmp[i].hasName())
		{
			QString styleName = tmp[i].name();
			QString parentName = QString::null;

			if (tmp[i].hasParent() && tmp[i].parent()->hasName())
				parentName = tmp[i].parent()->displayName();

			tmpList << StyleName(styleName, parentName);
		}
	}

	return tmpList;
}

void SMParagraphStyle::selected(const QStringList &styleNames)
{
	if (styleNames.count() == 1)
	{
		int index = doc_->docParagraphStyles.find(styleNames[0]);
		if (index > -1)
			pwidget_->show(doc_->docParagraphStyles[index]);
	}
	else // more than one item selected do the magic tricks here
	{
		
	}
}

QString SMParagraphStyle::fromSelection() const
{
//	Q_ASSERT(doc_ && doc_->m_Selection);
//	QString lsName = QString::null;

//	for (uint i = 0; i < doc_->m_Selection->count(); ++i)
//	{
//		PageItem *item = doc_->m_Selection->itemAt(i);
//	}
	return QString::null;
}

void SMParagraphStyle::apply()
{

}

void SMParagraphStyle::deleteStyles(const QValueList<RemoveItem> &removeList)
{

}

void SMParagraphStyle::nameChanged(const QString &newName)
{

}

SMParagraphStyle::~SMParagraphStyle()
{

}

/******************************************************************************/
/******************************************************************************/

SMCStylePage::SMCStylePage(QWidget *parent) : CStylePBase(parent)
{
	characterBoxLayout = new QVBoxLayout(characterBox);
	characterBoxLayout->setAlignment( Qt::AlignLeft );
	characterBoxLayout->setSpacing( 5 );
	characterBoxLayout->setMargin( 10 );

	fontFace_ = new FontComboH(characterBox);
	characterBoxLayout->addWidget( fontFace_ );

	characterBoxLayout->addSpacing( 10 );

	layout7 = new QHBoxLayout( 0, 0, 5, "layout7");
	fontSize_ = new MSpinBox( 1, 2048, characterBox, 1 );
	fontSize_->setMinimumSize( QSize( 70, 22 ) );
	fontSize_->setSuffix( tr( " pt" ) );

	TextF2 = new QLabel( "" ,characterBox, "TextF2" );
	TextF2->setPixmap(loadIcon("Zeichen.xpm"));
	TextF2->setMinimumSize( QSize( 22, 22 ) );
	TextF2->setMaximumSize( QSize( 22, 22 ) );
	layout7->addWidget( TextF2 );
	layout7->addWidget( fontSize_ );

	pixmapLabel3_3 = new QLabel( characterBox, "pixmapLabel3_3" );
	pixmapLabel3_3->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_3->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_3->setPixmap( loadIcon("textkern.png") );
	layout7->addWidget(pixmapLabel3_3);

	tracking_ = new MSpinBox( -300, 300, characterBox, 1 );
	tracking_->setSuffix( tr( " %" ) );
	layout7->addWidget(tracking_);
	

	pixmapLabel2 = new QLabel( characterBox, "pixmapLabel2" );
	pixmapLabel2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel2->setPixmap( loadIcon("textbase.png") );
	layout7->addWidget( pixmapLabel2 );
	baselineOffset_ = new MSpinBox( -100, 100, characterBox, 1 );
	baselineOffset_->setSuffix( tr( " %" ) );
	layout7->addWidget( baselineOffset_ );
	layout7->addStretch(10);
	characterBoxLayout->addLayout( layout7 );

	layout8 = new QHBoxLayout(0, 0, 5, "layout8");

	pixmapLabel3 = new QLabel( "", characterBox, "pixmapLabel3" );
	pixmapLabel3->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3->setPixmap( loadIcon("textscaleh.png") );
	layout8->addWidget( pixmapLabel3 );

	fontHScale_ = new MSpinBox( 10, 400, characterBox, 1 );
	fontHScale_->setSuffix( tr( " %" ) );
	layout8->addWidget( fontHScale_ );

	pixmapLabel3_2 = new QLabel( "", characterBox, "pixmapLabel3_2" );
	pixmapLabel3_2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setPixmap( loadIcon("textscalev.png") );
	layout8->addWidget( pixmapLabel3_2 );

	fontVScale_ = new MSpinBox( 10, 400, characterBox, 1 );
	fontVScale_->setSuffix( tr( " %" ) );
	layout8->addWidget( fontVScale_ );
	layout8->addStretch(10);
	characterBoxLayout->addLayout( layout8 );

	characterBoxLayout->addSpacing( 10 );

	layout9a = new QHBoxLayout( 0, 0, 0, "layout9");
	effects_ = new StyleSelect(characterBox);
	layout9a->addWidget( effects_ );

	spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9a->addItem( spacer1 );
	characterBoxLayout->addLayout( layout9a, Qt::AlignLeft );

	characterBoxLayout->addSpacing( 10 );

	layout5 = new QHBoxLayout( 0, 0, 5, "layout5");
	FillIcon = new QLabel( "", characterBox, "FillIcon" );
	FillIcon->setPixmap(loadIcon("fill.png"));
	layout5->addWidget( FillIcon );

	fillColor_ = new ScComboBox( false, characterBox, "TxFill" );
	layout5->addWidget( fillColor_ );

	pixmapLabel3_20 = new QLabel( characterBox, "pixmapLabel3_20" );
	pixmapLabel3_20->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_20->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_20->setPixmap( loadIcon("shade.png") );
	layout5->addWidget( pixmapLabel3_20 );

	fillShade_ = new ShadeButton(characterBox);
	layout5->addWidget( fillShade_ );
	QSpacerItem* spacer3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout5->addItem( spacer3 );
	characterBoxLayout->addLayout( layout5 );

	layout6 = new QHBoxLayout( 0, 0, 5, "layout6");
	StrokeIcon = new QLabel( "", characterBox, "StrokeIcon" );
	StrokeIcon->setPixmap(loadIcon("Stiftalt.xpm"));
	layout6->addWidget( StrokeIcon );

	strokeColor_ = new ScComboBox( false, characterBox, "TxStroke" );
	layout6->addWidget( strokeColor_ );

	pixmapLabel3_19 = new QLabel( "", characterBox, "pixmapLabel3_19" );
	pixmapLabel3_19->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_19->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_19->setPixmap( loadIcon("shade.png") );
	layout6->addWidget( pixmapLabel3_19 );

	strokeShade_ = new ShadeButton(characterBox);
	layout6->addWidget( strokeShade_ );

	spacer4 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout6->addItem( spacer4 );
	characterBoxLayout->addLayout( layout6 );

	characterBoxLayout->addStretch(10);

	fillColor_->clear();
	strokeColor_->clear();
	ColorList::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	fillColor_->insertItem(CommonStrings::NoneColor);
	strokeColor_->insertItem(CommonStrings::NoneColor);
	StrokeIcon->setEnabled(false);
	strokeShade_->setEnabled(false);
	strokeColor_->setEnabled(false);
}

void SMCStylePage::show(CharStyle &cstyle)
{
	// ASK Avox!
	fontSize_->setValue(cstyle.fontSize() / 10.0);
	fillShade_->setValue(cstyle.fillShade());
	strokeShade_->setValue(cstyle.strokeShade());
	effects_->setStyle(static_cast<int>(cstyle.effects()));
	effects_->ShadowVal->Xoffset->setValue(cstyle.shadowXOffset());
	effects_->ShadowVal->Yoffset->setValue(cstyle.shadowYOffset());
	effects_->OutlineVal->LWidth->setValue(cstyle.outlineWidth());
	effects_->StrikeVal->LPos->setValue(cstyle.strikethruOffset());
	effects_->StrikeVal->LWidth->setValue(cstyle.strikethruWidth());
	effects_->UnderlineVal->LPos->setValue(cstyle.underlineOffset());
	effects_->UnderlineVal->LWidth->setValue(cstyle.underlineWidth());
	fontHScale_->setValue(cstyle.scaleH());
	fontVScale_->setValue(cstyle.scaleV());
	baselineOffset_->setValue(cstyle.baselineOffset());
	tracking_->setValue(cstyle.tracking());
	fillColor_->setCurrentText(cstyle.fillColor());
	strokeColor_->setCurrentText(cstyle.strokeColor());
	fontFace_->setCurrentFont(cstyle.font().scName());
// 	QString language() const { return language_==NOLANG && parent()? inh().language() : language_; }
	
}

SMCStylePage::~SMCStylePage()
{
	
}

/******************************************************************************/
/******************************************************************************/

SMCharacterStyle::SMCharacterStyle() : StyleItem(), widget_(0), page_(0), doc_(0)
{

}

QTabWidget* SMCharacterStyle::widget()
{
	if (!widget_)
	{
		widget_ = new QTabWidget();
		Q_CHECK_PTR(widget_);
		page_ = new SMCStylePage();
		Q_CHECK_PTR(page_);
		widget_->addTab(page_, tr("Properties"));
	}

	return widget_;
}

QString SMCharacterStyle::typeName()
{
	return tr("Character Styles");
}

void SMCharacterStyle::currentDoc(ScribusDoc *doc)
{
	Q_ASSERT(doc);
	doc_ = doc;
}

QValueList<StyleName> SMCharacterStyle::styles()
{
	QValueList<StyleName> tmpList;

	if (!doc_)
		return tmpList; // no doc available

	StyleSet<CharStyle> &tmp = doc_->docCharStyles;
	for (uint i = 0; i < tmp.count(); ++i)
	{
		if (tmp[i].hasName())
		{
			QString styleName = tmp[i].name();
			QString parentName = QString::null;

			if (tmp[i].hasParent() && tmp[i].parent()->hasName())
				parentName = tmp[i].parent()->displayName();

			tmpList << StyleName(styleName, parentName);
		}
	}

	return tmpList;
}

void SMCharacterStyle::selected(const QStringList &styleNames)
{
	if (styleNames.count() == 1)
	{
		int index = doc_->docCharStyles.find(styleNames[0]);
		if (index > -1)
			page_->show(doc_->docCharStyles[index]);
	}
	else // more than one item selected do the magic tricks here
	{
		
	}
}

QString SMCharacterStyle::fromSelection() const
{
	return QString::null;
}

void SMCharacterStyle::apply()
{

}

void SMCharacterStyle::deleteStyles(const QValueList<RemoveItem> &removeList)
{

}

void SMCharacterStyle::nameChanged(const QString &newName)
{

}

SMCharacterStyle::~SMCharacterStyle()
{
	delete page_;
	delete widget_;
	page_ = 0;
	widget_ = 0;
}

