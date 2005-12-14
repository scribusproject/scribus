#ifndef SCRIBUS_DOCITEMATTRPREFS
#define SCRIBUS_DOCITEMATTRPREFS

#include "docitemattrprefsbase.h"

class DocumentItemAttributes : public DocumentItemAttributesBase
{
	Q_OBJECT
	public:
		DocumentItemAttributes( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
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
};

#endif
