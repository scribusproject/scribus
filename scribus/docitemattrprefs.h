#ifndef SCRIBUS_DOCITEMATTRPREFS
#define SCRIBUS_DOCITEMATTRPREFS

#include "docitemattrprefsbase.h"

class DocumentItemAttributes : public DocumentItemAttributesBase
{
	public:
		DocumentItemAttributes(QWidget* parent);
		~DocumentItemAttributes();
		QStringList getDocAttributesNames();
		ObjAttrVector* getNewAttributes();
		void setup(ObjAttrVector *docItemAttrs);
	protected:
		void tableItemChanged( int row, int col );
		void addEntry();
		void updateTable();
		void deleteEntry();
		void clearEntries();
		void copyEntry();
};

#endif
