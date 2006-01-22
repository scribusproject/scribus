/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
// Le bloc ifdef suivant est la façon standard de créer des macros qui facilitent l'exportation 
// à partir d'une DLL. Tous les fichiers contenus dans cette DLL sont compilés avec le symbole SVGEXPORT121_EXPORTS
// défini sur la ligne de commande. Ce symbole ne doit pas être défini dans les projets
// qui utilisent cette DLL. De cette manière, les autres projets dont les fichiers sources comprennent ce fichier considèrent les fonctions 
// SVGEXPORT121_API comme étant importées à partir d'une DLL, tandis que cette DLL considère les symboles
// définis avec cette macro comme étant exporté.

#ifndef PLUGIN_API_H
#define PLUGIN_API_H

#ifdef _WIN32
    #ifdef COMPILE_SCRIBUS_MAIN_APP
        #define PLUGIN_API __declspec(dllimport)
    #else
        #ifdef COMPILE_PLUGIN_AS_DLL
            #define PLUGIN_API __declspec(dllexport)
        #else
            #define PLUGIN_API
        #endif
    #endif
#else
    #ifdef HAVE_GCC_SYMBOL_VISIBILITY
        #define PLUGIN_API __attribute__ ((visibility("default")))
        #define PLUGIN_LOCAL __attribute__ ((visibility("hidden")))
    #else
        #define PLUGIN_API
        #define PLUGIN_LOCAL
    #endif
#endif

#endif
