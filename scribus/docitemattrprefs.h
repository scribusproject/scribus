/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS_DOCITEMATTRPREFS
#define SCRIBUS_DOCITEMATTRPREFS

#include "ui_docitemattrprefs.h"
#include "pagestructs.h"

class DocumentItemAttributes : public QWidget, Ui::DocumentItemAttributes
{
	Q_OBJECT
	public:
		DocumentItemAttributes( QWidget* parent = 0, Qt::WFlags fl = 0 );
		~DocumentItemAttributes();
		virtual QStringList getDocAttributesNames();
		virtual ObjAttrVector* getNewAttributes();
		virtual void setup(ObjAttrVector *docItemAttrs);
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

#endif
