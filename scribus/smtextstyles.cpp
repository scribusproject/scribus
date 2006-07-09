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

SMPStyleWidget::SMPStyleWidget()
{
	setupDistances();
	setupDropCaps();
	setupTabs();
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

	linespacingCombo = new QComboBox(distancesBox, "linespacingCombo");
	linespacingCombo->insertItem(tr("Fixed Linespacing"));
	linespacingCombo->insertItem(tr("Automatic Linespacing"));
	linespacingCombo->insertItem(tr("Align to Baseline Grid"));
	distancesBoxLayout->addMultiCellWidget(linespacingCombo, 0, 0, 1, 3);

	pixmapLabel0 = new QLabel(distancesBox, "pixmapLabel0");
	pixmapLabel0->setMinimumSize(QSize(22,22));
	pixmapLabel0->setMaximumSize(QSize(22,22));
	pixmapLabel0->setPixmap(loadIcon("linespacing2.png"));
	distancesBoxLayout->addWidget(pixmapLabel0, 1, 0);

	LineSpVal = new MSpinBox(1, 300, distancesBox, 1 );
	LineSpVal->setSuffix(tr( " pt" ));
	distancesBoxLayout->addWidget(LineSpVal, 1,1);

	pixmapLabel1 = new QLabel(distancesBox, "pixmapLabel2" );
	pixmapLabel1->setMinimumSize(QSize(22, 22));
	pixmapLabel1->setMaximumSize(QSize(22, 22));
	pixmapLabel1->setPixmap(loadIcon("textbase.png"));
	distancesBoxLayout->addWidget(pixmapLabel1, 2, 0);

	fontBase = new MSpinBox( -100, 100, distancesBox, 1 );
	fontBase->setSuffix( tr( " %" ) );
	distancesBoxLayout->addWidget( fontBase, 2, 1 );

	pixmapLabel3 = new QLabel( "", distancesBox, "TextLabel1_2_2" );
	pixmapLabel3->setPixmap( loadIcon("above.png") );
	distancesBoxLayout->addWidget( pixmapLabel3, 1, 2 );

	spaceAbove = new MSpinBox( 0, 300, distancesBox, 1 );
	spaceAbove->setSuffix( tr( " pt" ) );
	distancesBoxLayout->addWidget( spaceAbove, 1, 3 );

	pixmapLabel4 = new QLabel( "", distancesBox, "TextLabel1_2_3" );
	pixmapLabel4->setPixmap( loadIcon("below.png") );
	distancesBoxLayout->addWidget( pixmapLabel4, 2, 2 );

	spaceBelow = new MSpinBox( 0, 300, distancesBox, 1 );
	spaceBelow->setSuffix( tr( " pt" ) );
	distancesBoxLayout->addWidget( spaceBelow, 2, 3 );

	alignSelect = new AlignSelect(distancesBox);
	distancesBoxLayout->addMultiCellWidget(alignSelect, 3,3,1,3);
}

void SMPStyleWidget::setupDropCaps()
{
	dropCapsBox->setCheckable( true );
	dropCapsBox->setColumnLayout(0, Qt::Vertical );
	dropCapsBox->layout()->setSpacing( 5 );
	dropCapsBox->layout()->setMargin( 10 );

	dropCapsBoxLayout = new QGridLayout(dropCapsBox->layout());
	dropCapsBoxLayout->setAlignment(Qt::AlignTop);

	DropLines = new QSpinBox(dropCapsBox, "DropLines");
	DropLines->setMinValue(2);
	DropLines->setMaxValue(20);
	capLabel1 = new QLabel(DropLines, tr("&Lines:"), dropCapsBox, "CapLabel");
	dropCapsBoxLayout->addWidget(capLabel1, 0, 0);
	dropCapsBoxLayout->addWidget(DropLines, 0, 1);

	DropDist = new MSpinBox(-3000, 3000, dropCapsBox, 1);
	DropDist->setSuffix(tr(" pt"));
	capLabel2 = new QLabel(DropLines, tr("Distance from Text:"), dropCapsBox, "CapLabel2");
	dropCapsBoxLayout->addWidget(capLabel2, 1, 0);
	dropCapsBoxLayout->addWidget(DropDist, 1, 1);
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

	tabList = new Tabruler(tabsBox);
	tabsBoxLayout->addWidget( tabList );
}

SMPStyleWidget::~SMPStyleWidget()
{
	
}

/******************************************************************************/
/******************************************************************************/

SMParagraphStyle::SMParagraphStyle() : StyleItem(), pwidget_(0)
{

}

QTabWidget* SMParagraphStyle::widget()
{
	if (!pwidget_)
	{
		pwidget_ = new SMPStyleWidget();
		Q_CHECK_PTR(pwidget_);
	}

	Q_ASSERT(pwidget_);
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
	tmpPStyles_ = doc_->docParagraphStyles;
}

QValueList<StyleName> SMParagraphStyle::styles()
{
	QValueList<StyleName> tmpList;

	if (tmpPStyles_.count() < 6)
		return tmpList;

	for (uint x = 5; x < tmpPStyles_.count(); ++x)
		tmpList << StyleName(tmpPStyles_[x].name(),
		                     tmpPStyles_[x].parent() ? tmpPStyles_[x].parent()->name() : QString::null);

	return tmpList;
}

void SMParagraphStyle::selected(const QStringList &styleNames)
{

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

SMCStyleWidget::SMCStyleWidget()
{
	setupCharacter();
}

void SMCStyleWidget::setupCharacter()
{
	characterBoxLayout = new QVBoxLayout(characterBox);
	characterBoxLayout->setAlignment( Qt::AlignLeft );
	characterBoxLayout->setSpacing( 5 );
	characterBoxLayout->setMargin( 10 );

	FontC = new FontComboH(characterBox);
	characterBoxLayout->addWidget( FontC );

	characterBoxLayout->addSpacing( 10 );

	layout7 = new QHBoxLayout( 0, 0, 5, "layout7");
	SizeC = new MSpinBox( 1, 2048, characterBox, 1 );
	SizeC->setMinimumSize( QSize( 70, 22 ) );
	SizeC->setSuffix( tr( " pt" ) );

	TextF2 = new QLabel( "" ,characterBox, "TextF2" );
	TextF2->setPixmap(loadIcon("Zeichen.xpm"));
	TextF2->setMinimumSize( QSize( 22, 22 ) );
	TextF2->setMaximumSize( QSize( 22, 22 ) );
	layout7->addWidget( TextF2 );
	layout7->addWidget( SizeC );

	pixmapLabel3 = new QLabel( "", characterBox, "pixmapLabel3" );
	pixmapLabel3->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3->setPixmap( loadIcon("textscaleh.png") );
	layout7->addWidget( pixmapLabel3 );

	fontHScale = new MSpinBox( 10, 400, characterBox, 1 );
	fontHScale->setSuffix( tr( " %" ) );
	layout7->addWidget( fontHScale );

	pixmapLabel3_2 = new QLabel( "", characterBox, "pixmapLabel3_2" );
	pixmapLabel3_2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setPixmap( loadIcon("textscalev.png") );
	layout7->addWidget( pixmapLabel3_2 );

	fontVScale = new MSpinBox( 10, 400, characterBox, 1 );
	fontVScale->setSuffix( tr( " %" ) );
	layout7->addWidget( fontVScale );
	characterBoxLayout->addLayout( layout7 );

	characterBoxLayout->addSpacing( 10 );

	layout9a = new QHBoxLayout( 0, 0, 0, "layout9");
	EffeS = new StyleSelect(characterBox);
	layout9a->addWidget( EffeS );

	spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9a->addItem( spacer1 );
	characterBoxLayout->addLayout( layout9a, Qt::AlignLeft );

	characterBoxLayout->addSpacing( 10 );

	layout5 = new QHBoxLayout( 0, 0, 5, "layout5");
	FillIcon = new QLabel( "", characterBox, "FillIcon" );
	FillIcon->setPixmap(loadIcon("fill.png"));
	layout5->addWidget( FillIcon );

	TxFill = new ScComboBox( false, characterBox, "TxFill" );
	layout5->addWidget( TxFill );

	pixmapLabel3_20 = new QLabel( characterBox, "pixmapLabel3_20" );
	pixmapLabel3_20->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_20->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_20->setPixmap( loadIcon("shade.png") );
	layout5->addWidget( pixmapLabel3_20 );

	PM2 = new ShadeButton(characterBox);
	layout5->addWidget( PM2 );
	QSpacerItem* spacer3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout5->addItem( spacer3 );
	characterBoxLayout->addLayout( layout5 );

	layout6 = new QHBoxLayout( 0, 0, 5, "layout6");
	StrokeIcon = new QLabel( "", characterBox, "StrokeIcon" );
	StrokeIcon->setPixmap(loadIcon("Stiftalt.xpm"));
	layout6->addWidget( StrokeIcon );

	TxStroke = new ScComboBox( false, characterBox, "TxStroke" );
	layout6->addWidget( TxStroke );

	pixmapLabel3_19 = new QLabel( "", characterBox, "pixmapLabel3_19" );
	pixmapLabel3_19->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_19->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_19->setPixmap( loadIcon("shade.png") );
	layout6->addWidget( pixmapLabel3_19 );

	PM1 = new ShadeButton(characterBox);
	layout6->addWidget( PM1 );

	spacer4 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout6->addItem( spacer4 );
	characterBoxLayout->addLayout( layout6 );

	characterBoxLayout->addStretch(10);

	TxFill->clear();
	TxStroke->clear();
	ColorList::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	TxFill->insertItem(CommonStrings::NoneColor);
	TxStroke->insertItem(CommonStrings::NoneColor);
	StrokeIcon->setEnabled(false);
	PM1->setEnabled(false);
	TxStroke->setEnabled(false);
}

SMCStyleWidget::~SMCStyleWidget()
{

}

/******************************************************************************/
/******************************************************************************/

SMCharacterStyle::SMCharacterStyle() : StyleItem(), cwidget_(0)
{

}

QTabWidget* SMCharacterStyle::widget()
{
	if (!cwidget_)
	{
		cwidget_ = new SMCStyleWidget();
		Q_CHECK_PTR(cwidget_);
	}

	return cwidget_->tabWidget;
}

QString SMCharacterStyle::typeName()
{
	return tr("Character Styles");
}

void SMCharacterStyle::currentDoc(ScribusDoc *doc)
{
	Q_ASSERT(doc);
}

QValueList<StyleName> SMCharacterStyle::styles()
{
	QValueList<StyleName> tmpList;

	return tmpList;
}

void SMCharacterStyle::selected(const QStringList &styleNames)
{

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

}

