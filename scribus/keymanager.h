/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "scribusapi.h"
#include "scribusstructs.h"

#include <qmap.h>
class QWidget;
class QString;
class QKeyEvent;
class QPushButton;
class QGroupBox;
class QButtonGroup;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QTable;
class QHeader;
class QRadioButton;
class QLabel;
class QComboBox;
class QEvent;

class SCRIBUS_API KeyManager : public QWidget
{
	Q_OBJECT

public:
	KeyManager( QWidget* parent, QMap<QString, Keys> oldKeyMap);
	~KeyManager() {};

	const QMap<QString, Keys> getNewKeyMap();
	bool event( QEvent* ev );
	void keyPressEvent(QKeyEvent *k);
	void keyReleaseEvent(QKeyEvent *k);

	static QString getKeyText(int KeyC);

public slots:
	void setKeyText();
	void dispKey(int row);
	void setNoKey();

protected:
	QVBoxLayout* keyManagerLayout;
	QGridLayout* keyGroupLayout;
	QHBoxLayout* okCancelLayout;

	QTable* keyTable;
	QButtonGroup* keyGroup;
	QRadioButton* noKey;
	QRadioButton* userDef;
	QLabel* keyDisplay;
	QPushButton* setKeyButton;
	QHeader *header;
	QMap<QString,Keys> keyMap;
	QMap<QString,Keys>::Iterator currentKeyMapRow;
	int currRow;
	int keyCode;
	QString Part0;
	QString Part1;
	QString Part2;
	QString Part3;
	QString Part4;
	bool checkKey(int Code);
	QGroupBox* loadableGroupBox;
	QHBoxLayout* loadableGroupBoxLayout;
	QComboBox* loadableSets;
	QPushButton* loadSetButton;
	QPushButton* importSetButton;
	QPushButton* exportSetButton;
	QPushButton* resetSetButton;
	QMap<QString, QString> keySetList;

	void importKeySet(QString);
	bool exportKeySet(QString);
	QStringList scanForSets();
	
protected slots:
	void loadKeySetFile();
	void importKeySetFile();
	void exportKeySetFile();
	void resetKeySet();
};

#endif // KEYMANAGER_H
