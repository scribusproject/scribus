#ifndef SAXHELPER_H
#define SAXHELPER_H

#include <qstring.h>

class ScFace;
class FPointArray;

QString toXMLString(uint val);
QString toXMLString(int val);
QString toXMLString(double val);
QString toXMLString(const QString& val);
QString toXMLString(bool val);
QString toXMLString(const ScFace& val);
QString toXMLString(bool what);
QString toXMLString(const FPointArray& path);

#endif
