#include <cassert>  //added to make Fedora-5 happy

//#include <QDebug>

#include "fpoint.h"
#include "scfonts.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "sctext_shared.h"
#include "util.h"

ScText_Shared::ScText_Shared(const StyleContext* pstyles) : QList<ScText*>(), 
	defaultStyle(), 
	pstyleContext(NULL),
	refs(1), len(0), trailingStyle()
{
	pstyleContext.setDefaultStyle( & defaultStyle );
	defaultStyle.setContext( pstyles );
	trailingStyle.setContext( &pstyleContext );
//		defaultStyle.charStyle().setContext( cstyles );
//		qDebug(QString("ScText_Shared() %1 %2 %3 %4").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(&defaultStyle)).arg(reinterpret_cast<uint>(pstyles)).arg(reinterpret_cast<uint>(cstyles)));
}
		

ScText_Shared::ScText_Shared(const ScText_Shared& other) : QList<ScText*>(), 
	defaultStyle(other.defaultStyle), 
	pstyleContext(other.pstyleContext),
	refs(1), len(0), trailingStyle(other.trailingStyle)
{
	pstyleContext.setDefaultStyle( &defaultStyle );
	trailingStyle.setContext( &pstyleContext );
	QListIterator<ScText*> it( other );
	ScText* elem;
	while ( it.hasNext() ) {
		elem = it.next();
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

void ScText_Shared::clear()
{
	while(!this->isEmpty())
		delete this->takeFirst(); 
	QList<ScText*>::clear();
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
		QListIterator<ScText*> it( other );
		ScText* elem;
		while ( it.hasNext() ) {
			elem = it.next();
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

ScText_Shared::~ScText_Shared() 
{
//		qDebug(QString("~ScText_Shared() %1").arg(reinterpret_cast<uint>(this)));
	while(!this->isEmpty())
		delete this->takeFirst(); 
}

/**
	A char's stylecontext is the containing paragraph's style, 
	This routines makes sure that all charstyles look for defaults
	in the parstyle first.
	*/
void ScText_Shared::replaceCharStyleContextInParagraph(int pos, const StyleContext* newContext)
{
	assert (pos >= 0);
	assert (pos <= size());
	
	if (pos < size())
		value(pos)->setContext(newContext);
	for (int i=pos-1; i >=0 ; --i ) 
	{
		if ( (at(i)->ch) == SpecialChars::PARSEP)
			break;
		value(i)->setContext(newContext);
	}
	// we are done here but will do a sanity check:
	// assert that all chars point to the following parstyle
	QListIterator<ScText*> it( *this );
	const StyleContext* lastContext = NULL;
	while ( it.hasNext() ) {
		ScText* elem = it.next();
		assert( elem );
		if ( elem->ch.isNull() ) 
		{
			// nothing, see code in removeParSep
		}
		else if (elem->ch == SpecialChars::PARSEP)
		{
			assert( elem->parstyle );
			if ( lastContext )
			{
				assert( lastContext == elem->parstyle->charStyleContext() );
			}
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
	}
	if ( lastContext )
		assert( lastContext == trailingStyle.charStyleContext() );
}

