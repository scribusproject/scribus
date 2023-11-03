/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTEMBEDDINGCOMBO_H
#define FONTEMBEDDINGCOMBO_H

#include <QComboBox>

#include "scribusapi.h"
#include "pdfoptions.h"

class FontEmbeddingModel;

/*!
    \class FontEmbeddingCombo fontembeddingcombo.h
    \brief The FontEmbeddingCombo widget is a combo box for displaying font embedding modes.
*/
class SCRIBUS_API FontEmbeddingCombo : public QComboBox 
{
	Q_OBJECT

public:
	FontEmbeddingCombo(QWidget* parent = nullptr);
	~FontEmbeddingCombo();

	PDFOptions::PDFFontEmbedding embeddingMode() const;

	void setEmbeddingMode(PDFOptions::PDFFontEmbedding mode);
	void setNoEmbeddingEnabled(bool enabled);
	
private:
	bool m_allowNoFontEmbedding { true };
	FontEmbeddingModel* m_fontEmbeddingModel { nullptr };
};

#endif
