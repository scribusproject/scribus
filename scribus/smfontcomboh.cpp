/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "smfontcomboh.h"


SMFontComboH::SMFontComboH(QWidget *parent)
: FontComboH(parent, true),
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
		ret = fontFamily->currentText() == usePFont_ ||
			  fontStyle->currentText() == usePFont_;

		if (ret)
		{
			if (fontFamily->itemText(fontFamily->count() - 1) == usePFont_)
				fontFamily->removeItem(fontFamily->count() - 1);
			if (fontStyle->itemText(fontStyle->count() - 1) == usePFont_)
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
		fontFamily->addItem(usePFont_);
		fontStyle->addItem(usePFont_);
		useParentValue_ = true;
	}
	else if (hasParent_)
		checkStyle();
}

void SMFontComboH::checkStyle()
{
	if (hasParent_ && useParentValue_)
	{
		if (fontStyle->itemText(fontStyle->count() - 1) != usePFont_)
			fontStyle->addItem(usePFont_);
	}
}
