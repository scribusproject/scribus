/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef DOWNLOADDICTIONARIES
#define DOWNLOADDICTIONARIES

#include "scribusapi.h"
#include "ui_downloaddictionaries.h"

#include <QDialog>
#include <QString>
#include <QWidget>



class SCRIBUS_API DownloadDictionaries : public QDialog, Ui::DownloadDictionaries
{
	struct DictData
	{
			QString lang;
			QString version;
			QString files;
			QString url;
			QString desc;
			QString license;
			QString filetype;
			bool download;
	};

	Q_OBJECT
	public:
		DownloadDictionaries(QWidget* parent, Qt::WFlags fl=0);
		~DownloadDictionaries();

		void setXMLFile(const QString& f);


	protected:
		void parseData();


		QString m_xmlDataFile;
		QList <DictData> dictList;
		QList <DictData> downloadList;
		QString downloadLocation;

	protected slots:
		void download();
		void downloadDictList();
		void downloadDictListFinished();
		void downloadsFinished();
};

#endif
