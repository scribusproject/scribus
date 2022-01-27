/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QSignalBlocker>
#include <QStringList>
#include <QVariant>

#include "commonstrings.h"
#include "numformatcombo.h"

NumFormatCombo::NumFormatCombo(QWidget* parent)
              : QComboBox(parent)
{
	setupCombo();
}

NumFormatCombo::NumFormatCombo(QWidget* parent, bool showNone)
              : QComboBox(parent),
	            m_showNoneFormat(showNone)
{
	setupCombo();
}

void NumFormatCombo::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}

	QComboBox::changeEvent(e);
}

NumFormat NumFormatCombo::currentFormat() const
{
	QVariant currData = this->currentData();
	return (NumFormat) currData.toInt();
}

void NumFormatCombo::languageChange(void)
{
	if (!m_showNoneFormat)
		return;
	
	int itemCount = this->count();
	this->setItemText(itemCount - 1, CommonStrings::tr_None);
}

void NumFormatCombo::setupCombo()
{
	QSignalBlocker blocker(this);
	
	int oldIndex = this->currentIndex();
	if (oldIndex < 0)
		oldIndex = 0;
	
	QStringList formats = getFormatList();
	for (int i = 0; i < formats.count(); ++i)
	{
		const QString& format = formats.at(i);
		NumFormat type = fromUserStringToNum(format);
		this->addItem(format, (int) type);
	}

	if (m_showNoneFormat)
		this->addItem(CommonStrings::tr_None, (int) Type_None);
	
	this->setCurrentIndex(oldIndex);
}

void NumFormatCombo::setCurrentFormat(NumFormat format)
{
	int itemIndex = findData((int) format);
	if (itemIndex >= 0)
		setCurrentIndex(itemIndex);
}
