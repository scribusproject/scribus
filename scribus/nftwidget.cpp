/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include "nftwidget.h"

#include "scconfig.h"
#include "scribusapi.h"

#include <QAction>
#include <QPainter>

#include "util_icon.h"


nftwidget::nftwidget(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
}

void nftwidget::setupSettings(QString lang, QString templateDir)
{
	settings = new nftsettings(lang, templateDir);
	// context menu
	removeAction = new QAction(tr("&Remove"), tnailGrid);
	openAction = new QAction(tr("&Open"), tnailGrid);
	tnailGrid->addAction(removeAction);
	tnailGrid->addAction(openAction);
	setupAbout();
	toolBox->setItemIcon(0, loadIcon("16/information.png"));
	toolBox->setItemIcon(1, loadIcon("16/image-x-generic.png"));
	toolBox->setItemIcon(2, loadIcon("16/help-browser.png"));
	tnailGrid->setIconSize(QSize(60, 60));
	// Signals and Slots Connections
	connect(categoryList, SIGNAL(itemSelectionChanged()), this, SLOT(setTNails()));
	connect(tnailGrid, SIGNAL(itemSelectionChanged()), this, SLOT(setInfo()));
	connect(removeAction, SIGNAL(triggered()), this, SLOT(removeTemplate()));
	connect(openAction, SIGNAL(triggered()), this, SIGNAL(leaveOK()));
	connect(tnailGrid, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SIGNAL(leaveOK()));
	setupListItems();
	setupCategories();
}

void nftwidget::setupCategories() 
{
	QString categories("");
	categoryList->clear();
	categoryList->addItem(tr("All"));
	for (uint i = 0; i < settings->templates.size(); i++)
	{
		if ((categories.indexOf(settings->templates[i]->templateCategory) == -1) &&
		    (!settings->templates[i]->isDeleted))
		{
			categoryList->addItem(settings->templates[i]->templateCategory);
			categories += settings->templates[i]->templateCategory;
		}
	}
	categoryList->sortItems();
	categoryList->setCurrentRow(0);
}

void nftwidget::setupListItems()
{
	iconItems.clear();
	for (uint i = 0; i < settings->templates.size(); ++i)
	{
		if (!settings->templates[i]->isDeleted)
		{
			ListItem* tmp = new ListItem(settings->templates[i], NULL);
			iconItems.push_back(tmp);
		}
	}
}

void nftwidget::setTNails()
{
	if (categoryList->currentRow() == 0)
	{
		tnailGrid->clear();
		for (uint i = 0; i < iconItems.size(); ++i) 
		{
			QPixmap pm(iconItems[i]->first->tnail);
			if (pm.width() > 60)
				pm = pm.scaledToWidth(60, Qt::SmoothTransformation);
			if (pm.height() > 60)
				pm = pm.scaledToHeight(60, Qt::SmoothTransformation);
			QPixmap pmd(60, 60);
			pmd.fill(palette().color(QPalette::Base));
			QPainter p;
			p.begin(&pmd);
			p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
			p.end();
			QListWidgetItem* tmpQIVI = new QListWidgetItem(pmd, iconItems[i]->first->name, tnailGrid);
			iconItems[i]->second = tmpQIVI;
		}
		tnailGrid->sortItems();
		return;
	}
	
	QString curtype(categoryList->currentItem()->text());
	if (curtype != QString::null)
	{
		tnailGrid->clear();
		for (uint i = 0; i < iconItems.size(); ++i)
		{
			if (curtype.indexOf(iconItems[i]->first->templateCategory) != -1)
			{
				QPixmap pm(iconItems[i]->first->tnail);
				if (pm.width() > 60)
					pm = pm.scaledToWidth(60, Qt::SmoothTransformation);
				if (pm.height() > 60)
					pm = pm.scaledToHeight(60, Qt::SmoothTransformation);
				QPixmap pmd(60, 60);
				pmd.fill(palette().color(QPalette::Base));
				QPainter p;
				p.begin(&pmd);
				p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
				p.end();
				QListWidgetItem* tmpQIVI = new QListWidgetItem(pmd, iconItems[i]->first->name, tnailGrid);
				iconItems[i]->second = tmpQIVI;
			} 
			else
				iconItems[i]->second = NULL;
		}
		tnailGrid->sortItems();
	}
}

void nftwidget::setInfo() 
{
	QList<QListWidgetItem *> items = tnailGrid->selectedItems();
	if (items.count() <= 0)
		return;
	getCurrentDocumentTemplate(items.at(0));
	QString infoText = "<b>"+ tr("Name")+"</b><br>";
	infoText += currentDocumentTemplate->name + "<br>";
	infoText += "<b>"+ tr("Page Size")+"</b><br>";
	infoText += currentDocumentTemplate->psize + "<br>";
	infoText += "<b>"+ tr("Colors")+"</b><br>";
	infoText += currentDocumentTemplate->color + "<br>";
	infoText += "<b>"+ tr("Description")+"</b><br>";
	infoText += currentDocumentTemplate->descr + "<br>";
	infoText += "<b>"+ tr("Usage")+"</b><br>";
	infoText += currentDocumentTemplate->usage + "<br>";
	infoText += "<b>"+ tr("Created with")+"</b><br>";
	infoText += "Scribus " + currentDocumentTemplate->scribusVersion + "<br>";
	infoText += "<b>"+ tr("Date")+"</b><br>";
	infoText += currentDocumentTemplate->date + "<br>";
	infoText += "<b>"+ tr("Author")+"</b><br>";
	infoText += currentDocumentTemplate->author + "<br>";
	infoText += currentDocumentTemplate->email + "<br>";
	textBrowser->setText(infoText);
	imageView->clear();
	QPixmap tmplImg(currentDocumentTemplate->img);
	imageView->setIconSize(tmplImg.size());
	new QListWidgetItem(tmplImg, currentDocumentTemplate->name, imageView);
	emit ButtonBoxEnable(true);
}

void nftwidget::setupAbout()
{
	QString text = "New From Template - 0.0.7<br><br>";
	text += "<b>";
	text += tr("Downloading Templates");
	text += "</b><br>";
	text += tr("Document templates can be found at "
	           "http://www.scribus.net/ in the Downloads section.");
	text += "<br><br>";
	text += "<b>";
	text +=  tr("Installing Templates");
	text +=  "</b><br>";
	text += tr("Extract the package to the template directory " 
	           "~/.scribus/templates "
	           "for the current user or "
	           "PREFIX/share/scribus/templates "
	           "for all users in the system.");
	text +=  "<br><br>";
	text += "<b>";
	text +=  tr("Preparing a template");
	text +=  "</b><br>";
	text +=  tr("Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.");
	text += tr("The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.");
	text +=  "<br><br><b>";
	text +=  tr("Removing a template");
	text +=  "</b><br>";
	text += tr("Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.");
	text +=  "<br><br>";
	text += "<b>";
	text +=  tr("Translating template.xml");
	text +=  "</b><br>";
	text += tr("Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.");
	helpBrowser->setText(text);
}

void nftwidget::removeTemplate()
{
	currentDocumentTemplate->isDeleted = true;
	textBrowser->clear();
	imageView->clear();
	currentDocumentTemplate = NULL;
	emit ButtonBoxEnable(false);
	setupListItems();
	setupCategories();
	setTNails();
}

void nftwidget::getCurrentDocumentTemplate(QListWidgetItem* item)
{
	for (uint i = 0; i < iconItems.size(); ++i)
	{
		if (iconItems[i]->second == item)
		{
			currentDocumentTemplate = iconItems[i]->first;
			break;
		}
	}
}

nftwidget::~nftwidget()
{
	// TODO Get the window size and position information and save with settings
	delete settings;
	for (uint i = 0; i < iconItems.size(); i++)
	{
		if (iconItems[i] != NULL)
			delete iconItems[i];
	}
}
