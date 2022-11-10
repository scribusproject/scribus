/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fontembeddingcombo.h"
#include "fontembeddingmodel.h"
// #include "qdebug.h"

FontEmbeddingCombo::FontEmbeddingCombo(QWidget* parent) : QComboBox(parent)
{
#ifdef Q_OS_MACOS
//	setStyle( new FontEmbeddingCombo::ScMacStyle() );
#endif
	m_fontEmbeddingModel = new FontEmbeddingModel(this);
	
	setEditable(false);
	setModel(m_fontEmbeddingModel);
}

FontEmbeddingCombo::~FontEmbeddingCombo()
{
	if (m_fontEmbeddingModel)
	{
		delete m_fontEmbeddingModel;
		m_fontEmbeddingModel = nullptr;
	}
}

PDFOptions::PDFFontEmbedding FontEmbeddingCombo::embeddingMode() const
{
	int curIndex = currentIndex();
	if (curIndex == 0)
		return PDFOptions::EmbedFonts;
	if (curIndex == 1)
		return PDFOptions::OutlineFonts;
	if (curIndex == 2)
		return PDFOptions::DontEmbed;
	return PDFOptions::EmbedFonts;
}

void FontEmbeddingCombo::setEmbeddingMode(PDFOptions::PDFFontEmbedding mode)
{
	if ((mode == PDFOptions::DontEmbed) && !m_allowNoFontEmbedding)
		return;
	
	if (mode == PDFOptions::EmbedFonts)
		setCurrentIndex(0);
	else if (mode == PDFOptions::OutlineFonts)
		setCurrentIndex(1);
	else if  (mode == PDFOptions::DontEmbed)
		setCurrentIndex(2);
}

void FontEmbeddingCombo::setNoEmbeddingEnabled(bool enabled)
{
	if (m_allowNoFontEmbedding == enabled)
		return;
	m_allowNoFontEmbedding = enabled;
	
	PDFOptions::PDFFontEmbedding oldMode = embeddingMode();
	if ((oldMode == PDFOptions::DontEmbed) && !enabled)
		setCurrentIndex(0);
	m_fontEmbeddingModel->setNoFontEmbeddingEnabled(enabled);
}
