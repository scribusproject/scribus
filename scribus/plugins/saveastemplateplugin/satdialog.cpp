/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include "satdialog.h"
#include "scribusapi.h"
#include "prefsmanager.h"
#include "prefsfile.h"


extern QPixmap SCRIBUS_API loadIcon(QString nam);

satdialog::satdialog(QWidget* parent, QString tmplName, int pageW, int pageH) : QDialog(parent, "satdialog", true)
{
	setupUi(this);
	setCaption( tr("Save as Template"));
	setIcon(loadIcon("AppIcon.png"));
	readPrefs();
	
	nameEdit->setText(tmplName);
	setupCategories();
	setupPageSize(pageW, pageH);
	authorEdit->setText(author);
	emailEdit->setText(email);

	// Slots and signals connections
	connect(detailButton, SIGNAL(stateChanged(int)), this, SLOT(detailClicked(int)));
}

void satdialog::detailClicked(int state)
{
	isFullDetail = state == Qt::Checked;

	psizeLabel->setVisible(isFullDetail);
	catsLabel->setVisible(isFullDetail);
	colorsLabel->setVisible(isFullDetail);
	descrLabel->setVisible(isFullDetail);
	usageLabel->setVisible(isFullDetail);
	authorLabel->setVisible(isFullDetail);
	emailLabel->setVisible(isFullDetail);
	psizeEdit->setVisible(isFullDetail);
	catsCombo->setVisible(isFullDetail);
	colorsEdit->setVisible(isFullDetail);
	descrEdit->setVisible(isFullDetail);
	usageEdit->setVisible(isFullDetail);
	authorEdit->setVisible(isFullDetail);
	emailEdit->setVisible(isFullDetail);
}

void satdialog::readPrefs()
{
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("satemplate");
	author = prefs->get("author", "");
	email = prefs->get("email", "");
	isFullDetail = prefs->getBool("isFullDetail", false);
	detailClicked(isFullDetail ? Qt::Checked : Qt::Unchecked);
	detailButton->setCheckState(isFullDetail ? Qt::Checked : Qt::Unchecked);
}

void satdialog::writePrefs()
{
	prefs->set("author", authorEdit->text());
	prefs->set("email", emailEdit->text());
	prefs->set("isFullDetail", isFullDetail);
}

void satdialog::setupCategories() 
{
	// en will be used in template.xml and it will be then replaced with the lang when used for users
	// to get the categories in their language.
	QString* en = new QString("Newsletters"); QString* lang = new QString(QObject::tr("Newsletters"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Brochures"); lang = new QString(QObject::tr("Brochures"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Catalogs"); lang = new QString(QObject::tr("Catalogs"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Flyers"); lang = new QString(QObject::tr("Flyers"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Signs"); lang = new QString(QObject::tr("Signs"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Cards"); lang = new QString(QObject::tr("Cards"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Letterheads"); lang = new QString(QObject::tr("Letterheads"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Envelopes"); lang = new QString(QObject::tr("Envelopes"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Business Cards"); lang = new QString(QObject::tr("Business Cards"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Calendars"); lang = new QString(QObject::tr("Calendars"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Advertisements"); lang = new QString(QObject::tr("Advertisements"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Labels"); lang = new QString(QObject::tr("Labels"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Menus"); lang = new QString(QObject::tr("Menus"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Programs"); lang = new QString(QObject::tr("Programs"));
	cats.push_back(new Pair(en,lang));
	en = new QString("PDF Forms"); lang = new QString(QObject::tr("PDF Forms"));
	cats.push_back(new Pair(en,lang));
	en = new QString("PDF Presentations"); lang = new QString(QObject::tr("PDF Presentations"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Magazines"); lang = new QString(QObject::tr("Magazines"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Posters"); lang = new QString(QObject::tr("Posters"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Announcements"); lang = new QString(QObject::tr("Announcements"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Text Documents"); lang = new QString(QObject::tr("Text Documents"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Folds"); lang = new QString(QObject::tr("Folds"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Media Cases"); lang = new QString(QObject::tr("Media Cases"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Own Templates"); lang = new QString(QObject::tr("Own Templates"));
	cats.push_back(new Pair(en,lang));
	
	QStringList* list = new QStringList();
	for (uint i = 0; i < cats.size(); ++i)
		list->append(*cats[i]->second);
	list->sort();
	QStringList list2 = *list;
	catsCombo->insertStringList(list2);
	catsCombo->insertItem("",0);
	catsCombo->setEditable(true);
}

void satdialog::setupPageSize(int w, int h)
{
	QString sizelist[] = 
		{"2380x3368", "1684x2380", "1190x1684", "842x1190", "595x842", "421x595", "297x421", "210x297", "148x210",
	     "105x148", "2836x4008", "2004x2836", "1418x2004", "1002x1418", "709x1002", "501x709", "355x501", "250x355",
		 "178x250", "125x178", "89x125", "462x649", "298x683", "312x624", "542x720", "595x935", "1224x792", "612x1008",
		 "612x792", "792x1225"};
	QString namelist[] = 
		{"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8",
		 "A9", "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7",
		 "B8", "B9", "B10", "C5E", "Comm10E", "DLE", "Executive", "Folio", "Ledger", tr("Legal"),
		 tr("Letter"), tr("Tabloid")};

	QString orient, search, psize;
	QString wString = QString("%1").arg(w);
	QString hString = QString("%1").arg(h);
	if (w > h)
	{
		orient = ", "+ tr("landscape");
		search = hString+"x"+wString;
	}
	else if (w < h)
	{
		orient = ", "+ tr("portrait");
		search = wString+"x"+hString;
	}
	int index = -1;
	for (int i = 0; i < 30; ++i)
	{
		if (sizelist[i] == search)
		{
			index = i;
			psize = namelist[i];
			break;
		}
	}
	
	if (index == -1)
	{
		psize = tr("custom")+", "+wString+"x"+hString;
		orient = "";
	}

	psizeEdit->setText(psize + orient);
}

satdialog::~satdialog()
{
	writePrefs();
	for(uint i = 0; i < cats.size(); ++i)
		delete cats[i];

}
