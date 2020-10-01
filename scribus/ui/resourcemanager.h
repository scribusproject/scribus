/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  resourcemanager.h  -  description
							 -------------------
	begin                : Sun 20150201
	copyright            : (C) 2015 by Craig Bradney
	email                : cbradney@scribus.info
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QDialog>
#include <QList>
#include <QMap>
#include <QString>

#include "scribusapi.h"
#include "scribusstructs.h"
#include "ui_resourcemanagerbase.h"

class SCRIBUS_API ResourceManager : public QDialog, Ui::ResourceManagerBase
{
	Q_OBJECT

	enum {
		RM_FONTS,
		RM_HYPH,
		RM_PROFILES,
		RM_SPELL,
		RM_TEMPLATES,
		RM_HELP,
		RM_PALETTES,
		RM_TEST=99
	};
	public:
		ResourceManager(QWidget* parent);
		~ResourceManager() {};

	//events:
		//void changeEvent(QEvent *e) override;

	public slots:
		void languageChange();

	protected:
		void readAvailableFonts();
		void readAvailableHelp();
		void readAvailablePalettes();

		void updateInstalledFonts();
		void updateInstalledHyph();
		void updateInstalledSpell();
		void updateInstalledTemplates();
		void updateInstalledHelp();
		void updateInstalledPalettes();
		void updateInstalledTest();

		void updateAvailableFonts();
		void updateAvailableHyph();
		void updateAvailableSpell();
		void updateAvailableTemplates();
		void updateAvailableHelp();
		void updateAvailablePalettes();
		void updateAvailableTest();

		QMap <int, QString> m_resourceCategories;
		QMap <int, QString> m_dataFiles;

		QMap<QString, QString> m_dictionaryMap;
		QStringList m_dictionaryPaths;
		QList <DownloadItem> m_availableList;
		QList <DownloadItem> m_downloadList;
		QList <DownloadItem> m_testList;
		QString affixFileName(QStringList files);
		QString dictFileName(QStringList files);
		QString findDestinationFolder();

	protected slots:
		void categoryChanged();
		void updateDownloadLists();
		void downloadListFinished();
		void downloadFilesFinished();
		void downloadLicenseFinished();
		void downloadLicenseFileFinished(const QString &);
		void downloadLicenseFileFailed(const QString &);
		void installedSelectionChanged();
		void updateProgressBar();
		void updateProgressData(qint64 bytesReceived, qint64 bytesTotal);
		void startDownload();
		void showInFileBrowser();
		void showLicense();
};

#endif // RESOURCEMANAGER_H
