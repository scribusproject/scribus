#ifndef DOCSECTIONS_H
#define DOCSECTIONS_H

#include <qmap.h>
#include <qstringlist.h>
#include "docsectionsbase.h"
#include "pagestructs.h"

class ScribusDoc;

class DocSections : public DocSectionsBase
{
	Q_OBJECT

	public:
		DocSections(QWidget* parent);
		~DocSections();
		
		virtual void setup(ScribusDoc *);
		virtual void updateTable();
		const DocumentSectionMap& getNewSections();
		
	protected slots:
		virtual void languageChange();
		virtual void tableItemChanged(int, int);
		virtual void addEntry();
		virtual void deleteEntry();
	
	protected:
		ScribusDoc* currDoc;
		DocumentSectionMap localSections;
		uint maxpageindex;
		
		QStringList styles;
};

#endif
