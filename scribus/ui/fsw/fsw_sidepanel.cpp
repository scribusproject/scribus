/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fsw_sidepanel.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QVBoxLayout>

#include "iconmanager.h"

FSW_SidePanel::FSW_SidePanel(QWidget* parent)
	: QWidget(parent)
{
	setObjectName(QString::fromUtf8("FSW_SidePanel"));
	setFixedWidth(196);
	// A plain QWidget won't paint a stylesheet background unless it's told to.
	setAttribute(Qt::WA_StyledBackground, true);
	// Dark brand band, colour pulled from the splash slate; light, high-contrast step
	// text. A small logo + wordmark sit in a header row at the top.
	setStyleSheet(QString::fromUtf8(
					  "#FSW_SidePanel { background:#062744; }"
					  "#fswBrandName { color:#EAF3FB; font-size:16px; font-weight:500; }"
					  "#fswBrandSub  { color:#6E97BC; font-size:11px; }"
					  "QLabel[fswStep=\"true\"] { color:#9FC2E2; padding:8px 16px; font-size:13px; }"
					  "QLabel[fswStep=\"true\"][fswActive=\"true\"] { color:#EAF3FB; font-weight:500; border-left:3px solid #378ADD; background:rgba(55,138,221,0.16); }"
					  "QLabel[fswStep=\"true\"][fswDone=\"true\"] { color:#5DCAA5; }"));

	auto* outer = new QVBoxLayout(this);
	outer->setContentsMargins(0, 20, 0, 12);
	outer->setSpacing(0);

	// Brand header: small logo + Apscribe wordmark and subtitle.
	auto* header = new QWidget(this);
	auto* headerLay = new QHBoxLayout(header);
	headerLay->setContentsMargins(16, 0, 16, 18);
	headerLay->setSpacing(10);

	m_logo = new QLabel(header);
	m_logo->setFixedSize(34, 34);
	m_logo->setScaledContents(true);
	m_logo->setPixmap(IconManager::instance().loadPixmap("app-icon"));
	headerLay->addWidget(m_logo);

	auto* nameBox = new QVBoxLayout();
	nameBox->setContentsMargins(0, 0, 0, 0);
	nameBox->setSpacing(0);
	m_brandName = new QLabel(header);
	m_brandName->setObjectName(QString::fromUtf8("fswBrandName"));
	m_brandSub = new QLabel(header);
	m_brandSub->setObjectName(QString::fromUtf8("fswBrandSub"));
	nameBox->addWidget(m_brandName);
	nameBox->addWidget(m_brandSub);
	headerLay->addLayout(nameBox);
	headerLay->addStretch(1);

	outer->addWidget(header);

	auto* stepsHost = new QWidget(this);
	m_stepsLayout = new QVBoxLayout(stepsHost);
	m_stepsLayout->setContentsMargins(0, 0, 0, 0);
	m_stepsLayout->setSpacing(2);
	outer->addWidget(stepsHost);
	outer->addStretch(1);

	buildStepList();
	setCurrentStep(0);
}

void FSW_SidePanel::buildStepList()
{
	for (int i = 0; i < 7; ++i)
	{
		auto* lbl = new QLabel(this);
		lbl->setProperty("fswStep", true);
		m_steps.append(lbl);
		m_stepsLayout->addWidget(lbl);
	}
	retranslate();
}

void FSW_SidePanel::setCurrentStep(int id)
{
	m_current = id;
	for (int i = 0; i < m_steps.size(); ++i)
	{
		QLabel* lbl = m_steps.at(i);
		lbl->setProperty("fswActive", i == id);
		lbl->setProperty("fswDone", i < id);
		// re-polish so the property selectors take effect
		lbl->style()->unpolish(lbl);
		lbl->style()->polish(lbl);
	}
}

void FSW_SidePanel::retranslate()
{
	if (m_brandName)
		m_brandName->setText(tr("Apscribe"));
	if (m_brandSub)
		m_brandSub->setText(tr("First-time setup"));

	const QStringList names {
		tr("Welcome"), tr("Language"),
		tr("Appearance"), tr("New document"),
		tr("Fonts & scripts"), tr("Experimental"),
		tr("All set")
	};
	for (int i = 0; i < m_steps.size() && i < names.size(); ++i)
		m_steps.at(i)->setText(QString::fromUtf8("%1  %2").arg(i + 1).arg(names.at(i)));
}

void FSW_SidePanel::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange)
		retranslate();
	QWidget::changeEvent(e);
}
