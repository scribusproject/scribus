//FIXME: this include must go to sctextstruct.h !
#include <q3valuelist.h>
//Added by qt3to4:
#include <Q3PtrList>
#include <cassert>  //added to make Fedora-5 happy
#include "fpoint.h"
#include "scfonts.h"
#include "scribusdoc.h"
#include "sctext_shared.h"
// #include "storytext.moc"
#include "scribus.h"
#include "util.h"

ScText_Shared::ScText_Shared(const StyleContext* pstyles) : Q3PtrList<ScText>(), 
	defaultStyle(), 
	pstyleContext(NULL),
	refs(1), len(0), trailingStyle()
{
	pstyleContext.setDefaultStyle( & defaultStyle );
	setAutoDelete(true);
	defaultStyle.setContext( pstyles );
	trailingStyle.setContext( &pstyleContext );
//		defaultStyle.charStyle().setContext( cstyles );
//		qDebug(QString("ScText_Shared() %1 %2 %3 %4").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(&defaultStyle)).arg(reinterpret_cast<uint>(pstyles)).arg(reinterpret_cast<uint>(cstyles)));
}
		

ScText_Shared::ScText_Shared(const ScText_Shared& other) : Q3PtrList<ScText>(), 
	defaultStyle(other.defaultStyle), 
	pstyleContext(other.pstyleContext),
	refs(1), len(0), trailingStyle(other.trailingStyle)
{
	pstyleContext.setDefaultStyle( &defaultStyle );
	trailingStyle.setContext( &pstyleContext );
	setAutoDelete(true); 
	Q3PtrListIterator<ScText> it( other );
	ScText* elem;
	while ( (elem = it.current()) != NULL ) {
		++it;
		ScText* elem2 = new ScText(*elem);
		append(elem2);
		if (elem2->parstyle) {
			elem2->parstyle->setContext( & pstyleContext);
//				elem2->parstyle->charStyle().setContext( defaultStyle.charStyleContext() );
			replaceCharStyleContextInParagraph(count()-1, elem2->parstyle->charStyleContext());
		}
	}
	len = count();
	replaceCharStyleContextInParagraph(len,  trailingStyle.charStyleContext() );
//		qDebug(QString("ScText_Shared(%2) %1").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(&other)));
}

ScText_Shared& ScText_Shared::operator= (const ScText_Shared& other) 
{
	if (this != &other) 
	{
		defaultStyle = other.defaultStyle;
		trailingStyle = other.trailingStyle;
		pstyleContext = other.pstyleContext;
		pstyleContext.setDefaultStyle( &defaultStyle );
		defaultStyle.setContext( other.defaultStyle.context() );
		trailingStyle.setContext( &pstyleContext );
		clear();
		Q3PtrListIterator<ScText> it( other );
		ScText* elem;
		while ( (elem = it.current()) != NULL ) {
			++it;
			ScText* elem2 = new ScText(*elem);
			append(elem2);
			if (elem2->parstyle) {
				elem2->parstyle->setContext( & pstyleContext );
//					qDebug(QString("StoryText::copy: * %1 align=%2").arg(elem2->parstyle->parent())
//						   .arg(elem2->parstyle->alignment())
//						   .arg((uint)elem2->parstyle->context()));
					//				elem2->parstyle->charStyle().setContext( defaultStyle.charStyleContext() );
				replaceCharStyleContextInParagraph(count()-1, elem2->parstyle->charStyleContext());
			}
		}
		len = count();
		pstyleContext.invalidate();
//			qDebug(QString("StoryText::copy: %1 align=%2 %3").arg(trailingStyle.parentStyle()->name())
//				   .arg(trailingStyle.alignment()).arg((uint)trailingStyle.context()));
		replaceCharStyleContextInParagraph(len,  trailingStyle.charStyleContext());
	}
//			qDebug(QString("ScText_Shared: %1 = %2").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(&other)));
	return *this;
}

ScText_Shared::~ScText_Shared() {
//		qDebug(QString("~ScText_Shared() %1").arg(reinterpret_cast<uint>(this)));
}

/**
	A char's stylecontext is the containing paragraph's style, 
	This routines makes sure that all charstyles look for defaults
	in the parstyle first.
	*/
void ScText_Shared::replaceCharStyleContextInParagraph(int pos, const StyleContext* newContext)
{
	Q3PtrListIterator<ScText> it( *this );
	it += (pos-1);
	ScText* elem;
	while ( (elem = it.current()) != NULL ) {
		if (elem->ch[0] == SpecialChars::PARSEP)
			break;
		elem->setContext(newContext);
		--it;
	}
	// assert that all chars point to the following parstyle
	Q3PtrListIterator<ScText> test( *this );
	const StyleContext* lastContext = NULL;
	while ( (elem = it.current()) != NULL ) {
		if ( elem->ch.isEmpty() ) 
		{
			// nothing, see code in removeParSep
		}
		else if (elem->ch[0] == SpecialChars::PARSEP)
		{
			assert( elem->parstyle );
			if ( lastContext )
				assert( lastContext == elem->parstyle->charStyleContext() );
			lastContext = NULL;
		}
		else if (lastContext == NULL)
		{
			lastContext = elem->context();
		}
		else 
		{
			assert( lastContext == elem->context() );
		}
		++it;
	}
	if ( lastContext )
		assert( lastContext == trailingStyle.charStyleContext() );
}

