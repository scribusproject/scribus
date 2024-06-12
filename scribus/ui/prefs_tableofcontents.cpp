/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QSignalBlocker>

#include "commonstrings.h"
#include "iconmanager.h"
#include "pagestructs.h"
#include "prefsstructs.h"
#include "scribusdoc.h"
#include "ui/prefs_tableofcontents.h"
#include "util.h"

Prefs_TableOfContents::Prefs_TableOfContents(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent),
	  m_Doc(doc)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Tables of Contents");
	m_icon = "tabtocindex_16.png";

	tocStyleUpButton->setIcon(IconManager::instance().loadIcon("16/go-up.png"));
	tocStyleDownButton->setIcon(IconManager::instance().loadIcon("16/go-down.png"));

	itemDestFrameComboBox->setMaximumWidth(fontMetrics().horizontalAdvance( "This is a very long Name" ));
	itemAttrComboBox->setMaximumWidth(fontMetrics().horizontalAdvance( "This is a very long Name" ));
	itemNumberPlacementComboBox->setMaximumWidth(fontMetrics().horizontalAdvance( "This is a very long Name" ));
	itemParagraphStyleComboBox->setMaximumWidth(fontMetrics().horizontalAdvance( "This is a very long Name" ));

	// signals and slots connections
	//do not connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	connect(tocAddButton, SIGNAL(clicked()), this, SLOT(addToC()));
	connect(tocDeleteButton, SIGNAL(clicked()), this, SLOT(deleteToC()));
	connect(itemToCSource, SIGNAL(currentTextChanged(QString)), this, SLOT(tocSourceSelected(QString)));
	connect(itemAttrComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemAttributeSelected(QString)));
	connect(itemDestFrameComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemFrameSelected(QString)));
	connect(itemParagraphStyleComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemParagraphStyleSelected(QString)));
	connect(itemNumberPlacementComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemPageNumberPlacedSelected(QString)));
	connect(itemListNonPrintingCheckBox, SIGNAL( toggled(bool) ), this, SLOT( nonPrintingFramesSelected(bool)));
	connect(styleListWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(styleListUpdate()));
	connect(styleListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(styleListWidgetClicked()));
	connect(addStyleButton, SIGNAL(clicked()), this, SLOT(addStyleClicked()));
	connect(deleteStyleButton, SIGNAL(clicked()), this, SLOT(removeStyleClicked()));
	connect(tocStyleUpButton, SIGNAL(clicked()), this, SLOT(tocStyleMoveUp()));
	connect(tocStyleDownButton, SIGNAL(clicked()), this, SLOT(tocStyleMoveDown()));
	connect(tocEntryStyleComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(tocEntryParagraphStyleSelected(QString)));
	connect(tocEntryNumberPlacementComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(tocEntryPageNumberPlacedSelected(QString)));
	connect(tocEntryRemoveLineBreaksCheckBox, SIGNAL(toggled(bool)), this, SLOT(tocEntryRemoveLineBreaksSelected(bool)));

	itemToCSource->setEnabled(false);
	itemAttrComboBox->setEnabled(false);
	itemDestFrameComboBox->setEnabled(false);
	itemParagraphStyleComboBox->setEnabled(false);
	itemNumberPlacementComboBox->setEnabled(false);
	itemListNonPrintingCheckBox->setEnabled(false);
	styleListWidget->setEnabled(false);
	paragraphStyleComboBox->setEnabled(false);
	addStyleButton->setEnabled(false);
	deleteStyleButton->setEnabled(false);
	tocEntryStyleComboBox->setEnabled(false);
	tocEntryNumberPlacementComboBox->setEnabled(false);


	setCurrentComboItem(itemNumberPlacementComboBox, trStrPNEnd);
}

Prefs_TableOfContents::~Prefs_TableOfContents() = default;


void Prefs_TableOfContents::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void Prefs_TableOfContents::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	localToCSetupVector = prefsData->tocPrefs.defaultToCSetups;
	generatePageItemList();
	bool enabled = (localToCSetupVector.count() > 0);
	if (enabled)
	{
		updateToCListBox();
		updateParagraphStyleComboBox();
		updateDocParagraphStyleComboBox();
		tocListBox->setCurrentRow(0);
		selectToC(0);
	}
	else
		tocListBox->clear();
	enableGUIWidgets();
	connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	connect( tocListBox, SIGNAL( itemChanged(QListWidgetItem*) ), this, SLOT( tocListWidgetItemEdited(QListWidgetItem*)));
}

void Prefs_TableOfContents::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->tocPrefs.defaultToCSetups = localToCSetupVector;
}

void Prefs_TableOfContents::languageChange()
{
	trStrPNBeginning = tr("Beginning");
	strPNBeginning = "Beginning";
	trStrPNEnd = tr("End");
	strPNEnd = "End";
	trStrPNNotShown = tr("Not Shown");
	strPNNotShown = "Not Shown";
	strTOCSrcStyle = "Style";
	trStrTOCSrcStyle = tr("Style");
	strTOCSrcAttribute = "Attribute";
	trStrTOCSrcAttribute = tr("Attribute");

	QSignalBlocker sigBlocker1(itemToCSource);
	QSignalBlocker sigBlocker2(itemNumberPlacementComboBox);
	QSignalBlocker sigBlocker3(tocEntryNumberPlacementComboBox);

	int i = itemToCSource->currentIndex();
	itemToCSource->clear();
	itemToCSource->addItem(trStrTOCSrcStyle);
	itemToCSource->addItem(trStrTOCSrcAttribute);
	itemToCSource->setCurrentIndex(i);

	i = itemNumberPlacementComboBox->currentIndex();
	itemNumberPlacementComboBox->clear();
	itemNumberPlacementComboBox->addItem(trStrPNEnd);
	itemNumberPlacementComboBox->addItem(trStrPNBeginning);
	itemNumberPlacementComboBox->addItem(trStrPNNotShown);
	itemNumberPlacementComboBox->setCurrentIndex(i);

	i = tocEntryNumberPlacementComboBox->currentIndex();
	tocEntryNumberPlacementComboBox->clear();
	tocEntryNumberPlacementComboBox->addItem(trStrPNEnd);
	tocEntryNumberPlacementComboBox->addItem(trStrPNBeginning);
	tocEntryNumberPlacementComboBox->addItem(trStrPNNotShown);
	tocEntryNumberPlacementComboBox->setCurrentIndex(i);
}

void Prefs_TableOfContents::destroy()
{

}

void Prefs_TableOfContents::generatePageItemList()
{
	itemDestFrameComboBox->clear();
	itemDestFrameComboBox->addItem(CommonStrings::tr_None);
	if (m_Doc != nullptr)
	{
		QList<PageItem*> allItems;
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

void Prefs_TableOfContents::setupItemAttrs(const QStringList& newNames)
{
	QSignalBlocker sigBlocker1(itemAttrComboBox);
	itemAttrComboBox->clear();
	itemAttrComboBox->addItem(CommonStrings::tr_None);
	itemAttrComboBox->addItems(newNames);
	if (numSelected != 999 && numSelected != -1)
	{
		if (localToCSetupVector[numSelected].itemAttrName == CommonStrings::None)
			setCurrentComboItem(itemAttrComboBox, CommonStrings::tr_None);
		else
			setCurrentComboItem(itemAttrComboBox, localToCSetupVector[numSelected].itemAttrName);
	}
}

void Prefs_TableOfContents::selectToC(int numberSelected)
{
	QSignalBlocker sigBlocker1(tocListBox);
	QSignalBlocker sigBlocker2(itemToCSource);
	QSignalBlocker sigBlocker3(itemAttrComboBox);
	QSignalBlocker sigBlocker4(itemDestFrameComboBox);
	QSignalBlocker sigBlocker5(itemParagraphStyleComboBox);
	QSignalBlocker sigBlocker6(itemNumberPlacementComboBox);
	QSignalBlocker sigBlocker7(itemListNonPrintingCheckBox);

	numSelected = numberSelected;
	if (numSelected < 0)
	{
		enableGUIWidgets();
		return;
	}
	if (localToCSetupVector.isEmpty())
		return;
	if (localToCSetupVector.count() < numSelected)
		numSelected = 0;

	setCurrentComboItem(itemToCSource, localToCSetupVector[numSelected].tocSource);

	enableGUIWidgets();

	if (localToCSetupVector[numSelected].itemAttrName == CommonStrings::None)
		setCurrentComboItem(itemAttrComboBox, CommonStrings::tr_None);
	else
		setCurrentComboItem(itemAttrComboBox, localToCSetupVector[numSelected].itemAttrName);

	if (localToCSetupVector[numSelected].pageLocation == NotShown)
		setCurrentComboItem(itemNumberPlacementComboBox, trStrPNNotShown);
	else if (localToCSetupVector[numSelected].pageLocation == Beginning)
		setCurrentComboItem(itemNumberPlacementComboBox, trStrPNBeginning);
	else
		setCurrentComboItem(itemNumberPlacementComboBox, trStrPNEnd);

	itemListNonPrintingCheckBox->setChecked(localToCSetupVector[numSelected].listNonPrintingFrames);
	if (m_Doc != nullptr)
	{
		if (localToCSetupVector[numSelected].frameName == CommonStrings::None)
			setCurrentComboItem(itemDestFrameComboBox, CommonStrings::tr_None);
		else
			setCurrentComboItem(itemDestFrameComboBox, localToCSetupVector[numSelected].frameName);

		if (itemParagraphStyleComboBox->count() > 0)
		{
			if (!paragraphStyleList.contains(localToCSetupVector[numSelected].textStyle) || localToCSetupVector[numSelected].textStyle == CommonStrings::None)
				setCurrentComboItem(itemParagraphStyleComboBox, CommonStrings::tr_None);
			else
				setCurrentComboItem(itemParagraphStyleComboBox, localToCSetupVector[numSelected].textStyle);
		}
	}
	styleListWidget->clear();
	//load styles if its a style toc
	if (localToCSetupVector[numSelected].tocSource == strTOCSrcStyle)
	{
		for (QList<ToCSetupEntryStyleData>::Iterator tocEntryIterator
			 = localToCSetupVector[numSelected].entryData.begin();
			 tocEntryIterator != localToCSetupVector[numSelected].entryData.end();
			 ++tocEntryIterator)
		{
			styleListWidget->addItem((*tocEntryIterator).styleToFind);
		}
	}
}




void Prefs_TableOfContents::updateToCListBox()
{
	tocListBox->clear();
	if (localToCSetupVector.isEmpty())
		return;
	QStringList sl;
	for (ToCSetupVector::Iterator it = localToCSetupVector.begin(); it!= localToCSetupVector.end(); ++it)
		sl << (*it).name;
	tocListBox->insertItems(0, sl);
	for (int i = 0; i < tocListBox->count(); i++)
	{
		QListWidgetItem *qlwi = tocListBox->item(i);
		if (qlwi != nullptr)
			qlwi->setFlags(qlwi->flags() | Qt::ItemIsEditable);
	}
}

void Prefs_TableOfContents::updateParagraphStyleComboBox()
{
	paragraphStyleList.clear();
	paragraphStyleList.append(CommonStrings::tr_None);

	if (m_Doc != nullptr) // && m_Doc->docParagraphStyles.count()>5)
	{
		for (int i = 0; i < m_Doc->paragraphStyles().count(); ++i)
			paragraphStyleList.append(m_Doc->paragraphStyles()[i].name());
	}
	itemParagraphStyleComboBox->clear();
	itemParagraphStyleComboBox->addItems(paragraphStyleList);
}

void Prefs_TableOfContents::updateDocParagraphStyleComboBox()
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
	tocEntryStyleComboBox->clear();
	paragraphStyleComboBox->clear();
	tocEntryStyleComboBox->addItems(stylesList);
	paragraphStyleComboBox->addItems(stylesList);
}
void Prefs_TableOfContents::enableGUIWidgets()
{
	bool tocExists = (localToCSetupVector.count() > 0);
	tocListBox->setEnabled(tocExists);
	bool haveTocSelected = (tocExists && (tocListBox->currentRow() >=0));
	tocDeleteButton->setEnabled(haveTocSelected);
	itemToCSource->setEnabled(haveTocSelected);
	itemListNonPrintingCheckBox->setEnabled(haveTocSelected);
	bool haveDoc = (haveTocSelected && m_Doc != nullptr);
	itemDestFrameComboBox->setEnabled(haveDoc);
	itemParagraphStyleComboBox->setEnabled(haveDoc);

	int i = itemToCSource->currentIndex();
	bool isStyle = (haveTocSelected && (i == 0));
	stackedWidget->setCurrentIndex(i);
	itemAttrComboBox->setEnabled(haveDoc && !isStyle);
	itemNumberPlacementComboBox->setEnabled(haveDoc && !isStyle);
	itemParagraphStyleComboBox->setEnabled(haveDoc && !isStyle);
	styleListWidget->setEnabled(isStyle);
	paragraphStyleComboBox->setEnabled(isStyle);
	addStyleButton->setEnabled(isStyle);
	deleteStyleButton->setEnabled(isStyle && (styleListWidget->count() > 0));
	tocStyleUpButton->setEnabled(isStyle);
	tocStyleDownButton->setEnabled(isStyle);
	tocEntryStyleComboBox->setEnabled(isStyle && styleListWidget->currentItem() != nullptr);
	tocEntryNumberPlacementComboBox->setEnabled(isStyle && styleListWidget->currentItem() != nullptr);
	tocEntryRemoveLineBreaksCheckBox->setEnabled(isStyle && styleListWidget->currentItem() != nullptr);
}


void Prefs_TableOfContents::addToC()
{
	QString newName;
	if (localToCSetupVector.isEmpty())
		newName = tr("Table of Contents");
	else
		newName = tr("Table of Contents %1").arg(localToCSetupVector.count() + 1);
	ToCSetup newToCEntry;
	newToCEntry.name = newName;
	newToCEntry.tocSource = trStrTOCSrcStyle;
	newToCEntry.itemAttrName = CommonStrings::None;
	newToCEntry.frameName = CommonStrings::None;
	newToCEntry.textStyle = CommonStrings::None;
	newToCEntry.pageLocation = End;
	newToCEntry.listNonPrintingFrames = false;
	localToCSetupVector.append(newToCEntry);
	QSignalBlocker sigBlocker1(tocListBox);
	updateToCListBox();
	updateParagraphStyleComboBox();
	updateDocParagraphStyleComboBox();
	tocListBox->setCurrentRow(localToCSetupVector.count() - 1);
	selectToC(localToCSetupVector.count() - 1);
	enableGUIWidgets();
}


void Prefs_TableOfContents::deleteToC()
{
	int numberSelected = tocListBox->currentRow();
	if (numberSelected < 0)
		return;

	int i = 0;
	ToCSetupVector::Iterator it = localToCSetupVector.begin();
	while (it!= localToCSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	localToCSetupVector.erase(it);

	QSignalBlocker sigBlocker(tocListBox);
	updateToCListBox();
	if (numberSelected < localToCSetupVector.count())
	{
		tocListBox->setCurrentRow(numberSelected);
		selectToC(numberSelected);
	}
	else if (localToCSetupVector.count() > 0)
	{
		tocListBox->setCurrentRow(localToCSetupVector.count() - 1);
		selectToC(localToCSetupVector.count() - 1);
	}
	if (localToCSetupVector.isEmpty())
		numSelected = -1;
	enableGUIWidgets();
}

void Prefs_TableOfContents::tocSourceSelected(const QString &tocSource)
{
	int numberSelected = tocListBox->currentRow();
	if (numberSelected < 0)
		return;

	int i = 0;
	ToCSetupVector::Iterator it = localToCSetupVector.begin();
	while (it != localToCSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	(*it).tocSource = tocSource;
	enableGUIWidgets();
}

void Prefs_TableOfContents::itemAttributeSelected( const QString& itemAttributeName )
{
	int numberSelected = tocListBox->currentRow();
	if (numberSelected < 0)
		return;

	int i = 0;
	ToCSetupVector::Iterator it = localToCSetupVector.begin();
	while (it != localToCSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	if (itemAttributeName == CommonStrings::tr_None)
		(*it).itemAttrName = CommonStrings::None;
	else
		(*it).itemAttrName = itemAttributeName;
}


void Prefs_TableOfContents::itemFrameSelected( const QString& frameName )
{
	int numberSelected = tocListBox->currentRow();
	if (numberSelected < 0)
		return;

	int i = 0;
	ToCSetupVector::Iterator it = localToCSetupVector.begin();
	while (it != localToCSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	if (frameName == CommonStrings::tr_None)
		(*it).frameName = CommonStrings::None;
	else
		(*it).frameName = frameName;
}


void Prefs_TableOfContents::itemPageNumberPlacedSelected( const QString& pageLocation )
{
	int numberSelected = tocListBox->currentRow();
	if (numberSelected < 0)
		return;

	int i = 0;
	ToCSetupVector::Iterator it = localToCSetupVector.begin();
	while (it != localToCSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	if (pageLocation == trStrPNBeginning || pageLocation == strPNBeginning)
		(*it).pageLocation = Beginning;
	else
	if (pageLocation == trStrPNEnd || pageLocation == strPNEnd)
		(*it).pageLocation = End;
	else
		(*it).pageLocation = NotShown;
}


void Prefs_TableOfContents::itemParagraphStyleSelected( const QString& itemStyle )
{
	int numberSelected = tocListBox->currentRow();
	if (numberSelected < 0)
		return;

	int i = 0;
	ToCSetupVector::Iterator it = localToCSetupVector.begin();
	while (it != localToCSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	if (itemStyle == CommonStrings::tr_None)
		(*it).textStyle = CommonStrings::None;
	else
		(*it).textStyle = itemStyle;
}

void Prefs_TableOfContents::setToCName( const QString &newName )
{
	int numberSelected = tocListBox->currentRow();
	if (numberSelected < 0)
		return;

	tocListBox->item(numberSelected)->setText(newName);
	int i = 0;
	ToCSetupVector::Iterator it = localToCSetupVector.begin();
	while (it != localToCSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	(*it).name = newName;
}


void Prefs_TableOfContents::nonPrintingFramesSelected( bool showNonPrinting )
{
	int numberSelected = tocListBox->currentRow();
	if (numberSelected < 0)
		return;

	int i = 0;
	ToCSetupVector::Iterator it = localToCSetupVector.begin();
	while (it != localToCSetupVector.end() && i < numberSelected)
	{
		++it;
		++i;
	}
	(*it).listNonPrintingFrames = showNonPrinting;
}

void Prefs_TableOfContents::styleListUpdate()
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	styleListWidget->clear();
	for (QList<ToCSetupEntryStyleData>::Iterator tocEntryIterator = localToCSetupVector[numSelected].entryData.begin();
		 tocEntryIterator != localToCSetupVector[numSelected].entryData.end();
		 ++tocEntryIterator)
	{
		styleListWidget->addItem((*tocEntryIterator).styleToFind);
	}
	deleteStyleButton->setEnabled(styleListWidget->count() != 0);
}

void Prefs_TableOfContents::addStyleClicked()
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	QString paraStyleToAdd(paragraphStyleComboBox->currentText());
	bool found = false;
	for (QList<ToCSetupEntryStyleData>::Iterator tocEntryIterator = localToCSetupVector[numSelected]
																		.entryData.begin();
		 tocEntryIterator != localToCSetupVector[numSelected].entryData.end();
		 ++tocEntryIterator)
	{
		if ((*tocEntryIterator).styleToFind == paraStyleToAdd)
			found = true;
	}
	if (!found)
	{
		ToCSetupEntryStyleData entryData;
		entryData.styleToFind = paraStyleToAdd;
		// entryData.styleForText = attrs.valueAsString("TOCStyle");
		entryData.removeLineBreaks = false;
		entryData.pageLocation = End;
		localToCSetupVector[numSelected].entryData.append(entryData);
	}
	styleListUpdate();
}

void Prefs_TableOfContents::removeStyleClicked()
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	QListWidgetItem *qlwi = styleListWidget->currentItem();
	if (qlwi == nullptr)
		return;
	QString paraStyleToDelete(qlwi->text());
	int i = 0;
	for (QList<ToCSetupEntryStyleData>::Iterator tocEntryIterator = localToCSetupVector[numSelected]
																		.entryData.begin();
		 tocEntryIterator != localToCSetupVector[numSelected].entryData.end();
		 ++tocEntryIterator,i++)
	{
		if ((*tocEntryIterator).styleToFind == paraStyleToDelete)
		{
			localToCSetupVector[numSelected].entryData.remove(i);
			break;
		}
	}
	styleListUpdate();
}

void Prefs_TableOfContents::tocListWidgetItemEdited(QListWidgetItem* qlwi)
{
	setToCName(qlwi->text());
}

void Prefs_TableOfContents::tocStyleMoveUp()
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	int curr = styleListWidget->currentRow();
	if (curr <= 0)
		return;
	localToCSetupVector[numSelected].entryData.swapItemsAt(curr, curr - 1);
	styleListUpdate();
	styleListWidget->setCurrentRow(curr - 1);
}

void Prefs_TableOfContents::tocStyleMoveDown()
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	int curr = styleListWidget->currentRow();
	if (curr == styleListWidget->count() - 1)
		return;
	localToCSetupVector[numSelected].entryData.swapItemsAt(curr, curr + 1);
	styleListUpdate();
	styleListWidget->setCurrentRow(curr + 1);
}

void Prefs_TableOfContents::styleListWidgetClicked()
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	int curr = styleListWidget->currentRow();
	if (curr < 0)
		return;
	QSignalBlocker sigBlocker1(tocEntryStyleComboBox);
	QSignalBlocker sigBlocker2(tocEntryNumberPlacementComboBox);
	QSignalBlocker sigBlocker3(tocEntryRemoveLineBreaksCheckBox);
	int i = tocEntryStyleComboBox->findText(localToCSetupVector[numSelected].entryData[curr].styleForText);
	if (i >= 0)
		tocEntryStyleComboBox->setCurrentIndex(i);
	else
		qDebug() << "Style for TOC Entry not found";
	if (localToCSetupVector[numSelected].entryData[curr].pageLocation == NotShown)
		setCurrentComboItem(tocEntryNumberPlacementComboBox, trStrPNNotShown);
	else if (localToCSetupVector[numSelected].entryData[curr].pageLocation == Beginning)
		setCurrentComboItem(tocEntryNumberPlacementComboBox, trStrPNBeginning);
	else
		setCurrentComboItem(tocEntryNumberPlacementComboBox, trStrPNEnd);
	tocEntryRemoveLineBreaksCheckBox->setChecked(localToCSetupVector[numSelected].entryData[curr].removeLineBreaks);
	bool isStyle = (itemToCSource->currentIndex() == 0);
	tocEntryStyleComboBox->setEnabled(isStyle);
	tocEntryNumberPlacementComboBox->setEnabled(isStyle);
	tocEntryRemoveLineBreaksCheckBox->setEnabled(isStyle);
	deleteStyleButton->setEnabled(true);
}

void Prefs_TableOfContents::tocEntryParagraphStyleSelected(const QString &style)
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	int curr = styleListWidget->currentRow();
	if (curr < 0)
		return;
	localToCSetupVector[numSelected].entryData[curr].styleForText = style;
}

void Prefs_TableOfContents::tocEntryPageNumberPlacedSelected(const QString &pageLocation)
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	int curr = styleListWidget->currentRow();
	if (curr < 0)
		return;
	if (pageLocation == trStrPNBeginning || pageLocation == strPNBeginning)
		localToCSetupVector[numSelected].entryData[curr].pageLocation = Beginning;
	else if (pageLocation == trStrPNEnd || pageLocation == strPNEnd)
		localToCSetupVector[numSelected].entryData[curr].pageLocation = End;
	else
		localToCSetupVector[numSelected].entryData[curr].pageLocation = NotShown;
}
void Prefs_TableOfContents::tocEntryRemoveLineBreaksSelected(bool removeLineBreaks)
{
	if (numSelected < 0 || numSelected >= localToCSetupVector.count())
		return;
	int curr = styleListWidget->currentRow();
	if (curr < 0)
		return;
	localToCSetupVector[numSelected].entryData[curr].removeLineBreaks = removeLineBreaks;
}
