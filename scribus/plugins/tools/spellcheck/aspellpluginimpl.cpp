/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "aspellpluginimpl.h"
#include "pageitem_textframe.h"
#include "util.h"
#include <QMessageBox>

const char* AspellPluginImpl::kDEF_CONTEXT = "AspellPlugin";
const QString AspellPluginImpl::kDEF_ASPELL_ENTRY =
	QString( "en" ) + Speller::Aspell::Suggest::kDICT_DELIM +
	QString( "en" ) + Speller::Aspell::Suggest::kDICT_DELIM +
	QString( "*" ) + Speller::Aspell::Suggest::kDICT_DELIM +
	QString( "60" );  // I.e., en/en/*/60 corresponding to default English dictionary

// Initialize members here, if any
AspellPluginImpl::AspellPluginImpl(ScribusDoc* doc, QWidget* parent) :
	QDialog( parent ), fdoc( doc ), fpos(0), fidx(0),
	m_errorMessage("")
{
	// Constructor.
	setupUi( this );
	setModal( true );

	rememberedWords.clear();
	// Get stored language, jargon, encoding settings
	fprefs = PrefsManager::instance()->prefsFile->getPluginContext( kDEF_CONTEXT );
	getPreferences();
	QString text = tr( "Loaded " ) + (fentry == kDEF_ASPELL_ENTRY ? tr( "default " ) : "") + fentry + tr( " aspell dictionary." );
	doc->scMW()->setStatusBarInfoText( text );
	try
	{
		// Deactivate GUI elements in spell-checking tab until an
		// aspell dictionary is chosen.
		activateSpellGUI(false);

		// Create speller, and get list of available aspell
		// dictionaries.
		fsuggest = new Speller::Aspell::Suggest( flang.toUtf8().data(), fjargon.toUtf8().data(), fencoding.toUtf8().data() );
		Q_CHECK_PTR( fsuggest );

		// Get list of available aspell dictionaries
		std::vector<std::string> entries;
		fsuggest->listDicts( entries );
		for( std::vector<std::string>::const_iterator i = entries.begin(); i != entries.end(); ++i )
		{
			// FIXME: Handle encodings other than UTF-8.
			flistDicts->addItem(i->c_str());
// 			qDebug() << i->c_str();
		}
		// check the availability of any dict. If == 0 then the plugin
		// is disabled in all GUI.
		if (flistDicts->count() == 0)
			m_errorMessage = tr("No available Aspell dictionaries found. Install some, please.");
		// use dict for system local if there are no preferences set before
		QString locale(QLocale::system().name().left(2));
		if (fentry.isEmpty())
		{
			int ix = flistDicts->findText(locale, Qt::MatchStartsWith);
			if (ix != -1)
				flistDicts->setCurrentIndex(ix);
			else
			{
				fentry = kDEF_ASPELL_ENTRY;
				setCurrentComboItem(flistDicts, fentry);
			}
		}
		else
			setCurrentComboItem(flistDicts, fentry);
		handleSpellConfig(flistDicts->currentText());
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
	activateSpellGUI(true);
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
void AspellPluginImpl::activateSpellGUI(bool active)
{
	// Activates spell-checking GUI elements in spell-checking
	// tab, i.e., everything except combo box at top
	fcurrWord->setEnabled( active );
	flistReplacements->setEnabled( active );
	fchangeBtn->setEnabled( active );
	fchangeAllBtn->setEnabled( active );
	fskipAllBtn->setEnabled( active );
	fskipBtn->setEnabled( active );
	faddWordBtn->setEnabled( active );
	ftextLabel1->setEnabled( active );
	ftextLabel2->setEnabled( active );
	ftextLabel4->setEnabled( active );
	fmisSpelling->setEnabled( active );
	flistDicts->setEnabled( active );
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
	if (pa == pe)
	{
		spellCheckDone();	// No more new Text, bail out from Spellchecking
		return;
	}
	fcontent = fFrame->itemText.text(pa, pe - pa);
	fpos = pa;
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
	QString completeMsg(tr("Spelling check complete"));
	QMessageBox::information(fdoc->scMW(), tr("Spell Checker"), completeMsg);
	if( fFrame && fFrame->asTextFrame() )
		fFrame->asTextFrame()->invalidateLayout();
	if( fnchanges.fntot > 0 )
	{
		fdoc->changed();
	}
	// Redraw document
	fdoc->view()->DrawNew();
	QApplication::changeOverrideCursor(Qt::ArrowCursor);
//	QApplication::restoreOverrideCursor();
	fdoc->scMW()->setStatusBarInfoText(completeMsg);
	fdoc->scMW()->mainWindowProgressBar->reset();
	close();
}
//__________________________________________________________________________
void AspellPluginImpl::on_fcloseBtn_clicked()
{
	// Called when the "Close" button is clicked. Makes any pending
	// replacements and closes the spell-checking window.
	spellCheckDone();  // Also closes spell-checking window.
}
//__________________________________________________________________________
void AspellPluginImpl::on_fchangeBtn_clicked()
{
	// Called when the "Change" button is clicked. Replaces the word
	// being spell-checked with the current word in text edit box.
	// FIXME: Handle encodings other than UTF-8.
	QString repl = fcurrWord->text();
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
	QString repl = fcurrWord->text();
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
		fsuggest->addPersonalList( fcurrWord->text().toUtf8().data() );
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

bool AspellPluginImpl::handleSpellConfig(const QString & dictFullName)
{
	QString entry(dictFullName);
	QStringList fields = entry.split( Speller::Aspell::Suggest::kDICT_DELIM );
	// Ensure that we have at least the right no.of fields.
	if( fields.size() == 4 )
	{
		QString value =
		fields[0] + Speller::Aspell::Suggest::kDICT_DELIM +
		fields[1] + Speller::Aspell::Suggest::kDICT_DELIM +
		fields[2] + Speller::Aspell::Suggest::kDICT_DELIM +
		fields[3];
		fsuggest->resetConfig( fields[1].toAscii().data(), fields[2].toAscii().data() );
		// FIXME: Handle encodings other than UTF-8.
		setPreferences( fields[1], fields[2], Speller::Aspell::Suggest::kDEF_ENCODING, value );
		return true;
	}
	return false;
}
//__________________________________________________________________________
void AspellPluginImpl::on_flistDicts_activated()
{
	// Called when an item in the list of available aspell dictionaries is
	// selected, i.e., by double-clicking, or pressing enter. Resets
	// aspell configuration to use the selected dictionary.
	if (handleSpellConfig(flistDicts->currentText()))
	{
		// PV - 7523: In the spell checker, the only option to change
		// the dictionary is to open the spell checker dialog. However,
		// changing the dictionary does not recheck first word
		// --- ask user if he wants to restart spellchecker with new lang
		if (QMessageBox::question(this,
								  tr("Spell Checker"),
								  tr("Do you want start from the beginning of the selection "
								     "with new language selected?"),
								  QMessageBox::Yes | QMessageBox::No)
			== QMessageBox::Yes)
		{
			fpos = 0;
			parseSelection();
		}
	}
}
//__________________________________________________________________________
void AspellPluginImpl::getPreferences()
{
	// Retrieves user preferences from saved settings. Defaults are
	// supplied 
	flang =	fprefs->get( "lang", Speller::Aspell::Suggest::kDEF_LANG );
	fjargon = fprefs->get( "jargon", Speller::Aspell::Suggest::kDEF_JARGON );
	// FIXME: Handle encodings other than UTF-8.
	fencoding = fprefs->get( "encoding", Speller::Aspell::Suggest::kDEF_ENCODING );
	// Don't use kDEF_ASPELL_ENTRY here. It's checked
	// against system locale later when there is no preferences for it.
	fentry = fprefs->get( "entry", "");//kDEF_ASPELL_ENTRY );
}
//__________________________________________________________________________
void AspellPluginImpl::setPreferences(const QString& lang,
				      const QString& jargon,
				      const QString& encoding,
				      const QString& entry)
{
	// Saves user preferences using Scribus preferences manager.
	fprefs->set( "lang", lang );
	QString val = jargon == Speller::Aspell::Suggest::kEMPTY ? "" : jargon;
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
void AspellPluginImpl::parseItem()
{
	// Parse text in a frame, and spell-check it.
	// Process only text frames
	if( fFrame && fFrame->asTextFrame() )
	{
		nextWord();
		checkText();
	}
}
//__________________________________________________________________________
void AspellPluginImpl::parseSelection()
{
	fcontent.truncate( 0 );  // Start with empty string
	uint ndocs = fdoc->m_Selection->count();
	for( uint i = 0; i < ndocs; ++i )
	{
		fFrame = fdoc->m_Selection->itemAt( i );
		parseItem();
	}
}
//__________________________________________________________________________
//@@@@@@@@@@@@@@@@@@@@@@@@@ END OF FILE @@@@@@@@@@@@@@@@@@@@@@@@@
