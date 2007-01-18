#ifndef SCTEXT_SHARED_H
#define SCTEXT_SHARED_H

#include <qobject.h>
#include <qstring.h>
#include <qvaluevector.h>
#include <qptrvector.h>
#include <qptrlist.h>
#include <cassert>

//#include "text/paragraphlayout.h"
#include "text/frect.h"
#include "style.h"

#ifdef NLS_CONFORMANCE
#define NLS_PRIVATE private
#else
#define NLS_PRIVATE public
#endif

class CharStyle;
class ParagraphStyle;
class PageItem;


class SCRIBUS_API ScText_Shared : public QPtrList<ScText>
{
public:
	ParagraphStyle defaultStyle;
	StyleBaseProxy pstyleBase;
	uint refs;
	uint len;
	ParagraphStyle trailingStyle;
	ScText_Shared(const StyleBase* pstyles);	

	ScText_Shared(const ScText_Shared& other);

	ScText_Shared& operator= (const ScText_Shared& other);

	~ScText_Shared();
	
	/**
	   A char's stylebase is the containing paragraph's style, 
       This routines makes sure that all charstyles look for defaults
	   in the parstyle first.
	 */
	void replaceCharStyleBaseInParagraph(int pos, const StyleBase* newBase);
};

#endif /*SCTEXT_SHARED_H*/
