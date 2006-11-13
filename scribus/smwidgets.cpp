/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smwidgets.h"
#include "smwidgets.moc"
#include <qtooltip.h>
#include <qlineedit.h>
#include "units.h"

/***********************************************************************/
/***********************************************************************/

SMSpinBox::SMSpinBox(QWidget *parent, const char *name)
: QSpinBox(parent, name),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0)
{

}

void SMSpinBox::setValue(int val)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	hasParent_ = false;
	pValue_ = 0;
	setFont(false);

	QSpinBox::setValue(val);
}

void SMSpinBox::setValue(int val, bool isParentVal)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	hasParent_ = true;
	pValue_ = val;
	setFont(!isParentVal);

	QSpinBox::setValue(val);
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
}

void SMSpinBox::clear()
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	editor()->clear();
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
}

void SMSpinBox::setParentValue(int val)
{
	hasParent_ = true;
	pValue_ = val;
}

bool SMSpinBox::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;
	return ret;
}

void SMSpinBox::interpretText()
{
	QString t = text();
	if (hasParent_ && (t == "" || t.isEmpty() || t == QString::null))
	{
		useParentValue_ = true;
		setValue(pValue_, true);
	}
}

void SMSpinBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	QSpinBox::setFont(f);
}

void SMSpinBox::slotValueChanged()
{
	if(hasParent_)
		setFont(true);
}

/***********************************************************************/
/***********************************************************************/

SMMSpinBox::SMMSpinBox(QWidget *pa, int s)
: MSpinBox(pa, s),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

SMMSpinBox::SMMSpinBox(double minValue, double maxValue, QWidget *pa, int s)
: MSpinBox(minValue, maxValue, pa, s),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

SMMSpinBox::SMMSpinBox(QWidget *parent, const char * name)
: MSpinBox(parent, name),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

void SMMSpinBox::setValue(double val)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	hasParent_ = false;
	pValue_ = 0.0;
	setFont(false);

	MSpinBox::setValue(val);
}

void SMMSpinBox::setValue(double val, bool isParentVal)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	hasParent_ = true;
	pValue_ = val;
	setFont(!isParentVal);

	MSpinBox::setValue(val);
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
}

void SMMSpinBox::clear()
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	editor()->clear();
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
}

void SMMSpinBox::setParentValue(double val)
{
	hasParent_ = true;
	pValue_ = val;
}

bool SMMSpinBox::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;
	return ret;
}

void SMMSpinBox::interpretText()
{
	QString t = text();
	if (hasParent_ && (t == "" || t.isEmpty() || t == QString::null))
	{
		useParentValue_ = true;
		setValue(pValue_, true);
	}
}

void SMMSpinBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	MSpinBox::setFont(f);
}

void SMMSpinBox::slotValueChanged()
{
	if(hasParent_)
		setFont(true);
}

/***********************************************************************/
/***********************************************************************/

SMScComboBox::SMScComboBox(QWidget *parent, const char *name)
: ScComboBox(parent, name),
  hasParent_(false),
  useParentValue_(false),
  pItem_(0)
{
	
}

SMScComboBox::SMScComboBox(bool rw, QWidget* parent, const char* name)
: ScComboBox(rw, parent, name),
  hasParent_(false),
  useParentValue_(false),
  pItem_(0)
{
	
}

void SMScComboBox::setCurrentItem(int i)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pItem_ = 0;
	ScComboBox::setCurrentItem(i);
}

void SMScComboBox::setCurrentItem(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	pItem_ = i;
	setFont(!isParentValue);
	if (!isParentValue)
	{
		useParentValue_ = true;
		insertItem( tr("Use Parent Value"));
	}

	ScComboBox::setCurrentItem(i);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setParentItem(int i)
{
	hasParent_ = true;
	pItem_ = i;
}

bool SMScComboBox::useParentValue()
{
	bool ret = false;

	if (useParentValue_ && hasParent_)
	{
		ret = currentItem() == (count() - 1);
		if (ret)
		{
			removeItem(count() - 1);
			setFont(false);
			setCurrentItem(pItem_, true);
			useParentValue_ = false;
		}
	}

	return ret;
}

void SMScComboBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	ScComboBox::setFont(f);
}

void SMScComboBox::currentChanged()
{
	if (hasParent_ && !useParentValue_)
	{
		setFont(true);
		insertItem( tr("Use Parent Value"));
		useParentValue_ = true;
	}
}

/***********************************************************************/
/***********************************************************************/

SMAlignSelect::SMAlignSelect(QWidget *parent)
: AlignSelect(parent),
  hasParent_(false),
  useParentStyle_(false),
  pStyle_(0)
{
	parentButton = new QToolButton(this, "parentButton");
	parentButton->setMaximumSize( QSize( 22, 22 ) );
	parentButton->setToggleButton( true );
	parentButton->setText( tr("P", "P as in Parent"));
	QToolTip::add(parentButton, tr("Use parent style's alignment instead of overriding it"));
	GroupAlignLayout->addWidget( parentButton, 0, 5 );
	resize(minimumSizeHint());
	parentButton->hide();
}

void SMAlignSelect::setStyle(int i)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	setFont(false);
	hasParent_ = false;
	pStyle_ = 0;
	parentButton->hide();
	AlignSelect::setStyle(i);
}

void SMAlignSelect::setStyle(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	hasParent_ = true;
	pStyle_ = i;
	setFont(!isParentValue);
	if (isParentValue)
		parentButton->hide();
	else
		parentButton->show();

	AlignSelect::setStyle(i);
	connect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMAlignSelect::setParentItem(int i)
{
	hasParent_ = true;
	pStyle_ = i;
}

bool SMAlignSelect::useParentValue()
{
	bool ret = useParentStyle_;
	useParentStyle_ = false;
	if (ret)
		setStyle(pStyle_, true);

	return ret;
}

void SMAlignSelect::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	parentButton->setFont(f);
}

void SMAlignSelect::styleChanged()
{
	if (hasParent_)
	{
		setFont(true);
		parentButton->show();
	}
}

void SMAlignSelect::pbPressed()
{
	useParentStyle_ = true;
}

/***********************************************************************/
/***********************************************************************/

SMStyleSelect::SMStyleSelect(QWidget *parent)
: StyleSelect(parent),
  hasParent_(false),
  useParentStyle_(false),
  pStyle_(0)
{
	parentButton = new QToolButton(this, "parentButton");
	parentButton->setMaximumSize(QSize(22, 22));
	parentButton->setMinimumSize(QSize(22, 22));
	parentButton->setText( tr("P", "P as in Parent"));
	QToolTip::add(parentButton, tr("Use parent style's effects instead of overriding them"));
	ssLayout->addWidget( parentButton, 0, 5 );
	resize(minimumSizeHint());
	parentButton->hide();
}

void SMStyleSelect::setStyle(int i)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(ShadowVal->Xoffset, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(ShadowVal->Yoffset, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(OutlineVal->LWidth, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(UnderlineVal->LPos, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(UnderlineVal->LWidth, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(styleChanged()));
	disconnect(StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	setFont(false);
	hasParent_ = false;
	pStyle_ = 0;
	parentButton->hide();
	StyleSelect::setStyle(i);
}

void SMStyleSelect::setStyle(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(ShadowVal->Xoffset, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(ShadowVal->Yoffset, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(OutlineVal->LWidth, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(UnderlineVal->LPos, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(UnderlineVal->LWidth, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	disconnect(StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(styleChanged()));
	disconnect(StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	hasParent_ = true;
	pStyle_ = i;
	setFont(!isParentValue);
	if (isParentValue)
		parentButton->hide();
	else
		parentButton->show();
	parentButton->setOn(true);
	StyleSelect::setStyle(i);
	connect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	connect(ShadowVal->Xoffset, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	connect(ShadowVal->Yoffset, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	connect(OutlineVal->LWidth, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	connect(UnderlineVal->LPos, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	connect(UnderlineVal->LWidth, SIGNAL(valueChanged(int)),this, SLOT(styleChanged()));
	connect(StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(styleChanged()));
	connect(StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMStyleSelect::setParentItem(int i)
{
	hasParent_ = true;
	pStyle_ = i;
}

bool SMStyleSelect::useParentValue()
{
	bool ret = useParentStyle_;
	useParentStyle_ = false;
	if (ret)
		setStyle(pStyle_, true);

	return ret;
}

void SMStyleSelect::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	parentButton->setFont(f);
	ShadowVal->Xoffset->setFont(f);
	ShadowVal->Yoffset->setFont(f);
	OutlineVal->LWidth->setFont(f);
	UnderlineVal->LPos->setFont(f);
	UnderlineVal->LWidth->setFont(f);
	StrikeVal->LPos->setFont(f);
	StrikeVal->LWidth->setFont(f);
	StyleSelect::setFont(f);
}

void SMStyleSelect::styleChanged()
{
	if (hasParent_)
	{
		setFont(true);
		parentButton->show();
	}
}

void SMStyleSelect::pbPressed()
{
	useParentStyle_ = true;
	emit State(getStyle());
}

/***********************************************************************/
/***********************************************************************/

/***********************************************************************/
/***********************************************************************/

SMShadeButton::SMShadeButton(QWidget *parent)
: ShadeButton(parent),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0)
{
	
}

void SMShadeButton::setValue(int i)
{
	disconnect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pValue_ = 0;
	ShadeButton::setValue(i);
}

void SMShadeButton::setValue(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
	hasParent_ = true;
	pValue_ = i;
	setFont(!isParentValue);
	ShadeButton::setValue(i);
	connect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
}

void SMShadeButton::setParentValue(int i)
{
	hasParent_ = true;
	pValue_ = i;
}

bool SMShadeButton::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;

	if (ret)
	{
		setValue(pValue_, true);
		FillSh->removeItemAt(FillSh->count() - 1);
	}
	

	return ret;
}

void SMShadeButton::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	FillSh->setFont(f);
	ShadeButton::setFont(f);
}

void SMShadeButton::currentChanged()
{
	if (hasParent_)
	{
		setFont(true);
		QString upv = tr("Use Parent Value");
		if (FillSh->text(FillSh->idAt(FillSh->count() - 1)) != upv)
			FillSh->insertItem(upv, this, SLOT(slotUseParent()));
	}
}

void SMShadeButton::slotUseParent()
{
	useParentValue_ = true;
	FillSh->removeItemAt(FillSh->count() - 1);
	emit clicked();
}

/***********************************************************************/
/***********************************************************************/

/***********************************************************************/
/***********************************************************************/

SMColorCombo::SMColorCombo(QWidget *parent, const char *name)
: ColorCombo(parent, name),
  hasParent_(false),
  useParentValue_(false),
  pItem_(0)
{
	
}

SMColorCombo::SMColorCombo(bool rw, QWidget* parent, const char* name)
: ColorCombo(rw, parent, name),
  hasParent_(false),
  useParentValue_(false),
  pItem_(0),
  pText_(QString::null)
{
	
}

void SMColorCombo::setCurrentItem(int i)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pItem_ = 0;
	pText_ = QString::null;
	ColorCombo::setCurrentItem(i);
}

void SMColorCombo::setCurrentItem(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	pItem_ = i;
	pText_ = QString::null;
	ColorCombo::setCurrentItem(i);
	setFont(!isParentValue);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMColorCombo::setCurrentText(const QString &s)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pItem_ = -1;
	pText_ = s;
	ColorCombo::setCurrentText(s);
}

void SMColorCombo::setCurrentText(const QString &s, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	pItem_ = -1;
	pText_ = s;
	ColorCombo::setCurrentText(s);
	setFont(!isParentValue);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMColorCombo::setParentItem(int i)
{
	hasParent_ = true;
	pItem_ = i;
	pText_ = QString::null;
}

void SMColorCombo::setParentText(const QString &s)
{
	hasParent_ = true;
	pText_ = s;
}

bool SMColorCombo::useParentValue()
{
	bool ret = false;

	if (useParentValue_ && hasParent_)
	{
		ret = currentItem() == (count() - 1);
		if (ret)
		{
			removeItem(count() - 1);
			setFont(false);
			if (pText_ != QString::null)
				setCurrentText(pText_, true);
			else
				setCurrentItem(pItem_, true);
			useParentValue_ = false;
		}
	}

	return ret;
}

void SMColorCombo::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	ColorCombo::setFont(f);
}

void SMColorCombo::currentChanged()
{
	if (hasParent_ && !useParentValue_)
	{
		setFont(true);
		insertItem( tr("Use Parent Value"));
		useParentValue_ = true;
	}
}


/***********************************************************************/
/***********************************************************************/

SMFontComboH::SMFontComboH(QWidget *parent)
: FontComboH(parent),
  hasParent_(false),
  useParentValue_(false),
  pFont_(QString::null),
  usePFont_( tr("Use Parent Font"))
{
	
}

void SMFontComboH::setCurrentFont(const QString &s)
{
	disconnect(fontFamily, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	disconnect(fontStyle, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pFont_ = s;
	FontComboH::setCurrentFont(s);
}

void SMFontComboH::setCurrentFont(const QString &s, bool isParentValue)
{
	disconnect(fontFamily, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	disconnect(fontStyle, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	pFont_ = s;
	FontComboH::setCurrentFont(s);
	setFont(!isParentValue);
	connect(fontFamily, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	connect(fontStyle, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMFontComboH::setParentFont(const QString &s)
{
	hasParent_ = true;
	pFont_ = s;
}

bool SMFontComboH::useParentFont()
{
	bool ret = false;

	if (useParentValue_ && hasParent_)
	{
		ret = fontFamily->currentItem() == (fontFamily->count() - 1) ||
				fontStyle->currentItem() == (fontStyle->count() - 1);

		if (ret)
		{
			fontFamily->removeItem(fontFamily->count() - 1);
			fontStyle->removeItem(fontStyle->count() - 1);
			setFont(false);
			setCurrentFont(pFont_, true);
			useParentValue_ = false;
		}
	}

	return ret;
}

void SMFontComboH::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	fontFamily->setFont(f);
	fontStyle->setFont(f);
}

void SMFontComboH::currentChanged()
{
	if (hasParent_ && !useParentValue_)
	{
		setFont(true);
		fontFamily->insertItem(usePFont_);
		fontStyle->insertItem(usePFont_);
		useParentValue_ = true;
	}
	else if (hasParent_)
		checkStyle();
}

void SMFontComboH::checkStyle()
{
	if (hasParent_ && useParentValue_)
	{
		if (fontStyle->text(fontStyle->count() - 1) != usePFont_)
			fontStyle->insertItem(usePFont_);
	}
}

/***********************************************************************/
/***********************************************************************/

SMTabruler::SMTabruler(QWidget* parent, bool haveFirst, int dEin,
					   QValueList<ParagraphStyle::TabRecord> Tabs, double wid)
: Tabruler(parent, haveFirst, dEin, Tabs, wid)
{
	parentButton_ = new QToolButton(this, "parentButton_");
	Q_CHECK_PTR(parentButton_);
	parentButton_->setText( tr(" Parent Tabs "));
	indentLayout->addWidget(parentButton_);
	parentButton_->hide();
	QFont f(font());
	f.setBold(true);
	parentButton_->setFont(f);
	connect(parentButton_, SIGNAL(clicked()), this, SLOT(pbClicked()));
	hasParent_ = false;
	tabsChanged_ = false;
	useParentTabs_ = false;
	first_ = new SMMSpinBox(-3000, 4000, this, 1);
	Q_CHECK_PTR(first_);
	left_ = new SMMSpinBox(0, 4000, this, 1);
	Q_CHECK_PTR(left_);
	right_ = new SMMSpinBox(0, 4000, this, 1);
	Q_CHECK_PTR(right_);

	indentLayout->remove(firstLineData);
	firstLineData->hide();
	indentLayout->insertWidget(1, first_);
	first_->show();

	layout4->remove(leftIndentData);
	leftIndentData->hide();
	layout4->insertWidget(1, left_);
	left_->show();

	indentLayout->remove(rightIndentData);
	rightIndentData->hide();
	indentLayout->insertWidget(3, right_);
	right_->show();

	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));

	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setTabs(QValueList<ParagraphStyle::TabRecord> Tabs, int dEin)
{
	disconnect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	disconnect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
	hasParent_ = false;
	parentButton_->hide();
	Tabruler::setTabs(Tabs, dEin);
	Tabruler::repaint();
	QString ein = unitGetSuffixFromIndex(dEin);
	if (dEin == 2)
	{
		first_->setDecimals(10000);
		left_->setDecimals(10000);
		right_->setDecimals(10000);
		tabData->setDecimals(10000);
	} // TODO else What's the decimal for other dEins?
	first_->setSuffix(ein);
	left_->setSuffix(ein);
	right_->setSuffix(ein);
	tabData->setSuffix(ein);
}

void SMTabruler::setTabs(QValueList<ParagraphStyle::TabRecord> Tabs, int dEin, bool isParentValue)
{
	disconnect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	disconnect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
	hasParent_ = true;
	pDein_ = dEin;
	if (isParentValue)
		parentButton_->hide();
	else
		parentButton_->show();
	Tabruler::setTabs(Tabs, dEin);
	Tabruler::repaint();
	QString ein = unitGetSuffixFromIndex(dEin);
	if (dEin == 2)
	{
		first_->setDecimals(10000);
		left_->setDecimals(10000);
		right_->setDecimals(10000);
		tabData->setDecimals(10000);
	} // TODO else What's the decimal for other dEins?
	first_->setSuffix(ein);
	left_->setSuffix(ein);
	right_->setSuffix(ein);
	tabData->setSuffix(ein);

	connect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	connect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
}

void SMTabruler::setParentTabs(QValueList<ParagraphStyle::TabRecord> Tabs)
{
	hasParent_ = true;
	pTabs_ = Tabs;
}

void SMTabruler::setFirstLineValue(double t)
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	first_->setValue(t);
	firstLineData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setFirstLineValue(double t, bool isParentValue)
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	isSetupFirst_ = true;
	first_->setValue(t, isParentValue);
	firstLineData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setParentFirstLine(double t)
{
	first_->setParentValue(t);
}

void SMTabruler::setLeftIndentValue(double t)
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	left_->setValue(t);
	leftIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setLeftIndentValue(double t, bool isParentValue)
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	isSetupLeft_ = true;
	left_->setValue(t, isParentValue);
	leftIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setParentLeftIndent(double t)
{
	left_->setParentValue(t);
}

void SMTabruler::setRightIndentValue(double t)
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	right_->setValue(t);
	rightIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setRightIndentValue(double t, bool isParentValue)
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	isSetupRight_ = true;
	right_->setValue(t, isParentValue);
	rightIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setParentRightIndent(double t)
{
	right_->setParentValue(t);
}

bool SMTabruler::useParentTabs()
{
	bool ret = useParentTabs_;
	if (ret && hasParent_)
	{
		setTabs(pTabs_, pDein_, true);
		Tabruler::repaint();
		parentButton_->hide();
	}
	else if (hasParent_)
	{
		parentButton_->show();
	}

	return ret;
}

bool SMTabruler::useParentFirstLine()
{
	return first_->useParentValue();
}

bool SMTabruler::useParentLeftIndent()
{
	return left_->useParentValue();
}

bool SMTabruler::useParentRightIndent()
{
	return right_->useParentValue();
}

void SMTabruler::slotTabsChanged()
{
	if (hasParent_)
	{
		useParentTabs_ = false;
		tabsChanged_ = true;
	}
}

void SMTabruler::pbClicked()
{
	if (hasParent_)
	{
		useParentTabs_ = true;
		emit mouseReleased();
	}
}

void SMTabruler::leftDataChanged()
{
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	double a, b, value;
	int c;
	leftIndentData->getValues(&a, &b, &c, &value);
	if (hasParent_ && !isSetupLeft_)
		left_->setValue(value, false);
	else if (!hasParent_)
		left_->setValue(value);

	isSetupLeft_ = false;
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::rightDataChanged()
{
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	double a, b, value;
	int c;
	rightIndentData->getValues(&a, &b, &c, &value);
	if (hasParent_ && !isSetupRight_)
		right_->setValue(value, false);
	else if (!hasParent_)
		right_->setValue(value);

	isSetupRight_ = false;
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::firstDataChanged()
{
	disconnect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
	double a, b, value;
	int c;
	firstLineData->getValues(&a, &b, &c, &value);
	if (hasParent_ && !isSetupFirst_)
		first_->setValue(value, false);
	else if (!hasParent_)
		first_->setValue(value);

	isSetupFirst_ = false;
	connect(first_, SIGNAL(valueChanged(int)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(int)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(int)), this, SLOT(leftValueChanged()));
}

void SMTabruler::firstValueChanged()
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	double a, b, value;
	int c;
	first_->getValues(&a, &b, &c, &value);
	setFirstLineData(value);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	isSetupFirst_ = true;
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
}

void SMTabruler::leftValueChanged()
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	double a, b, value;
	int c;
	left_->getValues(&a, &b, &c, &value);
	setLeftIndentData(value);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	isSetupLeft_ = true;
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
}

void SMTabruler::rightValueChanged()
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
	double a, b, value;
	int c;
	right_->getValues(&a, &b, &c, &value);
	setRightIndentData(value);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	isSetupRight_ = true;
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(int)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(leftDataChanged()));
}


