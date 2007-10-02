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

#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>

#include "langmgr.h"
#include "pageitem.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scpaths.h"
#include "selection.h"
#include "prefsmanager.h"
#include "commonstrings.h"
#include "hyphenator.h"
#include "util_icon.h"

QString getLoremLocation(QString fname)
{
	return QDir::convertSeparators(ScPaths::instance().shareDir() + "/loremipsum/" + fname);
}

LoremParser::LoremParser(QString fname)
{
	name = author = url = "n/a";
	correct = false;
	QDomDocument doc("loremdoc");
	QFile file(getLoremLocation(fname));
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
	while(!node.isNull())
	{
		QDomElement element = node.toElement();
		if(!element.isNull())
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

QString LoremParser::createLorem(uint parCount)
{
	if (parCount < 1)
		return QString::null;
	// first paragraph is always the same
	QString lorem(loremIpsum[0]);
	if (!loremIpsum.isEmpty())
		for (uint i = 1; i < parCount + 1; ++i)
			lorem += SpecialChars::PARSEP + loremIpsum[rand()%loremIpsum.count()];
	return lorem.trimmed();
}


LoremManager::LoremManager(ScribusDoc* doc, QWidget* parent) : QDialog( parent )
{
	m_Doc=doc;
	setModal(true);
	setWindowTitle( tr( "Lorem Ipsum" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	LoremManagerLayout = new QGridLayout(this) ;
	LoremManagerLayout->setMargin(10);
	LoremManagerLayout->setSpacing(5);

	layout3 = new QVBoxLayout;
	layout3->setMargin(0);
	layout3->setSpacing(5);

	loremList = new QTreeWidget( this );
	loremList->setRootIsDecorated(true);
	loremList->setColumnCount(1);
	loremList->setHeaderLabel( tr("Select Lorem Ipsum"));
	loremList->header()->setClickable( false );
	loremList->header()->setResizeMode( QHeaderView::ResizeToContents );
	loremList->setSelectionMode(QAbstractItemView::SingleSelection);
	layout3->addWidget( loremList );

	layout2 = new QHBoxLayout;
	layout2->setMargin(0);
	layout2->setSpacing(5);

	paraLabel = new QLabel( this );
	layout2->addWidget( paraLabel );

	paraBox = new QSpinBox( this );
	paraBox->setMinimum( 1 );
	paraBox->setValue(PrefsManager::instance()->appPrefs.paragraphsLI);
	layout2->addWidget( paraBox );
	paraSpacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( paraSpacer );
	layout3->addLayout( layout2 );

	layout1 = new QHBoxLayout;
	layout1->setMargin(0);
	layout1->setSpacing(5);
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
	QDir d(getLoremLocation(QString::null), "*.xml");

	QFileInfoList list = d.entryInfoList();
	QListIterator<QFileInfo> it(list);
	QFileInfo fi;
	langmgr = new LanguageManager();
	langmgr->init(false);

	while (it.hasNext())
	{
		fi = it.next();
		if (langmgr->getLangFromAbbrev(fi.baseName(), false).isEmpty())
			continue;
		LoremParser *parser = new LoremParser(fi.fileName());
		if (!parser->correct)
		{
			delete parser;
			continue;
		}
		availableLorems[parser->name] = fi.fileName();
		QTreeWidgetItem *item = new QTreeWidgetItem(loremList);
		if (parser->name=="la")
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
		delete parser;
	}
	loremList->sortItems(0, Qt::AscendingOrder);
	loremList->setSortingEnabled(false);
	resize( QSize(320, 340).expandedTo(minimumSizeHint()) );
	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( okButton_clicked() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelButton_clicked() ) );
	connect( loremList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(okButton_clicked()));
}

LoremManager::~LoremManager()
{
	delete langmgr;
}

void LoremManager::languageChange()
{
	setWindowTitle( tr( "Lorem Ipsum" ) );
	paraLabel->setText( tr( "Paragraphs:" ) );
	okButton->setText( CommonStrings::tr_OK );
	okButton->setShortcut( QKeySequence( tr( "Alt+O" ) ) );
	cancelButton->setText( CommonStrings::tr_Cancel );
	cancelButton->setShortcut( QKeySequence( tr( "Alt+C" ) ) );
	standardloremtext = tr("Standard Lorem Ipsum");
}

void LoremManager::okButton_clicked()
{
	// only top level items are taken
	QTreeWidgetItem *li;
	if (loremList->currentItem()->parent() == 0)
		li = loremList->currentItem();
	else
		li = loremList->currentItem()->parent();
	QString name;
	if (li->text(0)==standardloremtext)
		name="la";
	else
		name=langmgr->getAbbrevFromLang(li->text(0), true, false);
		
	insertLoremIpsum(availableLorems[name], paraBox->value());
	accept();
}

void LoremManager::cancelButton_clicked()
{
	reject();
}

void LoremManager::insertLoremIpsum(QString name, int paraCount)
{
	//CB: Avox please make insertText for text frame to nuke all this
	// is it really applied?
// 	bool done = false;

	for (int i = 0; i < m_Doc->m_Selection->count(); ++i)
	{
		PageItem* currItem=m_Doc->m_Selection->itemAt(i);
		if (currItem == NULL)
			continue;
		if (!currItem->asTextFrame())
			continue;
		if (currItem->itemText.length() != 0)
		{
			m_Doc->itemSelection_ClearItem();
			/* ClearItem() doesn't return true or false so
			the following test has to be done */
			if (currItem->itemText.length() != 0)
				continue;
		}
		LoremParser *lp = new LoremParser(name);
		if (lp == NULL)
		{
			qDebug("LoremManager::okButton_clicked() *lp == NULL");
			return;
		}

#if 0		
// 		Set up the gtWriter instance with the selected paragraph style
		gtWriter* writer = new gtWriter(false, currItem);
		if (writer != NULL)
		{
				writer->setUpdateParagraphStyles(false);
				writer->setOverridePStyleFont(false);
				gtFrameStyle* fstyle = writer->getDefaultStyle();
				gtParagraphStyle* pstyle = new gtParagraphStyle(*fstyle);
				pstyle->setName(currItem->currentStyle().name());
				writer->setParagraphStyle(pstyle);
				done = true;
				writer->append(lp->createLorem(paraCount));
		}
		delete writer;		
#endif
		
		// K.I.S.S.:
		currItem->itemText.insertChars(0, lp->createLorem(paraCount));
		delete lp;

		//if (ScMW->view->SelItem.at(i)->Doc->docHyphenator->AutoCheck)
		//	ScMW->view->SelItem.at(i)->Doc->docHyphenator->slotHyphenate(ScMW->view->SelItem.at(i));
		if (m_Doc->docHyphenator->AutoCheck)
			m_Doc->docHyphenator->slotHyphenate(currItem);
	}
// 	if (done)
// 	{
 		m_Doc->regionsChanged()->update(QRectF());
		m_Doc->changed();
// 	}
}
