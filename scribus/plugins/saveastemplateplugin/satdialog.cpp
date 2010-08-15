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

#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>

#include "prefsmanager.h"
#include "prefsfile.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "util_icon.h"

class CategoriesReader : public QXmlDefaultHandler
{
public:
	QStringList categories;
	virtual bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs)
	{
		if (name == QLatin1String("template"))
		{
			QString cat = attrs.value(QLatin1String("category"));
			if (!categories.contains(cat))
				categories.append(cat);
		}
		return true;
	};
};

satdialog::satdialog(QWidget* parent, QString tmplName, int pageW, int pageH) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowTitle( tr("Save as Template"));
	setWindowIcon(loadIcon("AppIcon.png"));
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

QString satdialog::findTemplateXml(QString dir)
{
	QString lang = ScCore->getGuiLanguage();
	QString tmp = dir + "/template." + lang + ".xml";
	if (QFile::exists(tmp))
		return tmp;

	if (lang.length() > 2)
	{
		tmp = dir + "/template." + lang.left(2) + ".xml";
		if (QFile::exists(tmp))
			return tmp;
	}
	return dir + "/template.xml";	
}

void satdialog::addCategories(const QString& dir)
{
	// Read categories from the dir itself
	QString tmplFile = findTemplateXml(dir);
	if (QFile::exists(tmplFile))
		readCategories(tmplFile);

	// And from all the subdirectories. template.xml file is only searched one dir level deeper than the dir
	QDir tmpldir(dir);
	if (tmpldir.exists())
	{
		tmpldir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		QStringList dirs = tmpldir.entryList();
		for (int i = 0; i < dirs.size(); ++i)
		{
			tmplFile = findTemplateXml(dir + "/" + dirs[i]);
			if (QFile::exists(tmplFile))
				readCategories(tmplFile);
		}
	}
}

void satdialog::readCategories(const QString& fileName)
{
	QFile file(fileName);
	CategoriesReader catReader;
	QXmlInputSource  xmlSource(&file);
	QXmlSimpleReader reader;
	reader.setContentHandler(&catReader);
	reader.parse(&xmlSource);
	QStringList& categories = catReader.categories;
	for (int i = 0; i < categories.count(); ++i)
	{
		QString category = categories.at(i);
		if (!category.isEmpty() && !cats.contains(category))
			cats.insert(category, category);
	}
}

void satdialog::setupCategories() 
{
	// en will be used in template.xml and it will be then replaced with the lang when used for users
	// to get the categories in their language.
	cats.insert(QString("Newsletters"), QObject::tr("Newsletters"));
	cats.insert(QString("Brochures")  , QObject::tr("Brochures"));
	cats.insert(QString("Catalogs")   , QObject::tr("Catalogs"));
	cats.insert(QString("Flyers")     , QObject::tr("Flyers"));
	cats.insert(QString("Signs")      , QObject::tr("Signs"));
	cats.insert(QString("Cards")      , QObject::tr("Cards"));
	cats.insert(QString("Letterheads"), QObject::tr("Letterheads"));
	cats.insert(QString("Envelopes")  , QObject::tr("Envelopes"));
	cats.insert(QString("Business Cards"), QObject::tr("Business Cards"));
	cats.insert(QString("Calendars")  , QObject::tr("Calendars"));
	cats.insert(QString("Advertisements"), QObject::tr("Advertisements"));
	cats.insert(QString("Labels")     , QObject::tr("Labels"));
	cats.insert(QString("Menus")      , QObject::tr("Menus"));
	cats.insert(QString("Programs")   , QObject::tr("Programs"));
	cats.insert(QString("PDF Forms")  , QObject::tr("PDF Forms"));
	cats.insert(QString("PDF Presentations") , QObject::tr("PDF Presentations"));
	cats.insert(QString("Magazines")  , QObject::tr("Magazines"));
	cats.insert(QString("Posters")    , QObject::tr("Posters"));
	cats.insert(QString("Announcements") , QObject::tr("Announcements"));
	cats.insert(QString("Text Documents"), QObject::tr("Text Documents"));
	cats.insert(QString("Folds")        , QObject::tr("Folds"));
	cats.insert(QString("Media Cases")  , QObject::tr("Media Cases"));
	cats.insert(QString("Own Templates"), QObject::tr("Own Templates"));

	QString scribusHome  = ScPaths::getApplicationDataDir();
	QString scribusShare = ScPaths::instance().templateDir();
	
	addCategories(scribusHome + "/templates");
	addCategories(scribusShare); 
	QString userTemplateDir(PrefsManager::instance()->appPrefs.documentTemplatesDir);
	if ((!userTemplateDir.isNull()) && (!userTemplateDir.isEmpty()))
		addCategories(userTemplateDir);

	QStringList list;
	QMap<QString, QString>::ConstIterator it;
	for (it = cats.constBegin(); it != cats.constEnd(); ++it)
		list.append(it.value());
	list.sort();
	catsCombo->addItem("");
	catsCombo->addItems(list);
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
}
