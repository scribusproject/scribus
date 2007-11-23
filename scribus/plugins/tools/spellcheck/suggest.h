/*
  For general Scribus (>=1.3.2) copyright and licensing information
  please refer to the COPYING file provided with the
  program. Following this notice may exist a copyright and/or license
  notice that predates the release of Scribus 1.3.2 for which a new
  license (GPL+exception) is in place.
*/
#ifndef SUGGEST_H
#define SUGGEST_H

// ISO C++ include files
#include <sstream>            // For formatting aspell dictionary entries
#include <stdexcept>          // For exceptions
#include <string>             // Used in function arguments, errors 
#include <vector>             // Used in function arguments
#include <iostream>           // Printing to std::cout
// aspell include files
#include <aspell.h>

/*!
\brief Nested namespaces to allow different interfaces to different spell-checking engines.
 */
namespace Speller {
  namespace Aspell {
/*!
\class Speller::Aspell::Suggest
\author Gora Mohanty <gora@srijan.in>
\brief Class for interfacing with aspell. Should work with any aspell version
from 0.60 onwards
 */
    class Suggest {
    private:
	    /*! \brief Aspell configuration object. */
	    AspellConfig*  fconfig;
	    /*! \brief Aspell speller object. */
	    AspellSpeller* fspeller;
            /*! \brief Language for aspell dictionary. */
            std::string    flang;
            /*! \brief Jargon for aspell dictionary. */
            std::string    fjargon;
	    /*! \brief Character encoding for words. */
	    std::string    fencoding;

	    /*!
	      \fn Speller::Aspell::Suggest::checkError
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Checks aspell speller for an error condition.
	      \param None
	      \retval None
	      \exception std::runtime_error if an error condition exists.
	    */
	    void checkError() throw( std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::checkConfigError
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Checks for aspell configuration error.
	      \param None
	      \retval None
	      \exception std::runtime_error if an error condition exists
	    */
	    void checkConfigError() throw( std::runtime_error );
	    /*!
	      \fn AspellPluginImpl::init
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Initializes aspell: called from constructors.
	      \param lang: Language of aspell dictionary.
	      \param jargon: Jargon for aspell dictionary.
	      \param encoding: Character encoding for words.
	      \retval None
	      \exception std::invalid_argument from setConfig().
	      \exception std::runtime_error for error in creating speller.
	    */
	    void init(const std::string& lang, const std::string& jargon,
		    const std::string& encoding)
		    throw( std::invalid_argument, std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::printWordList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Prints list of words to stdout as UTF-8, with delimiter 'delim'.
	      \param wlist: Pointer to start of word list.
	      \param delim: Delimiter between words (default '\n').
	      \retval None
	      \exception std::invalid_argument if 'wlist' is null
	    */
	    void printWordList(const AspellWordList* wlist,
			       char delim='\n') throw(std::invalid_argument);
	    /*!
	      \fn Speller::Aspell::Suggest::setConfig
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Sets aspell speller configuration, as per current set of parameters.
	      \param None 
	      \retval None
	      \exception std::invalid_argument from setConfigOpt()
	    */
	    void setConfig() throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::storeWordList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Stores list of words in 'replacement'.
	      \param wlist: Pointer to start of word list.
	      \param replacement: Vector to hold suggested replacement words.
	      \retval None
	      \exception std::invalid_argument if 'wlist' is null
	    */
	    void storeWordList(const AspellWordList* wlist,
			       std::vector<std::string>& replacement)
		    throw( std::invalid_argument );

    public:
	    /*!
	      \brief Delimiter between different components of formatted strings for aspell dictionary entries.
	    */
	    static const char* kDICT_DELIM;
	    /*!
	      \brief Used in place of an empty string for an aspell setting (currently, only jargon might be empty).
	    */
	    static const char* kEMPTY;
	    /*! \brief Default language for aspell dictionary. */
	    static const char* kDEF_LANG;
	    /*! \brief Default jargon for aspell dictionary. */
	    static const char* kDEF_JARGON;
	    /*! \brief Default character encoding for words. */
	    static const char* kDEF_ENCODING;

	    // Defaults are for standard aspell English dictionary. Use
	    // "iso8859-1" for Basic Latin. See aspell lang.dat file for
	    // actual encoding used for a given dictionary.
	    /*!
	      \fn Speller::Aspell::Suggest::Suggest
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Default constructor for spell-checking class.
	      \param lang: Language of aspell dictionary.
	      \param jargon: Jargon for aspell dictionary.
	      \param encoding: Character encoding for words.
	      \retval None
	      \exception std::invalid_argument from setConfig().
	      \exception std::runtime_error for error in creating speller.
	    */
	    Suggest(const std::string& lang=kDEF_LANG,
		    const std::string& jargon=kDEF_JARGON,
		    const std::string& encoding=kDEF_ENCODING)
		    throw( std::invalid_argument, std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::Suggest
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Constructor for spell-checking class.
	      \param dinfo: aspell dictionary info structure.
	      \param encoding: Character encoding of words.
	      \retval None
	      \exception std::invalid_argument from setConfig().
	      \exception std::runtime_error for error in creating speller.
	    */
	    Suggest(const AspellDictInfo* dinfo,
		    const std::string& encoding=kDEF_ENCODING)
		    throw( std::invalid_argument, std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::addPersonalList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Adds 'word' to personal list.
	      \param word: Word to be added to personal list.
	      \retval None
	      \exception std::runtime_error for error in adding word.
	    */
	    void addPersonalList(const std::string& word)
		    throw( std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::checkWord
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Spell-checks 'word'.
	      \param word: Word to be checked.
	      \retval true if word is spelt correctly, else false
	    */
	    bool checkWord(const std::string& word);
	    /*!
	      \fn Speller::Aspell::Suggest::checkWord
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Spell-checks 'word'.
	      \param word: Word to be checked.
	      \retval true if word is spelt correctly, else false
	      \exception std::invalid_argument from storeWordList().
	    */
	    bool checkWord(const std::string& word,
			   std::vector<std::string>& replacement,
			   bool always=true) throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::clearSessionList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Clears the current session word list.
	      \param None
	      \retval None
	      \exception std::runtime_error for error in clearing list.
	    */
	    void clearSessionList() throw( std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::getConfigOpt
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Gets value of configuration option, 'opt'.
	      \param opt: Name of option whose setting is to be retrieved.
	      \retval Value of option setting.
	    */
	    std::string getConfigOpt(const std::string& opt);
	    /*!
	      \fn Speller::Aspell::Suggest::getConfigOpt
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Gets value of list type configuration option, 'opt'.
	      \param opt: Name of option whose setting is to be retrieved.
	      \param vals: Vector for storing values for the list type object.
	      \retval None.
	    */
	    void getConfigOpt(const std::string& opt,
			      std::vector<std::string>& vals);
	    /*!
	      \fn Speller::Aspell::Suggest::ignoreWord
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Ignores 'word' for current session.
	      \param word: Word to be ignored.
	      \retval None.
	      \exception std::runtime_error for error in ignoring word.
	    */
	    void ignoreWord(const std::string& word)
		    throw( std::runtime_error );
	    /*!
	      \fn AspellPluginImpl::listDicts
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Obtains list of available aspell dictionaries.
	      \param vals: Array of aspell dictionary info structs
	      \retval None
	    */
	    void listDicts(std::vector<AspellDictInfo>& vals);
	    /*!
	      \fn AspellPluginImpl::listDicts
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Obtains list of available aspell dictionaries.
	      \param vals: Array of formatted strings for aspell dictionary entries. Each has format <name>--<lang>--<jargon>--<size>
	      \retval None
	    */
	    void listDicts(std::vector<std::string>& vals);
	    /*!
	      \fn Speller::Aspell::Suggest::printMainList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Prints main word list. WARNING: This causes a crash due to a bug in aspell, as of aspell version 0.60.3.
	      \param None
	      \retval None.
	      \exception std::invalid_argument from printWordList().
	    */
	    void printMainList() throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::printPersonalList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Prints personal word list.
	      \param None
	      \retval None.
	      \exception std::invalid_argument from printWordList().
	    */
	    void printPersonalList() throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::printSessionList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Prints session word list.
	      \param None
	      \retval None.
	      \exception std::invalid_argument from printWordList().
	    */
	    void printSessionList() throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::printSuggestions
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Spell-checks word, and prints suggested replacements.
	      \param word: Word to be checked.
	      \param always: If true, replacements are always printed, else only if word is mis-spelt.
	      \retval true if word is spelt correctly, else false
	      \exception std::invalid_argument from printWordList().
	    */
	    bool printSuggestions(const std::string& word, bool always=true)
		    throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::resetConfig
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Resets aspell configuration, as per current set of parameters.
	      \param None
	      \retval None
	      \exception std::invalid_argument from setConfig().
	      \exception std::runtime_error for error in recreating speller.
	    */
	    void resetConfig()
		    throw( std::invalid_argument, std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::resetConfig
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Resets aspell configuration, as per current set of parameters, after resetting lang, jargon, and encoding.
	      \param lang: Language of aspell dictionary.
	      \param jargon: Jargon for aspell dictionary.
	      \param encoding: Character encoding for words.
	      \retval None
	      \exception std::invalid_argument from setConfig().
	      \exception std::runtime_error for error in recreating speller.
	    */
	    void resetConfig(const std::string& lang,
			     const std::string& jargon,
			     const std::string& encoding=kDEF_ENCODING)
		    throw( std::invalid_argument, std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::saveLists
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Saves all word lists.
	      \param None
	      \retval None
	      \exception std::runtime_error for error in saving lists.
	    */
	    void saveLists() throw( std::runtime_error );
	    /*!
	      \fn Speller::Aspell::Suggest::setConfigOpt
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Sets configuration option, 'opt' to value 'val'.
	      \param opt: Option to be set.
	      \param val: Value of option to be set.
	      \retval None
	      \exception std::invalid_argument if option value is incorrect
	    */
	    void setConfigOpt(const std::string& opt, const std::string& val)
		    throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::storeMainList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Stores main word list. WARNING: This causes a crash due to a bug in aspell, as of aspell version 0.60.3.
	      \param None
	      \retval None.
	      \exception std::invalid_argument from storeWordList().
	    */
	    void StoreMainList(std::vector<std::string>& replacement)
		    throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::storePersonalList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Stores personal word list.
	      \param None
	      \retval None.
	      \exception std::invalid_argument from storeWordList().
	    */
	    void StorePersonalList(std::vector<std::string>& replacement)
		    throw( std::invalid_argument );
	    /*!
	      \fn Speller::Aspell::Suggest::storeSessionList
	      \author Gora Mohanty <gora@srijan.in>
	      \brief Stores session word list.
	      \param None
	      \retval None.
	      \exception std::invalid_argument from storeWordList().
	    */
	    void StoreSessionList(std::vector<std::string>& replacement)
		    throw( std::invalid_argument );
    };  // class Suggest
  }  // namespace Aspell
}  // namespace Speller
#endif  // #ifndef SUGGEST_H
//@@@@@@@@@@@@@@@@@@@@@@@@@ END OF FILE @@@@@@@@@@@@@@@@@@@@@@@@@
