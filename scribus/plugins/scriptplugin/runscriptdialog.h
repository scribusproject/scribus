/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _RUNSCRIPTDIALOG_H
#define _RUNSCRIPTDIALOG_H

// Pulls in Python.h
// #include "cmdvar.h"
#include "ui_runscriptdialog.h"


/*! \brief Select a python script for execution.
\author Craig Ringer
\author Petr Vanek <petr@scribus.info>
*/
class RunScriptDialog : public QDialog, public Ui::RunScriptDialog
{
	Q_OBJECT

	public:
		RunScriptDialog(QWidget* parent, bool extEnable);
		~RunScriptDialog();

		//! \brief Check if the user wanted the script run as an extension script
		bool extensionRequested();
		//! \brief Return chosen filename
		QString selectedFile();

	protected:
		// true id there is "scripter extensions" feature enabled
		bool m_extEnable;

		static QString m_lastScriptDir;

	protected slots:
		virtual void accept();
};

#endif
