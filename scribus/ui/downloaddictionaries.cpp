/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "downloaddictionaries.h"

#include "fileunzip.h"
#include "scpaths.h"
#include "scribusapp.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QTableWidgetItem>
#include <QTextCodec>
#include <QTextStream>


extern ScribusQApp* ScQApp;

DownloadDictionaries::DownloadDictionaries(QWidget* parent, Qt::WFlags fl)
	: QDialog(parent, fl)
{
	setupUi(this);
	downloadLocation=ScPaths::downloadDir();

	connect(closeButton, SIGNAL(clicked()), SLOT(close()));
	connect(downloadButton, SIGNAL(clicked()), SLOT(download()));
	connect(downloadListButton, SIGNAL(clicked()), SLOT(downloadDictList()));
}

DownloadDictionaries::~DownloadDictionaries()
{
}

void DownloadDictionaries::setXMLFile(const QString &f)
{
	m_xmlDataFile=f;
	parseData();
}

void DownloadDictionaries::parseData()
{
	QFile dataFile(m_xmlDataFile);
	dataFile.open(QIODevice::ReadOnly);
	QTextStream ts(&dataFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( "scribus_spell_dicts" );
	QString data(ts.readAll());
	dataFile.close();
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol ))
	{
		if (data.toLower().contains("404 not found"))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<m_xmlDataFile;
		return;
	}
//	qDebug()<<"opened"<<m_xmlDataFile<<"for reading ok";
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while( !n.isNull() ) {
		QDomElement e = n.toElement();
		if( !e.isNull() ) {
			if (e.tagName()=="dictionary")
			{
				if (e.hasAttribute("type"))
				{
					if (e.attribute("type")=="spell")
					{
						struct DictData d;
						d.desc=e.attribute("description");
						d.download=false;
						d.files=e.attribute("files");
						d.url=e.attribute("URL");
						d.version=e.attribute("version");
						d.lang=e.attribute("language");
						d.license=e.attribute("license");
						dictList.append(d);
					}
				}
			}
		}
		n = n.nextSibling();
	}
	dictTableWidget->clear();
	dictTableWidget->setRowCount(dictList.count());
	dictTableWidget->setColumnCount(3);
	int row=0;
	foreach(DictData d, dictList)
	{
		int column=0;
		qDebug()<<d.version<<d.files<<d.url<<d.desc<<d.license;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(d.desc);
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable);
		dictTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(d.lang);
		newItem2->setFlags(newItem1->flags());
		dictTableWidget->setItem(row, column++, newItem2);
		QTableWidgetItem *newItem3 = new QTableWidgetItem();
		newItem3->setCheckState(d.download ? Qt::Checked : Qt::Unchecked);
		dictTableWidget->setItem(row, column++, newItem3);
		++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Location");
	dictTableWidget->setHorizontalHeaderLabels(headers);
	dictTableWidget->resizeColumnsToContents();
}

void DownloadDictionaries::download()
{
	qDebug()<<"Now attempting downloads";
	int rows=dictTableWidget->rowCount();
	QStringList dlLangs;
	for (int i=0; i<rows; ++i)
	{
		QTableWidgetItem *dlItem=dictTableWidget->item(i,2);
		if (dlItem->checkState()==Qt::Checked)
			dlLangs<<dictTableWidget->item(i,1)->text();
	}
	qDebug()<<dlLangs;
	int i=0;
	downloadList.clear();
	foreach(DictData d, dictList)
	{
		if (dlLangs.contains(d.lang))
		{
			ScQApp->dlManager()->addURL(d.url, true, downloadLocation);
			downloadList.append(d);
			++i;
		}
	}
	if (i>0)
	{
		connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadsFinished()));
		ScQApp->dlManager()->startDownloads();
	}
}

void DownloadDictionaries::downloadDictList()
{
	ScQApp->dlManager()->addURL("http://services.scribus.net/scribus_spell_dicts.xml", true, downloadLocation);
	connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	ScQApp->dlManager()->startDownloads();
}

void DownloadDictionaries::downloadDictListFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	setXMLFile(downloadLocation + "scribus_spell_dicts.xml");
}

void DownloadDictionaries::downloadsFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadsFinished()));
	qDebug()<<"Downloads All Finished";
	QString userDictDir(ScPaths::getUserDictDir(true));
	foreach(DictData d, downloadList)
	{
		QString basename = QFileInfo(d.url).fileName();
		QString filename=downloadLocation+"/"+basename;
		qDebug()<<filename;
		FileUnzip* fun = new FileUnzip(filename);
		QStringList files=d.files.split(";", QString::SkipEmptyParts);
		foreach (QString s, files)
		{
			qDebug()<<"Unzipping"<<userDictDir+s;
			QString data=fun->getFile(s, userDictDir);
		}
		delete fun;
	}
}

