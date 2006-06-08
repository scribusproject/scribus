/*
 *  desaxe_conf.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */

#ifndef DESAXE_CONF_H
#define DESAXE_CONF_H


// disable lots of diagnostic messages
#undef DESAXE_DEBUG


// choose datatypes for element tags and attributes
#define DESAXE_QT

#ifdef DESAXE_QT

#include <qstring.h>
#include <qmap.h>

typedef QString Xml_string;
typedef QMap<Xml_string, Xml_string> Xml_attr;

inline Xml_string Xml_key(Xml_attr::iterator& it) { return it.key(); }
inline Xml_string Xml_data(Xml_attr::iterator& it) { return it.data(); }

#else

#include <string>
#include <utility>
#include <map>

typedef std::string Xml_string;
typedef std::map<Xml_string, Xml_string> Xml_attr;

inline Xml_string Xml_key(Xml_attr::iterator& it) { return it->first; }
inline Xml_string Xml_data(Xml_attr::iterator& it) { return it->second; }

#endif

#endif
