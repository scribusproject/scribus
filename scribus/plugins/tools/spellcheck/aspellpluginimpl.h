/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ASPELLPLUGINIMPL_H
#define ASPELLPLUGINIMPL_H

// ISO C++ include files
#include <string>        // For replacement word from aspell
#include <vector>        // For array of replacements from aspell
// QT include files
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QHash>
// Scribus include files
#include "scribusdoc.h"           // For ScribusDoc
#include "scribus.h"              // For ScribusMainWindow
#include "selection.h"            // Item selected
#include "prefsfile.h"            // Stored plugin preferences
#include "prefsmanager.h"         // Preferences manager
// Local include files
#include "ui_aspellpluginbase.h"  // For GUI from QT4 designer
// #include "donedlgimpl.h"          // For spell-checking done dialog
#include "suggest.h"              // For aspell interface class

/*!
\class AspellPluginImpl
\author Gora Mohanty <gora@srijan.in>
\brief Implementation of plugin. GUI part is derived from AspellPluginBase class, as aspellpluginbase.{cpp,h} get overwritten by uic. Interface to aspell uses Speller::Aspell::Suggest
 */
class AspellPluginImpl : public QDialog, private Ui::AspellPluginBase
{
	Q_OBJECT
private:
	/*! \brief Context name for storing preferences */
	static const char* kDEF_CONTEXT;
	/*! Formatted string for aspell dictionary entry: Form of <name>--<lang>--<jargon>--<size> */
	static const QString kDEF_ASPELL_ENTRY;
	/*! \brief Aspell interface object. */
	Speller::Aspell::Suggest* fsuggest;
	/*! \brief Scribus preferences object. */
	PrefsContext* fprefs;
	/*! \brief Language for aspell dictionary. */
	QString   flang;
	/*! \brief Jargon for aspell dictionary. */
	QString   fjargon;
	/*! \brief Character encoding for words. */
	QString   fencoding;
	/*! \brief Formatted string for aspell dictionary entry: Form of <name>--<lang>--<jargon>--<size> */
	QString   fentry;
	/*! \brief Pointer to current document. */
	ScribusDoc* fdoc;
	/*! \brief Structure to keep track of number of changes made. */
	struct Changes
	{
		/*! brief Number of words changed in this frame. */
		uint fnwords;
		/*! brief Number of words changed in all. */
		uint fntot;
		/*! brief Number of frames changed. */
		uint fnframes;
		/*!
		  \author Gora Mohanty <gora@srijan.in>
		  \brief Constructor for counts of items changes
		  \param nwords: No. of words changed in this frame.
		  \param ntot: Total no. of words changed.
		  \param nframes: No. of frames changed.
		  \retval None
		*/
		Changes(uint nwords=0, uint ntot=0, uint nframes=0) :
			fnwords(nwords), fntot(ntot), fnframes(nframes) {}
	} fnchanges;
	/*! \brief Extracted string of text from one frame. */
	QString fcontent;
	/*! \brief Position in 'fcontent' of word being spell-checked. */
	uint fpos;
	QHash<QString, QString> rememberedWords;
	/*! \brief Index in 'fcontent' of word being spell-checked. */
	int fidx;
	/*! \brief Word currently being spell-checked. */
	QStringList fwordlist;
	/*! \brief Pointer to current frame. */
	PageItem* fFrame;
	/*! \brief Model for list of dictionaries. */

protected:
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Initializes the position in the current text, sets the current word, and starts spell-checking.
	  \param None
	  \retval None
	*/
	void checkText();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Skips to the beginning of the next word in 'fcontent', the text being spell-checked.
	  \param None
	  \retval None
	*/
	void nextWord();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Save user preferences
	  \param language: Language for aspell dictionary
	  \param jargon: Jargon for aspell dictionary
	  \param encoding: Encoding for aspell dictionary
	  \retval None
	*/
	void setPreferences(const QString& lang,
			    const QString& jargon,
			    const QString& encoding=Speller::Aspell::Suggest::kDEF_ENCODING,
			    const QString& entry=kDEF_ASPELL_ENTRY);
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Retrieve saved user preferences: language, jargon, encoding.
	  \param None
	  \retval None
	*/
	void getPreferences();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Activates spell-checking GUI elements in spell-checking tab, i.e., everything except combo box at top
	  \param None
	  \retval None
	*/
	void activateSpellGUI();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Deactivates spell-checking GUI elements in spell-checking tab, i.e., everything except combo box at top
	  \param None
	  \retval None
	*/
	void deactivateSpellGUI();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Base method to spell-check text in current frame.
	  \param None
	  \retval None
	*/
	void parseItem();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Spell-check selected frames in the page
	  \param None
	  \retval None
	*/
        void parseSelection();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Called when spell-checking is completed. Pops up an information dialog.
	  \param None
	  \retval None
	*/
	void spellCheckDone();

public:
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Constructor for spell-checking plugin implementation.
	  \param doc: Scribus doc
	  \param parent: Parent window that this is a child of.
	  \retval None
	*/
	AspellPluginImpl(ScribusDoc* doc, QWidget* parent=NULL);
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Destructor for spell-checking plugin implementation.
	  \param None
	  \retval None
	*/
	~AspellPluginImpl();

public slots:
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Slot: Called when the "Close" button is clicked. Makes any pending replacements, and closes spell-check window.
	  \param None
	  \retval None
	*/
	void on_fcloseBtn_clicked();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Slot: Called when the "Change" button is clicked. Replaces the word being checked with the currently selected replacement.
	  \param None
	  \retval None
	*/
	void on_fchangeBtn_clicked();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Slot: Called when the "Change All" button is clicked. Replaces all occurrences of the word being checked with the currently selected replacement.
	  \param None
	  \retval None
	*/
	void on_fchangeAllBtn_clicked();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Slot: Called when the "Skip" button is clicked. Skips the word currently being spell-checked.
	  \param None
	  \retval None
	*/
	void on_fskipBtn_clicked();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Slot: Called when the "Skip All" button is clicked. Skips all occurences of the word currently being spell-checked.
	  \param None
	  \retval None
	*/
	void on_fskipAllBtn_clicked();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Slot: Called when the "Add word" button is clicked. Adds current suggestion to user's personal directory.
	  \param None
	  \retval None
	*/
	void on_faddWordBtn_clicked();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Slot: Called when an item in the list of aspell suggestions is highlighted. Changes the current word (in the text edit box).
	  \param None
	  \retval None
	*/
	void on_flistReplacements_itemActivated();
	/*!
	  \author Gora Mohanty <gora@srijan.in>
	  \brief Slot: Called when an item in the list of available aspell dictionaries is selected. Resets aspell dictionary to selected one.
	  \param None
	  \retval None
	*/
	void on_flistDicts_activated();

protected slots:
    void languageChange();

};
#endif  // #ifndef ASPELLPLUGINIMPL_H
//@@@@@@@@@@@@@@@@@@@@@@@@@ END OF FILE @@@@@@@@@@@@@@@@@@@@@@@@@
