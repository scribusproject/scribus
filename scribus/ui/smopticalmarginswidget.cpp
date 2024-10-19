#include "smopticalmarginswidget.h"

#include <QToolTip>
#include "iconmanager.h"
#include "scribusapp.h"

SMOpticalMarginsWidget::SMOpticalMarginsWidget(QWidget *parent)
	: OpticalMarginsWidget(parent)
{
	parentButton->setText( "");
	parentButton->setToolTip( tr("Use parent style's optical margins instead of overriding it"));

	resize(minimumSizeHint());
	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

}

void SMOpticalMarginsWidget::setOpticalMargins(int o)
{
	ParagraphStyle::OpticalMarginType omt(static_cast<ParagraphStyle::OpticalMarginType>(o));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	disconnect(this, SIGNAL(opticalMarginChanged()), this, SLOT(styleChanged()));

	m_hasParent = false;
	parentButton->hide();
	m_pStyle = ParagraphStyle::OpticalMarginType::OM_None;

	OpticalMarginsWidget::setOpticalMargin(omt);

	connect(this, SIGNAL(opticalMarginChanged()), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMOpticalMarginsWidget::setOpticalMargins(int o, int ps, bool isParentValue )
{
	ParagraphStyle::OpticalMarginType omt(static_cast<ParagraphStyle::OpticalMarginType>(o));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	disconnect(this, SIGNAL(opticalMarginChanged()), this, SLOT(styleChanged()));

	if (ps != -1)
	{
		m_hasParent = true;
		parentButton->setVisible(!isParentValue);
		m_pStyle = ps;
	}
	else
		m_hasParent = false;

	OpticalMarginsWidget::setOpticalMargin(omt);

	connect(this, SIGNAL(opticalMarginChanged()), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

bool SMOpticalMarginsWidget::useParentValue()
{
	bool ret = m_useParentStyle;
	m_useParentStyle = false;
	if (ret)
		setOpticalMargins(m_pStyle, m_pStyle, true);

	return ret;
}

void SMOpticalMarginsWidget::pbPressed()
{
	m_useParentStyle = true;
	emit opticalMarginChanged();
}

void SMOpticalMarginsWidget::styleChanged()
{
	if (m_hasParent)
		parentButton->show();
}

void SMOpticalMarginsWidget::iconSetChange()
{
	IconManager &im = IconManager::instance();
	parentButton->setIcon(im.loadIcon("reset-style-changes"));
}
