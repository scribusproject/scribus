/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QHBoxLayout>

#include "opticalmarginswidget.h"
#include "iconmanager.h"
#include "scribus.h"
#include "scribusapp.h"

OpticalMarginsWidget::OpticalMarginsWidget(QWidget* parent) : FormWidget(parent)
{

	leftButton = new QToolButton();
	leftButton->setCheckable(true);
	rightButton = new QToolButton();
	rightButton->setCheckable(true);
	parentButton = new QToolButton();
	parentButton->hide(); // only used for style manager

	setsCombo = new QComboBox();

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setSpacing(4);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	mainLayout->addWidget(leftButton);
	mainLayout->addWidget(rightButton);
	mainLayout->addSpacerItem(new QSpacerItem(4, 0, QSizePolicy::Fixed));
	mainLayout->addWidget(setsCombo);
	mainLayout->addWidget(parentButton);
	mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));

	setLayout(mainLayout);

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(rightButton, SIGNAL(clicked()), this, SIGNAL(opticalMarginChanged()));
	connect(leftButton, SIGNAL(clicked()), this, SIGNAL(opticalMarginChanged()));
	connect(setsCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(opticalMarginChanged()));
}

int OpticalMarginsWidget::opticalMargin() const
{
	bool left = leftButton->isChecked();
	bool right = rightButton->isChecked();
	int omt(ParagraphStyle::OM_None);

	if (left && right)
		omt = ParagraphStyle::OM_Default;
	else if (left && !right)
		omt = ParagraphStyle::OM_LeftHangingPunct;
	else if (!left && right)
		omt = ParagraphStyle::OM_RightHangingPunct;

	return omt;

}

void OpticalMarginsWidget::setOpticalMargin(int omt)
{
	bool blocked = blockSignals(true);
	if (omt == ParagraphStyle::OM_Default)
	{
		rightButton->setChecked(true);
		leftButton->setChecked(true);
	}
	else if (omt == ParagraphStyle::OM_LeftHangingPunct)
	{
		rightButton->setChecked(false);
		leftButton->setChecked(true);
	}
	else if (omt == ParagraphStyle::OM_RightHangingPunct)
	{
		rightButton->setChecked(true);
		leftButton->setChecked(false);
	}
	else
	{
		rightButton->setChecked(false);
		leftButton->setChecked(false);
	}

	blockSignals(blocked);
}

void OpticalMarginsWidget::setOpticalMarginSets(const OpticalMarginSets& sets)
{
	m_sets = sets;
}

void OpticalMarginsWidget::setOpticalMarginSetId(const QString& id)
{	
	if (m_sets.size() <= 0)
		return;

	m_setId = m_sets.contains(id) ? id : m_sets.firstKey();

	updateSetSelector();
}

QString OpticalMarginsWidget::opticalMarginSetId() const
{
	return setsCombo->currentData().toString();
}

void OpticalMarginsWidget::iconSetChange()
{
	IconManager &im = IconManager::instance();
	leftButton->setIcon(im.loadIcon("paragraph-optical-margin-left"));
	rightButton->setIcon(im.loadIcon("paragraph-optical-margin-right"));
}

void OpticalMarginsWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void OpticalMarginsWidget::updateSetSelector()
{
	if (m_sets.size() <= 0)
		return;

	QSignalBlocker blockSets(setsCombo);
	setsCombo->setVisible( m_sets.size() > 1 ? true : false );
	setsCombo->clear();

	for (auto [id, set] : m_sets.asKeyValueRange())
		setsCombo->addItem(set.name, id);

	int index = setsCombo->findData(m_setId);
	setsCombo->setCurrentIndex(index);
}

void OpticalMarginsWidget::languageChange()
{
	setText( tr("Optical &Margins") );

	leftButton->setToolTip( tr("Optical character offset on left side"));
	rightButton->setToolTip( tr("Optical character offset on right side"));
}
