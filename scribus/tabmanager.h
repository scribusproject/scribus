/****************************************************************************
** Form interface generated from reading ui file 'tabmanager.ui'
**
** Created: Don Jan 1 12:57:07 2004
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <qvariant.h>
#include <qdialog.h>
#include <qvaluelist.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;
class QLabel;
class QListBox;
class QListBoxItem;
class QPushButton;
class MSpinBox;

class TabManager : public QDialog
{
    Q_OBJECT

public:
    TabManager( QWidget* parent, int dEin, QValueList<double> inTab);
    ~TabManager() {};
    void UnitChange(int dEin);
	void UpdateTabL();

    QListBox* TabList;
    QLabel* Text1;
    QLabel* Text2;
    QComboBox* Align;
    MSpinBox* Position;
    QPushButton* OKButton;
    QPushButton* CancelButton;
    QPushButton* DelButton;
    QPushButton* AddButton;
    QValueList<double> tmpTab;
	int selTab;
	int Einheit;

public slots:
	void DelTab();
	void AddTab();
	void ChangeType();
	void selIte(QListBoxItem *c);
	void PValChange();

protected:
    QVBoxLayout* TabManagerLayout;
    QHBoxLayout* layout9;
    QVBoxLayout* layout8;
    QGridLayout* layout7;
    QGridLayout* layout10;
};

#endif // TABMANAGER_H
