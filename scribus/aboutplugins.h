/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ABOUTPLUGINS_H
#define ABOUTPLUGINS_H

#include "ui_aboutplugins.h"
class QListWidgetItem;

/*! \brief Displays info about pligins.
This class implements only the non-GUI parts of the
About Plug-ins dialog. Please use Qt Designer on
aboutpluginsbase.ui if you need to modify the layout,
widget properties, etc.
*/
class AboutPlugins : public QDialog, Ui::AboutPlugins
{
	Q_OBJECT

	public:
		AboutPlugins(QWidget* parent);
		~AboutPlugins();

	private slots:
		//! \brief Update the info on a new selection
		void displayPlugin(QListWidgetItem* item);

	private:
		//! \brief Map list entry IDs to plugin names
		QStringList pluginNames;
		/*! \brief Convert string into its HTML safe representation
		\param s string to process
		\retval QString a string with changed lt/gt
		*/
		QString htmlize(QString s);
};

#endif
