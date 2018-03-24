/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Jan 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#include <iostream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QStringList> 
#include <QTextStream>

#include "scconfig.h"
#include "langmgr.h"
#include "scpaths.h"

LanguageManager * LanguageManager::m_instance = 0;

LanguageManager * LanguageManager::instance()
{
	if(!m_instance)
	{
		m_instance = new LanguageManager;
		Q_ASSERT(m_instance);
		m_instance->init();
	}
	return m_instance;
}

void LanguageManager::deleteInstance()
{
	if (m_instance)
		delete m_instance;
	m_instance = 0;
}

void LanguageManager::languageChange()
{
	QList<LangDef> oldLangList = m_langTable;
	generateLangList();

	for (int i = 0; i < m_langTable.count(); ++i)
	{
		m_langTable[i].m_hyphAvailable = oldLangList[i].m_hyphAvailable;
		m_langTable[i].m_hyphFile = oldLangList[i].m_hyphFile;

		m_langTable[i].m_spellAvailable = oldLangList[i].m_spellAvailable;
		m_langTable[i].m_spellFile = oldLangList[i].m_spellFile;

		m_langTable[i].m_transAvailable = oldLangList[i].m_transAvailable;
		m_langTable[i].m_transFile = oldLangList[i].m_transFile;
	}
}

void LanguageManager::init(bool generateInstalledList)
{
	//generateUntranslatedLangList();
	generateLangList();
	if (generateInstalledList)
	{
		generateInstalledGUILangList();
		generateInstalledSpellLangList();
		generateInstalledHyphLangList();
	}
}

void LanguageManager::generateLangList()
{
	// So internally language would always be manipulated as a code and otherwise presented translated.
	//Build table;
	m_langTable.clear();
	// Please keep this list sorted by language name.
	m_langTable.append(LangDef("aiw",      "",         "Aari",                    QObject::tr("Aari")));
	m_langTable.append(LangDef("abq",      "",         "Abaza",                   QObject::tr("Abaza")));
	m_langTable.append(LangDef("ab",       "",         "Abkhazian",               QObject::tr("Abkhazian")));
	m_langTable.append(LangDef("ach",      "",         "Acoli",                   QObject::tr("Acoli")));
	m_langTable.append(LangDef("ady",      "",         "Adyghe",                  QObject::tr("Adyghe")));
	m_langTable.append(LangDef("af",       "af_ZA",    "Afrikaans",               QObject::tr("Afrikaans")));
	m_langTable.append(LangDef("aa",       "",         "Afar",                    QObject::tr("Afar")));
	m_langTable.append(LangDef("aio",      "",         "Aiton",                   QObject::tr("Aiton")));
	m_langTable.append(LangDef("ak",       "",         "Akan",                    QObject::tr("Akan")));
	m_langTable.append(LangDef("sq",       "",         "Albanian",                QObject::tr("Albanian")));
	m_langTable.append(LangDef("atv",      "alt",      "Altai",                   QObject::tr("Altai")));
	m_langTable.append(LangDef("am",       "",         "Amharic",                 QObject::tr("Amharic")));
	m_langTable.append(LangDef("blo",      "",         "Anii",                    QObject::tr("Anii")));
	m_langTable.append(LangDef("ar",       "",         "Arabic",                  QObject::tr("Arabic")));
	m_langTable.append(LangDef("ar_dz",    "",         "Arabic (Algeria)",        QObject::tr("Arabic (Algeria)")));
	m_langTable.append(LangDef("ar_bh",    "",         "Arabic (Bahrain)",        QObject::tr("Arabic (Bahrain)")));
	m_langTable.append(LangDef("ar_eg",    "",         "Arabic (Egypt)",          QObject::tr("Arabic (Egypt)")));
	m_langTable.append(LangDef("ar_iq",    "",         "Arabic (Iraq)",           QObject::tr("Arabic (Iraq)")));
	m_langTable.append(LangDef("ar_jo",    "",         "Arabic (Jordan)",         QObject::tr("Arabic (Jordan)")));
	m_langTable.append(LangDef("ar_kw",    "",         "Arabic (Kuwait)",         QObject::tr("Arabic (Kuwait)")));
	m_langTable.append(LangDef("ar_lb",    "",         "Arabic (Lebanon)",        QObject::tr("Arabic (Lebanon)")));
	m_langTable.append(LangDef("ar_ly",    "",         "Arabic (Libya)",          QObject::tr("Arabic (Libya)")));
	m_langTable.append(LangDef("ar_ma",    "",         "Arabic (Morocco)",        QObject::tr("Arabic (Morocco)")));
	m_langTable.append(LangDef("ar_om",    "",         "Arabic (Oman)",           QObject::tr("Arabic (Oman)")));
	m_langTable.append(LangDef("ar_qa",    "",         "Arabic (Qatar)",          QObject::tr("Arabic (Qatar)")));
	m_langTable.append(LangDef("ar_sa",    "",         "Arabic (Saudi Arabia)",   QObject::tr("Arabic (Saudi Arabia)")));
	m_langTable.append(LangDef("ar_sd",    "",         "Arabic (Sudan)",          QObject::tr("Arabic (Sudan)")));
	m_langTable.append(LangDef("ar_sy",    "",         "Arabic (Syria)",          QObject::tr("Arabic (Syria)")));
	m_langTable.append(LangDef("ar_tn",    "",         "Arabic (Tunisia)",        QObject::tr("Arabic (Tunisia)")));
	m_langTable.append(LangDef("ar_ae",    "",         "Arabic (UAE)",            QObject::tr("Arabic (UAE)")));
	m_langTable.append(LangDef("ar_ye",    "",         "Arabic (Yemen)",          QObject::tr("Arabic (Yemen)")));
	m_langTable.append(LangDef("an",       "an_ES",    "Aragonese",               QObject::tr("Aragonese")));
	m_langTable.append(LangDef("hy",       "",         "Armenian",                QObject::tr("Armenian")));
	m_langTable.append(LangDef("rup",      "",         "Aromanian",               QObject::tr("Aromanian")));
	m_langTable.append(LangDef("as",       "",         "Assamese",                QObject::tr("Assamese")));
	m_langTable.append(LangDef("ast",      "",         "Asturian",                QObject::tr("Asturian")));
	m_langTable.append(LangDef("ath",      "",         "Athapaskan",              QObject::tr("Athapaskan")));
	m_langTable.append(LangDef("av",       "",         "Avaric",                  QObject::tr("Avaric")));
	m_langTable.append(LangDef("awa",      "",         "Awadhi",                  QObject::tr("Awadhi")));
	m_langTable.append(LangDef("ay",       "",         "Aymara",                  QObject::tr("Aymara")));
	m_langTable.append(LangDef("az",       "aze",      "Azerbaijani",             QObject::tr("Azerbaijani")));
	m_langTable.append(LangDef("azj",      "",         "Azerbaijani (North)",     QObject::tr("Azerbaijani (North)")));
	m_langTable.append(LangDef("azb",      "",         "Azerbaijani (South)",     QObject::tr("Azerbaijani (South)")));
	m_langTable.append(LangDef("bfq",      "",         "Badaga",                  QObject::tr("Badaga")));
	m_langTable.append(LangDef("bfy",      "",         "Baghelkhandi",            QObject::tr("Baghelkhandi")));
	m_langTable.append(LangDef("bgq",      "",         "Bagri",                   QObject::tr("Bagri")));
	m_langTable.append(LangDef("bqi",      "",         "Bakhtiari",               QObject::tr("Bakhtiari")));
	m_langTable.append(LangDef("krc",      "",         "Balkar",                  QObject::tr("Balkar")));
	m_langTable.append(LangDef("bjt",      "",         "Balanta-Ganja",           QObject::tr("Balanta-Ganja")));
	m_langTable.append(LangDef("ble",      "",         "Balanta-Kentohe",         QObject::tr("Balanta-Kentohe")));
	m_langTable.append(LangDef("ban",      "",         "Balinese",                QObject::tr("Balinese")));
	m_langTable.append(LangDef("bal",      "",         "Baluchi",                 QObject::tr("Baluchi")));
	m_langTable.append(LangDef("bft",      "",         "Balti",                   QObject::tr("Balti")));
	m_langTable.append(LangDef("bm",       "",         "Bambara",                 QObject::tr("Bambara")));
	m_langTable.append(LangDef("bai",      "",         "Bamileke",                QObject::tr("Bamileke")));
	m_langTable.append(LangDef("bad",      "",         "Banda",                   QObject::tr("Banda")));
	m_langTable.append(LangDef("ba",       "",         "Bashkir",                 QObject::tr("Bashkir")));
	m_langTable.append(LangDef("eu",       "",         "Basque",                  QObject::tr("Basque")));
	m_langTable.append(LangDef("bts",      "",         "Batak Simalungun",        QObject::tr("Batak Simalungun")));
	m_langTable.append(LangDef("bbc",      "",         "Batak Toba",              QObject::tr("Batak Toba")));
	m_langTable.append(LangDef("bci",      "",         "Baulé",                   QObject::trUtf8("Baul\303\251")));
	m_langTable.append(LangDef("bar",      "",         "Bavarian",                QObject::tr("Bavarian")));
	m_langTable.append(LangDef("bcq",      "",         "Bench",                   QObject::tr("Bench")));
	m_langTable.append(LangDef("bdy",      "",         "Bandjalang",              QObject::tr("Bandjalang")));
	m_langTable.append(LangDef("bem",      "",         "Bemba",                   QObject::tr("Bemba")));
	m_langTable.append(LangDef("be",       "be_BY",    "Belarusian",              QObject::tr("Belarusian")));
	m_langTable.append(LangDef("bn",       "",         "Bengali",                 QObject::tr("Bengali")));
	m_langTable.append(LangDef("bn_BD",    "",         "Bengali (Bangladesh)",    QObject::tr("Bengali (Bangladesh)")));
	m_langTable.append(LangDef("ber",      "",         "Berber",                  QObject::tr("Berber")));
	m_langTable.append(LangDef("btb",      "",         "Beti (Cameroon)",         QObject::tr("Beti (Cameroon)")));
	m_langTable.append(LangDef("eot",      "",         "Beti (Côte d'Ivoire)",    QObject::trUtf8("Beti (C\303\264te d'Ivoire)")));
	m_langTable.append(LangDef("bhb",      "",         "Bhili",                   QObject::tr("Bhili")));
	m_langTable.append(LangDef("bho",      "",         "Bhojpuri",                QObject::tr("Bhojpuri")));
	m_langTable.append(LangDef("bik",      "",         "Bikol",                   QObject::tr("Bikol")));
	m_langTable.append(LangDef("bcl",      "",         "Bikol (Central)",         QObject::tr("Bikol (Central)")));
	m_langTable.append(LangDef("byn",      "",         "Bilen",                   QObject::tr("Bilen")));
	m_langTable.append(LangDef("bin",      "",         "Bini",                    QObject::tr("Bini")));
	m_langTable.append(LangDef("bpy",      "",         "Bishnupriya",             QObject::tr("Bishnupriya")));
	m_langTable.append(LangDef("bi",       "",         "Bislama",                 QObject::tr("Bislama")));
	m_langTable.append(LangDef("bla",      "",         "Blackfoot",               QObject::tr("Blackfoot")));
	m_langTable.append(LangDef("brx",      "",         "Bodo (India)",            QObject::tr("Bodo (India)")));
	m_langTable.append(LangDef("pcc",      "",         "Bouyei",                  QObject::tr("Bouyei")));
	m_langTable.append(LangDef("bs_BA",    "bs_BA",    "Bosnian",                 QObject::tr("Bosnian")));
	m_langTable.append(LangDef("brh",      "",         "Brahui",                  QObject::tr("Brahui")));
	m_langTable.append(LangDef("bra",      "",         "Braj Bhasha",             QObject::tr("Braj Bhasha")));
	m_langTable.append(LangDef("br",       "br_FR",    "Breton",                  QObject::tr("Breton")));
	m_langTable.append(LangDef("bug",      "",         "Buginese",                QObject::tr("Buginese")));
	m_langTable.append(LangDef("bg",       "bg_BG",    "Bulgarian",               QObject::tr("Bulgarian")));
	m_langTable.append(LangDef("my",       "",         "Burmese",                 QObject::tr("Burmese")));
	m_langTable.append(LangDef("frc",      "",         "Cajun French",            QObject::tr("Cajun French")));
	m_langTable.append(LangDef("crx",      "",         "Carrier",                 QObject::tr("Carrier")));
	m_langTable.append(LangDef("ca",       "",         "Catalan",                 QObject::tr("Catalan")));
	m_langTable.append(LangDef("cts",      "",         "Catanduanes Bikol Northern", QObject::tr("Catanduanes Bikol Northern")));
	m_langTable.append(LangDef("bln",      "",         "Catanduanes Bikol Southern", QObject::tr("Catanduanes Bikol Southern")));
	m_langTable.append(LangDef("ceb",      "",         "Cebuano",                 QObject::tr("Cebuano")));
	m_langTable.append(LangDef("tzm",      "",         "Central Atlas Tamazight", QObject::tr("Central Atlas Tamazight")));
	m_langTable.append(LangDef("ch",       "",         "Chamorro",                QObject::tr("Chamorro")));
	m_langTable.append(LangDef("hne",      "",         "Chattisgarhi",            QObject::tr("Chattisgarhi")));
	m_langTable.append(LangDef("cbk",      "",         "Chavacano",               QObject::tr("Chavacano")));
	m_langTable.append(LangDef("ce",       "",         "Chechen",                 QObject::tr("Chechen")));
	m_langTable.append(LangDef("chr",      "",         "Cherokee",                QObject::tr("Cherokee")));
	m_langTable.append(LangDef("ny",       "",         "Chewa",                   QObject::tr("Chewa")));
	m_langTable.append(LangDef("chy",      "",         "Cheyenne",                QObject::tr("Cheyenne")));
	m_langTable.append(LangDef("cgg",      "",         "Chiga",                   QObject::tr("Chiga")));
	m_langTable.append(LangDef("zh",       "",         "Chinese",                 QObject::tr("Chinese")));
	m_langTable.append(LangDef("zh_TW",    "",         "Chinese (Trad.)",         QObject::tr("Chinese (Trad.)")));
	m_langTable.append(LangDef("chp",      "",         "Chipewyan",               QObject::tr("Chipewyan")));
	m_langTable.append(LangDef("ctg",      "",         "Chittagonian",            QObject::tr("Chittagonian")));
	m_langTable.append(LangDef("cho",      "",         "Choctaw",                 QObject::tr("Choctaw")));
	m_langTable.append(LangDef("ckt",      "",         "Chukchi",                 QObject::tr("Chukchi")));
	m_langTable.append(LangDef("cu",       "",         "Church Slavic",           QObject::tr("Church Slavic")));
	m_langTable.append(LangDef("cv",       "",         "Chuvash",                 QObject::tr("Chuvash")));
	m_langTable.append(LangDef("swb",      "",         "Comorian",                QObject::tr("Comorian")));
	m_langTable.append(LangDef("cop",      "",         "Coptic",                  QObject::tr("Coptic")));
	m_langTable.append(LangDef("kw",       "",         "Cornish",                 QObject::tr("Cornish")));
	m_langTable.append(LangDef("cr",       "",         "Cree",                    QObject::tr("Cree")));
	m_langTable.append(LangDef("crm",      "",         "Cree (Moose)",            QObject::tr("Cree (Moose)")));
	m_langTable.append(LangDef("crl",      "",         "Cree (East Northern)",    QObject::tr("Cree (East Southern)")));
	m_langTable.append(LangDef("crj",      "",         "Cree (East Southern)",    QObject::tr("Cree (East Southern)")));
	m_langTable.append(LangDef("cwd",      "",         "Cree (Woods)",            QObject::tr("Woods Cree")));
	m_langTable.append(LangDef("mus",      "",         "Creek",                   QObject::tr("Creek")));
	m_langTable.append(LangDef("crh",      "",         "Crimean Tatar",           QObject::tr("Crimean Tatar")));
	m_langTable.append(LangDef("hr",       "hr_HR",    "Croatian",                QObject::tr("Croatian")));
	m_langTable.append(LangDef("quz",      "",         "Cusco Quechua",           QObject::tr("Cusco Quechua")));
	m_langTable.append(LangDef("cs",       "cs_CZ",    "Czech",                   QObject::tr("Czech")));
	m_langTable.append(LangDef("ada",      "",         "Dangme",                  QObject::tr("Dangme")));
	m_langTable.append(LangDef("da",       "da_DK",    "Danish",                  QObject::tr("Danish")));
	m_langTable.append(LangDef("dar",      "",         "Dargwa",                  QObject::tr("Dargwa")));
	m_langTable.append(LangDef("prs",      "",         "Dari",                    QObject::tr("Afghan Dari")));
	m_langTable.append(LangDef("dhd",      "",         "Dhundari",                QObject::tr("Dhundari")));
	m_langTable.append(LangDef("din",      "",         "Dinka",                   QObject::tr("Dinka")));
	m_langTable.append(LangDef("diq",      "",         "Dimli",                   QObject::tr("Dimli")));
	m_langTable.append(LangDef("dgo",      "",         "Dogri",                   QObject::tr("Dogri")));
	m_langTable.append(LangDef("nl",       "nl_NL",    "Dutch",                   QObject::tr("Dutch")));
	m_langTable.append(LangDef("dz",       "",         "Dzongkha",                QObject::tr("Dzongkha")));
	m_langTable.append(LangDef("igb",      "",         "Ebira",                   QObject::tr("Ebira")));
	m_langTable.append(LangDef("efi",      "",         "Efik",                    QObject::tr("Efik")));
	m_langTable.append(LangDef("en_AU",    "",         "English (Australia)",     QObject::tr("English (Australia)")));
	m_langTable.append(LangDef("en_CA",    "",         "English (Canada)",        QObject::tr("English (Canada)")));
	m_langTable.append(LangDef("en_NZ",    "",         "English (New Zealand)",   QObject::tr("English (New Zealand)")));
	m_langTable.append(LangDef("en_ZA",    "",         "English (South Africa)",  QObject::tr("English (South Africa)")));
	m_langTable.append(LangDef("en_GB",    "en",       "English (UK)",            QObject::tr("English (UK)")));
	m_langTable.append(LangDef("en_US",    "",         "English (US)",            QObject::tr("English (US)")));
	m_langTable.append(LangDef("en_US",    "",         "English",                 QObject::tr("English")));
	m_langTable.append(LangDef("ang",      "",         "English (Old 450-1100)",  QObject::tr("English (Old 450-1100)")));
	m_langTable.append(LangDef("eo",       "",         "Esperanto",               QObject::tr("Esperanto")));
	m_langTable.append(LangDef("myv",      "",         "Erzya",                   QObject::tr("Erzya")));
	m_langTable.append(LangDef("et",       "et_EE",    "Estonian",                QObject::tr("Estonian")));
	m_langTable.append(LangDef("ekk",      "",         "Estonian (Standard)",     QObject::tr("Estonian (Standard)")));
	m_langTable.append(LangDef("eve",      "",         "Even",                    QObject::tr("Even")));
	m_langTable.append(LangDef("evn",      "",         "Evenki",                  QObject::tr("Evenki")));
	m_langTable.append(LangDef("ee",       "",         "Ewe",                     QObject::tr("Ewe")));
	m_langTable.append(LangDef("fo",       "",         "Faroese",                 QObject::tr("Faroese")));
	m_langTable.append(LangDef("fj",       "",         "Fijian",                  QObject::tr("Fijian")));
	m_langTable.append(LangDef("fil",      "",         "Filipino",                QObject::tr("Filipino")));
	m_langTable.append(LangDef("fi",       "",         "Finnish",                 QObject::tr("Finnish")));
	m_langTable.append(LangDef("fon",      "",         "Fon",                     QObject::tr("Fon")));
	m_langTable.append(LangDef("myq",      "",         "Forest Maninka",          QObject::tr("Forest Maninka")));
	m_langTable.append(LangDef("frp",      "",         "Franco-Provençal",        QObject::trUtf8("Franco-Proven\303\247al")));
	m_langTable.append(LangDef("fr",       "fr_FR",    "French",                  QObject::tr("French")));
	m_langTable.append(LangDef("fy",       "",         "Frisian",                 QObject::tr("Frisian")));
	m_langTable.append(LangDef("fur",      "",         "Friulian",                QObject::tr("Friulian")));
	m_langTable.append(LangDef("ff",       "",         "Fulah",                   QObject::tr("Fulah")));
	m_langTable.append(LangDef("gaa",      "",         "Ga",                      QObject::tr("Ga")));
	m_langTable.append(LangDef("gag",      "",         "Gagauz",                  QObject::tr("Gagauz")));
	m_langTable.append(LangDef("gl",       "gl_ES",    "Galician",                QObject::tr("Galician")));
	m_langTable.append(LangDef("lg",       "",         "Ganda",                   QObject::tr("Ganda")));
	m_langTable.append(LangDef("gbm",      "",         "Garhwali",                QObject::tr("Garhwali")));
	m_langTable.append(LangDef("grt",      "",         "Garo",                    QObject::tr("Garo")));
	m_langTable.append(LangDef("gez",      "",         "Ge'ez",                   QObject::tr("Ge'ez")));
	m_langTable.append(LangDef("ka",       "",         "Georgian",                QObject::tr("Georgian")));
	m_langTable.append(LangDef("de",       "de_DE",    "German",                  QObject::tr("German")));
	m_langTable.append(LangDef("de_AT",    "",         "German (Austria)",        QObject::tr("German (Austria)")));
	m_langTable.append(LangDef("de_CH",    "",         "German (Swiss)",          QObject::tr("German (Swiss)")));
	m_langTable.append(LangDef("de_1901",  "",         "German (Trad.)",          QObject::tr("German (Trad.)")));
	m_langTable.append(LangDef("glk",      "",         "Gilaki",                  QObject::tr("Gilaki")));
	m_langTable.append(LangDef("niv",      "",         "Gilyak",                  QObject::tr("Gilyak")));
	m_langTable.append(LangDef("gog",      "",         "Gogo",                    QObject::tr("Gogo")));
	m_langTable.append(LangDef("gon",      "",         "Gondi",                   QObject::tr("Gondi")));
	m_langTable.append(LangDef("gno",      "",         "Gondi Northern",          QObject::tr("Gondi Northern")));
	m_langTable.append(LangDef("ggo",      "",         "Gondi Southern",          QObject::tr("Gondi Southern")));
	m_langTable.append(LangDef("el",       "",         "Greek",                   QObject::tr("Greek")));
	m_langTable.append(LangDef("gn",       "",         "Guarani",                 QObject::tr("Guarani")));
	m_langTable.append(LangDef("gu",       "gu_IN",    "Gujarati",                QObject::tr("Gujarati")));
	m_langTable.append(LangDef("guk",      "",         "Gumuz",                   QObject::tr("Gumuz")));
	m_langTable.append(LangDef("guz",      "",         "Gusii",                   QObject::tr("Gusii")));
	m_langTable.append(LangDef("ht",       "",         "Haitian Creole",          QObject::tr("Haitian Creole")));
	m_langTable.append(LangDef("amf",      "",         "Hamer-Banna",             QObject::tr("Hamer-Banna")));
	m_langTable.append(LangDef("har",      "",         "Harari",                  QObject::tr("Harari")));
	m_langTable.append(LangDef("hoj",      "",         "Harauti",                 QObject::tr("Harauti")));
	m_langTable.append(LangDef("bgc",      "",         "Haryanvi",                QObject::tr("Haryanvi")));
	m_langTable.append(LangDef("ha",       "",         "Hausa",                   QObject::tr("Hausa")));
	m_langTable.append(LangDef("haw",      "",         "Hawaiian",                QObject::tr("Hawaiian")));
	m_langTable.append(LangDef("hay",      "",         "Haya",                    QObject::tr("Haya")));
	m_langTable.append(LangDef("haz",      "",         "Hazaragi",                QObject::tr("Hazaragi")));
	m_langTable.append(LangDef("he",       "he_IL",    "Hebrew",                  QObject::tr("Hebrew")));
	m_langTable.append(LangDef("hz",       "",         "Herero",                  QObject::tr("Herero")));
	m_langTable.append(LangDef("mrj",      "",         "High Mari",               QObject::tr("High Mari")));
	m_langTable.append(LangDef("hil",      "",         "Hiligaynon",              QObject::tr("Hiligaynon")));
	m_langTable.append(LangDef("hi_IN",    "",         "Hindi",                   QObject::tr("Hindi")));
	m_langTable.append(LangDef("hno",      "",         "Hindko (Northern)",       QObject::tr("Hindko (Northern)")));
	m_langTable.append(LangDef("hnd",      "",         "Hindko (Southern)",       QObject::tr("Hindko (Southern)")));
	m_langTable.append(LangDef("ho",       "",         "Hiri Motu",               QObject::tr("Hiri Motu")));
	m_langTable.append(LangDef("mww",      "",         "Hmong Daw",               QObject::tr("Hmong Daw")));
	m_langTable.append(LangDef("hoc",      "",         "Ho",                      QObject::tr("Ho")));
	m_langTable.append(LangDef("hu",       "hu_HU",    "Hungarian",               QObject::tr("Hungarian")));
	m_langTable.append(LangDef("ibb",      "",         "Ibibio",                  QObject::tr("Ibibio")));
	m_langTable.append(LangDef("is",       "",         "Icelandic",               QObject::tr("Icelandic")));
	m_langTable.append(LangDef("io",       "",         "Ido",                     QObject::tr("Ido")));
	m_langTable.append(LangDef("ig",       "",         "Igbo",                    QObject::tr("Igbo")));
	m_langTable.append(LangDef("ijo",      "",         "Ijo",                     QObject::tr("Ijo")));
	m_langTable.append(LangDef("ilo",      "",         "Ilokano",                 QObject::tr("Ilokano")));
	m_langTable.append(LangDef("id",       "",         "Indonesian",              QObject::tr("Indonesian")));
	m_langTable.append(LangDef("inh",      "",         "Ingush",                  QObject::tr("Ingush")));
	m_langTable.append(LangDef("ia",       "",         "Interlingua",             QObject::tr("Interlingua")));
	m_langTable.append(LangDef("iu",       "",         "Inuktitut",               QObject::tr("Inuktitut")));
	m_langTable.append(LangDef("ik",       "",         "Inupiaq",                 QObject::tr("Inupiaq")));
	m_langTable.append(LangDef("ga",       "",         "Irish",                   QObject::tr("Irish")));
	m_langTable.append(LangDef("sga",      "",         "Irish Old (to 900)",      QObject::tr("Irish Old (to 900)")));
	m_langTable.append(LangDef("it",       "it_IT",    "Italian",                 QObject::tr("Italian")));
	m_langTable.append(LangDef("ijc",      "",         "Izon",                    QObject::tr("Izon")));
	m_langTable.append(LangDef("jam",      "",         "Jamaican Creole English", QObject::tr("Jamaican Creole English")));
	m_langTable.append(LangDef("ja",       "",         "Japanese",                QObject::tr("Japanese")));
	m_langTable.append(LangDef("jv",       "",         "Javanese",                QObject::tr("Javanese")));
	m_langTable.append(LangDef("dyu",      "",         "Jula",                    QObject::tr("Jula")));
	m_langTable.append(LangDef("quc",      "",         "K'iche'",                 QObject::tr("K'iche'")));
	m_langTable.append(LangDef("kbd",      "",         "Kabardian",               QObject::tr("Kabardian")));
	m_langTable.append(LangDef("kab",      "",         "Kabyle",                  QObject::tr("Kabyle")));
	m_langTable.append(LangDef("kl",       "",         "Kalaallisut",             QObject::tr("Kalaallisut")));
	m_langTable.append(LangDef("xal",      "",         "Kalmyk",                  QObject::tr("Kalmyk")));
	m_langTable.append(LangDef("kln",      "",         "Kalenjin",                QObject::tr("Kalenjin")));
	m_langTable.append(LangDef("kam",      "",         "Kamba (Kenya)",           QObject::tr("Kamba (Kenya)")));
	m_langTable.append(LangDef("bjj",      "",         "Kanauji",                 QObject::tr("Kanauji")));
	m_langTable.append(LangDef("kn_IN",    "kn",       "Kannada",                 QObject::tr("Kannada")));
	m_langTable.append(LangDef("kr",       "",         "Kanuri",                  QObject::tr("Kanuri")));
	m_langTable.append(LangDef("pam",      "",         "Kapampangan",             QObject::tr("Kapampangan")));
	m_langTable.append(LangDef("kdr",      "",         "Karaim",                  QObject::tr("Karaim")));
	m_langTable.append(LangDef("kaa",      "",         "Karakalpak",              QObject::tr("Karakalpak")));
	m_langTable.append(LangDef("krl",      "",         "Karelian",                QObject::tr("Karelian")));
	m_langTable.append(LangDef("kar",      "",         "Karen",                   QObject::tr("Karen")));
	m_langTable.append(LangDef("kfr",      "",         "Kachchi",                 QObject::tr("Kachchi")));
	m_langTable.append(LangDef("ks",       "",         "Kashmiri",                QObject::tr("Kashmiri")));
	m_langTable.append(LangDef("csb",      "",         "Kashubian",               QObject::tr("Kashubian")));
	m_langTable.append(LangDef("kyu",      "",         "Kayah",                   QObject::tr("Kayah")));
	m_langTable.append(LangDef("kk",       "",         "Kazakh",                  QObject::tr("Kazakh")));
	m_langTable.append(LangDef("kjh",      "",         "Khakass",                 QObject::tr("Khakass")));
	m_langTable.append(LangDef("kht",      "",         "Khamti",                  QObject::tr("Khamti")));
	m_langTable.append(LangDef("kha",      "",         "Khasi",                   QObject::tr("Khasi")));
	m_langTable.append(LangDef("km",       "",         "Khmer",                   QObject::tr("Khmer")));
	m_langTable.append(LangDef("khw",      "",         "Khowar",                  QObject::tr("Khowar")));
	m_langTable.append(LangDef("ki",       "",         "Kikuyu",                  QObject::tr("Kikuyu")));
	m_langTable.append(LangDef("sjd",      "",         "Kildin Sami",             QObject::tr("Kildin Sami")));
	m_langTable.append(LangDef("kmb",      "",         "Kimbundu",                QObject::tr("Kimbundu")));
	m_langTable.append(LangDef("rw",       "",         "Kinyarwanda",             QObject::tr("Kinyarwanda")));
	m_langTable.append(LangDef("mwk",      "",         "Kita Maninkakan",         QObject::tr("Kita Maninkakan")));
	m_langTable.append(LangDef("kex",      "",         "Kituba (Congo)",          QObject::tr("Kituba (Congo)")));
	m_langTable.append(LangDef("kex",      "",         "Kokni",                   QObject::tr("Kokni")));
	m_langTable.append(LangDef("ksh",      "",         "Kölsch",                  QObject::trUtf8("K\303\266lsch")));
	m_langTable.append(LangDef("kv",       "",         "Komi",                    QObject::tr("Komi")));
	m_langTable.append(LangDef("koi",      "",         "Komi-Permyak",            QObject::tr("Komi-Permyak")));
	m_langTable.append(LangDef("kpv",      "",         "Komi-Zyrian",             QObject::tr("Komi-Zyrian")));
	m_langTable.append(LangDef("kmw",      "",         "Komo (Congo)",            QObject::tr("Komo (Congo)")));
	m_langTable.append(LangDef("xom",      "",         "Komo (Sudan)",            QObject::tr("Komo (Sudan)")));
	m_langTable.append(LangDef("kxc",      "",         "Komso",                   QObject::tr("Komso")));
	m_langTable.append(LangDef("kg",       "",         "Kongo",                   QObject::tr("Kongo")));
	m_langTable.append(LangDef("kok",      "",         "Konkani",                 QObject::tr("Konkani")));
	m_langTable.append(LangDef("kqy",      "",         "Koorete",                 QObject::tr("Koorete")));
	m_langTable.append(LangDef("ko",       "",         "Korean",                  QObject::tr("Korean")));
	m_langTable.append(LangDef("kpy",      "",         "Koryak",                  QObject::tr("Koryak")));
	m_langTable.append(LangDef("mku",      "",         "Konyanka Maninka",        QObject::tr("Konyanka Maninka")));
	m_langTable.append(LangDef("kpe",      "",         "Kpelle",                  QObject::tr("Kpelle")));
	m_langTable.append(LangDef("kri",      "",         "Krio",                    QObject::tr("Krio")));
	m_langTable.append(LangDef("kxu",      "",         "Kui (India)",             QObject::tr("Kui (India)")));
	m_langTable.append(LangDef("kvd",      "",         "Kui (Indonesia)",         QObject::tr("Kui (Indonesia)")));
	m_langTable.append(LangDef("kfy",      "",         "Kumaoni",                 QObject::tr("Kumaoni")));
	m_langTable.append(LangDef("kum",      "",         "Kumyk",                   QObject::tr("Kumyk")));
	m_langTable.append(LangDef("zum",      "",         "Kumzari",                 QObject::tr("Kumzari")));
	m_langTable.append(LangDef("ku",       "",         "Kurdish",                 QObject::tr("Kurdish")));
	m_langTable.append(LangDef("ckb",      "",         "Kurdish (Sorani)",        QObject::tr("Kurdish (Sorani)")));
	m_langTable.append(LangDef("kru",      "",         "Kurukh",                  QObject::tr("Kurukh")));
	m_langTable.append(LangDef("kdt",      "",         "Kuy",                     QObject::tr("Kuy")));
	m_langTable.append(LangDef("kj",       "",         "Kwanyama",                QObject::tr("Kwanyama")));
	m_langTable.append(LangDef("ky",       "",         "Kyrgyz",                  QObject::tr("Kyrgyz")));
	m_langTable.append(LangDef("lbj",      "",         "Ladakhi",                 QObject::tr("Ladakhi")));
	m_langTable.append(LangDef("lld",      "",         "Ladin",                   QObject::tr("Ladin")));
	m_langTable.append(LangDef("lad",      "",         "Ladino",                  QObject::tr("Ladino")));
	m_langTable.append(LangDef("lbe",      "",         "Lak",                     QObject::tr("Lak")));
	m_langTable.append(LangDef("lki",      "",         "Laki",                    QObject::tr("Laki")));
	m_langTable.append(LangDef("lmn",      "",         "Lambani",                 QObject::tr("Lambani")));
	m_langTable.append(LangDef("ljp",      "",         "Lampung Api",             QObject::tr("Lampung Api")));
	m_langTable.append(LangDef("lmo",      "",         "Lombard",                 QObject::tr("Lombard")));
	m_langTable.append(LangDef("lo",       "",         "Lao",                     QObject::tr("Lao")));
	m_langTable.append(LangDef("la",       "",         "Latin",                   QObject::tr("Latin")));
	m_langTable.append(LangDef("lv",       "",         "Latvian",                 QObject::tr("Latvian")));
	m_langTable.append(LangDef("lzz",      "",         "Laz",                     QObject::tr("Laz")));
	m_langTable.append(LangDef("lez",      "",         "Lezgi",                   QObject::tr("Lezgi")));
	m_langTable.append(LangDef("lij",      "",         "Ligurian",                QObject::tr("Ligurian")));
	m_langTable.append(LangDef("lif",      "",         "Limbu",                   QObject::tr("Limbu")));
	m_langTable.append(LangDef("li",       "",         "Limburgish",              QObject::tr("Limburgish")));
	m_langTable.append(LangDef("ln",       "",         "Lingala",                 QObject::tr("Lingala")));
	m_langTable.append(LangDef("lis",      "",         "Lisu",                    QObject::tr("Lisu")));
	m_langTable.append(LangDef("lt",       "lt_LT",    "Lithuanian",              QObject::tr("Lithuanian")));
	m_langTable.append(LangDef("jbo",      "",         "Lojban",                  QObject::tr("Lojban")));
	m_langTable.append(LangDef("ngl",      "",         "Lomwe",                   QObject::tr("Lomwe")));
	m_langTable.append(LangDef("nds",      "",         "Low German",              QObject::tr("Low German")));
	m_langTable.append(LangDef("mhr",      "",         "Low Mari",                QObject::tr("Low Mari")));
	m_langTable.append(LangDef("dsb",      "",         "Lower Sorbian",           QObject::tr("Lower Sorbian")));
	m_langTable.append(LangDef("lb",       "",         "Luxembourgish",           QObject::tr("Luxembourgish")));
	m_langTable.append(LangDef("khb",      "",         "Lü",                      QObject::trUtf8("L\303\274")));
	m_langTable.append(LangDef("lua",      "",         "Luba-Kasai",              QObject::tr("Luba-Kasai")));
	m_langTable.append(LangDef("lu",       "",         "Luba-Katanga",            QObject::tr("Luba-Katanga")));
	m_langTable.append(LangDef("luo",      "",         "Luo (Kenya and Tanzania)",QObject::tr("Luo (Kenya and Tanzania)")));
	m_langTable.append(LangDef("lrc",      "",         "Luri (Northern)",         QObject::tr("Luri (Northern)")));
	m_langTable.append(LangDef("luz",      "",         "Luri (Southern)",         QObject::tr("Luri (Southern)")));
	m_langTable.append(LangDef("mk",       "",         "Macedonian",              QObject::tr("Macedonian")));
	m_langTable.append(LangDef("mad",      "",         "Madurese",                QObject::tr("Madurese")));
	m_langTable.append(LangDef("mag",      "",         "Magahi",                  QObject::tr("Magahi")));
	m_langTable.append(LangDef("knn",      "",         "Maharashtrian Konkani",   QObject::tr("Maharashtrian Konkani")));
	m_langTable.append(LangDef("mai",      "",         "Maithili",                QObject::tr("Maithili")));
	m_langTable.append(LangDef("mpe",      "",         "Majang",                  QObject::tr("Majang")));
	m_langTable.append(LangDef("mak",      "",         "Makasar",                 QObject::tr("Makasar")));
	m_langTable.append(LangDef("vmw",      "",         "Makhuwa",                 QObject::tr("Makhuwa")));
	m_langTable.append(LangDef("kde",      "",         "Makonde",                 QObject::tr("Makonde")));
	m_langTable.append(LangDef("ms",       "",         "Malay",                   QObject::tr("Malay")));
	m_langTable.append(LangDef("mg",       "",         "Malagasy",                QObject::tr("Malagasy")));
	m_langTable.append(LangDef("ml",       "ml_IN",    "Malayalam",               QObject::tr("Malayalam")));
	m_langTable.append(LangDef("dv",       "",         "Maldivian",               QObject::tr("Maldivian")));
	m_langTable.append(LangDef("mdy",      "",         "Male (Ethiopia)",         QObject::tr("Male (Ethiopia)")));
	m_langTable.append(LangDef("mdc",      "",         "Male (Papua New Guinea)", QObject::tr("Male (Papua New Guinea)")));
	m_langTable.append(LangDef("mt",       "",         "Maltese",                 QObject::tr("Maltese")));
	m_langTable.append(LangDef("mi",       "",         "Maori",                   QObject::tr("Maori")));
	m_langTable.append(LangDef("man",      "",         "Manding",                 QObject::tr("Manding")));
	m_langTable.append(LangDef("mnc",      "",         "Manchu",                  QObject::tr("Manchu")));
	m_langTable.append(LangDef("mdr",      "",         "Mandar",                  QObject::tr("Mandar")));
	m_langTable.append(LangDef("mnk",      "",         "Mandinka",                QObject::tr("Mandinka")));
	m_langTable.append(LangDef("mni",      "",         "Manipuri",                QObject::tr("Manipuri")));
	m_langTable.append(LangDef("emk",      "",         "Maninkakan (Eastern)",    QObject::tr("Maninkakan (Eastern)")));
	m_langTable.append(LangDef("mlq",      "",         "Maninkakan (Western)",    QObject::tr("Maninkakan (Western)")));
	m_langTable.append(LangDef("mns",      "",         "Mansi",                   QObject::tr("Mansi")));
	m_langTable.append(LangDef("gv",       "",         "Manx",                    QObject::tr("Manx")));
	m_langTable.append(LangDef("mr",       "",         "Marathi",                 QObject::tr("Marathi")));
	m_langTable.append(LangDef("mh",       "",         "Marshallese",             QObject::tr("Marshallese")));
	m_langTable.append(LangDef("mwr",      "",         "Marwari",                 QObject::tr("Marwari")));
	m_langTable.append(LangDef("mve",      "",         "Marwari (Pakistan)",      QObject::tr("Marwari (Pakistan)")));
	m_langTable.append(LangDef("mzn",      "",         "Mazanderani",             QObject::tr("Mazanderani")));
	m_langTable.append(LangDef("men",      "",         "Mende (Sierra Leone)",    QObject::tr("Mende (Sierra Leone)")));
	m_langTable.append(LangDef("mym",      "",         "Me'en",                   QObject::tr("Me'en")));
	m_langTable.append(LangDef("mer",      "",         "Meru",                    QObject::tr("Meru")));
	m_langTable.append(LangDef("wry",      "",         "Merwari",                 QObject::tr("Merwari")));
	m_langTable.append(LangDef("mtr",      "",         "Mewari",                  QObject::tr("Mewari")));
	m_langTable.append(LangDef("rwr",      "",         "Mewari (India)",          QObject::tr("Mewari (India)")));
	m_langTable.append(LangDef("wtm",      "",         "Mewati",                  QObject::tr("Mewati")));
	m_langTable.append(LangDef("min",      "",         "Minangkabau",             QObject::tr("Minangkabau")));
	m_langTable.append(LangDef("mwl",      "",         "Mirandese",               QObject::tr("Mirandese")));
	m_langTable.append(LangDef("lus",      "",         "Mizo",                    QObject::tr("Mizo")));
	m_langTable.append(LangDef("mnw",      "",         "Mon",                     QObject::tr("Mon")));
	m_langTable.append(LangDef("moh",      "",         "Mohawk",                  QObject::tr("Mohawk")));
	m_langTable.append(LangDef("mdf",      "",         "Moksha",                  QObject::tr("Moksha")));
	m_langTable.append(LangDef("mo",       "",         "Moldavian",               QObject::tr("Moldavian")));
	m_langTable.append(LangDef("mn_MN",    "",         "Mongolian",               QObject::tr("Mongolian")));
	m_langTable.append(LangDef("mfe",      "",         "Morisyen",                QObject::tr("Morisyen")));
	m_langTable.append(LangDef("mos",      "",         "Mossi",                   QObject::tr("Mossi")));
	m_langTable.append(LangDef("unr",      "",         "Mundari",                 QObject::tr("Mundari")));
	m_langTable.append(LangDef("nqo",      "",         "N'Ko",                    QObject::tr("N'Ko")));
	m_langTable.append(LangDef("nah",      "",         "Nahuatl",                 QObject::tr("Nahuatl")));
	m_langTable.append(LangDef("gld",      "",         "Nanai",                   QObject::tr("Nanai")));
	m_langTable.append(LangDef("nsk",      "",         "Naskapi",                 QObject::tr("Naskapi")));
	m_langTable.append(LangDef("na",       "",         "Nauru",                   QObject::tr("Nauru")));
	m_langTable.append(LangDef("nv",       "",         "Navajo",                  QObject::tr("Navajo")));
	m_langTable.append(LangDef("ndc",      "",         "Ndau",                    QObject::tr("Ndau")));
	m_langTable.append(LangDef("nd",       "",         "Ndebele North",           QObject::tr("North Ndebele")));
	m_langTable.append(LangDef("nr",       "",         "Ndebele South",           QObject::tr("South Ndebele")));
	m_langTable.append(LangDef("ng",       "",         "Ndonga",                  QObject::tr("Ndonga")));
	m_langTable.append(LangDef("nap",      "",         "Neapolitan",              QObject::tr("Neapolitan")));
	m_langTable.append(LangDef("ne_NP",    "",         "Nepali",                  QObject::tr("Nepali")));
	m_langTable.append(LangDef("new",      "",         "Newari",                  QObject::tr("Newari")));
	m_langTable.append(LangDef("nga",      "",         "Ngabaka",                 QObject::tr("Ngabaka")));
	m_langTable.append(LangDef("fuv",      "",         "Nigerian Fulfulde",       QObject::tr("Nigerian Fulfulde")));
	m_langTable.append(LangDef("noe",      "",         "Nimadi",                  QObject::tr("Nimadi")));
	m_langTable.append(LangDef("dap",      "",         "Nisi (India)",            QObject::tr("Nisi (India)")));
	m_langTable.append(LangDef("yso",      "",         "Nisi (China)",            QObject::tr("Nisi (China)")));
	m_langTable.append(LangDef("niu",      "",         "Niuean",                  QObject::tr("Niuean")));
	m_langTable.append(LangDef("nog",      "",         "Nogai",                   QObject::tr("Nogai")));
	m_langTable.append(LangDef("no",       "no_NO",    "Norwegian",               QObject::tr("Norwegian")));
	m_langTable.append(LangDef("nb",       "nb_NO",    "Norwegian (Bokmål)",      QObject::trUtf8("Norwegian (Bokm\303\245l)")));
	m_langTable.append(LangDef("nn",       "nn_NO",    "Norwegian (Nnyorsk)",     QObject::tr("Norwegian (Nnyorsk)")));
	m_langTable.append(LangDef("nov",      "",         "Novial",                  QObject::tr("Novial")));
	m_langTable.append(LangDef("nym",      "",         "Nyamwezi",                QObject::tr("Nyamwezi")));
	m_langTable.append(LangDef("nyn",      "",         "Nyankole",                QObject::tr("Nyankole")));
	m_langTable.append(LangDef("ie",       "",         "Occidental",              QObject::tr("Occidental")));
	m_langTable.append(LangDef("oc",       "",         "Occitan (post 1500)",     QObject::tr("Occitan (post 1500)")));
	m_langTable.append(LangDef("oj",       "",         "Ojibwa",                  QObject::tr("Ojibwa")));
	m_langTable.append(LangDef("ojs",      "",         "Oji-Cree",                QObject::tr("Oji-Cree")));
	m_langTable.append(LangDef("om",       "",         "Oromo",                   QObject::tr("Oromo")));
	m_langTable.append(LangDef("or",       "",         "Oriya",                   QObject::tr("Oriya")));
	m_langTable.append(LangDef("os",       "",         "Ossetian",                QObject::tr("Ossetian")));
	m_langTable.append(LangDef("blk",      "",         "Pa'O",                    QObject::tr("Pa'O")));
	m_langTable.append(LangDef("pce",      "",         "Palaung (Ruching)",       QObject::tr("Palaung (Ruching)")));
	m_langTable.append(LangDef("pll",      "",         "Palaung (Shwe)",          QObject::tr("Palaung (Shwe)")));
	m_langTable.append(LangDef("pi",       "",         "Pali",                    QObject::tr("Pali")));
	m_langTable.append(LangDef("plp",      "",         "Palpa",                   QObject::tr("Palpa")));
	m_langTable.append(LangDef("pa",       "",         "Panjabi",                 QObject::tr("Panjabi")));
	m_langTable.append(LangDef("pnb",      "",         "Panjabi Western",         QObject::tr("Panjabi Western")));
	m_langTable.append(LangDef("pag",      "",         "Pangasinan",              QObject::tr("Pangasinan")));
	m_langTable.append(LangDef("ps",       "",         "Pashto",                  QObject::tr("Pashto")));
	m_langTable.append(LangDef("pdc",      "",         "Pennsylvania German",     QObject::tr("Pennsylvania German")));
	m_langTable.append(LangDef("fa",       "",         "Persian",                 QObject::tr("Persian")));
	m_langTable.append(LangDef("fa_AF",    "",         "Persian (Afghanistan)",   QObject::tr("Persian (Afghanistan)")));
	m_langTable.append(LangDef("fa_IR",    "",         "Persian (Iran)",          QObject::tr("Persian")));
	m_langTable.append(LangDef("phk",      "",         "Phake",                   QObject::tr("Phake")));
	m_langTable.append(LangDef("pih",      "",         "Pitcairn-Norfolk",        QObject::tr("Pitcairn-Norfolk")));
	m_langTable.append(LangDef("pcd",      "",         "Picard",                  QObject::tr("Picard")));
	m_langTable.append(LangDef("pms",      "",         "Piedmontese",             QObject::tr("Piedmontese")));
	m_langTable.append(LangDef("pl",       "pl_PL",    "Polish",                  QObject::tr("Polish")));
	m_langTable.append(LangDef("pap",      "",         "Papiamento",              QObject::tr("Papiamento")));
	m_langTable.append(LangDef("pt",       "pt_PT",    "Portuguese",              QObject::tr("Portuguese")));
	m_langTable.append(LangDef("pt_BR",    "",         "Portuguese (BR)",         QObject::tr("Portuguese (BR)")));
	m_langTable.append(LangDef("kjp",      "",         "Pwo Eastern Karen",       QObject::tr("Pwo Eastern Karen")));
	m_langTable.append(LangDef("pwo",      "",         "Pwo Western Karen",       QObject::tr("Pwo Western Karen")));
	m_langTable.append(LangDef("bto",      "",         "Rinconada Bikol",         QObject::tr("Rinconada Bikol")));
	m_langTable.append(LangDef("qu",       "",         "Quechua",                 QObject::tr("Quechua")));
	m_langTable.append(LangDef("rki",      "",         "Rakhine",                 QObject::tr("Rakhine")));
	m_langTable.append(LangDef("raj",      "",         "Rajasthani",              QObject::tr("Rajasthani")));
	m_langTable.append(LangDef("rej",      "",         "Rejang",                  QObject::tr("Rejang")));
	m_langTable.append(LangDef("ria",      "",         "Riang (India)",           QObject::tr("Riang (India)")));
	m_langTable.append(LangDef("ril",      "",         "Riang (Myanmar)",         QObject::tr("Riang (Myanmar)")));
	m_langTable.append(LangDef("ro",       "",         "Romanian",                QObject::tr("Romanian")));
	m_langTable.append(LangDef("rm",       "",         "Romansh",                 QObject::tr("Romansh")));
	m_langTable.append(LangDef("rom",      "",         "Romany",                  QObject::tr("Romany")));
	m_langTable.append(LangDef("rn",       "",         "Rundi",                   QObject::tr("Rundi")));
	m_langTable.append(LangDef("ru",       "ru_RU",    "Russian",                 QObject::tr("Russian")));
	m_langTable.append(LangDef("bxr",      "ru_RU",    "Russian Buriat",          QObject::tr("Russian Buriat")));
	m_langTable.append(LangDef("rue",      "",         "Rusyn",                   QObject::tr("Rusyn")));
	m_langTable.append(LangDef("ksw",      "",         "S'gaw Karen",             QObject::tr("S'gaw Karen")));
	m_langTable.append(LangDef("sck",      "",         "Sadri",                   QObject::tr("Sadri")));
	m_langTable.append(LangDef("msc",      "",         "Sasak",                   QObject::tr("Sasak")));
	m_langTable.append(LangDef("sm",       "",         "Samoan",                  QObject::tr("Samoan")));
	m_langTable.append(LangDef("sgs",      "",         "Samogitian",              QObject::tr("Samogitian")));
	m_langTable.append(LangDef("se",       "",         "Sami (Northern)",         QObject::tr("Sami (Northern)")));
	m_langTable.append(LangDef("sma",      "",         "Sami (Southern)",         QObject::tr("Sami (Southern)")));
	m_langTable.append(LangDef("smj",      "",         "Sami (Lule)",             QObject::tr("Sami (Lule)")));
	m_langTable.append(LangDef("sms",      "",         "Sami (Skolt)",            QObject::tr("Sami (Skolt)")));
	m_langTable.append(LangDef("sg",       "",         "Sango",                   QObject::tr("Sango")));
	m_langTable.append(LangDef("msc",      "",         "Sankaran Maninka",        QObject::tr("Sankaran Maninka")));
	m_langTable.append(LangDef("sa",       "",         "Sanskrit",                QObject::tr("Sanskrit")));
	m_langTable.append(LangDef("sat",      "",         "Santali",                 QObject::tr("Santali")));
	m_langTable.append(LangDef("sc",       "",         "Sardinian",               QObject::tr("Sardinian")));
	m_langTable.append(LangDef("stq",      "",         "Saterfriesisch",          QObject::tr("Saterfriesisch")));
	m_langTable.append(LangDef("sco",      "",         "Scots",                   QObject::tr("Scots")));
	m_langTable.append(LangDef("gd",       "gd_GB",    "Scottish Gaelic",         QObject::tr("Scottish Gaelic")));
	m_langTable.append(LangDef("sgw",      "",         "Sebat Bet Gurage",        QObject::tr("Sebat Bet Gurage")));
	m_langTable.append(LangDef("sel",      "",         "Selkup",                  QObject::tr("Selkup")));
	m_langTable.append(LangDef("seh",      "",         "Sena",                    QObject::tr("Sena")));
	m_langTable.append(LangDef("skr",      "",         "Seraiki",                 QObject::tr("Seraiki")));
	m_langTable.append(LangDef("sr",       "",         "Serbian",                 QObject::tr("Serbian")));
	m_langTable.append(LangDef("sr-Latn",  "sr-Latn",  "Serbian (Latin)",         QObject::tr("Serbian (Latin)")));
	m_langTable.append(LangDef("shn",      "",         "Shan",                    QObject::tr("Shan")));
	m_langTable.append(LangDef("srr",      "",         "Serer",                   QObject::tr("Serer")));
	m_langTable.append(LangDef("swv",      "",         "Shekhawati",              QObject::tr("Shekhawati")));
	m_langTable.append(LangDef("sn",       "",         "Shona",                   QObject::tr("Shona")));
	m_langTable.append(LangDef("nco",      "",         "Sibe",                    QObject::tr("Sibe")));
	m_langTable.append(LangDef("scn",      "",         "Sicilian",                QObject::tr("Sicilian")));
	m_langTable.append(LangDef("sid",      "",         "Sidamo",                  QObject::tr("Sidamo")));
	m_langTable.append(LangDef("szl",      "",         "Silesian",                QObject::tr("Silesian")));
	m_langTable.append(LangDef("stv",      "",         "Silt'e",                  QObject::tr("Silt'e")));
	m_langTable.append(LangDef("sd",       "",         "Sindhi",                  QObject::tr("Sindhi")));
	m_langTable.append(LangDef("si",       "",         "Sinhala",                 QObject::tr("Sinhala")));
	m_langTable.append(LangDef("scs",      "",         "Slavey (North)",          QObject::tr("Slavey (North)")));
	m_langTable.append(LangDef("xsl",      "",         "Slavey (South)",          QObject::tr("Slavey (South)")));
	m_langTable.append(LangDef("sk",       "sk_SK",    "Slovak",                  QObject::tr("Slovak")));
	m_langTable.append(LangDef("sl",       "sl_SL",    "Slovenian",               QObject::tr("Slovenian")));
	m_langTable.append(LangDef("gru",      "",         "Sodo Gurage",             QObject::tr("Sodo Gurage")));
	m_langTable.append(LangDef("so",       "",         "Somali",                  QObject::tr("Somali")));
	m_langTable.append(LangDef("xog",      "",         "Soga",                    QObject::tr("Soga")));
	m_langTable.append(LangDef("sop",      "",         "Songe",                   QObject::tr("Songe")));
	m_langTable.append(LangDef("snk",      "",         "Soninke",                 QObject::tr("Soninke")));
	m_langTable.append(LangDef("nso",      "",         "Sotho Northern",          QObject::tr("Sotho Northern")));
	m_langTable.append(LangDef("st",       "",         "Sotho Southern",          QObject::tr("Sotho Southern")));
	m_langTable.append(LangDef("es",       "es_ES",    "Spanish",                 QObject::tr("Spanish")));
	m_langTable.append(LangDef("es",       "es_ANY",   "Spanish",                 QObject::tr("Spanish")));
	m_langTable.append(LangDef("es_AR",    "",         "Spanish (Argentina)",     QObject::tr("Spanish (Argentina)")));
	m_langTable.append(LangDef("es_LA",    "",         "Spanish (Latin)",         QObject::tr("Spanish (Latin)")));
	m_langTable.append(LangDef("aii",      "",         "Swadaya Aramaic",         QObject::tr("Swadaya Aramaic")));
	m_langTable.append(LangDef("suk",      "",         "Sukama",                  QObject::tr("Sukama")));
	m_langTable.append(LangDef("su",       "",         "Sundanese",               QObject::tr("Sundanese")));
	m_langTable.append(LangDef("suq",      "",         "Suri",                    QObject::tr("Suri")));
	m_langTable.append(LangDef("sva",      "",         "Svan",                    QObject::tr("Svan")));
	m_langTable.append(LangDef("sw",       "",         "Swahili",                 QObject::tr("Swahili")));
	m_langTable.append(LangDef("sv",       "",         "Swedish",                 QObject::tr("Swedish")));
	m_langTable.append(LangDef("ss",       "",         "Swati",                   QObject::tr("Swati")));
	m_langTable.append(LangDef("syl",      "",         "Sylheti",                 QObject::tr("Sylheti")));
	m_langTable.append(LangDef("syr",      "",         "Syriac",                  QObject::tr("Syriac")));
	m_langTable.append(LangDef("tyz",      "",         "Tày",                     QObject::trUtf8("T\303\240y")));
	m_langTable.append(LangDef("tab",      "",         "Tabasaran",               QObject::tr("Tabasaran")));
	m_langTable.append(LangDef("tl",       "",         "Tagalog",                 QObject::tr("Tagalog")));
	m_langTable.append(LangDef("ty",       "",         "Tahitian",                QObject::tr("Tahitian")));
	m_langTable.append(LangDef("tdd",      "",         "Tai Nüa",                 QObject::trUtf8("Tai N\303\274a")));
	m_langTable.append(LangDef("tg",       "",         "Tajik",                   QObject::tr("Tajik")));
	m_langTable.append(LangDef("tmh",      "",         "Tamashek",                QObject::tr("Tamashek")));
	m_langTable.append(LangDef("ta",       "",         "Tamil",                   QObject::tr("Tamil")));
	m_langTable.append(LangDef("tt",       "",         "Tatar",                   QObject::tr("Tatar")));
	m_langTable.append(LangDef("te",       "te_IN",    "Telugu",                  QObject::tr("Telugu")));
	m_langTable.append(LangDef("tem",      "",         "Temne",                   QObject::tr("Temne")));
	m_langTable.append(LangDef("tet",      "",         "Tetum",                   QObject::tr("Tetum")));
	m_langTable.append(LangDef("th",       "th_TH",    "Thai",                    QObject::tr("Thai")));
	m_langTable.append(LangDef("nod",      "",         "Thai Northern",           QObject::tr("Thai Northern")));
	m_langTable.append(LangDef("bo",       "",         "Tibetan",                 QObject::tr("Tibetan")));
	m_langTable.append(LangDef("tig",      "",         "Tigre",                   QObject::tr("Tigre")));
	m_langTable.append(LangDef("ti",       "",         "Tigrinya",                QObject::tr("Tigrinya")));
	m_langTable.append(LangDef("tiv",      "",         "Tiv",                     QObject::tr("Tiv")));
	m_langTable.append(LangDef("tpi",      "",         "Tok Pisin",               QObject::tr("Tok Pisin")));
	m_langTable.append(LangDef("to",       "",         "Tonga (Tonga Islands)",   QObject::tr("Tonga (Tonga Islands)")));
	m_langTable.append(LangDef("ts",       "",         "Tsonga",                  QObject::tr("Tsonga")));
	m_langTable.append(LangDef("tn",       "",         "Tswana",                  QObject::tr("Tswana")));
	m_langTable.append(LangDef("tcy",      "",         "Tulu",                    QObject::tr("Tulu")));
	m_langTable.append(LangDef("tum",      "",         "Tumbuka",                 QObject::tr("Tumbuka")));
	m_langTable.append(LangDef("tr",       "tr_TR",    "Turkish",                 QObject::tr("Turkish")));
	m_langTable.append(LangDef("tk",       "",         "Turkmen",                 QObject::tr("Turkmen")));
	m_langTable.append(LangDef("tru",      "",         "Turoyo Aramaic",          QObject::tr("Turoyo Aramaic")));
	m_langTable.append(LangDef("tyv",      "",         "Tuvin",                   QObject::tr("Tuvin")));
	m_langTable.append(LangDef("tw",       "",         "Twi",                     QObject::tr("Twi")));
	m_langTable.append(LangDef("uk",       "uk_UA",    "Ukranian",                QObject::tr("Ukranian")));
	m_langTable.append(LangDef("sxu",      "",         "Upper Saxon",             QObject::tr("Upper Saxon")));
	m_langTable.append(LangDef("hsb",      "",         "Upper Sorbian",           QObject::tr("Upper Sorbian")));
	m_langTable.append(LangDef("udm",      "",         "Udmurt",                  QObject::tr("Udmurt")));
	m_langTable.append(LangDef("ug",       "",         "Uighur",                  QObject::tr("Uighur")));
	m_langTable.append(LangDef("umb",      "",         "Umbundu",                 QObject::tr("Umbundu")));
	m_langTable.append(LangDef("ur",       "",         "Urdu",                    QObject::tr("Urdu")));
	m_langTable.append(LangDef("uk",       "",         "Ukrainian",               QObject::tr("Ukrainian")));
	m_langTable.append(LangDef("uz",       "",         "Uzbek",                   QObject::tr("Uzbek")));
	m_langTable.append(LangDef("uzn",      "",         "Uzbek (Northern)",        QObject::tr("Uzbek (Northern)")));
	m_langTable.append(LangDef("uzs",      "",         "Uzbek (Southern)",        QObject::tr("Uzbek (Southern)")));
	m_langTable.append(LangDef("ve",       "",         "Venda",                   QObject::tr("Venda")));
	m_langTable.append(LangDef("vec",      "",         "Venetian",                QObject::tr("Venetian")));
	m_langTable.append(LangDef("vi",       "",         "Vietnamese",              QObject::tr("Vietnamese")));
	m_langTable.append(LangDef("vls",      "",         "Vlaams",                  QObject::tr("Vlaams")));
	m_langTable.append(LangDef("rmy",      "",         "Vlax Romani",             QObject::tr("Vlax Romani")));
	m_langTable.append(LangDef("vo",       "",         "Volapük",                 QObject::trUtf8("Volap\303\274k")));
	m_langTable.append(LangDef("vro",      "",         "Võro",                    QObject::trUtf8("V\303\265ro")));
	m_langTable.append(LangDef("wbm",      "",         "Wa",                      QObject::tr("Wa")));
	m_langTable.append(LangDef("wbr",      "",         "Wagdi",                   QObject::tr("Wagdi")));
	m_langTable.append(LangDef("wa",       "",         "Walloon",                 QObject::tr("Walloon")));
	m_langTable.append(LangDef("war",      "",         "Waray (Philippines)",     QObject::tr("Waray (Philippines)")));
	m_langTable.append(LangDef("guc",      "cy_GB",    "Wayuu",                   QObject::tr("Wayuu")));
	m_langTable.append(LangDef("cy",       "cy_GB",    "Welsh",                   QObject::tr("Welsh")));
	m_langTable.append(LangDef("wle",      "",         "Wolane",                  QObject::tr("Wolane")));
	m_langTable.append(LangDef("wo",       "",         "Wolof",                   QObject::tr("Wolof")));
	m_langTable.append(LangDef("xwo",      "",         "Written Oirat (Todo)",    QObject::tr("Written Oirat (Todo)")));
	m_langTable.append(LangDef("xh",       "",         "Xhosa",                   QObject::tr("Xhosa")));
	m_langTable.append(LangDef("yao",      "",         "Yao",                     QObject::tr("Yao")));
	m_langTable.append(LangDef("sah",      "",         "Yakut",                   QObject::tr("Yakut")));
	m_langTable.append(LangDef("yi",       "",         "Yiddish",                 QObject::tr("Yiddish")));
	m_langTable.append(LangDef("yo",       "",         "Yoruba",                  QObject::tr("Yoruba")));
	m_langTable.append(LangDef("zne",      "",         "Zande",                   QObject::tr("Zande")));
	m_langTable.append(LangDef("dje",      "cy_GB",    "Zarma",                   QObject::tr("Zarma")));
	m_langTable.append(LangDef("zea",      "",         "Zeeuws",                  QObject::tr("Zeeuws")));
	m_langTable.append(LangDef("za",       "",         "Zhuang",                  QObject::tr("Zhuang")));
	m_langTable.append(LangDef("zu",       "",         "Zulu",                    QObject::tr("Zulu")));
}

void LanguageManager::generateInstalledGUILangList()
{
	QString path = ScPaths::instance().translationDir();
	QString langAbbrev;
	QMap<QString, langPair>::Iterator it;
	QDir dir(path , "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if (dir.exists() && (dir.count() != 0))
	{
		for (uint i = 0; i < dir.count(); ++i) 
		{
			QFileInfo file(path + dir[i]);
			if (file.suffix().toLower() == "qm")
			{
				langAbbrev = file.completeSuffix().remove(".qm");
				int j=langTableIndex(langAbbrev);
				if (j!=-1)
				{
					m_langTable[j].m_transAvailable=true;
					m_langTable[j].m_transFile=file.absoluteFilePath();
					//qDebug()<<"Found installed GUI translation file:"<<langAbbrev<<" : "<<langTable[j].m_transFile<<langTable[j].m_transName;
				}
			}
		}
	}
}

void LanguageManager::generateInstalledHyphLangList()
{
	QStringList dictionaryPaths;
	bool dictPathFound=findHyphDictionaries(dictionaryPaths);
	if (!dictPathFound)
	{
//		qDebug()<<"No preinstalled hyphenation dictonaries or paths found";
		return;
	}
	QMap<QString, QString> dictionaryMap;
	findHyphDictionarySets(dictionaryPaths, dictionaryMap);
	if (dictionaryMap.count()==0)
		return;

	QMap<QString, QString>::iterator it = dictionaryMap.begin();
//	qDebug()<<"Installed Hyphenation Dictonaries:";
	while (it != dictionaryMap.end())
	{
		int j=langTableIndex(it.key());
		if (j!=-1)
		{
			m_langTable[j].m_hyphAvailable=true;
			m_langTable[j].m_hyphFile=it.value();
			//qDebug()<<"Found installed hyphenation dictionary:"<<it.key()<<" : "<<it.value();
		}
		++it;
	}
}

void LanguageManager::generateInstalledSpellLangList()
{
	QStringList dictionaryPaths;
	bool dictPathFound=findSpellingDictionaries(dictionaryPaths);
	if (!dictPathFound)
	{
		qDebug()<<"No preinstalled spelling dictonaries or paths found";
		return;
	}
	QMap<QString, QString> dictionaryMap;
	findSpellingDictionarySets(dictionaryPaths, dictionaryMap);
	if (dictionaryMap.count()==0)
		return;

	QMap<QString, QString>::iterator it = dictionaryMap.begin();
//	qDebug()<<"Installed Spelling Dictonaries:";
	while (it != dictionaryMap.end())
	{
		int j=langTableIndex(it.key());
		if (j!=-1)
		{
			m_langTable[j].m_spellAvailable=true;
			m_langTable[j].m_spellFile=it.value();
//			qDebug()<<"Found installed spelling dictionary:"<<it.key()<<" : "<<it.value();
		}
		++it;
	}
}



int LanguageManager::langTableIndex(const QString &abbrev)
{
//	qDebug()<<"langTableIndex: Trying to find:"<<abbrev;
	if (abbrev.isEmpty())
		return -1;
	for (int i = 0; i < m_langTable.size(); ++i)
	{
//		qDebug()<<abbrev<<langTable[i].m_priAbbrev<<langTable[i].m_altAbbrev;
		if (m_langTable[i].m_priAbbrev==abbrev || m_langTable[i].m_altAbbrev==abbrev)
			return i;
	}
	return -1;
}

const QString LanguageManager::getLangFromAbbrev(QString langAbbrev, bool getTranslated)
{
//	qDebug()<<"Trying to find:"<<langAbbrev;
	int i=langTableIndex(langAbbrev);
//	qDebug()<<"Index of"<<langAbbrev<<":"<<i;
	if (i==-1)
	{
		if (langAbbrev.length()>5)
		{
			langAbbrev.truncate(5);
			i=langTableIndex(langAbbrev);
		}
	}
	if (i!=-1)
	{
		if (getTranslated)
			return m_langTable[i].m_transName;
		else
			return m_langTable[i].m_name;
	}
	//qDebug()<<langAbbrev<<"not found";
	return "";
}

const QString LanguageManager::getAbbrevFromLang(QString lang, bool useInstalled)
{
	for (int i = 0; i < m_langTable.size(); ++i)
	{
		if (useInstalled)
		{
			if (m_langTable[i].m_transAvailable && (m_langTable[i].m_name==lang || m_langTable[i].m_transName==lang))
				return m_langTable[i].m_priAbbrev;
		}
		else
		{
			//qDebug()<<lang<<langTable[i].m_priAbbrev<<langTable[i].m_name<<langTable[i].m_transName;
			if (m_langTable[i].m_name==lang || m_langTable[i].m_transName==lang)
				return m_langTable[i].m_priAbbrev;
		}
	}
	return "";
}

const QString LanguageManager::getLangFromTransLang(QString transLang)
{
	for (int i = 0; i < m_langTable.size(); ++i)
	{
		if (m_langTable[i].m_transName==transLang)
			return m_langTable[i].m_name;
	}
	return "";
}

const QString LanguageManager::getTransLangFromLang(QString lang)
{
	for (int i = 0; i < m_langTable.size(); ++i)
	{
		if (m_langTable[i].m_name==lang)
			return m_langTable[i].m_transName;
	}
	return "";
}

const QString LanguageManager::getShortAbbrevFromAbbrev(QString langAbbrev)
{
	//	qDebug()<<"Trying to find:"<<langAbbrev;
	int i = langTableIndex(langAbbrev);
	//	qDebug()<<"Index of"<<langAbbrev<<":"<<i;
	if (i == -1)
	{
		if (langAbbrev.length()>5)
		{
			langAbbrev.truncate(5);
			i=langTableIndex(langAbbrev);
		}
	}
	if (i != -1)
		return m_langTable[i].m_priAbbrev;
	//qDebug()<<langAbbrev<<"not found";
	return "";
}

const QString LanguageManager::getShortAbbrevFromAbbrevDecomposition(QString langAbbrev)
{
	int tIndex = langTableIndex(langAbbrev);
	if (tIndex >= 0)
		return m_langTable[tIndex].m_priAbbrev;

	QStringList abbrevs = getAbbrevDecomposition(langAbbrev);
	for (int i = 1; i < abbrevs.count(); ++i)
	{
		tIndex = langTableIndex(abbrevs.at(i));
		if (tIndex >= 0)
			return m_langTable[tIndex].m_priAbbrev;
	}

	return "";
}

const QString LanguageManager::getAlternativeAbbrevfromAbbrev(QString langAbbrev)
{
	int i=langTableIndex(langAbbrev);
	if (i!=-1)
		return m_langTable[i].m_altAbbrev;
	return "";
}

QStringList LanguageManager::getAbbrevDecomposition(QString langAbbrev)
{
	QStringList abbrevs;
	abbrevs.append(langAbbrev);

	QString curAbbrev  = langAbbrev;
	QString delimiters = QString::fromLatin1("_.");

	while (true)
	{
		int rightMost = 0;
		for (int i = 0; i < delimiters.length(); i++)
		{
			int k = curAbbrev.lastIndexOf(delimiters[i]);
			if (k > rightMost)
				rightMost = k;
		}

		if (rightMost == 0)
			break;

		curAbbrev.truncate(rightMost);
		abbrevs.append(curAbbrev);
	}
	return abbrevs;
}

void LanguageManager::fillInstalledStringList(QStringList *stringListToFill)
{
	if (!stringListToFill)
		return;
	for (int i = 0; i < m_langTable.size(); ++i)
		stringListToFill->append(m_langTable[i].m_transName);
}

void LanguageManager::fillInstalledGUIStringList(QStringList *stringListToFill)
{
	if (!stringListToFill)
		return;
	for (int i = 0; i < m_langTable.size(); ++i)
	{
		if (m_langTable[i].m_transAvailable)
			stringListToFill->append(m_langTable[i].m_transName);
	}
}

void LanguageManager::fillInstalledHyphStringList(QStringList *stringListToFill)
{
	if (!stringListToFill)
		return;
	for (int i = 0; i < m_langTable.size(); ++i)
	{
		if (m_langTable[i].m_hyphAvailable)
			stringListToFill->append(m_langTable[i].m_transName);
	}
	stringListToFill->sort();
}

void LanguageManager::fillInstalledSpellStringList(QStringList *stringListToFill)
{
	if (!stringListToFill)
		return;
	for (int i = 0; i < m_langTable.size(); ++i)
	{
		if (m_langTable[i].m_spellAvailable)
			stringListToFill->append(m_langTable[i].m_transName);
	}
	stringListToFill->sort();
}

bool LanguageManager::isAvailableTranslation(QString langAbbrev)
{
	if (langAbbrev == "en" || langAbbrev == "en_GB")
		return true;

	int i = langTableIndex(langAbbrev);
	if (i == -1)
	{
		if (langAbbrev.length() > 5)
		{
			langAbbrev.truncate(5);
			i = langTableIndex(langAbbrev);
		}
	}
	if (i !=-1)
		return m_langTable[i].m_transAvailable;
	return false;
}

QStringList LanguageManager::languageList(bool getTranslated)
{
	QStringList sl;
	for (int i = 0; i < m_langTable.size(); ++i)
	{
		if (getTranslated)
			sl<<m_langTable[i].m_transName;
		else
			sl<<m_langTable[i].m_name;
	}
	sl.sort();
	return sl;
}

void LanguageManager::printInstalledList()
{
	QFile f;
	f.open(stderr, QIODevice::WriteOnly);
	QTextStream ts(&f);
	for (int i = 0; i < m_langTable.size(); ++i)
	{
		ts << "  " << m_langTable[i].m_priAbbrev.leftJustified(8) << ": " << m_langTable[i].m_name;
		endl(ts);
	}
	endl(ts);
	f.close();
}

QString LanguageManager::numericSequence(QString seq)
{
	QString retSeq;
	const int nsBengali=0,nsDevanagari=1,nsGujarati=2,nsGurumukhi=3,nsKannada=4,nsMalayalam=5,nsOriya=6,nsTamil=7,nsTelugu=8,nsTibetan=9,nsLepcha=10;
	switch (1)
	{
		case nsBengali:
			retSeq+="";
			break;
		case nsDevanagari:
			retSeq+="०१२३४५६७८९";
			break;
		case nsGujarati:
			retSeq+="૦૧૨૩૪૫૬૭૮૯";
			break;
		case nsGurumukhi:
			retSeq+="੦੧੨੩੪੫੬੭੮੯";
			break;
		case nsKannada:
			retSeq+="";
			break;
		case nsMalayalam:
			retSeq+="";
			break;
		case nsOriya:
			retSeq+="";
			break;
		case nsTamil:
			retSeq+="";
			break;
		case nsTelugu:
			retSeq+="";
			break;
		case nsTibetan:
			retSeq+="";
			break;
		case nsLepcha:
			retSeq+="";
			break;
		default:
			retSeq="0123456789";
			break;
	}
	return retSeq;
}

bool LanguageManager::findSpellingDictionaries(QStringList &sl)
{
	sl=ScPaths::instance().spellDirs();
	if (sl.count()==0)
		return false;
	return true;
}

void LanguageManager::findSpellingDictionarySets(QStringList &dictionaryPaths, QMap<QString, QString> &dictionaryMap)
{
	for (int i=0; i<dictionaryPaths.count(); ++i)
	{
		// Find the dic and aff files in the location
		QDir dictLocation(dictionaryPaths.at(i));
		QStringList dictFilters("*.dic");
		QStringList dictList(dictLocation.entryList(dictFilters, QDir::Files, QDir::Name));
		dictList.replaceInStrings(".dic","");

		//Ensure we have aff+dic file pairs, remove any hyphenation dictionaries from the list

		foreach(const QString& dictName, dictList)
		{
			if (!QFile::exists(dictionaryPaths.at(i)+dictName+".aff"))
				dictList.removeAll(dictName);
			else
			{
				if (!dictionaryMap.contains(dictName))
				{
					if (dictName.length()<=5)
					{
						//QString shortAbbrev(LanguageManager::getShortAbbrevFromAbbrev(dictName));
						//qDebug()<<"findSpellingDictionarySets"<<dictName<<shortAbbrev;
						dictionaryMap.insert(dictName, dictionaryPaths.at(i)+dictName);
						//dictionaryMap.insert(shortAbbrev, dictionaryPaths.at(i)+dictName);
					}
					//qDebug()<<"Spell Finder:"<<dictName<<dictionaryPaths.at(i)+dictName;
					if (dictName.length()>5)
					{
						QString shortAbbrev(LanguageManager::getShortAbbrevFromAbbrev(dictName));
						//qDebug()<<shortAbbrev;
						dictionaryMap.insert(shortAbbrev, dictionaryPaths.at(i)+dictName);
					}
				}
			}
		}
//		qDebug()<<"Number of dictionaries/AFFs found in"<<dictionaryPaths.at(i)<<":"<<dictList.count();
	}
	//Now rescan dictionary map for any extra languages we can support with the files we have
	QMap<QString, QString>::iterator it = dictionaryMap.begin();
	while (it != dictionaryMap.end())
	{
		QString lang(it.key());
		if (lang.length()==5)
		{
			QString shortAbbrev(LanguageManager::getShortAbbrevFromAbbrev(lang));
			if (!dictionaryMap.contains(shortAbbrev))
			{
				//qDebug()<<"Adding extra spelling definitions for:"<<lang<<":"<<shortAbbrev;
				dictionaryMap.insert(shortAbbrev, it.value());
			}
			//else
				//qDebug()<<"Short abbreviation:"<<shortAbbrev<<"already exists for:"<<lang;
		}
		if (lang.length()==2)
		{
			QString altAbbrev(LanguageManager::getAlternativeAbbrevfromAbbrev(lang));
			if (!dictionaryMap.contains(altAbbrev))
			{
				//qDebug()<<"Adding extra spelling definitions for:"<<lang<<":"<<altAbbrev;
				dictionaryMap.insert(altAbbrev, it.value());
			}
			//else
				//qDebug()<<"Alt. abbreviation:"<<altAbbrev<<"already exists for:"<<lang;
		}
		++it;
	}
}

bool LanguageManager::findHyphDictionaries(QStringList& sl)
{
	sl=ScPaths::instance().hyphDirs();
	if (sl.count()==0)
		return false;
	return true;
}

void LanguageManager::findHyphDictionarySets(QStringList& dictionaryPaths, QMap<QString, QString>& dictionaryMap)
{
	for (int i=0; i<dictionaryPaths.count(); ++i)
	{
		// Find the dic and aff files in the location
		QDir dictLocation(dictionaryPaths.at(i));
		QStringList dictFilters("hyph*.dic");
		if (dictionaryPaths.at(i)==ScPaths::userDictDir(ScPaths::Hyph, false))
				dictFilters.append("*.dic");
		QStringList dictList(dictLocation.entryList(dictFilters, QDir::Files, QDir::Name));
		dictList.replaceInStrings(".dic","");
		foreach(const QString& dn, dictList)
		{
//			qDebug()<<dn;
			QString dictName;
			if (dn.startsWith("hyph_"))
				dictName=dn.section('_',1);
			else
				dictName=dn;
			if (!dictionaryMap.contains(dictName))
			{
				if (dictName.length()<=2)
				{
					dictionaryMap.insert(dictName, dictionaryPaths.at(i)+dn+".dic");
				}
				if (dictName.length()>2)
				{
					QString shortAbbrev(LanguageManager::getShortAbbrevFromAbbrev(dictName));
					dictionaryMap.insert(shortAbbrev, dictionaryPaths.at(i)+dn+".dic");
				}
			}
		}
	}
}

LanguageManager::~LanguageManager()
{
	m_langTable.clear();
}

const QString LanguageManager::getHyphFilename(const QString & langAbbrev)
{
	int j = langTableIndex(langAbbrev);
	if (j!=-1 && m_langTable[j].m_hyphAvailable)
	{
//		qDebug()<<"Found requested hyphenation dictionary:"<<langAbbrev<<" : "<<langTable[j].m_hyphFile;
		return m_langTable[j].m_hyphFile;
	}
	return QString();
}

