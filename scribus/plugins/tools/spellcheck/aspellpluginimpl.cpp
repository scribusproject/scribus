/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "aspellpluginimpl.h"
#include "pageitem_textframe.h"
#include <QMessageBox>
const char* AspellPluginImpl::kDEF_CONTEXT = "AspellPlugin";
const QString AspellPluginImpl::kDEF_ASPELL_ENTRY =
	QString( "en" ) + Speller::Aspell::Suggest::kDICT_DELIM +
	QString( "en" ) + Speller::Aspell::Suggest::kDICT_DELIM +
	QString( "*" ) + Speller::Aspell::Suggest::kDICT_DELIM +
	QString( "60" );  // I.e., en/en/*/60 corresponding to default English dictionary

// Initialize members here, if any
AspellPluginImpl::AspellPluginImpl(ScribusDoc* doc, QWidget* parent) :
	QDialog( parent ), fdoc( doc ), fpos(0), fidx(0)
{
	// Constructor.
	setupUi( this );
	setModal( true );

	rememberedWords.clear();
	// Fill in choices in combo box for text to be spell-checked.
//	ftextSelect->insertItem( 0, "None" );
//	ftextSelect->insertItem( 1, "Selected frame" );
//	ftextSelect->insertItem( 2, "Active page" );
//	ftextSelect->insertItem( 3, "All items" );

	// Clear out default suggestions listbox entry: "New item"

	// Get stored language, jargon, encoding settings
	fprefs = PrefsManager::instance()->prefsFile->getPluginContext( kDEF_CONTEXT );
	getPreferences();

	// Start with settings tab, if dictionary had not been selected
	// earlier.
	fmainTab->setCurrentIndex( 0 );
	QString text = tr( "Loaded " ) + (fentry == kDEF_ASPELL_ENTRY ? tr( "default " ) : "") + fentry + tr( " aspell dictionary." );
	doc->scMW()->setStatusBarInfoText( text );

	ftextDictDetails->setText( fentry );

	try
	{
		// Deactivate GUI elements in spell-checking tab until an
		// aspell dictionary is chosen.
		deactivateSpellGUI();

		// Create speller, and get list of available aspell
		// dictionaries.
		fsuggest = new Speller::Aspell::Suggest( flang.toUtf8().data(), fjargon.toUtf8().data(), fencoding.toUtf8().data() );
		Q_CHECK_PTR( fsuggest );

		// Get list of available aspell dictionaries
		std::vector<std::string> entries;
		fsuggest->listDicts( entries );
		unsigned int idx = 0;
		for( std::vector<std::string>::const_iterator i = entries.begin(); i != entries.end(); ++i )
		{
			// FIXME: Handle encodings other than UTF-8.
			flistDicts->insertItem( idx, i->c_str() );
			idx++;
		}
	}
	catch( const std::invalid_argument& err )
	{
		QString warn = tr( "aspellplugin (AspellPluginImpl::"
				   "AspellPluginImpl): Error in aspell "
				   "speller configuration." );
		qWarning( "%s", warn.toUtf8().data() );
	}
	catch( const std::runtime_error& err )
	{
		QString warn = tr( "aspellplugin (AspellPluginImpl::"
				   "AspellPluginImpl): Error in creating "
				   "aspell speller." );
		qWarning( "%s", warn.toUtf8().data() );
	}
	activateSpellGUI();
	parseSelection();
}
//__________________________________________________________________________
AspellPluginImpl::~AspellPluginImpl()
{
	// Destructor
	try
	{
		fsuggest->saveLists();
	}
	catch( const std::runtime_error& err )
	{
		qWarning( "aspellplugin (AspellPluginImpl::~AspellPlugin"
			  "Impl): Error in saving aspell word lists." );
	}

	delete fsuggest;
}
//__________________________________________________________________________
void AspellPluginImpl::activateAll()
{
        // Activates all GUI elements in spell-checking tab. Called once an
        // aspell dictionary has been selected.
        fmainTab->widget(0)->setEnabled( true );
}
//__________________________________________________________________________
void AspellPluginImpl::activateSpellGUI()
{
	// Activates spell-checking GUI elements in spell-checking
	// tab, i.e., everything except combo box at top
	fcurrWord->setEnabled( true );
	flistReplacements->setEnabled( true );
	fchangeBtn->setEnabled( true );
	fchangeAllBtn->setEnabled( true );
	fskipAllBtn->setEnabled( true );
	fskipBtn->setEnabled( true );
	faddWordBtn->setEnabled( true );
	ftextLabel1->setEnabled( true );
	ftextLabel2->setEnabled( true );
	ftextLabel3->setEnabled( true );
	ftextLabel4->setEnabled( true );
	fmisSpelling->setEnabled( true );
	ftextDictDetails->setEnabled( true );
}
//__________________________________________________________________________
void AspellPluginImpl::activateTextSelect()
{
	// Activates text select combo box at top of spell-checking tab.
//	fgroupSelect->setEnabled( true );
//	ftextSelect->setEnabled( true );
}
//__________________________________________________________________________
void AspellPluginImpl::deactivateAll()
{
	// Deactivates all GUI elements in spell-checking tab,
	fmainTab->widget(0)->setEnabled( false );
}
//__________________________________________________________________________
void AspellPluginImpl::deactivateSpellGUI()
{
	// Deactivates spell-checking GUI elements in spell-checking tab,
	// i.e., everything except combo box at top
	fcurrWord->setEnabled( false );
	flistReplacements->setEnabled( false );
	fchangeBtn->setEnabled( false );
	fchangeAllBtn->setEnabled( false );
	fskipAllBtn->setEnabled( false );
	fskipBtn->setEnabled( false );
	faddWordBtn->setEnabled( false );
	ftextLabel1->setEnabled( false );
	ftextLabel2->setEnabled( false );
	ftextLabel3->setEnabled( false );
	ftextLabel4->setEnabled( false );
	fmisSpelling->setEnabled( false );
	ftextDictDetails->setEnabled( false );
}
//__________________________________________________________________________
void AspellPluginImpl::deactivateTextSelect()
{
	// Deactivates text select combo box at top of spell-checking tab.
//	fgroupSelect->setEnabled( false );
//	ftextSelect->setEnabled( false );
}
//__________________________________________________________________________
void AspellPluginImpl::nextWord()
{
	QString wordBoundaries = QString(" .,:;\"'!?\n");
	uint len = fFrame->itemText.length();
	while (fpos < len && wordBoundaries.indexOf(fFrame->itemText.text(fpos)) >= 0)
	{
		++fpos;
	}
	int pa = fpos;
	int pe = 0;
	while (fpos < len && wordBoundaries.indexOf(fFrame->itemText.text(fpos)) < 0)
	{
		++fpos;
	}
	pe = fpos;
	fcontent = fFrame->itemText.text(pa, pe - pa);
	fpos = pa;
	// Called to skip to the beginning of the next word. 'fpos' is
	// set to the position in 'fcontent' of the beginning of this
	// word, and 'fidx' is set to the index of this word in 'fwordlist'.
//#if 0
//	std::cerr << "nextWord: before, pos = " << fpos << ", length = "
//		  << fwordlist[fidx].length() << "\n";
//#endif
	// FIXME: Regexp searches are probably expensive. So, we should
	// replace these with something else. Certainly the first find()
	// can be replaced by adding the length of the word just replaced
	// to 'fpos'.
	// Skip to next whitespace character, i.e., end of last word.
//	fpos = fcontent.indexOf( QRegExp( "\\s|\\W|\\d|\\n|\\r|\\t" ), fpos );
	// Skip to next non-whitespace character, i.e., beginning of
	// next word.
//	fpos = fcontent.indexOf( QRegExp( "\\w" ), fpos );
//#if 0
//	std::cerr << "nextWord: after, pos = " << fpos << "\n";
//#endif
//	fidx++;  // Increment index in 'fcontent'
}
//__________________________________________________________________________
void AspellPluginImpl::checkText()
{
	// Called from parseXXX(), after filling in the currently
	// relevant text into 'fcontent'. Handles spell-checking of
	// the text.
	while (fpos < static_cast<uint>(fFrame->itemText.length()))
	{
		std::vector<std::string> replacement;
		bool status = fsuggest->checkWord(fcontent.toUtf8().data(), replacement);
		if (!status)
		{
			fmisSpelling->setText( fcontent );
			fcurrWord->setText( "" );
			flistReplacements->clear();
			unsigned int idx = 0;
			for( std::vector<std::string>::const_iterator i = replacement.begin(); i != replacement.end(); ++i )
			{
			// FIXME: Handle encodings other than UTF-8.
				QString dict = QString::fromUtf8(i->c_str());
				flistReplacements->insertItem( idx, dict );
				idx++;
			}
			if( flistReplacements->count() > 0 )
			{
				// FIXME: Is this the correct substitute for
				// setSelected()?
				flistReplacements->setCurrentRow( 0 );
				fcurrWord->setText( flistReplacements->currentItem()->text() );
			}
			if (rememberedWords.contains(fcontent))
			{
				QString repl = rememberedWords.value(fcontent);
				int cs, cx;
				if (fcontent.length() == repl.length())
				{
					for (cs = 0; cs < fcontent.length(); ++cs)
						fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
				}
				else
				{
					if (fcontent.length() < repl.length())
					{
						for (cs = 0; cs < fcontent.length(); ++cs)
							fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
						for (cx = cs; cx < repl.length(); ++cx)
							fFrame->itemText.insertChars(fpos+cx, repl.mid(cx,1), true);
					}
					else
					{
						for (cs = 0; cs < repl.length(); ++cs)
							fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
						fFrame->itemText.removeChars(fpos+cs, fcontent.length() - cs);
					}
				}
			}
			else
				break;
		}
		fpos += fcontent.length();
		nextWord();
	}
}
//__________________________________________________________________________
void AspellPluginImpl::spellCheckDone()
{
	// Called once all words in the current text, i.e., in 'fcontent'
	// have been spell-checked. Pops up an information dialog.
	QMessageBox::information(fdoc->scMW(), tr("Spell-Checker"), tr("Spell-checking completed."));
	if( fFrame && fFrame->asTextFrame() )
		fFrame->asTextFrame()->invalidateLayout();
	if( fnchanges.fntot > 0 )
	{
		fdoc->changed();
	}
	// Redraw document
	fdoc->view()->DrawNew();
	QApplication::restoreOverrideCursor();
	fdoc->scMW()->setStatusBarInfoText(tr("Spell-checking done." ));
	fdoc->scMW()->mainWindowProgressBar->reset();
	close();
}
//__________________________________________________________________________
void AspellPluginImpl::checkWord(const QString& word)
{
	// Called when "Spell-check" button is clicked, or return pressed in
	// text entry box.  Spell-checks current word (in the text entry box).
	std::vector<std::string> replacement;
	bool status = fsuggest->checkWord(word.toUtf8().data(), replacement);
#if 0
	std::cerr << "checkWord: checked \"" << word << "\". "
	  "Status = " << (status ? 1 : 0) << '\n';
#endif
	if( status )
	{
		// FIXME: Recursive calls
		nextWord();
		checkText();
	}
	else
	{
		fmisSpelling->setText( fwordlist[fidx] );
		fcurrWord->setText( "" );
		flistReplacements->clear();
		unsigned int idx = 0;
		for( std::vector<std::string>::const_iterator i =
			     replacement.begin();
		     i != replacement.end();
		     ++i )
		{
			// FIXME: Handle encodings other than UTF-8.
			QString dict = QString::fromUtf8(i->c_str());
			flistReplacements->insertItem( idx, dict );
			idx++;
		}

		if( flistReplacements->count() > 0 )
		{
			// FIXME: Is this the correct substitute for
			// setSelected()?
			flistReplacements->setCurrentRow( 0 );
			fcurrWord->setText( flistReplacements->currentItem()->text() );
		}
	}
}
//__________________________________________________________________________
void AspellPluginImpl::on_fcloseBtn_clicked()
{
	// Called when the "Close" button is clicked. Makes any pending
	// replacements and closes the spell-checking window.
#if 0
	std::cerr << "on_fcloseBtn_clicked: Making replacements.\n";
#endif
	spellCheckDone();  // Also closes spell-checking window.
}
//__________________________________________________________________________
void AspellPluginImpl::on_fchangeBtn_clicked()
{
	// Called when the "Change" button is clicked. Replaces the word
	// being spell-checked with the current word in text edit box.
	// FIXME: Handle encodings other than UTF-8.
	QString repl = fcurrWord->toPlainText();
	int cs, cx;
	if (fcontent.length() == repl.length())
	{
		for (cs = 0; cs < fcontent.length(); ++cs)
			fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
	}
	else
	{
		if (fcontent.length() < repl.length())
		{
			for (cs = 0; cs < fcontent.length(); ++cs)
				fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
			for (cx = cs; cx < repl.length(); ++cx)
				fFrame->itemText.insertChars(fpos+cx, repl.mid(cx,1), true);
		}
		else
		{
			for (cs = 0; cs < repl.length(); ++cs)
				fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
			fFrame->itemText.removeChars(fpos+cs, fcontent.length() - cs);
		}
	}
	fpos += fcontent.length();
	nextWord();
	checkText();
}
//__________________________________________________________________________
void AspellPluginImpl::on_fchangeAllBtn_clicked()
{
	// Called when the "Change All" button is clicked. Replaces all
	// instances of the word being spell-checked with the current word in
	// text edit box.
	// FIXME: Change commented-out statement below
	// fnchanges.fnwords += fcontent.contains( fwordlist[fidx] );
	// FIXME: Is this the right replacement for fcurrWord->text()?
	QString repl = fcurrWord->toPlainText();
	int cs, cx;
	if (fcontent.length() == repl.length())
	{
		for (cs = 0; cs < fcontent.length(); ++cs)
			fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
	}
	else
	{
		if (fcontent.length() < repl.length())
		{
			for (cs = 0; cs < fcontent.length(); ++cs)
				fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
			for (cx = cs; cx < repl.length(); ++cx)
				fFrame->itemText.insertChars(fpos+cx, repl.mid(cx,1), true);
		}
		else
		{
			for (cs = 0; cs < repl.length(); ++cs)
				fFrame->itemText.replaceChar(fpos+cs, repl[cs]);
			fFrame->itemText.removeChars(fpos+cs, fcontent.length() - cs);
		}
	}
	if (!rememberedWords.contains(fcontent))
		rememberedWords.insert(fcontent, repl);
	fpos += fcontent.length();
	nextWord();  // Go to next word
	checkText();
}
//__________________________________________________________________________
void AspellPluginImpl::on_fskipBtn_clicked()
{
	// Called when the "Skip" button is clicked. Skips the word currently
	// being spell-checked.
	fpos += fcontent.length();
	nextWord();  // Go to next word
	checkText();
}
//__________________________________________________________________________
void AspellPluginImpl::on_fskipAllBtn_clicked()
{
	// Called when the "Skip All" button is clicked. Puts the word
	// currently being spell-checked into the list of words to be ignored
	// for this session.
	try
	{
#if 0
		std::cerr << "on_fskipAllBtn_clicked: Ignoring \"" <<
		  fwordlist[fidx] << "\"\n";
#endif
		fsuggest->ignoreWord( fcontent.toUtf8().data() );
	}
	catch( const std::runtime_error& err )
	{
		
		QString warn =
		  tr( "AspellPluginImpl::on_fskipAllBtn_clicked(): Unable "
		      "to skip all instances of \"" ) + fcontent +
		  tr(" by adding it to the session list.");
		qWarning( "%s", warn.toUtf8().data() );
	}
	fpos += fcontent.length();
	nextWord();  // Go to next word
	checkText();
}
//__________________________________________________________________________
void AspellPluginImpl::on_faddWordBtn_clicked()
{
	// Called when the "Add word" button is clicked. Adds word to personal
	// word list.
	try
	{
		// FIXME: Handle encodings other than UTF-8.
		// FIXME: Is this the right replacement for fcurrWord->text()?
		fsuggest->addPersonalList( fcurrWord->toPlainText().toUtf8().data() );
	}
	catch( const std::runtime_error& err )
	{
		QString warn =
			tr( "AspellPluginImpl::on_faddWordBtn_clicked(): "
			    "Unable to add word to personal list." );
		qWarning( "%s", warn.toUtf8().data() );
	}
}
//__________________________________________________________________________
void AspellPluginImpl::on_flistReplacements_itemActivated()
{
	// Called when an item in the list of replacements is
	// selected. Replaces current word in text edit box with the text
	// of the selected item. 
	fcurrWord->setText( flistReplacements->currentItem()->text() );
}
//__________________________________________________________________________
void AspellPluginImpl::on_flistDicts_itemActivated()
{
	// Called when an item in the list of available aspell dictionaries is
	// selected, i.e., by double-clicking, or pressing enter. Resets
	// aspell configuration to use the selected dictionary.
	QString entry = flistDicts->currentItem()->text();
	QStringList fields = entry.split( Speller::Aspell::Suggest::kDICT_DELIM );
	// Ensure that we have at least the right no.of fields.
	if( fields.size() == 4 )
	{
		QString value =
			fields[0] + Speller::Aspell::Suggest::kDICT_DELIM +
			fields[1] + Speller::Aspell::Suggest::kDICT_DELIM +
			fields[2] + Speller::Aspell::Suggest::kDICT_DELIM +
			fields[3];
		ftextDictDetails->setText( value );
		fsuggest->resetConfig( fields[1].toAscii().data(),
				       fields[2].toAscii().data() );
		// FIXME: Handle encodings other than UTF-8.
		setPreferences( fields[1], fields[2], Speller::Aspell::Suggest::kDEF_ENCODING, value );
	}

	// Activate text select combo box at top of spell-checking tab.
//	activateTextSelect();
	fmainTab->setCurrentIndex( 0 );  // Switch to spell-checking tab.
}
//__________________________________________________________________________
void AspellPluginImpl::getPreferences()
{
	// Retrieves user preferences from saved settings. Defaults are
	// supplied 
	flang =	fprefs->get( "lang", Speller::Aspell::Suggest::kDEF_LANG );
	fjargon = fprefs->get( "jargon",
			       Speller::Aspell::Suggest::kDEF_JARGON );
	// FIXME: Handle encodings other than UTF-8.
	fencoding = fprefs->get( "encoding",
				 Speller::Aspell::Suggest::kDEF_ENCODING );
	fentry = fprefs->get( "entry", kDEF_ASPELL_ENTRY );
}
//__________________________________________________________________________
void AspellPluginImpl::setPreferences(const QString& lang,
				      const QString& jargon,
				      const QString& encoding,
				      const QString& entry)
{
	// Saves user preferences using Scribus preferences manager.
	fprefs->set( "lang", lang );
	QString val = jargon == Speller::Aspell::Suggest::kEMPTY ? "" :
		jargon;
	fprefs->set( "jargon", val );
	fprefs->set( "encoding", encoding );
	fprefs->set( "entry", entry );
}
//__________________________________________________________________________
void AspellPluginImpl::languageChange()
{
	qWarning( "AspellPluginImpl::languageChange(): Not implemented yet" );
}
//__________________________________________________________________________
void AspellPluginImpl::on_ftextSelect_activated(int key)
{
	// Called when item is chosen from combo box at top, selecting what
	// items to applyspell-scheking to.
	QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
	fdoc->scMW()->setStatusBarInfoText( tr( "Spell-checking in "
					       "progress. Please "
					       "wait..." ) );
	// Activate spell-checking GUI elements, and deactivate top combo
	// box.
	activateSpellGUI();
//	deactivateTextSelect();

	// Initialise counters for no. of changes. */
	fnchanges.fntot = fnchanges.fnframes = 0;

	// The numbering of the keys follows the order in which they were
	// inserted into the combo box. 0 is "None", the default choice.
	if( key == 1 )
	{
		parseSelection();
	}
	else if( key == 2 )
	{
		parsePage();

	}
	else if( key == 3 )
	{
		parseAll();
	}

	// Enable "Save" icon
	if( fnchanges.fntot > 0 )
	{
		fdoc->changed();
	}

	// Redraw document
	fdoc->view()->DrawNew();

	QApplication::restoreOverrideCursor();
	fdoc->scMW()->setStatusBarInfoText(tr("Spell-checking done." ));
	fdoc->scMW()->mainWindowProgressBar->reset();
}
//__________________________________________________________________________
void AspellPluginImpl::makeReplacements()
{
//	if( fnchanges.fnwords > 0 && fFrame && fFrame->asTextFrame() )
//	{
#if 0
		std::cerr << "makeReplacements: Replacing\n\"" <<
			fFrame->itemText.text( 0, fFrame->itemText.length() )
			  << "\"\nwith\n\"" << fcontent << "\"\n";
#endif
		// Remove all text, and replace it with the modified text.
		// FIXME: What does fFrame->frameDisplays() do? Should we be
		// concerned about it.
//		fFrame->itemText.removeChars( 0, fFrame->itemText.length() );
//		fFrame->itemText.insertChars( 0, fcontent );
//		fnchanges.fnframes++;
#if 0
		std::cerr << "makeReplacements: made replacements\n";
#endif
//	}

	// Done with spell-checking at this point.
#if 0
	if( ftextSelect->currentItem() == 1 )
	{
		std::cerr << "makeReplacements: Spell-checking finished\n";
#endif
		spellCheckDone();
#if 0
	}
#endif
}
//__________________________________________________________________________
void AspellPluginImpl::parseItem()
{
	// Parse text in a frame, and spell-check it.

	// Process only text frames
	if( fFrame && fFrame->asTextFrame() )
	{
		nextWord();
		checkText();
		// Process text from
		// frame.
		// FIXME: There must be a smarter method to get individual
		// words from Scribus text, than pulling out characters into a
		// string, and then parsing that.
		// FIXME: Should we worry about inordinately long strings of
		// text?
//		int i;
//		for( i = 0; i < fFrame->itemText.length() &&
//			     ! fFrame->frameDisplays( i ); ++i )
//		{
			;  // Do nothing.
//		}
//		for( ; i < fFrame->itemText.length() && fFrame->frameDisplays(i); ++i  )
//		{
//			fcontent += fFrame->itemText.text( i, 1 );
//		}

//		fnchanges.fnframes++;
//#if 0
//		std::cerr << "parseItem: fcontent = \"" << fcontent <<
//		  "\"\n";
//#endif
//		fwordlist = fcontent.split( QRegExp( "\\s|\\W|\\d|\\n|\\r|\\t" ) );
//#if 0
//		for( size_t i = 0; i < fwordlist.size(); i++ )
//		{
//			std::cerr << "parseItem: fwordlist[" << i << "] = \""
//				  << fwordlist[i] << "\"\n";
//		}
//#endif
//		fpos = fidx = 0;
//		checkText();
	}
}
//__________________________________________________________________________
void AspellPluginImpl::parseSelection()
{
	fcontent.truncate( 0 );  // Start with empty string
	uint ndocs = fdoc->m_Selection->count();
        //fdoc->scMW()->mainWindowProgressBar->setTotalSteps( ndocs );
	for( uint i = 0; i < ndocs; ++i )
	{
//		fnchanges.fnwords = 0;
		//fdoc->scMW()->mainWindowProgressBar->setProgress( i );
		fFrame = fdoc->m_Selection->itemAt( i );
		parseItem();
//		fnchanges.fntot += fnchanges.fnwords;
	}
        //fdoc->scMW()->mainWindowProgressBar->setProgress( ndocs );
}
//__________________________________________________________________________
void AspellPluginImpl::parsePage()
{
	parsePage( fdoc->currentPageNumber() );
	//spellCheckDone();
}
//__________________________________________________________________________
void AspellPluginImpl::parsePage(int page)
{
	fcontent.truncate( 0 );  // Start with empty string
	uint nproc = 0;
        uint ndocs = fdoc->Items->count();
        for( uint i = 0; i < ndocs; ++i )
        {
                if( fdoc->Items->at( i )->OwnPage == page )
		{
                        ++nproc;
		}
        }

        //fdoc->scMW()->mainWindowProgressBar->setTotalSteps( nproc );
        fdoc->view()->GotoPage( page );
        //uint n = 0;
        for( uint i = 0; i < ndocs; ++i )
        {
                PageItem* b = fdoc->Items->at( i );
                if( b->OwnPage == page )
                {
                        //fdoc->scMW()->mainWindowProgressBar->setProgress(++n);
			fFrame = b;
                        parseItem();
			fnchanges.fntot += fnchanges.fnwords;
                }
        }
        //fdoc->scMW()->mainWindowProgressBar->setProgress( n );
}
//__________________________________________________________________________
void AspellPluginImpl::parseAll()
{
	fcontent.truncate( 0 );  // Start with empty string
	for( int i = 0; i < fdoc->Pages->count(); ++i )
	{
                parsePage( i );
	}
	//spellCheckDone();
}
//__________________________________________________________________________
//@@@@@@@@@@@@@@@@@@@@@@@@@ END OF FILE @@@@@@@@@@@@@@@@@@@@@@@@@
