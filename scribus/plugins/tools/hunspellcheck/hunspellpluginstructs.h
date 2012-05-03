#ifndef HUNSPELLPLUGINSTRUCTS_H
#define HUNSPELLPLUGINSTRUCTS_H

#include <QString>
#include <QStringList>

struct WordsFound {
	int start;
	int end;
	QString w;
	QStringList replacements;
	bool changed;
	bool ignore;
	int changeOffset;
	QString lang;
};

#endif // HUNSPELLPLUGINSTRUCTS_H
