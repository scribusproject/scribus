/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>

#include "iconmanager.h"
#include "langmgr.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "smcstylewidget.h"
#include "util.h"
#include "units.h"


SMCStyleWidget::SMCStyleWidget(QWidget *parent) : QWidget(),
	m_Doc(0)
{
	setupUi(this);

	IconManager* im = IconManager::instance();
	fontSizeLabel_->setPixmap(im->loadPixmap("zeichen.png"));
	trackingLabel_->setPixmap(im->loadPixmap("textkern.png"));
	widthSpaceLabel->setPixmap(im->loadPixmap("spacewidth.png"));
	baselineOffsetLabel_->setPixmap(im->loadPixmap("textbase.png"));
	hscaleLabel_->setPixmap(im->loadPixmap("textscaleh.png"));
	vscaleLabel_->setPixmap(im->loadPixmap("textscalev.png"));
	FillIcon->setPixmap(im->loadPixmap("16/color-fill.png"));
	fillShadeLabel->setPixmap(im->loadPixmap("shade.png"));
	StrokeIcon->setPixmap(im->loadPixmap("16/color-stroke.png"));
	strokeShadeLabel->setPixmap(im->loadPixmap("shade.png"));
	backIcon->setPixmap(im->loadPixmap("16/color-fill.png"));
	backShadeLabel->setPixmap(im->loadPixmap("shade.png"));

	fillColor_->setPixmapType(ColorCombo::fancyPixmaps);
	fillColor_->clear();
	fillColor_->addItem(CommonStrings::tr_NoneColor);

	strokeColor_->setPixmapType(ColorCombo::fancyPixmaps);
	strokeColor_->clear();
	strokeColor_->addItem(CommonStrings::tr_NoneColor);

	backColor_->setPixmapType(ColorCombo::fancyPixmaps);
	backColor_->clear();
	backColor_->addItem(CommonStrings::tr_NoneColor);

	StrokeIcon->setEnabled(false);
	strokeShade_->setEnabled(false);
	strokeColor_->setEnabled(false);
	fontfeaturesSetting->resetFontFeatures();

	connect(effects_, SIGNAL(State(int)), this, SLOT(slotColorChange()));
	connect(fontFace_, SIGNAL(fontSelected(QString)), this, SLOT(slotEnableFontFeatures(QString)));
}

void SMCStyleWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void SMCStyleWidget::languageChange()
{
	QStringList languageList;
	LanguageManager::instance()->fillInstalledStringList(&languageList);
	fillLangComboFromList(languageList);

	if (fillColor_->count() > 0)
	{
		bool sigBlocked = fillColor_->blockSignals(true);
		fillColor_->setItemText(0, CommonStrings::tr_NoneColor);
		fillColor_->blockSignals(sigBlocked);
	}

	if (strokeColor_->count() > 0)
	{
		bool sigBlocked = strokeColor_->blockSignals(true);
		strokeColor_->setItemText(0, CommonStrings::tr_NoneColor);
		strokeColor_->blockSignals(sigBlocked);
	}

	if (backColor_->count() > 0)
	{
		bool sigBlocked = backColor_->blockSignals(true);
		backColor_->setItemText(0, CommonStrings::tr_NoneColor);
		backColor_->blockSignals(sigBlocked);
	}

	retranslateUi(this);

	fontSizeLabel_->setToolTip(fontSize_->toolTip());
	trackingLabel_->setToolTip(tracking_->toolTip());
	widthSpaceLabel->setToolTip(widthSpaceSpin->toolTip());
	baselineOffsetLabel_->setToolTip(baselineOffset_->toolTip());
	hscaleLabel_->setToolTip(fontHScale_->toolTip());
	vscaleLabel_->setToolTip(fontVScale_->toolTip());
	FillIcon->setToolTip(fillColor_->toolTip());
	fillShadeLabel->setToolTip(fillShade_->toolTip());
	StrokeIcon->setToolTip(strokeColor_->toolTip());
	strokeShadeLabel->setToolTip(strokeShade_->toolTip());
	backIcon->setToolTip(backColor_->toolTip());
	backShadeLabel->setToolTip(backShade_->toolTip());

// 	fontVScale_->setSuffix( tr(" %"));
// 	fontHScale_->setSuffix( tr(" %"));
// 	baselineOffset_->setSuffix( tr(" %"));
// 	tracking_->setSuffix( tr(" %"));
	fontSize_->setSuffix(unitGetSuffixFromIndex(0));
}
/*
void SMCStyleWidget::fillLangCombo(QMap<QString,QString> langMap)
{
	QStringList sortList;
	QMap<QString,QString>::Iterator it;

	m_langMap = langMap;

	language_->clear();

	for (it = langMap.begin(); it != langMap.end(); ++it)
		sortList.push_back(it.value());

	language_->addItems(sortQStringList(sortList));

	QListView *tmpView = dynamic_cast<QListView*>(language_->view()); Q_ASSERT(tmpView);
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
}
*/
void SMCStyleWidget::fillLangComboFromList(QStringList langList)
{
	language_->clear();
	language_->addItems(langList);

	QListView *tmpView = dynamic_cast<QListView*>(language_->view()); Q_ASSERT(tmpView);
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
}

void SMCStyleWidget::fillColorCombo(ColorList &colors)
{
	fillColor_->clear();
	strokeColor_->clear();
	backColor_->clear();

	fillColor_->setColors(colors, true);
	strokeColor_->setColors(colors, true);
	backColor_->setColors(colors, true);
	fillColor_->view()->setMinimumWidth(fillColor_->view()->maximumViewportSize().width()+24);
	strokeColor_->view()->setMinimumWidth(strokeColor_->view()->maximumViewportSize().width()+24);
	backColor_->view()->setMinimumWidth(backColor_->view()->maximumViewportSize().width()+24);
}

void SMCStyleWidget::handleUpdateRequest(int updateFlags)
{
	if (!m_Doc)
		return;
	if (updateFlags & reqColorsUpdate)
		fillColorCombo(m_Doc->PageColors);
}

void SMCStyleWidget::slotEnableFontFeatures(QString s)
{
	const ScFace& font = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[s];
	fontfeaturesSetting->enableFontFeatures(font.fontFeatures());
}

void SMCStyleWidget::setDoc(ScribusDoc *doc)
{
	if (m_Doc)
		disconnect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));

	m_Doc = doc;
	if (!m_Doc)
		return;

	QStringList languageList;
	LanguageManager::instance()->fillInstalledStringList(&languageList);
	fillLangComboFromList(languageList);
	fillColorCombo(m_Doc->PageColors);
	fontFace_->RebuildList(m_Doc);

	connect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
}

void SMCStyleWidget::show(CharStyle *cstyle, QList<CharStyle> &cstyles, const QString &defLang, int unitIndex)
{
	disconnect(effects_, SIGNAL(State(int)), this, SLOT(slotColorChange()));
	parentCombo->setEnabled(!cstyle->isDefaultStyle());
	const CharStyle *parent = dynamic_cast<const CharStyle*>(cstyle->parentStyle());
	bool hasParent =  cstyle->hasParent() && parent != 0 && parent->hasName() && cstyle->parent() != "";
	if (hasParent)
	{
		fontSize_->setValue(cstyle->fontSize() / 10.0, cstyle->isInhFontSize());
		fontSize_->setParentValue(parent->fontSize() / 10.0);

		fontHScale_->setValue(cstyle->scaleH() / 10.0, cstyle->isInhScaleH());
		fontHScale_->setParentValue(parent->scaleH() / 10.0);

		fontVScale_->setValue(cstyle->scaleV() / 10.0, cstyle->isInhScaleV());
		fontVScale_->setParentValue(parent->scaleV() / 10.0);

		baselineOffset_->setValue(cstyle->baselineOffset() / 10.0, cstyle->isInhBaselineOffset());
		baselineOffset_->setParentValue(parent->baselineOffset() / 10.0);

		tracking_->setValue(cstyle->tracking() / 10.0, cstyle->isInhTracking());
		tracking_->setParentValue(parent->tracking() / 10.0);
		
		widthSpaceSpin->setValue(cstyle->wordTracking() * 100.0 ,cstyle->isInhTracking());
		widthSpaceSpin->setParentValue(parent->wordTracking() * 100.0);

		effects_->setStyle(static_cast<int>(cstyle->effects()), cstyle->isInhFeatures());
		effects_->setParentItem(static_cast<int>(parent->effects())); // FIXME

		fillShade_->setValue(qRound(cstyle->fillShade()), cstyle->isInhFillShade());
		fillShade_->setParentValue(qRound(parent->fillShade()));

		strokeShade_->setValue(qRound(cstyle->strokeShade()), cstyle->isInhStrokeShade());
		strokeShade_->setParentValue(qRound(parent->strokeShade()));

		fillColor_->setCurrentText(cstyle->fillColor(), cstyle->isInhFillColor());
		fillColor_->setParentText(parent->fillColor());

		strokeColor_->setCurrentText(cstyle->strokeColor(), cstyle->isInhStrokeColor());
		strokeColor_->setParentText(parent->strokeColor());

		backColor_->setCurrentText(cstyle->backColor(), cstyle->isInhBackColor());
		backColor_->setParentText(parent->backColor());

		backShade_->setValue(qRound(cstyle->backShade()), cstyle->isInhBackShade());
		backShade_->setParentValue(qRound(parent->backShade()));

		fontFace_->setCurrentFont(cstyle->font().scName(), cstyle->isInhFont());
		fontFace_->setParentFont(parent->font().scName());

		fontfeaturesSetting->setFontFeatures(cstyle->fontFeatures(), cstyle->font().fontFeatures());
		fontfeaturesSetting->setParentValue(parent->fontFeatures());

		smallestWordSpinBox->setValue(cstyle->hyphenWordMin(), cstyle->isInhHyphenWordMin());
		smallestWordSpinBox->setParentValue(parent->hyphenWordMin());

		hyphenCharLineEdit->setValue(QString::fromUcs4(&cstyle->hyphenChar(), 1), cstyle->isInhHyphenChar());
		hyphenCharLineEdit->setParentValue(QString::fromUcs4(&parent->hyphenChar(), 1));
	}
	else
	{
		fontSize_->setValue(cstyle->fontSize() / 10.0);
		fontHScale_->setValue(cstyle->scaleH() / 10.0);
		fontVScale_->setValue(cstyle->scaleV() / 10.0);
		baselineOffset_->setValue(cstyle->baselineOffset() / 10.0);
		tracking_->setValue(cstyle->tracking() / 10.0);
		widthSpaceSpin->setValue(cstyle->wordTracking() * 100.0);
		effects_->setStyle(static_cast<int>(cstyle->effects()));
		fillShade_->setValue(qRound(cstyle->fillShade()));
		strokeShade_->setValue(qRound(cstyle->strokeShade()));
		fillColor_->setCurrentText(cstyle->fillColor());
		strokeColor_->setCurrentText(cstyle->strokeColor());
		backColor_->setCurrentText(cstyle->backColor());
		backShade_->setValue(qRound(cstyle->backShade()));
		fontFace_->setCurrentFont(cstyle->font().scName());
		fontfeaturesSetting->setFontFeatures(cstyle->fontFeatures(), cstyle->font().fontFeatures());
		smallestWordSpinBox->setValue(cstyle->hyphenWordMin());
		hyphenCharLineEdit->setValue(QString::fromUcs4(&cstyle->hyphenChar(), 1));
	}

	effects_->ShadowVal->Xoffset->setValue(cstyle->shadowXOffset() / 10.0);
	effects_->ShadowVal->Yoffset->setValue(cstyle->shadowYOffset() / 10.0);
	effects_->OutlineVal->LWidth->setValue(cstyle->outlineWidth() / 10.0);
	effects_->StrikeVal->LPos->setValue(cstyle->strikethruOffset() / 10.0);
	effects_->StrikeVal->LWidth->setValue(cstyle->strikethruWidth() / 10.0);
	effects_->UnderlineVal->LPos->setValue(cstyle->underlineOffset() / 10.0);
	effects_->UnderlineVal->LWidth->setValue(cstyle->underlineWidth() / 10.0);
	slotColorChange();

	parentCombo->clear();
	parentCombo->addItem( cstyle->isDefaultStyle()? tr("A default style cannot be assigned a parent style") : "");
	if (!cstyle->isDefaultStyle())
	{
		for (int i = 0; i < cstyles.count(); ++i)
		{
			if (cstyles[i].name() != cstyle->name())
				parentCombo->addItem(cstyles[i].name());
		}
	}

	if (cstyle->isDefaultStyle() || !hasParent)
		parentCombo->setCurrentIndex(0);
	else if (hasParent)
	{
		int index = 0;
		for (int i = 0; i < parentCombo->count(); ++i)
		{
			if (parentCombo->itemText(i) == cstyle->parentStyle()->name())
			{
				index = i;
				break;
			}
		}
		parentCombo->setCurrentIndex(index);
	}

	QString defaultLang(defLang.isEmpty() ? "en_GB" : defLang);
	QString clang(cstyle->language().isEmpty() ? defaultLang : cstyle->language());
	QString plang(QString::null);
	if (hasParent)
		plang = parent->language().isEmpty() ? defaultLang : parent->language();

	int ci = -1, pi = -1, di = -1;
	QString tl = LanguageManager::instance()->getAbbrevFromLang(defaultLang, false);
	for (int i = 0; i < language_->count(); ++i)
	{
		QString ltAbbrev(LanguageManager::instance()->getAbbrevFromLang(language_->itemText(i), false));
		if (ltAbbrev == clang)
			ci = i;
		if (hasParent && ltAbbrev == plang)
			pi = i;
		if (ltAbbrev == defaultLang || ltAbbrev == tl)
			di = i;
	}

	//qDebug() << QString("SMCStyleWidget::show(): deflan='%1'->'%2'").arg(defLang).arg(langMap_[defLang]);
	Q_ASSERT(di != -1);

	if (hasParent)
	{
		if (ci == -1)
			language_->setCurrentItem(di, cstyle->isInhLanguage());
		else
			language_->setCurrentItem(ci, cstyle->isInhLanguage());

		language_->setParentItem(pi);
	}
	else
	{
		if (ci == -1)
			language_->setCurrentIndex(di);
		else
			language_->setCurrentIndex(ci);
	}

	connect(effects_, SIGNAL(State(int)), this, SLOT(slotColorChange()));
}

void SMCStyleWidget::show(QList<CharStyle*> &cstyles, QList<CharStyle> &cstylesAll, const QString &defLang, int unitIndex)
{
// 	int decimals = unitGetDecimalsFromIndex(unitIndex);
// 	QString suffix = unitGetSuffixFromIndex(unitIndex);

	if (cstyles.count() == 1)
		show(cstyles[0], cstylesAll, defLang, unitIndex);
	else if (cstyles.count() > 1)
	{
		showSizeAndPosition(cstyles);
		showEffects(cstyles);
		showColors(cstyles);
		showLanguage(cstyles, defLang);
		showSmallestWord(cstyles);
		showHyphenChar(cstyles);
		showParent(cstyles);
		showFontFeatures(cstyles);
	}
}

void SMCStyleWidget::showSizeAndPosition(const QList<CharStyle*> &cstyles)
{
	double d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->fontSize() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->fontSize();
	}
	if (d == -30000)
		fontSize_->clear();
	else
		fontSize_->setValue(d / 10.0);

	d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->scaleH() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->scaleH();
	}
	if (d == -30000)
		fontHScale_->clear();
	else
		fontHScale_->setValue(d / 10.0);

	d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->scaleV() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->scaleV();
	}
	if (d == -30000)
		fontVScale_->clear();
	else
		fontVScale_->setValue(d / 10.0);

	d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->baselineOffset() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->baselineOffset();
	}
	if (d == -30000)
		baselineOffset_->clear();
	else
		baselineOffset_->setValue(d / 10.0);

	d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->tracking() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->tracking();
	}
	if (d == -30000)
		tracking_->clear();
	else
		tracking_->setValue(d / 10.0);
	
	d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->wordTracking() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->wordTracking();
	}
	if (d == -30000)
		widthSpaceSpin->clear();
	else
		widthSpaceSpin->setValue(d * 100.0);
}

void SMCStyleWidget::showEffects(const QList<CharStyle*> &cstyles)
{
	int d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && static_cast<int>(cstyles[i]->effects()) != d)
		{
			d = -30000;
			break;
		}
		else
			d = static_cast<int>(cstyles[i]->effects());
	}
	if (d == -30000)
		effects_->setStyle(0);
	else
		effects_->setStyle(d);

// 	TODO these things still missing:
// 		effects_->ShadowVal->Xoffset->setValue(cstyle->shadowXOffset() / 10.0);
// 		effects_->ShadowVal->Yoffset->setValue(cstyle->shadowYOffset() / 10.0);
// 		effects_->OutlineVal->LWidth->setValue(cstyle->outlineWidth() / 10.0);
// 		effects_->StrikeVal->LPos->setValue(cstyle->strikethruOffset() / 10.0);
// 		effects_->StrikeVal->LWidth->setValue(cstyle->strikethruWidth() / 10.0);
// 		effects_->UnderlineVal->LPos->setValue(cstyle->underlineOffset() / 10.0);
// 		effects_->UnderlineVal->LWidth->setValue(cstyle->underlineWidth() / 10.0);
}

void SMCStyleWidget::showColors(const QList<CharStyle*> &cstyles)
{
	strokeShade_->setEnabled(true);
	strokeColor_->setEnabled(true);

	double d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->fillShade() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->fillShade();
	}
	if (d == -30000)
		fillShade_->setText( tr("Shade"));
	else
		fillShade_->setValue(qRound(d));

	d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->strokeShade() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->strokeShade();
	}
	if (d == -30000)
	{
		strokeShade_->setValue(21);
		strokeShade_->setText( tr("Shade"));
	}
	else
		strokeShade_->setValue(qRound(d));

	d = -30000;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (d != -30000 && cstyles[i]->backShade() != d)
		{
			d = -30000;
			break;
		}
		else
			d = cstyles[i]->backShade();
	}
	if (d == -30000)
	{
		backShade_->setValue(21);
		backShade_->setText( tr("Shade"));
	}
	else
		backShade_->setValue(qRound(d));

	QString s;
	QString emptyString;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (!s.isNull() && s != cstyles[i]->fillColor())
		{
			s = emptyString;
			break;
		}
		else
			s = cstyles[i]->fillColor();
	}
	if (s.isEmpty())
	{
		if (fillColor_->itemText(fillColor_->count() - 1) != "")
			fillColor_->addItem("");
		fillColor_->setCurrentIndex(fillColor_->count() - 1);
	}
	else
		fillColor_->setCurrentText(s);

	s = emptyString;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (!s.isNull() && s != cstyles[i]->strokeColor())
		{
			s = emptyString;
			break;
		}
		else
			s = cstyles[i]->strokeColor();
	}
	if (s.isEmpty())
	{
		if (strokeColor_->itemText(strokeColor_->count() - 1) != "")
			strokeColor_->addItem("");
		strokeColor_->setCurrentIndex(strokeColor_->count() - 1);
	}
	else
		strokeColor_->setCurrentText(s);

	s = emptyString;
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (!s.isNull() && s != cstyles[i]->backColor())
		{
			s = emptyString;
			break;
		}
		else
			s = cstyles[i]->backColor();
	}
	if (s.isEmpty())
	{
		if (backColor_->itemText(backColor_->count() - 1) != "")
			backColor_->addItem("");
		backColor_->setCurrentIndex(backColor_->count() - 1);
	}
	else
		backColor_->setCurrentText(s);
}

void SMCStyleWidget::showLanguage(const QList<CharStyle*> &cstyles, const QString &defLang)
{
	QString emptyString;
	QString s(cstyles[0]->language());
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (s != cstyles[i]->language())
		{
			s = emptyString;
			break;
		}
		else
			s = cstyles[i]->language();
	}
	if (s.isEmpty())
	{
		if (language_->itemText(language_->count() - 1) != "")
			language_->addItem("");
		language_->setCurrentIndex(language_->count() - 1);
	}
	else
		setCurrentComboItem(language_, LanguageManager::instance()->getLangFromAbbrev(s));
}

void SMCStyleWidget::showSmallestWord(const QList<CharStyle *> &cstyles)
{
	int tmp = 0;
	int s(cstyles[0]->hyphenWordMin());
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (s != cstyles[i]->hyphenWordMin())
		{
			s = tmp;
			break;
		}
		else
			s = cstyles[i]->hyphenWordMin();
	}
	smallestWordSpinBox->setValue(s);
}

void SMCStyleWidget::showHyphenChar(const QList<CharStyle *> &cstyles)
{
	uint empty = 0;
	uint ch(cstyles[0]->hyphenChar());
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (ch != cstyles[i]->hyphenChar())
		{
			ch = empty;
			break;
		}
		else
			ch = cstyles[i]->hyphenChar();
	}
	hyphenCharLineEdit->setValue(QString::fromUcs4(&ch, 1));
}

void SMCStyleWidget::showParent(const QList<CharStyle*> &cstyles)
{
	parentCombo->setEnabled(false);
}

void SMCStyleWidget::showFontFeatures(const QList<CharStyle *> &cstyles)
{
	QString tmp;
	QStringList tmp2;
	QString fontfeatures(cstyles[0]->fontFeatures());
	QStringList featuresList(cstyles[0]->font().fontFeatures());
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (fontfeatures != cstyles[i]->fontFeatures())
		{
			fontfeatures = tmp;
			featuresList = tmp2;
			break;
		}
		else
		{
			fontfeatures = cstyles[i]->fontFeatures();
			featuresList = cstyles[i]->font().fontFeatures();
		}
	}
	fontfeaturesSetting->setFontFeatures(fontfeatures, featuresList);
}

void SMCStyleWidget::clearAll()
{
	
}

void SMCStyleWidget::slotColorChange()
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


SMCStyleWidget::~SMCStyleWidget()
{
	
}
