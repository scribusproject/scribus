/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QLabel>
#include <QSignalBlocker>

#include "commonstrings.h"
#include "iconmanager.h"
#include "pagestructs.h"
#include "prefsstructs.h"
#include "scribusdoc.h"
#include "ui/preferences/prefs_indexes.h"
#include "util.h"

Prefs_Indexes::Prefs_Indexes(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	  m_Doc(doc)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Indexes");
	m_icon = "pref-indexes";

	updateStyleComboBoxes();

	connect(indexAddButton, SIGNAL(clicked()), this, SLOT(addIndex()));
	connect(indexDeleteButton, SIGNAL(clicked()), this, SLOT(deleteIndex()));

	connect(itemDestFrameComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemFrameSelected(QString)));
	connect(listNonPrintingCheckBox, SIGNAL(toggled(bool)), this, SLOT(listNonPrintingChecked(bool)));
	connect(combinePageNumbersCheckBox, SIGNAL(toggled(bool)), this, SLOT(combinePageNumbersChecked(bool)));
	connect(combineIdenticalEntriesCheckBox, SIGNAL(toggled(bool)), this, SLOT(combineIdenticalEntriesChecked(bool)));
	connect(caseSensitiveCombinationCheckBox, SIGNAL(toggled(bool)), this, SLOT(caseSenstiveChecked(bool)));
	connect(autoCapitalizeEntriesCheckBox, SIGNAL(toggled(bool)), this, SLOT(autoCapitalizeEntriesChecked(bool)));
	connect(addAlphaSeparatorsCheckBox, SIGNAL(toggled(bool)), this, SLOT(addAlphaSeparatorsChecked(bool)));

	connect(styleHeadingComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(headingStyleSelected(QString)));
	connect(styleSeparatorComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(separatorStyleSelected(QString)));
	connect(styleLevel1ComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(level1StyleSelected(QString)));
	connect(styleLevel2ComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(level2StyleSelected(QString)));
	connect(styleLevel3ComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(level3StyleSelected(QString)));

}

Prefs_Indexes::~Prefs_Indexes() = default;


void Prefs_Indexes::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void Prefs_Indexes::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	localIndexSetupVector = prefsData->indexPrefs.defaultIndexSetup;
	generatePageItemList();
	bool enabled = (localIndexSetupVector.count() > 0);
	if (enabled)
	{
		updateIndexListBox();
		indexListBox->setCurrentRow(0);
		selectIndex(0);
	}
	else
	{
		indexListBox->clear();
		enableGUIWidgets();
	}
	connect( indexListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectIndex(int) ) );
	connect( indexListBox, SIGNAL( itemChanged(QListWidgetItem*) ), this, SLOT( indexListWidgetItemEdited(QListWidgetItem*)));

}

void Prefs_Indexes::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->indexPrefs.defaultIndexSetup = localIndexSetupVector;
}

void Prefs_Indexes::languageChange()
{

}

void Prefs_Indexes::enableGUIWidgets()
{
	bool indexExists = (localIndexSetupVector.count() > 0);
	indexListBox->setEnabled(indexExists);
	bool haveIndexSelected = (indexExists && (indexListBox->currentRow() >= 0));
	indexDeleteButton->setEnabled(haveIndexSelected);
	itemDestFrameComboBox->setEnabled(haveIndexSelected);
	listNonPrintingCheckBox->setEnabled(haveIndexSelected);
	combinePageNumbersCheckBox->setEnabled(haveIndexSelected);
	combineIdenticalEntriesCheckBox->setEnabled(haveIndexSelected);
	bool caseSensitiveTest = false;
	if (numSelected > 0 && numSelected < localIndexSetupVector.count())
		caseSensitiveTest = localIndexSetupVector[numSelected].combineIdenticalEntries;
	caseSensitiveCombinationCheckBox->setEnabled(haveIndexSelected && caseSensitiveTest);
	autoCapitalizeEntriesCheckBox->setEnabled(haveIndexSelected);
	addAlphaSeparatorsCheckBox->setEnabled(haveIndexSelected);
	styleHeadingComboBox->setEnabled(haveIndexSelected);
	styleSeparatorComboBox->setEnabled(haveIndexSelected);
	styleLevel1ComboBox->setEnabled(haveIndexSelected);
	styleLevel2ComboBox->setEnabled(haveIndexSelected);
	styleLevel3ComboBox->setEnabled(haveIndexSelected);
}

void Prefs_Indexes::addIndex()
{
	QString newName;
	if (localIndexSetupVector.isEmpty())
		newName = tr("Index");
	else
		newName = tr("Index %1").arg(localIndexSetupVector.count() + 1);

	IndexSetup newIndexEntry;
	newIndexEntry.name = newName;
	newIndexEntry.frameName = CommonStrings::None;
	localIndexSetupVector.append(newIndexEntry);
	QSignalBlocker sigBlocker(indexListBox);
	updateIndexListBox();
	indexListBox->setCurrentRow(localIndexSetupVector.count() - 1);
	numSelected = indexListBox->currentRow();
	enableGUIWidgets();
}

void Prefs_Indexes::deleteIndex()
{
	int numberSelected = indexListBox->currentRow();
	if (numberSelected < 0)
		return;

	int i = 0;
	IndexSetupVector::Iterator it = localIndexSetupVector.begin();
	while (it != localIndexSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	localIndexSetupVector.erase(it);

	QSignalBlocker sigBlocker(indexListBox);
	updateIndexListBox();
	if (numberSelected < localIndexSetupVector.count())
	{
		indexListBox->setCurrentRow(numberSelected);
		selectIndex(numberSelected);
	}
	else if (localIndexSetupVector.count() > 0)
	{
		indexListBox->setCurrentRow(localIndexSetupVector.count() - 1);
		selectIndex(localIndexSetupVector.count() - 1);
	}
	if (localIndexSetupVector.isEmpty())
		numSelected = -1;
	enableGUIWidgets();
}

void Prefs_Indexes::updateStyleComboBoxes()
{
	if (!m_Doc)
		return;
	QStringList stylesList;
	for (int i = 0; i < m_Doc->paragraphStyles().count(); ++i)
	{
		const ParagraphStyle &paraStyle = m_Doc->paragraphStyles()[i];
		if (!paraStyle.name().isEmpty() && !paraStyle.isDefaultStyle())
			stylesList.append(paraStyle.name());
	}
	stylesList.sort();
	stylesList.prepend(CommonStrings::trDefaultParagraphStyle);
	stylesList.prepend(QString());
	styleHeadingComboBox->clear();
	styleSeparatorComboBox->clear();
	styleLevel1ComboBox->clear();
	styleLevel2ComboBox->clear();
	styleLevel3ComboBox->clear();
	styleHeadingComboBox->addItems(stylesList);
	styleSeparatorComboBox->addItems(stylesList);
	styleLevel1ComboBox->addItems(stylesList);
	styleLevel2ComboBox->addItems(stylesList);
	styleLevel3ComboBox->addItems(stylesList);
	//TODO: Indexing doesn't support multiple styles for different layers yet
	styleLevel2Label->setVisible(false);
	styleLevel3Label->setVisible(false);
	styleLevel2ComboBox->setVisible(false);
	styleLevel3ComboBox->setVisible(false);

}

void Prefs_Indexes::updateIndexListBox()
{
	indexListBox->clear();
	if (localIndexSetupVector.isEmpty())
		return;
	QStringList sl;
	for (IndexSetupVector::Iterator it = localIndexSetupVector.begin(); it != localIndexSetupVector.end(); ++it)
		sl << (*it).name;
	indexListBox->insertItems(0, sl);
	for (int i = 0; i < indexListBox->count(); i++)
	{
		QListWidgetItem *qlwi = indexListBox->item(i);
		if (qlwi != nullptr)
			qlwi->setFlags(qlwi->flags() | Qt::ItemIsEditable);
	}
}

void Prefs_Indexes::selectIndex(int numberSelected)
{
	numSelected = numberSelected;
	if (numSelected < 0)
	{
		enableGUIWidgets();
		return;
	}
	if (localIndexSetupVector.isEmpty())
		return;
	if (localIndexSetupVector.count() < numSelected)
		numSelected = 0;

	if (localIndexSetupVector[numSelected].frameName == CommonStrings::None)
		setCurrentComboItem(itemDestFrameComboBox, CommonStrings::tr_None);
	else
		setCurrentComboItem(itemDestFrameComboBox, localIndexSetupVector[numSelected].frameName);

	listNonPrintingCheckBox->setChecked(localIndexSetupVector[numSelected].listNonPrintingFrames);
	combinePageNumbersCheckBox->setChecked(localIndexSetupVector[numSelected].combinePageNumbers);
	combineIdenticalEntriesCheckBox->setChecked(localIndexSetupVector[numSelected].combineIdenticalEntries);
	caseSensitiveCombinationCheckBox->setEnabled(localIndexSetupVector[numSelected].combineIdenticalEntries);
	caseSensitiveCombinationCheckBox->setChecked(localIndexSetupVector[numSelected].caseSensitiveCombination);
	autoCapitalizeEntriesCheckBox->setChecked(localIndexSetupVector[numSelected].autoCapitalizeEntries);
	addAlphaSeparatorsCheckBox->setChecked(localIndexSetupVector[numSelected].addAlphaSeparators);

	if (localIndexSetupVector[numSelected].headingStyle == CommonStrings::None)
		setCurrentComboItem(styleHeadingComboBox, CommonStrings::tr_None);
	else if (localIndexSetupVector[numSelected].headingStyle == CommonStrings::DefaultParagraphStyle)
		setCurrentComboItem(styleHeadingComboBox, CommonStrings::trDefaultParagraphStyle);
	else
		setCurrentComboItem(styleHeadingComboBox, localIndexSetupVector[numSelected].headingStyle);

	if (localIndexSetupVector[numSelected].separatorStyle == CommonStrings::None)
		setCurrentComboItem(styleSeparatorComboBox, CommonStrings::tr_None);
	else if (localIndexSetupVector[numSelected].separatorStyle == CommonStrings::DefaultParagraphStyle)
		setCurrentComboItem(styleSeparatorComboBox, CommonStrings::trDefaultParagraphStyle);
	else
		setCurrentComboItem(styleSeparatorComboBox, localIndexSetupVector[numSelected].separatorStyle);

	if (localIndexSetupVector[numSelected].level1Style == CommonStrings::None)
		setCurrentComboItem(styleLevel1ComboBox, CommonStrings::tr_None);
	else if (localIndexSetupVector[numSelected].level1Style == CommonStrings::DefaultParagraphStyle)
		setCurrentComboItem(styleLevel1ComboBox, CommonStrings::trDefaultParagraphStyle);
	else
		setCurrentComboItem(styleLevel1ComboBox, localIndexSetupVector[numSelected].level1Style);

	if (localIndexSetupVector[numSelected].level2Style == CommonStrings::None)
		setCurrentComboItem(styleLevel2ComboBox, CommonStrings::tr_None);
	else if (localIndexSetupVector[numSelected].level2Style == CommonStrings::DefaultParagraphStyle)
		setCurrentComboItem(styleLevel2ComboBox, CommonStrings::trDefaultParagraphStyle);
	else
		setCurrentComboItem(styleLevel2ComboBox, localIndexSetupVector[numSelected].level2Style);

	if (localIndexSetupVector[numSelected].level3Style == CommonStrings::None)
		setCurrentComboItem(styleLevel3ComboBox, CommonStrings::tr_None);
	else if (localIndexSetupVector[numSelected].level3Style == CommonStrings::DefaultParagraphStyle)
		setCurrentComboItem(styleLevel3ComboBox, CommonStrings::trDefaultParagraphStyle);
	else
		setCurrentComboItem(styleLevel3ComboBox, localIndexSetupVector[numSelected].level3Style);
}

void Prefs_Indexes::generatePageItemList()
{
	itemDestFrameComboBox->clear();
	itemDestFrameComboBox->addItem(CommonStrings::tr_None);
	if (m_Doc != nullptr)
	{
		QList<PageItem *> allItems;
		for (int i = 0; i < m_Doc->DocItems.count(); ++i)
		{
			PageItem *currItem = m_Doc->DocItems.at(i);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int j = 0; j < allItems.count(); j++)
			{
				currItem = allItems.at(j);
				if (currItem->itemType() == PageItem::TextFrame)
					itemDestFrameComboBox->addItem(currItem->itemName());
			}
			allItems.clear();
		}
	}
	else
		itemDestFrameComboBox->setEnabled(false);
}

void Prefs_Indexes::setIndexName(const QString &newName)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;

	indexListBox->item(numSelected)->setText(newName);
	int i = 0;
	IndexSetupVector::Iterator it = localIndexSetupVector.begin();
	while (it != localIndexSetupVector.end() && i < numSelected)
	{
		++it;
		++i;
	}
	(*it).name = newName;
}

void Prefs_Indexes::indexListWidgetItemEdited(QListWidgetItem *qlwi)
{
	setIndexName(qlwi->text());
}

void Prefs_Indexes::itemFrameSelected(const QString &frameName)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	localIndexSetupVector[numSelected].frameName = frameName;
}

void Prefs_Indexes::listNonPrintingChecked(bool b)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	localIndexSetupVector[numSelected].listNonPrintingFrames = b;
}

void Prefs_Indexes::combinePageNumbersChecked(bool b)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	localIndexSetupVector[numSelected].combinePageNumbers = b;
}

void Prefs_Indexes::combineIdenticalEntriesChecked(bool b)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	localIndexSetupVector[numSelected].combineIdenticalEntries = b;
	caseSensitiveCombinationCheckBox->setEnabled(b);
}

void Prefs_Indexes::caseSenstiveChecked(bool b)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	localIndexSetupVector[numSelected].caseSensitiveCombination = b;
}

void Prefs_Indexes::autoCapitalizeEntriesChecked(bool b)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	localIndexSetupVector[numSelected].autoCapitalizeEntries = b;
}

void Prefs_Indexes::addAlphaSeparatorsChecked(bool b)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	localIndexSetupVector[numSelected].addAlphaSeparators = b;
}
void Prefs_Indexes::headingStyleSelected(const QString &style)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	QString headingStyle = style;
	if (style == CommonStrings::trDefaultParagraphStyle)
		headingStyle = CommonStrings::DefaultParagraphStyle;
	localIndexSetupVector[numSelected].headingStyle = headingStyle;
}

void Prefs_Indexes::separatorStyleSelected(const QString &style)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	QString separatorStyle = style;
	if (style == CommonStrings::trDefaultParagraphStyle)
		separatorStyle = CommonStrings::DefaultParagraphStyle;
	localIndexSetupVector[numSelected].separatorStyle = separatorStyle;
}

void Prefs_Indexes::level1StyleSelected(const QString &style)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	QString level1Style = style;
	if (style == CommonStrings::trDefaultParagraphStyle)
		level1Style = CommonStrings::DefaultParagraphStyle;
	localIndexSetupVector[numSelected].level1Style = level1Style;
}

void Prefs_Indexes::level2StyleSelected(const QString &style)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	QString level2Style = style;
	if (style == CommonStrings::trDefaultParagraphStyle)
		level2Style = CommonStrings::DefaultParagraphStyle;
	localIndexSetupVector[numSelected].level2Style = level2Style;
}

void Prefs_Indexes::level3StyleSelected(const QString &style)
{
	if (numSelected < 0 || numSelected >= localIndexSetupVector.count())
		return;
	QString level3Style = style;
	if (style == CommonStrings::trDefaultParagraphStyle)
		level3Style = CommonStrings::DefaultParagraphStyle;
	localIndexSetupVector[numSelected].level2Style = level3Style;
}


