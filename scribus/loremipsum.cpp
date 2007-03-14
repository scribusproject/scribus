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

#include <qstring.h>
#include <qstringlist.h>
#include <qdom.h>
#include <qdir.h>
#include <qfile.h>
#include <qvariant.h>
#include <qpushbutton.h>
#include <q3header.h>
#include <q3listview.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3VBoxLayout>

#include "loremipsum.h"
//#include "loremipsum.moc"

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
#include "util.h"
// #include "gtparagraphstyle.h"
// #include "gtframestyle.h"
// #include "gtwriter.h"

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
				loremIpsum.append(element.text().simplifyWhiteSpace());
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
			lorem += loremIpsum[rand()%loremIpsum.count()] + SpecialChars::PARSEP;
	return lorem.stripWhiteSpace();
}


LoremManager::LoremManager(ScribusDoc* doc, QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
	: QDialog( parent, name, modal, fl )
{
	m_Doc=doc;
	if ( !name )
		setName( "LoremManager" );
	LoremManagerLayout = new Q3GridLayout( this, 1, 1, 11, 6, "LoremManagerLayout");

	layout3 = new Q3VBoxLayout( 0, 0, 6, "layout3");

	loremList = new Q3ListView( this, "loremList" );
	loremList->addColumn( tr( "Select Lorem Ipsum" ) );
	loremList->setRootIsDecorated(true);
	layout3->addWidget( loremList );

	layout2 = new Q3HBoxLayout( 0, 0, 6, "layout2");

	paraLabel = new QLabel( this, "paraLabel" );
	layout2->addWidget( paraLabel );

	paraBox = new QSpinBox( this, "paraBox" );
	paraBox->setMinValue( 1 );
	paraBox->setValue(PrefsManager::instance()->appPrefs.paragraphsLI);
	layout2->addWidget( paraBox );
	paraSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( paraSpacer );
	layout3->addLayout( layout2 );

	layout1 = new Q3HBoxLayout( 0, 0, 6, "layout1");
	buttonSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( buttonSpacer );

	okButton = new QPushButton( this, "okButton" );
	layout1->addWidget( okButton );

	cancelButton = new QPushButton( this, "cancelButton" );
	layout1->addWidget( cancelButton );
	layout3->addLayout( layout1 );

	LoremManagerLayout->addLayout( layout3, 0, 0 );
	languageChange();
	resize( QSize(439, 364).expandedTo(minimumSizeHint()) );
	//qt4 clearWState( WState_Polished );

	// reading lorems
	QDir d(getLoremLocation(QString::null), "*.xml");

	const QFileInfoList *list = d.entryInfoList();
	QFileInfoListIterator it(*list);
	QFileInfo *fi=0;
	langmgr=new LanguageManager();
	langmgr->init(false);
	
	while ( (fi = it.current()) != 0 )
	{
		if (langmgr->getLangFromAbbrev(fi->baseName(), false).isEmpty())
		{
			++it;
			continue;
		}
		LoremParser *parser = new LoremParser(fi->fileName());
		if (!parser->correct)
		{
			delete parser;
			++it;
			continue;
		}
		availableLorems[parser->name] = fi->fileName();
		Q3ListViewItem *item = new Q3ListViewItem(loremList);
		if (parser->name=="la")
			item->setText(0,standardloremtext);
		else
			item->setText(0, langmgr->getLangFromAbbrev(parser->name, true));
		new Q3ListViewItem(item, tr("Author:") + " " + parser->author);
		new Q3ListViewItem(item, tr("Get More:") + " " + parser->url);
		new Q3ListViewItem(item, tr("XML File:") + " " + fi->fileName());
		loremList->insertItem(item);
		++it;
		delete parser;
	}

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( okButton_clicked() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelButton_clicked() ) );
	connect( loremList, SIGNAL(doubleClicked(Q3ListViewItem *, const QPoint &, int)), this, SLOT(okButton_clicked()));
}

LoremManager::~LoremManager()
{
	delete langmgr;
}

void LoremManager::languageChange()
{
	setCaption( tr( "Lorem Ipsum" ) );
	paraLabel->setText( tr( "Paragraphs:" ) );
	okButton->setText( CommonStrings::tr_OK );
	okButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
	cancelButton->setText( CommonStrings::tr_Cancel );
	cancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
	standardloremtext = tr("Standard Lorem Ipsum");
}

void LoremManager::okButton_clicked()
{
	// only top level items are taken
	Q3ListViewItem *li;
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

	for (uint i = 0; i < m_Doc->m_Selection->count(); ++i)
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
		m_Doc->view()->updateContents();
// 		m_Doc->view()->DrawNew();
		m_Doc->changed();
// 	}
}
