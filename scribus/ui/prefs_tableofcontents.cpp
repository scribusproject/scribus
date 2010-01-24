/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_tableofcontents.h"
#include "prefsstructs.h"

Prefs_TableOfContents::Prefs_TableOfContents(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);

}

Prefs_TableOfContents::~Prefs_TableOfContents()
{
}

void Prefs_TableOfContents::restoreDefaults(struct ApplicationPrefs *prefsData)
{

}

void Prefs_TableOfContents::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{

}

void Prefs_TableOfContents::languageChange()
{

}

void Prefs_TableOfContents::setupItemAttrs( QStringList newNames )
{
//	disconnect( itemAttrComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemAttributeSelected(const QString&) ) );
//	itemAttrComboBox->clear();
//	itemAttrComboBox->addItem(trStrNone);
//	itemAttrComboBox->addItems(newNames);
//	if (numSelected!=999)
//	{
//		if (localToCSetupVector[numSelected].itemAttrName==strNone)
//			setCurrentComboItem(itemAttrComboBox, trStrNone);
//		else
//			setCurrentComboItem(itemAttrComboBox, localToCSetupVector[numSelected].itemAttrName);
//	}
//	connect( itemAttrComboBox, SIGNAL( activated(const QString&) ), this, SLOT( itemAttributeSelected(const QString&) ) );
}
