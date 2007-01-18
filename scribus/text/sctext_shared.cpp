//FIXME: this include must go to sctextstruct.h !
#include <qvaluelist.h>
#include <cassert>  //added to make Fedora-5 happy
#include "fpoint.h"
#include "scfonts.h"
#include "scribusdoc.h"
#include "sctext_shared.h"
// #include "storytext.moc"
#include "scribus.h"
#include "util.h"

ScText_Shared::ScText_Shared(const StyleBase* pstyles) : QPtrList<ScText>(), 
	defaultStyle(), 
	pstyleBase(NULL),
	refs(1), len(0), trailingStyle()
{
	pstyleBase.setDefaultStyle( & defaultStyle );
	setAutoDelete(true);
	defaultStyle.setBase( pstyles );
	trailingStyle.setBase( &pstyleBase );
//		defaultStyle.charStyle().setBase( cstyles );
//		qDebug(QString("ScText_Shared() %1 %2 %3 %4").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(&defaultStyle)).arg(reinterpret_cast<uint>(pstyles)).arg(reinterpret_cast<uint>(cstyles)));
}
		

ScText_Shared::ScText_Shared(const ScText_Shared& other) : QPtrList<ScText>(), 
	defaultStyle(other.defaultStyle), 
	pstyleBase(other.pstyleBase),
	refs(1), len(0), trailingStyle(other.trailingStyle)
{
	pstyleBase.setDefaultStyle( &defaultStyle );
	trailingStyle.setBase( &pstyleBase );
	setAutoDelete(true); 
	QPtrListIterator<ScText> it( other );
	ScText* elem;
	while ( (elem = it.current()) != NULL ) {
		++it;
		ScText* elem2 = new ScText(*elem);
		append(elem2);
		if (elem2->parstyle) {
			elem2->parstyle->setBase( & pstyleBase);
//				elem2->parstyle->charStyle().setBase( defaultStyle.charStyleBase() );
			replaceCharStyleBaseInParagraph(count()-1, elem2->parstyle->charStyleBase());
		}
	}
	len = count();
	replaceCharStyleBaseInParagraph(len,  trailingStyle.charStyleBase() );
//		qDebug(QString("ScText_Shared(%2) %1").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(&other)));
}

ScText_Shared& ScText_Shared::operator= (const ScText_Shared& other) 
{
	if (this != &other) 
	{
		defaultStyle = other.defaultStyle;
		trailingStyle = other.trailingStyle;
		pstyleBase = other.pstyleBase;
		pstyleBase.setDefaultStyle( &defaultStyle );
		defaultStyle.setBase( other.defaultStyle.base() );
		trailingStyle.setBase( &pstyleBase );
		clear();
		QPtrListIterator<ScText> it( other );
		ScText* elem;
		while ( (elem = it.current()) != NULL ) {
			++it;
			ScText* elem2 = new ScText(*elem);
			append(elem2);
			if (elem2->parstyle) {
				elem2->parstyle->setBase( & pstyleBase );
//					qDebug(QString("StoryText::copy: * %1 align=%2").arg(elem2->parstyle->parent())
//						   .arg(elem2->parstyle->alignment())
//						   .arg((uint)elem2->parstyle->base()));
					//				elem2->parstyle->charStyle().setBase( defaultStyle.charStyleBase() );
				replaceCharStyleBaseInParagraph(count()-1, elem2->parstyle->charStyleBase());
			}
		}
		len = count();
		pstyleBase.invalidate();
//			qDebug(QString("StoryText::copy: %1 align=%2 %3").arg(trailingStyle.parentStyle()->name())
//				   .arg(trailingStyle.alignment()).arg((uint)trailingStyle.base()));
		replaceCharStyleBaseInParagraph(len,  trailingStyle.charStyleBase());
	}
//			qDebug(QString("ScText_Shared: %1 = %2").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(&other)));
	return *this;
}

ScText_Shared::~ScText_Shared() {
//		qDebug(QString("~ScText_Shared() %1").arg(reinterpret_cast<uint>(this)));
}

/**
	A char's stylebase is the containing paragraph's style, 
	This routines makes sure that all charstyles look for defaults
	in the parstyle first.
	*/
void ScText_Shared::replaceCharStyleBaseInParagraph(int pos, const StyleBase* newBase)
{
	QPtrListIterator<ScText> it( *this );
	it += (pos-1);
	ScText* elem;
	while ( (elem = it.current()) != NULL ) {
		if (elem->ch[0] == SpecialChars::PARSEP)
			break;
		elem->setBase(newBase);
		--it;
	}
	// assert that all chars point to the following parstyle
	QPtrListIterator<ScText> test( *this );
	const StyleBase* lastBase = NULL;
	while ( (elem = it.current()) != NULL ) {
		if ( elem->ch.isEmpty() ) 
		{
			// nothing, see code in removeParSep
		}
		else if (elem->ch[0] == SpecialChars::PARSEP)
		{
			assert( elem->parstyle );
			if ( lastBase )
				assert( lastBase == elem->parstyle->charStyleBase() );
			lastBase = NULL;
		}
		else if (lastBase == NULL)
		{
			lastBase = elem->base();
		}
		else 
		{
			assert( lastBase == elem->base() );
		}
		++it;
	}
	if ( lastBase )
		assert( lastBase == trailingStyle.charStyleBase() );
}

