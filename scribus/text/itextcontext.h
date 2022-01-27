/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef ITEXTCONTEXT_H
#define ITEXTCONTEXT_H

#include <QRectF>
#include <QString>

#include "scribusapi.h"

class CharStyle;
class ParagraphStyle;
struct TypoPrefs;
class InlineFrame;
class ExpansionPoint;
class PageItem;
class ScribusDoc;

/**
 * This class collects all information that may be used during text layout:
 *  - the containing frame's dimensions
 *  - interpretation of inline objects
 *  - how to expand expansionpoints like pagenr and marks
 *  - style settings specific to the document or the containing frame (tyopgraphy, baseline grid, text margins, ...)
 */
class SCRIBUS_API ITextContext {

public:
	virtual const ScribusDoc* getDoc() const = 0;
	virtual const PageItem* getFrame() const = 0;
	virtual double height() const = 0;
	virtual double width() const = 0;
	virtual /* const */ PageItem* object(const InlineFrame& frame) const = 0;
	virtual QRectF getVisualBoundingBox(const InlineFrame& frame) const = 0;
	virtual const CharStyle& charStyle() const = 0;
	virtual const ParagraphStyle& paragraphStyle() const = 0;
	virtual QString expand(const ExpansionPoint& expansion) = 0;
	virtual const TypoPrefs& typographicPrefs() const = 0;
};


#endif // ITEXTCONTEXT_H
