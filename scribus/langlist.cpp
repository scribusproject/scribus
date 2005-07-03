/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <qmap.h>
#include <qobject.h>
#include "langlist.h"

LangList::LangList()
{
    insert("af",    QObject::tr( "Afrikaans" ));
    insert("sq",    QObject::tr( "Albanian" ));
    insert("eu",    QObject::tr( "Basque" ));
    insert("bg",    QObject::tr( "Bulgarian" ));
    insert("br",    QObject::tr( "Brazilian" ));
    insert("ca",    QObject::tr( "Catalan" ));
    insert("zh",    QObject::tr( "Chinese" ));
    insert("hr",    QObject::tr( "Croatian" ));
    insert("cs",    QObject::tr( "Czech" ));
    insert("cs_CZ", QObject::tr( "Czech" ));
    insert("da",    QObject::tr( "Danish" ));
    insert("da_DK", QObject::tr( "Danish" ));
    insert("nl",    QObject::tr( "Dutch" ));
    insert("en",    QObject::tr( "English" ));
    insert("en_GB", QObject::tr( "English (British)" ));
    insert("eo",    QObject::tr( "Esperanto" ));
    insert("de",    QObject::tr( "German" ));
    insert("fi",    QObject::tr( "Finnish" ));
    insert("fr",    QObject::tr( "French" ));
    insert("galician", QObject::tr( "Galician" ));
    insert("gl",    QObject::tr( "Galician" ));
    insert("el",    QObject::tr( "Greek" ));
    insert("hu",    QObject::tr( "Hungarian" ));
    insert("id",    QObject::tr( "Indonesian" ));
    insert("it",    QObject::tr( "Italian" ));
    insert("ko",    QObject::tr( "Korean" ));
    insert("lt",    QObject::tr( "Lithuanian" ));
    insert("lt_LT", QObject::tr( "Lithuanian" ));
    insert("nb",    QObject::tr( "Norwegian (Bokmaal)" ));
    insert("nb_NO", QObject::tr( "Norwegian (Bokmaal)" ));
    insert("nn",    QObject::tr( "Norwegian (Nnyorsk)" ));
    insert("nn_NO", QObject::tr( "Norwegian (Nnyorsk)" ));
    insert("no",    QObject::tr( "Norwegian" ));
    insert("no_NO", QObject::tr( "Norwegian" ));
    insert("pl",    QObject::tr( "Polish" ));
    insert("pl_PL", QObject::tr( "Polish" ));
    insert("pt_BR", QObject::tr( "Portuguese (Brazilian)" ));
    insert("ru",    QObject::tr( "Russian" ));
    insert("se",    QObject::tr( "Swedish" ));
    insert("es",    QObject::tr( "Spanish" ));
    insert("es_LA", QObject::tr( "Spanish (Latin)" ));
    insert("sk",    QObject::tr( "Slovak" ));
    insert("sl",    QObject::tr( "Slovenian" ));
    insert("sr",    QObject::tr( "Serbian" ));
    insert("tr",    QObject::tr( "Turkish" ));
    insert("uk",    QObject::tr( "Ukranian" ));
    insert("cy",    QObject::tr( "Welsh" ));
}
