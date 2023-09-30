/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "loremipsum.h"

#include <memory>

#include <QDir>
#include <QDomDocument>
#include <QEvent>
#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QCheckBox>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

#include "appmodes.h"
#include "commonstrings.h"
#include "hyphenator.h"
#include "iconmanager.h"
#include "langmgr.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribusdoc.h"
#include "selection.h"
#include "undomanager.h"

QString getLoremLocation(const QString& filename)
{
	return QDir::toNativeSeparators(ScPaths::instance().shareDir() + "/loremipsum/" + filename);
}

LoremParser::LoremParser(const QString& filename)
{
	QDomDocument doc("loremdoc");
	QFile file(getLoremLocation(filename));
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();

	QDomElement docElement = doc.documentElement();

	QDomNode node = docElement.firstChild();
	while (!node.isNull())
	{
		QDomElement element = node.toElement();
		if (!element.isNull())
		{
			if (element.tagName() == "name")
				name = element.text();
			if (element.tagName() == "author")
				author = element.text();
			if (element.tagName() == "url")
				url = element.text();
			if (element.tagName() == "p")
			{
				QString txt = element.text().simplified();
				txt.replace(SpecialChars::OLD_LINEBREAK, SpecialChars::LINEBREAK);
				txt.replace(SpecialChars::OLD_NBSPACE, SpecialChars::NBSPACE);
				txt.replace(SpecialChars::OLD_NBHYPHEN, SpecialChars::NBHYPHEN);
				loremIpsum.append(txt);
			}
		}
		node = node.nextSibling();
	}
	if (name != "n/a")
		correct = true;
}

QString LoremParser::createLorem(uint parCount, bool random)
{
	if (parCount < 1)
		return QString();
	// first paragraph is always the same
	QString lorem;
	if (!loremIpsum.isEmpty())
		lorem = loremIpsum[0];
	if (parCount > 1 && !loremIpsum.isEmpty())
	{
		int maxParagraph = loremIpsum.count();
		if (random)
		{
			for (uint i = 1; i < parCount; ++i)
				lorem += SpecialChars::PARSEP + loremIpsum[rand() % maxParagraph];
		}
		else if (maxParagraph != 0)
		{
			int currentParagraph = 1;
			for (uint i = 1; i < parCount; ++i)
			{
				lorem += SpecialChars::PARSEP + loremIpsum[currentParagraph];
				currentParagraph++;
				if (currentParagraph >= maxParagraph)
					currentParagraph = 0;
			}
		}
	}
	return lorem.trimmed();
}


LoremManager::LoremManager(ScribusDoc* doc, QWidget* parent) : QDialog( parent )
{
	undoManager = UndoManager::instance();
	m_Doc = doc;
	setModal(true);
	setWindowTitle( tr( "Lorem Ipsum" ) );
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	LoremManagerLayout = new QGridLayout(this) ;
	LoremManagerLayout->setContentsMargins(9, 9, 9, 9);
	LoremManagerLayout->setSpacing(6);

	layout3 = new QVBoxLayout;
	layout3->setContentsMargins(0, 0, 0, 0);
	layout3->setSpacing(6);

	loremList = new QTreeWidget( this );
	loremList->setRootIsDecorated(true);
	loremList->setColumnCount(1);
	loremList->setHeaderLabel( tr("Select Lorem Ipsum"));
	loremList->header()->setSectionsClickable( false );
	loremList->header()->setSectionResizeMode( QHeaderView::ResizeToContents );
	loremList->setSelectionMode(QAbstractItemView::SingleSelection);
	layout3->addWidget( loremList );

	layout2 = new QHBoxLayout;
	layout2->setContentsMargins(0, 0, 0, 0);
	layout2->setSpacing(6);

	paraLabel = new QLabel( this );
	layout2->addWidget( paraLabel );

	paraBox = new QSpinBox( this );
	paraBox->setMinimum( 1 );
	paraBox->setValue(PrefsManager::instance().appPrefs.miscPrefs.paragraphsLI);
	layout2->addWidget( paraBox );

	paraSpacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( paraSpacer );
	layout3->addLayout( layout2 );

	randomCheckBox = new QCheckBox(this);
	randomCheckBox->setChecked(true);
	layout3->addWidget( randomCheckBox );
	appendCheckBox = new QCheckBox(this);
	appendCheckBox->setChecked(false);
	layout3->addWidget( appendCheckBox );

//	layout3->addLayout( layout4 );

	layout1 = new QHBoxLayout;
	layout1->setContentsMargins(0, 0, 0, 0);
	layout1->setSpacing(6);
	buttonSpacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( buttonSpacer );

	okButton = new QPushButton( this );
	layout1->addWidget( okButton );

	cancelButton = new QPushButton( this );
	layout1->addWidget( cancelButton );
	layout3->addLayout( layout1 );

	LoremManagerLayout->addLayout( layout3, 0, 0 );
	languageChange();

	// reading lorems
	QDir d(getLoremLocation(QString()), "*.xml");

	QFileInfoList list = d.entryInfoList();
	QListIterator<QFileInfo> it(list);
	QFileInfo fi;
	LanguageManager * langmgr( LanguageManager::instance() );

	while (it.hasNext())
	{
		fi = it.next();
		if (langmgr->getLangFromAbbrev(fi.baseName(), false).isEmpty())
			continue;
		std::unique_ptr<LoremParser> parser(new LoremParser(fi.fileName()));
		if (!parser->correct)
			continue;
		availableLorems[parser->name] = fi.fileName();
		auto *item = new QTreeWidgetItem(loremList);
		if (parser->name == "la")
			item->setText(0, standardloremtext);
		else
			item->setText(0, langmgr->getLangFromAbbrev(parser->name, true));
		QTreeWidgetItem *subItem;
		subItem = new QTreeWidgetItem(item);
		subItem->setText(0, tr("Author:") + " " + parser->author);
		subItem = new QTreeWidgetItem(item);
		subItem->setText(0, tr("Get More:") + " " + parser->url);
		subItem = new QTreeWidgetItem(item);
		subItem->setText(0, tr("XML File:") + " " + fi.fileName());
	}
	loremList->sortItems(0, Qt::AscendingOrder);
	loremList->setSortingEnabled(false);
	resize( QSize(320, 340).expandedTo(minimumSizeHint()) );

	QList<QTreeWidgetItem *> defItem;
	defItem = loremList->findItems(langmgr->getLangFromAbbrev(m_Doc->language(), true), Qt::MatchExactly);
	if (defItem.isEmpty())
		defItem = loremList->findItems(standardloremtext, Qt::MatchExactly);
	if (!defItem.isEmpty())
	{
		loremList->setCurrentItem(defItem[0]);
		defItem[0]->setSelected(true);
	}

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( loremList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(accept()));
}

void LoremManager::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void LoremManager::languageChange()
{
	setWindowTitle( tr( "Lorem Ipsum" ) );
	paraLabel->setText( tr( "Paragraphs:" ) );
	randomCheckBox->setText( tr("Random Paragraphs"));
	appendCheckBox->setText( tr("Append to existing text"));
	okButton->setText( CommonStrings::tr_OK );
	okButton->setShortcut( QKeySequence( tr( "Alt+O" ) ) );
	cancelButton->setText( CommonStrings::tr_Cancel );
	cancelButton->setShortcut( QKeySequence( tr( "Alt+C" ) ) );
	standardloremtext = tr("Standard Lorem Ipsum");
	paraBox->setToolTip( tr( "Number of paragraphs of selected sample text to insert" ) );
	loremList->setToolTip( tr( "List of languages available to insert sample text in" ) );
}

void LoremManager::insertLoremIpsum(const QString& name, int paraCount, bool random)
{
	//CB: Avox please make insertText for text frame to nuke all this
	// is it really applied?
// 	bool done = false;

	for (int i = 0; i < m_Doc->m_Selection->count(); ++i)
	{
		PageItem* currItem = m_Doc->m_Selection->itemAt(i);
		if (currItem == nullptr)
			continue;
		//do not insert lorem ipsum text into notes frames
		if (currItem->isNoteFrame())
			continue;
		//removing marks and notes from current text
//		if (currItem->isTextFrame() && !currItem->asTextFrame()->removeMarksFromText(!m_Doc->hasGUI()))
//			continue;
		PageItem *item2 = currItem;
		if (m_Doc->appMode == modeEditTable)
			item2 = currItem->asTable()->activeCell().textFrame();
		if (!item2->isTextFrame())
			continue;
		UndoTransaction activeTransaction;
		if (!appendCheckBox->isChecked() && item2->itemText.length() != 0)
		{
			if (UndoManager::undoEnabled())
				activeTransaction = undoManager->beginTransaction(Um::Selection, Um::IGroup, Um::AddLoremIpsum, "", Um::ICreate);
			item2->itemText.selectAll();
			item2->asTextFrame()->deleteSelectedTextFromFrame();
			//We don't need to open a dialog box as the user can undo this action.
			//Selection tempSelection(this, false);
			//tempSelection.addItem(item2, true);
			//m_Doc->itemSelection_ClearItem(&tempSelection);
			/* ClearItem() doesn't return true or false so
			the following test has to be done */
			if (item2->itemText.length() != 0)
				continue;
		}
		
		// K.I.S.S.:
		std::unique_ptr<LoremParser> lp(new LoremParser(name));
		QString sampleText = lp->createLorem(paraCount, random);
		if (UndoManager::undoEnabled())
		{
			auto *ss = new SimpleState(Um::AddLoremIpsum, "", Um::ICreate);
			ss->set("LOREM_FRAMETEXT");
			ss->set("ETEA", QString("insert_frametext"));
			ss->set("TEXT_STR", sampleText);
			ss->set("START", item2->itemText.length());
			undoManager->action(item2, ss);
		}
		if (activeTransaction)
			activeTransaction.commit();

		int l = item2->itemText.length();
		item2->itemText.insertChars(l, sampleText);
		if (m_Doc->docHyphenator->autoCheck())
			m_Doc->docHyphenator->slotHyphenate(item2);
		item2->asTextFrame()->invalidateLayout(true);
	}
	m_Doc->regionsChanged()->update(QRectF());
	m_Doc->changed();
}

QString LoremManager::loremIpsum() const
{
	LoremParser lp(getName());
	return lp.createLorem(paraBox->value(), randomCheckBox->isChecked());
}

int LoremManager::paragraphCount() const
{
	return paraBox->value();
}

bool LoremManager::randomize() const
{
	return randomCheckBox->isChecked();
}

QString LoremManager::getName() const
{
	const QTreeWidgetItem *li;
	if (loremList->currentItem()->parent() == nullptr)
		li = loremList->currentItem();
	else
		li = loremList->currentItem()->parent();

	QString name;
	if (li->text(0) == standardloremtext)
		name = "la";
	else
		name = LanguageManager::instance()->getAbbrevFromLang(li->text(0), false);

	return availableLorems[name];
}
