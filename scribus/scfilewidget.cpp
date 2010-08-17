/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QPushButton>
#include <QUrl>

#include "scfilewidget.h"


ScFileWidget::ScFileWidget(QWidget * parent)
	: QFileDialog(parent, Qt::Widget)
{
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
	//desktop too?	QUrl computer(QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)));
	setSidebarUrls(urls);
#endif
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
