/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCFACE_TTF_H
#define SCFACE_TTF_H

#include "scribusapi.h"
#include "fonts/ftface.h"


#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_TAGS_H


/**
	An object holding a table of kerning pairs extracted from
	a kern feature such as found in a GPOS table
 */
class SCRIBUS_API KernFeature
{
	typedef QMap<quint16, QList<quint16> > ClassDefTable; // <Class index (0 to N) , list of glyphs >

	public:
		/**
		* Build a ready-to-use kerning pairs table
		* @param face a valid FT_Face, It won’t be store by KernFeature
		 */
		KernFeature ( FT_Face face );
		KernFeature ( const KernFeature& kf );
		~KernFeature();

		/**
		 * Get the kerning value for a pair of glyph indexes.
		 * @param glyph1 Index of the left glyph in logical order
		 * @param glyph2 Index of the right glyph in logical order
		 * @return the unscaled delta to apply to xadvance of the first glyph
		 */
		double getPairValue ( unsigned int glyph1, unsigned int glyph2 ) const;

		/**
		 * The table can have been invalidated if something went wrong at any moment.
		 * @return True if valid, False otherwise.
		 */
		bool isValid() const {return m_valid;}

	private:
		bool m_valid;
		QByteArray GPOSTableRaw;
		QMap<quint16,QList<quint16> > coverages;
		mutable QMap<quint16, QMap<quint16, double> > pairs;
		QMap< quint16, QMap<quint16, ClassDefTable> > classGlyphFirst; // < subtable offset, map<offset, class definition table> > for first glyph
		QMap< quint16, QMap<quint16, ClassDefTable> > classGlyphSecond; // < subtable offset, map<offset, class definition table> > for second glyph
		QMap< quint16, QMap<int, QMap<int, double> > > classValue; // < subtable offset, map<class1, map<class2, value> > >

		void makeCoverage();
		void makePairs ( quint16 subtableOffset );

		ClassDefTable getClass (bool leftGlyph, quint16 classDefOffset, quint16 coverageId );
		inline quint16 toUint16 ( quint16 index );
		inline qint16 toInt16 ( quint16 index );

		enum ValueFormat
		{
			XPlacement = 0x0001,
			YPlacement = 0x0002,
			XAdvance = 0x0004,
			YAdvance = 0x0008,
			XPlaDevice =0x0010,
			YPlaDevice =0x0020,
			XAdvDevice =0x0040,
			YAdvDevice =0x0080
		};
		QString FontName;// for debugging purpose
};



/*
	Class ScFace_ttf
	Subclass of ScFace, specifically for TrueType fonts.
	Implements: RealName() and EmbedFont().
*/

class SCRIBUS_API ScFace_ttf : public FtFace
{
public:
	ScFace_ttf ( QString fam, QString sty, QString alt, QString scname, QString psname, QString path, int face );
	~ScFace_ttf();

	void load () const;
	void unload () const;

	bool EmbedFont(QString &str) const;
	void RawData(QByteArray & bb) const;

	qreal glyphKerning ( uint gl1, uint gl2, qreal sz ) const;

private:
	mutable KernFeature * kernFeature;
};

#endif
