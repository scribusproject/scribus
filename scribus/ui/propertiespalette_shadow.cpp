/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertiespalette_shadow.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "commonstrings.h"
#include "sccolorengine.h"
#include "pageitem.h"
#include "propertiespalette_utils.h"
#include "sccombobox.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scraction.h"
#include "scribusview.h"
#include "selection.h"
#include "units.h"
#include "undomanager.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"

PropertiesPalette_Shadow::PropertiesPalette_Shadow( QWidget* parent) : QWidget(parent)
{
	m_ScMW = 0;
	m_doc = 0;
	m_haveDoc  = false;
	m_haveItem = false;
	m_unitRatio = 1.0;

	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	_userActionOn = false;
	userActionSniffer = new UserActionSniffer(this);
	connect(userActionSniffer, SIGNAL(actionStart()), this, SLOT(spinboxStartUserAction()));
	connect(userActionSniffer, SIGNAL(actionEnd()), this, SLOT(spinboxFinishUserAction()));

	installSniffer(softShadowXOffset);
	installSniffer(softShadowYOffset);
	installSniffer(softShadowShade);
	installSniffer(softShadowOpacity);
	installSniffer(softShadowBlurRadius);

	languageChange();

	connect(softShadowXOffset,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowYOffset,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowShade,      SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowOpacity,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowBlurRadius, SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowBlendMode,  SIGNAL(activated(int)),       this, SLOT(handleNewValues()));
	connect(softShadowColor,      SIGNAL(activated(int)),       this, SLOT(handleNewValues()));
	connect(hasSoftShadow,        SIGNAL(clicked()),            this, SLOT(handleNewValues()));

	m_haveItem = false;
	softShadowXOffset->showValue(0);
	softShadowYOffset->showValue(0);
	softShadowShade->showValue(100);
	softShadowOpacity->showValue(1);
	softShadowBlurRadius->showValue(5);
}

void PropertiesPalette_Shadow::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;
	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Shadow::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = NULL;
	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();
	int precision = unitGetPrecisionFromIndex(m_unitIndex);
//qt4 FIXME here
	double maxXYWHVal =  200 * m_unitRatio;
	double minXYVal   = -200 * m_unitRatio;

	m_haveDoc = true;
	m_haveItem = false;

	QMap<QString, double>* docConstants = m_doc? &m_doc->constants()  : NULL;
	softShadowXOffset->setValues( minXYVal, maxXYWHVal, precision, minXYVal);
	softShadowXOffset->setConstants(docConstants);
	softShadowYOffset->setValues( minXYVal, maxXYWHVal, precision, minXYVal);
	softShadowYOffset->setConstants(docConstants);
	softShadowOpacity->setValues(0, 100, 0, 100);
	softShadowBlurRadius->setValues(0, 20, 1, 5);
	softShadowShade->setValues(0, 100, 0, 100);

	updateSpinBoxConstants();
	updateColorList();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Shadow::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}
	m_haveDoc  = false;
	m_haveItem = false;
	m_doc   = NULL;
	m_item  = NULL;
	softShadowXOffset->setConstants(NULL);
	softShadowYOffset->setConstants(NULL);
	softShadowXOffset->showValue(0);
	softShadowYOffset->showValue(0);
	softShadowShade->showValue(100);
	softShadowOpacity->showValue(1);
	softShadowBlurRadius->showValue(5);
	setEnabled(false);
}

void PropertiesPalette_Shadow::unsetItem()
{
	m_haveItem = false;
	m_item     = NULL;
	handleSelectionChanged();
}

void PropertiesPalette_Shadow::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
}

PageItem* PropertiesPalette_Shadow::currentItemFromSelection()
{
	PageItem *currentItem = NULL;
	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 1)
			currentItem = m_doc->m_Selection->itemAt(0);
		else if (m_doc->m_Selection->count() == 1)
			currentItem = m_doc->m_Selection->itemAt(0);
	}
	return currentItem;
}

void PropertiesPalette_Shadow::setCurrentItem(PageItem *i)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (!m_doc)
		setDoc(i->doc());
	m_haveItem = false;
	m_item = i;
	disconnect(softShadowXOffset,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	disconnect(softShadowYOffset,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	disconnect(softShadowShade,      SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	disconnect(softShadowOpacity,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	disconnect(softShadowBlurRadius, SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	disconnect(softShadowBlendMode,  SIGNAL(activated(int)),       this, SLOT(handleNewValues()));
	disconnect(softShadowColor,      SIGNAL(activated(int)),       this, SLOT(handleNewValues()));
	disconnect(hasSoftShadow,        SIGNAL(clicked()),            this, SLOT(handleNewValues()));
	softShadowXOffset->setValue(i->softShadowXOffset() * m_unitRatio);
	softShadowYOffset->setValue(i->softShadowYOffset() * m_unitRatio);
	softShadowOpacity->setValue(qRound(100 - (i->softShadowOpacity() * 100)));
	softShadowBlurRadius->setValue(i->softShadowBlurRadius() * m_unitRatio);
	softShadowShade->setValue(i->softShadowShade());
	setCurrentComboItem(softShadowColor, i->softShadowColor());
	softShadowBlendMode->setCurrentIndex(i->softShadowBlendMode());
	hasSoftShadow->setChecked(i->hasSoftShadow());
	connect(softShadowXOffset,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowYOffset,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowShade,      SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowOpacity,    SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowBlurRadius, SIGNAL(valueChanged(double)), this, SLOT(handleNewValues()));
	connect(softShadowBlendMode,  SIGNAL(activated(int)),       this, SLOT(handleNewValues()));
	connect(softShadowColor,      SIGNAL(activated(int)),       this, SLOT(handleNewValues()));
	connect(hasSoftShadow,        SIGNAL(clicked()),            this, SLOT(handleNewValues()));
	m_haveItem = true;
	updateSpinBoxConstants();
}

void PropertiesPalette_Shadow::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (currItem)
		setCurrentItem(currItem);
	updateGeometry();
}

void PropertiesPalette_Shadow::unitChange()
{
	if (!m_haveDoc)
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();
	softShadowXOffset->setNewUnit( m_unitIndex );
	softShadowYOffset->setNewUnit( m_unitIndex );
	softShadowBlurRadius->setNewUnit( m_unitIndex );
	m_haveItem = tmp;
}

void PropertiesPalette_Shadow::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	softShadowColor->updateBox(m_doc->PageColors, ColorCombo::fancyPixmaps, false);
}

void PropertiesPalette_Shadow::handleNewValues()
{
	if (m_haveItem)
	{
		double x = softShadowXOffset->value() / m_unitRatio;
		double y = softShadowYOffset->value() / m_unitRatio;
		double r = softShadowBlurRadius->value() / m_unitRatio;
		double o = (100 - softShadowOpacity->value()) / 100.0;
		int b = softShadowBlendMode->currentIndex();
		QString color = softShadowColor->currentText();
		if (color == CommonStrings::tr_NoneColor)
			color = CommonStrings::None;
		double s = softShadowShade->value();
		if (m_haveDoc)
		{
			m_doc->itemSelection_SetSoftShadow(hasSoftShadow->isChecked(), color, x, y, r, s, o, b);
		}
	}
}

void PropertiesPalette_Shadow::installSniffer(ScrSpinBox *spinBox)
{
	const QList<QObject*> list = spinBox->children();
	if (!list.isEmpty())
	{
		QListIterator<QObject*> it(list);
		QObject *obj;
		while (it.hasNext())
		{
			obj = it.next();
			obj->installEventFilter(userActionSniffer);
		}
	}
}

bool PropertiesPalette_Shadow::userActionOn()
{
	return _userActionOn;
}

void PropertiesPalette_Shadow::spinboxStartUserAction()
{
	_userActionOn = true;
}

void PropertiesPalette_Shadow::spinboxFinishUserAction()
{
	_userActionOn = false;

	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);
	if (m_ScMW->view->groupTransactionStarted())
	{
		m_ScMW->view->endGroupTransaction();
	}
}

void PropertiesPalette_Shadow::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PropertiesPalette_Shadow::languageChange()
{
	hasSoftShadow->setText( tr("Object has a soft shadow"));
	label->setText( tr("X-Offset:"));
	label_2->setText( tr("Y-Offset:"));
	label_3->setText( tr("Blur:"));
	label_4->setText( tr("Color:"));
	label_5->setText( tr("Shade:"));
	label_6->setText( tr("Opacity:"));
	label_7->setText( tr("Blendmode:"));

	QString ptSuffix = tr(" pt");
	QString ein = (m_haveDoc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;
	softShadowXOffset->setSuffix(ein);
	softShadowYOffset->setSuffix(ein);
	softShadowBlurRadius->setSuffix(ein);
}

void PropertiesPalette_Shadow::updateSpinBoxConstants()
{
	if (!m_haveDoc)
		return;
	if(m_doc->m_Selection->count()==0)
		return;
	softShadowXOffset->setConstants(&m_doc->constants());
	softShadowYOffset->setConstants(&m_doc->constants());

}
