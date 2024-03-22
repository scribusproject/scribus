/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QSignalBlocker>

#include "commonstrings.h"
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

	itemDestFrameComboBox->setMaximumWidth(fontMetrics().horizontalAdvance( "This is a very long Name" ));
	itemAttrComboBox->setMaximumWidth(fontMetrics().horizontalAdvance( "This is a very long Name" ));
	itemNumberPlacementComboBox->setMaximumWidth(fontMetrics().horizontalAdvance( "This is a very long Name" ));
	itemParagraphStyleComboBox->setMaximumWidth(fontMetrics().horizontalAdvance( "This is a very long Name" ));

	// signals and slots connections
	//do not connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	connect( tocAddButton, SIGNAL( clicked() ), this, SLOT( addToC() ) );
	connect( tocDeleteButton, SIGNAL( clicked() ), this, SLOT( deleteToC() ) );
	connect( itemToCSource, SIGNAL(currentTextChanged(QString)), this, SLOT(tocSourceSelected(QString)));
	connect( itemAttrComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemAttributeSelected(QString)));
	connect( itemDestFrameComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemFrameSelected(QString)));
	connect( itemParagraphStyleComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemParagraphStyleSelected(QString)));
	connect( itemNumberPlacementComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemPageNumberPlacedSelected(QString)));
	connect( tocNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT( setToCName(QString)));
	connect( itemListNonPrintingCheckBox, SIGNAL( toggled(bool) ), this, SLOT( nonPrintingFramesSelected(bool)));
	connect(removeLineBreaksCheckBox, SIGNAL(toggled(bool)), this, SLOT(removeLineBreaksSelected(bool)));
	connect(styleListWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(styleListUpdate()));
	connect(styleListTOCWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(styleListTOCUpdate()));
	connect(addStyleButton, SIGNAL(clicked()), this, SLOT(addStyleClicked()));
	connect(deleteStyleButton, SIGNAL(clicked()), this, SLOT(removeStyleClicked()));
	connect(addStyleTOCButton, SIGNAL(clicked()), this, SLOT(addStyleTOCClicked()));
	connect(deleteStyleTOCButton, SIGNAL(clicked()), this, SLOT(removeStyleTOCClicked()));

	itemToCSource->setEnabled(false);
	itemAttrComboBox->setEnabled(false);
	itemDestFrameComboBox->setEnabled(false);
	itemParagraphStyleComboBox->setEnabled(false);
	itemNumberPlacementComboBox->setEnabled(false);
	itemListNonPrintingCheckBox->setEnabled(false);
	removeLineBreaksCheckBox->setEnabled(false);
	styleListWidget->setEnabled(false);
	styleListTOCWidget->setEnabled(false);
	paragraphStyleComboBox->setEnabled(false);
	paragraphStyleTOCComboBox->setEnabled(false);
	addStyleButton->setEnabled(false);
	deleteStyleButton->setEnabled(false);
	addStyleTOCButton->setEnabled(false);
	deleteStyleTOCButton->setEnabled(false);

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

	disconnect(itemToCSource, SIGNAL(currentTextChanged(QString)), this, SLOT(tocSourceSelected(QString)));
	int i = itemToCSource->currentIndex();
	itemToCSource->clear();
	itemToCSource->addItem(trStrTOCSrcStyle);
	itemToCSource->addItem(trStrTOCSrcAttribute);
	itemToCSource->setCurrentIndex(i);
	connect(itemToCSource, SIGNAL(currentTextChanged(QString)), this, SLOT(tocSourceSelected(QString)));

	disconnect(itemNumberPlacementComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemPageNumberPlacedSelected(QString)));
	i = itemNumberPlacementComboBox->currentIndex();
	itemNumberPlacementComboBox->clear();
	itemNumberPlacementComboBox->addItem(trStrPNEnd);
	itemNumberPlacementComboBox->addItem(trStrPNBeginning);
	itemNumberPlacementComboBox->addItem(trStrPNNotShown);
	itemNumberPlacementComboBox->setCurrentIndex(i);
	connect(itemNumberPlacementComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemPageNumberPlacedSelected(QString)));
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
	disconnect(itemAttrComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemAttributeSelected(QString)));
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
	connect(itemAttrComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemAttributeSelected(QString)));
}

void Prefs_TableOfContents::selectToC(int numberSelected)
{
	disconnect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	disconnect( itemToCSource, SIGNAL(currentTextChanged(QString)), this, SLOT(tocSourceSelected(QString)));
	disconnect( itemAttrComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemAttributeSelected(QString)) );
	disconnect( itemDestFrameComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemFrameSelected(QString)) );
	disconnect( itemParagraphStyleComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemParagraphStyleSelected(QString)));
	disconnect( itemNumberPlacementComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemPageNumberPlacedSelected(QString)));
	disconnect( tocNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setToCName(QString)));
	disconnect( itemListNonPrintingCheckBox, SIGNAL( toggled(bool) ), this, SLOT( nonPrintingFramesSelected(bool) ) );

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
	removeLineBreaksCheckBox->setChecked(localToCSetupVector[numSelected].removeLineBreaks);
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
	styleListTOCWidget->clear();
	//load styles if its a style toc
	if (localToCSetupVector[numSelected].tocSource == strTOCSrcStyle)
	{
		styleListWidget->addItems(localToCSetupVector[numSelected].styleListToFind);
		styleListTOCWidget->addItems(localToCSetupVector[numSelected].styleListForTOC);
	}

	if (tocListBox->currentItem())
		tocNameLineEdit->setText(tocListBox->currentItem()->text());

	connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	connect( itemToCSource, SIGNAL(currentTextChanged(QString)), this, SLOT(tocSourceSelected(QString)));
	connect( itemAttrComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemAttributeSelected(QString)) );
	connect( itemDestFrameComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemFrameSelected(QString)) );
	connect( itemParagraphStyleComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemParagraphStyleSelected(QString)));
	connect( itemNumberPlacementComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(itemPageNumberPlacedSelected(QString)));
	connect( tocNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setToCName(QString)));
	connect( itemListNonPrintingCheckBox, SIGNAL( toggled(bool) ), this, SLOT( nonPrintingFramesSelected(bool) ) );
}


void Prefs_TableOfContents::addToC()
{
	bool found = false;
	QString newName = tocNameLineEdit->text();
	for (ToCSetupVector::Iterator it = localToCSetupVector.begin(); it!= localToCSetupVector.end(); ++it)
	{
		if ((*it).name == newName)
			found = true;
	}
	if (found || newName.isEmpty())
	{
		if(localToCSetupVector.isEmpty())
			newName = tr("Table of Contents");
		else
			newName = tr("Table of Contents %1").arg(localToCSetupVector.count() + 1);
	}
	ToCSetup newToCEntry;
	newToCEntry.name = newName;
	newToCEntry.tocSource = trStrTOCSrcStyle;
	newToCEntry.itemAttrName = CommonStrings::None;
	newToCEntry.frameName = CommonStrings::None;
	newToCEntry.textStyle = CommonStrings::None;
	newToCEntry.pageLocation = End;
	newToCEntry.listNonPrintingFrames = false;
	newToCEntry.removeLineBreaks = false;
	localToCSetupVector.append(newToCEntry);
	disconnect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
	updateToCListBox();
	if (localToCSetupVector.count() == 1) //reinit parastyles if we are adding the first TOC
		updateParagraphStyleComboBox();
	tocListBox->setCurrentRow(localToCSetupVector.count() - 1);
	selectToC(localToCSetupVector.count() - 1);
	enableGUIWidgets();
	connect( tocListBox, SIGNAL( currentRowChanged(int) ), this, SLOT( selectToC(int) ) );
}


void Prefs_TableOfContents::updateToCListBox()
{
	tocListBox->clear();
	QStringList sl;
	for (ToCSetupVector::Iterator it = localToCSetupVector.begin(); it!= localToCSetupVector.end(); ++it)
		sl << (*it).name;
	tocListBox->insertItems(0, sl);
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
	QStringList stylesList;
	paragraphStyleComboBox->clear();
	paragraphStyleComboBox->addItem( CommonStrings::trDefaultParagraphStyle );
	for (int i = 0; i < m_Doc->paragraphStyles().count(); ++i)
	{
		const ParagraphStyle &paraStyle = m_Doc->paragraphStyles()[i];
		if (!paraStyle.name().isEmpty() && !paraStyle.isDefaultStyle())
			stylesList.append(paraStyle.name());
	}
	stylesList.sort();
	paragraphStyleComboBox->addItems(stylesList);
	paragraphStyleTOCComboBox->addItems(stylesList);
}
void Prefs_TableOfContents::enableGUIWidgets()
{
	bool enabled = (localToCSetupVector.count() > 0);
	tocListBox->setEnabled(enabled);

	bool haveCurrToc = (enabled && (tocListBox->currentRow() >=0));
	tocDeleteButton->setEnabled(haveCurrToc);
	itemToCSource->setEnabled(haveCurrToc);
	itemAttrComboBox->setEnabled(haveCurrToc);
	itemNumberPlacementComboBox->setEnabled(haveCurrToc);
	itemListNonPrintingCheckBox->setEnabled(haveCurrToc);
	removeLineBreaksCheckBox->setEnabled(haveCurrToc);
	bool haveDoc = (haveCurrToc && m_Doc != nullptr);
	itemDestFrameComboBox->setEnabled(haveDoc);
	itemParagraphStyleComboBox->setEnabled(haveDoc);

	int i = itemToCSource->currentIndex();
	bool isStyle = (haveCurrToc && (i == 0));

	itemAttrComboBox->setEnabled(haveDoc && !isStyle);
	itemParagraphStyleComboBox->setEnabled(haveDoc && isStyle);
	styleListWidget->setEnabled(isStyle);
	styleListTOCWidget->setEnabled(isStyle);
	paragraphStyleComboBox->setEnabled(isStyle);
	paragraphStyleTOCComboBox->setEnabled(isStyle);
	addStyleButton->setEnabled(isStyle);
	deleteStyleButton->setEnabled(isStyle && (styleListWidget->count() > 0));
	addStyleTOCButton->setEnabled(isStyle);
	deleteStyleTOCButton->setEnabled(isStyle && (styleListTOCWidget->count() > 0));
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

void Prefs_TableOfContents::removeLineBreaksSelected(bool removeLineBreaks)
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
	(*it).removeLineBreaks = removeLineBreaks;
}

void Prefs_TableOfContents::styleListUpdate()
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
	(*it).styleListToFind.clear();
	for (int i = 0; i < styleListWidget->count(); i++)
		(*it).styleListToFind.append(styleListWidget->item(i)->text());
	if (styleListWidget->count() == 0)
		deleteStyleButton->setEnabled(false);
}

void Prefs_TableOfContents::styleListTOCUpdate()
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
	(*it).styleListForTOC.clear();
	for (int i = 0; i < styleListTOCWidget->count(); i++)
		(*it).styleListForTOC.append(styleListTOCWidget->item(i)->text());
	if (styleListTOCWidget->count() == 0)
		deleteStyleTOCButton->setEnabled(false);
}

void Prefs_TableOfContents::addStyleClicked()
{
	QString paraStyleToAdd(paragraphStyleComboBox->currentText());
	QList<QListWidgetItem *> items = styleListWidget->findItems(paraStyleToAdd, Qt::MatchExactly);
	if (items.isEmpty())
		styleListWidget->addItem(paraStyleToAdd);
	styleListUpdate();
}

void Prefs_TableOfContents::removeStyleClicked()
{
	delete styleListWidget->takeItem(styleListWidget->currentRow());
	styleListUpdate();
}

void Prefs_TableOfContents::addStyleTOCClicked()
{
	QString paraStyleToAdd(paragraphStyleTOCComboBox->currentText());
	QList<QListWidgetItem *> items = styleListTOCWidget->findItems(paraStyleToAdd, Qt::MatchExactly);
	if (items.isEmpty())
		styleListTOCWidget->addItem(paraStyleToAdd);
	styleListTOCUpdate();
}

void Prefs_TableOfContents::removeStyleTOCClicked()
{
	delete styleListTOCWidget->takeItem(styleListTOCWidget->currentRow());
	styleListTOCUpdate();
}
