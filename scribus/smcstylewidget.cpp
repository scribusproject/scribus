/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>

#include "langmgr.h"
#include "smcstylewidget.h"
#include "util_icon.h"
#include "util.h"
#include "units.h"


SMCStyleWidget::SMCStyleWidget(QWidget *parent) : QWidget()
{
	setupUi(this);

// 	fontSize_->setMinimum(1.0);
// 	fontSize_->setMaximum(2048.0);
	fontSizeLabel_->setPixmap(loadIcon("Zeichen.xpm"));

	trackingLabel_->setPixmap( loadIcon("textkern.png") );
// 	tracking_->setMinimum(-300.0);
// 	tracking_->setMaximum(300.0);
// 	tracking_->setSuffix( tr( " %" ) );

	baselineOffsetLabel_->setPixmap( loadIcon("textbase.png") );
// 	baselineOffset_->setMinimum(-100.0);
// 	baselineOffset_->setMaximum(100.0);
// 	baselineOffset_->setSuffix( tr( " %" ) );

	hscaleLabel_->setPixmap( loadIcon("textscaleh.png") );
// 	fontHScale_->setMinimum(10.0);
// 	fontHScale_->setMaximum(400.0);
// 	fontHScale_->setSuffix( tr( " %" ) );

	vscaleLabel_->setPixmap( loadIcon("textscalev.png") );
// 	fontVScale_->setMinimum(10.0);
// 	fontVScale_->setMaximum(400.0);
// 	fontVScale_->setSuffix( tr( " %" ) );

	FillIcon->setPixmap(loadIcon("16/color-fill.png"));
	fillShadeLabel->setPixmap( loadIcon("shade.png") );
	StrokeIcon->setPixmap(loadIcon("16/color-stroke.png"));
	strokeShadeLabel->setPixmap( loadIcon("shade.png") );

	fillColor_->clear();
	strokeColor_->clear();
	ColorList::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	fillColor_->addItem(CommonStrings::tr_NoneColor);
	strokeColor_->addItem(CommonStrings::tr_NoneColor);
	StrokeIcon->setEnabled(false);
	strokeShade_->setEnabled(false);
	strokeColor_->setEnabled(false);

	connect(effects_, SIGNAL(State(int)), this, SLOT(slotColorChange()));
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
/***********************************/
/*      Begin Tooltips             */
/***********************************/
// These are for the character style page
// as in character styles and in paragraph style's character style

	parentCombo->setToolTip(     tr("Parent Style"));
	fontFace_->setToolTip(       tr("Font Face"));
	fontSize_->setToolTip(       tr("Font Size"));
	tracking_->setToolTip(       tr("Tracking"));
	baselineOffset_->setToolTip( tr("Baseline Offset"));
	fontHScale_->setToolTip(     tr("Horizontal Scaling"));
	fontVScale_->setToolTip(     tr("Vertical Scaling"));
	language_->setToolTip(       tr("Language"));
	fillColor_->setToolTip(      tr("Fill Color"));
	fillShade_->setToolTip(      tr("Fill Shade"));
	strokeColor_->setToolTip(    tr("Stroke Color"));
	strokeShade_->setToolTip(    tr("Stroke Shade"));

	fontSizeLabel_->setToolTip(fontSize_->toolTip());
	trackingLabel_->setToolTip(tracking_->toolTip());
	baselineOffsetLabel_->setToolTip(baselineOffset_->toolTip());
	hscaleLabel_->setToolTip(fontHScale_->toolTip());
	vscaleLabel_->setToolTip(fontVScale_->toolTip());
	FillIcon->setToolTip(fillColor_->toolTip());
	fillShadeLabel->setToolTip(fillShade_->toolTip());
	StrokeIcon->setToolTip(strokeColor_->toolTip());
	strokeShadeLabel->setToolTip(strokeShade_->toolTip());

/***********************************/
/*        End Tooltips             */
/***********************************/

	parentLabel->setText( tr("Based On:"));
	languageLabel_->setText( tr("Language:"));
// 	fontVScale_->setSuffix( tr(" %"));
// 	fontHScale_->setSuffix( tr(" %"));
// 	baselineOffset_->setSuffix( tr(" %"));
// 	tracking_->setSuffix( tr(" %"));
	fontSize_->setSuffix(unitGetSuffixFromIndex(0));
}

void SMCStyleWidget::fillLangCombo(QMap<QString,QString> langMap)
{
	QStringList sortList;
	QMap<QString,QString>::Iterator it;

	langMap_ = langMap;

	language_->clear();

	for (it = langMap.begin(); it != langMap.end(); ++it)
		sortList.push_back(it.value());

	language_->addItems(sortQStringList(sortList));

	QListView *tmpView = dynamic_cast<QListView*>(language_->view()); Q_ASSERT(tmpView);
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
}

void SMCStyleWidget::fillColorCombo(ColorList &colors)
{
	fillColor_->clear();
	strokeColor_->clear();

	fillColor_->addItem(CommonStrings::tr_NoneColor);
	strokeColor_->addItem(CommonStrings::tr_NoneColor);
	ColorList::Iterator itend=colors.end();
	ScribusDoc* doc = colors.document();
	for (ColorList::Iterator it = colors.begin(); it != itend; ++it)
	{
		fillColor_->insertSmallItem(it.value(), doc, it.key());
		strokeColor_->insertSmallItem(it.value(), doc, it.key());
	}
	fillColor_->view()->setMinimumWidth(fillColor_->view()->maximumViewportSize().width()+24);
	strokeColor_->view()->setMinimumWidth(strokeColor_->view()->maximumViewportSize().width()+24);
}

void SMCStyleWidget::show(CharStyle *cstyle, QList<CharStyle> &cstyles, const QString &defLang, int unitIndex)
{
	disconnect(effects_, SIGNAL(State(int)), this, SLOT(slotColorChange()));
	parentCombo->setEnabled(true);
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

		fontFace_->setCurrentFont(cstyle->font().scName(), cstyle->isInhFont());
		fontFace_->setParentFont(parent->font().scName());
	}
	else
	{
		fontSize_->setValue(cstyle->fontSize() / 10.0);
		fontHScale_->setValue(cstyle->scaleH() / 10.0);
		fontVScale_->setValue(cstyle->scaleV() / 10.0);
		baselineOffset_->setValue(cstyle->baselineOffset() / 10.0);
		tracking_->setValue(cstyle->tracking() / 10.0);
		effects_->setStyle(static_cast<int>(cstyle->effects()));
		fillShade_->setValue(qRound(cstyle->fillShade()));
		strokeShade_->setValue(qRound(cstyle->strokeShade()));
		fillColor_->setCurrentText(cstyle->fillColor());
		strokeColor_->setCurrentText(cstyle->strokeColor());
		fontFace_->setCurrentFont(cstyle->font().scName());
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
	parentCombo->addItem("");
	for (int i = 0; i < cstyles.count(); ++i)
	{
		if (cstyles[i].name() != cstyle->name())
			parentCombo->addItem(cstyles[i].name());
	}

	if (hasParent)
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
	else
		parentCombo->setCurrentIndex(0);

	QString clang = cstyle->language().isNull() || cstyle->language().isEmpty() ?
	                                      defLang : cstyle->language();
	QString plang(QString::null);
	if (hasParent)
		plang = parent->language().isNull() || parent->language().isEmpty() ?
		                              defLang : parent->language();

	int ci = -1, pi = -1, di = -1;
	LanguageManager langmgr;
	langmgr.init(true);
	QString tl;
	for (int i = 0; i < language_->count(); ++i)
	{
		if (language_->itemText(i) == langMap_[clang])
			ci = i;
		
		if (hasParent && language_->itemText(i) == langMap_[plang])
			pi = i;
		tl=langmgr.getTransLangFromLang(defLang);
// 		qDebug() << i << language_->itemText(i) << defLang << langMap_[defLang] << tl;
		if (language_->itemText(i) == defLang || language_->itemText(i) == langMap_[defLang] || language_->itemText(i) == tl)
// 		{
			di = i;
// 			qDebug() << "match on:" << di;
// 		}
	}

//	qDebug(QString("SMCStyleWidget::show(): deflan='%1'->'%2'").arg(defLang).arg(langMap_[defLang]));
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
		showParent(cstyles);
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
		strokeColor_->setCurrentIndex(fillColor_->count() - 1);
	}
	else
		strokeColor_->setCurrentText(s);
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
	{
		Q_ASSERT(langMap_.contains(s));
		setCurrentComboItem(language_, langMap_[s]);
	}
}

void SMCStyleWidget::showParent(const QList<CharStyle*> &cstyles)
{
	parentCombo->setEnabled(false);
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
