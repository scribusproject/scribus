#ifndef GLYPHRUN_H
#define GLYPHRUN_H

#include <QList>

#include "scribusapi.h"
#include "sctextstruct.h"

class SCRIBUS_API GlyphCluster
{
public:
	GlyphCluster(const CharStyle* style, LayoutFlags flags, int first, int last, const InlineFrame& o, int visualIdx, QString str);

	void append(GlyphLayout&);

	const CharStyle& style()  const;
	bool hasFlag(LayoutFlags f) const ;
	void setFlag(LayoutFlags f);
	void clearFlag(LayoutFlags f);

	QList<GlyphLayout>& glyphs();
	const QList<GlyphLayout>& glyphs() const;
	const InlineFrame& object() const;

	int firstChar() const;
	int lastChar() const;
	int visualIndex() const;

	double width() const;

	double ascent() const;
	double descent() const;

	double scaleH() const;
	double scaleV() const;
	void setScaleH(double);
	void setScaleV(double);

	bool isEmpty() const;
	bool isControlGlyphs() const;
	bool isSpace() const;
	QVector<FPointArray> glyphClusterOutline() const;
	// get text out
	QString getText() const;

	double extraWidth;
	double xoffset;
	double yoffset;
private:
	const CharStyle* m_style;
	LayoutFlags m_flags;
	QList<GlyphLayout> m_glyphs;
	InlineFrame m_object;
	int m_firstChar;
	int m_lastChar;
	int m_visualIndex;
	double m_scaleH;
	double m_scaleV;
	QString m_str;
};

#endif // GLYPHRUN_H
