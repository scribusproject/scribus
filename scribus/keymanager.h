#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "scribusapi.h"
#include "scribusstructs.h"

#include <qmap.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QKeyEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
class QWidget;
class QString;
class QKeyEvent;
class QPushButton;
class Q3GroupBox;
class Q3ButtonGroup;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class Q3Table;
class Q3Header;
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

public slots:
	void setKeyText();
	void dispKey(int row);
	void setNoKey();

protected:
	QVBoxLayout* keyManagerLayout;
	QGridLayout* keyGroupLayout;
	QHBoxLayout* okCancelLayout;

	Q3Table* keyTable;
	Q3ButtonGroup* keyGroup;
	QRadioButton* noKey;
	QRadioButton* userDef;
	QLabel* keyDisplay;
	QPushButton* setKeyButton;
	Q3Header *header;
	QMap<QString,Keys> keyMap;
	QMap<QString,Keys>::Iterator currentKeyMapRow;
	int currRow;
	int keyCode;
	QString Part1;
	QString Part2;
	QString Part3;
	QString Part4;
	QString getKeyText(int KeyC);
	bool checkKey(int Code);
	Q3GroupBox* loadableGroupBox;
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
