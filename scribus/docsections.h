#ifndef DOCSECTIONS_H
#define DOCSECTIONS_H

#include <qmap.h>
#include <qstringlist.h>
#include "docsectionsbase.h"
#include "pagestructs.h"

class DocSections : public DocSectionsBase
{
	Q_OBJECT

	public:
		DocSections(QWidget* parent);
		~DocSections();
		
		virtual void setup(const DocumentSectionMap docSections, int maxPageIndex);
		virtual void updateTable();
		const DocumentSectionMap& getNewSections();
		
	protected slots:
		virtual void languageChange();
		virtual void tableItemChanged(int, int);
		virtual void addEntry();
		virtual void deleteEntry();
	
	protected:
		DocumentSectionMap localSections;
		uint m_maxpageindex;
		QStringList styles;
};

#endif
