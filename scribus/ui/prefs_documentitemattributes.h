/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_DOCUMENTITEMATTRIBUTES_H
#define PREFS_DOCUMENTITEMATTRIBUTES_H

#include "ui_prefs_documentitemattributesbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API Prefs_DocumentItemAttributes : public Prefs_Pane, Ui::Prefs_DocumentItemAttributes
{
	Q_OBJECT

	public:
		Prefs_DocumentItemAttributes(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_DocumentItemAttributes();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		virtual QStringList getDocAttributesNames();

	protected:
		virtual void updateTable();
	protected slots:
		virtual void tableItemChanged( int row, int col );
		virtual void addEntry();
		virtual void deleteEntry();
		virtual void clearEntries();
		virtual void copyEntry();
	private:
		ObjAttrVector localAttributes;
		QStringList relationships;
		QStringList relationshipsData;
		QStringList autoAddTo;
		QStringList autoAddToData;
		QStringList types;
		QStringList typesData;
};

#endif // PREFS_DOCUMENTITEMATTRIBUTES_H
