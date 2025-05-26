/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus171format.h"
#include "scribus171formatimpl.h"

#include <ctime>
#include <memory>
#include <utility>

#include <QCursor>
#include <QFileInfo>
#include <QList>
#include <QDataStream>
#include <QScopedPointer>

#include "../../formatidlist.h"

#include "api/api_application.h"
#include "commonstrings.h"
#include "hyphenator.h"
#include "notesstyles.h"
#include "pageitem_arc.h"
#include "pageitem_latexframe.h"
#include "pageitem_noteframe.h"
#ifdef HAVE_OSG
	#include "pageitem_osgframe.h"
#endif
#include "pageitem_regularpolygon.h"
#include "pageitem_spiral.h"
#include "pageitem_table.h"
#include "prefsmanager.h"
#include "qtiocompressor.h"
#include "resourcecollection.h"
#include "scconfig.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scxmlstreamwriter.h"
#include "textnote.h"
#include "ui/missing.h"
#include "units.h"
#include "util.h"
#include "util_color.h"
#include "util_text.h"

QString Scribus171Format::saveElements(double xp, double yp, double wp, double hp, Selection* selection, QByteArray &prevData)
{
	ResourceCollection lists;
	QList<PageItem*> emG;
	QList<PageItem*> emF;

	QString fileDir = ScPaths::applicationDataDir();
	QString documentStr;
	documentStr.reserve(524288);

	ScXmlStreamWriter writer(&documentStr);
	writer.setAutoFormatting(true);
	writer.writeStartElement("SCRIBUSELEMUTF8");
	writer.writeAttribute("XP", xp);
	writer.writeAttribute("YP", yp);
	writer.writeAttribute("W", wp);
	writer.writeAttribute("H", hp);
	writer.writeAttribute("COUNT", selection->count());
	writer.writeAttribute("Version", ScribusAPI::getVersion());
	writer.writeAttribute("previewData", QString(prevData));
	writeColors(writer, true);
	writeGradients(writer, true);

	for (int i = 0; i < selection->count(); ++i)
	{
		PageItem *currItem = selection->itemAt(i);
		currItem->getNamedResources(lists);
		emG.append(currItem);
		if ((!currItem->isTextFrame()) && (!currItem->isPathText()))
			continue;
		//for notes frames text should not be saved
		if (currItem->isNoteFrame())
			continue;
		for (int j = currItem->firstInFrame(); j <= currItem->lastInFrame(); ++j)
		{
			QChar chr = currItem->itemText.text(j);
			if (chr != SpecialChars::OBJECT)
				continue;
			if (!currItem->itemText.hasObject(j))
				continue;
			PageItem* pi = currItem->itemText.object(j).getPageItem(currItem->doc());
			if (!emF.contains(pi))
				emF.append(pi);
		}
	}

	// Write character styles
	QList<QString> names = lists.charStyleNames();
	QList<int> styleList = m_Doc->getSortedCharStyleList();
	for (int i = 0; i < styleList.count(); ++i)
	{
		const CharStyle& charStyle = m_Doc->charStyles()[styleList[i]];
		if (!names.contains(charStyle.name()))
			continue;
		writer.writeStartElement("CharacterStyle");
		putNamedCStyle(writer, charStyle);
		writer.writeEndElement();
	}

	// Write paragraph styles
	names = lists.styleNames();
	styleList = m_Doc->getSortedStyleList();
	for (int i = 0; i < styleList.count(); ++i)
	{
		const ParagraphStyle& paragraphStyle = m_Doc->paragraphStyles()[styleList[i]];
		if (names.contains(paragraphStyle.name()))
			putPStyle(writer, paragraphStyle, "ParagraphStyle");
	}

	writeLineStyles(writer, lists.lineStyleNames());
	writeArrowStyles(writer);

	// TODO Write used marks
	// writeMarks(writer);

	// Write note styles
	names = lists.noteStyleNames();
	writeNotesStyles(writer, names);

	// Write note frames
	QList<PageItem_NoteFrame*> nfList;
	for (int i = 0; i < selection->count(); ++i)
	{
		PageItem* item = selection->itemAt(i);
		if (!item->isNoteFrame())
			continue;
		nfList.append(dynamic_cast<PageItem_NoteFrame*>(item));
	}
	writeNotesFrames(writer, nfList);

	// TODO Write text notes (once marks are also written)
	// Do not uncomment before otherwise pasting note frame
	// will trigger a crash
	/*QList<TextNote*> textNotes;
	for (int i = 0; i < nfList.count(); ++i)
	{
		PageItem_NoteFrame* noteFrame = nfList.at(i);
		textNotes.append(noteFrame->notesList());
	}
	writeNotes(writer, textNotes);*/

	writePatterns(writer, fileDir, true, selection);
	if (!emF.isEmpty())
		WriteObjects(m_Doc, writer, fileDir, nullptr, 0, ItemSelectionFrame, &emF);
	WriteObjects(m_Doc, writer, fileDir, nullptr, 0, ItemSelectionElements, &emG);
	writer.writeEndElement();

	documentStr.squeeze();
	return documentStr.trimmed();
}

bool Scribus171Format::saveStory(StoryText& story, PageItem* item, QByteArray& data)
{
	ResourceCollection lists;
	QList<PageItem*> embeddedFrames;

	QString fileDir = ScPaths::applicationDataDir();
	QString documentStr;
	documentStr.reserve(524288);

	story.getNamedResources(lists);

	ScXmlStreamWriter writer(&documentStr);
	writer.setAutoFormatting(true);
	writer.writeStartElement("ScribusStory");
	writer.writeAttribute("Version", ScribusAPI::getVersion());

	writeColors(writer, lists.colorNames());
	writeGradients(writer, lists.gradientNames());

	for (int i = 0; i < story.length(); ++i)
	{
		QChar chr = story.text(i);
		if (chr != SpecialChars::OBJECT)
			continue;
		if (!story.hasObject(i))
			continue;
		PageItem* pi = story.object(i).getPageItem(story.doc());
		if (pi && !embeddedFrames.contains(pi))
			embeddedFrames.append(pi);
	}
	
	// Write character styles
	QList<QString> names = lists.charStyleNames();
	QList<int> styleList = m_Doc->getSortedCharStyleList();
	for (int i = 0; i < styleList.count(); ++i)
	{
		const CharStyle& charStyle = m_Doc->charStyles()[styleList[i]];
		if (!names.contains(charStyle.name()))
			continue;
		writer.writeStartElement("CharacterStyle");
		putNamedCStyle(writer, charStyle);
		writer.writeEndElement();
	}

	// Write paragraph styles
	names = lists.styleNames();
	styleList = m_Doc->getSortedStyleList();
	for (int i = 0; i < styleList.count(); ++i)
	{
		const ParagraphStyle& paragraphStyle = m_Doc->paragraphStyles()[styleList[i]];
		if (names.contains(paragraphStyle.name()))
			putPStyle(writer, paragraphStyle, "ParagraphStyle");
	}

	writeLineStyles(writer, lists.lineStyleNames());
	writeArrowStyles(writer);

	// TODO Write used marks
	// writeMarks(writer);

	// Write note styles
	names = lists.noteStyleNames();
	writeNotesStyles(writer, names);

	// TODO Write text notes (once marks are also written)
	// Do not uncomment before otherwise pasting note frame
	// will trigger a crash
	/*QList<TextNote*> textNotes;
	for (int i = 0; i < nfList.count(); ++i)
	{
		PageItem_NoteFrame* noteFrame = nfList.at(i);
		textNotes.append(noteFrame->notesList());
	}
	writeNotes(writer, textNotes);*/

	// FIXME: may be used in embedded objects
	// writePatterns(writer, fileDir, true, selection);
	if (!embeddedFrames.isEmpty())
		WriteObjects(m_Doc, writer, fileDir, nullptr, 0, ItemSelectionFrame, &embeddedFrames);
	writeStoryText(m_Doc, writer, story);
	writer.writeEndElement();

	documentStr.squeeze();
	documentStr = documentStr.trimmed();
	data = documentStr.toUtf8();

	return true;
}

bool Scribus171Format::savePalette(const QString & fileName)
{
	QString fileDir = QFileInfo(fileName).absolutePath();
	QScopedPointer<QIODevice> outputFile;
	outputFile.reset( new QFile(fileName) );
	if (!outputFile->open(QIODevice::WriteOnly))
		return false;
	ScXmlStreamWriter docu;
	docu.setAutoFormatting(true);
	docu.setDevice(outputFile.data());
	docu.writeStartDocument();
	docu.writeStartElement("ScribusColors");
	writeColors(docu);
	writeGradients(docu);
	writePatterns(docu, fileDir);
	docu.writeEndElement();
	docu.writeEndDocument();
	bool  writeSucceed = false;
	const QFile* qFile = qobject_cast<QFile*>(outputFile.data());
	writeSucceed = (qFile->error() == QFile::NoError);
	outputFile->close();
	return writeSucceed;
}

bool Scribus171Format::saveFile(const QString & fileName, const FileFormat & /* fmt */)
{
	m_lastSavedFile = "";

	// #11279: Image links get corrupted when symlinks involved
	// We have to proceed in tow steps here as QFileInfo::canonicalPath()
	// may not return correct result if fileName does not exists
	QString fileDir = QFileInfo(fileName).absolutePath();
	QString canonicalPath = QFileInfo(fileDir).canonicalFilePath();
	if (!canonicalPath.isEmpty())
		fileDir = canonicalPath;

	// Create a random temporary file name
	srand(time(nullptr)); // initialize random sequence each time
	long randt = 0;
	long randn = 1 + (int) (((double) rand() / ((double) RAND_MAX + 1)) * 10000);
	QString  tmpFileName  = QString("%1.%2").arg(fileName).arg(randn);
	while (QFile::exists(tmpFileName) && (randt < 100))
	{
		randn = 1 + (int) (((double) rand() / ((double) RAND_MAX + 1)) * 10000);
		tmpFileName = QString("%1.%2").arg(fileName).arg(randn);
		++randt;
	}
	if (QFile::exists(tmpFileName))
		return false;

	QScopedPointer<QIODevice> outputFile;
	if (fileName.toLower().right(2) == "gz")
	{
		aFile.setFileName(tmpFileName);
		QtIOCompressor *compressor = new QtIOCompressor(&aFile);
		compressor->setStreamFormat(QtIOCompressor::GzipFormat);
		outputFile.reset(compressor);
	}
	else
		outputFile.reset( new QFile(tmpFileName) );

	if (!outputFile->open(QIODevice::WriteOnly))
		return false;

	ScXmlStreamWriter docu;
	docu.setAutoFormatting(true);
	docu.setDevice(outputFile.data());
	docu.writeStartDocument();
	docu.writeStartElement("SCRIBUSUTF8NEW");
	docu.writeAttribute("Version", ScribusAPI::getVersion());

	docu.writeStartElement("Document");
	docu.writeAttribute("PageCount", m_Doc->DocPages.count());
	docu.writeAttribute("PageWidth", m_Doc->pageWidth());
	docu.writeAttribute("PageHeight", m_Doc->pageHeight());
	docu.writeAttribute("BorderLeft", m_Doc->margins()->left());
	docu.writeAttribute("BorderRight", m_Doc->margins()->right());
	docu.writeAttribute("BorderTop", m_Doc->margins()->top());
	docu.writeAttribute("BorderBottom", m_Doc->margins()->bottom());
	docu.writeAttribute("MarginPreset", m_Doc->marginPreset());
	docu.writeAttribute("BleedTop", m_Doc->bleeds()->top());
	docu.writeAttribute("BleedLeft", m_Doc->bleeds()->left());
	docu.writeAttribute("BleedRight", m_Doc->bleeds()->right());
	docu.writeAttribute("BleedBottom", m_Doc->bleeds()->bottom());
	docu.writeAttribute("PageOrientation", m_Doc->pageOrientation());
	docu.writeAttribute("PageSize", m_Doc->pageSize());
	docu.writeAttribute("FirstPageNumber", m_Doc->FirstPnum);
	docu.writeAttribute("PagePositioning", m_Doc->pagePositioning());
	if (m_Doc->usesAutomaticTextFrames())
		docu.writeAttribute("AutomaticTextFrames", 1);
	docu.writeAttribute("AutomaticTextFrameColumnCount", m_Doc->PageSp);
	docu.writeAttribute("AutomaticTextFrameColumnGap", m_Doc->PageSpa);
	docu.writeAttribute("UnitIndex", m_Doc->unitIndex());
	docu.writeAttribute("FontFace", m_Doc->itemToolPrefs().textFont);
	docu.writeAttribute("FontSize", m_Doc->itemToolPrefs().textSize / 10.0);
	docu.writeAttribute("TextColumnCount", m_Doc->itemToolPrefs().textColumns);
	docu.writeAttribute("TextColumnGap", m_Doc->itemToolPrefs().textColumnGap);
	docu.writeAttribute("TabFillCharacter", m_Doc->itemToolPrefs().textTabFillChar);
	docu.writeAttribute("TabWidth", m_Doc->itemToolPrefs().textTabWidth);
	docu.writeAttribute("TextDistanceLeft", m_Doc->itemToolPrefs().textDistances.left());
	docu.writeAttribute("TextDistanceRight", m_Doc->itemToolPrefs().textDistances.right());
	docu.writeAttribute("TextDistanceBottom", m_Doc->itemToolPrefs().textDistances.bottom());
	docu.writeAttribute("TextDistanceTop", m_Doc->itemToolPrefs().textDistances.top());
	docu.writeAttribute("FirstLineOffset", (int) m_Doc->itemToolPrefs().firstLineOffset);
	docu.writeAttribute("DocumentInfoAuthor", m_Doc->documentInfo().author());
	docu.writeAttribute("DocumentInfoComments", m_Doc->documentInfo().comments());
	docu.writeAttribute("DocumentInfoKeywords", m_Doc->documentInfo().keywords());
	docu.writeAttribute("DocumentInfoPublisher",m_Doc->documentInfo().publisher());
	docu.writeAttribute("DocumentInfoDate",m_Doc->documentInfo().date());
	docu.writeAttribute("DocumentInfoType",m_Doc->documentInfo().type());
	docu.writeAttribute("DocumentInfoFormat",m_Doc->documentInfo().format());
	docu.writeAttribute("DocumentInfoIdentifier",m_Doc->documentInfo().ident());
	docu.writeAttribute("DocumentInfoSource",m_Doc->documentInfo().source());
	docu.writeAttribute("DocumentInfoLanguage",m_Doc->documentInfo().langInfo());
	docu.writeAttribute("DocumentInfoRelation",m_Doc->documentInfo().relation());
	docu.writeAttribute("DocumentInfoCoverage",m_Doc->documentInfo().cover());
	docu.writeAttribute("DocumentInfoRights",m_Doc->documentInfo().rights());
	docu.writeAttribute("DocumentInfoContributors",m_Doc->documentInfo().contrib());
	docu.writeAttribute("DocumentInfoTitle",m_Doc->documentInfo().title());
	docu.writeAttribute("DocumentInfoSubject",m_Doc->documentInfo().subject());
	docu.writeAttribute("SuperScriptDisplacement", m_Doc->typographicPrefs().valueSuperScript);
	docu.writeAttribute("SuperScriptScaling", m_Doc->typographicPrefs().scalingSuperScript);
	docu.writeAttribute("SubScriptDisplacement", m_Doc->typographicPrefs().valueSubScript);
	docu.writeAttribute("SubScriptScaling", m_Doc->typographicPrefs().scalingSubScript);
	docu.writeAttribute("SmallCapsScaling", m_Doc->typographicPrefs().valueSmallCaps);
	docu.writeAttribute("BaselineGridSpacing",m_Doc->guidesPrefs().valueBaselineGrid);
	docu.writeAttribute("BaselineGridOffset", m_Doc->guidesPrefs().offsetBaselineGrid);
	docu.writeAttribute("AutoLineSpacingPct", m_Doc->typographicPrefs().autoLineSpacing);
	docu.writeAttribute("UnderlinePosition", m_Doc->typographicPrefs().valueUnderlinePos);
	docu.writeAttribute("UnderlineWidth", m_Doc->typographicPrefs().valueUnderlineWidth);
	docu.writeAttribute("StrikeThruPosition", m_Doc->typographicPrefs().valueStrikeThruPos);
	docu.writeAttribute("StrikeThruWidth", m_Doc->typographicPrefs().valueStrikeThruWidth);
	docu.writeAttribute("GroupCounter",m_Doc->GroupCounter); //Unnecessary?
	docu.writeAttribute("DocumentHasCMS", static_cast<int>(m_Doc->HasCMS)); //Unnecessary?
	docu.writeAttribute("SoftProofOn", static_cast<int>(m_Doc->cmsSettings().SoftProofOn));
	docu.writeAttribute("SoftProofFullOn", static_cast<int>(m_Doc->cmsSettings().SoftProofFullOn));
	docu.writeAttribute("ColorManagementActive", static_cast<int>(m_Doc->cmsSettings().CMSinUse));
	docu.writeAttribute("GamutCheck", static_cast<int>(m_Doc->cmsSettings().GamutCheck));
	docu.writeAttribute("BlackPoint", static_cast<int>(m_Doc->cmsSettings().BlackPoint));
	docu.writeAttribute("DefaultPrinterProfile",m_Doc->cmsSettings().DefaultPrinterProfile);
	docu.writeAttribute("DefaultImageRGBProfile",m_Doc->cmsSettings().DefaultImageRGBProfile);
	docu.writeAttribute("DefaultImageCMYKProfile",m_Doc->cmsSettings().DefaultImageCMYKProfile);
	docu.writeAttribute("DefaultSolidColorRGBProfile",m_Doc->cmsSettings().DefaultSolidColorRGBProfile);
	docu.writeAttribute("DefaultSolidColorCMYKProfile",m_Doc->cmsSettings().DefaultSolidColorCMYKProfile);
	docu.writeAttribute("DefaultIntentColors",m_Doc->cmsSettings().DefaultIntentColors);
	docu.writeAttribute("DefaultIntentImages",m_Doc->cmsSettings().DefaultIntentImages);
	docu.writeAttribute("ActiveLayer", m_Doc->activeLayer());
	docu.writeAttribute("Language", m_Doc->language());
	docu.writeAttribute("AutomaticHyphenation", static_cast<int>(m_Doc->hyphAutomatic()));
	docu.writeAttribute("AutomaticHyphenationCheck", static_cast<int>(m_Doc->hyphAutoCheck()));
	docu.writeAttribute("GuideLock", static_cast<int>(m_Doc->GuideLock));
	docu.writeAttribute("SnapToGuides", static_cast<int>(m_Doc->SnapGuides));
	docu.writeAttribute("SnapToGrid", static_cast<int>(m_Doc->SnapGrid));
	docu.writeAttribute("SnapToElement", static_cast<int>(m_Doc->SnapElement));
	docu.writeAttribute("MinorGridSpacing", m_Doc->guidesPrefs().minorGridSpacing);
	docu.writeAttribute("MajorGridSpacing", m_Doc->guidesPrefs().majorGridSpacing);
	docu.writeAttribute("ShowGrid", static_cast<int>(m_Doc->guidesPrefs().gridShown));
	docu.writeAttribute("ShowGuides", static_cast<int>(m_Doc->guidesPrefs().guidesShown));
	docu.writeAttribute("ShowColumnBorders", static_cast<int>(m_Doc->guidesPrefs().colBordersShown));
	if (m_Doc->drawAsPreview)
	{
		docu.writeAttribute("ShowFrames", static_cast<int>(m_View->storedFramesShown));
		docu.writeAttribute("ShowControls", static_cast<int>(m_View->storedShowControls));
	}
	else
	{
		docu.writeAttribute("ShowFrames", static_cast<int>(m_Doc->guidesPrefs().framesShown));
		docu.writeAttribute("ShowControls", static_cast<int>(m_Doc->guidesPrefs().showControls));
	}
	docu.writeAttribute("ShowLayerMarkers", static_cast<int>(m_Doc->guidesPrefs().layerMarkersShown));
	docu.writeAttribute("ShowMargins", static_cast<int>(m_Doc->guidesPrefs().marginsShown));
	docu.writeAttribute("ShowBaselineGrid", static_cast<int>(m_Doc->guidesPrefs().baselineGridShown));
	docu.writeAttribute("ShowImages", static_cast<int>(m_Doc->guidesPrefs().showPic));
	docu.writeAttribute("ShowLinks", static_cast<int>(m_Doc->guidesPrefs().linkShown));
	docu.writeAttribute("RulerMode", static_cast<int>(m_Doc->guidesPrefs().rulerMode));
	docu.writeAttribute("ShowRulers", static_cast<int>(m_Doc->guidesPrefs().rulersShown));
	docu.writeAttribute("ShowBleed", static_cast<int>(m_Doc->guidesPrefs().showBleed));
	docu.writeAttribute("RulerXOffset", m_Doc->rulerXoffset);
	docu.writeAttribute("RulerYOffset", m_Doc->rulerYoffset);
	docu.writeAttribute("GuideRadius", m_Doc->guidesPrefs().guideRad);
	docu.writeAttribute("GrabRadius",m_Doc->guidesPrefs().grabRadius);
	docu.writeAttribute("PolygonCorners", m_Doc->itemToolPrefs().polyCorners);
	docu.writeAttribute("PolygonFactor", m_Doc->itemToolPrefs().polyFactor);
	docu.writeAttribute("PolygonRotation", m_Doc->itemToolPrefs().polyRotation);
	docu.writeAttribute("PolygonInnerRotation", m_Doc->itemToolPrefs().polyInnerRot);
	docu.writeAttribute("PolygonCurvature", m_Doc->itemToolPrefs().polyCurvature);
	docu.writeAttribute("PolygonOuterCurvature", m_Doc->itemToolPrefs().polyOuterCurvature);
	docu.writeAttribute("PolygonUseFactor", static_cast<int>(m_Doc->itemToolPrefs().polyUseFactor));
	docu.writeAttribute("ArcStartAngle", m_Doc->itemToolPrefs().arcStartAngle);
	docu.writeAttribute("ArcSweepAngle", m_Doc->itemToolPrefs().arcSweepAngle);
	docu.writeAttribute("SpiralStartAngle", m_Doc->itemToolPrefs().spiralStartAngle);
	docu.writeAttribute("SpiralEndAngle", m_Doc->itemToolPrefs().spiralEndAngle);
	docu.writeAttribute("SpiralFactor", m_Doc->itemToolPrefs().spiralFactor);
	docu.writeAttribute("AutoSave", static_cast<int>(m_Doc->autoSave()));
	docu.writeAttribute("AutoSaveTime", m_Doc->autoSaveTime());
	docu.writeAttribute("AutoSaveCount", m_Doc->autoSaveCount());
	docu.writeAttribute("AutoSaveKeep", static_cast<int>(m_Doc->autoSaveKeep()));
	docu.writeAttribute("AutoSaveInDocumentDirectory", static_cast<int>(m_Doc->autoSaveInDocDir()));
	docu.writeAttribute("AutoSaveDirectory", m_Doc->autoSaveDir());
	docu.writeAttribute("ScratchBottom", m_Doc->scratch()->bottom());
	docu.writeAttribute("ScratchLeft", m_Doc->scratch()->left());
	docu.writeAttribute("ScratchRight", m_Doc->scratch()->right());
	docu.writeAttribute("ScratchTop", m_Doc->scratch()->top());
	docu.writeAttribute("GapHorizontal", m_Doc->pageGapHorizontal());
	docu.writeAttribute("GapVertical", m_Doc->pageGapVertical());
	docu.writeAttribute("LineStartArrow", m_Doc->itemToolPrefs().lineStartArrow);
	docu.writeAttribute("LineEndArrow", m_Doc->itemToolPrefs().lineEndArrow);
	docu.writeAttribute("ShapeLineColor",m_Doc->itemToolPrefs().shapeLineColor);
	docu.writeAttribute("ShapeFillColor",m_Doc->itemToolPrefs().shapeFillColor);
	docu.writeAttribute("LineColor",m_Doc->itemToolPrefs().lineColor);
	docu.writeAttribute("TextColor",m_Doc->itemToolPrefs().textColor);
	docu.writeAttribute("TextStrokeColor",m_Doc->itemToolPrefs().textStrokeColor);
	docu.writeAttribute("TextBackgroundColor", m_Doc->itemToolPrefs().textFillColor);
	docu.writeAttribute("TextLineColor", m_Doc->itemToolPrefs().textLineColor);
	docu.writeAttribute("TextBackgroundColorShade", m_Doc->itemToolPrefs().textFillColorShade);
	docu.writeAttribute("TextLineColorShade", m_Doc->itemToolPrefs().textLineColorShade);
	docu.writeAttribute("TextColorShade", m_Doc->itemToolPrefs().textShade);
	docu.writeAttribute("TextStrokeColorShade", m_Doc->itemToolPrefs().textStrokeShade);
	docu.writeAttribute("ShapeLineStyle",m_Doc->itemToolPrefs().shapeLineStyle);
	docu.writeAttribute("LineStyle",m_Doc->itemToolPrefs().lineStyle);
	docu.writeAttribute("ShapeLineWidth",m_Doc->itemToolPrefs().shapeLineWidth);
	docu.writeAttribute("LineWidth",m_Doc->itemToolPrefs().lineWidth);
	docu.writeAttribute("ShapeLineColorShade",m_Doc->itemToolPrefs().shapeLineColorShade);
	docu.writeAttribute("LineColorShade",m_Doc->itemToolPrefs().lineColorShade);
	docu.writeAttribute("ShapeFillColorShade",m_Doc->itemToolPrefs().shapeFillColorShade);
	docu.writeAttribute("ImageFillColor",m_Doc->itemToolPrefs().imageFillColor);
	docu.writeAttribute("ImageFillColorShade",m_Doc->itemToolPrefs().imageFillColorShade);
	docu.writeAttribute("ImageStrokeColor",m_Doc->itemToolPrefs().imageStrokeColor);
	docu.writeAttribute("ImageStrokeColorShade",m_Doc->itemToolPrefs().imageStrokeColorShade);
	docu.writeAttribute("ImageScaleX",m_Doc->itemToolPrefs().imageScaleX);
	docu.writeAttribute("ImageScaleY",m_Doc->itemToolPrefs().imageScaleY);
	docu.writeAttribute("ImageScaleType", static_cast<int>(m_Doc->itemToolPrefs().imageScaleType));
	docu.writeAttribute("ImageAspectRatio", static_cast<int>(m_Doc->itemToolPrefs().imageAspectRatio));
	docu.writeAttribute("ImageUseEmbeddedPath", static_cast<int>(m_Doc->itemToolPrefs().imageUseEmbeddedPath));
	docu.writeAttribute("ImageLowResType", m_Doc->itemToolPrefs().imageLowResType);
	docu.writeAttribute("DisplayOffsetX", m_Doc->opToolPrefs().dispX);
	docu.writeAttribute("DisplayOffsetY", m_Doc->opToolPrefs().dispY);
	docu.writeAttribute("RotationConstrainAngle", m_Doc->opToolPrefs().constrain);
	docu.writeAttribute("MinorGridColor",m_Doc->guidesPrefs().minorGridColor.name());
	docu.writeAttribute("MajorGridColor",m_Doc->guidesPrefs().majorGridColor.name());
	docu.writeAttribute("GuidesColor", m_Doc->guidesPrefs().guideColor.name());
	docu.writeAttribute("BaselineGridColor", m_Doc->guidesPrefs().baselineGridColor.name());
	QString renderStack;
	int renderStackCount = m_Doc->guidesPrefs().renderStackOrder.count();
	for (int r = 0; r < renderStackCount; r++)
	{
		renderStack += QString("%1 ").arg(m_Doc->guidesPrefs().renderStackOrder[r]);
	}
	docu.writeAttribute("RenderStack", renderStack.trimmed());
	docu.writeAttribute("GridType", m_Doc->guidesPrefs().gridType);
	docu.writeAttribute("PageColor",m_Doc->paperColor().name());
	docu.writeAttribute("MarginColor",m_Doc->guidesPrefs().marginColor.name());
	docu.writeAttribute("ShowMarginsFilled", static_cast<int>(m_Doc->marginColored()));
	docu.writeAttribute("currentProfile", m_Doc->curCheckProfile());
	docu.writeAttribute("CalligraphicPenFillColor", m_Doc->itemToolPrefs().calligraphicPenFillColor);
	docu.writeAttribute("CalligraphicPenLineColor", m_Doc->itemToolPrefs().calligraphicPenLineColor);
	docu.writeAttribute("CalligraphicPenFillColorShade", m_Doc->itemToolPrefs().calligraphicPenFillColorShade);
	docu.writeAttribute("CalligraphicPenLineColorShade", m_Doc->itemToolPrefs().calligraphicPenLineColorShade);
	docu.writeAttribute("CalligraphicPenLineWidth", m_Doc->itemToolPrefs().calligraphicPenLineWidth);
	docu.writeAttribute("CalligraphicPenAngle", m_Doc->itemToolPrefs().calligraphicPenAngle);
	docu.writeAttribute("CalligraphicPenWidth", m_Doc->itemToolPrefs().calligraphicPenWidth);
	docu.writeAttribute("CalligraphicPenStyle", m_Doc->itemToolPrefs().calligraphicPenStyle);

	writeCheckerProfiles(docu);
	writeJavascripts(docu);
	writeBookmarks(docu);
	writeColors(docu);
	writeGradients(docu);
	writeHyphenatorLists(docu);
	writeCharStyles(docu);
	writeParagraphStyles(docu);
	writeTableStyles(docu);
	writeCellStyles(docu);
	writeLineStyles(docu);
	writeArrowStyles(docu);
	writeLayers(docu);
	writePrintOptions(docu);
	writePdfOptions(docu);
	writeDocItemAttributes(docu);
	writeIndexes(docu);
	writeTOC(docu);
	writeMarks(docu);
	writeNotesStyles(docu);
	writeOpticalMarginSets(docu);
	writeNotesFrames(docu);
	writeNotes(docu);
	writePageSets(docu);
	writeSections(docu);
	writePatterns(docu, fileDir);
	writeContent(docu, fileDir);

	docu.writeEndElement();
	docu.writeEndDocument();

	bool  writeSucceed = false;
	const QFile* qFile = qobject_cast<QFile*>(outputFile.data());
	if (qFile)
		writeSucceed = (qFile->error() == QFile::NoError);
	else
		writeSucceed = true;
	outputFile->close();
	if (writeSucceed)
	{
		if (QFile::exists(fileName))
			writeSucceed = QFile::remove(fileName) ? QFile::rename(tmpFileName, fileName) : false;
		else
			writeSucceed = QFile::rename(tmpFileName, fileName);
		m_lastSavedFile = writeSucceed ? fileName : tmpFileName;
	}
	else if (QFile::exists(tmpFileName))
		QFile::remove(tmpFileName);
	if (writeSucceed)
		QFile::remove(tmpFileName);
#ifdef Q_OS_UNIX
	if (writeSucceed)
		QFile::setPermissions(fileName, m_Doc->filePermissions());
#endif
	return writeSucceed;
}

void Scribus171Format::writeCheckerProfiles(ScXmlStreamWriter & docu) const
{
	auto itcpend = m_Doc->checkerProfiles().end();
	for (auto itcp = m_Doc->checkerProfiles().begin(); itcp != itcpend; ++itcp)
	{
		docu.writeEmptyElement("CheckProfile");
		docu.writeAttribute("Name",itcp.key());
		docu.writeAttribute("IgnoreErrors", static_cast<int>(itcp.value().ignoreErrors));
		docu.writeAttribute("AutoCheck", static_cast<int>(itcp.value().autoCheck));
		docu.writeAttribute("CheckGlyphs", static_cast<int>(itcp.value().checkGlyphs));
		docu.writeAttribute("CheckOrphans", static_cast<int>(itcp.value().checkOrphans));
		docu.writeAttribute("CheckOverflow", static_cast<int>(itcp.value().checkOverflow));
		docu.writeAttribute("CheckPictures", static_cast<int>(itcp.value().checkPictures));
		docu.writeAttribute("CheckPartFilledImageFrames", static_cast<int>(itcp.value().checkPartFilledImageFrames));
		docu.writeAttribute("CheckResolution", static_cast<int>(itcp.value().checkResolution));
		docu.writeAttribute("CheckTransparency", static_cast<int>(itcp.value().checkTransparency));
		docu.writeAttribute("MinimumResolution",itcp.value().minResolution);
		docu.writeAttribute("MaximumResolution",itcp.value().maxResolution);
		docu.writeAttribute("CheckAnnotations", static_cast<int>(itcp.value().checkAnnotations));
		docu.writeAttribute("CheckRasterPDF", static_cast<int>(itcp.value().checkRasterPDF));
		docu.writeAttribute("CheckForGIF", static_cast<int>(itcp.value().checkForGIF));
		docu.writeAttribute("IgnoreOffLayers", static_cast<int>(itcp.value().ignoreOffLayers));
		docu.writeAttribute("CheckNotCMYKOrSpot", static_cast<int>(itcp.value().checkNotCMYKOrSpot));
		docu.writeAttribute("CheckDeviceColorsAndOutputIntent", static_cast<int>(itcp.value().checkDeviceColorsAndOutputIntent));
		docu.writeAttribute("CheckFontNotEmbedded", static_cast<int>(itcp.value().checkFontNotEmbedded));
		docu.writeAttribute("CheckFontIsOpenType", static_cast<int>(itcp.value().checkFontIsOpenType));
		docu.writeAttribute("CheckAppliedMasterDifferentSide", static_cast<int>(itcp.value().checkAppliedMasterDifferentSide));
		docu.writeAttribute("CheckEmptyTextFrames", static_cast<int>(itcp.value().checkEmptyTextFrames));
		docu.writeAttribute("CheckImageHasProgressiveEncoding", static_cast<int>(itcp.value().checkImageHasProgressiveEncoding));
	}
}

void Scribus171Format::writeLineStyles(ScXmlStreamWriter& docu) const
{
	QStringList styleNames = m_Doc->docLineStyles.keys();
	if (styleNames.isEmpty())
		return;
	writeLineStyles(docu, styleNames);
}

void Scribus171Format::writeLineStyles(ScXmlStreamWriter& docu, const QStringList& styleNames) const
{
	if (styleNames.isEmpty())
		return;

	for (auto itMU = m_Doc->docLineStyles.begin(); itMU != m_Doc->docLineStyles.end(); ++itMU)
	{
		if (!styleNames.contains(itMU.key()))
			continue;
		docu.writeStartElement("MultiLine");
		docu.writeAttribute("Name", itMU.key());
		const MultiLine& ml = itMU.value();
		for (const auto& singleLine : ml)
		{
			docu.writeEmptyElement("SubLine");
			docu.writeAttribute("Color", singleLine.Color);
			docu.writeAttribute("Shade", singleLine.Shade);
			docu.writeAttribute("Dash", singleLine.Dash);
			docu.writeAttribute("LineEnd", singleLine.LineEnd);
			docu.writeAttribute("LineJoin", singleLine.LineJoin);
			docu.writeAttribute("Width", singleLine.Width);
			docu.writeAttribute("Shortcut", ml.shortcut);
		}
		docu.writeEndElement();
	}
}

void Scribus171Format::writeArrowStyles(ScXmlStreamWriter& docu) const
{
	const QList<ArrowDesc>& arrowStyles = m_Doc->arrowStyles();
	for (const ArrowDesc& arrow : arrowStyles)
	{
		if (!arrow.userArrow)
			continue;
		docu.writeEmptyElement("Arrows");
		docu.writeAttribute("NumPoints", arrow.points.size());
		QString arp;
		double xa, ya;
		for (int nxx = 0; nxx < arrow.points.size(); ++nxx)
		{
			arrow.points.point(nxx, &xa, &ya);
			arp += QString::number(xa) + " " + QString::number(ya) + " ";
		}
		docu.writeAttribute("Points", arp);
		docu.writeAttribute("Name", arrow.name);
	}
}

void Scribus171Format::writeJavascripts(ScXmlStreamWriter & docu) const
{
	for (auto itja = m_Doc->JavaScripts.begin(); itja != m_Doc->JavaScripts.end(); ++itja)
	{
		docu.writeEmptyElement("Java");
		docu.writeAttribute("Name", itja.key());
		docu.writeAttribute("Script", itja.value());
	}
}

void Scribus171Format::writeBookmarks(ScXmlStreamWriter & docu) const
{	
	for (const auto& bookmark : m_Doc->BookMarks)
	{
		docu.writeEmptyElement("Bookmark");
		docu.writeAttribute("Title", bookmark.Title);
		docu.writeAttribute("Text", bookmark.Text);
		docu.writeAttribute("Action", bookmark.Action);
		docu.writeAttribute("ItemNumber", bookmark.ItemNr);
		docu.writeAttribute("Element", qHash(bookmark.PageObject) & 0x7FFFFFFF);
		docu.writeAttribute("First", bookmark.First);
		docu.writeAttribute("Last", bookmark.Last);
		docu.writeAttribute("Previous", bookmark.Prev);
		docu.writeAttribute("Next", bookmark.Next);
		docu.writeAttribute("Parent", bookmark.Parent);
	}
}

void Scribus171Format::writeColors(ScXmlStreamWriter & docu, bool part) const
{	
	ColorList usedColors;
	if (part)
		m_Doc->getUsedColors(usedColors);
	else
		usedColors = m_Doc->PageColors;
	writeColors(docu, usedColors.keys());
}

void Scribus171Format::writeColors(ScXmlStreamWriter& docu, const QStringList& colorNames) const
{
	for (const QString& colorName : colorNames)
	{
		if (colorName == CommonStrings::None)
			continue;

		const ScColor& color = m_Doc->PageColors[colorName];
		docu.writeEmptyElement("Color");
		docu.writeAttribute("Name", colorName);
		if (color.getColorModel() == colorModelRGB)
		{
			double r, g, b;
			color.getRGB(&r, &g, &b);
			docu.writeAttribute("Space", "RGB");
			docu.writeAttribute("R", r * 255.0);
			docu.writeAttribute("G", g * 255.0);
			docu.writeAttribute("B", b * 255.0);
		}
		else if (color.getColorModel() == colorModelCMYK)
		{
			double c, m, y, k;
			color.getCMYK(&c, &m, &y, &k);
			docu.writeAttribute("Space", "CMYK");
			docu.writeAttribute("C", c * 100.0);
			docu.writeAttribute("M", m * 100.0);
			docu.writeAttribute("Y", y * 100.0);
			docu.writeAttribute("K", k * 100.0);
		}
		else
		{
			double L, a, b;
			color.getLab(&L, &a, &b);
			docu.writeAttribute("Space", "Lab");
			docu.writeAttribute("L", L);
			docu.writeAttribute("A", a);
			docu.writeAttribute("B", b);
		}
		if (color.isSpotColor())
			docu.writeAttribute("Spot", static_cast<int>(color.isSpotColor()));
		if (color.isRegistrationColor())
			docu.writeAttribute("Register", static_cast<int>(color.isRegistrationColor()));
	}
}

void Scribus171Format::writeGradients(ScXmlStreamWriter& docu, bool part) const
{
	QHash<QString, VGradient> gradMap;
	if (part)
		m_Doc->getUsedGradients(gradMap);
	else
		gradMap = m_Doc->docGradients;

	QStringList gradKeys = gradMap.keys();
	gradKeys.sort();
	writeGradients(docu, gradKeys);
}

void Scribus171Format::writeGradients(ScXmlStreamWriter & docu, const QStringList& gradientNames) const
{
	for (const QString& gradientName : gradientNames)
	{
		VGradient gradient = m_Doc->docGradients[gradientName];
		docu.writeStartElement("Gradient");
		docu.writeAttribute("Name", gradientName);
		docu.writeAttribute("Ext", gradient.repeatMethod());
		const QList<VColorStop*>& cstops = gradient.colorStops();
		for (int cst = 0; cst < gradient.stops(); ++cst)
		{
			docu.writeEmptyElement("ColorStop");
			docu.writeAttribute("Ramp", cstops.at(cst)->rampPoint);
			docu.writeAttribute("Name", cstops.at(cst)->name);
			docu.writeAttribute("Shade", cstops.at(cst)->shade);
			docu.writeAttribute("Opacity", cstops.at(cst)->opacity);
		}
		docu.writeEndElement();
	}
}

void Scribus171Format::writeHyphenatorLists(ScXmlStreamWriter& docu) const
{
	const auto& hyphenatorPrefs = m_Doc->hyphenatorPrefs();

	docu.writeStartElement("Hyphenator");
	for (auto hyit = hyphenatorPrefs.specialWords.begin(); hyit != hyphenatorPrefs.specialWords.end(); ++hyit)
	{
		docu.writeEmptyElement("Exception");
		docu.writeAttribute("Word", hyit.key());
		docu.writeAttribute("Hyphenated", hyit.value());
	}
	for (const auto& ignoredWord : hyphenatorPrefs.ignoredWords)
	{
		docu.writeEmptyElement("Ignore");
		docu.writeAttribute("Word", ignoredWord);
	}
	docu.writeEndElement();
}

void Scribus171Format::writeParagraphStyles(ScXmlStreamWriter & docu) const
{
	QList<int> styleList = m_Doc->getSortedStyleList();
	for (int i = 0; i < styleList.count(); ++i)
	{
		putPStyle(docu, m_Doc->paragraphStyles()[styleList[i]], "ParagraphStyle");
	}
}

void Scribus171Format::putPStyle(ScXmlStreamWriter & docu, const ParagraphStyle & style, const QString &nodeName) const
{
	bool styleHasTabs = (!style.isInhTabValues() && !style.tabValues().isEmpty());
	if (styleHasTabs)
		docu.writeStartElement(nodeName);
	else
		docu.writeEmptyElement(nodeName);
	if (!style.name().isEmpty() )
		docu.writeAttribute("Name", style.name());
	if (!style.parent().isEmpty())
		docu.writeAttribute("Parent", style.parent());
	if ( style.isDefaultStyle())
		docu.writeAttribute("DefaultStyle", style.isDefaultStyle());

	if (!style.isInhAlignment())
		docu.writeAttribute("Alignment", style.alignment());
	if (!style.isInhDirection())
		docu.writeAttribute("Direction", style.direction());
	if (!style.isInhLineSpacingMode())
		docu.writeAttribute("LineSpacingMode", style.lineSpacingMode());
	if (!style.isInhLineSpacing())
		docu.writeAttribute("LineSpacing", style.lineSpacing());
	if (!style.isInhLeftMargin())
		docu.writeAttribute("LeftMargin", style.leftMargin());
	if (!style.isInhRightMargin())
		docu.writeAttribute("RightMargin", style.rightMargin());
	if (!style.isInhFirstIndent())
		docu.writeAttribute("FirstIndent", style.firstIndent());
	if (!style.isInhGapBefore())
		docu.writeAttribute("GapBefore", style.gapBefore());
	if (!style.isInhGapAfter())
		docu.writeAttribute("GapAfter", style.gapAfter());
	if (!style.isInhPeCharStyleName())
		docu.writeAttribute("ParagraphEffectCharStyle", style.peCharStyleName());
	if (!style.isInhParEffectOffset())
		docu.writeAttribute("ParagraphEffectOffset", style.parEffectOffset());
	if (!style.isInhParEffectIndent())
		docu.writeAttribute("ParagraphEffectIndent", static_cast<int>(style.parEffectIndent()));
	if (!style.isInhHasDropCap())
		docu.writeAttribute("HasDropCap", static_cast<int>(style.hasDropCap()));
	if (!style.isInhDropCapLines())
		docu.writeAttribute("DropCapLines", style.dropCapLines());
	if (!style.isInhHasBullet())
		docu.writeAttribute("Bullet", static_cast<int>(style.hasBullet()));
	if (!style.isInhBulletStr())
		docu.writeAttribute("BulletStr", style.bulletStr());
	if (!style.isInhHasNum())
		docu.writeAttribute("Numeration", static_cast<int>(style.hasNum()));
	if (!style.isInhNumFormat())
		docu.writeAttribute("NumerationFormat", style.numFormat());
	if (!style.isInhNumName())
		docu.writeAttribute("NumerationName", style.numName());
	if (!style.isInhNumLevel())
		docu.writeAttribute("NumerationLevel", style.numLevel());
	if (!style.isInhNumPrefix())
		docu.writeAttribute("NumerationPrefix", style.numPrefix());
	if (!style.isInhNumSuffix())
		docu.writeAttribute("NumerationSuffix", style.numSuffix());
	if (!style.isInhNumStart())
		docu.writeAttribute("NumerationStart", style.numStart());
	if (!style.isInhNumRestart())
		docu.writeAttribute("NumerationRestart", style.numRestart());
	if (!style.isInhNumOther())
		docu.writeAttribute("NumerationOther", static_cast<int>(style.numOther()));
	if (!style.isInhNumHigher())
		docu.writeAttribute("NumerationHigher", static_cast<int>(style.numHigher()));
	if (!style.isInhOpticalMargins())
		docu.writeAttribute("OpticalMargins", style.opticalMargins());
	if (!style.isInhOpticalMarginSetId())
		docu.writeAttribute("OpticalMarginSetId", style.opticalMarginSetId());
	if (!style.isInhHyphenConsecutiveLines())
		docu.writeAttribute("HyphenConsecutiveLines", style.hyphenConsecutiveLines());
	if (!style.isInhHyphenationMode())
		docu.writeAttribute("HyphenationMode", style.hyphenationMode());
	if (!style.isInhMinWordTracking())
		docu.writeAttribute("MinWordTrack", style.minWordTracking());
	if (!style.isInhMinGlyphExtension())
		docu.writeAttribute("MinGlyphShrink", style.minGlyphExtension());
	if (!style.isInhMaxGlyphExtension())
		docu.writeAttribute("MaxGlyphExtend", style.maxGlyphExtension());
	if (!style.isInhKeepLinesStart())
		docu.writeAttribute("KeepLinesStart", style.keepLinesStart());
	if (!style.isInhKeepLinesEnd())
		docu.writeAttribute("KeepLinesEnd", style.keepLinesEnd());
	if (!style.isInhKeepWithNext())
		docu.writeAttribute("KeepWithNext", style.keepWithNext());
	if (!style.isInhKeepTogether())
		docu.writeAttribute("KeepTogether", style.keepTogether());
	if (!style.isInhBackgroundColor())
		docu.writeAttribute("ParagraphBackgroundColor", style.backgroundColor());
	if (!style.isInhBackgroundShade())
		docu.writeAttribute("ParagraphBackgroundColorShade", style.backgroundShade());

	if (!style.shortcut().isEmpty() )
		docu.writeAttribute("ParagraphStyleShortcut", style.shortcut()); // shortcuts won't be inherited

	putCStyle(docu, style.charStyle());

	if (!style.isInhTabValues())
	{
		for (int a = 0; a < style.tabValues().count(); ++a)
		{
			docu.writeEmptyElement("Tabs");
			docu.writeAttribute("Type", (style.tabValues().at(a)).tabType);
			docu.writeAttribute("Pos", (style.tabValues().at(a)).tabPosition);
			QString tabCh;
			if (!(style.tabValues().at(a)).tabFillChar.isNull())
				tabCh = QString((style.tabValues().at(a)).tabFillChar);
			docu.writeAttribute("Fill", tabCh);
		}
	}

	if (styleHasTabs)
		docu.writeEndElement();
}


void Scribus171Format::writeCharStyles(ScXmlStreamWriter & docu) const
{
	QList<int> styleList = m_Doc->getSortedCharStyleList();
	for (int i = 0; i < styleList.count(); ++i)
	{
		docu.writeStartElement("CharacterStyle");
		putNamedCStyle(docu, m_Doc->charStyles()[styleList[i]]);
		docu.writeEndElement();
	}
}

void Scribus171Format::putCStyle(ScXmlStreamWriter & docu, const CharStyle & style) const
{
	if (!style.parent().isEmpty() )
		docu.writeAttribute("CParent", style.parent());
	if (!style.isInhFont())
		docu.writeAttribute("Font", style.font().scName());
	if (!style.isInhFontSize())
		docu.writeAttribute("FontSize", style.fontSize() / 10.0);
	if (!style.isInhFontFeatures())
		docu.writeAttribute("FontFeatures", style.fontFeatures());
	if (!style.isInhFeatures())
		docu.writeAttribute("Features", style.features().join(" "));
	if (!style.isInhFillColor())
		docu.writeAttribute("FontColor", style.fillColor());
	if (!style.isInhFillShade())
		docu.writeAttribute("FillShade", style.fillShade());
	if (!style.isInhHyphenChar())
		docu.writeAttribute("HyphenCharacter", style.hyphenChar());
	if (!style.isInhHyphenWordMin())
		docu.writeAttribute("HyphenWordMinimum", style.hyphenWordMin());
	if (!style.isInhStrokeColor())
		docu.writeAttribute("StrokeColor", style.strokeColor());
	if (!style.isInhBackColor())
		docu.writeAttribute("BackgroundColor", style.backColor());
	if (!style.isInhBackShade())
		docu.writeAttribute("BackgroundShade", style.backShade());
	if (!style.isInhStrokeShade())
		docu.writeAttribute("StrokeShade", style.strokeShade());
	if (!style.isInhShadowXOffset())
		docu.writeAttribute("TextShadowXOffset", style.shadowXOffset() / 10.0);
	if (!style.isInhShadowYOffset())
		docu.writeAttribute("TextShadowYOffset", style.shadowYOffset() / 10.0);
	if (!style.isInhOutlineWidth())
		docu.writeAttribute("TextOutlineWidth", style.outlineWidth() / 10.0);
	if (!style.isInhUnderlineOffset())
		docu.writeAttribute("TextUnderlineOffset", style.underlineOffset() / 10.0);
	if (!style.isInhUnderlineWidth())
		docu.writeAttribute("TextUnderlineWidth", style.underlineWidth() / 10.0);
	if (!style.isInhStrikethruOffset())
		docu.writeAttribute("TextStrikeThroughOffset", style.strikethruOffset() / 10.0);
	if (!style.isInhStrikethruWidth())
		docu.writeAttribute("TextStrikeThroughWidth", style.strikethruWidth() / 10.0);
	if (!style.isInhScaleH())
		docu.writeAttribute("ScaleHorizontal", style.scaleH() / 10.0);
	if (!style.isInhScaleV())
		docu.writeAttribute("ScaleVertical", style.scaleV() / 10.0);
	if (!style.isInhBaselineOffset())
		docu.writeAttribute("BaselineOffset", style.baselineOffset() / 10.0);
	if (!style.isInhTracking())
		docu.writeAttribute("Kerning", style.tracking() / 10.0);
	if (!style.isInhWordTracking())
		docu.writeAttribute("WordTrack", style.wordTracking());
	if (!style.isInhLanguage())
		docu.writeAttribute("Language", style.language());
	if (!style.shortcut().isEmpty() )
		docu.writeAttribute("Shortcut", style.shortcut()); // shortcuts won't be inherited
}

void Scribus171Format::putNamedCStyle(ScXmlStreamWriter& docu, const CharStyle & style) const
{
	if (!style.name().isEmpty() )
		docu.writeAttribute("Name", style.name());
	if ( style.hasName() && style.isDefaultStyle())
		docu.writeAttribute("DefaultStyle", style.isDefaultStyle());
	putCStyle(docu, style);
}

void Scribus171Format::writeTableStyles(ScXmlStreamWriter& docu) const
{
	QList<int> styleList = m_Doc->getSortedTableStyleList();
	for (int i = 0; i < styleList.count(); ++i)
	{
		docu.writeStartElement("TableStyle");
		putTableStyle(docu, m_Doc->tableStyles()[styleList[i]]);
		docu.writeEndElement();
	}
}

void Scribus171Format::writeCellStyles(ScXmlStreamWriter& docu) const
{
	QList<int> styleList = m_Doc->getSortedCellStyleList();
	for (int i = 0; i < styleList.count(); ++i)
	{
		docu.writeStartElement("CellStyle");
		putCellStyle(docu, m_Doc->cellStyles()[styleList[i]]);
		docu.writeEndElement();
	}
}

void Scribus171Format::putTableStyle(ScXmlStreamWriter &docu, const TableStyle &style) const
{
	if (!style.name().isEmpty() )
		docu.writeAttribute("Name", style.name());
	if ( style.hasName() && style.isDefaultStyle())
		docu.writeAttribute("DefaultStyle", style.isDefaultStyle());
	if (!style.parent().isEmpty() )
		docu.writeAttribute("Parent", style.parent());
	if (!style.isInhFillColor())
		docu.writeAttribute("FillColor", style.fillColor());
	if (!style.isInhFillShade())
		docu.writeAttribute("FillShade", style.fillShade());
	if (!style.isInhLeftBorder())
	{
		const TableBorder& tbLeft = style.leftBorder();
		docu.writeStartElement("TableBorderLeft");
		for (const TableBorderLine& tbl : tbLeft.borderLines())
		{
			docu.writeStartElement("TableBorderLine");
			docu.writeAttribute("Width", tbl.width());
			docu.writeAttribute("PenStyle", tbl.style());
			docu.writeAttribute("Color", tbl.color());
			docu.writeAttribute("Shade", tbl.shade());
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
	if (!style.isInhRightBorder())
	{
		const TableBorder& tbRight = style.rightBorder();
		docu.writeStartElement("TableBorderRight");
		for (const TableBorderLine& tbl : tbRight.borderLines())
		{
			docu.writeStartElement("TableBorderLine");
			docu.writeAttribute("Width", tbl.width());
			docu.writeAttribute("PenStyle", tbl.style());
			docu.writeAttribute("Color", tbl.color());
			docu.writeAttribute("Shade", tbl.shade());
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
	if (!style.isInhTopBorder())
	{
		const TableBorder& tbTop = style.topBorder();
		docu.writeStartElement("TableBorderTop");
		for (const TableBorderLine& tbl : tbTop.borderLines())
		{
			docu.writeStartElement("TableBorderLine");
			docu.writeAttribute("Width", tbl.width());
			docu.writeAttribute("PenStyle", tbl.style());
			docu.writeAttribute("Color", tbl.color());
			docu.writeAttribute("Shade", tbl.shade());
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
	if (!style.isInhBottomBorder())
	{
		const TableBorder& tbBottom = style.bottomBorder();
		docu.writeStartElement("TableBorderBottom");
		for (const TableBorderLine& tbl : tbBottom.borderLines())
		{
			docu.writeStartElement("TableBorderLine");
			docu.writeAttribute("Width", tbl.width());
			docu.writeAttribute("PenStyle", tbl.style());
			docu.writeAttribute("Color", tbl.color());
			docu.writeAttribute("Shade", tbl.shade());
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
}

void Scribus171Format::putCellStyle(ScXmlStreamWriter &docu, const CellStyle &style) const
{
	if (!style.name().isEmpty() )
		docu.writeAttribute("Name", style.name());
	if ( style.hasName() && style.isDefaultStyle())
		docu.writeAttribute("DefaultStyle", style.isDefaultStyle());
	if (!style.parent().isEmpty() )
		docu.writeAttribute("Parent", style.parent());
	if (!style.isInhFillColor())
		docu.writeAttribute("FillColor", style.fillColor());
	if (!style.isInhFillShade())
		docu.writeAttribute("FillShade", style.fillShade());
	if (!style.isInhLeftPadding())
		docu.writeAttribute("LeftPadding",style.leftPadding());
	if (!style.isInhRightPadding())
		docu.writeAttribute("RightPadding", style.rightPadding());
	if (!style.isInhTopPadding())
		docu.writeAttribute("TopPadding",style.topPadding());
	if (!style.isInhBottomPadding())
		docu.writeAttribute("BottomPadding", style.bottomPadding());
	if (!style.isInhLeftBorder())
	{
		const TableBorder& tbLeft = style.leftBorder();
		docu.writeStartElement("TableBorderLeft");
		for (const TableBorderLine& tbl : tbLeft.borderLines())
		{
			docu.writeStartElement("TableBorderLine");
			docu.writeAttribute("Width", tbl.width());
			docu.writeAttribute("PenStyle", tbl.style());
			docu.writeAttribute("Color", tbl.color());
			docu.writeAttribute("Shade", tbl.shade());
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
	if (!style.isInhRightBorder())
	{
		const TableBorder& tbRight = style.rightBorder();
		docu.writeStartElement("TableBorderRight");
		for (const TableBorderLine& tbl : tbRight.borderLines())
		{
			docu.writeStartElement("TableBorderLine");
			docu.writeAttribute("Width", tbl.width());
			docu.writeAttribute("PenStyle", tbl.style());
			docu.writeAttribute("Color", tbl.color());
			docu.writeAttribute("Shade", tbl.shade());
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
	if (!style.isInhTopBorder())
	{
		const TableBorder& tbTop = style.topBorder();
		docu.writeStartElement("TableBorderTop");
		for (const TableBorderLine& tbl : tbTop.borderLines())
		{
			docu.writeStartElement("TableBorderLine");
			docu.writeAttribute("Width", tbl.width());
			docu.writeAttribute("PenStyle", tbl.style());
			docu.writeAttribute("Color", tbl.color());
			docu.writeAttribute("Shade", tbl.shade());
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
	if (!style.isInhBottomBorder())
	{
		const TableBorder& tbBottom = style.bottomBorder();
		docu.writeStartElement("TableBorderBottom");
		for (const TableBorderLine& tbl : tbBottom.borderLines())
		{
			docu.writeStartElement("TableBorderLine");
			docu.writeAttribute("Width", tbl.width());
			docu.writeAttribute("PenStyle", tbl.style());
			docu.writeAttribute("Color", tbl.color());
			docu.writeAttribute("Shade", tbl.shade());
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
}

void Scribus171Format::writeLayers(ScXmlStreamWriter & docu) const
{	
	uint layerCount = m_Doc->layerCount();
	for (uint lay = 0; lay < layerCount; ++lay)
	{
		docu.writeEmptyElement("Layers");
		docu.writeAttribute("Number",m_Doc->Layers[lay].ID);
		docu.writeAttribute("Level",m_Doc->Layers[lay].Level);
		docu.writeAttribute("Name",m_Doc->Layers[lay].Name);
		docu.writeAttribute("IsViewable", static_cast<int>(m_Doc->Layers[lay].isViewable));
		docu.writeAttribute("IsPrintable", static_cast<int>(m_Doc->Layers[lay].isPrintable));
		docu.writeAttribute("IsEditable", static_cast<int>(m_Doc->Layers[lay].isEditable));
		docu.writeAttribute("IsSelectable", static_cast<int>(m_Doc->Layers[lay].isSelectable));
		docu.writeAttribute("FlowControl", static_cast<int>(m_Doc->Layers[lay].flowControl));
		docu.writeAttribute("Transparency", m_Doc->Layers[lay].transparency);
		docu.writeAttribute("BlendMode", m_Doc->Layers[lay].blendMode);
		docu.writeAttribute("OutlineMode", static_cast<int>(m_Doc->Layers[lay].outlineMode));
		docu.writeAttribute("LayerColor",m_Doc->Layers[lay].markerColor.name());
	}
}

void Scribus171Format::writePrintOptions(ScXmlStreamWriter & docu) const
{
	docu.writeStartElement("Printer");
	docu.writeAttribute("firstUse", static_cast<int>(m_Doc->Print_Options.firstUse));
	docu.writeAttribute("toFile", static_cast<int>(m_Doc->Print_Options.toFile));
	docu.writeAttribute("useAltPrintCommand", static_cast<int>(m_Doc->Print_Options.useAltPrintCommand));
	docu.writeAttribute("outputSeparations", static_cast<int>(m_Doc->Print_Options.outputSeparations));
	docu.writeAttribute("useSpotColors", static_cast<int>(m_Doc->Print_Options.useSpotColors));
	docu.writeAttribute("useColor", static_cast<int>(m_Doc->Print_Options.useColor));
	docu.writeAttribute("mirrorH", static_cast<int>(m_Doc->Print_Options.mirrorH));
	docu.writeAttribute("mirrorV", static_cast<int>(m_Doc->Print_Options.mirrorV));
	docu.writeAttribute("useICC", static_cast<int>(m_Doc->cmsSettings().CMSinUse)); // Not used anymore, only for backward compatibility
	docu.writeAttribute("doGCR", static_cast<int>(m_Doc->Print_Options.doGCR));
	docu.writeAttribute("doClip", static_cast<int>(m_Doc->Print_Options.doClip));
	docu.writeAttribute("setDevParam", static_cast<int>(m_Doc->Print_Options.setDevParam));
	docu.writeAttribute("useDocBleeds", static_cast<int>(m_Doc->Print_Options.useDocBleeds));
	docu.writeAttribute("cropMarks", static_cast<int>(m_Doc->Print_Options.cropMarks));
	docu.writeAttribute("bleedMarks", static_cast<int>(m_Doc->Print_Options.bleedMarks));
	docu.writeAttribute("registrationMarks", static_cast<int>(m_Doc->Print_Options.registrationMarks));
	docu.writeAttribute("colorMarks", static_cast<int>(m_Doc->Print_Options.colorMarks));
	docu.writeAttribute("includePDFMarks", static_cast<int>(m_Doc->Print_Options.includePDFMarks));
	docu.writeAttribute("PSLevel", (int) ((m_Doc->Print_Options.prnLanguage < PrintLanguage::WindowsGDI) ? m_Doc->Print_Options.prnLanguage : PrintLanguage::PostScript3));
	docu.writeAttribute("PrintEngine", (int) m_Doc->Print_Options.prnLanguage);
	docu.writeAttribute("markLength", m_Doc->Print_Options.markLength);
	docu.writeAttribute("markOffset", m_Doc->Print_Options.markOffset);
	docu.writeAttribute("BleedTop", m_Doc->Print_Options.bleeds.top());
	docu.writeAttribute("BleedLeft", m_Doc->Print_Options.bleeds.left());
	docu.writeAttribute("BleedRight", m_Doc->Print_Options.bleeds.right());
	docu.writeAttribute("BleedBottom", m_Doc->Print_Options.bleeds.bottom());
	docu.writeAttribute("printer", m_Doc->Print_Options.printer);
	docu.writeAttribute("filename", m_Doc->Print_Options.filename);
	docu.writeAttribute("separationName", m_Doc->Print_Options.separationName);
	docu.writeAttribute("printerCommand", m_Doc->Print_Options.printerCommand);
	for (int i = 0; i < m_Doc->Print_Options.allSeparations.count(); ++i)
	{
		docu.writeEmptyElement("Separation");
		docu.writeAttribute("Name", m_Doc->Print_Options.allSeparations[i]);
	}
	docu.writeEndElement();
}

void Scribus171Format::writePdfOptions(ScXmlStreamWriter & docu) const
{
	docu.writeStartElement("PDF");
	docu.writeAttribute("firstUse", static_cast<int>(m_Doc->pdfOptions().firstUse));
	docu.writeAttribute("Thumbnails", static_cast<int>(m_Doc->pdfOptions().Thumbnails));
	docu.writeAttribute("Articles", static_cast<int>(m_Doc->pdfOptions().Articles));
	docu.writeAttribute("Bookmarks", static_cast<int>(m_Doc->pdfOptions().Bookmarks));
	docu.writeAttribute("Compress", static_cast<int>(m_Doc->pdfOptions().Compress));
	docu.writeAttribute("CMethod", m_Doc->pdfOptions().CompressMethod);
	docu.writeAttribute("Quality", m_Doc->pdfOptions().Quality);
	docu.writeAttribute("EmbedPDF", static_cast<int>(m_Doc->pdfOptions().embedPDF));
	docu.writeAttribute("MirrorH", static_cast<int>(m_Doc->pdfOptions().MirrorH));
	docu.writeAttribute("MirrorV", static_cast<int>(m_Doc->pdfOptions().MirrorV));
	docu.writeAttribute("Clip", static_cast<int>(m_Doc->pdfOptions().doClip));
	docu.writeAttribute("rangeSel", m_Doc->pdfOptions().pageRangeSelection);
	docu.writeAttribute("rangeTxt", m_Doc->pdfOptions().pageRangeString);
	docu.writeAttribute("RotateDeg", m_Doc->pdfOptions().RotateDeg);
	docu.writeAttribute("PresentMode", static_cast<int>(m_Doc->pdfOptions().PresentMode));
	docu.writeAttribute("RecalcPic", static_cast<int>(m_Doc->pdfOptions().RecalcPic));
	docu.writeAttribute("FontEmbedding", static_cast<int>(m_Doc->pdfOptions().FontEmbedding));
	docu.writeAttribute("Grayscale", static_cast<int>(m_Doc->pdfOptions().isGrayscale));
	docu.writeAttribute("RGBMode", static_cast<int>(m_Doc->pdfOptions().UseRGB));
	docu.writeAttribute("UseProfiles", static_cast<int>(m_Doc->pdfOptions().UseProfiles));
	docu.writeAttribute("UseProfiles2", static_cast<int>(m_Doc->pdfOptions().UseProfiles2));
	docu.writeAttribute("Binding", m_Doc->pdfOptions().Binding);
	docu.writeAttribute("PicRes", m_Doc->pdfOptions().PicRes);
	docu.writeAttribute("Resolution", m_Doc->pdfOptions().Resolution);
	docu.writeAttribute("Version", m_Doc->pdfOptions().Version);
	docu.writeAttribute("Intent", m_Doc->pdfOptions().Intent);
	docu.writeAttribute("Intent2", m_Doc->pdfOptions().Intent2);
	docu.writeAttribute("SolidP", m_Doc->pdfOptions().SolidProf);
	docu.writeAttribute("ImageP", m_Doc->pdfOptions().ImageProf);
	docu.writeAttribute("PrintP", m_Doc->pdfOptions().PrintProf);
	docu.writeAttribute("InfoString", m_Doc->pdfOptions().Info);
	docu.writeAttribute("BTop", m_Doc->pdfOptions().bleeds.top());
	docu.writeAttribute("BLeft", m_Doc->pdfOptions().bleeds.left());
	docu.writeAttribute("BRight", m_Doc->pdfOptions().bleeds.right());
	docu.writeAttribute("BBottom", m_Doc->pdfOptions().bleeds.bottom());
	docu.writeAttribute("useDocBleeds", static_cast<int>(m_Doc->pdfOptions().useDocBleeds));
	docu.writeAttribute("cropMarks", static_cast<int>(m_Doc->pdfOptions().cropMarks));
	docu.writeAttribute("bleedMarks", static_cast<int>(m_Doc->pdfOptions().bleedMarks));
	docu.writeAttribute("registrationMarks", static_cast<int>(m_Doc->pdfOptions().registrationMarks));
	docu.writeAttribute("colorMarks", static_cast<int>(m_Doc->pdfOptions().colorMarks));
	docu.writeAttribute("docInfoMarks", static_cast<int>(m_Doc->pdfOptions().docInfoMarks));
	docu.writeAttribute("markLength", m_Doc->pdfOptions().markLength);
	docu.writeAttribute("markOffset", m_Doc->pdfOptions().markOffset);
	docu.writeAttribute("ImagePr", static_cast<int>(m_Doc->pdfOptions().EmbeddedI));
	docu.writeAttribute("PassOwner", m_Doc->pdfOptions().PassOwner);
	docu.writeAttribute("PassUser", m_Doc->pdfOptions().PassUser);
	docu.writeAttribute("Permissions", m_Doc->pdfOptions().Permissions);
	docu.writeAttribute("Encrypt", static_cast<int>(m_Doc->pdfOptions().Encrypt));
	docu.writeAttribute("UseLayers", static_cast<int>(m_Doc->pdfOptions().useLayers));
	docu.writeAttribute("UseLpi", static_cast<int>(m_Doc->pdfOptions().UseLPI));
	docu.writeAttribute("UseSpotColors", static_cast<int>(m_Doc->pdfOptions().UseSpotColors));
	docu.writeAttribute("doMultiFile", static_cast<int>(m_Doc->pdfOptions().doMultiFile));
	docu.writeAttribute("displayBookmarks", static_cast<int>(m_Doc->pdfOptions().displayBookmarks));
	docu.writeAttribute("displayFullscreen", static_cast<int>(m_Doc->pdfOptions().displayFullscreen));
	docu.writeAttribute("displayLayers", static_cast<int>(m_Doc->pdfOptions().displayLayers));
	docu.writeAttribute("displayThumbs", static_cast<int>(m_Doc->pdfOptions().displayThumbs));
	docu.writeAttribute("hideMenuBar", static_cast<int>(m_Doc->pdfOptions().hideMenuBar));
	docu.writeAttribute("hideToolBar", static_cast<int>(m_Doc->pdfOptions().hideToolBar));
	docu.writeAttribute("fitWindow", static_cast<int>(m_Doc->pdfOptions().fitWindow));
	docu.writeAttribute("openAfterExport", static_cast<int>(m_Doc->pdfOptions().openAfterExport));
	docu.writeAttribute("PageLayout", m_Doc->pdfOptions().PageLayout);
	docu.writeAttribute("openAction", m_Doc->pdfOptions().openAction);

	const QList<QString>& embedList = m_Doc->pdfOptions().EmbedList;
	for (int pdoF = 0; pdoF < embedList.count(); ++pdoF)
	{
		docu.writeEmptyElement("Fonts");
		docu.writeAttribute("Name", embedList.at(pdoF));
	}

	const QList<QString>& subsetList = m_Doc->pdfOptions().SubsetList;
	for (int pdoS = 0; pdoS < subsetList.count(); ++pdoS)
	{
		docu.writeEmptyElement("Subset");
		docu.writeAttribute("Name", subsetList.at(pdoS));
	}

	for (auto itlp = m_Doc->pdfOptions().LPISettings.begin(); itlp != m_Doc->pdfOptions().LPISettings.end(); ++itlp)
	{
		docu.writeEmptyElement("LPI");
		docu.writeAttribute("Color", itlp.key());
		docu.writeAttribute("Frequency", itlp.value().Frequency);
		docu.writeAttribute("Angle", itlp.value().Angle);
		docu.writeAttribute("SpotFunction", itlp.value().SpotFunc);
	}
	docu.writeEndElement();
}

void Scribus171Format::writeDocItemAttributes(ScXmlStreamWriter & docu) const
{
	docu.writeStartElement("DocItemAttributes");
	for (const auto& objAttribute : m_Doc->itemAttributes())
	{
		docu.writeEmptyElement("ItemAttribute");
		docu.writeAttribute("Name", objAttribute.name);
		docu.writeAttribute("Type", objAttribute.type);
		docu.writeAttribute("Value", objAttribute.value);
		docu.writeAttribute("Parameter", objAttribute.parameter);
		docu.writeAttribute("Relationship", objAttribute.relationship);
		docu.writeAttribute("RelationshipTo", objAttribute.relationshipto);
		docu.writeAttribute("AutoAddTo", objAttribute.autoaddto);
	}
	docu.writeEndElement();
}

void Scribus171Format::writeIndexes(ScXmlStreamWriter & docu) const
{
	docu.writeStartElement("Indexes");
	for (auto indexSetupIt = m_Doc->indexSetups().begin() ; indexSetupIt != m_Doc->indexSetups().end(); ++indexSetupIt )
	{
		docu.writeStartElement("Index");
		docu.writeAttribute("Name", indexSetupIt->name);
		docu.writeAttribute("FrameName", indexSetupIt->frameName);
		docu.writeAttribute("ListNonPrinting", indexSetupIt->listNonPrintingFrames);
		docu.writeAttribute("CombineIdenticalEntries", indexSetupIt->combineIdenticalEntries);
		docu.writeAttribute("CaseSensitiveCombination", indexSetupIt->caseSensitiveCombination);
		docu.writeAttribute("AutoCapitalizeEntries", indexSetupIt->autoCapitalizeEntries);
		docu.writeAttribute("AddAlphaSeparators", indexSetupIt->addAlphaSeparators);
		docu.writeAttribute("HeadingStyle", indexSetupIt->headingStyle);
		docu.writeAttribute("SeparatorStyle", indexSetupIt->separatorStyle);
		docu.writeAttribute("Level1Style", indexSetupIt->level1Style);
		docu.writeAttribute("Level2Style", indexSetupIt->level2Style);
		docu.writeAttribute("Level3Style", indexSetupIt->level3Style);
		docu.writeEndElement();
	}
	docu.writeEndElement();
}

void Scribus171Format::writeTOC(ScXmlStreamWriter & docu) const
{
	docu.writeStartElement("TablesOfContents");
	for (const auto& tocSetup : m_Doc->tocSetups())
	{
		docu.writeStartElement("TableOfContents");
		docu.writeAttribute("Name", tocSetup.name);
		docu.writeAttribute("ToCSource", tocSetup.tocSource);
		docu.writeAttribute("ItemAttributeName", tocSetup.itemAttrName);
		docu.writeAttribute("FrameName", tocSetup.frameName);
		docu.writeAttribute("ListNonPrinting", tocSetup.listNonPrintingFrames);
		docu.writeAttribute("Style", tocSetup.textStyle);
		switch (tocSetup.pageLocation)
		{
			case Beginning:
				docu.writeAttribute("NumberPlacement", "Beginning");
				break;
			case End:
				docu.writeAttribute("NumberPlacement", "End");
				break;
			case NotShown:
				docu.writeAttribute("NumberPlacement", "NotShown");
				break;
		}
		if (tocSetup.tocSource == "Style")
		{
			for (const auto& tocSetupEntry : tocSetup.entryData)
			{
				docu.writeEmptyElement("StyleInTOC");
				docu.writeAttribute("StyleName", tocSetupEntry.styleToFind);
				docu.writeAttribute("TOCStyle", tocSetupEntry.styleForText);
				docu.writeAttribute("RemoveLineBreaks", tocSetupEntry.removeLineBreaks);
				switch (tocSetupEntry.pageLocation)
				{
					case Beginning:
						docu.writeAttribute("NumberPlacement", "Beginning");
						break;
					case End:
						docu.writeAttribute("NumberPlacement", "End");
						break;
					case NotShown:
						docu.writeAttribute("NumberPlacement", "NotShown");
						break;
				}
			}
		}
		docu.writeEndElement();
	}
	docu.writeEndElement();
}


void Scribus171Format::writeSections(ScXmlStreamWriter & docu) const
{
	docu.writeStartElement("Sections");
	for (auto it = m_Doc->sections().begin() ; it != m_Doc->sections().end(); ++it )
	{
		docu.writeEmptyElement("Section");
		docu.writeAttribute("Number", (*it).number);
		docu.writeAttribute("Name", (*it).name);
		docu.writeAttribute("From", (*it).fromindex);
		docu.writeAttribute("To", (*it).toindex);
		switch ((*it).type)
		{
			case Type_1_2_3:
				docu.writeAttribute("Type", "Type_1_2_3");
				break;
			case Type_1_2_3_ar:
				docu.writeAttribute("Type", "Type_1_2_3_ar");
				break;
			case Type_i_ii_iii:
				docu.writeAttribute("Type", "Type_i_ii_iii");
				break;
			case Type_I_II_III:
				docu.writeAttribute("Type", "Type_I_II_III");
				break;
			case Type_a_b_c:
				docu.writeAttribute("Type", "Type_a_b_c");
				break;
			case Type_A_B_C:
				docu.writeAttribute("Type", "Type_A_B_C");
				break;
			case Type_Alphabet_ar:
				docu.writeAttribute("Type", "Type_Alphabet_ar");
				break;
			case Type_Abjad_ar:
				docu.writeAttribute("Type", "Type_Abjad_ar");
				break;
			case Type_Hebrew:
				docu.writeAttribute("Type", "Type_Hebrew");
				break;
			case Type_asterix:
				docu.writeAttribute("Type", "Type_asterix");
				break;
			case Type_CJK:
				docu.writeAttribute("Type", "Type_CJK");
				break;
			case Type_None:
				docu.writeAttribute("Type", "Type_None");
				break;
		}
		docu.writeAttribute("Start", (*it).sectionstartindex);
		docu.writeAttribute("Reversed", (*it).reversed);
		docu.writeAttribute("Active", (*it).active);
		docu.writeAttribute("FillChar", (*it).pageNumberFillChar.unicode());
		docu.writeAttribute("FieldWidth", (*it).pageNumberWidth);
	}
	docu.writeEndElement();
}

void Scribus171Format::writeMarks(ScXmlStreamWriter & docu) const
{
	//write list of defined marks to SLA
	if (m_Doc->marksList().isEmpty())
		return;

	docu.writeStartElement("Marks");
	for (const Mark* mrk : m_Doc->marksList())
	{
		if (mrk->isType(MARKNoteFrameType))
			continue;
		docu.writeEmptyElement("Mark");
		docu.writeAttribute("label", mrk->label);
		docu.writeAttribute("type", mrk->getType());

		if (mrk->isType(MARK2ItemType) && mrk->hasItemPtr())
		{
			const PageItem* item = mrk->getItemPtr();
			assert(item != nullptr);
			docu.writeAttribute("ItemID", qHash(item) & 0x7FFFFFFF);
		}
		else if ((mrk->isType(MARKVariableTextType) || mrk->isType(MARKIndexType)) && mrk->hasString())
			docu.writeAttribute("str", mrk->getString());
		else if (mrk->isType(MARK2MarkType) && mrk->hasMark())
		{
			QString label = mrk->getDestMarkName();
			MarkType type = mrk->getDestMarkType();
			docu.writeAttribute("MARKlabel", label);
			docu.writeAttribute("MARKtype", type);
		}
	}
	docu.writeEndElement();
}

void Scribus171Format::writeOpticalMarginSets(ScXmlStreamWriter & docu) const
{
	docu.writeStartElement("OpticalMarginSets");

	OpticalMarginSets sets = m_Doc->typographicPrefs().opticalMarginSets;

	for (const auto &set : sets)
	{
		docu.writeStartElement("Set");
		docu.writeAttribute("Id", set.id);
		docu.writeAttribute("Type", set.type);
		docu.writeAttribute("Name", set.name);

		docu.writeStartElement("Rules");

		OpticalMarginRules rules = OpticalMarginLookup::instance().mergeCharsToRules(set.rules);

		for (const auto &rule : rules)
		{
			docu.writeEmptyElement("Rule");
			docu.writeAttribute("Left", rule.Left);
			docu.writeAttribute("Right", rule.Right);
			docu.writeAttribute("Unit", rule.Unit);
			docu.writeAttribute("Characters", stringToUnicode(rule.Chars));
		}

		docu.writeEndElement(); // Rules
		docu.writeEndElement(); // Set

	}

	docu.writeEndElement(); // OpticalMarginSets
}

void Scribus171Format::writeNotesStyles(ScXmlStreamWriter & docu) const
{
	if (m_Doc->m_docNotesStylesList.isEmpty())
		return;

	//write notes styles
	QStringList noteStyleNames; 
	QList<NotesStyle*>::ConstIterator end = m_Doc->m_docNotesStylesList.constEnd();
	for (auto itNS = m_Doc->m_docNotesStylesList.constBegin(); itNS != end; ++itNS)
	{
		const NotesStyle* noteStyle = (*itNS);
		noteStyleNames.append(noteStyle->name());
	}
	writeNotesStyles(docu, noteStyleNames);
}

void  Scribus171Format::writeNotesStyles(ScXmlStreamWriter & docu, const QStringList& styleSelection) const
{
	if (styleSelection.isEmpty())
		return;

	//write notes styles
	docu.writeStartElement("NotesStyles");

	auto end = m_Doc->m_docNotesStylesList.end();
	for (auto itNS = m_Doc->m_docNotesStylesList.begin(); itNS != end; ++itNS)
	{
		const NotesStyle* noteStyle = (*itNS);
		if (!styleSelection.contains(noteStyle->name()))
			continue;
		docu.writeEmptyElement("notesStyle");
		docu.writeAttribute("Name", noteStyle->name());
		docu.writeAttribute("Start", noteStyle->start());
		docu.writeAttribute("Endnotes", noteStyle->isEndNotes());
		switch (noteStyle->getType())
		{
			case Type_1_2_3:
				docu.writeAttribute("Type", "Type_1_2_3");
				break;
			case Type_1_2_3_ar:
				docu.writeAttribute("Type", "Type_1_2_3_ar");
				break;
			case Type_i_ii_iii:
				docu.writeAttribute("Type", "Type_i_ii_iii");
				break;
			case Type_I_II_III:
				docu.writeAttribute("Type", "Type_I_II_III");
				break;
			case Type_a_b_c:
				docu.writeAttribute("Type", "Type_a_b_c");
				break;
			case Type_A_B_C:
				docu.writeAttribute("Type", "Type_A_B_C");
				break;
			case Type_Alphabet_ar:
				docu.writeAttribute("Type", "Type_Alphabet_ar");
				break;
			case Type_Abjad_ar:
				docu.writeAttribute("Type", "Type_Abjad_ar");
				break;
			case Type_Hebrew:
				docu.writeAttribute("Type", "Type_Hebrew");
				break;
			case Type_asterix:
				docu.writeAttribute("Type", "Type_asterix");
				break;
			case Type_CJK:
				docu.writeAttribute("Type", "Type_CJK");
				break;
			case Type_None:
				docu.writeAttribute("Type", "Type_None");
				break;
		}
		docu.writeAttribute("Range", (int) noteStyle->range());
		docu.writeAttribute("Prefix", noteStyle->prefix());
		docu.writeAttribute("Suffix", noteStyle->suffix());
		docu.writeAttribute("AutoHeight", noteStyle->isAutoNotesHeight());
		docu.writeAttribute("AutoWidth", noteStyle->isAutoNotesWidth());
		docu.writeAttribute("AutoRemove", noteStyle->isAutoRemoveEmptyNotesFrames());
		docu.writeAttribute("AutoWeld", noteStyle->isAutoWeldNotesFrames());
		docu.writeAttribute("SuperNote", noteStyle->isSuperscriptInNote());
		docu.writeAttribute("SuperMaster", noteStyle->isSuperscriptInMaster());
		docu.writeAttribute("MarksStyle", noteStyle->marksChStyle());
		docu.writeAttribute("NotesStyle", noteStyle->notesParStyle());
	}
	docu.writeEndElement();
}

void Scribus171Format::writeNotesFrames(ScXmlStreamWriter &docu) const
{
	QList<PageItem_NoteFrame*> nfList;
	for (const NotesStyle* noteStyle : std::as_const(m_Doc->m_docNotesStylesList))
		nfList.append(m_Doc->listNotesFrames(noteStyle));

	writeNotesFrames(docu, nfList);
}

void Scribus171Format::writeNotesFrames(ScXmlStreamWriter & docu, const QList<PageItem_NoteFrame*>& itemList) const
{
	if (itemList.isEmpty())
		return;

	docu.writeStartElement("NotesFrames");

	for (int i = 0; i < itemList.count(); ++i)
	{
		PageItem_NoteFrame* noteFrame = itemList.at(i);
		const NotesStyle* noteStyle = noteFrame->notesStyle();
		if (noteStyle->isEndNotes())
		{
			docu.writeEmptyElement("EndNoteFrame");
			docu.writeAttribute("NoteStyleName", noteStyle->name());
			docu.writeAttribute("range", (int) noteStyle->range());
			docu.writeAttribute("myID", qHash(noteFrame) & 0x7FFFFFFF);
			
			rangeItem rI = m_Doc->m_docEndNotesFramesMap.value(noteFrame);
			if (noteStyle->range() == NSRstory)
				docu.writeAttribute("ItemID", qHash(rI.firstStoryFrame) & 0x7FFFFFFF);
		}
		else //footnotes frame
		{
			docu.writeEmptyElement("FootNoteFrame");
			docu.writeAttribute("NoteStyleName", noteStyle->name());
			docu.writeAttribute("myID", qHash(noteFrame) & 0x7FFFFFFF);
			docu.writeAttribute("MasterID", qHash(noteFrame->masterFrame()) & 0x7FFFFFFF);
		}
	}
	docu.writeEndElement();
}

void Scribus171Format::writeNotes(ScXmlStreamWriter & docu) const
{
	//write notes
	if (m_Doc->notesList().isEmpty())
		return;

	const QList<TextNote*>& noteList = m_Doc->notesList();
	writeNotes(docu, noteList);
}

void Scribus171Format::writeNotes(ScXmlStreamWriter & docu, const QList<TextNote*>& noteList) const
{
	if (noteList.isEmpty())
		return;

	docu.writeStartElement("Notes");
	for (int i = 0; i < noteList.count(); ++i)
	{
		TextNote* textNote = noteList.at(i);
		if (textNote->masterMark() == nullptr)
			continue;
		docu.writeEmptyElement("Note");
		docu.writeAttribute("Master", textNote->masterMark()->label);
		docu.writeAttribute("NoteStyle", textNote->notesStyle()->name());
		docu.writeAttribute("Text", textNote->saxedText());
	}
	docu.writeEndElement();
}

void Scribus171Format::writePageSets(ScXmlStreamWriter & docu) const
{	
	docu.writeStartElement("PageSets");
	QList<PageSet> pageSets(m_Doc->pageSets());
	for (const auto& pageSet : pageSets)
	{
		docu.writeStartElement("Set");
		docu.writeAttribute("Name", pageSet.Name);
		docu.writeAttribute("FirstPage", pageSet.FirstPage);
		docu.writeAttribute("Rows", pageSet.Rows);
		docu.writeAttribute("Columns", pageSet.Columns);
		for (const QString& pageName : pageSet.pageNames)
		{
			docu.writeEmptyElement("PageNames");
			docu.writeAttribute("Name", pageName);
		}
		docu.writeEndElement();
	}
	docu.writeEndElement();
}

void Scribus171Format::writePatterns(ScXmlStreamWriter & docu, const QString& baseDir, bool part, Selection* selection) const
{
	QStringList patterns;
	if (part)
		patterns = m_Doc->getPatternDependencyList(m_Doc->getUsedPatternsSelection(selection));
	else
		patterns = m_Doc->getPatternDependencyList(m_Doc->docPatterns.keys());
	for (int a = 0; a < patterns.count(); a++)
	{
		docu.writeStartElement("Pattern");
		docu.writeAttribute("Name", patterns[a]);
		ScPattern pa = m_Doc->docPatterns[patterns[a]];
		docu.writeAttribute("width", pa.width);
		docu.writeAttribute("height", pa.height);
		docu.writeAttribute("scaleX", pa.scaleX);
		docu.writeAttribute("scaleY", pa.scaleY);
		docu.writeAttribute("xoffset", pa.xoffset);
		docu.writeAttribute("yoffset", pa.yoffset);
		WriteObjects(m_Doc, docu, baseDir, nullptr, 0, ItemSelectionPattern, &pa.items);
		docu.writeEndElement();
	}	
}

void Scribus171Format::writeContent(ScXmlStreamWriter & docu, const QString& baseDir)
{
	if (m_mwProgressBar != nullptr)
	{
		m_mwProgressBar->setMaximum(m_Doc->DocPages.count()+m_Doc->MasterPages.count()+m_Doc->DocItems.count()+m_Doc->MasterItems.count()+m_Doc->FrameItems.count());
		m_mwProgressBar->setValue(0);
	}
	WritePages(m_Doc, docu, m_mwProgressBar, 0, true);
	WritePages(m_Doc, docu, m_mwProgressBar, m_Doc->MasterPages.count(), false);
	WriteObjects(m_Doc, docu, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count(), ItemSelectionFrame);
	WriteObjects(m_Doc, docu, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count()+m_Doc->FrameItems.count(), ItemSelectionMaster);
	WriteObjects(m_Doc, docu, baseDir, m_mwProgressBar, m_Doc->MasterPages.count()+m_Doc->DocPages.count()+m_Doc->MasterItems.count()+m_Doc->FrameItems.count(), ItemSelectionPage);
}

void Scribus171Format::WritePages(ScribusDoc *doc, ScXmlStreamWriter& docu, QProgressBar *dia2, uint maxC, bool master) const
{
	uint ObCount = maxC;
	ScPage *page;
	uint pages;
	if (master)
		pages = doc->MasterPages.count();
	else
		pages = doc->DocPages.count();
	for (uint i = 0; i < pages; ++i)
	{
		ObCount++;
		if (dia2 != nullptr)
			dia2->setValue(ObCount);
		if (master)
		{
			docu.writeStartElement("MasterPage");
			page = doc->MasterPages.at(i);
		}
		else
		{
			docu.writeStartElement("Page");
			page = doc->DocPages.at(i);
		}
		docu.writeAttribute("PageXPosition",page->xOffset());
		docu.writeAttribute("PageYPosition",page->yOffset());
		docu.writeAttribute("PageWidth",page->width());
		docu.writeAttribute("PageHeight",page->height());
		docu.writeAttribute("BorderLeft",page->initialMargins.left());
		docu.writeAttribute("BorderRight",page->initialMargins.right());
		docu.writeAttribute("BorderTop",page->initialMargins.top());
		docu.writeAttribute("BorderBottom",page->initialMargins.bottom());
		docu.writeAttribute("PageNumber",page->pageNr());
		docu.writeAttribute("PageName",page->pageName());
		docu.writeAttribute("MasterPageName",page->masterPageName());
		docu.writeAttribute("Size", page->size());
		docu.writeAttribute("Orientation", page->orientation());
		docu.writeAttribute("LeftPage", page->LeftPg);
		docu.writeAttribute("Preset", page->marginPreset);
		docu.writeAttribute("VerticalGuides", GuideManagerIO::writeVerticalGuides(page, GuideManagerCore::Standard));
		docu.writeAttribute("HorizontalGuides", GuideManagerIO::writeHorizontalGuides(page, GuideManagerCore::Standard));
		docu.writeAttribute("AutoGuideHorizontalGap", page->guides.horizontalAutoGap());
		docu.writeAttribute("AutoGuideVerticalGap", page->guides.verticalAutoGap());
		docu.writeAttribute("AutoGuideHorizontalCount", page->guides.horizontalAutoCount());
		docu.writeAttribute("AutoGuideVerticalCount", page->guides.verticalAutoCount());
		docu.writeAttribute("AutoGuideHorizontalReference", page->guides.horizontalAutoRefer());
		docu.writeAttribute("AutoGuideVerticalReference", page->guides.verticalAutoRefer());
		docu.writeAttribute("AutoGuideSelection", GuideManagerIO::writeSelection(page));
		docu.writeAttribute("PageEffectDuration", page->PresentVals.pageEffectDuration);
		docu.writeAttribute("PageViewDuration", page->PresentVals.pageViewDuration);
		docu.writeAttribute("PageEffectType", page->PresentVals.effectType);
		docu.writeAttribute("PageEffectLineDirection", page->PresentVals.Dm);
		docu.writeAttribute("PageEffectInsideOutside", page->PresentVals.M);
		docu.writeAttribute("PageEffectDirection", page->PresentVals.Di);
		docu.writeEndElement();
	}
}


namespace { // anon
	QString textWithSoftHyphens(const StoryText& itemText, int from, int to)
	{
		QString result;
		int lastPos = from;
		for (int i = from; i < to; ++i)
		{
			if (itemText.hasFlag(i, ScLayout_HyphenationPossible)
				// duplicate SHYPHEN if already present to indicate a user provided SHYPHEN:
				|| itemText.text(i) == SpecialChars::SHYPHEN)
			{
				result += itemText.text(lastPos, i + 1 - lastPos);
				result += SpecialChars::SHYPHEN;
				lastPos = i + 1;
			}
		}
		if (lastPos < to)
			result += itemText.text(lastPos, to - lastPos);
		return result;
	}
} // namespace anon

void Scribus171Format::writeStoryText(ScribusDoc *doc, ScXmlStreamWriter& docu, StoryText& story, PageItem* item) const
{
	docu.writeStartElement("StoryText");

	const ParagraphStyle& defaultStyle = story.defaultStyle();
	putPStyle(docu, defaultStyle, "DefaultStyle");

	writeITEXTs(doc, docu, story, item);

	docu.writeEndElement();
}

void Scribus171Format::writeITEXTs(ScribusDoc *doc, ScXmlStreamWriter &docu, StoryText& story, PageItem* item) const
{
	CharStyle lastStyle;
	int lastPos = 0;
	QString tmpnum;
	int iTLen = story.length();
	if (item && item->isNoteFrame())
		iTLen = 0;  //used for saving empty endnotes frames, as they will be filled automatically
	for (int k = 0; k < iTLen; ++k)
	{
		const CharStyle& style1(story.charStyle(k));
		const QChar ch = story.text(k);

		if (ch == SpecialChars::OBJECT ||
			ch == SpecialChars::TAB ||
			ch == SpecialChars::PARSEP ||
			ch == SpecialChars::LINEBREAK ||
			ch == SpecialChars::COLBREAK ||
			ch == SpecialChars::FRAMEBREAK ||
			ch == SpecialChars::PAGENUMBER ||
			ch == SpecialChars::PAGECOUNT ||
			ch == SpecialChars::NBHYPHEN ||
			ch == SpecialChars::NBSPACE ||
			ch == SpecialChars::ZWNBSPACE ||
			ch == SpecialChars::ZWSPACE ||
			ch.unicode() < 32 || 
			(0xd800 <= ch.unicode() && ch.unicode() < 0xe000) ||
			ch.unicode() == 0xfffe || ch.unicode() == 0xffff ||
			style1 != lastStyle)
		{
			// something new, write pending chars
			if  (k - lastPos > 0)
			{
				docu.writeEmptyElement("ITEXT");
				putCStyle(docu, lastStyle);
				docu.writeAttribute("CH", textWithSoftHyphens(story, lastPos, k));
			}
			lastStyle = style1;
			lastPos = k;
		}

		if (ch == SpecialChars::OBJECT && story.object(k).getPageItem(doc) != nullptr)
		{
			// each obj in its own ITEXT for now
			docu.writeEmptyElement("ITEXT");
			putCStyle(docu, lastStyle);
			tmpnum.setNum(ch.unicode());
			docu.writeAttribute("Unicode", tmpnum);
			docu.writeAttribute("COBJ", story.object(k).getInlineCharID());
		}
		else if (ch == SpecialChars::OBJECT && story.hasMark(k))
		{
			const Mark* mark = story.mark(k);
			if (!mark->isType(MARKBullNumType))
			{
				//dont save marks for bullets and numbering
				docu.writeEmptyElement("MARK");
				docu.writeAttribute("label", mark->label);
				docu.writeAttribute("type", mark->getType());
				putCStyle(docu, lastStyle);
			}
		}
		else if (ch == SpecialChars::PARSEP)	// stores also the paragraphstyle for preceding chars
			putPStyle(docu, story.paragraphStyle(k), "para");
		else if (ch == SpecialChars::TAB)
		{
			docu.writeEmptyElement("tab");
			putCStyle(docu, lastStyle);
		}
		else if (ch == SpecialChars::LINEBREAK)
			docu.writeEmptyElement("breakline");
		else if (ch == SpecialChars::COLBREAK)
			docu.writeEmptyElement("breakcol");
		else if (ch == SpecialChars::FRAMEBREAK)
			docu.writeEmptyElement("breakframe");
		else if (ch == SpecialChars::NBHYPHEN)
		{
			docu.writeEmptyElement("nbhyphen");
			putCStyle(docu, lastStyle);
		}
		else if (ch == SpecialChars::NBSPACE)
		{
			docu.writeEmptyElement("nbspace");
			putCStyle(docu, lastStyle);
		}
		else if (ch == SpecialChars::ZWNBSPACE)
		{
			docu.writeEmptyElement("zwnbspace");
			putCStyle(docu, lastStyle);
		}
		else if (ch == SpecialChars::ZWSPACE)
		{
			docu.writeEmptyElement("zwspace");
			putCStyle(docu, lastStyle);
		}
		else if (ch == SpecialChars::PAGENUMBER) 
		{
			docu.writeEmptyElement("var");
			docu.writeAttribute("name", "pgno");
			putCStyle(docu, lastStyle);
		}
		else if (ch == SpecialChars::PAGECOUNT) 
		{
			docu.writeEmptyElement("var");
			docu.writeAttribute("name", "pgco");
			putCStyle(docu, lastStyle);
		}
		else if (ch.unicode() < 32 || 
				 (0xd800 <= ch.unicode() && ch.unicode() < 0xe000) ||
				 ch.unicode() == 0xfffe || ch.unicode() == 0xffff)
		{
			docu.writeEmptyElement("ITEXT");
			putCStyle(docu, lastStyle);
			tmpnum.setNum(ch.unicode());
			docu.writeAttribute("Unicode", tmpnum);		
		}
		else
		{
			continue;
		}
		// otherwise we just wrote something special and are done with position 'k'
		lastPos = k + 1;
	}
	// write pending chars
	if (story.length() - lastPos > 0)
	{
		docu.writeEmptyElement("ITEXT");
		putCStyle(docu, lastStyle);
		docu.writeAttribute("CH", textWithSoftHyphens(story, lastPos, story.length()));
	}
	// paragraphstyle for trailing chars
	if (story.length() == 0 || story.text(story.length() - 1) != SpecialChars::PARSEP)
	{
		putPStyle(docu, story.paragraphStyle(story.length()), "trail");
	}
}

void Scribus171Format::WriteObjects(ScribusDoc *doc, ScXmlStreamWriter& docu, const QString& baseDir, QProgressBar *dia2, uint maxC, ItemSelection master, QList<PageItem*> *some_items) const
{
	uint ObCount = maxC;
	QList<PageItem*> *items = nullptr;
	QList<PageItem*> itemList;
	PageItem *item = nullptr;
	uint objects = 0;
	switch (master)
	{
		case ItemSelectionMaster:
			items = &doc->MasterItems;
			break;
		case ItemSelectionPage:
			items = &doc->DocItems;
			break;
		case ItemSelectionFrame:
			if (some_items != nullptr)
				items = some_items;
			else
			{
				itemList = doc->FrameItems.values();
				items = &itemList;
			}
			break;
		case ItemSelectionGroup:
		case ItemSelectionPattern:
		case ItemSelectionElements:
			items = some_items;
			break;
		default:
			assert(false);
	}
	objects = items->count();
	for (uint j = 0; j < objects;++j)
	{
		ObCount++;
		if (dia2 != nullptr)
			dia2->setValue(ObCount);
		item = items->at(j);
		switch (master)
		{
			case ItemSelectionMaster:
				docu.writeStartElement("MasterObject");
				break;
			case ItemSelectionGroup:
			case ItemSelectionPage:
				docu.writeStartElement("PageObject");
				break;
			case ItemSelectionFrame:
				docu.writeStartElement("FrameObject");
				break;
			case ItemSelectionPattern:
				docu.writeStartElement("PatternItem");
				break;
			case ItemSelectionElements:
				docu.writeStartElement("Item");
				break;
		}
		if (master == ItemSelectionFrame)
			docu.writeAttribute("InID", item->inlineCharID);
		if (master == ItemSelectionElements)
		{
			docu.writeAttribute("XPosition", item->xPos() - doc->currentPage()->xOffset());
			docu.writeAttribute("YPosition", item->yPos() - doc->currentPage()->yOffset());
		}
		else
		{
			docu.writeAttribute("XPosition", item->xPos());
			docu.writeAttribute("YPosition", item->yPos());
		}
		SetItemProps(docu, item, baseDir);
		if (!item->OnMasterPage.isEmpty())
			docu.writeAttribute("OnMasterPage", item->OnMasterPage);
		if (!item->pixm.imgInfo.usedPath.isEmpty())
			docu.writeAttribute("ImageClip", item->pixm.imgInfo.usedPath);
		if (item->pixm.imgInfo.lowResType != 1)
			docu.writeAttribute("ImageRes", item->pixm.imgInfo.lowResType);
		if (item->isEmbedded)
			docu.writeAttribute("isInline", 1);
		if (!item->fillRule)
			docu.writeAttribute("fillRule", 0);
		if (item->doOverprint)
			docu.writeAttribute("doOverprint", 1);
		docu.writeAttribute("gXpos", item->gXpos);
		docu.writeAttribute("gYpos", item->gYpos);
		docu.writeAttribute("gWidth", item->gWidth);
		docu.writeAttribute("gHeight", item->gHeight);
		if (item->itemType() == PageItem::Symbol)
			docu.writeAttribute("pattern", item->pattern());
		if (item->GrType != 0)
		{
			if (item->GrType == Gradient_Pattern)
			{
				docu.writeAttribute("pattern", item->pattern());
				const ScPatternTransform& patternTrans = item->patternTransform();
				bool mirrorX, mirrorY;
				item->patternFlip(mirrorX, mirrorY);
				docu.writeAttribute("pScaleX", patternTrans.scaleX * 100.0);
				docu.writeAttribute("pScaleY", patternTrans.scaleY * 100.0);
				docu.writeAttribute("pOffsetX", patternTrans.offsetX);
				docu.writeAttribute("pOffsetY", patternTrans.offsetY);
				docu.writeAttribute("pRotation", patternTrans.rotation);
				docu.writeAttribute("pSkewX", patternTrans.skewX);
				docu.writeAttribute("pSkewY", patternTrans.skewY);
				docu.writeAttribute("pMirrorX", mirrorX);
				docu.writeAttribute("pMirrorY", mirrorY);
			}
			else
			{
				if (item->GrType == Gradient_Mesh)
				{
					docu.writeAttribute("GMAY", item->meshGradientArray[0].count());
					docu.writeAttribute("GMAX", item->meshGradientArray.count());
				}
				else if (item->GrType == Gradient_PatchMesh)
				{
					docu.writeAttribute("GMAX", item->meshGradientPatches.count());
				}
				else if (item->GrType == Gradient_Hatch)
				{
					docu.writeAttribute("HatchMode", item->hatchType);
					docu.writeAttribute("HatchDist", item->hatchDistance);
					docu.writeAttribute("HatchAngle", item->hatchAngle);
					docu.writeAttribute("HatchSolidB", item->hatchUseBackground);
					docu.writeAttribute("HatchBackG", item->hatchBackground);
					docu.writeAttribute("HatchForeC", item->hatchForeground);
				}
				else
				{
					docu.writeAttribute("GRSTARTX", item->GrStartX);
					docu.writeAttribute("GRSTARTY", item->GrStartY);
					docu.writeAttribute("GRENDX", item->GrEndX);
					docu.writeAttribute("GRENDY", item->GrEndY);
					docu.writeAttribute("GRFOCALX", item->GrFocalX);
					docu.writeAttribute("GRFOCALY", item->GrFocalY);
					docu.writeAttribute("GRSCALE", item->GrScale);
					docu.writeAttribute("GRSKEW", item->GrSkew);
					docu.writeAttribute("GRExt", item->getGradientExtend());
					if ((item->GrType == Gradient_4Colors) || (item->GrType == Gradient_Diamond))
					{
						docu.writeAttribute("GRC1X", item->GrControl1.x());
						docu.writeAttribute("GRC1Y", item->GrControl1.y());
						docu.writeAttribute("GRCOLP1", item->GrColorP1);
						docu.writeAttribute("GRC2X", item->GrControl2.x());
						docu.writeAttribute("GRC2Y", item->GrControl2.y());
						docu.writeAttribute("GRCOLP2", item->GrColorP2);
						docu.writeAttribute("GRC3X", item->GrControl3.x());
						docu.writeAttribute("GRC3Y", item->GrControl3.y());
						docu.writeAttribute("GRCOLP3", item->GrColorP3);
						docu.writeAttribute("GRC4X", item->GrControl4.x());
						docu.writeAttribute("GRC4Y", item->GrControl4.y());
						docu.writeAttribute("GRC5X", item->GrControl5.x());
						docu.writeAttribute("GRC5Y", item->GrControl5.y());
						docu.writeAttribute("GRCOLP4", item->GrColorP4);
						docu.writeAttribute("GRCOLT1", item->GrCol1transp);
						docu.writeAttribute("GRCOLT2", item->GrCol2transp);
						docu.writeAttribute("GRCOLT3", item->GrCol3transp);
						docu.writeAttribute("GRCOLT4", item->GrCol4transp);
						docu.writeAttribute("GRCOLS1", item->GrCol1Shade);
						docu.writeAttribute("GRCOLS2", item->GrCol2Shade);
						docu.writeAttribute("GRCOLS3", item->GrCol3Shade);
						docu.writeAttribute("GRCOLS4", item->GrCol4Shade);
					}
				}
			}
		}
		if (!item->gradient().isEmpty())
			docu.writeAttribute("GRNAME", item->gradient());
		if (!item->strokeGradient().isEmpty())
			docu.writeAttribute("GRNAMES", item->strokeGradient());
		if (!item->gradientMask().isEmpty())
			docu.writeAttribute("GRNAMEM", item->gradientMask());
		if (item->GrTypeStroke > 0)
		{
			docu.writeAttribute("GRExtS", item->getStrokeGradientExtend());
			docu.writeAttribute("GRSTARTXS", item->GrStrokeStartX);
			docu.writeAttribute("GRSTARTYS", item->GrStrokeStartY);
			docu.writeAttribute("GRENDXS", item->GrStrokeEndX);
			docu.writeAttribute("GRENDYS", item->GrStrokeEndY);
			docu.writeAttribute("GRFOCALXS", item->GrStrokeFocalX);
			docu.writeAttribute("GRFOCALYS", item->GrStrokeFocalY);
			docu.writeAttribute("GRSCALES", item->GrStrokeScale);
			docu.writeAttribute("GRSKEWS", item->GrStrokeSkew);
		}
		if (!item->strokePattern().isEmpty())
		{
			docu.writeAttribute("patternS", item->strokePattern());
			const ScStrokePatternTransform& strokePatTrans = item->strokePatternTransform();
			bool mirrorX, mirrorY;
			item->strokePatternFlip(mirrorX, mirrorY);
			bool atPath = item->isStrokePatternToPath();
			docu.writeAttribute("pScaleXS", strokePatTrans.scaleX * 100.0);
			docu.writeAttribute("pScaleYS", strokePatTrans.scaleY * 100.0);
			docu.writeAttribute("pOffsetXS", strokePatTrans.offsetX);
			docu.writeAttribute("pOffsetYS", strokePatTrans.offsetY);
			docu.writeAttribute("pRotationS", strokePatTrans.rotation);
			docu.writeAttribute("pSkewXS", strokePatTrans.skewX);
			docu.writeAttribute("pSkewYS", strokePatTrans.skewY);
			docu.writeAttribute("pSpaceS", strokePatTrans.space);
			docu.writeAttribute("pMirrorXS", mirrorX);
			docu.writeAttribute("pMirrorYS", mirrorY);
			docu.writeAttribute("pAtPathS", atPath);
		}
		if (item->GrMask > 0)
		{
			docu.writeAttribute("GRExtM", item->mask_gradient.repeatMethod());
			docu.writeAttribute("GRTYPM", item->GrMask);
			docu.writeAttribute("GRSTARTXM", item->GrMaskStartX);
			docu.writeAttribute("GRSTARTYM", item->GrMaskStartY);
			docu.writeAttribute("GRENDXM", item->GrMaskEndX);
			docu.writeAttribute("GRENDYM", item->GrMaskEndY);
			docu.writeAttribute("GRFOCALXM", item->GrMaskFocalX);
			docu.writeAttribute("GRFOCALYM", item->GrMaskFocalY);
			docu.writeAttribute("GRSCALEM", item->GrMaskScale);
			docu.writeAttribute("GRSKEWM", item->GrMaskSkew);
		}
		if (!item->patternMask().isEmpty())
		{
			docu.writeAttribute("patternM", item->patternMask());
			const ScMaskTransform& maskTrans = item->maskTransform();
			bool mirrorX, mirrorY;
			item->maskFlip(mirrorX, mirrorY);
			docu.writeAttribute("pScaleXM", maskTrans.scaleX * 100.0);
			docu.writeAttribute("pScaleYM", maskTrans.scaleY * 100.0);
			docu.writeAttribute("pOffsetXM", maskTrans.offsetX);
			docu.writeAttribute("pOffsetYM", maskTrans.offsetY);
			docu.writeAttribute("pRotationM", maskTrans.rotation);
			docu.writeAttribute("pSkewXM", maskTrans.skewX);
			docu.writeAttribute("pSkewYM", maskTrans.skewY);
			docu.writeAttribute("pMirrorXM", mirrorX);
			docu.writeAttribute("pMirrorYM", mirrorY);
		}
		if (item->itemText.defaultStyle().hasParent())
			docu.writeAttribute("ParagraphStyle", item->itemText.defaultStyle().parent());
		if (! item->itemText.defaultStyle().isInhAlignment())
			docu.writeAttribute("ALIGN", item->itemText.defaultStyle().alignment());
		
		docu.writeAttribute("Layer", item->m_layerID);
		if (item->isBookmark)
			docu.writeAttribute("BookMark", 1);

		if (item->isTextFrame() || item->isPathText() || item->isImageFrame())
		{
			if (item->nextInChain() != nullptr)
				docu.writeAttribute("NextItem", qHash(item->nextInChain()) & 0x7FFFFFFF);
			else
				docu.writeAttribute("NextItem", -1);

			bool prevTopParentCheck = (master == ItemSelectionGroup);
			if (master != ItemSelectionGroup)
			{
				PageItem* prevTopParent = item->prevInChain();
				while (prevTopParent && prevTopParent->Parent)
					prevTopParent = prevTopParent->Parent;
				prevTopParentCheck = items->contains(prevTopParent);
			}

			if (item->prevInChain() != nullptr && prevTopParentCheck)
				docu.writeAttribute("BackItem", qHash(item->prevInChain()) & 0x7FFFFFFF);
			else
			{
				docu.writeAttribute("BackItem", -1);
				if (item->isNoteFrame())
					docu.writeAttribute("isNoteFrame", 1);
				else if (item->isTextFrame() || item->isPathText())
					writeStoryText(doc, docu, item->itemText, item);
			}
		}

		if (item->isWelded())
		{
			for (int i = 0 ; i <  item->weldList.count(); i++)
			{
				PageItem::WeldingInfo wInf = item->weldList.at(i);
				const PageItem *pIt = wInf.weldItem;
				if (pIt == nullptr)
				{
					qDebug() << "Saving welding info - empty pointer!!!";
					continue;
				}
				if (pIt->isAutoNoteFrame())
					continue;
				docu.writeEmptyElement("WeldEntry");
				docu.writeAttribute("Target", qHash(wInf.weldItem) & 0x7FFFFFFF);
				docu.writeAttribute("WX", wInf.weldPoint.x());
				docu.writeAttribute("WY", wInf.weldPoint.y());
			}
		}
		if (!item->effectsInUse.isEmpty())
		{
			for (int a = 0; a < item->effectsInUse.count(); ++a)
			{
				docu.writeEmptyElement("ImageEffect");
				docu.writeAttribute("Code", item->effectsInUse.at(a).effectCode);
				docu.writeAttribute("Param", item->effectsInUse.at(a).effectParameters);
			}
		}
		if (((item->isImageFrame()) || (item->isTextFrame())) && (!item->Pfile.isEmpty()) && !item->pixm.imgInfo.layerInfo.isEmpty() && item->pixm.imgInfo.isRequest)
		{
			for (auto it2 = item->pixm.imgInfo.RequestProps.begin(); it2 != item->pixm.imgInfo.RequestProps.end(); ++it2)
			{
				docu.writeEmptyElement("PSDLayer");
				docu.writeAttribute("Layer",it2.key());
				docu.writeAttribute("Visible", static_cast<int>(it2.value().visible));
				docu.writeAttribute("useMask", static_cast<int>(it2.value().useMask));
				docu.writeAttribute("Opacity", it2.value().opacity);
				docu.writeAttribute("Blend", it2.value().blend);
			}
		}
		if (((item->GrType > 0) && (item->GrType != Gradient_Pattern) && (item->GrType != Gradient_4Colors) && (item->GrType != Gradient_Mesh) && (item->GrType != Gradient_Hatch)) && (item->gradient().isEmpty()))
		{
			QList<VColorStop*> cstops = item->fill_gradient.colorStops();
			for (int cst = 0; cst < item->fill_gradient.stops(); ++cst)
			{
				docu.writeEmptyElement("ColorStop");
				docu.writeAttribute("Ramp", cstops.at(cst)->rampPoint);
				docu.writeAttribute("Name", cstops.at(cst)->name);
				docu.writeAttribute("Shade", cstops.at(cst)->shade);
				docu.writeAttribute("Opacity", cstops.at(cst)->opacity);
			}
		}
		if ((item->GrTypeStroke > 0) && (item->strokeGradient().isEmpty()))
		{
			QList<VColorStop*> cstops = item->stroke_gradient.colorStops();
			for (int cst = 0; cst < item->stroke_gradient.stops(); ++cst)
			{
				docu.writeEmptyElement("ColorStopStroke");
				docu.writeAttribute("Ramp", cstops.at(cst)->rampPoint);
				docu.writeAttribute("Name", cstops.at(cst)->name);
				docu.writeAttribute("Shade", cstops.at(cst)->shade);
				docu.writeAttribute("Opacity", cstops.at(cst)->opacity);
			}
		}
		if ((item->GrMask > 0) && (item->gradientMask().isEmpty()))
		{
			QList<VColorStop*> cstops = item->mask_gradient.colorStops();
			for (int cst = 0; cst < item->mask_gradient.stops(); ++cst)
			{
				docu.writeEmptyElement("ColorStopMaskGradient");
				docu.writeAttribute("Ramp", cstops.at(cst)->rampPoint);
				docu.writeAttribute("Name", cstops.at(cst)->name);
				docu.writeAttribute("Shade", cstops.at(cst)->shade);
				docu.writeAttribute("Opacity", cstops.at(cst)->opacity);
			}
		}
		if (item->GrType == Gradient_Mesh)
		{
			for (int grow = 0; grow < item->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < item->meshGradientArray[grow].count(); gcol++)
				{
					MeshPoint mp = item->meshGradientArray[grow][gcol];
					docu.writeStartElement("MPoint");
					docu.writeAttribute("GX", mp.gridPoint.x());
					docu.writeAttribute("GY", mp.gridPoint.y());
					docu.writeAttribute("CTX", mp.controlTop.x());
					docu.writeAttribute("CTY", mp.controlTop.y());
					docu.writeAttribute("CBX", mp.controlBottom.x());
					docu.writeAttribute("CBY", mp.controlBottom.y());
					docu.writeAttribute("CLX", mp.controlLeft.x());
					docu.writeAttribute("CLY", mp.controlLeft.y());
					docu.writeAttribute("CRX", mp.controlRight.x());
					docu.writeAttribute("CRY", mp.controlRight.y());
					docu.writeAttribute("CCX", mp.controlColor.x());
					docu.writeAttribute("CCY", mp.controlColor.y());
					docu.writeAttribute("NAME", mp.colorName);
					docu.writeAttribute("SHADE", mp.shade);
					docu.writeAttribute("TRANS", mp.transparency);
					docu.writeEndElement();
				}
			}
		}
		if (item->GrType == Gradient_PatchMesh)
		{
			for (int grow = 0; grow < item->meshGradientPatches.count(); grow++)
			{
				meshGradientPatch patch = item->meshGradientPatches[grow];
				for (int gcol = 0; gcol < 4; gcol++)
				{
					MeshPoint mp;
					docu.writeStartElement("PMPoint");
					if (gcol == 0)
					{
						mp = patch.TL;
						docu.writeAttribute("CBX", mp.controlBottom.x());
						docu.writeAttribute("CBY", mp.controlBottom.y());
						docu.writeAttribute("CRX", mp.controlRight.x());
						docu.writeAttribute("CRY", mp.controlRight.y());
					}
					else if (gcol == 1)
					{
						mp = patch.TR;
						docu.writeAttribute("CBX", mp.controlBottom.x());
						docu.writeAttribute("CBY", mp.controlBottom.y());
						docu.writeAttribute("CLX", mp.controlLeft.x());
						docu.writeAttribute("CLY", mp.controlLeft.y());
					}
					else if (gcol == 2)
					{
						mp = patch.BR;
						docu.writeAttribute("CTX", mp.controlTop.x());
						docu.writeAttribute("CTY", mp.controlTop.y());
						docu.writeAttribute("CLX", mp.controlLeft.x());
						docu.writeAttribute("CLY", mp.controlLeft.y());
					}
					else if (gcol == 3)
					{
						mp = patch.BL;
						docu.writeAttribute("CTX", mp.controlTop.x());
						docu.writeAttribute("CTY", mp.controlTop.y());
						docu.writeAttribute("CRX", mp.controlRight.x());
						docu.writeAttribute("CRY", mp.controlRight.y());
					}
					docu.writeAttribute("GX", mp.gridPoint.x());
					docu.writeAttribute("GY", mp.gridPoint.y());
					docu.writeAttribute("CCX", mp.controlColor.x());
					docu.writeAttribute("CCY", mp.controlColor.y());
					docu.writeAttribute("NAME", mp.colorName);
					docu.writeAttribute("SHADE", mp.shade);
					docu.writeAttribute("TRANS", mp.transparency);
					docu.writeEndElement();
				}
			}
		}

		if (item->isLatexFrame())
		{
			docu.writeStartElement("LaTeX");
			const PageItem_LatexFrame *latexitem = item->asLatexFrame();
			QFileInfo fi(latexitem->configFile());
			docu.writeAttribute("ConfigFile", fi.fileName());
			docu.writeAttribute("DPI", latexitem->dpi());
			docu.writeAttribute("UsePreamble", latexitem->usePreamble());
			QMapIterator<QString, QString> i(latexitem->editorProperties);
			while (i.hasNext())
			{
				i.next();
				docu.writeStartElement("Property");
				docu.writeAttribute("Name", i.key());
				docu.writeAttribute("Value", i.value());
				docu.writeEndElement();
			}
			docu.writeCharacters(latexitem->formula());
			docu.writeEndElement();
		}
#ifdef HAVE_OSG
		if (item->isOSGFrame())
		{
			PageItem_OSGFrame *osgitem = item->asOSGFrame();
			if (!item->Pfile.isEmpty())
			{
				for (auto itv = osgitem->viewMap.begin(); itv != osgitem->viewMap.end(); ++itv)
				{
					QString tmp;
					docu.writeStartElement("OSGViews");
					docu.writeAttribute("viewName", itv.key());
					docu.writeAttribute("angleFOV", itv.value().angleFOV);
					QString trackM;
					for (uint matx = 0; matx < 4; ++matx)
					{
						for (uint maty = 0; maty < 4; ++maty)
						{
							trackM += tmp.setNum(itv.value().trackerMatrix(matx, maty))+" ";
						}
					}
					docu.writeAttribute("trackM", trackM);
					QString trackC;
					trackC += tmp.setNum(itv.value().trackerCenter[0])+" ";
					trackC += tmp.setNum(itv.value().trackerCenter[1])+" ";
					trackC += tmp.setNum(itv.value().trackerCenter[2]);
					docu.writeAttribute("trackC", trackC);
					QString cameraP;
					cameraP += tmp.setNum(itv.value().cameraPosition[0])+" ";
					cameraP += tmp.setNum(itv.value().cameraPosition[1])+" ";
					cameraP += tmp.setNum(itv.value().cameraPosition[2]);
					docu.writeAttribute("cameraP", cameraP);
					QString cameraU;
					cameraU += tmp.setNum(itv.value().cameraUp[0])+" ";
					cameraU += tmp.setNum(itv.value().cameraUp[1])+" ";
					cameraU += tmp.setNum(itv.value().cameraUp[2]);
					docu.writeAttribute("cameraU", cameraU);
					docu.writeAttribute("trackerDist", itv.value().trackerDist);
					docu.writeAttribute("trackerSize", itv.value().trackerSize);
					docu.writeAttribute("illumination", itv.value().illumination);
					docu.writeAttribute("rendermode", itv.value().rendermode);
					docu.writeAttribute("trans", itv.value().addedTransparency);
					docu.writeAttribute("colorAC", itv.value().colorAC.name());
					docu.writeAttribute("colorFC", itv.value().colorFC.name());
					docu.writeEndElement();
				}
			}
		}
#endif
		if (item->isGroup())
		{
			WriteObjects(m_Doc, docu, baseDir, nullptr, 0, ItemSelectionGroup, &item->groupItemList);
		}
		//Write all the cells and their data to the document, as sub-elements of the pageitem.
		if (item->isTable())
		{
			//ItemType == PageItem::Table or 16 (pageitem.h)
			const PageItem_Table* tableItem = item->asTable();
			docu.writeStartElement("TableData");
			QString tstyle = tableItem->styleName();
			docu.writeAttribute("Style", tableItem->styleName());
			TableStyle ts;
			if (!tstyle.isEmpty())
				ts = tableItem->style();

			if ((tstyle.isEmpty()) || ((!tstyle.isEmpty()) && (!ts.isInhFillColor())))
				docu.writeAttribute("FillColor", tableItem->fillColor());
			if ((tstyle.isEmpty()) || ((!tstyle.isEmpty()) && ( !ts.isInhFillShade())))
				docu.writeAttribute("FillShade", tableItem->fillShade());
			if ((tstyle.isEmpty()) || ((!tstyle.isEmpty()) && ( !ts.isInhLeftBorder())))
			{
				TableBorder tbLeft = tableItem->leftBorder();
				docu.writeStartElement("TableBorderLeft");
				for (const TableBorderLine& tbl : tbLeft.borderLines())
				{
					docu.writeStartElement("TableBorderLine");
					docu.writeAttribute("Width", tbl.width());
					docu.writeAttribute("PenStyle", tbl.style());
					docu.writeAttribute("Color", tbl.color());
					docu.writeAttribute("Shade", tbl.shade());
					docu.writeEndElement();
				}
				docu.writeEndElement();
			}
			if ((tstyle.isEmpty()) || ((!tstyle.isEmpty()) && ( !ts.isInhRightBorder())))
			{
				TableBorder tbRight = tableItem->rightBorder();
				docu.writeStartElement("TableBorderRight");
				for (const TableBorderLine& tbl : tbRight.borderLines())
				{
					docu.writeStartElement("TableBorderLine");
					docu.writeAttribute("Width", tbl.width());
					docu.writeAttribute("PenStyle", tbl.style());
					docu.writeAttribute("Color", tbl.color());
					docu.writeAttribute("Shade", tbl.shade());
					docu.writeEndElement();
				}
				docu.writeEndElement();
			}
			if ((tstyle.isEmpty()) || ((!tstyle.isEmpty()) && ( !ts.isInhTopBorder())))
			{
				TableBorder tbTop = tableItem->topBorder();
				docu.writeStartElement("TableBorderTop");
				for (const TableBorderLine& tbl : tbTop.borderLines())
				{
					docu.writeStartElement("TableBorderLine");
					docu.writeAttribute("Width", tbl.width());
					docu.writeAttribute("PenStyle", tbl.style());
					docu.writeAttribute("Color", tbl.color());
					docu.writeAttribute("Shade", tbl.shade());
					docu.writeEndElement();
				}
				docu.writeEndElement();
			}
			if ((tstyle.isEmpty()) || ((!tstyle.isEmpty()) && ( !ts.isInhBottomBorder())))
			{
				TableBorder tbBottom = tableItem->bottomBorder();
				docu.writeStartElement("TableBorderBottom");
				for (const TableBorderLine& tbl : tbBottom.borderLines())
				{
					docu.writeStartElement("TableBorderLine");
					docu.writeAttribute("Width", tbl.width());
					docu.writeAttribute("PenStyle", tbl.style());
					docu.writeAttribute("Color", tbl.color());
					docu.writeAttribute("Shade", tbl.shade());
					docu.writeEndElement();
				}
				docu.writeEndElement();
			}
			//for each cell, write it to the doc
			for (int row = 0; row < tableItem->rows(); ++row)
			{
				for (int col = 0; col < tableItem->columns(); col ++)
				{
					TableCell cell = tableItem->cellAt(row, col);
					if (cell.row() != row || cell.column() != col)
						continue;
					const PageItem* textFrame = cell.textFrame();
					docu.writeStartElement("Cell");
					docu.writeAttribute("Row", cell.row());
					docu.writeAttribute("Column", cell.column());
					docu.writeAttribute("Style", cell.styleName());
					docu.writeAttribute("TextColumns", textFrame->columns());
					docu.writeAttribute("TextColGap", textFrame->columnGap());
					docu.writeAttribute("TextDistLeft", textFrame->textToFrameDistLeft());
					docu.writeAttribute("TextDistTop", textFrame->textToFrameDistTop());
					docu.writeAttribute("TextDistBottom", textFrame->textToFrameDistBottom());
					docu.writeAttribute("TextDistRight", textFrame->textToFrameDistRight());
					docu.writeAttribute("TextVertAlign", textFrame->verticalAlignment());
					docu.writeAttribute("Flop", textFrame->firstLineOffset());

					QString cstyle = cell.styleName();
					CellStyle cs;
					if (!cstyle.isEmpty())
						cs = cell.style();
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhFillColor())))
						docu.writeAttribute("FillColor", cell.fillColor());
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhFillShade())))
						docu.writeAttribute("FillShade", cell.fillShade());
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhLeftPadding())))
						docu.writeAttribute("LeftPadding",cell.leftPadding());
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhRightPadding())))
						docu.writeAttribute("RightPadding", cell.rightPadding());
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhTopPadding())))
						docu.writeAttribute("TopPadding",cell.topPadding());
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhBottomPadding())))
						docu.writeAttribute("BottomPadding", cell.bottomPadding());
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhLeftBorder())))
					{
						TableBorder tbLeft = cell.leftBorder();
						docu.writeStartElement("TableBorderLeft");
						docu.writeAttribute("Width", tbLeft.width());
						for (const TableBorderLine& tbl : tbLeft.borderLines())
						{
							docu.writeStartElement("TableBorderLine");
							docu.writeAttribute("Width", tbl.width());
							docu.writeAttribute("PenStyle", tbl.style());
							docu.writeAttribute("Color", tbl.color());
							docu.writeAttribute("Shade", tbl.shade());
							docu.writeEndElement();
						}
						docu.writeEndElement();
					}
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhRightBorder())))
					{
						TableBorder tbRight = cell.rightBorder();
						docu.writeStartElement("TableBorderRight");
						docu.writeAttribute("Width", tbRight.width());
						for (const TableBorderLine& tbl : tbRight.borderLines())
						{
							docu.writeStartElement("TableBorderLine");
							docu.writeAttribute("Width", tbl.width());
							docu.writeAttribute("PenStyle", tbl.style());
							docu.writeAttribute("Color", tbl.color());
							docu.writeAttribute("Shade", tbl.shade());
							docu.writeEndElement();
						}
						docu.writeEndElement();
					}
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhTopBorder())))
					{
						TableBorder tbTop = cell.topBorder();
						docu.writeStartElement("TableBorderTop");
						docu.writeAttribute("Width", tbTop.width());
						for (const TableBorderLine& tbl : tbTop.borderLines())
						{
							docu.writeStartElement("TableBorderLine");
							docu.writeAttribute("Width", tbl.width());
							docu.writeAttribute("PenStyle", tbl.style());
							docu.writeAttribute("Color", tbl.color());
							docu.writeAttribute("Shade", tbl.shade());
							docu.writeEndElement();
						}
						docu.writeEndElement();
					}
					if ((cstyle.isEmpty()) || ((!cstyle.isEmpty()) && ( !cs.isInhBottomBorder())))
					{
						TableBorder tbBottom = cell.bottomBorder();
						docu.writeStartElement("TableBorderBottom");
						docu.writeAttribute("Width", tbBottom.width());
						for (const TableBorderLine& tbl : tbBottom.borderLines())
						{
							docu.writeStartElement("TableBorderLine");
							docu.writeAttribute("Width", tbl.width());
							docu.writeAttribute("PenStyle", tbl.style());
							docu.writeAttribute("Color", tbl.color());
							docu.writeAttribute("Shade", tbl.shade());
							docu.writeEndElement();
						}
						docu.writeEndElement();
					}
					//End Cell
					
					writeStoryText(doc, docu, cell.textFrame()->itemText);
					docu.writeEndElement();
				}
			}
			docu.writeEndElement();
		}

		//CB PageItemAttributes
		ObjAttrVector *attributes = item->getObjectAttributes();
		if (attributes->count() > 0)
		{
			docu.writeStartElement("PageItemAttributes");
			for (const auto& itemAttr : *attributes)
			{
				docu.writeEmptyElement("ItemAttribute");
				docu.writeAttribute("Name", itemAttr.name);
				docu.writeAttribute("Type", itemAttr.type);
				docu.writeAttribute("Value", itemAttr.value);
				docu.writeAttribute("Parameter", itemAttr.parameter);
				docu.writeAttribute("Relationship", itemAttr.relationship);
				docu.writeAttribute("RelationshipTo", itemAttr.relationshipto);
				docu.writeAttribute("AutoAddTo", itemAttr.autoaddto);
			}
			docu.writeEndElement();
		}
		docu.writeEndElement();
	}
}

void Scribus171Format::SetItemProps(ScXmlStreamWriter& docu, PageItem* item, const QString& baseDir) const
{
	docu.writeAttribute("OwnPage", item->OwnPage);
	docu.writeAttribute("ItemID", qHash(item) & 0x7FFFFFFF);
	docu.writeAttribute("ItemType", item->realItemType());
	docu.writeAttribute("Width", item->width());
	docu.writeAttribute("Height", item->height());
	if (item->cornerRadius() != 0)
		docu.writeAttribute("CornerRadius", item->cornerRadius());
	docu.writeAttribute("FrameType", item->FrameType);
	docu.writeAttribute("ClipEdited", item->ClipEdited ? 1 : 0);
	if (item->GrType != 0)
		docu.writeAttribute("GradientType", item->GrType);
	if (item->GrTypeStroke != 0)
		docu.writeAttribute("GradientTypeStroke", item->GrTypeStroke);
	if (item->rotation() != 0)
		docu.writeAttribute("Rotation", item->rotation());
	if (!item->printEnabled())
		docu.writeAttribute("PrintEnabled", 0);
	if (item->imageFlippedH())
		docu.writeAttribute("FlippedHorizontal", 1);
	if (item->imageFlippedV())
		docu.writeAttribute("FlippedVertical", 1);
	if (!(item->isGroup() || item->isSymbol()))
	{
		docu.writeAttribute("LineWidth", item->lineWidth());
		if (item->fillColor() != CommonStrings::None)
			docu.writeAttribute("FillColor", item->fillColor());
		if (item->fillShade() != 100)
			docu.writeAttribute("FillShade", item->fillShade());
		if (item->lineColor() != CommonStrings::None)
			docu.writeAttribute("LineColor", item->lineColor());
		if (item->lineShade() != 100)
			docu.writeAttribute("LineShade", item->lineShade());
		if (!item->NamedLStyle.isEmpty())
			docu.writeAttribute("NamedLineStyle", item->NamedLStyle);
		if (item->PLineArt != 0)
			docu.writeAttribute("LinePenStyle", item->PLineArt);
		if (item->PLineEnd != 0)
			docu.writeAttribute("LineCapStyle", item->PLineEnd);
		if (item->PLineJoin != 0)
			docu.writeAttribute("LineJoinStyle", item->PLineJoin);
	}
	//write weld parameter
	if (item->isWelded())
	{
		bool isWelded = false;
		for (int i = 0 ; i <  item->weldList.count(); i++)
		{
			PageItem::WeldingInfo wInf = item->weldList.at(i);
			const PageItem *pIt = wInf.weldItem;
			if (pIt != nullptr && !pIt->isAutoNoteFrame())
			{
				isWelded = true;
				break;
			}
		}
		if (isWelded)
		{
			docu.writeAttribute("isWeldItem", 1);
			docu.writeAttribute("WeldSource", qHash(item) & 0x7FFFFFFF);
		}
	}
	if (item->isRegularPolygon())
	{
		const PageItem_RegularPolygon *regItem = item->asRegularPolygon();
		docu.writeAttribute("PolygonCorners", regItem->polyCorners);
		docu.writeAttribute("PolygonFactor", regItem->polyFactor);
		docu.writeAttribute("PolygonRotation", regItem->polyRotation);
		docu.writeAttribute("PolygonInnerRotation", regItem->polyInnerRot);
		docu.writeAttribute("PolygonCurvature", regItem->polyCurvature);
		docu.writeAttribute("PolygonOuterCurvature", regItem->polyOuterCurvature);
		docu.writeAttribute("PolygonUseFactor", static_cast<int>(regItem->polyUseFactor));
	}
	if (item->isArc())
	{
		const PageItem_Arc *arcItem = item->asArc();
		docu.writeAttribute("arcHeight", arcItem->arcHeight);
		docu.writeAttribute("arcWidth", arcItem->arcWidth);
		docu.writeAttribute("arcStartAngle", arcItem->arcStartAngle);
		docu.writeAttribute("arcSweepAngle", arcItem->arcSweepAngle);
	}
	if (item->isSpiral())
	{
		const PageItem_Spiral *spiralItem = item->asSpiral();
		docu.writeAttribute("spiralStartAngle", spiralItem->spiralStartAngle);
		docu.writeAttribute("spiralEndAngle", spiralItem->spiralEndAngle);
		docu.writeAttribute("spiralFactor", spiralItem->spiralFactor);
	}
	if (item->isAnnotation())
	{
		docu.writeAttribute("ANNOTATION",1);
		docu.writeAttribute("ANTYPE", item->annotation().Type());
		docu.writeAttribute("ANACTION", item->annotation().Action());
		docu.writeAttribute("ANEACT", item->annotation().E_act());
		docu.writeAttribute("ANXACT", item->annotation().X_act());
		docu.writeAttribute("ANDACT", item->annotation().D_act());
		docu.writeAttribute("ANFOACT", item->annotation().Fo_act());
		docu.writeAttribute("ANBLACT", item->annotation().Bl_act());
		docu.writeAttribute("ANKACT", item->annotation().K_act());
		docu.writeAttribute("ANFACT", item->annotation().F_act());
		docu.writeAttribute("ANVACT", item->annotation().V_act());
		docu.writeAttribute("ANCACT", item->annotation().C_act());
		if (item->annotation().ActionType() == Annotation::Action_URI)
			docu.writeAttribute("ANEXTERN", item->annotation().Extern());
		else
			docu.writeAttribute("ANEXTERN", Path2Relative(item->annotation().Extern(), baseDir));
		docu.writeAttribute("ANZIEL", item->annotation().Ziel());
		docu.writeAttribute("ANACTYP", item->annotation().ActionType());
		docu.writeAttribute("ANTOOLTIP", item->annotation().ToolTip());
		docu.writeAttribute("ANBWID", item->annotation().borderWidth());
		docu.writeAttribute("ANBSTY", item->annotation().borderStyle());
		docu.writeAttribute("ANFEED", item->annotation().Feed());
		docu.writeAttribute("ANFLAG", item->annotation().Flag());
		docu.writeAttribute("ANFONT", item->annotation().Font());
		docu.writeAttribute("ANFORMAT", item->annotation().Format());
		docu.writeAttribute("ANROLL", item->annotation().RollOver());
		docu.writeAttribute("ANDOWN", item->annotation().Down());
		docu.writeAttribute("ANVIS", item->annotation().Vis());
		docu.writeAttribute("ANMC", item->annotation().MaxChar());
		docu.writeAttribute("ANCHK", item->annotation().IsChk());
		docu.writeAttribute("ANAA", item->annotation().AAact());
		docu.writeAttribute("ANCHKS", item->annotation().ChkStil());
		docu.writeAttribute("ANBCOL", item->annotation().borderColor());
		docu.writeAttribute("ANHTML", item->annotation().HTML());
		docu.writeAttribute("ANICON", item->annotation().UseIcons());
		docu.writeAttribute("ANPLACE", item->annotation().IPlace());
		docu.writeAttribute("ANSCALE", item->annotation().ScaleW());
		docu.writeAttribute("ANITYP", item->annotation().Icon());
		docu.writeAttribute("ANOPEN", item->annotation().IsAnOpen());
	}
	if (!item->AutoName)
		docu.writeAttribute("ANNAME", item->itemName());
	if (item->textFlowMode() != 0)
		docu.writeAttribute("TEXTFLOWMODE", (int) item->textFlowMode() );
	if (item->isTextFrame() || item->isPathText() || item->isImageFrame())
	{
		docu.writeAttribute("ImageScaleX", item->imageXScale());
		docu.writeAttribute("ImageScaleY", item->imageYScale());
		docu.writeAttribute("ImageOffsetX", item->imageXOffset());
		docu.writeAttribute("ImageOffsetY", item->imageYOffset());
		docu.writeAttribute("ImageRotation", item->imageRotation());
		docu.writeAttribute("ImageVisible", item->imageVisible() ? 1 : 0);
		docu.writeAttribute("ImageScaleType", item->ScaleType ? 1 : 0);
		docu.writeAttribute("ImageRatio", item->AspectRatio ? 1 : 0);
	}
	if (item->isTextFrame() || item->isPathText())
	{
		docu.writeAttribute("Columns", item->columns());
		docu.writeAttribute("ColumnGap", item->columnGap());
		docu.writeAttribute("AutomaticTextFrame", item->isAutoText ? 1 : 0);
		docu.writeAttribute("TextToFrameDistanceLeft", item->textToFrameDistLeft());
		docu.writeAttribute("TextToFrameDistanceTop", item->textToFrameDistTop());
		docu.writeAttribute("TextToFrameDistanceBottom", item->textToFrameDistBottom());
		docu.writeAttribute("TextToFrameDistanceRight", item->textToFrameDistRight());
		docu.writeAttribute("VerticalAlignment", item->verticalAlignment());
		docu.writeAttribute("FirstLineOffset", item->firstLineOffset()); // here I think this FLOP "cher à mon cœur" is legitimate!
		docu.writeAttribute("PLTSHOW", item->PoShow ? 1 : 0);
		docu.writeAttribute("BASEOF", item->BaseOffs);
		docu.writeAttribute("textPathType", item->textPathType);
		docu.writeAttribute("textPathFlipped", static_cast<int>(item->textPathFlipped));
	}
#ifdef HAVE_OSG
	if (((item->isImageFrame() && !(item->isLatexFrame() || item->isOSGFrame())) || (item->isTextFrame())) && (!item->Pfile.isEmpty()))
#else
	if (((item->isImageFrame() && !(item->isLatexFrame())) || (item->isTextFrame())) && (!item->Pfile.isEmpty()))
#endif
	{
		docu.writeAttribute("Pagenumber", item->pixm.imgInfo.actualPageNumber);
		if (item->isInlineImage)
		{
			docu.writeAttribute("PFILE", "");
			docu.writeAttribute("isInlineImage", static_cast<int>(item->isInlineImage));
			QFileInfo inlFi(item->Pfile);
			docu.writeAttribute("inlineImageExt", inlFi.suffix());
			QFile inFil(item->Pfile);
			if (inFil.open(QIODevice::ReadOnly))
			{
				QByteArray ba = qCompress(inFil.readAll()).toBase64();
				docu.writeAttribute("ImageData", QString(ba));
				inFil.close();
			}
		}
		else
			docu.writeAttribute("PFILE",Path2Relative(item->Pfile, baseDir));
	}
#ifdef HAVE_OSG
	else if (item->isOSGFrame())
	{
		if (!item->Pfile.isEmpty())
		{
			docu.writeAttribute("PFILE", "");
			docu.writeAttribute("isInlineImage", static_cast<int>(item->isInlineImage));
			QFileInfo inlFi(item->Pfile);
			docu.writeAttribute("inlineImageExt", inlFi.suffix());
			QFile inFil(item->Pfile);
			if (inFil.open(QIODevice::ReadOnly))
			{
				QByteArray ba = qCompress(inFil.readAll()).toBase64();
				docu.writeAttribute("ImageData", QString(ba));
				inFil.close();
			}
			PageItem_OSGFrame *osgframe = item->asOSGFrame();
			docu.writeAttribute("modelFile", Path2Relative(osgframe->modelFile, baseDir));
			docu.writeAttribute("currentViewName", osgframe->currentView);
		}
	}
#endif
	if (!item->Pfile2.isEmpty())
		docu.writeAttribute("PFILE2",Path2Relative(item->Pfile2, baseDir));
	if (!item->Pfile3.isEmpty())
		docu.writeAttribute("PFILE3",Path2Relative(item->Pfile3, baseDir));
	if (!item->ImageProfile.isEmpty())
		docu.writeAttribute("PRFILE", item->ImageProfile);
	if (item->ImageIntent != 1)
		docu.writeAttribute("IRENDER", item->ImageIntent);
	if (!item->EmbeddedProfile.isEmpty())
		docu.writeAttribute("EPROF", item->EmbeddedProfile);
	if (!item->UseEmbedded)
		docu.writeAttribute("EMBEDDED", 0);
	if (item->isImageFrame())
	{
		if (item->OverrideCompressionMethod)
			docu.writeAttribute("COMPRESSIONMETHOD", item->CompressionMethodIndex);
		if (item->OverrideCompressionQuality)
			docu.writeAttribute("COMPRESSIONQUALITY", item->CompressionQualityIndex);
	}
	if (item->locked())
		docu.writeAttribute("LOCK", 1);
	if (item->sizeLocked())
		docu.writeAttribute("LOCKR", 1);
	if (item->fillTransparency() != 0)
		docu.writeAttribute("TransValue", item->fillTransparency());
	if (item->lineTransparency() != 0)
		docu.writeAttribute("TransValueS", item->lineTransparency());
	if (item->fillBlendmode() != 0)
		docu.writeAttribute("TransBlend", item->fillBlendmode());
	if (item->lineBlendmode() != 0)
		docu.writeAttribute("TransBlendS", item->lineBlendmode());
	if (item->hasSoftShadow())
	{
		docu.writeAttribute("HASSOFTSHADOW", item->hasSoftShadow() ? 1 : 0);
		docu.writeAttribute("SOFTSHADOWXOFFSET", item->softShadowXOffset());
		docu.writeAttribute("SOFTSHADOWYOFFSET", item->softShadowYOffset());
		docu.writeAttribute("SOFTSHADOWCOLOR", item->softShadowColor());
		docu.writeAttribute("SOFTSHADOWBLURRADIUS", item->softShadowBlurRadius());
		docu.writeAttribute("SOFTSHADOWSHADE", item->softShadowShade());
		docu.writeAttribute("SOFTSHADOWBLENDMODE", item->softShadowBlendMode());
		docu.writeAttribute("SOFTSHADOWOPACITY", item->softShadowOpacity());
		docu.writeAttribute("SOFTSHADOWERASE", item->softShadowErasedByObject());
		docu.writeAttribute("SOFTSHADOWOBJTRANS", item->softShadowHasObjectTransparency());
	}

	QString tmp;
	if (item->isTable())
	{
		//ItemType == PageItem::Table or 16 (pageitem.h)
		const PageItem_Table* tableItem = item->asTable();
		docu.writeAttribute("Rows", tableItem->rows());
		docu.writeAttribute("Columns", tableItem->columns());

		QString outputData;
		//Row Positions
		for (double value : tableItem->rowPositions())
			outputData += tmp.setNum(value) + " ";
		docu.writeAttribute("RowPositions", outputData.simplified());
		outputData.clear();
		//Row Heights
		for (double value : tableItem->rowHeights())
			outputData += tmp.setNum(value) + " ";
		docu.writeAttribute("RowHeights", outputData.simplified());
		outputData.clear();
		//Column Positions
		for (double value : tableItem->columnPositions())
			outputData += tmp.setNum(value) + " ";
		docu.writeAttribute("ColumnPositions", outputData.simplified());
		outputData.clear();
		//Column Widths
		for (double value : tableItem->columnWidths())
			outputData += tmp.setNum(value) + " ";
		docu.writeAttribute("ColumnWidths", outputData.simplified());
		outputData.clear();
		//Cell Areas
		//TODO Is this the best format to write these out?
		for (const CellArea& ca : tableItem->cellAreas())
			outputData += QString::number(ca.row()) + " " + QString::number(ca.column()) + " " + QString::number(ca.height()) + " " + QString::number(ca.width()) + " ";
		docu.writeAttribute("CellAreas", outputData.simplified());
		outputData.clear();

		docu.writeAttribute("TableStyle", tableItem->styleName());
	}

	if (item->isGroup())
	{
		docu.writeAttribute("groupWidth", item->groupWidth);
		docu.writeAttribute("groupHeight", item->groupHeight);
		docu.writeAttribute("groupClips", item->groupClipping());
	}
	if (!item->DashValues.isEmpty())
	{
		docu.writeAttribute("DashValues", item->DashValues.count());
		QString dlp;
		for (auto dashValue : item->DashValues)
			dlp += QString::number(dashValue) + " ";
		docu.writeAttribute("Dashes", dlp);
		docu.writeAttribute("DashOffset", item->DashOffset);
	}
	if (!(item->isArc() || item->isSpiral() || item->isRegularPolygon()))
		docu.writeAttribute("path", item->PoLine.svgPath(!(item->isPolyLine() || item->isPathText())));
	QString colp = item->ContourLine.svgPath(true);
	if (!colp.isEmpty())
		docu.writeAttribute("copath", colp);
	if (item->isLine() || item->isPolyLine() || item->isSpiral())
	{
		if (item->startArrowIndex() != 0)
			docu.writeAttribute("startArrowIndex", item->startArrowIndex());
		if (item->endArrowIndex() != 0)
			docu.writeAttribute("endArrowIndex", item->endArrowIndex());
		if (item->startArrowScale() != 100)
			docu.writeAttribute("startArrowScale", item->startArrowScale());
		if (item->endArrowScale() != 100)
			docu.writeAttribute("endArrowScale", item->endArrowScale());
	}
}
