/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>

#include "numeration.h"
#include "smpstylewidget.h"
#include "scribus.h"
#include "units.h"
#include "util.h"
#include "util_icon.h"
#include "ui/charselectenhanced.h"

static bool isEqual(double a, double b)
{
	Q_ASSERT(a >  -21473 && b > -21473 && a < 21474 && b < 21474);
	long al = static_cast<long>(10000 * a);
	long bl = static_cast<long>(10000 * b);
    return al == bl;
}


SMPStyleWidget::SMPStyleWidget(ScribusDoc* doc) : QWidget()
{
	m_Doc = doc;
	setupUi(this);
	//Not used yet
// 	optMarginCheckLeftProtruding->setVisible(false);
	lineSpacingLabel->setPixmap(loadIcon("linespacing2.png"));
	spaceAboveLabel->setPixmap( loadIcon("above.png") );
	spaceBelowLabel->setPixmap( loadIcon("below.png") );

	lineSpacingMode->addItem( tr("Fixed Linespacing"));
	lineSpacingMode->addItem( tr("Automatic Linespacing"));
	lineSpacingMode->addItem( tr("Align to Baseline Grid"));
	connect(lineSpacingMode, SIGNAL(highlighted(int)), this, SLOT(slotLineSpacingModeChanged(int)));

	lineSpacing->setSuffix(unitGetSuffixFromIndex(0));
	spaceAbove->setSuffix(unitGetSuffixFromIndex(0));
	spaceBelow->setSuffix(unitGetSuffixFromIndex(0));

//	optMarginCombo->addItem(tr("None"), ParagraphStyle::OM_None);
//	optMarginCombo->addItem(tr("Left Protruding"), ParagraphStyle::OM_LeftProtruding);
//	optMarginCombo->addItem(tr("Right Protruding"), ParagraphStyle::OM_RightProtruding);
//	optMarginCombo->addItem(tr("Left Hanging Punctuation"), ParagraphStyle::OM_LeftHangingPunct);
//	optMarginCombo->addItem(tr("Right Hanging Punctuation"), ParagraphStyle::OM_RightHangingPunct);
//	optMarginCombo->addItem(tr("Default"), ParagraphStyle::OM_Default);

	parEffectOffset->setSuffix(unitGetSuffixFromIndex(0));
	
	fillBulletStrEditCombo();
	bulletCharTableButton->setIcon(loadIcon("22/insert-table.png"));
	fillNumFormatCombo();
	numStartSpin->setMinimum(1);
	numStartSpin->setMaximum(9999);
	numLevelSpin->setMinimum(1);
	numLevelSpin->setMaximum(1);
	fillNumRestartCombo();
	dropCapLines->setMinimum(2);
	dropCapLines->setMaximum(99);

	minSpaceSpin->setSuffix(unitGetSuffixFromIndex(SC_PERCENT));
	minGlyphExtSpin->setSuffix(unitGetSuffixFromIndex(SC_PERCENT));
	maxGlyphExtSpin->setSuffix(unitGetSuffixFromIndex(SC_PERCENT));

	connect(optMarginDefaultButton, SIGNAL(clicked()), this, SLOT(slotDefaultOpticalMargins()));
	if (m_Doc)
		connect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
	m_enhanced = NULL;
}

void SMPStyleWidget::slotLineSpacingModeChanged(int i)
{
	lineSpacing->setEnabled(i == 0);
}

void SMPStyleWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void SMPStyleWidget::languageChange()
{
/***********************************/
/*      Begin Tooltips             */
/***********************************/
// These are for the paragraph style

	parentCombo->setToolTip(      tr("Parent Style"));
	lineSpacingMode->setToolTip( tr("Line Spacing Mode"));
	lineSpacing->setToolTip(     tr("Line Spacing"));
	spaceAbove->setToolTip(      tr("Space Above"));
	spaceBelow->setToolTip(      tr("Space Below"));
	lineSpacingLabel->setToolTip(lineSpacing->toolTip());
	spaceAboveLabel->setToolTip(spaceAbove->toolTip());
	spaceBelowLabel->setToolTip(spaceBelow->toolTip());
//	optMarginCombo->setToolTip(tr("Activate an optical margins layout"));
//	optMarginLabel->setToolTip(optMarginCombo->toolTip());
	//CB Unneeded, gets in the way of single widget tooltips
	//dropCapsBox->setToolTip(      tr("Enable or disable drop cap"));
	dropCapLines->setToolTip(    tr("Drop Cap Lines"));
	bulletCharTableButton->setToolTip(tr("Enhanced Char Table for inserting customs chars as bullets"));
	parEffectOffset->setToolTip(   tr("Paragraph Effects Chars Offset"));
	parEffectIndentBox->setToolTip(   tr("Hang Paragraph Effect before paragraph indent"));
	parEffectCharStyleCombo->setToolTip("<qt>" + tr("Choose chracter style or leave blank for use default paragraph style"));
	alignement->setToolTip(      tr("Alignment"));
	tabList->first_->setToolTip( tr("First Line Indent"));
	tabList->left_->setToolTip(  tr("Left Indent"));
	tabList->right_->setToolTip( tr("Right Indent"));
	//CB Unneeded, gets in the way of single widget tooltips
	//tabList->setToolTip(         tr("Tabulators"));
	
	minSpaceSpin->setToolTip(tr("Maximum white space compression allowed.\nExpressed as a percentage of the current white space value."));
	minSpaceLabel->setToolTip(minSpaceSpin->toolTip());
	minGlyphExtSpin->setToolTip(tr("Maximum compression of glyphs"));
	minGlyphExtLabel->setToolTip(minGlyphExtSpin->toolTip());
	maxGlyphExtSpin->setToolTip(tr("Maximum extension of glyphs"));
	maxGlyphExtLabel->setToolTip(maxGlyphExtSpin->toolTip());

	keepLinesStart->setToolTip ("<qt>" + tr ("Ensure that first lines of a paragraph won't end up separated from the rest (known as widow/orphan control)") + "</qt>");
	keepLinesEnd->setToolTip ("<qt>" + tr ("Ensure that last lines of a paragraph won't end up separated from the rest (known as widow/orphan control)") + "</qt>");
	keepLabelStart->setToolTip (keepLinesStart->toolTip());
	keepLabelEnd->setToolTip (keepLinesEnd->toolTip());
	keepTogether->setToolTip ("<qt>" + tr ("If checked, ensures that the paragraph won't be split across multiple pages or columns") + "</qt>");
	keepWithNext->setToolTip ("<qt>" + tr ("If checked, automatically moves the paragraph to the next column or page if the next paragraph isn't on the same page or column") + "</qt>");

/***********************************/
/*      End Tooltips               */
/***********************************/

	lineSpacingMode->clear();
	lineSpacingMode->addItem( tr("Fixed Linespacing"));
	lineSpacingMode->addItem( tr("Automatic Linespacing"));
	lineSpacingMode->addItem( tr("Align to Baseline Grid"));
	
//	optMarginCombo->clear();
//	optMarginCombo->addItem(tr("None"), ParagraphStyle::OM_None);
//	optMarginCombo->addItem(tr("Left Protruding"), ParagraphStyle::OM_LeftProtruding);
//	optMarginCombo->addItem(tr("Right Protruding"), ParagraphStyle::OM_RightProtruding);
//	optMarginCombo->addItem(tr("Left Hanging Punctuation"), ParagraphStyle::OM_LeftHangingPunct);
//	optMarginCombo->addItem(tr("Right Hanging Punctuation"), ParagraphStyle::OM_RightHangingPunct);
//	optMarginCombo->addItem(tr("Default"), ParagraphStyle::OM_Default);
//
//	optMarginLabel->setText(tr("Optical Margins:"));

	lineSpacing->setSuffix(unitGetSuffixFromIndex(0));
	spaceAbove->setSuffix(unitGetSuffixFromIndex(0));
	spaceBelow->setSuffix(unitGetSuffixFromIndex(0));
	parentLabel->setText( tr("Based On:"));
	distancesBox->setTitle( tr("Distances and Alignment"));

	parEffectsBox->setTitle(tr("Paragraph Effects"));
	dropCapsBox->setTitle( tr("Drop Caps"));
	bulletBox->setTitle(tr("Bullets"));
	numBox->setTitle(tr("Numeration"));
	dropCapsLineLabel->setText( tr("Lines:"));
	bulletCharLabel->setText(tr("Bullet Char/String"));
	bulletCharTableButton->setText(tr("Char Table"));
	numFormatLabel->setText(tr("Numbering Style"));
	numLevelLabel->setText(tr("Level"));
	numPrefixLabel->setText(tr("Prefix"));
	numSuffixLabel->setText(tr("Suffix"));
	numStartLabel->setText(tr("Start with"));
	numRestartOtherBox->setText(tr("Restart after other format"));
	numRestartHigherBox->setText(tr("Restart after higher level"));

	parEffectCharStyleComboLabel->setText(tr("Character Style for Effect:"));
	distFromTextLabel->setText(tr("Distance from Text:"));
	parentParEffectsButton->setText(tr("Use Parent`s Values"));
	
	QFont font1;
	if (font1.pointSize())
		font1.setPointSize(font1.pointSize() *2);
	else if (font1.pixelSize())
		font1.setPixelSize(font1.pixelSize() *2);
	((QComboBox*) bulletStrEdit)->setFont(font1);
	(bulletStrEdit->lineEdit())->setFont(font1);

	tabsBox->setTitle( tr("Tabulators and Indentation"));
	tabWidget->setTabText(0, tr("Properties"));
	tabWidget->setTabText(1, tr("Character Style"));
	
	advSettingsGroupBox->setTitle( tr("Advanced Settings"));
	minSpaceLabel->setText( tr("Min. Space Width:"));
	glyphExtensionLabel->setText( tr("Glyph Extension"));
	minGlyphExtLabel->setText( tr("Min:", "Glyph Extension"));
	maxGlyphExtLabel->setText (tr("Max:", "Glyph Extension"));

	opticalMarginsGroupBox->setTitle( tr("Optical Margins"));
	optMarginRadioNone->setText( tr("None","optical margins") );
	optMarginRadioBoth->setText( tr("Both Sides","optical margins") );
	optMarginRadioLeft->setText( tr("Left Only","optical margins") );
	optMarginRadioRight->setText( tr("Right Only","optical margins") );

	optMarginDefaultButton->setText( tr("Reset to Default") );
	optMarginParentButton->setText( tr("Use Parent Value") );
}

void SMPStyleWidget::unitChange(double oldRatio, double newRatio, int unitIndex)
{
	parEffectOffset->setNewUnit(unitIndex);
	tabList->unitChange(unitIndex);
}

void SMPStyleWidget::setDoc(ScribusDoc *doc)
{
	if (m_Doc)
		disconnect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
	m_Doc = doc;
	if (m_Doc)
	{
		connect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
		fillNumerationsCombo();
	}
}

void SMPStyleWidget::fillBulletStrEditCombo()
{
	bulletStrEdit->clear();
	bulletStrEdit->addItem(QChar(0x2022));
	bulletStrEdit->addItem("*");
	bulletStrEdit->addItem(QChar(0x2013));
	bulletStrEdit->setMinimumWidth(50);
	if (bulletStrEdit->currentText().isEmpty())
		bulletStrEdit->setEditText(QChar(0x2022));
}

void SMPStyleWidget::fillNumFormatCombo()
{
	numFormatCombo->clear();
	numFormatCombo->addItems(getFormatList());
}

void SMPStyleWidget::fillNumerationsCombo()
{
	QStringList numNames;
	foreach (QString numName, m_Doc->numerations.keys())
		numNames.append(numName);
	numNames.sort();
	numComboBox->clear();
	numComboBox->insertItems(0, numNames);
	numComboBox->setCurrentItem(0);
}

void SMPStyleWidget::fillNumRestartCombo()
{
	numRestartCombo->clear();
	numRestartCombo->addItem(tr("Document"));
	numRestartCombo->addItem(tr("Section"));
	numRestartCombo->addItem(tr("Story"));
	numRestartCombo->addItem(tr("Page"));
	numRestartCombo->addItem(tr("Frame"));
}

void SMPStyleWidget::checkParEffectState()
{
	bool enable = false;
	if (dropCapsBox->isChecked() || bulletBox->isChecked() || numBox->isChecked())
		enable = true;

	parEffectCharStyleCombo->setEnabled(enable);
	parEffectOffset->setEnabled(enable);
	parEffectIndentBox->setEnabled(enable);
}

void SMPStyleWidget::show(ParagraphStyle *pstyle, QList<ParagraphStyle> &pstyles, QList<CharStyle> &cstyles, int unitIndex, const QString &defLang)
{
	currPStyle = pstyle;
	double unitRatio = unitGetRatioFromIndex(unitIndex);
	parentCombo->setEnabled(!pstyle->isDefaultStyle());
	const ParagraphStyle *parent = dynamic_cast<const ParagraphStyle*>(pstyle->parentStyle());
	hasParent_ = pstyle->hasParent() && parent != NULL && parent->hasName() && pstyle->parent() != "";

	lineSpacingMode->clear();
	lineSpacingMode->addItem( tr("Fixed Linespacing"));
	lineSpacingMode->addItem( tr("Automatic Linespacing"));
	lineSpacingMode->addItem( tr("Align to Baseline Grid"));
	
//	optMarginCombo->clear();
//	optMarginCombo->addItem(tr("None"), ParagraphStyle::OM_None);
//	optMarginCombo->addItem(tr("Left Protruding"), ParagraphStyle::OM_LeftProtruding);
//	optMarginCombo->addItem(tr("Right Protruding"), ParagraphStyle::OM_RightProtruding);
//	optMarginCombo->addItem(tr("Left Hanging Punctuation"), ParagraphStyle::OM_LeftHangingPunct);
//	optMarginCombo->addItem(tr("Right Hanging Punctuation"), ParagraphStyle::OM_RightHangingPunct);
//	optMarginCombo->addItem(tr("Default"), ParagraphStyle::OM_Default);
	
	// One could think it’s too much (aesthetic) or not enough (freedom)!
	minSpaceSpin->setRange(1.0,100.0);
	minGlyphExtSpin->setRange(90.0,100.0);
	maxGlyphExtSpin->setRange(100.0,110.0);
	
	//fillBulletStrEditCombo();
	//fillNumFormatCombo();
	//fillNumerationsCombo();
	//fillNumRestartCombo();

	if (hasParent_)
	{
		lineSpacingMode->setCurrentItem(pstyle->lineSpacingMode(), pstyle->isInhLineSpacingMode());
		lineSpacingMode->setParentItem(parent->lineSpacingMode());
		
//		optMarginCombo->setCurrentItemByData( pstyle->opticalMargins(),  pstyle->isInhOpticalMargins() );
//		optMarginCombo->setParentItem(optMarginCombo->getItemIndexForData( parent->opticalMargins()));
		setOpticalMargins(pstyle->opticalMargins(), pstyle->isInhOpticalMargins(), parent);
		connect(optMarginParentButton, SIGNAL(clicked()), this, SLOT(slotParentOpticalMargins()));
		
		minSpaceSpin->setValue(pstyle->minWordTracking() * 100.0,  pstyle->isInhMinWordTracking());
		minSpaceSpin->setParentValue(parent->minWordTracking());
		minGlyphExtSpin->setValue(pstyle->minGlyphExtension() * 100.0,  pstyle->isInhMinGlyphExtension());
		minGlyphExtSpin->setParentValue(parent->minGlyphExtension());
		maxGlyphExtSpin->setValue(pstyle->maxGlyphExtension() * 100.0,  pstyle->isInhMaxGlyphExtension());
		maxGlyphExtSpin->setParentValue(parent->maxGlyphExtension());

		lineSpacing->setValue(pstyle->lineSpacing(), pstyle->isInhLineSpacing());
		lineSpacing->setParentValue(parent->lineSpacing());

		spaceAbove->setValue(pstyle->gapBefore(), pstyle->isInhGapBefore());
		spaceAbove->setParentValue(parent->gapBefore());

		spaceBelow->setValue(pstyle->gapAfter(), pstyle->isInhGapAfter());
		spaceBelow->setParentValue(parent->gapAfter());


		alignement->setStyle(pstyle->alignment(), pstyle->isInhAlignment());
		alignement->setParentItem(parent->alignment());

		bool hasParentTabs = pstyle->isInhTabValues();
		QList<ParagraphStyle::TabRecord> tabs;
		if (hasParentTabs)
			tabs = QList<ParagraphStyle::TabRecord>(parent->tabValues());
		else
			tabs = pstyle->tabValues();

		tabList->setTabs(tabs, unitIndex, hasParentTabs);
		tabList->setParentTabs(parent->tabValues());

		tabList->setLeftIndentValue(pstyle->leftMargin() * unitRatio,pstyle->isInhLeftMargin());
		tabList->setParentLeftIndent(parent->leftMargin() * unitRatio);

		tabList->setFirstLineValue(pstyle->firstIndent() * unitRatio, pstyle->isInhFirstIndent());
		tabList->setParentFirstLine(parent->firstIndent() * unitRatio);

		tabList->setRightIndentValue(pstyle->rightMargin() * unitRatio, pstyle->isInhRightMargin());
		tabList->setParentRightIndent(parent->rightMargin() * unitRatio);

		keepLinesStart->setValue (pstyle->keepLinesStart(), pstyle->isInhKeepLinesStart());
		keepLinesEnd->setValue (pstyle->keepLinesEnd(), pstyle->isInhKeepLinesEnd());
		keepTogether->setChecked (pstyle->keepTogether(), pstyle->isInhKeepTogether());
		keepWithNext->setChecked (pstyle->keepWithNext(), pstyle->isInhKeepWithNext());
		keepLinesStart->setParentValue (parent->keepLinesStart());
		keepLinesEnd->setParentValue (parent->keepLinesEnd());
		keepTogether->setParentValue (parent->keepTogether());
		keepWithNext->setParentValue (parent->keepWithNext());
		
//Effects Gropup Box
		parentDC_ = parent->hasDropCap();
		parentBul_ = parent->hasBullet();
		parentNum_ = parent->hasNum();
		//parentParEffects_ = (parentDC_ || parentBul_ || parentNum_);
		if (pstyle->isInhHasDropCap() && pstyle->isInhHasBullet() && pstyle->isInhHasNum())
		{
			parentParEffectsButton->hide();
			disconnect(parentParEffectsButton, SIGNAL(clicked()), this, SLOT(slotParentParEffects()));
		}
		else
		{
			parentParEffectsButton->show();
			QFont f(font());
			f.setBold(true);
			parentParEffectsButton->setFont(f);
		}
		connect(parentParEffectsButton, SIGNAL(clicked()), this, SLOT(slotParentParEffects()));

		dropCapsBox->setChecked(pstyle->hasDropCap());
		setWidgetBoldFont(dropCapsBox, !pstyle->isInhHasDropCap());
		dropCapLines->setValue(pstyle->dropCapLines(), pstyle->isInhDropCapLines());
		dropCapLines->setParentValue(parent->dropCapLines());

		parEffectOffset->setValue(pstyle->parEffectOffset() * unitRatio, pstyle->isInhParEffectOffset());
		parEffectOffset->setParentValue(parent->parEffectOffset() * unitRatio);
		parEffectIndentBox->setChecked(pstyle->parEffectIndent(),pstyle->isInhParEffectIndent());
		parEffectIndentBox->setParentValue(parent->parEffectIndent());

		bulletBox->setChecked(pstyle->hasBullet());
		setWidgetBoldFont(bulletBox, !pstyle->isInhHasBullet());
		bulletStrEdit->setEditText(pstyle->bulletStr());
		setWidgetBoldFont(bulletCharLabel, !pstyle->isInhBulletStr());
		numBox->setChecked(pstyle->hasNum());
		setWidgetBoldFont(numBox, !pstyle->isInhHasNum());
		QString numName = pstyle->numName();
		if (numName.isEmpty())
			numName = "default";
		numComboBox->setCurrentItem(numComboBox->findText(numName), pstyle->isInhNumName());
		if (!parent->numName().isEmpty())
			numComboBox->setParentItem(numComboBox->findText(parent->numName()));
		else
			numComboBox->setParentItem(0);
		numFormatCombo->setCurrentItem(pstyle->numFormat());
		numFormatCombo->setParentItem(parent->numFormat());
		numLevelSpin->setValue(pstyle->numLevel() +1, pstyle->isInhNumLevel());
		NumStruct * numS = m_Doc->numerations.value(pstyle->numName());
		if (numS)
			numLevelSpin->setMaximum(numS->m_counters.count()+1);
		else
			numLevelSpin->setMaximum(1);
		numLevelSpin->setParentValue(parent->numLevel()+1);
		numPrefix->setText(pstyle->numPrefix());
		setWidgetBoldFont(numPrefixLabel, !pstyle->isInhNumPrefix());
		numSuffix->setText(pstyle->numSuffix());
		setWidgetBoldFont(numSuffixLabel, !pstyle->isInhNumSuffix());
		numStartSpin->setValue(pstyle->numStart(), pstyle->isInhNumStart());
		numStartSpin->setParentValue(parent->numStart());
		numRestartCombo->setCurrentItem(pstyle->numRestart(), pstyle->isInhNumRestart());
		numRestartCombo->setParentItem(parent->numRestart());
		numRestartOtherBox->setChecked(pstyle->numOther(), pstyle->isInhNumOther());
		numRestartOtherBox->setParentValue(parent->numOther());
		numRestartHigherBox->setChecked(pstyle->numHigher(), pstyle->isInhNumHigher());
		numRestartHigherBox->setParentValue(parent->numHigher());
	}
	else
	{
		lineSpacingMode->setCurrentIndex(pstyle->lineSpacingMode());
		lineSpacing->setValue(pstyle->lineSpacing());
		spaceAbove->setValue(pstyle->gapBefore());
		spaceBelow->setValue(pstyle->gapAfter());
//		optMarginCombo->setCurrentItemByData( pstyle->opticalMargins() );
		setOpticalMargins(pstyle->opticalMargins());
		optMarginParentButton->hide();
		minSpaceSpin->setValue(pstyle->minWordTracking() * 100.0);
		minGlyphExtSpin->setValue(pstyle->minGlyphExtension() * 100.0);
		maxGlyphExtSpin->setValue(pstyle->maxGlyphExtension() * 100.0);

		parEffectOffset->setValue(pstyle->parEffectOffset() * unitRatio);
		parEffectIndentBox->setChecked(pstyle->parEffectIndent());
		parentParEffectsButton->hide();
		disconnect(parentParEffectsButton, SIGNAL(clicked()), this, SLOT(slotParentParEffects()));
		dropCapsBox->setChecked(pstyle->hasDropCap());
		setWidgetBoldFont(dropCapsBox, false);
		dropCapLines->setValue(pstyle->dropCapLines());
		bulletBox->setChecked(pstyle->hasBullet());
		setWidgetBoldFont(bulletBox, false);
		bulletStrEdit->setEditText(pstyle->bulletStr());
		setWidgetBoldFont(bulletCharLabel, false);
		numBox->setChecked(pstyle->hasNum());
		setWidgetBoldFont(numBox, false);
		QString numName = pstyle->numName();
		if (numName.isEmpty())
			numName = "default";
		numComboBox->setCurrentItem(numComboBox->findText(numName));
		numNewLineEdit->clear();
		numFormatCombo->setCurrentIndex(pstyle->numFormat());
		numLevelSpin->setValue(pstyle->numLevel()+1);
		NumStruct * numS = m_Doc->numerations.value(pstyle->numName());
		if (numS)
			numLevelSpin->setMaximum(numS->m_counters.count()+1);
		else
			numLevelSpin->setMaximum(1);
		numPrefix->setText(pstyle->numPrefix());
		setWidgetBoldFont(numPrefixLabel, false);
		numSuffix->setText(pstyle->numSuffix());
		setWidgetBoldFont(numSuffixLabel, false);
		numStartSpin->setValue(pstyle->numStart());
		numRestartCombo->setCurrentItem(pstyle->numRestart());
		numRestartOtherBox->setChecked(pstyle->numOther());
		numRestartHigherBox->setChecked(pstyle->numHigher());

		alignement->setStyle(pstyle->alignment());
		tabList->setTabs(pstyle->tabValues(), unitIndex);
		tabList->setLeftIndentValue(pstyle->leftMargin() * unitRatio);
		tabList->setFirstLineValue(pstyle->firstIndent() * unitRatio);
		tabList->setRightIndentValue(pstyle->rightMargin() * unitRatio);

		keepLinesStart->setValue (pstyle->keepLinesStart());
		keepLinesEnd->setValue (pstyle->keepLinesEnd());
		keepTogether->setChecked (pstyle->keepTogether());
		keepWithNext->setChecked (pstyle->keepWithNext());
	}

	lineSpacing->setEnabled(pstyle->lineSpacingMode() == ParagraphStyle::FixedLineSpacing);
	dropCapLines->setEnabled(pstyle->hasDropCap());

	checkParEffectState();
	parEffectCharStyleCombo->clear();
	parEffectCharStyleCombo->addItem(tr("No Style"));
	for (int i =0; i < cstyles.count(); i++)
		parEffectCharStyleCombo->addItem(cstyles.at(i).name());
	setCurrentComboItem(parEffectCharStyleCombo, pstyle->peCharStyleName().isEmpty() ? tr("No Style") : pstyle->peCharStyleName());

	cpage->parentLabel->setText( tr("Based On:"));
	cpage->show(&pstyle->charStyle(), cstyles, defLang, unitIndex);

	parentCombo->clear();
	parentCombo->addItem( pstyle->isDefaultStyle()? tr("A default style cannot be assigned a parent style") : "");
	if (!pstyle->isDefaultStyle())
	{
		for (int i = 0; i < pstyles.count(); ++i)
		{
			if (pstyles[i].hasName() && pstyles[i].name() != pstyle->name())
				parentCombo->addItem(pstyles[i].name());
		}
	}
	if (pstyle->isDefaultStyle() || !hasParent_)
		parentCombo->setCurrentIndex(0);
	else
	{
		int index = 0;
		for (int i = 0; i < parentCombo->count(); ++i)
		{
			if (parentCombo->itemText(i) == parent->name())
			{
				index = i;
				break;
			}
		}
		parentCombo->setCurrentIndex(index);
	}

	connect(dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap(bool)));
	connect(bulletBox, SIGNAL(toggled(bool)), this, SLOT(slotBullets(bool)));
	connect(numBox, SIGNAL(toggled(bool)), this, SLOT(slotNumbering(bool)));
}

void SMPStyleWidget::show(QList<ParagraphStyle*> &pstyles, QList<ParagraphStyle> &pstylesAll, QList<CharStyle> &cstyles, int unitIndex, const QString &defLang)
{
	if (pstyles.count() == 1)
		show(pstyles[0], pstylesAll, cstyles, unitIndex, defLang);
	else if (pstyles.count() > 1)
	{
		currPStyle = pstyles[0];
		showLineSpacing(pstyles);
		showSpaceAB(pstyles, unitIndex);
		showDropCap(pstyles, cstyles, unitIndex);
		showBullet(pstyles, cstyles, unitIndex);
		showNumeration(pstyles, cstyles, unitIndex);
		showAlignment(pstyles);
		showOpticalMargin(pstyles);
		showMinSpace(pstyles);
		showMinGlyphExt(pstyles);
		showMaxGlyphExt(pstyles);
		showTabs(pstyles, unitIndex);
		showCStyle(pstyles, cstyles, defLang, unitIndex);
		showParent(pstyles);
		checkParEffectState();
	}
}

void SMPStyleWidget::showLineSpacing(QList<ParagraphStyle*> &pstyles)
{
	lineSpacingMode->clear();
	lineSpacingMode->addItem( tr("Fixed Linespacing"));
	lineSpacingMode->addItem( tr("Automatic Linespacing"));
	lineSpacingMode->addItem( tr("Align to Baseline Grid"));

	int tmpLP = -1;
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (tmpLP != -1 && pstyles[i]->lineSpacingMode() != tmpLP)
		{
			tmpLP = -1;
			break;
		}
		else
			tmpLP = pstyles[i]->lineSpacingMode();
	}

	if (tmpLP == -1)
	{
		if (lineSpacingMode->itemText(lineSpacingMode->count() - 1) != "")
			lineSpacingMode->addItem("");
		lineSpacingMode->setCurrentIndex(lineSpacingMode->count() - 1);
	}
	else
		lineSpacingMode->setCurrentIndex(tmpLP);

	double tmpLS = -1.0;
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (tmpLS > 0 && !isEqual(pstyles[i]->lineSpacing(), tmpLS))
		{
			tmpLS = -1.0;
			break;
		}
		else
			tmpLS = pstyles[i]->lineSpacing();
	}
	lineSpacing->setEnabled(true);
	if (tmpLS < 0)
		lineSpacing->clear();
	else
		lineSpacing->setValue(tmpLS);
}

void SMPStyleWidget::showSpaceAB(QList<ParagraphStyle*> &pstyles, int unitIndex)
{
// 	double unitRatio = unitGetRatioFromIndex(unitIndex);
	double tmpA = -1.2;
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (tmpA > -1.0 && !isEqual(pstyles[i]->gapBefore(), tmpA))
		{
			tmpA = -1.2;
			break;
		}
		else
			tmpA = pstyles[i]->gapBefore();
	}

	if (tmpA < 0)
		spaceAbove->clear();
	else
		spaceAbove->setValue(tmpA);

	tmpA = -1.2;
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (tmpA > -1.0 && !isEqual(pstyles[i]->gapAfter(), tmpA))
		{
			tmpA = -1.2;
			break;
		}
	}

	if (tmpA < 0)
		spaceBelow->clear();
	else
		spaceBelow->setValue(tmpA);
}

void SMPStyleWidget::showDropCap(QList<ParagraphStyle*> &pstyles, QList<CharStyle> &cstyles, int unitIndex)
{
	disconnectPESignals();
	bool dc = pstyles[0]->hasDropCap();
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (dc != pstyles[i]->hasDropCap())
		{
			dc = false;
			break;
		}
	}
	dropCapsBox->setChecked(dc);

	int lines = -1;
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (lines > -1 && pstyles[i]->dropCapLines() != lines)
		{
			lines = -1;
			break;
		}
		else
			lines = pstyles[i]->dropCapLines();
	}
	if (lines == -1)
		dropCapLines->clear();
	else
		dropCapLines->setValue(lines);

	dropCapsBox->setEnabled(true);
	dropCapLines->setEnabled(true);
	connectPESignals();
}

void SMPStyleWidget::showBullet(QList<ParagraphStyle *> &pstyles, QList<CharStyle> &cstyles, int unitIndex)
{
//	double unitRatio = unitGetRatioFromIndex(unitIndex);

	disconnectPESignals();
	bool hb = pstyles[0]->hasBullet();
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (hb != pstyles[i]->hasBullet())
		{
			hb = false;
			break;
		}
	}
	bulletBox->setChecked(hb);

	QString chStr = pstyles[0]->bulletStr();
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (chStr != pstyles[i]->bulletStr())
		{
			chStr.clear();
			break;
		}
		else
			chStr = pstyles[i]->bulletStr();
	}
	bulletStrEdit->setEditText(chStr);

	connectPESignals();
	bulletCharTableButton->setEnabled(true);
}

void SMPStyleWidget::showNumeration(QList<ParagraphStyle *> &pstyles, QList<CharStyle> &cstyles, int unitIndex)
{
	disconnectPESignals();
	QString prefix = pstyles[0]->numPrefix();
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (prefix != pstyles[i]->numPrefix())
		{
			prefix.clear();
			break;
		}
		else
			prefix = pstyles[i]->numPrefix();
	}
	numPrefix->setText(prefix);
	
	QString suffix = pstyles[0]->numSuffix();
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (suffix != pstyles[i]->numSuffix())
		{
			suffix.clear();
			break;
		}
		else
			suffix = pstyles[i]->numSuffix();
	}
	numSuffix->setText(suffix);

	numFormatCombo->setEnabled(true);
	numLevelSpin->setEnabled(true);
	connectPESignals();
}

void SMPStyleWidget::showAlignment(QList<ParagraphStyle*> &pstyles)
{
	if(pstyles.isEmpty())
	{
		qDebug()<<"Warning showAlignment called with an empty list of styles";
		return;
	}
	ParagraphStyle::AlignmentType a = pstyles[0]->alignment();
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (a != pstyles[i]->alignment())
		{
			if (alignement->selectedId() > -1 && alignement->selectedId() < 5)
			{
				alignement->buttonGroup->setExclusive(false);
				alignement->buttonGroup->button(alignement->selectedId())->toggle();
				alignement->buttonGroup->setExclusive(true);
			}
			return;
		}
	}
	alignement->setStyle(a);
}

void SMPStyleWidget::showOpticalMargin(QList< ParagraphStyle * > & pstyles)
{
	if(pstyles.isEmpty())
	{
		qDebug()<<"Warning showOpticalMargin called with an empty list of styles";
		return;
	}
	
//	optMarginCombo->clear();
//	optMarginCombo->addItem(tr("None"), ParagraphStyle::OM_None);
//	optMarginCombo->addItem(tr("Left Protruding"), ParagraphStyle::OM_LeftProtruding);
//	optMarginCombo->addItem(tr("Right Protruding"), ParagraphStyle::OM_RightProtruding);
//	optMarginCombo->addItem(tr("Left Hanging Punctuation"), ParagraphStyle::OM_LeftHangingPunct);
//	optMarginCombo->addItem(tr("Right Hanging Punctuation"), ParagraphStyle::OM_RightHangingPunct);
//	optMarginCombo->addItem(tr("Default"), ParagraphStyle::OM_Default);
//
//	// the static cast should not be required if opticalMargins() would return OpticalMarginType. Why it does not? mystery
//	ParagraphStyle::OpticalMarginType o( static_cast<ParagraphStyle::OpticalMarginType>(pstyles[0]->opticalMargins()) );
//	for (int i = 0; i < pstyles.count(); ++i)
//	{
//		if (o != pstyles[i]->opticalMargins())
//		{
//			optMarginCombo->setCurrentItem(0);
//			return;
//		}
//	}
//	optMarginCombo->setCurrentItemByData(o);
	setOpticalMargins(pstyles[0]->opticalMargins());
}

void SMPStyleWidget::showMinSpace(QList< ParagraphStyle * > & pstyles)
{
	if(pstyles.isEmpty())
	{
		qDebug()<<"Warning showMinSpace called with an empty list of styles";
		return;
	}
	
	double ms(pstyles[0]->minWordTracking());
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (ms != pstyles[i]->minWordTracking())
		{
			minSpaceSpin->setValue(100.0);
			return;
		}
	}
	minSpaceSpin->setValue(ms * 100.0);
}

void SMPStyleWidget::showMinGlyphExt(QList< ParagraphStyle * > & pstyles)
{
	if(pstyles.isEmpty())
	{
		qDebug()<<"Warning showMinGlyphExt called with an empty list of styles";
		return;
	}
	
	double mge(pstyles[0]->minGlyphExtension());
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (mge != pstyles[i]->minGlyphExtension())
		{
			minGlyphExtSpin->setValue(100.0);
			return;
		}
	}
	minGlyphExtSpin->setValue(mge * 100.0);
}

void SMPStyleWidget::showMaxGlyphExt(QList< ParagraphStyle * > & pstyles)
{
	if(pstyles.isEmpty())
	{
		qDebug()<<"Warning showMaxGlyphExt called with an empty list of styles";
		return;
	}
	
	double mge(pstyles[0]->maxGlyphExtension());
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (mge != pstyles[i]->maxGlyphExtension())
		{
			maxGlyphExtSpin->setValue(100.0);
			return;
		}
	}
	maxGlyphExtSpin->setValue(mge * 100.0);
}


void SMPStyleWidget::showTabs(QList<ParagraphStyle*> &pstyles, int unitIndex)
{
	double unitRatio = unitGetRatioFromIndex(unitIndex);
	QList<ParagraphStyle::TabRecord> t = pstyles[0]->tabValues();
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (t != pstyles[i]->tabValues())
		{
			t = QList<ParagraphStyle::TabRecord>();
			break;
		}
	}
	tabList->setTabs(t, unitIndex);

	double l = -4000.0;
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (l > -3800.0 && !isEqual(pstyles[i]->leftMargin(), l))
		{
			l = -4000.0;
			break;
		}
		else
			l = pstyles[i]->leftMargin();
	}
	if (l < -3800.0)
	{
		tabList->setLeftIndentValue(0.0);
		tabList->left_->clear();
	}
	else
		tabList->setLeftIndentValue(l * unitRatio);

	l = -4000.0;
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (l > -3800.0 && !isEqual(pstyles[i]->firstIndent(), l))
		{
			l = -4000.0;
			break;
		}
		else
			l = pstyles[i]->firstIndent();
	}
	if (l < -3800.0)
	{
		tabList->setFirstLineValue(0.0);
		tabList->first_->clear();
	}
	else
		tabList->setFirstLineValue(l * unitRatio);

	l = -4000.0;
	for (int i = 0; i < pstyles.count(); ++i)
	{
		if (l > -3800.0 && !isEqual(pstyles[i]->rightMargin(), l))
		{
			l = -4000.0;
			break;
		}
		else
			l = pstyles[i]->rightMargin();
	}
	if (l < -3800.0)
	{
		tabList->setRightIndentData(0.0);
		tabList->right_->clear();
	}
	else
		tabList->setRightIndentValue(l * unitRatio);

}

void SMPStyleWidget::showCStyle(QList<ParagraphStyle*> &pstyles, QList<CharStyle> &cstyles, const QString &defLang, int unitIndex)
{
	cpage->parentLabel->setText( tr("Based On:"));

	QList<CharStyle*> cstyle;
	for (int i = 0; i < pstyles.count(); ++i)
		cstyle << &pstyles[i]->charStyle();

	cpage->show(cstyle, cstyles, defLang, unitIndex);
}

void SMPStyleWidget::showParent(QList<ParagraphStyle*> &pstyles)
{
	parentCombo->setEnabled(false);

// 	parentCombo->clear();
// 	parentCombo->insertItem("");
// 	for (uint i = 0; i < pstyles.count(); ++i)
// 	{
// 		if (pstyles[i].hasName() && pstyles[i].name() != pstyle->name())
// 			parentCombo->insertItem(pstyles[i].name());
// 	}
// 
// 	if (hasParent_)
// 	{
// 		int index = 0;
// 		for (int i = 0; i < parentCombo->count(); ++i)
// 		{
// 			if (parentCombo->text(i) == parent->name())
// 			{
// 				index = i;
// 				break;
// 			}
// 		}
// 		parentCombo->setCurrentItem(index);
// 	}
// 	else
// 		parentCombo->setCurrentItem(0);
}

void SMPStyleWidget::setOpticalMargins(int o, bool inhO, const ParagraphStyle *parent)
{
	ParagraphStyle::OpticalMarginType om( static_cast<ParagraphStyle::OpticalMarginType>(o) );

	if (parent==NULL)
	{
		if(om == ParagraphStyle::OM_Default)
			optMarginRadioBoth->setChecked(true);
		else if(om == ParagraphStyle::OM_LeftHangingPunct)
			optMarginRadioLeft->setChecked(true);
		else if(om == ParagraphStyle::OM_RightHangingPunct)
			optMarginRadioRight->setChecked(true);
		else
			optMarginRadioNone->setChecked(true);
	}
	else
	{
		optMarginParentButton->setVisible(!inhO);

		if(om == ParagraphStyle::OM_Default)
			optMarginRadioBoth->setChecked(true,
				(parent->opticalMargins() == ParagraphStyle::OM_Default));
		else if(om == ParagraphStyle::OM_LeftHangingPunct)
			optMarginRadioLeft->setChecked(true,
				(parent->opticalMargins() == ParagraphStyle::OM_LeftHangingPunct));
		else if(om == ParagraphStyle::OM_RightHangingPunct)
			optMarginRadioRight->setChecked(true,
				(parent->opticalMargins() == ParagraphStyle::OM_RightHangingPunct));
		else
			optMarginRadioNone->setChecked(true,
				(parent->opticalMargins() == ParagraphStyle::OM_None));
		
		optMarginRadioBoth->setParentValue(parent->opticalMargins() ==  ParagraphStyle::OM_Default);
		optMarginRadioLeft->setParentValue(parent->opticalMargins() ==  ParagraphStyle::OM_LeftHangingPunct);
		optMarginRadioRight->setParentValue(parent->opticalMargins() ==  ParagraphStyle::OM_RightHangingPunct);
		optMarginRadioNone->setParentValue(parent->opticalMargins() ==  ParagraphStyle::OM_None);
	}
}


void SMPStyleWidget::slotDefaultOpticalMargins()
{
	optMarginRadioNone->setChecked(true);
	if (hasParent_)
		optMarginParentButton->show();
}

void SMPStyleWidget::slotParentOpticalMargins()
{
	disconnect(optMarginParentButton, SIGNAL(clicked()), this, SLOT(slotParentOpticalMargins()));
	optMarginParentButton->hide();
	emit useParentOptMargins();
	connect(optMarginParentButton, SIGNAL(clicked()), this, SLOT(slotParentOpticalMargins()));
}

void SMPStyleWidget::clearAll()
{

}

void SMPStyleWidget::slotDropCap(bool isOn)
{
	disconnectPESignals();
	if (isOn)
	{
		dropCapLines->setEnabled(true);

		bulletBox->setChecked(false);
		bulletStrEdit->setEnabled(false);
		bulletCharTableButton->setEnabled(false);

		numBox->setChecked(false);
		numFormatCombo->setEnabled(false);
		numLevelSpin->setEnabled(false);
		numComboBox->setEnabled(false);
		numRestartCombo->setEnabled(false);
		numNewLineEdit->setEnabled(false);
	}
	else
		dropCapLines->setEnabled(false);
	if (hasParent_)
		parentParEffectsButton->show();
	checkParEffectState();
	connectPESignals();
}

void SMPStyleWidget::slotBullets(bool isOn)
{
	disconnectPESignals();
	if (isOn)
	{
		bulletStrEdit->setEnabled(true);
		if (bulletStrEdit->currentText().isEmpty())
			bulletStrEdit->setEditText(bulletStrEdit->itemText(0));
		bulletCharTableButton->setEnabled(true);

		numBox->setChecked(false);
		numFormatCombo->setEnabled(false);
		numLevelSpin->setEnabled(false);
		numComboBox->setEnabled(false);
		numRestartCombo->setEnabled(false);
		numNewLineEdit->setEnabled(false);

		dropCapsBox->setChecked(false);
		dropCapLines->setEnabled(false);
	}
	else
	{
		bulletStrEdit->setEnabled(false);
		bulletCharTableButton->setEnabled(false);
	}
	if (hasParent_)
		parentParEffectsButton->show();
	checkParEffectState();
	connectPESignals();
}

void SMPStyleWidget::insertSpecialChars(const QString &chars)
{
	bulletStrEdit->lineEdit()->setText(chars);
}

void SMPStyleWidget::slotNumbering(bool isOn)
{
	disconnectPESignals();
	if (isOn)
	{
		numFormatCombo->setEnabled(true);
		numLevelSpin->setEnabled(true);
		numComboBox->setEnabled(true);
		if (numComboBox->currentIndex() < 0)
			numComboBox->setCurrentIndex(0);
		numRestartCombo->setEnabled(true);
		numNewLineEdit->setEnabled(true);

		bulletBox->setChecked(false);
		bulletStrEdit->setEnabled(false);
		bulletCharTableButton->setEnabled(false);

		dropCapsBox->setChecked(false);
		dropCapLines->setEnabled(false);
	}
	else
	{
		numFormatCombo->setEnabled(false);
		numLevelSpin->setEnabled(false);
	}
	if (hasParent_)
		parentParEffectsButton->show();
	checkParEffectState();
	connectPESignals();
}

void SMPStyleWidget::slotParentParEffects()
{
	disconnectPESignals();
	parentParEffectsButton->hide();
	dropCapsBox->setChecked(parentDC_);
	bulletBox->setChecked(parentBul_);
	numBox->setChecked(parentNum_);
	emit useParentParaEffects();
	connectPESignals();
}

SMPStyleWidget::~SMPStyleWidget()
{
	
}

void SMPStyleWidget::openEnhanced()
{
	if (m_enhanced)
		return;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	m_enhanced = new CharSelectEnhanced(this);
	m_enhanced->setModal(true);
	connect(m_enhanced, SIGNAL(insertSpecialChars(const QString &)), this, SLOT(insertSpecialChars(const QString &)));
	connect(m_enhanced, SIGNAL(paletteShown(bool)), bulletCharTableButton, SLOT(setChecked(bool)));
	m_enhanced->setDoc(m_Doc);
	m_enhanced->setEnabled(true);
	QString styleName = parEffectCharStyleCombo->currentText();
	if (styleName != tr("No Style") && !styleName.isEmpty())
	{
		CharStyle chStyle = m_Doc->charStyle(styleName);
		setCurrentComboItem(m_enhanced->fontSelector, chStyle.font().scName());
	}
	else if (currPStyle)
		setCurrentComboItem(m_enhanced->fontSelector, currPStyle->charStyle().font().scName());
	m_enhanced->newFont(m_enhanced->fontSelector->currentIndex());
	m_enhanced->show();
	QApplication::restoreOverrideCursor();
}

void SMPStyleWidget::closeEnhanced(bool show)
{
	if (!m_enhanced || show)
		return;
	disconnect(m_enhanced, SIGNAL(insertSpecialChars(const QString &)), this, SLOT(insertSpecialChars(const QString &)));
	disconnect(m_enhanced, SIGNAL(paletteShown(bool)), bulletCharTableButton, SLOT(setChecked(bool)));
	m_enhanced->close();
	delete m_enhanced;
	m_enhanced = NULL;
}

void SMPStyleWidget::connectPESignals()
{
	connect(parentParEffectsButton, SIGNAL(clicked()), this, SLOT(slotParentParEffects()));
	connect(bulletBox, SIGNAL(toggled(bool)), this, SLOT(slotBullets(bool)));
	connect(numBox, SIGNAL(toggled(bool)), this, SLOT(slotNumbering(bool)));
	connect(dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap(bool)));
}

void SMPStyleWidget::disconnectPESignals()
{
	disconnect(parentParEffectsButton, SIGNAL(clicked()), this, SLOT(slotParentParEffects()));
	disconnect(bulletBox, SIGNAL(toggled(bool)), this, SLOT(slotBullets(bool)));
	disconnect(numBox, SIGNAL(toggled(bool)), this, SLOT(slotNumbering(bool)));
	disconnect(dropCapsBox, SIGNAL(toggled(bool)), this, SLOT(slotDropCap(bool)));
}

void SMPStyleWidget::on_bulletCharTableButton_toggled(bool checked)
{
	if (m_enhanced && !checked)
		closeEnhanced();
	else if (!m_enhanced && checked)
		openEnhanced();
}

void SMPStyleWidget::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqNumUpdate)
		fillNumerationsCombo();
}
