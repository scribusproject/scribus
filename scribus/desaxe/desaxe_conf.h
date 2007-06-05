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

#include <string>
#include <QString>
#include <QMap>

typedef QString Xml_string;
typedef QMap<Xml_string, Xml_string> Xml_attr;

inline Xml_string Xml_key(Xml_attr::iterator& it) { return it.key(); }
inline Xml_string Xml_data(Xml_attr::iterator& it) { return it.value(); }
inline std::string fromXMLString(const Xml_string& s) { QByteArray ba(s.toUtf8()); return std::string(ba.constData()); }
inline Xml_string fromSTLString(const std::string& s) { return QString::fromUtf8(s.c_str()); }

#else

#include <string>
#include <utility>
#include <map>

typedef std::string Xml_string;
typedef std::map<Xml_string, Xml_string> Xml_attr;

inline Xml_string Xml_key(Xml_attr::iterator& it) { return it->first; }
inline Xml_string Xml_data(Xml_attr::iterator& it) { return it->second; }
inline const char* fromXMLString(const Xml_string& s) { return s.c_str(); }
inline Xml_string fromSTLString(const std::string& s) { return s; }

#endif

#endif
