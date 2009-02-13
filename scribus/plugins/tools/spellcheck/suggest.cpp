/*
  For general Scribus (>=1.3.2) copyright and licensing information
  please refer to the COPYING file provided with the
  program. Following this notice may exist a copyright and/or license
  notice that predates the release of Scribus 1.3.2 for which a new
  license (GPL+exception) is in place.
*/
#include "suggest.h"
#include "scpaths.h"
#include <QFileInfo>
/*!
\brief Delimiter between different components of formatted strings for aspell dictionary entries.
 */
const char* Speller::Aspell::Suggest::kDICT_DELIM = "/";
const char* Speller::Aspell::Suggest::kEMPTY = "*";
const char* Speller::Aspell::Suggest::kDEF_LANG = "en";
const char* Speller::Aspell::Suggest::kDEF_JARGON = "";  // I.e., no jargon
const char* Speller::Aspell::Suggest::kDEF_ENCODING = "utf-8";

void Speller::Aspell::Suggest::checkError() throw( std::runtime_error )
{
	if( aspell_speller_error_number( fspeller ) != 0 )
	{
		std::string err_msg =
			std::string( "(Aspell::Speller::Suggest::checkError): "
				     "aspell speller error " ) +
			aspell_speller_error_message( fspeller );
		throw std::runtime_error( err_msg );
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::checkConfigError()
	throw( std::runtime_error )
{
	if( aspell_config_error_number( fconfig ) != 0 )
	{
		std::string err_msg =
			std::string( "(Aspell::Speller::Suggest::checkConfig"
				     "Error): aspell speller error " ) +
			aspell_config_error_message( fconfig );
		throw std::runtime_error( err_msg );
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::init(const std::string& lang,
				    const std::string& jargon,
				    const std::string& encoding)
	throw( std::invalid_argument, std::runtime_error )
{
	// Save aspell configuration values
	flang = lang;
	fjargon = jargon;
	fencoding = encoding;

	fconfig = new_aspell_config();
	try
	{
		setConfig();
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}

	AspellCanHaveError* ret = new_aspell_speller( fconfig );
	delete_aspell_config( fconfig );
	if( aspell_error_number( ret ) != 0 )
	{
		delete_aspell_can_have_error( ret );
		throw std::runtime_error( "(Aspell::Speller::Suggest::init"
					  "): Error in creating speller." );
	}
	else
	{
		fspeller = to_aspell_speller( ret );
		fconfig = aspell_speller_config( fspeller );
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::listDicts(std::vector<AspellDictInfo>& vals)
{
	AspellDictInfoList* dlist = get_aspell_dict_info_list( fconfig );
	AspellDictInfoEnumeration* dinfo_enum =
		aspell_dict_info_list_elements( dlist );
	const AspellDictInfo* entry;
	while( (entry = aspell_dict_info_enumeration_next( dinfo_enum )) )
	{
		vals.push_back( *entry );
	}
	delete_aspell_dict_info_enumeration( dinfo_enum );
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::listDicts(std::vector<std::string>& vals)
{
	std::vector<AspellDictInfo> entries;
	listDicts( entries );
	for( std::vector<AspellDictInfo>::const_iterator i = entries.begin();
             i != entries.end();
             ++i )
        {
		std::string jargon( i->jargon );
		std::ostringstream fmt_entry;
		fmt_entry << i->name << kDICT_DELIM << i->code << kDICT_DELIM
			  << (jargon == "" ? kEMPTY : jargon) << kDICT_DELIM
			  << i->size;
                vals.push_back( fmt_entry.str() );
        }
}
//__________________________________________________________________________
void
Speller::Aspell::Suggest::printWordList(const AspellWordList* wlist,
					char delim)
	throw( std::invalid_argument )
{
	if( ! wlist )
	{
		throw std::invalid_argument( "(Aspell.Speller.Suggest.print"
					     "WordList): word list pointer "
					     "is null." );
	}

	AspellStringEnumeration* enum_list =
		aspell_word_list_elements( wlist );
	const char* next;
	while( (next = aspell_string_enumeration_next( enum_list )) )
	{
		std::cout << next << delim;
	}
	delete_aspell_string_enumeration( enum_list );
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::setConfig() throw( std::invalid_argument )
{
	try
	{
		setConfigOpt( "lang", flang );
		setConfigOpt( "jargon", fjargon );
		setConfigOpt( "encoding", fencoding );
#ifdef ASPELLRELATIVEDICTDIR
		QString location(ScPaths::instance().bundleDir()+"/Contents/"+ASPELLRELATIVEDICTDIR);
		QFileInfo fi(location);
		if (fi.exists())
			setConfigOpt( "dict-dir", (location.toStdString()));
#endif
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void
Speller::Aspell::Suggest::storeWordList(const AspellWordList* wlist,
					std::vector<std::string>& replacement)
	throw( std::invalid_argument )
{
	if( ! wlist )
	{
		throw std::invalid_argument( "(Aspell.Speller.Suggest.store"
					     "WordList): word list pointer "
					     "is null." );
	}

	AspellStringEnumeration* enum_list =
		aspell_word_list_elements( wlist );
	const char* next;
	while( (next = aspell_string_enumeration_next( enum_list )) )
	{
		replacement.push_back( next );
	}
	delete_aspell_string_enumeration( enum_list );
}
//__________________________________________________________________________
Speller::Aspell::Suggest::Suggest(const std::string& lang,
				  const std::string& jargon,
				  const std::string& encoding)
	throw( std::invalid_argument, std::runtime_error )
{
	// Default constructor
	try
	{
		init( lang, jargon, encoding );
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
	catch( const std::runtime_error& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
Speller::Aspell::Suggest::Suggest(const AspellDictInfo* dinfo,
				  const std::string& encoding)
	throw( std::invalid_argument, std::runtime_error )
{
	try
	{
		init( dinfo->code, dinfo->jargon, encoding );
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
	catch( const std::runtime_error& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::addPersonalList(const std::string& word)
	throw( std::runtime_error )
{
	//  A std::runtime_error exception is thrown if
	// an error occcurs.

	// FIXME: Return value should be something meaningful from
	// aspell_speller_add_to_personal.
	aspell_speller_add_to_personal( fspeller, word.c_str(), -1 );
	try
	{
		checkError();
	}
	catch( const std::runtime_error& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
bool Speller::Aspell::Suggest::checkWord(const std::string& word)
{
	bool status = true;

	if( aspell_speller_check( fspeller, word.c_str(), -1 ) == 0 )
	{
		status = false;
	}
	return status;
}
//__________________________________________________________________________
bool Speller::Aspell::Suggest::checkWord(const std::string& word,
					 std::vector<std::string>& replacement,
					 bool always)
	throw( std::invalid_argument )
{
	// Checks 'word', and returns true if it is spelt correctly,
	// else returns false. If 'always' is true, returns an
	// array of suggestions in 'replacement', regardless of
	// whether 'word' is spelt incorrectly or not. Else, if
	// 'always' is false, the list of suggestions is stored only if
	// 'word'is spelt incorrectly.
	bool status = checkWord( word );

	if( always )
	{
		const AspellWordList* wlist =
			aspell_speller_suggest( fspeller, word.c_str(), -1 );
		try
		{
			storeWordList( wlist, replacement );
		}
		catch( const std::invalid_argument& err )
		{
			throw err;
		}
	}
	else
	{
		if( ! status )
		{
			const AspellWordList* wlist =
				aspell_speller_suggest( fspeller, word.c_str(),
							-1 );
			try
			{
				storeWordList( wlist, replacement );
			}
			catch( const std::invalid_argument& err )
			{
				throw err;
			}
		}
	}
	return status;
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::clearSessionList() throw( std::runtime_error )
{
	aspell_speller_clear_session( fspeller );
	try
	{
		checkError();
	}
	catch( const std::runtime_error& err )
	{
		throw err;
	}  
}
//__________________________________________________________________________
std::string Speller::Aspell::Suggest::getConfigOpt(const std::string& opt)
{
	return std::string( aspell_config_retrieve( fconfig, opt.c_str() ) );
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::getConfigOpt(const std::string& opt,
					    std::vector<std::string>& vals)
{
	// Stores current setting of configuration option, 'opt', which
	// has a value of list type, in 'vals'.
	AspellStringList* list = new_aspell_string_list();
	AspellMutableContainer* lst0 =
		aspell_string_list_to_mutable_container( list );
	aspell_config_retrieve_list( fconfig, opt.c_str(), lst0 );
	AspellStringEnumeration* enum_list =
		aspell_string_list_elements( list );
	const char* next;
	while( (next = aspell_string_enumeration_next( enum_list )) )
	{
		vals.push_back( next );
	}
	delete_aspell_string_enumeration( enum_list );
	delete_aspell_string_list( list );
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::ignoreWord(const std::string& word)
	throw( std::runtime_error )
{
	// FIXME: Return value should be something meaningful from
	// aspell_speller_add_to_session.
	aspell_speller_add_to_session( fspeller, word.c_str(), -1 );
	try
	{
		checkError();
	}
	catch( const std::runtime_error& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::printMainList() throw( std::invalid_argument )
{
	const AspellWordList* wlist =
		aspell_speller_main_word_list( fspeller );
	try
	{
		printWordList( wlist );
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::printPersonalList()
	throw( std::invalid_argument )
{
	const AspellWordList* wlist =
		aspell_speller_personal_word_list( fspeller );
	try
	{
		printWordList( wlist );
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::printSessionList()
	throw( std::invalid_argument )
{
	const AspellWordList* wlist =
		aspell_speller_session_word_list( fspeller );
	try
	{
		printWordList( wlist );
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
bool Speller::Aspell::Suggest::printSuggestions(const std::string& word,
						bool always)
	throw( std::invalid_argument )
{
	// Checks 'word', and returns true if it is spelt correctly,
	// else returns false. If 'always' is true, prints a list of
	// suggestions to the console as UTF-8, regardless of whether
	// 'word' is spelt correctly, or not. Else, if 'always' is
	// false, only prints the list if 'word' is incorrect.
	bool status = checkWord( word );

	if( always )
	{
		const AspellWordList* wlist =
			aspell_speller_suggest( fspeller, word.c_str(), -1 );
		try
		{
			printWordList( wlist );
		}
		catch( const std::invalid_argument& err )
		{
			throw err;
		}
	}
	else
	{
		if( ! status )
		{
			const AspellWordList* wlist =
				aspell_speller_suggest( fspeller, word.c_str(),
							-1 );
			try
			{
				printWordList( wlist );
			}
			catch( const std::invalid_argument& err )
			{
				throw err;
			}
		}
	}
	return status;  
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::resetConfig()
	throw( std::invalid_argument, std::runtime_error )
{
	delete_aspell_config( fconfig );
	fconfig = new_aspell_config();
	try
	{
		setConfig();
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}

	AspellCanHaveError* ret = new_aspell_speller( fconfig );
	if( aspell_error_number( ret ) != 0 )
	{
		delete_aspell_can_have_error( ret );
		throw std::runtime_error( "(Aspell::Speller::Suggest::Reset"
					  "Config): Error in creating "
					  "speller." );
	}
	else
	{
		// Following statement causes a crash, hence commented out
		//delete_aspell_speller( fspeller );
		fspeller = to_aspell_speller( ret );
		delete_aspell_config( fconfig );
		fconfig = aspell_speller_config( fspeller );
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::resetConfig(const std::string& lang,
					   const std::string& jargon,
					   const std::string& encoding)
	throw( std::invalid_argument, std::runtime_error )
{
	// Save new aspell configuration values
	flang = lang;
	fjargon = jargon;
	fencoding = encoding;

	try
	{
		resetConfig();  // Actually reset configuration
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
	catch( const std::runtime_error& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::saveLists() throw( std::runtime_error )
{
	// Saves all word lists.
	aspell_speller_save_all_word_lists( fspeller );
	try
	{
		checkError();
	}
	catch( const std::runtime_error& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::setConfigOpt(const std::string& opt,
					    const std::string& val)
	throw( std::invalid_argument )
{
	aspell_config_replace( fconfig, opt.c_str(), val.c_str() );
	try
	{
		checkConfigError();
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void
Speller::Aspell::Suggest::StoreMainList(std::vector<std::string>& replacement)
	throw( std::invalid_argument )
{
	const AspellWordList* wlist =
		aspell_speller_main_word_list( fspeller );
	try
	{
		storeWordList( wlist, replacement );
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::StorePersonalList(std::vector<std::string>& replacement) throw( std::invalid_argument )
{
	// Stores personal word list. A std::invalid_argument
	// exception is thrown in case of error.
	const AspellWordList* wlist =
		aspell_speller_personal_word_list( fspeller );
	try
	{
		storeWordList( wlist, replacement );
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
void Speller::Aspell::Suggest::StoreSessionList(std::vector<std::string>& replacement) throw( std::invalid_argument )
{
	// Stores session word list. A std::invalid_argument exception
	// is thrown in case of error.
	const AspellWordList* wlist =
		aspell_speller_session_word_list( fspeller );
	try
	{
		storeWordList( wlist, replacement );
	}
	catch( const std::invalid_argument& err )
	{
		throw err;
	}
}
//__________________________________________________________________________
//@@@@@@@@@@@@@@@@@@@@@@@@@ END OF FILE @@@@@@@@@@@@@@@@@@@@@@@@@
