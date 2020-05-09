/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "smfontcomboh.h"


SMFontComboH::SMFontComboH(QWidget *parent)
: FontComboH(parent, true),
  m_useParentFont( tr("Use Parent Font"))
{
	
}

void SMFontComboH::setCurrentFont(const QString &s)
{
	disconnect(fontFamily, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	disconnect(fontStyle, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	m_hasParent = false;
	m_parentFont = s;
	FontComboH::setCurrentFont(s);
}

void SMFontComboH::setCurrentFont(const QString &s, bool isParentValue)
{
	disconnect(fontFamily, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	disconnect(fontStyle, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	m_hasParent = true;
	m_parentFont = s;
	FontComboH::setCurrentFont(s);
	setFont(!isParentValue);
	connect(fontFamily, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	connect(fontStyle, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMFontComboH::setParentFont(const QString &s)
{
	m_hasParent = true;
	m_parentFont = s;
}

bool SMFontComboH::useParentFont()
{
	bool ret = false;

	if (m_useParentValue && m_hasParent)
	{
		ret = fontFamily->currentText() == m_useParentFont ||
			  fontStyle->currentText() == m_useParentFont;

		if (ret)
		{
			bool familySigBlocked = fontFamily->blockSignals(true);
			bool styleSigBlocked = fontStyle->blockSignals(true);
			if (fontFamily->itemText(fontFamily->count() - 1) == m_useParentFont)
				fontFamily->removeItem(fontFamily->count() - 1);
			if (fontStyle->itemText(fontStyle->count() - 1) == m_useParentFont)
				fontStyle->removeItem(fontStyle->count() - 1);
			fontFamily->blockSignals(familySigBlocked);
			fontStyle->blockSignals(styleSigBlocked);

			setFont(false);
			setCurrentFont(m_parentFont, true);
			m_useParentValue = false;
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
	if (m_hasParent && !m_useParentValue)
	{
		setFont(true);
		fontFamily->addItem(m_useParentFont);
		fontStyle->addItem(m_useParentFont);
		m_useParentValue = true;
	}
	else if (m_hasParent)
		checkStyle();
}

void SMFontComboH::checkStyle()
{
	if (m_hasParent && m_useParentValue)
	{
		if (fontStyle->itemText(fontStyle->count() - 1) != m_useParentFont)
			fontStyle->addItem(m_useParentFont);
	}
}
