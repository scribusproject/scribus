#include "smcolorbutton.h"
#include "iconmanager.h"
#include "scribusapp.h"
#include "util_gui.h"

SMColorButton::SMColorButton(QWidget *parent) : FormWidget(parent)
{
	colorButton = new ColorButton(this);
	colorButton->setMenuContextType(ColorButton::Floating);
	addWidget( colorButton );

	parentButton = new QToolButton(this);
	parentButton->setText( "");
	parentButton->setToolTip( tr("Use parent style's color instead of overriding it"));
	addWidget( parentButton );

	resize(minimumSizeHint());
	parentButton->hide();

	iconSetChange();
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void SMColorButton::setColor(const QString name, const double shade)
{
	disconnect(colorButton, SIGNAL(colorChanged()), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));

	m_hasParent = false;
	m_pShade = -1;
	m_pColor = "";
	parentButton->hide();
	colorButton->setColor(name, shade);
	colorButton->setHasDot(false);
	colorButton->updatePreview();
	colorButton->updateFloatingContext();
}

void SMColorButton::setColor(const QString name, const double shade, const QString pName, const double pShade, bool isParentValue)
{
	disconnect(colorButton, SIGNAL(colorChanged()), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));

	m_hasParent = true;
	m_pShade = pShade;
	m_pColor = pName;

	if (isParentValue)
		parentButton->hide();
	else
	{
		parentButton->show();
	}

	if (m_pColor.isEmpty() || m_pColor == CommonStrings::tr_NoneColor)
		colorButton->setDotBrush(renderEmptyPattern(colorButton->dotSize(), colorButton->devicePixelRatio()));
	else
		colorButton->setDotBrush(renderColor(
			colorButton->dotSize(), colorButton->devicePixelRatio(),
			colorButton->colorFromName(m_pColor, 100.0),
			colorButton->colorFromName(m_pColor, m_pShade)
		));

	colorButton->setHasDot(!isParentValue);
	colorButton->setColor(name, shade);
	colorButton->updatePreview();
	colorButton->updateFloatingContext();

	connect(colorButton, SIGNAL(colorChanged()), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMColorButton::styleChanged()
{
	if (m_hasParent)
	{
		parentButton->show();
		colorButton->setHasDot(true);
	}
}

bool SMColorButton::useParentValue()
{
	bool ret = m_useParentValue;
	m_useParentValue = false;

	if (ret && m_hasParent)
	{
		setColor(m_pColor, m_pShade, m_pColor, m_pShade, true);
		colorButton->setHasDot(false);
	}
	return ret;
}

void SMColorButton::pbPressed()
{
	m_useParentValue = true;
}

void SMColorButton::iconSetChange()
{
	IconManager &im = IconManager::instance();
	parentButton->setIcon(im.loadIcon("reset-style-changes"));
}
