/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QStandardPaths>
#include <QListView>
#include <QPushButton>
#include <QStringList>
#include <QUrl>

#include "filedialogeventcatcher.h"
#include "scfilewidget.h"


ScFileWidget::ScFileWidget(QWidget * parent) : QFileDialog(parent, Qt::Widget)
{
	setOption(QFileDialog::DontUseNativeDialog);
	setSizeGripEnabled(false);
	setModal(false);
	setViewMode(QFileDialog::List);
	setWindowFlags(Qt::Widget);

#ifdef Q_OS_MAC
	QList<QUrl> urls;
	QUrl computer(QUrl::fromLocalFile(QLatin1String("")));
	if (!urls.contains(computer))
		urls << computer;
	QUrl volumes(QUrl::fromLocalFile("/Volumes"));
	if (!urls.contains(volumes))
		urls << volumes;
	QUrl dt(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
	if (!urls.contains(dt))
		urls << dt;
	setSidebarUrls(urls);
#endif

	FileDialogEventCatcher* keyCatcher = new FileDialogEventCatcher(this);
	QList<QListView *> lv = findChildren<QListView *>();
	QListIterator<QListView *> lvi(lv);
	while (lvi.hasNext())
		lvi.next()->installEventFilter(keyCatcher);
	connect(keyCatcher, SIGNAL(escapePressed()), this, SLOT(reject()));
	connect(keyCatcher, SIGNAL(dropLocation(QString)), this, SLOT(locationDropped(QString)));
	connect(keyCatcher, SIGNAL(desktopPressed()), this, SLOT(gotoDesktopDirectory()));
	connect(keyCatcher, SIGNAL(homePressed()), this, SLOT(gotoHomeDirectory()));
	connect(keyCatcher, SIGNAL(parentPressed()), this, SLOT(gotoParentDirectory()));
	connect(keyCatcher, SIGNAL(enterSelectedPressed()), this, SLOT(gotoSelectedDirectory()));

	QList<QPushButton *> b = findChildren<QPushButton *>();
	QListIterator<QPushButton *> i(b);
	while (i.hasNext())
		i.next()->setVisible(false);
	setMinimumSize(QSize(480, 310));
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QString ScFileWidget::selectedFile()
{
	QStringList l(selectedFiles());
	if (l.count() == 0)
		return QString();
	return l.at(0);
}

/* Hack to make the previews in our file dialogs useable again,
   needed e.g on OpenSuse KDE. Otherwise file would open on first
   click, leaving user no time to see preview. Drawback: in other 
   Linux desktop environment, this may force user to click OK for
   opening/saving file. */
void ScFileWidget::accept()
{
#ifndef Q_OS_LINUX
	QFileDialog::accept();
#endif
}

void ScFileWidget::locationDropped(QString fileUrl)
{
	QFileInfo fi(fileUrl);
	if (fi.isDir())
		setDirectory(fi.absoluteFilePath());
	else
	{
		setDirectory(fi.absolutePath());
		selectFile(fi.fileName());
	}
}

void ScFileWidget::gotoParentDirectory()
{
	QDir d(directory());
	d.cdUp();
	setDirectory(d);
}

void ScFileWidget::gotoSelectedDirectory()
{
	QStringList s(selectedFiles());
	if (!s.isEmpty())
	{
		QFileInfo fi(s.first());
//		qDebug()<<s.first()<<fi.absoluteFilePath();
		if (fi.isDir())
			setDirectory(fi.absoluteFilePath());
	}
}

void ScFileWidget::gotoDesktopDirectory()
{
	QString dp=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	QFileInfo fi(dp);
	if (fi.exists())
		setDirectory(dp);
}

void ScFileWidget::gotoHomeDirectory()
{
	QString dp=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
	QFileInfo fi(dp);
	if (fi.exists())
		setDirectory(dp);
}

