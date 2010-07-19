/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scribusapi.h"
#include "fonts/scface.h"
#include "text/storytext.h"

ScFace::ScFaceData::ScFaceData() : 
	refs(0), 
	usage(0), 
	scName(""),
	fontFile("(None)"),
	faceIndex(-1),
	psName(""),
	family(""),
	style(""),
	variant(""),
	forDocument(""),
	status(ScFace::NULLFACE),
	typeCode(ScFace::UNKNOWN_TYPE),
	formatCode(ScFace::UNKNOWN_FORMAT),
	usable(false),
	embedPs(false),
	subset(false),
	isStroked(false),
	isFixedPitch(false),
	hasNames(false),
	maxGlyph(0),
	cachedStatus(ScFace::UNKNOWN)
{
}

qreal ScFace::ScFaceData::glyphKerning(uint /*gl1*/, uint /*gl2*/, qreal /*sz*/) const 
{ 
	return 0.0; 
}


bool ScFace::ScFaceData::glyphNames(QMap<uint, std::pair<QChar, QString> >& /*gList*/) const 
{ 
	return false; 
}


QMap<QString,QString> ScFace::ScFaceData::fontDictionary(qreal /*sz*/) const
{
	return QMap<QString, QString>();
}


GlyphMetrics ScFace::ScFaceData::glyphBBox(uint gl, qreal sz) const
{
	GlyphMetrics res;
	if (gl == 0 || gl >= CONTROL_GLYPHS)
	{	res.width   = glyphWidth(gl, sz);
		res.ascent  = (gl == 0? ascent(sz) : 0);
		res.descent = 0;
		return res;
	}
	else if (! m_glyphWidth.contains(gl)) {
		loadGlyph(gl);
	}			
	const struct GlyphData & data(m_glyphOutline[gl]);
	res.width = data.bbox_width * sz;
	res.ascent = data.bbox_ascent * sz;
	res.descent = data.bbox_descent * sz;	
	return res;
}


qreal ScFace::ScFaceData::glyphWidth(uint gl, qreal size) const
{
	if (gl >= CONTROL_GLYPHS)
		return 0.0;
	else if (gl == 0)
		return size;
	else if (! m_glyphWidth.contains(gl)) {
		loadGlyph(gl);
	}		
	return m_glyphWidth[gl] * size;
}


FPointArray ScFace::ScFaceData::glyphOutline(uint gl, qreal sz) const 
{ 
	if (gl >= CONTROL_GLYPHS)
		return FPointArray();
	else if (gl == 0) {
		sz *= 10;
		FPointArray sq;
		sq.addQuadPoint(0,0,0,0,sz,0,sz,0);
		sq.addQuadPoint(sz,0,sz,0,sz,sz,sz,sz);
		sq.addQuadPoint(sz,sz,sz,sz,0,sz,0,sz);
		sq.addQuadPoint(0,sz,0,sz,0,0,0,0);
		return sq;
	}
	else if (! m_glyphWidth.contains(gl)) {
		loadGlyph(gl);
	}			
	FPointArray res = m_glyphOutline[gl].Outlines.copy();
	if (sz != 1.0)
		res.scale(sz, sz);
	return res;
}


FPoint ScFace::ScFaceData::glyphOrigin(uint gl, qreal sz) const 
{
	if (gl == 0 || gl >= CONTROL_GLYPHS)
		return FPoint(0,0);
	else if (! m_glyphWidth.contains(gl)) {
		loadGlyph(gl);
	}			
	const struct GlyphData & res(m_glyphOutline[gl]);
	return FPoint(res.x, res.y) * sz; 
}


/*****
   ScFace lifecycle:  unchecked -> loaded -> glyphs checked
                               |         \-> broken glyphs
							   \-> broken
   usable() == ! broken
   embeddable() == glyphs_checked
   
   canRender(unicode) -> CharMap cache? -> loadChar/Glyph -> !broken
   Glyphs:  width    status
            -1000    unkown
            -2000    broken
            >= 0     ok, outline valid
   CharMap:  unicode -> glyph index
             uint[256][256]
   unicode ignores: < 32, ...
   unicode emulate: spaces, hyphen, ligatures?, diacritics?
 *****/

ScFace::ScFace() :  replacedName(), replacedInDoc()
{
	m = new ScFaceData();
	m->refs = 1;
	m->usage = 0;
}


ScFace::ScFace(ScFaceData* data) : replacedName(), replacedInDoc()
{
	m = data;
	++(m->refs);
	m->cachedStatus = ScFace::UNKNOWN;
}

ScFace::ScFace(const ScFace& other) : replacedName(other.replacedName), replacedInDoc(other.replacedInDoc)
{
	m = other.m;
	++(m->refs);
}

ScFace::~ScFace()
{
	if ( m && --(m->refs) == 0 ) {
		m->unload();
		delete m;
		m = 0;
	}
}


ScFace& ScFace::operator=(const ScFace& other)
{
	if (m != other.m)
	{
		if (other.m)
			++(other.m->refs);
		if ( m && --(m->refs) == 0 ) {
			m->unload();
			delete m;
		}
		m = other.m;
	}
	replacedName = other.replacedName;
	return *this;
}


/** two ScFaces are equal if they either are both NULLFACEs or they
agree on family, style, variant and fontpath
*/
bool ScFace::operator==(const ScFace& other) const
{
	return replacedName == other.replacedName && 
		( (isNone() && other.isNone() )
			 || (m == other.m)
			 || (m->family == other.m->family
				 && m->style == other.m->style
				 && m->variant == other.m->variant
				 && m->fontFile == other.m->fontFile
				 && m-> faceIndex == other.m->faceIndex) );
}


const ScFace& ScFace::none() 
{ 
	static ScFace NONE;
	return NONE; 
}

QString ScFace::ascentAsString() const
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->ascentAsString();
}

QString ScFace::descentAsString() const
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->descentAsString();
}
QString ScFace::capHeightAsString() const
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->capHeightAsString();
}

QString ScFace::fontBBoxAsString() const
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->FontBBoxAsString();
}

QString ScFace::italicAngleAsString() const
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->ItalicAngleAsString();
}

qreal ScFace::ascent(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->ascent(sz); 
}

qreal ScFace::descent(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->descent(sz); 
}
qreal ScFace::xHeight(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->xHeight(sz); 
}

qreal ScFace::capHeight(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->capHeight(sz); 
}

qreal ScFace::height(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->height(sz); 
}

qreal ScFace::strikeoutPos(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->strikeoutPos(sz); 
}

qreal ScFace::underlinePos(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->underlinePos(sz); 
}

qreal ScFace::strokeWidth(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->strokeWidth(sz); 
}

qreal ScFace::maxAdvanceWidth(qreal sz) const 
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->maxAdvanceWidth(sz); 
}

void ScFace::increaseUsage() const
{
	m->usage++;
}


void ScFace::decreaseUsage() const
{
	if (m->usage == 1) 
		unload();
	m->usage--;
}


void ScFace::unload() const
{
	if (m->status >= ScFace::LOADED && usable()) {
		m->unload();
	}
	// clear caches
	m->m_glyphWidth.clear();
	m->m_glyphOutline.clear();
	m->m_cMap.clear();
	m->status = ScFace::UNKNOWN;
}


uint ScFace::emulateGlyph(QChar ch) const
{
	if (ch == SpecialChars::LINEBREAK || ch == SpecialChars::PARSEP 
		|| ch == SpecialChars::FRAMEBREAK || ch == SpecialChars::COLBREAK 
		|| ch == SpecialChars::TAB || ch == SpecialChars::SHYPHEN
		 || ch == SpecialChars::ZWSPACE || ch == SpecialChars::ZWNBSPACE)
		return CONTROL_GLYPHS + ch.unicode();
	else if (ch == SpecialChars::NBSPACE)
		return  m->char2CMap(QChar(' '));
	else if(ch == SpecialChars::NBHYPHEN)
		return  m->char2CMap(QChar('-'));
	else
		return 0;
}


uint ScFace::char2CMap(QChar ch) const
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	
	if (ch == SpecialChars::SHYPHEN)
		return emulateGlyph(ch);

	uint gl = m->char2CMap(ch);
	
	if (gl == 0)
		return emulateGlyph(ch);
	else
		return gl;
}


bool ScFace::canRender(QChar ch) const 
{
	if (!usable())
		return false;
	else {
		uint gl = char2CMap(ch);    //  calls load()
		if (gl >= CONTROL_GLYPHS)   //  those are always empty
			return true;
		else if (gl != 0) {
			m->loadGlyph(gl);
			return ! m->m_glyphOutline[gl].broken; 
		}
		else  {
			return false;
		}
	}
}


qreal ScFace::charWidth(QChar ch, qreal size, QChar ch2) const
{
	if (!canRender(ch)) // calls loadGlyph()
		return size;
	else if (ch.unicode() == 28 || ch.unicode() == 13 || ch.unicode() == 9)
		return ch.unicode() == 9 ? 1.0 : 0.0;
	else {
		uint gl1 = char2CMap(ch);
		uint gl2 = char2CMap(ch2);
		qreal width = glyphWidth(gl1, size);
		if (gl2 != 0)
			width += glyphKerning(gl1, gl2, size);
//		qDebug() << QString("scface::glyphkerning: %1_%2 = %3 (%4, %5)").arg(ch).arg(ch2).arg(glyphKerning(gl1, gl2,size)).arg(gl1).arg(gl2);
		return width;
	}
}


bool ScFace::EmbedFont(QString &str)
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->EmbedFont(str);
}


bool ScFace::glyphNames(QMap<uint, std::pair<QChar, QString> >& gList)
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->glyphNames(gList);
}


void ScFace::RawData(QByteArray & bb)
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	m->RawData(bb);
}

void ScFace::checkAllGlyphs()
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	if (m->status != ScFace::LOADED) {
		return;
	}
	for (uint gl=0; gl <= m->maxGlyph; ++gl) {
		if (! m->m_glyphWidth.contains(gl)) {
			m->loadGlyph(gl);
			m->m_glyphWidth.remove(gl);
			m->m_glyphOutline.remove(gl);
		}
	}
}
