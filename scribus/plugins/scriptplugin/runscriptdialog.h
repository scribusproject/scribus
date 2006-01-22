/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _RUNSCRIPTDIALOG_H
#define _RUNSCRIPTDIALOG_H

// Pulls in Python.h
#include "cmdvar.h"

#include "qobject.h"
#include "qfiledialog.h"
#include "qlabel.h"
#include "qcheckbox.h"

class RunScriptDialog : public QFileDialog
{
	Q_OBJECT

	public:
		RunScriptDialog(QWidget* parent, bool extEnable);
		~RunScriptDialog();

		/// Check if the user wanted the script run as an extension script
		bool extensionRequested();

	protected:
		QCheckBox *extChk;
		bool extEnable;
};

#endif
