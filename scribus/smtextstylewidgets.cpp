/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smtextstylewidgets.h"
#include "smtextstylewidgets.moc"
#include "util.h"
#include "commonstrings.h"
#include "smwidgets.h"

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
#include <qmap.h>

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

	lineSpacingMode_ = new SMScComboBox(distancesBox, "linespacingCombo");
	lineSpacingMode_->insertItem(tr("Fixed Linespacing"));
	lineSpacingMode_->insertItem(tr("Automatic Linespacing"));
	lineSpacingMode_->insertItem(tr("Align to Baseline Grid"));
	distancesBoxLayout->addMultiCellWidget(lineSpacingMode_, 0, 0, 1, 3);
	connect(lineSpacingMode_, SIGNAL(highlighted(int)), this, SLOT(slotLineSpacingModeChanged(int)));

	pixmapLabel0 = new QLabel(distancesBox, "pixmapLabel0");
	pixmapLabel0->setMinimumSize(QSize(22,22));
	pixmapLabel0->setMaximumSize(QSize(22,22));
	pixmapLabel0->setPixmap(loadIcon("linespacing2.png"));
	distancesBoxLayout->addWidget(pixmapLabel0, 1, 0);

	lineSpacing_ = new SMMSpinBox(1, 300, distancesBox, 1 );
	lineSpacing_->setSuffix(tr( " pt" ));
	distancesBoxLayout->addWidget(lineSpacing_, 1,1);

	pixmapLabel3 = new QLabel( "", distancesBox, "TextLabel1_2_2" );
	pixmapLabel3->setPixmap( loadIcon("above.png") );
	distancesBoxLayout->addWidget( pixmapLabel3, 1, 2 );

	spaceAbove_ = new SMMSpinBox( 0, 300, distancesBox, 1 );
	spaceAbove_->setSuffix( tr( " pt" ) );
	distancesBoxLayout->addWidget( spaceAbove_, 1, 3 );

	pixmapLabel4 = new QLabel( "", distancesBox, "TextLabel1_2_3" );
	pixmapLabel4->setPixmap( loadIcon("below.png") );
	distancesBoxLayout->addWidget( pixmapLabel4, 2, 2 );

	spaceBelow_ = new SMMSpinBox( 0, 300, distancesBox, 1 );
	spaceBelow_->setSuffix( tr( " pt" ) );
	distancesBoxLayout->addWidget( spaceBelow_, 2, 3 );

	alignement_ = new SMAlignSelect(distancesBox);
	distancesBoxLayout->addMultiCellWidget(alignement_, 3,3,1,3);
}

void SMPStyleWidget::slotLineSpacingModeChanged(int i)
{
	if (i == 0)
		lineSpacing_->setEnabled(true);
	else
		lineSpacing_->setEnabled(false);
}

void SMPStyleWidget::languageChange()
{
	lineSpacingMode_->clear();
	lineSpacingMode_->insertItem(tr("Fixed Linespacing"));
	lineSpacingMode_->insertItem(tr("Automatic Linespacing"));
	lineSpacingMode_->insertItem(tr("Align to Baseline Grid"));
	lineSpacing_->setSuffix(tr(" pt"));
	spaceAbove_->setSuffix(tr(" pt"));
	spaceBelow_->setSuffix(tr(" pt"));
	parentLabel->setText(tr("Parent"));
	distancesBox->setTitle(tr("Distances and Alignment"));
	dropCapsBox->setTitle(tr("Drop Caps"));
	tabsBox->setTitle(tr("Tabulators and Indentation"));
	tabWidget->setTabLabel(tabWidget->page(0), tr("Properties"));
	tabWidget->setTabLabel(tabWidget->page(1), tr("Character Style"));
}

void SMPStyleWidget::setupDropCaps()
{
	dropCapsBox->setCheckable( true );
	dropCapsBox->setColumnLayout(0, Qt::Vertical );
	dropCapsBox->layout()->setSpacing( 5 );
	dropCapsBox->layout()->setMargin( 10 );

	dropCapsBoxLayout = new QGridLayout(dropCapsBox->layout());
	dropCapsBoxLayout->setAlignment(Qt::AlignTop);

	dropCapLines_ = new SMSpinBox(dropCapsBox, "DropLines");
	dropCapLines_->setMinValue(2);
	dropCapLines_->setMaxValue(20);
	capLabel1 = new QLabel(dropCapLines_, tr("&Lines:"), dropCapsBox, "CapLabel");
	dropCapsBoxLayout->addWidget(capLabel1, 0, 0);
	dropCapsBoxLayout->addWidget(dropCapLines_, 0, 1);

	dropCapOffset_ = new SMMSpinBox(-3000, 3000, dropCapsBox, 1);
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

	tabList_ = new SMTabruler(tabsBox);
	tabsBoxLayout->addWidget( tabList_ );
}

void SMPStyleWidget::setupCharStyle()
{
	QHBoxLayout *la = new QHBoxLayout(characterBox, 0, 0, "la");
	cpage = new SMCStylePage(characterBox);
	la->addWidget(cpage);
	characterBox->setEnabled(true);
}

void SMPStyleWidget::show(ParagraphStyle &pstyle, QValueList<ParagraphStyle> &pstyles, QValueList<CharStyle> &cstyles, int unitIndex)
{
	bool hasParent = pstyle.hasParent();
	const ParagraphStyle *parent = dynamic_cast<const ParagraphStyle*>(pstyle.parent());

	lineSpacingMode_->clear();
	lineSpacingMode_->insertItem(tr("Fixed Linespacing"));
	lineSpacingMode_->insertItem(tr("Automatic Linespacing"));
	lineSpacingMode_->insertItem(tr("Align to Baseline Grid"));

	hasParent_ = hasParent;

	if (hasParent)
	{
		lineSpacingMode_->setCurrentItem(pstyle.lineSpacingMode(), pstyle.isPlineSpacingMode());
		lineSpacingMode_->setParentItem(parent->lineSpacingMode());

		lineSpacing_->setValue(pstyle.lineSpacing(), pstyle.isPlineSpacing());
		lineSpacing_->setParentValue(parent->lineSpacing());

		spaceAbove_->setValue(pstyle.gapBefore(), pstyle.isPgapBefore());
		spaceAbove_->setParentValue(parent->gapBefore());

		spaceBelow_->setValue(pstyle.gapAfter(), pstyle.isPgapAfter());
		spaceBelow_->setParentValue(parent->gapAfter());

		dropCapsBox->setChecked(pstyle.hasDropCap());;
		parentDropCap_ = parent->hasDropCap();
		if (pstyle.isPhasDropCap())
			parentDropCapButton->hide();
		else
			parentDropCapButton->show();

		connect(parentDropCapButton, SIGNAL(clicked()), this, SLOT(slotParentDropCap()));
		connect(dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap()));

		dropCapLines_->setValue(pstyle.dropCapLines(), pstyle.isPdropCapLines());
		dropCapLines_->setParentValue(parent->dropCapLines());

		dropCapOffset_->setValue(pstyle.dropCapOffset(), pstyle.isPdropCapOffset());
		dropCapOffset_->setParentValue(parent->dropCapOffset());

		alignement_->setStyle(pstyle.alignment(), pstyle.isPalignment());
		alignement_->setParentItem(parent->alignment());

		bool hasParentTabs = pstyle.isPtabValues();
		QValueList<ParagraphStyle::TabRecord> tabs;
		if (hasParentTabs)
			tabs = QValueList<ParagraphStyle::TabRecord>(parent->tabValues());
		else
			tabs = pstyle.tabValues();

		tabList_->setTabs(tabs, unitIndex, hasParentTabs);
		tabList_->setParentTabs(parent->tabValues());

		tabList_->setLeftIndentValue(pstyle.leftMargin(),pstyle.isPleftMargin());
		tabList_->setParentLeftIndent(parent->leftMargin());

		tabList_->setFirstLineValue(pstyle.firstIndent(), pstyle.isPfirstIndent());
		tabList_->setParentFirstLine(parent->firstIndent());

		tabList_->setRightIndentValue(pstyle.rightMargin(), pstyle.isPrightMargin());
		tabList_->setParentRightIndent(parent->rightMargin());
	}
	else
	{
		lineSpacingMode_->setCurrentItem(pstyle.lineSpacingMode());
		lineSpacing_->setValue(pstyle.lineSpacing());
		spaceAbove_->setValue(pstyle.gapBefore());
		spaceBelow_->setValue(pstyle.gapAfter());
		dropCapsBox->setChecked(pstyle.hasDropCap());
		parentDropCapButton->hide();
		disconnect(parentDropCapButton, SIGNAL(clicked()), this, SLOT(slotParentDropCap()));
		disconnect(dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap()));
		dropCapLines_->setValue(pstyle.dropCapLines());
		dropCapOffset_->setValue(pstyle.dropCapOffset());
		parentDropCapButton->hide();
		alignement_->setStyle(pstyle.alignment());
		tabList_->setTabs(pstyle.tabValues(), unitIndex);
		tabList_->setLeftIndentValue(pstyle.leftMargin());
		tabList_->setFirstLineValue(pstyle.firstIndent());
		tabList_->setRightIndentValue(pstyle.rightMargin());
	}

	lineSpacing_->setEnabled(pstyle.lineSpacingMode() == ParagraphStyle::FixedLineSpacing);

	QFont f(font());
	f.setBold(true);
	parentDropCapButton->setFont(f);

//  ASK Avox!
// 	bool useBaselineGrid() const { return BaseAdj==NOVALUE && parent()? inh().useBaselineGrid() : BaseAdj > 0; }

	cpage->parentLabel->setText(tr("Based on"));
	cpage->show(pstyle.charStyle(), cstyles);

	if (hasParent)
	{
		cpage->parentCombo->insertItem(tr("Parent's Character Style"), 1);
		if (pstyle.charStyle().hasParent() && pstyle.charStyle().parent()->hasName())
		{
			QString pname = pstyle.charStyle().parent()->name();
			for (int i = 2; i < cpage->parentCombo->count(); ++i)
			{
				if (cpage->parentCombo->text(i) == pname)
				{
					cpage->parentCombo->setCurrentItem(i);
					break;
				}
			}
		}
		else if (pstyle.charStyle().hasParent())
			cpage->parentCombo->setCurrentItem(1); // from parent
		else
			cpage->parentCombo->setCurrentItem(0); // custom
	}
	else
	{
		if (pstyle.charStyle().hasParent() && pstyle.charStyle().parent()->hasName())
		{
			QString pname = pstyle.charStyle().parent()->name();
			for (int i = 1; i < cpage->parentCombo->count(); ++i)
			{
				if (cpage->parentCombo->text(i) == pname)
				{
					cpage->parentCombo->setCurrentItem(i);
					break;
				}
			}
		}
		else
		{
			cpage->parentCombo->setCurrentItem(0); // custom
		}
	}

	

	parentCombo->clear();
	parentCombo->insertItem("");

	for (uint i = 5; i < pstyles.count(); ++i)
	{
		if (pstyles[i].displayName() != pstyle.displayName())
			parentCombo->insertItem(pstyles[i].displayName());
	}

	if (hasParent)
	{
		int index = 0;
		for (int i = 0; i < parentCombo->count(); ++i)
		{
			if (parentCombo->text(i) == parent->displayName())
			{
				index = i;
				break;
			}
		}
		parentCombo->setCurrentItem(index);
	}
	else
		parentCombo->setCurrentItem(0);
}

void SMPStyleWidget::show(QValueList<ParagraphStyle> &pstyles, QValueList<ParagraphStyle> &pstylesAll, QValueList<CharStyle> &cstyles, int unitIndex)
{
	if (pstyles.count() == 1)
		show(pstyles[0], pstylesAll, cstyles, unitIndex);
	else if (pstyles.count() > 1)
	{
		
	}
}

void SMPStyleWidget::clearAll()
{

}

void SMPStyleWidget::slotDropCap()
{
	parentDropCapButton->show();
}

void SMPStyleWidget::slotParentDropCap()
{
	disconnect(parentDropCapButton, SIGNAL(clicked()), this, SLOT(slotParentDropCap()));
	disconnect(dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap()));
	parentDropCapButton->hide();
	dropCapsBox->setChecked(parentDropCap_);
	emit useParentDropCap();
	connect(parentDropCapButton, SIGNAL(clicked()), this, SLOT(slotParentDropCap()));
	connect(dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap()));
}

SMPStyleWidget::~SMPStyleWidget()
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

	fontFace_ = new SMFontComboH(characterBox);
	characterBoxLayout->addWidget( fontFace_ );

	characterBoxLayout->addSpacing( 10 );

	layout7 = new QHBoxLayout( 0, 0, 5, "layout7");
	fontSize_ = new SMMSpinBox( 1, 2048, characterBox, 1 );
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

	tracking_ = new SMMSpinBox( -300, 300, characterBox, 1 );
	tracking_->setSuffix( tr( " %" ) );
	layout7->addWidget(tracking_);
	

	pixmapLabel2 = new QLabel( characterBox, "pixmapLabel2" );
	pixmapLabel2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel2->setPixmap( loadIcon("textbase.png") );
	layout7->addWidget( pixmapLabel2 );
	baselineOffset_ = new SMMSpinBox( -100, 100, characterBox, 1 );
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

	fontHScale_ = new SMMSpinBox( 10, 400, characterBox, 1 );
	fontHScale_->setSuffix( tr( " %" ) );
	layout8->addWidget( fontHScale_ );

	pixmapLabel3_2 = new QLabel( "", characterBox, "pixmapLabel3_2" );
	pixmapLabel3_2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setPixmap( loadIcon("textscalev.png") );
	layout8->addWidget( pixmapLabel3_2 );

	fontVScale_ = new SMMSpinBox( 10, 400, characterBox, 1 );
	fontVScale_->setSuffix( tr( " %" ) );
	layout8->addWidget( fontVScale_ );
	layout8->addStretch(10);
	characterBoxLayout->addLayout( layout8 );

	characterBoxLayout->addSpacing( 10 );

	layout9a = new QHBoxLayout( 0, 0, 0, "layout9");
	effects_ = new SMStyleSelect(characterBox);
	layout9a->addWidget( effects_ );

	layout9a->addSpacing(10);

	language_ = new SMScComboBox(false, characterBox, "language_");
	layout9a->addWidget(language_);

	spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9a->addItem( spacer1 );
	characterBoxLayout->addLayout( layout9a, Qt::AlignLeft );

	characterBoxLayout->addSpacing( 10 );

	layout5 = new QHBoxLayout( 0, 0, 5, "layout5");
	FillIcon = new QLabel( "", characterBox, "FillIcon" );
	FillIcon->setPixmap(loadIcon("fill.png"));
	layout5->addWidget( FillIcon );

	fillColor_ = new SMColorCombo(characterBox, "TxFill");
	layout5->addWidget( fillColor_ );

	pixmapLabel3_20 = new QLabel( characterBox, "pixmapLabel3_20" );
	pixmapLabel3_20->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_20->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_20->setPixmap( loadIcon("shade.png") );
	layout5->addWidget( pixmapLabel3_20 );

	fillShade_ = new SMShadeButton(characterBox);
	layout5->addWidget( fillShade_ );
	QSpacerItem* spacer3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout5->addItem( spacer3 );
	characterBoxLayout->addLayout( layout5 );

	layout6 = new QHBoxLayout( 0, 0, 5, "layout6");
	StrokeIcon = new QLabel( "", characterBox, "StrokeIcon" );
	StrokeIcon->setPixmap(loadIcon("Stiftalt.xpm"));
	layout6->addWidget( StrokeIcon );

	strokeColor_ = new SMColorCombo(characterBox, "TxStroke");
	layout6->addWidget( strokeColor_ );

	pixmapLabel3_19 = new QLabel( "", characterBox, "pixmapLabel3_19" );
	pixmapLabel3_19->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_19->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_19->setPixmap( loadIcon("shade.png") );
	layout6->addWidget( pixmapLabel3_19 );

	strokeShade_ = new SMShadeButton(characterBox);
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

	connect(effects_, SIGNAL(State(int)), this, SLOT(slotColorChange()));
}

void SMCStylePage::languageChange()
{
	parentLabel->setText(tr("Parent"));
	fontVScale_->setSuffix(tr(" %"));
	fontHScale_->setSuffix(tr(" %"));
	baselineOffset_->setSuffix(tr(" %"));
	tracking_->setSuffix(tr(" %"));
	fontSize_->setSuffix(tr(" pt"));
}

void SMCStylePage::fillLangCombo(QMap<QString,QString> langMap, const QString &defaultLang)
{
	QStringList sortList;
	QMap<QString,QString>::Iterator it;

	langMap_ = langMap;
	defaultLang_ = defaultLang;

	language_->clear();

	for (it = langMap.begin(); it != langMap.end(); ++it)
		sortList.push_back(it.data());

	language_->insertStringList(sortQStringList(sortList));
	language_->listBox()->setMinimumWidth(language_->listBox()->maxItemWidth() + 24);
}

void SMCStylePage::fillColorCombo(ColorList &colors)
{
	fillColor_->clear();
	strokeColor_->clear();

	QPixmap pm = QPixmap(15, 15);
	fillColor_->insertItem(CommonStrings::NoneColor);
	strokeColor_->insertItem(CommonStrings::NoneColor);
	ColorList::Iterator itend=colors.end();
	for (ColorList::Iterator it = colors.begin(); it != itend; ++it)
	{
		pm.fill(colors[it.key()].getRawRGBColor());
		fillColor_->insertItem(pm, it.key());
		strokeColor_->insertItem(pm, it.key());
	}
	fillColor_->listBox()->setMinimumWidth(fillColor_->listBox()->maxItemWidth()+24);
	strokeColor_->listBox()->setMinimumWidth(strokeColor_->listBox()->maxItemWidth()+24);
}

void SMCStylePage::show(CharStyle &cstyle, QValueList<CharStyle> &cstyles)
{
	disconnect(effects_, SIGNAL(State(int)), this, SLOT(slotColorChange()));
	bool hasParent = cstyle.hasParent();
	const CharStyle *parent = dynamic_cast<const CharStyle*>(cstyle.parent());

	if (hasParent)
	{
		fontSize_->setValue(cstyle.fontSize() / 10.0, cstyle.isPfontSize());
		fontSize_->setParentValue(parent->fontSize() / 10.0);

		fontHScale_->setValue(cstyle.scaleH() / 10.0, cstyle.isPscaleH());
		fontHScale_->setParentValue(parent->scaleH() / 10.0);

		fontVScale_->setValue(cstyle.scaleV() / 10.0, cstyle.isPscaleV());
		fontVScale_->setParentValue(parent->scaleV() / 10.0);

		baselineOffset_->setValue(cstyle.baselineOffset() / 10.0, cstyle.isPbaselineOffset());
		baselineOffset_->setParentValue(parent->baselineOffset() / 10.0);

		tracking_->setValue(cstyle.tracking() / 10.0, cstyle.isPtracking());
		tracking_->setParentValue(parent->tracking() / 10.0);

		effects_->setStyle(static_cast<int>(cstyle.effects()), cstyle.isPeffects());
		effects_->setParentItem(static_cast<int>(parent->effects()));

		fillShade_->setValue(cstyle.fillShade(), cstyle.isPfillShade());
		fillShade_->setParentValue(parent->fillShade());

		strokeShade_->setValue(cstyle.strokeShade(), cstyle.isPstrokeShade());
		strokeShade_->setParentValue(parent->strokeShade());

		fillColor_->setCurrentText(cstyle.fillColor(), cstyle.isPfillColor());
		fillColor_->setParentText(parent->fillColor());

		strokeColor_->setCurrentText(cstyle.strokeColor(), cstyle.isPstrokeColor());
		strokeColor_->setParentText(parent->strokeColor());

		fontFace_->setCurrentFont(cstyle.font().scName(), cstyle.isPfont());
		fontFace_->setParentFont(parent->font().scName());
	}
	else
	{
		fontSize_->setValue(cstyle.fontSize() / 10.0);
		fontHScale_->setValue(cstyle.scaleH() / 10.0);
		fontVScale_->setValue(cstyle.scaleV() / 10.0);
		baselineOffset_->setValue(cstyle.baselineOffset() / 10.0);
		tracking_->setValue(cstyle.tracking() / 10.0);
		effects_->setStyle(static_cast<int>(cstyle.effects()));
		fillShade_->setValue(cstyle.fillShade());
		strokeShade_->setValue(cstyle.strokeShade());
		fillColor_->setCurrentText(cstyle.fillColor());
		strokeColor_->setCurrentText(cstyle.strokeColor());
		fontFace_->setCurrentFont(cstyle.font().scName());
	}

	effects_->ShadowVal->Xoffset->setValue(cstyle.shadowXOffset() / 10.0);
	effects_->ShadowVal->Yoffset->setValue(cstyle.shadowYOffset() / 10.0);
	effects_->OutlineVal->LWidth->setValue(cstyle.outlineWidth() / 10.0);
	effects_->StrikeVal->LPos->setValue(cstyle.strikethruOffset() / 10.0);
	effects_->StrikeVal->LWidth->setValue(cstyle.strikethruWidth() / 10.0);
	effects_->UnderlineVal->LPos->setValue(cstyle.underlineOffset() / 10.0);
	effects_->UnderlineVal->LWidth->setValue(cstyle.underlineWidth() / 10.0);
	slotColorChange();

	

	parentCombo->clear();
	parentCombo->insertItem("");
	for (uint i = 0; i < cstyles.count(); ++i)
	{
		if (cstyles[i].displayName() != cstyle.displayName())
			parentCombo->insertItem(cstyles[i].displayName());
	}

	if (cstyle.hasParent())
	{
		int index = 0;
		for (int i = 0; i < parentCombo->count(); ++i)
		{
			if (parentCombo->text(i) == cstyle.parent()->displayName())
			{
				index = i;
				break;
			}
		}
		parentCombo->setCurrentItem(index);
	}
	else
		parentCombo->setCurrentItem(0);

	fillLangCombo(langMap_, defaultLang_);
	QString clang = cstyle.language() == QString::null ? defaultLang_ : cstyle.language();
	QString plang = QString::null;
	if (hasParent)
		plang = parent->language() == QString::null ? defaultLang_ : parent->language();

	int ci, pi;
	for (int i = 0; i < language_->count(); ++i)
	{
		if (language_->text(i) == langMap_[clang])
			ci = i;
		
		if (hasParent && language_->text(i) == langMap_[plang])
			pi = i;
	}

	if (hasParent)
	{
		language_->setCurrentItem(ci, cstyle.isPlanguage());
		language_->setParentItem(pi);
	}
	else
		language_->setCurrentItem(ci);

	connect(effects_, SIGNAL(State(int)), this, SLOT(slotColorChange()));
}

void SMCStylePage::show(QValueList<CharStyle> &cstyles, QValueList<CharStyle> &cstylesAll)
{
	if (cstyles.count() == 1)
		show(cstyles[0], cstylesAll);
	else if (cstyles.count() > 1)
	{
		
	}
}

void SMCStylePage::clearAll()
{
	
}

void SMCStylePage::slotColorChange()
{
	int s = effects_->getStyle();
	bool enabled;
	if ((s & 4) || (s & 256))
		enabled = true;
	else
		enabled = false;
	StrokeIcon->setEnabled(enabled);
	strokeShade_->setEnabled(enabled);
	strokeColor_->setEnabled(enabled);
}


SMCStylePage::~SMCStylePage()
{
	
}

/******************************************************************************/
/******************************************************************************/
