#ifndef SCRIBUS_PAGEITEMATTRIBUTES_H
#define SCRIBUS_PAGEITEMATTRIBUTES_H

#include "pageitemattributesbase.h"

class PageItemAttributes : public PageItemAttributesBase
{
    Q_OBJECT
	public:
		PageItemAttributes( QWidget* parent = 0, const char* name = 0, bool modal = false, WFlags fl = 0 );
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
};

#endif
