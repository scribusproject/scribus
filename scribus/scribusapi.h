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

#endif
