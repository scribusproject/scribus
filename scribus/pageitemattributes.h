/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS_PAGEITEMATTRIBUTES_H
#define SCRIBUS_PAGEITEMATTRIBUTES_H

#include "ui_pageitemattributes.h"

class PageItemAttributes : public QDialog, Ui::PageItemAttributes
{
    Q_OBJECT
	public:
		PageItemAttributes( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
		~PageItemAttributes();

		virtual ObjAttrVector * getNewAttributes();

	public slots:
		virtual void setup( ObjAttrVector * pageItemAttrs, ObjAttrVector * docItemAttrs );
		virtual void tableItemChanged( int row, int col );

	protected:
		virtual void updateTable();

	protected slots:
		virtual void languageChange();
		virtual void addEntry();
		virtual void deleteEntry();
		virtual void clearEntries();
		virtual void copyEntry();
		virtual void okClicked();

	private:
		ObjAttrVector localAttributes;
		ObjAttrVector localDocAttributes;
		QStringList relationships;
		QStringList relationshipsData;
		QStringList nameList;
};

#endif
