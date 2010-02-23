/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef COLLECTFOROUTPUT_UI_H
#define COLLECTFOROUTPUT_UI_H

#include "collect4output.h"
#include "multiprogressdialog.h"

class CollectForOutput_UI : public CollectForOutput
{
	Q_OBJECT
	public:
		CollectForOutput_UI(QWidget *parent, ScribusDoc* doc, QString outputDirectory=QString::null, bool withFonts=false, bool withProfiles=false, bool compressDoc=false);
		virtual QString collect(QString &newFileName);
	signals:

	protected slots:
		void collectedFonts(int);
		void collectedItems(int);
		void collectedPatterns(int);
		void collectedProfiles(int);

	protected:
		MultiProgressDialog* progressDialog;
};

#endif // COLLECTFOROUTPUT_UI_H
