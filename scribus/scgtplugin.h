/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCGTPLUGIN_H
#define SCGTPLUGIN_H

#include "scribusapi.h"
#include "scplugin.h"
#include "ui/customfdialog.h"

#include <QCheckBox>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QWidget>

/**
  @brief Super class for all text importer plugins.
  @sa ScPlugin
  @date 2006-06-05
  @author Riku Leino <riku@scribus.info>
  @note KK2006
 */
class SCRIBUS_API ScGTPlugin : public ScPlugin
{
	Q_OBJECT
public:
	ScGTPlugin();
	~ScGTPlugin() = default;

	/**
	  @brief Returns the file format's name that this plugin can import or QString()
	  @brief if this plugin doesn't handle files.
	  @return file format's name or QString() if this plugin doesn't handle files
	 */
	virtual QString fileFormatName() const = 0;

	/**
	  @brief Returns file extensions this plugin can handle or an empty list if
	  @brief this plugin is a text/style formatter plugin.
	  @return file extensions this plugin can handle
	 */
	virtual QStringList fileExtensions() const = 0;

	/**
	  @brief Run the plugin and import from the file <code>filename</code>

	  This function is meant to be overridden by all file format plugins.
	  Once the file decoding and text and style has been sorted out use the
	  function forward() to pass the text and it's style forward to a text frame
	  or to another plugin.
	  @sa forward()
	  @param filename name of the file that is wanted to be imported
	  @param encoding encoding as selected by a user in the import file dialog
	 */
	virtual void run(const QString&filename, const QString&encoding = QString()) {};

	/**
	  @brief Run the plugin and do the magic with the <code>text</code> and
	  @brief <code>style</code>.
	
	  This function is meant to be overridden by all text and style handler plugins.
	  @param text text to work with
	 */
	virtual void run(const QString&text /*, insert style stuff here */) {};

};

/***************************************************************************************/
/***************************************************************************************/

/**
  @brief Manages the import process.

  All ScGTPlugins must be registered to the ScGTPluginManager with the function
  registerGTPlugin(). Only registered plugins will be used.
  @date 2006-06-05
  @author Riku Leino <riku@scribus.info>
  @note KK2006
 */
class SCRIBUS_API ScGTPluginManager {
public:
	static ScGTPluginManager* instance(); // singleton
	static void deleteInstance();

	void registerGTPlugin(ScGTPlugin *plugin);
	void unRegisterGTPlugin(ScGTPlugin *plugin);

	/** @brief Run the Get Text importer. Attached to the Get Text action */
	void run();

private:
	static ScGTPluginManager *m_instance;

	QList<ScGTPlugin*> m_plugins;

	ScGTPluginManager();
	~ScGTPluginManager();

	QString fileFilter();

	/** @brief User has requested options which will be launched from here */
	void options();
};

/***************************************************************************************/
/***************************************************************************************/

class SCRIBUS_API ScGTFileDialog : public CustomFDialog {
	Q_OBJECT
public:
	ScGTFileDialog(const QString& dirName, const QString& filters,
				QWidget* parent = nullptr, const char* name = nullptr);
	~ScGTFileDialog();

	bool showOptions() const;
	bool append() const;

private:
	QWidget   *m_diaExtension {nullptr};
	QCheckBox *m_showOptionsBox {nullptr};
	QCheckBox *m_appendBox {nullptr};
	void customize();
};

/***************************************************************************************/
/***************************************************************************************/

class SCRIBUS_API ScGTOptions {
	
};

/***************************************************************************************/
/***************************************************************************************/

class SCRIBUS_API ScGTOptionsWindow {
	
};

#endif
