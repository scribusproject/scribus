/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scribusapi.h"
#include "fonts/scface.h"
#include "text/storytext.h"

static const QString NONE_LITERAL("(None)");

ScFace::ScFaceData::ScFaceData() : 
	refs(0), 
	usage(0), 
	fontFile(NONE_LITERAL),
	faceIndex(-1),
	status(ScFace::NULLFACE),
	typeCode(ScFace::UNKNOWN_TYPE),
	formatCode(ScFace::UNKNOWN_FORMAT),
	usable(false),
	embedPs(false),
	subset(false),
	isStroked(false),
	isFixedPitch(false),
	hasGlyphNames(false),
	maxGlyph(0),
	m_cachedStatus(ScFace::UNKNOWN)
{
}

qreal ScFace::ScFaceData::glyphKerning(gid_type /*gl1*/, gid_type /*gl2*/, qreal /*sz*/) const
{ 
	return 0.0; 
}


bool ScFace::ScFaceData::glyphNames(FaceEncoding& /*gList*/) const
{ 
	return false; 
}


QMap<QString,QString> ScFace::ScFaceData::fontDictionary(qreal /*sz*/) const
{
	return QMap<QString, QString>();
}


GlyphMetrics ScFace::ScFaceData::glyphBBox(gid_type gl, qreal sz) const
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


qreal ScFace::ScFaceData::glyphWidth(gid_type gl, qreal size) const
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


FPointArray ScFace::ScFaceData::glyphOutline(gid_type gl, qreal sz) const
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


FPoint ScFace::ScFaceData::glyphOrigin(gid_type gl, qreal sz) const
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
             gid_type[256][256]
   unicode ignores: < 32, ...
   unicode emulate: spaces, hyphen, ligatures?, diacritics?
 *****/

ScFace::ScFace() :  m_m(new ScFaceData())
{
	m_m->refs = 1;
	m_m->usage = 0;
}


ScFace::ScFace(ScFaceData* data) : m_m(data)
{
	++(m_m->refs);
	m_m->m_cachedStatus = ScFace::UNKNOWN;
}

ScFace::ScFace(const ScFace& other) : m_m(other.m_m), m_replacedName(other.m_replacedName), m_replacedInDoc(other.m_replacedInDoc)
{
	++(m_m->refs);
}

ScFace::~ScFace()
{
	if ( m_m && --(m_m->refs) == 0 ) {
		m_m->unload();
		delete m_m;
		m_m = 0;
	}
}


ScFace& ScFace::operator=(const ScFace& other)
{
	if (m_m != other.m_m)
	{
		if (other.m_m)
			++(other.m_m->refs);
		if ( m_m && --(m_m->refs) == 0 ) {
			m_m->unload();
			delete m_m;
		}
		m_m = other.m_m;
	}
	m_replacedName = other.m_replacedName;
	return *this;
}


/** two ScFaces are equal if they either are both NULLFACEs or they
agree on family, style, variant and fontpath
*/
bool ScFace::operator==(const ScFace& other) const
{
	return m_replacedName == other.m_replacedName && 
		( (isNone() && other.isNone() )
			 || (m_m == other.m_m)
			 || (m_m->family == other.m_m->family
				 && m_m->style == other.m_m->style
				 && m_m->variant == other.m_m->variant
				 && m_m->fontFile == other.m_m->fontFile
				 && m_m-> faceIndex == other.m_m->faceIndex) );
}


const ScFace& ScFace::none() 
{ 
	static ScFace NONE;
	return NONE; 
}

bool ScFace::isSymbolic() const
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->isSymbolic();
}

QString ScFace::pdfAscentAsString() const
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->pdfAscentAsString();
}

QString ScFace::pdfDescentAsString() const
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->pdfDescentAsString();
}
QString ScFace::pdfCapHeightAsString() const
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->pdfCapHeightAsString();
}

QString ScFace::pdfFontBBoxAsString() const
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->pdfFontBBoxAsString();
}

QString ScFace::italicAngleAsString() const
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->italicAngleAsString();
}

qreal ScFace::ascent(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->ascent(sz); 
}

qreal ScFace::descent(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->descent(sz); 
}
qreal ScFace::xHeight(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->xHeight(sz); 
}

qreal ScFace::capHeight(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->capHeight(sz); 
}

qreal ScFace::height(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->height(sz); 
}

qreal ScFace::strikeoutPos(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->strikeoutPos(sz); 
}

qreal ScFace::underlinePos(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->underlinePos(sz); 
}

qreal ScFace::strokeWidth(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->strokeWidth(sz); 
}

qreal ScFace::maxAdvanceWidth(qreal sz) const 
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->maxAdvanceWidth(sz); 
}

void ScFace::increaseUsage() const
{
	m_m->usage++;
}


void ScFace::decreaseUsage() const
{
	if (m_m->usage == 1) 
		unload();
	m_m->usage--;
}


void ScFace::unload() const
{
	if (m_m->status >= ScFace::LOADED && usable()) {
		m_m->unload();
	}
	// clear caches
	m_m->m_glyphWidth.clear();
	m_m->m_glyphOutline.clear();
	//m->m_cMap.clear();
	m_m->status = ScFace::UNKNOWN;
}


ScFace::gid_type ScFace::emulateGlyph(QChar ch) const
{
	if (ch == SpecialChars::LINEBREAK || ch == SpecialChars::PARSEP 
		|| ch == SpecialChars::FRAMEBREAK || ch == SpecialChars::COLBREAK 
		|| ch == SpecialChars::TAB || ch == SpecialChars::SHYPHEN
		 || ch == SpecialChars::ZWSPACE || ch == SpecialChars::ZWNBSPACE || ch==SpecialChars::OBJECT)
		return CONTROL_GLYPHS + ch.unicode();
	else if (ch == SpecialChars::NBSPACE)
		return  m_m->char2CMap(QChar(' '));
	else if(ch == SpecialChars::NBHYPHEN)
		return  m_m->char2CMap(QChar('-'));
	else
		return 0;
}


ScFace::gid_type ScFace::char2CMap(QChar ch) const
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	
	if (ch == SpecialChars::SHYPHEN)
		return emulateGlyph(ch);

	gid_type gl = m_m->char2CMap(ch);

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
		gid_type gl = char2CMap(ch);    //  calls load()
		if (gl >= CONTROL_GLYPHS)   //  those are always empty
			return true;
		else if (gl != 0) {
			m_m->loadGlyph(gl);
			return ! m_m->m_glyphOutline[gl].broken; 
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
		gid_type gl1 = char2CMap(ch);
		gid_type gl2 = char2CMap(ch2);
		qreal width = glyphWidth(gl1, size);
		if (gl2 != 0)
			width += glyphKerning(gl1, gl2, size);
//		qDebug() << QString("scface::glyphkerning: %1_%2 = %3 (%4, %5)").arg(ch).arg(ch2).arg(glyphKerning(gl1, gl2,size)).arg(gl1).arg(gl2);
		return width;
	}
}


bool ScFace::EmbedFont(QByteArray &str)
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->EmbedFont(str);
}


bool ScFace::glyphNames(FaceEncoding& gList)
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	return m_m->glyphNames(gList);
}


void ScFace::RawData(QByteArray & bb)
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	m_m->RawData(bb);
}

void ScFace::checkAllGlyphs()
{
	if (m_m->status == ScFace::UNKNOWN) {
		m_m->load();
	}
	if (m_m->status != ScFace::LOADED) {
		return;
	}
	for (gid_type gl=0; gl <= m_m->maxGlyph; ++gl) {
		if (! m_m->m_glyphWidth.contains(gl)) {
			m_m->loadGlyph(gl);
			m_m->m_glyphWidth.remove(gl);
			m_m->m_glyphOutline.remove(gl);
		}
	}
}
