#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <qwidget.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qtable.h>
#include <qheader.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qevent.h>
#include "scribusstructs.h"

class KeyManager : public QWidget
{
	Q_OBJECT

public:
	KeyManager( QWidget* parent, QMap<int, Keys> oldKeyMap);
	~KeyManager() {};

	const QMap<int, Keys> getNewKeyMap();
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

	QTable* keyTable;
	QButtonGroup* keyGroup;
	QRadioButton* noKey;
	QRadioButton* userDef;
	QLabel* keyDisplay;
	QPushButton* setKeyButton;
	QHeader *header;
	QMap<int,Keys> keyMap;
	int actRow;
	int keyCode;
	QString Part1;
	QString Part2;
	QString Part3;
	QString Part4;
	QString getKeyText(int KeyC);
	bool checkKey(int Code);
};

#endif // KEYMANAGER_H
