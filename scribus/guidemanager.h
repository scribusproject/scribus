#ifndef GUIDEMANAGER_H
#define GUIDEMANAGER_H

#include <qdialog.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "mspinbox.h"
#include "page.h"

class GuideManager : public QDialog
{ 
    Q_OBJECT

public:
    GuideManager( QWidget* parent, Page* page, int Einh);
    ~GuideManager() {};
		void UnitChange();
		QString GetUnit();
		void UpdateHList();
		void UpdateVList();

    QGroupBox* VerGroup;
    QListBox* VerticalList;
    QLabel* TextLabel1;
    MSpinBox* VertSpin;
    QPushButton* VertSet;
    QPushButton* VertDel;
    QGroupBox* HorGroup;
    QListBox* HorList;
    QLabel* TextLabel2;
    MSpinBox* HoriSpin;
    QPushButton* HoriSet;
    QPushButton* HoriDel;
    QCheckBox* Lock;
    QPushButton* OK;
    QPushButton* Cancel;
		int Einheit;
		Page* pag;
		int curX;
		int curY;
		QValueList<float> XLocal;
		QValueList<float> YLocal;

private slots:
		void HandleLock();
		void GetHVal(int n);
		void GetVVal(int n);
		void SetHVal();
		void SetVVal();
		void DelHVal();
		void DelVVal();
		void valChange();
		void ExitOK();

protected:
    QVBoxLayout* GuideManagerLayout;
    QHBoxLayout* Layout6;
    QVBoxLayout* VerGroupLayout;
    QHBoxLayout* Layout2;
    QHBoxLayout* Layout1;
    QVBoxLayout* HorGroupLayout;
    QHBoxLayout* Layout4;
    QHBoxLayout* Layout3;
    QHBoxLayout* Layout5;
};

#endif // GUIDEMANAGER_H
