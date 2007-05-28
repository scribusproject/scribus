/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS_API_H
#define SCRIBUS_API_H

/*
 * This header provides a macro to handle correct symbol imports/exports
 * on platforms that require explicit instructions to make symbols public,
 * or differentiate between exported and imported symbols.
 *
 * Currently, that's win32, but gcc4's facilities for more selective
 * exports can be tied into this too (see bug #1961).
 *
 * Use this macro in the declaration of classes that must be exported
 * to plug-ins. With current Scribus code, that's most of them.
 *
 * Usage examples:
 *
 * class SCRIBUS_API PageItem
 * {
 *     ...
 * };
 *
 * bool SCRIBUS_API doThatThing(void);
 *
 * For an exception type that may be thrown across a DSO boundary, you must
 * use:
 *
 * class SCEXCEPTIONAPI(SCRIBUS_API) MyException
 * {
 *     ...
 * };
 *
 * For information on the gcc visibility support see:
 *	http://gcc.gnu.org/wiki/Visibility
 *	http://people.redhat.com/drepper/dsohowto.pdf
 *
 */

#ifdef _WIN32
    #ifdef COMPILE_SCRIBUS_MAIN_APP
        #define SCRIBUS_API __declspec(dllexport)
    #else
        #ifdef COMPILE_PLUGIN_AS_DLL
            #define SCRIBUS_API __declspec(dllimport)
        #else
            #define SCRIBUS_API
        #endif
    #endif
#else
    #ifdef HAVE_GCC_SYMBOL_VISIBILITY
        /* Forces inclusion of a symbol in the symbol table, so
           software outside the current library / app can use it. */
        #define SCRIBUS_API __attribute__ ((visibility("default")))
        /* Within a section exported with SCRIBUS_API, forces a symbol to be
           private to the library / app. Good for private members. */
        #define SCRIBUS_LOCAL __attribute__ ((visibility("hidden")))
    #else
        #define SCRIBUS_API
        #define SCRIBUS_LOCAL
    #endif
#endif

/* Throwable classes must always be visible on GCC in all binaries */
#ifdef WIN32
  #define SCEXCEPTIONAPI(api) api
#elif defined(HAVE_GCC_SYMBOL_VISIBILITY)
  #define SCEXCEPTIONAPI(api) SCRIBUS_API
#else
  #define SCEXCEPTIONAPI(api)
#endif

#endif
