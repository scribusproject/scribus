#ifndef SCFONTS_ENCODING_H
#define SCFONTS_ENCODING_H

#include <qstring.h>
#include <qmap.h>

class SCFonts_Encoding
{
	public:
		SCFonts_Encoding()
		{
		}
		SCFonts_Encoding(QString name);
		bool read(QString name);
		bool present(int code)
		{
			return(Names.find(code)!=Names.end());
		}
		bool present(QString &name)
		{
			return(Codes.find(name)!=Codes.end());
		}
		QString &operator[](int code)
		{
			return(Names[code]);
		}
		int &operator[](QString &name)
		{
			return(Codes[name]);
		}
		QMap<int,QString>::Iterator beginByCode()
		{
			return(Names.begin());
		}
		QMap<int,QString>::Iterator endByCode()
		{
			return(Names.end());
		}
		QMap<QString,int>::Iterator beginByName()
		{
			return(Codes.begin());
		}
		QMap<QString,int>::Iterator endByName()
		{
			return(Codes.end());
		}
	private:
		QMap<QString,int> Codes;
		QMap<int,QString> Names;
};

#endif
