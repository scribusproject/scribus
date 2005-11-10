#ifndef ABOUTPLUGINS_H
#define ABOUTPLUGINS_H

#include <aboutpluginsbase.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <Q3CString>

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
		Q3ValueList<Q3CString> pluginNames;
};

#endif
