/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						latexeditor.h  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef LATEXEDITOR_H
#define LATEXEDITOR_H

#include "ui_latexeditor.h"
#include "scribusapi.h"
#include <QWidget>
#include <QProcess>
#include <QMap>
#include <QPushButton>
#include <QStringList>

class PageItem_LatexFrame;
class LatexHighlighter;
class QXmlStreamReader;
class QComboBox;
class QListWidgetItem;
class QFile;
class XmlWidget;

class SCRIBUS_API LatexEditor : public QDialog, Ui::LatexEditor
{
	Q_OBJECT
	public:
		LatexEditor(PageItem_LatexFrame *);
		~LatexEditor();
		void startEditor();
		void exitEditor();
		void revert();
		void apply();
		void initialize();
	protected:
		PageItem_LatexFrame *frame;
		LatexHighlighter *highlighter;
		QString iconFile, configFile;
		void loadSettings();
		void createNewSettingsTab(QXmlStreamReader *xml);
		void createNewItemsTab(QXmlStreamReader *xml);
		void setConfigFile(QString newConfig);
		QMap<QString, XmlWidget *> widgetMap;
	public slots:
		void okClicked();
		void cancelClicked();
		void updateClicked(bool);
		void revertClicked(bool);
		void updateAppSettings(bool);
		void formulaChanged(QString, QString);
		void latexFinished();
		void stateChanged(QProcess::ProcessState);
		//Slots for the dynamic interface
		void tagButtonClicked(QString);
		void insertButtonClicked(QObject *);
		void newItemSelected(QListWidgetItem *, QListWidgetItem *);
		void itemDoubleClicked(QListWidgetItem *);
};


class SCRIBUS_API DataPushButton : public QPushButton
{
	Q_OBJECT
	public:
		DataPushButton(QString caption, QObject *data, bool deleteOnDestroy=false):
			QPushButton(caption), data(data), del(deleteOnDestroy) {
				connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));}
		~DataPushButton() {
			if (del) delete data;
		}
	protected:
		QObject *data;
		bool del;
	private slots:
		void buttonClicked(bool checked) {
			emit clickedWithData(data);
		}
	signals:
		void clickedWithData(QObject *data);
};

class SCRIBUS_API StringPushButton : public QPushButton
{
	Q_OBJECT
	public:
		StringPushButton(QString caption, QString data):
			QPushButton(caption), data(data) {
			connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));}
	protected:
		QString data;
	private slots:
		void buttonClicked(bool checked) {
			emit clickedWithData(data);
		}
	signals:
		void clickedWithData(QString data);
};

class SCRIBUS_API XmlWidget
{
	public:
		static XmlWidget *fromXml(QXmlStreamReader *xml);
		XmlWidget(QXmlStreamReader *xml, bool readDescription=true);
		QString name() const { return m_name; }
		QString description() const { return m_description; }
		QString defaultValue() const { return m_defaultValue; }
		virtual QString toString() const { return ""; }
		virtual void fromString(QString str){}
		static QString readI18nText(QXmlStreamReader *xml);
	protected:
		QString m_name;
		QString m_description;
		QString m_defaultValue;
};

class SCRIBUS_API IconBuffer
{
	public:
		IconBuffer() { len = 0; basePos = 0; file = 0; }
		static IconBuffer *instance();
		void loadFile(QString filename);
		QIcon *getIcon(QString filename, QString name);
	protected:
		QFile *file;
		QMap<QString, QIcon> icons;
		QStringList loadedFiles;
		QString readHeader();
		QIcon readData();
		int len, basePos;
		static IconBuffer *_instance;
};

#endif
