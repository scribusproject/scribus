#include "scplugin.h"
#include "qwidget.h"
#include "deferredtask.h"
#include "scribus.h"
#include "scribusapp.h"

//=====================================================//
//                        ScPlugin                     //
//=====================================================//

QWidget* ScPlugin::newPrefsPanelWidget()
{
	return 0;
}

const QString & ScPlugin::lastError() const
{
	return m_lastError;
}

//=====================================================//
//                   ScImportExportPlugin              //
//=====================================================//


// Stub for plugins that don't implement this method to inherit
// Just calls the QIODevice version, assuming target is a file.
bool ScImportExportPlugin::run(QWidget* parent, ScribusApp* mainWindow, QString target)
{
	int flag = id == PluginType_Import ? IO_ReadOnly : IO_WriteOnly;
	QFile f(target);
	if (!f.exists())
	{
		m_lastError = tr("Could not find target file %1: %2", "plugins")
			.arg(target)
			.arg(qApp->translate("QFile",f.errorString()));
		return false;
	}
	if (!f.open(flag))
	{
		m_lastError = tr("Could not open target file %1: %2", "plugins")
			.arg(target)
			.arg(qApp->translate("QFile",f.errorString()));
		return false;
	}
	// Call the QIODevice* based implementation
	bool result = run(parent, mainWindow, &f);
	if (f.isOpen())
		f.close();
	return result;
}

// Stub for plugins that don't implement this method to inherit
bool ScImportExportPlugin::run(
		QWidget* /* parent */,
		ScribusApp* /* mainWindow */,
		QIODevice* /* target */)
{
	return false;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScImportExportPlugin::runAsync(
		QWidget* /* parent */,
		ScribusApp* /* mainWindow */,
		QString /* target */)
{
	return 0;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScImportExportPlugin::runAsync(
		QWidget* /* parent */,
		ScribusApp* /* mainWindow */,
		QIODevice* /* target */)
{
	return 0;
}
