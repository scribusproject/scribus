//FIXME: this include must go to sctextstruct.h !
#include <qvaluelist.h>
#include <cassert>  //added to make Fedora-5 happy
#include "fpoint.h"
#include "scfonts.h"
//#include "sctextstruct.h"
#include "storytext.h"
#include "scribus.h"
#include "util.h"


QChar SpecialChars::OBJECT     = QChar(25);
QChar SpecialChars::TAB        = QChar(9);
QChar SpecialChars::PARSEP     = QChar(13);
QChar SpecialChars::LINEBREAK  = QChar(28);
QChar SpecialChars::COLBREAK   = QChar(26);
QChar SpecialChars::FRAMEBREAK = QChar(27);
QChar SpecialChars::SHYPHEN    = QChar(0xAD);
QChar SpecialChars::NBHYPHEN   = QChar(0x2011);
QChar SpecialChars::NBSPACE    = QChar(0xA0);
QChar SpecialChars::OLD_NBHYPHEN   = QChar(24);
QChar SpecialChars::OLD_NBSPACE    = QChar(29);
QChar SpecialChars::ZWNBSPACE  = QChar(0x2060);
QChar SpecialChars::ZWSPACE    = QChar(0x200B);
QChar SpecialChars::PAGENUMBER      = QChar(30);
//QChar SpecialChars::SPACE      = QChar(32);



class ScText_Shared : public QPtrList<ScText>
{
public:
	ParagraphStyle defaultStyle;
	StyleBaseProxy pstyleBase;
	uint refs;
	uint len;
	ParagraphStyle trailingStyle;
	ScText_Shared(const StyleBase* pstyles) : QPtrList<ScText>(), 
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
			

	ScText_Shared(const ScText_Shared& other) : QPtrList<ScText>(), 
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

	ScText_Shared& operator= (const ScText_Shared& other) 
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
//		qDebug(QString("ScText_Shared: %1 = %2").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(&other)));
		return *this;
	}

	~ScText_Shared() {
//		qDebug(QString("~ScText_Shared() %1").arg(reinterpret_cast<uint>(this)));
	}
	
	/**
	   A char's stylebase is the containing paragraph's style, 
       This routines makes sure that all charstyles look for defaults
	   in the parstyle first.
	 */
	void replaceCharStyleBaseInParagraph(int pos, const StyleBase* newBase)
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
};


StoryText::StoryText(ScribusDoc * doc_) : doc(doc_)
{
	if (doc_) {
		d = new ScText_Shared(&doc_->paragraphStyles());
	}
	else {
		d = new ScText_Shared(NULL);
	}
	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;
	m_magicX = 0.0;
	m_lastMagicPos = -1;
	
	d->len = 0;
	invalidateAll();
}

StoryText::StoryText() : doc(NULL)
{
	d = new ScText_Shared(NULL);

	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;
	m_magicX = 0.0;
	m_lastMagicPos = -1;
}

StoryText::StoryText(const StoryText & other) : doc(other.doc)
{
	d = other.d;
	d->refs++;
	
	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;
	m_magicX = 0.0;
	m_lastMagicPos = -1;

	invalidateLayout();
}

StoryText::~StoryText()
{
	d->refs--;
	if (d->refs == 0) {
		for (ScText *it = d->first(); it != 0; it = d->next())
		{
			if ((it->ch[0] == SpecialChars::OBJECT) && (it->cembedded != 0) && doc) {
				doc->FrameItems.remove(it->cembedded);
				delete it->cembedded;
				it->cembedded = 0;
			}
			else if ((it->ch[0] == SpecialChars::PARSEP)) {
				delete it->parstyle;
				it->parstyle = 0;
			}
		}
		
		d->clear();
		d->len = 0;
		delete d;
	}	
}

StoryText StoryText::copy() const
{
	StoryText result(doc);
	*(result.d) = *d;
	return result;
//	qDebug(QString("StoryText::copy:"));
	QPtrListIterator<ScText> it( *(result.d) );
	ScText* elem;
	while ( (elem = it.current()) != NULL ) {
		++it;
//		qDebug(QString("\tchar '%1' size %2 (orig %3)").arg(elem->ch).arg(elem->fontSize()).arg(charStyle(i++).fontSize()));
	}
	
	return result;
}


StoryText& StoryText::operator= (const StoryText & other)
{
	other.d->refs++;
	
	d->refs--;
	if (d->refs == 0) {
		clear();
		delete d;
	}
	
	doc = other.doc; 
	d = other.d;
	
	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;

	invalidateLayout();
	return *this;
}


void StoryText::clear()
{
	selFirst = 0;
	selLast = -1;

	firstFrameItem = 0;
	lastFrameItem = -1;
	
	d->defaultStyle.erase();
	d->trailingStyle.erase();
	
	for (ScText *it = d->first(); it != 0; it = d->next())
	{
		if ((it->ch[0] == SpecialChars::OBJECT) && (it->cembedded != 0) && doc) {
			doc->FrameItems.remove(it->cembedded);
			delete it->cembedded;
			it->cembedded = 0;
		}
		else if ((it->ch[0] == SpecialChars::PARSEP) && it->parstyle) {
			delete it->parstyle;
			it->parstyle = 0;
		}
	}
	
	d->clear();
	d->len = 0;
	invalidateAll();
}


void StoryText::insert(int pos, const StoryText& other, bool onlySelection)
{
	CharStyle cstyle(charStyle(pos));
	ParagraphStyle pstyle(paragraphStyle(pos));
	
	// this style represents all differences between this and other's defaultstyles
	ParagraphStyle otherDefault(other.defaultStyle());
	otherDefault.eraseStyle(defaultStyle());
	
	int otherStart  = onlySelection? other.startOfSelection() : 0;
	int otherEnd    = onlySelection? other.endOfSelection() : other.length();
	int cstyleStart = otherStart;
	for (int i=otherStart; i < otherEnd; ++i) {
		if (other.charStyle(i) == cstyle 
			&& other.text(i) != SpecialChars::OBJECT
			&& other.text(i) != SpecialChars::PARSEP)
			continue;
		int len = i - cstyleStart;
		if (len > 0) {
			insertChars(pos, other.text(cstyleStart, len));
			applyCharStyle(pos, len, otherDefault.charStyle());
			applyCharStyle(pos, len, cstyle);
			pos += len;
		}
		if (other.text(i) == SpecialChars::PARSEP) {
			insertChars(pos, SpecialChars::PARSEP);
			applyStyle(pos, otherDefault);
			applyStyle(pos, other.paragraphStyle(i));
			cstyleStart = i+1;
			pos += 1;
		}
		else if (other.text(i) == SpecialChars::OBJECT) {
			insertObject(pos+len, other.object(i));
			cstyleStart = i+1;
			pos += 1;
		}
		else {
			cstyle = other.charStyle(i);
			cstyleStart = i;
		}
	}
	int len = otherEnd - cstyleStart;
	if (len > 0) {
		insertChars(pos, other.text(cstyleStart, len));
		applyCharStyle(pos, len, otherDefault.charStyle());
		applyCharStyle(pos, len, cstyle);
		pos += len;
		if (other.text(otherEnd-1) != SpecialChars::PARSEP) {
			applyStyle(pos, otherDefault);
			applyStyle(pos, other.paragraphStyle(otherEnd-1));
		}
	}
}


/**
    Make sure that the paragraph CharStyle's point to the new ParagraphStyle
 */
static void insertParSep(StoryText* that, ScText_Shared* d, int pos)
{
	ScText* it = that->item_p(pos);
	if(!it->parstyle) {
		it->parstyle = new ParagraphStyle();
		it->parstyle->setBase( & d->pstyleBase);
//		it->parstyle->charStyle().setBase( d->defaultStyle.charStyleBase() );
	}
	d->replaceCharStyleBaseInParagraph(pos, it->parstyle->charStyleBase());
}
/**
     need to remove the ParagraphStyle structure and replace all pointers
     to it...
 */
static void removeParSep(StoryText* that, ScText_Shared* d, int pos)
{
	ScText* it = that->item_p(pos);
	if (it->parstyle) {
//		const CharStyle* oldP = & it->parstyle->charStyle();
//		const CharStyle* newP = & that->paragraphStyle(pos+1).charStyle();
//		d->replaceParentStyle(pos, oldP, newP);
		delete it->parstyle;
		it->parstyle = 0;
	}
	// demote this parsep so the assert code in replaceCharStyleBaseInParagraph()
	// doesnt choke:
	it->ch = "";
	d->replaceCharStyleBaseInParagraph(pos, that->paragraphStyle(pos+1).charStyleBase());	
}

void StoryText::removeChars(int pos, uint len)
{
	if (pos < 0)
		pos += length();

	assert( len > 0 );
	assert( pos >= 0 );
	assert( pos + static_cast<int>(len) <= length() );

	for ( int i=pos + static_cast<int>(len) - 1; i >= pos; --i )
	{
		ScText *it = d->at(i);
		if ((it->ch[0] == SpecialChars::OBJECT) && (it->cembedded != 0) && doc) {
			doc->FrameItems.remove(it->cembedded);
			it->cembedded = 0;
		}
		else if ((it->ch[0] == SpecialChars::PARSEP)) {
			removeParSep(this, this->d, i);
		}
//		qDebug("remove char %d at %d", it->ch[0].unicode(), i);
		d->take(i);
		d->len--;
		delete it;
	}

	d->len = d->count();
	invalidate(pos, -1);
}

void StoryText::insertChars(int pos, QString txt) //, const CharStyle&
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());
	
	if (txt.length() == 0)
		return;
	
	const StyleBase* cStyleBase = paragraphStyle(pos).charStyleBase();
	//	assert( !style.font().isNone() );
	
	for (uint i = 0; i < txt.length(); ++i) {
		ScText * item = new ScText();
		item->ch= txt.mid(i, 1);
		item->setBase(cStyleBase);
		d->insert(pos + i, item);
		d->len++;
		if (item->ch[0] == SpecialChars::PARSEP) {
//			qDebug(QString("new PARSEP %2 at %1").arg(pos).arg(paragraphStyle(pos).name()));
			insertParSep(this, this->d, pos + i);
		}
	}

	d->len = d->count();
	invalidate(pos, pos + txt.length());
}

void StoryText::replaceChar(int pos, QChar ch)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	ScText* item = d->at(pos);
	if (item->ch[0] == ch)
		return;
	
	if (d->at(pos)->ch[0] == SpecialChars::PARSEP) {
		removeParSep(this, this->d, pos);
	}
	item->ch = ch;
	if (d->at(pos)->ch[0] == SpecialChars::PARSEP) {
		insertParSep(this, this->d, pos);
	}
	
	invalidate(pos, pos + 1);
}

void StoryText::hyphenateWord(int pos, uint len, char* hyphens)
{
	assert(pos >= 0);
	assert(pos + signed(len) <= length());
	
//	QString dump("");
	for (int i=pos; i < pos+signed(len); ++i)
	{
//		dump += d->at(i)->ch;
		if(hyphens && hyphens[i-pos] & 1) {
			d->at(i)->setEffects(d->at(i)->effects() | ScStyle_HyphenationPossible);
//			dump += "-";
		}
		else {
			d->at(i)->setEffects(d->at(i)->effects() & ~ScStyle_HyphenationPossible);
		}
	}
//	qDebug(QString("st: %1").arg(dump));
	invalidate(pos, pos + len);
}

void StoryText::insertObject(int pos, PageItem* ob)
{
	if (pos < 0)
		pos += length();

	insertChars(pos, SpecialChars::OBJECT);
	const_cast<StoryText *>(this)->d->at(pos)->cembedded = ob;
}


int StoryText::length() const
{
	return d->len;
}

QChar StoryText::text(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	return const_cast<StoryText *>(this)->d->at(pos)->ch[0];
}

QString StoryText::text(int pos, uint len) const
{
	if (pos < 0)
		pos += length();

	assert(len >= 0);
	assert(pos >= 0);
	assert(pos + signed(len) <= length());

	QString result;
	StoryText* that(const_cast<StoryText*>(this));
	that->d->at(pos);
	for (int i = pos; i < pos+signed(len); ++i) {
		result += that->d->current()->ch;
		that->d->next();
	}

	return result;
}

PageItem* StoryText::object(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);	
	if (that->d->at(pos)->cembedded != NULL)
		return that->d->at(pos)->cembedded;
	else
		return NULL;
}


const CharStyle & StoryText::charStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	if (length() == 0) {
//		qDebug("storytext::charstyle: default");
		return defaultStyle().charStyle();
	}
	else if (pos == length()) {
		qDebug(QString("storytext::charstyle: access at end of text %1").arg(pos));
		--pos;
	}
	
	StoryText* that = const_cast<StoryText *>(this);
	return dynamic_cast<const CharStyle &> (*that->d->at(pos));
}

const ParagraphStyle & StoryText::paragraphStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	assert(d);
	
	StoryText * that = const_cast<StoryText *> (this);
//	assert( that->at(pos)->cab >= 0 );
//	assert( that->at(pos)->cab < doc->docParagraphStyles.count() );
//	return doc->docParagraphStyles[that->at(pos)->cab];
	
	that->d->at(pos);
	while (pos < length() && that->d->current()->ch[0] != SpecialChars::PARSEP) {
		++pos;
		that->d->next();
	}
	if (pos >= length()) {
		return that->d->trailingStyle;
	}
	else if ( !that->d->current()->parstyle ) {
		qDebug(QString("inserting default parstyle at %1").arg(pos));
		that->d->current()->parstyle = new ParagraphStyle();
		that->d->current()->parstyle->setBase( & d->pstyleBase);
//		that->d->current()->parstyle->charStyle().setBase( d->defaultStyle.charStyleBase());
	}
	else {
//		qDebug(QString("using parstyle at %1").arg(pos));
	}
	assert (that->d->current()->parstyle);
	return *that->d->current()->parstyle;
}

const ParagraphStyle& StoryText::defaultStyle() const
{
	assert(d);
	return d->defaultStyle;
}


void StoryText::setDefaultStyle(const ParagraphStyle& style)
{
	const StyleBase * oldPBase = d->defaultStyle.base();
	const StyleBase * oldCBase = d->defaultStyle.charStyle().base();
	d->defaultStyle = style;
	d->defaultStyle.setBase( oldPBase );
	d->defaultStyle.charStyle().setBase( oldCBase );
	d->pstyleBase.invalidate();
}


void StoryText::applyCharStyle(int pos, uint len, const CharStyle& style )
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos + signed(len) <= length());

	if (len == 0)
		return;

	d->at(pos);
	for (uint i=pos; i < pos+len; ++i) {
		d->current()->applyCharStyle(style);
		d->next();
	}

	invalidate(pos, pos + len);
}



void StoryText::eraseCharStyle(int pos, uint len, const CharStyle& style )
{
	if (pos < 0)
		pos += length();
	
	assert(pos >= 0);
	assert(pos + signed(len) <= length());
	
	if (len == 0)
		return;
	
	d->at(pos);
	for (uint i=pos; i < pos+len; ++i) {
		d->current()->eraseCharStyle(style);
		d->next();
	}
	
	invalidate(pos, pos + len);
}


void StoryText::applyStyle(int pos, const ParagraphStyle& style)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

/*	int paraStyle = QMAX(0, findParagraphStyle(doc, style)); //FIXME:NLS
	int i = pos - 1;
	if (length() > 0) {
		// find start of para
		while (i >= 0 && at(i)->ch[0] != SpecialChars::PARSEP) {
			--i;
		}
		// set 'cab' field
		do {
			++i;
			at(i)->cab = paraStyle;
		}
		while (i+1 < length() && d->at(i)->ch[0] != SpecialChars::PARSEP);
	} 
	++i; */
	
	int i = pos;
	while (i < length() && d->at(i)->ch[0] != SpecialChars::PARSEP) {
		++i;
	}
	if (i < length()) {
		if (!d->at(i)->parstyle) {
			qDebug(QString("PARSEP without style at pos %1").arg(i));
			d->at(i)->parstyle = new ParagraphStyle();
			d->at(i)->parstyle->setBase( & d->pstyleBase);
//			d->at(i)->parstyle->charStyle().setBase( d->defaultStyle.charStyleBase() );
		}
//		qDebug(QString("applying parstyle %2 at %1 for %3").arg(i).arg(paragraphStyle(pos).name()).arg(pos));
		d->at(i)->parstyle->applyStyle(style);
	}
	else {
		// not happy about this but inserting a new PARSEP makes more trouble
//		qDebug(QString("applying parstyle %1 as defaultstyle for %2").arg(paragraphStyle(pos).name()).arg(pos));
		d->trailingStyle.applyStyle(style);
	}
	invalidate(pos, QMIN(i, length()));
}

void StoryText::eraseStyle(int pos, const ParagraphStyle& style)
{
	if (pos < 0)
		pos += length();
	
	assert(pos >= 0);
	assert(pos <= length());
		
	int i = pos;
	while (i < length() && d->at(i)->ch[0] != SpecialChars::PARSEP) {
		++i;
	}
	if (i < length()) {
		if (!d->at(i)->parstyle) {
			qDebug(QString("PARSEP without style at pos %1").arg(i));
			d->at(i)->parstyle = new ParagraphStyle();
			d->at(i)->parstyle->setBase( & d->pstyleBase);
//			d->at(i)->parstyle->charStyle().setBase( d->defaultStyle.charStyleBase());
		}
		//		qDebug(QString("applying parstyle %2 at %1 for %3").arg(i).arg(paragraphStyle(pos).name()).arg(pos));
		d->at(i)->parstyle->eraseStyle(style);
	}
	else {
		// not happy about this but inserting a new PARSEP makes more trouble
		//		qDebug(QString("applying parstyle %1 as defaultstyle for %2").arg(paragraphStyle(pos).name()).arg(pos));
		d->trailingStyle.eraseStyle(style);
	}
	invalidate(pos, QMIN(i, length()));
}


void StoryText::setStyle(int pos, const ParagraphStyle& style)
{
	eraseStyle(pos, paragraphStyle(pos));
	applyStyle(pos, style);
}


void StoryText::setCharStyle(int pos, uint len, const CharStyle& style)
{
	// FIXME
	eraseCharStyle(pos, len, charStyle(pos));
	applyCharStyle(pos, len, style);
}


void StoryText::replaceStyles(QMap<QString,QString> newNameForOld)
{
	int len = length();
	if (len == 0)
		return;
	
	d->at(0);
	for (int i=0; i < len; ++i) {
		if (d->current()->parstyle && newNameForOld.contains(d->current()->parstyle->parent()))
			d->current()->parstyle->setParent(newNameForOld[d->current()->parstyle->parent()]);
		d->next();
	}
	if (newNameForOld.contains(d->defaultStyle.parent()))
		d->defaultStyle.setParent(newNameForOld[d->defaultStyle.parent()]);
	if (newNameForOld.contains(d->trailingStyle.parent()))
		d->trailingStyle.setParent(newNameForOld[d->trailingStyle.parent()]);
	
	invalidate(0, len);	
}

void StoryText::replaceCharStyles(QMap<QString,QString> newNameForOld)
{
	int len = length();
	if (len == 0)
		return;
	
	d->at(0);
	for (int i=0; i < len; ++i) {
		if (newNameForOld.contains(d->current()->parent()))
			d->current()->setParent(newNameForOld[d->current()->parent()]);
		
		if (d->current()->parstyle && newNameForOld.contains(d->current()->parstyle->charStyle().parent()))
			d->current()->parstyle->charStyle().setParent(newNameForOld[d->current()->parstyle->charStyle().parent()]);
		d->next();
	}
	if (newNameForOld.contains(d->defaultStyle.charStyle().parent()))
		d->defaultStyle.charStyle().setParent(newNameForOld[d->defaultStyle.charStyle().parent()]);
	if (newNameForOld.contains(d->trailingStyle.charStyle().parent()))
		d->trailingStyle.charStyle().setParent(newNameForOld[d->trailingStyle.charStyle().parent()]);
	
	invalidate(0, len);	
}

uint StoryText::nrOfParagraphs() const
{
	uint result = 0;
	StoryText* that = const_cast<StoryText *>(this);
	that->d->at(0);
	bool lastWasPARSEP = true;
	for (int i=0; i < length(); ++i) {
		lastWasPARSEP = that->d->current()->ch[0] == SpecialChars::PARSEP;
		if (lastWasPARSEP)
			++result;
		that->d->next();
	}
	return lastWasPARSEP? result : result + 1;
}

int StoryText::startOfParagraph(uint index) const
{
	if (index == 0)
		return 0;

	StoryText* that = const_cast<StoryText *>(this);
	that->d->at(0);
	for (int i=0; i < length(); ++i) {
		if (that->d->current()->ch[0] == SpecialChars::PARSEP && ! --index)
			return i + 1;
		that->d->next();
	}
	return length();
}

int StoryText::endOfParagraph(uint index) const
{
	++index;
	StoryText* that = const_cast<StoryText *>(this);
	that->d->at(0);
	for (int i=0; i < length(); ++i) {
		if (that->d->current()->ch[0] == SpecialChars::PARSEP && ! --index)
			return i;
		that->d->next();
	}
	return length();
}

uint StoryText::nrOfRuns() const
{
	return length();
}

int StoryText::startOfRun(uint index) const
{
	return index;
}

int StoryText::endOfRun(uint index) const
{
	return index + 1;
}

// positioning. all positioning methods return char positions
// FIXME: make that methods use correct semantic boundaries

static QString wordBoundaries(" .,:;\"'!?\n");
static QString sentenceBoundaries(".:!?\n");

int StoryText::nextChar(int pos)
{
	if (pos < length())
		return pos+1;
	else
		return length();
}
int StoryText::prevChar(int pos)
{
	if (pos > 0)
		return pos - 1;
	else 
		return 0;
}
int StoryText::nextWord(int pos)
{
	int len = length();
	pos = QMIN(len, pos+1);
	while (pos < len  && wordBoundaries.find(text(pos)) < 0)
		++pos;
	return pos < len ? pos + 1 : pos;
}
int StoryText::prevWord(int pos)
{
	pos = QMAX(0, pos-1);
	while (pos > 0 && wordBoundaries.find(text(pos)) < 0)
		--pos;
	return wordBoundaries.find(text(pos)) < 0 ? pos + 1 : pos;
}
int StoryText::nextSentence(int pos)
{
	int len = length();
	pos = QMIN(len, pos+1);
	while (pos < len && sentenceBoundaries.find(text(pos)) < 0)
		++pos;
	return pos < len ? pos + 1 : pos;
}
int StoryText::prevSentence(int pos)
{
	pos = QMAX(0, pos-1);
	while (pos > 0 && sentenceBoundaries.find(text(pos)) < 0)
		--pos;
	return sentenceBoundaries.find(text(pos)) < 0 ? pos + 1 : pos;
}
int StoryText::nextParagraph(int pos)
{
	int len = length();
	pos = QMIN(len, pos+1);
	while (pos < len && text(pos) != SpecialChars::PARSEP)
		++pos;
	return pos;
}
int StoryText::prevParagraph(int pos)
{
	pos = QMAX(0, pos-1);
	while (pos > 0 && text(pos) != SpecialChars::PARSEP)
		--pos;
	return pos;
}

// these need valid layout:

int StoryText::startOfLine(int pos)
{
	for (uint i=0; i < m_lines.count(); ++i) {
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem)
			return ls.firstItem;
	}
	return 0;
}
int StoryText::endOfLine(int pos)
{
	for (uint i=0; i < m_lines.count(); ++i) {
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem)
			return text(ls.lastItem) == SpecialChars::PARSEP ? ls.lastItem : 
				text(ls.lastItem) == ' ' ? ls.lastItem : ls.lastItem + 1;
	}
	return length();
}
int StoryText::prevLine(int pos)
{
	for (uint i=0; i < m_lines.count(); ++i) 
	{
		// find line for pos
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem) 
		{
			if (i == 0)
				return startOfLine(pos);
			// find current xpos
			double xpos = 0.0;
			for (int j = ls.firstItem; j < pos; ++j)
				xpos += item(j)->glyph.wide();
			if (pos != m_lastMagicPos || xpos > m_magicX)
				m_magicX = xpos;
			const LineSpec & ls2(m_lines.at(i-1));
			// find new cpos
			xpos = 0.0;
			for (int j = ls2.firstItem; j <= ls2.lastItem; ++j) 
			{
				xpos += item(j)->glyph.wide();
				if (xpos > m_magicX) {
					m_lastMagicPos = j;
					return j;
				}
			}
			m_lastMagicPos = ls2.lastItem;
			return ls2.lastItem;
		}
	}
	return firstFrameItem;
}

int StoryText::nextLine(int pos)
{
	for (uint i=0; i < m_lines.count(); ++i) 
	{
		// find line for pos
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem) 
		{
			if (i+1 == m_lines.count())
				return endOfLine(pos);
			// find current xpos
			double xpos = 0.0;
			for (int j = ls.firstItem; j < pos; ++j)
				xpos += item(j)->glyph.wide();
			if (pos != m_lastMagicPos || xpos > m_magicX)
				m_magicX = xpos;
			const LineSpec & ls2(m_lines.at(i+1));
			// find new cpos
			xpos = 0.0;
			for (int j = ls2.firstItem; j <= ls2.lastItem; ++j) 
			{
				xpos += item(j)->glyph.wide();
				if (xpos > m_magicX) {
					m_lastMagicPos = j;
					return j;
				}
			}
			m_lastMagicPos = ls2.lastItem + 1;
			return ls2.lastItem + 1;
		}
	}
	return lastFrameItem;
}

int StoryText::startOfFrame(int pos) 
{
	return firstFrameItem;
}
int StoryText::endOfFrame(int pos)
{
	return lastFrameItem + 1;
}

// selection

int StoryText::startOfSelection() const
{
	return selFirst <= selLast? selFirst : 0;
}

int StoryText::endOfSelection() const
{
	return selFirst <= selLast? selLast + 1 : -1;
}

int StoryText::lengthOfSelection() const
{
	return selFirst <= selLast? selLast - selFirst + 1 : 0;
}


bool StoryText::selected(int pos) const
{
	return (selFirst <= pos && pos <= selLast) 
//	       || (pos >= 0 && pos < length() && const_cast<StoryText*>(this)->d->at(pos)->cselect)
	;
}

void StoryText::select(int pos, uint len, bool on)
{
	if (pos < 0)
		pos += length();

	assert( pos >= 0 );
	assert( pos + signed(len) <= length() );

//	qDebug("old selection: %d - %d", selFirst, selLast);

//	StoryText* that = const_cast<StoryText *>(this);
//	for (int i=pos; i < pos+signed(len); ++i)
//		that->at(i)->cselect = on;

	if (on) {
		// extend if possible
		if (selected(pos - 1))
			selLast = QMAX(selLast, pos + static_cast<int>(len) - 1);
		else if (selected(pos + len))
			selFirst = QMIN(selFirst, pos);
		else {
			selFirst = pos;
			selLast = pos + len - 1;
		}
	}
	else {
		if (pos <= selFirst && selLast < pos + signed(len))
			deselectAll();
		// shrink
		else if (!selected(pos - 1) && selected(pos + len - 1))
			selFirst = pos + len;
		else if (selected(pos) && !selected(pos + len))
			selLast = pos - 1;
		else if (selected(pos) || selected(pos + len - 1))
			// Grr, deselection splits selection
			selLast = pos - 1;
	}
	
//	qDebug("new selection: %d - %d", selFirst, selLast);
}

void StoryText::selectAll()
{
/*	StoryText* that = const_cast<StoryText *>(this);
	that->at(0);
	for (int i=0; i < length(); ++i) {
		that->current()->cselect = true;
		that->next();
	}
*/
	selFirst = 0;
	selLast = length() - 1;
}

void StoryText::deselectAll()
{
/*	StoryText* that = const_cast<StoryText *>(this);
	that->at(0);
	for (int i=0; i < length(); ++i) {
		that->current()->cselect = false;
		that->next();
	}
*/	
	selFirst = 0;
	selLast = -1;
}

void StoryText::removeSelection()
{
//	qDebug("removeSelection: %d - %d", selFirst, selLast);
	if (selFirst > selLast)
		return;

	assert( selFirst >= 0 );
	assert( selLast < length() );

	removeChars(selFirst, selLast - selFirst+1);
	deselectAll();
}



void StoryText::invalidateObject(const PageItem * embedded)
{
}

void StoryText::invalidateLayout()
{
}

void StoryText::invalidateAll()
{
}

void StoryText::invalidate(int firstitem, int lastitem)
{
}


// physical view

/*
void StoryText::validate()
{
	static bool withinValidate = false;
	
	assert( !withinValidate );
	withinValidate = true;
	
	withinValidate = false;
}
*/

int StoryText::screenToPosition(FPoint coord) const
{
	double maxx = coord.x() - 1.0;
	for (unsigned int i=0; i < lines(); ++i)
	{
		LineSpec ls = line(i);
//		qDebug(QString("screenToPosition: (%1,%2) -> y %3 - %4 + %5").arg(coord.x()).arg(coord.y()).arg(ls.y).arg(ls.ascent).arg(ls.descent));
		if (ls.y + ls.descent < coord.y())
			continue;
		double xpos = ls.x;
		for (int j = ls.firstItem; j <= ls.lastItem; ++j) {
//				qDebug(QString("screenToPosition: (%1,%2) -> x %3 + %4").arg(coord.x()).arg(coord.y()).arg(xpos).arg(item(j)->glyph.wide()));
			double width = item(j)->glyph.wide();
			xpos += width;
			if (xpos >= coord.x())
				return xpos - width/2 > coord.x() ? j : j+1;
		}
		if (xpos > maxx)
			maxx = xpos;
		if (xpos + 1.0 > coord.x()) // allow 1pt after end of line
			return ls.lastItem + 1;
		else if (xpos < ls.x + 0.01 && maxx >= coord.x()) // check for empty line
			return ls.firstItem;
	}
	return QMAX(lastFrameItem+1, firstFrameItem);
}


FRect StoryText::boundingBox(int pos, uint len) const
{
	FRect result;
	for (uint i=0; i < lines(); ++i)
	{
		LineSpec ls = line(i);
		if (ls.lastItem < pos)
			continue;
		if (ls.firstItem <= pos) {
			double xpos = ls.x;
			for (int j = ls.firstItem; j < pos; ++j)
				xpos += item(j)->glyph.wide();
			result.setRect(xpos, ls.y-ls.ascent, item(pos)->glyph.wide(), ls.ascent + ls.descent);
			break;
		}
	}
	return result;
}


int StoryText::layout(int startItem)
{
	//FIXME:NLS
	return -1;
}


uint StoryText::nrOfItems() const
{
	return length();
}


ScText*  StoryText::item(uint itm)
{
	assert( static_cast<int>(itm) < length() );
	return const_cast<StoryText *>(this)->d->at(itm);
}


const ScText*  StoryText::item(uint itm) const
{
	assert( static_cast<int>(itm) < length() );
	return const_cast<StoryText *>(this)->d->at(itm);
}


const QString StoryText::itemText(uint itm) const
{
	
	assert( static_cast<int>(itm) < length() );

	return text(itm, 1);
}


const CharStyle StoryText::itemStyle(uint itm) const
{
	assert( static_cast<int>(itm) < length() );

	return charStyle(itm);
}
	

int StoryText::startOfItem(uint itm) const
{
	assert( static_cast<int>(itm) < length() );

	return itm;
}

int StoryText::endOfItem(uint itm) const
{
	assert( static_cast<int>(itm) < length() );

	return itm + 1;
}

