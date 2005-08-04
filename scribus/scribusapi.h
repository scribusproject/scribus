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
    #define SCRIBUS_API
#endif

#endif
