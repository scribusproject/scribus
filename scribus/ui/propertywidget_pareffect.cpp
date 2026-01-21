/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QSignalBlocker>

#include "propertywidget_pareffect.h"

#include "appmodehelper.h"
#include "appmodes.h"
#include "iconmanager.h"
#include "localemgr.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"
#include "util.h"

PropertyWidget_ParEffect::PropertyWidget_ParEffect(QWidget *parent) : QFrame(parent)
{
	setupUi(this);

	languageChange();
	dropCapLines->setValue(2);

	if (m_doc)
		peCharStyleCombo->updateStyleList();
	fillBulletStrEditCombo();

	numStart->setMinimum(1);
	numStart->setMaximum(9999);
	numLevelSpin->setMinimum(1);
	numLevelSpin->setMaximum(3);
	numPrefix->setMaxLength(5);
	numPrefix->setMaximumWidth(QFontMetrics(numPrefix->font()).averageCharWidth() * 7);
	numSuffix->setMaxLength(5);
	numSuffix->setMaximumWidth(QFontMetrics(numSuffix->font()).averageCharWidth() * 7);
	dropCapLines->setMinimum(2);
	dropCapLines->setMaximum(99);
	peCombo->setCurrentIndex(0);
	setType(peCombo->currentData().toInt());

	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
}

void PropertyWidget_ParEffect::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(handleAppModeChanged(int,int)));
	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertyWidget_ParEffect::setDoc(ScribusDoc *doc)
{
	if (doc == (ScribusDoc*) m_doc)
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc = doc;
	peCharStyleCombo->setDoc(doc);

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	fillNumerationsCombo();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));

	// Handle properties update when switching document
	handleSelectionChanged();
}

void PropertyWidget_ParEffect::setCurrentItem(PageItem *item)
{
	if (item && m_doc.isNull())
		setDoc(item->doc());

	m_item = item;
	disconnectSignals();

	if (!m_item) return;

	if (m_item->isTextFrame() || m_item->isPathText() || m_item->isTable())
	{
		configureWidgets();
		ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
		if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditTable)
			m_item->currentTextProps(parStyle);
		updateStyle(parStyle);
		connectSignals();
	}
}

void PropertyWidget_ParEffect::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();
	bulletCharTableButton->setIcon(iconManager.loadIcon("tool-insert-table"));
	peCharStyleLabel->setPixmap(iconManager.loadPixmap("character-style"));
	peOffsetLabel->setPixmap(iconManager.loadPixmap("paragraph-list-offset"));
	peLabel->setPixmap(iconManager.loadPixmap("paragraph-effects-style"));
}

void PropertyWidget_ParEffect::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	bool sigBlocked = peOffset->blockSignals(true);
	peOffset->setNewUnit( m_unitIndex );
	peOffset->blockSignals(sigBlocked);
}

void PropertyWidget_ParEffect::localeChange()
{
	const QLocale& l(LocaleManager::instance().userPreferredLocale());
	peOffset->setLocale(l);
}

void PropertyWidget_ParEffect::fillNumerationsCombo()
{
	QStringList numNames;
	if (m_doc)
	{
		const auto numerationKeys = m_doc->numerations.keys();
		for (const QString& numName : numerationKeys)
		{
			if (numName.isEmpty())
				continue;
			numNames.append(numName);
		}
		numNames.sort();
	}
	numNames.prepend("<local block>");
	numComboBox->clear();
	numComboBox->insertItems(0, numNames);
}

void PropertyWidget_ParEffect::updateTextStyles()
{
	peCharStyleCombo->updateStyleList();
}

void PropertyWidget_ParEffect::showCharStyle(const QString& name)
{
	bool blocked = peCharStyleCombo->blockSignals(true);
	peCharStyleCombo->setStyle(name);
	peCharStyleCombo->blockSignals(blocked);
}

void PropertyWidget_ParEffect::setType(int id)
{
	if (id == -1) // No effect
	{
		stackedWidget->setVisible(false);
		peGroup->setVisible(false);
		peCombo->setCurrentIndex(0);
	}
	else
	{
		stackedWidget->setCurrentIndex(id);
		stackedWidget->setVisible(true);
		peGroup->setVisible(true);
	}
}

void PropertyWidget_ParEffect::fillBulletStrEditCombo()
{
	bulletStrEdit->clear();
	bulletStrEdit->addItem(QChar(0x2022));
	bulletStrEdit->addItem("*");
	bulletStrEdit->addItem(QChar(0x2013));
	bulletStrEdit->setMinimumWidth(50);
	if (bulletStrEdit->currentText().isEmpty())
		bulletStrEdit->setEditText(QChar(0x2022));
}

void PropertyWidget_ParEffect::fillPECombo()
{
	QSignalBlocker sb(peCombo);
	int currIndex = peCombo->currentIndex();
	peCombo->clear();
	peCombo->addItem(tr("No Paragraph Effects"), -1);
	peCombo->addItem(tr("Drop Caps"), 0);
	peCombo->addItem(tr("Bulleted List"), 1);
	peCombo->addItem(tr("Numbered List"), 2);
	peCombo->setCurrentIndex(currIndex);
}

void PropertyWidget_ParEffect::updateStyle(const ParagraphStyle& newPStyle)
{
	int oldPeComboIndex = peCombo->currentIndex();

	if (peCombo->currentIndex() && !newPStyle.hasBullet() && !newPStyle.hasDropCap() && !newPStyle.hasNum())
	{
		setType(-1);
		return;
	}

	QSignalBlocker blocker1(peCombo);
	QSignalBlocker blocker2(dropCapLines);
	QSignalBlocker blocker3(bulletStrEdit);
	QSignalBlocker blocker4(numComboBox);
	QSignalBlocker blocker5(numLevelSpin);
	QSignalBlocker blocker6(numFormatCombo);
	QSignalBlocker blocker7(numPrefix);
	QSignalBlocker blocker8(numSuffix);
	QSignalBlocker blocker9(numStart);
	QSignalBlocker blockerA(peOffset);
	QSignalBlocker blockerB(peIndent);
	QSignalBlocker blockerC(peCharStyleCombo);

	if (newPStyle.hasDropCap())
	{
		peCombo->setCurrentIndex(1);
	}
	else if (newPStyle.hasBullet())
	{
		peCombo->setCurrentIndex(2);
	}
	else if (newPStyle.hasNum())
	{
		peCombo->setCurrentIndex(3);
	}
	else
		peCombo->setCurrentIndex(0);

	setType(peCombo->currentData().toInt());

	QString numName = numComboBox->currentText();
	dropCapLines->setValue(newPStyle.dropCapLines());
	bulletStrEdit->setEditText(newPStyle.bulletStr());
	numName = newPStyle.numName();
	if (numName.isEmpty())
		numName = "<local block>";
	numComboBox->setCurrentIndex(numComboBox->findText(numName));
	NumStruct * numS = m_doc->numerations.value(numName);
	if (numS)
		numLevelSpin->setMaximum(numS->m_counters.count() + 1);
	else
		numLevelSpin->setMaximum(3);
	numLevelSpin->setValue(newPStyle.numLevel() + 1);
	numPrefix->setText(newPStyle.numPrefix());
	numSuffix->setText(newPStyle.numSuffix());
	numStart->setValue(newPStyle.numStart());

	numFormatCombo->setCurrentFormat((NumFormat) newPStyle.numFormat());
	peOffset->setValue(newPStyle.parEffectOffset() * m_unitRatio);
	peIndent->setChecked(newPStyle.parEffectIndent());
	showCharStyle(newPStyle.peCharStyleName());

	if (oldPeComboIndex != peCombo->currentIndex())
		emit needsRelayout();
}

void PropertyWidget_ParEffect::connectSignals()
{
	connect(peCombo, SIGNAL(activated(int)), this, SLOT(handleParEffectUse()), Qt::UniqueConnection);
	connect(dropCapLines, SIGNAL(valueChanged(int)), this, SLOT(handleDropCapLines(int)), Qt::UniqueConnection);
	connect(bulletStrEdit, SIGNAL(editTextChanged(QString)), this, SLOT(handleBulletStr(QString)), Qt::UniqueConnection);
	connect(numComboBox, SIGNAL(textActivated(QString)), this, SLOT(handleNumName(QString)), Qt::UniqueConnection);
	connect(numLevelSpin, SIGNAL(valueChanged(int)), this, SLOT(handleNumLevel(int)), Qt::UniqueConnection);
	connect(numFormatCombo, SIGNAL(activated(int)), this, SLOT(handleNumFormat(int)), Qt::UniqueConnection);
	connect(numPrefix, SIGNAL(textChanged(QString)), this, SLOT(handleNumPrefix(QString)), Qt::UniqueConnection);
	connect(numSuffix, SIGNAL(textChanged(QString)), this, SLOT(handleNumSuffix(QString)), Qt::UniqueConnection);
	connect(numStart, SIGNAL(valueChanged(int)), this, SLOT(handleNumStart(int)), Qt::UniqueConnection);
	connect(peOffset, SIGNAL(valueChanged(double)), this, SLOT(handlePEOffset(double)), Qt::UniqueConnection);
	connect(peIndent, SIGNAL(toggled(bool)), this, SLOT(handlePEIndent(bool)), Qt::UniqueConnection);
	connect(peCharStyleCombo, SIGNAL(newStyle(QString)), this, SLOT(handlePECharStyle(QString)), Qt::UniqueConnection);
}

void PropertyWidget_ParEffect::disconnectSignals()
{
	disconnect(peCombo, SIGNAL(activated(int)), this, SLOT(handleParEffectUse()));
	disconnect(dropCapLines, SIGNAL(valueChanged(int)), this, SLOT(handleDropCapLines(int)));
	disconnect(bulletStrEdit, SIGNAL(editTextChanged(QString)), this, SLOT(handleBulletStr(QString)));
	disconnect(numComboBox, SIGNAL(textActivated(QString)), this, SLOT(handleNumName(QString)));
	disconnect(numLevelSpin, SIGNAL(valueChanged(int)), this, SLOT(handleNumLevel(int)));
	disconnect(numFormatCombo, SIGNAL(activated(int)), this, SLOT(handleNumFormat(int)));
	disconnect(numPrefix, SIGNAL(textChanged(QString)), this, SLOT(handleNumPrefix(QString)));
	disconnect(numSuffix, SIGNAL(textChanged(QString)), this, SLOT(handleNumSuffix(QString)));
	disconnect(numStart, SIGNAL(valueChanged(int)), this, SLOT(handleNumStart(int)));
	disconnect(peOffset, SIGNAL(valueChanged(double)), this, SLOT(handlePEOffset(double)));
	disconnect(peIndent, SIGNAL(toggled(bool)), this, SLOT(handlePEIndent(bool)));
	disconnect(peCharStyleCombo, SIGNAL(newStyle(QString)), this, SLOT(handlePECharStyle(QString)));
}

void PropertyWidget_ParEffect::configureWidgets()
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		PageItem_TextFrame *textItem = m_item->asTextFrame();
		if (m_doc->appMode == modeEditTable)
			textItem = m_item->asTable()->activeCell().textFrame();
		if (textItem)// || m_item->isPathText())
			enabled = true;
	}
	setEnabled(enabled);
}

void PropertyWidget_ParEffect::handleAppModeChanged(int oldMode, int mode)
{
	if (oldMode == modeEditTable || mode == modeEditTable)
	{
		setCurrentItem(m_item);
	}
}

void PropertyWidget_ParEffect::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
}

void PropertyWidget_ParEffect::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqCharStylesUpdate)
		updateTextStyles();
	if (updateFlags & reqStyleComboDocUpdate)
		setDoc(m_doc ? m_doc : nullptr);
	if (updateFlags & reqNumUpdate)
		fillNumerationsCombo();
}

void PropertyWidget_ParEffect::handleParEffectUse()
{
	if (!m_doc || !m_item)
		return;

	int id = peCombo->currentIndex();
	double newMinimum = -3000.0;
	if (id == 2 || id == 3)
		newMinimum = 0.0;
	peOffset->setMinimum(newMinimum);

	ParagraphStyle newStyle;

	if (peCombo->currentIndex() == 1)
	{
		newStyle.setDropCapLines(dropCapLines->value());
		newStyle.setHasDropCap(true);
		newStyle.setHasBullet(false);
		newStyle.setHasNum(false);
	}
	else if (peCombo->currentIndex() == 2)
	{
		newStyle.setHasBullet(true);
		QString bStr = bulletStrEdit->currentText();
		if (bStr.isEmpty())
			bStr = QChar(0x2022);
		newStyle.setBulletStr(bStr);
		newStyle.setHasNum(false);
		newStyle.setHasDropCap(false);
	}
	else if (peCombo->currentIndex() == 3)
	{
		newStyle.setHasDropCap(false);
		newStyle.setHasBullet(false);
		newStyle.setHasNum(true);
		newStyle.setNumName(numComboBox->currentText());
		newStyle.setNumFormat(numFormatCombo->currentFormat());
		newStyle.setNumLevel(numLevelSpin->value() - 1);
		newStyle.setNumStart(numStart->value());
		newStyle.setNumPrefix(numPrefix->text());
		newStyle.setNumSuffix(numSuffix->text());
		if (newStyle.numName() == "<local block>")
		{
			newStyle.setNumOther(true);
			newStyle.setNumHigher(true);
		}
	}
	else
	{
		newStyle.setHasDropCap(false);
		newStyle.setHasBullet(false);
		newStyle.setHasNum(false);
	}
	newStyle.setParEffectOffset(peOffset->value() / m_unitRatio);
	newStyle.setParEffectIndent(peIndent->isChecked());

	setType(peCombo->currentData().toInt());

	handleChanges(m_item, newStyle);

	emit needsRelayout();
}

void PropertyWidget_ParEffect::handleBulletStr(QString bulStr)
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	if (bulStr.isEmpty())
		bulStr = QChar(0x2022);
	newStyle.setBulletStr(bulStr);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handleDropCapLines(int dcLines)
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setDropCapLines(dcLines);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handleNumName(const QString& numName)
{
	if (!m_doc || !m_item)
		return;
	disconnectSignals();
	ParagraphStyle newStyle;
	if (numName == "<local block>")
	{
		newStyle.setNumOther(true);
		newStyle.setNumHigher(true);
		newStyle.setNumRestart(NSRstory);
	}
	else
	{
		NumStruct * numS = m_doc->numerations.value(numName);
		Q_ASSERT(numS);
		int level = qMin(numLevelSpin->value(), numS->m_counters.count()) - 1;
		numLevelSpin->setValue(level + 1);
		newStyle.setNumLevel(level);
		Numeration num = numS->m_nums[level];
		numFormatCombo->setCurrentFormat(num.numFormat);
		numStart->setValue(num.start);
		numPrefix->setText(num.prefix);
		numSuffix->setText(num.suffix);
	}
	newStyle.setNumPrefix(numPrefix->text());
	newStyle.setNumSuffix(numSuffix->text());
	newStyle.setNumName(numName);
	newStyle.setNumFormat(numFormatCombo->currentFormat());
	handleChanges(m_item, newStyle);
	connectSignals();
}

void PropertyWidget_ParEffect::handleNumFormat(int /*style*/)
{
	if (!m_doc || !m_item)
		return;
	NumFormat format = numFormatCombo->currentFormat();

	ParagraphStyle newStyle;
	newStyle.setNumFormat(format);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handleNumLevel(int level)
{
	if (!m_doc || !m_item)
		return;
	if ((numComboBox->currentText() != "<local block>") && level == numLevelSpin->maximum())
	{
		NumStruct * numS = m_doc->numerations.value(numComboBox->currentText());
		Q_ASSERT(numS);
		while (level > numS->m_counters.count())
		{
			numS->m_counters.append(0);
			Numeration num;
			num.numFormat = numFormatCombo->currentFormat();
			num.prefix = numPrefix->text();
			num.suffix = numSuffix->text();
			num.start = numStart->value();
			numS->m_nums.append(num);
		}
	}
	ParagraphStyle newStyle;
	newStyle.setNumLevel(level -1);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handleNumPrefix(const QString& prefix)
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setNumPrefix(prefix);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handleNumSuffix(const QString& suffix)
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setNumSuffix(suffix);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handleNumStart(int start)
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setNumStart(start);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handlePEOffset(double offset)
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setParEffectOffset(offset / m_unitRatio);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handlePEIndent(bool indent)
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setParEffectIndent(indent);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::handlePECharStyle(const QString& name)
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setPeCharStyleName(name);
	handleChanges(m_item, newStyle);
}

void PropertyWidget_ParEffect::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_ParEffect::handleChanges(PageItem *item, ParagraphStyle &newStyle)
{
	if (m_doc->appMode == modeEditTable)
		item = item->asTable()->activeCell().textFrame();
	if (item != nullptr)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));

		Selection tempSelection(this, false);
		tempSelection.addItem(item);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
		m_doc->updateNumbers();

		connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}
}

void PropertyWidget_ParEffect::languageChange()
{
	fillPECombo();
	retranslateUi(this);
}

void PropertyWidget_ParEffect::openEnhanced()
{
	if (m_enhanced)
		return;

	QApplication::changeOverrideCursor(QCursor(Qt::WaitCursor));
	m_enhanced = new CharSelectEnhanced(this);
	m_enhanced->setModal(true);
	connect(m_enhanced, SIGNAL(insertSpecialChars(const QVector<uint>&)), this, SLOT(insertSpecialChars(const QVector<uint>&)));
	connect(m_enhanced, SIGNAL(paletteShown(bool)), bulletCharTableButton, SLOT(setChecked(bool)));
	m_enhanced->setDoc(m_doc);
	m_enhanced->setEnabled(true);

	setCurrentComboItem(m_enhanced->fontSelector, m_item->currentStyle().charStyle().font().scName());
	m_enhanced->newFont(m_enhanced->fontSelector->currentIndex());
	m_enhanced->show();
	QApplication::changeOverrideCursor(Qt::ArrowCursor);
}

void PropertyWidget_ParEffect::closeEnhanced(bool show)
{
	if (!m_enhanced || show)
		return;
	disconnect(m_enhanced, SIGNAL(insertSpecialChars(const QVector<uint> &)), this, SLOT(insertSpecialChars(const QVector<uint> &)));
	disconnect(m_enhanced, SIGNAL(paletteShown(bool)), bulletCharTableButton, SLOT(setChecked(bool)));
	m_enhanced->close();
	m_enhanced->deleteLater();
	m_enhanced = nullptr;
}

void PropertyWidget_ParEffect::on_bulletCharTableButton_toggled(bool checked)
{
	if (m_enhanced && !checked)
		closeEnhanced();
	else if (!m_enhanced && checked)
		openEnhanced();
}
void PropertyWidget_ParEffect::insertSpecialChars(const QVector<uint> &charCodes)
{
	QString chars = QString::fromUcs4(charCodes.data(), charCodes.length());
	bulletStrEdit->lineEdit()->setText(chars);
}
