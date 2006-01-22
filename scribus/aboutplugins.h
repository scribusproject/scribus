/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ABOUTPLUGINS_H
#define ABOUTPLUGINS_H

#include <aboutpluginsbase.h>

class AboutPlugins : public AboutPluginsBase
{
	Q_OBJECT

	public:
		AboutPlugins(QWidget* parent);
		~AboutPlugins();

	private slots:
		// Update the info on a new selection
		void displayPlugin(int sel);

	private:
		// Map list entry IDs to plugin names
		QValueList<QCString> pluginNames;
};

#endif
