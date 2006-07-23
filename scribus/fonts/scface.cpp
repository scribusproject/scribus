

#include "scribusapi.h"
#include "fonts/scface.h"


double ScFace::ScFaceData::glyphKerning(uint gl1, uint gl2, double sz) const 
{ 
	return 0.0; 
}


bool ScFace::ScFaceData::glyphNames(QMap<QChar, std::pair<uint, QString> >& gList) const 
{ 
	return false; 
}


QMap<QString,QString> ScFace::ScFaceData::fontDictionary(double sz) const
{
	return QMap<QString, QString>();
}

GlyphMetrics ScFace::ScFaceData::glyphBBox(uint gl, double sz) const
{
	GlyphMetrics res;
	res.width = sz;
	res.ascent = sz;
	res.descent = 0;
	return res;
}


double ScFace::ScFaceData::glyphWidth(uint gl, double size) const
{
	if (! m_glyphWidth.contains(gl)) {
		loadGlyph(gl);
	}		
	return m_glyphWidth[gl] * size;
}


FPointArray ScFace::ScFaceData::glyphOutline(uint gl, double sz) const 
{ 
	if (! m_glyphWidth.contains(gl)) {
		loadGlyph(gl);
	}			
	FPointArray res = m_glyphOutline[gl].Outlines.copy();
	if (sz != 1.0)
		res.scale(sz, sz);
	return res;
}


FPoint ScFace::ScFaceData::glyphOrigin(uint gl, double sz) const 
{
	if (! m_glyphWidth.contains(gl)) {
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

ScFace::ScFace()
{
	m = new ScFaceData();
	m->refs = 1;
	m->usage = 0;

	m->status = ScFace::NULLFACE;
	m->typeCode = ScFace::UNKNOWN_TYPE;
	m->formatCode = ScFace::UNKNOWN_FORMAT;

	m->scName = "";
	m->family = "";
	m->style = "";
	m->variant = "";
	m->psName = "";
	m->fontFile = "(None)";
	m->faceIndex = -1;
	m->forDocument = "";
	m->usable = false;
	m->embedPs = false;
	m->subset = false;
	m->maxGlyph = 0;
	m->cachedStatus = ScFace::UNKNOWN;
}


ScFace::ScFace(ScFaceData* data)
{
	m = data;
	m->refs = 1;
	m->cachedStatus = ScFace::UNKNOWN;
}

ScFace::ScFace(const ScFace& other)
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
	++(other.m->refs);	
	if ( m && --(m->refs) == 0 ) {
		m->unload();
		delete m;
	}
	m = other.m;
}


/** two ScFaces are equal if they either are both NULLFACEs or they
agree on family, style, variant and fontpath
*/
bool ScFace::operator==(const ScFace& other) const
{
	return ( (isNone() && other.isNone() )
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


uint ScFace::char2CMap(QChar ch) const
{
	if (m->status == ScFace::UNKNOWN) {
		m->load();
	}
	return m->char2CMap(ch);
}


bool ScFace::canRender(QChar ch) const 
{
	if (!usable())
		return false;
	else if (ch.unicode() < 32) {
		return ch.unicode() == 28 || ch.unicode() == 13 || ch.unicode() == 9; 
	}
	else {
		uint gl = char2CMap(ch); //  calls load()
		if (gl != 0) {
			m->loadGlyph(gl);
			return ! m->m_glyphOutline[gl].broken; 
		}
		else  {
			return false;
		}
	}
}


double ScFace::charWidth(QChar ch, double size, QChar ch2) const
{
	if (!canRender(ch)) // calls loadGlyph()
		return size;
	else if (ch.unicode() == 28 || ch.unicode() == 13 || ch.unicode() == 9)
		return ch.unicode() == 9 ? 1.0 : 0.0;
	else {
		uint gl1 = char2CMap(ch);
		uint gl2 = ch2.unicode() != 32 ? char2CMap(ch2) : 0;
		double width = glyphWidth(gl1, size);
		if (gl2 != 0)
			width += glyphKerning(gl1, gl2, size);
//		qDebug(QString("scface::glyphkerning: %1_%2 = %3 (%4, %5)").arg(ch).arg(ch2).arg(glyphKerning(gl1, gl2,size)).arg(gl1).arg(gl2));
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


bool ScFace::glyphNames(QMap<QChar, std::pair<uint, QString> >& gList)
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
