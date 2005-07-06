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
#include <qheader.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "loremipsum.h"
#include "loremipsum.moc"
#include "scribus.h"
#include "scpaths.h"
#include "serializer.h"

extern ScribusApp *ScApp;

QString getLoremLocation(QString fname)
{
	return QDir::convertSeparators(ScPaths::instance().shareDir() + "/loremipsum/" + fname);
}

LoremParser::LoremParser(QString fname)
{
	name = author = url = "n/a";
	QDomDocument doc("loremdoc");
	QFile file(getLoremLocation(fname));
	if (!file.open(IO_ReadOnly))
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
				loremIpsum.append(element.text());
		}
		node = node.nextSibling();
	}
}

QString LoremParser::createLorem(uint parCount)
{
	if (parCount < 1)
		return QString::null;
	// first paragraph is always the same
	QString lorem(loremIpsum[0]);
	for (uint i = 1; i < parCount + 1; ++i)
		lorem += loremIpsum[rand()%loremIpsum.count()] + '\n';
	return lorem.stripWhiteSpace();
}


LoremManager::LoremManager(QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog( parent, name, modal, fl )
{
	// setup checking
	if (!ScApp->HaveDoc)
		return;
	if (ScApp->view->SelItem.count() == 0)
	{
		ScApp->mainWindowStatusLabel->setText(tr("Select any text item to apply dummy text"));
		return;
	}

	// UI construction
	if ( !name )
		setName( "LoremManager" );
	LoremManagerLayout = new QGridLayout( this, 1, 1, 11, 6, "LoremManagerLayout");

	layout3 = new QVBoxLayout( 0, 0, 6, "layout3");

	loremList = new QListView( this, "loremList" );
	loremList->addColumn( tr( "Select Lorem Ipsum" ) );
	loremList->setRootIsDecorated(true);
	layout3->addWidget( loremList );

	layout2 = new QHBoxLayout( 0, 0, 6, "layout2");

	paraLabel = new QLabel( this, "paraLabel" );
	layout2->addWidget( paraLabel );

	paraBox = new QSpinBox( this, "paraBox" );
	paraBox->setMinValue( 1 );
	paraBox->setValue( 4 );
	layout2->addWidget( paraBox );
	paraSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( paraSpacer );
	layout3->addLayout( layout2 );

	alwaysCheckBox = new QCheckBox( this, "alwaysCheckBox" );
	layout3->addWidget( alwaysCheckBox );

	layout1 = new QHBoxLayout( 0, 0, 6, "layout1");
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
	clearWState( WState_Polished );

	// reading lorems
	QDir d(getLoremLocation(QString::null), "*.xml");

	const QFileInfoList *list = d.entryInfoList();
	QFileInfoListIterator it(*list);
	QFileInfo *fi;

	while ( (fi = it.current()) != 0 )
	{
		LoremParser *parser = new LoremParser(fi->fileName());
		availableLorems[parser->name] = fi->fileName();
		QListViewItem *item = new QListViewItem(loremList);
		item->setText(0, parser->name);
		new QListViewItem(item, tr("Author:") + " " + parser->author);
		new QListViewItem(item, tr("Get More:") + " " + parser->url);
		new QListViewItem(item, tr("XML File:") + " " + fi->fileName());
		loremList->insertItem(item);
		++it;
	}

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( okButton_clicked() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelButton_clicked() ) );
}

void LoremManager::languageChange()
{
	setCaption( tr( "Lorem Ipsum" ) );
	paraLabel->setText( tr( "Paragraphs:" ) );
	alwaysCheckBox->setText( tr( "&Always Use Selected Lorem Ipsum" ) );
	alwaysCheckBox->setAccel( QKeySequence( tr( "Alt+A" ) ) );
	okButton->setText( tr( "&OK" ) );
	okButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
	cancelButton->setText( tr( "&Cancel" ) );
	cancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

void LoremManager::okButton_clicked()
{
	QListViewItem *li = loremList->currentItem();
	LoremParser *lp = new LoremParser(availableLorems[li->text(0)]);

	for (uint i = 0; i < ScApp->view->SelItem.count(); ++i)
	{
		if (ScApp->view->SelItem.at(i) == NULL)
			continue;
		if (ScApp->view->SelItem.at(i)->itemType() != PageItem::TextFrame)
			continue;
		if (ScApp->view->SelItem.at(i)->itemText.count() != 0)
		{
			QString text = "<qt>" + tr("Do you really want to replace all your text\nin the frame named %1 with sample text?") + "</qt>";
			int t = QMessageBox::warning(ScApp, tr("Warning"),
						QString(text).arg(ScApp->view->SelItem.at(i)->itemName()),
						QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);
			if (t == QMessageBox::No)
				continue;
		}
		Serializer *ss = new Serializer("");
		if (ss != NULL)
		{
			ss->Objekt = lp->createLorem(paraBox->value());
			int st = ScApp->view->SelItem.at(i)->Doc->currentParaStyle;
			if (st > 5)
				ss->GetText(ScApp->view->SelItem.at(i), st, ScApp->view->SelItem.at(i)->Doc->docParagraphStyles[st].Font, ScApp->view->SelItem.at(i)->Doc->docParagraphStyles[st].FontSize, true);
			else
				ss->GetText(ScApp->view->SelItem.at(i), st, ScApp->view->SelItem.at(i)->IFont, ScApp->view->SelItem.at(i)->ISize, true);
			delete ss;
		}
		if (ScApp->view->SelItem.at(i)->Doc->docHyphenator->AutoCheck)
			ScApp->view->SelItem.at(i)->Doc->docHyphenator->slotHyphenate(ScApp->view->SelItem.at(i));
	}
	ScApp->view->updateContents();
	ScApp->slotDocCh();
	accept();
}

void LoremManager::cancelButton_clicked()
{
	reject();
}

