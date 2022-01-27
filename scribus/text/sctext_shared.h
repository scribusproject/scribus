#ifndef SCTEXT_SHARED_H
#define SCTEXT_SHARED_H

#include <QList>
#include <QObject>
#include <QString>
#include <cassert>

//#include "text/paragraphlayout.h"
#include "text/frect.h"
#include "style.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "styles/stylecontextproxy.h"


class SCRIBUS_API ScText_Shared : public QList<ScText*>
{
public:
	ScText_Shared(const StyleContext* pstyles);	
	ScText_Shared(const ScText_Shared& other);
	~ScText_Shared();

	ScText_Shared& operator= (const ScText_Shared& other);

	ParagraphStyle defaultStyle;
	StyleContextProxy pstyleContext;
	uint refs { 1 };
	uint len { 0 };
	uint cursorPosition { 0 };
	int  selFirst { 0 };
	int  selLast { -1 };
	uint marksCount { 0 };
	bool marksCountChanged { false };
	ParagraphStyle trailingStyle;
	CharStyle orphanedCharStyle;

	void clear();
	
	/**
	   A char's stylecontext is the containing paragraph's style, 
       This routines makes sure that all charstyles look for defaults
	   in the parstyle first.
	 */
	void replaceCharStyleContextInParagraph(int pos, const StyleContext* newContext);
};

#endif /*SCTEXT_SHARED_H*/
