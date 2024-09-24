#include "colorpicker_pattern.h"
#include "scribusapp.h"
#include "iconmanager.h"
#include "units.h"
#include "util_gui.h"
#include "ui/linkbutton.h"

ColorPickerPattern::ColorPickerPattern(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	iconSetChange();

	// offset
	numberOffsetX->setMinimum(-3000);
	numberOffsetX->setMaximum(3000);
	numberOffsetX->setNewUnit(SC_POINTS);
	numberOffsetY->setMinimum(-3000);
	numberOffsetY->setMaximum(3000);
	numberOffsetY->setNewUnit(SC_POINTS);
	// scale
	numberScaleX->setMinimum(0.01);
	numberScaleX->setMaximum(500);
	numberScaleX->setNewUnit(SC_PERCENT);
	numberScaleX->setValue( 100 );
	numberScaleY->setMinimum(0.01);
	numberScaleY->setMaximum(500);
	numberScaleY->setNewUnit(SC_PERCENT);
	numberScaleY->setValue( 100 );
	buttonScaleRatio->setCheckable( true );
	buttonScaleRatio->setAutoRaise( true );
	// angle
	numberAngle->setMinimum(-180);
	numberAngle->setMaximum(180);
	numberAngle->setNewUnit(SC_DEGREES);
	// skew
	numberSkewX->setMinimum(-89);
	numberSkewX->setMaximum(89);
	numberSkewX->setNewUnit(SC_DEGREES);
	numberSkewX->setValue( 0 );
	numberSkewY->setMinimum(-89);
	numberSkewY->setMaximum(89);
	numberSkewY->setNewUnit(SC_DEGREES);
	numberSkewY->setValue( 0 );
	// spacing
	numberSpacing->setMinimum(1);
	numberSpacing->setMaximum(500);
	numberSpacing->setValue( 100 );
	// flip
	buttonFlipHorizontal->setCheckable( true );
	buttonFlipVertical->setCheckable( true );

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(sectionProperties,		&SectionContainer::sizeChanged, this,			&ColorPickerPattern::updateSize);
	connect(sectionSwatches,		&SectionContainer::sizeChanged, this,			&ColorPickerPattern::updateSize);

	connectSlots();

}


void ColorPickerPattern::connectSlots()
{
	connect(listPattern,				&QListWidget::itemClicked, this,	&ColorPickerPattern::updateName);
	connect(numberOffsetX,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	connect(numberOffsetY,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	connect(numberScaleX,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updateScaleX);
	connect(numberScaleY,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updateScaleY);
	connect(numberSkewX,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	connect(numberSkewY,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	connect(numberAngle,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	connect(numberSpacing,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	connect(buttonFlipHorizontal,		&QToolButton::toggled, this,		&ColorPickerPattern::updatePattern);
	connect(buttonFlipVertical,			&QToolButton::toggled, this,		&ColorPickerPattern::updatePattern);
	connect(buttonScaleRatio,			&LinkButton::toggled, this,			&ColorPickerPattern::toggleScaleRatio);
	connect(checkboxFollowsPath,		&QCheckBox::toggled, this,			&ColorPickerPattern::updatePattern);
	connect(checkboxLumAlpha,			&QCheckBox::toggled, this,			&ColorPickerPattern::updatePattern);
	connect(checkboxInvertedPattern,	&QCheckBox::toggled, this,			&ColorPickerPattern::updatePattern);
}

void ColorPickerPattern::disconnectSlots()
{
	disconnect(listPattern,				&QListWidget::itemClicked, this,	&ColorPickerPattern::updateName);
	disconnect(numberOffsetX,			&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	disconnect(numberOffsetY,			&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	disconnect(numberScaleX,			&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updateScaleX);
	disconnect(numberScaleY,			&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updateScaleY);
	disconnect(numberSkewX,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	disconnect(numberSkewY,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	disconnect(numberAngle,				&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	disconnect(numberSpacing,			&ScrSpinBox::valueChanged, this,	&ColorPickerPattern::updatePattern);
	disconnect(buttonFlipHorizontal,	&QToolButton::toggled, this,		&ColorPickerPattern::updatePattern);
	disconnect(buttonFlipVertical,		&QToolButton::toggled, this,		&ColorPickerPattern::updatePattern);
	disconnect(buttonScaleRatio,		&LinkButton::toggled, this,			&ColorPickerPattern::toggleScaleRatio);
	disconnect(checkboxFollowsPath,		&QCheckBox::toggled, this,			&ColorPickerPattern::updatePattern);
	disconnect(checkboxLumAlpha,		&QCheckBox::toggled, this,			&ColorPickerPattern::updatePattern);
	disconnect(checkboxInvertedPattern,	&QCheckBox::toggled, this,			&ColorPickerPattern::updatePattern);
}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

void ColorPickerPattern::setPatternList(QHash<QString, ScPattern> *docPatterns)
{
	QSignalBlocker sigListPattern(listPattern);

	listPattern->clear();
	listPattern->setIconSize(QSize(48, 48));
	QStringList patK = docPatterns->keys();
	patK.sort();
	for (int a = 0; a < patK.count(); a++)
	{
		ScPattern sp = docPatterns->value(patK[a]);
		QListWidgetItem *item = new QListWidgetItem(renderPattern(listPattern->iconSize(), devicePixelRatio(), sp), patK[a], listPattern);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	listPattern->clearSelection();
}

const CPPatternData& ColorPickerPattern::patternData() const
{
	return m_pattern;
}

Context ColorPickerPattern::context() const
{
    return m_context;
}

void ColorPickerPattern::setContext(Context config)
{
    m_context = config;

	switch (m_context)
	{
	case Context::Line:
		checkboxLumAlpha->setVisible(false);
		checkboxInvertedPattern->setVisible(false);
		checkboxFollowsPath->setVisible(true);
		labelSpacing->setVisible(true);
		break;
	case Context::FillMask:
		checkboxLumAlpha->setVisible(true);
		checkboxInvertedPattern->setVisible(true);
		checkboxFollowsPath->setVisible(false);
		labelSpacing->setVisible(false);
		break;
	case Context::Fill:
		checkboxLumAlpha->setVisible(false);
		checkboxInvertedPattern->setVisible(false);
		checkboxFollowsPath->setVisible(false);
		labelSpacing->setVisible(false);
		break;
	case Context::Simple:
	default:
		break;
	}

	updateSize();
}

void ColorPickerPattern::hideEditedPatterns(QStringList names)
{
	for (int i = 0; i < names.count(); i++)
	{
		QList<QListWidgetItem*> items = listPattern->findItems(names[i], Qt::MatchExactly);
		if (items.count() > 0)
			items[0]->setHidden(true);
	}
}

int ColorPickerPattern::type() const
{
	switch (m_context)
	{
	case Context::FillMask:
		if (checkboxLumAlpha->isChecked())
		{
			if (checkboxInvertedPattern->isChecked())
				return GradMask_PatternLumAlphaInverted;
			else
				return GradMask_PatternLumAlpha;
		}
		else
		{
			if (checkboxInvertedPattern->isChecked())
				return GradMask_PatternInverted;
			else
				return GradMask_Pattern;
		}
		break;
	default:
		return Gradient_Pattern;
		break;
	}
}

QString ColorPickerPattern::toolTipText() const
{
	return QString( tr("Pattern: %1") ).arg(m_pattern.Name);
}

void ColorPickerPattern::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
	unitChange();
}

void ColorPickerPattern::setPatternData(const CPPatternData& pattern)
{
	m_pattern = pattern;

	disconnectSlots();

	// no patterns available
	if (m_pattern.Name.isEmpty() && (listPattern->count() == 0))
		return;
	// set name by first pattern in the list
	if (m_pattern.Name.isEmpty() && (listPattern->count() > 0))
		m_pattern.Name = listPattern->item(0)->text();

	QList<QListWidgetItem*> itl = listPattern->findItems(m_pattern.Name, Qt::MatchExactly);
	if (itl.count() != 0)
	{
		QListWidgetItem *it = itl[0];
		listPattern->setCurrentItem(it);
	}
	else
		listPattern->clearSelection();

	numberOffsetX->setValue(m_pattern.OffsetX * unitGetRatioFromIndex(currentUnit));
	numberOffsetY->setValue(m_pattern.OffsetY * unitGetRatioFromIndex(currentUnit));
	numberScaleX->setValue(m_pattern.ScaleX * 100.0);
	numberScaleY->setValue(m_pattern.ScaleY * 100.0);
	numberSpacing->setValue(m_pattern.Spacing * 100.0);
	numberSpacing->setEnabled(m_pattern.OnPath);
	double asina = atan(m_pattern.SkewX);
	numberSkewX->setValue(asina / (M_PI / 180.0));
	double asinb = atan(m_pattern.SkewY);
	numberSkewY->setValue(asinb / (M_PI / 180.0));
	numberAngle->setValue(m_pattern.Angle);
	buttonFlipHorizontal->setChecked(m_pattern.MirrorX);
	buttonFlipVertical->setChecked(m_pattern.MirrorY);
	checkboxFollowsPath->setChecked(m_pattern.OnPath);
	buttonScaleRatio->setChecked( m_pattern.ScaleX == m_pattern.ScaleY );

	connectSlots();

}

/* ********************************************************************************* *
 *
 * Slots
 *
 * ********************************************************************************* */

void ColorPickerPattern::languageChange()
{
	sectionProperties->setText( tr("Properties") );
	sectionSwatches->setText( tr("Patterns") );

	labelOffset->setText( tr("Offset") );
	labelScale->setText( tr("Scale") );
	labelSkew->setText( tr("Skew") );
	labelRotation->setText( tr("Rotation") );
	labelFlip->setText( tr("Flip") );
	labelX->setText( tr("X:") );
	labelY->setText( tr("Y:") );

}

void ColorPickerPattern::iconSetChange()
{
	IconManager &iconManager = IconManager::instance();

	buttonFlipHorizontal->setIcon( iconManager.loadIcon("16/flip-object-horizontal.png"));
	buttonFlipVertical->setIcon( iconManager.loadIcon("16/flip-object-vertical.png"));
}

void ColorPickerPattern::unitChange()
{
	if (!m_doc)
		return;

	currentUnit = m_doc->unitIndex();

	numberOffsetX->setNewUnit(currentUnit);
	numberOffsetY->setNewUnit(currentUnit);
}

void ColorPickerPattern::updatePattern()
{
	m_pattern.OffsetX = numberOffsetX->value() / unitGetRatioFromIndex(currentUnit);
	m_pattern.OffsetY = numberOffsetY->value() / unitGetRatioFromIndex(currentUnit);
	m_pattern.ScaleX = numberScaleX->value() / 100.0;
	m_pattern.ScaleY = numberScaleY->value() / 100.0;
	double a    = M_PI / 180.0 * numberSkewX->value();
	double b    = M_PI / 180.0 * numberSkewY->value();
	m_pattern.SkewX = tan(a);
	m_pattern.SkewY = tan(b);
	m_pattern.MirrorX = buttonFlipHorizontal->isChecked();
	m_pattern.MirrorY = buttonFlipVertical->isChecked();
	m_pattern.Spacing = numberSpacing->value() / 100.0;
	m_pattern.Angle = numberAngle->value();
	m_pattern.OnPath = checkboxFollowsPath->isChecked();

	numberSpacing->setEnabled(checkboxFollowsPath->isChecked());

	emit patternChanged();
}

void ColorPickerPattern::updateScaleX()
{
	QSignalBlocker sigScaleY(numberScaleY);

	if (buttonScaleRatio->isChecked())
		numberScaleY->setValue(numberScaleX->value());

	updatePattern();
}

void ColorPickerPattern::updateScaleY()
{
	QSignalBlocker sigScaleX(numberScaleX);

	if (buttonScaleRatio->isChecked())
		numberScaleX->setValue(numberScaleY->value());

	updatePattern();
}

void ColorPickerPattern::updateName(QListWidgetItem *item)
{
	if (!item)
		return;
	m_pattern.Name = item->text();
	updatePattern();
}

void ColorPickerPattern::updateSize()
{
	int w = this->width();
	adjustSize();
	resize(w, sizeHint().height());
	emit sizeChanged();
}

void ColorPickerPattern::toggleScaleRatio()
{
	QSignalBlocker sigScaleY(numberScaleY);

	if (buttonScaleRatio->isChecked())
	{
		numberScaleY->setValue(numberScaleX->value());
		updatePattern();
		buttonScaleRatio->setChecked(true);
	}
	else
		buttonScaleRatio->setChecked(false);
}


