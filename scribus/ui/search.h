/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SEARCHREPLACE_H
#define SEARCHREPLACE_H

#include "ui_searchbase.h"

#include "scribusapi.h"

class PrefsContext;
class ParagraphStyle;
class CharStyle;

class ScribusDoc;
class PageItem;

class SCRIBUS_API SearchReplace : public QDialog, Ui::SearchReplaceBase
{
	Q_OBJECT

private:
	struct Options 
	{
		bool alignmentEnabled { false };
		bool fillColorEnabled { false };
		bool fillShadeEnabled { false };
		bool fontEffectsEnabled { false };
		bool fontEnabled { false };
		bool fontSizeEnabled { false };
		bool ignoreCase { false };
		bool paragraphStyleEnabled { false };
		bool strokeColorEnabled { false };
		bool strokeShadeEnabled { false };
		bool textEnabled { false };
		bool wholeWords { false };

		QString fillColor;
		QString font;
		QString strokeColor;
		QString text;

		int alignment { 0 };
		int fillColorId { 0 };
		int fillShade { 100 };
		int fontEffects { 0 };
		int fontSize { 0 };
		int paragraphStyle { 0 };
		int strokeColorId { 0 };
		int strokeShade { 100 };
	};

	struct MatchRange
	{
		int start;
		int end;
		int nextPosition;
	};

public:
	// mode is false when calling from the story editor
	SearchReplace( QWidget* parent, ScribusDoc *doc);
	~SearchReplace() {};

	void setStoryEditorMode(bool mode = true) {m_storyEditorMode = true;}
	QPair<int, int> cursorPosition();
	//! \brief fill the text field with the current selection, if the selection does not contain a newline.
	void processCurrentSelection(QString selection);

public slots:
	void slotCollapseFormat();
	void slotSearch();
	void slotReplace();
	void slotReplaceAll();
	void slotStop();
	void clear();
	void accept() override;

	void enableTextSearch();
	void enableStyleSearch();
	void enableAlignmentSearch();
	void enableFontSearch();
	void enableFontSizeSearch();
	void enableFontEffectsSearch();
	void enableFillColorSearch();
	void enableFillShadeSearch();
	void enableStrokeColorSearch();
	void enableStrokeShadeSearch();
	void enableStyleReplace();
	void enableAlignmentReplace();
	void enableFontReplace();
	void enableFontSizeReplace();
	void enableFontEffectsReplace();
	void enableFillColorReplace();
	void enableFillShadeReplace();
	void enableStrokeColorReplace();
	void enableStrokeShadeReplace();

	void updateButtonState();

private:
	ScribusDoc* m_doc;

	bool m_storyEditorMode { false };
	bool m_itemMode;
	bool m_stateCollapsed { true };

	PrefsContext* m_prefs;

	void collapseFormat();
	void readPrefs();
	void writePrefs();

	void hideMessage();
	void showMessage(const QString& message = QString());
	const char* messageNoMatch = "No match found";
	const char* messageEndOfSelection = "Reached the end of the selection";
	const char* messageOccurrences = "%1 occurrences replaced";


	void searchInStoryEditor();
	void searchOnCanvas();
	//! /brief Search storyText starting from start.
	//! /param length The length of the story
	//! /return The start and end position of the selection or (-1, -1) if nothing found.
	QPair<int, int> searchStory(const StoryText& storyText, int start, const int length, const Options& options);

	//! \brief Check if there is a match starting at position
	//!
	//! When textEnabled, the resulting start position might be different than position.
	MatchRange getMatchRange(const StoryText& storyText, int position, const int length, const Options& options);
	//! If the current selection matches the search, replace the selection.
	// Otherwise do a search.
	void replaceInStoryEditor();
	void replaceOnCanvas();
	//! \brief if the current selection matches the search, replace the selection.
	// otherwise do a search.
	void replaceSelectionInStoryEditor();
	void replaceSelectionOnCanvas();
	void replaceAllInStoryEditor();
	void replaceAllOnCanvas();

	//! \brief Initialize pageItems based on the current selection
	void initCanvasSelection();
	//! \brief Put all the document's text frames into pageItems
	void readAllPageItems();
	//! \brief Put all the selected text frames into pageItems
	void readSelectedPageItems();
	PageItem* currentPageItem();
	void nextPageItem();
	void selectPageItem(PageItem* pageItem);

	std::vector<PageItem*> m_pageItems {};
	size_t m_currentPageItem { 0 };
	bool m_endReached { false };

	bool m_found { false };
	bool m_searching { false };
	//! Number of matches found (thus far) in a search
	int m_matchesFound { 0 };

	void updatePageItemSelection(PageItem* pageItem);

	// \brief is any search checkbox checked?
	bool anySearchChecked();
	// \brief is any replace checkbox checked?
	bool anyReplaceChecked();
	Options getSearchOptions();
	Options getReplaceOptions();
	// \brief Check for all search options but the text
	bool isFormatMatching(const ParagraphStyle& parStyle, const CharStyle& charStyle, const Options& options);

	// TODO: isn't 0, 0 good enough?
	int m_selectionStart { -1} ;
	int m_selectionEnd { -1 };
};

#endif // SEARCHREPLACE_H
