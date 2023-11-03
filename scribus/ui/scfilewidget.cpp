/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cstdlib>

#include <QDebug>
#include <QStandardPaths>
#include <QLayout>
#include <QLineEdit>
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

	// The margins' content should be set both on the widget and its layout.
	setContentsMargins(0, 0, 0, 0);
	layout()->setContentsMargins(0, 0, 0, 0);

#ifdef Q_OS_MACOS
	QList<QUrl> urls;
	QUrl computer(QUrl::fromLocalFile(QLatin1String("")));
	if (!urls.contains(computer))
		urls << computer;
	QUrl volumes(QUrl::fromLocalFile("/Volumes"));
	if (!urls.contains(volumes))
		urls << volumes;
	QUrl home(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)));
	if (!urls.contains(home))
		urls << home;
	QUrl documents(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)));
	if (!urls.contains(documents))
		urls << documents;
	QUrl dt(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
	if (!urls.contains(dt))
		urls << dt;
	setSidebarUrls(urls);
#endif

	FileDialogEventCatcher* keyCatcher = new FileDialogEventCatcher(this);
	QList<QListView *> childListViews = findChildren<QListView *>();
	for (QListView * lvi : childListViews)
		lvi->installEventFilter(keyCatcher);
	connect(keyCatcher, SIGNAL(escapePressed()), this, SLOT(reject()));
	connect(keyCatcher, SIGNAL(dropLocation(QString)), this, SLOT(locationDropped(QString)));
	connect(keyCatcher, SIGNAL(desktopPressed()), this, SLOT(gotoDesktopDirectory()));
	connect(keyCatcher, SIGNAL(homePressed()), this, SLOT(gotoHomeDirectory()));
	connect(keyCatcher, SIGNAL(parentPressed()), this, SLOT(gotoParentDirectory()));
	connect(keyCatcher, SIGNAL(enterSelectedPressed()), this, SLOT(gotoSelectedDirectory()));

	QList<QPushButton *> childPushButtons = findChildren<QPushButton *>();
	for (QPushButton* pb : childPushButtons)
		pb->setVisible(false);
	setMinimumSize(QSize(480, 310));
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void ScFileWidget::forceDoubleClickActivation(bool force)
{
	// Hack to make the previews in our file dialogs usable again,
	// needed e.g on OpenSuse KDE. Otherwise file would open on first
	// click, leaving user no time to see preview.
	if (m_forceDoubleClickActivation == force)
		return;

	if (force)
		setStyleSheet(QStringLiteral("QAbstractItemView { activate-on-singleclick: 0; }"));
	else
		setStyleSheet(QString());
	m_forceDoubleClickActivation = force;
}

QString ScFileWidget::selectedFile()
{
	QStringList l(selectedFiles());
	if (l.count() == 0)
		return QString();
	return l.at(0);
}

void ScFileWidget::locationDropped(const QString& fileUrl)
{
	QFileInfo fi(fileUrl);
	if (fi.isDir())
	{
		setDirectory(fi.absoluteFilePath());
		return;
	}

	QString absFilePath = fi.absolutePath();
	QString fileName = fi.fileName();
		
	setDirectory(absFilePath);
	selectFile(fileName);
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
	QString dp = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	QFileInfo fi(dp);
	if (fi.exists())
		setDirectory(dp);
}

void ScFileWidget::gotoHomeDirectory()
{
	QString dp = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
	QFileInfo fi(dp);
	if (fi.exists())
		setDirectory(dp);
}

