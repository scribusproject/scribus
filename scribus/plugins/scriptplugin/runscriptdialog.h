#ifndef _RUNSCRIPTDIALOG_H
#define _RUNSCRIPTDIALOG_H

// Pulls in Python.h
#include "cmdvar.h"

#include "qobject.h"
#include "q3filedialog.h"
#include "qlabel.h"
#include "qcheckbox.h"

class RunScriptDialog : public Q3FileDialog
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
